// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuireewidget.h"

#include <qheaderview.h>
#include <qpainter.h>
#include <qstack.h>
#include <qdebug.h>
#include <private/bobuireewidget_p.h>
#include <private/qwidgetitemdata_p.h>
#include <private/bobuireewidgetitemiterator_p.h>

#include <BobUICore/private/qduplicatetracker_p.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

class BOBUIreeModelLessThan
{
public:
    inline bool operator()(BOBUIreeWidgetItem *i1, BOBUIreeWidgetItem *i2) const
        { return *i1 < *i2; }
};

class BOBUIreeModelGreaterThan
{
public:
    inline bool operator()(BOBUIreeWidgetItem *i1, BOBUIreeWidgetItem *i2) const
        { return *i2 < *i1; }
};

/*
    \class BOBUIreeModel
    \brief The BOBUIreeModel class manages the items stored in a tree view.

    \ingroup model-view
    \inmodule BobUIWidgets

*/

/*!
    \enum BOBUIreeWidgetItem::ChildIndicatorPolicy

    \value ShowIndicator     The controls for expanding and collapsing will be shown for this item even if there are no children.
    \value DontShowIndicator   The controls for expanding and collapsing will never be shown even if there are children.  If the node is forced open the user will not be able to expand or collapse the item.
    \value DontShowIndicatorWhenChildless  The controls for expanding and collapsing will be shown if the item contains children.
*/

/*!
    \fn void BOBUIreeWidgetItem::setDisabled(bool disabled)

    Disables the item if \a disabled is true; otherwise enables the item.

    \sa setFlags()
*/

/*!
    \fn bool BOBUIreeWidgetItem::isDisabled() const

    Returns \c true if the item is disabled; otherwise returns \c false.

    \sa setFlags()
*/

/*!
  \internal

  Constructs a tree model with a \a parent object and the given
  number of \a columns.
*/

BOBUIreeModel::BOBUIreeModel(int columns, BOBUIreeWidget *parent)
    : QAbstractItemModel(*new BOBUIreeModelPrivate, parent),
      rootItem(new BOBUIreeWidgetItem),
      headerItem(new BOBUIreeWidgetItem)
{
    rootItem->view = parent;
    rootItem->itemFlags = BobUI::ItemIsDropEnabled;
    headerItem->view = parent;
    setColumnCount(columns);
}

/*!
  \internal

*/

BOBUIreeModel::BOBUIreeModel(BOBUIreeModelPrivate &dd, BOBUIreeWidget *parent)
    : QAbstractItemModel(dd, parent), rootItem(new BOBUIreeWidgetItem), headerItem(new BOBUIreeWidgetItem)
{
    rootItem->view = parent;
    rootItem->itemFlags = BobUI::ItemIsDropEnabled;
    headerItem->view = parent;
}

/*!
  \internal

  Destroys this tree model.
*/

BOBUIreeModel::~BOBUIreeModel()
{
    clear();
    headerItem->view = nullptr;
    delete headerItem;
    rootItem->view = nullptr;
    delete rootItem;
}

/*!
  \internal

  Removes all items in the model.
*/

void BOBUIreeModel::clear()
{
    SkipSorting skipSorting(this);
    beginResetModel();
    for (int i = 0; i < rootItem->childCount(); ++i) {
        BOBUIreeWidgetItem *item = rootItem->children.at(i);
        item->par = nullptr;
        item->view = nullptr;
        delete item;
    }
    rootItem->children.clear();
    sortPendingTimer.stop();
    endResetModel();
}

/*!
  \internal

  Sets the number of \a columns in the tree model.
*/

void BOBUIreeModel::setColumnCount(int columns)
{
    SkipSorting skipSorting(this);
    if (columns < 0)
        return;
    if (!headerItem) {
        headerItem = new BOBUIreeWidgetItem();
        headerItem->view = view();
    }
    int count = columnCount();
    if (count == columns)
        return;

    if (columns < count) {
        beginRemoveColumns(QModelIndex(), columns, count - 1);
        headerItem->values.resize(columns);
        endRemoveColumns();
    } else {
        beginInsertColumns(QModelIndex(), count, columns - 1);
        headerItem->values.resize(columns);
        for (int i = count; i < columns; ++i) {// insert data without emitting the dataChanged signal
            headerItem->values[i].append(QWidgetItemData(BobUI::DisplayRole, QString::number(i + 1)));
            headerItem->d->display.append(QString::number(i + 1));
        }
        endInsertColumns();
    }
}

/*!
  \internal

  Returns the tree view item corresponding to the \a index given.

  \sa QModelIndex
*/

BOBUIreeWidgetItem *BOBUIreeModel::item(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;
    return static_cast<BOBUIreeWidgetItem*>(index.internalPointer());
}

/*!
  \internal

  Returns the model index that refers to the
  tree view \a item and \a column.
*/

QModelIndex BOBUIreeModel::index(const BOBUIreeWidgetItem *item, int column) const
{
    executePendingSort();

    if (!item || (item == rootItem))
        return QModelIndex();
    const BOBUIreeWidgetItem *par = item->parent();
    BOBUIreeWidgetItem *itm = const_cast<BOBUIreeWidgetItem*>(item);
    if (!par)
        par = rootItem;
    int row;
    int guess = item->d->rowGuess;
    if (guess >= 0
        && par->children.size() > guess
        && par->children.at(guess) == itm) {
        row = guess;
    } else {
        row = par->children.lastIndexOf(itm);
        itm->d->rowGuess = row;
    }
    return createIndex(row, column, itm);
}

/*!
  \internal
  \reimp

  Returns the model index with the given \a row,
  \a column and \a parent.
*/

QModelIndex BOBUIreeModel::index(int row, int column, const QModelIndex &parent) const
{
    executePendingSort();

    int c = columnCount(parent);
    if (row < 0 || column < 0 || column >= c)
        return QModelIndex();

    BOBUIreeWidgetItem *parentItem = parent.isValid() ? item(parent) : rootItem;
    if (parentItem && row < parentItem->childCount()) {
        BOBUIreeWidgetItem *itm = parentItem->child(row);
        if (itm)
            return createIndex(row, column, itm);
        return QModelIndex();
    }

    return QModelIndex();
}

/*!
  \internal
  \reimp

  Returns the parent model index of the index given as
  the \a child.
*/

QModelIndex BOBUIreeModel::parent(const QModelIndex &child) const
{
    SkipSorting skipSorting(this); //The reason we don't sort here is that this might be called from a valid QPersistentModelIndex
                                   //We don't want it to become suddenly invalid

    if (!child.isValid())
        return QModelIndex();
    BOBUIreeWidgetItem *itm = static_cast<BOBUIreeWidgetItem *>(child.internalPointer());
    if (!itm || itm == rootItem)
        return QModelIndex();
    BOBUIreeWidgetItem *parent = itm->parent();
    return index(parent, 0);
}

/*!
  \internal
  \reimp

  Returns the number of rows in the \a parent model index.
*/

int BOBUIreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return rootItem->childCount();

    BOBUIreeWidgetItem *parentItem = item(parent);
    if (parentItem)
        return parentItem->childCount();
    return 0;
}

/*!
  \internal
  \reimp

  Returns the number of columns in the item referred to by
  the given \a index.
*/

int BOBUIreeModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    if (!headerItem)
        return 0;
    return headerItem->columnCount();
}

bool BOBUIreeModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return (rootItem->childCount() > 0);

    BOBUIreeWidgetItem *itm = item(parent);
    if (!itm)
        return false;
    switch (itm->d->policy) {
    case BOBUIreeWidgetItem::ShowIndicator:
        return true;
    case BOBUIreeWidgetItem::DontShowIndicator:
        return false;
    case BOBUIreeWidgetItem::DontShowIndicatorWhenChildless:
        return (itm->childCount() > 0);
    }
    return false;
}

/*!
  \internal
  \reimp

  Returns the data corresponding to the given model \a index
  and \a role.
*/

QVariant BOBUIreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    BOBUIreeWidgetItem *itm = item(index);
    if (itm)
        return itm->data(index.column(), role);
    return QVariant();
}

/*!
  \internal
  \reimp

  Sets the data for the item specified by the \a index and \a role
  to that referred to by the \a value.

  Returns \c true if successful; otherwise returns \c false.
*/

bool BOBUIreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    BOBUIreeWidgetItem *itm = item(index);
    if (itm) {
        itm->setData(index.column(), role, value);
        return true;
    }
    return false;
}

bool BOBUIreeModel::clearItemData(const QModelIndex &index)
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return false;
    BOBUIreeWidgetItem *itm = item(index);
    if (!itm)
        return false;
    const auto beginIter = itm->values.at(index.column()).cbegin();
    const auto endIter = itm->values.at(index.column()).cend();
    if (std::all_of(beginIter, endIter, [](const QWidgetItemData& data) -> bool { return !data.value.isValid(); })
        && !itm->d->display.at(index.column()).isValid()) {
        return true; //it's already cleared
    }
    itm->d->display[index.column()] = QVariant();
    itm->values[index.column()].clear();
    emit dataChanged(index, index, QList<int> {});
    return true;
}

QMap<int, QVariant> BOBUIreeModel::itemData(const QModelIndex &index) const
{
    QMap<int, QVariant> roles;
    BOBUIreeWidgetItem *itm = item(index);
    if (itm) {
        int column = index.column();
        if (column < itm->values.size()) {
            for (int i = 0; i < itm->values.at(column).size(); ++i) {
                roles.insert(itm->values.at(column).at(i).role,
                             itm->values.at(column).at(i).value);
            }
        }

        // the two special cases
        QVariant displayValue = itm->data(column, BobUI::DisplayRole);
        if (displayValue.isValid())
            roles.insert(BobUI::DisplayRole, displayValue);

        QVariant checkValue = itm->data(column, BobUI::CheckStateRole);
        if (checkValue.isValid())
            roles.insert(BobUI::CheckStateRole, checkValue);
    }
    return roles;
}

/*!
  \internal
  \reimp
*/
bool BOBUIreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    SkipSorting skipSorting(this);
    if (count < 1 || row < 0 || row > rowCount(parent) || parent.column() > 0)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    BOBUIreeWidgetItem *par = item(parent);
    while (count > 0) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem();
        item->view = view();
        item->par = par;
        if (par)
            par->children.insert(row++, item);
        else
            rootItem->children.insert(row++, item);
        --count;
    }
    endInsertRows();
    return true;
}

/*!
  \internal
  \reimp
*/
bool BOBUIreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    SkipSorting skipSorting(this);
    if (count < 1 || column < 0 || column > columnCount(parent) || parent.column() > 0 || !headerItem)
        return false;

    beginInsertColumns(parent, column, column + count - 1);

    int oldCount = columnCount(parent);
    column = qBound(0, column, oldCount);
    headerItem->values.resize(oldCount + count);
    for (int i = oldCount; i < oldCount + count; ++i) {
        headerItem->values[i].append(QWidgetItemData(BobUI::DisplayRole, QString::number(i + 1)));
        headerItem->d->display.append(QString::number(i + 1));
    }

    QStack<BOBUIreeWidgetItem*> itemstack;
    itemstack.push(0);
    while (!itemstack.isEmpty()) {
        BOBUIreeWidgetItem *par = itemstack.pop();
        QList<BOBUIreeWidgetItem*> children = par ? par->children : rootItem->children;
        for (int row = 0; row < children.size(); ++row) {
            BOBUIreeWidgetItem *child = children.at(row);
            if (child->children.size())
                itemstack.push(child);
            child->values.insert(column, count, QList<QWidgetItemData>());
        }
    }

    endInsertColumns();
    return true;
}

/*!
  \internal
  \reimp
*/
bool BOBUIreeModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || (row + count) > rowCount(parent))
        return false;
    BOBUIreeWidgetItem *parentItem = item(parent);
    // if parentItem is valid, begin/end RemoveRows is handled by takeChild below
    if (!parentItem)
        beginRemoveRows(parent, row, row + count - 1);
    for (int i = row + count - 1; i >= row; --i) {
        BOBUIreeWidgetItem *child = parentItem ? parentItem->takeChild(i) : rootItem->children.takeAt(i);
        Q_ASSERT(child);
        child->view = nullptr;
        delete child;
    }
    if (!parentItem)
        endRemoveRows();
    return true;
}

/*!
  \internal
  \reimp

  Returns the header data corresponding to the given header \a section,
  \a orientation and data \a role.
*/

