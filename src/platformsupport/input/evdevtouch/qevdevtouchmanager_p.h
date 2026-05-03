// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEVDEVTOUCHMANAGER_P_H
#define QEVDEVTOUCHMANAGER_P_H

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

#include <BobUIInputSupport/private/devicehandlerlist_p.h>

#include <QObject>
#include <QHash>
#include <QSocketNotifier>

BOBUI_BEGIN_NAMESPACE

class QDeviceDiscovery;
class QEvdevTouchScreenHandlerThread;

class QEvdevTouchManager : public QObject
{
public:
    QEvdevTouchManager(const QString &key, const QString &spec, QObject *parent = nullptr);
    ~QEvdevTouchManager();

    void addDevice(const QString &deviceNode);
    void removeDevice(const QString &deviceNode);

    void updateInputDeviceCount();

private:
    QString m_spec;
    BobUIInputSupport::DeviceHandlerList<QEvdevTouchScreenHandlerThread> m_activeDevices;
};

BOBUI_END_NAMESPACE

#endif // QEVDEVTOUCHMANAGER_P_H
