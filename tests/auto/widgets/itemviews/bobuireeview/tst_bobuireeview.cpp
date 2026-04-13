// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "../../../../shared/fakedirmodel.h"

#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QProxyStyle>
#include <QPushButton>
#include <QScrollBar>
#include <QSignalSpy>
#include <QSortFilterProxyModel>
#include <QStatusBar>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <BOBUIextEdit>
#include <BOBUIimer>
#include <BOBUIoolButton>
#include <BOBUIreeWidget>
#include <BOBUIest>
#include <QVBoxLayout>
#include <private/bobuireeview_p.h>
#include <private/bobuiesthelpers_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

using namespace BOBUIestPrivate;

#if BOBUI_CONFIG(draganddrop)
Q_DECLARE_METATYPE(QAbstractItemView::DragDropMode)
#endif
Q_DECLARE_METATYPE(QAbstractItemView::EditTriggers)
Q_DECLARE_METATYPE(QAbstractItemView::EditTrigger)

using IntBounds = std::numeric_limits<int>;
static void initStandardTreeModel(QStandardItemModel *model)
{
    QStandardItem *item;
    item = new QStandardItem(QLatin1String("Row 1 Item"));
    model->insertRow(0, item);

    item = new QStandardItem(QLatin1String("Row 2 Item"));
    item->setCheckable(true);
    model->insertRow(1, item);

    QStandardItem *childItem = new QStandardItem(QLatin1String("Row 2 Child Item"));
    item->setChild(0, childItem);

    item = new QStandardItem(QLatin1String("Row 3 Item"));
    item->setIcon(QIcon());
    model->insertRow(2, item);
}

class TreeView : public BOBUIreeView
{
    Q_OBJECT
public:
    using BOBUIreeView::BOBUIreeView;
    using BOBUIreeView::selectedIndexes;

    void paintEvent(QPaintEvent *event) override
    {
        BOBUIreeView::paintEvent(event);
        wasPainted = true;
    }
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QList<int> &roles = QList<int>()) override
    {
        BOBUIreeView::dataChanged(topLeft, bottomRight, roles);
        BOBUIreeViewPrivate *av = static_cast<BOBUIreeViewPrivate*>(bobui_widget_private(this));
        m_intersectecRect = av->intersectedRect(av->viewport->rect(), topLeft, bottomRight);
    }
    mutable QRect m_intersectecRect;
    bool wasPainted = false;
public slots:
    void handleSelectionChanged()
    {
        //let's select the last item
        QModelIndex idx = model()->index(0, 0);
        selectionModel()->select(QItemSelection(idx, idx), QItemSelectionModel::Select);
        disconnect(selectionModel(), &QItemSelectionModel::selectionChanged,
                   this, &TreeView::handleSelectionChanged);
    }
};

class tst_BOBUIreeView : public QObject
{
    Q_OBJECT

public slots:
    void selectionOrderTest();

private slots:
    void initTestCase() { QApplication::setKeyboardInputInterval(100); }
    void getSetCheck();

    // one test per BOBUIreeView property
    void construction();
    void alternatingRowColors();
    void currentIndex_data();
    void currentIndex();
#if BOBUI_CONFIG(draganddrop)
    void dragDropMode_data();
    void dragDropMode();
    void dragDropModeFromDragEnabledAndAcceptDrops_data();
    void dragDropModeFromDragEnabledAndAcceptDrops();
    void dragDropOverwriteMode();
#endif
    void editTriggers_data();
    void editTriggers();
    void hasAutoScroll();
    void horizontalScrollMode();
    void iconSize();
    void indexAt();
    void indexWidget();
    void itemDelegate();
    void itemDelegateForColumnOrRow();
    void keyboardSearch();
    void keyboardSearchMultiColumn();
    void setModel();
    void openPersistentEditor();
    void rootIndex();

    // specialized tests below
    void setHeader();
    void columnHidden();
    void rowHidden();
    void noDelegate();
    void noModel();
    void emptyModel();
    void removeRows();
    void removeCols();
    void limitedExpand();
    void expandAndCollapse_data();
    void expandAndCollapse();
    void expandAndCollapseAll();
    void expandWithNoChildren();
#if BOBUI_CONFIG(animation)
    void quickExpandCollapse();
#endif
    void keyboardNavigation();
    void headerSections();
    void moveCursor_data();
    void moveCursor();
    void setSelection_data();
    void setSelection();
    void extendedSelection_data();
    void extendedSelection();
    void indexAbove();
    void indexBelow();
    void clicked();
    void mouseDoubleClick();
    void rowsAboutToBeRemoved();
    void headerSections_unhideSection();
    void columnAt();
    void scrollTo();
    void rowsAboutToBeRemoved_move();
    void resizeColumnToContents();
    void insertAfterSelect();
    void removeAfterSelect();
    void hiddenItems();
    void spanningItems();
    void rowSizeHint();
    void setSortingEnabledTopLevel();
    void setSortingEnabledChild();
    void headerHidden();
    void indentation();

    void selection();
    void removeAndInsertExpandedCol0();
    void selectionWithHiddenItems();
    void selectAll();

    void disabledButCheckable();
    void sortByColumn_data();
    void sortByColumn();

    void evilModel_data();
    void evilModel();

    void indexRowSizeHint();
    void addRowsWhileSectionsAreHidden();
    void filterProxyModelCrash();
    void renderToPixmap_data();
    void renderToPixmap();
    void styleOptionViewItem();
    void keyboardNavigationWithDisabled();
    void saveRestoreState();

    void statusTip_data();
    void statusTip();
    void fetchMoreOnScroll();
    void checkIntersectedRect_data();
    void checkIntersectedRect();

    // task-specific tests:
    void task174627_moveLeftToRoot();
    void task171902_expandWith1stColHidden();
    void task203696_hidingColumnsAndRowsn();
    void task211293_removeRootIndex();
    void task216717_updateChildren();
    void task220298_selectColumns();
    void task224091_appendColumns();
    void task225539_deleteModel();
    void task230123_setItemsExpandable();
    void task202039_closePersistentEditor();
    void task238873_avoidAutoReopening();
    void task244304_clickOnDecoration();
    void task246536_scrollbarsNotWorking();
    void task250683_wrongSectionSize();
    void task239271_addRowsWithFirstColumnHidden();
    void task254234_proxySort();
    void task248022_changeSelection();
    void task245654_changeModelAndExpandAll();
    void doubleClickedWithSpans();
    void taskBOBUIBUG_6450_selectAllWith1stColumnHidden();
    void taskBOBUIBUG_9216_setSizeAndUniformRowHeightsWrongRepaint();
    void taskBOBUIBUG_11466_keyboardNavigationRegression();
    void taskBOBUIBUG_13567_removeLastItemRegression();
    void taskBOBUIBUG_25333_adjustViewOptionsForIndex();
    void taskBOBUIBUG_18539_emitLayoutChanged();
    void taskBOBUIBUG_8176_emitOnExpandAll();
    void taskBOBUIBUG_37813_crash();
    void taskBOBUIBUG_45697_crash();
    void taskBOBUIBUG_7232_AllowUserToControlSingleStep();
    void taskBOBUIBUG_8376();
    void taskBOBUIBUG_61476();
    void taskBOBUIBUG_42469_crash();
    void testInitialFocus();
    void fetchUntilScreenFull();
    void expandAfterTake();
};

class BobUITestModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    BobUITestModel(int _rows, int _cols, QObject *parent = nullptr)
        : QAbstractItemModel(parent), rows(_rows), cols(_cols)
    {}

    inline qint32 level(const QModelIndex &index) const
    {
        return index.isValid() ? qint32(index.internalId()) : qint32(-1);
    }

    bool canFetchMore(const QModelIndex &) const override { return !fetched; }

    void fetchMore(const QModelIndex &) override { fetched = true; }

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override
    {
        bool hasFetched = fetched;
        fetched = true;
        bool r = QAbstractItemModel::hasChildren(parent);
        fetched = hasFetched;
        return r;
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        if (!fetched)
            qFatal("%s: rowCount should not be called before fetching", Q_FUNC_INFO);
        if ((parent.column() > 0) || (level(parent) > levels))
            return 0;
        return rows;
    }
    int columnCount(const QModelIndex& parent = QModelIndex()) const override
    {
        if ((parent.column() > 0) || (level(parent) > levels))
            return 0;
        return cols;
    }

    bool isEditable(const QModelIndex &index) const
    {
        if (index.isValid())
            return true;
        return false;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if (onlyValidCalls) {
            Q_ASSERT(row >= 0);
            Q_ASSERT(column >= 0);
            Q_ASSERT(row < rows);
            Q_ASSERT(column < cols);
        }
        if (row < 0 || column < 0 || (level(parent) > levels) || column >= cols || row >= rows) {
            return QModelIndex();
        }
        QModelIndex i = createIndex(row, column, quintptr(level(parent) + 1));
        parentHash[i] = parent;
        return i;
    }

    QModelIndex parent(const QModelIndex &index) const override
    {
        if (!parentHash.contains(index))
            return QModelIndex();
        return parentHash[index];
    }

    QVariant data(const QModelIndex &idx, int role) const override
    {
        if (!idx.isValid())
            return QVariant();

        if (role == BobUI::DisplayRole) {
            if (idx.row() < 0 || idx.column() < 0 || idx.column() >= cols || idx.row() >= rows) {
                wrongIndex = true;
                qWarning("Invalid modelIndex [%d,%d,%p]", idx.row(), idx.column(),
                         idx.internalPointer());
            }
            QString result = QLatin1Char('[') + QString::number(idx.row()) + QLatin1Char(',')
                + QString::number(idx.column()) + QLatin1Char(',') +  QString::number(level(idx))
                + QLatin1Char(']');
            if (idx.row() & 1)
                result += QLatin1String(" - this item is extra wide");
            return result;
        }
        if (decorationsEnabled && role == BobUI::DecorationRole) {
            QPixmap pm(16,16);
            pm.fill(QColor::fromHsv((idx.column() % 16)*8 + 64, 254, (idx.row() % 16)*8 + 32));
            return pm;
        }
        if (statusTipsEnabled && role == BobUI::StatusTipRole)
            return QString("[%1,%2,%3] -- Status").arg(idx.row()).arg(idx.column()).arg(level(idx));
        return QVariant();
    }

    QVariant headerData(int section, BobUI::Orientation orientation,
                        int role = BobUI::DisplayRole) const override
    {
        Q_UNUSED(orientation);
        if (section < 0 || section >= columnCount())
            return QVariant();
        if (statusTipsEnabled && role == BobUI::StatusTipRole)
            return QString("Header %1 -- Status").arg(section);
        return QVariant();
    }

    void simulateMoveRows()
    {
        beginMoveRows(QModelIndex(), 0, 0, QModelIndex(), 2);
        endMoveRows();
    }

    void removeLastRow()
    {
        beginRemoveRows(QModelIndex(), rows - 1, rows - 1);
        --rows;
        endRemoveRows();
    }

    void removeAllRows()
    {
        beginRemoveRows(QModelIndex(), 0, rows - 1);
        rows = 0;
        endRemoveRows();
    }

    void removeLastColumn()
    {
        beginRemoveColumns(QModelIndex(), cols - 1, cols - 1);
        --cols;
        endRemoveColumns();
    }

    void removeAllColumns()
    {
        beginRemoveColumns(QModelIndex(), 0, cols - 1);
        cols = 0;
        endRemoveColumns();
    }

    void insertNewRow()
    {
        beginInsertRows(QModelIndex(), rows - 1, rows - 1);
        ++rows;
        endInsertRows();
    }

    void setDecorationsEnabled(bool enable)
    {
        decorationsEnabled = enable;
    }

    mutable QMap<QModelIndex,QModelIndex> parentHash;
    int rows = 0;
    int cols = 0;
    int levels = IntBounds::max();
    mutable bool wrongIndex = false;
    mutable bool fetched = false;
    bool decorationsEnabled = false;
    bool statusTipsEnabled = false;
    bool onlyValidCalls = false;
};

// Testing get/set functions
void tst_BOBUIreeView::getSetCheck()
{
    BOBUIreeView obj1;

    // int BOBUIreeView::indentation()
    // void BOBUIreeView::setIndentation(int)
    const int styledIndentation = obj1.style()->pixelMetric(
          QStyle::PM_TreeViewIndentation, nullptr, &obj1);
    QCOMPARE(obj1.indentation(), styledIndentation);
    obj1.setIndentation(0);
    QCOMPARE(obj1.indentation(), 0);
    obj1.setIndentation(IntBounds::min());
    QCOMPARE(obj1.indentation(), IntBounds::min());
    obj1.setIndentation(IntBounds::max());
    QCOMPARE(obj1.indentation(), IntBounds::max());

    // bool BOBUIreeView::rootIsDecorated()
    // void BOBUIreeView::setRootIsDecorated(bool)
    QCOMPARE(obj1.rootIsDecorated(), true);
    obj1.setRootIsDecorated(false);
    QCOMPARE(obj1.rootIsDecorated(), false);
    obj1.setRootIsDecorated(true);
    QCOMPARE(obj1.rootIsDecorated(), true);

    // bool BOBUIreeView::uniformRowHeights()
    // void BOBUIreeView::setUniformRowHeights(bool)
    QCOMPARE(obj1.uniformRowHeights(), false);
    obj1.setUniformRowHeights(false);
    QCOMPARE(obj1.uniformRowHeights(), false);
    obj1.setUniformRowHeights(true);
    QCOMPARE(obj1.uniformRowHeights(), true);

    // bool BOBUIreeView::itemsExpandable()
    // void BOBUIreeView::setItemsExpandable(bool)
    QCOMPARE(obj1.itemsExpandable(), true);
    obj1.setItemsExpandable(false);
    QCOMPARE(obj1.itemsExpandable(), false);
    obj1.setItemsExpandable(true);
    QCOMPARE(obj1.itemsExpandable(), true);

    // bool BOBUIreeView::allColumnsShowFocus
    // void BOBUIreeView::setAllColumnsShowFocus
    QCOMPARE(obj1.allColumnsShowFocus(), false);
    obj1.setAllColumnsShowFocus(false);
    QCOMPARE(obj1.allColumnsShowFocus(), false);
    obj1.setAllColumnsShowFocus(true);
    QCOMPARE(obj1.allColumnsShowFocus(), true);

    // bool BOBUIreeView::isAnimated
    // void BOBUIreeView::setAnimated
    QCOMPARE(obj1.isAnimated(), false);
    obj1.setAnimated(false);
    QCOMPARE(obj1.isAnimated(), false);
    obj1.setAnimated(true);
    QCOMPARE(obj1.isAnimated(), true);

    // bool BOBUIreeView::setSortingEnabled
    // void BOBUIreeView::isSortingEnabled
    QCOMPARE(obj1.isSortingEnabled(), false);
    obj1.setSortingEnabled(false);
    QCOMPARE(obj1.isSortingEnabled(), false);
    obj1.setSortingEnabled(true);
    QCOMPARE(obj1.isSortingEnabled(), true);
}

void tst_BOBUIreeView::construction()
{
    BOBUIreeView view;

    // QAbstractItemView properties
    QVERIFY(!view.alternatingRowColors());
    QCOMPARE(view.currentIndex(), QModelIndex());
#if BOBUI_CONFIG(draganddrop)
    QCOMPARE(view.dragDropMode(), QAbstractItemView::NoDragDrop);
    QVERIFY(!view.dragDropOverwriteMode());
    QVERIFY(!view.dragEnabled());
#endif
    QCOMPARE(view.editTriggers(), QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    QVERIFY(view.hasAutoScroll());
    QCOMPARE(view.horizontalScrollMode(), QAbstractItemView::ScrollPerPixel);
    QCOMPARE(view.iconSize(), QSize());
    QCOMPARE(view.indexAt(QPoint()), QModelIndex());
    QVERIFY(!view.indexWidget(QModelIndex()));
    QVERIFY(qobject_cast<QStyledItemDelegate *>(view.itemDelegate()));
    QVERIFY(!view.itemDelegateForColumn(-1));
    QVERIFY(!view.itemDelegateForColumn(0));
    QVERIFY(!view.itemDelegateForColumn(1));
    QVERIFY(!view.itemDelegateForRow(-1));
    QVERIFY(!view.itemDelegateForRow(0));
    QVERIFY(!view.itemDelegateForRow(1));
    QVERIFY(!view.model());
    QCOMPARE(view.rootIndex(), QModelIndex());
    QCOMPARE(view.selectionBehavior(), QAbstractItemView::SelectRows);
    QCOMPARE(view.selectionMode(), QAbstractItemView::SingleSelection);
    QVERIFY(!view.selectionModel());
#if BOBUI_CONFIG(draganddrop)
    QVERIFY(view.showDropIndicator());
#endif
    QCOMPARE(view.QAbstractItemView::sizeHintForColumn(-1), -1); // <- protected in BOBUIreeView
    QCOMPARE(view.QAbstractItemView::sizeHintForColumn(0), -1); // <- protected in BOBUIreeView
    QCOMPARE(view.QAbstractItemView::sizeHintForColumn(1), -1); // <- protected in BOBUIreeView
    QCOMPARE(view.sizeHintForIndex(QModelIndex()), QSize());
    QCOMPARE(view.sizeHintForRow(-1), -1);
    QCOMPARE(view.sizeHintForRow(0), -1);
    QCOMPARE(view.sizeHintForRow(1), -1);
    QVERIFY(!view.tabKeyNavigation());
    QCOMPARE(view.textElideMode(), BobUI::ElideRight);
    QCOMPARE(static_cast<int>(view.verticalScrollMode()),
             view.style()->styleHint(QStyle::SH_ItemView_ScrollMode, nullptr, &view));
    QCOMPARE(view.visualRect(QModelIndex()), QRect());

    // BOBUIreeView properties
    QVERIFY(!view.allColumnsShowFocus());
    QCOMPARE(view.autoExpandDelay(), -1);
    QCOMPARE(view.columnAt(-1), -1);
    QCOMPARE(view.columnAt(0), -1);
    QCOMPARE(view.columnAt(1), -1);
    QCOMPARE(view.columnViewportPosition(-1), -1);
    QCOMPARE(view.columnViewportPosition(0), -1);
    QCOMPARE(view.columnViewportPosition(1), -1);
    QCOMPARE(view.columnWidth(-1), 0);
    QCOMPARE(view.columnWidth(0), 0);
    QCOMPARE(view.columnWidth(1), 0);
    QVERIFY(view.header());
    QCOMPARE(view.indentation(),
             view.style()->pixelMetric(QStyle::PM_TreeViewIndentation, nullptr, &view));
    QCOMPARE(view.indexAbove(QModelIndex()), QModelIndex());
    QCOMPARE(view.indexBelow(QModelIndex()), QModelIndex());
    QVERIFY(!view.isAnimated());
    QVERIFY(!view.isColumnHidden(-1));
    QVERIFY(!view.isColumnHidden(0));
    QVERIFY(!view.isColumnHidden(1));
    QVERIFY(!view.isExpanded(QModelIndex()));
    QVERIFY(!view.isRowHidden(-1, QModelIndex()));
    QVERIFY(!view.isRowHidden(0, QModelIndex()));
    QVERIFY(!view.isRowHidden(1, QModelIndex()));
    QVERIFY(!view.isFirstColumnSpanned(-1, QModelIndex()));
    QVERIFY(!view.isFirstColumnSpanned(0, QModelIndex()));
    QVERIFY(!view.isFirstColumnSpanned(1, QModelIndex()));
    QVERIFY(!view.isSortingEnabled());
    QVERIFY(view.itemsExpandable());
    QVERIFY(view.rootIsDecorated());
    QVERIFY(!view.uniformRowHeights());
    QCOMPARE(view.visualRect(QModelIndex()), QRect());
    QVERIFY(!view.wordWrap());
}

void tst_BOBUIreeView::alternatingRowColors()
{
    BOBUIreeView view;
    QVERIFY(!view.alternatingRowColors());
    view.setAlternatingRowColors(true);
    QVERIFY(view.alternatingRowColors());
    view.setAlternatingRowColors(false);
    QVERIFY(!view.alternatingRowColors());

    // ### Test visual effect.
}

void tst_BOBUIreeView::currentIndex_data()
{
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("indexRow");
    BOBUIest::addColumn<int>("indexColumn");
    BOBUIest::addColumn<int>("parentIndexRow");
    BOBUIest::addColumn<int>("parentIndexColumn");

    BOBUIest::newRow("-1, -1") << -1 << -1 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("-1, 0") << -1 << 0 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("0, -1") << 0 << -1 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("0, 0") << 0 << 0 << 0 << 0 << -1 << -1;
    BOBUIest::newRow("0, 1") << 0 << 0 << 0 << 0 << -1 << -1;
    BOBUIest::newRow("1, 0") << 1 << 0 << 1 << 0 << -1 << -1;
    BOBUIest::newRow("1, 1") << 1 << 1 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("2, 0") << 2 << 0 << 2 << 0 << -1 << -1;
    BOBUIest::newRow("2, 1") << 2 << 1 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("3, -1") << 3 << -1 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("3, 0") << 3 << 0 << -1 << -1 << -1 << -1;
    BOBUIest::newRow("3, 1") << 3 << 1 << -1 << -1 << -1 << -1;
}

void tst_BOBUIreeView::currentIndex()
{
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, indexRow);
    QFETCH(int, indexColumn);
    QFETCH(int, parentIndexRow);
    QFETCH(int, parentIndexColumn);

    BOBUIreeView view;
    QStandardItemModel treeModel;
    initStandardTreeModel(&treeModel);
    view.setModel(&treeModel);

    QCOMPARE(view.currentIndex(), QModelIndex());
    view.setCurrentIndex(view.model()->index(row, column));
    QCOMPARE(view.currentIndex().row(), indexRow);
    QCOMPARE(view.currentIndex().column(), indexColumn);
    QCOMPARE(view.currentIndex().parent().row(), parentIndexRow);
    QCOMPARE(view.currentIndex().parent().column(), parentIndexColumn);

    // ### Test child and grandChild indexes.
}

#if BOBUI_CONFIG(draganddrop)

void tst_BOBUIreeView::dragDropMode_data()
{
    BOBUIest::addColumn<QAbstractItemView::DragDropMode>("dragDropMode");
    BOBUIest::addColumn<bool>("acceptDrops");
    BOBUIest::addColumn<bool>("dragEnabled");
    BOBUIest::newRow("NoDragDrop") << QAbstractItemView::NoDragDrop << false << false;
    BOBUIest::newRow("DragOnly") << QAbstractItemView::DragOnly << false << true;
    BOBUIest::newRow("DropOnly") << QAbstractItemView::DropOnly << true << false;
    BOBUIest::newRow("DragDrop") << QAbstractItemView::DragDrop << true << true;
    BOBUIest::newRow("InternalMove") << QAbstractItemView::InternalMove << true << true;
}

void tst_BOBUIreeView::dragDropMode()
{
    QFETCH(QAbstractItemView::DragDropMode, dragDropMode);
    QFETCH(bool, acceptDrops);
    QFETCH(bool, dragEnabled);

    BOBUIreeView view;
    QCOMPARE(view.dragDropMode(), QAbstractItemView::NoDragDrop);
    QVERIFY(!view.acceptDrops());
    QVERIFY(!view.dragEnabled());

    view.setDragDropMode(dragDropMode);
    QCOMPARE(view.dragDropMode(), dragDropMode);
    QCOMPARE(view.acceptDrops(), acceptDrops);
    QCOMPARE(view.dragEnabled(), dragEnabled);

    // ### Test effects of this mode
}

