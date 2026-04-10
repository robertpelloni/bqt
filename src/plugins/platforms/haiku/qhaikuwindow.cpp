// Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Tobias Koenig <tobias.koenig@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qhaikuwindow.h"

#include "private/qguiapplication_p.h"

#include <QCoreApplication>
#include <BOBUIhread>
#include <QWindow>
#include <qpa/qwindowsysteminterface.h>

#include <Rect.h>
#include <Window.h>

BOBUI_BEGIN_NAMESPACE

enum {
    DefaultWindowWidth = 160,
    DefaultWindowHeight = 160
};

HaikuWindowProxy::HaikuWindowProxy(QWindow *window, const QRect &rect, QObject *parent)
    : QObject(parent)
    , BWindow(BRect(rect.x(), rect.y(), rect.right() - 1, rect.bottom() - 1),
              window->title().toUtf8(), B_NO_BORDER_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
    , m_bobuiCalledZoom(false)
    , m_zoomInProgress(false)
{
}

void HaikuWindowProxy::FrameMoved(BPoint pos)
{
    Q_EMIT moved(QPoint(pos.x, pos.y));
}

void HaikuWindowProxy::FrameResized(float width, float height)
{
    Q_EMIT resized(QSize(static_cast<int>(width), static_cast<int>(height)), m_zoomInProgress);

    m_zoomInProgress = false;
}

void HaikuWindowProxy::WindowActivated(bool activated)
{
    Q_EMIT windowActivated(activated);
}

void HaikuWindowProxy::Minimize(bool minimize)
{
    BWindow::Minimize(minimize);

    Q_EMIT minimized(minimize);
}

void HaikuWindowProxy::Zoom(BPoint pos, float width, float height)
{
    m_zoomInProgress = true;
    BWindow::Zoom(pos, width, height);

    // Only notify about Zoom invocations from the Haiku windowing system
    if (!m_bobuiCalledZoom)
        Q_EMIT zoomed();
}

bool HaikuWindowProxy::QuitRequested()
{
    Q_EMIT quitRequested();

    // Return false to prevent Haiku windowing system to clean up
    // the BWindow and BView instances. We will do that ourself when
    // BobUI invokes the dtor of QHaikuWindow
    return false;
}

void HaikuWindowProxy::zoomByBobUI()
{
    m_bobuiCalledZoom = true;
    BWindow::Zoom();
    m_bobuiCalledZoom = false;
}

QHaikuWindow::QHaikuWindow(QWindow *window)
    : QPlatformWindow(window)
    , m_window(nullptr)
    , m_windowState(BobUI::WindowNoState)
{
    const QRect rect = initialGeometry(window, window->geometry(), DefaultWindowWidth, DefaultWindowHeight);

    HaikuWindowProxy *haikuWindow = new HaikuWindowProxy(window, rect, nullptr);
    connect(haikuWindow, SIGNAL(moved(QPoint)), SLOT(haikuWindowMoved(QPoint)));
    connect(haikuWindow, SIGNAL(resized(QSize,bool)), SLOT(haikuWindowResized(QSize,bool)));
    connect(haikuWindow, SIGNAL(windowActivated(bool)), SLOT(haikuWindowActivated(bool)));
    connect(haikuWindow, SIGNAL(minimized(bool)), SLOT(haikuWindowMinimized(bool)));
    connect(haikuWindow, SIGNAL(zoomed()), SLOT(haikuWindowZoomed()));
    connect(haikuWindow, SIGNAL(quitRequested()), SLOT(haikuWindowQuitRequested()), BobUI::BlockingQueuedConnection);

    m_window = haikuWindow;

    if (Q_UNLIKELY(!m_window))
        qFatal("QHaikuWindow: failed to create window");

    setGeometry(rect);
    setWindowFlags(window->flags());
}

QHaikuWindow::~QHaikuWindow()
{
    m_window->LockLooper();
    m_window->Quit();

    m_window = nullptr;
}

void QHaikuWindow::setGeometry(const QRect &rect)
{
    QPlatformWindow::setGeometry(rect);

    m_window->MoveTo(rect.x(), rect.y());
    m_window->ResizeTo(rect.width(), rect.height());
}

QMargins QHaikuWindow::frameMargins() const
{
    const BRect decoratorFrame = m_window->DecoratorFrame();
    const BRect frame = m_window->Frame();

    return QMargins(frame.left - decoratorFrame.left,
                    frame.top - decoratorFrame.top,
                    decoratorFrame.right - frame.right,
                    decoratorFrame.bottom - frame.bottom);
}

void QHaikuWindow::setVisible(bool visible)
{
    if (visible) {
        m_window->Show();

        window()->requestActivate();

        QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), window()->geometry().size()));
    } else {
        m_window->Hide();
    }
}

