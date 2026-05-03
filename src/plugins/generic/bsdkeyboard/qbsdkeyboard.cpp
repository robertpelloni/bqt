// Copyright (C) 2015-2016 Oleksandr Tymoshenko <gonzo@bluezbox.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qbsdkeyboard.h"

#include <QByteArray>
#include <QFile>
#include <QGuiApplication>
#include <QPoint>
#include <QSocketNotifier>
#include <QString>
#include <QStringList>

#include <BobUICore/qglobal.h>
#include <qpa/qwindowsysteminterface.h>
#include <private/qcore_unix_p.h>
#include <private/qguiapplication_p.h>
#include <private/qinputdevicemanager_p_p.h>

#include <qdebug.h>
#include <cstdio>

#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

#include <termios.h>
#include <sys/kbio.h>

// #define BOBUI_BSD_KEYBOARD_DEBUG

#ifdef BOBUI_BSD_KEYBOARD_DEBUG
#include <qdebug.h>
#endif

BOBUI_BEGIN_NAMESPACE

enum {
    Bsd_KeyCodeMask     = 0x7f,
    Bsd_KeyPressedMask  = 0x80
};

#include "qbsdkeyboard_defaultmap.h"

QBsdKeyboardHandler::QBsdKeyboardHandler(const QString &key, const QString &specification)
{
    Q_UNUSED(key);

    setObjectName(QLatin1String("BSD Keyboard Handler"));

    QByteArray device;
    if (specification.startsWith("/dev/"))
        device = QFile::encodeName(specification);

    if (device.isEmpty()) {
        device = QByteArrayLiteral("STDIN");
        m_fd = fileno(stdin);
    }
    else {
        m_fd = BOBUI_OPEN(device.constData(), O_RDONLY);
        if (!m_fd) {
            qErrnoWarning(errno, "open(%s) failed", device.constData());
            return;
        }
        m_shouldClose = true;
    }

    if (ioctl(m_fd, KDGKBMODE, &m_origKbdMode)) {
        qErrnoWarning(errno, "ioctl(%s, KDGKBMODE) failed", device.constData());
        revertTTYSettings();
        return;
    }

    if (ioctl(m_fd, KDSKBMODE, K_CODE) < 0) {
        qErrnoWarning(errno, "ioctl(%s, KDSKBMODE) failed", device.constData());
        revertTTYSettings();
        return;
    }

    termios kbdtty;
    if (tcgetattr(m_fd, &kbdtty) == 0) {

        m_kbdOrigTty.reset(new termios);
        *m_kbdOrigTty = kbdtty;

        kbdtty.c_iflag = IGNPAR | IGNBRK;
        kbdtty.c_oflag = 0;
        kbdtty.c_cflag = CREAD | CS8;
        kbdtty.c_lflag = 0;
        kbdtty.c_cc[VTIME] = 0;
        kbdtty.c_cc[VMIN] = 0;
        cfsetispeed(&kbdtty, 9600);
        cfsetospeed(&kbdtty, 9600);
        if (tcsetattr(m_fd, TCSANOW, &kbdtty) < 0) {
            qErrnoWarning(errno, "tcsetattr(%s) failed", device.constData());

            // TTY is still at old settings so we can
            // dispose of original termios data
            m_kbdOrigTty.reset();

            revertTTYSettings();
            return;
        }
    } else {
        qErrnoWarning(errno, "tcgetattr(%s) failed", device.constData());
        revertTTYSettings();
        return;
    }

    if (fcntl(m_fd, F_SETFL, O_NONBLOCK)) {
        qErrnoWarning(errno, "fcntl(%s, F_SETFL, O_NONBLOCK) failed", device.constData());
        revertTTYSettings();
        return;
    }

    resetKeymap();

    m_notifier.reset(new QSocketNotifier(m_fd, QSocketNotifier::Read, this));
    connect(m_notifier.data(), &QSocketNotifier::activated, this, &QBsdKeyboardHandler::readKeyboardData);
    QInputDeviceManagerPrivate::get(QGuiApplicationPrivate::inputDeviceManager())->setDeviceCount(
        QInputDeviceManager::DeviceTypeKeyboard, 1);
}

QBsdKeyboardHandler::~QBsdKeyboardHandler()
{
    revertTTYSettings();
}

