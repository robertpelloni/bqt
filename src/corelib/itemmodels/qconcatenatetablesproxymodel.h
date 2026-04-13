// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCONCATENATEROWSPROXYMODEL_H
#define QCONCATENATEROWSPROXYMODEL_H

#include <BobUICore/qabstractitemmodel.h>

BOBUI_REQUIRE_CONFIG(concatenatetablesproxymodel);

BOBUI_BEGIN_NAMESPACE

class QConcatenateTablesProxyModelPrivate;

class Q_CORE_EXPORT QConcatenateTablesProxyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit QConcatenateTablesProxyModel(QObject *parent = nullptr);
    ~QConcatenateTablesProxyModel();

    QList<QAbstractItemModel *> sourceModels() const;
    Q_SCRIPTABLE void addSourceModel(QAbstractItemModel *sourceModel);
    Q_SCRIPTABLE void removeSourceModel(QAbstractItemModel *sourceModel);

    Q_INVOKABLE Q_REVISION(6, 11) QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    Q_INVOKABLE Q_REVISION(6, 11) QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole) override;
    QMap<int, QVariant> itemData(const QModelIndex &proxyIndex) const override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
    BobUI::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool canDropMimeData(const QMimeData *data, BobUI::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, BobUI::DropAction action, int row, int column, const QModelIndex &parent) override;
    QSize span(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    Q_DECLARE_PRIVATE(QConcatenateTablesProxyModel)
    Q_DISABLE_COPY(QConcatenateTablesProxyModel)
};

BOBUI_END_NAMESPACE

#endif // QCONCATENATEROWSPROXYMODEL_H
