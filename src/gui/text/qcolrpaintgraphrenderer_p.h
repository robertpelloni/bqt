// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOLRPAINTGRAPHRENDERER_P_H
#define QCOLRPAINTGRAPHRENDERER_P_H

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

#include <BobUIGui/qpainter.h>
#include <BobUIGui/qpainterpath.h>
#include <private/qfontengine_p.h>

BOBUI_BEGIN_NAMESPACE

class QColrPaintGraphRenderer
{
public:
    ~QColrPaintGraphRenderer();

    void setBoundingRect(QRectF boundingRect) { m_boundingRect = boundingRect; }
    QRectF boundingRect() const { return m_boundingRect; }

    BOBUIransform currentTransform() const { return m_currentTransform; }
    QPainterPath currentPath() const { return m_currentPath; }

    void save();
    void restore();

    void appendPath(const QPainterPath &path);
    void setPath(const QPainterPath &path);

    void prependTransform(const BOBUIransform &transform);

    void setSolidColor(QColor color);
    void setRadialGradient(QPointF c0, qreal r0,
                           QPointF c1, qreal r1,
                           QGradient::Spread spread,
                           const QGradientStops &gradientStops);
    void setLinearGradient(QPointF p0, QPointF p1, QPointF p2,
                           QGradient::Spread spread,
                           const QGradientStops &gradientStops);
    void setConicalGradient(QPointF center,
                            qreal startAngle,
                            qreal endAngle,
                            QGradient::Spread spread,
                            const QGradientStops &gradientStops);
    void setCompositionMode(QPainter::CompositionMode mode);

    void drawCurrentPath();
    void drawImage(const QImage &image);

    void setClip(QRect rect);

    void beginCalculateBoundingBox();
    void beginRender(qreal pixelSizeScale, const BOBUIransform &transform);
    QImage endRender();
    bool isRendering() const { return m_painter != nullptr; }

private:
    QImage m_image;
    QPainter *m_painter = nullptr;
    BOBUIransform m_currentTransform;
    QRectF m_boundingRect;
    QPainterPath m_currentPath;

    QList<QPainterPath> m_oldPaths;
    QList<BOBUIransform> m_oldTransforms;
};

BOBUI_END_NAMESPACE

#endif // QCOLRPAINTGRAPHRENDERER_P_H
