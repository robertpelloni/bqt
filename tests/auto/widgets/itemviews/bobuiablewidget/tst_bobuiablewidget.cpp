// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QHeaderView>
#include <QLineEdit>
#include <QMimeData>
#include <QScrollBar>
#include <QSignalSpy>
#include <BOBUIableWidget>
#include <BOBUIest>

BOBUI_BEGIN_NAMESPACE
QDebug operator<<(QDebug dbg, const BOBUIableWidgetSelectionRange &range)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "Range(" << range.topRow() << ","  << range.leftColumn() << "->"
                              << range.bottomRow() << "," << range.rightColumn() << ")";
    return dbg;
}
BOBUI_END_NAMESPACE

class QObjectTableItem : public QObject, public BOBUIableWidgetItem
{
    Q_OBJECT
};

class tst_BOBUIableWidget : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

private slots:
    void initTestCase();
    void init();
    void getSetCheck();
    void selectionRange();
    void clear();
    void clearContents();
    void rowCount();
    void columnCount();
    void itemAssignment();
    void item_data();
    void item();
    void takeItem_data();
    void takeItem();
    void selectedItems_data();
    void selectedItems();
    void selectedSpannedCells_data();
    void selectedSpannedCells();
    void removeRow_data();
    void removeRow();
    void removeColumn_data();
    void removeColumn();
    void insertRow_data();
    void insertRow();
    void insertColumn_data();
    void insertColumn();
    void itemStreaming_data();
    void itemStreaming();
    void itemOwnership();
    void sortItems_data();
    void sortItems();
    void setItemWithSorting_data();
    void setItemWithSorting();
    void itemData();
    void setItemData();
    void cellWidget();
    void cellWidgetGeometry();
    void sizeHint_data();
    void sizeHint();
    void task231094();
    void task219380_removeLastRow();
    void task262056_sortDuplicate();
    void itemWithHeaderItems();
    void checkHeaderItemFlagsConflict();
    void mimeData();
    void selectedRowAfterSorting();
    void search();
#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(6, 0, 0)
    void clearItemData();
#endif
    void createPersistentOnLayoutAboutToBeChanged();
    void createPersistentOnLayoutAboutToBeChangedAutoSort();
    void moveRows_data();
    void moveRows();
    void moveRowsInvalid_data();
    void moveRowsInvalid();
#if BOBUI_CONFIG(draganddrop)
    void supportedDragActions();
#endif

private:
    std::unique_ptr<BOBUIableWidget> testWidget;
};

using IntPair = std::pair<int, int>;
using IntList = QList<int>;
using IntIntList = QList<IntPair>;

Q_DECLARE_METATYPE(BOBUIableWidgetSelectionRange)


// Testing get/set functions
void tst_BOBUIableWidget::getSetCheck()
{
    BOBUIableWidget obj1;
    // int BOBUIableWidget::rowCount()
    // void BOBUIableWidget::setRowCount(int)
    obj1.setRowCount(0);
    QCOMPARE(0, obj1.rowCount());
    obj1.setRowCount(INT_MIN);
    QCOMPARE(0, obj1.rowCount()); // Row count can never be negative
//    obj1.setRowCount(INT_MAX);
//    QCOMPARE(INT_MAX, obj1.rowCount());
    obj1.setRowCount(100);
    QCOMPARE(100, obj1.rowCount());


    // int BOBUIableWidget::columnCount()
    // void BOBUIableWidget::setColumnCount(int)
    obj1.setColumnCount(0);
    QCOMPARE(0, obj1.columnCount());
    obj1.setColumnCount(INT_MIN);
    QCOMPARE(0, obj1.columnCount()); // Column count can never be negative
    obj1.setColumnCount(1000);
    QCOMPARE(1000, obj1.columnCount());
//    obj1.setColumnCount(INT_MAX);
//    QCOMPARE(INT_MAX, obj1.columnCount());

    // BOBUIableWidgetItem * BOBUIableWidget::currentItem()
    // void BOBUIableWidget::setCurrentItem(BOBUIableWidgetItem *)
    BOBUIableWidgetItem *var3 = new BOBUIableWidgetItem("0,0");
    obj1.setItem(0, 0, var3);
    obj1.setItem(1, 1, new BOBUIableWidgetItem("1,1"));
    obj1.setItem(2, 2, new BOBUIableWidgetItem("2,2"));
    obj1.setItem(3, 3, new BOBUIableWidgetItem("3,3"));
    obj1.setCurrentItem(var3);
    QCOMPARE(var3, obj1.currentItem());
    obj1.setCurrentItem(nullptr);
    QCOMPARE(obj1.currentItem(), nullptr);
    obj1.setItem(0, 0, nullptr);
    QCOMPARE(obj1.item(0, 0), nullptr);

    // const BOBUIableWidgetItem * BOBUIableWidget::itemPrototype()
    // void BOBUIableWidget::setItemPrototype(const BOBUIableWidgetItem *)
    const BOBUIableWidgetItem *var4 = new BOBUIableWidgetItem;
    obj1.setItemPrototype(var4);
    QCOMPARE(var4, obj1.itemPrototype());
    obj1.setItemPrototype(nullptr);
    QCOMPARE(obj1.itemPrototype(), nullptr);
}

void tst_BOBUIableWidget::selectionRange()
{
    BOBUIableWidgetSelectionRange defaultSelection;
    BOBUIableWidgetSelectionRange selection(1, 2, 3, 4);

    BOBUIableWidgetSelectionRange copy(selection);
    QCOMPARE(copy, selection);
    QVERIFY(copy != defaultSelection);

    defaultSelection = copy;
    QCOMPARE(defaultSelection, copy);
}

void tst_BOBUIableWidget::initTestCase()
{
    testWidget.reset(new BOBUIableWidget);
    testWidget->show();
    QApplication::setKeyboardInputInterval(100);
}

void tst_BOBUIableWidget::init()
{
    testWidget->clear();
    testWidget->setRowCount(5);
    testWidget->setColumnCount(5);

    for (int row = 0; row < testWidget->rowCount(); ++row)
        testWidget->showRow(row);
    for (int column = 0; column < testWidget->columnCount(); ++column)
        testWidget->showColumn(column);
}

void tst_BOBUIableWidget::clearContents()
{
    BOBUIableWidgetItem *item = new BOBUIableWidgetItem("test");
    testWidget->setHorizontalHeaderItem(0, item);
    QCOMPARE(testWidget->horizontalHeaderItem(0), item);
    testWidget->clearContents();
    QCOMPARE(testWidget->horizontalHeaderItem(0), item);
}

void tst_BOBUIableWidget::clear()
{
    BOBUIableWidgetItem *item = new BOBUIableWidgetItem("foo");
    testWidget->setItem(0, 0, item);
    item->setSelected(true);

    QVERIFY(testWidget->item(0, 0) == item);
    QVERIFY(item->isSelected());


    QPointer<QObjectTableItem> bla = new QObjectTableItem();
    testWidget->setItem(1, 1, bla);

    testWidget->clear();

    QVERIFY(bla.isNull());

    QVERIFY(!testWidget->item(0,0));
    QVERIFY(!testWidget->selectedRanges().size());
    QVERIFY(!testWidget->selectedItems().size());
}

void tst_BOBUIableWidget::rowCount()
{
    int rowCountBefore = 5;
    int rowCountAfter = 10;

    int rowCount = testWidget->rowCount();
    QCOMPARE(rowCount, rowCountBefore);

    testWidget->setRowCount(rowCountAfter);
    rowCount = testWidget->rowCount();
    QCOMPARE(rowCount, rowCountAfter);

    QPersistentModelIndex index(testWidget->model()->index(rowCountAfter - 1, 0,
                                                           testWidget->rootIndex()));
    QCOMPARE(index.row(), rowCountAfter - 1);
    QCOMPARE(index.column(), 0);
    QVERIFY(index.isValid());
    testWidget->setRowCount(rowCountBefore);
    QCOMPARE(index.row(), -1);
    QCOMPARE(index.column(), -1);
    QVERIFY(!index.isValid());

    rowCountBefore = testWidget->rowCount();
    testWidget->setRowCount(-1);
    QCOMPARE(testWidget->rowCount(), rowCountBefore);
}

void tst_BOBUIableWidget::columnCount()
{
    int columnCountBefore = 5;
    int columnCountAfter = 10;

    int columnCount = testWidget->columnCount();
    QCOMPARE(columnCount, columnCountBefore);

    testWidget->setColumnCount(columnCountAfter);
    columnCount = testWidget->columnCount();
    QCOMPARE(columnCount, columnCountAfter);

    QPersistentModelIndex index(testWidget->model()->index(0, columnCountAfter - 1,
                                                           testWidget->rootIndex()));
    QCOMPARE(index.row(), 0);
    QCOMPARE(index.column(), columnCountAfter - 1);
    QVERIFY(index.isValid());
    testWidget->setColumnCount(columnCountBefore);
    QCOMPARE(index.row(), -1);
    QCOMPARE(index.column(), -1);
    QVERIFY(!index.isValid());

    columnCountBefore = testWidget->columnCount();
    testWidget->setColumnCount(-1);
    QCOMPARE(testWidget->columnCount(), columnCountBefore);
}

void tst_BOBUIableWidget::itemAssignment()
{
    BOBUIableWidgetItem itemInWidget("inWidget");
    testWidget->setItem(0, 0, &itemInWidget);
    itemInWidget.setFlags(itemInWidget.flags() | BobUI::ItemIsUserTristate);
    BOBUIableWidgetItem itemOutsideWidget("outsideWidget");

    QVERIFY(itemInWidget.tableWidget());
    QCOMPARE(itemInWidget.text(), QString("inWidget"));
    QVERIFY(itemInWidget.flags() & BobUI::ItemIsUserTristate);

    QVERIFY(!itemOutsideWidget.tableWidget());
    QCOMPARE(itemOutsideWidget.text(), QString("outsideWidget"));
    QVERIFY(!(itemOutsideWidget.flags() & BobUI::ItemIsUserTristate));

    itemOutsideWidget = itemInWidget;
    QVERIFY(!itemOutsideWidget.tableWidget());
    QCOMPARE(itemOutsideWidget.text(), QString("inWidget"));
    QVERIFY(itemOutsideWidget.flags() & BobUI::ItemIsUserTristate);
}

