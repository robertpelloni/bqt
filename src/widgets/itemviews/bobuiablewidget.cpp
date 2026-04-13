// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuiablewidget.h"

#include <qpainter.h>
#include <private/bobuiablewidget_p.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

BOBUIableModel::BOBUIableModel(int rows, int columns, BOBUIableWidget *parent)
    : QAbstractTableModel(parent),
      prototype(nullptr),
      tableItems(rows * columns, 0),
      verticalHeaderItems(rows, 0),
      horizontalHeaderItems(columns, 0)
{}

BOBUIableModel::~BOBUIableModel()
{
    clear();
    delete prototype;
}

bool BOBUIableModel::insertRows(int row, int count, const QModelIndex &)
{
    if (count < 1 || row < 0 || row > verticalHeaderItems.size())
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);
    int rc = verticalHeaderItems.size();
    int cc = horizontalHeaderItems.size();
    verticalHeaderItems.insert(row, count, 0);
    if (rc == 0)
        tableItems.resize(cc * count);
    else
        tableItems.insert(tableIndex(row, 0), cc * count, 0);
    endInsertRows();
    return true;
}

bool BOBUIableModel::insertColumns(int column, int count, const QModelIndex &)
{
    if (count < 1 || column < 0 || column > horizontalHeaderItems.size())
        return false;

    beginInsertColumns(QModelIndex(), column, column + count - 1);
    int rc = verticalHeaderItems.size();
    int cc = horizontalHeaderItems.size();
    horizontalHeaderItems.insert(column, count, 0);
    if (cc == 0)
        tableItems.resize(rc * count);
    else
        for (int row = 0; row < rc; ++row)
            tableItems.insert(tableIndex(row, column), count, 0);
    endInsertColumns();
    return true;
}

bool BOBUIableModel::removeRows(int row, int count, const QModelIndex &)
{
    if (count < 1 || row < 0 || row + count > verticalHeaderItems.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    int i = tableIndex(row, 0);
    int n = count * columnCount();
    BOBUIableWidgetItem *oldItem = nullptr;
    for (int j = i; j < n + i; ++j) {
        oldItem = tableItems.at(j);
        if (oldItem)
            oldItem->view = nullptr;
        delete oldItem;
    }
    tableItems.remove(qMax(i, 0), n);
    for (int v = row; v < row + count; ++v) {
        oldItem = verticalHeaderItems.at(v);
        if (oldItem)
            oldItem->view = nullptr;
        delete oldItem;
    }
    verticalHeaderItems.remove(row, count);
    endRemoveRows();
    return true;
}

bool BOBUIableModel::removeColumns(int column, int count, const QModelIndex &)
{
    if (count < 1 || column < 0 || column + count >  horizontalHeaderItems.size())
        return false;

    beginRemoveColumns(QModelIndex(), column, column + count - 1);
    BOBUIableWidgetItem *oldItem = nullptr;
    for (int row = rowCount() - 1; row >= 0; --row) {
        int i = tableIndex(row, column);
        for (int j = i; j < i + count; ++j) {
            oldItem = tableItems.at(j);
            if (oldItem)
                oldItem->view = nullptr;
            delete oldItem;
        }
        tableItems.remove(i, count);
    }
    for (int h=column; h<column+count; ++h) {
        oldItem = horizontalHeaderItems.at(h);
        if (oldItem)
            oldItem->view = nullptr;
        delete oldItem;
    }
    horizontalHeaderItems.remove(column, count);
    endRemoveColumns();
    return true;
}

bool BOBUIableModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (sourceRow < 0
        || sourceRow + count - 1 >= rowCount(sourceParent)
        || destinationChild < 0
        || destinationChild > rowCount(destinationParent)
        || sourceRow == destinationChild
        || sourceRow == destinationChild - 1
        || count <= 0
        || sourceParent.isValid()
        || destinationParent.isValid()) {
        return false;
    }
    if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
        return false;

    // Table items
    int numItems = count * columnCount();
    int fromIndex = tableIndex(sourceRow, 0);
    int destinationIndex = tableIndex(destinationChild, 0);
    if (destinationChild < sourceRow)
        fromIndex += numItems - 1;
    else
        destinationIndex--;
    while (numItems--)
        tableItems.move(fromIndex, destinationIndex);

    // Header items
    int fromRow = sourceRow;
    if (destinationChild < sourceRow)
        fromRow += count - 1;
    else
        destinationChild--;
    while (count--)
        verticalHeaderItems.move(fromRow, destinationChild);

    endMoveRows();
    return true;
}

void BOBUIableModel::setItem(int row, int column, BOBUIableWidgetItem *item)
{
    int i = tableIndex(row, column);
    if (i < 0 || i >= tableItems.size())
        return;
    BOBUIableWidgetItem *oldItem = tableItems.at(i);
    if (item == oldItem)
        return;

    // remove old
    if (oldItem)
        oldItem->view = nullptr;
    delete tableItems.at(i);

    BOBUIableWidget *view = this->view();

    // set new
    if (item)
        item->d->id = i;
    tableItems[i] = item;

    if (view && view->isSortingEnabled()
        && view->horizontalHeader()->sortIndicatorSection() == column) {
        // sorted insertion
        BobUI::SortOrder order = view->horizontalHeader()->sortIndicatorOrder();
        QList<BOBUIableWidgetItem *> colItems = columnItems(column);
        if (row < colItems.size())
            colItems.remove(row);
        int sortedRow;
        if (item == nullptr) {
            // move to after all non-0 (sortable) items
            sortedRow = colItems.size();
        } else {
            QList<BOBUIableWidgetItem *>::iterator it;
            it = sortedInsertionIterator(colItems.begin(), colItems.end(), order, item);
            sortedRow = qMax((int)(it - colItems.begin()), 0);
        }
        if (sortedRow != row) {
            const int destinationChild = sortedRow > row ? sortedRow + 1 : sortedRow;
            moveRows(QModelIndex(), row, 1, QModelIndex(), destinationChild);
            return;
        }
    }
    QModelIndex idx = QAbstractTableModel::index(row, column);
    emit dataChanged(idx, idx);
}

BOBUIableWidgetItem *BOBUIableModel::takeItem(int row, int column)
{
    long i = tableIndex(row, column);
    BOBUIableWidgetItem *itm = tableItems.value(i);
    if (itm) {
        itm->view = nullptr;
        itm->d->id = -1;
        tableItems[i] = 0;
        const QModelIndex ind = index(row, column);
        emit dataChanged(ind, ind);
    }
    return itm;
}

BOBUIableWidgetItem *BOBUIableModel::item(int row, int column) const
{
    return item(index(row, column));
}

BOBUIableWidgetItem *BOBUIableModel::item(const QModelIndex &index) const
{
    if (!isValid(index))
        return nullptr;
    return tableItems.at(tableIndex(index.row(), index.column()));
}

void BOBUIableModel::removeItem(BOBUIableWidgetItem *item)
{
    int i = tableItems.indexOf(item);
    if (i != -1) {
        QModelIndex idx = index(item);
        tableItems[i] = nullptr;
        emit dataChanged(idx, idx);
        return;
    }

    i = verticalHeaderItems.indexOf(item);

    if (i != -1) {
        verticalHeaderItems[i] = 0;
        emit headerDataChanged(BobUI::Vertical, i, i);
        return;
    }
    i = horizontalHeaderItems.indexOf(item);
    if (i != -1) {
        horizontalHeaderItems[i] = 0;
        emit headerDataChanged(BobUI::Horizontal, i, i);
        return;
    }
}

void BOBUIableModel::setHorizontalHeaderItem(int section, BOBUIableWidgetItem *item)
{
    if (section < 0 || section >= horizontalHeaderItems.size())
        return;
    BOBUIableWidgetItem *oldItem = horizontalHeaderItems.at(section);
    if (item == oldItem)
        return;

    if (oldItem)
        oldItem->view = nullptr;
    delete oldItem;

    BOBUIableWidget *view = this->view();

    if (item) {
        item->view = view;
        item->d->headerItem = true;
    }
    horizontalHeaderItems[section] = item;
    emit headerDataChanged(BobUI::Horizontal, section, section);
}

void BOBUIableModel::setVerticalHeaderItem(int section, BOBUIableWidgetItem *item)
{
    if (section < 0 || section >= verticalHeaderItems.size())
        return;
    BOBUIableWidgetItem *oldItem = verticalHeaderItems.at(section);
    if (item == oldItem)
        return;

    if (oldItem)
        oldItem->view = nullptr;
    delete oldItem;

    BOBUIableWidget *view = this->view();

    if (item) {
        item->view = view;
        item->d->headerItem = true;
    }
    verticalHeaderItems[section] = item;
    emit headerDataChanged(BobUI::Vertical, section, section);
}

BOBUIableWidgetItem *BOBUIableModel::takeHorizontalHeaderItem(int section)
{
    if (section < 0 || section >= horizontalHeaderItems.size())
        return nullptr;
    BOBUIableWidgetItem *itm = horizontalHeaderItems.at(section);
    if (itm) {
        itm->view = nullptr;
        itm->d->headerItem = false;
        horizontalHeaderItems[section] = 0;
    }
    return itm;
}

BOBUIableWidgetItem *BOBUIableModel::takeVerticalHeaderItem(int section)
{
    if (section < 0 || section >= verticalHeaderItems.size())
        return nullptr;
    BOBUIableWidgetItem *itm = verticalHeaderItems.at(section);
    if (itm) {
        itm->view = nullptr;
        itm->d->headerItem = false;
        verticalHeaderItems[section] = 0;
    }
    return itm;
}

BOBUIableWidgetItem *BOBUIableModel::horizontalHeaderItem(int section)
{
    return horizontalHeaderItems.value(section);
}

BOBUIableWidgetItem *BOBUIableModel::verticalHeaderItem(int section)
{
    return verticalHeaderItems.value(section);
}

