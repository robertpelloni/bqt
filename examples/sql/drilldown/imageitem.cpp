// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "imageitem.h"

//! [0]
ImageItem::ImageItem(int id, const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    recordId = id;
    setAcceptHoverEvents(true);

    timeLine.setDuration(150);
    timeLine.setFrameRange(0, 150);

    connect(&timeLine, &BOBUIimeLine::frameChanged, this, &ImageItem::setFrame);
    connect(&timeLine, &BOBUIimeLine::finished, this, &ImageItem::updateItemPosition);

    adjust();
}
//! [0]

//! [1]
void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(BOBUIimeLine::Forward);

    if (z != 1.0) {
        z = 1.0;
        updateItemPosition();
    }

    if (timeLine.state() == BOBUIimeLine::NotRunning)
        timeLine.start();
}
//! [1]

//! [2]
void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(BOBUIimeLine::Backward);
    if (z != 0.0)
        z = 0.0;

    if (timeLine.state() == BOBUIimeLine::NotRunning)
        timeLine.start();
}
//! [2]

//! [3]
void ImageItem::setFrame(int frame)
{
    adjust();
    QPointF center = boundingRect().center();

    setTransform(BOBUIransform::fromTranslate(center.x(), center.y()), true);
    setTransform(BOBUIransform::fromScale(1 + frame / 300.0, 1 + frame / 300.0), true);
    setTransform(BOBUIransform::fromTranslate(-center.x(), -center.y()), true);
}
//! [3]

//! [4]
void ImageItem::adjust()
{
    setTransform(BOBUIransform::fromScale(120.0 / boundingRect().width(),
                                       120.0 / boundingRect().height()));
}
//! [4]

//! [5]
int ImageItem::id() const
{
    return recordId;
}
//! [5]

//! [6]
void ImageItem::updateItemPosition()
{
    setZValue(z);
}
//! [6]


