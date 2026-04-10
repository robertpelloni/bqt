// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mymodel.h"

#include <QFont>
#include <QBrush>
#include <QDebug>

MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MyModel::rowCount(const QModelIndex & /*parent */) const
{
    return 2;
}

int MyModel::columnCount(const QModelIndex & /*parent */) const
{
    return 3;
}

//! [Quoting ModelView Tutorial]
// mymodel.cpp
QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    // generate a log message when this method gets called
    qDebug() << QString("row %1, col%2, role %3")
            .arg(row).arg(col).arg(role);

    switch (role) {
    case BobUI::DisplayRole:
        if (row == 0 && col == 1) return QString("<--left");
        if (row == 1 && col == 1) return QString("right-->");

        return QString("Row%1, Column%2")
                .arg(row + 1)
                .arg(col +1);
    case BobUI::FontRole:
        if (row == 0 && col == 0) { // change font only for cell(0,0)
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case BobUI::BackgroundRole:
        if (row == 1 && col == 2)  // change background only for cell(1,2)
            return QBrush(BobUI::red);
        break;
    case BobUI::TextAlignmentRole:
        if (row == 1 && col == 1) // change text alignment only for cell(1,1)
            return int(BobUI::AlignRight | BobUI::AlignVCenter);
        break;
    case BobUI::CheckStateRole:
        if (row == 1 && col == 0) // add a checkbox to cell(1,0)
            return BobUI::Checked;
        break;
    }
    return QVariant();
}
//! [Quoting ModelView Tutorial]