void tst_BOBUIableWidget::item_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<bool>("expectItem");

    BOBUIest::newRow("0x0 take [0,0]") << 0 << 0 << 0 << 0 << false;
    BOBUIest::newRow("0x0 take [4,4]") << 0 << 0 << 4 << 4 << false;
    BOBUIest::newRow("4x4 take [0,0]") << 4 << 4 << 0 << 0 << true;
    BOBUIest::newRow("4x4 take [4,4]") << 4 << 4 << 4 << 4 << false;
    BOBUIest::newRow("4x4 take [2,2]") << 4 << 4 << 2 << 2 << true;
}

void tst_BOBUIableWidget::item()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(bool, expectItem);

    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    for (int r = 0; r < testWidget->rowCount(); ++r) {
        for (int c = 0; c < testWidget->columnCount(); ++c)
            testWidget->setItem(r, c, new BOBUIableWidgetItem(QString::number(r * c + c)));
    }

    for (int r = 0; r < testWidget->rowCount(); ++r) {
        for (int c = 0; c < testWidget->columnCount(); ++c)
            QCOMPARE(testWidget->item(r, c)->text(), QString::number(r * c + c));
    }

    BOBUIableWidgetItem *item = testWidget->item(row, column);
    QCOMPARE(!!item, expectItem);
    if (expectItem)
        QCOMPARE(item->text(), QString::number(row * column + column));
}

void tst_BOBUIableWidget::takeItem_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<bool>("expectItem");

    BOBUIest::newRow("0x0 take [0,0]") << 0 << 0 << 0 << 0 << false;
    BOBUIest::newRow("0x0 take [4,4]") << 0 << 0 << 4 << 4 << false;
    BOBUIest::newRow("4x4 take [0,0]") << 4 << 4 << 0 << 0 << true;
    BOBUIest::newRow("4x4 take [4,4]") << 4 << 4 << 4 << 4 << false;
    BOBUIest::newRow("4x4 take [2,2]") << 4 << 4 << 2 << 2 << true;
}

void tst_BOBUIableWidget::takeItem()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(bool, expectItem);

    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    for (int r = 0; r < testWidget->rowCount(); ++r) {
        for (int c = 0; c < testWidget->columnCount(); ++c)
            testWidget->setItem(r, c, new BOBUIableWidgetItem(QString::number(r * c + c)));
    }

    for (int r = 0; r < testWidget->rowCount(); ++r) {
        for (int c = 0; c < testWidget->columnCount(); ++c)
            QCOMPARE(testWidget->item(r, c)->text(), QString::number(r * c + c));
    }

    QSignalSpy spy(testWidget.get(), &BOBUIableWidget::cellChanged);
    BOBUIableWidgetItem *item = testWidget->takeItem(row, column);
    QCOMPARE(!!item, expectItem);
    if (expectItem) {
        QCOMPARE(item->text(), QString::number(row * column + column));
        delete item;

        BOBUIRY_COMPARE(spy.size(), 1);
        const QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.size(), 2);
        QCOMPARE(arguments.at(0).toInt(), row);
        QCOMPARE(arguments.at(1).toInt(), column);
    }
    QVERIFY(!testWidget->takeItem(row, column));
}

void tst_BOBUIableWidget::selectedItems_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<IntIntList>("createItems");
    BOBUIest::addColumn<IntList>("hiddenRows");
    BOBUIest::addColumn<IntList>("hiddenColumns");
    BOBUIest::addColumn<BOBUIableWidgetSelectionRange>("selectionRange");
    BOBUIest::addColumn<IntIntList>("expectedItems");

    BOBUIest::newRow("3x3 empty cells, no hidden rows/columns, none selected")
        << 3 << 3
        << IntIntList()
        << IntList()
        << IntList()
        << BOBUIableWidgetSelectionRange()
        << IntIntList();

    BOBUIest::newRow("3x3 empty cells,no hidden rows/columnms, all selected")
        << 3 << 3
        << IntIntList()
        << IntList()
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << IntIntList();

    BOBUIest::newRow("3x3 (1,1) exists, no hidden rows/columnms, all selected")
        << 3 << 3
        << (IntIntList() << IntPair(1,1))
        << IntList()
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << (IntIntList() << IntPair(1,1));

    BOBUIest::newRow("3x3 (1,1) exists, row 1 hidden, all selected")
        << 3 << 3
        << (IntIntList() << IntPair(1,1))
        << (IntList() << 1)
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << IntIntList();

    BOBUIest::newRow("3x3 (1,1) exists, column 1 hidden, all selected")
        << 3 << 3
        << (IntIntList() << IntPair(1,1))
        << IntList()
        << (IntList() << 1)
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << IntIntList();

    BOBUIest::newRow("3x3 all exists, no hidden rows/columns, all selected")
        << 3 << 3
        << (IntIntList()
            << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
            << IntPair(1,0) << IntPair(1,1) << IntPair(1,2)
            << IntPair(2,0) << IntPair(2,1) << IntPair(2,2))
        << IntList()
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << (IntIntList()
            << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
            << IntPair(1,0) << IntPair(1,1) << IntPair(1,2)
            << IntPair(2,0) << IntPair(2,1) << IntPair(2,2));

    BOBUIest::newRow("3x3 all exists, row 1 hidden, all selected")
        << 3 << 3
        << (IntIntList()
            << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
            << IntPair(1,0) << IntPair(1,1) << IntPair(1,2)
            << IntPair(2,0) << IntPair(2,1) << IntPair(2,2))
        << (IntList() << 1)
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << (IntIntList()
            << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
            << IntPair(2,0) << IntPair(2,1) << IntPair(2,2));

    BOBUIest::newRow("3x3 all exists, column 1 hidden, all selected")
        << 3 << 3
        << (IntIntList()
            << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
            << IntPair(1,0) << IntPair(1,1) << IntPair(1,2)
            << IntPair(2,0) << IntPair(2,1) << IntPair(2,2))
        << IntList()
        << (IntList() << 1)
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << (IntIntList()
            << IntPair(0,0) << IntPair(0,2)
            << IntPair(1,0) << IntPair(1,2)
            << IntPair(2,0) << IntPair(2,2));

    BOBUIest::newRow("3x3 none exists, no hidden rows/columns, all selected")
        << 3 << 3
        << IntIntList()
        << IntList()
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << IntIntList();
//         << (IntIntList()
//             << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
//             << IntPair(1,0) << IntPair(1,1) << IntPair(1,2)
//             << IntPair(2,0) << IntPair(2,1) << IntPair(2,2));

    BOBUIest::newRow("3x3 none exists,  row 1 hidden, all selected, filling empty cells")
        << 3 << 3
        << IntIntList()
        << (IntList() << 1)
        << IntList()
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << IntIntList();
//         << (IntIntList()
//             << IntPair(0,0) << IntPair(0,1) << IntPair(0,2)
//             << IntPair(2,0) << IntPair(2,1) << IntPair(2,2));

    BOBUIest::newRow("3x3 none exists,  column 1 hidden, all selected")
        << 3 << 3
        << IntIntList()
        << IntList()
        << (IntList() << 1)
        << BOBUIableWidgetSelectionRange(0, 0, 2, 2)
        << IntIntList();
//         << (IntIntList()
//             << IntPair(0,0) << IntPair(0,2)
//             << IntPair(1,0) << IntPair(1,2)
//             << IntPair(2,0) << IntPair(2,2));
}

void tst_BOBUIableWidget::selectedItems()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(const IntIntList, createItems);
    QFETCH(const IntList, hiddenRows);
    QFETCH(const IntList, hiddenColumns);
    QFETCH(const BOBUIableWidgetSelectionRange, selectionRange);
    QFETCH(const IntIntList, expectedItems);

    // set dimensions and test they are ok
    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    // create and set items
    for (const auto &intPair : createItems) {
        testWidget->setItem(intPair.first, intPair.second,
                            new BOBUIableWidgetItem(QString("Item %1 %2")
                                                 .arg(intPair.first).arg(intPair.second)));
    }
    // hide rows/columns
    for (int row : hiddenRows)
        testWidget->setRowHidden(row, true);
    for (int column : hiddenColumns)
        testWidget->setColumnHidden(column, true);

    // make sure we don't have any previous selections hanging around
    QVERIFY(!testWidget->selectedRanges().size());
    QVERIFY(!testWidget->selectedItems().size());

    // select range and check that it is set correctly
    testWidget->setRangeSelected(selectionRange, true);
    if (selectionRange.topRow() >= 0) {
        QCOMPARE(testWidget->selectedRanges().size(), 1);
        QCOMPARE(testWidget->selectedRanges().at(0).topRow(), selectionRange.topRow());
        QCOMPARE(testWidget->selectedRanges().at(0).bottomRow(), selectionRange.bottomRow());
        QCOMPARE(testWidget->selectedRanges().at(0).leftColumn(), selectionRange.leftColumn());
        QCOMPARE(testWidget->selectedRanges().at(0).rightColumn(), selectionRange.rightColumn());
    } else {
        QCOMPARE(testWidget->selectedRanges().size(), 0);
    }

    // check that the correct number of items and the expected items are there
    const QList<BOBUIableWidgetItem *> selectedItems = testWidget->selectedItems();
    QCOMPARE(selectedItems.size(), expectedItems.size());
    for (const auto &intPair : expectedItems)
        QVERIFY(selectedItems.contains(testWidget->item(intPair.first, intPair.second)));

    // check that setItemSelected agrees with selectedItems
    for (int row = 0; row < testWidget->rowCount(); ++row) {
        if (hiddenRows.contains(row))
            continue;

        for (int column = 0; column < testWidget->columnCount(); ++column) {
            if (hiddenColumns.contains(column))
                continue;

            BOBUIableWidgetItem *item = testWidget->item(row, column);
            if (item && item->isSelected())
                QVERIFY(selectedItems.contains(item));
        }
    }
}