void tst_BOBUIreeView::dragDropModeFromDragEnabledAndAcceptDrops_data()
{
    BOBUIest::addColumn<bool>("dragEnabled");
    BOBUIest::addColumn<bool>("acceptDrops");
    BOBUIest::addColumn<QAbstractItemView::DragDropMode>("dragDropMode");
    BOBUIest::addColumn<bool>("setBehavior");
    BOBUIest::addColumn<QAbstractItemView::DragDropMode>("behavior");

    BOBUIest::newRow("NoDragDrop -1") << false << false << QAbstractItemView::NoDragDrop << false << QAbstractItemView::DragDropMode();
    BOBUIest::newRow("NoDragDrop 0") << false << false << QAbstractItemView::NoDragDrop << true << QAbstractItemView::NoDragDrop;
    BOBUIest::newRow("NoDragDrop 1") << false << false << QAbstractItemView::NoDragDrop << true << QAbstractItemView::DragOnly;
    BOBUIest::newRow("NoDragDrop 2") << false << false << QAbstractItemView::NoDragDrop << true << QAbstractItemView::DropOnly;
    BOBUIest::newRow("NoDragDrop 3") << false << false << QAbstractItemView::NoDragDrop << true << QAbstractItemView::DragDrop;
    BOBUIest::newRow("NoDragDrop 4") << false << false << QAbstractItemView::NoDragDrop << true << QAbstractItemView::InternalMove;
    BOBUIest::newRow("DragOnly -1") << true << false << QAbstractItemView::DragOnly << false << QAbstractItemView::DragDropMode();
    BOBUIest::newRow("DragOnly 0") << true << false << QAbstractItemView::DragOnly << true << QAbstractItemView::NoDragDrop;
    BOBUIest::newRow("DragOnly 1") << true << false << QAbstractItemView::DragOnly << true << QAbstractItemView::DragOnly;
    BOBUIest::newRow("DragOnly 2") << true << false << QAbstractItemView::DragOnly << true << QAbstractItemView::DropOnly;
    BOBUIest::newRow("DragOnly 3") << true << false << QAbstractItemView::DragOnly << true << QAbstractItemView::DragDrop;
    BOBUIest::newRow("DragOnly 4") << true << false << QAbstractItemView::DragOnly << true << QAbstractItemView::InternalMove;
    BOBUIest::newRow("DropOnly -1") << false << true << QAbstractItemView::DropOnly << false << QAbstractItemView::DragDropMode();
    BOBUIest::newRow("DropOnly 0") << false << true << QAbstractItemView::DropOnly << true << QAbstractItemView::NoDragDrop;
    BOBUIest::newRow("DropOnly 1") << false << true << QAbstractItemView::DropOnly << true << QAbstractItemView::DragOnly;
    BOBUIest::newRow("DropOnly 2") << false << true << QAbstractItemView::DropOnly << true << QAbstractItemView::DropOnly;
    BOBUIest::newRow("DropOnly 3") << false << true << QAbstractItemView::DropOnly << true << QAbstractItemView::DragDrop;
    BOBUIest::newRow("DropOnly 4") << false << true << QAbstractItemView::DropOnly << true << QAbstractItemView::InternalMove;
    BOBUIest::newRow("DragDrop -1") << true << true << QAbstractItemView::DragDrop << false << QAbstractItemView::DragDropMode();
    BOBUIest::newRow("DragDrop 0") << true << true << QAbstractItemView::DragDrop << false << QAbstractItemView::DragDropMode();
    BOBUIest::newRow("DragDrop 1") << true << true << QAbstractItemView::DragDrop << true << QAbstractItemView::NoDragDrop;
    BOBUIest::newRow("DragDrop 2") << true << true << QAbstractItemView::DragDrop << true << QAbstractItemView::DragOnly;
    BOBUIest::newRow("DragDrop 3") << true << true << QAbstractItemView::DragDrop << true << QAbstractItemView::DropOnly;
    BOBUIest::newRow("DragDrop 4") << true << true << QAbstractItemView::DragDrop << true << QAbstractItemView::DragDrop;
    BOBUIest::newRow("DragDrop 5") << true << true << QAbstractItemView::InternalMove << true << QAbstractItemView::InternalMove;
}

void tst_BOBUIreeView::dragDropModeFromDragEnabledAndAcceptDrops()
{
    QFETCH(bool, acceptDrops);
    QFETCH(bool, dragEnabled);
    QFETCH(QAbstractItemView::DragDropMode, dragDropMode);
    QFETCH(bool, setBehavior);
    QFETCH(QAbstractItemView::DragDropMode, behavior);

    BOBUIreeView view;
    QCOMPARE(view.dragDropMode(), QAbstractItemView::NoDragDrop);

    if (setBehavior)
        view.setDragDropMode(behavior);

    view.setAcceptDrops(acceptDrops);
    view.setDragEnabled(dragEnabled);
    QCOMPARE(view.dragDropMode(), dragDropMode);

    // ### Test effects of this mode
}

void tst_BOBUIreeView::dragDropOverwriteMode()
{
    BOBUIreeView view;
    QVERIFY(!view.dragDropOverwriteMode());
    view.setDragDropOverwriteMode(true);
    QVERIFY(view.dragDropOverwriteMode());
    view.setDragDropOverwriteMode(false);
    QVERIFY(!view.dragDropOverwriteMode());

    // ### This property changes the behavior of dropIndicatorPosition(),
    // which is protected and called only from within QListWidget and
    // BOBUIableWidget, from their reimplementations of dropMimeData(). Hard to
    // test.
}
#endif

void tst_BOBUIreeView::editTriggers_data()
{
    BOBUIest::addColumn<QAbstractItemView::EditTriggers>("editTriggers");
    BOBUIest::addColumn<QAbstractItemView::EditTrigger>("triggeredTrigger");
    BOBUIest::addColumn<bool>("editorOpened");

    // NoEditTriggers
    BOBUIest::newRow("NoEditTriggers 0") << QAbstractItemView::EditTriggers(QAbstractItemView::NoEditTriggers)
                                      << QAbstractItemView::NoEditTriggers << false;
    BOBUIest::newRow("NoEditTriggers 1") << QAbstractItemView::EditTriggers(QAbstractItemView::NoEditTriggers)
                                      << QAbstractItemView::CurrentChanged << false;
    BOBUIest::newRow("NoEditTriggers 2") << QAbstractItemView::EditTriggers(QAbstractItemView::NoEditTriggers)
                                      << QAbstractItemView::DoubleClicked << false;
    BOBUIest::newRow("NoEditTriggers 3") << QAbstractItemView::EditTriggers(QAbstractItemView::NoEditTriggers)
                                      << QAbstractItemView::SelectedClicked << false;
    BOBUIest::newRow("NoEditTriggers 4") << QAbstractItemView::EditTriggers(QAbstractItemView::NoEditTriggers)
                                      << QAbstractItemView::EditKeyPressed << false;

    // CurrentChanged
    BOBUIest::newRow("CurrentChanged 0") << QAbstractItemView::EditTriggers(QAbstractItemView::CurrentChanged)
                                      << QAbstractItemView::NoEditTriggers << false;
    BOBUIest::newRow("CurrentChanged 1") << QAbstractItemView::EditTriggers(QAbstractItemView::CurrentChanged)
                                      << QAbstractItemView::CurrentChanged << true;
    BOBUIest::newRow("CurrentChanged 2") << QAbstractItemView::EditTriggers(QAbstractItemView::CurrentChanged)
                                      << QAbstractItemView::DoubleClicked << false;
    BOBUIest::newRow("CurrentChanged 3") << QAbstractItemView::EditTriggers(QAbstractItemView::CurrentChanged)
                                      << QAbstractItemView::SelectedClicked << false;
    BOBUIest::newRow("CurrentChanged 4") << QAbstractItemView::EditTriggers(QAbstractItemView::CurrentChanged)
                                      << QAbstractItemView::EditKeyPressed << false;

    // DoubleClicked
    BOBUIest::newRow("DoubleClicked 0") << QAbstractItemView::EditTriggers(QAbstractItemView::DoubleClicked)
                                     << QAbstractItemView::NoEditTriggers << false;
    BOBUIest::newRow("DoubleClicked 1") << QAbstractItemView::EditTriggers(QAbstractItemView::DoubleClicked)
                                     << QAbstractItemView::CurrentChanged << false;
    BOBUIest::newRow("DoubleClicked 2") << QAbstractItemView::EditTriggers(QAbstractItemView::DoubleClicked)
                                     << QAbstractItemView::DoubleClicked << true;
    BOBUIest::newRow("DoubleClicked 3") << QAbstractItemView::EditTriggers(QAbstractItemView::DoubleClicked)
                                     << QAbstractItemView::SelectedClicked << false;
    BOBUIest::newRow("DoubleClicked 4") << QAbstractItemView::EditTriggers(QAbstractItemView::DoubleClicked)
                                     << QAbstractItemView::EditKeyPressed << false;

    // SelectedClicked
    BOBUIest::newRow("SelectedClicked 0") << QAbstractItemView::EditTriggers(QAbstractItemView::SelectedClicked)
                                       << QAbstractItemView::NoEditTriggers << false;
    BOBUIest::newRow("SelectedClicked 1") << QAbstractItemView::EditTriggers(QAbstractItemView::SelectedClicked)
                                       << QAbstractItemView::CurrentChanged << false;
    BOBUIest::newRow("SelectedClicked 2") << QAbstractItemView::EditTriggers(QAbstractItemView::SelectedClicked)
                                       << QAbstractItemView::DoubleClicked << false;
    BOBUIest::newRow("SelectedClicked 3") << QAbstractItemView::EditTriggers(QAbstractItemView::SelectedClicked)
                                       << QAbstractItemView::SelectedClicked << true;
    BOBUIest::newRow("SelectedClicked 4") << QAbstractItemView::EditTriggers(QAbstractItemView::SelectedClicked)
                                       << QAbstractItemView::EditKeyPressed << false;

    // EditKeyPressed
    BOBUIest::newRow("EditKeyPressed 0") << QAbstractItemView::EditTriggers(QAbstractItemView::EditKeyPressed)
                                      << QAbstractItemView::NoEditTriggers << false;
    BOBUIest::newRow("EditKeyPressed 1") << QAbstractItemView::EditTriggers(QAbstractItemView::EditKeyPressed)
                                      << QAbstractItemView::CurrentChanged << false;
    BOBUIest::newRow("EditKeyPressed 2") << QAbstractItemView::EditTriggers(QAbstractItemView::EditKeyPressed)
                                      << QAbstractItemView::DoubleClicked << false;
    BOBUIest::newRow("EditKeyPressed 3") << QAbstractItemView::EditTriggers(QAbstractItemView::EditKeyPressed)
                                      << QAbstractItemView::SelectedClicked << false;
    BOBUIest::newRow("EditKeyPressed 4") << QAbstractItemView::EditTriggers(QAbstractItemView::EditKeyPressed)
                                      << QAbstractItemView::EditKeyPressed << true;
}

void tst_BOBUIreeView::editTriggers()
{
    QFETCH(QAbstractItemView::EditTriggers, editTriggers);
    QFETCH(QAbstractItemView::EditTrigger, triggeredTrigger);
    QFETCH(bool, editorOpened);

    BOBUIreeView view;
    QStandardItemModel treeModel;
    initStandardTreeModel(&treeModel);
    view.setModel(&treeModel);
    view.show();

    QCOMPARE(view.editTriggers(), QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);

    // Initialize the first index
    view.setCurrentIndex(view.model()->index(0, 0));

    // Verify that we don't have any editor initially
    QVERIFY(!view.findChild<QLineEdit *>(QString()));

    // Set the triggers
    view.setEditTriggers(editTriggers);

    // Interact with the view
    switch (triggeredTrigger) {
    case QAbstractItemView::NoEditTriggers:
        // Do nothing, the editor shouldn't be there
        break;
    case QAbstractItemView::CurrentChanged:
        // Change the index to open an editor
        view.setCurrentIndex(view.model()->index(1, 0));
        break;
    case QAbstractItemView::DoubleClicked:
        // Doubleclick the center of the current cell
        BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {},
                          view.visualRect(view.model()->index(0, 0)).center());
        BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton, {},
                           view.visualRect(view.model()->index(0, 0)).center());
        break;
    case QAbstractItemView::SelectedClicked:
        // Click the center of the current cell
        view.selectionModel()->select(view.model()->index(0, 0), QItemSelectionModel::Select);
        BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {},
                          view.visualRect(view.model()->index(0, 0)).center());
        BOBUIest::qWait(qRound(QApplication::doubleClickInterval() * 1.5));
        break;
    case QAbstractItemView::EditKeyPressed:
        view.setFocus();
#ifdef Q_OS_MAC
        // OS X uses Enter for editing
        BOBUIest::keyPress(&view, BobUI::Key_Enter);
#else
        // All other platforms use F2
        BOBUIest::keyPress(&view, BobUI::Key_F2);
#endif
        break;
    default:
        break;
    }

    // Check if we got an editor
    BOBUIRY_COMPARE(view.findChild<QLineEdit *>(QString()) != nullptr, editorOpened);
}

void tst_BOBUIreeView::hasAutoScroll()
{
    BOBUIreeView view;
    QVERIFY(view.hasAutoScroll());
    view.setAutoScroll(false);
    QVERIFY(!view.hasAutoScroll());
    view.setAutoScroll(true);
    QVERIFY(view.hasAutoScroll());
}

void tst_BOBUIreeView::horizontalScrollMode()
{
    QStandardItemModel model;
    for (int i = 0; i < 100; ++i) {
        model.appendRow(QList<QStandardItem *>()
                        << new QStandardItem("An item that has very long text and should"
                                             " cause the horizontal scroll bar to appear.")
                        << new QStandardItem("An item that has very long text and should"
                                             " cause the horizontal scroll bar to appear."));
    }

    BOBUIreeView view;
    setFrameless(&view);
    view.setModel(&model);
    view.setFixedSize(100, 1000);
    view.header()->resizeSection(0, 2000);
    view.show();

    QCOMPARE(view.horizontalScrollMode(), QAbstractItemView::ScrollPerPixel);
    QCOMPARE(view.horizontalScrollBar()->minimum(), 0);
    QCOMPARE_GT(view.horizontalScrollBar()->maximum(), 2);

    view.setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    QCOMPARE(view.horizontalScrollMode(), QAbstractItemView::ScrollPerItem);
    QCOMPARE(view.horizontalScrollBar()->minimum(), 0);
    QCOMPARE(view.horizontalScrollBar()->maximum(), 1);

    view.setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    QCOMPARE(view.horizontalScrollMode(), QAbstractItemView::ScrollPerPixel);
    QCOMPARE(view.horizontalScrollBar()->minimum(), 0);
    QCOMPARE_GT(view.horizontalScrollBar()->maximum(), 2);
}

class RepaintTreeView : public BOBUIreeView
{
public:
    using BOBUIreeView::BOBUIreeView;
    bool repainted = false;

protected:
    void paintEvent(QPaintEvent *event) override
    { repainted = true; BOBUIreeView::paintEvent(event); }
};

void tst_BOBUIreeView::iconSize()
{
    RepaintTreeView view;
    QCOMPARE(view.iconSize(), QSize());

    QStandardItemModel treeModel;
    initStandardTreeModel(&treeModel);
    view.setModel(&treeModel);
    QCOMPARE(view.iconSize(), QSize());
    QVERIFY(!view.repainted);

    view.show();
    view.update();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    BOBUIRY_VERIFY(view.repainted);
    QCOMPARE(view.iconSize(), QSize());

    view.repainted = false;
    view.setIconSize(QSize());
    BOBUIRY_VERIFY(!view.repainted);
    QCOMPARE(view.iconSize(), QSize());

    view.setIconSize(QSize(10, 10));
    BOBUIRY_VERIFY(view.repainted);
    QCOMPARE(view.iconSize(), QSize(10, 10));

    view.repainted = false;
    view.setIconSize(QSize(10000, 10000));
    BOBUIRY_VERIFY(view.repainted);
    QCOMPARE(view.iconSize(), QSize(10000, 10000));
}

void tst_BOBUIreeView::indexAt()
{
    BobUITestModel model(5, 5);

    BOBUIreeView view;
    QCOMPARE(view.indexAt(QPoint()), QModelIndex());
    view.setModel(&model);
    QCOMPARE_NE(view.indexAt(QPoint()), QModelIndex());

    QSize itemSize = view.visualRect(model.index(0, 0)).size();
    for (int i = 0; i < model.rowCount(); ++i) {
        QPoint pos(itemSize.width() / 2, (i * itemSize.height()) + (itemSize.height() / 2));
        QModelIndex index = view.indexAt(pos);
        QCOMPARE(index, model.index(i, 0));
    }

    /*
      // ### this is wrong; the widget _will_ affect the item size
    for (int j = 0; j < model.rowCount(); ++j)
        view.setIndexWidget(model.index(j, 0), new QPushButton);
    */

    for (int k = 0; k < model.rowCount(); ++k) {
        QPoint pos(itemSize.width() / 2, (k * itemSize.height()) + (itemSize.height() / 2));
        QModelIndex index = view.indexAt(pos);
        QCOMPARE(index, model.index(k, 0));
    }

    view.show();
    view.resize(600, 600);
    view.header()->setStretchLastSection(false);
    QCOMPARE(view.indexAt(QPoint(550, 20)), QModelIndex());
}

void tst_BOBUIreeView::indexWidget()
{
    BOBUIreeView view;
    QStandardItemModel treeModel;
    initStandardTreeModel(&treeModel);
    view.setModel(&treeModel);
    view.resize(300, 400);  // make sure the width of the view is larger than the widgets below

    QModelIndex index = view.model()->index(0, 0);

    QVERIFY(!view.indexWidget(QModelIndex()));
    QVERIFY(!view.indexWidget(index));

    QString text = QLatin1String("TestLabel");

    QWidget *label = new QLabel(text);
    view.setIndexWidget(QModelIndex(), label);
    QVERIFY(!view.indexWidget(QModelIndex()));
    QVERIFY(!label->parent());
    view.setIndexWidget(index, label);
    QCOMPARE(view.indexWidget(index), label);
    QCOMPARE(label->parentWidget(), view.viewport());


    BOBUIextEdit *widget = new BOBUIextEdit(text);
    widget->setFixedSize(200,100);
    view.setIndexWidget(index, widget);
    QCOMPARE(view.indexWidget(index), static_cast<QWidget *>(widget));

    QCOMPARE(widget->parentWidget(), view.viewport());
    QCOMPARE(widget->geometry(), view.visualRect(index).intersected(widget->geometry()));
    QCOMPARE(widget->toPlainText(), text);

    //now let's try to do that later when the widget is already shown
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    index = view.model()->index(1, 0);
    QVERIFY(!view.indexWidget(index));

    widget = new BOBUIextEdit(text);
    widget->setFixedSize(200,100);
    view.setIndexWidget(index, widget);
    QCOMPARE(view.indexWidget(index), static_cast<QWidget *>(widget));

    QCOMPARE(widget->parentWidget(), view.viewport());
    QCOMPARE(widget->geometry(), view.visualRect(index).intersected(widget->geometry()));
    QCOMPARE(widget->toPlainText(), text);
}

void tst_BOBUIreeView::itemDelegate()
{
    QPointer<QAbstractItemDelegate> oldDelegate;
    QPointer<QStyledItemDelegate> otherItemDelegate;

    {
        BOBUIreeView view;
        QVERIFY(qobject_cast<QStyledItemDelegate *>(view.itemDelegate()));
        QPointer<QAbstractItemDelegate> oldDelegate = view.itemDelegate();

        otherItemDelegate = new QStyledItemDelegate;
        view.setItemDelegate(otherItemDelegate);
        QVERIFY(!otherItemDelegate->parent());
        QVERIFY(oldDelegate);

        QCOMPARE(view.itemDelegate(), otherItemDelegate);

        view.setItemDelegate(nullptr);
        QVERIFY(!view.itemDelegate()); // <- view does its own drawing?
        QVERIFY(otherItemDelegate);
    }

    // This is strange. Why doesn't setItemDelegate() reparent the delegate?
    QVERIFY(!oldDelegate);
    QVERIFY(otherItemDelegate);

    delete otherItemDelegate;
}

void tst_BOBUIreeView::itemDelegateForColumnOrRow()
{
    BOBUIreeView view;
    QAbstractItemDelegate *defaultDelegate = view.itemDelegate();
    QVERIFY(defaultDelegate);

    QVERIFY(!view.itemDelegateForRow(0));
    QVERIFY(!view.itemDelegateForColumn(0));
    QCOMPARE(view.itemDelegateForIndex(QModelIndex()), defaultDelegate);

    QStandardItemModel model;
    for (int i = 0; i < 100; ++i) {
        model.appendRow(QList<QStandardItem *>()
                        << new QStandardItem("An item that has very long text and should"
                                             " cause the horizontal scroll bar to appear.")
                        << new QStandardItem("An item that has very long text and should"
                                             " cause the horizontal scroll bar to appear.")
                        << new QStandardItem("An item that has very long text and should"
                                             " cause the horizontal scroll bar to appear."));
    }
    view.setModel(&model);

    QVERIFY(!view.itemDelegateForRow(0));
    QVERIFY(!view.itemDelegateForColumn(0));
    QCOMPARE(view.itemDelegateForIndex(QModelIndex()), defaultDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(0, 0)), defaultDelegate);

    QPointer<QAbstractItemDelegate> rowDelegate = new QStyledItemDelegate;
    view.setItemDelegateForRow(0, rowDelegate);
    QVERIFY(!rowDelegate->parent());
    QCOMPARE(view.itemDelegateForRow(0), rowDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(0, 0)), rowDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(0, 1)), rowDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(1, 0)), defaultDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(1, 1)), defaultDelegate);

    QPointer<QAbstractItemDelegate> columnDelegate = new QStyledItemDelegate;
    view.setItemDelegateForColumn(1, columnDelegate);
    QVERIFY(!columnDelegate->parent());
    QCOMPARE(view.itemDelegateForColumn(1), columnDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(0, 0)), rowDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(0, 1)), rowDelegate); // row wins
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(1, 0)), defaultDelegate);
    QCOMPARE(view.itemDelegateForIndex(view.model()->index(1, 1)), columnDelegate);

    view.setItemDelegateForRow(0, nullptr);
    QVERIFY(!view.itemDelegateForRow(0));
    QVERIFY(rowDelegate); // <- wasn't deleted

    view.setItemDelegateForColumn(1, nullptr);
    QVERIFY(!view.itemDelegateForColumn(1));
    QVERIFY(columnDelegate); // <- wasn't deleted

    delete rowDelegate;
    delete columnDelegate;
}

void tst_BOBUIreeView::keyboardSearch()
{
    BOBUIreeView view;
    QStandardItemModel model;
    model.appendRow(new QStandardItem("Andreas"));
    model.appendRow(new QStandardItem("Baldrian"));
    model.appendRow(new QStandardItem("Cecilie"));
    view.setModel(&model);
    view.show();

    // Nothing is selected
    QVERIFY(!view.selectionModel()->hasSelection());
    QVERIFY(!view.selectionModel()->isSelected(model.index(0, 0)));

    // First item is selected
    view.keyboardSearch(QLatin1String("A"));
    BOBUIRY_VERIFY(view.selectionModel()->isSelected(model.index(0, 0)));

    // First item is still selected
    view.keyboardSearch(QLatin1String("n"));
    QVERIFY(view.selectionModel()->isSelected(model.index(0, 0)));

    // No "AnB" item - keep the same selection.
    view.keyboardSearch(QLatin1String("B"));
    QVERIFY(view.selectionModel()->isSelected(model.index(0, 0)));

    // Wait a bit.
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);

    // The item that starts with B is selected.
    view.keyboardSearch(QLatin1String("B"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 0)));

    // Test that it wraps round
    model.appendRow(new QStandardItem("Andy"));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(3, 0)));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(0, 0)));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(3, 0)));

    // Test that it handles the case where the first item is hidden correctly
    model.insertRow(0, new QStandardItem("Hidden item"));
    view.setRowHidden(0, QModelIndex(), true);

    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 0)));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(4, 0)));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 0)));

    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    model.clear();
    view.setCurrentIndex(QModelIndex());
    model.appendRow({ new QStandardItem("Andreas"), new QStandardItem("Alicia") });
    model.appendRow({ new QStandardItem("Baldrian"), new QStandardItem("Belinda") });
    model.appendRow({ new QStandardItem("Cecilie"), new QStandardItem("Claire") });
    QVERIFY(!view.selectionModel()->hasSelection());
    QVERIFY(!view.selectionModel()->isSelected(model.index(0, 0)));

    // We want to search on the 2nd column so we have to force it to have
    // an index in that column as a starting point
    view.setCurrentIndex(QModelIndex(model.index(0, 1)));
    // Second item in first row is selected
    view.keyboardSearch(QLatin1String("A"));
    BOBUIRY_VERIFY(view.selectionModel()->isSelected(model.index(0, 1)));
    QVERIFY(view.currentIndex() == model.index(0, 1));

    // Second item in first row is still selected
    view.keyboardSearch(QLatin1String("l"));
    QVERIFY(view.selectionModel()->isSelected(model.index(0, 1)));
    QCOMPARE(view.currentIndex(), model.index(0, 1));

    // No "AnB" item - keep the same selection.
    view.keyboardSearch(QLatin1String("B"));
    QVERIFY(view.selectionModel()->isSelected(model.index(0, 1)));
    QCOMPARE(view.currentIndex(), model.index(0, 1));

    // Wait a bit.
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);

    // The item that starts with B is selected.
    view.keyboardSearch(QLatin1String("B"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 1)));
    QCOMPARE(view.currentIndex(), model.index(1, 1));

    // Test that it wraps round
    model.appendRow({ new QStandardItem("Andy"), new QStandardItem("Adele") });
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(3, 1)));
    QCOMPARE(view.currentIndex(), model.index(3, 1));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(0, 1)));
    QCOMPARE(view.currentIndex(), model.index(0, 1));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(3, 1)));
    QCOMPARE(view.currentIndex(), model.index(3, 1));

    // Test that it handles the case where the first item is hidden correctly
    model.insertRow(0, new QStandardItem("Hidden item"));
    view.setRowHidden(0, QModelIndex(), true);

    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 1)));
    QCOMPARE(view.currentIndex(), model.index(1, 1));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(4, 1)));
    QCOMPARE(view.currentIndex(), model.index(4, 1));
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    view.keyboardSearch(QLatin1String("A"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 1)));
    QCOMPARE(view.currentIndex(), model.index(1, 1));
}

