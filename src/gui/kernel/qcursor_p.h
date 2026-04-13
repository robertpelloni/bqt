// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCURSOR_P_H
#define QCURSOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of a number of BobUI sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUICore/qatomic.h"
#include "BobUICore/qnamespace.h"
#include "BobUIGui/qpixmap.h"


BOBUI_BEGIN_NAMESPACE


class QBitmap;
class QCursorData {
public:
    QCursorData(BobUI::CursorShape s = BobUI::ArrowCursor);
    ~QCursorData();

    static void initialize();
    static void cleanup();

    QAtomicInt ref;
    BobUI::CursorShape cshape;
    QBitmap  *bm, *bmm;
    QPixmap pixmap;
    short     hx, hy;
    static bool initialized;
    void update();
    static QCursorData *setBitmap(const QBitmap &bitmap, const QBitmap &mask, int hotX, int hotY,
                                  qreal devicePixelRatio);
};

extern QCursorData *bobui_cursorTable[BobUI::LastCursor + 1]; // qcursor.cpp

BOBUI_END_NAMESPACE

#endif // QCURSOR_P_H
