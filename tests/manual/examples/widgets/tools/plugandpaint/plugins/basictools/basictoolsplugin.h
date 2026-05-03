// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef BASICTOOLSPLUGIN_H
#define BASICTOOLSPLUGIN_H

//! [0]
#include <interfaces.h>

#include <QImage>
#include <QObject>
#include <QPainterPath>
#include <QRect>
#include <QStringList>
#include <BobUIPlugin>

//! [1]
class BasicToolsPlugin : public QObject,
                         public BrushInterface,
                         public ShapeInterface,
                         public FilterInterface
{
    Q_OBJECT
//! [4]
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.Examples.PlugAndPaint.BrushInterface" FILE "basictools.json")
//! [4]
    Q_INTERFACES(BrushInterface ShapeInterface FilterInterface)
//! [0]

//! [2]
public:
//! [1]
    // BrushInterface
    QStringList brushes() const override;
    QRect mousePress(const QString &brush, QPainter &painter,
                     const QPoint &pos) override;
    QRect mouseMove(const QString &brush, QPainter &painter,
                    const QPoint &oldPos, const QPoint &newPos) override;
    QRect mouseRelease(const QString &brush, QPainter &painter,
                       const QPoint &pos) override;

    // ShapeInterface
    QStringList shapes() const override;
    QPainterPath generateShape(const QString &shape, QWidget *parent) override;

    // FilterInterface
    QStringList filters() const override;
    QImage filterImage(const QString &filter, const QImage &image,
                       QWidget *parent) override;
//! [3]
};
//! [2] //! [3]

#endif