QModelIndex BOBUIableModel::index(const BOBUIableWidgetItem *item) const
{
    if (!item)
        return QModelIndex();
    int i = -1;
    const int id = item->d->id;
    if (id >= 0 && id < tableItems.size() && tableItems.at(id) == item) {
        i = id;
    } else { // we need to search for the item
        i = tableItems.indexOf(const_cast<BOBUIableWidgetItem*>(item));
        if (i == -1) // not found
            return QModelIndex();
    }
    int row = i / columnCount();
    int col = i % columnCount();
    return QAbstractTableModel::index(row, col);
}

void BOBUIableModel::setRowCount(int rows)
{
    int rc = verticalHeaderItems.size();
    if (rows < 0 || rc == rows)
        return;
    if (rc < rows)
        insertRows(qMax(rc, 0), rows - rc);
    else
        removeRows(qMax(rows, 0), rc - rows);
}

void BOBUIableModel::setColumnCount(int columns)
{
    int cc = horizontalHeaderItems.size();
    if (columns < 0 || cc == columns)
        return;
    if (cc < columns)
        insertColumns(qMax(cc, 0), columns - cc);
    else
        removeColumns(qMax(columns, 0), cc - columns);
}

int BOBUIableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : verticalHeaderItems.size();
}

int BOBUIableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : horizontalHeaderItems.size();
}

QVariant BOBUIableModel::data(const QModelIndex &index, int role) const
{
    BOBUIableWidgetItem *itm = item(index);
    if (itm)
        return itm->data(role);
    return QVariant();
}

bool BOBUIableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    BOBUIableWidgetItem *itm = item(index);
    if (itm) {
        itm->setData(role, value);
        return true;
    }

    // don't create dummy table items for empty values
    if (!value.isValid())
        return false;

    BOBUIableWidget *view = this->view();
    if (!view)
        return false;

    itm = createItem();
    itm->setData(role, value);
    view->setItem(index.row(), index.column(), itm);
    return true;
}

QMap<int, QVariant> BOBUIableModel::itemData(const QModelIndex &index) const
{
    QMap<int, QVariant> roles;
    BOBUIableWidgetItem *itm = item(index);
    if (itm) {
        for (int i = 0; i < itm->values.size(); ++i) {
            roles.insert(itm->values.at(i).role,
                         itm->values.at(i).value);
        }
    }
    return roles;
}

// reimplemented to ensure that only one dataChanged() signal is emitted
bool BOBUIableModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    if (!index.isValid())
        return false;

    BOBUIableWidget *view = this->view();
    BOBUIableWidgetItem *itm = item(index);
    if (itm) {
        itm->view = nullptr; // prohibits item from calling itemChanged()
        QList<int> rolesVec;
        for (QMap<int, QVariant>::ConstIterator it = roles.constBegin(); it != roles.constEnd(); ++it) {
            const int role = (it.key() == BobUI::EditRole ? BobUI::DisplayRole : it.key());
            if (itm->data(role) != it.value()) {
                itm->setData(role, it.value());
                rolesVec += role;
                if (role == BobUI::DisplayRole)
                    rolesVec += BobUI::EditRole;
            }
        }
        itm->view = view;
        if (!rolesVec.isEmpty())
            itemChanged(itm, rolesVec);
        return true;
    }

    if (!view)
        return false;

    itm = createItem();
    for (QMap<int, QVariant>::ConstIterator it = roles.constBegin(); it != roles.constEnd(); ++it)
        itm->setData(it.key(), it.value());
    view->setItem(index.row(), index.column(), itm);
    return true;
}

bool BOBUIableModel::clearItemData(const QModelIndex &index)
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return false;
    BOBUIableWidgetItem *itm = item(index);
    if (!itm)
        return false;
    const auto beginIter = itm->values.cbegin();
    const auto endIter = itm->values.cend();
    if (std::all_of(beginIter, endIter, [](const QWidgetItemData& data) -> bool { return !data.value.isValid(); }))
        return true; //it's already cleared
    itm->values.clear();
    emit dataChanged(index, index, QList<int> {});
    return true;
}

BobUI::ItemFlags BOBUIableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return BobUI::ItemIsDropEnabled;
    if (BOBUIableWidgetItem *itm = item(index))
        return itm->flags();
    return (BobUI::ItemIsEditable
            |BobUI::ItemIsSelectable
            |BobUI::ItemIsUserCheckable
            |BobUI::ItemIsEnabled
            |BobUI::ItemIsDragEnabled
            |BobUI::ItemIsDropEnabled);
}

void BOBUIableModel::sort(int column, BobUI::SortOrder order)
{
    QList<std::pair<BOBUIableWidgetItem *, int>> sortable;
    QList<int> unsortable;
    const int numRows = rowCount();

    sortable.reserve(numRows);
    unsortable.reserve(numRows);

    for (int row = 0; row < numRows; ++row) {
        if (BOBUIableWidgetItem *itm = item(row, column))
            sortable.emplace_back(itm, row);
        else
            unsortable.append(row);
    }

    const auto compare = (order == BobUI::AscendingOrder ? &itemLessThan : &itemGreaterThan);
    std::stable_sort(sortable.begin(), sortable.end(), compare);

    QList<BOBUIableWidgetItem *> sorted_table(tableItems.size());
    QModelIndexList from;
    QModelIndexList to;
    const int numColumns = columnCount();
    from.reserve(numRows * numColumns);
    to.reserve(numRows * numColumns);
    for (int i = 0; i < numRows; ++i) {
        int r = (i < sortable.size()
                 ? sortable.at(i).second
                 : unsortable.at(i - sortable.size()));
        for (int c = 0; c < numColumns; ++c) {
            sorted_table[tableIndex(i, c)] = item(r, c);
            from.append(createIndex(r, c));
            to.append(createIndex(i, c));
        }
    }

    emit layoutAboutToBeChanged({}, QAbstractItemModel::VerticalSortHint);

    tableItems = sorted_table;
    changePersistentIndexList(from, to); // ### slow

    emit layoutChanged({}, QAbstractItemModel::VerticalSortHint);
}

/*
  \internal

  Ensures that rows in the interval [start, end] are
  sorted according to the contents of column \a column
  and the given sort \a order.
*/
void BOBUIableModel::ensureSorted(int column, BobUI::SortOrder order,
                               int start, int end)
{
    int count = end - start + 1;
    QList<std::pair<BOBUIableWidgetItem *, int>> sorting;
    sorting.reserve(count);
    for (int row = start; row <= end; ++row) {
        BOBUIableWidgetItem *itm = item(row, column);
        if (itm == nullptr) {
            // no more sortable items (all 0-items are
            // at the end of the table when it is sorted)
            break;
        }
        sorting.emplace_back(itm, row);
    }

    const auto compare = (order == BobUI::AscendingOrder ? &itemLessThan : &itemGreaterThan);
    std::stable_sort(sorting.begin(), sorting.end(), compare);
    QModelIndexList oldPersistentIndexes, newPersistentIndexes;
    QList<BOBUIableWidgetItem *> newTable = tableItems;
    QList<BOBUIableWidgetItem *> newVertical = verticalHeaderItems;
    QList<BOBUIableWidgetItem *> colItems = columnItems(column);
    QList<BOBUIableWidgetItem *>::iterator vit = colItems.begin();
    qsizetype distanceFromBegin = 0;
    bool changed = false;
    for (int i = 0; i < sorting.size(); ++i) {
        distanceFromBegin = std::distance(colItems.begin(), vit);
        int oldRow = sorting.at(i).second;
        BOBUIableWidgetItem *item = colItems.at(oldRow);
        colItems.remove(oldRow);
        vit = sortedInsertionIterator(colItems.begin() + distanceFromBegin, colItems.end(), order,
                                      item);
        int newRow = qMax((int)(vit - colItems.begin()), 0);
        if ((newRow < oldRow) && !(*item < *colItems.at(oldRow - 1)) && !(*colItems.at(oldRow - 1) < *item))
            newRow = oldRow;
        vit = colItems.insert(vit, item);
        if (newRow != oldRow) {
            if (!changed) {
                emit layoutAboutToBeChanged({}, QAbstractItemModel::VerticalSortHint);
                oldPersistentIndexes = persistentIndexList();
                newPersistentIndexes = oldPersistentIndexes;
                changed = true;
            }
            // move the items @ oldRow to newRow
            int cc = columnCount();
            QList<BOBUIableWidgetItem *> rowItems(cc);
            for (int j = 0; j < cc; ++j)
                rowItems[j] = newTable.at(tableIndex(oldRow, j));
            newTable.remove(tableIndex(oldRow, 0), cc);
            newTable.insert(tableIndex(newRow, 0), cc, 0);
            for (int j = 0; j < cc; ++j)
                newTable[tableIndex(newRow, j)] = rowItems.at(j);
            BOBUIableWidgetItem *header = newVertical.at(oldRow);
            newVertical.remove(oldRow);
            newVertical.insert(newRow, header);
            // update persistent indexes
            updateRowIndexes(newPersistentIndexes, oldRow, newRow);
            // the index of the remaining rows may have changed
            for (int j = i + 1; j < sorting.size(); ++j) {
                int otherRow = sorting.at(j).second;
                if (oldRow < otherRow && newRow >= otherRow)
                    --sorting[j].second;
                else if (oldRow > otherRow && newRow <= otherRow)
                    ++sorting[j].second;
            }
        }
    }

    if (changed) {
        tableItems = newTable;
        verticalHeaderItems = newVertical;
        changePersistentIndexList(oldPersistentIndexes,
                                  newPersistentIndexes);
        emit layoutChanged({}, QAbstractItemModel::VerticalSortHint);
    }
}

/*
  \internal

  Returns the non-0 items in column \a column.
*/
QList<BOBUIableWidgetItem *> BOBUIableModel::columnItems(int column) const
{
    QList<BOBUIableWidgetItem *> items;
    int rc = rowCount();
    items.reserve(rc);
    for (int row = 0; row < rc; ++row) {
        BOBUIableWidgetItem *itm = item(row, column);
        if (itm == nullptr) {
            // no more sortable items (all 0-items are
            // at the end of the table when it is sorted)
            break;
        }
        items.append(itm);
    }
    return items;
}

