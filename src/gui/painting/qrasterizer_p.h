// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QRASTERIZER_P_H
#define QRASTERIZER_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/qpainter.h"

#include <private/qdrawhelper_p.h>
#include <private/qrasterdefs_p.h>

BOBUI_BEGIN_NAMESPACE

struct QSpanData;
class QRasterBuffer;
class QRasterizerPrivate;

class
QRasterizer
{
public:
    QRasterizer();
    ~QRasterizer();

    void setAntialiased(bool antialiased);
    void setClipRect(const QRect &clipRect);

    void initialize(ProcessSpans blend, void *data);

    void rasterize(const BOBUI_FT_Outline *outline, BobUI::FillRule fillRule);
    void rasterize(const QPainterPath &path, BobUI::FillRule fillRule);

    // width should be in units of |a-b|
    void rasterizeLine(const QPointF &a, const QPointF &b, qreal width, bool squareCap = false);

private:
    QRasterizerPrivate *d;
};

BOBUI_END_NAMESPACE

#endif
