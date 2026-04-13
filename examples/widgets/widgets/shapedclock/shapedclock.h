// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SHAPEDCLOCK_H
#define SHAPEDCLOCK_H

#include <QWidget>

//! [0]
class ShapedClock : public QWidget
{
    Q_OBJECT

public:
    ShapedClock(QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint dragPosition;
};
//! [0]

#endif
