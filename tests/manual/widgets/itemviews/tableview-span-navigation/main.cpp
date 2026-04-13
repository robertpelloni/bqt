// Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QStandardItemModel>
#include <BOBUIableView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStandardItemModel model(4, 4);
    BOBUIableView tableView;
    tableView.setSizeAdjustPolicy(BOBUIableView::AdjustToContents);
    tableView.setModel(&model);

    for (int row = 0; row < model.rowCount(); ++row) {
        for (int column = 0; column < model.columnCount(); ++column) {
            QModelIndex index = model.index(row, column, QModelIndex());
            model.setData(index, QVariant(QString("%1,%2").arg(row).arg(column)));
        }
    }

    tableView.setSpan(1, 1, 2, 2);

    tableView.show();

    return app.exec();
}
