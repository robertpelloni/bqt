// Copyright (C) 2016 LG Electronics Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDINPUTDEVICEINTEGRATION_H
#define QWAYLANDINPUTDEVICEINTEGRATION_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <stdint.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;
class QWaylandInputDevice;

class Q_WAYLANDCLIENT_EXPORT QWaylandInputDeviceIntegration
{
public:
    QWaylandInputDeviceIntegration() {}
    virtual ~QWaylandInputDeviceIntegration() {}

    virtual QWaylandInputDevice *createInputDevice(QWaylandDisplay *d, int version, uint32_t id) = 0;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDINPUTDEVICEINTEGRATION_H
