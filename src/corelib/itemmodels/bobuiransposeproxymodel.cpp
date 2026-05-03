// Copyright (C) 2018 Luca Beldi <v.ronin@yahoo.it>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuiransposeproxymodel.h"
#include <private/bobuiransposeproxymodel_p.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qsize.h>
#include <BobUICore/qmap.h>

BOBUI_BEGIN_NAMESPACE

QModelIndex BOBUIransposeProxyModelPrivate::uncheckedMapToSource(const QModelIndex &proxyIndex) const
{
    if (!model || !proxyIndex.isValid())
        return QModelIndex();
    Q_Q(const BOBUIransposeProxyModel);
    return q->createSourceIndex(proxyIndex.column(), proxyIndex.row(), proxyIndex.internalPointer());
}

QModelIndex BOBUIransposeProxyModelPrivate::uncheckedMapFromSource(const QModelIndex &sourceIndex) const
{
    if (!model || !sourceIndex.isValid())
        return QModelIndex();
    Q_Q(const BOBUIransposeProxyModel);
    return q->createIndex(sourceIndex.column(), sourceIndex.row(), sourceIndex.internalPointer());
}

void BOBUIransposeProxyModelPrivate::onLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    Q_Q(BOBUIransposeProxyModel);
    Q_ASSERT(layoutChangeProxyIndexes.size() == layoutChangePersistentIndexes.size());
    QModelIndexList toList;
    toList.reserve(layoutChangePersistentIndexes.size());
    for (const QPersistentModelIndex &persistIdx : std::as_const(layoutChangePersistentIndexes))
        toList << q->mapFromSource(persistIdx);
    q->changePersistentIndexList(layoutChangeProxyIndexes, toList);
    layoutChangeProxyIndexes.clear();
    layoutChangePersistentIndexes.clear();
    QList<QPersistentModelIndex> proxyParents;
    proxyParents.reserve(parents.size());
    for (const QPersistentModelIndex &srcParent : parents)
        proxyParents << q->mapFromSource(srcParent);
    QAbstractItemModel::LayoutChangeHint proxyHint = QAbstractItemModel::NoLayoutChangeHint;
    if (hint == QAbstractItemModel::VerticalSortHint)
        proxyHint = QAbstractItemModel::HorizontalSortHint;
    else if (hint == QAbstractItemModel::HorizontalSortHint)
        proxyHint = QAbstractItemModel::VerticalSortHint;
    emit q->layoutChanged(proxyParents, proxyHint);
}

void BOBUIransposeProxyModelPrivate::onLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &sourceParents, QAbstractItemModel::LayoutChangeHint hint)
{
    Q_Q(BOBUIransposeProxyModel);
    QList<QPersistentModelIndex> proxyParents;
    proxyParents.reserve(sourceParents.size());
    for (const QPersistentModelIndex &parent : sourceParents) {
        if (!parent.isValid()) {
            proxyParents << QPersistentModelIndex();
            continue;
        }
        const QModelIndex mappedParent = q->mapFromSource(parent);
        Q_ASSERT(mappedParent.isValid());
        proxyParents << mappedParent;
    }
    QAbstractItemModel::LayoutChangeHint proxyHint = QAbstractItemModel::NoLayoutChangeHint;
    if (hint == QAbstractItemModel::VerticalSortHint)
        proxyHint = QAbstractItemModel::HorizontalSortHint;
    else if (hint == QAbstractItemModel::HorizontalSortHint)
        proxyHint = QAbstractItemModel::VerticalSortHint;
    emit q->layoutAboutToBeChanged(proxyParents, proxyHint);
    const QModelIndexList proxyPersistentIndexes = q->persistentIndexList();
    layoutChangeProxyIndexes.clear();
    layoutChangePersistentIndexes.clear();
    layoutChangeProxyIndexes.reserve(proxyPersistentIndexes.size());
    layoutChangePersistentIndexes.reserve(proxyPersistentIndexes.size());
    for (const QModelIndex &proxyPersistentIndex : proxyPersistentIndexes) {
        layoutChangeProxyIndexes << proxyPersistentIndex;
        Q_ASSERT(proxyPersistentIndex.isValid());
        const QPersistentModelIndex srcPersistentIndex = q->mapToSource(proxyPersistentIndex);
        Q_ASSERT(srcPersistentIndex.isValid());
        layoutChangePersistentIndexes << srcPersistentIndex;
    }
}

