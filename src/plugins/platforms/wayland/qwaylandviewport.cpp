// Copyright (C) 2022 David Edmundson <davidedmundson@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandviewport_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandViewport::QWaylandViewport(::wp_viewport *viewport)
    : BobUIWayland::wp_viewport(viewport)
{
}

QWaylandViewport::~QWaylandViewport()
{
    destroy();
}

void QWaylandViewport::setSource(const QRectF &source)
{
    set_source(wl_fixed_from_double(source.x()),
               wl_fixed_from_double(source.y()),
               wl_fixed_from_double(source.width()),
               wl_fixed_from_double(source.height()));
}

void QWaylandViewport::setDestination(const QSize &destination)
{
    set_destination(destination.width(), destination.height());
}

}

BOBUI_END_NAMESPACE
