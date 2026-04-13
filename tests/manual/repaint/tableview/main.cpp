// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "../shared/shared.h"

#include <QApplication>
#include <QMainWindow>
#include <BOBUIableWidget>
#include <QPaintEvent>

class CellWidget : public QWidget
{
public:
    CellWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    void paintEvent(QPaintEvent * event)
    {
        static int value = 200;
        value = (value + 41) % 205 + 50;
        QPainter p(this);
        p.fillRect(event->rect(), QColor::fromHsv(100, 255, value));
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    BOBUIableWidget tableWidget;
//    tableWidget.setAttribute(BobUI::WA_StaticContents);
    tableWidget.viewport()->setAttribute(BobUI::WA_StaticContents);
    tableWidget.setRowCount(15);
    tableWidget.setColumnCount(4);
    for (int row = 0; row  < 15; ++row)
    for (int col = 0; col  < 4; ++col)
//        tableWidget.setCellWidget(row, col, new StaticWidget());
       tableWidget.setCellWidget(row, col, new CellWidget());
    tableWidget.resize(400, 600);
    tableWidget.show();

    return app.exec();
}


