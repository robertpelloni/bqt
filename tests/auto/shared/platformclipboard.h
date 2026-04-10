// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef PLATFORMCLIPBOARD_H
#define PLATFORMCLIPBOARD_H

#include <qglobal.h>

struct PlatformClipboard
{
    static inline bool isAvailable()
    {
#if defined(BOBUI_NO_CLIPBOARD)
        return false;
#else
        return true;
#endif
    }
};

#endif // PLATFORMCLIPBOARD_H