void BOBUIransposeProxyModelPrivate::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                                const QList<int> &roles)
{
    Q_Q(BOBUIransposeProxyModel);
    emit q->dataChanged(q->mapFromSource(topLeft), q->mapFromSource(bottomRight), roles);
}

void BOBUIransposeProxyModelPrivate::onHeaderDataChanged(BobUI::Orientation orientation, int first, int last)
{
    Q_Q(BOBUIransposeProxyModel);
    emit q->headerDataChanged(orientation == BobUI::Horizontal ? BobUI::Vertical : BobUI::Horizontal, first, last);
}

void BOBUIransposeProxyModelPrivate::onColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(BOBUIransposeProxyModel);
    q->beginInsertRows(q->mapFromSource(parent), first, last);
}

void BOBUIransposeProxyModelPrivate::onColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_Q(BOBUIransposeProxyModel);
    q->beginRemoveRows(q->mapFromSource(parent), first, last);
}

void BOBUIransposeProxyModelPrivate::onColumnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn)
{
    Q_Q(BOBUIransposeProxyModel);
    q->beginMoveRows(q->mapFromSource(sourceParent), sourceStart, sourceEnd, q->mapFromSource(destinationParent), destinationColumn);
}

void BOBUIransposeProxyModelPrivate::onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(BOBUIransposeProxyModel);
    q->beginInsertColumns(q->mapFromSource(parent), first, last);
}

void BOBUIransposeProxyModelPrivate::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_Q(BOBUIransposeProxyModel);
    q->beginRemoveColumns(q->mapFromSource(parent), first, last);
}

void BOBUIransposeProxyModelPrivate::onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    Q_Q(BOBUIransposeProxyModel);
    q->beginMoveColumns(q->mapFromSource(sourceParent), sourceStart, sourceEnd, q->mapFromSource(destinationParent), destinationRow);
}

/*!
    \since 5.13
    \class BOBUIransposeProxyModel
    \inmodule BobUICore
    \brief This proxy transposes the source model.

    This model will make the rows of the source model become columns of the proxy model and vice-versa.

    If the model is a tree, the parents will be transposed as well. For example, if an index in the source model had parent `index(2,0)`, it will have parent `index(0,2)` in the proxy.
*/

/*!
    Constructs a new proxy model with the given \a parent.
*/
BOBUIransposeProxyModel::BOBUIransposeProxyModel(QObject* parent)
    : QAbstractProxyModel(*new BOBUIransposeProxyModelPrivate, parent)
{}

/*!
    Destructs the proxy model.
*/
BOBUIransposeProxyModel::~BOBUIransposeProxyModel() = default;

/*!
    \internal
*/
BOBUIransposeProxyModel::BOBUIransposeProxyModel(BOBUIransposeProxyModelPrivate &dd, QObject *parent)
    : QAbstractProxyModel(dd, parent)
{}

/*!
    \reimp
*/
void BOBUIransposeProxyModel::setSourceModel(QAbstractItemModel* newSourceModel)
{
    Q_D(BOBUIransposeProxyModel);
    if (newSourceModel == d->model)
        return;
    beginResetModel();
    if (d->model) {
        for (const QMetaObject::Connection& discIter : std::as_const(d->sourceConnections))
            disconnect(discIter);
    }
    d->sourceConnections.clear();
    QAbstractProxyModel::setSourceModel(newSourceModel);
    if (d->model) {
        using namespace std::placeholders;
        d->sourceConnections = QList<QMetaObject::Connection>{
            connect(d->model, &QAbstractItemModel::modelAboutToBeReset, this, &BOBUIransposeProxyModel::beginResetModel),
            connect(d->model, &QAbstractItemModel::modelReset, this, &BOBUIransposeProxyModel::endResetModel),
            connect(d->model, &QAbstractItemModel::dataChanged, this, std::bind(&BOBUIransposeProxyModelPrivate::onDataChanged, d, _1, _2, _3)),
            connect(d->model, &QAbstractItemModel::headerDataChanged, this, std::bind(&BOBUIransposeProxyModelPrivate::onHeaderDataChanged, d, _1, _2, _3)),
            connect(d->model, &QAbstractItemModel::columnsAboutToBeInserted, this, std::bind(&BOBUIransposeProxyModelPrivate::onColumnsAboutToBeInserted, d, _1, _2, _3)),
            connect(d->model, &QAbstractItemModel::columnsAboutToBeMoved, this, std::bind(&BOBUIransposeProxyModelPrivate::onColumnsAboutToBeMoved, d, _1, _2, _3, _4, _5)),
            connect(d->model, &QAbstractItemModel::columnsAboutToBeRemoved, this, std::bind(&BOBUIransposeProxyModelPrivate::onColumnsAboutToBeRemoved, d, _1, _2, _3)),
            connect(d->model, &QAbstractItemModel::columnsInserted, this, &BOBUIransposeProxyModel::endInsertRows),
            connect(d->model, &QAbstractItemModel::columnsRemoved, this, &BOBUIransposeProxyModel::endRemoveRows),
            connect(d->model, &QAbstractItemModel::columnsMoved, this, &BOBUIransposeProxyModel::endMoveRows),
            connect(d->model, &QAbstractItemModel::rowsAboutToBeInserted, this, std::bind(&BOBUIransposeProxyModelPrivate::onRowsAboutToBeInserted, d, _1, _2, _3)),
            connect(d->model, &QAbstractItemModel::rowsAboutToBeMoved, this, std::bind(&BOBUIransposeProxyModelPrivate::onRowsAboutToBeMoved, d, _1, _2, _3, _4, _5)),
            connect(d->model, &QAbstractItemModel::rowsAboutToBeRemoved, this, std::bind(&BOBUIransposeProxyModelPrivate::onRowsAboutToBeRemoved, d, _1, _2, _3)),
            connect(d->model, &QAbstractItemModel::rowsInserted, this, &BOBUIransposeProxyModel::endInsertColumns),
            connect(d->model, &QAbstractItemModel::rowsRemoved, this, &BOBUIransposeProxyModel::endRemoveColumns),
            connect(d->model, &QAbstractItemModel::rowsMoved, this, &BOBUIransposeProxyModel::endMoveColumns),
            connect(d->model, &QAbstractItemModel::layoutAboutToBeChanged, this, std::bind(&BOBUIransposeProxyModelPrivate::onLayoutAboutToBeChanged, d, _1, _2)),
            connect(d->model, &QAbstractItemModel::layoutChanged, this, std::bind(&BOBUIransposeProxyModelPrivate::onLayoutChanged, d, _1, _2))
        };
    }
    endResetModel();
}

/*!
    \reimp
*/
int BOBUIransposeProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const BOBUIransposeProxyModel);
    if (!d->model)
        return 0;
    Q_ASSERT(checkIndex(parent));
    return d->model->columnCount(mapToSource(parent));
}

/*!
    \reimp
*/
int BOBUIransposeProxyModel::columnCount(const QModelIndex &parent) const
{
    Q_D(const BOBUIransposeProxyModel);
    if (!d->model)
        return 0;
    Q_ASSERT(checkIndex(parent));
    return d->model->rowCount(mapToSource(parent));
}

