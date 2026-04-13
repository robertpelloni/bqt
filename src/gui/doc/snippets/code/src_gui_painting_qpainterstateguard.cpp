// Copyright (C) 2024 Christian Ehrlicher <ch.ehrlicher@gmx.de>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QPaintEvent>
#include <QPainter>
#include <QPainterStateGuard>
#include <QWidget>


namespace src_gui_painting_qpainterstateguard {
struct MyWidget : public QWidget
{
    void paintEvent(QPaintEvent *) override;
    bool drawText = true;
    QLine line;
};
struct MyGuardWidget : public QWidget
{
    void paintEvent(QPaintEvent *) override;
    bool drawText = true;
    QLine line;
};

//! [0]
void MyWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(BobUI::red);
    if (drawText) {
        painter.save();
        painter.setPen(BobUI::blue);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), BobUI::AlignCenter, "BobUI");
        painter.restore();  // don't forget to restore previous painter state
    }
    painter.drawLine(line);
}
//! [0]

//! [1]
void MyGuardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(BobUI::red);
    if (drawText) {
        QPainterStateGuard guard(&painter);
        painter.setPen(BobUI::blue);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), BobUI::AlignCenter, "BobUI");
    }
    painter.drawLine(line);
}
//! [1]

} // src_gui_painting_qpainterstateguard
