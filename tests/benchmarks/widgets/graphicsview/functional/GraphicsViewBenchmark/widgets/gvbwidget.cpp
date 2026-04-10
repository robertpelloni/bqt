// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui>
#include "gvbwidget.h"

GvbWidget::GvbWidget(QGraphicsItem * parent, BobUI::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags)
{

}

GvbWidget::~GvbWidget()
{
}

void GvbWidget::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

