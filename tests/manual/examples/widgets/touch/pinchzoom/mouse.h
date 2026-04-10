// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsObject>

//! [0]
class Mouse : public QGraphicsObject
{
    Q_OBJECT

public:
    Mouse();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected:
    void timerEvent(BOBUIimerEvent *event) override;

private:
    qreal angle = 0;
    qreal speed = 0;
    qreal mouseEyeDirection = 0;
    QColor color;
};
//! [0]

#endif
