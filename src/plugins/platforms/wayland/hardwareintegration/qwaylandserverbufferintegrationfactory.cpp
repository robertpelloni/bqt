// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandserverbufferintegrationfactory_p.h"
#include "qwaylandserverbufferintegrationplugin_p.h"

#include <BobUICore/private/qfactoryloader_p.h>
#include <BobUICore/QCoreApplication>
#include <BobUICore/QDir>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, qwsbifLoader,
    (QWaylandServerBufferIntegrationFactoryInterface_iid, QLatin1String("/wayland-graphics-integration-client"), BobUI::CaseInsensitive))

QStringList QWaylandServerBufferIntegrationFactory::keys()
{
    return qwsbifLoader->keyMap().values();
}

QWaylandServerBufferIntegration *QWaylandServerBufferIntegrationFactory::create(const QString &name, const QStringList &args)
{
    return qLoadPlugin<QWaylandServerBufferIntegration, QWaylandServerBufferIntegrationPlugin>(qwsbifLoader(), name, args);
}

}

BOBUI_END_NAMESPACE
