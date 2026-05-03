// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdevicediscovery_dummy_p.h"

BOBUI_BEGIN_NAMESPACE

QDeviceDiscovery *QDeviceDiscovery::create(QDeviceTypes types, QObject *parent)
{
    return new QDeviceDiscoveryDummy(types, parent);
}

QDeviceDiscoveryDummy::QDeviceDiscoveryDummy(QDeviceTypes types, QObject *parent)
    : QDeviceDiscovery(types, parent)
{
}

QStringList QDeviceDiscoveryDummy::scanConnectedDevices()
{
    return QStringList();
}

BOBUI_END_NAMESPACE
