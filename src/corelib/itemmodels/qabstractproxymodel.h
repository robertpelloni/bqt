// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTPROXYMODEL_H
#define QABSTRACTPROXYMODEL_H

#include <BobUICore/qabstractitemmodel.h>

BOBUI_REQUIRE_CONFIG(proxymodel);

BOBUI_BEGIN_NAMESPACE

class QAbstractProxyModelPrivate;
class QItemSelection;

class Q_CORE_EXPORT QAbstractProxyModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *sourceModel READ sourceModel WRITE setSourceModel
               NOTIFY sourceModelChanged BINDABLE bindableSourceModel)

public:
    explicit QAbstractProxyModel(QObject *parent = nullptr);
    ~QAbstractProxyModel();

    virtual void setSourceModel(QAbstractItemModel *sourceModel);
    QAbstractItemModel *sourceModel() const;
    QBindable<QAbstractItemModel *> bindableSourceModel();

    Q_INVOKABLE virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const = 0;
    Q_INVOKABLE virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const = 0;

    Q_INVOKABLE virtual QItemSelection mapSelectionToSource(const QItemSelection &selection) const;
    Q_INVOKABLE virtual QItemSelection mapSelectionFromSource(const QItemSelection &selection) const;

    bool submit() override;
    void revert() override;

    QVariant data(const QModelIndex &proxyIndex, int role = BobUI::DisplayRole) const override;
    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    BobUI::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole) override;
    bool setItemData(const QModelIndex& index, const QMap<int, QVariant> &roles) override;
    bool setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value, int role = BobUI::EditRole) override;
    bool clearItemData(const QModelIndex &index) override;

    QModelIndex buddy(const QModelIndex &index) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    void sort(int column, BobUI::SortOrder order = BobUI::AscendingOrder) override;
    QSize span(const QModelIndex &index) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;

    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    bool canDropMimeData(const QMimeData *data, BobUI::DropAction action,
                         int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, BobUI::DropAction action,
                      int row, int column, const QModelIndex &parent) override;
    QStringList mimeTypes() const override;
    BobUI::DropActions supportedDragActions() const override;
    BobUI::DropActions supportedDropActions() const override;
    QHash<int, QByteArray> roleNames() const override;

Q_SIGNALS:
    void sourceModelChanged(QPrivateSignal);

protected:
    QModelIndex createSourceIndex(int row, int col, void *internalPtr) const;
    QAbstractProxyModel(QAbstractProxyModelPrivate &, QObject *parent);

private:
    Q_DECLARE_PRIVATE(QAbstractProxyModel)
    Q_DISABLE_COPY(QAbstractProxyModel)
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelRowsAboutToBeInserted(QModelIndex, int, int))
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelRowsInserted(QModelIndex, int, int))
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelRowsRemoved(QModelIndex, int, int))
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelColumnsAboutToBeInserted(QModelIndex, int, int))
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelColumnsInserted(QModelIndex, int, int))
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelColumnsRemoved(QModelIndex, int, int))
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTPROXYMODEL_H
