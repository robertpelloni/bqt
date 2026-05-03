// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandwlshellsurface_p.h"

#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include <BobUIWaylandClient/private/qwaylandinputdevice_p.h>
#include <BobUIWaylandClient/private/qwaylandabstractdecoration_p.h>
#include <BobUIWaylandClient/private/qwaylandscreen_p.h>

#include <BobUICore/QDebug>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandWlShellSurface::QWaylandWlShellSurface(struct ::wl_shell_surface *shell_surface, QWaylandWindow *window)
    : QWaylandShellSurface(window)
    , BobUIWayland::wl_shell_surface(shell_surface)
    , m_window(window)
{
    BobUI::WindowType type = window->window()->type();
    auto *transientParent = window->transientParent();
    if (type == BobUI::Popup && transientParent && transientParent->wlSurface())
        setPopup(transientParent, m_window->display()->lastInputDevice(), m_window->display()->lastInputSerial());
    else if (transientParent && transientParent->wlSurface())
        updateTransientParent(transientParent->window());
    else
        setTopLevel();
}

QWaylandWlShellSurface::~QWaylandWlShellSurface()
{
    wl_shell_surface_destroy(object());
}

bool QWaylandWlShellSurface::resize(QWaylandInputDevice *inputDevice, BobUI::Edges edges)
{
    enum resize resizeEdges = convertToResizeEdges(edges);
    resize(inputDevice->wl_seat(), inputDevice->serial(), resizeEdges);
    return true;
}

bool QWaylandWlShellSurface::move(QWaylandInputDevice *inputDevice)
{
    move(inputDevice->wl_seat(),
         inputDevice->serial());
    return true;
}

void QWaylandWlShellSurface::setTitle(const QString & title)
{
    return BobUIWayland::wl_shell_surface::set_title(title);
}

void QWaylandWlShellSurface::setAppId(const QString & appId)
{
    return BobUIWayland::wl_shell_surface::set_class(appId);
}

void QWaylandWlShellSurface::applyConfigure()
{
    if ((m_pending.states & (BobUI::WindowMaximized|BobUI::WindowFullScreen))
            && !(m_applied.states & (BobUI::WindowMaximized|BobUI::WindowFullScreen))) {
        m_normalSize = m_window->windowFrameGeometry().size();
    }

    if (m_pending.states != m_applied.states)
        m_window->handleWindowStatesChanged(m_pending.states);

    if (!m_pending.size.isEmpty()) {
        int x = 0;
        int y = 0;
        if (m_pending.edges & resize_left)
            x = m_applied.size.width() - m_pending.size.width();
        if (m_pending.edges & resize_top)
            y = m_applied.size.height() - m_pending.size.height();
        QPoint offset(x, y);
        m_window->resizeFromApplyConfigure(m_pending.size, offset);
    } else if (m_pending.size.isValid() && !m_normalSize.isEmpty()) {
        m_window->resizeFromApplyConfigure(m_normalSize);
    }

    m_applied = m_pending;
}

bool QWaylandWlShellSurface::wantsDecorations() const
{
    return !(m_pending.states & BobUI::WindowFullScreen);
}

void QWaylandWlShellSurface::requestWindowStates(BobUI::WindowStates states)
{
    // On wl-shell the client is in charge of states, so diff from the pending state
    BobUI::WindowStates changedStates = m_pending.states ^ states;
    BobUI::WindowStates addedStates = changedStates & states;

    if (addedStates & BobUI::WindowMinimized)
        qCWarning(lcQpaWayland) << "Minimizing is not supported on wl-shell. Consider using xdg-shell instead.";

    if (addedStates & BobUI::WindowMaximized) {
        set_maximized(nullptr);
        m_window->applyConfigureWhenPossible();
    }

    if (addedStates & BobUI::WindowFullScreen) {
        set_fullscreen(WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT, 0, nullptr);
        m_window->applyConfigureWhenPossible();
    }

    bool isNormal = !(states & BobUI::WindowMaximized) && !(states & BobUI::WindowFullScreen);
    if (isNormal && (changedStates & (BobUI::WindowMaximized | BobUI::WindowFullScreen))) {
        setTopLevel(); // set normal window
        // There's usually no configure event after this, so just clear the rest of the pending
        // configure here and queue the applyConfigure call
        m_pending.size = {0, 0};
        m_pending.edges = resize_none;
        m_window->applyConfigureWhenPossible();
    }

    m_pending.states = states & ~BobUI::WindowMinimized;
}

