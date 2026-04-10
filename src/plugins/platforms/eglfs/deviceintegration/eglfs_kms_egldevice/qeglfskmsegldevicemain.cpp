// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qeglfskmsegldeviceintegration.h"

BOBUI_BEGIN_NAMESPACE

class QEglFSKmsEglDeviceIntegrationPlugin : public QEglFSDeviceIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QEglFSDeviceIntegrationFactoryInterface_iid FILE "eglfs_kms_egldevice.json")

public:
    QEglFSDeviceIntegration *create() override { return new QEglFSKmsEglDeviceIntegration; }
};

BOBUI_END_NAMESPACE

#include "qeglfskmsegldevicemain.moc"
