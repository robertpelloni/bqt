// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandclientbufferintegrationplugin_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandClientBufferIntegrationPlugin::QWaylandClientBufferIntegrationPlugin(QObject *parent) :
    QObject(parent)
{
}

QWaylandClientBufferIntegrationPlugin::~QWaylandClientBufferIntegrationPlugin()
{
}

}

BOBUI_END_NAMESPACE

#include "moc_qwaylandclientbufferintegrationplugin_p.cpp"
