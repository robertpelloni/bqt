// Copyright (C) 2022 David Reondo <kde@david-redondo.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandxdgexporterv2_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandXdgExportedV2::QWaylandXdgExportedV2(::zxdg_exported_v2 *object)
    : BobUIWayland::zxdg_exported_v2(object)
{
}

QWaylandXdgExportedV2::~QWaylandXdgExportedV2()
{
    destroy();
}

void QWaylandXdgExportedV2::zxdg_exported_v2_handle(const QString &handle)
{
    mHandle = handle;
}

QString QWaylandXdgExportedV2::handle() const
{
    return mHandle;
}

QWaylandXdgExporterV2::QWaylandXdgExporterV2(wl_registry *registry, uint32_t id, int version)
    : BobUIWayland::zxdg_exporter_v2(registry, id, qMin(version, 1))
{
}

QWaylandXdgExporterV2::~QWaylandXdgExporterV2()
{
    destroy();
}
}

BOBUI_END_NAMESPACE
