// Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIWaylandClient/private/qwaylandscreen_p.h>

#include "qwaylandfullscreenshellv1surface.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandFullScreenShellV1Surface::QWaylandFullScreenShellV1Surface(BobUIWayland::zwp_fullscreen_shell_v1 *shell, QWaylandWindow *window)
    : QWaylandShellSurface(window)
    , m_shell(shell)
    , m_window(window)
{
    auto *screen = m_window->waylandScreen();
    auto *output = screen ? screen->output() : nullptr;
    m_shell->present_surface(m_window->wlSurface(),
                             BobUIWayland::zwp_fullscreen_shell_v1::present_method_default,
                             output);
}

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
