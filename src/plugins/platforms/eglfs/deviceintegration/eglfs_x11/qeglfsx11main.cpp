// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "private/qeglfsdeviceintegration_p.h"
#include "qeglfsx11integration.h"

BOBUI_BEGIN_NAMESPACE

class QEglFSX11IntegrationPlugin : public QEglFSDeviceIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QEglFSDeviceIntegrationFactoryInterface_iid FILE "eglfs_x11.json")

public:
    QEglFSDeviceIntegration *create() override { return new QEglFSX11Integration; }
};

BOBUI_END_NAMESPACE

#include "qeglfsx11main.moc"
