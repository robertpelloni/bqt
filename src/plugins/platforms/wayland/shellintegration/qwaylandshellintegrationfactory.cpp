// Copyright (C) 2016 Jolla Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandshellintegrationfactory_p.h"
#include "qwaylandshellintegrationplugin_p.h"
#include "qwaylandshellintegration_p.h"
#include <BobUICore/private/qfactoryloader_p.h>
#include <BobUICore/QCoreApplication>
#include <BobUICore/QDir>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, qwsifLoader,
    (QWaylandShellIntegrationFactoryInterface_iid, QLatin1String("/wayland-shell-integration"), BobUI::CaseInsensitive))

QStringList QWaylandShellIntegrationFactory::keys()
{
    return qwsifLoader->keyMap().values();
}

QWaylandShellIntegration *QWaylandShellIntegrationFactory::create(const QString &name, QWaylandDisplay *display, const QStringList &args)
{
    std::unique_ptr<QWaylandShellIntegration> integration;
    integration.reset(qLoadPlugin<QWaylandShellIntegration, QWaylandShellIntegrationPlugin>(qwsifLoader(), name, args));

    if (integration && !integration->initialize(display))
        return nullptr;

    return integration.release();
}

}

BOBUI_END_NAMESPACE