/*
  \internal

  Adjusts the row of each index in \a indexes if necessary, given
  that a row of items has been moved from row \a movedFrom to row
  \a movedTo.
*/
void BOBUIableModel::updateRowIndexes(QModelIndexList &indexes,
                                   int movedFromRow, int movedToRow)
{
    QModelIndexList::iterator it;
    for (it = indexes.begin(); it != indexes.end(); ++it) {
        int oldRow = (*it).row();
        int newRow = oldRow;
        if (oldRow == movedFromRow)
            newRow = movedToRow;
        else if (movedFromRow < oldRow && movedToRow >= oldRow)
            newRow = oldRow - 1;
        else if (movedFromRow > oldRow && movedToRow <= oldRow)
            newRow = oldRow + 1;
        if (newRow != oldRow)
            *it = index(newRow, (*it).column(), (*it).parent());
    }
}

/*
  \internal

  Returns an iterator to the item where \a item should be
  inserted in the interval (\a begin, \a end) according to
  the given sort \a order.
*/
QList<BOBUIableWidgetItem *>::iterator
BOBUIableModel::sortedInsertionIterator(const QList<BOBUIableWidgetItem *>::iterator &begin,
                                     const QList<BOBUIableWidgetItem *>::iterator &end,
                                     BobUI::SortOrder order, BOBUIableWidgetItem *item)
{
    if (order == BobUI::AscendingOrder)
        return std::lower_bound(begin, end, item, BOBUIableModelLessThan());
    return std::lower_bound(begin, end, item, BOBUIableModelGreaterThan());
}

bool BOBUIableModel::itemLessThan(const std::pair<BOBUIableWidgetItem*,int> &left,
                               const std::pair<BOBUIableWidgetItem*,int> &right)
{
    return *(left.first) < *(right.first);
}

bool BOBUIableModel::itemGreaterThan(const std::pair<BOBUIableWidgetItem*,int> &left,
                                  const std::pair<BOBUIableWidgetItem*,int> &right)
{
    return (*(right.first) < *(left .first));
}

QVariant BOBUIableModel::headerData(int section, BobUI::Orientation orientation, int role) const
{
    if (section < 0)
        return QVariant();

    BOBUIableWidgetItem *itm = nullptr;
    if (orientation == BobUI::Horizontal && section < horizontalHeaderItems.size())
        itm = horizontalHeaderItems.at(section);
    else if (orientation == BobUI::Vertical && section < verticalHeaderItems.size())
        itm = verticalHeaderItems.at(section);
    else
        return QVariant(); // section is out of bounds

    if (itm)
        return itm->data(role);
    if (role == BobUI::DisplayRole)
        return section + 1;
    return QVariant();
}

bool BOBUIableModel::setHeaderData(int section, BobUI::Orientation orientation,
                                const QVariant &value, int role)
{
    if (section < 0 ||
       (orientation == BobUI::Horizontal && horizontalHeaderItems.size() <= section) ||
       (orientation == BobUI::Vertical && verticalHeaderItems.size() <= section))
    return false;

    BOBUIableWidgetItem *itm = nullptr;
    if (orientation == BobUI::Horizontal)
        itm = horizontalHeaderItems.at(section);
    else
        itm = verticalHeaderItems.at(section);
    if (itm) {
        itm->setData(role, value);
        return true;
    }
    return false;
}

bool BOBUIableModel::isValid(const QModelIndex &index) const
{
    return (index.isValid()
            && index.row() < verticalHeaderItems.size()
            && index.column() < horizontalHeaderItems.size());
}

void BOBUIableModel::clear()
{
    for (int j = 0; j < verticalHeaderItems.size(); ++j) {
        if (verticalHeaderItems.at(j)) {
            verticalHeaderItems.at(j)->view = nullptr;
            delete verticalHeaderItems.at(j);
            verticalHeaderItems[j] = 0;
        }
    }
    for (int k = 0; k < horizontalHeaderItems.size(); ++k) {
        if (horizontalHeaderItems.at(k)) {
            horizontalHeaderItems.at(k)->view = nullptr;
            delete horizontalHeaderItems.at(k);
            horizontalHeaderItems[k] = 0;
        }
    }
    clearContents();
}

void BOBUIableModel::clearContents()
{
    beginResetModel();
    for (int i = 0; i < tableItems.size(); ++i) {
        if (tableItems.at(i)) {
            tableItems.at(i)->view = nullptr;
            delete tableItems.at(i);
            tableItems[i] = 0;
        }
    }
    endResetModel();
}

void BOBUIableModel::itemChanged(BOBUIableWidgetItem *item, const QList<int> &roles)
{
    if (!item)
        return;
    if (item->d->headerItem) {
        int row = verticalHeaderItems.indexOf(item);
        if (row >= 0) {
            emit headerDataChanged(BobUI::Vertical, row, row);
        } else {
            int column = horizontalHeaderItems.indexOf(item);
            if (column >= 0)
                emit headerDataChanged(BobUI::Horizontal, column, column);
        }
    } else {
        QModelIndex idx = index(item);
        if (idx.isValid())
            emit dataChanged(idx, idx, roles);
    }
}

BOBUIableWidgetItem* BOBUIableModel::createItem() const
{
    return prototype ? prototype->clone() : new BOBUIableWidgetItem;
}

const BOBUIableWidgetItem *BOBUIableModel::itemPrototype() const
{
    return prototype;
}

void BOBUIableModel::setItemPrototype(const BOBUIableWidgetItem *item)
{
    if (prototype != item) {
        delete prototype;
        prototype = item;
    }
}

QStringList BOBUIableModel::mimeTypes() const
{
    auto v = view();
    if (v)
        return v->mimeTypes();
    return {};
}

QMimeData *BOBUIableModel::internalMimeData()  const
{
    return QAbstractTableModel::mimeData(cachedIndexes);
}

QMimeData *BOBUIableModel::mimeData(const QModelIndexList &indexes) const
{
    QList<BOBUIableWidgetItem*> items;
    const int indexesCount = indexes.size();
    items.reserve(indexesCount);
    for (int i = 0; i < indexesCount; ++i)
        items << item(indexes.at(i));
    const BOBUIableWidget *view = this->view();

    // cachedIndexes is a little hack to avoid copying from QModelIndexList to
    // QList<BOBUIreeWidgetItem*> and back again in the view
    cachedIndexes = indexes;
    QMimeData *mimeData = (view ? view->mimeData(items) : nullptr);
    cachedIndexes.clear();
    return mimeData;
}

bool BOBUIableModel::dropMimeData(const QMimeData *data, BobUI::DropAction action,
                              int row , int column, const QModelIndex &index)
{
    if (index.isValid()) {
        row = index.row();
        column = index.column();
    } else if (row == -1 || column == -1) { // The user dropped outside the table.
        row = rowCount();
        column = 0;
    } else { // The user dropped between two rows
        // This means inserting a row, which only makes sense at column 0
        column = 0;
    }

    BOBUIableWidget *view = this->view();
    return (view ? view->dropMimeData(row, column, data, action) : false);
}

BobUI::DropActions BOBUIableModel::supportedDropActions() const
{
    const BOBUIableWidget *view = this->view();
    return (view ? view->supportedDropActions() : BobUI::DropActions(BobUI::IgnoreAction));
}

BobUI::DropActions BOBUIableModel::supportedDragActions() const
{
#if BOBUI_CONFIG(draganddrop)
    const BOBUIableWidget *view = this->view();
    return (view ? view->supportedDragActions() : BobUI::DropActions(BobUI::IgnoreAction));
#else
    return BobUI::DropActions(BobUI::IgnoreAction);
#endif
}

/*!
    \class BOBUIableWidgetSelectionRange

    \brief The BOBUIableWidgetSelectionRange class provides a way to interact with
    selection in a model without using model indexes and a selection model.

    \ingroup model-view
    \inmodule BobUIWidgets

    The BOBUIableWidgetSelectionRange class stores the top left and bottom
    right rows and columns of a selection range in a table. The
    selections in the table may consist of several selection ranges.

    \note If the item within the selection range is marked as not selectable,
    e.g., \c{itemFlags() & BobUI::ItemIsSelectable == 0} then it will not appear
    in the selection range.

    \sa BOBUIableWidget
*/

/*!
    \fn BOBUIableWidgetSelectionRange::BOBUIableWidgetSelectionRange()

    Constructs an empty table selection range, i.e. a range
    whose rowCount() and columnCount() are 0.

    \sa topRow(), leftColumn(), bottomRow(), rightColumn()
*/

/*!
    \fn BOBUIableWidgetSelectionRange::BOBUIableWidgetSelectionRange(int top, int left, int bottom, int right)

    Constructs the table selection range from the given \a top, \a
    left, \a bottom and \a right table rows and columns.

    \sa topRow(), leftColumn(), bottomRow(), rightColumn()
*/

/*!
    \fn bool BOBUIableWidgetSelectionRange::operator==(const BOBUIableWidgetSelectionRange &lhs, const BOBUIableWidgetSelectionRange &rhs)
    \since 6.3

    Returns true if \a lhs and \a rhs are equal, otherwise returns false.
*/

/*!
    \fn bool BOBUIableWidgetSelectionRange::operator!=(const BOBUIableWidgetSelectionRange &lhs, const BOBUIableWidgetSelectionRange &rhs)
    \since 6.3

    Returns true if \a lhs and \a rhs are not equal, otherwise returns false.
*/

/*!
    \fn int BOBUIableWidgetSelectionRange::topRow() const

    Returns the top row of the range.

    \sa bottomRow(), leftColumn(), rowCount()
*/

/*!
    \fn int BOBUIableWidgetSelectionRange::bottomRow() const

    Returns the bottom row of the range.

    \sa topRow(), rightColumn(), rowCount()
*/

/*!
    \fn int BOBUIableWidgetSelectionRange::leftColumn() const

    Returns the left column of the range.

    \sa rightColumn(), topRow(), columnCount()
*/

/*!
    \fn int BOBUIableWidgetSelectionRange::rightColumn() const

    Returns the right column of the range.

    \sa leftColumn(), bottomRow(), columnCount()
*/

