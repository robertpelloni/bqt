// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIABLEWIDGET_P_H
#define BOBUIABLEWIDGET_P_H

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
#include <qheaderview.h>
#include <bobuiablewidget.h>
#include <qabstractitemmodel.h>
#include <private/qabstractitemmodel_p.h>
#include <private/bobuiableview_p.h>
#include <private/qwidgetitemdata_p.h>

#include <array>

BOBUI_REQUIRE_CONFIG(tablewidget);

BOBUI_BEGIN_NAMESPACE

class BOBUIableWidgetMimeData : public QMimeData
{
    Q_OBJECT
public:
    QList<BOBUIableWidgetItem*> items;
};

class BOBUIableModelLessThan
{
public:
    inline bool operator()(BOBUIableWidgetItem *i1, BOBUIableWidgetItem *i2) const
        { return (*i1 < *i2); }
};

class BOBUIableModelGreaterThan
{
public:
    inline bool operator()(BOBUIableWidgetItem *i1, BOBUIableWidgetItem *i2) const
        { return (*i2 < *i1); }
};

class BOBUIableModel : public QAbstractTableModel
{
    Q_OBJECT
    friend class BOBUIableWidget;

public:
    BOBUIableModel(int rows, int columns, BOBUIableWidget *parent);
    ~BOBUIableModel();

    inline BOBUIableWidget *view() const { return qobject_cast<BOBUIableWidget *>(QObject::parent()); }

    bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex()) override;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

    void setItem(int row, int column, BOBUIableWidgetItem *item);
    BOBUIableWidgetItem *takeItem(int row, int column);
    BOBUIableWidgetItem *item(int row, int column) const;
    BOBUIableWidgetItem *item(const QModelIndex &index) const;
    void removeItem(BOBUIableWidgetItem *item);

    void setHorizontalHeaderItem(int section, BOBUIableWidgetItem *item);
    void setVerticalHeaderItem(int section, BOBUIableWidgetItem *item);
    BOBUIableWidgetItem *takeHorizontalHeaderItem(int section);
    BOBUIableWidgetItem *takeVerticalHeaderItem(int section);
    BOBUIableWidgetItem *horizontalHeaderItem(int section);
    BOBUIableWidgetItem *verticalHeaderItem(int section);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
        { return QAbstractTableModel::index(row, column, parent); }

    QModelIndex index(const BOBUIableWidgetItem *item) const;

    void setRowCount(int rows);
    void setColumnCount(int columns);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
    bool clearItemData(const QModelIndex &index) override;

    QMap<int, QVariant> itemData(const QModelIndex &index) const override;

    QVariant headerData(int section, BobUI::Orientation orientation, int role) const override;
    bool setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value, int role) override;

    BobUI::ItemFlags flags(const QModelIndex &index) const override;

    void sort(int column, BobUI::SortOrder order) override;
    static bool itemLessThan(const std::pair<BOBUIableWidgetItem*,int> &left,
                             const std::pair<BOBUIableWidgetItem*,int> &right);
    static bool itemGreaterThan(const std::pair<BOBUIableWidgetItem*,int> &left,
                                const std::pair<BOBUIableWidgetItem*,int> &right);

    void ensureSorted(int column, BobUI::SortOrder order, int start, int end);
    QList<BOBUIableWidgetItem *> columnItems(int column) const;
    void updateRowIndexes(QModelIndexList &indexes, int movedFromRow, int movedToRow);
    static QList<BOBUIableWidgetItem *>::iterator
    sortedInsertionIterator(const QList<BOBUIableWidgetItem *>::iterator &begin,
                            const QList<BOBUIableWidgetItem *>::iterator &end, BobUI::SortOrder order,
                            BOBUIableWidgetItem *item);

    bool isValid(const QModelIndex &index) const;
    inline long tableIndex(int row, int column) const
        { return (row * horizontalHeaderItems.size()) + column; }

    void clear();
    void clearContents();
    void itemChanged(BOBUIableWidgetItem *item, const QList<int> &roles = QList<int>());

    BOBUIableWidgetItem *createItem() const;
    const BOBUIableWidgetItem *itemPrototype() const;
    void setItemPrototype(const BOBUIableWidgetItem *item);

    // dnd
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, BobUI::DropAction action,
            int row, int column, const QModelIndex &parent) override;
    BobUI::DropActions supportedDropActions() const override;
    BobUI::DropActions supportedDragActions() const override;

    QMimeData *internalMimeData()  const;

private:
    const BOBUIableWidgetItem *prototype;
    QList<BOBUIableWidgetItem *> tableItems;
    QList<BOBUIableWidgetItem *> verticalHeaderItems;
    QList<BOBUIableWidgetItem *> horizontalHeaderItems;

    // A cache must be mutable if get-functions should have const modifiers
    mutable QModelIndexList cachedIndexes;
};

class BOBUIableWidgetPrivate : public BOBUIableViewPrivate
{
    Q_DECLARE_PUBLIC(BOBUIableWidget)
public:
    BOBUIableWidgetPrivate() : BOBUIableViewPrivate() {}
    inline BOBUIableModel *tableModel() const { return qobject_cast<BOBUIableModel*>(model); }
    void setup();
    void clearConnections();

    // view signals
    void emitItemPressed(const QModelIndex &index);
    void emitItemClicked(const QModelIndex &index);
    void emitItemDoubleClicked(const QModelIndex &index);
    void emitItemActivated(const QModelIndex &index);
    void emitItemEntered(const QModelIndex &index);
    // model signals
    void emitItemChanged(const QModelIndex &index);
    // selection signals
    void emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current);
    // sorting
    void sort();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    std::array<QMetaObject::Connection, 10> connections;
    std::optional<BobUI::DropActions> supportedDragActions;
};

class BOBUIableWidgetItemPrivate
{
public:
    BOBUIableWidgetItemPrivate(BOBUIableWidgetItem *item) : q(item), id(-1), headerItem(false) {}
    BOBUIableWidgetItem *q;
    int id;
    bool headerItem; // BobUI 7 TODO: inline this stuff in the public class.
};

BOBUI_END_NAMESPACE

#endif // BOBUIABLEWIDGET_P_H
