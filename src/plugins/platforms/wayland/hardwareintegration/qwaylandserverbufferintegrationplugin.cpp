// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandserverbufferintegrationplugin_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandServerBufferIntegrationPlugin::QWaylandServerBufferIntegrationPlugin(QObject *parent)
    : QObject(parent)
{
}
QWaylandServerBufferIntegrationPlugin::~QWaylandServerBufferIntegrationPlugin()
{
}

}

BOBUI_END_NAMESPACE

#include "moc_qwaylandserverbufferintegrationplugin_p.cpp"