void tst_BOBUIreeView::keyboardSearchMultiColumn()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIreeView view;
    QStandardItemModel model(4, 2);

    model.setItem(0, 0, new QStandardItem("1"));    model.setItem(0, 1, new QStandardItem("green"));
    model.setItem(1, 0, new QStandardItem("bad"));  model.setItem(1, 1, new QStandardItem("eggs"));
    model.setItem(2, 0, new QStandardItem("moof")); model.setItem(2, 1, new QStandardItem("and"));
    model.setItem(3, 0, new QStandardItem("elf"));  model.setItem(3, 1, new QStandardItem("ham"));

    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    view.setCurrentIndex(model.index(0, 1));

    // First item is selected
    view.keyboardSearch(QLatin1String("eggs"));
    QVERIFY(view.selectionModel()->isSelected(model.index(1, 1)));

    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);

    // 'ham' is selected
    view.keyboardSearch(QLatin1String("h"));
    QVERIFY(view.selectionModel()->isSelected(model.index(3, 1)));
}

void tst_BOBUIreeView::setModel()
{
    BOBUIreeView view;
    view.show();
    QCOMPARE(view.model(), nullptr);
    QCOMPARE(view.selectionModel(), nullptr);
    QCOMPARE(view.header()->model(), nullptr);
    QCOMPARE(view.header()->selectionModel(), nullptr);

    for (int x = 0; x < 2; ++x) {
        BobUITestModel *model = new BobUITestModel(10, 8);
        QAbstractItemModel *oldModel = view.model();
        QSignalSpy modelDestroyedSpy(oldModel ? oldModel : model, &QObject::destroyed);
        // set the same model twice
        for (int i = 0; i < 2; ++i) {
            QItemSelectionModel *oldSelectionModel = view.selectionModel();
            QItemSelectionModel *dummy = new QItemSelectionModel(model);
            QSignalSpy selectionModelDestroyedSpy(
                oldSelectionModel ? oldSelectionModel : dummy, &QObject::destroyed);
            view.setModel(model);
//                QCOMPARE(selectionModelDestroyedSpy.count(), (x == 0 || i == 1) ? 0 : 1);
            QCOMPARE(view.model(), model);
            QCOMPARE(view.header()->model(), model);
            QCOMPARE(view.selectionModel() != oldSelectionModel, (i == 0));
        }
        BOBUIRY_COMPARE(modelDestroyedSpy.size(), 0);

        view.setModel(nullptr);
        QCOMPARE(view.model(), nullptr);
        // ### shouldn't selectionModel also be 0 now?
//        QCOMPARE(view.selectionModel(), nullptr);
        delete model;
    }
}

void tst_BOBUIreeView::openPersistentEditor()
{
    BOBUIreeView view;
    QStandardItemModel treeModel;
    initStandardTreeModel(&treeModel);
    view.setModel(&treeModel);
    view.show();

    QVERIFY(!view.viewport()->findChild<QLineEdit *>());
    view.openPersistentEditor(view.model()->index(0, 0));
    QVERIFY(view.viewport()->findChild<QLineEdit *>());

    view.closePersistentEditor(view.model()->index(0, 0));
    QVERIFY(!view.viewport()->findChild<QLineEdit *>()->isVisible());

    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    QVERIFY(!view.viewport()->findChild<QLineEdit *>());
}

void tst_BOBUIreeView::rootIndex()
{
    BOBUIreeView view;
    QCOMPARE(view.rootIndex(), QModelIndex());
    QStandardItemModel treeModel;
    initStandardTreeModel(&treeModel);
    view.setModel(&treeModel);
    QCOMPARE(view.rootIndex(), QModelIndex());

    view.setRootIndex(view.model()->index(1, 0));

    QCOMPARE(view.model()->data(view.model()->index(0, view.header()->visualIndex(0), view.rootIndex()), BobUI::DisplayRole)
             .toString(), QString("Row 2 Child Item"));
}

void tst_BOBUIreeView::setHeader()
{
    BOBUIreeView view;
    QVERIFY(view.header() != nullptr);
    QCOMPARE(view.header()->orientation(), BobUI::Horizontal);
    QCOMPARE(view.header()->parent(), &view);
    for (int x = 0; x < 2; ++x) {
        QSignalSpy destroyedSpy(view.header(), &QObject::destroyed);
        BobUI::Orientation orient = x ? BobUI::Vertical : BobUI::Horizontal;
        QHeaderView *head = new QHeaderView(orient);
        view.setHeader(head);
        QCOMPARE(destroyedSpy.size(), 1);
        QCOMPARE(head->parent(), &view);
        QCOMPARE(view.header(), head);
        view.setHeader(head);
        QCOMPARE(view.header(), head);
        view.setHeader(nullptr);
        QCOMPARE(view.header(), head);
    }
}

void tst_BOBUIreeView::columnHidden()
{
    BOBUIreeView view;
    BobUITestModel model(10, 8);
    view.setModel(&model);
    view.show();
    for (int c = 0; c < model.columnCount(); ++c)
        QCOMPARE(view.isColumnHidden(c), false);
    // hide even columns
    for (int c = 0; c < model.columnCount(); c += 2)
        view.setColumnHidden(c, true);
    for (int c = 0; c < model.columnCount(); ++c)
        QCOMPARE(view.isColumnHidden(c), (c & 1) == 0);
    view.update();
    // hide odd columns too
    for (int c = 1; c < model.columnCount(); c += 2)
        view.setColumnHidden(c, true);
    for (int c = 0; c < model.columnCount(); ++c)
        QCOMPARE(view.isColumnHidden(c), true);
    view.update();

    // BOBUIBUG 54610
    // QAbstractItemViewPrivate::_q_layoutChanged() is called on
    // rows/columnMoved and because this function is virtual,
    // QHeaderViewPrivate::_q_layoutChanged() was called and unhided
    // all sections because QHeaderViewPrivate::_q_layoutAboutToBeChanged()
    // could not fill persistentHiddenSections (and is not needed)
    view.hideColumn(model.cols - 1);
    QCOMPARE(view.isColumnHidden(model.cols - 1), true);
    model.simulateMoveRows();
    QCOMPARE(view.isColumnHidden(model.cols - 1), true);
}

void tst_BOBUIreeView::rowHidden()
{
    BobUITestModel model(4, 6);
    model.levels = 3;
    BOBUIreeView view;
    view.resize(500,500);
    view.setModel(&model);
    view.show();

    QCOMPARE(view.isRowHidden(-1, QModelIndex()), false);
    QCOMPARE(view.isRowHidden(999999, QModelIndex()), false);
    view.setRowHidden(-1, QModelIndex(), true);
    view.setRowHidden(999999, QModelIndex(), true);
    QCOMPARE(view.isRowHidden(-1, QModelIndex()), false);
    QCOMPARE(view.isRowHidden(999999, QModelIndex()), false);

    view.setRowHidden(0, QModelIndex(), true);
    QCOMPARE(view.isRowHidden(0, QModelIndex()), true);
    view.setRowHidden(0, QModelIndex(), false);
    QCOMPARE(view.isRowHidden(0, QModelIndex()), false);

    QStack<QModelIndex> parents;
    parents.push(QModelIndex());
    while (!parents.isEmpty()) {
        QModelIndex p = parents.pop();
        if (model.canFetchMore(p))
            model.fetchMore(p);
        int rows = model.rowCount(p);
        // hide all
        for (int r = 0; r < rows; ++r) {
            view.setRowHidden(r, p, true);
            QCOMPARE(view.isRowHidden(r, p), true);
        }
        // hide none
        for (int r = 0; r < rows; ++r) {
            view.setRowHidden(r, p, false);
            QCOMPARE(view.isRowHidden(r, p), false);
        }
        // hide only even rows
        for (int r = 0; r < rows; ++r) {
            bool hide = (r & 1) == 0;
            view.setRowHidden(r, p, hide);
            QCOMPARE(view.isRowHidden(r, p), hide);
        }
        for (int r = 0; r < rows; ++r)
            parents.push(model.index(r, 0, p));
    }

    parents.push(QModelIndex());
    while (!parents.isEmpty()) {
        QModelIndex p = parents.pop();
        // all even rows should still be hidden
        for (int r = 0; r < model.rowCount(p); ++r)
            QCOMPARE(view.isRowHidden(r, p), (r & 1) == 0);
        if (model.rowCount(p) > 0) {
            for (int r = 0; r < model.rowCount(p); ++r)
                parents.push(model.index(r, 0, p));
        }
    }
}

void tst_BOBUIreeView::noDelegate()
{
    BobUITestModel model(10, 7);
    BOBUIreeView view;
    view.setModel(&model);
    view.setItemDelegate(nullptr);
    QCOMPARE(view.itemDelegate(), nullptr);
}

void tst_BOBUIreeView::noModel()
{
    BOBUIreeView view;
    view.show();
    view.setRowHidden(0, QModelIndex(), true);
    // no model -> not able to hide a row
    QVERIFY(!view.isRowHidden(0, QModelIndex()));
}

void tst_BOBUIreeView::emptyModel()
{
    BobUITestModel model(0, 0);
    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QVERIFY(!model.wrongIndex);
}

void tst_BOBUIreeView::removeRows()
{
    BobUITestModel model(7, 10);

    BOBUIreeView view;

    view.setModel(&model);
    view.show();

    model.removeLastRow();
    QVERIFY(!model.wrongIndex);

    model.removeAllRows();
    QVERIFY(!model.wrongIndex);
}

void tst_BOBUIreeView::removeCols()
{
    BobUITestModel model(5, 8);

    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    model.fetched = true;
    model.removeLastColumn();
    QVERIFY(!model.wrongIndex);
    QCOMPARE(view.header()->count(), model.cols);

    model.removeAllColumns();
    QVERIFY(!model.wrongIndex);
    QCOMPARE(view.header()->count(), model.cols);
}

void tst_BOBUIreeView::limitedExpand()
{
    {
        QStandardItemModel model;
        QStandardItem *parentItem = model.invisibleRootItem();
        parentItem->appendRow(new QStandardItem);
        parentItem->appendRow(new QStandardItem);
        parentItem->appendRow(new QStandardItem);

        QStandardItem *firstItem = model.item(0, 0);
        firstItem->setFlags(firstItem->flags() | BobUI::ItemNeverHasChildren);

        BOBUIreeView view;
        view.setModel(&model);

        QSignalSpy spy(&view, &BOBUIreeView::expanded);
        QVERIFY(spy.isValid());

        view.expand(model.index(0, 0));
        QCOMPARE(spy.size(), 0);

        view.expand(model.index(1, 0));
        QCOMPARE(spy.size(), 1);
    }
    {
        QStringListModel model(QStringList() << "one" << "two");
        BOBUIreeView view;
        view.setModel(&model);

        QSignalSpy spy(&view, &BOBUIreeView::expanded);
        QVERIFY(spy.isValid());

        view.expand(model.index(0, 0));
        QCOMPARE(spy.size(), 0);
        view.expandAll();
        QCOMPARE(spy.size(), 0);
    }
}

void tst_BOBUIreeView::expandAndCollapse_data()
{
    BOBUIest::addColumn<bool>("animationEnabled");
    BOBUIest::newRow("normal") << false;
    BOBUIest::newRow("animated") << true;
}

void tst_BOBUIreeView::expandAndCollapse()
{
    QFETCH(bool, animationEnabled);

    BobUITestModel model(10, 9);

    BOBUIreeView view;
    view.setUniformRowHeights(true);
    view.setModel(&model);
    view.setAnimated(animationEnabled);
    view.show();

    QModelIndex a = model.index(0, 0, QModelIndex());
    QModelIndex b = model.index(0, 0, a);

    QSignalSpy expandedSpy(&view, &BOBUIreeView::expanded);
    QSignalSpy collapsedSpy(&view, &BOBUIreeView::collapsed);
    QVariantList args;

    for (int y = 0; y < 2; ++y) {
        view.setVisible(y == 0);
        for (int x = 0; x < 2; ++x) {
            view.setItemsExpandable(x == 0);

            // Test bad args
            view.expand(QModelIndex());
            QCOMPARE(view.isExpanded(QModelIndex()), false);
            view.collapse(QModelIndex());
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 0);

            // expand a first level item
            QVERIFY(!view.isExpanded(a));
            view.expand(a);
            QVERIFY(view.isExpanded(a));
            QCOMPARE(expandedSpy.size(), 1);
            QCOMPARE(collapsedSpy.size(), 0);
            args = expandedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), a);

            view.expand(a);
            QVERIFY(view.isExpanded(a));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 0);

            // expand a second level item
            QVERIFY(!view.isExpanded(b));
            view.expand(b);
            QVERIFY(view.isExpanded(a));
            QVERIFY(view.isExpanded(b));
            QCOMPARE(expandedSpy.size(), 1);
            QCOMPARE(collapsedSpy.size(), 0);
            args = expandedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), b);

            view.expand(b);
            QVERIFY(view.isExpanded(b));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 0);

            // collapse the first level item
            view.collapse(a);
            QVERIFY(!view.isExpanded(a));
            QVERIFY(view.isExpanded(b));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 1);
            args = collapsedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), a);

            view.collapse(a);
            QVERIFY(!view.isExpanded(a));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 0);

            // expand the first level item again
            view.expand(a);
            QVERIFY(view.isExpanded(a));
            QVERIFY(view.isExpanded(b));
            QCOMPARE(expandedSpy.size(), 1);
            QCOMPARE(collapsedSpy.size(), 0);
            args = expandedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), a);

            // collapse the second level item
            view.collapse(b);
            QVERIFY(view.isExpanded(a));
            QVERIFY(!view.isExpanded(b));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 1);
            args = collapsedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), b);

            // collapse the first level item
            view.collapse(a);
            QVERIFY(!view.isExpanded(a));
            QVERIFY(!view.isExpanded(b));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 1);
            args = collapsedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), a);

            // expand and remove row
            QPersistentModelIndex c = model.index(9, 0, b);
            view.expand(a);
            view.expand(b);
            model.removeLastRow(); // remove c
            QVERIFY(view.isExpanded(a));
            QVERIFY(view.isExpanded(b));
            QVERIFY(!view.isExpanded(c));
            QCOMPARE(expandedSpy.size(), 2);
            QCOMPARE(collapsedSpy.size(), 0);
            args = expandedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), a);
            args = expandedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), b);

            view.collapse(a);
            view.collapse(b);
            QVERIFY(!view.isExpanded(a));
            QVERIFY(!view.isExpanded(b));
            QVERIFY(!view.isExpanded(c));
            QCOMPARE(expandedSpy.size(), 0);
            QCOMPARE(collapsedSpy.size(), 2);
            args = collapsedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), a);
            args = collapsedSpy.takeFirst();
            QCOMPARE(qvariant_cast<QModelIndex>(args.at(0)), b);
        }
    }
}

static void checkExpandState(const QAbstractItemModel &model, const BOBUIreeView &view,
                             const QModelIndex &startIdx, bool bIsExpanded, int *count)
{
    *count = 0;
    QStack<QModelIndex> parents;
    parents.push(startIdx);
    if (startIdx.isValid()) {
        QCOMPARE(view.isExpanded(startIdx), bIsExpanded);
        *count += 1;
    }
    while (!parents.isEmpty()) {
        const QModelIndex p = parents.pop();
        const int rows = model.rowCount(p);
        for (int r = 0; r < rows; ++r) {
            const QModelIndex c = model.index(r, 0, p);
            QCOMPARE(view.isExpanded(c), bIsExpanded);
            parents.push(c);
        }
        *count += rows;
    }
}

void tst_BOBUIreeView::expandAndCollapseAll()
{
    QStandardItemModel model;
    // BobUITestModel has a broken parent/child handling which will break the test
    for (int i1 = 0; i1 < 3; ++i1) {
        QStandardItem *s1 = new QStandardItem;
        s1->setText(QString::number(i1));
        model.appendRow(s1);
        for (int i2 = 0; i2 < 3; ++i2) {
            QStandardItem *s2 = new QStandardItem;
            s2->setText(QStringLiteral("%1 - %2").arg(i1).arg(i2));
            s1->appendRow(s2);
            for (int i3 = 0; i3 < 3; ++i3) {
                QStandardItem *s3 = new QStandardItem;
                s3->setText(QStringLiteral("%1 - %2 - %3").arg(i1).arg(i2).arg(i3));
                s2->appendRow(s3);
            }
        }
    }
    BOBUIreeView view;
    view.setUniformRowHeights(true);
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QSignalSpy expandedSpy(&view, &BOBUIreeView::expanded);
    QSignalSpy collapsedSpy(&view, &BOBUIreeView::collapsed);
    int count;

    view.expandAll();
    checkExpandState(model, view, QModelIndex(), true, &count);
    QCOMPARE(collapsedSpy.size(), 0);
    QCOMPARE(expandedSpy.size(),  39); // == 3 (first) + 9 (second) + 27 (third level)
    QCOMPARE(count, 39);

    collapsedSpy.clear();
    expandedSpy.clear();
    view.collapseAll();
    checkExpandState(model, view, QModelIndex(), false, &count);
    QCOMPARE(collapsedSpy.size(), 39);
    QCOMPARE(expandedSpy.size(), 0);
    QCOMPARE(count, 39);

    collapsedSpy.clear();
    expandedSpy.clear();
    view.expandRecursively(model.index(0, 0));
    QCOMPARE(expandedSpy.size(), 13); // 1 + 3 + 9

    checkExpandState(model, view, model.index(0, 0), true, &count);
    QCOMPARE(count, 13);
    checkExpandState(model, view, model.index(1, 0), false, &count);
    QCOMPARE(count, 13);
    checkExpandState(model, view, model.index(2, 0), false, &count);
    QCOMPARE(count, 13);

    expandedSpy.clear();
    view.collapseAll();
    view.expandRecursively(model.index(0, 0), 1);
    QCOMPARE(expandedSpy.size(), 4); // 1 + 3
    view.expandRecursively(model.index(0, 0), 2);
    QCOMPARE(expandedSpy.size(), 13); // (1 + 3) + 9

    checkExpandState(model, view, model.index(0, 0), true, &count);
    QCOMPARE(count, 13);
    checkExpandState(model, view, model.index(1, 0), false, &count);
    QCOMPARE(count, 13);
    checkExpandState(model, view, model.index(2, 0), false, &count);
    QCOMPARE(count, 13);
}

void tst_BOBUIreeView::expandWithNoChildren()
{
    BOBUIreeView tree;
    QStandardItemModel model(1, 1);
    tree.setModel(&model);
    tree.setAnimated(true);
    tree.doItemsLayout();
    //this test should not output warnings
    tree.expand(model.index(0,0));
}



void tst_BOBUIreeView::keyboardNavigation()
{
    const int rows = 10;
    const int columns = 7;

    BobUITestModel model(rows, columns);

    BOBUIreeView view;
    view.setModel(&model);
    view.show();

    const auto keymoves = {
        BobUI::Key_Down, BobUI::Key_Right, BobUI::Key_Right, BobUI::Key_Right,
        BobUI::Key_Down, BobUI::Key_Down, BobUI::Key_Down, BobUI::Key_Down,
        BobUI::Key_Right, BobUI::Key_Right, BobUI::Key_Right,
        BobUI::Key_Left, BobUI::Key_Up, BobUI::Key_Left, BobUI::Key_Left,
        BobUI::Key_Up, BobUI::Key_Down, BobUI::Key_Up, BobUI::Key_Up,
        BobUI::Key_Up, BobUI::Key_Up, BobUI::Key_Up, BobUI::Key_Up,
        BobUI::Key_Left, BobUI::Key_Left, BobUI::Key_Up, BobUI::Key_Down
    };

    int row    = 0;
    int column = 0;
    QModelIndex index = model.index(row, column, QModelIndex());
    view.setCurrentIndex(index);
    QCOMPARE(view.currentIndex(), index);

    for (BobUI::Key key : keymoves) {
        BOBUIest::keyClick(&view, key);

        switch (key) {
        case BobUI::Key_Up:
            if (row > 0) {
                index = index.sibling(row - 1, column);
                row -= 1;
            } else if (index.parent() != QModelIndex()) {
                index = index.parent();
                row = index.row();
            }
            break;
        case BobUI::Key_Down:
            if (view.isExpanded(index)) {
                row = 0;
                index = model.index(row, column, index);
            } else {
                row = qMin(rows - 1, row + 1);
                index = index.sibling(row, column);
            }
            break;
        case BobUI::Key_Left: {
            QScrollBar *b = view.horizontalScrollBar();
            if (b->value() == b->minimum())
                QVERIFY(!view.isExpanded(index));
            // windows style right will walk to the parent
            if (view.currentIndex() != index) {
                QCOMPARE(view.currentIndex(), index.parent());
                index = view.currentIndex();
                row = index.row();
                column = index.column();
            }
            break;
        }
        case BobUI::Key_Right:
            QVERIFY(view.isExpanded(index));
            // windows style right will walk to the first child
            if (view.currentIndex() != index) {
                QCOMPARE(view.currentIndex().parent(), index);
                row = view.currentIndex().row();
                column = view.currentIndex().column();
                index = view.currentIndex();
            }
            break;
        default:
            QFAIL(qPrintable(QStringLiteral("Unexpected key: %1").arg(key)));
        }

        QCOMPARE(view.currentIndex().row(), row);
        QCOMPARE(view.currentIndex().column(), column);
        QCOMPARE(view.currentIndex(), index);
    }
}

class Dmodel : public BobUITestModel
{
    Q_OBJECT
public:
    using BobUITestModel::BobUITestModel;
    int columnCount(const QModelIndex &parent) const override
    {
        if (parent.row() == 5)
            return 1;
        return BobUITestModel::columnCount(parent);
    }
};

void tst_BOBUIreeView::headerSections()
{
    Dmodel model(10, 10);

    BOBUIreeView view;
    QHeaderView *header = view.header();

    view.setModel(&model);
    QModelIndex index = model.index(5, 0);
    view.setRootIndex(index);
    QCOMPARE(model.columnCount(QModelIndex()), 10);
    QCOMPARE(model.columnCount(index), 1);
    QCOMPARE(header->count(), model.columnCount(index));
}

