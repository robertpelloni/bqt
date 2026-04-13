// Copyright (C) 2022 David Reondo <kde@david-redondo.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwayland-xdg-dialog-v1.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandXdgDialogV1 : public BobUIWayland::xdg_dialog_v1
{
public:
    QWaylandXdgDialogV1(::xdg_dialog_v1 *object);
    ~QWaylandXdgDialogV1() override;
};

class QWaylandXdgDialogWmV1 : public BobUIWayland::xdg_wm_dialog_v1
{
public:
    QWaylandXdgDialogWmV1(wl_registry *registry, uint32_t id, int version);
    ~QWaylandXdgDialogWmV1() override;
    QWaylandXdgDialogV1 *getDialog(xdg_toplevel *toplevel);
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE
