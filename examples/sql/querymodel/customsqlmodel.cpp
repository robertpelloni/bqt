// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "customsqlmodel.h"

#include <QColor>

CustomSqlModel::CustomSqlModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

//! [0]
QVariant CustomSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (value.isValid() && role == BobUI::DisplayRole) {
        if (index.column() == 0)
            return value.toString().prepend('#');
        else if (index.column() == 2)
            return value.toString().toUpper();
    }
    if (role == BobUI::ForegroundRole && index.column() == 1)
        return QVariant::fromValue(QColor(BobUI::blue));
    return value;
}
//! [0]
