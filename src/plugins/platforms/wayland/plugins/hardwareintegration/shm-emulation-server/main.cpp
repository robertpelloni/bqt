// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIWaylandClient/private/qwaylandserverbufferintegrationplugin_p.h>
#include "shmserverbufferintegration.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class ShmServerBufferPlugin : public QWaylandServerBufferIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandServerBufferIntegrationFactoryInterface_iid FILE "shm-emulation-server.json")
public:
    QWaylandServerBufferIntegration *create(const QString&, const QStringList&) override;
};

QWaylandServerBufferIntegration *ShmServerBufferPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    Q_UNUSED(system);
    return new ShmServerBufferIntegration();
}

}

BOBUI_END_NAMESPACE

#include "main.moc"