void tst_BOBUIableWidget::selectedSpannedCells_data()
{
    BOBUIest::addColumn<QRect>("spannedCells"); // in cells, not pixels
    BOBUIest::addColumn<QPoint>("selectionStartCell");
    BOBUIest::addColumn<QPoint>("selectionEndCell");
    BOBUIest::addColumn<int>("expectedSelectionRangeCount");
    BOBUIest::addColumn<BOBUIableWidgetSelectionRange>("expectedFirstSelectionRange");

    BOBUIest::newRow("merge 2 cells in column, select adjacent left")
            << QRect(1, 2, 1, 2) << QPoint(0, 1) << QPoint(0, 3)
            << 1 << BOBUIableWidgetSelectionRange(1, 0, 3, 0);

    BOBUIest::newRow("merge 2 cells in column, select those and one more")
            << QRect(1, 2, 1, 2) << QPoint(1, 1) << QPoint(1, 3)
            << 1 << BOBUIableWidgetSelectionRange(1, 1, 3, 1);

    BOBUIest::newRow("merge 2 cells in column, select rows above")
            << QRect(1, 2, 1, 2) << QPoint(0, 0) << QPoint(3, 1)
            << 1 << BOBUIableWidgetSelectionRange(0, 0, 1, 3);

    BOBUIest::newRow("merge 4 cells in column, select adjacent right")
            << QRect(1, 0, 1, 4) << QPoint(2, 0) << QPoint(3, 3)
            << 1 << BOBUIableWidgetSelectionRange(0, 2, 3, 3);

    BOBUIest::newRow("merge 3 cells in row, select those and one more")
            << QRect(0, 1, 3, 1) << QPoint(0, 1) << QPoint(3, 1)
            << 1 << BOBUIableWidgetSelectionRange(1, 0, 1, 3);

    BOBUIest::newRow("merge 3 cells in row, select adjacent to right")
            << QRect(0, 1, 3, 1) << QPoint(3, 0) << QPoint(3, 2)
            << 1 << BOBUIableWidgetSelectionRange(0, 3, 2, 3);

    BOBUIest::newRow("merge 3 cells in row, select adjacent above")
            << QRect(0, 2, 3, 2) << QPoint(0, 1) << QPoint(2, 1)
            << 1 << BOBUIableWidgetSelectionRange(1, 0, 1, 2);
}

void tst_BOBUIableWidget::selectedSpannedCells() // BOBUIBUG-255
{
    QFETCH(QRect, spannedCells);
    QFETCH(QPoint, selectionStartCell);
    QFETCH(QPoint, selectionEndCell);
    QFETCH(int, expectedSelectionRangeCount);
    QFETCH(const BOBUIableWidgetSelectionRange, expectedFirstSelectionRange);

    BOBUIableWidget testWidget(4, 4);
    testWidget.resize(600, 200);
    testWidget.show();

    // create and set items
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r)
            testWidget.setItem(r, c, new BOBUIableWidgetItem(QString("Item %1 %2").arg(c).arg(r)));
    }

    // merge some cells
    testWidget.setSpan(spannedCells.top(), spannedCells.left(), spannedCells.height(), spannedCells.width());

    // click one cell and shift-click another, to select a range
    BOBUIest::mouseClick(testWidget.viewport(), BobUI::LeftButton, BobUI::NoModifier,
                      testWidget.visualRect(testWidget.model()->index(selectionStartCell.y(), selectionStartCell.x())).center());
    BOBUIest::mouseClick(testWidget.viewport(), BobUI::LeftButton, BobUI::ShiftModifier,
                      testWidget.visualRect(testWidget.model()->index(selectionEndCell.y(), selectionEndCell.x())).center());

    auto ranges = testWidget.selectedRanges();
    QCOMPARE(ranges.size(), expectedSelectionRangeCount);
    QCOMPARE(ranges.first(), expectedFirstSelectionRange);
}

void tst_BOBUIableWidget::removeRow_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("expectedRowCount");
    BOBUIest::addColumn<int>("expectedColumnCount");

    BOBUIest::newRow("Empty") << 0 << 0 << 0 << 0 << 0;
    BOBUIest::newRow("1x1:0") << 1 << 1 << 0 << 0 << 1;
    BOBUIest::newRow("3x3:0") << 3 << 3 << 0 << 2 << 3;
    BOBUIest::newRow("3x3:1") << 3 << 3 << 1 << 2 << 3;
    BOBUIest::newRow("3x3:2") << 3 << 3 << 2 << 2 << 3;
}

void tst_BOBUIableWidget::removeRow()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, expectedRowCount);
    QFETCH(int, expectedColumnCount);

    // set dimensions and test they are ok
    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    // fill table with items
    for (int r = 0; r < rowCount; ++r) {
        const QString rRow = QString::number(r) + QLatin1Char(':');
        for (int c = 0; c < columnCount; ++c) {
            testWidget->setItem(r, c,
                                new BOBUIableWidgetItem(rRow + QString::number(c)));
        }
    }

    // remove and compare the results
    testWidget->removeRow(row);
    QCOMPARE(testWidget->rowCount(), expectedRowCount);
    QCOMPARE(testWidget->columnCount(), expectedColumnCount);

    // check if the correct items were removed
    for (int r = 0; r < expectedRowCount; ++r) {
        const QString rRow = QString::number(r < row ? r : r + 1) +
            QLatin1Char(':');
        for (int c = 0; c < expectedColumnCount; ++c) {
            if (r < row)
                QCOMPARE(testWidget->item(r, c)->text(),
                         rRow + QString::number(c));
            else
                QCOMPARE(testWidget->item(r, c)->text(),
                         rRow + QString::number(c));
        }
    }
}

void tst_BOBUIableWidget::removeColumn_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("expectedRowCount");
    BOBUIest::addColumn<int>("expectedColumnCount");

    BOBUIest::newRow("Empty") << 0 << 0 << 0 << 0 << 0;
    BOBUIest::newRow("1x1:0") << 1 << 1 << 0 << 1 << 0;
    BOBUIest::newRow("3x3:0") << 3 << 3 << 0 << 3 << 2;
    BOBUIest::newRow("3x3:1") << 3 << 3 << 1 << 3 << 2;
    BOBUIest::newRow("3x3:2") << 3 << 3 << 2 << 3 << 2;
}

void tst_BOBUIableWidget::removeColumn()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, column);
    QFETCH(int, expectedRowCount);
    QFETCH(int, expectedColumnCount);

    // set dimensions and test they are ok
    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    // fill table with items
    for (int r = 0; r < rowCount; ++r) {
        const QString rStr = QString::number(r) + QLatin1Char(':');
        for (int c = 0; c < columnCount; ++c) {
            testWidget->setItem(r, c,
                                new BOBUIableWidgetItem(
                                    rStr + QString::number(c)));
        }
    }

    // remove and compare the results
    testWidget->removeColumn(column);
    QCOMPARE(testWidget->rowCount(), expectedRowCount);
    QCOMPARE(testWidget->columnCount(), expectedColumnCount);


    // check if the correct items were removed
    for (int r = 0; r < expectedRowCount; ++r) {
        const QString rStr = QString::number(r) + QLatin1Char(':');
        for (int c = 0; c < expectedColumnCount; ++c) {
            if (c < column)
                QCOMPARE(testWidget->item(r, c)->text(),
                         rStr + QString::number(c));
            else
                QCOMPARE(testWidget->item(r, c)->text(),
                         rStr + QString::number(c + 1));
        }
    }
}

void tst_BOBUIableWidget::insertRow_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("expectedRowCount");
    BOBUIest::addColumn<int>("expectedColumnCount");

    BOBUIest::newRow("Empty")  << 0 << 0 << 0  << 1 << 0;
    BOBUIest::newRow("1x1:0")  << 1 << 1 << 0  << 2 << 1;
    BOBUIest::newRow("3x3:-1") << 3 << 3 << -1 << 3 << 3;
    BOBUIest::newRow("3x3:0")  << 3 << 3 << 0  << 4 << 3;
    BOBUIest::newRow("3x3:1")  << 3 << 3 << 1  << 4 << 3;
    BOBUIest::newRow("3x3:2")  << 3 << 3 << 2  << 4 << 3;
    BOBUIest::newRow("3x3:3")  << 3 << 3 << 3  << 4 << 3;
    BOBUIest::newRow("3x3:4")  << 3 << 3 << 4  << 3 << 3;
}

void tst_BOBUIableWidget::insertRow()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, expectedRowCount);
    QFETCH(int, expectedColumnCount);

    // set dimensions and test they are ok
    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    // insert and compare the results
    testWidget->insertRow(row);
    QCOMPARE(testWidget->rowCount(), expectedRowCount);
    QCOMPARE(testWidget->columnCount(), expectedColumnCount);
}

void tst_BOBUIableWidget::insertColumn_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("expectedRowCount");
    BOBUIest::addColumn<int>("expectedColumnCount");

    BOBUIest::newRow("Empty")  << 0 << 0 << 0  << 0 << 1;
    BOBUIest::newRow("1x1:0")  << 1 << 1 << 0  << 1 << 2;
    BOBUIest::newRow("3x3:-1") << 3 << 3 << -1 << 3 << 3;
    BOBUIest::newRow("3x3:0")  << 3 << 3 << 0  << 3 << 4;
    BOBUIest::newRow("3x3:1")  << 3 << 3 << 1  << 3 << 4;
    BOBUIest::newRow("3x3:2")  << 3 << 3 << 2  << 3 << 4;
    BOBUIest::newRow("3x3:3")  << 3 << 3 << 3  << 3 << 4;
    BOBUIest::newRow("3x3:4")  << 3 << 3 << 4  << 3 << 3;
}

void tst_BOBUIableWidget::insertColumn()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, column);
    QFETCH(int, expectedRowCount);
    QFETCH(int, expectedColumnCount);

    // set dimensions and test they are ok
    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);
    QCOMPARE(testWidget->rowCount(), rowCount);
    QCOMPARE(testWidget->columnCount(), columnCount);

    // insert and compare the results
    testWidget->insertColumn(column);
    QCOMPARE(testWidget->rowCount(), expectedRowCount);
    QCOMPARE(testWidget->columnCount(), expectedColumnCount);
}

void tst_BOBUIableWidget::itemStreaming_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QString>("toolTip");

    BOBUIest::newRow("Data") << "item text" << "tool tip text";
}

