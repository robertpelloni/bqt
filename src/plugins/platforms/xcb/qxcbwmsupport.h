// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbobject.h"
#include "qxcbconnection.h"
#include <qlist.h>

BOBUI_BEGIN_NAMESPACE

class QXcbWMSupport : public QXcbObject
{
public:
    QXcbWMSupport(QXcbConnection *c);


    bool isSupportedByWM(xcb_atom_t atom) const;
    const QList<xcb_window_t> &virtualRoots() const { return net_virtual_roots; }

private:
    friend class QXcbConnection;
    void updateNetWMAtoms();
    void updateVirtualRoots();

    QList<xcb_atom_t> net_wm_atoms;
    QList<xcb_window_t> net_virtual_roots;
};

BOBUI_END_NAMESPACE
