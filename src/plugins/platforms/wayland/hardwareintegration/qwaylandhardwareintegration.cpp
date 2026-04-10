// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandhardwareintegration_p.h"

#include "qwaylanddisplay_p.h"
BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandHardwareIntegration::QWaylandHardwareIntegration(struct ::wl_registry *registry, int id)
    : bobui_hardware_integration(registry, id, 1)
{
}

QString QWaylandHardwareIntegration::clientBufferIntegration()
{
    return m_client_buffer;
}

QString QWaylandHardwareIntegration::serverBufferIntegration()
{
    return m_server_buffer;
}

void QWaylandHardwareIntegration::hardware_integration_client_backend(const QString &name)
{
    m_client_buffer = name;
}

void QWaylandHardwareIntegration::hardware_integration_server_backend(const QString &name)
{
    m_server_buffer = name;
}

}

BOBUI_END_NAMESPACE
