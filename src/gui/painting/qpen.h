// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPEN_H
#define QPEN_H

#include <BobUICore/qshareddata.h>
#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qcolor.h>
#include <BobUIGui/qbrush.h>

BOBUI_BEGIN_NAMESPACE


class QVariant;
class QPenPrivate;
class QBrush;
class QPen;

#ifndef BOBUI_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QPen &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QPen &);
#endif

BOBUI_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QPenPrivate, Q_GUI_EXPORT)

class Q_GUI_EXPORT QPen
{
public:
    QPen();
    QPen(BobUI::PenStyle);
    QPen(const QColor &color);
    QPen(const QBrush &brush, qreal width, BobUI::PenStyle s = BobUI::SolidLine,
         BobUI::PenCapStyle c = BobUI::SquareCap, BobUI::PenJoinStyle j = BobUI::BevelJoin);
    QPen(const QPen &pen) noexcept;

    ~QPen();

    QPen &operator=(const QPen &pen) noexcept;
    QPen(QPen &&other) noexcept = default;
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QPen)
    void swap(QPen &other) noexcept { d.swap(other.d); }

    QPen &operator=(QColor color);
    QPen &operator=(BobUI::PenStyle style);

    BobUI::PenStyle style() const;
    void setStyle(BobUI::PenStyle);

    QList<qreal> dashPattern() const;
    void setDashPattern(const QList<qreal> &pattern);

    qreal dashOffset() const;
    void setDashOffset(qreal doffset);

    qreal miterLimit() const;
    void setMiterLimit(qreal limit);

    qreal widthF() const;
    void setWidthF(qreal width);

    int width() const;
    void setWidth(int width);

    QColor color() const;
    void setColor(const QColor &color);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    bool isSolid() const;

    BobUI::PenCapStyle capStyle() const;
    void setCapStyle(BobUI::PenCapStyle pcs);

    BobUI::PenJoinStyle joinStyle() const;
    void setJoinStyle(BobUI::PenJoinStyle pcs);

    bool isCosmetic() const;
    void setCosmetic(bool cosmetic);

    bool operator==(const QPen &p) const;
    inline bool operator!=(const QPen &p) const { return !(operator==(p)); }
    operator QVariant() const;

    bool isDetached();

private:
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QPen &);
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QPen &);

    bool isSolidDefaultLine() const noexcept;

    bool doCompareEqualColor(QColor rhs) const noexcept;
    friend bool comparesEqual(const QPen &lhs, QColor rhs) noexcept
    {
        return lhs.doCompareEqualColor(rhs);
    }
    Q_DECLARE_EQUALITY_COMPARABLE(QPen, QColor)

    bool doCompareEqualStyle(BobUI::PenStyle rhs) const;
    friend bool comparesEqual(const QPen &lhs, BobUI::PenStyle rhs)
    {
        return lhs.doCompareEqualStyle(rhs);
    }
    Q_DECLARE_EQUALITY_COMPARABLE_NON_NOEXCEPT(QPen, BobUI::PenStyle)

public:
    using DataPtr = QExplicitlySharedDataPointer<QPenPrivate>;

private:
    void detach();
    DataPtr d;

public:
    inline DataPtr &data_ptr() { return d; }
};

Q_DECLARE_SHARED(QPen)

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QPen &);
#endif

BOBUI_END_NAMESPACE

#endif // QPEN_H
