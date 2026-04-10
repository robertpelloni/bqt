// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSSCENELINEARINDEX_H
#define QGRAPHICSSCENELINEARINDEX_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>

#include <BobUICore/qrect.h>
#include <BobUICore/qlist.h>
#include <BobUIWidgets/qgraphicsitem.h>
#include <private/qgraphicssceneindex_p.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QGraphicsSceneLinearIndex : public QGraphicsSceneIndex
{
    Q_OBJECT

public:
    QGraphicsSceneLinearIndex(QGraphicsScene *scene = nullptr) : QGraphicsSceneIndex(scene), m_numSortedElements(0)
    { }

    QList<QGraphicsItem *> items(BobUI::SortOrder order = BobUI::DescendingOrder) const override
    { Q_UNUSED(order); return m_items; }

    virtual QList<QGraphicsItem *> estimateItems(const QRectF &rect, BobUI::SortOrder order) const override
    {
        Q_UNUSED(rect);
        Q_UNUSED(order);
        return m_items;
    }

protected :
    virtual void clear() override
    {
        m_items.clear();
        m_numSortedElements = 0;
    }

    virtual void addItem(QGraphicsItem *item) override
    { m_items << item; }

    virtual void removeItem(QGraphicsItem *item) override
    {
        // Sort m_items if needed
        if (m_numSortedElements < m_items.size())
        {
            std::sort(m_items.begin() + m_numSortedElements, m_items.end() );
            std::inplace_merge(m_items.begin(), m_items.begin() + m_numSortedElements, m_items.end());
            m_numSortedElements = m_items.size();
        }

        QList<QGraphicsItem*>::iterator element = std::lower_bound(m_items.begin(), m_items.end(), item);
        if (element != m_items.end() && *element == item)
        {
            m_items.erase(element);
            --m_numSortedElements;
        }
    }

private:
    QList<QGraphicsItem*> m_items;
    int m_numSortedElements;
};

BOBUI_END_NAMESPACE

#endif // QGRAPHICSSCENELINEARINDEX_H