QVariant BOBUIreeModel::headerData(int section, BobUI::Orientation orientation, int role) const
{
    if (orientation != BobUI::Horizontal)
        return QVariant();

    if (headerItem)
        return headerItem->data(section, role);
    if (role == BobUI::DisplayRole)
        return QString::number(section + 1);
    return QVariant();
}

/*!
  \internal
  \reimp

  Sets the header data for the item specified by the header \a section,
  \a orientation and data \a role to the given \a value.

  Returns \c true if successful; otherwise returns \c false.
*/

bool BOBUIreeModel::setHeaderData(int section, BobUI::Orientation orientation,
                               const QVariant &value, int role)
{
    if (section < 0 || orientation != BobUI::Horizontal || !headerItem || section >= columnCount())
        return false;

    headerItem->setData(section, role, value);
    return true;
}

/*!
    \class BOBUIreeModel
    \inmodule BobUIWidgets
    \internal
*/

/*!
  \reimp

  Returns the flags for the item referred to the given \a index.

*/

BobUI::ItemFlags BOBUIreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return rootItem->flags();
    BOBUIreeWidgetItem *itm = item(index);
    Q_ASSERT(itm);
    return itm->flags();
}

/*!
  \internal

  Sorts the entire tree in the model in the given \a order,
  by the values in the given \a column.
*/

void BOBUIreeModel::sort(int column, BobUI::SortOrder order)
{
    SkipSorting skipSorting(this);
    sortPendingTimer.stop();

    if (column < 0 || column >= columnCount())
        return;

    //layoutAboutToBeChanged and layoutChanged will be called by sortChildren
    rootItem->sortChildren(column, order, true);
}

/*!
  \internal
*/
void BOBUIreeModel::ensureSorted(int column, BobUI::SortOrder order,
                              int start, int end, const QModelIndex &parent)
{
    if (isChanging())
        return;

    sortPendingTimer.stop();

    if (column < 0 || column >= columnCount())
        return;

    SkipSorting skipSorting(this);

    BOBUIreeWidgetItem *itm = item(parent);
    if (!itm)
        itm = rootItem;
    QList<BOBUIreeWidgetItem*> lst = itm->children;

    int count = end - start + 1;
    QList<std::pair<BOBUIreeWidgetItem *, int>> sorting(count);
    for (int i = 0; i < count; ++i) {
        sorting[i].first = lst.at(start + i);
        sorting[i].second = start + i;
    }

    const auto compare = (order == BobUI::AscendingOrder ? &itemLessThan : &itemGreaterThan);
    std::stable_sort(sorting.begin(), sorting.end(), compare);

    QModelIndexList oldPersistentIndexes;
    QModelIndexList newPersistentIndexes;
    QList<BOBUIreeWidgetItem*>::iterator lit = lst.begin();
    bool changed = false;

    for (int i = 0; i < count; ++i) {
        int oldRow = sorting.at(i).second;

        int tmpitepos = lit - lst.begin();
        BOBUIreeWidgetItem *item = lst.takeAt(oldRow);
        if (tmpitepos > lst.size())
            --tmpitepos;
        lit = lst.begin() + tmpitepos;

        lit = sortedInsertionIterator(lit, lst.end(), order, item);
        int newRow = qMax<qsizetype>(lit - lst.begin(), 0);

        if ((newRow < oldRow) && !(*item < *lst.at(oldRow - 1)) && !(*lst.at(oldRow - 1) < *item ))
            newRow = oldRow;

        lit = lst.insert(lit, item);
        if (newRow != oldRow) {
            // we are going to change the persistent indexes, so we need to prepare
            if (!changed) { // this will only happen once
                changed = true;
                emit layoutAboutToBeChanged({parent}, QAbstractItemModel::VerticalSortHint); // the selection model needs to know
                oldPersistentIndexes = persistentIndexList();
                newPersistentIndexes = oldPersistentIndexes;
            }
            for (int j = i + 1; j < count; ++j) {
                int otherRow = sorting.at(j).second;
                if (oldRow < otherRow && newRow >= otherRow)
                    --sorting[j].second;
                else if (oldRow > otherRow && newRow <= otherRow)
                    ++sorting[j].second;
            }
            for (int k = 0; k < newPersistentIndexes.size(); ++k) {
                QModelIndex pi = newPersistentIndexes.at(k);
                if (pi.parent() != parent)
                    continue;
                int oldPersistentRow = pi.row();
                int newPersistentRow = oldPersistentRow;
                if (oldPersistentRow == oldRow)
                    newPersistentRow = newRow;
                else if (oldRow < oldPersistentRow && newRow >= oldPersistentRow)
                    newPersistentRow = oldPersistentRow - 1;
                else if (oldRow > oldPersistentRow && newRow <= oldPersistentRow)
                    newPersistentRow = oldPersistentRow + 1;
                if (newPersistentRow != oldPersistentRow)
                    newPersistentIndexes[k] = createIndex(newPersistentRow,
                                                          pi.column(), pi.internalPointer());
            }
        }
    }

    if (changed) {
        itm->children = lst;
        changePersistentIndexList(oldPersistentIndexes, newPersistentIndexes);
        emit layoutChanged({parent}, QAbstractItemModel::VerticalSortHint);
    }
}

/*!
  \internal

  Returns \c true if the value of the \a left item is
  less than the value of the \a right item.

  Used by the sorting functions.
*/

bool BOBUIreeModel::itemLessThan(const std::pair<BOBUIreeWidgetItem*,int> &left,
                              const std::pair<BOBUIreeWidgetItem*,int> &right)
{
    return *(left.first) < *(right.first);
}

/*!
  \internal

  Returns \c true if the value of the \a left item is
  greater than the value of the \a right item.

  Used by the sorting functions.
*/

bool BOBUIreeModel::itemGreaterThan(const std::pair<BOBUIreeWidgetItem*,int> &left,
                                 const std::pair<BOBUIreeWidgetItem*,int> &right)
{
    return *(right.first) < *(left.first);
}

/*!
  \internal
*/
QList<BOBUIreeWidgetItem*>::iterator BOBUIreeModel::sortedInsertionIterator(
    const QList<BOBUIreeWidgetItem*>::iterator &begin,
    const QList<BOBUIreeWidgetItem*>::iterator &end,
    BobUI::SortOrder order, BOBUIreeWidgetItem *item)
{
    if (order == BobUI::AscendingOrder)
        return std::lower_bound(begin, end, item, BOBUIreeModelLessThan());
    return std::lower_bound(begin, end, item, BOBUIreeModelGreaterThan());
}

QStringList BOBUIreeModel::mimeTypes() const
{
    auto v = view();
    if (v)
        return v->mimeTypes();
    return {};
}

QMimeData *BOBUIreeModel::internalMimeData()  const
{
    return QAbstractItemModel::mimeData(cachedIndexes);
}

QMimeData *BOBUIreeModel::mimeData(const QModelIndexList &indexes) const
{
    QList<BOBUIreeWidgetItem *> items;
    std::transform(indexes.begin(), indexes.end(), std::back_inserter(items),
                   [this](const QModelIndex &idx) -> BOBUIreeWidgetItem * { return item(idx); });

    // Ensure we only have one item as an item may have more than
    // one index selected if there is more than one column
    std::sort(items.begin(), items.end());
    items.erase(std::unique(items.begin(), items.end()), items.end());

    // cachedIndexes is a little hack to avoid copying from QModelIndexList to
    // QList<BOBUIreeWidgetItem*> and back again in the view
    cachedIndexes = indexes;
    QMimeData *mimeData = view()->mimeData(items);
    cachedIndexes.clear();
    return mimeData;
}

bool BOBUIreeModel::dropMimeData(const QMimeData *data, BobUI::DropAction action,
                              int row, int column, const QModelIndex &parent)
{
    if (row == -1 && column == -1)
        row = rowCount(parent); // append
    return view()->dropMimeData(item(parent), row, data, action);
}

BobUI::DropActions BOBUIreeModel::supportedDropActions() const
{
    return view()->supportedDropActions();
}

BobUI::DropActions BOBUIreeModel::supportedDragActions() const
{
#if BOBUI_CONFIG(draganddrop)
    return view()->supportedDragActions();
#else
    return BobUI::DropActions(BobUI::IgnoreAction);
#endif
}

void BOBUIreeModel::itemChanged(BOBUIreeWidgetItem *item)
{
    if (item->columnCount() <= 0)
        return;
    SkipSorting skipSorting(this); //this is kind of wrong, but not doing this would kill performance
    QModelIndex left = index(item, 0);
    QModelIndex right = index(item, item->columnCount() - 1);
    emit dataChanged(left, right);
}

bool BOBUIreeModel::isChanging() const
{
    Q_D(const BOBUIreeModel);
    return !d->changes.isEmpty();
}

/*!
  \internal
    Emits the dataChanged() signal for the given \a item.
    if column is -1 then all columns have changed
*/

void BOBUIreeModel::emitDataChanged(BOBUIreeWidgetItem *item, int column, const QList<int> &roles)
{
    if (signalsBlocked())
        return;

    if (headerItem == item && column < item->columnCount()) {
        if (column == -1)
            emit headerDataChanged(BobUI::Horizontal, 0, columnCount() - 1);
        else
            emit headerDataChanged(BobUI::Horizontal, column, column);
        return;
    }

    SkipSorting skipSorting(this); //This is a little bit wrong, but not doing it would kill performance

    QModelIndex bottomRight, topLeft;
    if (column == -1) {
        topLeft = index(item, 0);
        bottomRight = createIndex(topLeft.row(), columnCount() - 1, item);
    } else {
        topLeft = index(item, column);
        bottomRight = topLeft;
    }
    emit dataChanged(topLeft, bottomRight, roles);
}

void BOBUIreeModel::beginInsertItems(BOBUIreeWidgetItem *parent, int row, int count)
{
    QModelIndex par = index(parent, 0);
    beginInsertRows(par, row, row + count - 1);
}

void BOBUIreeModel::endInsertItems()
{
    endInsertRows();
}

void BOBUIreeModel::beginRemoveItems(BOBUIreeWidgetItem *parent, int row, int count)
{
    Q_ASSERT(row >= 0);
    Q_ASSERT(count > 0);
    beginRemoveRows(index(parent, 0), row, row + count - 1);
    if (!parent)
        parent = rootItem;
    // now update the iterators
    for (int i = 0; i < iterators.size(); ++i) {
        for (int j = 0; j < count; j++) {
            BOBUIreeWidgetItem *c = parent->child(row + j);
            iterators[i]->d_func()->ensureValidIterator(c);
        }
    }
}

void BOBUIreeModel::endRemoveItems()
{
    endRemoveRows();
}

void BOBUIreeModel::sortItems(QList<BOBUIreeWidgetItem*> *items, int column, BobUI::SortOrder order)
{
    // see BOBUIreeViewItem::operator<
    Q_UNUSED(column);
    if (isChanging())
        return;

    // store the original order of indexes
    QList<std::pair<BOBUIreeWidgetItem *, int>> sorting(items->size());
    for (int i = 0; i < sorting.size(); ++i) {
        sorting[i].first = items->at(i);
        sorting[i].second = i;
    }

    // do the sorting
    const auto compare = (order == BobUI::AscendingOrder ? &itemLessThan : &itemGreaterThan);
    std::stable_sort(sorting.begin(), sorting.end(), compare);

    QModelIndexList fromList;
    QModelIndexList toList;
    int colCount = columnCount();
    for (int r = 0; r < sorting.size(); ++r) {
        int oldRow = sorting.at(r).second;
        if (oldRow == r)
            continue;
        BOBUIreeWidgetItem *item = sorting.at(r).first;
        items->replace(r, item);
        for (int c = 0; c < colCount; ++c) {
            QModelIndex from = createIndex(oldRow, c, item);
            if (static_cast<QAbstractItemModelPrivate *>(d_ptr.data())->persistent.indexes.contains(from)) {
                QModelIndex to = createIndex(r, c, item);
                fromList << from;
                toList << to;
            }
        }
    }
    changePersistentIndexList(fromList, toList);
}

void BOBUIreeModel::timerEvent(BOBUIimerEvent *ev)
{
    if (ev->timerId() == sortPendingTimer.timerId()) {
        executePendingSort();
    } else {
        QAbstractItemModel::timerEvent(ev);
    }
}

