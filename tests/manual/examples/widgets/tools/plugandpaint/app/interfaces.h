// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef INTERFACES_H
#define INTERFACES_H

#include <BobUIPlugin>

BOBUI_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QWidget;
class QPainterPath;
class QPoint;
class QRect;
class QString;
BOBUI_END_NAMESPACE

//! [0]
class BrushInterface
{
public:
    virtual ~BrushInterface() = default;

    virtual QStringList brushes() const = 0;
    virtual QRect mousePress(const QString &brush, QPainter &painter,
                             const QPoint &pos) = 0;
    virtual QRect mouseMove(const QString &brush, QPainter &painter,
                            const QPoint &oldPos, const QPoint &newPos) = 0;
    virtual QRect mouseRelease(const QString &brush, QPainter &painter,
                               const QPoint &pos) = 0;
};
//! [0]

//! [1]
class ShapeInterface
{
public:
    virtual ~ShapeInterface() = default;

    virtual QStringList shapes() const = 0;
    virtual QPainterPath generateShape(const QString &shape,
                                       QWidget *parent) = 0;
};
//! [1]

//! [2]
class FilterInterface
{
public:
    virtual ~FilterInterface() = default;

    virtual QStringList filters() const = 0;
    virtual QImage filterImage(const QString &filter, const QImage &image,
                               QWidget *parent) = 0;
};
//! [2]

BOBUI_BEGIN_NAMESPACE
//! [3] //! [4]
#define BrushInterface_iid "org.bobui-project.BobUI.Examples.PlugAndPaint.BrushInterface/1.0"

Q_DECLARE_INTERFACE(BrushInterface, BrushInterface_iid)
//! [3]

#define ShapeInterface_iid  "org.bobui-project.BobUI.Examples.PlugAndPaint.ShapeInterface/1.0"

Q_DECLARE_INTERFACE(ShapeInterface, ShapeInterface_iid)
//! [5]
#define FilterInterface_iid "org.bobui-project.BobUI.Examples.PlugAndPaint.FilterInterface/1.0"

Q_DECLARE_INTERFACE(FilterInterface, FilterInterface_iid)
//! [4] //! [5]
BOBUI_END_NAMESPACE

#endif
