// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandclientbufferintegrationfactory_p.h"
#include "qwaylandclientbufferintegrationplugin_p.h"
#include "qwaylandclientbufferintegration_p.h"
#include <BobUICore/private/qfactoryloader_p.h>
#include <BobUICore/QCoreApplication>
#include <BobUICore/QDir>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, qwcbifLoader,
    (QWaylandClientBufferIntegrationFactoryInterface_iid, QLatin1String("/wayland-graphics-integration-client"), BobUI::CaseInsensitive))

QStringList QWaylandClientBufferIntegrationFactory::keys()
{
    return qwcbifLoader->keyMap().values();
}

QWaylandClientBufferIntegration *QWaylandClientBufferIntegrationFactory::create(const QString &name, const QStringList &args)
{
    return qLoadPlugin<QWaylandClientBufferIntegration, QWaylandClientBufferIntegrationPlugin>(qwcbifLoader(), name, args);
}

}

BOBUI_END_NAMESPACE