void tst_BOBUIableWidget::itemStreaming()
{
    QFETCH(QString, text);
    QFETCH(QString, toolTip);

    BOBUIableWidgetItem item;
    QCOMPARE(item.text(), QString());
    QCOMPARE(item.toolTip(), QString());

    item.setText(text);
    item.setToolTip(toolTip);
    QCOMPARE(item.text(), text);
    QCOMPARE(item.toolTip(), toolTip);

    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out << item;

    BOBUIableWidgetItem item2;
    QCOMPARE(item2.text(), QString());
    QCOMPARE(item2.toolTip(), QString());

    QVERIFY(!buffer.isEmpty());

    QDataStream in(&buffer, QIODevice::ReadOnly);
    in >> item2;
    QCOMPARE(item2.text(), text);
    QCOMPARE(item2.toolTip(), toolTip);
}

void tst_BOBUIableWidget::itemOwnership()
{
    QPointer<QObjectTableItem> item;
    QPointer<QObjectTableItem> headerItem;

    //delete from outside
    item = new QObjectTableItem();
    testWidget->setItem(0, 0, item);
    delete item;
    QCOMPARE(testWidget->item(0, 0), nullptr);

    //delete vertical headeritem from outside
    headerItem = new QObjectTableItem();
    testWidget->setVerticalHeaderItem(0, headerItem);
    delete headerItem;
    QCOMPARE(testWidget->verticalHeaderItem(0), nullptr);

    //delete horizontal headeritem from outside
    headerItem = new QObjectTableItem();
    testWidget->setHorizontalHeaderItem(0, headerItem);
    delete headerItem;
    QCOMPARE(testWidget->horizontalHeaderItem(0), nullptr);

    //setItem
    item = new QObjectTableItem();
    testWidget->setItem(0, 0, item);
    testWidget->setItem(0, 0, new BOBUIableWidgetItem());
    QVERIFY(item.isNull());

    //setHorizontalHeaderItem
    headerItem = new QObjectTableItem();
    testWidget->setHorizontalHeaderItem(0, headerItem);
    testWidget->setHorizontalHeaderItem(0, new BOBUIableWidgetItem());
    QVERIFY(headerItem.isNull());

    //setVerticalHeaderItem
    headerItem = new QObjectTableItem();
    testWidget->setVerticalHeaderItem(0, headerItem);
    testWidget->setVerticalHeaderItem(0, new BOBUIableWidgetItem());
    QVERIFY(headerItem.isNull());

    //takeItem
    item = new QObjectTableItem();
    testWidget->setItem(0, 0, item);
    testWidget->takeItem(0, 0);
    QVERIFY(!item.isNull());
    delete item;

    // removeRow
    item = new QObjectTableItem();
    headerItem = new QObjectTableItem();
    testWidget->setItem(0, 0, item);
    testWidget->setVerticalHeaderItem(0, headerItem);
    testWidget->removeRow(0);
    QVERIFY(item.isNull());
    QVERIFY(headerItem.isNull());

    // removeColumn
    item = new QObjectTableItem();
    headerItem = new QObjectTableItem();
    testWidget->setItem(0, 0, item);
    testWidget->setHorizontalHeaderItem(0, headerItem);
    testWidget->removeColumn(0);
    QVERIFY(item.isNull());
    QVERIFY(headerItem.isNull());

    // clear
    item = new QObjectTableItem();
    testWidget->setItem(0, 0, item);
    testWidget->clear();
    QVERIFY(item.isNull());
}

void tst_BOBUIableWidget::sortItems_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<BobUI::SortOrder>("sortOrder");
    BOBUIest::addColumn<int>("sortColumn");
    BOBUIest::addColumn<QStringList>("initial");
    BOBUIest::addColumn<QStringList>("expected");
    BOBUIest::addColumn<IntList>("rows");
    BOBUIest::addColumn<IntList>("initialHidden");
    BOBUIest::addColumn<IntList>("expectedHidden");

    BOBUIest::newRow("ascending")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << "0" << "a" << "o" << "8" << "k"
            << "3" << "d" << "k" << "o" << "6"
            << "2" << "c" << "9" << "y" << "8"
            << "1" << "b" << "7" << "3" << "u")
        << (QStringList()
            << "0" << "a" << "o" << "8" << "k"
            << "1" << "b" << "7" << "3" << "u"
            << "2" << "c" << "9" << "y" << "8"
            << "3" << "d" << "k" << "o" << "6")
        << (IntList() << 0 << 3 << 2 << 1)
        << IntList()
        << IntList();

    BOBUIest::newRow("descending")
        << 4 << 5
        << BobUI::DescendingOrder
        << 0
        << (QStringList()
            << "0" << "a" << "o" << "8" << "k"
            << "3" << "d" << "k" << "o" << "6"
            << "2" << "c" << "9" << "y" << "8"
            << "1" << "b" << "7" << "3" << "u")
        << (QStringList()
            << "3" << "d" << "k" << "o" << "6"
            << "2" << "c" << "9" << "y" << "8"
            << "1" << "b" << "7" << "3" << "u"
            << "0" << "a" << "o" << "8" << "k")
        << (IntList() << 3 << 0 << 1 << 2)
        << IntList()
        << IntList();

    BOBUIest::newRow("empty table")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << QString() << QString() << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString())
        << (QStringList()
            << QString() << QString() << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString())
        << IntList()
        << IntList()
        << IntList();

    BOBUIest::newRow("half-empty table")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << "0"       << QString() << QString() << QString() << QString()
            << "3"       << "d"       << QString() << QString() << QString()
            << "2"       << "c"       << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString())
        << (QStringList()
            << "0"       << QString() << QString() << QString() << QString()
            << "2"       << "c"       << QString() << QString() << QString()
            << "3"       << "d"       << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString())
        << (IntList() << 0 << 2 << 1)
        << IntList()
        << IntList();

    BOBUIest::newRow("empty column, should not sort.")
        << 4 << 5
        << BobUI::AscendingOrder
        << 3
        << (QStringList()
            << "0"       << QString() << QString() << QString() << QString()
            << "3"       << "d"       << QString() << QString() << QString()
            << "2"       << "c"       << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString())
        << (QStringList()
            << "0"       << QString() << QString() << QString() << QString()
            << "3"       << "d"       << QString() << QString() << QString()
            << "2"       << "c"       << QString() << QString() << QString()
            << QString() << QString() << QString() << QString() << QString())
        << IntList()
        << IntList()
        << IntList();

    BOBUIest::newRow("descending with null cell, the null cell should be placed at the bottom")
        << 4 << 5
        << BobUI::DescendingOrder
        << 0
        << (QStringList()
            << "0"       << "a" << "o" << "8" << "k"
            << "3"       << "d" << "k" << "o" << "6"
            << "2"       << "c" << "9" << "y" << "8"
            << QString() << "b" << "7" << "3" << "u")
        << (QStringList()
            << "3"       << "d" << "k" << "o" << "6"
            << "2"       << "c" << "9" << "y" << "8"
            << "0"       << "a" << "o" << "8" << "k"
            << QString() << "b" << "7" << "3" << "u")
        << (IntList() << 2 << 0 << 1)
        << IntList()
        << IntList();

    BOBUIest::newRow("ascending with null cell, the null cell should be placed at the bottom")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << "0"       << "a" << "o" << "8" << "k"
            << "3"       << "d" << "k" << "o" << "6"
            << "2"       << "c" << "9" << "y" << "8"
            << QString() << "b" << "7" << "3" << "u")
        << (QStringList()
            << "0"       << "a" << "o" << "8" << "k"
            << "2"       << "c" << "9" << "y" << "8"
            << "3"       << "d" << "k" << "o" << "6"
            << QString() << "b" << "7" << "3" << "u")
        << (IntList() << 0 << 2 << 1)
        << IntList()
        << IntList();

    BOBUIest::newRow("ascending with null cells, the null cells should be placed at the bottom")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << "3"       << "d" << "k" << "o" << "6"
            << "0"       << "a" << "o" << "8" << "k"
            << QString() << "c" << "9" << "y" << "8"
            << QString() << "b" << "7" << "3" << "u")
        << (QStringList()
            << "0"       << "a" << "o" << "8" << "k"
            << "3"       << "d" << "k" << "o" << "6"
            << QString() << "c" << "9" << "y" << "8"
            << QString() << "b" << "7" << "3" << "u")
        << (IntList() << 1 << 0)
        << IntList()
        << IntList();

    BOBUIest::newRow("ascending... Check a bug in PersistentIndexes")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << "3" << "c" << "9" << "y" << "8"
            << "2" << "b" << "7" << "3" << "u"
            << "4" << "d" << "k" << "o" << "6"
            << "1" << "a" << "o" << "8" << "k")
        << (QStringList()
            << "1" << "a" << "o" << "8" << "k"
            << "2" << "b" << "7" << "3" << "u"
            << "3" << "c" << "9" << "y" << "8"
            << "4" << "d" << "k" << "o" << "6")
        << (IntList() << 2 << 1 << 3 << 0)
        << IntList()
        << IntList();

    BOBUIest::newRow("ascending with some null cells inbetween")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << QString() << "a" << "o" << "8" << "k"
            << "2"       << "c" << "9" << "y" << "8"
            << QString() << "d" << "k" << "o" << "6"
            << "1"       << "b" << "7" << "3" << "u")
        << (QStringList()
            << "1"       << "b" << "7" << "3" << "u"
            << "2"       << "c" << "9" << "y" << "8"
            << QString() << "a" << "o" << "8" << "k"
            << QString() << "d" << "k" << "o" << "6")
        << (IntList() << 1 << 0)
        << IntList()
        << IntList();

    BOBUIest::newRow("ascending hidden")
        << 4 << 5
        << BobUI::AscendingOrder
        << 0
        << (QStringList()
            << "0" << "a" << "o" << "8" << "k"
            << "3" << "d" << "k" << "o" << "6"
            << "2" << "c" << "9" << "y" << "8"
            << "1" << "b" << "7" << "3" << "u")
        << (QStringList()
            << "0" << "a" << "o" << "8" << "k"
            << "1" << "b" << "7" << "3" << "u"
            << "2" << "c" << "9" << "y" << "8"
            << "3" << "d" << "k" << "o" << "6")
        << (IntList() << 0 << 3 << 2 << 1)
        << (IntList() << 0 << 2)
        << (IntList() << 0 << 2);

    BOBUIest::newRow("descending hidden")
        << 4 << 5
        << BobUI::DescendingOrder
        << 0
        << (QStringList()
            << "0" << "a" << "o" << "8" << "k"
            << "3" << "d" << "k" << "o" << "6"
            << "2" << "c" << "9" << "y" << "8"
            << "1" << "b" << "7" << "3" << "u")
        << (QStringList()
            << "3" << "d" << "k" << "o" << "6"
            << "2" << "c" << "9" << "y" << "8"
            << "1" << "b" << "7" << "3" << "u"
            << "0" << "a" << "o" << "8" << "k")
        << (IntList() << 3 << 0 << 1 << 2)
        << (IntList() << 0 << 2)
        << (IntList() << 3 << 1);
}

