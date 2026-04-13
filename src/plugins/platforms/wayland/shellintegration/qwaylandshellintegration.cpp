// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include "qwaylandshellintegration_p.h"
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/private/qwaylandwindow_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandShellIntegration::~QWaylandShellIntegration()
    = default; // MUST stay empty until BobUI 7 (was inline in BobUI < 6.9)

wl_surface *QWaylandShellIntegration::wlSurfaceForWindow(QWaylandWindow *window)
{
    return window->wlSurface();
}

}

BOBUI_END_NAMESPACE
