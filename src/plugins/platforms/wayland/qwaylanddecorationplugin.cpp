// Copyright (C) 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylanddecorationplugin_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandDecorationPlugin::QWaylandDecorationPlugin(QObject *parent)
    : QObject(parent)
{
}
QWaylandDecorationPlugin::~QWaylandDecorationPlugin()
{
}

}

BOBUI_END_NAMESPACE

#include "moc_qwaylanddecorationplugin_p.cpp"
