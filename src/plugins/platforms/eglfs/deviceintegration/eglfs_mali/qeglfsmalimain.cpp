// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "private/qeglfsdeviceintegration_p.h"
#include "qeglfsmaliintegration.h"

BOBUI_BEGIN_NAMESPACE

class QEglFSMaliIntegrationPlugin : public QEglFSDeviceIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QEglFSDeviceIntegrationFactoryInterface_iid FILE "eglfs_mali.json")

public:
    QEglFSDeviceIntegration *create() override { return new QEglFSMaliIntegration; }
};

BOBUI_END_NAMESPACE

#include "qeglfsmalimain.moc"
