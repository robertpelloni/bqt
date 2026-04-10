// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mousestatwidget.h"

#include <BOBUIabletEvent>
#include <QPainter>
#include <BOBUIextOption>
#include <BOBUIest>

MouseStatWidget::MouseStatWidget(bool acceptTabletEvent):acceptTabletEvent(acceptTabletEvent),
    receivedMouseEventCount(0),
    receivedMouseEventCountToPaint(0),
    receivedTabletEventCount(0),
    receivedTabletEventCountToPaint(0)
{
    startTimer(1000);
}


void MouseStatWidget::tabletEvent(BOBUIabletEvent *event)
{
    ++receivedTabletEventCount;
    if (acceptTabletEvent)
        event->accept();
    else
        event->ignore();
    // make sure the event loop is slow
    BOBUIest::qSleep(15);
}

void MouseStatWidget::mouseMoveEvent(QMouseEvent *)
{
    ++receivedMouseEventCount;
}

void MouseStatWidget::timerEvent(BOBUIimerEvent *)
{
    receivedMouseEventCountToPaint = receivedMouseEventCount;
    receivedTabletEventCountToPaint = receivedTabletEventCount;
    receivedMouseEventCount = 0;
    receivedTabletEventCount = 0;
    update();
}

void MouseStatWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(BobUI::black);
    painter.drawRect(rect());
    QStringList text;
    text << ((acceptTabletEvent) ? " - tablet events accepted - " : " - tablet events ignored - ");
    text << QString("Number of tablet events received in the last second: %1").arg(QString::number(receivedTabletEventCountToPaint));
    text << QString("Number of mouse events received in the last second: %1").arg(QString::number(receivedMouseEventCountToPaint));

    BOBUIextOption textOption(BobUI::AlignCenter);
    textOption.setWrapMode(BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    painter.drawText(rect(),
                     text.join("\n"),
                     textOption);
}