/*!
  \class BOBUIreeWidgetItem

  \brief The BOBUIreeWidgetItem class provides an item for use with the
  BOBUIreeWidget convenience class.

  \ingroup model-view
  \inmodule BobUIWidgets

  Tree widget items are used to hold rows of information for tree widgets.
  Rows usually contain several columns of data, each of which can contain
  a text label and an icon.

  The BOBUIreeWidgetItem class is a convenience class that replaces the
  QListViewItem class in BobUI 3. It provides an item for use with
  the BOBUIreeWidget class.

  Items are usually constructed with a parent that is either a BOBUIreeWidget
  (for top-level items) or a BOBUIreeWidgetItem (for items on lower levels of
  the tree). For example, the following code constructs a top-level item
  to represent cities of the world, and adds a entry for Oslo as a child
  item:

  \snippet bobuireewidget-using/mainwindow.cpp 3

  Items can be added in a particular order by specifying the item they
  follow when they are constructed:

  \snippet bobuireewidget-using/mainwindow.cpp 5

  Each column in an item can have its own background brush which is set with
  the setBackground() function. The current background brush can be
  found with background().
  The text label for each column can be rendered with its own font and brush.
  These are specified with the setFont() and setForeground() functions,
  and read with font() and foreground().

  The main difference between top-level items and those in lower levels of
  the tree is that a top-level item has no parent(). This information
  can be used to tell the difference between items, and is useful to know
  when inserting and removing items from the tree.
  Children of an item can be removed with takeChild() and inserted at a
  given index in the list of children with the insertChild() function.

  By default, items are enabled, selectable, checkable, and can be the source
  of a drag and drop operation.
  Each item's flags can be changed by calling setFlags() with the appropriate
  value (see \l{BobUI::ItemFlags}). Checkable items can be checked and unchecked
  with the setCheckState() function. The corresponding checkState() function
  indicates whether the item is currently checked.

  \section1 Subclassing

  When subclassing BOBUIreeWidgetItem to provide custom items, it is possible to
  define new types for them so that they can be distinguished from standard
  items. The constructors for subclasses that require this feature need to
  call the base class constructor with a new type value equal to or greater
  than \l UserType.

  \sa BOBUIreeWidget, BOBUIreeWidgetItemIterator, {Model/View Programming},
  QListWidgetItem, BOBUIableWidgetItem
*/

/*!
    \enum BOBUIreeWidgetItem::ItemType

    This enum describes the types that are used to describe tree widget items.

    \value Type     The default type for tree widget items.
    \value UserType The minimum value for custom types. Values below UserType are
                    reserved by BobUI.

    You can define new user types in BOBUIreeWidgetItem subclasses to ensure that
    custom items are treated specially; for example, when items are sorted.

    \sa type()
*/

/*!
    \fn int BOBUIreeWidgetItem::type() const

    Returns the type passed to the BOBUIreeWidgetItem constructor.
*/

/*!
    \fn void BOBUIreeWidgetItem::sortChildren(int column, BobUI::SortOrder order)

    Sorts the children of the item using the given \a order,
    by the values in the given \a column.

    \note This function does nothing if the item is not associated with a
    BOBUIreeWidget.
*/

/*!
    \fn BOBUIreeWidget *BOBUIreeWidgetItem::treeWidget() const

    Returns the tree widget that contains the item.
*/

/*!
  \fn void BOBUIreeWidgetItem::setSelected(bool select)

  Sets the selected state of the item to \a select.

  \sa isSelected()
*/
void BOBUIreeWidgetItem::setSelected(bool select)
{
    const BOBUIreeModel *model = treeModel();
    if (!model || !view->selectionModel())
        return;
    const QModelIndex index = model->index(this, 0);
    view->selectionModel()->select(index, (select ? QItemSelectionModel::Select
                                                  : QItemSelectionModel::Deselect)
                                   | QItemSelectionModel::Rows);
    d->selected = select;
}

/*!
  \fn bool BOBUIreeWidgetItem::isSelected() const

  Returns \c true if the item is selected, otherwise returns \c false.

  \sa setSelected()
*/
bool BOBUIreeWidgetItem::isSelected() const
{
    return d->selected;
}

/*!
  \fn void BOBUIreeWidgetItem::setHidden(bool hide)

  Hides the item if \a hide is true, otherwise shows the item.
  \note A call to this function has no effect if the item is not currently in a view. In particular,
        calling \c setHidden(true) on an item and only then adding it to a view will result in
        a visible item.

  \sa isHidden()
*/

void BOBUIreeWidgetItem::setHidden(bool hide)
{
    const BOBUIreeModel *model = treeModel();
    if (!model)
        return;
    if (this == model->headerItem) {
        view->header()->setHidden(hide);
    } else {
        const QModelIndex index = view->d_func()->index(this);
        view->setRowHidden(index.row(), index.parent(), hide);
    }
    d->hidden = hide;
}

/*!
  \fn bool BOBUIreeWidgetItem::isHidden() const

  Returns \c true if the item is hidden, otherwise returns \c false.

  \sa setHidden()
*/

bool BOBUIreeWidgetItem::isHidden() const
{
    const BOBUIreeModel *model = treeModel();
    if (!model)
        return false;
    if (this == model->headerItem)
        return view->header()->isHidden();
    if (view->d_func()->hiddenIndexes.isEmpty())
        return false;
    BOBUIreeModel::SkipSorting skipSorting(model);
    return view->d_func()->isRowHidden(view->d_func()->index(this));
}

/*!
  \fn void BOBUIreeWidgetItem::setExpanded(bool expand)

  Expands the item if \a expand is true, otherwise collapses the item.
  \warning The BOBUIreeWidgetItem must be added to the BOBUIreeWidget before calling this function.

  \sa isExpanded()
*/
void BOBUIreeWidgetItem::setExpanded(bool expand)
{
    const BOBUIreeModel *model = treeModel();
    if (!model)
        return;
    BOBUIreeModel::SkipSorting skipSorting(model);
    view->setExpanded(view->d_func()->index(this), expand);
}

/*!
  \fn bool BOBUIreeWidgetItem::isExpanded() const

  Returns \c true if the item is expanded, otherwise returns \c false.

  \sa setExpanded()
*/
bool BOBUIreeWidgetItem::isExpanded() const
{
    const BOBUIreeModel *model = treeModel();
    if (!model)
        return false;
    BOBUIreeModel::SkipSorting skipSorting(model);
    return view->isExpanded(view->d_func()->index(this));
}

/*!
  \fn void BOBUIreeWidgetItem::setFirstColumnSpanned(bool span)

  Sets the first section to span all columns if \a span is true;
  otherwise all item sections are shown.

  \sa isFirstColumnSpanned()
*/
void BOBUIreeWidgetItem::setFirstColumnSpanned(bool span)
{
    const BOBUIreeModel *model = treeModel();
    if (!model || this == model->headerItem)
        return; // We can't set the header items to spanning
    const QModelIndex index = model->index(this, 0);
    view->setFirstColumnSpanned(index.row(), index.parent(), span);
}

/*!
  \fn bool BOBUIreeWidgetItem::isFirstColumnSpanned() const

  Returns \c true if the item is spanning all the columns in a row; otherwise returns \c false.

  \sa setFirstColumnSpanned()
*/
bool BOBUIreeWidgetItem::isFirstColumnSpanned() const
{
    const BOBUIreeModel *model = treeModel();
    if (!model || this == model->headerItem)
        return false;
    const QModelIndex index = model->index(this, 0);
    return view->isFirstColumnSpanned(index.row(), index.parent());
}

/*!
    \fn QString BOBUIreeWidgetItem::text(int column) const

    Returns the text in the specified \a column.

    \sa setText()
*/

/*!
    \fn void BOBUIreeWidgetItem::setText(int column, const QString &text)

    Sets the text to be displayed in the given \a column to the given \a text.

    \sa text(), setFont(), setForeground()
*/

/*!
    \fn QIcon BOBUIreeWidgetItem::icon(int column) const

    Returns the icon that is displayed in the specified \a column.

    \sa setIcon(), {QAbstractItemView::iconSize}{iconSize}
*/

/*!
    \fn void BOBUIreeWidgetItem::setIcon(int column, const QIcon &icon)

    Sets the icon to be displayed in the given \a column to \a icon.

    \sa icon(), setText(), {QAbstractItemView::iconSize}{iconSize}
*/

/*!
    \fn QString BOBUIreeWidgetItem::statusTip(int column) const

    Returns the status tip for the contents of the given \a column.

    \sa setStatusTip()
*/

/*!
    \fn void BOBUIreeWidgetItem::setStatusTip(int column, const QString &statusTip)

    Sets the status tip for the given \a column to the given \a statusTip.
    BOBUIreeWidget mouse tracking needs to be enabled for this feature to work.

    \sa statusTip(), setToolTip(), setWhatsThis()
*/

/*!
    \fn QString BOBUIreeWidgetItem::toolTip(int column) const

    Returns the tool tip for the given \a column.

    \sa setToolTip()
*/

/*!
    \fn void BOBUIreeWidgetItem::setToolTip(int column, const QString &toolTip)

    Sets the tooltip for the given \a column to \a toolTip.

    \sa toolTip(), setStatusTip(), setWhatsThis()
*/

/*!
    \fn QString BOBUIreeWidgetItem::whatsThis(int column) const

    Returns the "What's This?" help for the contents of the given \a column.

    \sa setWhatsThis()
*/

/*!
    \fn void BOBUIreeWidgetItem::setWhatsThis(int column, const QString &whatsThis)

    Sets the "What's This?" help for the given \a column to \a whatsThis.

    \sa whatsThis(), setStatusTip(), setToolTip()
*/

/*!
    \fn QFont BOBUIreeWidgetItem::font(int column) const

    Returns the font used to render the text in the specified \a column.

    \sa setFont()
*/

/*!
    \fn void BOBUIreeWidgetItem::setFont(int column, const QFont &font)

    Sets the font used to display the text in the given \a column to the given
    \a font.

    \sa font(), setText(), setForeground()
*/

/*!
    \fn QBrush BOBUIreeWidgetItem::background(int column) const

    Returns the brush used to render the background of the specified \a column.

    \sa foreground()
*/

/*!
    \fn void BOBUIreeWidgetItem::setBackground(int column, const QBrush &brush)

    Sets the background brush of the label in the given \a column to the
    specified \a brush.
    Setting a default-constructed brush will let the view use the
    default color from the style.

    \note If \l{BobUI Style Sheets} are used on the same widget as setBackground(),
    style sheets will take precedence if the settings conflict.

    \sa setForeground()
*/


/*!
    \fn QBrush BOBUIreeWidgetItem::foreground(int column) const

    Returns the brush used to render the foreground (e.g. text) of the
    specified \a column.
    Setting a default-constructed brush will let the view use the
    default color from the style.

    \sa background()
*/

/*!
    \fn void BOBUIreeWidgetItem::setForeground(int column, const QBrush &brush)

    Sets the foreground brush of the label in the given \a column to the
    specified \a brush.

    \sa setBackground()
*/

/*!
    \fn BobUI::CheckState BOBUIreeWidgetItem::checkState(int column) const

    Returns the check state of the label in the given \a column.

    \sa BobUI::CheckState
*/

/*!
    \fn void BOBUIreeWidgetItem::setCheckState(int column, BobUI::CheckState state)

    Sets the item in the given \a column check state to be \a state.

    \sa checkState()
*/

/*!
  \fn QSize BOBUIreeWidgetItem::sizeHint(int column) const

  Returns the size hint set for the tree item in the given
  \a column (see \l{QSize}).
*/

/*!
  \fn void BOBUIreeWidgetItem::setSizeHint(int column, const QSize &size)

  Sets the size hint for the tree item in the given \a column to be \a size.
  If no size hint is set or \a size is invalid, the item
  delegate will compute the size hint based on the item data.
*/

/*!
    \fn BOBUIreeWidgetItem *BOBUIreeWidgetItem::parent() const

    Returns the item's parent.

    \sa child()
*/

/*!
    \fn BOBUIreeWidgetItem *BOBUIreeWidgetItem::child(int index) const

    Returns the item at the given \a index in the list of the item's children.

    \sa parent()
*/

/*!
    \fn int BOBUIreeWidgetItem::childCount() const

    Returns the number of child items.
*/

/*!
    \fn int BOBUIreeWidgetItem::columnCount() const

    Returns the number of columns in the item.
*/

/*!
  \if defined(bobui7)

  \fn BobUI::Alignment BOBUIreeWidgetItem::textAlignment(int column) const

  Returns the text alignment for the label in the given \a column.

  \else

  \fn int BOBUIreeWidgetItem::textAlignment(int column) const

  Returns the text alignment for the label in the given \a column.

  \note This function returns an int for historical reasons. It will
  be corrected to return BobUI::Alignment in BobUI 7.

  \sa BobUI::Alignment

  \endif
*/

