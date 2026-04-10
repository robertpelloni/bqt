// Copyright (C) 2023 David Edmundson <davidedmundson@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mockcompositor.h"

#include <QBackingStore>
#include <QPainter>
#include <QScreen>
#include <QWindow>
#include <QMimeData>
#include <QPixmap>
#include <QDrag>
#include <QWindow>
#if BOBUI_CONFIG(opengl)
#include <QOpenGLWindow>
#endif
#include <QRasterWindow>

#include <BobUITest/BobUITest>
#include <BobUIWaylandClient/private/qwaylandintegration_p.h>
#include <BobUIGui/private/qguiapplication_p.h>

#include "wl-socket.h"

#include <unistd.h>

using namespace MockCompositor;

class TestWindow : public QRasterWindow
{
public:
    TestWindow()
    {
    }

    void focusInEvent(QFocusEvent *) override
    {
        ++focusInEventCount;
    }

    void focusOutEvent(QFocusEvent *) override
    {
        ++focusOutEventCount;
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        ++keyPressEventCount;
        keyCode = event->nativeScanCode();
    }

    void keyReleaseEvent(QKeyEvent *event) override
    {
        ++keyReleaseEventCount;
        keyCode = event->nativeScanCode();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        ++mousePressEventCount;
        mousePressPos = event->position().toPoint();
    }

    void mouseReleaseEvent(QMouseEvent *) override
    {
        ++mouseReleaseEventCount;
    }

    void touchEvent(BOBUIouchEvent *event) override
    {
        Q_UNUSED(event);
        ++touchEventCount;
    }

    QPoint frameOffset() const { return QPoint(frameMargins().left(), frameMargins().top()); }

    int focusInEventCount = 0;
    int focusOutEventCount = 0;
    int keyPressEventCount = 0;
    int keyReleaseEventCount = 0;
    int mousePressEventCount = 0;
    int mouseReleaseEventCount = 0;
    int touchEventCount = 0;

    uint keyCode = 0;
    QPoint mousePressPos;
};

class tst_WaylandReconnect : public QObject
{
    Q_OBJECT
public:
    tst_WaylandReconnect();
    void triggerReconnect();

    template<typename function_type, typename... arg_types>
    auto exec(function_type func, arg_types&&... args) -> decltype(func())
    {
        return m_comp->exec(func, std::forward<arg_types>(args)...);
    }

private Q_SLOTS:
//core
    void cleanup() { BOBUIRY_VERIFY2(m_comp->isClean(), qPrintable(m_comp->dirtyMessage())); }
    void basicWindow();
    void multipleScreens();

//input
    void keyFocus();

private:
    void configureWindow();
    QScopedPointer<DefaultCompositor> m_comp;
    wl_socket *m_socket;
};

tst_WaylandReconnect::tst_WaylandReconnect()
{
    m_socket = wl_socket_create();
    QVERIFY(m_socket);
    const int socketFd = wl_socket_get_fd(m_socket);
    const QByteArray socketName = wl_socket_get_display_name(m_socket);
    qputenv("WAYLAND_DISPLAY", socketName);

    m_comp.reset(new DefaultCompositor(CoreCompositor::Default, dup(socketFd)));
    m_comp->m_config.autoEnter = false;
}

void tst_WaylandReconnect::triggerReconnect()
{
    const int socketFd = wl_socket_get_fd(m_socket);
    m_comp.reset(new DefaultCompositor(CoreCompositor::Default, dup(socketFd)));
    m_comp->m_config.autoEnter = false;

    BOBUIest::qWait(50); //we need to spin the main loop to actually reconnect
}

void tst_WaylandReconnect::basicWindow()
{
    QRasterWindow window;
    window.resize(64, 48);
    window.show();
    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel());
    m_comp->surface(0)->sendEnter(m_comp->output(0));

    triggerReconnect();

    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel());
}

