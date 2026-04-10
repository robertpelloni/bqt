// Copyright (C) 2024 David Redondo <kde@david-redondo.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDAPPMENU_H
#define QWAYLANDAPPMENU_H

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

#include <BobUICore/QObject>

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwayland-appmenu.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandAppMenu : public QObject, public BobUIWayland::org_kde_kwin_appmenu
{
public:
    QWaylandAppMenu();
    ~QWaylandAppMenu();
};

class QWaylandAppMenuManager : public BobUIWayland::org_kde_kwin_appmenu_manager
{
public:
    QWaylandAppMenuManager(wl_registry *registry, quint32 id, int version);
    ~QWaylandAppMenuManager();
};

} // namespace BobUIWaylandClient
BOBUI_END_NAMESPACE

#endif