bool QHaikuWindow::isExposed() const
{
    return !m_window->IsHidden();
}

bool QHaikuWindow::isActive() const
{
    return m_window->IsActive();
}

WId QHaikuWindow::winId() const
{
    return (WId)static_cast<BWindow*>(m_window);
}

BWindow* QHaikuWindow::nativeHandle() const
{
    return m_window;
}

void QHaikuWindow::requestActivateWindow()
{
    m_window->Activate(true);
}

void QHaikuWindow::setWindowState(BobUI::WindowStates state)
{
    if (m_windowState == state)
        return;

    const BobUI::WindowStates oldState = m_windowState;

    m_windowState = state;

    if (m_windowState & BobUI::WindowMinimized)
        m_window->Minimize(true);
    else if (m_windowState & BobUI::WindowMaximized)
        m_window->zoomByBobUI();
    else if (oldState & BobUI::WindowMinimized)
        m_window->Minimize(false); // undo minimize
    else if (oldState & BobUI::WindowMaximized)
        m_window->zoomByBobUI(); // undo zoom
}

void QHaikuWindow::setWindowFlags(BobUI::WindowFlags flags)
{
    const BobUI::WindowType type = static_cast<BobUI::WindowType>(static_cast<int>(flags & BobUI::WindowType_Mask));

    const bool isPopup = (type == BobUI::Popup);
    const bool isSplashScreen = (type == BobUI::SplashScreen);
    const bool isDialog = ((type == BobUI::Dialog) || (type == BobUI::Sheet) || (type == BobUI::MSWindowsFixedSizeDialogHint));
    const bool isTool = ((type == BobUI::Tool) || (type == BobUI::Drawer));
    const bool isToolTip = (type == BobUI::ToolTip);

    window_look wlook = B_TITLED_WINDOW_LOOK;
    window_feel wfeel = B_NORMAL_WINDOW_FEEL;
    uint32 wflag = (B_NO_WORKSPACE_ACTIVATION | B_NOT_ANCHORED_ON_ACTIVATE);

    if (isTool) {
        wlook = B_FLOATING_WINDOW_LOOK;
        wflag |= B_WILL_ACCEPT_FIRST_CLICK;
    }

    if (isSplashScreen) {
        wlook = B_NO_BORDER_WINDOW_LOOK;
    }

    if (isPopup) {
        wlook = B_NO_BORDER_WINDOW_LOOK;
        wflag |= (B_WILL_ACCEPT_FIRST_CLICK | B_AVOID_FRONT | B_AVOID_FOCUS);
        flags |= BobUI::WindowStaysOnTopHint;
    }

    if (isDialog) {
        if (window()->modality() == BobUI::WindowModal)
            wfeel = B_MODAL_SUBSET_WINDOW_FEEL;
        else if (window()->modality() == BobUI::ApplicationModal)
            wfeel = B_MODAL_APP_WINDOW_FEEL;
    }

    if (isToolTip) {
        wlook = B_NO_BORDER_WINDOW_LOOK;
        wflag |= (B_WILL_ACCEPT_FIRST_CLICK | B_AVOID_FOCUS);
        flags |= BobUI::WindowStaysOnTopHint;
    }

    if (flags & BobUI::FramelessWindowHint)
        wlook = B_NO_BORDER_WINDOW_LOOK;

    if (flags & BobUI::MSWindowsFixedSizeDialogHint)
        wflag |= (B_NOT_RESIZABLE | B_NOT_ZOOMABLE);

    if (flags & BobUI::CustomizeWindowHint) {
        if (!(flags & BobUI::WindowMinimizeButtonHint))
            wflag |= B_NOT_MINIMIZABLE;
        if (!(flags & BobUI::WindowMaximizeButtonHint))
            wflag |= B_NOT_ZOOMABLE;
        if (!(flags & BobUI::WindowCloseButtonHint))
            wflag |= B_NOT_CLOSABLE;
    }

    if (flags & BobUI::WindowStaysOnTopHint)
        wfeel = B_FLOATING_ALL_WINDOW_FEEL;

    m_window->SetLook(wlook);
    m_window->SetFeel(wfeel);
    m_window->SetFlags(wflag);
}