/*!
  \obsolete [6.4] Use the overload that takes a BobUI::Alignment argument.

  \fn void BOBUIreeWidgetItem::setTextAlignment(int column, int alignment)

  Sets the text alignment for the label in the given \a column to
  the \a alignment specified.

  \sa BobUI::Alignment
*/

/*!
  \since 6.4

  \fn void BOBUIreeWidgetItem::setTextAlignment(int column, BobUI::Alignment alignment)

  Sets the text alignment for the label in the given \a column to
  the \a alignment specified.
*/

/*!
  \fn void BOBUIreeWidgetItem::setTextAlignment(int column, BobUI::AlignmentFlag alignment)
  \internal
*/

/*!
    \fn int BOBUIreeWidgetItem::indexOfChild(BOBUIreeWidgetItem *child) const

    Returns the index of the given \a child in the item's list of children.
*/

/*!
    Constructs a tree widget item of the specified \a type. The item
    must be inserted into a tree widget.

    \sa type()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(int type) : rtti(type), d(new BOBUIreeWidgetItemPrivate(this)) { }

/*!
    Constructs a tree widget item of the specified \a type. The item
    must be inserted into a tree widget.
    The given list of \a strings will be set as the item text for each
    column in the item.

    \sa type()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(const QStringList &strings, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    for (int i = 0; i < strings.size(); ++i)
        setText(i, strings.at(i));
}

/*!
    \fn BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidget *parent, int type)

    Constructs a tree widget item of the specified \a type and appends it
    to the items in the given \a parent.

    \sa type()
*/

BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidget *treeview, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    // do not set this->view here otherwise insertChild() will fail
    if (BOBUIreeModel *model = treeModel(treeview)) {
        model->rootItem->addChild(this);
        values.reserve(model->headerItem->columnCount());
    }
}

/*!
  \fn BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidget *parent, const QStringList &strings, int type)

  Constructs a tree widget item of the specified \a type and appends it
  to the items in the given \a parent. The given list of \a strings will be set as
  the item text for each column in the item.

  \sa type()
*/

BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidget *treeview, const QStringList &strings, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    for (int i = 0; i < strings.size(); ++i)
        setText(i, strings.at(i));
    // do not set this->view here otherwise insertChild() will fail
    if (BOBUIreeModel *model = treeModel(treeview)) {
        model->rootItem->addChild(this);
        values.reserve(model->headerItem->columnCount());
    }
}

/*!
    \fn BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidget *parent, BOBUIreeWidgetItem *preceding, int type)

    Constructs a tree widget item of the specified \a type and inserts it into
    the given \a parent after the \a preceding item.

    \sa type()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidget *treeview, BOBUIreeWidgetItem *after, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    // do not set this->view here otherwise insertChild() will fail
    if (BOBUIreeModel *model = treeModel(treeview)) {
        int i = model->rootItem->children.indexOf(after) + 1;
        model->rootItem->insertChild(i, this);
        values.reserve(model->headerItem->columnCount());
    }
}

/*!
    Constructs a tree widget item and append it to the given \a parent.

    \sa type()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    if (parent)
        parent->addChild(this);
}

/*!
    Constructs a tree widget item and append it to the given \a parent.
    The given list of \a strings will be set as the item text for each column in the item.

    \sa type()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, const QStringList &strings, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    for (int i = 0; i < strings.size(); ++i)
        setText(i, strings.at(i));
    if (parent)
        parent->addChild(this);
}

/*!
    \fn BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, BOBUIreeWidgetItem *preceding, int type)

    Constructs a tree widget item of the specified \a type that is inserted
    into the \a parent after the \a preceding child item.

    \sa type()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, BOBUIreeWidgetItem *after, int type)
    : rtti(type), d(new BOBUIreeWidgetItemPrivate(this))
{
    if (parent) {
        int i = parent->children.indexOf(after) + 1;
        parent->insertChild(i, this);
    }
}

/*!
  Destroys this tree widget item.

  The item will be removed from \l{BOBUIreeWidget}s to which it has
  been added. This makes it safe to delete an item at any time.

*/

BOBUIreeWidgetItem::~BOBUIreeWidgetItem()
{
    BOBUIreeModel *model = treeModel();
    BOBUIreeModel::SkipSorting skipSorting(model);

    if (par) {
        int i = par->children.indexOf(this);
        if (i >= 0) {
            if (model) model->beginRemoveItems(par, i, 1);
            // users _could_ do changes when connected to rowsAboutToBeRemoved,
            // so we check again to make sure 'i' is valid
            if (!par->children.isEmpty() && par->children.at(i) == this)
                par->children.takeAt(i);
            if (model) model->endRemoveItems();
        }
    } else if (model) {
        if (this == model->headerItem) {
            model->headerItem = nullptr;
        } else {
            int i = model->rootItem->children.indexOf(this);
            if (i >= 0) {
                model->beginRemoveItems(nullptr, i, 1);
                // users _could_ do changes when connected to rowsAboutToBeRemoved,
                // so we check again to make sure 'i' is valid
                if (!model->rootItem->children.isEmpty() && model->rootItem->children.at(i) == this)
                    model->rootItem->children.takeAt(i);
                model->endRemoveItems();
            }
        }
    }
    // at this point the persistent indexes for the children should also be invalidated
    // since we invalidated the parent
    for (int i = 0; i < children.size(); ++i) {
        BOBUIreeWidgetItem *child = children.at(i);
        // make sure the child does not try to remove itself from our children list
        child->par = nullptr;
        // make sure the child does not try to remove itself from the top level list
        child->view = nullptr;
        delete child;
    }

    children.clear();
    delete d;
}

/*!
    Creates a deep copy of the item and of its children.
*/
BOBUIreeWidgetItem *BOBUIreeWidgetItem::clone() const
{
    BOBUIreeWidgetItem *copy = nullptr;

    QStack<const BOBUIreeWidgetItem*> stack;
    QStack<BOBUIreeWidgetItem*> parentStack;
    stack.push(this);
    parentStack.push(0);

    BOBUIreeWidgetItem *root = nullptr;
    const BOBUIreeWidgetItem *item = nullptr;
    BOBUIreeWidgetItem *parent = nullptr;
    while (!stack.isEmpty()) {
        // get current item, and copied parent
        item = stack.pop();
        parent = parentStack.pop();

        // copy item
        copy = new BOBUIreeWidgetItem(*item);
        if (!root)
            root = copy;

        // set parent and add to parents children list
        if (parent) {
            copy->par = parent;
            parent->children.insert(0, copy);
        }

        for (int i = 0; i < item->childCount(); ++i) {
            stack.push(item->child(i));
            parentStack.push(copy);
        }
    }
    return root;
}

/*!
   Sets the item indicator \a policy. This policy decides when the
   tree branch expand/collapse indicator is shown.
   The default value is DontShowIndicatorWhenChildless.

   \sa childIndicatorPolicy()
*/
void BOBUIreeWidgetItem::setChildIndicatorPolicy(BOBUIreeWidgetItem::ChildIndicatorPolicy policy)
{
    if (d->policy == policy)
        return;
    d->policy = policy;

    if (!view)
        return;

    view->scheduleDelayedItemsLayout();
}

/*!
   Returns the item indicator policy. This policy decides when the
   tree branch expand/collapse indicator is shown.

   \sa setChildIndicatorPolicy()
*/
BOBUIreeWidgetItem::ChildIndicatorPolicy BOBUIreeWidgetItem::childIndicatorPolicy() const
{
    return d->policy;
}

/*!
    \fn void BOBUIreeWidgetItem::setFlags(BobUI::ItemFlags flags)

    Sets the flags for the item to the given \a flags. These determine whether
    the item can be selected or modified.  This is often used to disable an item.

    \sa flags()
*/
void BOBUIreeWidgetItem::setFlags(BobUI::ItemFlags flags)
{
    const bool enable = (flags & BobUI::ItemIsEnabled);
    const bool changedState = bool(itemFlags & BobUI::ItemIsEnabled) != enable;
    const bool changedExplicit = d->disabled != !enable;

    d->disabled = !enable;

    if (enable && par && !(par->itemFlags & BobUI::ItemIsEnabled)) // inherit from parent
        itemFlags = flags & ~BobUI::ItemIsEnabled;
    else // this item is explicitly disabled or has no parent
        itemFlags = flags;

    if (changedState && changedExplicit) { // if the propagate the change to the children
        QStack<BOBUIreeWidgetItem*> parents;
        parents.push(this);
        while (!parents.isEmpty()) {
            BOBUIreeWidgetItem *parent = parents.pop();
            for (int i = 0; i < parent->children.size(); ++i) {
                BOBUIreeWidgetItem *child = parent->children.at(i);
                if (!child->d->disabled) { // if not explicitly disabled
                    parents.push(child);
                    if (enable)
                        child->itemFlags = child->itemFlags | BobUI::ItemIsEnabled;
                    else
                        child->itemFlags = child->itemFlags & ~BobUI::ItemIsEnabled;
                    child->itemChanged(); // ### we may want to optimize this
                }
            }
        }
    }
    itemChanged();
}

void BOBUIreeWidgetItemPrivate::updateHiddenStatus(BOBUIreeWidgetItem *item, bool inserting)
{
    BOBUIreeModel *model = item->treeModel();
    if (!model)
        return;
    QStack<BOBUIreeWidgetItem *> parents;
    parents.push(item);
    while (!parents.isEmpty()) {
        BOBUIreeWidgetItem *parent = parents.pop();
        if (parent->d->hidden) {
            const QModelIndex index = model->index(parent, 0);
            item->view->setRowHidden(index.row(), index.parent(), inserting);
        }
        for (int i = 0; i < parent->children.size(); ++i) {
            BOBUIreeWidgetItem *child = parent->children.at(i);
            parents.push(child);
        }
    }
}

void BOBUIreeWidgetItemPrivate::propagateDisabled(BOBUIreeWidgetItem *item)
{
    Q_ASSERT(item);
    const bool enable = item->par ? (item->par->itemFlags.testFlag(BobUI::ItemIsEnabled)) : true;

    QStack<BOBUIreeWidgetItem*> parents;
    parents.push(item);
    while (!parents.isEmpty()) {
        BOBUIreeWidgetItem *parent = parents.pop();
        if (!parent->d->disabled) { // if not explicitly disabled
            BobUI::ItemFlags oldFlags = parent->itemFlags;
            if (enable)
                parent->itemFlags = parent->itemFlags | BobUI::ItemIsEnabled;
            else
                parent->itemFlags = parent->itemFlags & ~BobUI::ItemIsEnabled;
            if (parent->itemFlags != oldFlags)
                parent->itemChanged();
        }

        for (int i = 0; i < parent->children.size(); ++i) {
            BOBUIreeWidgetItem *child = parent->children.at(i);
            parents.push(child);
        }
    }
}
/*!
    \fn BobUI::ItemFlags BOBUIreeWidgetItem::flags() const

    Returns the flags used to describe the item. These determine whether
    the item can be checked, edited, and selected.

    The default value for flags is
    BobUI::ItemIsSelectable | BobUI::ItemIsUserCheckable | BobUI::ItemIsEnabled |
    BobUI::ItemIsDragEnabled | BobUI::ItemIsDropEnabled.

    \sa setFlags()
*/
BobUI::ItemFlags BOBUIreeWidgetItem::flags() const
{
    return itemFlags;
}

