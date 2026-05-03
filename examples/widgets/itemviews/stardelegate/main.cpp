// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <BOBUIableWidget>

#include "stardelegate.h"
#include "stareditor.h"
#include "starrating.h"

//! [0]
void populateTableWidget(BOBUIableWidget *tableWidget)
{
    static constexpr struct {
        const char *title;
        const char *genre;
        const char *artist;
        int rating;
    } staticData[] = {
//! [0] //! [1]
        { "Mass in B-Minor", "Baroque", "J.S. Bach", 5 },
//! [1]
        { "Three More Foxes", "Jazz", "Maynard Ferguson", 4 },
        { "Sex Bomb", "Pop", "Tom Jones", 3 },
        { "Barbie Girl", "Pop", "Aqua", 5 },
//! [2]
        { nullptr, nullptr, nullptr, 0 }
//! [2] //! [3]
    };
//! [3] //! [4]

    for (int row = 0; staticData[row].title != nullptr; ++row) {
        BOBUIableWidgetItem *item0 = new BOBUIableWidgetItem(staticData[row].title);
        BOBUIableWidgetItem *item1 = new BOBUIableWidgetItem(staticData[row].genre);
        BOBUIableWidgetItem *item2 = new BOBUIableWidgetItem(staticData[row].artist);
        BOBUIableWidgetItem *item3 = new BOBUIableWidgetItem;
        item3->setData(0,
                       QVariant::fromValue(StarRating(staticData[row].rating)));

        tableWidget->setItem(row, 0, item0);
        tableWidget->setItem(row, 1, item1);
        tableWidget->setItem(row, 2, item2);
        tableWidget->setItem(row, 3, item3);
    }
}
//! [4]

//! [5]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BOBUIableWidget tableWidget(4, 4);
    tableWidget.setItemDelegate(new StarDelegate);
    tableWidget.setEditTriggers(QAbstractItemView::DoubleClicked
                                | QAbstractItemView::SelectedClicked);
    tableWidget.setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget.setHorizontalHeaderLabels({"Title", "Genre", "Artist", "Rating"});

    populateTableWidget(&tableWidget);

    tableWidget.resizeColumnsToContents();
    tableWidget.resize(500, 300);
    tableWidget.show();

    return app.exec();
}
//! [5]
