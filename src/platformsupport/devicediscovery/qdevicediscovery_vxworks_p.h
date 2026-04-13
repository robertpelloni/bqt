// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDEVICEDISCOVERY_STATIC_H
#define QDEVICEDISCOVERY_STATIC_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdevicediscovery_p.h"

BOBUI_BEGIN_NAMESPACE

class QDeviceDiscoveryVxWorks : public QDeviceDiscovery
{
    Q_OBJECT

public:
    QDeviceDiscoveryVxWorks(QDeviceTypes types, QObject *parent = nullptr);
    QStringList scanConnectedDevices() override;

private:
    bool checkDeviceType(const QString &device);
};

BOBUI_END_NAMESPACE

#endif // QDEVICEDISCOVERY_STATIC_H
