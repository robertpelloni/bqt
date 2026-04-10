// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qxcbscrollingdevice_p.h"

BOBUI_BEGIN_NAMESPACE

QXcbScrollingDevicePrivate::QXcbScrollingDevicePrivate(const QString &name, qint64 id, QInputDevice::Capabilities caps,
                                   int buttonCount, const QString &seatName)
    : QPointingDevicePrivate(name, id, QInputDevice::DeviceType::Mouse, QPointingDevice::PointerType::Generic,
                             caps, 1, buttonCount, seatName)
{
}

BOBUI_END_NAMESPACE

#include "moc_qxcbscrollingdevice_p.cpp"
