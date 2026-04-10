// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEMULATIONPAINTENGINE_P_H
#define QEMULATIONPAINTENGINE_P_H

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
#include <private/qpaintengineex_p.h>

BOBUI_BEGIN_NAMESPACE


class QEmulationPaintEngine : public QPaintEngineEx
{
public:
    QEmulationPaintEngine(QPaintEngineEx *engine);

    bool begin(QPaintDevice *pdev) override;
    bool end() override;

    Type type() const override;
    QPainterState *createState(QPainterState *orig) const override;

    void fill(const QVectorPath &path, const QBrush &brush) override;
    void stroke(const QVectorPath &path, const QPen &pen) override;
    void clip(const QVectorPath &path, BobUI::ClipOperation op) override;

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) override;
    void drawTextItem(const QPointF &p, const BOBUIextItem &textItem) override;
    void drawStaticTextItem(QStaticTextItem *item) override;
    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s) override;
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, BobUI::ImageConversionFlags flags) override;

    void clipEnabledChanged() override;
    void penChanged() override;
    void brushChanged() override;
    void brushOriginChanged() override;
    void opacityChanged() override;
    void compositionModeChanged() override;
    void renderHintsChanged() override;
    void transformChanged() override;

    void setState(QPainterState *s) override;

    void beginNativePainting() override;
    void endNativePainting() override;

    uint flags() const override { return QPaintEngineEx::IsEmulationEngine | QPaintEngineEx::DoNotEmulate; }

    inline QPainterState *state() { return (QPainterState *)QPaintEngine::state; }
    inline const QPainterState *state() const { return (const QPainterState *)QPaintEngine::state; }

    QPaintEngineEx *real_engine;
private:
    void fillBGRect(const QRectF &r);
};

BOBUI_END_NAMESPACE

#endif
