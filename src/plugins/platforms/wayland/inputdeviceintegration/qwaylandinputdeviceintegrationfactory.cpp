// Copyright (C) 2016 LG Electronics Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandinputdeviceintegrationfactory_p.h"
#include "qwaylandinputdeviceintegrationplugin_p.h"
#include "qwaylandinputdeviceintegration_p.h"
#include <BobUICore/private/qfactoryloader_p.h>
#include <BobUICore/QCoreApplication>
#include <BobUICore/QDir>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, qwidfiLoader,
    (QWaylandInputDeviceIntegrationFactoryInterface_iid, QLatin1String("/wayland-inputdevice-integration"), BobUI::CaseInsensitive))

QStringList QWaylandInputDeviceIntegrationFactory::keys()
{
    return qwidfiLoader->keyMap().values();
}

QWaylandInputDeviceIntegration *QWaylandInputDeviceIntegrationFactory::create(const QString &name, const QStringList &args)
{
    return qLoadPlugin<QWaylandInputDeviceIntegration, QWaylandInputDeviceIntegrationPlugin>(qwidfiLoader(), name, args);
}

}

BOBUI_END_NAMESPACE