void tst_BOBUIreeView::moveCursor_data()
{
    BOBUIest::addColumn<bool>("uniformRowHeights");
    BOBUIest::addColumn<bool>("scrollPerPixel");
    BOBUIest::newRow("uniformRowHeights = false, scrollPerPixel = false")
        << false << false;
    BOBUIest::newRow("uniformRowHeights = true, scrollPerPixel = false")
        << true << false;
    BOBUIest::newRow("uniformRowHeights = false, scrollPerPixel = true")
        << false << true;
    BOBUIest::newRow("uniformRowHeights = true, scrollPerPixel = true")
        << true << true;
}

void tst_BOBUIreeView::moveCursor()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(bool, uniformRowHeights);
    QFETCH(bool, scrollPerPixel);
    BobUITestModel model(8, 6);

    BOBUIreeView view;
    view.setUniformRowHeights(uniformRowHeights);
    view.setModel(&model);
    view.setRowHidden(0, QModelIndex(), true);
    view.setVerticalScrollMode(scrollPerPixel ?
            QAbstractItemView::ScrollPerPixel :
            QAbstractItemView::ScrollPerItem);
    QVERIFY(view.isRowHidden(0, QModelIndex()));
    view.setColumnHidden(0, true);
    QVERIFY(view.isColumnHidden(0));
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    //here the first visible index should be selected
    //because the view got the focus
    QModelIndex expected = model.index(1, 1, QModelIndex());
    QCOMPARE(view.currentIndex(), expected);

    //then pressing down should go to the next line
    QModelIndex actual = view.moveCursor(BOBUIreeView::MoveDown, BobUI::NoModifier);
    expected = model.index(2, 1, QModelIndex());
    QCOMPARE(actual, expected);

    view.setRowHidden(0, QModelIndex(), false);
    view.setColumnHidden(0, false);

    // PageUp was broken with uniform row heights turned on
    view.setCurrentIndex(model.index(1, 0));
    actual = view.moveCursor(BOBUIreeView::MovePageUp, BobUI::NoModifier);
    expected = model.index(0, 0, QModelIndex());
    QCOMPARE(actual, expected);

    //let's try another column
    view.setCurrentIndex(model.index(1, 1));
    view.setSelectionBehavior(QAbstractItemView::SelectItems);
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Up);
    expected = model.index(0, 1, QModelIndex());
    QCOMPARE(view.currentIndex(), expected);
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Down);
    expected = model.index(1, 1, QModelIndex());
    QCOMPARE(view.currentIndex(), expected);
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Up);
    expected = model.index(0, 1, QModelIndex());
    QCOMPARE(view.currentIndex(), expected);
    view.setColumnHidden(0, true);
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Left);
    expected = model.index(0, 1, QModelIndex()); //it shouldn't have changed
    QCOMPARE(view.currentIndex(), expected);
    view.setColumnHidden(0, false);
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Left);
    expected = model.index(0, 0, QModelIndex());
    QCOMPARE(view.currentIndex(), expected);
}

class TestDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override
    { return QSize(200, 50); }
};

typedef QList<QPoint> PointList;

void tst_BOBUIreeView::setSelection_data()
{
    BOBUIest::addColumn<QRect>("selectionRect");
    BOBUIest::addColumn<QAbstractItemView::SelectionMode>("selectionMode");
    BOBUIest::addColumn<QItemSelectionModel::SelectionFlags>("selectionCommand");
    BOBUIest::addColumn<PointList>("expectedItems");
    BOBUIest::addColumn<int>("verticalOffset");

    const PointList pl1{QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(3, 0), QPoint(4, 0)};
    const PointList pl2{QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(3, 0), QPoint(4, 0),
                        QPoint(0, 1), QPoint(1, 1), QPoint(2, 1), QPoint(3, 1), QPoint(4, 1)};
    const QItemSelectionModel::SelectionFlags selFlags(QItemSelectionModel::ClearAndSelect |
                                                       QItemSelectionModel::Rows);
    BOBUIest::newRow("(0,0,50,20),rows")
       << QRect(0, 0, 50, 20)
       << QAbstractItemView::SingleSelection
       << selFlags << pl1 << 0;

    BOBUIest::newRow("(0,0,50,90),rows")
       << QRect(0, 0, 50, 90)
       << QAbstractItemView::ExtendedSelection
       << selFlags << pl2 << 0;

    BOBUIest::newRow("(50,0,0,90),rows,invalid rect")
       << QRect(QPoint(50, 0), QPoint(0, 90))
       << QAbstractItemView::ExtendedSelection
       << selFlags << pl2 << 0;

    BOBUIest::newRow("(0,-20,20,50),rows")
       << QRect(0, -20, 20, 50)
       << QAbstractItemView::ExtendedSelection
       << selFlags << pl2 << 1;
    BOBUIest::newRow("(0,-50,20,90),rows")
       << QRect(0, -50, 20, 90)
       << QAbstractItemView::ExtendedSelection
       << selFlags << pl2 << 1;
}

void tst_BOBUIreeView::setSelection()
{
    QFETCH(QRect, selectionRect);
    QFETCH(QAbstractItemView::SelectionMode, selectionMode);
    QFETCH(QItemSelectionModel::SelectionFlags, selectionCommand);
    QFETCH(PointList, expectedItems);
    QFETCH(int, verticalOffset);

    BobUITestModel model(100, 5);
    model.levels = 1;
    model.setDecorationsEnabled(true);
    BOBUIreeView view;
    view.resize(400, 300);
    view.show();
    view.setRootIsDecorated(false);
    view.setItemDelegate(new TestDelegate(&view));
    view.setSelectionMode(selectionMode);
    view.setModel(&model);
    view.setUniformRowHeights(true);
    view.setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    view.scrollTo(model.index(verticalOffset, 0), QAbstractItemView::PositionAtTop);
    view.setSelection(selectionRect, selectionCommand);
    QItemSelectionModel *selectionModel = view.selectionModel();
    QVERIFY(selectionModel);

    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    QCOMPARE(selectedIndexes.size(), expectedItems.size());
    for (const QModelIndex &idx : selectedIndexes)
        QVERIFY(expectedItems.contains(QPoint(idx.column(), idx.row())));
}

void tst_BOBUIreeView::indexAbove()
{
    BobUITestModel model(6, 7);
    model.levels = 2;
    BOBUIreeView view;

    QCOMPARE(view.indexAbove(QModelIndex()), QModelIndex());
    view.setModel(&model);
    QCOMPARE(view.indexAbove(QModelIndex()), QModelIndex());

    QStack<QModelIndex> parents;
    parents.push(QModelIndex());
    while (!parents.isEmpty()) {
        QModelIndex p = parents.pop();
        if (model.canFetchMore(p))
            model.fetchMore(p);
        int rows = model.rowCount(p);
        for (int r = rows - 1; r > 0; --r) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex idx = model.index(r, column, p);
                QModelIndex expected = model.index(r - 1, column, p);
                QCOMPARE(view.indexAbove(idx), expected);
            }
        }
        // hide even rows
        for (int r = 0; r < rows; r+=2)
            view.setRowHidden(r, p, true);
        for (int r = rows - 1; r > 0; r-=2) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex idx = model.index(r, column, p);
                QModelIndex expected = model.index(r - 2, column, p);
                QCOMPARE(view.indexAbove(idx), expected);
            }
        }
//        for (int r = 0; r < rows; ++r)
//            parents.push(model.index(r, 0, p));
    }
}

void tst_BOBUIreeView::indexBelow()
{
    BobUITestModel model(2, 2);

    BOBUIreeView view;
    view.setModel(&model);
    view.show();

    {
        QModelIndex i = model.index(0, 0, view.rootIndex());
        QVERIFY(i.isValid());
        QCOMPARE(i.row(), 0);
        QCOMPARE(i.column(), 0);

        i = view.indexBelow(i);
        QVERIFY(i.isValid());
        QCOMPARE(i.row(), 1);
        QCOMPARE(i.column(), 0);
        i = view.indexBelow(i);
        QVERIFY(!i.isValid());
    }

    {
        QModelIndex i = model.index(0, 1, view.rootIndex());
        QVERIFY(i.isValid());
        QCOMPARE(i.row(), 0);
        QCOMPARE(i.column(), 1);

        i = view.indexBelow(i);
        QVERIFY(i.isValid());
        QCOMPARE(i.row(), 1);
        QCOMPARE(i.column(), 1);
        i = view.indexBelow(i);
        QVERIFY(!i.isValid());
    }
}

void tst_BOBUIreeView::clicked()
{
    BobUITestModel model(10, 2);

    BOBUIreeView view;
    view.setModel(&model);
    view.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QModelIndex firstIndex = model.index(0, 0, QModelIndex());
    QVERIFY(firstIndex.isValid());
    int itemHeight = view.visualRect(firstIndex).height();
    int itemOffset = view.visualRect(firstIndex).width() / 2;
    view.resize(200, itemHeight * (model.rows + 2));

    for (int i = 0; i < model.rowCount(); ++i) {
        QPoint p(itemOffset, 1 + itemHeight * i);
        QModelIndex index = view.indexAt(p);
        if (!index.isValid())
            continue;
        QSignalSpy spy(&view, &BOBUIreeView::clicked);
        BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
        BOBUIRY_COMPARE(spy.size(), 1);
    }
}

void tst_BOBUIreeView::mouseDoubleClick()
{
    // Test double clicks outside the viewport.
    // (Should be a no-op and should not expand any item.)

    QStandardItemModel model(20, 2);
    for (int i = 0; i < model.rowCount(); i++) {
        QModelIndex index = model.index(i, 0, QModelIndex());
        model.insertRows(0, 20, index);
        model.insertColumns(0, 2,index);
        for (int i1 = 0; i1 <  model.rowCount(index); i1++) {
            QVERIFY(model.index(i1, 0, index).isValid());
        }
    }

    BOBUIreeView view;
    view.setModel(&model);

    // make sure the viewport height is smaller than the contents height.
    view.resize(200, 200);
    view.move(0, 0);
    view.show();
    QModelIndex index = model.index(0, 0, QModelIndex());
    view.setCurrentIndex(index);

    view.setExpanded(model.index(0,0, QModelIndex()), true);
    view.setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOn);
    view.setVerticalScrollBarPolicy(BobUI::ScrollBarAlwaysOn);

    // Make sure all items are collapsed
    for (int i = 0; i < model.rowCount(QModelIndex()); i++)
        view.setExpanded(model.index(i, 0, QModelIndex()), false);

    int maximum = view.verticalScrollBar()->maximum();

    // Doubleclick in the bottom right corner, in the unused area between the vertical and horizontal scrollbar.
    int vsw = view.verticalScrollBar()->width();
    int hsh = view.horizontalScrollBar()->height();
    BOBUIest::mouseDClick(&view, BobUI::LeftButton, BobUI::NoModifier, QPoint(view.width() - vsw + 1, view.height() - hsh + 1));
    // Should not have expanded, thus maximum() should have the same value.
    QCOMPARE(maximum, view.verticalScrollBar()->maximum());

    view.setExpandsOnDoubleClick(false);
    BOBUIest::mouseDClick(&view, BobUI::LeftButton, BobUI::NoModifier, view.visualRect(index).center());
    QVERIFY(!view.isExpanded(index));
}

void tst_BOBUIreeView::rowsAboutToBeRemoved()
{
    QStandardItemModel model(3, 1);
    for (int i = 0; i < model.rowCount(); i++) {
        const QString iS = QString::number(i);
        QModelIndex index = model.index(i, 0, QModelIndex());
        model.setData(index, iS);
        model.insertRows(0, 4, index);
        model.insertColumns(0,1,index);
        for (int i1 = 0; i1 <  model.rowCount(index); i1++) {
            QModelIndex index2 = model.index(i1, 0, index);
            model.setData(index2, iS + QString::number(i1));
        }
    }

    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QModelIndex index = model.index(0,0, QModelIndex());
    view.setCurrentIndex(index);
    view.setExpanded(model.index(0,0, QModelIndex()), true);

    for (int i = 0; i < model.rowCount(QModelIndex()); i++)
        view.setExpanded(model.index(i, 0, QModelIndex()), true);

    QSignalSpy spy1(&model, &QAbstractItemModel::rowsAboutToBeRemoved);

    model.removeRows(1,1);
    QCOMPARE((view.state()), 0);
    // Should not be 5 (or any other number for that sake :)
    QCOMPARE(spy1.size(), 1);

}

void tst_BOBUIreeView::headerSections_unhideSection()
{
    BobUITestModel model(10, 7);

    BOBUIreeView view;

    view.setModel(&model);
    view.show();
    int size = view.header()->sectionSize(0);
    view.setColumnHidden(0, true);

    // should go back to old size
    view.setColumnHidden(0, false);
    QCOMPARE(size, view.header()->sectionSize(0));
}

void tst_BOBUIreeView::columnAt()
{
    BobUITestModel model(10, 10);
    BOBUIreeView view;
    view.resize(500,500);
    view.setModel(&model);

    QCOMPARE(view.columnAt(0), 0);
    // really this is testing the header... so not much more should be needed if the header is working...
}

void tst_BOBUIreeView::scrollTo()
{
#define CHECK_VISIBLE(ROW,COL) QVERIFY(QRect(QPoint(),view.viewport()->size()).contains(\
                    view.visualRect(model.index((ROW),(COL),QModelIndex()))))

    BobUITestModel model(100, 100);
    BOBUIreeView view;
    view.setUniformRowHeights(true);
    view.scrollTo(QModelIndex(), BOBUIreeView::PositionAtTop);
    view.setModel(&model);

    // ### check the scrollbar values an make sure it actually scrolls to the item
    // ### check for bot item based and pixel based scrolling
    // ### create a data function for this test

    view.scrollTo(QModelIndex());
    view.scrollTo(model.index(0, 0, QModelIndex()));
    view.scrollTo(model.index(0, 0, QModelIndex()), BOBUIreeView::PositionAtTop);
    view.scrollTo(model.index(0, 0, QModelIndex()), BOBUIreeView::PositionAtBottom);

    view.show();
    view.setVerticalScrollMode(QAbstractItemView::ScrollPerItem); //some styles change that in Polish
    view.resize(300, 200);

    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    //view.verticalScrollBar()->setValue(0);

    view.scrollTo(model.index(0, 0, QModelIndex()));
    CHECK_VISIBLE(0,0);
    QCOMPARE(view.verticalScrollBar()->value(), 0);

    view.header()->resizeSection(0, 5); // now we only see the branches
    view.scrollTo(model.index(5, 0, QModelIndex()), BOBUIreeView::PositionAtTop);
    QCOMPARE(view.verticalScrollBar()->value(), 5);

    view.scrollTo(model.index(60, 60, QModelIndex()));

    CHECK_VISIBLE(60,60);
    view.scrollTo(model.index(60, 30, QModelIndex()));
    CHECK_VISIBLE(60,30);
    view.scrollTo(model.index(30, 30, QModelIndex()));
    CHECK_VISIBLE(30,30);

    // TODO force it to move to the left and then the right
}

void tst_BOBUIreeView::rowsAboutToBeRemoved_move()
{
    QStandardItemModel model(3,1);
    BOBUIreeView view;
    view.setModel(&model);
    QModelIndex indexThatWantsToLiveButWillDieDieITellYou;
    QModelIndex parent = model.index(2, 0);
    view.expand(parent);
    for (int i = 0; i < 6; ++i) {
        model.insertRows(0, 1, parent);
        model.insertColumns(0, 1, parent);
        QModelIndex index = model.index(0, 0, parent);
        view.expand(index);
        if (i == 3)
            indexThatWantsToLiveButWillDieDieITellYou = index;
        model.setData(index, i);
        parent = index;
    }
    view.resize(600,800);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    view.doItemsLayout();
    view.executeDelayedItemsLayout();
    parent = indexThatWantsToLiveButWillDieDieITellYou.parent();
    QCOMPARE(view.isExpanded(indexThatWantsToLiveButWillDieDieITellYou), true);
    QCOMPARE(parent.isValid(), true);
    QCOMPARE(parent.parent().isValid(), true);
    view.expand(parent);
    QCOMPARE(view.isExpanded(parent), true);
    QCOMPARE(view.isExpanded(indexThatWantsToLiveButWillDieDieITellYou), true);
    model.removeRow(0, indexThatWantsToLiveButWillDieDieITellYou);
    QCOMPARE(view.isExpanded(parent), true);
    QCOMPARE(view.isExpanded(indexThatWantsToLiveButWillDieDieITellYou), true);
}

void tst_BOBUIreeView::resizeColumnToContents()
{
    QStandardItemModel model(50,2);
    for (int r = 0; r < model.rowCount(); ++r) {
        const QString rS = QString::number(r);
        for (int c = 0; c < model.columnCount(); ++c) {
            QModelIndex idx = model.index(r, c);
            model.setData(idx, rS + QLatin1Char(',') + QString::number(c));
            model.insertColumns(0, 2, idx);
            model.insertRows(0, 6, idx);
            for (int i = 0; i < 6; ++i) {
                const QString iS = QString::number(i);
                for (int j = 0; j < 2 ; ++j) {
                    model.setData(model.index(i, j, idx), QLatin1String("child") + iS + QString::number(j));
                }
            }
        }
    }
    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    view.scrollToBottom();
    view.resizeColumnToContents(0);
    int oldColumnSize = view.header()->sectionSize(0);
    view.setRootIndex(model.index(0, 0));
    view.resizeColumnToContents(0);        //Earlier, this gave an assert
    QCOMPARE_GT(view.header()->sectionSize(0), oldColumnSize);
}

void tst_BOBUIreeView::insertAfterSelect()
{
    BobUITestModel model(10, 10);
    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QModelIndex firstIndex = model.index(0, 0, QModelIndex());
    QVERIFY(firstIndex.isValid());
    int itemOffset = view.visualRect(firstIndex).width() / 2;
    QPoint p(itemOffset, 1);
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
    QVERIFY(view.selectionModel()->isSelected(firstIndex));
    model.insertNewRow();
    QVERIFY(view.selectionModel()->isSelected(firstIndex)); // Should still be selected
}

void tst_BOBUIreeView::removeAfterSelect()
{
    BobUITestModel model(10, 10);
    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QModelIndex firstIndex = model.index(0, 0, QModelIndex());
    QVERIFY(firstIndex.isValid());
    int itemOffset = view.visualRect(firstIndex).width() / 2;
    QPoint p(itemOffset, 1);
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
    QVERIFY(view.selectionModel()->isSelected(firstIndex));
    model.removeLastRow();
    QVERIFY(view.selectionModel()->isSelected(firstIndex)); // Should still be selected
}

void tst_BOBUIreeView::hiddenItems()
{
    BobUITestModel model(10, 10);
    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QModelIndex firstIndex = model.index(1, 0, QModelIndex());
    QVERIFY(firstIndex.isValid());
    if (model.canFetchMore(firstIndex))
        model.fetchMore(firstIndex);
    for (int i = 0; i < model.rowCount(firstIndex); i++)
        view.setRowHidden(i , firstIndex, true );

    int itemOffset = view.visualRect(firstIndex).width() / 2;
    int itemHeight = view.visualRect(firstIndex).height();
    QPoint p(itemOffset, itemHeight + 1);
    view.setExpanded(firstIndex, false);
    BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
    QCOMPARE(view.isExpanded(firstIndex), false);

    p.setX(5);
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
    QCOMPARE(view.isExpanded(firstIndex), false);
}

void tst_BOBUIreeView::spanningItems()
{
    BobUITestModel model(10, 10);
    model.onlyValidCalls = true;
    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    int itemWidth = view.header()->sectionSize(0);
    int itemHeight = view.visualRect(model.index(0, 0, QModelIndex())).height();

    // every second row is spanning
    for (int i = 1; i < model.rowCount(QModelIndex()); i += 2)
        view.setFirstColumnSpanned(i , QModelIndex(), true);

    // non-spanning item
    QPoint p(itemWidth / 2, itemHeight / 2); // column 0, row 0
    view.setCurrentIndex(QModelIndex());
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
    QCOMPARE(view.currentIndex(), model.index(0, 0, QModelIndex()));
    QCOMPARE(view.header()->sectionSize(0) - view.indentation(),
             view.visualRect(model.index(0, 0, QModelIndex())).width());

    // spanning item
    p.setX(itemWidth + (itemWidth / 2)); // column 1
    p.setY(itemHeight + (itemHeight / 2)); // row 1
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, BobUI::NoModifier, p);
    QCOMPARE(view.currentIndex(), model.index(1, 0, QModelIndex()));
    QCOMPARE(view.header()->length() - view.indentation(),
             view.visualRect(model.index(1, 0, QModelIndex())).width());

    // size hint
    // every second row is un-spanned
    QStyleOptionViewItem option;
    view.initViewItemOption(&option);
    int w = view.header()->sectionSizeHint(0);
    for (int i = 0; i < model.rowCount(QModelIndex()); ++i) {
        if (!view.isFirstColumnSpanned(i, QModelIndex())) {
            QModelIndex index = model.index(i, 0, QModelIndex());
            w = qMax(w, view.itemDelegateForIndex(index)->sizeHint(option, index).width() + view.indentation());
        }
    }
    QCOMPARE(view.sizeHintForColumn(0), w);

    view.repaint(); // to check that this doesn't hit any assert
}

void tst_BOBUIreeView::selectionOrderTest()
{
    QVERIFY(static_cast<QItemSelectionModel*>(sender())->currentIndex().row() != -1);
}

void tst_BOBUIreeView::selection()
{
    if (!QGuiApplication::platformName().compare(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This causes a crash triggered by setVisible(false)");

    BOBUIreeView treeView;
    QStandardItemModel m(10, 2);
    for (int i = 0;i < 10; ++i)
        m.setData(m.index(i, 0), i);
    treeView.setModel(&m);
    treeView.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&treeView));

    treeView.setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView.setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(treeView.selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &tst_BOBUIreeView::selectionOrderTest);

    BOBUIest::mousePress(treeView.viewport(), BobUI::LeftButton, {},
                      treeView.visualRect(m.index(1, 0)).center());
    BOBUIest::keyPress(treeView.viewport(), BobUI::Key_Down);
    auto selectedRows = treeView.selectionModel()->selectedRows();
    QCOMPARE(selectedRows.size(), 1);
    QCOMPARE(selectedRows.first(), m.index(2, 0, QModelIndex()));
    BOBUIest::keyPress(treeView.viewport(), BobUI::Key_5);
    selectedRows = treeView.selectionModel()->selectedRows();
    QCOMPARE(selectedRows.size(), 1);
    QCOMPARE(selectedRows.first(), m.index(5, 0, QModelIndex()));
}

//From task 151686 BOBUIreeView ExtendedSelection selects hidden rows
void tst_BOBUIreeView::selectionWithHiddenItems()
{
    QStandardItemModel model;

    QStandardItem item0("row 0");
    QStandardItem item1("row 1");
    QStandardItem item2("row 2");
    QStandardItem item3("row 3");
    model.appendColumn({&item0, &item1, &item2, &item3});

    QStandardItem child("child");
    item1.appendRow(&child);

    BOBUIreeView view;
    view.setModel(&model);
    view.setSelectionMode(QAbstractItemView::ExtendedSelection);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    //child should not be selected as it is hidden (its parent is not expanded)
    view.selectAll();
    QCOMPARE(view.selectionModel()->selection().size(), 1); //one range
    QCOMPARE(view.selectionModel()->selectedRows().size(), 4);
    view.expandAll();
    QVERIFY(view.isExpanded(item1.index()));
    QCOMPARE(view.selectionModel()->selection().size(), 1);
    QCOMPARE(view.selectionModel()->selectedRows().size(), 4);
    QVERIFY( !view.selectionModel()->isSelected(model.indexFromItem(&child)));
    view.clearSelection();
    QVERIFY(view.isExpanded(item1.index()));

    //child should be selected as it is visible (its parent is expanded)
    view.selectAll();
    QCOMPARE(view.selectionModel()->selection().size(), 2);
    QCOMPARE(view.selectionModel()->selectedRows().size(), 5); //everything is selected
    view.clearSelection();

    //we hide the node with a child (there should then be 3 items selected in 2 ranges)
    view.setRowHidden(1, QModelIndex(), true);
    QVERIFY(view.isExpanded(item1.index()));
    view.selectAll();
    QCOMPARE(view.selectionModel()->selection().size(), 2);
    QCOMPARE(view.selectionModel()->selectedRows().size(), 3);
    QVERIFY(!view.selectionModel()->isSelected(model.indexFromItem(&item1)));
    QVERIFY(!view.selectionModel()->isSelected(model.indexFromItem(&child)));

    view.setRowHidden(1, QModelIndex(), false);
    QVERIFY(view.isExpanded(item1.index()));
    view.clearSelection();

    //we hide a node without children (there should then be 4 items selected in 3 ranges)
    view.setRowHidden(2, QModelIndex(), true);
    QVERIFY(view.isExpanded(item1.index()));
    view.selectAll();
    QVERIFY(view.isExpanded(item1.index()));
    QCOMPARE(view.selectionModel()->selection().size(), 3);
    QCOMPARE(view.selectionModel()->selectedRows().size(), 4);
    QVERIFY( !view.selectionModel()->isSelected(model.indexFromItem(&item2)));
    view.setRowHidden(2, QModelIndex(), false);
    QVERIFY(view.isExpanded(item1.index()));
    view.clearSelection();
}

