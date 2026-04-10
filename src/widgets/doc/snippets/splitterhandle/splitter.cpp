// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>

#include "splitter.h"

SplitterHandle::SplitterHandle(BobUI::Orientation orientation, QSplitter *parent)
    : QSplitterHandle(orientation, parent)
{
    gradient.setColorAt(0.0, BobUI::darkGreen);
    gradient.setColorAt(0.25, BobUI::white);
    gradient.setColorAt(1.0, BobUI::darkGreen);
}

//! [0]
void SplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (orientation() == BobUI::Horizontal) {
        gradient.setStart(rect().left(), rect().height()/2);
        gradient.setFinalStop(rect().right(), rect().height()/2);
    } else {
        gradient.setStart(rect().width()/2, rect().top());
        gradient.setFinalStop(rect().width()/2, rect().bottom());
    }
    painter.fillRect(event->rect(), QBrush(gradient));
}
//! [0]

Splitter::Splitter(BobUI::Orientation orientation, QWidget *parent)
    : QSplitter(orientation, parent)
{
}

//! [1]
QSplitterHandle *Splitter::createHandle()
{
    return new SplitterHandle(orientation(), this);
}
//! [1]
