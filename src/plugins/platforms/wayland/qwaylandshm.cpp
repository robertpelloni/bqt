// Copyright (C) 2016 LG Electronics Inc, author: <mikko.levonmaa@lge.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include <BobUIWaylandClient/private/qwaylandshm_p.h>
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>

#include "qwaylandsharedmemoryformathelper_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandShm::QWaylandShm(QWaylandDisplay *display, int version, uint32_t id)
    : BobUIWayland::wl_shm(display->wl_registry(), id, qMin(version, 2))
{
}

QWaylandShm::~QWaylandShm()
{
    if (version() < WL_SHM_RELEASE_SINCE_VERSION) {
        wl_shm_destroy(object());
    } else {
        wl_shm_release(object());
    }
}

void QWaylandShm::shm_format(uint32_t format)
{
    m_formats << format;
}

bool QWaylandShm::formatSupported(wl_shm_format format) const
{
    return m_formats.contains(format);
}

bool QWaylandShm::formatSupported(QImage::Format format) const
{
    wl_shm_format fmt = formatFrom(format);
    return formatSupported(fmt);
}

wl_shm_format QWaylandShm::formatFrom(QImage::Format format)
{
    return QWaylandSharedMemoryFormatHelper::fromQImageFormat(format);
}

QImage::Format QWaylandShm::formatFrom(wl_shm_format format)
{
    return QWaylandSharedMemoryFormatHelper::fromWaylandShmFormat(format);
}

}

BOBUI_END_NAMESPACE
