// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandappmenu_p.h"
#include "qwaylandplatformservices_p.h"
#include "qwaylandwindow_p.h"
#include "qwaylanddisplay_p.h"
#include "qwaylandshellsurface_p.h"
#include "qwaylandwindowmanagerintegration_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandPlatformServices::QWaylandPlatformServices(QWaylandDisplay *display)
    : m_display(display) { }

QWaylandPlatformServices::~QWaylandPlatformServices()
{
    qDeleteAll(m_appMenus);
}

bool QWaylandPlatformServices::openUrl(const QUrl &url)
{
    if (auto windowManagerIntegration = m_display->windowManagerIntegration()) {
        windowManagerIntegration->openUrl(url);
        return true;
    }
    return QDesktopUnixServices::openUrl(url);
}

bool QWaylandPlatformServices::openDocument(const QUrl &url)
{
    if (auto windowManagerIntegration = m_display->windowManagerIntegration()) {
        windowManagerIntegration->openUrl(url);
        return true;
    }
    return QDesktopUnixServices::openDocument(url);
}

QString QWaylandPlatformServices::portalWindowIdentifier(QWindow *window)
{
    if (window && window->handle()) {
        auto shellSurface = static_cast<QWaylandWindow *>(window->handle())->shellSurface();
        if (shellSurface) {
            const QString handle = shellSurface->externWindowHandle();
            return QLatin1String("wayland:") + handle;
        }
    }
    return QString();
}

void QWaylandPlatformServices::registerDBusMenuForWindow(QWindow *window, const QString &service,
                                                         const QString &path)
{
    if (!m_display->appMenuManager())
        return;
    if (!window)
        return;
    if (!window->handle())
        window->create();
    auto waylandWindow = static_cast<QWaylandWindow *>(window->handle());
    auto menu = *m_appMenus.insert(window, new QWaylandAppMenu);

    auto createAppMenu = [waylandWindow, menu, service, path] {
        menu->init(waylandWindow->display()->appMenuManager()->create(waylandWindow->wlSurface()));
        menu->set_address(service, path);
    };

    if (waylandWindow->wlSurface())
        createAppMenu();

    QObject::connect(waylandWindow, &QWaylandWindow::wlSurfaceCreated, menu, createAppMenu);
    QObject::connect(waylandWindow, &QWaylandWindow::wlSurfaceDestroyed, menu,
                     [menu] { menu->release(); });
}

void QWaylandPlatformServices::unregisterDBusMenuForWindow(QWindow *window)
{
    delete m_appMenus.take(window);
}
} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
