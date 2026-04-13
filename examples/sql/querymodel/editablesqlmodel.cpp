// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "editablesqlmodel.h"

#include <QSqlQuery>

EditableSqlModel::EditableSqlModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

//! [0]
BobUI::ItemFlags EditableSqlModel::flags(const QModelIndex &index) const
{
    BobUI::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1 || index.column() == 2)
        flags |= BobUI::ItemIsEditable;
    return flags;
}
//! [0]

//! [1]
bool EditableSqlModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() < 1 || index.column() > 2)
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt();

    clear();

    bool ok;
    if (index.column() == 1)
        ok = setFirstName(id, value.toString());
    else
        ok = setLastName(id, value.toString());
    refresh();
    return ok;
}
//! [1]

void EditableSqlModel::refresh()
{
    setQuery("select * from person");
    setHeaderData(0, BobUI::Horizontal, QObject::tr("ID"));
    setHeaderData(1, BobUI::Horizontal, QObject::tr("First name"));
    setHeaderData(2, BobUI::Horizontal, QObject::tr("Last name"));
}

//! [2]
bool EditableSqlModel::setFirstName(int personId, const QString &firstName)
{
    QSqlQuery query;
    query.prepare("update person set firstname = ? where id = ?");
    query.addBindValue(firstName);
    query.addBindValue(personId);
    return query.exec();
}
//! [2]

bool EditableSqlModel::setLastName(int personId, const QString &lastName)
{
    QSqlQuery query;
    query.prepare("update person set lastname = ? where id = ?");
    query.addBindValue(lastName);
    query.addBindValue(personId);
    return query.exec();
}