enum QWaylandWlShellSurface::resize QWaylandWlShellSurface::convertToResizeEdges(BobUI::Edges edges)
{
    return static_cast<enum resize>(
                ((edges & BobUI::TopEdge) ? resize_top : 0)
                | ((edges & BobUI::BottomEdge) ? resize_bottom : 0)
                | ((edges & BobUI::LeftEdge) ? resize_left : 0)
                | ((edges & BobUI::RightEdge) ? resize_right : 0));
}

void QWaylandWlShellSurface::setTopLevel()
{
    set_toplevel();
}

static inline bool testShowWithoutActivating(const QWindow *window)
{
    // QWidget-attribute BobUI::WA_ShowWithoutActivating.
    const QVariant showWithoutActivating = window->property("_q_showWithoutActivating");
    return showWithoutActivating.isValid() && showWithoutActivating.toBool();
}

void QWaylandWlShellSurface::updateTransientParent(QWindow *parent)
{
    QWaylandWindow *parent_wayland_window = static_cast<QWaylandWindow *>(parent->handle());
    if (!parent_wayland_window)
        return;

    // set_transient expects a position relative to the parent
    QPoint transientPos = m_window->geometry().topLeft(); // this is absolute
    transientPos -= parent->geometry().topLeft();
    if (parent_wayland_window->decoration()) {
        transientPos.setX(transientPos.x() + parent_wayland_window->decoration()->margins().left());
        transientPos.setY(transientPos.y() + parent_wayland_window->decoration()->margins().top());
    }

    uint32_t flags = 0;
    BobUI::WindowFlags wf = m_window->window()->flags();
    if (wf.testFlag(BobUI::ToolTip)
            || wf.testFlag(BobUI::WindowTransparentForInput)
            || testShowWithoutActivating(m_window->window()))
        flags |= WL_SHELL_SURFACE_TRANSIENT_INACTIVE;

    auto *parentSurface = parent_wayland_window->wlSurface();
    Q_ASSERT(parentSurface);
    set_transient(parentSurface, transientPos.x(), transientPos.y(), flags);
}

void QWaylandWlShellSurface::setPopup(QWaylandWindow *parent, QWaylandInputDevice *device, uint serial)
{
    QWaylandWindow *parent_wayland_window = parent;
    if (!parent_wayland_window) {
        qCWarning(lcQpaWayland) << "setPopup called without a parent window";
        return;
    }
    if (!device) {
        qCWarning(lcQpaWayland) << "setPopup called without an input device";
        return;
    }

    // set_popup expects a position relative to the parent
    QPoint transientPos = m_window->geometry().topLeft(); // this is absolute
    transientPos -= parent_wayland_window->geometry().topLeft();
    if (parent_wayland_window->decoration()) {
        transientPos.setX(transientPos.x() + parent_wayland_window->decoration()->margins().left());
        transientPos.setY(transientPos.y() + parent_wayland_window->decoration()->margins().top());
    }

    auto *parentSurface = parent_wayland_window->wlSurface();
    Q_ASSERT(parentSurface);
    uint flags = 0;
    set_popup(device->wl_seat(), serial, parentSurface, transientPos.x(), transientPos.y(), flags);
}

void QWaylandWlShellSurface::shell_surface_ping(uint32_t serial)
{
    pong(serial);
}

void QWaylandWlShellSurface::shell_surface_configure(uint32_t edges, int32_t width, int32_t height)
{
    m_pending.size = QSize(width, height);
    m_pending.edges = static_cast<enum resize>(edges);
    if (m_pending.edges && !m_pending.size.isEmpty())
        m_normalSize = m_pending.size;
    m_window->applyConfigureWhenPossible();
}

void QWaylandWlShellSurface::shell_surface_popup_done()
{
    QCoreApplication::postEvent(m_window->window(), new QCloseEvent());
}

}

BOBUI_END_NAMESPACE

#include "moc_qwaylandwlshellsurface_p.cpp"
