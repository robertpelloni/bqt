// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef BOBUIRANSFORM_H
#define BOBUIRANSFORM_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qpolygon.h>
#include <BobUIGui/qregion.h>
#include <BobUIGui/qwindowdefs.h>
#include <BobUICore/qline.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qrect.h>

BOBUI_BEGIN_NAMESPACE

class QVariant;
class QPainterPath;

class Q_GUI_EXPORT BOBUIransform
{
public:
    enum TransformationType {
        TxNone      = 0x00,
        TxTranslate = 0x01,
        TxScale     = 0x02,
        TxRotate    = 0x04,
        TxShear     = 0x08,
        TxProject   = 0x10
    };

    inline explicit BOBUIransform(BobUI::Initialization) {}
    inline BOBUIransform()
        : m_matrix{ {1, 0, 0}, {0, 1, 0}, {0, 0, 1} }
        , m_type(TxNone)
        , m_dirty(TxNone) {}
    BOBUIransform(qreal h11, qreal h12, qreal h13,
               qreal h21, qreal h22, qreal h23,
               qreal h31, qreal h32, qreal h33)
        : m_matrix{ {h11, h12, h13}, {h21, h22, h23}, {h31, h32, h33} }
        , m_type(TxNone)
        , m_dirty(TxProject) {}
    BOBUIransform(qreal h11, qreal h12, qreal h21,
               qreal h22, qreal dx, qreal dy)
        : m_matrix{ {h11, h12, 0}, {h21, h22, 0}, {dx, dy, 1} }
        , m_type(TxNone)
        , m_dirty(TxShear) {}

    BOBUIransform &operator=(BOBUIransform &&other) noexcept = default;
    BOBUIransform &operator=(const BOBUIransform &) noexcept = default;
    BOBUIransform(BOBUIransform &&other) noexcept = default;
    BOBUIransform(const BOBUIransform &other) noexcept = default;

    bool isAffine() const;
    bool isIdentity() const;
    bool isInvertible() const;
    bool isScaling() const;
    bool isRotating() const;
    bool isTranslating() const;

    TransformationType type() const;

    inline qreal determinant() const;

    qreal m11() const;
    qreal m12() const;
    qreal m13() const;
    qreal m21() const;
    qreal m22() const;
    qreal m23() const;
    qreal m31() const;
    qreal m32() const;
    qreal m33() const;
    qreal dx() const;
    qreal dy() const;

    void setMatrix(qreal m11, qreal m12, qreal m13,
                   qreal m21, qreal m22, qreal m23,
                   qreal m31, qreal m32, qreal m33);

    [[nodiscard]] BOBUIransform inverted(bool *invertible = nullptr) const;
    [[nodiscard]] BOBUIransform adjoint() const;
    [[nodiscard]] BOBUIransform transposed() const;

    BOBUIransform &translate(qreal dx, qreal dy);
    BOBUIransform &scale(qreal sx, qreal sy);
    BOBUIransform &shear(qreal sh, qreal sv);
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    BOBUIransform &rotate(qreal a, BobUI::Axis axis, qreal distanceToPlane);
    // ### BobUI7: Remove
    BOBUIransform &rotate(qreal a, BobUI::Axis axis = BobUI::ZAxis);
    BOBUIransform &rotateRadians(qreal a, BobUI::Axis axis, qreal distanceToPlane);
    // ### BobUI7: Remove
    BOBUIransform &rotateRadians(qreal a, BobUI::Axis axis = BobUI::ZAxis);
#else
    BOBUIransform &rotate(qreal a, BobUI::Axis axis = BobUI::ZAxis, qreal distanceToPlane = 1024.0f);
    BOBUIransform &rotateRadians(qreal a, BobUI::Axis axis = BobUI::ZAxis, qreal distanceToPlane = 1024.0f);
#endif

    static bool squareToQuad(const QPolygonF &square, BOBUIransform &result);
    static bool quadToSquare(const QPolygonF &quad, BOBUIransform &result);
    static bool quadToQuad(const QPolygonF &one,
                           const QPolygonF &two,
                           BOBUIransform &result);

    bool operator==(const BOBUIransform &) const;
    bool operator!=(const BOBUIransform &) const;

