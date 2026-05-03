// Copyright (C) 2024 David Redondo <kde@david-redondo.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandappmenu_p.h"

#include "qwaylandwindow_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandAppMenu::QWaylandAppMenu() : org_kde_kwin_appmenu() { }

QWaylandAppMenu::~QWaylandAppMenu()
{
    if (object())
        release();
}

QWaylandAppMenuManager::QWaylandAppMenuManager(wl_registry *registry, quint32 id, int version)
    : org_kde_kwin_appmenu_manager(registry, id, version)
{
}

QWaylandAppMenuManager::~QWaylandAppMenuManager()
{
    org_kde_kwin_appmenu_manager_destroy(object());
}

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
