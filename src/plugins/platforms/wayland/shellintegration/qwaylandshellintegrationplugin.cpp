// Copyright (C) 2016 Jolla Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandshellintegrationplugin_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandShellIntegrationPlugin::QWaylandShellIntegrationPlugin(QObject *parent)
                              : QObject(parent)
{
}

QWaylandShellIntegrationPlugin::~QWaylandShellIntegrationPlugin()
{
}

}

BOBUI_END_NAMESPACE

#include "moc_qwaylandshellintegrationplugin_p.cpp"