/*!
    Sets the value for the item's \a column and \a role to the given
    \a value.

    The \a role describes the type of data specified by \a value, and is defined by
    the BobUI::ItemDataRole enum.

    \note The default implementation treats BobUI::EditRole and BobUI::DisplayRole as
    referring to the same data.
*/
void BOBUIreeWidgetItem::setData(int column, int role, const QVariant &value)
{
    if (column < 0)
        return;

    BOBUIreeModel *model = treeModel();
    switch (role) {
    case BobUI::EditRole:
    case BobUI::DisplayRole: {
        if (values.size() <= column) {
            if (model && this == model->headerItem)
                model->setColumnCount(column + 1);
            else
                values.resize(column + 1);
        }
        if (d->display.size() <= column) {
            for (int i = d->display.size() - 1; i < column - 1; ++i)
                d->display.append(QVariant());
            d->display.append(value);
        } else if (d->display[column] != value) {
            d->display[column] = value;
        } else {
            return; // value is unchanged
        }
    } break;
    case BobUI::CheckStateRole:
        if ((itemFlags & BobUI::ItemIsAutoTristate) && value != BobUI::PartiallyChecked) {
            for (int i = 0; i < children.size(); ++i) {
                BOBUIreeWidgetItem *child = children.at(i);
                if (child->data(column, role).isValid()) {// has a CheckState
                    BobUI::ItemFlags f = itemFlags; // a little hack to avoid multiple dataChanged signals
                    itemFlags &= ~BobUI::ItemIsAutoTristate;
                    child->setData(column, role, value);
                    itemFlags = f;
                }
            }
        }
        Q_FALLTHROUGH();
    default:
        if (column < values.size()) {
            bool found = false;
            const QList<QWidgetItemData> column_values = values.at(column);
            for (int i = 0; i < column_values.size(); ++i) {
                if (column_values.at(i).role == role) {
                    if (column_values.at(i).value == value)
                        return; // value is unchanged
                    values[column][i].value = value;
                    found = true;
                    break;
                }
            }
            if (!found)
                values[column].append(QWidgetItemData(role, value));
        } else {
            if (model && this == model->headerItem)
                model->setColumnCount(column + 1);
            else
                values.resize(column + 1);
            values[column].append(QWidgetItemData(role, value));
        }
    }

    if (model) {
        const QList<int> roles((role == BobUI::DisplayRole || role == BobUI::EditRole)
                                       ? QList<int>({ BobUI::DisplayRole, BobUI::EditRole })
                                       : QList<int>({ role }));
        model->emitDataChanged(this, column, roles);
        if (role == BobUI::CheckStateRole) {
            BOBUIreeWidgetItem *p;
            for (p = par; p && (p->itemFlags & BobUI::ItemIsAutoTristate); p = p->par)
                model->emitDataChanged(p, column, roles);
        }
    }
}

/*!
    Returns the value for the item's \a column and \a role.
*/
QVariant BOBUIreeWidgetItem::data(int column, int role) const
{
    switch (role) {
    case BobUI::EditRole:
    case BobUI::DisplayRole:
        if (column >= 0 && column < d->display.size())
            return d->display.at(column);
        break;
    case BobUI::CheckStateRole:
        // special case for check state in tristate
        if (children.size() && (itemFlags & BobUI::ItemIsAutoTristate))
            return childrenCheckState(column);
        Q_FALLTHROUGH();
   default:
        if (column >= 0 && column < values.size()) {
            const QList<QWidgetItemData> &column_values = values.at(column);
            for (const auto &column_value : column_values) {
                if (column_value.role == role)
                    return column_value.value;
            }
        }
    }
    return QVariant();
}

/*!
  Returns \c true if the text in the item is less than the text in the
  \a other item, otherwise returns \c false.
*/

bool BOBUIreeWidgetItem::operator<(const BOBUIreeWidgetItem &other) const
{
    int column = view ? view->sortColumn() : 0;
    const QVariant v1 = data(column, BobUI::DisplayRole);
    const QVariant v2 = other.data(column, BobUI::DisplayRole);
    return QAbstractItemModelPrivate::variantLessThan(v1, v2);
}

#ifndef BOBUI_NO_DATASTREAM

/*!
    Reads the item from stream \a in. This only reads data into a single item.

    \sa write()
*/
void BOBUIreeWidgetItem::read(QDataStream &in)
{
    // convert from streams written before we introduced display (4.2.0)
    if (in.version() < QDataStream::BobUI_4_2) {
        d->display.clear();
        in >> values;
        // move the display value over to the display string list
        for (int column = 0; column < values.size(); ++column) {
            d->display << QVariant();
            for (int i = 0; i < values.at(column).size(); ++i) {
                if (values.at(column).at(i).role == BobUI::DisplayRole) {
                    d->display[column] = values.at(column).at(i).value;
                    values[column].remove(i--);
                }
            }
        }
    } else {
        in >> values >> d->display;
    }
}

/*!
    Writes the item to stream \a out. This only writes data from one single item.

    \sa read()
*/
void BOBUIreeWidgetItem::write(QDataStream &out) const
{
    out << values << d->display;
}
#endif // BOBUI_NO_DATASTREAM

/*!
    Constructs a copy of \a other. Note that type() and treeWidget()
    are not copied.

    This function is useful when reimplementing clone().

    \sa data(), flags()
*/
BOBUIreeWidgetItem::BOBUIreeWidgetItem(const BOBUIreeWidgetItem &other)
    : rtti(Type),
      values(other.values),
      d(new BOBUIreeWidgetItemPrivate(this)),
      itemFlags(other.itemFlags)
{
    d->display = other.d->display;
}

/*!
    Assigns \a other's data and flags to this item. Note that type()
    and treeWidget() are not copied.

    This function is useful when reimplementing clone().

    \sa data(), flags()
*/
BOBUIreeWidgetItem &BOBUIreeWidgetItem::operator=(const BOBUIreeWidgetItem &other)
{
    values = other.values;
    d->display = other.d->display;
    d->policy = other.d->policy;
    itemFlags = other.itemFlags;
    return *this;
}

/*!
  Appends the \a child item to the list of children.

  \sa insertChild(), takeChild()
*/
void BOBUIreeWidgetItem::addChild(BOBUIreeWidgetItem *child)
{
    if (child) {
        insertChild(children.size(), child);
        child->d->rowGuess = children.size() - 1;
    }
}

/*!
  Inserts the \a child item at \a index in the list of children.

  If the child has already been inserted somewhere else it won't be inserted again.
*/
void BOBUIreeWidgetItem::insertChild(int index, BOBUIreeWidgetItem *child)
{
    if (index < 0 || index > children.size() || child == nullptr || child->view != nullptr || child->par != nullptr)
        return;

    if (BOBUIreeModel *model = treeModel()) {
        BOBUIreeModel::SkipSorting skipSorting(model);
        if (model->rootItem == this)
            child->par = nullptr;
        else
            child->par = this;
        if (view->isSortingEnabled()) {
            // do a delayed sort instead
            if (!model->sortPendingTimer.isActive())
                model->sortPendingTimer.start(0, model);
        }
        model->beginInsertItems(this, index, 1);
        int cols = model->columnCount();
        QStack<BOBUIreeWidgetItem*> stack;
        stack.push(child);
        while (!stack.isEmpty()) {
            BOBUIreeWidgetItem *i = stack.pop();
            i->view = view;
            i->values.reserve(cols);
            for (int c = 0; c < i->children.size(); ++c)
                stack.push(i->children.at(c));
        }
        children.insert(index, child);
        d->updateHiddenStatus(child, true);
        model->endInsertItems();
    } else {
        child->par = this;
        children.insert(index, child);
    }
    if (child->par)
        d->propagateDisabled(child);
}

/*!
  Removes the given item indicated by \a child.
  The removed item will not be deleted.
*/
void BOBUIreeWidgetItem::removeChild(BOBUIreeWidgetItem *child)
{
    (void)takeChild(children.indexOf(child));
}

/*!
  Removes the item at \a index and returns it, otherwise return 0.
*/
BOBUIreeWidgetItem *BOBUIreeWidgetItem::takeChild(int index)
{
    // we move this outside the check of the index to allow executing
    // pending sorts from inline functions, using this function (hack)
    BOBUIreeModel *model = treeModel();
    if (model) {
        // This will trigger a layoutChanged signal, thus we might want to optimize
        // this function by not emitting the rowsRemoved signal etc to the view.
        // On the other hand we also need to make sure that the selectionmodel
        // is updated in case we take an item that is selected.
        model->skipPendingSort = false;
        model->executePendingSort();
    }
    if (index >= 0 && index < children.size()) {
        if (model) model->beginRemoveItems(this, index, 1);
        d->updateHiddenStatus(children.at(index), false);
        BOBUIreeWidgetItem *item = children.takeAt(index);
        item->par = nullptr;
        QStack<BOBUIreeWidgetItem*> stack;
        stack.push(item);
        while (!stack.isEmpty()) {
            BOBUIreeWidgetItem *i = stack.pop();
            i->view = nullptr;
            for (int c = 0; c < i->children.size(); ++c)
                stack.push(i->children.at(c));
        }
        d->propagateDisabled(item);
        if (model) model->endRemoveRows();
        return item;
    }
    return nullptr;
}

/*!
  Appends the given list of \a children to the item.

  \sa insertChildren(), takeChildren()
*/
void BOBUIreeWidgetItem::addChildren(const QList<BOBUIreeWidgetItem*> &children)
{
    insertChildren(this->children.size(), children);
}

/*!
  Inserts the given list of \a children into the list of the item children at \a index .

  Children that have already been inserted somewhere else won't be inserted.
*/
void BOBUIreeWidgetItem::insertChildren(int index, const QList<BOBUIreeWidgetItem*> &children)
{
    if (index < 0 || index > this->children.size() || children.isEmpty())
        return;

    if (view && view->isSortingEnabled()) {
        for (int n = 0; n < children.size(); ++n)
            insertChild(index, children.at(n));
        return;
    }
    BOBUIreeModel *model = treeModel();
    QStack<BOBUIreeWidgetItem*> stack;
    QList<BOBUIreeWidgetItem*> itemsToInsert;
    for (int n = 0; n < children.size(); ++n) {
        BOBUIreeWidgetItem *child = children.at(n);
        if (child->view || child->par)
            continue;
        itemsToInsert.append(child);
        if (view && model) {
            if (child->childCount() == 0)
                child->view = view;
            else
                stack.push(child);
        }
        if (model && (model->rootItem == this))
            child->par = nullptr;
        else
            child->par = this;
    }
    if (!itemsToInsert.isEmpty()) {
        while (!stack.isEmpty()) {
            BOBUIreeWidgetItem *i = stack.pop();
            i->view = view;
            for (int c = 0; c < i->children.size(); ++c)
                stack.push(i->children.at(c));
        }
        if (model) model->beginInsertItems(this, index, itemsToInsert.size());
        for (int n = 0; n < itemsToInsert.size(); ++n) {
            BOBUIreeWidgetItem *child = itemsToInsert.at(n);
            this->children.insert(index + n, child);
            if (child->par)
                d->propagateDisabled(child);
            d->updateHiddenStatus(child, true);
        }
        if (model) model->endInsertItems();
    }
}

/*!
  Removes the list of children and returns it, otherwise returns an empty list.
*/
QList<BOBUIreeWidgetItem*> BOBUIreeWidgetItem::takeChildren()
{
    QList<BOBUIreeWidgetItem*> removed;
    if (children.size() > 0) {
        BOBUIreeModel *model = treeModel();
        if (model) {
            // This will trigger a layoutChanged signal, thus we might want to optimize
            // this function by not emitting the rowsRemoved signal etc to the view.
            // On the other hand we also need to make sure that the selectionmodel
            // is updated in case we take an item that is selected.
            model->executePendingSort();
        }
        if (model) model->beginRemoveItems(this, 0, children.size());
        for (int n = 0; n < children.size(); ++n) {
            BOBUIreeWidgetItem *item = children.at(n);
            item->par = nullptr;
            QStack<BOBUIreeWidgetItem*> stack;
            stack.push(item);
            while (!stack.isEmpty()) {
                BOBUIreeWidgetItem *i = stack.pop();
                i->view = nullptr;
                for (int c = 0; c < i->children.size(); ++c)
                    stack.push(i->children.at(c));
            }
            d->propagateDisabled(item);
        }
        removed = children;
        children.clear(); // detach
        if (model) model->endRemoveItems();
    }
    return removed;
}


void BOBUIreeWidgetItemPrivate::sortChildren(int column, BobUI::SortOrder order, bool climb)
{
    BOBUIreeModel *model = q->treeModel();
    if (!model)
        return;
    model->sortItems(&q->children, column, order);
    if (climb) {
        QList<BOBUIreeWidgetItem*>::iterator it = q->children.begin();
        for (; it != q->children.end(); ++it) {
            //here we call the private object's method to avoid emitting
            //the layoutAboutToBeChanged and layoutChanged signals
            (*it)->d->sortChildren(column, order, climb);
        }
    }
}

/*!
  \internal

  Sorts the children by the value in the given \a column, in the \a order
  specified. If \a climb is true, the items below each of the children will
  also be sorted.
*/
void BOBUIreeWidgetItem::sortChildren(int column, BobUI::SortOrder order, bool climb)
{
    BOBUIreeModel *model = treeModel();
    if (!model)
        return;
    if (model->isChanging())
        return;
    BOBUIreeModel::SkipSorting skipSorting(model);
    int oldSortColumn = view->d_func()->explicitSortColumn;
    view->d_func()->explicitSortColumn = column;
    emit model->layoutAboutToBeChanged({}, QAbstractItemModel::VerticalSortHint);
    d->sortChildren(column, order, climb);
    emit model->layoutChanged({}, QAbstractItemModel::VerticalSortHint);
    view->d_func()->explicitSortColumn = oldSortColumn;
}

