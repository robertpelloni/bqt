// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDEVICEDISCOVERY_H
#define QDEVICEDISCOVERY_H

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

#include <QObject>
#include <QSocketNotifier>
#include <QStringList>
#include <private/qglobal_p.h>

#define BOBUI_EVDEV_DEVICE_PATH "/dev/input/"
#define BOBUI_EVDEV_DEVICE_PREFIX "event"
#define BOBUI_EVDEV_DEVICE BOBUI_EVDEV_DEVICE_PATH BOBUI_EVDEV_DEVICE_PREFIX

#define BOBUI_DRM_DEVICE_PATH "/dev/dri/"
#define BOBUI_DRM_DEVICE_PREFIX "card"
#define BOBUI_DRM_DEVICE BOBUI_DRM_DEVICE_PATH BOBUI_DRM_DEVICE_PREFIX

BOBUI_BEGIN_NAMESPACE

class QDeviceDiscovery : public QObject
{
    Q_OBJECT

public:
    enum QDeviceType {
        Device_Unknown = 0x00,
        Device_Mouse = 0x01,
        Device_Touchpad = 0x02,
        Device_Touchscreen = 0x04,
        Device_Keyboard = 0x08,
        Device_DRM = 0x10,
        Device_DRM_PrimaryGPU = 0x20,
        Device_Tablet = 0x40,
        Device_Joystick = 0x80,
        Device_InputMask = Device_Mouse | Device_Touchpad | Device_Touchscreen | Device_Keyboard | Device_Tablet | Device_Joystick,
        Device_VideoMask = Device_DRM
    };
    Q_ENUM(QDeviceType)
    Q_DECLARE_FLAGS(QDeviceTypes, QDeviceType)

    static QDeviceDiscovery *create(QDeviceTypes type, QObject *parent = nullptr);

    virtual QStringList scanConnectedDevices() = 0;

signals:
    void deviceDetected(const QString &deviceNode);
    void deviceRemoved(const QString &deviceNode);
    void deviceChanged(const QString &deviceNode);

protected:
    QDeviceDiscovery(QDeviceTypes types, QObject *parent) : QObject(parent), m_types(types) { }
    Q_DISABLE_COPY_MOVE(QDeviceDiscovery)

    QDeviceTypes m_types;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDeviceDiscovery::QDeviceTypes)

BOBUI_END_NAMESPACE

#endif // QDEVICEDISCOVERY_H
