// Copyright (C) 2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIDENTITYPROXYMODEL_H
#define QIDENTITYPROXYMODEL_H

#include <BobUICore/qabstractproxymodel.h>

BOBUI_REQUIRE_CONFIG(identityproxymodel);

BOBUI_BEGIN_NAMESPACE


class QIdentityProxyModelPrivate;

class Q_CORE_EXPORT QIdentityProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit QIdentityProxyModel(QObject* parent = nullptr);
    ~QIdentityProxyModel();

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    using QObject::parent;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;
    bool dropMimeData(const QMimeData* data, BobUI::DropAction action, int row, int column, const QModelIndex& parent) override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;

    QItemSelection mapSelectionFromSource(const QItemSelection& selection) const override;
    QItemSelection mapSelectionToSource(const QItemSelection& selection) const override;
    QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits = 1, BobUI::MatchFlags flags = BobUI::MatchFlags(BobUI::MatchStartsWith|BobUI::MatchWrap)) const override;
    void setSourceModel(QAbstractItemModel* sourceModel) override;

    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) override;

    bool handleSourceLayoutChanges() const;
    bool handleSourceDataChanges() const;

protected:
    QIdentityProxyModel(QIdentityProxyModelPrivate &dd, QObject* parent);
    void setHandleSourceLayoutChanges(bool);
    void setHandleSourceDataChanges(bool);

private:
    Q_DECLARE_PRIVATE(QIdentityProxyModel)
    Q_DISABLE_COPY(QIdentityProxyModel)
};

BOBUI_END_NAMESPACE

#endif // QIDENTITYPROXYMODEL_H