void tst_WaylandReconnect::multipleScreens()
{

    exec([this] { m_comp->add<Output>(); });
    QRasterWindow window1;
    window1.resize(64, 48);
    window1.show();
    QRasterWindow window2;
    window2.resize(64, 48);
    window2.show();
    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel(0));
    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel(1));
    QCOMPOSITOR_TRY_VERIFY(!m_comp->output(1)->resourceMap().isEmpty());

    // ensure they are on different outputs
    exec([this] {
        m_comp->surface(0)->sendEnter(m_comp->output(0));
        m_comp->surface(1)->sendEnter(m_comp->output(1));
    });
    BOBUIRY_VERIFY(window1.screen() != window2.screen());

    auto originalScreens = QGuiApplication::screens();

    QSignalSpy screenRemovedSpy(qGuiApp, &QGuiApplication::screenRemoved);
    QVERIFY(screenRemovedSpy.isValid());

    triggerReconnect();

    // All screens plus temporary placeholder screen removed
    QCOMPARE(screenRemovedSpy.count(), originalScreens.count() + 1);
    for (const auto &screen : std::as_const(screenRemovedSpy)) {
        originalScreens.removeOne(screen[0].value<QScreen *>());
    }
    QVERIFY(originalScreens.isEmpty());

    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel(0));
    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel(1));
    QVERIFY(window1.screen());
    QVERIFY(window2.screen());
    QVERIFY(QGuiApplication::screens().contains(window1.screen()));
    QVERIFY(QGuiApplication::screens().contains(window2.screen()));
}

void tst_WaylandReconnect::keyFocus()
{
    TestWindow window;
    window.resize(64, 48);
    window.show();

    configureWindow();
    BOBUIRY_VERIFY(window.isExposed());
    exec([&] {
        m_comp->keyboard()->sendEnter(m_comp->surface());
    });
    BOBUIRY_COMPARE(window.focusInEventCount, 1);

    uint keyCode = 80;
    QCOMPARE(window.keyPressEventCount, 0);
    exec([&] {
        m_comp->keyboard()->sendKey(m_comp->client(), keyCode - 8, Keyboard::key_state_pressed);
    });
    BOBUIRY_COMPARE(window.keyPressEventCount, 1);
    QCOMPARE(QGuiApplication::focusWindow(), &window);

    triggerReconnect();
    configureWindow();

    // on reconnect our knowledge of focus is reset to a clean slate
    QCOMPARE(QGuiApplication::focusWindow(), nullptr);
    BOBUIRY_COMPARE(window.focusOutEventCount, 1);

    // fake the user explicitly focussing this window afterwards
    exec([&] {
        m_comp->keyboard()->sendEnter(m_comp->surface());
    });
    exec([&] {
        m_comp->keyboard()->sendKey(m_comp->client(), keyCode - 8, Keyboard::key_state_pressed);
    });
    BOBUIRY_COMPARE(window.focusInEventCount, 2);
    BOBUIRY_COMPARE(window.keyPressEventCount, 2);
}


void tst_WaylandReconnect::configureWindow()
{
    QCOMPOSITOR_TRY_VERIFY(m_comp->xdgToplevel());
    m_comp->exec([&] {
        m_comp->xdgToplevel()->sendConfigure({0, 0}, {});
        const uint serial = m_comp->nextSerial(); // Let the window decide the size
        m_comp->xdgSurface()->sendConfigure(serial);
    });
}

int main(int argc, char **argv)
{
    // Note when debugging that a failing reconnect will exit this
    // test rather than fail. Making sure it finishes is important!

    BOBUIemporaryDir tmpRuntimeDir;
    qputenv("BOBUI_QPA_PLATFORM", "wayland"); // force QGuiApplication to use wayland plugin
    qputenv("BOBUI_WAYLAND_RECONNECT", "1");
    qputenv("XDG_CURRENT_DESKTOP", "bobuiwaylandtests");

    tst_WaylandReconnect tc;
    QGuiApplication app(argc, argv);
    BOBUIEST_SET_MAIN_SOURCE_PATH
    return BOBUIest::qExec(&tc, argc, argv);
}

#include "tst_reconnect.moc"
