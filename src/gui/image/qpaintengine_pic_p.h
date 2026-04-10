// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPAINTENGINE_PIC_P_H
#define QPAINTENGINE_PIC_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qpaintengine.h>

#ifndef BOBUI_NO_PICTURE

BOBUI_BEGIN_NAMESPACE

class QPicturePaintEnginePrivate;
class QBuffer;

class QPicturePaintEngine : public QPaintEngine
{
    Q_DECLARE_PRIVATE(QPicturePaintEngine)
public:
    QPicturePaintEngine();
    ~QPicturePaintEngine();

    bool begin(QPaintDevice *pdev) override;
    bool end() override;

    void updateState(const QPaintEngineState &state) override;

    void updatePen(const QPen &pen);
    void updateBrush(const QBrush &brush);
    void updateBrushOrigin(const QPointF &origin);
    void updateFont(const QFont &font);
    void updateBackground(BobUI::BGMode bgmode, const QBrush &bgBrush);
    void updateMatrix(const BOBUIransform &matrix);
    void updateClipRegion(const QRegion &region, BobUI::ClipOperation op);
    void updateClipPath(const QPainterPath &path, BobUI::ClipOperation op);
    void updateRenderHints(QPainter::RenderHints hints);
    void updateCompositionMode(QPainter::CompositionMode cmode);
    void updateClipEnabled(bool enabled);
    void updateOpacity(qreal opacity);

    void drawEllipse(const QRectF &rect) override;
    void drawPath(const QPainterPath &path) override;
    void drawPolygon(const QPointF *points, int numPoints, PolygonDrawMode mode) override;
    using QPaintEngine::drawPolygon;

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) override;
    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s) override;
    void drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
                   BobUI::ImageConversionFlags flags = BobUI::AutoColor) override;
    void drawTextItem(const QPointF &p, const BOBUIextItem &ti) override;

    Type type() const override { return Picture; }

protected:
    QPicturePaintEngine(QPaintEnginePrivate &dptr);

private:
    Q_DISABLE_COPY_MOVE(QPicturePaintEngine)

    void writeCmdLength(int pos, const QRectF &r, bool corr);
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_PICTURE

#endif // QPAINTENGINE_PIC_P_H