void tst_BOBUIreeView::selectAll()
{
    QStandardItemModel model(4, 4);
    BOBUIreeView view2;
    view2.setModel(&model);
    view2.setSelectionMode(QAbstractItemView::ExtendedSelection);
    view2.selectAll();  // Should work with an empty model
    //everything should be selected since we are in ExtendedSelection mode
    QCOMPARE(view2.selectedIndexes().size(), model.rowCount() * model.columnCount());

    for (int i = 0; i < model.rowCount(); ++i)
        model.setData(model.index(i,0), QLatin1String("row ") + QString::number(i));
    BOBUIreeView view;
    view.setModel(&model);
    int selectedCount = view.selectedIndexes().size();
    view.selectAll();
    QCOMPARE(view.selectedIndexes().size(), selectedCount);

    BOBUIreeView view3;
    view3.setModel(&model);
    view3.setSelectionMode(QAbstractItemView::NoSelection);
    view3.selectAll();
    QCOMPARE(view3.selectedIndexes().size(), 0);
}

void tst_BOBUIreeView::extendedSelection_data()
{
    BOBUIest::addColumn<QPoint>("mousePressPos");
    BOBUIest::addColumn<int>("selectedCount");

    BOBUIest::newRow("select") << QPoint(10, 10) << 2;
    BOBUIest::newRow("unselect") << QPoint(10, 300) << 0;
}

void tst_BOBUIreeView::extendedSelection()
{
    QFETCH(QPoint, mousePressPos);
    QFETCH(int, selectedCount);

    QStandardItemModel model(5, 2);
    QWidget topLevel;
    BOBUIreeView view(&topLevel);
    view.resize(qMax(mousePressPos.x() * 2, 300), qMax(mousePressPos.y() * 2, 350));
    view.setModel(&model);
    view.setSelectionMode(QAbstractItemView::ExtendedSelection);
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));
    BOBUIest::mousePress(view.viewport(), BobUI::LeftButton, {}, mousePressPos);
    QCOMPARE(view.selectionModel()->selectedIndexes().size(), selectedCount);
}

void tst_BOBUIreeView::rowSizeHint()
{
    //tests whether the correct visible columns are taken into account when
    //calculating the height of a line
    QStandardItemModel model(1, 3);
    model.setData(model.index(0, 0), QSize(20, 40), BobUI::SizeHintRole);
    model.setData(model.index(0, 1), QSize(20, 10), BobUI::SizeHintRole);
    model.setData(model.index(0, 2), QSize(20, 10), BobUI::SizeHintRole);
    BOBUIreeView view;
    view.setModel(&model);

    view.header()->moveSection(1, 0); //the 2nd column goes to the 1st place

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    //it must be 40 since the tallest item that defines the height of a line
    QCOMPARE(view.visualRect(model.index(0,0)).height(), 40);
    QCOMPARE(view.visualRect(model.index(0,1)).height(), 40);
    QCOMPARE(view.visualRect(model.index(0,2)).height(), 40);
}


//From task 155449 (BOBUIreeWidget has a large width for the first section when sorting
//is turned on before items are added)

void tst_BOBUIreeView::setSortingEnabledTopLevel()
{
    BOBUIreeView view;
    QStandardItemModel model(1, 1);
    view.setModel(&model);
    const int size = view.header()->sectionSize(0);
    view.setSortingEnabled(true);
    model.setColumnCount(3);
    //we test that changing the column count doesn't change the 1st column size
    QCOMPARE(view.header()->sectionSize(0), size);
}

void tst_BOBUIreeView::setSortingEnabledChild()
{
    QMainWindow win;
    BOBUIreeView view;
    // two columns to not get in trouble with stretchLastSection
    QStandardItemModel model(1, 2);
    view.setModel(&model);
    view.header()->setDefaultSectionSize(92);
    win.setCentralWidget(&view);
    const int size = view.header()->sectionSize(0);
    view.setSortingEnabled(true);
    model.setColumnCount(3);
    //we test that changing the column count doesn't change the 1st column size
    QCOMPARE(view.header()->sectionSize(0), size);
}

void tst_BOBUIreeView::headerHidden()
{
    BOBUIreeView view;
    QStandardItemModel model;
    view.setModel(&model);
    QCOMPARE(view.isHeaderHidden(), false);
    QCOMPARE(view.header()->isHidden(), false);
    view.setHeaderHidden(true);
    QCOMPARE(view.isHeaderHidden(), true);
    QCOMPARE(view.header()->isHidden(), true);
}

class TestTreeViewStyle : public QProxyStyle
{
    Q_OBJECT
public:
    using QProxyStyle::QProxyStyle;
    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override
    {
        if (metric == QStyle::PM_TreeViewIndentation)
            return indentation;
        else
            return QProxyStyle::pixelMetric(metric, option, widget);
    }
    int indentation = 20;
};

void tst_BOBUIreeView::indentation()
{
    TestTreeViewStyle style1;
    TestTreeViewStyle style2;
    style1.indentation = 20;
    style2.indentation = 30;

    BOBUIreeView view;
    view.setStyle(&style1);
    QCOMPARE(view.indentation(), style1.indentation);
    view.setStyle(&style2);
    QCOMPARE(view.indentation(), style2.indentation);
    view.setIndentation(70);
    QCOMPARE(view.indentation(), 70);
    view.setStyle(&style1);
    QCOMPARE(view.indentation(), 70);
    view.resetIndentation();
    QCOMPARE(view.indentation(), style1.indentation);
    view.setStyle(&style2);
    QCOMPARE(view.indentation(), style2.indentation);
}

// From Task 145199 (crash when column 0 having at least one expanded item is removed and then
// inserted). The test passes simply if it doesn't crash, hence there are no calls
// to QCOMPARE() or QVERIFY().
void tst_BOBUIreeView::removeAndInsertExpandedCol0()
{
    BOBUIreeView view;
    QStandardItemModel model;
    view.setModel(&model);

    model.setColumnCount(1);

    QStandardItem *item0 = new QStandardItem(QString("item 0"));
    model.invisibleRootItem()->appendRow(item0);
    view.expand(item0->index());
    QStandardItem *item1 = new QStandardItem(QString("item 1"));
    item0->appendRow(item1);

    model.removeColumns(0, 1);
    model.insertColumns(0, 1);

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
}

void tst_BOBUIreeView::disabledButCheckable()
{
    BOBUIreeView view;
    QStandardItemModel model;
    QStandardItem *item;
    item = new QStandardItem(QLatin1String("Row 1 Item"));
    model.insertRow(0, item);

    item = new QStandardItem(QLatin1String("Row 2 Item"));
    item->setCheckable(true);
    item->setEnabled(false);
    model.insertRow(1, item);

    view.setModel(&model);
    view.setCurrentIndex(model.index(1,0));
    QCOMPARE(item->checkState(), BobUI::Unchecked);
    view.show();

    BOBUIest::keyClick(&view, BobUI::Key_Space);
    QCOMPARE(item->checkState(), BobUI::Unchecked);
}

void tst_BOBUIreeView::sortByColumn_data()
{
    BOBUIest::addColumn<bool>("sortingEnabled");
    BOBUIest::newRow("sorting enabled") << true;
    BOBUIest::newRow("sorting disabled") << false;
}

// Checks sorting and that sortByColumn also sets the sortIndicator
void tst_BOBUIreeView::sortByColumn()
{
    QFETCH(bool, sortingEnabled);
    BOBUIreeView view;
    QStandardItemModel model(4, 2);
    QSortFilterProxyModel sfpm; // default QStandardItemModel does not support 'unsorted' state
    sfpm.setSourceModel(&model);
    model.setItem(0, 0, new QStandardItem("b"));
    model.setItem(1, 0, new QStandardItem("d"));
    model.setItem(2, 0, new QStandardItem("c"));
    model.setItem(3, 0, new QStandardItem("a"));
    model.setItem(0, 1, new QStandardItem("e"));
    model.setItem(1, 1, new QStandardItem("g"));
    model.setItem(2, 1, new QStandardItem("h"));
    model.setItem(3, 1, new QStandardItem("f"));

    view.setSortingEnabled(sortingEnabled);
    view.setModel(&sfpm);

    view.sortByColumn(1, BobUI::DescendingOrder);
    QCOMPARE(view.header()->sortIndicatorSection(), 1);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("c"));
    QCOMPARE(view.model()->data(view.model()->index(1, 0)).toString(), QString::fromLatin1("d"));
    QCOMPARE(view.model()->data(view.model()->index(0, 1)).toString(), QString::fromLatin1("h"));
    QCOMPARE(view.model()->data(view.model()->index(1, 1)).toString(), QString::fromLatin1("g"));

    view.sortByColumn(0, BobUI::AscendingOrder);
    QCOMPARE(view.header()->sortIndicatorSection(), 0);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("a"));
    QCOMPARE(view.model()->data(view.model()->index(1, 0)).toString(), QString::fromLatin1("b"));
    QCOMPARE(view.model()->data(view.model()->index(0, 1)).toString(), QString::fromLatin1("f"));
    QCOMPARE(view.model()->data(view.model()->index(1, 1)).toString(), QString::fromLatin1("e"));

    view.sortByColumn(-1, BobUI::AscendingOrder);
    QCOMPARE(view.header()->sortIndicatorSection(), -1);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("b"));
    QCOMPARE(view.model()->data(view.model()->index(1, 0)).toString(), QString::fromLatin1("d"));
    QCOMPARE(view.model()->data(view.model()->index(0, 1)).toString(), QString::fromLatin1("e"));
    QCOMPARE(view.model()->data(view.model()->index(1, 1)).toString(), QString::fromLatin1("g"));

    // a new 'sortByColumn()' should do a re-sort (e.g. due to the data changed), BOBUIBUG-86268
    view.setModel(&model);
    view.sortByColumn(0, BobUI::AscendingOrder);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("a"));
    model.setItem(0, 0, new QStandardItem("x"));
    view.sortByColumn(0, BobUI::AscendingOrder);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("b"));
}

/*
    This is a model that every time kill() is called it will completely change
    all of its nodes for new nodes.  It then qFatal's if you later use a dead node.
 */
class EvilModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    class Node
    {
    public:
        Node(Node *p = nullptr, int level = 0) : parent(p)
        {
            populate(level);
        }
        ~Node()
        {
            qDeleteAll(children.begin(), children.end());
            qDeleteAll(deadChildren.begin(), deadChildren.end());
        }

        void populate(int level = 0)
        {
            if (level < 4) {
                for (int i = 0; i < 5; ++i)
                    children.append(new Node(this, level + 1));
            }
        }
        void kill()
        {
            for (int i = children.size() -1; i >= 0; --i) {
                children.at(i)->kill();
                if (parent == nullptr) {
                    deadChildren.append(children.at(i));
                    children.removeAt(i);
                }
            }
            if (parent == nullptr) {
                if (!children.isEmpty())
                    qFatal("%s: children should be empty when parent is null", Q_FUNC_INFO);
                populate();
            } else {
                isDead = true;
            }
        }

        QList<Node *> children;
        QList<Node *> deadChildren;
        Node *parent;
        bool isDead = false;
    };

    Node *root;
    bool crash = false;

    EvilModel(QObject *parent = nullptr): QAbstractItemModel(parent), root(new Node)
    {}
    ~EvilModel()
    {
        delete root;
    }

    void setCrash()
    {
        crash = true;
    }

    void change()
    {
        emit layoutAboutToBeChanged();
        QModelIndexList oldList = persistentIndexList();
        QList<QStack<int>> oldListPath;
        for (int i = 0; i < oldList.size(); ++i) {
            QModelIndex idx = oldList.at(i);
            QStack<int> path;
            while (idx.isValid()) {
                path.push(idx.row());
                idx = idx.parent();
            }
            oldListPath.append(path);
        }
        root->kill();

        QModelIndexList newList;
        for (auto path : std::as_const(oldListPath)) {
            QModelIndex idx;
            while (!path.isEmpty())
                idx = index(path.pop(), 0, idx);
            newList.append(idx);
        }

        changePersistentIndexList(oldList, newList);
        emit layoutChanged();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Node *parentNode = root;
        if (parent.isValid()) {
            parentNode = static_cast<Node*>(parent.internalPointer());
            if (parentNode->isDead)
                qFatal("%s: parentNode is dead!", Q_FUNC_INFO);
        }
        return parentNode->children.size();
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return parent.column() > 0 ? 0 : 1;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        Node *grandparentNode = static_cast<Node*>(parent.internalPointer());
        Node *parentNode = root;
        if (parent.isValid()) {
            if (grandparentNode->isDead)
                qFatal("%s: grandparentNode is dead!", Q_FUNC_INFO);
            parentNode = grandparentNode->children[parent.row()];
            if (parentNode->isDead)
                qFatal("%s: grandparentNode is dead!", Q_FUNC_INFO);
        }
        return createIndex(row, column, parentNode);
    }

    QModelIndex parent(const QModelIndex &index) const override
    {
        Node *parent = static_cast<Node*>(index.internalPointer());
        Node *grandparent = parent->parent;
        if (!grandparent)
            return QModelIndex();
        return createIndex(grandparent->children.indexOf(parent), 0, grandparent);
    }

    QVariant data(const QModelIndex &idx, int role) const override
    {
        if (crash) {
            BOBUIest::qFail("Should not get here...", __FILE__, __LINE__);
            return QVariant();
        }
        if (idx.isValid() && role == BobUI::DisplayRole) {
            Node *parentNode = root;
            if (idx.isValid()) {
                parentNode = static_cast<Node*>(idx.internalPointer());
                if (parentNode->isDead)
                    qFatal("%s: grandparentNode is dead!", Q_FUNC_INFO);
            }
            return QLatin1Char('[') + QString::number(idx.row()) + QLatin1Char(',')
                + QString::number(idx.column()) + QLatin1Char(',')
                + QLatin1String(parentNode->isDead ? "dead" : "alive") + QLatin1Char(']');
        }
        return QVariant();
    }
};

void tst_BOBUIreeView::evilModel_data()
{
    BOBUIest::addColumn<bool>("visible");
    BOBUIest::newRow("visible") << false;
    BOBUIest::newRow("visible") << true;
}

void tst_BOBUIreeView::evilModel()
{
    QFETCH(bool, visible);
    // init
    TreeView view;
    EvilModel model;
    view.setModel(&model);
    view.setVisible(visible);
    QPersistentModelIndex firstLevel = model.index(0, 0);
    QPersistentModelIndex secondLevel = model.index(1, 0, firstLevel);
    QPersistentModelIndex thirdLevel = model.index(2, 0, secondLevel);
    view.setExpanded(firstLevel, true);
    view.setExpanded(secondLevel, true);
    view.setExpanded(thirdLevel, true);
    model.change();

    // tests
    view.setRowHidden(0, firstLevel, true);
    model.change();

    view.setFirstColumnSpanned(1, QModelIndex(), true);
    model.change();

    view.expand(secondLevel);
    model.change();

    view.collapse(secondLevel);
    model.change();

    view.isExpanded(secondLevel);
    view.setCurrentIndex(firstLevel);
    model.change();

    view.keyboardSearch("foo");
    model.change();

    view.visualRect(secondLevel);
    model.change();

    view.scrollTo(thirdLevel);
    model.change();

    view.update();  // will not do anything since view is not visible
    model.change();

    BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton);
    model.change();

    view.indexAt(QPoint(10, 10));
    model.change();

    view.indexAbove(model.index(2, 0));
    model.change();

    view.indexBelow(model.index(1, 0));
    model.change();

    QRect rect(0, 0, 10, 10);
    view.setSelection(rect, QItemSelectionModel::Select);
    model.change();

    view.moveCursor(BOBUIreeView::MoveDown, BobUI::NoModifier);
    model.change();

    view.resizeColumnToContents(1);
    model.change();

    view.QAbstractItemView::sizeHintForColumn(1);
    model.change();

    view.rowHeight(secondLevel);
    model.change();

    view.setRootIsDecorated(true);
    model.change();

    view.setItemsExpandable(false);
    model.change();

    view.columnViewportPosition(0);
    model.change();

    view.columnWidth(0);
    model.change();

    view.setColumnWidth(0, 30);
    model.change();

    view.columnAt(15);
    model.change();

    view.isColumnHidden(1);
    model.change();

    view.setColumnHidden(2, true);
    model.change();

    view.isHeaderHidden();
    model.change();

    view.setHeaderHidden(true);
    model.change();

    view.isRowHidden(2, secondLevel);
    model.change();

    view.setRowHidden(3, secondLevel, true);
    model.change();

    view.isFirstColumnSpanned(3, thirdLevel);
    model.change();

    view.setSortingEnabled(true);
    model.change();

    view.isSortingEnabled();
    model.change();

    view.setAnimated(true);
    model.change();

    view.isAnimated();
    model.change();

    view.setAllColumnsShowFocus(true);
    model.change();

    view.allColumnsShowFocus();
    model.change();

    view.doItemsLayout();
    model.change();

    view.reset();
    model.change();

    view.sortByColumn(1, BobUI::AscendingOrder);
    model.change();

    view.dataChanged(secondLevel, secondLevel);
    model.change();

    view.hideColumn(1);
    model.change();

    view.showColumn(1);
    model.change();

    view.resizeColumnToContents(1);
    model.change();

    view.sortByColumn(1, BobUI::DescendingOrder);
    model.change();

    view.selectAll();
    model.change();

    view.expandAll();
    model.change();

    view.collapseAll();
    model.change();

    view.expandToDepth(3);
    model.change();

    view.setRootIndex(secondLevel);

    model.setCrash();
    view.setModel(nullptr);
}

void tst_BOBUIreeView::indexRowSizeHint()
{
    QStandardItemModel model(10, 1);
    BOBUIreeView view;

    view.setModel(&model);

    QModelIndex index = model.index(5, 0);
    QPushButton *w = new QPushButton("Test");
    view.setIndexWidget(index, w);

    view.show();

    QCOMPARE(view.indexRowSizeHint(index), w->sizeHint().height());
}

void tst_BOBUIreeView::filterProxyModelCrash()
{
    QStandardItemModel model;
    QList<QStandardItem *> items;
    for (int i = 0; i < 100; i++)
        items << new QStandardItem(QLatin1String("item ") + QString::number(i));
    model.appendColumn(items);

    QSortFilterProxyModel proxy;
    proxy.setSourceModel(&model);

    TreeView view;
    view.setModel(&proxy);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    proxy.invalidate();
    view.verticalScrollBar()->setValue(15);
    BOBUIest::qWait(20);

    proxy.invalidate();
    view.update(); //used to crash
    BOBUIRY_VERIFY(view.wasPainted);
}

void tst_BOBUIreeView::renderToPixmap_data()
{
    BOBUIest::addColumn<int>("row");
    BOBUIest::newRow("row-0") << 0;
    BOBUIest::newRow("row-1") << 1;
}

void tst_BOBUIreeView::renderToPixmap()
{
    QFETCH(int, row);
    BOBUIreeView view;
    QStandardItemModel model;

    model.appendRow(new QStandardItem("Spanning"));
    model.appendRow({ new QStandardItem("Not"), new QStandardItem("Spanning") });

    view.setModel(&model);
    view.setFirstColumnSpanned(0, QModelIndex(), true);

#ifdef BOBUI_BUILD_INTERNAL
    {
        // We select the index at row=0 because it spans the
        // column (regression test for an assert)
        // We select the index at row=1 for coverage.
        QItemSelection sel(model.index(row,0), model.index(row,1));
        QRect rect;
        view.d_func()->renderToPixmap(sel.indexes(), &rect);
    }
#endif
}

