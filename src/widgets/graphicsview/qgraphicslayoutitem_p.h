// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSLAYOUTITEM_P_H
#define QGRAPHICSLAYOUTITEM_P_H

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
#include <BobUICore/QSizeF>
#include <BobUIWidgets/QSizePolicy>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsLayoutItem;
class Q_AUTOTEST_EXPORT QGraphicsLayoutItemPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsLayoutItem)
public:
    virtual ~QGraphicsLayoutItemPrivate();
    QGraphicsLayoutItemPrivate(QGraphicsLayoutItem *parent, bool isLayout);
    static QGraphicsLayoutItemPrivate *get(QGraphicsLayoutItem *q) { return q->d_func();}
    static const QGraphicsLayoutItemPrivate *get(const QGraphicsLayoutItem *q) { return q->d_func();}

    void init();
    QSizeF *effectiveSizeHints(const QSizeF &constraint) const;
    QGraphicsItem *parentItem() const;
    void ensureUserSizeHints();
    void setSize(BobUI::SizeHint which, const QSizeF &size);
    enum SizeComponent { Width, Height };
    void setSizeComponent(BobUI::SizeHint which, SizeComponent component, qreal value);

    bool hasHeightForWidth() const;
    bool hasWidthForHeight() const;

    QSizePolicy sizePolicy;
    QGraphicsLayoutItem *parent;

    QSizeF *userSizeHints;
    mutable QSizeF cachedSizeHints[BobUI::NSizeHints];
    mutable QSizeF cachedConstraint;
    mutable QSizeF cachedSizeHintsWithConstraints[BobUI::NSizeHints];

    mutable quint32 sizeHintCacheDirty : 1;
    mutable quint32 sizeHintWithConstraintCacheDirty : 1;
    quint32 isLayout : 1;
    quint32 ownedByLayout : 1;

    QGraphicsLayoutItem *q_ptr;
    QRectF geom;
    QGraphicsItem *graphicsItem;
};

BOBUI_END_NAMESPACE

#endif //QGRAPHICSLAYOUTITEM_P_H

