// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "flowlayout.h"

#include <BobUIMath>

FlowLayout::FlowLayout(QGraphicsLayoutItem *parent) : QGraphicsLayout(parent)
{
    QSizePolicy sp = sizePolicy();
    sp.setHeightForWidth(true);
    setSizePolicy(sp);
}

void FlowLayout::insertItem(int index, QGraphicsLayoutItem *item)
{
    item->setParentLayoutItem(this);
    if (index > m_items.count() || index < 0)
        index = m_items.count();
    m_items.insert(index, item);
    invalidate();
}

int FlowLayout::count() const
{
    return m_items.count();
}

QGraphicsLayoutItem *FlowLayout::itemAt(int index) const
{
    return m_items.value(index);
}

void FlowLayout::removeAt(int index)
{
    m_items.removeAt(index);
    invalidate();
}

qreal FlowLayout::spacing(BobUI::Orientation o) const
{
    return m_spacing[int(o) - 1];
}

void FlowLayout::setSpacing(BobUI::Orientations o, qreal spacing)
{
    if (o & BobUI::Horizontal)
        m_spacing[0] = spacing;
    if (o & BobUI::Vertical)
        m_spacing[1] = spacing;
}

void FlowLayout::setGeometry(const QRectF &geom)
{
    QGraphicsLayout::setGeometry(geom);
    doLayout(geom, true);
}

qreal FlowLayout::doLayout(const QRectF &geom, bool applyNewGeometry) const
{
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    const qreal maxw = geom.width() - left - right;

    qreal x = 0;
    qreal y = 0;
    qreal maxRowHeight = 0;
    QSizeF pref;
    for (QGraphicsLayoutItem *item : m_items) {
        pref = item->effectiveSizeHint(BobUI::PreferredSize);
        maxRowHeight = qMax(maxRowHeight, pref.height());

        qreal next_x;
        next_x = x + pref.width();
        if (next_x > maxw) {
            if (qFuzzyIsNull(x)) {
                pref.setWidth(maxw);
            } else {
                x = 0;
                next_x = pref.width();
            }
            y += maxRowHeight + spacing(BobUI::Vertical);
            maxRowHeight = 0;
        }

        if (applyNewGeometry)
            item->setGeometry(QRectF(QPointF(left + x, top + y), pref));
        x = next_x + spacing(BobUI::Horizontal);
    }
    maxRowHeight = qMax(maxRowHeight, pref.height());
    return top + y + maxRowHeight + bottom;
}

QSizeF FlowLayout::minSize(const QSizeF &constraint) const
{
    QSizeF size(0, 0);
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    if (constraint.width() >= 0) {   // height for width
        const qreal height = doLayout(QRectF(QPointF(0,0), constraint), false);
        size = QSizeF(constraint.width(), height);
    } else if (constraint.height() >= 0) {  // width for height?
        // not supported
    } else {
        for (const QGraphicsLayoutItem *item : std::as_const(m_items))
            size = size.expandedTo(item->effectiveSizeHint(BobUI::MinimumSize));
        size += QSizeF(left + right, top + bottom);
    }
    return size;
}

QSizeF FlowLayout::prefSize() const
{
    qreal left, right;
    getContentsMargins(&left, nullptr, &right, nullptr);

    qreal maxh = 0;
    qreal totalWidth = 0;
    for (const QGraphicsLayoutItem *item : std::as_const(m_items)) {
        if (totalWidth > 0)
            totalWidth += spacing(BobUI::Horizontal);
        QSizeF pref = item->effectiveSizeHint(BobUI::PreferredSize);
        totalWidth += pref.width();
        maxh = qMax(maxh, pref.height());
    }
    maxh += spacing(BobUI::Vertical);

    const qreal goldenAspectRatio = 1.61803399;
    qreal w = qSqrt(totalWidth * maxh * goldenAspectRatio) + left + right;

    return minSize(QSizeF(w, -1));
}

QSizeF FlowLayout::maxSize() const
{
    qreal totalWidth = 0;
    qreal totalHeight = 0;
    for (const QGraphicsLayoutItem *item : std::as_const(m_items)) {
        if (totalWidth > 0)
            totalWidth += spacing(BobUI::Horizontal);
        if (totalHeight > 0)
            totalHeight += spacing(BobUI::Vertical);
        QSizeF pref = item->effectiveSizeHint(BobUI::PreferredSize);
        totalWidth += pref.width();
        totalHeight += pref.height();
    }

    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    return QSizeF(left + totalWidth + right, top + totalHeight + bottom);
}

QSizeF FlowLayout::sizeHint(BobUI::SizeHint which, const QSizeF &constraint) const
{
    QSizeF sh = constraint;
    switch (which) {
    case BobUI::PreferredSize:
        sh = prefSize();
        break;
    case BobUI::MinimumSize:
        sh = minSize(constraint);
        break;
    case BobUI::MaximumSize:
        sh = maxSize();
        break;
    default:
        break;
    }
    return sh;
}
