// Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIUIO_P_H
#define BOBUIUIO_P_H

BOBUI_BEGIN_NAMESPACE

inline bool bobui_readOscString(const QByteArray &source, QByteArray &dest, quint32 &pos)
{
    int end = source.indexOf('\0', pos);
    if (end < 0) {
        pos = source.size();
        dest = QByteArray();
        return false;
    }

    dest = source.mid(pos, end - pos);

    // Skip additional NULL bytes at the end of the string to make sure the
    // total number of bits a multiple of 32 bits ("OSC-string" in the
    // specification).
    end += 4 - ((end - pos) % 4);

    pos = end;
    return true;
}

BOBUI_END_NAMESPACE

#endif
