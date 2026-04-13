// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandxdgshell_p.h"
#include "qwaylandxdgshellintegration_p.h"
#include "qwaylandxdgdecorationv1_p.h"

#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>

#include <qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandXdgShellIntegration::QWaylandXdgShellIntegration() : QWaylandShellIntegrationTemplate(6)
{
    connect(this, &QWaylandShellIntegrationTemplate::activeChanged, this, [this] {
        if (isActive()) {
            mXdgShell.reset(new QWaylandXdgShell(mDisplay, this));
        } else {
            mXdgShell.reset(nullptr);
            destroy();
        }
    });
}

QWaylandXdgShellIntegration::~QWaylandXdgShellIntegration()
{
    if (isActive())
        destroy();
}

bool QWaylandXdgShellIntegration::initialize(QWaylandDisplay *display)
{
    mDisplay = display;
    return QWaylandShellIntegrationTemplate::initialize(display);
}

void QWaylandXdgShellIntegration::xdg_wm_base_ping(uint32_t serial)
{
    pong(serial);
}

QWaylandShellSurface *QWaylandXdgShellIntegration::createShellSurface(QWaylandWindow *window)
{
    QWaylandDisplay *display = window->display();
    BobUI::WindowType type =  static_cast<BobUI::WindowType>(int(window->windowFlags() & BobUI::WindowType_Mask));
    auto *transientParent = window->transientParent();

    if (type == BobUI::ToolTip && !transientParent) {
        qCWarning(lcQpaWayland) << "Failed to create popup. Ensure popup " << window->window() << "has a transientParent set.";
        QWindowSystemInterface::handleCloseEvent<QWindowSystemInterface::AsynchronousDelivery>(window->window());
        return new QWaylandShellSurface(window);
    }

    if (type == BobUI::Popup && (!transientParent || !display->lastInputDevice())) {
        qCWarning(lcQpaWayland) << "Failed to create grabbing popup. Ensure popup " << window->window() << "has a transientParent set and that parent window has received input.";
        QWindowSystemInterface::handleCloseEvent<QWindowSystemInterface::AsynchronousDelivery>(window->window());
        return new QWaylandShellSurface(window);
    }

    return new QWaylandXdgSurface(mXdgShell.get(), get_xdg_surface(window->wlSurface()), window);
}

void *QWaylandXdgShellIntegration::nativeResourceForWindow(const QByteArray &resource, QWindow *window)
{
    if (auto waylandWindow = static_cast<QWaylandWindow *>(window->handle())) {
        if (auto xdgSurface = qobject_cast<QWaylandXdgSurface *>(waylandWindow->shellSurface())) {
            return xdgSurface->nativeResource(resource);
        }
    }
    return nullptr;
}

}

BOBUI_END_NAMESPACE
