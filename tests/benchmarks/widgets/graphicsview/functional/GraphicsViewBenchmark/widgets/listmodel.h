// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>

class RecycledListItem;
class ListItemCache;

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    ListModel(QObject *parent = nullptr);
    ~ListModel();

public:

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole ) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    void insert(const int row, RecycledListItem *item);
    void appendRow(RecycledListItem *item);

    void clear();

    RecycledListItem *item(const int row) const;

    RecycledListItem *takeItem(const int row);

private:
    Q_DISABLE_COPY(ListModel)
    QList<RecycledListItem *> m_items;
};

#endif // LISTMODEL_H
