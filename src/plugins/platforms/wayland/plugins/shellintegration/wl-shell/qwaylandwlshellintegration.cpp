// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandwlshellintegration_p.h"
#include "qwaylandwlshellsurface_p.h"

#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandWlShellIntegration::QWaylandWlShellIntegration() : QWaylandShellIntegrationTemplate(1)
{
    qCWarning(lcQpaWayland) << "\"wl-shell\" is a deprecated shell extension, prefer using"
                            << "\"xdg-shell\" if supported by the compositor"
                            << "by setting the environment variable BOBUI_WAYLAND_SHELL_INTEGRATION";
}

QWaylandWlShellIntegration::~QWaylandWlShellIntegration()
{
    if (object())
        wl_shell_destroy(object());
}

QWaylandShellSurface *QWaylandWlShellIntegration::createShellSurface(QWaylandWindow *window)
{
    return new QWaylandWlShellSurface(get_shell_surface(window->wlSurface()), window);
}

void *QWaylandWlShellIntegration::nativeResourceForWindow(const QByteArray &resource, QWindow *window)
{
    QByteArray lowerCaseResource = resource.toLower();
    if (lowerCaseResource == "wl_shell_surface") {
        if (auto waylandWindow = static_cast<QWaylandWindow *>(window->handle())) {
            if (auto shellSurface = qobject_cast<QWaylandWlShellSurface *>(waylandWindow->shellSurface())) {
                return shellSurface->object();
            }
        }
    }
    return nullptr;
}

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
