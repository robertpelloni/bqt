// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIWaylandClient/private/qwaylandserverbufferintegrationplugin_p.h>
#include "dmabufserverbufferintegration.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class DmaBufServerBufferPlugin : public QWaylandServerBufferIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandServerBufferIntegrationFactoryInterface_iid FILE "dmabuf-server.json")
public:
    QWaylandServerBufferIntegration *create(const QString&, const QStringList&) override;
};

QWaylandServerBufferIntegration *DmaBufServerBufferPlugin::create(const QString&, const QStringList&)
{
    return new DmaBufServerBufferIntegration();
}

}

BOBUI_END_NAMESPACE

#include "main.moc"
