// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qoffscreenwindow.h"
#include "qoffscreencommon.h"

#include <qpa/qplatformscreen.h>
#include <qpa/qwindowsysteminterface.h>

#include <private/qwindow_p.h>
#include <private/qguiapplication_p.h>

BOBUI_BEGIN_NAMESPACE

QOffscreenWindow::QOffscreenWindow(QWindow *window, bool frameMarginsEnabled)
    : QPlatformWindow(window)
    , m_positionIncludesFrame(false)
    , m_visible(false)
    , m_pendingGeometryChangeOnShow(true)
    , m_frameMarginsRequested(frameMarginsEnabled)
{
    if (window->windowState() == BobUI::WindowNoState) {
        setGeometry(windowGeometry());
    } else {
        setWindowState(window->windowStates());
    }

    static WId counter = 0;
    m_winId = ++counter;

    m_windowForWinIdHash[m_winId] = this;
}

QOffscreenWindow::~QOffscreenWindow()
{
    if (QOffscreenScreen::windowContainingCursor == this)
        QOffscreenScreen::windowContainingCursor = nullptr;
    m_windowForWinIdHash.remove(m_winId);
}

void QOffscreenWindow::setGeometry(const QRect &rect)
{
    if (window()->windowState() != BobUI::WindowNoState)
        return;

    m_positionIncludesFrame = bobui_window_private(window())->positionPolicy == QWindowPrivate::WindowFrameInclusive;

    setFrameMarginsEnabled(m_frameMarginsRequested);
    setGeometryImpl(rect);

    m_normalGeometry = geometry();
}

void QOffscreenWindow::setGeometryImpl(const QRect &rect)
{
    QRect adjusted = rect;
    if (adjusted.width() <= 0)
        adjusted.setWidth(1);
    if (adjusted.height() <= 0)
        adjusted.setHeight(1);

    if (m_positionIncludesFrame) {
        adjusted.translate(m_margins.left(), m_margins.top());
    } else {
        // make sure we're not placed off-screen
        if (adjusted.left() < m_margins.left())
            adjusted.translate(m_margins.left(), 0);
        if (adjusted.top() < m_margins.top())
            adjusted.translate(0, m_margins.top());
    }

    QPlatformWindow::setGeometry(adjusted);

    if (m_visible) {
        QWindowSystemInterface::handleGeometryChange(window(), adjusted);
        QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(), adjusted.size()));
    } else {
        m_pendingGeometryChangeOnShow = true;
    }
}

void QOffscreenWindow::setVisible(bool visible)
{
    if (visible == m_visible)
        return;

    if (visible) {
        if (window()->type() != BobUI::ToolTip)
            QWindowSystemInterface::handleFocusWindowChanged(window(), BobUI::ActiveWindowFocusReason);

        if (m_pendingGeometryChangeOnShow) {
            m_pendingGeometryChangeOnShow = false;
            QWindowSystemInterface::handleGeometryChange(window(), geometry());
        }
    }

    const QPoint cursorPos = QCursor::pos();
    if (visible) {
        QRect rect(QPoint(), geometry().size());
        QWindowSystemInterface::handleExposeEvent(window(), rect);
        if (QWindowPrivate::get(window())->isPopup() && QGuiApplicationPrivate::currentMouseWindow) {
            QWindowSystemInterface::handleLeaveEvent<QWindowSystemInterface::SynchronousDelivery>
                (QGuiApplicationPrivate::currentMouseWindow);
        }
        if (geometry().contains(cursorPos))
            QWindowSystemInterface::handleEnterEvent(window(),
                                                     window()->mapFromGlobal(cursorPos), cursorPos);
    } else {
        QWindowSystemInterface::handleExposeEvent(window(), QRegion());
        if (window()->type() & BobUI::Window) {
            if (QWindow *windowUnderMouse = QGuiApplication::topLevelAt(cursorPos)) {
                QWindowSystemInterface::handleEnterEvent(windowUnderMouse,
                                                        windowUnderMouse->mapFromGlobal(cursorPos),
                                                        cursorPos);
            }
        }
    }

    m_visible = visible;
}

void QOffscreenWindow::requestActivateWindow()
{
    if (m_visible)
        QWindowSystemInterface::handleFocusWindowChanged(window(), BobUI::ActiveWindowFocusReason);
}

WId QOffscreenWindow::winId() const
{
    return m_winId;
}

QSurfaceFormat QOffscreenWindow::format() const
{
    return window()->requestedFormat();
}

QMargins QOffscreenWindow::frameMargins() const
{
    return m_margins;
}

void QOffscreenWindow::setFrameMarginsEnabled(bool enabled)
{
    if (enabled
        && !(window()->flags() & BobUI::FramelessWindowHint)
        && (parent() == nullptr)) {
        m_margins = QMargins(2, 2, 2, 2);
    } else {
        m_margins = QMargins(0, 0, 0, 0);
    }
}

void QOffscreenWindow::setWindowState(BobUI::WindowStates state)
{
    setFrameMarginsEnabled(m_frameMarginsRequested && !(state & BobUI::WindowFullScreen));
    m_positionIncludesFrame = false;

    if (state & BobUI::WindowMinimized)
        ; // nothing to do
    else if (state & BobUI::WindowFullScreen)
        setGeometryImpl(screen()->geometry());
    else if (state & BobUI::WindowMaximized)
        setGeometryImpl(screen()->availableGeometry().adjusted(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom()));
    else
        setGeometryImpl(m_normalGeometry);

    QWindowSystemInterface::handleWindowStateChanged(window(), state);
}

QOffscreenWindow *QOffscreenWindow::windowForWinId(WId id)
{
    return m_windowForWinIdHash.value(id, nullptr);
}

Q_CONSTINIT QHash<WId, QOffscreenWindow *> QOffscreenWindow::m_windowForWinIdHash;

BOBUI_END_NAMESPACE
