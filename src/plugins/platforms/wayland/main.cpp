// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qpa/qplatformintegrationplugin.h>
#include <BobUIWaylandClient/private/qwaylandintegration_p.h>
#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "qwayland.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration *QWaylandIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList)
#if !BOBUI_CONFIG(wayland_egl)
    if (system == "wayland-egl")
        return nullptr;
#endif
#if !BOBUI_CONFIG(wayland_brcm)
    if (system == "wayland-brcm")
        return nullptr;
#endif
    auto *integration = new QWaylandIntegration(system);

    if (!integration->init()) {
        delete integration;
        integration = nullptr;
    }

    return integration;
}

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE

#include "main.moc"
