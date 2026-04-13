// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbconnection.h"

BOBUI_BEGIN_NAMESPACE

class QXcbObject
{
public:
    QXcbObject(QXcbConnection *connection = nullptr) : m_connection(connection) {}

    void setConnection(QXcbConnection *connection) { m_connection = connection; }
    QXcbConnection *connection() const { return m_connection; }

    xcb_atom_t atom(QXcbAtom::Atom atom) const { return m_connection->atom(atom); }
    xcb_connection_t *xcb_connection() const { return m_connection->xcb_connection(); }

private:
    QXcbConnection *m_connection;
};

BOBUI_END_NAMESPACE
