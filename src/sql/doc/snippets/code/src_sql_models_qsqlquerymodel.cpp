// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QVariant>
#include "../sqldatabase/sqldatabase.cpp"

void MyModel::fetchModel()
{
MyModel *myModel = new MyModel;
//! [0]
while (myModel->canFetchMore())
    myModel->fetchMore();
//! [0]

//! [1]
QSqlQueryModel model;
model.setQuery("select * from MyTable");
if (model.lastError().isValid())
    qDebug() << model.lastError();
//! [1]
}
