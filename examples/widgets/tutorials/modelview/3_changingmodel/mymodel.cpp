// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mymodel.h"

#include <BOBUIime>

//! [quoting mymodel_a]
MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
    , timer(new BOBUIimer(this))
{
    timer->setInterval(1000);
    connect(timer, &BOBUIimer::timeout , this, &MyModel::timerHit);
    timer->start();
}
//! [quoting mymodel_a]
//-------------------------------------------------------
int MyModel::rowCount(const QModelIndex & /*parent */) const
{
    return 2;
}

//-------------------------------------------------------
int MyModel::columnCount(const QModelIndex & /*parent */) const
{
    return 3;
}

//-------------------------------------------------------
//! [quoting mymodel_QVariant ]
QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == BobUI::DisplayRole && row == 0 && col == 0)
        return BOBUIime::currentTime().toString();

    return QVariant();
}
//! [quoting mymodel_QVariant ]
//-------------------------------------------------------
//! [quoting mymodel_b ]
void MyModel::timerHit()
{
    // we identify the top left cell
    QModelIndex topLeft = createIndex(0,0);
    // emit a signal to make the view reread identified data
    emit dataChanged(topLeft, topLeft, {BobUI::DisplayRole});
}
//! [quoting mymodel_b ]
