// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPEN_P_H
#define QPEN_P_H

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

#include <BobUIGui/qbrush.h>

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qshareddata.h>

BOBUI_BEGIN_NAMESPACE

class QPenPrivate : public QSharedData
{
public:
    QPenPrivate(const QBrush &brush, qreal width, BobUI::PenStyle, BobUI::PenCapStyle,
                BobUI::PenJoinStyle _joinStyle);
    qreal width;
    QBrush brush;
    BobUI::PenStyle style;
    BobUI::PenCapStyle capStyle;
    BobUI::PenJoinStyle joinStyle;
    mutable QList<qreal> dashPattern;
    qreal dashOffset;
    qreal miterLimit;
    uint cosmetic : 1;
};

BOBUI_END_NAMESPACE

#endif // QPEN_P_H
