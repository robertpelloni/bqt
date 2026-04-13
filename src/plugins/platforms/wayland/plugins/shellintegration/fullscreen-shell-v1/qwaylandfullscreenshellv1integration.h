// Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <wayland-client-core.h>
#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/qwaylandshellintegration_p.h>

#include "qwayland-fullscreen-shell-unstable-v1.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class Q_WAYLANDCLIENT_EXPORT QWaylandFullScreenShellV1Integration
    : public QWaylandShellIntegrationTemplate<QWaylandFullScreenShellV1Integration>,
      public BobUIWayland::zwp_fullscreen_shell_v1
{
public:
    QWaylandFullScreenShellV1Integration();
    ~QWaylandFullScreenShellV1Integration() override;
    QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