void tst_BOBUIreeView::styleOptionViewItem()
{
    class MyDelegate : public QStyledItemDelegate
    {
        static QString posToString(QStyleOptionViewItem::ViewItemPosition pos)
        {
            static const char* s_pos[] = { "Invalid", "Beginning", "Middle", "End", "OnlyOne" };
            return s_pos[pos];
        }
    public:
        using QStyledItemDelegate::QStyledItemDelegate;
        bool checkModel(BOBUIreeView *view, QAbstractItemModel *model)
        {
            expectedIndices.clear();
            nonexpectedIndices.clear();
            findIndexesToCheck(view, model, QModelIndex());
            //qDebug() << "Found" << expectedIndices.size() << "indexes to check";
            if (expectedIndices.empty()) {
                qWarning() << "Testcase is wrong - need at least one visible item!";
                return false;
            }
            return checkModel(view);
        }

        bool checkModel(BOBUIreeView *view)
        {
            isTesting = true;
            foundError = false;
            const auto vp = view->viewport();
            for (const auto &idx : std::as_const(expectedIndices)) {
                if (!BOBUIest::qWaitFor([&]() { return !view->visualRect(idx).isEmpty(); }, 2000)) {
                    qWarning() << "Index" << idx << "has an invalid visible rect:" << view->visualRect(idx);
                    isTesting = false;
                    return false;
                }
                vp->update(view->visualRect(idx));
            }
            if (!BOBUIest::qWaitFor([&]() { return expectedIndices.empty(); }, 2000)) {
                for (const auto &idx : std::as_const(expectedIndices))
                    qDebug() << "Missing indexes:" << idx << idx.data(BobUI::DisplayRole).toString();
                isTesting = false;
                return false;
            }
            isTesting = false;
            return !foundError;
        }
        void findIndexesToCheck(BOBUIreeView *view,
                                QAbstractItemModel *model,
                                const QModelIndex &parent)
        {
            for (int r = 0; r < model->rowCount(parent); ++r) {
                for (int c = 0; c < model->columnCount(parent); ++c) {
                    const auto child = model->index(r, c, parent);
                    if (view->isRowHidden(r, parent) || view->isColumnHidden(c) ||
                        view->isIndexHidden(child) ||
                        (parent.isValid() && view->isIndexHidden(parent)) ||
                        (parent.isValid() && !view->isExpanded(parent)) ||
                        (view->isFirstColumnSpanned(r, parent) && c != 0)) {
                        nonexpectedIndices.append(child);
                    } else {
                        expectedIndices.append(child);
                    }
                    findIndexesToCheck(view, model, child);
                }
            }
        }
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
        {
            QStyleOptionViewItem opt(option);
            initStyleOption(&opt, index);
            QStyledItemDelegate::paint(painter, option, index);

            QVERIFY(!opt.text.isEmpty());
            QCOMPARE(opt.index, index);
            //qDebug() << index << opt.text;

            expectedIndices.removeAll(index);
            if (!isTesting)
                return;
            if (nonexpectedIndices.contains(index)) {
                qDebug() << "Unexpected index found:" << index << index.data(BobUI::DisplayRole).toString();
                foundError = true;
            }

            if (allCollapsed) {
                QCOMPARE(!opt.features.testFlag(QStyleOptionViewItem::Alternate),
                         !(index.row() % 2));
            }
            QCOMPARE(!opt.features.testFlag(QStyleOptionViewItem::HasCheckIndicator),
                     !opt.text.contains("Checkable"));

            const QString posStr(posToString(opt.viewItemPosition));
            if (opt.text.contains("Beginning"))
                QCOMPARE(posStr, posToString(QStyleOptionViewItem::Beginning));

            if (opt.text.contains("Middle"))
                QCOMPARE(posStr, posToString(QStyleOptionViewItem::Middle));

            if (opt.text.contains("End"))
                QCOMPARE(posStr, posToString(QStyleOptionViewItem::End));

            if (opt.text.contains("OnlyOne"))
                QCOMPARE(posStr, posToString(QStyleOptionViewItem::OnlyOne));

            if (opt.text.contains("Checked"))
                QCOMPARE(opt.checkState, BobUI::Checked);
            else
                QCOMPARE(opt.checkState, BobUI::Unchecked);

            QCOMPARE(!opt.state.testFlag(QStyle::State_Children),
                     !opt.text.contains("HasChildren"));
            QCOMPARE(opt.state.testFlag(QStyle::State_Sibling),
                     !opt.text.contains("Last"));

            QVERIFY(!opt.text.contains("Assert"));
        }
        bool allCollapsed = false;
        mutable QModelIndexList expectedIndices;
        QModelIndexList nonexpectedIndices;
        mutable bool foundError = false;
        bool isTesting = false;
    };

    BOBUIreeView view;
    QStandardItemModel model;
    view.setModel(&model);
    MyDelegate delegate;
    view.setItemDelegate(&delegate);
    model.appendRow({ new QStandardItem("Beginning"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Middle"),
                      new QStandardItem("Middle"),
                      new QStandardItem("End") });
    QStandardItem *par1 = new QStandardItem("Beginning HasChildren");
    model.appendRow({ par1,
                      new QStandardItem("Hidden"),
                      new QStandardItem("Middle HasChildren"),
                      new QStandardItem("Middle HasChildren"),
                      new QStandardItem("End HasChildren") });
    model.appendRow({ new QStandardItem("OnlyOne"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Assert"),
                      new QStandardItem("Assert"),
                      new QStandardItem("Assert") });
    QStandardItem *checkable = new QStandardItem("Checkable");
    checkable->setCheckable(true);
    QStandardItem *checked = new QStandardItem("Checkable Checked");
    checked->setCheckable(true);
    checked->setCheckState(BobUI::Checked);
    model.appendRow({ new QStandardItem("Beginning"),
                      new QStandardItem("Hidden"),
                      checkable, checked,
                      new QStandardItem("End") });
    model.appendRow({ new QStandardItem("Beginning Last"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Middle Last"),
                      new QStandardItem("Middle Last"),
                      new QStandardItem("End Last") });
    par1->appendRow({ new QStandardItem("Beginning"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Middle"),
                      new QStandardItem("Middle"),
                      new QStandardItem("End") });
    QStandardItem *par2 = new QStandardItem("Beginning HasChildren");
    par1->appendRow({ par2,
                      new QStandardItem("Hidden"),
                      new QStandardItem("Middle HasChildren"),
                      new QStandardItem("Middle HasChildren"),
                      new QStandardItem("End HasChildren") });
    par2->appendRow({ new QStandardItem("Beginning Last"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Middle Last"),
                      new QStandardItem("Middle Last"),
                      new QStandardItem("End Last") });
    QStandardItem *par3 = new QStandardItem("Beginning Last");
    par1->appendRow({ par3, new QStandardItem("Hidden"),
                      new QStandardItem("Middle Last"),
                      new QStandardItem("Middle Last"),
                      new QStandardItem("End Last") });
    par3->appendRow({ new QStandardItem("Assert"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Assert"),
                      new QStandardItem("Assert"),
                      new QStandardItem("Assert") });
    view.setRowHidden(0, par3->index(), true);
    par1->appendRow({ new QStandardItem("Assert"),
                      new QStandardItem("Hidden"),
                      new QStandardItem("Assert"),
                      new QStandardItem("Assert"),
                      new QStandardItem("Assert") });
    view.setRowHidden(3, par1->index(), true);

    view.setColumnHidden(1, true);
    view.header()->setMinimumSectionSize(10);
    // make sure that all columns are drawn in the view by using a very small section size
    for (int i = 0; i < view.header()->count(); ++i)
        view.header()->resizeSection(i, 40);
    view.setMinimumWidth(view.header()->count() * 40);

    const int modelColumns = 5;

    view.header()->swapSections(2, 3);
    view.setFirstColumnSpanned(2, QModelIndex(), true);
    view.setAlternatingRowColors(true);

#ifdef BOBUI_BUILD_INTERNAL
    {
        // Test the rendering to pixmap before painting the widget.
        // The rendering to pixmap should not depend on having been
        // painted already yet.
        QItemSelection sel(model.index(0,0), model.index(0,modelColumns-1));
        delegate.expectedIndices = sel.indexes();
        delegate.expectedIndices.removeAll(model.index(0, 1)); // hidden
        delegate.nonexpectedIndices.clear();
        delegate.nonexpectedIndices.append(model.index(0, 1));
        QRect rect;
        view.d_func()->renderToPixmap(sel.indexes(), &rect);
        QVERIFY(delegate.checkModel(&view));
    }
#endif

    delegate.allCollapsed = true;
    view.showMaximized();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    QVERIFY(delegate.checkModel(&view, &model));
    delegate.allCollapsed = false;
    view.expandAll();
    QVERIFY(delegate.checkModel(&view, &model));
    view.collapse(par2->index());
    QVERIFY(delegate.checkModel(&view, &model));

    // test that the rendering of drag pixmap sets the correct options too (BOBUIBUG-15834)
#ifdef BOBUI_BUILD_INTERNAL
    QItemSelection sel(model.index(0,0), model.index(0,modelColumns-1));
    delegate.expectedIndices = sel.indexes();
    delegate.expectedIndices.removeAll(model.index(0, 1)); // hidden
    delegate.nonexpectedIndices.clear();
    delegate.nonexpectedIndices.append(model.index(0, 1));
    QRect rect;
    view.d_func()->renderToPixmap(sel.indexes(), &rect);
    QVERIFY(delegate.checkModel(&view));
#endif

    //test dynamic models
    {
        QStandardItemModel model2;
        QStandardItem *item0 = new QStandardItem("OnlyOne Last");
        model2.appendRow(item0);
        view.setModel(&model2);
        QVERIFY(delegate.checkModel(&view, &model2));

        QStandardItem *item00 = new QStandardItem("OnlyOne Last");
        item0->appendRow(item00);
        item0->setText("OnlyOne Last HasChildren");
        view.expandAll();
        QVERIFY(delegate.checkModel(&view, &model2));

        QStandardItem *item1 = new QStandardItem("OnlyOne Last");
        item0->setText("OnlyOne HasChildren");
        model2.appendRow(item1);
        QVERIFY(delegate.checkModel(&view, &model2));

        QStandardItem *item01 = new QStandardItem("OnlyOne Last");
        item00->setText("OnlyOne");
        item0->appendRow(item01);
        QVERIFY(delegate.checkModel(&view, &model2));

        QStandardItem *item000 = new QStandardItem("OnlyOne Last");
        item00->setText("OnlyOne HasChildren");
        item00->appendRow(item000);
        QVERIFY(delegate.checkModel(&view, &model2));

        item0->removeRow(0);
        QVERIFY(delegate.checkModel(&view, &model2));

        item00 = new QStandardItem("OnlyOne");
        item0->insertRow(0, item00);

        view.expandAll();
        QVERIFY(delegate.checkModel(&view, &model2));

        item0->removeRow(1);
        item00->setText("OnlyOne Last");
        QVERIFY(delegate.checkModel(&view, &model2));

        item0->removeRow(0);
        item0->setText("OnlyOne");
        QVERIFY(delegate.checkModel(&view, &model2));

        //with hidden items
        item0->setText("OnlyOne HasChildren");
        item00 = new QStandardItem("OnlyOne");
        item0->appendRow(item00);
        item01 = new QStandardItem("Assert");
        item0->appendRow(item01);
        view.setRowHidden(1, item0->index(), true);
        view.expandAll();
        QStandardItem *item02 = new QStandardItem("OnlyOne Last");
        item0->appendRow(item02);
        QVERIFY(delegate.checkModel(&view, &model2));

        item0->removeRow(2);
        item00->setText("OnlyOne Last");
        QVERIFY(delegate.checkModel(&view, &model2));

        item00->setText("OnlyOne");
        item0->insertRow(2, new QStandardItem("OnlyOne Last"));
        view.collapse(item0->index());
        item0->removeRow(0);
        QVERIFY(delegate.checkModel(&view, &model2));

        item0->removeRow(1);
        item0->setText("OnlyOne");
        QVERIFY(delegate.checkModel(&view, &model2));
    }

    // special case, two column, first is hidden
    model.clear();
    model.appendRow({ new QStandardItem("Hidden"),
                      new QStandardItem("OnlyOne Last") });
    view.setModel(&model);
    view.setColumnHidden(0, true);
    view.setColumnHidden(1, false);
    QVERIFY(delegate.checkModel(&view, &model));

    // special case, four columns, only one is updated but
    // calcLogicalIndices() returns the correct value
    {
        model.clear();
        model.appendRow({ new QStandardItem("Hidden"),
                          new QStandardItem("Beginning Last"),
                          new QStandardItem("Middle Last"),
                          new QStandardItem("End Last") });
        view.setColumnHidden(0, true);
        QVERIFY(delegate.checkModel(&view, &model));
        // do not rely on paintEvent() as this might redraw a bigger
        // rect than we expect
        QList<int> logicalIndices;
        QList<QStyleOptionViewItem::ViewItemPosition> itemPositions;
        view.d_func()->calcLogicalIndices(&logicalIndices, &itemPositions, 3, 3);
        QCOMPARE(itemPositions.size(), 1);
        QCOMPARE(itemPositions.at(0), QStyleOptionViewItem::End);
    }
}

class task174627_TreeView : public BOBUIreeView
{
    Q_OBJECT
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &) override
    { emit signalCurrentChanged(current); }
signals:
    void signalCurrentChanged(const QModelIndex &);
};

void tst_BOBUIreeView::task174627_moveLeftToRoot()
{
    QStandardItemModel model;
    QStandardItem *item1 = new QStandardItem(QString("item 1"));
    model.invisibleRootItem()->appendRow(item1);
    QStandardItem *item2 = new QStandardItem(QString("item 2"));
    item1->appendRow(item2);

    task174627_TreeView view;
    view.setModel(&model);
    view.setRootIndex(item1->index());
    view.setCurrentIndex(item2->index());

    QSignalSpy spy(&view, &task174627_TreeView::signalCurrentChanged);
    BOBUIest::keyClick(&view, BobUI::Key_Left);
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIreeView::task171902_expandWith1stColHidden()
{
    //the task was: if the first column of a treeview is hidden, the expanded state is not correctly restored
    QStandardItemModel model;
    QStandardItem root("root"), root2("root"),
        subitem("subitem"), subitem2("subitem"),
        subsubitem("subsubitem"), subsubitem2("subsubitem");

    model.appendRow({ &root, &root2 });
    root.appendRow({ &subitem, &subitem2 });
    subitem.appendRow({ &subsubitem, &subsubitem2 });

    BOBUIreeView view;
    view.setModel(&model);

    view.setColumnHidden(0, true);
    view.expandAll();
    view.collapse(root.index());
    view.expand(root.index());

    QCOMPARE(view.isExpanded(root.index()), true);
    QCOMPARE(view.isExpanded(subitem.index()), true);

}

void tst_BOBUIreeView::task203696_hidingColumnsAndRowsn()
{
    BOBUIreeView view;
    QStandardItemModel model(0, 3);
    for (int i = 0; i < 3; ++i) {
        const QString prefix = QLatin1String("row ") + QString::number(i) + QLatin1String(" col ");
        model.insertRow(model.rowCount());
        for (int j = 0; j < model.columnCount(); ++j)
            model.setData(model.index(i, j), prefix + QString::number(j));
    }
    view.setModel(&model);
    view.show();
    view.setColumnHidden(0, true);
    view.setRowHidden(0, QModelIndex(), true);
    QCOMPARE(view.indexAt(QPoint(0, 0)), model.index(1, 1));
}


void tst_BOBUIreeView::addRowsWhileSectionsAreHidden()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIreeView view;
    for (int pass = 1; pass <= 2; ++pass) {
        QStandardItemModel *model = new QStandardItemModel(6, pass, &view);
        view.setModel(model);
        view.show();
        QVERIFY(BOBUIest::qWaitForWindowActive(&view));

        for (int i = 0; i < 3; ++i)
        {
            model->insertRow(model->rowCount());
            const QString prefix = QLatin1String("row ") + QString::number(i) + QLatin1String(" col ");
            for (int j = 0; j < model->columnCount(); ++j)
                model->setData(model->index(i, j), prefix + QString::number(j));
        }
        for (int col = 0; col < pass; ++col)
            view.setColumnHidden(col, true);
        for (int i = 3; i < 6; ++i)
        {
            model->insertRow(model->rowCount());
            const QString prefix = QLatin1String("row ") + QString::number(i) + QLatin1String(" col ");
            for (int j = 0; j < model->columnCount(); ++j)
                model->setData(model->index(i, j), prefix + QString::number(j));
        }
        for (int col = 0; col < pass; ++col)
            view.setColumnHidden(col, false);

        auto allVisualRectsValid = [](BOBUIreeView *view, QStandardItemModel *model) {
            for (int i = 0; i < 6; ++i) {
                if (!view->visualRect(model->index(i, 0)).isValid())
                    return false;
            }
            return true;
        };
        BOBUIRY_VERIFY(allVisualRectsValid(&view, model));

        delete model;
    }
}

void tst_BOBUIreeView::task216717_updateChildren()
{
    class Tree : public BOBUIreeWidget
    {
        protected:
            void paintEvent(QPaintEvent *e) override
            {
                BOBUIreeWidget::paintEvent(e);
                refreshed = true;
            }
        public:
            bool refreshed = false;
    } tree;
    tree.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tree));
    tree.refreshed = false;
    BOBUIreeWidgetItem *parent = new BOBUIreeWidgetItem({ "parent" });
    tree.addTopLevelItem(parent);
    BOBUIRY_VERIFY(tree.refreshed);
    tree.refreshed = false;
    parent->addChild(new BOBUIreeWidgetItem({ "child" }));
    BOBUIRY_VERIFY(tree.refreshed);

}

void tst_BOBUIreeView::task220298_selectColumns()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    //this is a very simple 3x3 model where the internalId of the index are different for each cell
    class Model : public QAbstractTableModel
    {
        public:
            int columnCount(const QModelIndex & parent = QModelIndex()) const override
            { return parent.isValid() ? 0 : 3; }
            int rowCount(const QModelIndex & parent = QModelIndex()) const override
            { return parent.isValid() ? 0 : 3; }

            QVariant data(const QModelIndex & index, int role = BobUI::DisplayRole) const override
            {
                if (role == BobUI::DisplayRole) {
                    return QVariant(QString::number(index.column()) + QLatin1Char('-')
                        + QString::number(index.row()));
                }
                return QVariant();
            }

            QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override
            {
                return hasIndex(row, column, parent) ? createIndex(row, column, quintptr(column * 10 + row)) : QModelIndex();
            }
    };

    TreeView view;
    Model model;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {},
                      view.visualRect(view.model()->index(1, 1)).center());
    BOBUIRY_VERIFY(view.selectedIndexes().contains(view.model()->index(1, 2)));
    QVERIFY(view.selectedIndexes().contains(view.model()->index(1, 1)));
    QVERIFY(view.selectedIndexes().contains(view.model()->index(1, 0)));
}


void tst_BOBUIreeView::task224091_appendColumns()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStandardItemModel *model = new QStandardItemModel();
    QWidget* topLevel= new QWidget;
    setFrameless(topLevel);
    BOBUIreeView *treeView = new BOBUIreeView(topLevel);
    treeView->setModel(model);
    topLevel->show();
    treeView->resize(50, 50);
    QVERIFY(BOBUIest::qWaitForWindowActive(topLevel));

    QVERIFY(!treeView->verticalScrollBar()->isVisible());

    QList<QStandardItem *> projlist;
    for (int k = 0; k < 10; ++k)
        projlist.append(new QStandardItem(QLatin1String("Top Level ") + QString::number(k)));
    model->appendColumn(projlist);
    model->invisibleRootItem()->appendRow(new QStandardItem("end"));

    BOBUIRY_VERIFY(treeView->verticalScrollBar()->isVisible());

    delete topLevel;
    delete model;
}

void tst_BOBUIreeView::task211293_removeRootIndex()
{
    BOBUIreeView view;
    QStandardItemModel model;
    QStandardItem *A1 = new QStandardItem("A1");
    QStandardItem *B11 = new QStandardItem("B1.1");
    QStandardItem *C111 = new QStandardItem("C1.1.1");
    QStandardItem *C112 = new QStandardItem("C1.1.2");
    QStandardItem *C113 = new QStandardItem("C1.1.3");
    QStandardItem *D1131 = new QStandardItem("D1.1.3.1");
    QStandardItem *E11311 = new QStandardItem("E1.1.3.1.1");
    QStandardItem *E11312 = new QStandardItem("E1.1.3.1.2");
    QStandardItem *E11313 = new QStandardItem("E1.1.3.1.3");
    QStandardItem *E11314 = new QStandardItem("E1.1.3.1.4");
    QStandardItem *D1132 = new QStandardItem("D1.1.3.2");
    QStandardItem *E11321 = new QStandardItem("E1.1.3.2.1");
    D1132->appendRow(E11321);
    D1131->appendRow(E11311);
    D1131->appendRow(E11312);
    D1131->appendRow(E11313);
    D1131->appendRow(E11314);
    C113->appendRow(D1131);
    C113->appendRow(D1132);
    B11->appendRow(C111);
    B11->appendRow(C112);
    B11->appendRow(C113);
    A1->appendRow(B11);
    model.appendRow(A1);
    view.setModel(&model);
    view.setRootIndex(model.indexFromItem(B11));
    view.setExpanded(model.indexFromItem(B11), true);
    view.setCurrentIndex(model.indexFromItem(E11314));
    view.setExpanded(model.indexFromItem(E11314), true);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    QVERIFY(model.removeRows(0, 1));
}

void tst_BOBUIreeView::task225539_deleteModel()
{
    BOBUIreeView treeView;
    treeView.show();
    QStandardItemModel *model = new QStandardItemModel(&treeView);

    QStandardItem *parentItem = model->invisibleRootItem();
    QStandardItem *item = new QStandardItem(QString("item"));
    parentItem->appendRow(item);

    treeView.setModel(model);

    QCOMPARE(item->index(), treeView.indexAt(QPoint()));

    delete model;

    QVERIFY(!treeView.indexAt(QPoint()).isValid());
}

void tst_BOBUIreeView::task230123_setItemsExpandable()
{
    //let's check that we prevent the expansion inside a treeview
    //when the property is set.
    BOBUIreeWidget tree;

    BOBUIreeWidgetItem root;
    BOBUIreeWidgetItem child;
    root.addChild(&child);
    tree.addTopLevelItem(&root);

    tree.setCurrentItem(&root);

    tree.setItemsExpandable(false);

    BOBUIest::keyClick(&tree, BobUI::Key_Plus);
    QVERIFY(!root.isExpanded());

    BOBUIest::keyClick(&tree, BobUI::Key_Right);
    QVERIFY(!root.isExpanded());

    tree.setItemsExpandable(true);

    BOBUIest::keyClick(&tree, BobUI::Key_Plus);
    QVERIFY(root.isExpanded());

    BOBUIest::keyClick(&tree, BobUI::Key_Minus);
    QVERIFY(!root.isExpanded());

    BOBUIest::keyClick(&tree, BobUI::Key_Right);
    QVERIFY(root.isExpanded());

    BOBUIest::keyClick(&tree, BobUI::Key_Left);
    QVERIFY(!root.isExpanded());

    BOBUIest::keyClick(&tree, BobUI::Key_Right);
    QVERIFY(root.isExpanded());

    const bool navToChild = tree.style()->styleHint(QStyle::SH_ItemView_ArrowKeysNavigateIntoChildren, nullptr, &tree);
    BOBUIest::keyClick(&tree, BobUI::Key_Right);
    QCOMPARE(tree.currentItem(), navToChild ? &child : &root);

    BOBUIest::keyClick(&tree, BobUI::Key_Right);
    //it should not be expanded: it has no leaf
    QCOMPARE(child.isExpanded(), false);

    BOBUIest::keyClick(&tree, BobUI::Key_Left);
    QCOMPARE(tree.currentItem(), &root);

    BOBUIest::keyClick(&tree, BobUI::Key_Left);
    QVERIFY(!root.isExpanded());
}

void tst_BOBUIreeView::task202039_closePersistentEditor()
{
    QStandardItemModel model(1, 1);
    BOBUIreeView view;
    view.setModel(&model);

    QModelIndex current = model.index(0,0);
    BOBUIest::mousePress(view.viewport(), BobUI::LeftButton, {}, view.visualRect(current).center());
    BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton, {}, view.visualRect(current).center());
    QCOMPARE(view.currentIndex(), current);
    QVERIFY(view.indexWidget(current));

    view.closePersistentEditor(current);
    QVERIFY(!view.indexWidget(current));

    //here was the bug: closing the persistent editor would not reset the state
    //and it was impossible to go into editinon again
    BOBUIest::mousePress(view.viewport(), BobUI::LeftButton, {}, view.visualRect(current).center());
    BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton, {}, view.visualRect(current).center());
    QCOMPARE(view.currentIndex(), current);
    QVERIFY(view.indexWidget(current));
}

void tst_BOBUIreeView::task238873_avoidAutoReopening()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStandardItemModel model;

    QStandardItem item0("row 0");
    QStandardItem item1("row 1");
    QStandardItem item2("row 2");
    QStandardItem item3("row 3");
    model.appendColumn( QList<QStandardItem*>() << &item0 << &item1 << &item2 << &item3);

    QStandardItem child("child");
    item1.appendRow( &child);

    BOBUIreeView view;
    view.setModel(&model);
    view.show();
    view.expandAll();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {}, view.visualRect(child.index()).center());
    BOBUIRY_COMPARE(view.currentIndex(), child.index());

    view.setExpanded(item1.index(), false);

    BOBUIRY_VERIFY(!view.isExpanded(item1.index()));
}

void tst_BOBUIreeView::task244304_clickOnDecoration()
{
    BOBUIreeView view;
    QStandardItemModel model;
    QStandardItem item0("row 0");
    QStandardItem item00("row 0");
    item0.appendRow(&item00);
    QStandardItem item1("row 1");
    model.appendColumn({ &item0, &item1 });
    view.setModel(&model);

    QVERIFY(!view.currentIndex().isValid());
    QRect rect = view.visualRect(item0.index());
    //we click on the decoration
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {},
                      rect.topLeft() + QPoint(-rect.left() / 2, rect.height() / 2));
    QVERIFY(!view.currentIndex().isValid());
    QVERIFY(view.isExpanded(item0.index()));

    rect = view.visualRect(item1.index());
    //the item has no decoration, it should get selected
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {},
                      rect.topLeft() + QPoint(-rect.left() / 2, rect.height() / 2));
    QCOMPARE(view.currentIndex(), item1.index());
}

void tst_BOBUIreeView::task246536_scrollbarsNotWorking()
{
    class MyObject : public QObject
    {
    public:
        using QObject::QObject;
        bool eventFilter(QObject*, QEvent *e) override
        {
            if (e->type() == QEvent::Paint)
                count++;

            return false;
        }
        int count = 0;
    };
    BOBUIreeView tree;
    MyObject o;
    tree.viewport()->installEventFilter(&o);
    QStandardItemModel model;
    tree.setModel(&model);
    tree.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tree));
    QList<QStandardItem *> items;
    for (int i = 0; i < 100; ++i)
        items << new QStandardItem(QLatin1String("item ") + QString::number(i));
    o.count = 0;
    model.invisibleRootItem()->appendColumn(items);
    BOBUIRY_COMPARE_GT(o.count, 0);
    o.count = 0;
    tree.verticalScrollBar()->setValue(50);
    BOBUIRY_COMPARE_GT(o.count, 0);
}

