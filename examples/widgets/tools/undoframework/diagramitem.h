// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPolygonItem>

BOBUI_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
BOBUI_END_NAMESPACE

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 1 };
    enum DiagramType { Box, Triangle };

    explicit DiagramItem(DiagramType diagramType, QGraphicsItem *item = nullptr);

    DiagramType diagramType() const
    {
        return polygon() == boxPolygon ? Box : Triangle;
    }
    int type() const override { return Type; }

private:
    QPolygonF boxPolygon;
    QPolygonF trianglePolygon;
};

#endif