void tst_BOBUIableWidget::sortItems()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(BobUI::SortOrder, sortOrder);
    QFETCH(int, sortColumn);
    QFETCH(QStringList, initial);
    QFETCH(QStringList, expected);
    QFETCH(IntList, rows);
    QFETCH(IntList, initialHidden);
    QFETCH(IntList, expectedHidden);

    testWidget->setRowCount(rowCount);
    testWidget->setColumnCount(columnCount);

    QAbstractItemModel *model = testWidget->model();
    QList<QPersistentModelIndex> persistent;

    int ti = 0;
    for (int r = 0; r < rowCount; ++r) {
        for (int c = 0; c < columnCount; ++c) {
            QString str = initial.at(ti++);
            if (!str.isEmpty()) {
                testWidget->setItem(r, c, new BOBUIableWidgetItem(str));
            }
        }
        if (testWidget->item(r, sortColumn))
            persistent << model->index(r, sortColumn, QModelIndex());
    }

    for (int h = 0; h < initialHidden.size(); ++h)
        testWidget->hideRow(initialHidden.at(h));

    QCOMPARE(testWidget->verticalHeader()->hiddenSectionCount(), initialHidden.size());

    testWidget->sortItems(sortColumn, sortOrder);

    int te = 0;
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < columnCount; ++j) {
            QString value;
            BOBUIableWidgetItem *itm = testWidget->item(i, j);
            if (itm) {
                value = itm->text();
            }
            QCOMPARE(value, expected.at(te++));
        }
        QCOMPARE(persistent.at(i).row(), rows.at(i));
        //qDebug() << "persistent" << persistent.at(i).row()
        //         << "expected" << rows.at(i);
    }

    for (int k = 0; k < expectedHidden.size(); ++k)
        QVERIFY(testWidget->isRowHidden(expectedHidden.at(k)));
}

void tst_BOBUIableWidget::setItemWithSorting_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<BobUI::SortOrder>("sortOrder");
    BOBUIest::addColumn<int>("sortColumn");
    BOBUIest::addColumn<QStringList>("initialValues");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<QString>("newValue");
    BOBUIest::addColumn<QStringList>("expectedValues");
    BOBUIest::addColumn<IntList>("expectedRows");
    BOBUIest::addColumn<bool>("reorderingExpected");

    BOBUIest::newRow("2x1 no change (ascending)")
        << 2 << 1
        << BobUI::AscendingOrder << 0
        << (QStringList() << "0" << "1")
        << 1 << 0 << "2"
        << (QStringList() << "0" << "2")
        << (IntList() << 0 << 1)
        << false;
    BOBUIest::newRow("2x1 no change (descending)")
        << 2 << 1
        << BobUI::DescendingOrder << 0
        << (QStringList() << "1" << "0")
        << 0 << 0 << "2"
        << (QStringList() << "2" << "0")
        << (IntList() << 0 << 1)
        << false;
    BOBUIest::newRow("2x1 reorder (ascending)")
        << 2 << 1
        << BobUI::AscendingOrder << 0
        << (QStringList() << "0" << "1")
        << 0 << 0 << "2"
        << (QStringList() << "1" << "2")
        << (IntList() << 1 << 0)
        << true;
    BOBUIest::newRow("2x1 reorder (descending)")
        << 2 << 1
        << BobUI::DescendingOrder << 0
        << (QStringList() << "1" << "0")
        << 1 << 0 << "2"
        << (QStringList() << "2" << "1")
        << (IntList() << 1 << 0)
        << true;
    BOBUIest::newRow("2x2 no change (ascending)")
        << 2 << 2
        << BobUI::AscendingOrder << 0
        << (QStringList()
            << "0" << "00"
            << "1" << "11")
        << 1 << 0 << "2"
        << (QStringList()
            << "0" << "00"
            << "2" << "11")
        << (IntList() << 0 << 1)
        << false;
    BOBUIest::newRow("2x2 reorder (ascending)")
        << 2 << 2
        << BobUI::AscendingOrder << 0
        << (QStringList()
            << "0" << "00"
            << "1" << "11")
        << 0 << 0 << "2"
        << (QStringList()
            << "1" << "11"
            << "2" << "00")
        << (IntList() << 1 << 0)
        << true;
    BOBUIest::newRow("2x2 reorder (ascending, sortColumn = 1)")
        << 2 << 2
        << BobUI::AscendingOrder << 1
        << (QStringList()
            << "00" << "0"
            << "11" << "1")
        << 0 << 1 << "2"
        << (QStringList()
            << "11" << "1"
            << "00" << "2")
        << (IntList() << 1 << 0)
        << true;
    BOBUIest::newRow("2x2 no change (column != sortColumn)")
        << 2 << 2
        << BobUI::AscendingOrder << 1
        << (QStringList()
            << "00" << "0"
            << "11" << "1")
        << 0 << 0 << "22"
        << (QStringList()
            << "22" << "0"
            << "11" << "1")
        << (IntList() << 0 << 1)
        << false;
    BOBUIest::newRow("8x4 reorder (ascending, sortColumn = 3)")
        << 8 << 4
        << BobUI::AscendingOrder << 3
        << (QStringList()
            << "q" << "v" << "u" << "0"
            << "e" << "j" << "i" << "10"
            << "h" << "d" << "c" << "12"
            << "k" << "g" << "f" << "14"
            << "w" << "y" << "x" << "2"
            << "t" << "s" << "o" << "4"
            << "z" << "p" << "r" << "6"
            << "n" << "m" << "l" << "8")
        << 2 << 3 << "5"
        << (QStringList()
            << "q" << "v" << "u" << "0"
            << "e" << "j" << "i" << "10"
            << "k" << "g" << "f" << "14"
            << "w" << "y" << "x" << "2"
            << "t" << "s" << "o" << "4"
            << "h" << "d" << "c" << "5"
            << "z" << "p" << "r" << "6"
            << "n" << "m" << "l" << "8")
        << (IntList() << 0 << 1 << 5 << 2 << 3 << 4 << 6 << 7)
        << true;
}

void tst_BOBUIableWidget::setItemWithSorting()
{
    static int dummy1 = qRegisterMetaType<QList<QPersistentModelIndex>>();
    static int dummy2 = qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>();
    Q_UNUSED(dummy1);
    Q_UNUSED(dummy2);
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(BobUI::SortOrder, sortOrder);
    QFETCH(int, sortColumn);
    QFETCH(QStringList, initialValues);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(QString, newValue);
    QFETCH(QStringList, expectedValues);
    QFETCH(IntList, expectedRows);
    QFETCH(bool, reorderingExpected);

    for (int i = 0; i < 2; ++i) {
        BOBUIableWidget w(rowCount, columnCount);

        QAbstractItemModel *model = w.model();
        QList<QPersistentModelIndex> persistent;

        int ti = 0;
        for (int r = 0; r < rowCount; ++r) {
            for (int c = 0; c < columnCount; ++c) {
                QString str = initialValues.at(ti++);
                w.setItem(r, c, new BOBUIableWidgetItem(str));
            }
            persistent << model->index(r, sortColumn);
        }

        w.sortItems(sortColumn, sortOrder);
        w.setSortingEnabled(true);

        QSignalSpy dataChangedSpy(model, &QAbstractItemModel::dataChanged);
        QSignalSpy layoutChangedSpy(model, &QAbstractItemModel::layoutChanged);
        QSignalSpy rowsMovedSpy(model, &QAbstractItemModel::rowsMoved);

        if (i == 0) {
            // set a new item
            BOBUIableWidgetItem *item = new BOBUIableWidgetItem(newValue);
            w.setItem(row, column, item);
        } else {
            // change the data of existing item
            BOBUIableWidgetItem *item = w.item(row, column);
            item->setText(newValue);
        }

        ti = 0;
        for (int r = 0; r < rowCount; ++r) {
            for (int c = 0; c < columnCount; ++c) {
                QString str = expectedValues.at(ti++);
                QCOMPARE(w.item(r, c)->text(), str);
            }
        }

        for (int k = 0; k < persistent.size(); ++k) {
            QCOMPARE(persistent.at(k).row(), expectedRows.at(k));
            int i = (persistent.at(k).row() * columnCount) + sortColumn;
            QCOMPARE(persistent.at(k).data().toString(), expectedValues.at(i));
        }

        if (i == 0) {
            QCOMPARE(dataChangedSpy.size(), reorderingExpected ? 0 : 1);
            QCOMPARE(rowsMovedSpy.size(), reorderingExpected ? 1 : 0);
        } else {
            QCOMPARE(dataChangedSpy.size(), 1);
            QCOMPARE(layoutChangedSpy.size(), reorderingExpected ? 1 : 0);
        }

    }
}

class BOBUIableWidgetDataChanged : public BOBUIableWidget
{
    Q_OBJECT
public:
    using BOBUIableWidget::BOBUIableWidget;

    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QList<int> &roles) override
    {
        BOBUIableWidget::dataChanged(topLeft, bottomRight, roles);
        currentRoles = roles;
    }
    QList<int> currentRoles;
};

void tst_BOBUIableWidget::itemData()
{
    BOBUIableWidgetDataChanged widget(2, 2);
    widget.setItem(0, 0, new BOBUIableWidgetItem);
    BOBUIableWidgetItem *item = widget.item(0, 0);
    QVERIFY(item);
    item->setFlags(item->flags() | BobUI::ItemIsEditable);
    item->setData(BobUI::DisplayRole,  QString("0"));
    QCOMPARE(widget.currentRoles, QList<int>({ BobUI::DisplayRole, BobUI::EditRole }));
    item->setData(BobUI::CheckStateRole, BobUI::PartiallyChecked);
    QCOMPARE(widget.currentRoles, QList<int> { BobUI::CheckStateRole });
    for (int i = 0; i < 4; ++i)
    {
        item->setData(BobUI::UserRole + i, QString::number(i + 1));
        QCOMPARE(widget.currentRoles, QList<int> { BobUI::UserRole + i });
    }
    QMap<int, QVariant> flags = widget.model()->itemData(widget.model()->index(0, 0));
    QCOMPARE(flags.size(), 6);
    for (int i = 0; i < 4; ++i)
        QCOMPARE(flags[BobUI::UserRole + i].toString(), QString::number(i + 1));
}

