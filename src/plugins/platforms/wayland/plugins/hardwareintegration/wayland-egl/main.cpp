// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIWaylandClient/private/qwaylandclientbufferintegrationplugin_p.h>
#include "qwaylandeglclientbufferintegration_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandEglClientBufferPlugin : public QWaylandClientBufferIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandClientBufferIntegrationFactoryInterface_iid FILE "wayland-egl.json")
public:
    QWaylandClientBufferIntegration *create(const QString&, const QStringList&) override;
};

QWaylandClientBufferIntegration *QWaylandEglClientBufferPlugin::create(const QString&, const QStringList&)
{
    return new QWaylandEglClientBufferIntegration();
}

}

BOBUI_END_NAMESPACE

#include "main.moc"