/*!
  \internal

  Calculates the checked state of the item based on the checked state
  of its children. E.g. if all children checked => this item is also
  checked; if some children checked => this item is partially checked;
  if no children checked => this item is unchecked.
*/
QVariant BOBUIreeWidgetItem::childrenCheckState(int column) const
{
    if (column < 0)
        return QVariant();
    bool checkedChildren = false;
    bool uncheckedChildren = false;
    for (const auto *child : children) {
        QVariant value = child->data(column, BobUI::CheckStateRole);
        if (!value.isValid())
            return QVariant();

        switch (static_cast<BobUI::CheckState>(value.toInt()))
        {
        case BobUI::Unchecked:
            uncheckedChildren = true;
            break;
        case BobUI::Checked:
            checkedChildren = true;
            break;
        case BobUI::PartiallyChecked:
        default:
            return BobUI::PartiallyChecked;
        }

        if (uncheckedChildren && checkedChildren)
            return BobUI::PartiallyChecked;
    }

    if (uncheckedChildren)
        return BobUI::Unchecked;
    else if (checkedChildren)
        return BobUI::Checked;
    else
        return QVariant(); // value was not defined
}

/*!
  Causes the model associated with this item to emit a
  \l{QAbstractItemModel::dataChanged()}{dataChanged}() signal for this
  item.

  You normally only need to call this function if you have subclassed
  BOBUIreeWidgetItem and reimplemented data() and/or setData().

  \sa setData()
*/
void BOBUIreeWidgetItem::emitDataChanged()
{
    itemChanged();
}

/*!
  \internal
*/
void BOBUIreeWidgetItem::itemChanged()
{
    if (BOBUIreeModel *model = treeModel())
        model->itemChanged(this);
}

/*!
  \internal
*/
void BOBUIreeWidgetItem::executePendingSort() const
{
    if (BOBUIreeModel *model = treeModel())
        model->executePendingSort();
}

/*!
  \internal
  returns the BOBUIreeModel if a view is set
*/
BOBUIreeModel *BOBUIreeWidgetItem::treeModel(BOBUIreeWidget *v) const
{
    if (!v)
        v = view;
    return (v ? qobject_cast<BOBUIreeModel*>(v->model()) : nullptr);
}


#ifndef BOBUI_NO_DATASTREAM
/*!
    \relates BOBUIreeWidgetItem

    Writes the tree widget item \a item to stream \a out.

    This operator uses BOBUIreeWidgetItem::write().

    \sa {Serializing BobUI Data Types}
*/
QDataStream &operator<<(QDataStream &out, const BOBUIreeWidgetItem &item)
{
    item.write(out);
    return out;
}

/*!
    \relates BOBUIreeWidgetItem

    Reads a tree widget item from stream \a in into \a item.

    This operator uses BOBUIreeWidgetItem::read().

    \sa {Serializing BobUI Data Types}
*/
QDataStream &operator>>(QDataStream &in, BOBUIreeWidgetItem &item)
{
    item.read(in);
    return in;
}
#endif // BOBUI_NO_DATASTREAM


void BOBUIreeWidgetPrivate::clearConnections()
{
    for (const QMetaObject::Connection &connection : connections)
        QObject::disconnect(connection);
}

void BOBUIreeWidgetPrivate::emitItemPressed(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemPressed(item(index), index.column());
}

void BOBUIreeWidgetPrivate::emitItemClicked(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemClicked(item(index), index.column());
}

void BOBUIreeWidgetPrivate::emitItemDoubleClicked(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemDoubleClicked(item(index), index.column());
}

void BOBUIreeWidgetPrivate::emitItemActivated(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemActivated(item(index), index.column());
}

void BOBUIreeWidgetPrivate::emitItemEntered(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemEntered(item(index), index.column());
}

void BOBUIreeWidgetPrivate::emitItemChanged(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    BOBUIreeWidgetItem *indexItem = item(index);
    if (indexItem)
        emit q->itemChanged(indexItem, index.column());
}

void BOBUIreeWidgetPrivate::emitItemExpanded(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemExpanded(item(index));
}

void BOBUIreeWidgetPrivate::emitItemCollapsed(const QModelIndex &index)
{
    Q_Q(BOBUIreeWidget);
    emit q->itemCollapsed(item(index));
}

void BOBUIreeWidgetPrivate::emitCurrentItemChanged(const QModelIndex &current,
                                                const QModelIndex &previous)
{
    Q_Q(BOBUIreeWidget);
    BOBUIreeWidgetItem *currentItem = item(current);
    BOBUIreeWidgetItem *previousItem = item(previous);
    emit q->currentItemChanged(currentItem, previousItem);
}

void BOBUIreeWidgetPrivate::sort()
{
    if (sortingEnabled) {
        int column = header->sortIndicatorSection();
        BobUI::SortOrder order = header->sortIndicatorOrder();
        treeModel()->sort(column, order);
    }
}

void BOBUIreeWidgetPrivate::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_Q(BOBUIreeWidget);
    QModelIndexList indices = selected.indexes();
    int i;
    BOBUIreeModel *m = treeModel();
    for (i = 0; i < indices.size(); ++i) {
        BOBUIreeWidgetItem *item = m->item(indices.at(i));
        item->d->selected = true;
    }

    indices = deselected.indexes();
    for (i = 0; i < indices.size(); ++i) {
        BOBUIreeWidgetItem *item = m->item(indices.at(i));
        item->d->selected = false;
    }

    emit q->itemSelectionChanged();
}

void BOBUIreeWidgetPrivate::dataChanged(const QModelIndex &topLeft,
                                     const QModelIndex &bottomRight)
{
    if (sortingEnabled && topLeft.isValid() && bottomRight.isValid()
        && !treeModel()->sortPendingTimer.isActive()) {
        int column = header->sortIndicatorSection();
        if (column >= topLeft.column() && column <= bottomRight.column()) {
            BobUI::SortOrder order = header->sortIndicatorOrder();
            treeModel()->ensureSorted(column, order, topLeft.row(),
                                  bottomRight.row(), topLeft.parent());
        }
    }
}

/*!
  \class BOBUIreeWidget

  \brief The BOBUIreeWidget class provides a tree view that uses a predefined
  tree model.

  \ingroup model-view
  \inmodule BobUIWidgets

  \image fusion-treeview.png {Directory displaying its contents as a tree}

  The BOBUIreeWidget class is a convenience class that provides a standard
  tree widget with a classic item-based interface similar to that used by
  the QListView class in BobUI 3.
  This class is based on BobUI's Model/View architecture and uses a default
  model to hold items, each of which is a BOBUIreeWidgetItem.

  Developers who do not need the flexibility of the Model/View framework
  can use this class to create simple hierarchical lists very easily. A more
  flexible approach involves combining a BOBUIreeView with a standard item model.
  This allows the storage of data to be separated from its representation.

  In its simplest form, a tree widget can be constructed in the following way:

  \snippet code/src_gui_itemviews_bobuireewidget.cpp 0

  Before items can be added to the tree widget, the number of columns must
  be set with setColumnCount(). This allows each item to have one or more
  labels or other decorations. The number of columns in use can be found
  with the columnCount() function.

  The tree can have a header that contains a section for each column in
  the widget. It is easiest to set up the labels for each section by
  supplying a list of strings with setHeaderLabels(), but a custom header
  can be constructed with a BOBUIreeWidgetItem and inserted into the tree
  with the setHeaderItem() function.

  The items in the tree can be sorted by column according to a predefined
  sort order. If sorting is enabled, the user can sort the items by clicking
  on a column header. Sorting can be enabled or disabled by calling
  \l{BOBUIreeView::setSortingEnabled()}{setSortingEnabled()}. The
  \l{BOBUIreeView::isSortingEnabled()}{isSortingEnabled()} function indicates
  whether sorting is enabled.

  \sa BOBUIreeWidgetItem, BOBUIreeWidgetItemIterator, BOBUIreeView,
  {Model/View Programming}
*/

/*!
    \property BOBUIreeWidget::columnCount
    \brief the number of columns displayed in the tree widget

    By default, this property has a value of 1.
*/

/*!
    \fn void BOBUIreeWidget::itemActivated(BOBUIreeWidgetItem *item, int column)

    This signal is emitted when the user activates an item by single-
    or double-clicking (depending on the platform, i.e. on the
    QStyle::SH_ItemView_ActivateItemOnSingleClick style hint) or
    pressing a special key (e.g., \uicontrol Enter).

    The specified \a item is the item that was clicked, or \nullptr if
    no item was clicked. The \a column is the item's column that was
    clicked, or -1 if no item was clicked.
*/

/*!
    \fn void BOBUIreeWidget::itemPressed(BOBUIreeWidgetItem *item, int column)

    This signal is emitted when the user presses a mouse button inside
    the widget.

    The specified \a item is the item that was clicked, or \nullptr if
    no item was clicked. The \a column is the item's column that was
    clicked, or -1 if no item was clicked.
*/

/*!
    \fn void BOBUIreeWidget::itemClicked(BOBUIreeWidgetItem *item, int column)

    This signal is emitted when the user clicks inside the widget.

    The specified \a item is the item that was clicked. The \a column is the
    item's column that was clicked. If no item was clicked, no signal will be
    emitted.
*/

/*!
    \fn void BOBUIreeWidget::itemDoubleClicked(BOBUIreeWidgetItem *item, int column)

    This signal is emitted when the user double clicks inside the
    widget.

    The specified \a item is the item that was clicked, or \nullptr if
    no item was clicked. The \a column is the item's column that was
    clicked. If no item was double clicked, no signal will be emitted.
*/

/*!
    \fn void BOBUIreeWidget::itemExpanded(BOBUIreeWidgetItem *item)

    This signal is emitted when the specified \a item is expanded so that
    all of its children are displayed.

    \sa BOBUIreeWidgetItem::isExpanded(), itemCollapsed(), expandItem()
*/

/*!
    \fn void BOBUIreeWidget::itemCollapsed(BOBUIreeWidgetItem *item)

    This signal is emitted when the specified \a item is collapsed so that
    none of its children are displayed.

    \note This signal will not be emitted if an item changes its state when
    collapseAll() is invoked.

    \sa BOBUIreeWidgetItem::isExpanded(), itemExpanded(), collapseItem()
*/

/*!
    \fn void BOBUIreeWidget::currentItemChanged(BOBUIreeWidgetItem *current, BOBUIreeWidgetItem *previous)

    This signal is emitted when the current item changes. The current
    item is specified by \a current, and this replaces the \a previous
    current item.

    \sa setCurrentItem()
*/

/*!
    \fn void BOBUIreeWidget::itemSelectionChanged()

    This signal is emitted when the selection changes in the tree widget.
    The current selection can be found with selectedItems().
*/

/*!
    \fn void BOBUIreeWidget::itemEntered(BOBUIreeWidgetItem *item, int column)

    This signal is emitted when the mouse cursor enters an \a item over the
    specified \a column.
    BOBUIreeWidget mouse tracking needs to be enabled for this feature to work.
*/

/*!
    \fn void BOBUIreeWidget::itemChanged(BOBUIreeWidgetItem *item, int column)

    This signal is emitted when the contents of the \a column in the specified
    \a item changes.
*/

/*!
  \fn void BOBUIreeWidget::removeItemWidget(BOBUIreeWidgetItem *item, int column)

  Removes the widget set in the given \a item in the given \a column.

  \sa itemWidget(), setItemWidget()
*/

