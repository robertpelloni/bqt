// Copyright (C) 2016 LG Electronics Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandinputdeviceintegrationplugin_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandInputDeviceIntegrationPlugin::QWaylandInputDeviceIntegrationPlugin(QObject *parent)
                              : QObject(parent)
{
}

QWaylandInputDeviceIntegrationPlugin::~QWaylandInputDeviceIntegrationPlugin()
{
}

}

BOBUI_END_NAMESPACE

#include "moc_qwaylandinputdeviceintegrationplugin_p.cpp"