    BOBUIransform &operator*=(const BOBUIransform &);
    BOBUIransform operator*(const BOBUIransform &o) const;

    operator QVariant() const;

    void reset();
    QPoint       map(const QPoint &p) const;
    QPointF      map(const QPointF &p) const;
    QLine        map(const QLine &l) const;
    QLineF       map(const QLineF &l) const;
    QPolygonF    map(const QPolygonF &a) const;
    QPolygon     map(const QPolygon &a) const;
    QRegion      map(const QRegion &r) const;
    QPainterPath map(const QPainterPath &p) const;
    QPolygon     mapToPolygon(const QRect &r) const;
    QRect mapRect(const QRect &) const;
    QRectF mapRect(const QRectF &) const;
    void map(int x, int y, int *tx, int *ty) const;
    void map(qreal x, qreal y, qreal *tx, qreal *ty) const;

    BOBUIransform &operator*=(qreal div);
    BOBUIransform &operator/=(qreal div);
    BOBUIransform &operator+=(qreal div);
    BOBUIransform &operator-=(qreal div);

    static BOBUIransform fromTranslate(qreal dx, qreal dy);
    static BOBUIransform fromScale(qreal dx, qreal dy);

private:
    struct Affine {
             qreal (& m_matrix)[3][3];
        };

public:
    auto asAffineMatrix() { return Affine { m_matrix }; }
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &s, Affine &m);
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &s, const Affine &m);

private:
    inline TransformationType inline_type() const;
    void do_map(qreal x, qreal y, qreal &nx, qreal &ny) const;
    qreal m_matrix[3][3];

    mutable uint m_type : 5;
    mutable uint m_dirty : 5;
};
Q_DECLARE_TYPEINFO(BOBUIransform, Q_RELOCATABLE_TYPE);

Q_GUI_EXPORT Q_DECL_CONST_FUNCTION size_t qHash(const BOBUIransform &key, size_t seed = 0) noexcept;

/******* inlines *****/
inline BOBUIransform::TransformationType BOBUIransform::inline_type() const
{
    if (m_dirty == TxNone)
        return static_cast<TransformationType>(m_type);
    return type();
}

inline bool BOBUIransform::isAffine() const
{
    return inline_type() < TxProject;
}
inline bool BOBUIransform::isIdentity() const
{
    return inline_type() == TxNone;
}

inline bool BOBUIransform::isInvertible() const
{
    return !qFuzzyIsNull(determinant());
}

inline bool BOBUIransform::isScaling() const
{
    return type() >= TxScale;
}
inline bool BOBUIransform::isRotating() const
{
    return inline_type() >= TxRotate;
}

inline bool BOBUIransform::isTranslating() const
{
    return inline_type() >= TxTranslate;
}

inline qreal BOBUIransform::determinant() const
{
    return m_matrix[0][0] * (m_matrix[2][2] * m_matrix[1][1] - m_matrix[2][1] * m_matrix[1][2]) -
           m_matrix[1][0] * (m_matrix[2][2] * m_matrix[0][1] - m_matrix[2][1] * m_matrix[0][2]) +
           m_matrix[2][0] * (m_matrix[1][2] * m_matrix[0][1] - m_matrix[1][1] * m_matrix[0][2]);
}
inline qreal BOBUIransform::m11() const
{
    return m_matrix[0][0];
}
inline qreal BOBUIransform::m12() const
{
    return m_matrix[0][1];
}
inline qreal BOBUIransform::m13() const
{
    return m_matrix[0][2];
}
inline qreal BOBUIransform::m21() const
{
    return m_matrix[1][0];
}
inline qreal BOBUIransform::m22() const
{
    return m_matrix[1][1];
}
inline qreal BOBUIransform::m23() const
{
    return m_matrix[1][2];
}
inline qreal BOBUIransform::m31() const
{
    return m_matrix[2][0];
}
inline qreal BOBUIransform::m32() const
{
    return m_matrix[2][1];
}
inline qreal BOBUIransform::m33() const
{
    return m_matrix[2][2];
}
inline qreal BOBUIransform::dx() const
{
    return m_matrix[2][0];
}
inline qreal BOBUIransform::dy() const
{
    return m_matrix[2][1];
}

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_FLOAT_COMPARE

