// Copyright (C) 2024 David Reondo <kde@david-redondo.de>
// Copyright (C) 2024 David Edmundson <davidedmundson@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "wayland-xdg-shell-client-protocol.h"
#include <qwayland-xdg-toplevel-icon-v1.h>

#include <QList>

BOBUI_BEGIN_NAMESPACE

class QIcon;

namespace BobUIWaylandClient {

class QWaylandDisplay;

class QWaylandXdgToplevelIconManagerV1 : public BobUIWayland::xdg_toplevel_icon_manager_v1
{
public:
    QWaylandXdgToplevelIconManagerV1(QWaylandDisplay *display, wl_registry *registry, uint32_t id,
                                     int version);
    ~QWaylandXdgToplevelIconManagerV1() override;
    void setIcon(const QIcon &icon, ::xdg_toplevel *window);

protected:
    void xdg_toplevel_icon_manager_v1_icon_size(int32_t size) override;
    void xdg_toplevel_icon_manager_v1_done() override;

private:
    QList<int> mPreferredSizes;
    QWaylandDisplay *mDisplay;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