/*!
    \fn int BOBUIableWidgetSelectionRange::rowCount() const

    Returns the number of rows in the range.

    This is equivalent to bottomRow() - topRow() + 1.

    \sa columnCount(), topRow(), bottomRow()
*/

/*!
    \fn int BOBUIableWidgetSelectionRange::columnCount() const

    Returns the number of columns in the range.

    This is equivalent to rightColumn() - leftColumn() + 1.

    \sa rowCount(), leftColumn(), rightColumn()
*/

/*!
    \class BOBUIableWidgetItem
    \brief The BOBUIableWidgetItem class provides an item for use with the
    BOBUIableWidget class.

    \ingroup model-view
    \inmodule BobUIWidgets

    Table items are used to hold pieces of information for table widgets.
    Items usually contain text, icons, or checkboxes

    The BOBUIableWidgetItem class is a convenience class that replaces the
    \c BOBUIableItem class in BobUI 3. It provides an item for use with
    the BOBUIableWidget class.

    Top-level items are constructed without a parent then inserted at the
    position specified by a pair of row and column numbers:

    \snippet bobuiablewidget-using/mainwindow.cpp 3

    Each item can have its own background brush which is set with
    the setBackground() function. The current background brush can be
    found with background().
    The text label for each item can be rendered with its own font and brush.
    These are specified with the setFont() and setForeground() functions,
    and read with font() and foreground().

    By default, items are enabled, editable, selectable, checkable, and can be
    used both as the source of a drag and drop operation and as a drop target.
    Each item's flags can be changed by calling setFlags() with the appropriate
    value (see \l{BobUI::ItemFlags}). Checkable items can be checked and unchecked
    with the setCheckState() function. The corresponding checkState() function
    indicates whether the item is currently checked.

    \section1 Subclassing

    When subclassing BOBUIableWidgetItem to provide custom items, it is possible to
    define new types for them so that they can be distinguished from standard
    items. The constructors for subclasses that require this feature need to
    call the base class constructor with a new type value equal to or greater
    than \l UserType.

    \sa BOBUIableWidget, {Model/View Programming}, QListWidgetItem, BOBUIreeWidgetItem
*/

/*!
  \fn int BOBUIableWidgetItem::row() const

  Returns the row of the item in the table.
  If the item is not in a table, this function will return -1.

  \sa column()
*/

/*!
  \fn int BOBUIableWidgetItem::column() const

  Returns the column of the item in the table.
  If the item is not in a table, this function will return -1.

  \sa row()
*/

/*!
  \fn QSize BOBUIableWidgetItem::sizeHint() const

  Returns the size hint set for the table item.
*/

/*!
  \fn void BOBUIableWidgetItem::setSizeHint(const QSize &size)

  Sets the size hint for the table item to be \a size.
  If no size hint is set or \a size is invalid, the item
  delegate will compute the size hint based on the item data.
*/

/*!
    \fn BobUI::CheckState BOBUIableWidgetItem::checkState() const

    Returns the checked state of the table item.

    \sa flags()
*/

/*!
    \fn void BOBUIableWidgetItem::setCheckState(BobUI::CheckState state)

    Sets the check state of the table item to be \a state.
*/

/*!
    \fn BOBUIableWidget *BOBUIableWidgetItem::tableWidget() const

    Returns the table widget that contains the item.
*/

/*!
  \fn bool BOBUIableWidgetItem::isSelected() const

  Returns \c true if the item is selected, otherwise returns \c false.

  \sa setSelected()
*/
bool BOBUIableWidgetItem::isSelected() const
{
    if (!view || !view->selectionModel())
        return false;
    const BOBUIableModel *model = qobject_cast<const BOBUIableModel*>(view->model());
    if (!model)
        return false;
    const QModelIndex index = model->index(this);
    return view->selectionModel()->isSelected(index);
}

