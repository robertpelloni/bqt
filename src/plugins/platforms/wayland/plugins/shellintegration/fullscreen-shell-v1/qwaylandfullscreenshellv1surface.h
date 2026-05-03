// Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwaylandshellsurface_p.h>
#include <BobUIWaylandClient/private/qwaylandwindow_p.h>

#include "qwayland-fullscreen-shell-unstable-v1.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class Q_WAYLANDCLIENT_EXPORT QWaylandFullScreenShellV1Surface : public QWaylandShellSurface
{
public:
    QWaylandFullScreenShellV1Surface(BobUIWayland::zwp_fullscreen_shell_v1 *shell, QWaylandWindow *window);
    std::any surfaceRole() const override { return m_shell->object(); }

private:
    BobUIWayland::zwp_fullscreen_shell_v1 *m_shell = nullptr;
    QWaylandWindow *m_window = nullptr;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
