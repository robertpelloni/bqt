// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylanddatadevicemanager_p.h"

#include "qwaylandinputdevice_p.h"
#include "qwaylanddatadevice_p.h"
#include "qwaylanddataoffer_p.h"
#include "qwaylanddisplay_p.h"

#include <BobUICore/QDebug>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandDataDeviceManager::QWaylandDataDeviceManager(QWaylandDisplay *display, int version, uint32_t id)
    : wl_data_device_manager(display->wl_registry(), id, qMin(version, 3))
    , m_display(display)
{
    // Create transfer devices for all input devices.
    // ### This only works if we get the global before all devices and is surely wrong when hotplugging.
    QList<QWaylandInputDevice *> inputDevices = m_display->inputDevices();
    for (int i = 0; i < inputDevices.size();i++) {
        inputDevices.at(i)->setDataDevice(getDataDevice(inputDevices.at(i)));
    }
}

QWaylandDataDeviceManager::~QWaylandDataDeviceManager()
{
    wl_data_device_manager_destroy(object());
}

QWaylandDataDevice *QWaylandDataDeviceManager::getDataDevice(QWaylandInputDevice *inputDevice)
{
    return new QWaylandDataDevice(this, inputDevice);
}

QWaylandDisplay *QWaylandDataDeviceManager::display() const
{
    return m_display;
}

}

BOBUI_END_NAMESPACE
