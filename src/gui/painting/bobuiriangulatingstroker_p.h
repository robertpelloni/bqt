// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIRIANGULATINGSTROKER_P_H
#define BOBUIRIANGULATINGSTROKER_P_H

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

#include <BobUICore/qmath.h>
#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <private/qdatabuffer_p.h>
#include <qvarlengtharray.h>
#include <private/qvectorpath_p.h>
#include <private/qbezier_p.h>
#include <private/qnumeric_p.h>
#include <private/qmath_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT BOBUIriangulatingStroker
{
public:
    BOBUIriangulatingStroker() : m_vertices(0), m_cx(0), m_cy(0), m_nvx(0), m_nvy(0), m_width(1), m_miter_limit(2),
        m_roundness(0), m_sin_theta(0), m_cos_theta(0), m_inv_scale(1), m_curvyness_mul(1), m_curvyness_add(0),
        m_join_style(BobUI::BevelJoin), m_cap_style(BobUI::SquareCap) {}

    void process(const QVectorPath &path, const QPen &pen, const QRectF &clip, QPainter::RenderHints hints);

    inline int vertexCount() const { return m_vertices.size(); }
    inline const float *vertices() const { return m_vertices.data(); }

    inline void setInvScale(qreal invScale) { m_inv_scale = invScale; }

private:
    inline void emitLineSegment(float x, float y, float nx, float ny);
    void moveTo(const qreal *pts);
    inline void lineTo(const qreal *pts);
    void cubicTo(const qreal *pts);
    void join(const qreal *pts);
    inline void normalVector(float x1, float y1, float x2, float y2, float *nx, float *ny);
    void endCap(const qreal *pts);
    void arcPoints(float cx, float cy, float fromX, float fromY, float toX, float toY, QVarLengthArray<float> &points);
    void endCapOrJoinClosed(const qreal *start, const qreal *cur, bool implicitClose, bool endsAtStart);


    QDataBuffer<float> m_vertices;

    float m_cx, m_cy;           // current points
    float m_nvx, m_nvy;         // normal vector...
    float m_width;
    qreal m_miter_limit;

    int m_roundness;            // Number of line segments in a round join
    qreal m_sin_theta;          // sin(m_roundness / 360);
    qreal m_cos_theta;          // cos(m_roundness / 360);
    qreal m_inv_scale;
    float m_curvyness_mul;
    float m_curvyness_add;

    BobUI::PenJoinStyle m_join_style;
    BobUI::PenCapStyle m_cap_style;
};

class Q_GUI_EXPORT QDashedStrokeProcessor
{
public:
    QDashedStrokeProcessor();

    void process(const QVectorPath &path, const QPen &pen, const QRectF &clip, QPainter::RenderHints hints);

    inline void addElement(QPainterPath::ElementType type, qreal x, qreal y) {
        m_points.add(x);
        m_points.add(y);
        m_types.add(type);
    }

    inline int elementCount() const { return m_types.size(); }
    inline qreal *points() const { return m_points.data(); }
    inline QPainterPath::ElementType *elementTypes() const { return m_types.data(); }

    inline void setInvScale(qreal invScale) { m_inv_scale = invScale; }

private:
    QDataBuffer<qreal> m_points;
    QDataBuffer<QPainterPath::ElementType> m_types;
    QDashStroker m_dash_stroker;
    qreal m_inv_scale;
};

inline void BOBUIriangulatingStroker::normalVector(float x1, float y1, float x2, float y2,
                                                float *nx, float *ny)
{
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    const float pw = m_width / qHypot(dx, dy);

    *nx = -dy * pw;
    *ny = dx * pw;
}

inline void BOBUIriangulatingStroker::emitLineSegment(float x, float y, float vx, float vy)
{
    m_vertices.add(x + vx);
    m_vertices.add(y + vy);
    m_vertices.add(x - vx);
    m_vertices.add(y - vy);
}

void BOBUIriangulatingStroker::lineTo(const qreal *pts)
{
    emitLineSegment(pts[0], pts[1], m_nvx, m_nvy);
    m_cx = pts[0];
    m_cy = pts[1];
}

BOBUI_END_NAMESPACE

#endif
