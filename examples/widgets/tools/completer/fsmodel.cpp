// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "fsmodel.h"

//! [0]
FileSystemModel::FileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
}
//! [0]

//! [1]
QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == BobUI::DisplayRole && index.column() == 0) {
        QString path  = QDir::toNativeSeparators(filePath(index));
        if (path.endsWith(QDir::separator()))
            path.chop(1);
        return path;
    }

    return QFileSystemModel::data(index, role);
}

//! [1]
