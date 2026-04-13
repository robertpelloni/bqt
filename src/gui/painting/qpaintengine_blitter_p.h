// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPAINTENGINE_BLITTER_P_H
#define QPAINTENGINE_BLITTER_P_H

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
#include "private/qpaintengine_raster_p.h"

#ifndef BOBUI_NO_BLITTABLE
BOBUI_BEGIN_NAMESPACE

class QBlitterPaintEnginePrivate;
class QBlittablePlatformPixmap;
class QBlittable;

class Q_GUI_EXPORT QBlitterPaintEngine : public QRasterPaintEngine
{
    Q_DECLARE_PRIVATE(QBlitterPaintEngine)
public:
    QBlitterPaintEngine(QBlittablePlatformPixmap *p);

    virtual QPaintEngine::Type type() const override
    { return Blitter; }

    virtual bool begin(QPaintDevice *pdev) override;
    virtual bool end() override;

    // Call down into QBlittable
    void fill(const QVectorPath &path, const QBrush &brush) override;
    void fillRect(const QRectF &rect, const QBrush &brush) override;
    void fillRect(const QRectF &rect, const QColor &color) override;
    void drawRects(const QRect *rects, int rectCount) override;
    void drawRects(const QRectF *rects, int rectCount) override;
    void drawPixmap(const QPointF &p, const QPixmap &pm) override;
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) override;

    // State tracking
    void setState(QPainterState *s) override;
    virtual void clipEnabledChanged() override;
    virtual void penChanged() override;
    virtual void brushChanged() override;
    virtual void opacityChanged() override;
    virtual void compositionModeChanged() override;
    virtual void renderHintsChanged() override;
    virtual void transformChanged() override;

    // Override to lock the QBlittable before using raster
    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode) override;
    void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode) override;
    void fillPath(const QPainterPath &path, QSpanData *fillData) override;
    void fillPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode) override;
    void drawEllipse(const QRectF &rect) override;
    void drawImage(const QPointF &p, const QImage &img) override;
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                   BobUI::ImageConversionFlags flags = BobUI::AutoColor) override;
    void drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &sr) override;
    void drawTextItem(const QPointF &p, const BOBUIextItem &textItem) override;
    void drawPoints(const QPointF *points, int pointCount) override;
    void drawPoints(const QPoint *points, int pointCount) override;
    void stroke(const QVectorPath &path, const QPen &pen) override;
    void drawStaticTextItem(QStaticTextItem *) override;
    bool drawCachedGlyphs(int numGlyphs, const glyph_t *glyphs, const QFixedPoint *positions,
                          QFontEngine *fontEngine) override;
};

BOBUI_END_NAMESPACE
#endif //BOBUI_NO_BLITTABLE
#endif // QPAINTENGINE_BLITTER_P_H