void tst_BOBUIableWidget::setItemData()
{
    BOBUIableWidgetDataChanged table(10, 10);
    table.setSortingEnabled(false);
    QSignalSpy dataChangedSpy(table.model(), &QAbstractItemModel::dataChanged);

    BOBUIableWidgetItem *item = new BOBUIableWidgetItem;
    table.setItem(0, 0, item);
    QCOMPARE(dataChangedSpy.size(), 1);
    QModelIndex idx = qvariant_cast<QModelIndex>(dataChangedSpy.takeFirst().at(0));

    QMap<int, QVariant> data;
    data.insert(BobUI::DisplayRole, QLatin1String("Display"));
    data.insert(BobUI::ToolTipRole, QLatin1String("ToolTip"));
    table.model()->setItemData(idx, data);
    QCOMPARE(table.currentRoles, QList<int>({ BobUI::DisplayRole, BobUI::EditRole, BobUI::ToolTipRole }));

    QCOMPARE(table.model()->data(idx, BobUI::DisplayRole).toString(), QLatin1String("Display"));
    QCOMPARE(table.model()->data(idx, BobUI::EditRole).toString(), QLatin1String("Display"));
    QCOMPARE(table.model()->data(idx, BobUI::ToolTipRole).toString(), QLatin1String("ToolTip"));
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(idx, qvariant_cast<QModelIndex>(dataChangedSpy.first().at(0)));
    QCOMPARE(idx, qvariant_cast<QModelIndex>(dataChangedSpy.first().at(1)));
    const auto roles = qvariant_cast<QList<int>>(dataChangedSpy.first().at(2));
    QCOMPARE(roles.size(), 3);
    QVERIFY(roles.contains(BobUI::DisplayRole));
    QVERIFY(roles.contains(BobUI::EditRole));
    QVERIFY(roles.contains(BobUI::ToolTipRole));
    dataChangedSpy.clear();

    table.model()->setItemData(idx, data);
    QCOMPARE(dataChangedSpy.size(), 0);

    data.clear();
    data.insert(BobUI::DisplayRole, QLatin1String("dizplaye"));
    table.model()->setItemData(idx, data);
    QCOMPARE(table.model()->data(idx, BobUI::DisplayRole).toString(), QLatin1String("dizplaye"));
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(QList<int>({ BobUI::DisplayRole, BobUI::EditRole }),
             qvariant_cast<QList<int>>(dataChangedSpy.first().at(2)));

    item->setBackground(QBrush(BobUI::red));
    item->setForeground(QBrush(BobUI::green));
    item->setSizeHint(QSize(10, 10));
    QCOMPARE(item->data(BobUI::BackgroundRole), QVariant(QBrush(BobUI::red)));
    QCOMPARE(item->data(BobUI::ForegroundRole), QVariant(QBrush(BobUI::green)));
    QCOMPARE(item->data(BobUI::SizeHintRole), QVariant(QSize(10, 10)));
    // an empty brush should result in a QVariant()
    item->setBackground(QBrush());
    item->setForeground(QBrush());
    item->setSizeHint(QSize());
    QCOMPARE(item->data(BobUI::BackgroundRole), QVariant());
    QCOMPARE(item->data(BobUI::ForegroundRole), QVariant());
    QCOMPARE(item->data(BobUI::SizeHintRole), QVariant());
}

void tst_BOBUIableWidget::cellWidget()
{
    BOBUIableWidget table(10, 10);
    QWidget widget;

    QCOMPARE(table.cellWidget(5, 5), nullptr);
    table.setCellWidget(5, 5, &widget);
    QCOMPARE(table.cellWidget(5, 5), &widget);
    table.removeCellWidget(5, 5);
    QCOMPARE(table.cellWidget(5, 5), nullptr);
}

void tst_BOBUIableWidget::cellWidgetGeometry()
{
    BOBUIableWidget tw(3,2);
    tw.show();
    // make sure the next row added is not completely visibile
    tw.resize(300, tw.rowHeight(0) * 3 + tw.rowHeight(0) / 2);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tw));

    tw.scrollToBottom();
    tw.setRowCount(tw.rowCount() + 1);
    auto item = new BOBUIableWidgetItem("Hello");
    tw.setItem(0,0,item);
    auto le = new QLineEdit("world");
    tw.setCellWidget(0,1,le);
    // process delayedPendingLayout triggered by setting the row count
    tw.doItemsLayout();
    // so y pos is 0 for the first row
    tw.scrollToTop();
    // check if updateEditorGeometries has set the correct y pos for the editors
    QCOMPARE(tw.visualItemRect(item).top(), le->geometry().top());
}

void tst_BOBUIableWidget::sizeHint_data()
{
    BOBUIest::addColumn<BobUI::ScrollBarPolicy>("scrollBarPolicy");
    BOBUIest::addColumn<QSize>("viewSize");
    BOBUIest::newRow("ScrollBarAlwaysOn") << BobUI::ScrollBarAlwaysOn << QSize();
    BOBUIest::newRow("ScrollBarAlwaysOff") << BobUI::ScrollBarAlwaysOff << QSize();
    // make sure the scrollbars are shown by resizing the view to 40x40
    BOBUIest::newRow("ScrollBarAsNeeded (40x40)") << BobUI::ScrollBarAsNeeded << QSize(40, 40);
    BOBUIest::newRow("ScrollBarAsNeeded (1000x1000)") << BobUI::ScrollBarAsNeeded << QSize(1000, 1000);
}

void tst_BOBUIableWidget::sizeHint()
{
    QFETCH(BobUI::ScrollBarPolicy, scrollBarPolicy);
    QFETCH(QSize, viewSize);

    const QString defaultStyle = QApplication::style()->name();
    QApplication::setStyle("windows");
    const auto resetStyle = qScopeGuard([defaultStyle]{
        QApplication::setStyle(defaultStyle);
    });

    BOBUIableWidget view(2, 2);
    view.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view.setVerticalScrollBarPolicy(scrollBarPolicy);
    view.setHorizontalScrollBarPolicy(scrollBarPolicy);
    for (int r = 0 ; r < view.rowCount(); ++r) {
        const QString rStr = QString::number(r) + QLatin1Char('/');
        for (int c = 0 ; c < view.columnCount(); ++c)
            view.setItem(r, c, new BOBUIableWidgetItem(rStr + QString::number(c)));
    }

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    if (viewSize.isValid()) {
        view.resize(viewSize);
        view.setColumnWidth(0, 100);
        view.setRowHeight(0, 100);
        BOBUIRY_COMPARE(view.size(), viewSize);
    }

    QApplication::processEvents(); // execute delayed layouts
    auto sizeHint = view.sizeHint();
    view.hide();
    QCOMPARE(view.sizeHint(), sizeHint);

    view.horizontalHeader()->hide();
    view.show();
    QApplication::processEvents(); // execute delayed layouts
    sizeHint = view.sizeHint();
    view.hide();
    QCOMPARE(view.sizeHint(), sizeHint);

    view.verticalHeader()->hide();
    view.show();
    QApplication::processEvents(); // execute delayed layouts
    sizeHint = view.sizeHint();
    view.hide();
    QCOMPARE(view.sizeHint(), sizeHint);
}

void tst_BOBUIableWidget::task231094()
{
    BOBUIableWidget tw(5, 3);
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 5; y++) {
            BOBUIableWidgetItem *twi = new BOBUIableWidgetItem(QLatin1String("1"));
            if (y == 1)
                twi->setFlags(BobUI::ItemIsEnabled);
            else
                twi->setFlags({});
            tw.setItem(y, x, twi);
        }
    }

    tw.setCurrentCell(1, 1);
    QCOMPARE(tw.currentRow(), 1);
    QCOMPARE(tw.currentColumn(), 1);

    //this would provoke a end-less loop
    BOBUIest::keyClick(&tw, '1');

    //all the items are disabled: the current item shouldn't have changed
    QCOMPARE(tw.currentRow(), 1);
    QCOMPARE(tw.currentColumn(), 1);
}

void tst_BOBUIableWidget::task219380_removeLastRow()
{
    testWidget->setColumnCount(1);
    testWidget->setRowCount(20);
    BOBUIableWidgetItem item;
    testWidget->setItem(18, 0, &item); //we put the item in the second last row
    testWidget->openPersistentEditor(&item);

    testWidget->scrollToBottom();

    testWidget->removeRow(19); //we remove the last row

    //we make sure the editor is at the cell position
    BOBUIRY_COMPARE(testWidget->cellWidget(18, 0)->geometry(), testWidget->visualItemRect(&item));
}

void tst_BOBUIableWidget::task262056_sortDuplicate()
{
    testWidget->setColumnCount(2);
    testWidget->setRowCount(8);
    testWidget->setSortingEnabled(true);
    const QStringList items({"AAA", "BBB", "CCC", "CCC", "DDD",
                             "EEE", "FFF", "GGG"});
    for (int i = 0; i < 8; i++ ) {
        BOBUIableWidgetItem *twi = new BOBUIableWidgetItem(items.at(i));
        testWidget->setItem(i, 0, twi);
        testWidget->setItem(i, 1, new BOBUIableWidgetItem(QLatin1String("item ") + QString::number(i)));
    }
    testWidget->sortItems(0, BobUI::AscendingOrder);
    QSignalSpy layoutChangedSpy(testWidget->model(), &QAbstractItemModel::layoutChanged);
    testWidget->item(3,0)->setBackground(BobUI::red);

    QCOMPARE(layoutChangedSpy.size(),0);

}