/*!
    \reimp
*/
QVariant BOBUIransposeProxyModel::headerData(int section, BobUI::Orientation orientation, int role) const
{
    Q_D(const BOBUIransposeProxyModel);
    if (!d->model)
        return QVariant();
    return d->model->headerData(section, orientation == BobUI::Horizontal ? BobUI::Vertical : BobUI::Horizontal, role);
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value, int role)
{
    Q_D(BOBUIransposeProxyModel);
    if (!d->model)
        return false;
    return d->model->setHeaderData(section, orientation == BobUI::Horizontal ? BobUI::Vertical : BobUI::Horizontal, value, role);
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(index));
    if (!d->model || !index.isValid())
        return false;
    return d->model->setItemData(mapToSource(index), roles);
}

/*!
    \reimp
*/
QSize BOBUIransposeProxyModel::span(const QModelIndex &index) const
{
    Q_D(const BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(index));
    if (!d->model || !index.isValid())
        return QSize();
    return d->model->span(mapToSource(index)).transposed();
}

/*!
    \reimp
*/
QMap<int, QVariant> BOBUIransposeProxyModel::itemData(const QModelIndex &index) const
{
    Q_D(const BOBUIransposeProxyModel);
    if (!d->model)
        return QMap<int, QVariant>();
    Q_ASSERT(checkIndex(index));
    return d->model->itemData(mapToSource(index));
}

/*!
    \reimp
*/
QModelIndex BOBUIransposeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    Q_D(const BOBUIransposeProxyModel);
    if (!d->model || !sourceIndex.isValid())
        return QModelIndex();
    Q_ASSERT(d->model->checkIndex(sourceIndex));
    return d->uncheckedMapFromSource(sourceIndex);
}

/*!
    \reimp
*/
QModelIndex BOBUIransposeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    Q_D(const BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(proxyIndex));
    if (!d->model || !proxyIndex.isValid())
        return QModelIndex();
    return d->uncheckedMapToSource(proxyIndex);
}

/*!
    \reimp
*/
QModelIndex BOBUIransposeProxyModel::parent(const QModelIndex &index) const
{
    Q_D(const BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(index, CheckIndexOption::DoNotUseParent));
    if (!d->model || !index.isValid())
        return QModelIndex();
    return d->uncheckedMapFromSource(d->uncheckedMapToSource(index).parent());
}

/*!
    \reimp
*/
QModelIndex BOBUIransposeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(parent));
    if (!d->model)
        return QModelIndex();
    return mapFromSource(d->model->index(column, row, mapToSource(parent)));
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(parent));
    if (!d->model)
        return false;
    return d->model->insertColumns(row, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(parent));
    if (!d->model)
        return false;
    return d->model->removeColumns(row, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(sourceParent));
    Q_ASSERT(checkIndex(destinationParent));
    if (!d->model)
        return false;
    return d->model->moveColumns(mapToSource(sourceParent), sourceRow, count, mapToSource(destinationParent), destinationChild);
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(parent));
    if (!d->model)
        return false;
    return d->model->insertRows(column, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(parent));
    if (!d->model)
        return false;
    return d->model->removeRows(column, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool BOBUIransposeProxyModel::moveColumns(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    Q_D(BOBUIransposeProxyModel);
    Q_ASSERT(checkIndex(sourceParent));
    Q_ASSERT(checkIndex(destinationParent));
    if (!d->model)
        return false;
    return d->model->moveRows(mapToSource(sourceParent), sourceRow, count, mapToSource(destinationParent), destinationChild);
}

/*!
    \reimp
    This method will perform no action. Use a QSortFilterProxyModel on top of this one if you require sorting.
*/
void BOBUIransposeProxyModel::sort(int column, BobUI::SortOrder order)
{
    Q_UNUSED(column);
    Q_UNUSED(order);
    return;
}

BOBUI_END_NAMESPACE

#include "moc_bobuiransposeproxymodel.cpp"