void tst_BOBUIreeView::task250683_wrongSectionSize()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStandardItemModel model;
    populateFakeDirModel(&model);

    BOBUIreeView treeView;
    treeView.header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView.setModel(&model);
    treeView.setColumnHidden(2, true);
    treeView.setColumnHidden(3, true);

    treeView.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&treeView));

    QCOMPARE(treeView.header()->sectionSize(0) + treeView.header()->sectionSize(1), treeView.viewport()->width());
}

void tst_BOBUIreeView::task239271_addRowsWithFirstColumnHidden()
{
    class MyDelegate : public QStyledItemDelegate
    {
    public:
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
        {
            paintedIndexes << index;
            QStyledItemDelegate::paint(painter, option, index);
        }
        mutable QSet<QModelIndex> paintedIndexes;
    };

    BOBUIreeView view;
    QStandardItemModel model;
    view.setModel(&model);
    MyDelegate delegate;
    view.setItemDelegate(&delegate);
    QStandardItem root0("root0"), root1("root1");
    model.invisibleRootItem()->appendRow(QList<QStandardItem*>() << &root0 << &root1);
    QStandardItem sub0("sub0"), sub00("sub00");
    root0.appendRow(QList<QStandardItem*>() << &sub0 << &sub00);
    view.expand(root0.index());

    view.hideColumn(0);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    delegate.paintedIndexes.clear();
    QStandardItem sub1("sub1"), sub11("sub11");
    root0.appendRow(QList<QStandardItem*>() << &sub1 << &sub11);

    //items in the 2nd column should have been painted
    BOBUIRY_VERIFY(!delegate.paintedIndexes.isEmpty());
    QVERIFY(delegate.paintedIndexes.contains(sub00.index()));
    QVERIFY(delegate.paintedIndexes.contains(sub11.index()));
}

void tst_BOBUIreeView::task254234_proxySort()
{
    //based on tst_BOBUIreeView::sortByColumn
    // it used not to work when setting the source of a proxy after enabling sorting
    BOBUIreeView view;
    QStandardItemModel model(4, 2);
    model.setItem(0, 0, new QStandardItem("b"));
    model.setItem(1, 0, new QStandardItem("d"));
    model.setItem(2, 0, new QStandardItem("c"));
    model.setItem(3, 0, new QStandardItem("a"));
    model.setItem(0, 1, new QStandardItem("e"));
    model.setItem(1, 1, new QStandardItem("g"));
    model.setItem(2, 1, new QStandardItem("h"));
    model.setItem(3, 1, new QStandardItem("f"));

    view.sortByColumn(1, BobUI::DescendingOrder);
    view.setSortingEnabled(true);

    QSortFilterProxyModel proxy;
    proxy.setDynamicSortFilter(true);
    view.setModel(&proxy);
    proxy.setSourceModel(&model);
    QCOMPARE(view.header()->sortIndicatorSection(), 1);
    QCOMPARE(view.model()->data(view.model()->index(0, 1)).toString(), QString::fromLatin1("h"));
    QCOMPARE(view.model()->data(view.model()->index(1, 1)).toString(), QString::fromLatin1("g"));
}

void tst_BOBUIreeView::task248022_changeSelection()
{
    //we check that changing the selection between the mouse press and the mouse release
    //works correctly
    TreeView view;
    const QStringList list({"1", "2"});
    QStringListModel model(list);
    view.setSelectionMode(QAbstractItemView::ExtendedSelection);
    view.setModel(&model);
    connect(view.selectionModel(), &QItemSelectionModel::selectionChanged,
            &view, &TreeView::handleSelectionChanged);
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {},
                      view.visualRect(model.index(1)).center());
    QCOMPARE(view.selectionModel()->selectedIndexes().size(), list.size());
}

void tst_BOBUIreeView::task245654_changeModelAndExpandAll()
{
    BOBUIreeView view;
    QScopedPointer<QStandardItemModel> model(new QStandardItemModel);
    QStandardItem *top = new QStandardItem("top");
    QStandardItem *sub = new QStandardItem("sub");
    top->appendRow(sub);
    model->appendRow(top);
    view.setModel(model.data());
    view.expandAll();
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    BOBUIRY_VERIFY(view.isExpanded(top->index()));

    //now let's try to delete the model
    //then repopulate and expand again
    model.reset(new QStandardItemModel);
    top = new QStandardItem("top");
    sub = new QStandardItem("sub");
    top->appendRow(sub);
    model->appendRow(top);
    view.setModel(model.data());
    view.expandAll();
    BOBUIRY_VERIFY(view.isExpanded(top->index()));
}

void tst_BOBUIreeView::doubleClickedWithSpans()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIreeView view;
    QStandardItemModel model(1, 2);
    view.setModel(&model);
    view.setFirstColumnSpanned(0, QModelIndex(), true);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));
    QVERIFY(view.isActiveWindow());

    QPoint p(10, 10);
    QCOMPARE(view.indexAt(p), model.index(0, 0));
    QSignalSpy spy(&view, &QAbstractItemView::doubleClicked);
    BOBUIest::mousePress(view.viewport(), BobUI::LeftButton, {}, p);
    BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton, {}, p);
    BOBUIest::mouseRelease(view.viewport(), BobUI::LeftButton, {}, p);
    QCOMPARE(spy.size(), 1);

    //let's click on the 2nd column
    p.setX(p.x() + view.header()->sectionSize(0));
    QCOMPARE(view.indexAt(p), model.index(0, 0));

    //end the previous edition
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {}, p);
    BOBUIest::mousePress(view.viewport(), BobUI::LeftButton, {}, p);
    BOBUIest::mouseDClick(view.viewport(), BobUI::LeftButton, {}, p);
    BOBUIest::mouseRelease(view.viewport(), BobUI::LeftButton, {}, p);
    BOBUIRY_COMPARE(spy.size(), 2);
}

void tst_BOBUIreeView::taskBOBUIBUG_6450_selectAllWith1stColumnHidden()
{
    BOBUIreeWidget tree;
    tree.setSelectionMode(QAbstractItemView::MultiSelection);
    tree.setColumnCount(2);
    QList<BOBUIreeWidgetItem *> items;
    const int nrRows = 10;
    for (int i = 0; i < nrRows; ++i) {
        const QString text = QLatin1String("item: ") + QString::number(i);
        items.append(new BOBUIreeWidgetItem(static_cast<BOBUIreeWidget *>(nullptr),
                                         QStringList(text)));
        items.last()->setText(1, QString("is an item"));
    }
    tree.insertTopLevelItems(0, items);

    tree.hideColumn(0);
    tree.selectAll();

    QVERIFY(tree.selectionModel()->hasSelection());
    for (int i = 0; i < nrRows; ++i)
        QVERIFY(tree.selectionModel()->isRowSelected(i, QModelIndex()));
}

class TreeViewBOBUIBUG_9216 : public BOBUIreeView
{
    Q_OBJECT
public:
    void paintEvent(QPaintEvent *event) override
    {
        if (doCompare)
            QCOMPARE(event->rect(), viewport()->rect());
        BOBUIreeView::paintEvent(event);
        painted++;
    }
    int painted = 0;
    bool doCompare = false;
};

void tst_BOBUIreeView::taskBOBUIBUG_9216_setSizeAndUniformRowHeightsWrongRepaint()
{
    QStandardItemModel model(10, 10, this);
    for (int row = 0; row < 10; row++) {
        const QString prefix = QLatin1String("row ") + QString::number(row) + QLatin1String(", col ");
        for (int col = 0; col < 10; col++)
            model.setItem(row, col, new QStandardItem(prefix + QString::number(col)));
    }
    TreeViewBOBUIBUG_9216 view;
    view.setUniformRowHeights(true);
    view.setModel(&model);
    view.painted = 0;
    view.doCompare = false;
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    BOBUIRY_COMPARE_GT(view.painted, 0);

    BOBUIest::qWait(100);  // This one is needed to make the test fail before the patch.
    view.painted = 0;
    view.doCompare = true;
    model.setData(model.index(0, 0), QVariant(QSize(50, 50)), BobUI::SizeHintRole);
    BOBUIRY_COMPARE_GT(view.painted, 0);
}

void tst_BOBUIreeView::keyboardNavigationWithDisabled()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget topLevel;
    BOBUIreeView view(&topLevel);
    QStandardItemModel model(90, 0);
    for (int i = 0; i < 90; i ++) {
        model.setItem(i, new QStandardItem(QString::number(i)));
        model.item(i)->setEnabled(i % 6 == 0);
    }
    view.setModel(&model);

    view.resize(200, view.visualRect(model.index(0,0)).height()*10);
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&topLevel));
    QVERIFY(topLevel.isActiveWindow());

    view.setCurrentIndex(model.index(1, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Up);
    QCOMPARE(view.currentIndex(), model.index(0, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Down);
    QCOMPARE(view.currentIndex(), model.index(6, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_PageDown);
    QCOMPARE(view.currentIndex(), model.index(18, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Down);
    QCOMPARE(view.currentIndex(), model.index(24, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_PageUp);
    QCOMPARE(view.currentIndex(), model.index(12, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Up);
    QCOMPARE(view.currentIndex(), model.index(6, 0));
    // BOBUIBUG-44746 - when first/last item is disabled,
    // Key_PageUp/Down/Home/End will not work as expected.
    model.item(0)->setEnabled(false);
    model.item(1)->setEnabled(true);
    model.item(2)->setEnabled(true);
    model.item(model.rowCount() - 1)->setEnabled(false);
    model.item(model.rowCount() - 2)->setEnabled(true);
    model.item(model.rowCount() - 3)->setEnabled(true);
    // PageUp
    view.setCurrentIndex(model.index(2, 0));
    QCOMPARE(view.currentIndex(), model.index(2, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_PageUp);
    QCOMPARE(view.currentIndex(), model.index(1, 0));
    // PageDown
    view.setCurrentIndex(model.index(model.rowCount() - 3, 0));
    QCOMPARE(view.currentIndex(), model.index(model.rowCount() - 3, 0));
    BOBUIest::keyClick(view.viewport(), BobUI::Key_PageDown);
    QCOMPARE(view.currentIndex(), model.index(model.rowCount() - 2, 0));
    // Key_Home
    BOBUIest::keyClick(view.viewport(), BobUI::Key_Home);
    QCOMPARE(view.currentIndex(), model.index(1, 0));
    // Key_End
    BOBUIest::keyClick(view.viewport(), BobUI::Key_End);
    QCOMPARE(view.currentIndex(), model.index(model.rowCount() - 2, 0));
}

class RemoveColumnOne : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    bool filterAcceptsColumn(int source_column, const QModelIndex &) const override
    {
        if (m_removeColumn)
            return source_column != 1;
        return true;
    }
    void removeColumn()
    {
        m_removeColumn = true;
        invalidate();
    }
private:
    bool m_removeColumn = false;
};


void tst_BOBUIreeView::saveRestoreState()
{
    QStandardItemModel model;
    for (int i = 0; i < 100; i++) {
        model.appendRow({new QStandardItem(QStringLiteral("item ") + QString::number(i)),
                         new QStandardItem(QStringLiteral("hidden by proxy")),
                         new QStandardItem(QStringLiteral("hidden by user")) });
    }
    QCOMPARE(model.columnCount(), 3);

    RemoveColumnOne proxy;
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.columnCount(), 3);

    BOBUIreeView view;
    view.setModel(&proxy);
    view.resize(500, 500);
    view.show();
    view.header()->hideSection(2);
    QVERIFY(view.header()->isSectionHidden(2));
    proxy.removeColumn();
    QCOMPARE(proxy.columnCount(), 2);
    QVERIFY(view.header()->isSectionHidden(1));
    const QByteArray data = view.header()->saveState();

    BOBUIreeView view2;
    view2.setModel(&proxy);
    view2.resize(500, 500);
    view2.show();
    view2.header()->restoreState(data);
    QVERIFY(view2.header()->isSectionHidden(1));
}

class Model_11466 : public QAbstractItemModel
{
    Q_OBJECT
public:
    Model_11466(QObject *parent = nullptr) : QAbstractItemModel(parent)
        , m_selectionModel(new QItemSelectionModel(this, this))
    {
        connect(m_selectionModel, &QItemSelectionModel::currentChanged,
                this, &Model_11466::slotCurrentChanged);
    }

    int rowCount(const QModelIndex &parent) const override
    {
        if (parent.isValid())
            return (parent.internalId() == 0) ? 4 : 0;
        return 2; // two top level items
    }

    int columnCount(const QModelIndex & /* parent */) const override
    {
        return 2;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (role == BobUI::DisplayRole && index.isValid()) {
            qint64 parentRowPlusOne = qint64(index.internalId());
            QString str;
            BOBUIextStream stream(&str);
            if (parentRowPlusOne > 0)
                stream << parentRowPlusOne << " -> " << index.row() << " : " << index.column();
            else
                stream << index.row() << " : " << index.column();
            return QVariant(str);
        }
        return QVariant();
    }

    QModelIndex parent(const QModelIndex &index) const override
    {
        if (index.isValid()) {
            qint64 parentRowPlusOne = qint64(index.internalId());
            if (parentRowPlusOne > 0) {
                int row = static_cast<int>(parentRowPlusOne - 1);
                return createIndex(row, 0);
            }
        }
        return QModelIndex();
    }

    void bindView(BOBUIreeView *view)
    {
        // sets the view to this model with a shared selection model
        QItemSelectionModel *oldModel = view->selectionModel();
        if (oldModel != m_selectionModel)
            delete oldModel;
        view->setModel(this); // this creates a new selection model for the view, but we don't want it either ...
        oldModel = view->selectionModel();
        view->setSelectionModel(m_selectionModel);
        delete oldModel;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const override
    {
        return createIndex(row, column, parent.isValid() ? quintptr(parent.row() + 1) : quintptr(0));
    }

public slots:
    void slotCurrentChanged(const QModelIndex &current,const QModelIndex &)
    {
        if (m_block)
            return;

        if (current.isValid()) {
            int selectedRow = current.row();
            const quintptr parentRowPlusOne = current.internalId();

            for (int i = 0; i < 2; ++i) {
                // announce the removal of all non top level items
                beginRemoveRows(createIndex(i, 0), 0, 3);
                // nothing to actually do for the removal
                endRemoveRows();

                // put them back in again
                beginInsertRows(createIndex(i, 0), 0, 3);
                // nothing to actually do for the insertion
                endInsertRows();
            }
            // reselect the current item ...
            QModelIndex selectedIndex = createIndex(selectedRow, 0, parentRowPlusOne);

            m_block = true; // recursion block
            m_selectionModel->select(selectedIndex, QItemSelectionModel::ClearAndSelect|QItemSelectionModel::Current|QItemSelectionModel::Rows);
            m_selectionModel->setCurrentIndex(selectedIndex, QItemSelectionModel::NoUpdate);
            m_block = false;
        } else {
            m_selectionModel->clear();
        }
    }

private:
    bool m_block = false;
    QItemSelectionModel *m_selectionModel;
};

void tst_BOBUIreeView::taskBOBUIBUG_11466_keyboardNavigationRegression()
{
    BOBUIreeView treeView;
    treeView.setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView.setSelectionMode(QAbstractItemView::SingleSelection);
    Model_11466 model(&treeView);
    model.bindView(&treeView);
    treeView.expandAll();
    treeView.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&treeView));

    BOBUIest::keyPress(treeView.viewport(), BobUI::Key_Down);
    BOBUIRY_COMPARE(treeView.currentIndex(), treeView.selectionModel()->selection().indexes().first());
}

void tst_BOBUIreeView::taskBOBUIBUG_13567_removeLastItemRegression()
{
    BobUITestModel model(200, 1);

    BOBUIreeView view;
    view.setSelectionMode(QAbstractItemView::ExtendedSelection);
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    view.scrollToBottom();
    BOBUIest::qWait(10);
    CHECK_VISIBLE(199, 0);

    view.setCurrentIndex(model.index(199, 0));
    model.removeLastRow();
    BOBUIRY_COMPARE(view.currentIndex(), model.index(198, 0));
    CHECK_VISIBLE(198, 0);
}

// From BOBUIBUG-25333 (BOBUIreeWidget drag crashes when there was a hidden item in tree)
// The test passes simply if it doesn't crash, hence there are no calls
// to QCOMPARE() or QVERIFY().
// Note: define BOBUI_BUILD_INTERNAL to run this test
void tst_BOBUIreeView::taskBOBUIBUG_25333_adjustViewOptionsForIndex()
{
    BOBUIreeView view;
    QStandardItemModel model;
    QStandardItem *item1 = new QStandardItem("Item1");
    QStandardItem *item2 = new QStandardItem("Item2");
    QStandardItem *item3 = new QStandardItem("Item3");
    QStandardItem *data1 = new QStandardItem("Data1");
    QStandardItem *data2 = new QStandardItem("Data2");
    QStandardItem *data3 = new QStandardItem("Data3");

    // Create a treeview
    model.appendRow({ item1, data1 });
    model.appendRow({ item2, data2 });
    model.appendRow({ item3, data3 });

    view.setModel(&model);

    // Hide a row
    view.setRowHidden(1, QModelIndex(), true);
    view.expandAll();

    view.show();

#ifdef BOBUI_BUILD_INTERNAL
    {
        QStyleOptionViewItem option;

        view.d_func()->adjustViewOptionsForIndex(&option, model.indexFromItem(item1));

        view.d_func()->adjustViewOptionsForIndex(&option, model.indexFromItem(item3));
    }
#endif

}

void tst_BOBUIreeView::taskBOBUIBUG_18539_emitLayoutChanged()
{
    qRegisterMetaType<QList<QPersistentModelIndex>>();
    qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>();

    BOBUIreeView view;

    QStandardItem* item = new QStandardItem("Orig");
    QStandardItem* child = new QStandardItem("Child");
    item->setChild(0, 0, child);

    QStandardItemModel model;
    model.appendRow(item);

    view.setModel(&model);

    QStandardItem* replacementItem = new QStandardItem("Replacement");
    QStandardItem* replacementChild = new QStandardItem("ReplacementChild");

    replacementItem->setChild(0, 0, replacementChild);

    QSignalSpy beforeSpy(&model, &QAbstractItemModel::layoutAboutToBeChanged);
    QSignalSpy afterSpy(&model, &QAbstractItemModel::layoutChanged);

    QSignalSpy beforeRISpy(&model, &QAbstractItemModel::rowsAboutToBeInserted);
    QSignalSpy afterRISpy(&model, &QAbstractItemModel::rowsInserted);

    QSignalSpy beforeRRSpy(&model, &QAbstractItemModel::rowsAboutToBeRemoved);
    QSignalSpy afterRRSpy(&model, &QAbstractItemModel::rowsRemoved);

    model.setItem(0, 0, replacementItem);

    QCOMPARE(beforeSpy.size(), 1);
    QCOMPARE(afterSpy.size(), 1);

    QCOMPARE(beforeRISpy.size(), 0);
    QCOMPARE(afterRISpy.size(), 0);

    QCOMPARE(beforeRISpy.size(), 0);
    QCOMPARE(afterRISpy.size(), 0);
}

void tst_BOBUIreeView::taskBOBUIBUG_8176_emitOnExpandAll()
{
    BOBUIreeWidget tw;
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(&tw, QStringList(QString("item 1")));
    BOBUIreeWidgetItem *item2 = new BOBUIreeWidgetItem(item, QStringList(QString("item 2")));
    new BOBUIreeWidgetItem(item2, QStringList(QString("item 3")));
    new BOBUIreeWidgetItem(item2, QStringList(QString("item 4")));
    BOBUIreeWidgetItem *item5 = new BOBUIreeWidgetItem(&tw, QStringList(QString("item 5")));
    new BOBUIreeWidgetItem(item5, QStringList(QString("item 6")));
    QSignalSpy spy(&tw, &BOBUIreeView::expanded);

    // expand all
    tw.expandAll();
    QCOMPARE(spy.size(), 6);
    spy.clear();
    tw.collapseAll();
    item2->setExpanded(true);
    spy.clear();
    tw.expandAll();
    QCOMPARE(spy.size(), 5);

    // collapse all
    QSignalSpy spy2(&tw, &BOBUIreeView::collapsed);
    tw.collapseAll();
    QCOMPARE(spy2.size(), 6);
    tw.expandAll();
    item2->setExpanded(false);
    spy2.clear();
    tw.collapseAll();
    QCOMPARE(spy2.size(), 5);

    // expand to depth
    item2->setExpanded(true);
    spy.clear();
    spy2.clear();
    tw.expandToDepth(0);

    QCOMPARE(spy.size(), 2); // item and item5 are expanded
    QCOMPARE(spy2.size(), 1); // item2 is collapsed
}

void tst_BOBUIreeView::testInitialFocus()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIreeWidget treeWidget;
    treeWidget.setColumnCount(5);
    new BOBUIreeWidgetItem(&treeWidget, QString("1;2;3;4;5").split(QLatin1Char(';')));
    treeWidget.setTreePosition(2);
    treeWidget.header()->hideSection(0);      // make sure we skip hidden section(s)
    treeWidget.header()->swapSections(1, 2);  // make sure that we look for first visual index (and not first logical)
    treeWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&treeWidget));
    BOBUIRY_COMPARE(treeWidget.currentIndex().column(), 2);
}

#if BOBUI_CONFIG(animation)
void tst_BOBUIreeView::quickExpandCollapse()
{
    //this unit tests makes sure the state after the animation is restored correctly
    //after starting a 2nd animation while the first one was still on-going
    //this tests that the stateBeforeAnimation is not set to AnimatingState
    BOBUIreeView tree;
    tree.setAnimated(true);
    QStandardItemModel model;
    QStandardItem *root = new QStandardItem("root");
    root->appendRow(new QStandardItem("subnode"));
    model.appendRow(root);
    tree.setModel(&model);

    QModelIndex rootIndex = root->index();
    QVERIFY(rootIndex.isValid());

    tree.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tree));

    const QAbstractItemView::State initialState = tree.state();

    tree.expand(rootIndex);
    QCOMPARE(tree.state(), BOBUIreeView::AnimatingState);

    tree.collapse(rootIndex);
    QCOMPARE(tree.state(), BOBUIreeView::AnimatingState);

    //the animation lasts for 250ms max so 5000 (default) should be enough
    BOBUIRY_COMPARE(tree.state(), initialState);
}
#endif // animation

void tst_BOBUIreeView::taskBOBUIBUG_37813_crash()
{
    // BOBUIBUG_37813: Crash in visual / logical index mapping in BOBUIreeViewPrivate::adjustViewOptionsForIndex()
    // when hiding/moving columns. It is reproduceable with a BOBUIreeWidget only.
#ifdef BOBUI_BUILD_INTERNAL
    BOBUIreeWidget treeWidget;
    treeWidget.setDragEnabled(true);
    treeWidget.setColumnCount(2);
    QList<BOBUIreeWidgetItem *> items;
    for (int r = 0; r < 2; ++r) {
        const QString prefix = QLatin1String("Row ") + QString::number(r) + QLatin1String(" Column ");
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem();
        for (int c = 0; c < treeWidget.columnCount(); ++c)
            item->setText(c, prefix + QString::number(c));
        items.append(item);
    }
    treeWidget.addTopLevelItems(items);
    treeWidget.setColumnHidden(0, true);
    treeWidget.header()->moveSection(0, 1);
    QItemSelection sel(treeWidget.model()->index(0, 0), treeWidget.model()->index(0, 1));
    QRect rect;
    QAbstractItemViewPrivate *av = static_cast<QAbstractItemViewPrivate*>(bobui_widget_private(&treeWidget));
    const QPixmap pixmap = av->renderToPixmap(sel.indexes(), &rect);
    QVERIFY(pixmap.size().isValid());
#endif // BOBUI_BUILD_INTERNAL
}

