// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qplatforminputcontextplugin_p.h"

BOBUI_BEGIN_NAMESPACE

QPlatformInputContextPlugin::QPlatformInputContextPlugin(QObject *parent)
    : QObject(parent)
{
}

QPlatformInputContextPlugin::~QPlatformInputContextPlugin()
{
}

BOBUI_END_NAMESPACE

#include "moc_qplatforminputcontextplugin_p.cpp"
