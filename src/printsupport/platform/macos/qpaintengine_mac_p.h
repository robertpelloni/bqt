// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPAINTENGINE_MAC_P_H
#define QPAINTENGINE_MAC_P_H

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

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>

#include <BobUIGui/qpaintengine.h>
#include <BobUIGui/private/qpaintengine_p.h>
#include <BobUIGui/private/qfont_p.h>
#include <BobUICore/qhash.h>

typedef struct CGColorSpace *CGColorSpaceRef;
typedef struct CGContext *CGContextRef;

BOBUI_BEGIN_NAMESPACE

class QCoreGraphicsPaintEnginePrivate;
class QCoreGraphicsPaintEngine : public QPaintEngine
{
    Q_DECLARE_PRIVATE(QCoreGraphicsPaintEngine)

public:
    QCoreGraphicsPaintEngine();
    ~QCoreGraphicsPaintEngine();

    bool begin(QPaintDevice *pdev);
    bool end();

    void updateState(const QPaintEngineState &state);

    void updatePen(const QPen &pen);
    void updateBrush(const QBrush &brush, const QPointF &pt);
    void updateFont(const QFont &font);
    void updateOpacity(qreal opacity);
    void updateMatrix(const BOBUIransform &matrix);
    void updateTransform(const BOBUIransform &matrix);
    void updateClipRegion(const QRegion &region, BobUI::ClipOperation op);
    void updateClipPath(const QPainterPath &path, BobUI::ClipOperation op);
    void updateCompositionMode(QPainter::CompositionMode mode);
    void updateRenderHints(QPainter::RenderHints hints);

    void drawLines(const QLineF *lines, int lineCount);
    void drawRects(const QRectF *rects, int rectCount);
    void drawPoints(const QPointF *p, int pointCount);
    void drawEllipse(const QRectF &r);
    void drawPath(const QPainterPath &path);

    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);

    void drawTextItem(const QPointF &pos, const BOBUIextItem &item);
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                   BobUI::ImageConversionFlags flags = BobUI::AutoColor);

    Type type() const { return QPaintEngine::CoreGraphics; }

    CGContextRef handle() const;

    static void initialize();
    static void cleanup();

    QPainter::RenderHints supportedRenderHints() const;

    //avoid partial shadowed overload warnings...
    void drawLines(const QLine *lines, int lineCount) { QPaintEngine::drawLines(lines, lineCount); }
    void drawRects(const QRect *rects, int rectCount) { QPaintEngine::drawRects(rects, rectCount); }
    void drawPoints(const QPoint *p, int pointCount) { QPaintEngine::drawPoints(p, pointCount); }
    void drawEllipse(const QRect &r) { QPaintEngine::drawEllipse(r); }
    void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
    { QPaintEngine::drawPolygon(points, pointCount, mode); }

protected:
    friend class QMacPrintEngine;
    friend class QMacPrintEnginePrivate;
    QCoreGraphicsPaintEngine(QPaintEnginePrivate &dptr);

private:
    Q_DISABLE_COPY(QCoreGraphicsPaintEngine)
};

/*****************************************************************************
  Private data
 *****************************************************************************/
class QCoreGraphicsPaintEnginePrivate : public QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QCoreGraphicsPaintEngine)
public:
    QCoreGraphicsPaintEnginePrivate()
        : hd(nullptr), shading(nullptr), stackCount(0), complexXForm(false), disabledSmoothFonts(false)
    {
    }

    struct {
        QPen pen;
        QBrush brush;
        uint clipEnabled : 1;
        QRegion clip;
        BOBUIransform transform;
   } current;

    //state info (shared with QD)
    CGAffineTransform orig_xform;

    //cg structures
    CGContextRef hd;
    CGShadingRef shading;
    int stackCount;
    bool complexXForm;
    bool disabledSmoothFonts;
    enum { CosmeticNone, CosmeticTransformPath, CosmeticSetPenWidth } cosmeticPen;

    // pixel and cosmetic pen size in user coordinates.
    QPointF pixelSize;
    float cosmeticPenSize;

    //internal functions
    enum { CGStroke=0x01, CGEOFill=0x02, CGFill=0x04 };
    void drawPath(uchar ops, CGMutablePathRef path = nullptr);
    void setClip(const QRegion *rgn = nullptr);
    void resetClip();
    void setFillBrush(const QPointF &origin=QPoint());
    void setStrokePen(const QPen &pen);
    inline void saveGraphicsState();
    inline void restoreGraphicsState();
    float penOffset();
    QPointF devicePixelSize(CGContextRef context);
    float adjustPenWidth(float penWidth);
    inline void setTransform(const BOBUIransform *matrix = nullptr)
    {
        CGContextConcatCTM(hd, CGAffineTransformInvert(CGContextGetCTM(hd)));
        CGAffineTransform xform = orig_xform;
        if (matrix) {
            extern CGAffineTransform bobui_mac_convert_transform_to_cg(const BOBUIransform &);
            xform = CGAffineTransformConcat(bobui_mac_convert_transform_to_cg(*matrix), xform);
        }
        CGContextConcatCTM(hd, xform);
        CGContextSetTextMatrix(hd, xform);
    }
};

inline void QCoreGraphicsPaintEnginePrivate::saveGraphicsState()
{
    ++stackCount;
    CGContextSaveGState(hd);
}

inline void QCoreGraphicsPaintEnginePrivate::restoreGraphicsState()
{
    --stackCount;
    Q_ASSERT(stackCount >= 0);
    CGContextRestoreGState(hd);
}

BOBUI_END_NAMESPACE

#endif // QPAINTENGINE_MAC_P_H
