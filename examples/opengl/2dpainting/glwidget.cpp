// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "helper.h"

#include <QPainter>
#include <BOBUIimer>

//! [0]
GLWidget::GLWidget(Helper *helper, QWidget *parent)
    : QOpenGLWidget(parent), helper(helper)
{
    elapsed = 0;
    setFixedSize(200, 200);
    setAutoFillBackground(false);
}
//! [0]

//! [1]
void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<BOBUIimer*>(sender())->interval()) % 1000;
    update();
}
//! [1]

//! [2]
void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper->paint(&painter, event, elapsed);
    painter.end();
}
//! [2]
