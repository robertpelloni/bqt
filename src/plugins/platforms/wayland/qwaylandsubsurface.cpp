// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandsubsurface_p.h"

#include "qwaylandwindow_p.h"

#include <BobUICore/QDebug>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandSubSurface::QWaylandSubSurface(QWaylandWindow *window, QWaylandWindow *parent, ::wl_subsurface *sub_surface)
    : BobUIWayland::wl_subsurface(sub_surface)
    , m_window(window)
    , m_parent(parent)
{
    m_parent->mChildren << this;
    setDeSync();
}

QWaylandSubSurface::~QWaylandSubSurface()
{
    m_parent->mChildren.removeOne(this);
    destroy();
}

void QWaylandSubSurface::setSync()
{
    QMutexLocker l(&m_syncLock);
    QWaylandSubSurface::set_sync();
}

void QWaylandSubSurface::setDeSync()
{
    QMutexLocker l(&m_syncLock);
    QWaylandSubSurface::set_desync();
}

void QWaylandSubSurface::set_sync()
{
    m_synchronized = true;
    BobUIWayland::wl_subsurface::set_sync();
}

void QWaylandSubSurface::set_desync()
{
    m_synchronized = false;
    BobUIWayland::wl_subsurface::set_desync();
}

}

BOBUI_END_NAMESPACE