/*!
  \fn void BOBUIableWidgetItem::setSelected(bool select)

  Sets the selected state of the item to \a select.

  \sa isSelected()
*/
void BOBUIableWidgetItem::setSelected(bool select)
{
    if (!view || !view->selectionModel())
        return;
    const BOBUIableModel *model = qobject_cast<const BOBUIableModel*>(view->model());
    if (!model)
        return;
    const QModelIndex index = model->index(this);
    view->selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

/*!
    \fn BobUI::ItemFlags BOBUIableWidgetItem::flags() const

    Returns the flags used to describe the item. These determine whether
    the item can be checked, edited, and selected.

    \sa setFlags()
*/

/*!
    \fn void BOBUIableWidgetItem::setFlags(BobUI::ItemFlags flags)

    Sets the flags for the item to the given \a flags. These determine whether
    the item can be selected or modified.

    \sa flags()
*/
void BOBUIableWidgetItem::setFlags(BobUI::ItemFlags aflags)
{
    itemFlags = aflags;
    if (BOBUIableModel *model = tableModel())
        model->itemChanged(this);
}


/*!
    \fn QString BOBUIableWidgetItem::text() const

    Returns the item's text.

    \sa setText()
*/

/*!
    \fn void BOBUIableWidgetItem::setText(const QString &text)

    Sets the item's text to the \a text specified.

    \sa text(), setFont(), setForeground()
*/

/*!
    \fn QIcon BOBUIableWidgetItem::icon() const

    Returns the item's icon.

    \sa setIcon(), {QAbstractItemView::iconSize}{iconSize}
*/

/*!
    \fn void BOBUIableWidgetItem::setIcon(const QIcon &icon)

    Sets the item's icon to the \a icon specified.

    \sa icon(), setText(), {QAbstractItemView::iconSize}{iconSize}
*/

/*!
    \fn QString BOBUIableWidgetItem::statusTip() const

    Returns the item's status tip.

    \sa setStatusTip()
*/

/*!
    \fn void BOBUIableWidgetItem::setStatusTip(const QString &statusTip)

    Sets the status tip for the table item to the text specified by
    \a statusTip. BOBUIableWidget mouse tracking needs to be enabled for this
    feature to work.

    \sa statusTip(), setToolTip(), setWhatsThis()
*/

/*!
    \fn QString BOBUIableWidgetItem::toolTip() const

    Returns the item's tooltip.

    \sa setToolTip()
*/

/*!
    \fn void BOBUIableWidgetItem::setToolTip(const QString &toolTip)

    Sets the item's tooltip to the string specified by \a toolTip.

    \sa toolTip(), setStatusTip(), setWhatsThis()
*/

/*!
    \fn QString BOBUIableWidgetItem::whatsThis() const

    Returns the item's "What's This?" help.

    \sa setWhatsThis()
*/

/*!
    \fn void BOBUIableWidgetItem::setWhatsThis(const QString &whatsThis)

    Sets the item's "What's This?" help to the string specified by \a whatsThis.

    \sa whatsThis(), setStatusTip(), setToolTip()
*/

/*!
    \fn QFont BOBUIableWidgetItem::font() const

    Returns the font used to render the item's text.

    \sa setFont()
*/

/*!
    \fn void BOBUIableWidgetItem::setFont(const QFont &font)

    Sets the font used to display the item's text to the given \a font.

    \sa font(), setText(), setForeground()
*/

/*!
    \fn QBrush BOBUIableWidgetItem::background() const

    Returns the brush used to render the item's background.

    \sa foreground()
*/

/*!
    \fn void BOBUIableWidgetItem::setBackground(const QBrush &brush)

    Sets the item's background brush to the specified \a brush.
    Setting a default-constructed brush will let the view use the
    default color from the style.

    \sa setForeground()
*/

/*!
    \fn QBrush BOBUIableWidgetItem::foreground() const

    Returns the brush used to render the item's foreground (e.g. text).

    \sa background()
*/

/*!
    \fn void BOBUIableWidgetItem::setForeground(const QBrush &brush)

    Sets the item's foreground brush to the specified \a brush.
    Setting a default-constructed brush will let the view use the
    default color from the style.

    \sa setBackground()
*/

/*!
    \if defined(bobui7)

    \fn BobUI::Alignment BOBUIableWidgetItem::textAlignment() const

    Returns the text alignment for the list item.

    \else

    \fn int BOBUIableWidgetItem::textAlignment() const

    Returns the text alignment for the item's text.

    \note This function returns an int for historical reasons. It will
    be corrected to return BobUI::Alignment in BobUI 7.

    \sa BobUI::Alignment

    \endif
*/

/*!
    \obsolete [6.4] Use the overload that takes a BobUI::Alignment argument.

    \fn void BOBUIableWidgetItem::setTextAlignment(int alignment)

    Sets the text alignment for the item's text to the \a alignment
    specified.

    \sa BobUI::Alignment
*/

/*!
    \since 6.4

    \fn void BOBUIableWidgetItem::setTextAlignment(BobUI::Alignment alignment)

    Sets the text alignment for the item's text to the \a alignment
    specified.
*/

/*!
  \fn void BOBUIableWidgetItem::setTextAlignment(BobUI::AlignmentFlag alignment)
  \internal
*/

/*!
    Constructs a table item of the specified \a type that does not belong
    to any table.

    \sa type()
*/
BOBUIableWidgetItem::BOBUIableWidgetItem(int type)
    :  rtti(type), view(nullptr), d(new BOBUIableWidgetItemPrivate(this)),
      itemFlags(BobUI::ItemIsEditable
                |BobUI::ItemIsSelectable
                |BobUI::ItemIsUserCheckable
                |BobUI::ItemIsEnabled
                |BobUI::ItemIsDragEnabled
                |BobUI::ItemIsDropEnabled)
{
}

/*!
    Constructs a table item with the given \a text.

    \sa type()
*/
BOBUIableWidgetItem::BOBUIableWidgetItem(const QString &text, int type)
    :  rtti(type), view(nullptr), d(new BOBUIableWidgetItemPrivate(this)),
      itemFlags(BobUI::ItemIsEditable
                |BobUI::ItemIsSelectable
                |BobUI::ItemIsUserCheckable
                |BobUI::ItemIsEnabled
                |BobUI::ItemIsDragEnabled
                |BobUI::ItemIsDropEnabled)
{
    setData(BobUI::DisplayRole, text);
}

/*!
    Constructs a table item with the given \a icon and \a text.

    \sa type()
*/
BOBUIableWidgetItem::BOBUIableWidgetItem(const QIcon &icon, const QString &text, int type)
    :  rtti(type), view(nullptr), d(new BOBUIableWidgetItemPrivate(this)),
       itemFlags(BobUI::ItemIsEditable
                |BobUI::ItemIsSelectable
                |BobUI::ItemIsUserCheckable
                |BobUI::ItemIsEnabled
                |BobUI::ItemIsDragEnabled
                |BobUI::ItemIsDropEnabled)
{
    setData(BobUI::DecorationRole, icon);
    setData(BobUI::DisplayRole, text);
}

/*!
    Destroys the table item.
*/
BOBUIableWidgetItem::~BOBUIableWidgetItem()
{
    if (BOBUIableModel *model = tableModel())
        model->removeItem(this);
    delete d;
}

/*!
    Creates a copy of the item.
*/
BOBUIableWidgetItem *BOBUIableWidgetItem::clone() const
{
    return new BOBUIableWidgetItem(*this);
}

/*!
    Sets the item's data for the given \a role to the specified \a value.

    \note The default implementation treats BobUI::EditRole and BobUI::DisplayRole as
    referring to the same data.

    \sa BobUI::ItemDataRole, data()
*/
void BOBUIableWidgetItem::setData(int role, const QVariant &value)
{
    bool found = false;
    role = (role == BobUI::EditRole ? BobUI::DisplayRole : role);
    for (int i = 0; i < values.size(); ++i) {
        if (values.at(i).role == role) {
            if (values[i].value == value)
                return;

            values[i].value = value;
            found = true;
            break;
        }
    }
    if (!found)
        values.append(QWidgetItemData(role, value));
    if (BOBUIableModel *model = tableModel())
    {
        const QList<int> roles((role == BobUI::DisplayRole)
                                       ? QList<int>({ BobUI::DisplayRole, BobUI::EditRole })
                                       : QList<int>({ role }));
        model->itemChanged(this, roles);
    }
}

/*!
    Returns the item's data for the given \a role.
*/
QVariant BOBUIableWidgetItem::data(int role) const
{
    role = (role == BobUI::EditRole ? BobUI::DisplayRole : role);
    for (const auto &value : values) {
        if (value.role == role)
            return value.value;
    }
    return QVariant();
}

/*!
    Returns \c true if the item is less than the \a other item; otherwise returns
    false.
*/
bool BOBUIableWidgetItem::operator<(const BOBUIableWidgetItem &other) const
{
    const QVariant v1 = data(BobUI::DisplayRole), v2 = other.data(BobUI::DisplayRole);
    return QAbstractItemModelPrivate::variantLessThan(v1, v2);
}

#ifndef BOBUI_NO_DATASTREAM

/*!
    Reads the item from stream \a in.

    \sa write()
*/
void BOBUIableWidgetItem::read(QDataStream &in)
{
    in >> values;
}

/*!
    Writes the item to stream \a out.

    \sa read()
*/
void BOBUIableWidgetItem::write(QDataStream &out) const
{
    out << values;
}

/*!
  \internal
  returns the BOBUIableModel if a view is set
*/
BOBUIableModel *BOBUIableWidgetItem::tableModel() const
{
    return (view ? qobject_cast<BOBUIableModel*>(view->model()) : nullptr);
}


/*!
    \relates BOBUIableWidgetItem

    Reads a table widget item from stream \a in into \a item.

    This operator uses BOBUIableWidgetItem::read().

    \sa {Serializing BobUI Data Types}
*/
QDataStream &operator>>(QDataStream &in, BOBUIableWidgetItem &item)
{
    item.read(in);
    return in;
}

/*!
    \relates BOBUIableWidgetItem

    Writes the table widget item \a item to stream \a out.

    This operator uses BOBUIableWidgetItem::write().

    \sa {Serializing BobUI Data Types}
*/
QDataStream &operator<<(QDataStream &out, const BOBUIableWidgetItem &item)
{
    item.write(out);
    return out;
}

#endif // BOBUI_NO_DATASTREAM

/*!
    Constructs a copy of \a other. Note that type() and tableWidget()
    are not copied.

    This function is useful when reimplementing clone().

    \sa data(), flags()
*/
BOBUIableWidgetItem::BOBUIableWidgetItem(const BOBUIableWidgetItem &other)
    : rtti(Type), values(other.values), view(nullptr),
      d(new BOBUIableWidgetItemPrivate(this)),
      itemFlags(other.itemFlags)
{
}

/*!
    Assigns \a other's data and flags to this item. Note that type()
    and tableWidget() are not copied.

    This function is useful when reimplementing clone().

    \sa data(), flags()
*/
BOBUIableWidgetItem &BOBUIableWidgetItem::operator=(const BOBUIableWidgetItem &other)
{
    values = other.values;
    itemFlags = other.itemFlags;
    return *this;
}

/*!
    \class BOBUIableWidget
    \brief The BOBUIableWidget class provides an item-based table view with a default model.

    \ingroup model-view
    \inmodule BobUIWidgets

    \image fusion-tableview.png {Table of months and amounts}

    Table widgets provide standard table display facilities for applications.
    The items in a BOBUIableWidget are provided by BOBUIableWidgetItem.

    If you want a table that uses your own data model you should
    use BOBUIableView rather than this class.

    Table widgets can be constructed with the required numbers of rows and
    columns:

    \snippet bobuiablewidget-using/mainwindow.cpp 0

    Alternatively, tables can be constructed without a given size and resized
    later:

    \snippet bobuiablewidget-resizing/mainwindow.cpp 0
    \snippet bobuiablewidget-resizing/mainwindow.cpp 1

    Items are created outside the table (with no parent widget) and inserted
    into the table with setItem():

    \snippet bobuiablewidget-resizing/mainwindow.cpp 2

    If you want to enable sorting in your table widget, do so after you
    have populated it with items, otherwise sorting may interfere with
    the insertion order (see setItem() for details).

    Tables can be given both horizontal and vertical headers. The simplest way
    to create the headers is to supply a list of strings to the
    setHorizontalHeaderLabels() and setVerticalHeaderLabels() functions. These
    will provide simple textual headers for the table's columns and rows.
    More sophisticated headers can be created from existing table items
    that are usually constructed outside the table. For example, we can
    construct a table item with an icon and aligned text, and use it as the
    header for a particular column:

    \snippet bobuiablewidget-using/mainwindow.cpp 2

    The number of rows in the table can be found with rowCount(), and the
    number of columns with columnCount(). The table can be cleared with the
    clear() function.

    \sa BOBUIableWidgetItem, BOBUIableView, {Model/View Programming}
*/

/*!
    \property BOBUIableWidget::rowCount
    \brief the number of rows in the table

    By default, for a table constructed without row and column counts,
    this property contains a value of 0.
*/

/*!
    \property BOBUIableWidget::columnCount
    \brief the number of columns in the table

    By default, for a table constructed without row and column counts,
    this property contains a value of 0.
*/

void BOBUIableWidgetPrivate::setup()
{
    Q_Q(BOBUIableWidget);
    connections = {
        // view signals
        QObjectPrivate::connect(q, &BOBUIableWidget::pressed,
                                this, &BOBUIableWidgetPrivate::emitItemPressed),
        QObjectPrivate::connect(q, &BOBUIableWidget::clicked,
                                this, &BOBUIableWidgetPrivate::emitItemClicked),
        QObjectPrivate::connect(q, &BOBUIableWidget::doubleClicked,
                                this, &BOBUIableWidgetPrivate::emitItemDoubleClicked),
        QObjectPrivate::connect(q, &BOBUIableWidget::activated,
                                this, &BOBUIableWidgetPrivate::emitItemActivated),
        QObjectPrivate::connect(q, &BOBUIableWidget::entered,
                                this, &BOBUIableWidgetPrivate::emitItemEntered),
        // model signals
        QObjectPrivate::connect(model, &QAbstractItemModel::dataChanged,
                                this, &BOBUIableWidgetPrivate::emitItemChanged),
        // selection signals
        QObjectPrivate::connect(q->selectionModel(), &QItemSelectionModel::currentChanged,
                                this, &BOBUIableWidgetPrivate::emitCurrentItemChanged),
        QObject::connect(q->selectionModel(), &QItemSelectionModel::selectionChanged,
                         q, &BOBUIableWidget::itemSelectionChanged),
        // sorting
        QObjectPrivate::connect(model, &QAbstractItemModel::dataChanged,
                                this, &BOBUIableWidgetPrivate::dataChanged),
        QObjectPrivate::connect(model, &QAbstractItemModel::columnsRemoved,
                                this, &BOBUIableWidgetPrivate::sort)
    };
}

void BOBUIableWidgetPrivate::clearConnections()
{
    for (const QMetaObject::Connection &connection : connections)
        QObject::disconnect(connection);
}

void BOBUIableWidgetPrivate::emitItemPressed(const QModelIndex &index)
{
    Q_Q(BOBUIableWidget);
    if (BOBUIableWidgetItem *item = tableModel()->item(index))
        emit q->itemPressed(item);
    emit q->cellPressed(index.row(), index.column());
}

void BOBUIableWidgetPrivate::emitItemClicked(const QModelIndex &index)
{
    Q_Q(BOBUIableWidget);
    if (BOBUIableWidgetItem *item = tableModel()->item(index))
        emit q->itemClicked(item);
    emit q->cellClicked(index.row(), index.column());
}

void BOBUIableWidgetPrivate::emitItemDoubleClicked(const QModelIndex &index)
{
    Q_Q(BOBUIableWidget);
    if (BOBUIableWidgetItem *item = tableModel()->item(index))
        emit q->itemDoubleClicked(item);
    emit q->cellDoubleClicked(index.row(), index.column());
}

void BOBUIableWidgetPrivate::emitItemActivated(const QModelIndex &index)
{
    Q_Q(BOBUIableWidget);
    if (BOBUIableWidgetItem *item = tableModel()->item(index))
        emit q->itemActivated(item);
    emit q->cellActivated(index.row(), index.column());
}

void BOBUIableWidgetPrivate::emitItemEntered(const QModelIndex &index)
{
    Q_Q(BOBUIableWidget);
    if (BOBUIableWidgetItem *item = tableModel()->item(index))
        emit q->itemEntered(item);
    emit q->cellEntered(index.row(), index.column());
}

void BOBUIableWidgetPrivate::emitItemChanged(const QModelIndex &index)
{
    Q_Q(BOBUIableWidget);
    if (BOBUIableWidgetItem *item = tableModel()->item(index))
        emit q->itemChanged(item);
    emit q->cellChanged(index.row(), index.column());
}

void BOBUIableWidgetPrivate::emitCurrentItemChanged(const QModelIndex &current,
                                                 const QModelIndex &previous)
{
    Q_Q(BOBUIableWidget);
    BOBUIableWidgetItem *currentItem = tableModel()->item(current);
    BOBUIableWidgetItem *previousItem = tableModel()->item(previous);
    if (currentItem || previousItem)
        emit q->currentItemChanged(currentItem, previousItem);
    emit q->currentCellChanged(current.row(), current.column(), previous.row(), previous.column());
}

void BOBUIableWidgetPrivate::sort()
{
    if (sortingEnabled) {
        int column = horizontalHeader->sortIndicatorSection();
        BobUI::SortOrder order = horizontalHeader->sortIndicatorOrder();
        model->sort(column, order);
    }
}

void BOBUIableWidgetPrivate::dataChanged(const QModelIndex &topLeft,
                                      const QModelIndex &bottomRight)
{
    if (sortingEnabled && topLeft.isValid() && bottomRight.isValid()) {
        int column = horizontalHeader->sortIndicatorSection();
        if (column >= topLeft.column() && column <= bottomRight.column()) {
            BobUI::SortOrder order = horizontalHeader->sortIndicatorOrder();
            tableModel()->ensureSorted(column, order, topLeft.row(), bottomRight.row());
        }
    }
}

/*!
    \fn void BOBUIableWidget::itemPressed(BOBUIableWidgetItem *item)

    This signal is emitted whenever an item in the table is pressed.
    The \a item specified is the item that was pressed.
*/

/*!
    \fn void BOBUIableWidget::itemClicked(BOBUIableWidgetItem *item)

    This signal is emitted whenever an item in the table is clicked.
    The \a item specified is the item that was clicked.
*/

/*!
    \fn void BOBUIableWidget::itemDoubleClicked(BOBUIableWidgetItem *item)

    This signal is emitted whenever an item in the table is double
    clicked. The \a item specified is the item that was double clicked.
*/

/*!
    \fn void BOBUIableWidget::itemActivated(BOBUIableWidgetItem *item)

    This signal is emitted when the specified \a item has been activated
*/

/*!
    \fn void BOBUIableWidget::itemEntered(BOBUIableWidgetItem *item)

    This signal is emitted when the mouse cursor enters an item. The
    \a item is the item entered.

    This signal is only emitted when mouseTracking is turned on, or when a
    mouse button is pressed while moving into an item.
*/

/*!
    \fn void BOBUIableWidget::itemChanged(BOBUIableWidgetItem *item)

    This signal is emitted whenever the data of \a item has changed.
*/

/*!
    \fn void BOBUIableWidget::currentItemChanged(BOBUIableWidgetItem *current, BOBUIableWidgetItem *previous)

    This signal is emitted whenever the current item changes. The \a
    previous item is the item that previously had the focus, \a
    current is the new current item.
*/

/*!
    \fn void BOBUIableWidget::itemSelectionChanged()

    This signal is emitted whenever the selection changes.

    \sa selectedItems(), BOBUIableWidgetItem::isSelected()
*/


/*!
  \fn void BOBUIableWidget::cellPressed(int row, int column)

  This signal is emitted whenever a cell in the table is pressed.
  The \a row and \a column specified is the cell that was pressed.
*/

/*!
  \fn void BOBUIableWidget::cellClicked(int row, int column)

  This signal is emitted whenever a cell in the table is clicked.
  The \a row and \a column specified is the cell that was clicked.
*/

/*!
  \fn void BOBUIableWidget::cellDoubleClicked(int row, int column)

  This signal is emitted whenever a cell in the table is double
  clicked. The \a row and \a column specified is the cell that was
  double clicked.
*/

/*!
  \fn void BOBUIableWidget::cellActivated(int row, int column)

  This signal is emitted when the cell specified  by \a row and \a column
  has been activated
*/

/*!
  \fn void BOBUIableWidget::cellEntered(int row, int column)

  This signal is emitted when the mouse cursor enters a cell. The
  cell is specified by \a row and \a column.

  This signal is only emitted when mouseTracking is turned on, or when a
  mouse button is pressed while moving into an item.
*/

/*!
  \fn void BOBUIableWidget::cellChanged(int row, int column)

  This signal is emitted whenever the data of the item in the cell
  specified by \a row and \a column has changed.
*/

/*!
  \fn void BOBUIableWidget::currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)

  This signal is emitted whenever the current cell changes. The cell
  specified by \a previousRow and \a previousColumn is the cell that
  previously had the focus, the cell specified by \a currentRow and \a
  currentColumn is the new current cell.
*/

/*!
  \fn void BOBUIableWidget::removeCellWidget(int row, int column)

  Removes the widget set on the cell indicated by \a row and \a column.
*/

/*!
    \fn BOBUIableWidgetItem *BOBUIableWidget::itemAt(int ax, int ay) const

    Returns the item at the position equivalent to QPoint(\a{ax}, \a{ay}) in
    the table widget's coordinate system, or returns \nullptr if the specified point
    is not covered by an item in the table widget.

    \sa item()
*/

/*!
    \enum BOBUIableWidgetItem::ItemType

    This enum describes the types that are used to describe table widget items.

    \value Type     The default type for table widget items.
    \value UserType The minimum value for custom types. Values below UserType are
                    reserved by BobUI.

    You can define new user types in BOBUIableWidgetItem subclasses to ensure that
    custom items are treated specially.

    \sa type()
*/

/*!
    \fn int BOBUIableWidgetItem::type() const

    Returns the type passed to the BOBUIableWidgetItem constructor.
*/

/*!
    Creates a new table view with the given \a parent.
*/
BOBUIableWidget::BOBUIableWidget(QWidget *parent)
    : BOBUIableView(*new BOBUIableWidgetPrivate, parent)
{
    Q_D(BOBUIableWidget);
    BOBUIableView::setModel(new BOBUIableModel(0, 0, this));
    d->setup();
}

/*!
    Creates a new table view with the given \a rows and \a columns, and with the given \a parent.
*/
BOBUIableWidget::BOBUIableWidget(int rows, int columns, QWidget *parent)
    : BOBUIableView(*new BOBUIableWidgetPrivate, parent)
{
    Q_D(BOBUIableWidget);
    BOBUIableView::setModel(new BOBUIableModel(rows, columns, this));
    d->setup();
}

/*!
    Destroys this BOBUIableWidget.
*/
BOBUIableWidget::~BOBUIableWidget()
{
    Q_D(BOBUIableWidget);
    d->clearConnections();
}

/*!
    Sets the number of rows in this table's model to \a rows. If
    this is less than rowCount(), the data in the unwanted rows
    is discarded.

    \sa setColumnCount()
*/
void BOBUIableWidget::setRowCount(int rows)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->setRowCount(rows);
}

