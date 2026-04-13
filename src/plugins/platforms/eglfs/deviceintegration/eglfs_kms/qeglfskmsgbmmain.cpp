// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "private/qeglfsdeviceintegration_p.h"
#include "qeglfskmsgbmintegration_p.h"

BOBUI_BEGIN_NAMESPACE

class QEglFSKmsGbmIntegrationPlugin : public QEglFSDeviceIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QEglFSDeviceIntegrationFactoryInterface_iid FILE "eglfs_kms.json")

public:
    QEglFSDeviceIntegration *create() override { return new QEglFSKmsGbmIntegration; }
};

BOBUI_END_NAMESPACE

#include "qeglfskmsgbmmain.moc"