void tst_BOBUIableWidget::itemWithHeaderItems()
{
    // Need a separate testcase for this because the tst_BOBUIableWidget::item testcase
    // does creates BOBUIableWidgetItems for each available cell in the table. We're testing
    // the case of not all available cells having a BOBUIableWidgetItem set.
    BOBUIableWidget table(2, 1);

    BOBUIableWidgetItem *item0_0 = new BOBUIableWidgetItem(BOBUIableWidgetItem::UserType);
    table.setItem(0, 0, item0_0);

    BOBUIableWidgetItem *item1_0 = new BOBUIableWidgetItem(BOBUIableWidgetItem::UserType);
    table.setItem(1, 0, item1_0);

    QCOMPARE(table.item(0, 1), nullptr);
}

void tst_BOBUIableWidget::checkHeaderItemFlagsConflict()
{
    // BOBUIBUG-113209
    // Check that setting header item doesn't set BobUI::ItemNeverHasChildren
    // Chech that header items do not emit itemChanged.
    BOBUIableWidget table(1, 1);
    QSignalSpy itemChangeSpy(&table, &BOBUIableWidget::itemChanged);
    QVERIFY(itemChangeSpy.isValid());

    BOBUIableWidgetItem *item = new BOBUIableWidgetItem("Initial");
    table.setHorizontalHeaderItem(0, item);

    QVERIFY(!(item->flags() & BobUI::ItemNeverHasChildren));

    item->setData(BobUI::DisplayRole, "updated");

    QCOMPARE(itemChangeSpy.size(), 0);
}

class TestTableWidget : public BOBUIableWidget
{
    Q_OBJECT
public:
    using BOBUIableWidget::BOBUIableWidget;
    using BOBUIableWidget::mimeData;
    using BOBUIableWidget::indexFromItem;
    using BOBUIableWidget::keyPressEvent;
};

void tst_BOBUIableWidget::mimeData()
{
    TestTableWidget table(10, 10);

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            BOBUIableWidgetItem *item = new BOBUIableWidgetItem(QStringLiteral("123"));
            table.setItem(y, x, item);
        }
    }

    QList<BOBUIableWidgetItem *> tableWidgetItemList;
    QModelIndexList modelIndexList;

    // do these checks more than once to ensure that the "cached indexes" work as expected
    QVERIFY(!table.mimeData(tableWidgetItemList));
    QVERIFY(!table.model()->mimeData(modelIndexList));
    QVERIFY(!table.model()->mimeData(modelIndexList));
    QVERIFY(!table.mimeData(tableWidgetItemList));

    tableWidgetItemList << table.item(1, 1);
    modelIndexList << table.indexFromItem(table.item(1, 1));

    QMimeData *data;

    QVERIFY((data = table.mimeData(tableWidgetItemList)));
    delete data;

    QVERIFY((data = table.model()->mimeData(modelIndexList)));
    delete data;

    QVERIFY((data = table.model()->mimeData(modelIndexList)));
    delete data;

    QVERIFY((data = table.mimeData(tableWidgetItemList)));
    delete data;

    // check the saved data is actually the same

    QMimeData *data2;

    data = table.mimeData(tableWidgetItemList);
    data2 = table.model()->mimeData(modelIndexList);

    const QString format = QStringLiteral("application/x-qabstractitemmodeldatalist");

    QVERIFY(data->hasFormat(format));
    QVERIFY(data2->hasFormat(format));
    QCOMPARE(data->data(format), data2->data(format));

    delete data;
    delete data2;
}

void tst_BOBUIableWidget::selectedRowAfterSorting()
{
    TestTableWidget table(3,3);
    table.setSelectionBehavior(QAbstractItemView::SelectRows);
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++)
            table.setItem(r, c, new BOBUIableWidgetItem(QStringLiteral("0")));
    }
    QHeaderView *localHorizontalHeader = table.horizontalHeader();
    localHorizontalHeader->setSortIndicator(1,BobUI::DescendingOrder);
    table.setProperty("sortingEnabled",true);
    table.selectRow(1);
    table.item(1,1)->setText("9");
    QCOMPARE(table.selectedItems().size(),3);
    const auto selectedItems = table.selectedItems();
    for (BOBUIableWidgetItem *item : selectedItems)
        QCOMPARE(item->row(), 0);
}

void tst_BOBUIableWidget::search()
{
    auto createItem = [](const QString &txt)
    {
        auto item = new BOBUIableWidgetItem(txt);
        item->setFlags(item->flags().setFlag(BobUI::ItemIsEditable, false));
        return item;
    };

    struct KeyPress {
        BobUI::Key key;
        QString text;
    };
    auto checkSeries = [](TestTableWidget &tw, const QList<std::pair<KeyPress, int>> &series) {
        for (const auto &p : series) {
            QKeyEvent e(QEvent::KeyPress, p.first.key, BobUI::NoModifier, p.first.text);
            tw.keyPressEvent(&e);
            QVERIFY(tw.selectionModel()->isSelected(tw.model()->index(p.second, 0)));
        }
    };
    TestTableWidget tw(5, 1);
    tw.setItem(0, 0, createItem("12"));
    tw.setItem(1, 0, createItem("123"));
    tw.setItem(2, 0, createItem("123 4"));
    tw.setItem(3, 0, createItem("123 5"));
    tw.setItem(4, 0, createItem(" "));
    tw.show();

    KeyPress evSpace{BobUI::Key_Space, " "};
    KeyPress ev1{BobUI::Key_1, "1"};
    KeyPress ev2{BobUI::Key_2, "2"};
    KeyPress ev3{BobUI::Key_3, "3"};
    KeyPress ev4{BobUI::Key_4, "4"};
    KeyPress ev5{BobUI::Key_5, "5"};

    checkSeries(tw, {{evSpace, 4}, {ev1, 4}});
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    checkSeries(tw, {{ev1, 0}, {ev2, 0}, {ev3, 1}, {evSpace, 2}, {ev5, 3}});
    BOBUIest::qWait(QApplication::keyboardInputInterval() * 2);
    checkSeries(tw, {{ev1, 0}, {ev2, 0}, {ev3, 1}, {evSpace, 2}, {ev4, 2}});
}

#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(6, 0, 0)
void tst_BOBUIableWidget::clearItemData()
{
    BOBUIableWidget table(3, 3);
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++)
            table.setItem(r, c, new BOBUIableWidgetItem(QStringLiteral("0")));
    }
    QSignalSpy dataChangeSpy(table.model(), &QAbstractItemModel::dataChanged);
    QVERIFY(dataChangeSpy.isValid());
    QVERIFY(!table.model()->clearItemData(QModelIndex()));
    QCOMPARE(dataChangeSpy.size(), 0);
    QVERIFY(table.model()->clearItemData(table.model()->index(0, 0)));
    QVERIFY(!table.model()->index(0, 0).data().isValid());
    QCOMPARE(dataChangeSpy.size(), 1);
    const QList<QVariant> dataChangeArgs = dataChangeSpy.takeFirst();
    QCOMPARE(dataChangeArgs.at(0).value<QModelIndex>(), table.model()->index(0, 0));
    QCOMPARE(dataChangeArgs.at(1).value<QModelIndex>(), table.model()->index(0, 0));
    QVERIFY(dataChangeArgs.at(2).value<QList<int>>().isEmpty());
    QVERIFY(table.model()->clearItemData(table.model()->index(0, 0)));
    QCOMPARE(dataChangeSpy.size(), 0);
}
#endif

void tst_BOBUIableWidget::createPersistentOnLayoutAboutToBeChanged() // BOBUIBUG-93466
{
    BOBUIableWidget widget;
    widget.model()->insertColumn(0);
    QCOMPARE(widget.model()->columnCount(), 1);
    widget.model()->insertRows(0, 3);
    for (int row = 0; row < 3; ++row)
        widget.model()->setData(widget.model()->index(row, 0), row);
    QList<QPersistentModelIndex> idxList;
    QSignalSpy layoutAboutToBeChangedSpy(widget.model(), &QAbstractItemModel::layoutAboutToBeChanged);
    QSignalSpy layoutChangedSpy(widget.model(), &QAbstractItemModel::layoutChanged);
    connect(widget.model(), &QAbstractItemModel::layoutAboutToBeChanged, this, [&idxList, &widget](){
        idxList.clear();
        for (int row = 0; row < 3; ++row)
            idxList << QPersistentModelIndex(widget.model()->index(row, 0));
    });
    connect(widget.model(), &QAbstractItemModel::layoutChanged, this, [&idxList](){
        QCOMPARE(idxList.size(), 3);
        QCOMPARE(idxList.at(0).row(), 1);
        QCOMPARE(idxList.at(0).column(), 0);
        QCOMPARE(idxList.at(0).data().toInt(), 0);
        QCOMPARE(idxList.at(1).row(), 0);
        QCOMPARE(idxList.at(1).column(), 0);
        QCOMPARE(idxList.at(1).data().toInt(), -1);
        QCOMPARE(idxList.at(2).row(), 2);
        QCOMPARE(idxList.at(2).column(), 0);
        QCOMPARE(idxList.at(2).data().toInt(), 2);
    });
    widget.model()->setData(widget.model()->index(1, 0), -1);
    widget.model()->sort(0);
    QCOMPARE(layoutAboutToBeChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.size(), 1);
}

void tst_BOBUIableWidget::createPersistentOnLayoutAboutToBeChangedAutoSort() // BOBUIBUG-93466
{
    BOBUIableWidget widget;
    widget.model()->insertColumn(0);
    QCOMPARE(widget.model()->columnCount(), 1);
    widget.model()->insertRows(0, 3);
    for (int row = 0; row < 3; ++row)
        widget.model()->setData(widget.model()->index(row, 0), row);
    widget.sortByColumn(0, BobUI::AscendingOrder);
    widget.setSortingEnabled(true);
    QList<QPersistentModelIndex> idxList;
    QSignalSpy layoutAboutToBeChangedSpy(widget.model(), &QAbstractItemModel::layoutAboutToBeChanged);
    QSignalSpy layoutChangedSpy(widget.model(), &QAbstractItemModel::layoutChanged);
    connect(widget.model(), &QAbstractItemModel::layoutAboutToBeChanged, this, [&idxList, &widget](){
        idxList.clear();
        for (int row = 0; row < 3; ++row)
            idxList << QPersistentModelIndex(widget.model()->index(row, 0));
    });
    connect(widget.model(), &QAbstractItemModel::layoutChanged, this, [&idxList](){
        QCOMPARE(idxList.size(), 3);
        QCOMPARE(idxList.at(0).row(), 1);
        QCOMPARE(idxList.at(0).column(), 0);
        QCOMPARE(idxList.at(0).data().toInt(), 0);
        QCOMPARE(idxList.at(1).row(), 0);
        QCOMPARE(idxList.at(1).column(), 0);
        QCOMPARE(idxList.at(1).data().toInt(), -1);
        QCOMPARE(idxList.at(2).row(), 2);
        QCOMPARE(idxList.at(2).column(), 0);
        QCOMPARE(idxList.at(2).data().toInt(), 2);
    });
    widget.model()->setData(widget.model()->index(1, 0), -1);
    QCOMPARE(layoutAboutToBeChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.size(), 1);
}