/*!
  Constructs a tree widget with the given \a parent.
*/
BOBUIreeWidget::BOBUIreeWidget(QWidget *parent)
    : BOBUIreeView(*new BOBUIreeWidgetPrivate(), parent)
{
    Q_D(BOBUIreeWidget);
    BOBUIreeView::setModel(new BOBUIreeModel(1, this));
    d->connections = {
        QObjectPrivate::connect(this, &BOBUIreeWidget::pressed,
                                d, &BOBUIreeWidgetPrivate::emitItemPressed),
        QObjectPrivate::connect(this, &BOBUIreeWidget::clicked,
                                d, &BOBUIreeWidgetPrivate::emitItemClicked),
        QObjectPrivate::connect(this, &BOBUIreeWidget::doubleClicked,
                                d, &BOBUIreeWidgetPrivate::emitItemDoubleClicked),
        QObjectPrivate::connect(this, &BOBUIreeWidget::activated,
                                d, &BOBUIreeWidgetPrivate::emitItemActivated),
        QObjectPrivate::connect(this, &BOBUIreeWidget::entered,
                                d, &BOBUIreeWidgetPrivate::emitItemEntered),
        QObjectPrivate::connect(this, &BOBUIreeWidget::expanded,
                                d, &BOBUIreeWidgetPrivate::emitItemExpanded),
        QObjectPrivate::connect(this, &BOBUIreeWidget::collapsed,
                                d, &BOBUIreeWidgetPrivate::emitItemCollapsed),
        QObjectPrivate::connect(model(), &QAbstractItemModel::dataChanged,
                                d, &BOBUIreeWidgetPrivate::emitItemChanged),
        QObjectPrivate::connect(model(), &QAbstractItemModel::dataChanged,
                                d, &BOBUIreeWidgetPrivate::dataChanged),
        QObjectPrivate::connect(model(), &QAbstractItemModel::columnsRemoved,
                                d, &BOBUIreeWidgetPrivate::sort),
        QObjectPrivate::connect(selectionModel(), &QItemSelectionModel::currentChanged,
                                d, &BOBUIreeWidgetPrivate::emitCurrentItemChanged),
        QObjectPrivate::connect(selectionModel(), &QItemSelectionModel::selectionChanged,
                                d, &BOBUIreeWidgetPrivate::selectionChanged)
    };
    header()->setSectionsClickable(false);
}

/*!
    Destroys the tree widget and all its items.
*/

BOBUIreeWidget::~BOBUIreeWidget()
{
    Q_D(BOBUIreeWidget);
    d->clearConnections();
}

/*
  Returns the number of header columns in the view.

  \sa sortColumn(), currentColumn(), topLevelItemCount()
*/

int BOBUIreeWidget::columnCount() const
{
    Q_D(const BOBUIreeWidget);
    return d->model->columnCount();
}

/*
  Sets the number of header \a columns in the tree widget.
*/

void BOBUIreeWidget::setColumnCount(int columns)
{
    Q_D(BOBUIreeWidget);
    if (columns < 0)
        return;
    d->treeModel()->setColumnCount(columns);
}

/*!
  Returns the tree widget's invisible root item.

  The invisible root item provides access to the tree widget's top-level items
  through the BOBUIreeWidgetItem API, making it possible to write functions that
  can treat top-level items and their children in a uniform way; for example,
  recursive functions.
*/

BOBUIreeWidgetItem *BOBUIreeWidget::invisibleRootItem() const
{
    Q_D(const BOBUIreeWidget);
    return d->treeModel()->rootItem;
}

/*!
  Returns the top level item at the given \a index, or \nullptr if the
  item does not exist.

  \sa topLevelItemCount(), insertTopLevelItem()
*/

BOBUIreeWidgetItem *BOBUIreeWidget::topLevelItem(int index) const
{
    Q_D(const BOBUIreeWidget);
    return d->treeModel()->rootItem->child(index);
}

/*!
    \property BOBUIreeWidget::topLevelItemCount
    \brief the number of top-level items

    By default, this property has a value of 0.

    \sa columnCount(), currentItem()
*/

int BOBUIreeWidget::topLevelItemCount() const
{
    Q_D(const BOBUIreeWidget);
    return d->treeModel()->rootItem->childCount();
}

/*!
  Inserts the \a item at \a index in the top level in the view.

  If the item has already been inserted somewhere else it won't be inserted.

  \sa addTopLevelItem(), columnCount()
*/

void BOBUIreeWidget::insertTopLevelItem(int index, BOBUIreeWidgetItem *item)
{
    Q_D(BOBUIreeWidget);
    d->treeModel()->rootItem->insertChild(index, item);
}

/*!
    Appends the \a item as a top-level item in the widget.

    \sa insertTopLevelItem()
*/
void BOBUIreeWidget::addTopLevelItem(BOBUIreeWidgetItem *item)
{
    insertTopLevelItem(topLevelItemCount(), item);
}

/*!
  Removes the top-level item at the given \a index in the tree and
  returns it, otherwise returns \nullptr;

  \sa insertTopLevelItem(), topLevelItem(), topLevelItemCount()
*/

BOBUIreeWidgetItem *BOBUIreeWidget::takeTopLevelItem(int index)
{
    Q_D(BOBUIreeWidget);
    return d->treeModel()->rootItem->takeChild(index);
}

/*!
  Returns the index of the given top-level \a item, or -1 if the item
  cannot be found.

  \sa sortItems(), topLevelItemCount()
 */
int BOBUIreeWidget::indexOfTopLevelItem(BOBUIreeWidgetItem *item) const
{
    Q_D(const BOBUIreeWidget);
    d->treeModel()->executePendingSort();
    return d->treeModel()->rootItem->children.indexOf(item);
}

/*!
  Inserts the list of \a items at \a index in the top level in the view.

  Items that have already been inserted somewhere else won't be inserted.

  \sa addTopLevelItems()
*/
void BOBUIreeWidget::insertTopLevelItems(int index, const QList<BOBUIreeWidgetItem*> &items)
{
    Q_D(BOBUIreeWidget);
    d->treeModel()->rootItem->insertChildren(index, items);
}

/*!
  Appends the list of \a items as a top-level items in the widget.

  \sa insertTopLevelItems()
*/
void BOBUIreeWidget::addTopLevelItems(const QList<BOBUIreeWidgetItem*> &items)
{
    insertTopLevelItems(topLevelItemCount(), items);
}

/*!
    Returns the item used for the tree widget's header.

    \sa setHeaderItem()
*/

BOBUIreeWidgetItem *BOBUIreeWidget::headerItem() const
{
    Q_D(const BOBUIreeWidget);
    return d->treeModel()->headerItem;
}

/*!
    Sets the header \a item for the tree widget. The label for each column in
    the header is supplied by the corresponding label in the item.

    The tree widget takes ownership of the item.

    \sa headerItem(), setHeaderLabels()
*/

void BOBUIreeWidget::setHeaderItem(BOBUIreeWidgetItem *item)
{
    Q_D(BOBUIreeWidget);
    if (!item)
        return;
    item->view = this;

    int oldCount = columnCount();
    if (oldCount < item->columnCount())
         d->treeModel()->beginInsertColumns(QModelIndex(), oldCount, item->columnCount() - 1);
    else if (oldCount > item->columnCount())
         d->treeModel()->beginRemoveColumns(QModelIndex(), item->columnCount(), oldCount - 1);
    delete d->treeModel()->headerItem;
    d->treeModel()->headerItem = item;
    if (oldCount < item->columnCount())
        d->treeModel()->endInsertColumns();
    else if (oldCount > item->columnCount())
        d->treeModel()->endRemoveColumns();
    d->treeModel()->headerDataChanged(BobUI::Horizontal, 0, oldCount);
}


/*!
  Adds a column in the header for each item in the \a labels list, and sets
  the label for each column.

  Note that setHeaderLabels() won't remove existing columns.

  \sa setHeaderItem(), setHeaderLabel()
*/
void BOBUIreeWidget::setHeaderLabels(const QStringList &labels)
{
    Q_D(BOBUIreeWidget);
    if (columnCount() < labels.size())
        setColumnCount(labels.size());
    BOBUIreeWidgetItem *item = d->treeModel()->headerItem;
    for (int i = 0; i < labels.size(); ++i)
        item->setText(i, labels.at(i));
}

/*!
    \fn void BOBUIreeWidget::setHeaderLabel(const QString &label)

    Same as setHeaderLabels(QStringList(\a label)).
*/

/*!
    Returns the current item in the tree widget.

    \sa setCurrentItem(), currentItemChanged()
*/
BOBUIreeWidgetItem *BOBUIreeWidget::currentItem() const
{
    Q_D(const BOBUIreeWidget);
    return d->item(currentIndex());
}

/*!
    Returns the current column in the tree widget.

    \sa setCurrentItem(), columnCount()
*/
int BOBUIreeWidget::currentColumn() const
{
    return currentIndex().column();
}

/*!
  Sets the current \a item in the tree widget.

  Unless the selection mode is \l{QAbstractItemView::}{NoSelection},
  the item is also selected.

  \sa currentItem(), currentItemChanged()
*/
void BOBUIreeWidget::setCurrentItem(BOBUIreeWidgetItem *item)
{
    setCurrentItem(item, 0);
}

/*!
  Sets the current \a item in the tree widget and the current column to \a column.

  \sa currentItem()
*/
void BOBUIreeWidget::setCurrentItem(BOBUIreeWidgetItem *item, int column)
{
    Q_D(BOBUIreeWidget);
    setCurrentIndex(d->index(item, column));
}

/*!
  Sets the current \a item in the tree widget and the current column to \a column,
  using the given \a command.

  \sa currentItem()
*/
void BOBUIreeWidget::setCurrentItem(BOBUIreeWidgetItem *item, int column,
                                 QItemSelectionModel::SelectionFlags command)
{
    Q_D(BOBUIreeWidget);
    d->selectionModel->setCurrentIndex(d->index(item, column), command);
}


/*!
  Returns a pointer to the item at the coordinates \a p. The coordinates
  are relative to the tree widget's \l{QAbstractScrollArea::}{viewport()}.

  \sa visualItemRect()
*/
BOBUIreeWidgetItem *BOBUIreeWidget::itemAt(const QPoint &p) const
{
    Q_D(const BOBUIreeWidget);
    return d->item(indexAt(p));
}

/*!
    \fn BOBUIreeWidgetItem *BOBUIreeWidget::itemAt(int x, int y) const
    \overload

    Returns a pointer to the item at the coordinates (\a x, \a y). The coordinates
    are relative to the tree widget's \l{QAbstractScrollArea::}{viewport()}.
*/

/*!
  Returns the rectangle on the viewport occupied by the item at \a item.

  \sa itemAt()
*/
QRect BOBUIreeWidget::visualItemRect(const BOBUIreeWidgetItem *item) const
{
    Q_D(const BOBUIreeWidget);
    //the visual rect for an item is across all columns. So we need to determine
    //what is the first and last column and get their visual index rects
    const QModelIndex base = d->index(item);
    const int firstVisiblesection = header()->logicalIndexAt(- header()->offset());
    const int lastVisibleSection = header()->logicalIndexAt(header()->length() - header()->offset() - 1);
    const QModelIndex first = base.sibling(base.row(), firstVisiblesection);
    const QModelIndex last = base.sibling(base.row(), lastVisibleSection);
    return visualRect(first) | visualRect(last);
}

/*!
  Returns the column used to sort the contents of the widget.

  \sa sortItems()
*/
int BOBUIreeWidget::sortColumn() const
{
    Q_D(const BOBUIreeWidget);
    return (d->explicitSortColumn != -1
            ? d->explicitSortColumn
            : header()->sortIndicatorSection());
}

/*!
  Sorts the items in the widget in the specified \a order by the values in
  the given \a column.

  \sa sortColumn()
*/

void BOBUIreeWidget::sortItems(int column, BobUI::SortOrder order)
{
    Q_D(BOBUIreeWidget);
    header()->setSortIndicator(column, order);
    d->model->sort(column, order);
}

/*!
  Starts editing the \a item in the given \a column if it is editable.
*/

void BOBUIreeWidget::editItem(BOBUIreeWidgetItem *item, int column)
{
    Q_D(BOBUIreeWidget);
    edit(d->index(item, column));
}

/*!
  Opens a persistent editor for the \a item in the given \a column.

  \sa closePersistentEditor(), isPersistentEditorOpen()
*/

void BOBUIreeWidget::openPersistentEditor(BOBUIreeWidgetItem *item, int column)
{
    Q_D(BOBUIreeWidget);
    QAbstractItemView::openPersistentEditor(d->index(item, column));
}

/*!
  Closes the persistent editor for the \a item in the given \a column.

  This function has no effect if no persistent editor is open for this
  combination of item and column.

  \sa openPersistentEditor(), isPersistentEditorOpen()
*/

void BOBUIreeWidget::closePersistentEditor(BOBUIreeWidgetItem *item, int column)
{
    Q_D(BOBUIreeWidget);
    QAbstractItemView::closePersistentEditor(d->index(item, column));
}

/*!
    \since 5.10

    Returns whether a persistent editor is open for item \a item in
    column \a column.

    \sa openPersistentEditor(), closePersistentEditor()
*/

bool BOBUIreeWidget::isPersistentEditorOpen(BOBUIreeWidgetItem *item, int column) const
{
    Q_D(const BOBUIreeWidget);
    return QAbstractItemView::isPersistentEditorOpen(d->index(item, column));
}