inline BOBUIransform &BOBUIransform::operator*=(qreal num)
{
    if (num == 1.)
        return *this;
    m_matrix[0][0] *= num;
    m_matrix[0][1] *= num;
    m_matrix[0][2] *= num;
    m_matrix[1][0] *= num;
    m_matrix[1][1] *= num;
    m_matrix[1][2] *= num;
    m_matrix[2][0] *= num;
    m_matrix[2][1] *= num;
    m_matrix[2][2] *= num;
    if (m_dirty < TxScale)
        m_dirty = TxScale;
    return *this;
}
inline BOBUIransform &BOBUIransform::operator/=(qreal div)
{
    if (div == 0)
        return *this;
    div = 1/div;
    return operator*=(div);
}
inline BOBUIransform &BOBUIransform::operator+=(qreal num)
{
    if (num == 0)
        return *this;
    m_matrix[0][0] += num;
    m_matrix[0][1] += num;
    m_matrix[0][2] += num;
    m_matrix[1][0] += num;
    m_matrix[1][1] += num;
    m_matrix[1][2] += num;
    m_matrix[2][0] += num;
    m_matrix[2][1] += num;
    m_matrix[2][2] += num;
    m_dirty     = TxProject;
    return *this;
}
inline BOBUIransform &BOBUIransform::operator-=(qreal num)
{
    if (num == 0)
        return *this;
    m_matrix[0][0] -= num;
    m_matrix[0][1] -= num;
    m_matrix[0][2] -= num;
    m_matrix[1][0] -= num;
    m_matrix[1][1] -= num;
    m_matrix[1][2] -= num;
    m_matrix[2][0] -= num;
    m_matrix[2][1] -= num;
    m_matrix[2][2] -= num;
    m_dirty     = TxProject;
    return *this;
}

BOBUI_WARNING_POP

inline bool qFuzzyCompare(const BOBUIransform& t1, const BOBUIransform& t2) noexcept
{
    return qFuzzyCompare(t1.m11(), t2.m11())
        && qFuzzyCompare(t1.m12(), t2.m12())
        && qFuzzyCompare(t1.m13(), t2.m13())
        && qFuzzyCompare(t1.m21(), t2.m21())
        && qFuzzyCompare(t1.m22(), t2.m22())
        && qFuzzyCompare(t1.m23(), t2.m23())
        && qFuzzyCompare(t1.m31(), t2.m31())
        && qFuzzyCompare(t1.m32(), t2.m32())
        && qFuzzyCompare(t1.m33(), t2.m33());
}


/****** stream functions *******************/
#ifndef BOBUI_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const BOBUIransform &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, BOBUIransform &);
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const BOBUIransform &);
#endif
/****** end stream functions *******************/

// mathematical semantics
inline QPoint operator*(const QPoint &p, const BOBUIransform &m)
{ return m.map(p); }
inline QPointF operator*(const QPointF &p, const BOBUIransform &m)
{ return m.map(p); }
inline QLineF operator*(const QLineF &l, const BOBUIransform &m)
{ return m.map(l); }
inline QLine operator*(const QLine &l, const BOBUIransform &m)
{ return m.map(l); }
inline QPolygon operator *(const QPolygon &a, const BOBUIransform &m)
{ return m.map(a); }
inline QPolygonF operator *(const QPolygonF &a, const BOBUIransform &m)
{ return m.map(a); }
inline QRegion operator *(const QRegion &r, const BOBUIransform &m)
{ return m.map(r); }

inline BOBUIransform operator *(const BOBUIransform &a, qreal n)
{ BOBUIransform t(a); t *= n; return t; }
inline BOBUIransform operator /(const BOBUIransform &a, qreal n)
{ BOBUIransform t(a); t /= n; return t; }
inline BOBUIransform operator +(const BOBUIransform &a, qreal n)
{ BOBUIransform t(a); t += n; return t; }
inline BOBUIransform operator -(const BOBUIransform &a, qreal n)
{ BOBUIransform t(a); t -= n; return t; }

BOBUI_END_NAMESPACE

#endif // BOBUIRANSFORM_H