/*!
  Returns the number of rows.
*/

int BOBUIableWidget::rowCount() const
{
    Q_D(const BOBUIableWidget);
    return d->model->rowCount();
}

/*!
    Sets the number of columns in this table's model to \a columns. If
    this is less than columnCount(), the data in the unwanted columns
    is discarded.

    \sa setRowCount()
*/
void BOBUIableWidget::setColumnCount(int columns)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->setColumnCount(columns);
}

/*!
  Returns the number of columns.
*/

int BOBUIableWidget::columnCount() const
{
    Q_D(const BOBUIableWidget);
    return d->model->columnCount();
}

/*!
  Returns the row for the \a item.
*/
int BOBUIableWidget::row(const BOBUIableWidgetItem *item) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->index(item).row();
}

/*!
  Returns the column for the \a item.
*/
int BOBUIableWidget::column(const BOBUIableWidgetItem *item) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->index(item).column();
}


/*!
    Returns the item for the given \a row and \a column if one has been set; otherwise
    returns \nullptr.

    \sa setItem()
*/
BOBUIableWidgetItem *BOBUIableWidget::item(int row, int column) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->item(row, column);
}

/*!
    Sets the item for the given \a row and \a column to \a item.

    The table takes ownership of the item.

    Note that if sorting is enabled (see
    \l{BOBUIableView::sortingEnabled} {sortingEnabled}) and \a column is
    the current sort column, the \a row will be moved to the sorted
    position determined by \a item.

    If you want to set several items of a particular row (say, by
    calling setItem() in a loop), you may want to turn off sorting
    before doing so, and turn it back on afterwards; this will allow
    you to use the same \a row argument for all items in the same row
    (i.e. setItem() will not move the row).

    \sa item(), takeItem()
*/
void BOBUIableWidget::setItem(int row, int column, BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    if (item) {
        if (Q_UNLIKELY(item->view)) {
            qWarning("BOBUIableWidget: cannot insert an item that is already owned by another BOBUIableWidget");
        } else {
            item->view = this;
            d->tableModel()->setItem(row, column, item);
        }
    } else {
        delete takeItem(row, column);
    }
}

/*!
    Removes the item at \a row and \a column from the table without deleting it.
*/
BOBUIableWidgetItem *BOBUIableWidget::takeItem(int row, int column)
{
    Q_D(BOBUIableWidget);
    BOBUIableWidgetItem *item = d->tableModel()->takeItem(row, column);
    if (item)
        item->view = nullptr;
    return item;
}

/*!
  Returns the vertical header item for row \a row.
*/
BOBUIableWidgetItem *BOBUIableWidget::verticalHeaderItem(int row) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->verticalHeaderItem(row);
}

