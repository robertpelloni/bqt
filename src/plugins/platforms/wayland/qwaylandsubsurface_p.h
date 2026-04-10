// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSUBSURFACE_H
#define QWAYLANDSUBSURFACE_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qmutex.h>

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwayland-wayland.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;
class QWaylandWindow;

class Q_WAYLANDCLIENT_EXPORT QWaylandSubSurface : public BobUIWayland::wl_subsurface
{
public:
    QWaylandSubSurface(QWaylandWindow *window, QWaylandWindow *parent, ::wl_subsurface *subsurface);
    ~QWaylandSubSurface() override;

    QWaylandWindow *window() const { return m_window; }
    QWaylandWindow *parent() const { return m_parent; }

    void setSync();
    void setDeSync();
    bool isSync() const { return m_synchronized; }
    QMutex *syncMutex() { return &m_syncLock; }

private:

    // Intentionally hide public methods from ::wl_subsurface
    // to keep track of the sync state
    void set_sync();
    void set_desync();
    QWaylandWindow *m_window = nullptr;
    QWaylandWindow *m_parent = nullptr;
    bool m_synchronized = false;
    QMutex m_syncLock;

};

BOBUI_END_NAMESPACE

}

#endif // QWAYLANDSUBSURFACE_H