void tst_BOBUIableWidget::moveRows_data()
{
    BOBUIest::addColumn<int>("startRow");
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<int>("destination");
    BOBUIest::addColumn<QStringList>("expected");
    BOBUIest::newRow("1_Item_from_top_to_middle") << 0 << 1 << 3 << QStringList{"B", "C", "A", "D", "E", "F"};
    BOBUIest::newRow("1_Item_from_top_to_bottom") << 0 << 1 << 6 << QStringList{"B", "C", "D", "E", "F", "A"};
    BOBUIest::newRow("1_Item_from_middle_to_top") << 2 << 1 << 0 << QStringList{"C", "A", "B", "D", "E", "F"};
    BOBUIest::newRow("1_Item_from_bottom_to_middle") << 5 << 1 << 2 << QStringList{"A", "B", "F", "C", "D", "E"};
    BOBUIest::newRow("1_Item_from_bottom to_top") << 5 << 1 << 0 << QStringList{"F", "A", "B", "C", "D", "E"};
    BOBUIest::newRow("1_Item_from_middle_to_bottom") << 2 << 1 << 6 << QStringList{"A", "B", "D", "E", "F", "C"};
    BOBUIest::newRow("1_Item_from_middle_to_middle_before") << 2 << 1 << 1 << QStringList{"A", "C", "B", "D", "E", "F"};
    BOBUIest::newRow("1_Item_from_middle_to_middle_after") << 2 << 1 << 4 << QStringList{"A", "B", "D", "C", "E", "F"};

    BOBUIest::newRow("2_Items_from_top_to_middle") << 0 << 2 << 3 << QStringList{"C", "A", "B", "D", "E", "F"};
    BOBUIest::newRow("2_Items_from_top_to_bottom") << 0 << 2 << 6 << QStringList{"C", "D", "E", "F", "A", "B"};
    BOBUIest::newRow("2_Items_from_middle_to_top") << 2 << 2 << 0 << QStringList{"C", "D", "A", "B", "E", "F"};
    BOBUIest::newRow("2_Items_from_bottom_to_middle") << 4 << 2 << 2 << QStringList{"A", "B", "E", "F", "C", "D"};
    BOBUIest::newRow("2_Items_from_bottom_to_top") << 4 << 2 << 0 << QStringList{"E", "F", "A", "B", "C", "D"};
    BOBUIest::newRow("2_Items_from_middle_to_bottom") << 2 << 2 << 6 << QStringList{"A", "B", "E", "F", "C", "D"};
    BOBUIest::newRow("2_Items_from_middle_to_middle_before") << 3 << 2 << 1 << QStringList{"A", "D", "E", "B", "C", "F"};
    BOBUIest::newRow("2_Items_from_middle_to_middle_after") << 1 << 2 << 5 << QStringList{"A", "D", "E", "B", "C", "F"};
}

void tst_BOBUIableWidget::moveRows()
{
    QFETCH(const int, startRow);
    QFETCH(const int, count);
    QFETCH(const int, destination);
    QFETCH(const QStringList, expected);
    BOBUIableWidget baseWidget;
    baseWidget.setRowCount(6);
    baseWidget.setColumnCount(2);
    for (int r = 0; r < 6; ++r) {
        baseWidget.setItem(r, 0, new BOBUIableWidgetItem(QString(QLatin1Char('A' + r)))); // "A", "B", "C", "D", "E", "F"
        baseWidget.setItem(r, 1, new BOBUIableWidgetItem(QString(QLatin1Char('a' + r)))); // "a", "b", "c", "d", "e", "f"
    }
    QAbstractItemModel *baseModel = baseWidget.model();
    QSignalSpy rowMovedSpy(baseModel, &QAbstractItemModel::rowsMoved);
    QSignalSpy rowAboutMovedSpy(baseModel, &QAbstractItemModel::rowsAboutToBeMoved);
    QVERIFY(baseModel->moveRows(QModelIndex(), startRow, count, QModelIndex(), destination));
    QCOMPARE(baseModel->rowCount(), expected.size());
    for (int i = 0; i < expected.size(); ++i) {
        QCOMPARE(baseModel->index(i, 0).data().toString(), expected.at(i));
        QCOMPARE(baseModel->index(i, 1).data().toString(), expected.at(i).toLower());
    }
    QCOMPARE(rowMovedSpy.size(), 1);
    QCOMPARE(rowAboutMovedSpy.size(), 1);
    for (const QList<QVariant> &signalArgs : {rowMovedSpy.first(), rowAboutMovedSpy.first()}){
        QVERIFY(!signalArgs.at(0).value<QModelIndex>().isValid());
        QCOMPARE(signalArgs.at(1).toInt(), startRow);
        QCOMPARE(signalArgs.at(2).toInt(), startRow + count - 1);
        QVERIFY(!signalArgs.at(3).value<QModelIndex>().isValid());
        QCOMPARE(signalArgs.at(4).toInt(), destination);
    }
}

void tst_BOBUIableWidget::moveRowsInvalid_data()
{
    BOBUIest::addColumn<BOBUIableWidget*>("baseWidget");
    BOBUIest::addColumn<QModelIndex>("startParent");
    BOBUIest::addColumn<int>("startRow");
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<QModelIndex>("destinationParent");
    BOBUIest::addColumn<int>("destination");

    constexpr int rowCount = 6;
    const auto createWidget = []() -> BOBUIableWidget* {
        BOBUIableWidget* result = new BOBUIableWidget;
        result->setRowCount(rowCount);
        result->setColumnCount(1);
        int c = 0;
        for (int r = 0; r < rowCount; ++r)
            result->setItem(r, c, new BOBUIableWidgetItem(QString(QLatin1Char('A' + r)))); // "A", "B", "C", "D", "E", "F"
        return result;
    };

    BOBUIest::addRow("destination_equal_source") << createWidget() << QModelIndex() << 0 << 1 << QModelIndex() << 0;
    BOBUIest::addRow("count_equal_0") << createWidget() << QModelIndex() << 0 << 0 << QModelIndex() << 2;
    BOBUIableWidget* tempWidget = createWidget();
    BOBUIest::addRow("move_child") << tempWidget << tempWidget->model()->index(0, 0) << 0 << 1 << QModelIndex() << 2;
    tempWidget = createWidget();
    BOBUIest::addRow("move_to_child") << tempWidget << QModelIndex() << 0 << 1 << tempWidget->model()->index(0, 0) << 2;
    BOBUIest::addRow("negative_count") << createWidget() << QModelIndex() << 0 << -1 << QModelIndex() << 2;
    BOBUIest::addRow("negative_source_row") << createWidget() << QModelIndex() << -1 << 1 << QModelIndex() << 2;
    BOBUIest::addRow("negative_destination_row") << createWidget() << QModelIndex() << 0 << 1 << QModelIndex() << -1;
    BOBUIest::addRow("source_row_equal_rowCount") << createWidget() << QModelIndex() << rowCount << 1 << QModelIndex() << 1;
    BOBUIest::addRow("source_row_equal_destination_row") << createWidget() << QModelIndex() << 2 << 1 << QModelIndex() << 2;
    BOBUIest::addRow("source_row_equal_destination_row_plus1") << createWidget() << QModelIndex() << 2 << 1 << QModelIndex() << 3;
    BOBUIest::addRow("destination_row_greater_rowCount") << createWidget() << QModelIndex() << 0 << 1 << QModelIndex() << rowCount + 1;
    BOBUIest::addRow("move_row_within_source_range") << createWidget() << QModelIndex() << 0 << 3 << QModelIndex() << 2;
}

void tst_BOBUIableWidget::moveRowsInvalid()
{
    QFETCH(BOBUIableWidget* const, baseWidget);
    QFETCH(const QModelIndex, startParent);
    QFETCH(const int, startRow);
    QFETCH(const int, count);
    QFETCH(const QModelIndex, destinationParent);
    QFETCH(const int, destination);
    QAbstractItemModel *baseModel = baseWidget->model();
    QSignalSpy rowMovedSpy(baseModel, &QAbstractItemModel::rowsMoved);
    QSignalSpy rowAboutMovedSpy(baseModel, &QAbstractItemModel::rowsAboutToBeMoved);
    QVERIFY(rowMovedSpy.isValid());
    QVERIFY(rowAboutMovedSpy.isValid());
    QVERIFY(!baseModel->moveRows(startParent, startRow, count, destinationParent, destination));
    QCOMPARE(rowMovedSpy.size(), 0);
    QCOMPARE(rowAboutMovedSpy.size(), 0);
    delete baseWidget;
}

#if BOBUI_CONFIG(draganddrop)
class MoveOnlyTableWidget : public BOBUIableWidget
{
    Q_OBJECT
public:
    using BOBUIableWidget::BOBUIableWidget;
    BobUI::DropActions supportedDropActions() const override { return BobUI::MoveAction; }
};

void tst_BOBUIableWidget::supportedDragActions()
{
    MoveOnlyTableWidget tableWidget;
    QCOMPARE(tableWidget.model()->supportedDropActions(), BobUI::MoveAction);
    // For BobUI < 6.8 compatibility reasons, supportedDragActions defaults to supportedDropActions
    QCOMPARE(tableWidget.model()->supportedDragActions(), BobUI::MoveAction);

    tableWidget.setSupportedDragActions(BobUI::CopyAction);
    QCOMPARE(tableWidget.model()->supportedDragActions(), BobUI::CopyAction);
}
#endif // BOBUI_CONFIG(draganddrop)

BOBUIEST_MAIN(tst_BOBUIableWidget)
#include "tst_bobuiablewidget.moc"
