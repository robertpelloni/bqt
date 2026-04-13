// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qfbwindow_p.h"
#include "qfbscreen_p.h"

#include <BobUIGui/QScreen>
#include <qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

Q_CONSTINIT static QBasicAtomicInt winIdGenerator = Q_BASIC_ATOMIC_INITIALIZER(0);

QFbWindow::QFbWindow(QWindow *window)
    : QPlatformWindow(window), mBackingStore(0), mWindowState(BobUI::WindowNoState)
{
    mWindowId = winIdGenerator.fetchAndAddRelaxed(1) + 1;
}

QFbWindow::~QFbWindow()
{
}

QFbScreen *QFbWindow::platformScreen() const
{
    return static_cast<QFbScreen *>(window()->screen()->handle());
}

void QFbWindow::setGeometry(const QRect &rect)
{
    // store previous geometry for screen update
    mOldGeometry = geometry();

    QWindowSystemInterface::handleGeometryChange(window(), rect);

    QPlatformWindow::setGeometry(rect);

    if (mOldGeometry != rect)
        QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), geometry().size()));
}

void QFbWindow::setVisible(bool visible)
{
    QRect newGeom;
    QFbScreen *fbScreen = platformScreen();
    if (visible) {
        bool convOk = false;
        static bool envDisableForceFullScreen = qEnvironmentVariableIntValue("BOBUI_QPA_FB_FORCE_FULLSCREEN", &convOk) == 0 && convOk;
        const bool platformDisableForceFullScreen = fbScreen->flags().testFlag(QFbScreen::DontForceFirstWindowToFullScreen);
        const bool forceFullScreen = !envDisableForceFullScreen && !platformDisableForceFullScreen && fbScreen->windowCount() == 0;
        if (forceFullScreen || (mWindowState & BobUI::WindowFullScreen))
            newGeom = platformScreen()->geometry();
        else if (mWindowState & BobUI::WindowMaximized)
            newGeom = platformScreen()->availableGeometry();
    }
    QPlatformWindow::setVisible(visible);

    if (visible)
        fbScreen->addWindow(this);
    else
        fbScreen->removeWindow(this);

    if (!newGeom.isEmpty())
        setGeometry(newGeom); // may or may not generate an expose

    if (newGeom.isEmpty() || newGeom == mOldGeometry) {
        // QWindow::isExposed() maps to QWindow::visible() by default so simply
        // generating an expose event regardless of this being a show or hide is
        // just what is needed here.
        QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), geometry().size()));
    }
}

void QFbWindow::setWindowState(BobUI::WindowStates state)
{
    QPlatformWindow::setWindowState(state);
    mWindowState = state;
}

void QFbWindow::setWindowFlags(BobUI::WindowFlags flags)
{
    mWindowFlags = flags;
}

BobUI::WindowFlags QFbWindow::windowFlags() const
{
    return mWindowFlags;
}

void QFbWindow::raise()
{
    platformScreen()->raise(this);
    QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), geometry().size()));
}

void QFbWindow::lower()
{
    platformScreen()->lower(this);
    QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), geometry().size()));
}

void QFbWindow::repaint(const QRegion &region)
{
    const QRect currentGeometry = geometry();
    const QRect oldGeometryLocal = mOldGeometry;
    mOldGeometry = currentGeometry;
    // If this is a move, redraw the previous location
    if (oldGeometryLocal != currentGeometry)
        platformScreen()->setDirty(oldGeometryLocal);
    auto topLeft = currentGeometry.topLeft();
    for (auto rect : region)
        platformScreen()->setDirty(rect.translated(topLeft));
}

BOBUI_END_NAMESPACE
