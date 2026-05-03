// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDHARDWAREINTEGRATION_H
#define QWAYLANDHARDWAREINTEGRATION_H

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

#include <BobUIWaylandClient/private/qwayland-hardware-integration.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;

class Q_WAYLANDCLIENT_EXPORT QWaylandHardwareIntegration : public BobUIWayland::bobui_hardware_integration
{
public:
    QWaylandHardwareIntegration(struct ::wl_registry *registry, int id);

    QString clientBufferIntegration();
    QString serverBufferIntegration();

protected:
    void hardware_integration_client_backend(const QString &name) override;
    void hardware_integration_server_backend(const QString &name) override;

private:
    QString m_client_buffer;
    QString m_server_buffer;
};

}

BOBUI_END_NAMESPACE

#endif
