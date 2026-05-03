// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef BUBBLE_H
#define BUBBLE_H

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>

BOBUI_FORWARD_DECLARE_CLASS(QPainter)

class Bubble
{
public:
    Bubble(const QPointF &position, qreal radius, const QPointF &velocity);
    ~Bubble();

    void drawBubble(QPainter *painter);
    void updateBrush();
    void move(const QRect &bbox);
    void updateCache();
    QRectF rect();

private:
    QColor randomColor();

    QBrush brush;
    QPointF position;
    QPointF vel;
    qreal radius;
    QColor innerColor;
    QColor outerColor;
    QImage *cache = nullptr;
};

#endif
