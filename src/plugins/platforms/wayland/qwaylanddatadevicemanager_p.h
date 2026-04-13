// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDATADEVICEMANAGER_H
#define QWAYLANDDATADEVICEMANAGER_H

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

#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>
#include <BobUIWaylandClient/private/qwayland-wayland.h>

BOBUI_REQUIRE_CONFIG(wayland_datadevice);

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;
class QWaylandDataDevice;
class QWaylandDataSource;
class QWaylandInputDevice;

class Q_WAYLANDCLIENT_EXPORT QWaylandDataDeviceManager : public BobUIWayland::wl_data_device_manager
{
public:
    QWaylandDataDeviceManager(QWaylandDisplay *display, int version, uint32_t id);
    ~QWaylandDataDeviceManager() override;

    QWaylandDataDevice *getDataDevice(QWaylandInputDevice *inputDevice);

    QWaylandDisplay *display() const;

private:
    QWaylandDisplay *m_display = nullptr;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDATADEVICEMANAGER_H