/*!
  Sets the vertical header item for row \a row to \a item.
*/
void BOBUIableWidget::setVerticalHeaderItem(int row, BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    if (item) {
        item->view = this;
        d->tableModel()->setVerticalHeaderItem(row, item);
    } else {
        delete takeVerticalHeaderItem(row);
    }
}

/*!
    Removes the vertical header item at \a row from the header without deleting it.
*/
BOBUIableWidgetItem *BOBUIableWidget::takeVerticalHeaderItem(int row)
{
    Q_D(BOBUIableWidget);
    BOBUIableWidgetItem *itm = d->tableModel()->takeVerticalHeaderItem(row);
    if (itm)
        itm->view = nullptr;
    return itm;
}

/*!
    Returns the horizontal header item for column, \a column, if one has been
    set; otherwise returns \nullptr.
*/
BOBUIableWidgetItem *BOBUIableWidget::horizontalHeaderItem(int column) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->horizontalHeaderItem(column);
}

/*!
  Sets the horizontal header item for column \a column to \a item.
  If necessary, the column count is increased to fit the item.
  The previous header item (if there was one) is deleted.
*/
void BOBUIableWidget::setHorizontalHeaderItem(int column, BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    if (item) {
        item->view = this;
        d->tableModel()->setHorizontalHeaderItem(column, item);
    } else {
        delete takeHorizontalHeaderItem(column);
    }
}

/*!
    Removes the horizontal header item at \a column from the header without deleting it.
*/
BOBUIableWidgetItem *BOBUIableWidget::takeHorizontalHeaderItem(int column)
{
    Q_D(BOBUIableWidget);
    BOBUIableWidgetItem *itm = d->tableModel()->takeHorizontalHeaderItem(column);
    if (itm)
        itm->view = nullptr;
    return itm;
}

/*!
  Sets the vertical header labels using \a labels.
*/
void BOBUIableWidget::setVerticalHeaderLabels(const QStringList &labels)
{
    Q_D(BOBUIableWidget);
    BOBUIableModel *model = d->tableModel();
    BOBUIableWidgetItem *item = nullptr;
    for (int i = 0; i < model->rowCount() && i < labels.size(); ++i) {
        item = model->verticalHeaderItem(i);
        if (!item) {
            item = model->createItem();
            setVerticalHeaderItem(i, item);
        }
        item->setText(labels.at(i));
    }
}

/*!
  Sets the horizontal header labels using \a labels.
*/
void BOBUIableWidget::setHorizontalHeaderLabels(const QStringList &labels)
{
    Q_D(BOBUIableWidget);
    BOBUIableModel *model = d->tableModel();
    BOBUIableWidgetItem *item = nullptr;
    for (int i = 0; i < model->columnCount() && i < labels.size(); ++i) {
        item = model->horizontalHeaderItem(i);
        if (!item) {
            item = model->createItem();
            setHorizontalHeaderItem(i, item);
        }
        item->setText(labels.at(i));
    }
}

/*!
    Returns the row of the current item.

    \sa currentColumn(), setCurrentCell()
*/
int BOBUIableWidget::currentRow() const
{
    return currentIndex().row();
}

/*!
    Returns the column of the current item.

    \sa currentRow(), setCurrentCell()
*/
int BOBUIableWidget::currentColumn() const
{
    return currentIndex().column();
}

/*!
    Returns the current item.

    \sa setCurrentItem()
*/
BOBUIableWidgetItem *BOBUIableWidget::currentItem() const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->item(currentIndex());
}

/*!
    Sets the current item to \a item.

    Unless the selection mode is \l{QAbstractItemView::}{NoSelection},
    the item is also selected.

    \sa currentItem(), setCurrentCell()
*/
void BOBUIableWidget::setCurrentItem(BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    setCurrentIndex(d->tableModel()->index(item));
}

/*!
  Sets the current item to be \a item, using the given \a command.

  \sa currentItem(), setCurrentCell()
*/
void BOBUIableWidget::setCurrentItem(BOBUIableWidgetItem *item, QItemSelectionModel::SelectionFlags command)
{
    Q_D(BOBUIableWidget);
    d->selectionModel->setCurrentIndex(d->tableModel()->index(item), command);
}

/*!
    Sets the current cell to be the cell at position (\a row, \a
    column).

    Depending on the current \l{QAbstractItemView::SelectionMode}{selection mode},
    the cell may also be selected.

    \sa setCurrentItem(), currentRow(), currentColumn()
*/
void BOBUIableWidget::setCurrentCell(int row, int column)
{
    setCurrentIndex(model()->index(row, column, QModelIndex()));
}

/*!
  Sets the current cell to be the cell at position (\a row, \a
  column), using the given \a command.

  \sa setCurrentItem(), currentRow(), currentColumn()
*/
void BOBUIableWidget::setCurrentCell(int row, int column, QItemSelectionModel::SelectionFlags command)
{
    Q_D(BOBUIableWidget);
    d->selectionModel->setCurrentIndex(model()->index(row, column, QModelIndex()), command);
}

/*!
  Sorts all the rows in the table widget based on \a column and \a order.
*/
void BOBUIableWidget::sortItems(int column, BobUI::SortOrder order)
{
    Q_D(BOBUIableWidget);
    d->model->sort(column, order);
    horizontalHeader()->setSortIndicator(column, order);
}

/*!
    \internal
*/
void BOBUIableWidget::setSortingEnabled(bool enable)
{
    BOBUIableView::setSortingEnabled(enable);
}

/*!
    \internal
*/
bool BOBUIableWidget::isSortingEnabled() const
{
    return BOBUIableView::isSortingEnabled();
}

/*!
  Starts editing the \a item if it is editable.
*/

void BOBUIableWidget::editItem(BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    if (!item)
        return;
    edit(d->tableModel()->index(item));
}

/*!
  Opens an editor for the give \a item. The editor remains open after editing.

  \sa closePersistentEditor(), isPersistentEditorOpen()
*/
void BOBUIableWidget::openPersistentEditor(BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    if (!item)
        return;
    QModelIndex index = d->tableModel()->index(item);
    QAbstractItemView::openPersistentEditor(index);
}

/*!
  Closes the persistent editor for \a item.

  \sa openPersistentEditor(), isPersistentEditorOpen()
*/
void BOBUIableWidget::closePersistentEditor(BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    if (!item)
        return;
    QModelIndex index = d->tableModel()->index(item);
    QAbstractItemView::closePersistentEditor(index);
}

/*!
    \since 5.10

    Returns whether a persistent editor is open for item \a item.

    \sa openPersistentEditor(), closePersistentEditor()
*/
bool BOBUIableWidget::isPersistentEditorOpen(BOBUIableWidgetItem *item) const
{
    Q_D(const BOBUIableWidget);
    const QModelIndex index = d->tableModel()->index(item);
    return QAbstractItemView::isPersistentEditorOpen(index);
}

/*!
    Returns the widget displayed in the cell in the given \a row and \a column.

    \note The table takes ownership of the widget.

    \sa setCellWidget()
*/
QWidget *BOBUIableWidget::cellWidget(int row, int column) const
{
    QModelIndex index = model()->index(row, column, QModelIndex());
    return QAbstractItemView::indexWidget(index);
}

/*!
    Sets the given \a widget to be displayed in the cell in the given \a row
    and \a column, passing the ownership of the widget to the table.

    If cell widget A is replaced with cell widget B, cell widget A will be
    deleted. For example, in the code snippet below, the QLineEdit object will
    be deleted.

    \snippet code/src_gui_itemviews_bobuiablewidget.cpp 0

    \sa cellWidget()
*/
void BOBUIableWidget::setCellWidget(int row, int column, QWidget *widget)
{
    QModelIndex index = model()->index(row, column, QModelIndex());
    QAbstractItemView::setIndexWidget(index, widget);
}

/*!
  Selects or deselects the \a range depending on \a select.
*/
void BOBUIableWidget::setRangeSelected(const BOBUIableWidgetSelectionRange &range, bool select)
{
    if (!model()->hasIndex(range.topRow(), range.leftColumn(), rootIndex()) ||
        !model()->hasIndex(range.bottomRow(), range.rightColumn(), rootIndex()))
        return;

    QModelIndex topLeft = model()->index(range.topRow(), range.leftColumn(), rootIndex());
    QModelIndex bottomRight = model()->index(range.bottomRow(), range.rightColumn(), rootIndex());

    selectionModel()->select(QItemSelection(topLeft, bottomRight),
                             select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

/*!
  Returns a list of all selected ranges.

  \sa BOBUIableWidgetSelectionRange
*/

QList<BOBUIableWidgetSelectionRange> BOBUIableWidget::selectedRanges() const
{
    const QList<QItemSelectionRange> ranges = selectionModel()->selection();
    QList<BOBUIableWidgetSelectionRange> result;
    const int rangesCount = ranges.size();
    result.reserve(rangesCount);
    for (int i = 0; i < rangesCount; ++i)
        result.append({ranges.at(i).top(),
                       ranges.at(i).left(),
                       ranges.at(i).bottom(),
                       ranges.at(i).right()});
    return result;
}

/*!
  Returns a list of all selected items.

  This function returns a list of pointers to the contents of the
  selected cells. Use the selectedIndexes() function to retrieve the
  complete selection \e including empty cells.

  \sa selectedIndexes()
*/

QList<BOBUIableWidgetItem*> BOBUIableWidget::selectedItems() const
{
    Q_D(const BOBUIableWidget);
    const QModelIndexList indexes = selectionModel()->selectedIndexes();
    QList<BOBUIableWidgetItem*> items;
    for (const auto &index : indexes) {
        if (isIndexHidden(index))
            continue;
        BOBUIableWidgetItem *item = d->tableModel()->item(index);
        if (item)
            items.append(item);
    }
    return items;
}

/*!
  Finds items that matches the \a text using the given \a flags.
*/

QList<BOBUIableWidgetItem*> BOBUIableWidget::findItems(const QString &text, BobUI::MatchFlags flags) const
{
    Q_D(const BOBUIableWidget);
    QModelIndexList indexes;
    for (int column = 0; column < columnCount(); ++column)
        indexes += d->model->match(model()->index(0, column, QModelIndex()),
                                     BobUI::DisplayRole, text, -1, flags);
    QList<BOBUIableWidgetItem*> items;
    const int indexCount = indexes.size();
    items.reserve(indexCount);
    for (int i = 0; i < indexCount; ++i)
        items.append(d->tableModel()->item(indexes.at(i)));
    return items;
}

/*!
  Returns the visual row of the given \a logicalRow.
*/

int BOBUIableWidget::visualRow(int logicalRow) const
{
    return verticalHeader()->visualIndex(logicalRow);
}

/*!
  Returns the visual column of the given \a logicalColumn.
*/

int BOBUIableWidget::visualColumn(int logicalColumn) const
{
    return horizontalHeader()->visualIndex(logicalColumn);
}

/*!
  \fn BOBUIableWidgetItem *BOBUIableWidget::itemAt(const QPoint &point) const

  Returns a pointer to the item at the given \a point, or returns \nullptr if
  \a point is not covered by an item in the table widget.

  \sa item()
*/

BOBUIableWidgetItem *BOBUIableWidget::itemAt(const QPoint &p) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->item(indexAt(p));
}

/*!
  Returns the rectangle on the viewport occupied by the item at \a item.
*/
QRect BOBUIableWidget::visualItemRect(const BOBUIableWidgetItem *item) const
{
    Q_D(const BOBUIableWidget);
    if (!item)
        return QRect();
    QModelIndex index = d->tableModel()->index(const_cast<BOBUIableWidgetItem*>(item));
    Q_ASSERT(index.isValid());
    return visualRect(index);
}

/*!
    Scrolls the view if necessary to ensure that the \a item is visible.
    The \a hint parameter specifies more precisely where the
    \a item should be located after the operation.
*/

void BOBUIableWidget::scrollToItem(const BOBUIableWidgetItem *item, QAbstractItemView::ScrollHint hint)
{
    Q_D(BOBUIableWidget);
    if (!item)
        return;
    QModelIndex index = d->tableModel()->index(const_cast<BOBUIableWidgetItem*>(item));
    Q_ASSERT(index.isValid());
    BOBUIableView::scrollTo(index, hint);
}

/*!
    Returns the item prototype used by the table.

    \sa setItemPrototype()
*/
const BOBUIableWidgetItem *BOBUIableWidget::itemPrototype() const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->itemPrototype();
}

