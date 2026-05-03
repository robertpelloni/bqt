// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIREEWIDGET_P_H
#define BOBUIREEWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. This header file may change
// from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <BobUICore/qabstractitemmodel.h>
#include <private/qabstractitemmodel_p.h>
#include <BobUICore/qbasictimer.h>
#include <BobUIWidgets/bobuireewidget.h>
#include <private/bobuireeview_p.h>
#include <BobUIWidgets/qheaderview.h>

#include <array>

BOBUI_REQUIRE_CONFIG(treewidget);

BOBUI_BEGIN_NAMESPACE

class BOBUIreeWidgetItem;
class BOBUIreeWidgetItemIterator;
class BOBUIreeModelPrivate;

class BOBUIreeModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class BOBUIreeWidget;
    friend class BOBUIreeWidgetPrivate;
    friend class BOBUIreeWidgetItem;
    friend class BOBUIreeWidgetItemPrivate;
    friend class BOBUIreeWidgetItemIterator;
    friend class BOBUIreeWidgetItemIteratorPrivate;

public:
    explicit BOBUIreeModel(int columns = 0, BOBUIreeWidget *parent = nullptr);
    ~BOBUIreeModel();

    inline BOBUIreeWidget *view() const
        { return qobject_cast<BOBUIreeWidget*>(QObject::parent()); }

    void clear();
    void setColumnCount(int columns);

    BOBUIreeWidgetItem *item(const QModelIndex &index) const;
    void itemChanged(BOBUIreeWidgetItem *item);

    QModelIndex index(const BOBUIreeWidgetItem *item, int column) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool clearItemData(const QModelIndex &index) override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;

    QVariant headerData(int section, BobUI::Orientation orientation, int role) const override;
    bool setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value,
                       int role) override;

    BobUI::ItemFlags flags(const QModelIndex &index) const override;

    void sort(int column, BobUI::SortOrder order) override;
    void ensureSorted(int column, BobUI::SortOrder order,
                      int start, int end, const QModelIndex &parent);
    static bool itemLessThan(const std::pair<BOBUIreeWidgetItem*,int> &left,
                             const std::pair<BOBUIreeWidgetItem*,int> &right);
    static bool itemGreaterThan(const std::pair<BOBUIreeWidgetItem*,int> &left,
                                const std::pair<BOBUIreeWidgetItem*,int> &right);
    static QList<BOBUIreeWidgetItem*>::iterator sortedInsertionIterator(
        const QList<BOBUIreeWidgetItem*>::iterator &begin,
        const QList<BOBUIreeWidgetItem*>::iterator &end,
        BobUI::SortOrder order, BOBUIreeWidgetItem *item);

    bool insertRows(int row, int count, const QModelIndex &) override;
    bool insertColumns(int column, int count, const QModelIndex &) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // dnd
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, BobUI::DropAction action,
                      int row, int column, const QModelIndex &parent) override;
    BobUI::DropActions supportedDropActions() const override;
    BobUI::DropActions supportedDragActions() const override;

    QMimeData *internalMimeData() const;

    inline QModelIndex createIndexFromItem(int row, int col, BOBUIreeWidgetItem *item) const
    { return createIndex(row, col, item); }

protected:
    BOBUIreeModel(BOBUIreeModelPrivate &, BOBUIreeWidget *parent = nullptr);
    void emitDataChanged(BOBUIreeWidgetItem *item, int column, const QList<int> &roles);
    void beginInsertItems(BOBUIreeWidgetItem *parent, int row, int count);
    void endInsertItems();
    void beginRemoveItems(BOBUIreeWidgetItem *parent, int row, int count);
    void endRemoveItems();
    void sortItems(QList<BOBUIreeWidgetItem*> *items, int column, BobUI::SortOrder order);
    void timerEvent(BOBUIimerEvent *) override;