void QBsdKeyboardHandler::revertTTYSettings()
{
    if (m_fd >= 0) {
        if (m_kbdOrigTty) {
            tcsetattr(m_fd, TCSANOW, m_kbdOrigTty.data());
            m_kbdOrigTty.reset();
        }

        if (m_origKbdMode != Bsd_NoKeyMode) {
            ioctl(m_fd, KDSKBMODE, m_origKbdMode);
            m_origKbdMode = Bsd_NoKeyMode;
        }

        if (m_shouldClose)
            close(m_fd);
        m_fd = -1;
    }
}

void QBsdKeyboardHandler::readKeyboardData()
{

    for (;;) {
        uint8_t buffer[32];
        int bytesRead = bobui_safe_read(m_fd, buffer, sizeof(buffer));

        if (!bytesRead) {
            qWarning("Got EOF from the input device.");
            return;
        } else if (bytesRead < 0) {
            if (errno != EINTR && errno != EAGAIN)
                qWarning("Could not read from input device: %s", strerror(errno));
            return;
        }

        for (int i = 0; i < bytesRead; ++i) {
            const quint16 code = buffer[i] & Bsd_KeyCodeMask;
            const bool pressed = (buffer[i] & Bsd_KeyPressedMask) ? false : true;

            processKeycode(code, pressed, false);
        }
    }
}

void QBsdKeyboardHandler::processKeyEvent(int nativecode, int unicode, int bobuicode,
                                          BobUI::KeyboardModifiers modifiers, bool isPress,
                                          bool autoRepeat)
{
    const QString text = (unicode != 0xffff ) ? QString(unicode) : QString();
    const QEvent::Type eventType = isPress ? QEvent::KeyPress : QEvent::KeyRelease;

    QWindowSystemInterface::handleExtendedKeyEvent(0, eventType, bobuicode, modifiers, nativecode, 0,
                                                   int(modifiers), text, autoRepeat);
}

void QBsdKeyboardHandler::processKeycode(quint16 keycode, bool pressed, bool autorepeat)
{
    const bool first_press = pressed && !autorepeat;

    const QBsdKeyboardMap::Mapping *map_plain = nullptr;
    const QBsdKeyboardMap::Mapping *map_withmod = nullptr;

    quint8 modifiers = m_modifiers;

    // get a specific and plain mapping for the keycode and the current modifiers
    for (const QBsdKeyboardMap::Mapping &m : m_keymap) {
        if (m.keycode == keycode) {
            if (m.modifiers == 0)
                map_plain = &m;

            quint8 testmods = m_modifiers;
            if (m_capsLock && (m.flags & QBsdKeyboardMap::IsLetter))
                testmods ^= QBsdKeyboardMap::ModShift;
            if (m.modifiers == testmods)
                map_withmod = &m;
        }
    }

    if (m_capsLock && map_withmod && (map_withmod->flags & QBsdKeyboardMap::IsLetter))
        modifiers ^= QBsdKeyboardMap::ModShift;

#ifdef BOBUI_BSD_KEYBOARD_DEBUG
    qWarning("Processing key event: keycode=%3d, modifiers=%02x pressed=%d, autorepeat=%d", \
             keycode, modifiers, pressed ? 1 : 0, autorepeat ? 1 : 0);
#endif

    const QBsdKeyboardMap::Mapping *it = map_withmod ? map_withmod : map_plain;

    if (!it) {
#ifdef BOBUI_BSD_KEYBOARD_DEBUG
        // we couldn't even find a plain mapping
        qWarning("Could not find a suitable mapping for keycode: %3d, modifiers: %02x", keycode, modifiers);
#endif
        return;
    }

    bool skip = false;
    quint16 unicode = it->unicode;
    quint32 bobuicode = it->bobuicode;

    if ((it->flags & QBsdKeyboardMap::IsModifier) && it->special) {
        // this is a modifier, i.e. Shift, Alt, ...
        if (pressed)
            m_modifiers |= quint8(it->special);
        else
            m_modifiers &= ~quint8(it->special);
    } else if (bobuicode >= BobUI::Key_CapsLock && bobuicode <= BobUI::Key_ScrollLock) {
        // (Caps|Num|Scroll)Lock
        if (first_press) {
            switch (bobuicode) {
            case BobUI::Key_CapsLock:
                m_capsLock = !m_capsLock;
                switchLed(LED_CAP, m_capsLock);
                break;
            case BobUI::Key_NumLock:
                m_numLock = !m_numLock;
                switchLed(LED_NUM, m_numLock);
                break;
            case BobUI::Key_ScrollLock:
                m_scrollLock = !m_scrollLock;
                switchLed(LED_SCR, m_scrollLock);
                break;
            default:
                break;
            }
        }
    }

    if (!skip) {
        // a normal key was pressed
        const int modmask = BobUI::ShiftModifier | BobUI::ControlModifier | BobUI::AltModifier
            | BobUI::MetaModifier | BobUI::KeypadModifier;

        // we couldn't find a specific mapping for the current modifiers,
        // or that mapping didn't have special modifiers:
        // so just report the plain mapping with additional modifiers.
        if ((it == map_plain && it != map_withmod) ||
            (map_withmod && !(map_withmod->bobuicode & modmask))) {
            bobuicode |= QBsdKeyboardHandler::toBobUIModifiers(modifiers);
        }

#ifdef BOBUI_BSD_KEYBOARD_DEBUG
        qWarning("Processing: uni=%04x, bobui=%08x, bobuimod=%08x", unicode, bobuicode & ~modmask, (bobuicode & modmask));
#endif
        //If NumLockOff and keypad key pressed remap event sent
        if (!m_numLock &&
             (bobuicode & BobUI::KeypadModifier)) {
            unicode = 0xffff;
            const int oldMask = (bobuicode & modmask);
            switch (bobuicode & ~modmask) {
            case BobUI::Key_7: //7 --> Home
                bobuicode = BobUI::Key_Home;
                break;
            case BobUI::Key_8: //8 --> Up
                bobuicode = BobUI::Key_Up;
                break;
            case BobUI::Key_9: //9 --> PgUp
                bobuicode = BobUI::Key_PageUp;
                break;
            case BobUI::Key_4: //4 --> Left
                bobuicode = BobUI::Key_Left;
                break;
            case BobUI::Key_5: //5 --> Clear
                bobuicode = BobUI::Key_Clear;
                break;
            case BobUI::Key_6: //6 --> right
                bobuicode = BobUI::Key_Right;
                break;
            case BobUI::Key_1: //1 --> End
                bobuicode = BobUI::Key_End;
                break;
            case BobUI::Key_2: //2 --> Down
                bobuicode = BobUI::Key_Down;
                break;
            case BobUI::Key_3: //3 --> PgDn
                bobuicode = BobUI::Key_PageDown;
                break;
            case BobUI::Key_0: //0 --> Ins
                bobuicode = BobUI::Key_Insert;
                break;
            case BobUI::Key_Period: //. --> Del
                bobuicode = BobUI::Key_Delete;
                break;
            }
            bobuicode |= oldMask;
        }

        // send the result to the server
        processKeyEvent(keycode, unicode, bobuicode & ~modmask,
                        BobUI::KeyboardModifiers(bobuicode & modmask), pressed, autorepeat);
    }
}

