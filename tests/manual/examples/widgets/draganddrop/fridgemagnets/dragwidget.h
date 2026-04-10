// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
BOBUI_END_NAMESPACE

//! [0]
class DragWidget : public QWidget
{
public:
    explicit DragWidget(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
//! [0]

#endif // DRAGWIDGET_H