/*!
    Returns the widget displayed in the cell specified by \a item and the given \a column.

    \sa setItemWidget(), removeItemWidget()
*/
QWidget *BOBUIreeWidget::itemWidget(BOBUIreeWidgetItem *item, int column) const
{
    Q_D(const BOBUIreeWidget);
    return QAbstractItemView::indexWidget(d->index(item, column));
}

/*!
    Sets the given \a widget to be displayed in the cell specified by the given
    \a item and \a column.

    The given \a widget's \l {QWidget::}{autoFillBackground} property must be
    set to true, otherwise the widget's background will be transparent, showing
    both the model data and the tree widget item.

    This function should only be used to display static content in the place of
    a tree widget item. If you want to display custom dynamic content or
    implement a custom editor widget, use BOBUIreeView and subclass QStyledItemDelegate
    instead.

    This function cannot be called before the item hierarchy has been set up,
    i.e., the BOBUIreeWidgetItem that will hold \a widget must have been added to
    the view before \a widget is set.

    \note The tree takes ownership of \a widget.

    \sa itemWidget(), removeItemWidget(), {Delegate Classes}
*/
void BOBUIreeWidget::setItemWidget(BOBUIreeWidgetItem *item, int column, QWidget *widget)
{
    Q_D(BOBUIreeWidget);
    QAbstractItemView::setIndexWidget(d->index(item, column), widget);
}

/*!
  Returns a list of all selected non-hidden items.

  \sa itemSelectionChanged()
*/
QList<BOBUIreeWidgetItem*> BOBUIreeWidget::selectedItems() const
{
    Q_D(const BOBUIreeWidget);
    const QModelIndexList indexes = selectionModel()->selectedIndexes();
    QList<BOBUIreeWidgetItem*> items;
    items.reserve(indexes.size());
    QDuplicateTracker<BOBUIreeWidgetItem *> seen(indexes.size());
    for (const auto &index : indexes) {
        BOBUIreeWidgetItem *item = d->item(index);
        if (item->isHidden() || seen.hasSeen(item))
            continue;
        items.append(item);
    }
    return items;
}

/*!
  Returns a list of items that match the given \a text, using the given \a flags, in the given \a column.
*/
QList<BOBUIreeWidgetItem*> BOBUIreeWidget::findItems(const QString &text, BobUI::MatchFlags flags, int column) const
{
    Q_D(const BOBUIreeWidget);
    QModelIndexList indexes = d->model->match(model()->index(0, column, QModelIndex()),
                                                BobUI::DisplayRole, text, -1, flags);
    QList<BOBUIreeWidgetItem*> items;
    const int indexesSize = indexes.size();
    items.reserve(indexesSize);
    for (int i = 0; i < indexesSize; ++i)
        items.append(d->item(indexes.at(i)));
    return items;
}


/*!
  Returns the item above the given \a item.
*/
BOBUIreeWidgetItem *BOBUIreeWidget::itemAbove(const BOBUIreeWidgetItem *item) const
{
    Q_D(const BOBUIreeWidget);
    if (item == d->treeModel()->headerItem)
        return nullptr;
    const QModelIndex index = d->index(item);
    const QModelIndex above = indexAbove(index);
    return d->item(above);
}

/*!
  Returns the item visually below the given \a item.
*/
BOBUIreeWidgetItem *BOBUIreeWidget::itemBelow(const BOBUIreeWidgetItem *item) const
{
    Q_D(const BOBUIreeWidget);
    if (item == d->treeModel()->headerItem)
        return nullptr;
    const QModelIndex index = d->index(item);
    const QModelIndex below = indexBelow(index);
    return d->item(below);
}

/*!
   \reimp
 */
void BOBUIreeWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
    Q_D(BOBUIreeWidget);
    BOBUIreeView::setSelectionModel(selectionModel);
    QItemSelection newSelection = selectionModel->selection();
    if (!newSelection.isEmpty())
        d->selectionChanged(newSelection, QItemSelection());
}

/*!
    Ensures that the \a item is visible, scrolling the view if necessary using
    the specified \a hint.

    \sa currentItem(), itemAt(), topLevelItem()
*/
void BOBUIreeWidget::scrollToItem(const BOBUIreeWidgetItem *item, QAbstractItemView::ScrollHint hint)
{
    Q_D(BOBUIreeWidget);
    BOBUIreeView::scrollTo(d->index(item), hint);
}

/*!
    Expands the \a item. This causes the tree containing the item's children
    to be expanded.

    \sa collapseItem(), currentItem(), itemAt(), topLevelItem(), itemExpanded()
*/
void BOBUIreeWidget::expandItem(const BOBUIreeWidgetItem *item)
{
    Q_D(BOBUIreeWidget);
    BOBUIreeModel::SkipSorting skipSorting(d->treeModel());
    expand(d->index(item));
}

/*!
    Closes the \a item. This causes the tree containing the item's children
    to be collapsed.

    \sa expandItem(), currentItem(), itemAt(), topLevelItem()
*/
void BOBUIreeWidget::collapseItem(const BOBUIreeWidgetItem *item)
{
    Q_D(BOBUIreeWidget);
    BOBUIreeModel::SkipSorting skipSorting(d->treeModel());
    collapse(d->index(item));
}

/*!
    Clears the tree widget by removing all of its items and selections.

    \b{Note:} Since each item is removed from the tree widget before being
    deleted, the return value of BOBUIreeWidgetItem::treeWidget() will be invalid
    when called from an item's destructor.

    \sa takeTopLevelItem(), topLevelItemCount(), columnCount()
*/
void BOBUIreeWidget::clear()
{
    Q_D(BOBUIreeWidget);
    selectionModel()->clear();
    d->treeModel()->clear();
}

/*!
    Returns a list of MIME types that can be used to describe a list of
    treewidget items.

    \sa mimeData()
*/
QStringList BOBUIreeWidget::mimeTypes() const
{
    return model()->QAbstractItemModel::mimeTypes();
}

/*!
    Returns an object that contains a serialized description of the specified
    \a items. The format used to describe the items is obtained from the
    mimeTypes() function.

    If the list of items is empty, \nullptr is returned rather than a
    serialized empty list.
*/
QMimeData *BOBUIreeWidget::mimeData(const QList<BOBUIreeWidgetItem *> &items) const
{
    Q_D(const BOBUIreeWidget);
    if (d->treeModel()->cachedIndexes.isEmpty()) {
        QList<QModelIndex> indexes;
        for (const auto *item : items) {
            if (Q_UNLIKELY(!item)) {
                qWarning("BOBUIreeWidget::mimeData: Null-item passed");
                return nullptr;
            }

            for (int c = 0; c < item->values.size(); ++c) {
                const QModelIndex index = indexFromItem(item, c);
                if (Q_UNLIKELY(!index.isValid())) {
                    qWarning() << "BOBUIreeWidget::mimeData: No index associated with item :" << item;
                    return nullptr;
                }
                indexes << index;
            }
        }
        return d->model->QAbstractItemModel::mimeData(indexes);
    }
    return d->treeModel()->internalMimeData();
}

/*!
    Handles the \a data supplied by a drag and drop operation that ended with
    the given \a action in the \a index in the given \a parent item.

    The default implementation returns \c true if the drop was
    successfully handled by decoding the mime data and inserting it
    into the model; otherwise it returns \c false.

    \sa supportedDropActions(), supportedDragActions
*/
bool BOBUIreeWidget::dropMimeData(BOBUIreeWidgetItem *parent, int index,
                               const QMimeData *data, BobUI::DropAction action)
{
    QModelIndex idx;
    if (parent) idx = indexFromItem(parent);
    return model()->QAbstractItemModel::dropMimeData(data, action , index, 0, idx);
}

/*!
  Returns the drop actions supported by this view.

  \sa BobUI::DropActions, supportedDragActions, dropMimeData()
*/
BobUI::DropActions BOBUIreeWidget::supportedDropActions() const
{
    return model()->QAbstractItemModel::supportedDropActions() | BobUI::MoveAction;
}

#if BOBUI_CONFIG(draganddrop)
/*!
    \property BOBUIreeWidget::supportedDragActions
    \brief the drag actions supported by this view

    \since 6.10
    \sa BobUI::DropActions, supportedDropActions()
*/
BobUI::DropActions BOBUIreeWidget::supportedDragActions() const
{
    Q_D(const BOBUIreeWidget);
    return d->supportedDragActions.value_or(supportedDropActions());
}

void BOBUIreeWidget::setSupportedDragActions(BobUI::DropActions actions)
{
    Q_D(BOBUIreeWidget);
    d->supportedDragActions = actions;
}
#endif // BOBUI_CONFIG(draganddrop)

/*!
    Returns the QModelIndex associated with the given \a item in the given \a column.

    \note In BobUI versions prior to 5.7, this function took a non-\c{const} \a item.

    \sa itemFromIndex(), topLevelItem()
*/
QModelIndex BOBUIreeWidget::indexFromItem(const BOBUIreeWidgetItem *item, int column) const
{
    Q_D(const BOBUIreeWidget);
    return d->index(item, column);
}

/*!
    Returns a pointer to the BOBUIreeWidgetItem associated with the given \a index.

    \sa indexFromItem()
*/
BOBUIreeWidgetItem *BOBUIreeWidget::itemFromIndex(const QModelIndex &index) const
{
    Q_D(const BOBUIreeWidget);
    return d->item(index);
}

#if BOBUI_CONFIG(draganddrop)
/*! \reimp */
void BOBUIreeWidget::dropEvent(QDropEvent *event) {
    Q_D(BOBUIreeWidget);
    if (event->source() == this && (event->dropAction() == BobUI::MoveAction ||
                                    dragDropMode() == QAbstractItemView::InternalMove)) {
        QModelIndex topIndex;
        int col = -1;
        int row = -1;
        // check whether a subclass has already accepted the event, ie. moved the data
        if (!event->isAccepted() && d->dropOn(event, &row, &col, &topIndex)) {
            const QList<QModelIndex> idxs = selectedIndexes();
            QList<QPersistentModelIndex> indexes;
            const int indexesCount = idxs.size();
            indexes.reserve(indexesCount);
            for (const auto &idx : idxs)
                indexes.append(idx);

            if (indexes.contains(topIndex))
                return;

            // When removing items the drop location could shift
            QPersistentModelIndex dropRow = model()->index(row, col, topIndex);

            // Remove the items
            QList<BOBUIreeWidgetItem *> taken;
            for (const auto &index : indexes) {
                BOBUIreeWidgetItem *parent = itemFromIndex(index);
                if (!parent || !parent->parent()) {
                    taken.append(takeTopLevelItem(index.row()));
                } else {
                    taken.append(parent->parent()->takeChild(index.row()));
                }
            }

            // insert them back in at their new positions
            for (int i = 0; i < indexes.size(); ++i) {
                // Either at a specific point or appended
                if (row == -1) {
                    if (topIndex.isValid()) {
                        BOBUIreeWidgetItem *parent = itemFromIndex(topIndex);
                        parent->insertChild(parent->childCount(), taken.takeFirst());
                    } else {
                        insertTopLevelItem(topLevelItemCount(), taken.takeFirst());
                    }
                } else {
                    int r = dropRow.row() >= 0 ? dropRow.row() : row;
                    if (topIndex.isValid()) {
                        BOBUIreeWidgetItem *parent = itemFromIndex(topIndex);
                        parent->insertChild(qMin(r, parent->childCount()), taken.takeFirst());
                    } else {
                        insertTopLevelItem(qMin(r, topLevelItemCount()), taken.takeFirst());
                    }
                }
            }

            event->accept();
        }
        // either we or a subclass accepted the move event, so assume that the data was
        // moved and that QAbstractItemView shouldn't remove the source when QDrag::exec returns
        if (event->isAccepted())
            d->dropEventMoved = true;
    }

    BOBUIreeView::dropEvent(event);
}
#endif

/*!
  \reimp
*/

void BOBUIreeWidget::setModel(QAbstractItemModel * /*model*/)
{
    Q_ASSERT(!"BOBUIreeWidget::setModel() - Changing the model of the BOBUIreeWidget is not allowed.");
}

/*!
    \reimp
*/
bool BOBUIreeWidget::event(QEvent *e)
{
    Q_D(BOBUIreeWidget);
    if (e->type() == QEvent::Polish)
        d->treeModel()->executePendingSort();
    return BOBUIreeView::event(e);
}

/*!
    see BOBUIBUG-94546
*/
void BOBUIreeModelPrivate::executePendingOperations() const
{
    q_func()->executePendingSort();
}

BOBUI_END_NAMESPACE

#include "moc_bobuireewidget.cpp"
#include "moc_bobuireewidget_p.cpp"
