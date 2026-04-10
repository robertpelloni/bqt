// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "private/qeglfsdeviceintegration_p.h"
#include "qeglfsbrcmintegration.h"

BOBUI_BEGIN_NAMESPACE

class QEglFSBrcmIntegrationPlugin : public QEglFSDeviceIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QEglFSDeviceIntegrationFactoryInterface_iid FILE "eglfs_brcm.json")

public:
    QEglFSDeviceIntegration *create() override { return new QEglFSBrcmIntegration; }
};

BOBUI_END_NAMESPACE

#include "qeglfsbrcmmain.moc"
