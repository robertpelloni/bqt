// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "imageitem.h"
#include "gestures.h"

#include <QPainter>
#include <QEvent>

ImageItem::ImageItem(const QImage &image)
{
    setImage(image);
}

void ImageItem::setImage(const QImage &image)
{
    image_ = image;
    pixmap_ = QPixmap::fromImage(image.scaled(400, 400, BobUI::KeepAspectRatio));
    update();
}

QImage ImageItem::image() const
{
    return image_;
}

QRectF ImageItem::boundingRect() const
{
    const QSize size = pixmap_.size();
    return QRectF(0, 0, size.width(), size.height());
}

void ImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap_);
}


GestureImageItem::GestureImageItem(const QImage &image)
    : ImageItem(image)
{
    grabGesture(BobUI::PanGesture);
    grabGesture(ThreeFingerSlideGesture::Type);
}

bool GestureImageItem::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture) {
        qDebug("gestureimageitem: gesture triggered");
        return true;
    }
    return ImageItem::event(event);
}

#include "moc_imageitem.cpp"
