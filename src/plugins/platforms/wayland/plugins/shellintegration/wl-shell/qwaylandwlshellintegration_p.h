// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDWLSHELLINTEGRATION_P_H
#define QWAYLANDWLSHELLINTEGRATION_P_H

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

#include <private/qwayland-wayland.h>

#include <BobUIWaylandClient/private/qwaylandshellintegration_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class Q_WAYLANDCLIENT_EXPORT QWaylandWlShellIntegration
    : public QWaylandShellIntegrationTemplate<QWaylandWlShellIntegration>,
      public BobUIWayland::wl_shell
{
public:
    QWaylandWlShellIntegration();
    ~QWaylandWlShellIntegration();
    QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;
    void *nativeResourceForWindow(const QByteArray &resource, QWindow *window) override;

private:
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDWLSHELLINTEGRATION_P_H
