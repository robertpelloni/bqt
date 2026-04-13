// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTRINGLISTMODEL_H
#define QSTRINGLISTMODEL_H

#include <BobUICore/qabstractitemmodel.h>
#include <BobUICore/qstringlist.h>

BOBUI_REQUIRE_CONFIG(stringlistmodel);

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QStringListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QStringListModel(QObject *parent = nullptr);
    explicit QStringListModel(const QStringList &strings, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole) override;
    bool clearItemData(const QModelIndex &index) override;

    BobUI::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;

    void sort(int column, BobUI::SortOrder order = BobUI::AscendingOrder) override;

    QStringList stringList() const;
    void setStringList(const QStringList &strings);

    BobUI::DropActions supportedDropActions() const override;

private:
    Q_DISABLE_COPY(QStringListModel)
    QStringList lst;
};

BOBUI_END_NAMESPACE

#endif // QSTRINGLISTMODEL_H
