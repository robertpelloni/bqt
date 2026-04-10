// Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandfullscreenshellv1integration.h"
#include "qwaylandfullscreenshellv1surface.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandFullScreenShellV1Integration::QWaylandFullScreenShellV1Integration()
    : QWaylandShellIntegrationTemplate(1)
{
}

QWaylandFullScreenShellV1Integration::~QWaylandFullScreenShellV1Integration()
{
    if (isActive())
        release();
}

QWaylandShellSurface *QWaylandFullScreenShellV1Integration::createShellSurface(QWaylandWindow *window)
{
    return new QWaylandFullScreenShellV1Surface(this, window);
}

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
