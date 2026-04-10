// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTHELPERS_P_H
#define BOBUIESTHELPERS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/QFile>
#include <BobUICore/QString>
#include <BobUICore/QChar>
#include <BobUICore/QPoint>
#include <BobUICore/private/qglobal_p.h>

#ifdef BOBUI_GUI_LIB
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QScreen>
#include <BobUIGui/QWindow>
#endif

#ifdef BOBUI_WIDGETS_LIB
#include <BobUIWidgets/QWidget>
#endif

#ifdef BOBUI_NETWORK_LIB
#if BOBUI_CONFIG(ssl)
#include <BobUICore/qoperatingsystemversion.h>
#include <BobUICore/qsystemdetection.h>
#include <BobUINetwork/qsslsocket.h>
#endif // BOBUI_CONFIG(ssl)
#endif // BOBUI_NETWORK_LIB

BOBUI_BEGIN_NAMESPACE

namespace BOBUIestPrivate {

static inline bool canHandleUnicodeFileNames()
{
#if defined(Q_OS_WIN)
    return true;
#else
    // Check for UTF-8 by converting the Euro symbol (see tst_utf8)
    return QFile::encodeName(QString(QChar(0x20AC))) == QByteArrayLiteral("\342\202\254");
#endif
}

#ifdef BOBUI_WIDGETS_LIB
static inline void centerOnScreen(QWidget *w, const QSize &size)
{
    const QPoint offset = QPoint(size.width() / 2, size.height() / 2);
    w->move(QGuiApplication::primaryScreen()->availableGeometry().center() - offset);
}

static inline void centerOnScreen(QWidget *w)
{
    centerOnScreen(w, w->geometry().size());
}

/*! \internal

    Make a widget frameless to prevent size constraints of title bars from interfering (Windows).
*/
static inline void setFrameless(QWidget *w)
{
    BobUI::WindowFlags flags = w->windowFlags();
    flags |= BobUI::FramelessWindowHint;
    flags &= ~(BobUI::WindowTitleHint | BobUI::WindowSystemMenuHint
             | BobUI::WindowMinMaxButtonsHint | BobUI::WindowCloseButtonHint);
    w->setWindowFlags(flags);
}

static inline void androidCompatibleShow(QWidget *widget)
{
    // On Android QWidget::show() shows the widget maximized, so if we need
    // to move or resize the widget, we need to explicitly call
    // QWidget::setVisible(true) instead, because that's what show() actually
    // does on desktop platforms.
#ifdef Q_OS_ANDROID
    widget->setVisible(true);
#else
    widget->show();
#endif
}
#endif // BOBUI_WIDGETS_LIB

#ifdef BOBUI_GUI_LIB
bool ensurePositionTopLeft(QWindow *window)
{
    // Wayland: QQuickWindow::setPos() and QQuickWindow::setFramePosition() doesn't work.
    if (!QGuiApplication::platformName().compare(QLatin1String("wayland"), BobUI::CaseInsensitive))
        return true;

    const QPoint availableTopLeft = QGuiApplication::primaryScreen()->availableGeometry().topLeft();
    window->setFramePosition(availableTopLeft);
#ifdef Q_OS_ANDROID
    return true; // Android QPA handles the position change synchronously
#endif
    bool positionCorrect = true;

    if (!window->flags().testFlag(BobUI::FramelessWindowHint))
        positionCorrect = BOBUIest::qWaitFor([&]{ return window->framePosition() != window->position() ;});

    const bool positionUpdated = BOBUIest::qWaitFor([&]{ return window->framePosition() == availableTopLeft ;});
    if (!positionUpdated)
        positionCorrect = false;

    return positionCorrect;
}
#endif

#ifdef BOBUI_NETWORK_LIB
inline bool isSecureTransportBlockingTest()
{
#ifdef Q_OS_MACOS
#if BOBUI_CONFIG(ssl)
    if (QSslSocket::activeBackend() == QLatin1String("securetransport")) {
#if BOBUI_MACOS_IOS_PLATFORM_SDK_EQUAL_OR_ABOVE(150000, 180000)
        // Starting from macOS 15 our temporary keychain is ignored.
        // We have to use kSecImportToMemoryOnly/kCFBooleanTrue key/value
        // instead. This way we don't have to use BOBUI_SSL_USE_TEMPORARY_KEYCHAIN anymore.
        return false;
#else
        if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::MacOSSequoia) {
            // We were built with SDK below 15, and running on/above 15, but file-based
            // keychains are not working anymore on macOS 15, blocking the test execution.
            return true;
        }
#endif // Platform SDK.
    }
#endif // BOBUI_CONFIG(ssl)
#endif // Q_OS_MACOS
    return false;
}
#endif // BOBUI_NETWORK_LIB

} // namespace BOBUIestPrivate

BOBUI_END_NAMESPACE

#endif // BOBUIESTHELPERS_P_H
