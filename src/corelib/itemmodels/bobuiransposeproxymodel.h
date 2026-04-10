// Copyright (C) 2018 Luca Beldi <v.ronin@yahoo.it>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIRANSPOSEPROXYMODEL_H
#define BOBUIRANSPOSEPROXYMODEL_H

#include <BobUICore/qabstractproxymodel.h>
#include <BobUICore/qscopedpointer.h>

BOBUI_REQUIRE_CONFIG(transposeproxymodel);

BOBUI_BEGIN_NAMESPACE

class BOBUIransposeProxyModelPrivate;

class Q_CORE_EXPORT BOBUIransposeProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
    Q_DISABLE_COPY(BOBUIransposeProxyModel)
    Q_DECLARE_PRIVATE(BOBUIransposeProxyModel)
public:
    explicit BOBUIransposeProxyModel(QObject* parent = nullptr);
    ~BOBUIransposeProxyModel();
    void setSourceModel(QAbstractItemModel* newSourceModel) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;
    bool setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value, int role = BobUI::EditRole) override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
    QSize span(const QModelIndex &index) const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) override;
    void sort(int column, BobUI::SortOrder order = BobUI::AscendingOrder) override;
protected:
    BOBUIransposeProxyModel(BOBUIransposeProxyModelPrivate &, QObject *parent);
};

BOBUI_END_NAMESPACE

#endif // BOBUIRANSPOSEPROXYMODEL_H