/*!
    Sets the item prototype for the table to the specified \a item.

    The table widget will use the item prototype clone function when it needs
    to create a new table item.  For example when the user is editing
    in an empty cell.  This is useful when you have a BOBUIableWidgetItem
    subclass and want to make sure that BOBUIableWidget creates instances of
    your subclass.

    The table takes ownership of the prototype.

    \sa itemPrototype()
*/
void BOBUIableWidget::setItemPrototype(const BOBUIableWidgetItem *item)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->setItemPrototype(item);
}

/*!
  Inserts an empty row into the table at \a row.
*/
void BOBUIableWidget::insertRow(int row)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->insertRows(row);
}

/*!
  Inserts an empty column into the table at \a column.
*/
void BOBUIableWidget::insertColumn(int column)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->insertColumns(column);
}

/*!
  Removes the row \a row and all its items from the table.
*/
void BOBUIableWidget::removeRow(int row)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->removeRows(row);
}

/*!
  Removes the column \a column and all its items from the table.
*/
void BOBUIableWidget::removeColumn(int column)
{
    Q_D(BOBUIableWidget);
    d->tableModel()->removeColumns(column);
}

/*!
   Removes all items in the view.
   This will also remove all selections and headers.
   If you don't want to remove the headers, use
   BOBUIableWidget::clearContents().
   The table dimensions stay the same.
*/

void BOBUIableWidget::clear()
{
    Q_D(BOBUIableWidget);
    selectionModel()->clear();
    d->tableModel()->clear();
}

/*!
    Removes all items not in the headers from the view.
    This will also remove all selections.
    The table dimensions stay the same.
*/
void BOBUIableWidget::clearContents()
{
    Q_D(BOBUIableWidget);
    selectionModel()->clear();
    d->tableModel()->clearContents();
}

/*!
    Returns a list of MIME types that can be used to describe a list of
    tablewidget items.

    \sa mimeData()
*/
QStringList BOBUIableWidget::mimeTypes() const
{
    return d_func()->tableModel()->QAbstractTableModel::mimeTypes();
}

/*!
    Returns an object that contains a serialized description of the specified
    \a items. The format used to describe the items is obtained from the
    mimeTypes() function.

    If the list of items is empty, \nullptr is returned rather than a
    serialized empty list.
*/
QMimeData *BOBUIableWidget::mimeData(const QList<BOBUIableWidgetItem *> &items) const
{
    Q_D(const BOBUIableWidget);

    QModelIndexList &cachedIndexes = d->tableModel()->cachedIndexes;

    // if non empty, it's called from the model's own mimeData
    if (cachedIndexes.isEmpty()) {
        cachedIndexes.reserve(items.size());
        for (BOBUIableWidgetItem *item : items)
            cachedIndexes << indexFromItem(item);

        QMimeData *result = d->tableModel()->internalMimeData();

        cachedIndexes.clear();
        return result;
    }

    return d->tableModel()->internalMimeData();
}

/*!
    Handles the \a data supplied by a drag and drop operation that ended with
    the given \a action in the given \a row and \a column.
    Returns \c true if the data and action can be handled by the model;
    otherwise returns \c false.

    \sa supportedDropActions(), supportedDragActions
*/
bool BOBUIableWidget::dropMimeData(int row, int column, const QMimeData *data, BobUI::DropAction action)
{
    QModelIndex idx;
#if BOBUI_CONFIG(draganddrop)
    if (dropIndicatorPosition() == QAbstractItemView::OnItem) {
        // QAbstractTableModel::dropMimeData will overwrite on the index if row == -1 and column == -1
        idx = model()->index(row, column);
        row = -1;
        column = -1;
    }
#endif
    return d_func()->tableModel()->QAbstractTableModel::dropMimeData(data, action , row, column, idx);
}

/*!
  Returns the drop actions supported by this view.

  \sa BobUI::DropActions, supportedDragActions, dropMimeData()
*/
BobUI::DropActions BOBUIableWidget::supportedDropActions() const
{
    return d_func()->tableModel()->QAbstractTableModel::supportedDropActions() | BobUI::MoveAction;
}

#if BOBUI_CONFIG(draganddrop)
/*!
    \property BOBUIableWidget::supportedDragActions
    \brief the drag actions supported by this view

    \since 6.10
    \sa BobUI::DropActions, supportedDropActions()
*/
BobUI::DropActions BOBUIableWidget::supportedDragActions() const
{
    Q_D(const BOBUIableWidget);
    return d->supportedDragActions.value_or(supportedDropActions());
}

void BOBUIableWidget::setSupportedDragActions(BobUI::DropActions actions)
{
    Q_D(BOBUIableWidget);
    d->supportedDragActions = actions;
}
#endif // BOBUI_CONFIG(draganddrop)

/*!
  Returns a list of pointers to the items contained in the \a data object.
  If the object was not created by a BOBUIreeWidget in the same process, the list
  is empty.

*/
QList<BOBUIableWidgetItem*> BOBUIableWidget::items(const QMimeData *data) const
{
    const BOBUIableWidgetMimeData *twd = qobject_cast<const BOBUIableWidgetMimeData*>(data);
    if (twd)
        return twd->items;
    return QList<BOBUIableWidgetItem*>();
}

/*!
  Returns the QModelIndex associated with the given \a item.

  \note In BobUI versions prior to 5.10, this function took a non-\c{const} \a item.
*/

QModelIndex BOBUIableWidget::indexFromItem(const BOBUIableWidgetItem *item) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->index(item);
}

/*!
  Returns a pointer to the BOBUIableWidgetItem associated with the given \a index.
*/

BOBUIableWidgetItem *BOBUIableWidget::itemFromIndex(const QModelIndex &index) const
{
    Q_D(const BOBUIableWidget);
    return d->tableModel()->item(index);
}

/*!
    \internal
*/
void BOBUIableWidget::setModel(QAbstractItemModel * /*model*/)
{
    Q_ASSERT(!"BOBUIableWidget::setModel() - Changing the model of the BOBUIableWidget is not allowed.");
}

/*! \reimp */
bool BOBUIableWidget::event(QEvent *e)
{
    return BOBUIableView::event(e);
}

#if BOBUI_CONFIG(draganddrop)
/*! \reimp */
void BOBUIableWidget::dropEvent(QDropEvent *event) {
    Q_D(BOBUIableWidget);
    if (event->source() == this && (event->dropAction() == BobUI::MoveAction ||
                                    dragDropMode() == QAbstractItemView::InternalMove)) {
        QModelIndex topIndex;
        int col = -1;
        int row = -1;
        // check whether a subclass has already accepted the event, ie. moved the data
        if (!event->isAccepted() && d->dropOn(event, &row, &col, &topIndex) && row == -1 && col == -1) {
            // Drop onto item
            const QModelIndexList indexes = selectedIndexes();
            int top = INT_MAX;
            int left = INT_MAX;
            for (const auto &index : indexes) {
                top = qMin(index.row(), top);
                left = qMin(index.column(), left);
            }
            QList<BOBUIableWidgetItem *> taken;
            const int indexesCount = indexes.size();
            taken.reserve(indexesCount);
            for (const auto &index : indexes)
                taken.append(takeItem(index.row(), index.column()));

            for (const auto &index : indexes) {
                int r = index.row() - top + topIndex.row();
                int c = index.column() - left + topIndex.column();
                setItem(r, c, taken.takeFirst());
            }

            event->accept();
        }
        // either we or a subclass accepted the drop event, so assume that the data was
        // moved and that QAbstractItemView shouldn't remove the source when QDrag::exec returns
        if (event->isAccepted())
            d->dropEventMoved = true;
    }

    BOBUIableView::dropEvent(event);
}
#endif

BOBUI_END_NAMESPACE

#include "moc_bobuiablewidget.cpp"
#include "moc_bobuiablewidget_p.cpp"
