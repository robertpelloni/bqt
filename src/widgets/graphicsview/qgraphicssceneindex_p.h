// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSSCENEINDEX_H
#define QGRAPHICSSCENEINDEX_H

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
#include "qgraphicsscene_p.h"
#include "qgraphicsscene.h"
#include <private/qobject_p.h>

#include <BobUICore/qnamespace.h>
#include <BobUICore/qobject.h>
#include <BobUIGui/bobuiransform.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsSceneIndexPrivate;
class QPointF;
class QRectF;

typedef bool (*QGraphicsSceneIndexIntersector)(const QGraphicsItem *item, const QRectF &exposeRect, BobUI::ItemSelectionMode mode,
                                               const BOBUIransform &deviceTransform, const void *data);

class Q_AUTOTEST_EXPORT QGraphicsSceneIndex : public QObject
{
    Q_OBJECT

public:
    QGraphicsSceneIndex(QGraphicsScene *scene = nullptr);
    virtual ~QGraphicsSceneIndex();

    QGraphicsScene *scene() const;

    virtual QList<QGraphicsItem *> items(BobUI::SortOrder order = BobUI::DescendingOrder) const  = 0;
    virtual QList<QGraphicsItem *> items(const QPointF &pos, BobUI::ItemSelectionMode mode,
                                         BobUI::SortOrder order, const BOBUIransform &deviceTransform = BOBUIransform()) const;
    virtual QList<QGraphicsItem *> items(const QRectF &rect, BobUI::ItemSelectionMode mode,
                                         BobUI::SortOrder order, const BOBUIransform &deviceTransform = BOBUIransform()) const;
    virtual QList<QGraphicsItem *> items(const QPolygonF &polygon, BobUI::ItemSelectionMode mode,
                                         BobUI::SortOrder order, const BOBUIransform &deviceTransform = BOBUIransform()) const;
    virtual QList<QGraphicsItem *> items(const QPainterPath &path, BobUI::ItemSelectionMode mode,
                                         BobUI::SortOrder order, const BOBUIransform &deviceTransform = BOBUIransform()) const;
    virtual QList<QGraphicsItem *> estimateItems(const QPointF &point, BobUI::SortOrder order) const;
    virtual QList<QGraphicsItem *> estimateItems(const QRectF &rect, BobUI::SortOrder order) const = 0;
    virtual QList<QGraphicsItem *> estimateTopLevelItems(const QRectF &, BobUI::SortOrder order) const;

protected Q_SLOTS:
    virtual void updateSceneRect(const QRectF &rect);

protected:
    virtual void clear();
    virtual void addItem(QGraphicsItem *item) = 0;
    virtual void removeItem(QGraphicsItem *item) = 0;
    virtual void deleteItem(QGraphicsItem *item);

    virtual void itemChange(const QGraphicsItem *item, QGraphicsItem::GraphicsItemChange, const void *const value);
    virtual void prepareBoundingRectChange(const QGraphicsItem *item);

    QGraphicsSceneIndex(QGraphicsSceneIndexPrivate &dd, QGraphicsScene *scene);

    friend class QGraphicsScene;
    friend class QGraphicsScenePrivate;
    friend class QGraphicsItem;
    friend class QGraphicsItemPrivate;
    friend class QGraphicsSceneBspTreeIndex;
private:
    Q_DISABLE_COPY_MOVE(QGraphicsSceneIndex)
    Q_DECLARE_PRIVATE(QGraphicsSceneIndex)
};

class QGraphicsSceneIndexPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsSceneIndex)
public:
    QGraphicsSceneIndexPrivate(QGraphicsScene *scene);
    ~QGraphicsSceneIndexPrivate();

    void init();
    static bool itemCollidesWithPath(const QGraphicsItem *item, const QPainterPath &path, BobUI::ItemSelectionMode mode);

    void recursive_items_helper(QGraphicsItem *item, QRectF exposeRect,
                                QGraphicsSceneIndexIntersector intersect, QList<QGraphicsItem *> *items,
                                const BOBUIransform &viewTransform,
                                BobUI::ItemSelectionMode mode, qreal parentOpacity, const void *intersectData) const;
    inline void items_helper(const QRectF &rect, QGraphicsSceneIndexIntersector intersect,
                             QList<QGraphicsItem *> *items, const BOBUIransform &viewTransform,
                             BobUI::ItemSelectionMode mode, BobUI::SortOrder order, const void *intersectData) const;

    QGraphicsScene *scene;
};

inline void QGraphicsSceneIndexPrivate::items_helper(const QRectF &rect, QGraphicsSceneIndexIntersector intersect,
                                                     QList<QGraphicsItem *> *items, const BOBUIransform &viewTransform,
                                                     BobUI::ItemSelectionMode mode, BobUI::SortOrder order, const void *intersectData) const
{
    Q_Q(const QGraphicsSceneIndex);
    const QList<QGraphicsItem *> tli = q->estimateTopLevelItems(rect, BobUI::AscendingOrder);
    for (int i = 0; i < tli.size(); ++i)
        recursive_items_helper(tli.at(i), rect, intersect, items, viewTransform, mode, 1.0, intersectData);
    if (order == BobUI::DescendingOrder) {
        const int n = items->size();
        for (int i = 0; i < n / 2; ++i)
            items->swapItemsAt(i, n - i - 1);
    }
}

BOBUI_END_NAMESPACE

#endif // QGRAPHICSSCENEINDEX_H