void QHaikuWindow::setWindowTitle(const QString &title)
{
    m_window->SetTitle(title.toLocal8Bit().constData());
}

void QHaikuWindow::propagateSizeHints()
{
    m_window->SetSizeLimits(window()->minimumSize().width(),
                            window()->maximumSize().width(),
                            window()->minimumSize().height(),
                            window()->maximumSize().height());

    m_window->SetZoomLimits(window()->maximumSize().width(),
                            window()->maximumSize().height());
}

void QHaikuWindow::haikuWindowMoved(const QPoint &pos)
{
    const QRect newGeometry(pos, geometry().size());

    QWindowSystemInterface::handleGeometryChange(window(), newGeometry);
    QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), newGeometry.size()));
}

void QHaikuWindow::haikuWindowResized(const QSize &size, bool zoomInProgress)
{
    const QRect newGeometry(geometry().topLeft(), size);

    QWindowSystemInterface::handleGeometryChange(window(), newGeometry);
    QWindowSystemInterface::handleExposeEvent(window(), QRect(QPoint(0, 0), newGeometry.size()));

    if ((m_windowState == BobUI::WindowMaximized) && !zoomInProgress) {
        // the user has resized the window while maximized -> reset maximized flag
        m_windowState = BobUI::WindowNoState;

        QWindowSystemInterface::handleWindowStateChanged(window(), m_windowState);
    }
}

void QHaikuWindow::haikuWindowActivated(bool activated)
{
    QWindowSystemInterface::handleFocusWindowChanged(activated ? window() : nullptr);
}

void QHaikuWindow::haikuWindowMinimized(bool minimize)
{
    m_windowState = (minimize ? BobUI::WindowMinimized : BobUI::WindowNoState);

    QWindowSystemInterface::handleWindowStateChanged(window(), m_windowState);
}

void QHaikuWindow::haikuWindowZoomed()
{
    m_windowState = (m_windowState == BobUI::WindowMaximized ? BobUI::WindowNoState : BobUI::WindowMaximized);

    QWindowSystemInterface::handleWindowStateChanged(window(), m_windowState);
}

void QHaikuWindow::haikuWindowQuitRequested()
{
    QWindowSystemInterface::handleCloseEvent(window());
}

void QHaikuWindow::haikuMouseEvent(const QPoint &localPosition, const QPoint &globalPosition, BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers, BobUI::MouseEventSource source)
{
    QWindowSystemInterface::handleMouseEvent(window(), localPosition, globalPosition,
                                             buttons, modifiers, source);
}

void QHaikuWindow::haikuWheelEvent(const QPoint &localPosition, const QPoint &globalPosition, int delta, BobUI::Orientation orientation, BobUI::KeyboardModifiers modifiers)
{
    QWindowSystemInterface::handleWheelEvent(window(), localPosition, globalPosition, delta, orientation, modifiers);
}

void QHaikuWindow::haikuKeyEvent(QEvent::Type type, int key, BobUI::KeyboardModifiers modifiers, const QString &text)
{
    QWindowSystemInterface::handleKeyEvent(window(), type, key, modifiers, text);
}

void QHaikuWindow::haikuEnteredView()
{
    QWindowSystemInterface::handleEnterEvent(window());
}

void QHaikuWindow::haikuExitedView()
{
    QWindowSystemInterface::handleLeaveEvent(window());
}

void QHaikuWindow::haikuDrawRequest(const QRect &rect)
{
    QWindowSystemInterface::handleExposeEvent(window(), QRegion(rect));
}

BOBUI_END_NAMESPACE