void QBsdKeyboardHandler::switchLed(int led, bool state)
{
#ifdef BOBUI_BSD_KEYBOARD_DEBUG
    qWarning() << "switchLed" << led << state;
#endif
    int leds = 0;
    if (ioctl(m_fd, KDGETLED, &leds) < 0) {
        qWarning("switchLed: Failed to query led states.");
        return;
    }

    if (state)
        leds |= led;
    else
        leds &= ~led;

    if (ioctl(m_fd, KDSETLED, leds) < 0)
        qWarning("switchLed: Failed to set led states.");
}

void QBsdKeyboardHandler::resetKeymap()
{
#ifdef BOBUI_BSD_KEYBOARD_DEBUG
    qWarning() << "Unload current keymap and restore built-in";
#endif

    m_keymap.clear();

    const size_t mappingSize = sizeof(keymapDefault) / sizeof(keymapDefault[0]);
    m_keymap.resize(mappingSize);
    std::copy_n( &keymapDefault[0], mappingSize, m_keymap.begin() );

    // reset state, so we could switch keymaps at runtime
    m_modifiers = 0;
    m_capsLock = false;
    m_numLock = false;
    m_scrollLock = false;

    //Set locks according to keyboard leds
    int leds = 0;
    if (ioctl(m_fd, KDGETLED, &leds) < 0) {
        qWarning("Failed to query led states. Settings numlock & capslock off");
        switchLed(LED_NUM, false);
        switchLed(LED_CAP, false);
        switchLed(LED_SCR, false);
    } else {
        if ((leds & LED_CAP) > 0)
            m_capsLock = true;
        if ((leds & LED_NUM) > 0)
            m_numLock = true;
        if ((leds & LED_SCR) > 0)
            m_scrollLock = true;
#ifdef BOBUI_BSD_KEYBOARD_DEBUG
        qWarning("numlock=%d , capslock=%d, scrolllock=%d",m_numLock, m_capsLock, m_scrollLock);
#endif
    }
}
