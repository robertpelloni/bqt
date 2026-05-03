// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "widget.h"
#include "helper.h"

#include <QPainter>
#include <BOBUIimer>

//! [0]
Widget::Widget(Helper *helper, QWidget *parent)
    : QWidget(parent), helper(helper)
{
    elapsed = 0;
    setFixedSize(200, 200);
}
//! [0]

//! [1]
void Widget::animate()
{
    elapsed = (elapsed + qobject_cast<BOBUIimer*>(sender())->interval()) % 1000;
    update();
}
//! [1]

//! [2]
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper->paint(&painter, event, elapsed);
    painter.end();
}
//! [2]