private:
    BOBUIreeWidgetItem *rootItem;
    BOBUIreeWidgetItem *headerItem;

    mutable QModelIndexList cachedIndexes;
    QList<BOBUIreeWidgetItemIterator*> iterators;

    mutable QBasicTimer sortPendingTimer;
    mutable bool skipPendingSort = false; // no sorting during internal operations
    bool inline executePendingSort() const;

    bool isChanging() const;

private:
    Q_DECLARE_PRIVATE(BOBUIreeModel)
public:
    struct SkipSorting
    {
        const BOBUIreeModel * const model;
        const bool previous;
        SkipSorting(const BOBUIreeModel *m) : model(m), previous(model ? model->skipPendingSort : false)
        { if (model) model->skipPendingSort = true; }
        ~SkipSorting() { if (model) model->skipPendingSort = previous; }
    };
    friend struct SkipSorting;
};

BOBUI_BEGIN_INCLUDE_NAMESPACE
#include "private/qabstractitemmodel_p.h"
BOBUI_END_INCLUDE_NAMESPACE

class BOBUIreeModelPrivate : public QAbstractItemModelPrivate
{
    Q_DECLARE_PUBLIC(BOBUIreeModel)
    void executePendingOperations() const override;
};

class BOBUIreeWidgetItemPrivate
{
public:
    BOBUIreeWidgetItemPrivate(BOBUIreeWidgetItem *item)
        : q(item), disabled(false), selected(false), hidden(false), rowGuess(-1),
          policy(BOBUIreeWidgetItem::DontShowIndicatorWhenChildless) {}
    void propagateDisabled(BOBUIreeWidgetItem *item);
    void updateHiddenStatus(BOBUIreeWidgetItem *item, bool inserting);
    void sortChildren(int column, BobUI::SortOrder order, bool climb);
    BOBUIreeWidgetItem *q;
    QVariantList display;
    uint disabled : 1;
    uint selected : 1;
    uint hidden : 1;
    int rowGuess;
    BOBUIreeWidgetItem::ChildIndicatorPolicy policy;
};


inline bool BOBUIreeModel::executePendingSort() const
{
    if (!skipPendingSort && sortPendingTimer.isActive() && !isChanging()) {
        sortPendingTimer.stop();
        int column = view()->header()->sortIndicatorSection();
        BobUI::SortOrder order = view()->header()->sortIndicatorOrder();
        BOBUIreeModel *that = const_cast<BOBUIreeModel*>(this);
        that->sort(column, order);
        return true;
    }
    return false;
}

class BOBUIreeWidgetPrivate : public BOBUIreeViewPrivate
{
    friend class BOBUIreeModel;
    Q_DECLARE_PUBLIC(BOBUIreeWidget)
public:
    BOBUIreeWidgetPrivate() : BOBUIreeViewPrivate(), explicitSortColumn(-1) {}
    void clearConnections();
    inline BOBUIreeModel *treeModel() const { return qobject_cast<BOBUIreeModel*>(model); }
    inline QModelIndex index(const BOBUIreeWidgetItem *item, int column = 0) const
        { return treeModel()->index(item, column); }
    inline BOBUIreeWidgetItem *item(const QModelIndex &index) const
        { return treeModel()->item(index); }
    void emitItemPressed(const QModelIndex &index);
    void emitItemClicked(const QModelIndex &index);
    void emitItemDoubleClicked(const QModelIndex &index);
    void emitItemActivated(const QModelIndex &index);
    void emitItemEntered(const QModelIndex &index);
    void emitItemChanged(const QModelIndex &index);
    void emitItemExpanded(const QModelIndex &index);
    void emitItemCollapsed(const QModelIndex &index);
    void emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &index);
    void sort();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

     // used by BOBUIreeWidgetItem::sortChildren to make sure the column argument is used
    int explicitSortColumn;

    std::array<QMetaObject::Connection, 12> connections;
    std::optional<BobUI::DropActions> supportedDragActions;
};

BOBUI_END_NAMESPACE

#endif // BOBUIREEWIDGET_P_H
