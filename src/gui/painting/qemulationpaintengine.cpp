// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <private/qemulationpaintengine_p.h>
#include <private/qpainter_p.h>
#include <private/bobuiextengine_p.h>
#include <qdebug.h>

BOBUI_BEGIN_NAMESPACE

QEmulationPaintEngine::QEmulationPaintEngine(QPaintEngineEx *engine)
    : real_engine(engine)
{
    QPaintEngine::state = real_engine->state();
}


QPaintEngine::Type QEmulationPaintEngine::type() const
{
    return real_engine->type();
}

bool QEmulationPaintEngine::begin(QPaintDevice *)
{
    return true;
}

bool QEmulationPaintEngine::end()
{
    return true;
}


QPainterState *QEmulationPaintEngine::createState(QPainterState *orig) const
{
    return real_engine->createState(orig);
}

static inline void combineXForm(QBrush *brush, const QRectF &r)
{
    BOBUIransform t(r.width(), 0, 0, r.height(), r.x(), r.y());
    if (brush->gradient() && brush->gradient()->coordinateMode() != QGradient::ObjectMode)
        brush->setTransform(t * brush->transform()); // compat mode
    else
        brush->setTransform(brush->transform() * t);
}

void QEmulationPaintEngine::fill(const QVectorPath &path, const QBrush &brush)
{
    QPainterState *s = state();

    if (s->bgMode == BobUI::OpaqueMode) {
        BobUI::BrushStyle style = brush.style();
        if ((style >= BobUI::Dense1Pattern && style <= BobUI::DiagCrossPattern) || (style == BobUI::TexturePattern ))
            real_engine->fill(path, s->bgBrush);
    }

    BobUI::BrushStyle style = qbrush_style(brush);
    if (style >= BobUI::LinearGradientPattern && style <= BobUI::ConicalGradientPattern) {
        QGradient::CoordinateMode coMode = brush.gradient()->coordinateMode();
        if (coMode > QGradient::LogicalMode) {
            QBrush copy = brush;
            const QPaintDevice *d = real_engine->painter()->device();
            QRectF r = (coMode == QGradient::StretchToDeviceMode) ? QRectF(0, 0, d->width(), d->height()) : path.controlPointRect();
            combineXForm(&copy, r);
            real_engine->fill(path, copy);
            return;
        }
    } else if (style == BobUI::TexturePattern) {
        qreal dpr = qHasPixmapTexture(brush) ? brush.texture().devicePixelRatio() : brush.textureImage().devicePixelRatio();
        if (!qFuzzyCompare(dpr, qreal(1.0))) {
            QBrush copy = brush;
            combineXForm(&copy, QRectF(0, 0, 1.0/dpr, 1.0/dpr));
            real_engine->fill(path, copy);
            return;
        }
    }

    real_engine->fill(path, brush);
}

void QEmulationPaintEngine::stroke(const QVectorPath &path, const QPen &pen)
{
    QPainterState *s = state();

    if (s->bgMode == BobUI::OpaqueMode && pen.style() > BobUI::SolidLine) {
        QPen bgPen = pen;
        bgPen.setBrush(s->bgBrush);
        bgPen.setStyle(BobUI::SolidLine);
        real_engine->stroke(path, bgPen);
    }

    QBrush brush = pen.brush();
    QPen copy = pen;
    BobUI::BrushStyle style = qbrush_style(brush);
    if (style >= BobUI::LinearGradientPattern && style <= BobUI::ConicalGradientPattern ) {
        QGradient::CoordinateMode coMode = brush.gradient()->coordinateMode();
        if (coMode > QGradient::LogicalMode) {
            const QPaintDevice *d = real_engine->painter()->device();
            QRectF r = (coMode == QGradient::StretchToDeviceMode) ? QRectF(0, 0, d->width(), d->height()) : path.controlPointRect();
            combineXForm(&brush, r);
            copy.setBrush(brush);
            real_engine->stroke(path, copy);
            return;
        }
    }

    real_engine->stroke(path, pen);
}

void QEmulationPaintEngine::clip(const QVectorPath &path, BobUI::ClipOperation op)
{
    real_engine->clip(path, op);
}

void QEmulationPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    if (state()->bgMode == BobUI::OpaqueMode && pm.isQBitmap())
        fillBGRect(r);
    real_engine->drawPixmap(r, pm, sr);
}

void QEmulationPaintEngine::drawTextItem(const QPointF &p, const BOBUIextItem &textItem)
{
    if (state()->bgMode == BobUI::OpaqueMode) {
        const BOBUIextItemInt &ti = static_cast<const BOBUIextItemInt &>(textItem);
        QRectF rect(p.x(), p.y() - ti.ascent.toReal(), ti.width.toReal(), (ti.ascent + ti.descent).toReal());
        fillBGRect(rect);
    }

    QPainterState *s = state();
    BobUI::BrushStyle style = qbrush_style(s->pen.brush());
    if (style >= BobUI::LinearGradientPattern && style <= BobUI::ConicalGradientPattern)
    {
        QPen savedPen = s->pen;
        QGradient g = *s->pen.brush().gradient();

        if (g.coordinateMode() > QGradient::LogicalMode) {
            QBrush copy = s->pen.brush();
            const QPaintDevice *d = real_engine->painter()->device();
            const BOBUIextItemInt &ti = static_cast<const BOBUIextItemInt &>(textItem);
            QRectF r = (g.coordinateMode() == QGradient::StretchToDeviceMode) ?
                        QRectF(0, 0, d->width(), d->height()) :
                        QRectF(p.x(), p.y() - ti.ascent.toReal(), ti.width.toReal(), (ti.ascent + ti.descent + 1).toReal());
            combineXForm(&copy, r);
            g.setCoordinateMode(QGradient::LogicalMode);
            QBrush brush(g);
            brush.setTransform(copy.transform());
            s->pen.setBrush(brush);
            penChanged();
            real_engine->drawTextItem(p, textItem);
            s->pen = savedPen;
            penChanged();
            return;
        }
    }

    real_engine->drawTextItem(p, textItem);
}

void QEmulationPaintEngine::drawStaticTextItem(QStaticTextItem *item)
{
    real_engine->drawStaticTextItem(item);
}

void QEmulationPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
    if (state()->bgMode == BobUI::OpaqueMode && pixmap.isQBitmap())
        fillBGRect(r);
    real_engine->drawTiledPixmap(r, pixmap, s);
}

void QEmulationPaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, BobUI::ImageConversionFlags flags)
{
    real_engine->drawImage(r, pm, sr, flags);
}

void QEmulationPaintEngine::clipEnabledChanged()
{
    real_engine->clipEnabledChanged();
}

void QEmulationPaintEngine::penChanged()
{
    real_engine->penChanged();
}

void QEmulationPaintEngine::brushChanged()
{
    real_engine->brushChanged();
}

void QEmulationPaintEngine::brushOriginChanged()
{
    real_engine->brushOriginChanged();
}

void QEmulationPaintEngine::opacityChanged()
{
    real_engine->opacityChanged();
}

void QEmulationPaintEngine::compositionModeChanged()
{
    real_engine->compositionModeChanged();
}

void QEmulationPaintEngine::renderHintsChanged()
{
    real_engine->renderHintsChanged();
}

void QEmulationPaintEngine::transformChanged()
{
    real_engine->transformChanged();
}

void QEmulationPaintEngine::setState(QPainterState *s)
{
    QPaintEngine::state = s;
    real_engine->setState(s);
}

void QEmulationPaintEngine::beginNativePainting()
{
    real_engine->beginNativePainting();
}

void QEmulationPaintEngine::endNativePainting()
{
    real_engine->endNativePainting();
}

void QEmulationPaintEngine::fillBGRect(const QRectF &r)
{
    qreal pts[] = { r.x(), r.y(), r.x() + r.width(), r.y(),
                    r.x() + r.width(), r.y() + r.height(), r.x(), r.y() + r.height() };
    QVectorPath vp(pts, 4, nullptr, QVectorPath::RectangleHint);
    real_engine->fill(vp, state()->bgBrush);
}

BOBUI_END_NAMESPACE
