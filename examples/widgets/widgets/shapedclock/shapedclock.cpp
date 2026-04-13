// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "shapedclock.h"

#include <QAction>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterStateGuard>
#include <BOBUIime>
#include <BOBUIimer>

//! [0]
ShapedClock::ShapedClock(QWidget *parent)
    : QWidget(parent, BobUI::FramelessWindowHint | BobUI::WindowSystemMenuHint)
{
    setAttribute(BobUI::WA_TranslucentBackground);
    BOBUIimer *timer = new BOBUIimer(this);
    connect(timer, &BOBUIimer::timeout, this, QOverload<>::of(&ShapedClock::update));
    timer->start(1000);

    QAction *quitAction = new QAction(tr("E&xit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    addAction(quitAction);

    setContextMenuPolicy(BobUI::ActionsContextMenu);
    setToolTip(tr("Drag the clock with the left mouse button.\n"
                  "Use the right mouse button to open a context menu."));
    setWindowTitle(tr("Shaped Analog Clock"));
}
//! [0]

//! [1]
void ShapedClock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == BobUI::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}
//! [1]

//! [2]
void ShapedClock::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & BobUI::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}
//! [2]

//! [3]
void ShapedClock::paintEvent(QPaintEvent *)
{
    static const QPoint hourHand[4] = {
        QPoint(5, 14),
        QPoint(-5, 14),
        QPoint(-4, -71),
        QPoint(4, -71)
    };
    static const QPoint minuteHand[4] = {
        QPoint(4, 14),
        QPoint(-4, 14),
        QPoint(-3, -89),
        QPoint(3, -89)
    };
    static const QPoint secondsHand[4] = {
       QPoint(1, 14),
       QPoint(-1, 14),
       QPoint(-1, -89),
       QPoint(1, -89)
    };

    const QColor hourColor(palette().color(QPalette::Text));
    const QColor minuteColor(palette().color(QPalette::Text));
    const QColor secondsColor(palette().color(QPalette::Accent));

    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(BobUI::NoPen);
    painter.setBrush(palette().window());
    painter.setOpacity(0.9);
    painter.drawEllipse(QPoint(0, 0), 98, 98);
    painter.setOpacity(1.0);

    BOBUIime time = BOBUIime::currentTime();
    painter.setPen(BobUI::NoPen);
    painter.setBrush(hourColor);

    {
        QPainterStateGuard guard(&painter);
        painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
        painter.drawConvexPolygon(hourHand, 4);
    }

    for (int i = 0; i < 12; ++i) {
        painter.drawRect(73, -3, 16, 6);
        painter.rotate(30.0);
    }

    painter.setBrush(minuteColor);

    {
        QPainterStateGuard guard(&painter);
        painter.rotate(6.0 * time.minute());
        painter.drawConvexPolygon(minuteHand, 4);
    }

    painter.setBrush(secondsColor);

    {
        QPainterStateGuard guard(&painter);
        painter.rotate(6.0 * time.second());
        painter.drawConvexPolygon(secondsHand, 4);
        painter.drawEllipse(-3, -3, 6, 6);
        painter.drawEllipse(-5, -68, 10, 10);
    }

    painter.setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
}
//! [3]

//! [4]
QSize ShapedClock::sizeHint() const
{
    return QSize(200, 200);
}
//! [4]