// BOBUIBUG-45697: Using a BOBUIreeView with a multi-column model filtered by QSortFilterProxyModel,
// when sorting the source model while the widget is not yet visible and showing the widget
// later on, corruption occurs in BOBUIreeView.
class BobUIbug45697TestWidget : public QWidget
{
   Q_OBJECT
public:
    static const int columnCount = 3;

    explicit BobUIbug45697TestWidget(QWidget *parent = nullptr);
    int timerTick() const { return m_timerTick; }

public slots:
    void slotTimer();

private:
   BOBUIreeView *m_treeView;
   QStandardItemModel *m_model;
   QSortFilterProxyModel *m_sortFilterProxyModel;
   int m_timerTick = 0;
};

BobUIbug45697TestWidget::BobUIbug45697TestWidget(QWidget *parent)
    : QWidget(parent), m_treeView(new BOBUIreeView(this))
    , m_model(new QStandardItemModel(0, BobUIbug45697TestWidget::columnCount, this))
    , m_sortFilterProxyModel(new QSortFilterProxyModel(this))
 {
   QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
   vBoxLayout->addWidget(m_treeView);

   for (char sortChar = 'z'; sortChar >= 'a' ; --sortChar) {
       QList<QStandardItem *>  items;
       for (int column = 0; column < BobUIbug45697TestWidget::columnCount; ++column) {
           const QString text = QLatin1Char(sortChar) + QLatin1String(" ") + QString::number(column);
           items.append(new QStandardItem(text));
       }
       m_model->appendRow(items);
   }

   m_sortFilterProxyModel->setSourceModel(m_model);
   m_treeView->setModel(m_sortFilterProxyModel);

   QHeaderView *headerView = m_treeView->header();
   for (int s = 1, lastSection = headerView->count() - 1; s < lastSection; ++s)
       headerView->setSectionResizeMode(s, QHeaderView::ResizeToContents);

   BOBUIimer *timer = new BOBUIimer(this);
   timer->setInterval(50);
   connect(timer, &BOBUIimer::timeout, this, &BobUIbug45697TestWidget::slotTimer);
   timer->start();
}

void BobUIbug45697TestWidget::slotTimer()
{
    switch (m_timerTick++) {
    case 0:
        m_model->sort(0);
        break;
    case 1:
        show();
        break;
    default:
        close();
        break;
    }
}

void tst_BOBUIreeView::taskBOBUIBUG_45697_crash()
{
    BobUIbug45697TestWidget testWidget;
    testWidget.setWindowTitle(BOBUIest::currentTestFunction());
    testWidget.resize(400, 400);
    testWidget.move(QGuiApplication::primaryScreen()->availableGeometry().topLeft() + QPoint(100, 100));
    BOBUIRY_COMPARE_GE(testWidget.timerTick(), 2);
}

void tst_BOBUIreeView::taskBOBUIBUG_7232_AllowUserToControlSingleStep()
{
    // When we set the scrollMode to ScrollPerPixel it will adjust the scrollbars singleStep automatically
    // Setting a singlestep on a scrollbar should however imply that the user takes control.
    // Setting a singlestep to -1 return to an automatic control of the singleStep.
    BOBUIreeWidget t;
    t.setVerticalScrollBarPolicy(BobUI::ScrollBarAlwaysOn);
    t.setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOn);
    t.setColumnCount(2);
    BOBUIreeWidgetItem *mainItem = new BOBUIreeWidgetItem(&t, QStringList() << "Root");
    for (int i = 0; i < 200; ++i) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(mainItem, QStringList(QString("Item")));
        new BOBUIreeWidgetItem(item, QStringList() << "Child" << "1");
        new BOBUIreeWidgetItem(item, QStringList() << "Child" << "2");
        new BOBUIreeWidgetItem(item, QStringList() << "Child" << "3");
    }
    t.expandAll();

    t.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    t.setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    t.setGeometry(200, 200, 200, 200);
    int vStep1 = t.verticalScrollBar()->singleStep();
    int hStep1 = t.horizontalScrollBar()->singleStep();
    QCOMPARE_GT(vStep1, 1);
    QCOMPARE_GT(hStep1, 1);

    t.verticalScrollBar()->setSingleStep(1);
    t.setGeometry(300, 300, 300, 300);
    QCOMPARE(t.verticalScrollBar()->singleStep(), 1);

    t.horizontalScrollBar()->setSingleStep(1);
    t.setGeometry(400, 400, 400, 400);
    QCOMPARE(t.horizontalScrollBar()->singleStep(), 1);

    t.setGeometry(200, 200, 200, 200);
    t.verticalScrollBar()->setSingleStep(-1);
    t.horizontalScrollBar()->setSingleStep(-1);
    QCOMPARE(vStep1, t.verticalScrollBar()->singleStep());
    QCOMPARE(hStep1, t.horizontalScrollBar()->singleStep());
}

void tst_BOBUIreeView::statusTip_data()
{
    BOBUIest::addColumn<bool>("intermediateParent");
    BOBUIest::newRow("noIntermediate") << false;
    BOBUIest::newRow("intermediate") << true;
}

void tst_BOBUIreeView::statusTip()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(bool, intermediateParent);
    QMainWindow mw;
    BobUITestModel model(5, 5);
    model.statusTipsEnabled = true;
    BOBUIreeView *view = new BOBUIreeView;
    view->setModel(&model);
    view->viewport()->setMouseTracking(true);
    view->header()->viewport()->setMouseTracking(true);
    if (intermediateParent) {
        QWidget *inter = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout;
        inter->setLayout(vbox);
        vbox->addWidget(view);
        mw.setCentralWidget(inter);
    } else {
        mw.setCentralWidget(view);
    }
    mw.statusBar();
    mw.setGeometry(QRect(QPoint(QGuiApplication::primaryScreen()->geometry().center() - QPoint(250, 250)),
                                QSize(500, 500)));
    mw.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mw));
    // Ensure it is moved away first and then moved to the relevant section
    BOBUIest::mouseMove(mw.windowHandle(), view->mapTo(&mw, view->rect().bottomLeft() + QPoint(20, 20)));
    QPoint centerPoint = view->viewport()->mapTo(&mw, view->visualRect(model.index(0, 0)).center());
    BOBUIest::mouseMove(mw.windowHandle(), centerPoint);
    BOBUIRY_COMPARE(mw.statusBar()->currentMessage(), QLatin1String("[0,0,0] -- Status"));
    centerPoint = view->viewport()->mapTo(&mw, view->visualRect(model.index(0, 1)).center());
    BOBUIest::mouseMove(mw.windowHandle(), centerPoint);
    BOBUIRY_COMPARE(mw.statusBar()->currentMessage(), QLatin1String("[0,1,0] -- Status"));
    centerPoint = view->header()->viewport()->mapTo(&mw,
                    QPoint(view->header()->sectionViewportPosition(0) + view->header()->sectionSize(0) / 2,
                           view->header()->height() / 2));
    BOBUIest::mouseMove(mw.windowHandle(), centerPoint);
    BOBUIRY_COMPARE(mw.statusBar()->currentMessage(), QLatin1String("Header 0 -- Status"));
}

class FetchMoreModel : public QStandardItemModel
{
    Q_OBJECT
public:
    FetchMoreModel(QObject *parent = nullptr) : QStandardItemModel(parent)
    {
        for (int i = 0; i < 20; ++i) {
            QStandardItem *item = new QStandardItem("Row");
            item->appendRow(new QStandardItem("Child"));
            appendRow(item);
        }
    }
    bool canFetchMore(const QModelIndex &parent) const override
    {
        if (!canFetchReady || !parent.isValid())
            return false;
        if (!parent.parent().isValid())
            return rowCount(parent) < 20;
        return false;
    }
    void fetchMore(const QModelIndex &parent) override
    {
        QStandardItem *item = itemFromIndex(parent);
        for (int i = 0; i < 19; ++i)
            item->appendRow(new QStandardItem(QStringLiteral("New Child ") + QString::number(i)));
    }
    bool canFetchReady = false;
};

void tst_BOBUIreeView::fetchMoreOnScroll()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    if (QGuiApplication::platformName().startsWith(QLatin1String("eglfs"), BobUI::CaseInsensitive))
        QSKIP("EGLFS does not allow resizing on top level window");

    BOBUIreeView tw;
    FetchMoreModel im;
    tw.setModel(&im);
    tw.show();
    tw.expandAll();
    QVERIFY(BOBUIest::qWaitForWindowActive(&tw));
    // Now we can allow the fetch to happen
    im.canFetchReady = true;
    tw.verticalScrollBar()->setValue(tw.verticalScrollBar()->maximum());
    // The item should have now fetched the other children, thus bringing the count to 20
    QCOMPARE(im.item(19)->rowCount(), 20);
}

void tst_BOBUIreeView::checkIntersectedRect_data()
{
    auto createModel = [](int rowCount)
    {
        QStandardItemModel *model = new QStandardItemModel;
        for (int i = 0; i < rowCount; ++i) {
            const QList<QStandardItem *> sil({new QStandardItem(QString("Row %1 Item").arg(i)),
                                              new QStandardItem(QString("2nd column"))});
            model->appendRow(sil);
        }
        for (int i = 2; i < 4; ++i) {
            const QList<QStandardItem *> sil({new QStandardItem(QString("Row %1 Item").arg(i)),
                                              new QStandardItem(QString("2nd column"))});
            model->item(i)->appendRow(sil);
        }
        return model;
    };
    BOBUIest::addColumn<QStandardItemModel *>("model");
    BOBUIest::addColumn<QList<QModelIndex>>("changedIndexes");
    BOBUIest::addColumn<bool>("isEmpty");
    {
        auto model = createModel(5);
        BOBUIest::newRow("multiple columns")
                << model << QList<QModelIndex>({ model->index(0, 0), model->index(0, 1) }) << false;
    }
    {
        auto model = createModel(5);
        BOBUIest::newRow("multiple rows")
                << model
                << QList<QModelIndex>(
                           { model->index(0, 0), model->index(1, 0), model->index(2, 0) })
                << false;
    }
    {
        auto model = createModel(5);
        const QModelIndex idxRow2(model->indexFromItem(model->item(2)));
        BOBUIest::newRow("child row")
                << model
                << QList<QModelIndex>({ model->index(0, 0, idxRow2), model->index(0, 1, idxRow2) })
                << false;
        }
    {
        auto model = createModel(5);
        BOBUIest::newRow("hidden row")
                << model << QList<QModelIndex>({ model->index(3, 0), model->index(3, 1) }) << true;
    }
    {
        auto model = createModel(5);
        const QModelIndex idxRow3(model->indexFromItem(model->item(3)));
        BOBUIest::newRow("hidden child row")
                << model
                << QList<QModelIndex>({ model->index(0, 0, idxRow3), model->index(0, 1, idxRow3) })
                << true;
    }
    {
        auto model = createModel(50);
        BOBUIest::newRow("row outside viewport")
                << model << QList<QModelIndex>({ model->index(49, 0), model->index(49, 1) })
                << true;
    }
}

void tst_BOBUIreeView::checkIntersectedRect()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("eglfs"), BobUI::CaseInsensitive))
        QSKIP("EGLFS does not allow resizing on top level window");

    QFETCH(QStandardItemModel *, model);
    QFETCH(const QList<QModelIndex>, changedIndexes);
    QFETCH(bool, isEmpty);

    TreeView view;
    model->setParent(&view);
    view.setModel(model);
    view.resize(400, 400);
    view.show();
    view.expandAll();
    view.setRowHidden(3, QModelIndex(), true);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    view.m_intersectecRect = QRect();
    emit view.model()->dataChanged(changedIndexes.first(), changedIndexes.last());
    if (isEmpty) {
        QVERIFY(view.m_intersectecRect.isEmpty());
    } else if (!changedIndexes.first().isValid()) {
        QCOMPARE(view.m_intersectecRect, view.viewport()->rect());
    } else {
        const auto parent = changedIndexes.first().parent();
        const int rCount = view.model()->rowCount(parent);
        const int cCount = view.model()->columnCount(parent);
        for (int r = 0; r < rCount; ++r) {
            for (int c = 0; c < cCount; ++c) {
                const QModelIndex &idx = view.model()->index(r, c, parent);
                const auto rect = view.visualRect(idx);
                if (changedIndexes.contains(idx))
                    QVERIFY(view.m_intersectecRect.contains(rect));
                else
                    QVERIFY(!view.m_intersectecRect.contains(rect));
            }
        }
    }
}

static void fillModeltaskBOBUIBUG_8376(QAbstractItemModel &model)
{
    model.insertRow(0);
    model.insertColumn(0);
    model.insertColumn(1);
    QModelIndex index = model.index(0, 0);
    model.setData(index, "Level0");
    {
        model.insertRow(0, index);
        model.insertRow(1, index);
        model.insertColumn(0, index);
        model.insertColumn(1, index);

        QModelIndex idx;
        idx = model.index(0, 0, index);
        model.setData(idx, "Level1");

        idx = model.index(0, 1, index);
        model.setData(idx, "very\nvery\nhigh\ncell");
    }
}

void tst_BOBUIreeView::taskBOBUIBUG_8376()
{
    BOBUIreeView tv;
    QStandardItemModel model;
    fillModeltaskBOBUIBUG_8376(model);
    tv.setModel(&model);
    tv.expandAll(); // init layout

    QModelIndex idxLvl0 = model.index(0, 0);
    QModelIndex idxLvl1 = model.index(0, 1, idxLvl0);
    const int rowHeightLvl0 = tv.rowHeight(idxLvl0);
    const int rowHeightLvl1Visible = tv.rowHeight(idxLvl1);
    QCOMPARE_LT(rowHeightLvl0, rowHeightLvl1Visible);

    tv.hideColumn(1);
    const int rowHeightLvl1Hidden = tv.rowHeight(idxLvl1);
    QCOMPARE(rowHeightLvl0, rowHeightLvl1Hidden);

    tv.showColumn(1);
    const int rowHeightLvl1Visible2 = tv.rowHeight(idxLvl1);
    QCOMPARE(rowHeightLvl1Visible, rowHeightLvl1Visible2);
}

void tst_BOBUIreeView::taskBOBUIBUG_61476()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    // This checks that if a user clicks on an item to collapse it that it
    // does not edit (in this case change the check state) the item that is
    // now over the mouse just because it got a release event
    BOBUIreeView tv;
    QStandardItemModel model;
    QStandardItem *lastTopLevel = nullptr;
    {
        for (int i = 0; i < 4; ++i) {
            QStandardItem *item = new QStandardItem(QLatin1String("Row Item"));
            item->setCheckable(true);
            item->setCheckState(BobUI::Checked);
            model.appendRow(item);
            lastTopLevel = item;
            for (int j = 0; j < 2; ++j) {
                QStandardItem *childItem = new QStandardItem(QLatin1String("Child row Item"));
                childItem->setCheckable(true);
                childItem->setCheckState(BobUI::Checked);
                item->appendRow(childItem);
                QStandardItem *grandChild = new QStandardItem(QLatin1String("Grand child row Item"));
                grandChild->setCheckable(true);
                grandChild->setCheckState(BobUI::Checked);
                childItem->appendRow(grandChild);
            }
        }
    }
    tv.setModel(&model);
    tv.expandAll();
    // We need it to be this size so that the effect of the collapsing will
    // cause the parent item to move to be under the cursor
    tv.resize(200, 200);
    tv.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&tv));
    tv.verticalScrollBar()->setValue(tv.verticalScrollBar()->maximum());

    // We want to press specifically right around where a checkbox for the
    // parent item could be when collapsing
    BOBUIreeViewPrivate *priv = static_cast<BOBUIreeViewPrivate*>(bobui_widget_private(&tv));
    const QModelIndex mi = lastTopLevel->child(0)->index();
    const QRect rect = priv->itemDecorationRect(mi);
    const QPoint pos = rect.center();

    BOBUIest::mousePress(tv.viewport(), BobUI::LeftButton, {}, pos);
    const bool expandsOnPress =
        (tv.style()->styleHint(QStyle::SH_ListViewExpand_SelectMouseType, nullptr, &tv) == QEvent::MouseButtonPress);
    if (expandsOnPress)
        BOBUIRY_VERIFY(!tv.isExpanded(mi));

    BOBUIest::mouseRelease(tv.viewport(), BobUI::LeftButton, {}, pos);
    BOBUIRY_VERIFY(!tv.isExpanded(mi));
    QCOMPARE(lastTopLevel->checkState(), BobUI::Checked);

    // Test that it does not toggle the check state of a previously selected item when collapsing an
    // item causes it to position the item under the mouse to be the decoration for the selected item
    tv.expandAll();
    tv.verticalScrollBar()->setValue(tv.verticalScrollBar()->maximum());
    // It is not enough to programmatically select the item, we need to have it clicked on
    BOBUIest::mouseClick(tv.viewport(), BobUI::LeftButton, {}, tv.visualRect(lastTopLevel->index()).center());
    BOBUIest::mousePress(tv.viewport(), BobUI::LeftButton, {}, pos);
    if (expandsOnPress)
        BOBUIRY_VERIFY(!tv.isExpanded(mi));
    BOBUIest::mouseRelease(tv.viewport(), BobUI::LeftButton, {}, pos);
    BOBUIRY_VERIFY(!tv.isExpanded(mi));
    QCOMPARE(lastTopLevel->checkState(), BobUI::Checked);
}

void tst_BOBUIreeView::taskBOBUIBUG_42469_crash()
{
    BOBUIreeWidget treeWidget;
    BOBUIreeWidgetItem *itemOne = new BOBUIreeWidgetItem(QStringList("item1"));
    BOBUIreeWidgetItem *itemTwo = new BOBUIreeWidgetItem(QStringList("item2"));
    treeWidget.addTopLevelItem(itemOne);
    treeWidget.addTopLevelItem(itemTwo);
    treeWidget.topLevelItem(1)->addChild(new BOBUIreeWidgetItem(QStringList("child1")));

    treeWidget.setAnimated(true);
    QObject::connect(&treeWidget, &BOBUIreeWidget::itemExpanded, [&](BOBUIreeWidgetItem* p_item) {
        auto tempCount = treeWidget.topLevelItemCount();
        for (int j = 0; j < tempCount; ++j)
            if (treeWidget.topLevelItem(j) != p_item) {
                auto temp = treeWidget.topLevelItem(j);
                temp->setHidden(true);
            }
    });

    treeWidget.show();
    itemTwo->setExpanded(true);
}

void tst_BOBUIreeView::fetchUntilScreenFull()
{
    class TreeModel : public QAbstractItemModel
    {
    public:
        const int maxChildren = 49;
        explicit TreeModel(QObject* parent = nullptr) : QAbstractItemModel(parent)
        {
            QVariant rootData1("Parent Col 1");
            QVariant rootData2("Parent Col 2");
            QVector<QVariant> rootData;
            rootData.append(rootData1);
            rootData.append(rootData2);

            m_root = std::make_unique<TreeItem>(rootData, nullptr);

            QVariant childData1("Col 1");
            QVariant childData2("Col 2");
            QVector<QVariant> childData;
            childData.append(childData1);
            childData.append(childData2);

            TreeItem* item_1 = new TreeItem(childData, m_root.get());
            m_root->children.append(item_1);

            TreeItem* item_2 = new TreeItem(childData, item_1);
            item_1->children.append(item_2);
        }

        QModelIndex index(const int row, const int column,
            const QModelIndex& parent = QModelIndex()) const override
        {
            if (!hasIndex(row, column, parent))
                return QModelIndex();

            TreeItem* parentItem =
                parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : m_root.get();
            TreeItem* childItem = parentItem->children.at(row);
            return createIndex(row, column, childItem);
        }

        int rowCount(const QModelIndex& parent) const override
        {
            if (parent.column() > 0)
                return 0;

            TreeItem* parentItem = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer())
                                                    : m_root.get();
            return parentItem->children.size();
        }

        int columnCount(const QModelIndex&) const override { return 2; }

        QModelIndex parent(const QModelIndex& childIndex) const override
        {
            if (!childIndex.isValid())
                return QModelIndex();

            TreeItem* parentItem =
                static_cast<TreeItem*>(childIndex.internalPointer())->parent;
            return parentItem == m_root.get() ? QModelIndex()
                : createIndex(parentItem->rowInParent(), 0, parentItem);
        }

        QVariant data(const QModelIndex& index, const int role) const override
        {
            if (!index.isValid() || role != BobUI::DisplayRole)
                return QVariant();

            TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
            return item->data.at(index.column());
        }

        bool canFetchMore(const QModelIndex& parent) const override
        {
            if (!parent.isValid()) {
                return false;
            } else {
                TreeItem* item = static_cast<TreeItem*>(parent.internalPointer());
                return item->children.size() < maxChildren;
            }
        }

        void fetchMore(const QModelIndex& parent) override
        {
            if (!parent.isValid())
                return;

            fetchMoreCount++;
            TreeItem* parentItem = static_cast<TreeItem*>(parent.internalPointer());
            int childCount = parentItem->children.size();

            beginInsertRows(parent, childCount, childCount);

            QVariant childData1("Col 1");
            QVariant childData2("Col 2");
            QVector<QVariant> childData;
            childData.append(childData1);
            childData.append(childData2);
            TreeItem* newChild = new TreeItem(childData, parentItem);
            parentItem->children.append(newChild);

            endInsertRows();
        }

        int fetchMoreCount = 0;
    private:
        struct TreeItem
        {
            TreeItem(const QVector<QVariant>& values, TreeItem* parent)
                : data(values), parent(parent)
            {
            }
            ~TreeItem() { qDeleteAll(children); }
            int rowInParent() const
            {
                if (parent)
                    return parent->children.indexOf(const_cast<TreeItem*>(this));
                return 0;
            }
            QVector<QVariant> data;
            QVector<TreeItem*> children;
            TreeItem* parent = nullptr;
        };
        std::unique_ptr<TreeItem> m_root;
    };

    if (QGuiApplication::platformName().startsWith(QLatin1String("eglfs"), BobUI::CaseInsensitive))
        QSKIP("EGLFS does not allow resizing on top level window");


    BOBUIreeView tv;
    TreeModel model;
    tv.setModel(&model);

    const int itemHeight = tv.sizeHintForRow(0);
    tv.resize(250, itemHeight * 10);
    tv.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tv));

    tv.expand(model.index(0, 0));
    const int viewportHeight = tv.viewport()->height();
    const int itemCount = viewportHeight / itemHeight;
    const int minFetchCount = itemCount - 1;
    const int maxFetchCount = itemCount + 1;

    const bool expectedItemNumberFetched = model.fetchMoreCount >= minFetchCount
                                         && model.fetchMoreCount <= maxFetchCount;
    if (!expectedItemNumberFetched)
        qDebug() << model.fetchMoreCount << minFetchCount << maxFetchCount;
    QVERIFY(expectedItemNumberFetched);
}

static void populateModel(QStandardItemModel *model)
{
    const int depth = 10;
    for (int i1 = 0; i1 < depth; ++i1) {
        QStandardItem *s1 = new QStandardItem;
        s1->setText(QString::number(i1));
        model->appendRow(s1);
        for (int i2 = 0; i2 < depth; ++i2) {
            QStandardItem *s2 = new QStandardItem;
            s2->setText(QStringLiteral("%1 - %2").arg(i1).arg(i2));
            s1->appendRow(s2);
            for (int i3 = 0; i3 < depth; ++i3) {
                QStandardItem *s3 = new QStandardItem;
                s3->setText(QStringLiteral("%1 - %2 - %3").arg(i1).arg(i2).arg(i3));
                s2->appendRow(s3);
            }
        }
    }
}

void tst_BOBUIreeView::expandAfterTake()
{
    QStandardItemModel model;
    populateModel(&model);
    BOBUIreeView view;
    view.setUniformRowHeights(true);
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    view.expandAll();
    const std::unique_ptr<QStandardItem> reaper{model.takeItem(0)};
    populateModel(&model); // populate model again, having corrupted items inside BOBUIreeViewPrivate::expandedIndexes
    view.expandAll(); // adding new items to BOBUIreeViewPrivate::expandedIndexes with corrupted persistent indices, causing crash sometimes
}
BOBUIEST_MAIN(tst_BOBUIreeView)
#include "tst_bobuireeview.moc"
