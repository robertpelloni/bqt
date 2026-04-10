// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QIdentityProxyModel>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QSignalSpy>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <BOBUIableView>
#include <BOBUIest>
#include <private/qapplication_p.h>
#include <private/bobuiablewidget_p.h>
#include <private/bobuiesthelpers_p.h>
#if BOBUI_CONFIG(textmarkdownwriter)
#include <private/bobuiextmarkdownwriter_p.h>
#endif


using namespace BOBUIestPrivate;

#ifdef BOBUI_BUILD_INTERNAL
#define VERIFY_SPANS_CONSISTENCY(TEST_VIEW_) \
    QVERIFY(static_cast<BOBUIableViewPrivate*>(QObjectPrivate::get(TEST_VIEW_))->spans.checkConsistency())
#else
#define VERIFY_SPANS_CONSISTENCY(TEST_VIEW_) (void)false
#endif

Q_DECLARE_METATYPE(BobUI::Key);
Q_DECLARE_METATYPE(BobUI::KeyboardModifier);
Q_DECLARE_METATYPE(QItemSelectionModel::SelectionFlag);
using BoolList = QList<bool>;
using IntList = QList<int>;
using KeyList = QList<BobUI::Key>;
using SpanList = QList<QRect>;

class BobUITestTableModel: public QAbstractTableModel
{
    Q_OBJECT

signals:
    void invalidIndexEncountered() const;

public slots:
    bool submit() override { ++submit_count; return QAbstractTableModel::submit(); }

public:
    BobUITestTableModel(int rows = 0, int columns = 0, QObject *parent = nullptr)
        : QAbstractTableModel(parent), row_count(rows), column_count(columns)
    {}

    void insertRows(int rows)
    {
        beginInsertRows(QModelIndex(), row_count, row_count + rows - 1);
        row_count += rows;
        endInsertRows();
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override
    {
        return row_count;
    }

    int columnCount(const QModelIndex& = QModelIndex()) const override
    {
        return column_count;
    }

    bool isEditable(const QModelIndex &) const { return true; }

    BobUI::ItemFlags flags(const QModelIndex &index) const override
    {
        BobUI::ItemFlags index_flags = QAbstractTableModel::flags(index);
        if (disabled_rows.contains(index.row())
            || disabled_columns.contains(index.column()))
            index_flags &= ~BobUI::ItemIsEnabled;
        return index_flags;
    }

    void disableRow(int row)
    {
        disabled_rows.insert(row);
    }

    void enableRow(int row)
    {
        disabled_rows.remove(row);
    }

    void disableColumn(int column)
    {
        disabled_columns.insert(column);
    }

    void enableColumn(int column)
    {
        disabled_columns.remove(column);
    }

    QVariant data(const QModelIndex &idx, int role = BobUI::DisplayRole) const override
    {
        if (!idx.isValid() || idx.row() >= row_count || idx.column() >= column_count) {
            qWarning() << "Invalid modelIndex [%d,%d,%p]" << idx;
            emit invalidIndexEncountered();
            return QVariant();
        }

        if (role == BobUI::DisplayRole || role == BobUI::EditRole) {
            return QLatin1Char('[') + QString::number(idx.row()) + QLatin1Char(',')
                + QString::number(idx.column()) + QLatin1String(",0]");
        }

        return QVariant();
    }

    bool insertRows(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start > row_count)
            return false;

        beginInsertRows(parent, start, start + count - 1);
        row_count += count;
        endInsertRows();
        return true;
    }

    bool removeRows(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start >= row_count || row_count < count)
            return false;

        beginRemoveRows(parent, start, start + count - 1);
        row_count -= count;
        endRemoveRows();
        return true;
    }

    void removeLastRow()
    {
        beginRemoveRows(QModelIndex(), row_count - 1, row_count - 1);
        --row_count;
        endRemoveRows();
    }

    void removeAllRows()
    {
        beginRemoveRows(QModelIndex(), 0, row_count - 1);
        row_count = 0;
        endRemoveRows();
    }

    bool insertColumns(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start > column_count)
            return false;

        beginInsertColumns(parent, start, start + count - 1);
        column_count += count;
        endInsertColumns();
        return true;
    }

    bool removeColumns(int start, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if (start < 0 || start >= column_count || column_count < count)
            return false;

        beginRemoveColumns(parent, start, start + count - 1);
        column_count -= count;
        endRemoveColumns();
        return true;
    }

    void removeLastColumn()
    {
        beginRemoveColumns(QModelIndex(), column_count - 1, column_count - 1);
        --column_count;
        endRemoveColumns();
    }

    void removeAllColumns()
    {
        beginRemoveColumns(QModelIndex(), 0, column_count - 1);
        column_count = 0;
        endRemoveColumns();
    }

    bool canFetchMore(const QModelIndex &) const override
    {
        return can_fetch_more;
    }

    void fetchMore(const QModelIndex &) override
    {
        ++fetch_more_count;
    }

    QSet<int> disabled_rows;
    QSet<int> disabled_columns;
    int row_count;
    int column_count;
    int submit_count = 0;
    int fetch_more_count = 0;
    bool can_fetch_more = false;
};

class BobUITestTableView : public BOBUIableView
{
    Q_OBJECT
public:
    using BOBUIableView::BOBUIableView;

    void setModel(QAbstractItemModel *model) override
    {
        BOBUIableView::setModel(model);
        connect(selectionModel(), &QItemSelectionModel::currentChanged,
                this, &BobUITestTableView::slotCurrentChanged);
        connect(selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &BobUITestTableView::itemSelectionChanged);
        // Allow small sections in this test, since this test was made before we correctly enforced minimum sizes.
        horizontalHeader()->setMinimumSectionSize(0);
        verticalHeader()->setMinimumSectionSize(0);
    }

    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QList<int> &roles = QList<int>()) override
    {
        BOBUIableView::dataChanged(topLeft, bottomRight, roles);
        BOBUIableViewPrivate *av = static_cast<BOBUIableViewPrivate*>(bobui_widget_private(this));
        m_intersectecRect = av->intersectedRect(av->viewport->rect(), topLeft, bottomRight);
    }
    mutable QRect m_intersectecRect;

    using BOBUIableView::moveCursor;
    using BOBUIableView::isIndexHidden;
    using BOBUIableView::setSelection;
    using BOBUIableView::selectedIndexes;
    using BOBUIableView::sizeHintForRow;
    using BOBUIableView::initViewItemOption;

    bool checkSignalOrder = false;
public slots:
    void slotCurrentChanged(QModelIndex, QModelIndex) {
        hasCurrentChanged++;
        if (checkSignalOrder)
            QVERIFY(hasCurrentChanged > hasSelectionChanged);
    }

    void itemSelectionChanged(QItemSelection , QItemSelection ) {
        hasSelectionChanged++;
        if (checkSignalOrder)
            QVERIFY(hasCurrentChanged >= hasSelectionChanged);
    }
private:
    int hasCurrentChanged = 0;
    int hasSelectionChanged = 0;

    friend class tst_BOBUIableView;
    friend struct QMetaTypeId<BobUITestTableView::CursorAction>;
};
Q_DECLARE_METATYPE(BobUITestTableView::CursorAction);

class BobUITestItemDelegate : public QStyledItemDelegate
{
public:
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        return hint;
    }

    QSize hint;
};

class tst_BOBUIableView : public QObject
{
    Q_OBJECT

private:
    using CursorActionList = QList<BobUITestTableView::CursorAction>;
private slots:
    void getSetCheck();

    void noDelegate();
    void noModel();
    void emptyModel();

    void removeRows_data();
    void removeRows();

    void removeColumns_data();
    void removeColumns();

    void keyboardNavigation_data();
    void keyboardNavigation();

    void headerSections_data();
    void headerSections();

    void moveCursor_data();
    void moveCursor();

    void moveCursorStrikesBack_data();
    void moveCursorStrikesBack();

    void moveCursorBiggerJump();

    void hideRows_data();
    void hideRows();

    void hideColumns_data();
    void hideColumns();

    void selection_data();
    void selection();

    void selectRow_data();
    void selectRow();

    void selectColumn_data();
    void selectColumn();

#if BOBUI_CONFIG(shortcut)
    void selectall_data();
    void selectall();
#endif

    void visualRect_data();
    void visualRect();

    void fetchMore();
    void setHeaders();

    void resizeRowsToContents_data();
    void resizeRowsToContents();

    void resizeColumnsToContents_data();
    void resizeColumnsToContents();

    void rowViewportPosition_data();
    void rowViewportPosition();

    void rowAt_data();
    void rowAt();

    void rowHeight_data();
    void rowHeight();

    void columnViewportPosition_data();
    void columnViewportPosition();

    void columnAt_data();
    void columnAt();

    void columnWidth_data();
    void columnWidth();

    void hiddenRow_data();
    void hiddenRow();

    void hiddenColumn_data();
    void hiddenColumn();

    void sortingEnabled_data();
    void sortingEnabled();

    void sortByColumn_data();
    void sortByColumn();

    void scrollTo_data();
    void scrollTo();

    void indexAt_data();
    void indexAt();

    void span_data();
    void span();
    void spans();
    void spans_data();
    void spansAfterRowInsertion();
    void spansAfterColumnInsertion();
    void spansAfterRowRemoval();
    void spansAfterColumnRemoval();
    void spanWithMovedSections_data();
    void spanWithMovedSections();
    void editSpanFromDirections_data();
    void editSpanFromDirections();

    void checkHeaderReset();
    void checkHeaderMinSize();

    void resizeToContents();
    void resizeToContentsSpans();
    void resizeToContentsEarly();

    void tabFocus();
    void bigModel();
    void selectionSignal();
    void setCurrentIndex();

    void checkIntersectedRect_data();
    void checkIntersectedRect();

    // task-specific tests:
    void task173773_updateVerticalHeader();
    void task227953_setRootIndex();
    void task240266_veryBigColumn();
    void task248688_autoScrollNavigation();
    void task259308_scrollVerticalHeaderSwappedSections();
    void task191545_dragSelectRows();
    void taskBOBUIBUG_5062_spansInconsistency();
    void taskBOBUIBUG_4516_clickOnRichTextLabel();
#if BOBUI_CONFIG(wheelevent)
    void taskBOBUIBUG_5237_wheelEventOnHeader();
#endif
    void taskBOBUIBUG_8585_crashForNoGoodReason();
    void taskBOBUIBUG_7774_RtoLVisualRegionForSelection();
    void taskBOBUIBUG_8777_scrollToSpans();
    void taskBOBUIBUG_10169_sizeHintForRow();
    void taskBOBUIBUG_30653_doItemsLayout();
    void taskBOBUIBUG_50171_selectRowAfterSwapColumns();
    void deselectRow();
    void selectRowsAndCells();
    void selectColumnsAndCells();
    void selectWithHeader_data();
    void selectWithHeader();
    void resetDefaultSectionSize();

#if BOBUI_CONFIG(wheelevent)
    void mouseWheel_data();
    void mouseWheel();
#endif

    void addColumnWhileEditing();
    void task234926_setHeaderSorting();

    void changeHeaderData();
    void viewOptions();

    void taskBOBUIBUG_7232_AllowUserToControlSingleStep();
    void rowsInVerticalHeader();

#if BOBUI_CONFIG(textmarkdownwriter)
    void markdownWriter();
#endif
};

// Testing get/set functions
void tst_BOBUIableView::getSetCheck()
{
    BOBUIableView obj1;

    obj1.setSortingEnabled(false);
    QCOMPARE(false, obj1.isSortingEnabled());
    obj1.setSortingEnabled(true);
    QCOMPARE(true, obj1.isSortingEnabled());

    obj1.setShowGrid(false);
    QCOMPARE(false, obj1.showGrid());
    obj1.setShowGrid(true);
    QCOMPARE(true, obj1.showGrid());

    obj1.setGridStyle(BobUI::NoPen);
    QCOMPARE(BobUI::NoPen, obj1.gridStyle());
    obj1.setGridStyle(BobUI::SolidLine);
    QCOMPARE(BobUI::SolidLine, obj1.gridStyle());

    obj1.setRootIndex(QModelIndex());
    QCOMPARE(QModelIndex(), obj1.rootIndex());
    QStandardItemModel model(10, 10);
    obj1.setModel(&model);
    QModelIndex index = model.index(0, 0);
    obj1.setRootIndex(index);
    QCOMPARE(index, obj1.rootIndex());

    QHeaderView *var1 = new QHeaderView(BobUI::Horizontal);
    obj1.setHorizontalHeader(var1);
    QCOMPARE(var1, obj1.horizontalHeader());
    obj1.setHorizontalHeader(nullptr);
    QCOMPARE(var1, obj1.horizontalHeader());
    delete var1;

    QHeaderView *var2 = new QHeaderView(BobUI::Vertical);
    obj1.setVerticalHeader(var2);
    QCOMPARE(var2, obj1.verticalHeader());
    obj1.setVerticalHeader(nullptr);
    QCOMPARE(var2, obj1.verticalHeader());
    delete var2;

    QCOMPARE(obj1.isCornerButtonEnabled(), true);
    obj1.setCornerButtonEnabled(false);
    QCOMPARE(obj1.isCornerButtonEnabled(), false);
}
void tst_BOBUIableView::noDelegate()
{
    BobUITestTableModel model(3, 3);
    BOBUIableView view;
    view.setModel(&model);
    view.setItemDelegate(nullptr);
    view.show();
}

void tst_BOBUIableView::noModel()
{
    BOBUIableView view;
    view.show();
}

void tst_BOBUIableView::emptyModel()
{
    BobUITestTableModel model;
    BOBUIableView view;
    QSignalSpy spy(&model, &BobUITestTableModel::invalidIndexEncountered);
    view.setModel(&model);
    view.show();
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIableView::removeRows_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");

    BOBUIest::newRow("2x2") << 2 << 2;
    BOBUIest::newRow("10x10") << 10  << 10;
}

void tst_BOBUIableView::removeRows()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);

    BobUITestTableModel model(rowCount, columnCount);
    QSignalSpy spy(&model, &BobUITestTableModel::invalidIndexEncountered);

    BOBUIableView view;
    view.setModel(&model);
    view.show();

    model.removeLastRow();
    QCOMPARE(spy.size(), 0);

    model.removeAllRows();
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIableView::removeColumns_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");

    BOBUIest::newRow("2x2") << 2 << 2;
    BOBUIest::newRow("10x10") << 10  << 10;
}

void tst_BOBUIableView::removeColumns()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);

    BobUITestTableModel model(rowCount, columnCount);
    QSignalSpy spy(&model, &BobUITestTableModel::invalidIndexEncountered);

    BOBUIableView view;
    view.setModel(&model);
    view.show();

    model.removeLastColumn();
    QCOMPARE(spy.size(), 0);

    model.removeAllColumns();
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIableView::keyboardNavigation_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<bool>("tabKeyNavigation");
    BOBUIest::addColumn<KeyList>("keyPresses");

    const KeyList keyList {
            BobUI::Key_Up,   BobUI::Key_Up,   BobUI::Key_Right, BobUI::Key_Right,
            BobUI::Key_Up,   BobUI::Key_Left, BobUI::Key_Left,  BobUI::Key_Up,
            BobUI::Key_Down, BobUI::Key_Up,   BobUI::Key_Up,    BobUI::Key_Up,
            BobUI::Key_Up,   BobUI::Key_Up,   BobUI::Key_Up,    BobUI::Key_Left,
            BobUI::Key_Left, BobUI::Key_Up,   BobUI::Key_Down,  BobUI::Key_Down,
            BobUI::Key_Tab,  BobUI::Key_Backtab};

    BOBUIest::newRow("16x16 model") << 16  << 16 << true << keyList;
    BOBUIest::newRow("no tab") << 8  << 8 <<  false << keyList;
}

void tst_BOBUIableView::keyboardNavigation()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(bool, tabKeyNavigation);
    QFETCH(const KeyList, keyPresses);

    BobUITestTableModel model(rowCount, columnCount);
    BOBUIableView view;
    view.setModel(&model);

    view.setTabKeyNavigation(tabKeyNavigation);
    QModelIndex index = model.index(rowCount - 1, columnCount - 1);
    view.setCurrentIndex(index);

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    int row = rowCount - 1;
    int column = columnCount - 1;
    for (BobUI::Key key : keyPresses) {

        switch (key) {
        case BobUI::Key_Up:
            row = qMax(0, row - 1);
            break;
        case BobUI::Key_Down:
            row = qMin(rowCount - 1, row + 1);
            break;
        case BobUI::Key_Backtab:
            if (!tabKeyNavigation)
                break;
            Q_FALLTHROUGH();
        case BobUI::Key_Left:
            column = qMax(0, column - 1);
            break;
        case BobUI::Key_Tab:
            if (!tabKeyNavigation)
                break;
            Q_FALLTHROUGH();
        case BobUI::Key_Right:
            column = qMin(columnCount - 1, column + 1);
            break;
        default:
            break;
        }

        BOBUIest::keyClick(&view, key);
        QApplication::processEvents();

        QModelIndex index = model.index(row, column);
        QCOMPARE(view.currentIndex(), index);
    }
}

void tst_BOBUIableView::headerSections_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");

    BOBUIest::newRow("") << 10 << 10 << 5 << 5 << 30 << 30;
}

void tst_BOBUIableView::headerSections()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);

    BobUITestTableModel model(rowCount, columnCount);

    BOBUIableView view;
    QHeaderView *hheader = view.horizontalHeader();
    QHeaderView *vheader = view.verticalHeader();

    view.setModel(&model);
    hheader->setMinimumSectionSize(columnWidth);
    vheader->setMinimumSectionSize(rowHeight);
    view.show();

    hheader->doItemsLayout();
    vheader->doItemsLayout();

    QCOMPARE(hheader->count(), model.columnCount());
    QCOMPARE(vheader->count(), model.rowCount());

    view.setRowHeight(row, rowHeight);
    QCOMPARE(view.rowHeight(row), rowHeight);
    view.hideRow(row);
    QCOMPARE(view.rowHeight(row), 0);
    view.showRow(row);
    QCOMPARE(view.rowHeight(row), rowHeight);

    view.setColumnWidth(column, columnWidth);
    QCOMPARE(view.columnWidth(column), columnWidth);
    view.hideColumn(column);
    QCOMPARE(view.columnWidth(column), 0);
    view.showColumn(column);
    QCOMPARE(view.columnWidth(column), columnWidth);
}

using IntPair = std::pair<int,int>;

void tst_BOBUIableView::moveCursor_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("hideRow");
    BOBUIest::addColumn<int>("hideColumn");

    BOBUIest::addColumn<int>("startRow");
    BOBUIest::addColumn<int>("startColumn");

    BOBUIest::addColumn<BobUITestTableView::CursorAction>("cursorMoveAction");
    BOBUIest::addColumn<BobUI::KeyboardModifier>("modifier");

    BOBUIest::addColumn<int>("expectedRow");
    BOBUIest::addColumn<int>("expectedColumn");
    BOBUIest::addColumn<IntPair>("moveRow");
    BOBUIest::addColumn<IntPair>("moveColumn");

    // MoveRight
    BOBUIest::newRow("MoveRight (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveRight << BobUI::NoModifier
        << 0 << 1 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveRight (3,0)")
        << 4 << 4 << -1 << -1
        << 3 << 0
        << BobUITestTableView::MoveRight << BobUI::NoModifier
        << 3 << 1 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveRight (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveRight << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0); // ###

    BOBUIest::newRow("MoveRight, hidden column 1 (0,0)")
        << 4 << 4 << -1 << 1
        << 0 << 0
        << BobUITestTableView::MoveRight << BobUI::NoModifier
        << 0 << 2 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveRight, hidden column 3 (0,2)")
        << 4 << 4 << -1 << 3
        << 0 << 2
        << BobUITestTableView::MoveRight << BobUI::NoModifier
        << 0 << 2 << IntPair(0,0) << IntPair(0,0); // ###

    // MoveNext should in addition wrap
    BOBUIest::newRow("MoveNext (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 0 << 1 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext (0,2)")
        << 4 << 4 << -1 << -1
        << 0 << 2
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 0 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, wrap (0,3)")
        << 4 << 4 << -1 << -1
        << 0 << 3
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 1 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, wrap (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, hidden column 1 (0,0)")
        << 4 << 4 << -1 << 1
        << 0 << 0
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 0 << 2 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, wrap, hidden column 3 (0,2)")
        << 4 << 4 << -1 << 3
        << 0 << 2
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 1 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, wrap, hidden column 3 (3,2)")
        << 4 << 4 << -1 << 3
        << 3 << 2
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, wrapy, wrapx, hidden column 3, hidden row 3 (2,2)")
        << 4 << 4 << 3 << 3
        << 2 << 2
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveNext, wrap, hidden column 2, moved column from 3 to 0. (0,2)")
        << 4 << 4 << -1 << 2
        << 0 << 2
        << BobUITestTableView::MoveNext << BobUI::NoModifier
        << 1 << 3 << IntPair(0,0) << IntPair(3,0);

    // MoveLeft
    BOBUIest::newRow("MoveLeft (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveLeft << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveLeft (0,3)")
        << 4 << 4 << -1 << -1
        << 0 << 3
        << BobUITestTableView::MoveLeft << BobUI::NoModifier
        << 0 << 2 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveLeft (1,0)")
        << 4 << 4 << -1 << -1
        << 1 << 0
        << BobUITestTableView::MoveLeft << BobUI::NoModifier
        << 1 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveLeft, hidden column 0 (0,2)")
        << 4 << 4 << -1 << 1
        << 0 << 2
        << BobUITestTableView::MoveLeft << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveLeft, hidden column 0 (0,1)")
        << 4 << 4 << -1 << 0
        << 0 << 1
        << BobUITestTableView::MoveLeft << BobUI::NoModifier
        << 0 << 1 << IntPair(0,0) << IntPair(0,0);

    // MovePrevious should in addition wrap
    BOBUIest::newRow("MovePrevious (0,3)")
        << 4 << 4 << -1 << -1
        << 0 << 3
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 0 << 2 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious (0,1)")
        << 4 << 4 << -1 << -1
        << 0 << 1
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, wrap (1,0)")
        << 4 << 4 << -1 << -1
        << 1 << 0
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 0 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, wrap, (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, hidden column 1 (0,2)")
        << 4 << 4 << -1 << 1
        << 0 << 2
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, wrap, hidden column 3 (0,2)")
        << 4 << 4 << -1 << 3
        << 0 << 2
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 0 << 1 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, wrapy, hidden column 0 (0,1)")
        << 4 << 4 << -1 << 0
        << 0 << 1
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, wrap, hidden column 0, hidden row 0 (1,1)")
        << 4 << 4 << 0 << 0
        << 1 << 1
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePrevious, wrap, hidden column 1, moved column from 0 to 3. (1,2)")
        << 4 << 4 << -1 << 1
        << 1 << 2
        << BobUITestTableView::MovePrevious << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,3);

    // MoveDown
    BOBUIest::newRow("MoveDown (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveDown << BobUI::NoModifier
        << 1 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveDown (3,0)")
        << 4 << 4 << -1 << -1
        << 3 << 0
        << BobUITestTableView::MoveDown << BobUI::NoModifier
        << 3 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveDown (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveDown << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveDown, hidden row 1 (0,0)")
        << 4 << 4 << 1 << -1
        << 0 << 0
        << BobUITestTableView::MoveDown << BobUI::NoModifier
        << 2 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveDown, hidden row 3 (2,0)")
        << 4 << 4 << 3 << -1
        << 2 << 0
        << BobUITestTableView::MoveDown << BobUI::NoModifier
        << 2 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveDown, hidden row 0 hidden column 0 (0,0)")
        << 4 << 4 << 0 << 0
        << 0 << 0
        << BobUITestTableView::MoveDown << BobUI::NoModifier
        << 1 << 1 << IntPair(0,0) << IntPair(0,0);

    // MoveUp
    BOBUIest::newRow("MoveUp (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveUp << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveUp (3, 0)")
        << 4 << 4 << -1 << -1
        << 3 << 0
        << BobUITestTableView::MoveUp << BobUI::NoModifier
        << 2 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveUp (0,1)")
        << 4 << 4 << -1 << -1
        << 0 << 1
        << BobUITestTableView::MoveUp << BobUI::NoModifier
        << 0 << 1 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveUp, hidden row 1 (2,0)")
        << 4 << 4 << 1 << -1
        << 2 << 0
        << BobUITestTableView::MoveUp << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveUp, hidden row (1,0)")
        << 4 << 4 << 0 << -1
        << 1 << 0
        << BobUITestTableView::MoveUp << BobUI::NoModifier
        << 1 << 0 << IntPair(0,0) << IntPair(0,0);

    // MoveHome
    BOBUIest::newRow("MoveHome (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveHome << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveHome (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveHome << BobUI::NoModifier
        << 3 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveHome, hidden column 0 (3,3)")
        << 4 << 4 << -1 << 0
        << 3 << 3
        << BobUITestTableView::MoveHome << BobUI::NoModifier
        << 3 << 1 << IntPair(0,0) << IntPair(0,0);

    // Use Ctrl modifier
    BOBUIest::newRow("MoveHome + Ctrl (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveHome << BobUI::ControlModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveHome + Ctrl (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveHome << BobUI::ControlModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveHome + Ctrl, hidden column 0, hidden row 0 (3,3)")
        << 4 << 4 << 0 << 0
        << 3 << 3
        << BobUITestTableView::MoveHome << BobUI::ControlModifier
        << 1 << 1 << IntPair(0,0) << IntPair(0,0);

    // MoveEnd
    BOBUIest::newRow("MoveEnd (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveEnd << BobUI::NoModifier
        << 0 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveEnd (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveEnd << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveEnd, hidden column (0,0)")
        << 4 << 4 << -1 << 3
        << 0 << 0
        << BobUITestTableView::MoveEnd << BobUI::NoModifier
        << 0<< 2 << IntPair(0,0) << IntPair(0,0);

    // Use Ctrl modifier
    BOBUIest::newRow("MoveEnd + Ctrl (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MoveEnd << BobUI::ControlModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveEnd + Ctrl (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MoveEnd << BobUI::ControlModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveEnd + Ctrl, hidden column 3 (0,0)")
        << 4 << 4 << -1 << 3
        << 0 << 0
        << BobUITestTableView::MoveEnd << BobUI::ControlModifier
        << 3 << 2 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MoveEnd + Ctrl, hidden column 3, hidden row 3 (0,0)")
        << 4 << 4 << 3 << 3
        << 0 << 0
        << BobUITestTableView::MoveEnd << BobUI::ControlModifier
        << 2 << 2 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePageUp (0,0)")
        << 4 << 4 << -1 << -1
        << 0 << 0
        << BobUITestTableView::MovePageUp << BobUI::NoModifier
        << 0 << 0 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePageUp (3,3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MovePageUp << BobUI::NoModifier
        << 0 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePageDown (3, 3)")
        << 4 << 4 << -1 << -1
        << 3 << 3
        << BobUITestTableView::MovePageDown << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);

    BOBUIest::newRow("MovePageDown (0, 3)")
        << 4 << 4 << -1 << -1
        << 0 << 3
        << BobUITestTableView::MovePageDown << BobUI::NoModifier
        << 3 << 3 << IntPair(0,0) << IntPair(0,0);
}

void tst_BOBUIableView::moveCursor()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, hideRow);
    QFETCH(int, hideColumn);
    QFETCH(int, startRow);
    QFETCH(int, startColumn);
    QFETCH(BobUITestTableView::CursorAction, cursorMoveAction);
    QFETCH(BobUI::KeyboardModifier, modifier);
    QFETCH(int, expectedRow);
    QFETCH(int, expectedColumn);
    QFETCH(IntPair, moveRow);
    QFETCH(IntPair, moveColumn);

    BobUITestTableModel model(rowCount, columnCount);
    BobUITestTableView view;

    view.setModel(&model);
    // we have to make sure that PgUp/PgDown can scroll to the bottom/top
    view.resize(view.horizontalHeader()->length() + 50,
                view.verticalHeader()->length() + 50);
    view.hideRow(hideRow);
    view.hideColumn(hideColumn);
    if (moveColumn.first != moveColumn.second)
        view.horizontalHeader()->moveSection(moveColumn.first, moveColumn.second);
    if (moveRow.first != moveRow.second)
        view.verticalHeader()->moveSection(moveRow.first, moveRow.second);

    view.show();

    QModelIndex index = model.index(startRow, startColumn);
    view.setCurrentIndex(index);

    QModelIndex newIndex = view.moveCursor(cursorMoveAction, modifier);
    // expected fails, task 119433
    if(newIndex.row() == -1)
        return;
    QCOMPARE(newIndex.row(), expectedRow);
    QCOMPARE(newIndex.column(), expectedColumn);
}

void tst_BOBUIableView::moveCursorStrikesBack_data()
{
    BOBUIest::addColumn<int>("hideRow");
    BOBUIest::addColumn<int>("hideColumn");
    BOBUIest::addColumn<IntList>("disableRows");
    BOBUIest::addColumn<IntList>("disableColumns");
    BOBUIest::addColumn<QRect>("span");

    BOBUIest::addColumn<int>("startRow");
    BOBUIest::addColumn<int>("startColumn");
    BOBUIest::addColumn<CursorActionList>("cursorMoveActions");
    BOBUIest::addColumn<int>("expectedRow");
    BOBUIest::addColumn<int>("expectedColumn");

    BOBUIest::newRow("Last column disabled. Task BOBUIBUG-3878") << -1 << -1
            << IntList()
            << (IntList() << 6)
            << QRect()
            << 0 << 5
            << CursorActionList{BobUITestTableView::MoveNext}
            << 1 << 0;

    BOBUIest::newRow("Last column disabled 2. Task BOBUIBUG-3878") << -1 << -1
            << IntList()
            << (IntList() << 6)
            << QRect()
            << 1 << 0
            << CursorActionList{BobUITestTableView::MovePrevious}
            << 0 << 5;

    BOBUIest::newRow("Span, anchor column hidden") << -1 << 1
            << IntList()
            << IntList()
            << QRect(1, 2, 2, 3)
            << 2 << 0
            << CursorActionList{BobUITestTableView::MoveNext}
            << 2 << 1;

    BOBUIest::newRow("Span, anchor column disabled") << -1 << -1
            << IntList()
            << (IntList() << 1)
            << QRect(1, 2, 2, 3)
            << 2 << 0
            << CursorActionList{BobUITestTableView::MoveNext}
            << 2 << 1;

    BOBUIest::newRow("Span, anchor row hidden") << 2 << -1
            << IntList()
            << IntList()
            << QRect(1, 2, 2, 3)
            << 1 << 2
            << CursorActionList{BobUITestTableView::MoveDown}
            << 2 << 1;

    BOBUIest::newRow("Span, anchor row disabled") << -1 << -1
            << (IntList() << 2)
            << IntList()
            << QRect(1, 2, 2, 3)
            << 1 << 2
            << CursorActionList{BobUITestTableView::MoveDown}
            << 2 << 1;

    BOBUIest::newRow("Move through span right") << -1 << -1
            << IntList()
            << IntList()
            << QRect(1, 2, 2, 3)
            << 3 << 0
            << CursorActionList{BobUITestTableView::MoveRight,
                                BobUITestTableView::MoveRight}
            << 3 << 3;

    BOBUIest::newRow("Move through span left") << -1 << -1
            << IntList()
            << IntList()
            << QRect(1, 2, 2, 3)
            << 3 << 3
            << CursorActionList{BobUITestTableView::MoveLeft,
                                BobUITestTableView::MoveLeft}
            << 3 << 0;

    BOBUIest::newRow("Move through span down") << -1 << -1
            << IntList()
            << IntList()
            << QRect(1, 2, 2, 3)
            << 1 << 2
            << CursorActionList{BobUITestTableView::MoveDown,
                                BobUITestTableView::MoveDown}
            << 5 << 2;

    BOBUIest::newRow("Move through span up") << -1 << -1
            << IntList()
            << IntList()
            << QRect(1, 2, 2, 3)
            << 5 << 2
            << CursorActionList{BobUITestTableView::MoveUp,
                                BobUITestTableView::MoveUp}
            << 1 << 2;

    IntList fullList;
    for (int i = 0; i < 7; ++i)
        fullList << i;

    BOBUIest::newRow("All disabled, wrap forward. => invalid index") << -1 << -1
            << fullList
            << fullList
            << QRect()
            << 1 << 0
            << CursorActionList{BobUITestTableView::MoveNext}
            << -1 << -1;

    BOBUIest::newRow("All disabled, wrap backwards. => invalid index") << -1 << -1
            << fullList
            << fullList
            << QRect()
            << 1 << 0
            << CursorActionList{BobUITestTableView::MovePrevious}
            << -1 << -1;

    BOBUIest::newRow("Last column disabled, MoveEnd. BOBUIBUG-72400") << -1 << -1
            << IntList()
            << (IntList() << 6)
            << QRect()
            << 0 << 0
            << CursorActionList{BobUITestTableView::MoveEnd}
            << 0 << 5;

    BOBUIest::newRow("First column disabled, MoveHome. BOBUIBUG-72400") << -1 << -1
            << IntList()
            << (IntList() << 0)
            << QRect()
            << 0 << 6
            << CursorActionList{BobUITestTableView::MoveHome}
            << 0 << 1;

    BOBUIest::newRow("First row disabled, MovePageUp. BOBUIBUG-72400") << -1 << -1
            << (IntList() << 0)
            << IntList()
            << QRect()
            << 2 << 0
            << CursorActionList{BobUITestTableView::MovePageUp}
            << 1 << 0;

    BOBUIest::newRow("Last row disabled, MovePageDown. BOBUIBUG-72400") << -1 << -1
            << (IntList() << 6)
            << IntList()
            << QRect()
            << 4 << 0
            << CursorActionList{BobUITestTableView::MovePageDown}
            << 5 << 0;
}

void tst_BOBUIableView::moveCursorStrikesBack()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(int, hideRow);
    QFETCH(int, hideColumn);
    QFETCH(const IntList, disableRows);
    QFETCH(const IntList, disableColumns);
    QFETCH(QRect, span);

    QFETCH(int, startRow);
    QFETCH(int, startColumn);
    QFETCH(const CursorActionList, cursorMoveActions);
    QFETCH(int, expectedRow);
    QFETCH(int, expectedColumn);

    BobUITestTableModel model(7, 7);
    BobUITestTableView view;
    view.setModel(&model);
    view.hideRow(hideRow);
    view.hideColumn(hideColumn);

    if (span.height() && span.width())
        view.setSpan(span.top(), span.left(), span.height(), span.width());
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));
    // resize to make sure there are scrollbars
    view.resize(view.columnWidth(0) * 7, view.rowHeight(0) * 7);

    QModelIndex index = model.index(startRow, startColumn);
    view.setCurrentIndex(index);

    for (int row : disableRows)
        model.disableRow(row);
    for (int column : disableColumns)
        model.disableColumn(column);

    int newRow = -1;
    int newColumn = -1;
    for (auto cursorMoveAction : cursorMoveActions) {
        QModelIndex newIndex = view.moveCursor(cursorMoveAction, {});
        view.setCurrentIndex(newIndex);
        newRow = newIndex.row();
        newColumn = newIndex.column();
    }

    QCOMPARE(newRow, expectedRow);
    QCOMPARE(newColumn, expectedColumn);
}

void tst_BOBUIableView::moveCursorBiggerJump()
{
    BobUITestTableModel model(50, 7);
    BOBUIableView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    int height = view.horizontalHeader()->height();
    for (int i=0;i<8;i++)
        height += view.verticalHeader()->sectionSize(i);
    view.resize(view.width(), height);
    view.setCurrentIndex(model.index(0,0));

    BOBUIest::keyClick(&view, BobUI::Key_PageDown);
    QCOMPARE(view.indexAt(QPoint(0,0)), model.index(1,0));
    BOBUIest::keyClick(&view, BobUI::Key_PageDown);
    QCOMPARE(view.indexAt(QPoint(0,0)), model.index(8,0));
    BOBUIest::keyClick(&view, BobUI::Key_PageDown);
    QCOMPARE(view.indexAt(QPoint(0,0)), model.index(15,0));
    BOBUIest::keyClick(&view, BobUI::Key_PageUp);
    QCOMPARE(view.indexAt(QPoint(0,0)), model.index(14,0));
    BOBUIest::keyClick(&view, BobUI::Key_PageUp);
    QCOMPARE(view.indexAt(QPoint(0,0)), model.index(7,0));
    BOBUIest::keyClick(&view, BobUI::Key_PageUp);
    QCOMPARE(view.indexAt(QPoint(0,0)), model.index(0,0));

    BOBUIest::keyClick(&view, BobUI::Key_PageDown);
    view.verticalHeader()->hideSection(0);
    BOBUIest::keyClick(&view, BobUI::Key_PageUp);
    BOBUIRY_COMPARE(view.currentIndex().row(), view.rowAt(0));
}

void tst_BOBUIableView::hideRows_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("showRow"); // hide, then show
    BOBUIest::addColumn<int>("hideRow"); // hide only
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");

    BOBUIest::newRow("show row 0, hide row 3, no span")
      << 10 << 10
      << 0
      << 3
      << -1 << -1
      << 1 << 1;

    BOBUIest::newRow("show row 0, hide row 3, span")
      << 10 << 10
      << 0
      << 3
      << 0 << 0
      << 3 << 2;
}

void tst_BOBUIableView::hideRows()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, showRow);
    QFETCH(int, hideRow);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);

    BobUITestTableModel model(rowCount, columnCount);
    BOBUIableView view;

    view.setModel(&model);
    view.setSpan(row, column, rowSpan, columnSpan);

    view.hideRow(showRow);
    QVERIFY(view.isRowHidden(showRow));

    view.hideRow(hideRow);
    QVERIFY(view.isRowHidden(hideRow));

    view.showRow(showRow);
    QVERIFY(!view.isRowHidden(showRow));
    QVERIFY(view.isRowHidden(hideRow));
}

void tst_BOBUIableView::hideColumns_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("showColumn"); // hide, then show
    BOBUIest::addColumn<int>("hideColumn"); // hide only
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");

    BOBUIest::newRow("show col 0, hide col 3, no span")
      << 10 << 10
      << 0
      << 3
      << -1 << -1
      << 1 << 1;

    BOBUIest::newRow("show col 0, hide col 3, span")
      << 10 << 10
      << 0
      << 3
      << 0 << 0
      << 3 << 2;
}

void tst_BOBUIableView::hideColumns()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, showColumn);
    QFETCH(int, hideColumn);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);

    BobUITestTableModel model(rowCount, columnCount);

    BOBUIableView view;
    view.setModel(&model);
    view.setSpan(row, column, rowSpan, columnSpan);

    view.hideColumn(showColumn);
    QVERIFY(view.isColumnHidden(showColumn));

    view.hideColumn(hideColumn);
    QVERIFY(view.isColumnHidden(hideColumn));

    view.showColumn(showColumn);
    QVERIFY(!view.isColumnHidden(showColumn));
    QVERIFY(view.isColumnHidden(hideColumn));
}

void tst_BOBUIableView::selection_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");
    BOBUIest::addColumn<int>("hideRow");
    BOBUIest::addColumn<int>("hideColumn");
    BOBUIest::addColumn<int>("moveRowFrom");
    BOBUIest::addColumn<int>("moveRowTo");
    BOBUIest::addColumn<int>("moveColumnFrom");
    BOBUIest::addColumn<int>("moveColumnTo");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");
    BOBUIest::addColumn<int>("x");
    BOBUIest::addColumn<int>("y");
    BOBUIest::addColumn<int>("width");
    BOBUIest::addColumn<int>("height");
    BOBUIest::addColumn<QItemSelectionModel::SelectionFlag>("command");
    BOBUIest::addColumn<int>("selectedCount"); // ### make this more detailed

    BOBUIest::newRow("no span, no hidden, no moved, 3x3 select")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 9;                                // selected count

    BOBUIest::newRow("row span, no hidden, no moved, 3x3 select")
      << 10 << 10                          // dim
      << 1 << 1                            // pos
      << 2 << 1                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 8;                                // selected count

    BOBUIest::newRow("col span, no hidden, no moved, 3x3 select")
      << 10 << 10                          // dim
      << 1 << 1                            // pos
      << 1 << 2                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 8;                                // selected count

    BOBUIest::newRow("no span, row hidden, no moved, 3x3 select")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << 1 << -1                           // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 9;                                // selected count

    BOBUIest::newRow("no span, col hidden, no moved, 3x3 select")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << 1                           // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 9;                                // selected count

    BOBUIest::newRow("no span, no hidden, row moved, 3x3 select")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << -1                          // hide
      << 1 << 3                            // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 9;                                // selected count

    BOBUIest::newRow("no span, no hidden, col moved, 3x3 select")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << 1 << 3                            // move col
      << 40 << 40                          // cell size
      << 20 << 20 << 80 << 80              // rect
      << QItemSelectionModel::Select       // command
      << 9;                                // selected count
}

void tst_BOBUIableView::selection()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);
    QFETCH(int, hideRow);
    QFETCH(int, hideColumn);
    QFETCH(int, moveRowFrom);
    QFETCH(int, moveRowTo);
    QFETCH(int, moveColumnFrom);
    QFETCH(int, moveColumnTo);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    QFETCH(int, x);
    QFETCH(int, y);
    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(QItemSelectionModel::SelectionFlag, command);
    QFETCH(int, selectedCount);

    BobUITestTableModel model(rowCount, columnCount);

    BobUITestTableView view;
    view.show();
    view.setModel(&model);

    view.setSpan(row, column, rowSpan, columnSpan);

    view.hideRow(hideRow);
    view.hideColumn(hideColumn);

    view.verticalHeader()->moveSection(moveRowFrom, moveRowTo);
    view.horizontalHeader()->moveSection(moveColumnFrom, moveColumnTo);

    for (int r = 0; r < rowCount; ++r)
        view.setRowHeight(r, rowHeight);
    for (int c = 0; c < columnCount; ++c)
        view.setColumnWidth(c, columnWidth);

    view.setSelection(QRect(x, y, width, height), command);

    QCOMPARE(view.selectedIndexes().size(), selectedCount);
}

void tst_BOBUIableView::selectRow_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<QAbstractItemView::SelectionMode>("mode");
    BOBUIest::addColumn<QAbstractItemView::SelectionBehavior>("behavior");
    BOBUIest::addColumn<int>("selectedItems");

    BOBUIest::newRow("SingleSelection and SelectItems")
        << 10 << 10
        << 0
        << QAbstractItemView::SingleSelection
        << QAbstractItemView::SelectItems
        << 0;

    BOBUIest::newRow("SingleSelection and SelectRows")
        << 10 << 10
        << 0
        << QAbstractItemView::SingleSelection
        << QAbstractItemView::SelectRows
        << 10;

    BOBUIest::newRow("SingleSelection and SelectColumns")
        << 10 << 10
        << 0
        << QAbstractItemView::SingleSelection
        << QAbstractItemView::SelectColumns
        << 0;

    BOBUIest::newRow("MultiSelection and SelectItems")
        << 10 << 10
        << 0
        << QAbstractItemView::MultiSelection
        << QAbstractItemView::SelectItems
        << 10;

    BOBUIest::newRow("MultiSelection and SelectRows")
        << 10 << 10
        << 0
        << QAbstractItemView::MultiSelection
        << QAbstractItemView::SelectRows
        << 10;

    BOBUIest::newRow("MultiSelection and SelectColumns")
        << 10 << 10
        << 0
        << QAbstractItemView::MultiSelection
        << QAbstractItemView::SelectColumns
        << 0;

    BOBUIest::newRow("ExtendedSelection and SelectItems")
        << 10 << 10
        << 0
        << QAbstractItemView::ExtendedSelection
        << QAbstractItemView::SelectItems
        << 10;

    BOBUIest::newRow("ExtendedSelection and SelectRows")
        << 10 << 10
        << 0
        << QAbstractItemView::ExtendedSelection
        << QAbstractItemView::SelectRows
        << 10;

    BOBUIest::newRow("ExtendedSelection and SelectColumns")
        << 10 << 10
        << 0
        << QAbstractItemView::ExtendedSelection
        << QAbstractItemView::SelectColumns
        << 0;

    BOBUIest::newRow("ContiguousSelection and SelectItems")
        << 10 << 10
        << 0
        << QAbstractItemView::ContiguousSelection
        << QAbstractItemView::SelectItems
        << 10;

    BOBUIest::newRow("ContiguousSelection and SelectRows")
        << 10 << 10
        << 0
        << QAbstractItemView::ContiguousSelection
        << QAbstractItemView::SelectRows
        << 10;

    BOBUIest::newRow("ContiguousSelection and SelectColumns")
        << 10 << 10
        << 0
        << QAbstractItemView::ContiguousSelection
        << QAbstractItemView::SelectColumns
        << 0;
}

void tst_BOBUIableView::selectRow()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(QAbstractItemView::SelectionMode, mode);
    QFETCH(QAbstractItemView::SelectionBehavior, behavior);
    QFETCH(int, selectedItems);

    BobUITestTableModel model(rowCount, columnCount);
    BOBUIableView view;

    view.setModel(&model);
    view.setSelectionMode(mode);
    view.setSelectionBehavior(behavior);

    QCOMPARE(view.selectionModel()->selectedIndexes().size(), 0);

    view.selectRow(row);

    //test we have 10 items selected
    QCOMPARE(view.selectionModel()->selectedIndexes().size(), selectedItems);
    //test that all 10 items are in the same row
    for (int i = 0; selectedItems > 0 && i < rowCount; ++i)
        QCOMPARE(view.selectionModel()->selectedIndexes().at(i).row(), row);
}

void tst_BOBUIableView::selectColumn_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<QAbstractItemView::SelectionMode>("mode");
    BOBUIest::addColumn<QAbstractItemView::SelectionBehavior>("behavior");
    BOBUIest::addColumn<int>("selectedItems");

        BOBUIest::newRow("SingleSelection and SelectItems")
            << 10 << 10
            << 0
            << QAbstractItemView::SingleSelection
            << QAbstractItemView::SelectItems
            << 0;

        BOBUIest::newRow("SingleSelection and SelectRows")
            << 10 << 10
            << 0
            << QAbstractItemView::SingleSelection
            << QAbstractItemView::SelectRows
            << 0;

        BOBUIest::newRow("SingleSelection and SelectColumns")
            << 10 << 10
            << 0
            << QAbstractItemView::SingleSelection
            << QAbstractItemView::SelectColumns
            << 10;

        BOBUIest::newRow("MultiSelection and SelectItems")
            << 10 << 10
            << 0
            << QAbstractItemView::MultiSelection
            << QAbstractItemView::SelectItems
            << 10;

        BOBUIest::newRow("MultiSelection and SelectRows")
            << 10 << 10
            << 0
            << QAbstractItemView::MultiSelection
            << QAbstractItemView::SelectRows
            << 0;

        BOBUIest::newRow("MultiSelection and SelectColumns")
            << 10 << 10
            << 0
            << QAbstractItemView::MultiSelection
            << QAbstractItemView::SelectColumns
            << 10;

        BOBUIest::newRow("ExtendedSelection and SelectItems")
            << 10 << 10
            << 0
            << QAbstractItemView::ExtendedSelection
            << QAbstractItemView::SelectItems
            << 10;

        BOBUIest::newRow("ExtendedSelection and SelectRows")
            << 10 << 10
            << 0
            << QAbstractItemView::ExtendedSelection
            << QAbstractItemView::SelectRows
            << 0;

        BOBUIest::newRow("ExtendedSelection and SelectColumns")
            << 10 << 10
            << 0
            << QAbstractItemView::ExtendedSelection
            << QAbstractItemView::SelectColumns
            << 10;

        BOBUIest::newRow("ContiguousSelection and SelectItems")
            << 10 << 10
            << 0
            << QAbstractItemView::ContiguousSelection
            << QAbstractItemView::SelectItems
            << 10;

        BOBUIest::newRow("ContiguousSelection and SelectRows")
            << 10 << 10
            << 0
            << QAbstractItemView::ContiguousSelection
            << QAbstractItemView::SelectRows
            << 0;

        BOBUIest::newRow("ContiguousSelection and SelectColumns")
            << 10 << 10
            << 0
            << QAbstractItemView::ContiguousSelection
            << QAbstractItemView::SelectColumns
            << 10;
}

void tst_BOBUIableView::selectColumn()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, column);
    QFETCH(QAbstractItemView::SelectionMode, mode);
    QFETCH(QAbstractItemView::SelectionBehavior, behavior);
    QFETCH(int, selectedItems);

    BobUITestTableModel model(rowCount, columnCount);

    BOBUIableView view;
    view.setModel(&model);
    view.setSelectionMode(mode);
    view.setSelectionBehavior(behavior);

    QCOMPARE(view.selectionModel()->selectedIndexes().size(), 0);

    view.selectColumn(column);

    QCOMPARE(view.selectionModel()->selectedIndexes().size(), selectedItems);
    for (int i = 0; selectedItems > 0 && i < columnCount; ++i)
        QCOMPARE(view.selectionModel()->selectedIndexes().at(i).column(), column);
}

#if BOBUI_CONFIG(shortcut)

void tst_BOBUIableView::selectall_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");
    BOBUIest::addColumn<int>("hideRow");
    BOBUIest::addColumn<int>("hideColumn");
    BOBUIest::addColumn<int>("moveRowFrom");
    BOBUIest::addColumn<int>("moveRowTo");
    BOBUIest::addColumn<int>("moveColumnFrom");
    BOBUIest::addColumn<int>("moveColumnTo");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");
    BOBUIest::addColumn<int>("selectedCount"); // ### make this more detailed

    BOBUIest::newRow("no span, no hidden, no moved")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 100;                              // selected count

    BOBUIest::newRow("row span, no hidden, no moved")
      << 10 << 10                          // dim
      << 1 << 1                            // pos
      << 2 << 1                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 99;                               // selected count

    BOBUIest::newRow("col span, no hidden, no moved")
      << 10 << 10                          // dim
      << 1 << 1                            // pos
      << 1 << 2                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 99;                               // selected count

    BOBUIest::newRow("no span, row hidden, no moved")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << 1 << -1                           // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 90;                               // selected count

    BOBUIest::newRow("no span, col hidden, no moved")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << 1                           // hide
      << -1 << -1                          // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 90;                               // selected count

    BOBUIest::newRow("no span, no hidden, row moved")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << -1                          // hide
      << 1 << 3                            // move row
      << -1 << -1                          // move col
      << 40 << 40                          // cell size
      << 100;                              // selected count

    BOBUIest::newRow("no span, no hidden, col moved")
      << 10 << 10                          // dim
      << -1 << -1                          // pos
      << 1 << 1                            // span
      << -1 << -1                          // hide
      << -1 << -1                          // move row
      << 1 << 3                            // move col
      << 40 << 40                          // cell size
      << 100;                              // selected count
}

void BOBUIest__keySequence(QWidget* widget, const QKeySequence &ks)
{
    for (int i = 0; i < ks.count(); ++i)
    {
        BobUI::Key key = ks[i].key();
        BobUI::KeyboardModifiers modifiers = ks[i].keyboardModifiers();
        BOBUIest::keyClick(widget, key, modifiers);
    }
}

void tst_BOBUIableView::selectall()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);
    QFETCH(int, hideRow);
    QFETCH(int, hideColumn);
    QFETCH(int, moveRowFrom);
    QFETCH(int, moveRowTo);
    QFETCH(int, moveColumnFrom);
    QFETCH(int, moveColumnTo);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    QFETCH(int, selectedCount);

    BobUITestTableModel model(rowCount, columnCount);

    BobUITestTableView view;
    view.show();
    view.setModel(&model);

    view.setSpan(row, column, rowSpan, columnSpan);

    view.hideRow(hideRow);
    view.hideColumn(hideColumn);

    view.verticalHeader()->moveSection(moveRowFrom, moveRowTo);
    view.horizontalHeader()->moveSection(moveColumnFrom, moveColumnTo);

    for (int r = 0; r < rowCount; ++r)
        view.setRowHeight(r, rowHeight);
    for (int c = 0; c < columnCount; ++c)
        view.setColumnWidth(c, columnWidth);

    // try slot first
    view.clearSelection();
    QCOMPARE(view.selectedIndexes().size(), 0);
    view.selectAll();
    QCOMPARE(view.selectedIndexes().size(), selectedCount);

    // try by key sequence
    view.clearSelection();
    QCOMPARE(view.selectedIndexes().size(), 0);
    BOBUIest__keySequence(&view, QKeySequence(QKeySequence::SelectAll));
    QCOMPARE(view.selectedIndexes().size(), selectedCount);

    // check again with no selection mode
    view.clearSelection();
    view.setSelectionMode(QAbstractItemView::NoSelection);
    QCOMPARE(view.selectedIndexes().size(), 0);
    BOBUIest__keySequence(&view, QKeySequence(QKeySequence::SelectAll));
    QCOMPARE(view.selectedIndexes().size(), 0);
}

#endif // BOBUI_CONFIG(shortcut)

void tst_BOBUIableView::visualRect_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("hideRow");
    BOBUIest::addColumn<int>("hideColumn");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");
    BOBUIest::addColumn<QRect>("expectedRect");

    BOBUIest::newRow("(0,0)")
        << 10 << 10
        << -1 << -1
        << 0 << 0
        << 20 << 30
        << QRect(0, 0, 29, 19);

    BOBUIest::newRow("(0,0) hidden row")
        << 10 << 10
        << 0 << -1
        << 0 << 0
        << 20 << 30
        << QRect();

    BOBUIest::newRow("(0,0) hidden column")
        << 10 << 10
        << -1 << 0
        << 0 << 0
        << 20 << 30
        << QRect();

    BOBUIest::newRow("(0,0) hidden row and column")
        << 10 << 10
        << 0 << 0
        << 0 << 0
        << 20 << 30
        << QRect();

    BOBUIest::newRow("(0,0) out of bounds")
        << 10 << 10
        << -1 << -1
        << 20 << 20
        << 20 << 30
        << QRect();

    BOBUIest::newRow("(5,5), hidden row")
        << 10 << 10
        << 5 << -1
        << 5 << 5
        << 20 << 30
        << QRect();

    BOBUIest::newRow("(9,9)")
        << 10 << 10
        << -1 << -1
        << 9 << 9
        << 20 << 30
        << QRect(30*9, 20*9, 29, 19);
}

void tst_BOBUIableView::visualRect()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, hideRow);
    QFETCH(int, hideColumn);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    QFETCH(QRect, expectedRect);

    BobUITestTableModel model(rowCount, columnCount);

    BOBUIableView view;
    view.setModel(&model);
    view.horizontalHeader()->setMinimumSectionSize(0);
    view.verticalHeader()->setMinimumSectionSize(0);
    // Make sure that it has 1 pixel between each cell.
    view.setGridStyle(BobUI::SolidLine);
    for (int i = 0; i < view.verticalHeader()->count(); ++i)
        view.verticalHeader()->resizeSection(i, rowHeight);
    for (int i = 0; i < view.horizontalHeader()->count(); ++i)
        view.horizontalHeader()->resizeSection(i, columnWidth);

    view.hideRow(hideRow);
    view.hideColumn(hideColumn);

    QRect rect = view.visualRect(model.index(row, column));
    QCOMPARE(rect, expectedRect);
}

void tst_BOBUIableView::fetchMore()
{
    BobUITestTableModel model(64, 64);

    model.can_fetch_more = true;

    BOBUIableView view;
    view.setModel(&model);
    view.show();

    QCOMPARE(model.fetch_more_count, 0);
    view.verticalScrollBar()->setValue(view.verticalScrollBar()->maximum());
    QVERIFY(model.fetch_more_count > 0);

    model.fetch_more_count = 0; //reset
    view.scrollToTop();
    QCOMPARE(model.fetch_more_count, 0);

    view.scrollToBottom();
    QVERIFY(model.fetch_more_count > 0);

    model.fetch_more_count = 0; //reset
    view.scrollToTop();
    view.setCurrentIndex(model.index(0, 0));
    QCOMPARE(model.fetch_more_count, 0);

    for (int i = 0; i < 64; ++i)
        BOBUIest::keyClick(&view, BobUI::Key_Down);
    QCOMPARE(view.currentIndex(), model.index(63, 0));
    QVERIFY(model.fetch_more_count > 0);
}

void tst_BOBUIableView::setHeaders()
{
    BOBUIableView view;

    // Make sure we don't delete ourselves
    view.setVerticalHeader(view.verticalHeader());
    view.verticalHeader()->count();
    view.setHorizontalHeader(view.horizontalHeader());
    view.horizontalHeader()->count();

    // Try passing around a header without it being deleted
    BOBUIableView view2;
    view2.setVerticalHeader(view.verticalHeader());
    view2.setHorizontalHeader(view.horizontalHeader());
    view.setHorizontalHeader(new QHeaderView(BobUI::Horizontal));
    view.setVerticalHeader(new QHeaderView(BobUI::Vertical));
    view2.verticalHeader()->count();
    view2.horizontalHeader()->count();

}

void tst_BOBUIableView::resizeRowsToContents_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<bool>("showGrid");
    BOBUIest::addColumn<int>("cellWidth");
    BOBUIest::addColumn<int>("cellHeight");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");

    BOBUIest::newRow("10x10 grid shown 40x40")
        << 10 << 10 << false << 40 << 40 << 40 << 40;

    BOBUIest::newRow("10x10 grid not shown 40x40")
        << 10 << 10 << true << 40 << 40 << 41 << 41;
}

void tst_BOBUIableView::resizeRowsToContents()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(bool, showGrid);
    QFETCH(int, cellWidth);
    QFETCH(int, cellHeight);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    Q_UNUSED(columnWidth);

    BobUITestTableModel model(rowCount, columnCount);
    BobUITestTableView view;
    BobUITestItemDelegate delegate;

    view.setModel(&model);
    view.setItemDelegate(&delegate);
    view.setShowGrid(showGrid); // the grid will add to the row height

    delegate.hint = QSize(cellWidth, cellHeight);

    QSignalSpy resizedSpy(view.verticalHeader(), &QHeaderView::sectionResized);
    view.resizeRowsToContents();

    QCOMPARE(resizedSpy.size(), model.rowCount());
    for (int r = 0; r < model.rowCount(); ++r)
        QCOMPARE(view.rowHeight(r), rowHeight);
}

void tst_BOBUIableView::resizeColumnsToContents_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<bool>("showGrid");
    BOBUIest::addColumn<int>("cellWidth");
    BOBUIest::addColumn<int>("cellHeight");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");

    BOBUIest::newRow("10x10 grid not shown 60x60")
        << 10 << 10 << false << 60 << 60 << 60 << 60;

    BOBUIest::newRow("10x10 grid shown 60x60")
        << 10 << 10 << true << 60 << 60 << 61 << 61;
}

void tst_BOBUIableView::resizeColumnsToContents()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(bool, showGrid);
    QFETCH(int, cellWidth);
    QFETCH(int, cellHeight);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    Q_UNUSED(rowHeight);

    BobUITestTableModel model(rowCount, columnCount);
    BobUITestTableView view;
    BobUITestItemDelegate delegate;

    view.setModel(&model);
    view.setItemDelegate(&delegate);
    view.setShowGrid(showGrid); // the grid will add to the row height

    delegate.hint = QSize(cellWidth, cellHeight);

    QSignalSpy resizedSpy(view.horizontalHeader(), &QHeaderView::sectionResized);
    view.resizeColumnsToContents();

    QCOMPARE(resizedSpy.size(), model.columnCount());
    for (int c = 0; c < model.columnCount(); ++c)
        QCOMPARE(view.columnWidth(c), columnWidth);
}

void tst_BOBUIableView::rowViewportPosition_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("verticalScrollMode");
    BOBUIest::addColumn<int>("verticalScrollValue");
    BOBUIest::addColumn<int>("rowViewportPosition");

    BOBUIest::newRow("row 0, scroll per item 0")
        << 100 << 40 << 0 << QAbstractItemView::ScrollPerItem << 0 << 0;

    BOBUIest::newRow("row 1, scroll per item, 0")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerItem << 0 << 1 * 40;

    BOBUIest::newRow("row 1, scroll per item, 1")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerItem << 1 << 0;

    BOBUIest::newRow("row 5, scroll per item, 0")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerItem << 0 << 5 * 40;

    BOBUIest::newRow("row 5, scroll per item, 5")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerItem << 5 << 0;

    BOBUIest::newRow("row 9, scroll per item, 0")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerItem << 0 << 9 * 40;

    BOBUIest::newRow("row 9, scroll per item, 5")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerItem << 5 << 4 * 40;

    BOBUIest::newRow("row 0, scroll per pixel 0")
        << 100 << 40 << 0 << QAbstractItemView::ScrollPerPixel << 0 << 0;

    BOBUIest::newRow("row 1, scroll per pixel, 0")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerPixel << 0 << 1 * 40;

    BOBUIest::newRow("row 1, scroll per pixel, 1")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerPixel << 1 * 40 << 0;

    BOBUIest::newRow("row 5, scroll per pixel, 0")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerPixel << 0 << 5 * 40;

    BOBUIest::newRow("row 5, scroll per pixel, 5")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerPixel << 5 * 40 << 0;

    BOBUIest::newRow("row 9, scroll per pixel, 0")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerPixel << 0 << 9 * 40;

    BOBUIest::newRow("row 9, scroll per pixel, 5")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerPixel << 5 * 40 << 4 * 40;
}

void tst_BOBUIableView::rowViewportPosition()
{
    QFETCH(int, rowCount);
    QFETCH(int, rowHeight);
    QFETCH(int, row);
    QFETCH(QAbstractItemView::ScrollMode, verticalScrollMode);
    QFETCH(int, verticalScrollValue);
    QFETCH(int, rowViewportPosition);

    BobUITestTableModel model(rowCount, 1);
    BobUITestTableView view;
    setFrameless(&view);
    view.resize(100, 2 * rowHeight);
    view.show();

    view.setModel(&model);
    for (int r = 0; r < rowCount; ++r)
        view.setRowHeight(r, rowHeight);

    view.setVerticalScrollMode(verticalScrollMode);
    view.verticalScrollBar()->setValue(verticalScrollValue);

    QCOMPARE(view.rowViewportPosition(row), rowViewportPosition);
}

void tst_BOBUIableView::rowAt_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<IntList>("hiddenRows");
    BOBUIest::addColumn<int>("coordinate");
    BOBUIest::addColumn<int>("row");

    BOBUIest::newRow("row at 100") << 5 << 40 << IntList() << 100 << 2;
    BOBUIest::newRow("row at 180") << 5 << 40 << IntList() << 180 << 4;
    BOBUIest::newRow("row at 20")  << 5 << 40 << IntList() <<  20 << 0;

    // ### expand the dataset to include hidden rows
}

void tst_BOBUIableView::rowAt()
{
    QFETCH(int, rowCount);
    QFETCH(int, rowHeight);
    QFETCH(IntList, hiddenRows);
    QFETCH(int, coordinate);
    QFETCH(int, row);

    BobUITestTableModel model(rowCount, 1);
    BobUITestTableView view;
    view.resize(100, 2 * rowHeight);

    view.setModel(&model);

    for (int r = 0; r < rowCount; ++r)
        view.setRowHeight(r, rowHeight);

    for (int i = 0; i < hiddenRows.size(); ++i)
        view.hideRow(hiddenRows.at(i));

    QCOMPARE(view.rowAt(coordinate), row);
}

void tst_BOBUIableView::rowHeight_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<IntList>("rowHeights");
    BOBUIest::addColumn<BoolList>("hiddenRows");

    BOBUIest::newRow("increasing")
      << 5
      << (IntList() << 20 << 30 << 40 << 50 << 60)
      << (BoolList() << false << false << false << false << false);

    BOBUIest::newRow("decreasing")
      << 5
      << (IntList() << 60 << 50 << 40 << 30 << 20)
      << (BoolList() << false << false << false << false << false);

    BOBUIest::newRow("random")
      << 5
      << (IntList() << 87 << 34 << 68 << 91 << 27)
      << (BoolList() << false << false << false << false << false);

    // ### expand the dataset to include hidden rows
}

void tst_BOBUIableView::rowHeight()
{
    QFETCH(int, rowCount);
    QFETCH(IntList, rowHeights);
    QFETCH(BoolList, hiddenRows);

    BobUITestTableModel model(rowCount, 1);
    BobUITestTableView view;

    view.setModel(&model);

    for (int r = 0; r < rowCount; ++r) {
        view.setRowHeight(r, rowHeights.at(r));
        view.setRowHidden(r, hiddenRows.at(r));
    }

    for (int r = 0; r < rowCount; ++r) {
        if (hiddenRows.at(r))
            QCOMPARE(view.rowHeight(r), 0);
        else
            QCOMPARE(view.rowHeight(r), rowHeights.at(r));
    }
}

void tst_BOBUIableView::columnViewportPosition_data()
{
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("columnWidth");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("horizontalScrollMode");
    BOBUIest::addColumn<int>("horizontalScrollValue");
    BOBUIest::addColumn<int>("columnViewportPosition");

    BOBUIest::newRow("column 0, scroll per item 0")
        << 100 << 40 << 0 << QAbstractItemView::ScrollPerItem << 0 << 0;

    BOBUIest::newRow("column 1, scroll per item, 0")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerItem << 0 << 1 * 40;

    BOBUIest::newRow("column 1, scroll per item, 1")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerItem << 1 << 0;

    BOBUIest::newRow("column 5, scroll per item, 0")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerItem << 0 << 5 * 40;

    BOBUIest::newRow("column 5, scroll per item, 5")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerItem << 5 << 0;

    BOBUIest::newRow("column 9, scroll per item, 0")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerItem << 0 << 9 * 40;

    BOBUIest::newRow("column 9, scroll per item, 5")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerItem << 5 << 4 * 40;

    BOBUIest::newRow("column 0, scroll per pixel 0")
        << 100 << 40 << 0 << QAbstractItemView::ScrollPerPixel << 0 << 0;

    BOBUIest::newRow("column 1, scroll per pixel 0")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerPixel << 0 << 1 * 40;

    BOBUIest::newRow("column 1, scroll per pixel 1")
        << 100 << 40 << 1 << QAbstractItemView::ScrollPerPixel << 1 * 40 << 0;

    BOBUIest::newRow("column 5, scroll per pixel 0")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerPixel << 0 << 5 * 40;

    BOBUIest::newRow("column 5, scroll per pixel 5")
        << 100 << 40 << 5 << QAbstractItemView::ScrollPerPixel << 5 * 40 << 0;

    BOBUIest::newRow("column 9, scroll per pixel 0")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerPixel << 0 << 9 * 40;

    BOBUIest::newRow("column 9, scroll per pixel 5")
        << 100 << 40 << 9 << QAbstractItemView::ScrollPerPixel << 5 * 40 << 4 * 40;
}

void tst_BOBUIableView::columnViewportPosition()
{
    QFETCH(int, columnCount);
    QFETCH(int, columnWidth);
    QFETCH(int, column);
    QFETCH(QAbstractItemView::ScrollMode, horizontalScrollMode);
    QFETCH(int, horizontalScrollValue);
    QFETCH(int, columnViewportPosition);

    BobUITestTableModel model(1, columnCount);
    BobUITestTableView view;
    setFrameless(&view);
    view.resize(2 * columnWidth, 100);
    view.show();

    view.setModel(&model);
    for (int c = 0; c < columnCount; ++c)
        view.setColumnWidth(c, columnWidth);

    view.setHorizontalScrollMode(horizontalScrollMode);
    view.horizontalScrollBar()->setValue(horizontalScrollValue);

    QCOMPARE(view.columnViewportPosition(column), columnViewportPosition);
}

void tst_BOBUIableView::columnAt_data()
{
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("columnWidth");
    BOBUIest::addColumn<IntList>("hiddenColumns");
    BOBUIest::addColumn<int>("coordinate");
    BOBUIest::addColumn<int>("column");

    BOBUIest::newRow("column at 100") << 5 << 40 << IntList() << 100 << 2;
    BOBUIest::newRow("column at 180") << 5 << 40 << IntList() << 180 << 4;
    BOBUIest::newRow("column at 20")  << 5 << 40 << IntList() <<  20 << 0;

    // ### expand the dataset to include hidden coumns
}

void tst_BOBUIableView::columnAt()
{
    QFETCH(int, columnCount);
    QFETCH(int, columnWidth);
    QFETCH(IntList, hiddenColumns);
    QFETCH(int, coordinate);
    QFETCH(int, column);

    BobUITestTableModel model(1, columnCount);
    BobUITestTableView view;
    view.resize(2 * columnWidth, 100);

    view.setModel(&model);

    for (int c = 0; c < columnCount; ++c)
        view.setColumnWidth(c, columnWidth);

    for (int i = 0; i < hiddenColumns.size(); ++i)
        view.hideColumn(hiddenColumns.at(i));

    QCOMPARE(view.columnAt(coordinate), column);
}

void tst_BOBUIableView::columnWidth_data()
{
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<IntList>("columnWidths");
    BOBUIest::addColumn<BoolList>("hiddenColumns");

    BOBUIest::newRow("increasing")
      << 5
      << (IntList() << 20 << 30 << 40 << 50 << 60)
      << (BoolList() << false << false << false << false << false);

    BOBUIest::newRow("decreasing")
      << 5
      << (IntList() << 60 << 50 << 40 << 30 << 20)
      << (BoolList() << false << false << false << false << false);

    BOBUIest::newRow("random")
      << 5
      << (IntList() << 87 << 34 << 68 << 91 << 27)
      << (BoolList() << false << false << false << false << false);

    // ### expand the dataset to include hidden columns
}

void tst_BOBUIableView::columnWidth()
{
    QFETCH(int, columnCount);
    QFETCH(IntList, columnWidths);
    QFETCH(BoolList, hiddenColumns);

    BobUITestTableModel model(1, columnCount);
    BobUITestTableView view;

    view.setModel(&model);

    for (int c = 0; c < columnCount; ++c) {
        view.setColumnWidth(c, columnWidths.at(c));
        view.setColumnHidden(c, hiddenColumns.at(c));
    }

    for (int c = 0; c < columnCount; ++c) {
        if (hiddenColumns.at(c))
            QCOMPARE(view.columnWidth(c), 0);
        else
            QCOMPARE(view.columnWidth(c), columnWidths.at(c));
    }
}

void tst_BOBUIableView::hiddenRow_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<BoolList>("hiddenRows");

    BOBUIest::newRow("first hidden")
      << 5 << (BoolList() << true << false << false << false << false);

    BOBUIest::newRow("last hidden")
      << 5 << (BoolList() << false << false << false << false << true);

    BOBUIest::newRow("none hidden")
      << 5 << (BoolList() << false << false << false << false << false);

    BOBUIest::newRow("all hidden")
      << 5 << (BoolList() << true << true << true << true << true);
 }

void tst_BOBUIableView::hiddenRow()
{
    QFETCH(int, rowCount);
    QFETCH(BoolList, hiddenRows);


    BobUITestTableModel model(rowCount, 1);
    BobUITestTableView view;

    view.setModel(&model);

    for (int r = 0; r < rowCount; ++r)
        QVERIFY(!view.isRowHidden(r));

    for (int r = 0; r < rowCount; ++r)
        view.setRowHidden(r, hiddenRows.at(r));

    for (int r = 0; r < rowCount; ++r)
        QCOMPARE(view.isRowHidden(r), hiddenRows.at(r));

    for (int r = 0; r < rowCount; ++r)
        view.setRowHidden(r, false);

    for (int r = 0; r < rowCount; ++r)
        QVERIFY(!view.isRowHidden(r));
}

void tst_BOBUIableView::hiddenColumn_data()
{
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<BoolList>("hiddenColumns");

    BOBUIest::newRow("first hidden")
      << 5 << (BoolList() << true << false << false << false << false);

    BOBUIest::newRow("last hidden")
      << 5 << (BoolList() << false << false << false << false << true);

    BOBUIest::newRow("none hidden")
      << 5 << (BoolList() << false << false << false << false << false);

    BOBUIest::newRow("all hidden")
      << 5 << (BoolList() << true << true << true << true << true);
}

void tst_BOBUIableView::hiddenColumn()
{
    QFETCH(int, columnCount);
    QFETCH(BoolList, hiddenColumns);

    BobUITestTableModel model(1, columnCount);
    BobUITestTableView view;

    view.setModel(&model);

    for (int c = 0; c < columnCount; ++c)
        QVERIFY(!view.isColumnHidden(c));

    for (int c = 0; c < columnCount; ++c)
        view.setColumnHidden(c, hiddenColumns.at(c));

    for (int c = 0; c < columnCount; ++c)
        QCOMPARE(view.isColumnHidden(c), hiddenColumns.at(c));

    for (int c = 0; c < columnCount; ++c)
        view.setColumnHidden(c, false);

    for (int c = 0; c < columnCount; ++c)
        QVERIFY(!view.isColumnHidden(c));
}

void tst_BOBUIableView::sortingEnabled_data()
{
//    BOBUIest::addColumn<int>("columnCount");
}

void tst_BOBUIableView::sortingEnabled()
{
//    QFETCH(int, columnCount);
}

void tst_BOBUIableView::sortByColumn_data()
{
    BOBUIest::addColumn<bool>("sortingEnabled");
    BOBUIest::newRow("sorting enabled") << true;
    BOBUIest::newRow("sorting disabled") << false;
}

// Checks sorting and that sortByColumn also sets the sortIndicator
void tst_BOBUIableView::sortByColumn()
{
    QFETCH(bool, sortingEnabled);
    BOBUIableView view;
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
    view.show();

    view.sortByColumn(1, BobUI::DescendingOrder);
    QCOMPARE(view.horizontalHeader()->sortIndicatorSection(), 1);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("c"));
    QCOMPARE(view.model()->data(view.model()->index(1, 0)).toString(), QString::fromLatin1("d"));
    QCOMPARE(view.model()->data(view.model()->index(0, 1)).toString(), QString::fromLatin1("h"));
    QCOMPARE(view.model()->data(view.model()->index(1, 1)).toString(), QString::fromLatin1("g"));

    view.sortByColumn(0, BobUI::AscendingOrder);
    QCOMPARE(view.horizontalHeader()->sortIndicatorSection(), 0);
    QCOMPARE(view.model()->data(view.model()->index(0, 0)).toString(), QString::fromLatin1("a"));
    QCOMPARE(view.model()->data(view.model()->index(1, 0)).toString(), QString::fromLatin1("b"));
    QCOMPARE(view.model()->data(view.model()->index(0, 1)).toString(), QString::fromLatin1("f"));
    QCOMPARE(view.model()->data(view.model()->index(1, 1)).toString(), QString::fromLatin1("e"));

    view.sortByColumn(-1, BobUI::AscendingOrder);
    QCOMPARE(view.horizontalHeader()->sortIndicatorSection(), -1);
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

void tst_BOBUIableView::scrollTo_data()
{
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("verticalScrollMode");
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("horizontalScrollMode");
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");
    BOBUIest::addColumn<int>("hiddenRow");
    BOBUIest::addColumn<int>("hiddenColumn");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");
    BOBUIest::addColumn<int>("horizontalScroll");
    BOBUIest::addColumn<int>("verticalScroll");
    BOBUIest::addColumn<QAbstractItemView::ScrollHint>("scrollHint");
    BOBUIest::addColumn<int>("expectedHorizontalScroll");
    BOBUIest::addColumn<int>("expectedVerticalScroll");

    BOBUIest::newRow("no hidden, no span, no scroll, per item")
        << QAbstractItemView::ScrollPerItem
        << QAbstractItemView::ScrollPerItem
        << 10 << 10  // table
        << 80 << 80  // size
        << -1 << -1  // hide
        << 0 << 0    // cell
        << 1 << 1    // span
        << 0 << 0    // scroll
        << QAbstractItemView::PositionAtTop
        << 0 << 0;   // expected

    BOBUIest::newRow("no hidden, no span, no scroll, per pixel")
        << QAbstractItemView::ScrollPerPixel
        << QAbstractItemView::ScrollPerPixel
        << 10 << 10  // table
        << 80 << 80  // size
        << -1 << -1  // hide
        << 0 << 0    // cell
        << 1 << 1    // span
        << 0 << 0    // scroll
        << QAbstractItemView::PositionAtTop
        << 0 << 0;   // expected

    BOBUIest::newRow("hidden, no span, no scroll, per item")
        << QAbstractItemView::ScrollPerItem
        << QAbstractItemView::ScrollPerItem
        << 10 << 10  // table
        << 80 << 80  // size
        << 3 << 3    // hide
        << 5 << 5    // cell
        << 1 << 1    // span
        << 0 << 0    // scroll
        << QAbstractItemView::PositionAtTop
        << 4 << 4;   // expected
}

void tst_BOBUIableView::scrollTo()
{
    QFETCH(QAbstractItemView::ScrollMode, horizontalScrollMode);
    QFETCH(QAbstractItemView::ScrollMode, verticalScrollMode);
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    QFETCH(int, hiddenRow);
    QFETCH(int, hiddenColumn);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);
    QFETCH(int, horizontalScroll);
    QFETCH(int, verticalScroll);
    QFETCH(QAbstractItemView::ScrollHint, scrollHint);
    QFETCH(int, expectedHorizontalScroll);
    QFETCH(int, expectedVerticalScroll);

    BobUITestTableModel model(rowCount, columnCount);
    QWidget toplevel;
    setFrameless(&toplevel);
    BobUITestTableView view(&toplevel);

    toplevel.show();
    // resizing to this size will ensure that there can ONLY_BE_ONE_CELL inside the view.
    QSize forcedSize(columnWidth * 2, rowHeight * 2);
    view.resize(forcedSize);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&toplevel));
    BOBUIRY_COMPARE(view.size(), forcedSize);

    view.setModel(&model);
    view.setSpan(row, column, rowSpan, columnSpan);
    view.hideRow(hiddenRow);
    view.hideColumn(hiddenColumn);
    view.setHorizontalScrollMode(horizontalScrollMode);
    view.setVerticalScrollMode(verticalScrollMode);

    for (int r = 0; r < rowCount; ++r)
        view.setRowHeight(r, rowHeight);
    for (int c = 0; c < columnCount; ++c)
        view.setColumnWidth(c, columnWidth);

    view.horizontalScrollBar()->setValue(horizontalScroll);
    view.verticalScrollBar()->setValue(verticalScroll);

    QModelIndex index = model.index(row, column);
    QVERIFY(index.isValid());
    view.scrollTo(index, scrollHint);
    BOBUIRY_COMPARE(view.verticalScrollBar()->value(), expectedVerticalScroll);
    BOBUIRY_COMPARE(view.horizontalScrollBar()->value(), expectedHorizontalScroll);
}

void tst_BOBUIableView::indexAt_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");

    BOBUIest::addColumn<int>("rowHeight");
    BOBUIest::addColumn<int>("columnWidth");

    BOBUIest::addColumn<int>("hiddenRow");
    BOBUIest::addColumn<int>("hiddenColumn");

    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");
    BOBUIest::addColumn<int>("horizontalScroll");
    BOBUIest::addColumn<int>("verticalScroll");
    BOBUIest::addColumn<int>("x");
    BOBUIest::addColumn<int>("y");
    BOBUIest::addColumn<int>("expectedRow");
    BOBUIest::addColumn<int>("expectedColumn");

    BOBUIest::newRow("no hidden, no span, no scroll, (20,20)")
      << 10 << 10  // dim
      << 40 << 40  // size
      << -1 << -1  // hide
      << -1 << -1  // pos
      << 1 << 1    // span
      << 0 << 0    // scroll
      << 20 << 20  // point
      << 0 << 0;   // expected

    BOBUIest::newRow("row hidden, no span, no scroll, at (20,20)")
      << 10 << 10  // dim
      << 40 << 40  // size
      << 0 << -1   // hide
      << -1 << -1  // pos
      << 1 << 1    // span
      << 0 << 0    // scroll
      << 20 << 20  // point
      << 1 << 0;   // expected

    BOBUIest::newRow("col hidden, no span, no scroll, at (20,20)")
      << 10 << 10  // dim
      << 40 << 40  // size
      << -1 << 0   // hide
      << -1 << -1  // pos
      << 1 << 1    // span
      << 0 << 0    // scroll
      << 20 << 20  // point
      << 0 << 1;   // expected

    BOBUIest::newRow("no hidden, row span, no scroll, at (60,20)")
      << 10 << 10  // dim
      << 40 << 40  // size
      << -1 << -1  // hide
      << 0 << 0    // pos
      << 2 << 1    // span
      << 0 << 0    // scroll
      << 20 << 60  // point
      << 0 << 0;   // expected


    BOBUIest::newRow("no hidden, col span, no scroll, at (60,20)")
      << 10 << 10  // dim
      << 40 << 40  // size
      << -1 << -1  // hide
      << 0 << 0    // pos
      << 1 << 2    // span
      << 0 << 0    // scroll
      << 60 << 20  // point
      << 0 << 0;   // expected

    BOBUIest::newRow("no hidden, no span, scroll (5,0), at (20,20)")
      << 20 << 20  // dim
      << 40 << 40  // size
      << -1 << -1  // hide
      << -1 << -1  // pos
      << 1 << 1    // span
      << 5 << 0    // scroll
      << 20 << 20  // point
      << 0 << 5;   // expected

    BOBUIest::newRow("no hidden, no span, scroll (0,5), at (20,20)")
      << 20 << 20  // dim
      << 40 << 40  // size
      << -1 << -1  // hide
      << -1 << -1  // pos
      << 1 << 1    // span
      << 0 << 5    // scroll
      << 20 << 20  // point
      << 5 << 0;   // expected

    BOBUIest::newRow("no hidden, no span, scroll (5,5), at (20,20)")
      << 20 << 20  // dim
      << 40 << 40  // size
      << -1 << -1  // hide
      << -1 << -1  // pos
      << 1 << 1    // span
      << 5 << 5    // scroll
      << 20 << 20  // point
      << 5 << 5;   // expected
}

void tst_BOBUIableView::indexAt()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, rowHeight);
    QFETCH(int, columnWidth);
    QFETCH(int, hiddenRow);
    QFETCH(int, hiddenColumn);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);
    QFETCH(int, horizontalScroll);
    QFETCH(int, verticalScroll);
    QFETCH(int, x);
    QFETCH(int, y);
    QFETCH(int, expectedRow);
    QFETCH(int, expectedColumn);

    BobUITestTableModel model(rowCount, columnCount);
    QWidget toplevel;
    BobUITestTableView view(&toplevel);

    toplevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&toplevel));

    //some styles change the scroll mode in their polish
    view.setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    view.setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    view.setModel(&model);
    view.setSpan(row, column, rowSpan, columnSpan);
    view.hideRow(hiddenRow);
    view.hideColumn(hiddenColumn);

    for (int r = 0; r < rowCount; ++r)
        view.setRowHeight(r, rowHeight);
    for (int c = 0; c < columnCount; ++c)
        view.setColumnWidth(c, columnWidth);

    view.horizontalScrollBar()->setValue(horizontalScroll);
    view.verticalScrollBar()->setValue(verticalScroll);

    QModelIndex index = view.indexAt(QPoint(x, y));
    BOBUIRY_COMPARE(index.row(), expectedRow);
    BOBUIRY_COMPARE(index.column(), expectedColumn);
}

void tst_BOBUIableView::span_data()
{
    BOBUIest::addColumn<int>("rowCount");
    BOBUIest::addColumn<int>("columnCount");
    BOBUIest::addColumn<int>("hiddenRow");
    BOBUIest::addColumn<int>("hiddenColumn");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");
    BOBUIest::addColumn<int>("expectedRowSpan");
    BOBUIest::addColumn<int>("expectedColumnSpan");
    BOBUIest::addColumn<bool>("clear");

    BOBUIest::newRow("top left 2x2")
      << 10 << 10
      << -1 << -1
      << 0 << 0
      << 2 << 2
      << 2 << 2
      << false;

    BOBUIest::newRow("top left 1x2")
      << 10 << 10
      << 3 << 3
      << 0 << 0
      << 1 << 2
      << 1 << 2
      << false;

    BOBUIest::newRow("top left 2x1")
      << 10 << 10
      << -1 << -1
      << 0 << 0
      << 2 << 1
      << 2 << 1
      << false;

  /* This makes no sens.
    BOBUIest::newRow("top left 2x0")
      << 10 << 10
      << -1 << -1
      << 0 << 0
      << 2 << 0
      << 2 << 0
      << false;

    BOBUIest::newRow("top left 0x2")
      << 10 << 10
      << -1 << -1
      << 0 << 0
      << 0 << 2
      << 0 << 2
      << false;*/

    BOBUIest::newRow("invalid 2x2")
      << 10 << 10
      << -1 << -1
      << -1 << -1
      << 2 << 2
      << 1 << 1
      << false;

    BOBUIest::newRow("top left 2x2")
      << 10 << 10
      << -1 << -1
      << 0 << 0
      << 2 << 2
      << 2 << 2
      << false;

    BOBUIest::newRow("bottom right 2x2")
      << 10 << 10
      << -1 << -1
      << 8 << 8
      << 2 << 2
      << 2 << 2
      << false;

    BOBUIest::newRow("invalid span 2x2")
      << 10 << 10
      << -1 << -1
      << 8 << 8
      << 2 << 2
      << 2 << 2
      << false;

    BOBUIest::newRow("invalid span 3x3")
      << 10 << 10
      << -1 << -1
      << 6 << 6
      << 3 << 3
      << 2 << 3
      << true;

}

void tst_BOBUIableView::span()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, hiddenRow);
    QFETCH(int, hiddenColumn);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);
    QFETCH(int, expectedRowSpan);
    QFETCH(int, expectedColumnSpan);
    QFETCH(bool, clear);

    BobUITestTableModel model(rowCount, columnCount);
    BobUITestTableView view;

    view.setModel(&model);
    view.show();

    view.setSpan(row, column, rowSpan, columnSpan);
    if (clear) {
        model.removeLastRow();
        model.removeLastRow();
        view.update();
    }

    view.hideRow(hiddenRow);
    view.hideColumn(hiddenColumn);
    view.show();

    QCOMPARE(view.rowSpan(row, column), expectedRowSpan);
    QCOMPARE(view.columnSpan(row, column), expectedColumnSpan);

    if (hiddenRow > -1) {
        QModelIndex hidden = model.index(hiddenRow, columnCount - 1);
        QVERIFY(view.isIndexHidden(hidden));
    }

    if (hiddenColumn > -1) {
        QModelIndex hidden = model.index(rowCount - 1, hiddenColumn);
        QVERIFY(view.isIndexHidden(hidden));
    }

    view.clearSpans();
    QCOMPARE(view.rowSpan(row, column), 1);
    QCOMPARE(view.columnSpan(row, column), 1);

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::spans_data()
{
    BOBUIest::addColumn<int>("rows");
    BOBUIest::addColumn<int>("columns");
    BOBUIest::addColumn<SpanList>("spans");
    BOBUIest::addColumn<bool>("hideRowLastRowOfFirstSpan");
    BOBUIest::addColumn<QPoint>("pos");
    BOBUIest::addColumn<int>("expectedRowSpan");
    BOBUIest::addColumn<int>("expectedColumnSpan");

    BOBUIest::newRow("1x3 span, query 3,0")
      << 5 << 5
      << (SpanList() << QRect(3, 0, 1, 3))
      << false //no hidden row
      << QPoint(3, 0)
      << 1
      << 3;

    BOBUIest::newRow("1x3 span, query 3,1")
      << 5 << 5
      << (SpanList() << QRect(3, 0, 1, 3))
      << false //no hidden row
      << QPoint(3, 1)
      << 1
      << 3;

    BOBUIest::newRow("1x3 span, query 3,2")
      << 5 << 5
      << (SpanList() << QRect(3, 0, 1, 3))
      << false //no hidden row
      << QPoint(3, 2)
      << 1
      << 3;

    BOBUIest::newRow("two 1x2 spans at the same column, query at 3,0")
      << 5 << 5
      << (SpanList() << QRect(3, 0, 1, 2) << QRect(4, 0, 1, 2))
      << false //no hidden row
      << QPoint(3, 0)
      << 1
      << 2;

    BOBUIest::newRow("two 1x2 spans at the same column, query at 4,0")
      << 5 << 5
      << (SpanList() << QRect(3, 0, 1, 2) << QRect(4, 0, 1, 2))
      << false //no hidden row
      << QPoint(4, 0)
      << 1
      << 2;

    BOBUIest::newRow("how to order spans (1,1)")
      << 5 << 5
      << (SpanList() << QRect(1, 1, 3, 1) << QRect(1, 2, 2, 1))
      << false //no hidden row
      << QPoint(1, 1)
      << 3
      << 1;

    BOBUIest::newRow("how to order spans (2,1)")
      << 5 << 5
      << (SpanList() << QRect(1, 1, 3, 1) << QRect(1, 2, 2, 1))
      << false //no hidden row
      << QPoint(2, 1)
      << 3
      << 1;

    BOBUIest::newRow("how to order spans (3,1)")
      << 5 << 5
      << (SpanList() << QRect(1, 1, 3, 1) << QRect(1, 2, 2, 1))
      << false //no hidden row
      << QPoint(3, 1)
      << 3
      << 1;

    BOBUIest::newRow("how to order spans (1,2)")
      << 5 << 5
      << (SpanList() << QRect(1, 1, 3, 1) << QRect(1, 2, 2, 1))
      << false //no hidden row
      << QPoint(1, 2)
      << 2
      << 1;

    BOBUIest::newRow("how to order spans (2,2)")
      << 5 << 5
      << (SpanList() << QRect(1, 1, 3, 1) << QRect(1, 2, 2, 1))
      << false //no hidden row
      << QPoint(2, 2)
      << 2
      << 1;

    BOBUIest::newRow("spans with hidden rows")
      << 3 << 2
      << (SpanList() << QRect(0, 0, 2, 2) << QRect(2, 0, 1, 2))
      << true //we hide the last row of the first span
      << QPoint(2, 0)
      << 1
      << 2;

    BOBUIest::newRow("BOBUIBUG-6004: No failing assertion, then it passes.")
      << 5 << 5
      << (SpanList() << QRect(0, 0, 2, 2) << QRect(0, 0, 1, 1))
      << false
      << QPoint(0, 0)
      << 1
      << 1;

    BOBUIest::newRow("BOBUIBUG-6004 (follow-up): No failing assertion, then it passes.")
      << 10 << 10
      << (SpanList() << QRect(2, 2, 1, 3) << QRect(2, 2, 1, 1))
      << false
      << QPoint(0, 0)
      << 1
      << 1;

    BOBUIest::newRow("BOBUIBUG-9631: remove one span")
      << 10 << 10
      << (SpanList() << QRect(1, 1, 2, 1) << QRect(2, 2, 2, 2) << QRect(1, 1, 1, 1))
      << false
      << QPoint(1, 1)
      << 1
      << 1;
}

void tst_BOBUIableView::spans()
{
    QFETCH(int, rows);
    QFETCH(int, columns);
    QFETCH(const SpanList, spans);
    QFETCH(bool, hideRowLastRowOfFirstSpan);
    QFETCH(QPoint, pos);
    QFETCH(int, expectedRowSpan);
    QFETCH(int, expectedColumnSpan);

    BobUITestTableModel model(rows, columns);
    BobUITestTableView view;

    view.setModel(&model);
    view.show();

    for (const auto &sp : spans)
        view.setSpan(sp.x(), sp.y(), sp.width(), sp.height());

    if (hideRowLastRowOfFirstSpan) {
        view.setRowHidden(spans.at(0).bottom(), true);
        //we check that the span didn't break the visual rects of the model indexes
        QRect first = view.visualRect( model.index(spans.at(0).top(), 0));
        QRect next = view.visualRect( model.index(spans.at(0).bottom() + 1, 0));
        QVERIFY(first.intersected(next).isEmpty());
    }

    QCOMPARE(view.columnSpan(pos.x(), pos.y()), expectedColumnSpan);
    QCOMPARE(view.rowSpan(pos.x(), pos.y()), expectedRowSpan);

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::spansAfterRowInsertion()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BobUITestTableModel model(10, 10);
    BobUITestTableView view;
    view.setModel(&model);
    view.setSpan(3, 3, 3, 3);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    // Insertion before the span only shifts the span.
    view.model()->insertRows(0, 2);
    QCOMPARE(view.rowSpan(3, 3), 1);
    QCOMPARE(view.columnSpan(3, 3), 1);
    QCOMPARE(view.rowSpan(5, 3), 3);
    QCOMPARE(view.columnSpan(5, 3), 3);

    // Insertion happens before the given row, so it only shifts the span also.
    view.model()->insertRows(5, 2);
    QCOMPARE(view.rowSpan(5, 3), 1);
    QCOMPARE(view.columnSpan(5, 3), 1);
    QCOMPARE(view.rowSpan(7, 3), 3);
    QCOMPARE(view.columnSpan(7, 3), 3);

    // Insertion inside the span expands it.
    view.model()->insertRows(8, 2);
    QCOMPARE(view.rowSpan(7, 3), 5);
    QCOMPARE(view.columnSpan(7, 3), 3);

    // Insertion after the span does nothing to it.
    view.model()->insertRows(12, 2);
    QCOMPARE(view.rowSpan(7, 3), 5);
    QCOMPARE(view.columnSpan(7, 3), 3);

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::spansAfterColumnInsertion()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BobUITestTableModel model(10, 10);
    BobUITestTableView view;
    view.setModel(&model);
    view.setSpan(3, 3, 3, 3);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    // Insertion before the span only shifts the span.
    view.model()->insertColumns(0, 2);
    QCOMPARE(view.rowSpan(3, 3), 1);
    QCOMPARE(view.columnSpan(3, 3), 1);
    QCOMPARE(view.rowSpan(3, 5), 3);
    QCOMPARE(view.columnSpan(3, 5), 3);

    // Insertion happens before the given column, so it only shifts the span also.
    view.model()->insertColumns(5, 2);
    QCOMPARE(view.rowSpan(3, 5), 1);
    QCOMPARE(view.columnSpan(3, 5), 1);
    QCOMPARE(view.rowSpan(3, 7), 3);
    QCOMPARE(view.columnSpan(3, 7), 3);

    // Insertion inside the span expands it.
    view.model()->insertColumns(8, 2);
    QCOMPARE(view.rowSpan(3, 7), 3);
    QCOMPARE(view.columnSpan(3, 7), 5);

    // Insertion after the span does nothing to it.
    view.model()->insertColumns(12, 2);
    QCOMPARE(view.rowSpan(3, 7), 3);
    QCOMPARE(view.columnSpan(3, 7), 5);

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::spansAfterRowRemoval()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BobUITestTableModel model(10, 10);
    BobUITestTableView view;
    view.setModel(&model);

    static const QRect spans[] = {
        {0, 1, 1, 2},
        {1, 2, 1, 2},
        {2, 2, 1, 5},
        {2, 8, 1, 2},
        {3, 4, 1, 2},
        {4, 4, 1, 4},
        {5, 6, 1, 3},
        {6, 7, 1, 3}
    };
    for (const QRect &span : spans)
        view.setSpan(span.top(), span.left(), span.height(), span.width());

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));
    view.model()->removeRows(3, 3);

    static const QRect expectedSpans[] = {
        {0, 1, 1, 2},
        {1, 2, 1, 1},
        {2, 2, 1, 2},
        {2, 5, 1, 2},
        {3, 4, 1, 1},
        {4, 3, 1, 2},
        {5, 3, 1, 3},
        {6, 4, 1, 3}
    };
    for (const QRect &span : expectedSpans) {
        QCOMPARE(view.columnSpan(span.top(), span.left()), span.width());
        QCOMPARE(view.rowSpan(span.top(), span.left()), span.height());
    }

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::spansAfterColumnRemoval()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BobUITestTableModel model(10, 10);
    BobUITestTableView view;
    view.setModel(&model);

    // Same set as above just swapping columns and rows.
    static const QRect spans[] = {
        {0, 1, 1, 2},
        {1, 2, 1, 2},
        {2, 2, 1, 5},
        {2, 8, 1, 2},
        {3, 4, 1, 2},
        {4, 4, 1, 4},
        {5, 6, 1, 3},
        {6, 7, 1, 3}
    };
    for (const QRect &span : spans)
      view.setSpan(span.left(), span.top(), span.width(), span.height());

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));
    view.model()->removeColumns(3, 3);

    static const QRect expectedSpans[] = {
        {0, 1, 1, 2},
        {1, 2, 1, 1},
        {2, 2, 1, 2},
        {2, 5, 1, 2},
        {3, 4, 1, 1},
        {4, 3, 1, 2},
        {5, 3, 1, 3},
        {6, 4, 1, 3}
    };
    for (const QRect &span : expectedSpans) {
        QCOMPARE(view.columnSpan(span.left(), span.top()), span.height());
        QCOMPARE(view.rowSpan(span.left(), span.top()), span.width());
    }

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::spanWithMovedSections_data()
{
    BOBUIest::addColumn<int>("rowSectionFrom");
    BOBUIest::addColumn<int>("rowSectionTo");
    BOBUIest::addColumn<int>("columnSectionFrom");
    BOBUIest::addColumn<int>("columnSectionTo");

    BOBUIest::newRow("noSectionMove") << -1 << -1 << -1 << -1;
    BOBUIest::newRow("horizontalSectionMove") << 4 << 2 << -1 << -1;
    BOBUIest::newRow("verticalSectionMove") << -1 << -1 << 4 << 2;
    BOBUIest::newRow("horizontalAndVerticallSectionMove") << 4 << 2 << 4 << 2;
}

void tst_BOBUIableView::spanWithMovedSections()
{
    QFETCH(int, rowSectionFrom);
    QFETCH(int, rowSectionTo);
    QFETCH(int, columnSectionFrom);
    QFETCH(int, columnSectionTo);

    BobUITestTableModel model(10, 10);
    BobUITestTableView view;
    view.setModel(&model);
    view.setSpan(0, 0, 2, 2);

    if (columnSectionFrom >= 0 && columnSectionTo >= 0)
        view.horizontalHeader()->moveSection(columnSectionFrom, columnSectionTo);
    if (rowSectionFrom >= 0 && rowSectionTo >= 0)
        view.verticalHeader()->moveSection(rowSectionFrom, rowSectionTo);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    auto sm = view.selectionModel();

    const auto nonSelIdx = model.index(rowSectionFrom < 0 ? 0 : rowSectionFrom,
                                       columnSectionFrom < 0 ? 0 : columnSectionFrom);
    const auto selIdx = model.index(rowSectionTo < 0 ? 0 : rowSectionTo,
                                    columnSectionTo < 0 ? 0 : columnSectionTo);
    const auto selRect = view.visualRect(selIdx);
    view.setSelection(selRect, QItemSelectionModel::ClearAndSelect);
    QVERIFY(sm->isSelected(selIdx));
    if (selIdx != nonSelIdx)
        QVERIFY(!sm->isSelected(nonSelIdx));
}

void tst_BOBUIableView::editSpanFromDirections_data()
{
    BOBUIest::addColumn<KeyList>("keyPresses");
    BOBUIest::addColumn<QSharedPointer<QStandardItemModel>>("model");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("rowSpan");
    BOBUIest::addColumn<int>("columnSpan");
    BOBUIest::addColumn<QModelIndex>("expectedVisualCursorIndex");
    BOBUIest::addColumn<QModelIndex>("expectedEditedIndex");

    /* x = the cell that should be edited
       c = the cell that should actually be the current index
       +---+---+
       |   |   |
       +---+---+
       |   | x |
       +---+   +
       |   | c |
       +---+---+
       |   | ^ |
       +---+---+ */
    KeyList keyPresses {BobUI::Key_Right, BobUI::Key_PageDown, BobUI::Key_Up};
    QSharedPointer<QStandardItemModel> model(new QStandardItemModel(4, 2));
    BOBUIest::newRow("row span, bottom up")
        << keyPresses << model << 1 << 1 << 2 << 1 << model->index(2, 1) << model->index(1, 1);

    /* +---+---+
       |   | v |
       +---+---+
       |   |x,c|
       +---+   +
       |   |   |
       +---+---+
       |   |   |
       +---+---+ */
    keyPresses = {BobUI::Key_Right, BobUI::Key_Down};
    model = QSharedPointer<QStandardItemModel>::create(4, 2);
    BOBUIest::newRow("row span, top down")
        << keyPresses << model << 1 << 1 << 2 << 1 << model->index(1, 1) << model->index(1, 1);

    /* +---+---+---+
       |   |   |   |
       +---+---+---+
       |   |x,c| < |
       +---+   +---+
       |   |   |   |
       +---+---+---+ */
    keyPresses = {BobUI::Key_End, BobUI::Key_Down, BobUI::Key_Left};
    model = QSharedPointer<QStandardItemModel>::create(3, 3);
    BOBUIest::newRow("row span, right to left")
        << keyPresses << model << 1 << 1 << 2 << 1 << model->index(1, 1) << model->index(1, 1);

    /* +---+---+---+
       |   |   |   |
       +---+---+---+
       |   | x |   |
       +---+   +---+
       | > | c |   |
       +---+---+---+ */
    keyPresses = {BobUI::Key_PageDown, BobUI::Key_Right};
    model = QSharedPointer<QStandardItemModel>::create(3, 3);
    BOBUIest::newRow("row span, left to right")
        << keyPresses << model << 1 << 1 << 2 << 1 << model->index(2, 1) << model->index(1, 1);

    /* +---+---+---+
       |   |   |   |
       +---+---+---+
       |x,c        |
       +---+---+---+
       | ^ |   |   |
       +---+---+---+ */
    keyPresses = {BobUI::Key_PageDown, BobUI::Key_Up};
    model = QSharedPointer<QStandardItemModel>::create(3, 3);
    BOBUIest::newRow("col span, bottom up")
        << keyPresses << model << 1 << 0 << 1 << 3 << model->index(1, 0) << model->index(1, 0);

    /* +---+---+---+
       |   |   |   |
       +---+---+---+
       | x   c     |
       +---+---+---+
       |   | ^ |   |
       +---+---+---+ */
    keyPresses = {BobUI::Key_PageDown, BobUI::Key_Right, BobUI::Key_Up};
    model = QSharedPointer<QStandardItemModel>::create(3, 3);
    BOBUIest::newRow("col span, bottom up #2")
        << keyPresses << model << 1 << 0 << 1 << 3 << model->index(1, 1) << model->index(1, 0);

    /* +---+---+---+
       |   |   | v |
       +---+---+---+
       | x       c |
       +---+---+---+
       |   |   |   |
       +---+---+---+ */
    keyPresses = {BobUI::Key_End, BobUI::Key_Down};
    model = QSharedPointer<QStandardItemModel>::create(3, 3);
    BOBUIest::newRow("col span, top down")
        << keyPresses << model << 1 << 0 << 1 << 3 << model->index(1, 2) << model->index(1, 0);
}

class TableViewWithCursorExposed : public BOBUIableView
{
public:
    using BOBUIableView::BOBUIableView;

    QModelIndex visualCursorIndex()
    {
        BOBUIableViewPrivate *d = static_cast<BOBUIableViewPrivate*>(bobui_widget_private(this));
        return d->model->index(d->visualCursor.y(), d->visualCursor.x());
    }
};

void tst_BOBUIableView::editSpanFromDirections()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(const KeyList, keyPresses);
    QFETCH(QSharedPointer<QStandardItemModel>, model);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(int, rowSpan);
    QFETCH(int, columnSpan);
    QFETCH(QModelIndex, expectedVisualCursorIndex);
    QFETCH(QModelIndex, expectedEditedIndex);

    TableViewWithCursorExposed view;
    view.setModel(model.data());
    // we have to make sure that PgUp/PgDown can scroll to the bottom/top
    view.resize(view.horizontalHeader()->length() + 50,
                view.verticalHeader()->length() + 50);
    view.setSpan(row, column, rowSpan, columnSpan);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    for (BobUI::Key key : keyPresses)
        BOBUIest::keyClick(&view, key);
    QCOMPARE(view.visualCursorIndex(), expectedVisualCursorIndex);
    QCOMPARE(view.selectionModel()->currentIndex(), expectedEditedIndex);

    BOBUIest::keyClick(&view, BobUI::Key_X);
    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Enter);
    BOBUIRY_COMPARE(view.model()->data(expectedEditedIndex).toString(), QLatin1String("x"));
}

class Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    using QAbstractTableModel::QAbstractTableModel;

    int rowCount(const QModelIndex &) const override
    {
        return rows;
    }
    int columnCount(const QModelIndex &) const override
    {
        return columns;
    }
    QVariant data(const QModelIndex &, int) const override
    {
        return QVariant();
    }
    void res()
    {
        beginResetModel();
        endResetModel();
    }

    int rows = 0;
    int columns = 0;
};

void tst_BOBUIableView::checkHeaderReset()
{
    BOBUIableView view;
    Model m;
    m.rows = 3;
    m.columns = 3;
    view.setModel(&m);

    m.rows = 4;
    m.columns = 4;
    m.res();
    QCOMPARE(view.horizontalHeader()->count(), 4);
}

void tst_BOBUIableView::checkHeaderMinSize()
{
    //tests if the minimumsize is of a header is taken into account
    //while computing BOBUIableView geometry. For that we test the position of the
    //viewport.
    BOBUIableView view;
    QStringListModel m;
    m.setStringList({QLatin1String("one cell is enough")});
    view.setModel(&m);

    //setting the minimum height on the horizontal header
    //and the minimum width on the vertical header
    view.horizontalHeader()->setMinimumHeight(50);
    view.verticalHeader()->setMinimumWidth(100);

    view.show();

    QVERIFY( view.verticalHeader()->y() >= view.horizontalHeader()->minimumHeight());
    QVERIFY( view.horizontalHeader()->x() >= view.verticalHeader()->minimumWidth());
}

void tst_BOBUIableView::resizeToContents()
{
    //checks that the resize to contents is consistent
    BOBUIableWidget table(2,3);
    BOBUIableWidget table2(2,3);
    BOBUIableWidget table3(2,3);


    table.setHorizontalHeaderItem(0, new BOBUIableWidgetItem("A Lot of text here: BLA BLA BLA"));
    table2.setHorizontalHeaderItem(0, new BOBUIableWidgetItem("A Lot of text here: BLA BLA BLA"));
    table3.setHorizontalHeaderItem(0, new BOBUIableWidgetItem("A Lot of text here: BLA BLA BLA"));
    table.horizontalHeader()->setVisible(false);
    table2.horizontalHeader()->setVisible(false);
    table.verticalHeader()->setVisible(false);
    table2.verticalHeader()->setVisible(false);


    for (int i = 0; i < table.columnCount(); i++)
        table.resizeColumnToContents(i);
    for (int i = 0; i < table.rowCount(); i++)
        table.resizeRowToContents(i);
    table2.resizeColumnsToContents();
    table2.resizeRowsToContents();
    table3.resizeColumnsToContents();
    table3.resizeRowsToContents();

    //now let's check the row/col sizes
    for (int i = 0; i < table.columnCount(); i++) {
        QCOMPARE(table.columnWidth(i), table2.columnWidth(i));
        QCOMPARE(table2.columnWidth(i), table3.columnWidth(i));
    }
    for (int i = 0; i < table.rowCount(); i++) {
        QCOMPARE(table.rowHeight(i), table2.rowHeight(i));
        QCOMPARE(table2.rowHeight(i), table3.rowHeight(i));
    }

}


class SpanModel : public QAbstractTableModel
{
public:
    SpanModel(bool sectionsMoved)
        : _sectionsMoved(sectionsMoved)
    {}
    int columnCount(const QModelIndex & = {}) const override { return 2; }
    int rowCount(const QModelIndex & = {}) const override { return 1; }
    QVariant data(const QModelIndex &idx, int role = BobUI::DisplayRole) const override
    {
        if (role != BobUI::DisplayRole)
            return QVariant();
        const int col = _sectionsMoved ? 1 - idx.column() : idx.column();
        if (col == 0)
            return "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
        return QVariant();
    }
private:
    bool _sectionsMoved;
};


void tst_BOBUIableView::resizeToContentsSpans()
{
    SpanModel model1(false);
    SpanModel model2(true);
    BOBUIableView view1, view2, view3;
    view1.setModel(&model1);
    view2.setModel(&model2);
    view2.horizontalHeader()->moveSection(0, 1);
    view3.setModel(&model1);

    view1.setSpan(0, 0, 1, 2);
    view2.setSpan(0, 1, 1, 2);
    view1.show();
    view2.show();
    view3.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view1));
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view2));
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view3));
    view1.setColumnWidth(0, 100);
    view1.setColumnWidth(1, 100);
    view2.setColumnWidth(0, 100);
    view2.setColumnWidth(1, 100);
    view3.setColumnWidth(0, 200);

    view1.resizeRowToContents(0);
    view2.resizeRowToContents(0);
    view3.resizeRowToContents(0);
    QCOMPARE(view1.rowHeight(0), view3.rowHeight(0));
    QCOMPARE(view2.rowHeight(0), view3.rowHeight(0));

    view3.resizeColumnToContents(0);
    view3.resizeRowToContents(0);
    // height should be only 1 text line for easy testing
    view1.setRowHeight(0, view3.verticalHeader()->sectionSize(0));
    view2.setRowHeight(0, view3.verticalHeader()->sectionSize(0));
    view1.resizeColumnToContents(0);
    view2.resizeColumnToContents(1);
    QCOMPARE(view1.columnWidth(0), view3.columnWidth(0) - view1.columnWidth(1));
    QCOMPARE(view2.columnWidth(0), view3.columnWidth(0) - view2.columnWidth(1));
}

void tst_BOBUIableView::resizeToContentsEarly()
{
    QStringListModel model;
    BOBUIableView view;

    // connect to the model before setting it on the view
    connect(&model, &QStringListModel::modelReset, &model, [&view]{
        view.resizeColumnsToContents();
    });
    connect(&model, &QStringListModel::modelReset, &model, [&view]{
        view.resizeRowsToContents();
    });

    // the view only connects now to the model's signals, so responds to the
    // reset signal *after* the lambdas above
    view.setModel(&model);

    QStringList data(200, QString("Hello World"));
    model.setStringList(data);

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    view.verticalScrollBar()->setValue(view.verticalScrollBar()->maximum());

    data = data.sliced(data.size() / 2);
    model.setStringList(data);
}

BOBUI_BEGIN_NAMESPACE
extern bool Q_WIDGETS_EXPORT bobui_tab_all_widgets(); // qapplication.cpp
BOBUI_END_NAMESPACE

void tst_BOBUIableView::tabFocus()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    if (!bobui_tab_all_widgets())
        QSKIP("This test requires full keyboard control to be enabled.");

    // BOBUIableView enables tabKeyNavigation by default, but you should be able
    // to change focus on an empty table view, or on a table view that doesn't
    // have this property set.
    QWidget window;
    window.resize(200, 200);

    BOBUIableView *view = new BOBUIableView(&window);
    QLineEdit *edit = new QLineEdit(&window);

    window.show();
    window.setFocus();
    window.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    // window
    QVERIFY(window.hasFocus());
    QVERIFY(!view->hasFocus());
    QVERIFY(!edit->hasFocus());

    for (int i = 0; i < 2; ++i) {
        // tab to view
        BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Tab);
        BOBUIRY_VERIFY(!window.hasFocus());
        QVERIFY(view->hasFocus());
        QVERIFY(!edit->hasFocus());

        // tab to edit
        BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Tab);
        BOBUIRY_VERIFY(edit->hasFocus());
        QVERIFY(!window.hasFocus());
        QVERIFY(!view->hasFocus());
    }

    // backtab to view
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    BOBUIRY_VERIFY(view->hasFocus());
    QVERIFY(!window.hasFocus());
    QVERIFY(!edit->hasFocus());

    // backtab to edit
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    BOBUIRY_VERIFY(edit->hasFocus());
    QVERIFY(!window.hasFocus());
    QVERIFY(!view->hasFocus());

    QStandardItemModel model;
    view->setModel(&model);

    // backtab to view
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    BOBUIRY_VERIFY(view->hasFocus());
    QVERIFY(!window.hasFocus());
    QVERIFY(!edit->hasFocus());

    // backtab to edit
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    BOBUIRY_VERIFY(edit->hasFocus());
    QVERIFY(!window.hasFocus());
    QVERIFY(!view->hasFocus());

    model.insertRow(0, new QStandardItem("Hei"));
    model.insertRow(0, new QStandardItem("Hei"));
    model.insertRow(0, new QStandardItem("Hei"));

    // backtab to view
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    BOBUIRY_VERIFY(view->hasFocus());
    QVERIFY(!window.hasFocus());
    QVERIFY(!edit->hasFocus());

    // backtab to edit doesn't work
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    QVERIFY(!window.hasFocus());
    QVERIFY(view->hasFocus());
    QVERIFY(!edit->hasFocus());

    view->setTabKeyNavigation(false);

    // backtab to edit
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Backtab);
    BOBUIRY_VERIFY(edit->hasFocus());
    QVERIFY(!window.hasFocus());
    QVERIFY(!view->hasFocus());

    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Tab);
    BOBUIRY_VERIFY(view->hasFocus());
    BOBUIest::keyPress(QApplication::focusWidget(), BobUI::Key_Tab);
    BOBUIRY_VERIFY(edit->hasFocus());
}

class BigModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QVariant data(const QModelIndex &index,
                  int role = BobUI::DisplayRole) const override
    {
        if (role == BobUI::DisplayRole)
            return QString::number(index.column()) + QLatin1String(" - ") + QString::number(index.row());
        return QVariant();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 10000000;
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 20000000;
    }
};

void tst_BOBUIableView::bigModel()
{
    //should not crash
    BOBUIableView view;
    BigModel model;
    view.setModel(&model);
    view.show();
    view.setSpan(10002,10002,6,6);
    BOBUIest::qWait(100);
    view.resize(1000,1000);
    BOBUIest::qWait(100);
    view.scrollTo(model.index(10010,10010));
    BOBUIest::qWait(100);
}

void tst_BOBUIableView::selectionSignal()
{
    BobUITestTableModel model(10, 10);
    BobUITestTableView view;
    view.checkSignalOrder = true;
    view.setModel(&model);
    view.resize(200, 200);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    BOBUIest::mouseClick(view.viewport(), BobUI::LeftButton, {}, view.visualRect(model.index(2, 0)).center());
}

void tst_BOBUIableView::setCurrentIndex()
{
    BobUITestTableModel model(4, 4);
    BobUITestTableView view;
    view.setModel(&model);

    // submit() slot should be called in model when current row changes
    view.setCurrentIndex(model.index(0,0));
    QCOMPARE(model.submit_count, 1);
    view.setCurrentIndex(model.index(0,2));
    QCOMPARE(model.submit_count, 1);
    view.setCurrentIndex(model.index(1,0));
    QCOMPARE(model.submit_count, 2);
    view.setCurrentIndex(model.index(3,3));
    QCOMPARE(model.submit_count, 3);
    view.setCurrentIndex(model.index(0,1));
    QCOMPARE(model.submit_count, 4);
    view.setCurrentIndex(model.index(0,0));
    QCOMPARE(model.submit_count, 4);
}

void tst_BOBUIableView::checkIntersectedRect_data()
{
    BOBUIest::addColumn<BobUITestTableModel *>("model");
    BOBUIest::addColumn<QList<QModelIndex>>("changedIndexes");
    BOBUIest::addColumn<bool>("isEmpty");
    BOBUIest::addColumn<bool>("swapFirstAndLastIndexRow");  // for QHeaderView::sectionsMoved()
    BOBUIest::addColumn<bool>("swapFirstAndLastIndexColumn");  // for QHeaderView::sectionsMoved()
    BOBUIest::addColumn<BobUI::LayoutDirection>("layoutDirection");
    BOBUIest::addColumn<int>("hiddenRow");
    BOBUIest::addColumn<int>("hiddenCol");
    const auto testName = [](const QByteArray &prefix, BobUI::LayoutDirection dir, bool r, bool c)
    {
        const char *strDir = dir == BobUI::LeftToRight ? ", LeftToRight" : ", RightToLeft";
        const char *strRow = r ? ", rowsSwapped" : "";
        const char *strCol = c ? ", colsSwapped" : "";
        return prefix + strDir + strRow + strCol;
    };
    for (int i = 0; i < 2; ++i) {
        const BobUI::LayoutDirection dir(i == 0 ? BobUI::LeftToRight : BobUI::RightToLeft);
        for (int j = 0; j < 4; ++j) {
            const bool swapRow = ((j & 1) == 1);
            const bool swapColumn = ((j & 2) == 2);
            {
                BobUITestTableModel *model = new BobUITestTableModel(10, 3);
                BOBUIest::newRow(testName("multiple columns", dir, swapRow, swapColumn).data())
                        << model << QList<QModelIndex>({ model->index(0, 0), model->index(0, 1) })
                        << false << swapRow << swapColumn << dir << -1 << -1;
            }
            {
                BobUITestTableModel *model = new BobUITestTableModel(10, 3);
                BOBUIest::newRow(testName("multiple rows", dir, swapRow, swapColumn).data())
                        << model
                        << QList<QModelIndex>(
                                   { model->index(0, 0), model->index(1, 0), model->index(2, 0) })
                        << false << swapRow << swapColumn << dir << -1 << -1;
            }
            {
                BobUITestTableModel *model = new BobUITestTableModel(10, 3);
                BOBUIest::newRow(testName("hidden row", dir, swapRow, swapColumn).data())
                        << model << QList<QModelIndex>({ model->index(3, 0), model->index(3, 1) })
                        << true << swapRow << swapColumn << dir << 3 << -1;
            }
            {
                BobUITestTableModel *model = new BobUITestTableModel(50, 2);
                BOBUIest::newRow(testName("row outside viewport", dir, swapRow, swapColumn).data())
                        << model << QList<QModelIndex>({ model->index(49, 0), model->index(49, 1) })
                        << true << swapRow << swapColumn << dir << -1 << -1;
            }
        }
    }
}

void tst_BOBUIableView::checkIntersectedRect()
{
    QFETCH(BobUITestTableModel *, model);
    QFETCH(const QList<QModelIndex>, changedIndexes);
    QFETCH(bool, isEmpty);
    QFETCH(bool, swapFirstAndLastIndexRow);
    QFETCH(bool, swapFirstAndLastIndexColumn);
    QFETCH(BobUI::LayoutDirection, layoutDirection);
    QFETCH(int, hiddenRow);
    QFETCH(int, hiddenCol);

    BobUITestTableView view;
    model->setParent(&view);
    view.setLayoutDirection(layoutDirection);
    view.setModel(model);
    view.resize(400, 400);
    view.show();
    if (hiddenRow >= 0)
        view.hideRow(hiddenRow);
    if (hiddenCol >= 0)
        view.hideRow(hiddenCol);
    if (swapFirstAndLastIndexRow)
        view.verticalHeader()->swapSections(changedIndexes.first().row(), changedIndexes.last().row());
    if (swapFirstAndLastIndexColumn)
        view.horizontalHeader()->swapSections(changedIndexes.first().column(), changedIndexes.last().column());

    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    const auto toString = [](const QModelIndex &idx)
    {
        return QStringLiteral("idx: %1/%2").arg(idx.row()).arg(idx.column());
    };

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
                    QVERIFY2(view.m_intersectecRect.contains(rect), qPrintable(toString(idx)));
                else
                    QVERIFY2(!view.m_intersectecRect.contains(rect), qPrintable(toString(idx)));
            }
        }
    }
}

class task173773_EventFilter : public QObject
{
    int paintEventCount_ = 0;
public:
    using QObject::QObject;
    int paintEventCount() const { return paintEventCount_; }
private:
    bool eventFilter(QObject *obj, QEvent *e) override
    {
        Q_UNUSED(obj);
        if (e->type() == QEvent::Paint)
            ++paintEventCount_;
        return false;
    }
};

void tst_BOBUIableView::task173773_updateVerticalHeader()
{
    QStandardItemModel model(2, 1);
    model.setData(model.index(0, 0), 0);
    model.setData(model.index(1, 0), 1);

    QSortFilterProxyModel proxyModel;
    proxyModel.setSourceModel(&model);

    BOBUIableView view;
    view.setModel(&proxyModel);
    view.setSortingEnabled(true);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    view.sortByColumn(0, BobUI::AscendingOrder);
    BOBUIest::qWait(100);

    task173773_EventFilter eventFilter;
    view.verticalHeader()->viewport()->installEventFilter(&eventFilter);

    view.sortByColumn(0, BobUI::DescendingOrder);
    BOBUIest::qWait(100);

    // ### note: this test may occasionally pass even if the bug is present!
    QVERIFY(eventFilter.paintEventCount() > 0);
}

void tst_BOBUIableView::task227953_setRootIndex()
{
    BOBUIableView tableView;

    //model = tree with two items with tables as children
    QStandardItemModel model;
    QStandardItem item1, item2;
    model.appendColumn(QList<QStandardItem*>() << &item1 << &item2);


    //setup the first table as a child of the first item
    for ( int row = 0; row < 40; ++row ) {
        item1.appendRow(QList<QStandardItem*>() << new QStandardItem(QLatin1String("row ") + QString::number(row)));
    }

    //setup the second table as a child of the second item
    for ( int row = 0; row < 10; ++row ) {
        item2.appendRow(QList<QStandardItem*>() << new QStandardItem(QLatin1String("row ") + QString::number(row)));
    }

    tableView.setModel(&model);

    //show the first 10 rows of the first table
    QModelIndex root = model.indexFromItem(&item1);
    tableView.setRootIndex(root);
    for (int i = 10; i != 40; ++i) {
        tableView.setRowHidden(i, true);
    }

    QCOMPARE(tableView.verticalHeader()->count(), 40);
    QCOMPARE(tableView.verticalHeader()->hiddenSectionCount(), 30);

    //show the first 10 rows of the second table
    tableView.setRootIndex(model.indexFromItem(&item2));

    QCOMPARE(tableView.verticalHeader()->count(), 10);
    QCOMPARE(tableView.verticalHeader()->hiddenSectionCount(), 0);
    QVERIFY(!tableView.verticalHeader()->isHidden());
}

void tst_BOBUIableView::task240266_veryBigColumn()
{
    BOBUIableView table;
    table.setFixedSize(500, 300); //just to make sure we have the 2 first columns visible
    QStandardItemModel model(1, 3);
    table.setModel(&model);
    table.setColumnWidth(0, 100); //normal column
    table.setColumnWidth(1, 100); //normal column
    table.setColumnWidth(2, 9000); //very big column
    table.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&table));

    //some styles change the scroll mode in their polish
    table.setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    table.setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    QScrollBar *scroll = table.horizontalScrollBar();
    QCOMPARE(scroll->minimum(), 0);
    QCOMPARE(scroll->maximum(), model.columnCount() - 1);
    QCOMPARE(scroll->singleStep(), 1);

    //1 is not always a very correct value for pageStep. Ideally this should be dynamic.
    //Maybe something for BobUI 5 ;-)
    QCOMPARE(scroll->pageStep(), 1);

}

void tst_BOBUIableView::task248688_autoScrollNavigation()
{
    //we make sure that when navigating with the keyboard the view is correctly scrolled
    //to the current item
    QStandardItemModel model(16, 16);
    BOBUIableView view;
    view.setModel(&model);

    view.hideColumn(8);
    view.hideRow(8);
    view.show();
    for (int r = 0; r < model.rowCount(); ++r) {
        if (view.isRowHidden(r))
            continue;
        for (int c = 0; c < model.columnCount(); ++c) {
            if (view.isColumnHidden(c))
                continue;
            QModelIndex index = model.index(r, c);
            view.setCurrentIndex(index);
            QVERIFY(view.viewport()->rect().contains(view.visualRect(index)));
        }
    }
}

#if BOBUI_CONFIG(wheelevent)
void tst_BOBUIableView::mouseWheel_data()
{
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("scrollMode");
    BOBUIest::addColumn<int>("delta");
    BOBUIest::addColumn<int>("horizontalPositon");
    BOBUIest::addColumn<int>("verticalPosition");

    BOBUIest::newRow("scroll up per item")
            << QAbstractItemView::ScrollPerItem << 120
            << 10 - QApplication::wheelScrollLines() << 10 - QApplication::wheelScrollLines();
    BOBUIest::newRow("scroll down per item")
            << QAbstractItemView::ScrollPerItem << -120
            << 10 + QApplication::wheelScrollLines() << 10 + QApplication::wheelScrollLines();
    BOBUIest::newRow("scroll down per pixel")
            << QAbstractItemView::ScrollPerPixel << -120
            << 10 + QApplication::wheelScrollLines() * 91 << 10 + QApplication::wheelScrollLines() * 46;
}

void tst_BOBUIableView::mouseWheel()
{
    QFETCH(QAbstractItemView::ScrollMode, scrollMode);
    QFETCH(int, delta);
    QFETCH(int, horizontalPositon);
    QFETCH(int, verticalPosition);

    BobUITestTableModel model(100, 100);
    QWidget topLevel;
    BobUITestTableView view(&topLevel);
    view.resize(500, 500);
    topLevel.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));

    view.setModel(&model);

    for (int r = 0; r < 100; ++r)
        view.setRowHeight(r, 50);
    for (int c = 0; c < 100; ++c)
        view.setColumnWidth(c, 100);

    view.setHorizontalScrollMode(scrollMode);
    view.setVerticalScrollMode(scrollMode);
    view.horizontalScrollBar()->setValue(10);
    view.verticalScrollBar()->setValue(10);

    QPoint pos = view.viewport()->geometry().center();
    QWheelEvent verticalEvent(pos, view.mapToGlobal(pos), QPoint(), QPoint(0, delta),
                              BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false);
    QWheelEvent horizontalEvent(pos, view.mapToGlobal(pos), QPoint(), QPoint(delta, 0),
                                BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false);
    QApplication::sendEvent(view.viewport(), &horizontalEvent);
    QVERIFY(qAbs(view.horizontalScrollBar()->value() - horizontalPositon) < 15);
    QApplication::sendEvent(view.viewport(), &verticalEvent);
    QVERIFY(qAbs(view.verticalScrollBar()->value() - verticalPosition) < 15);
}
#endif // BOBUI_CONFIG(wheelevent)

void tst_BOBUIableView::addColumnWhileEditing()
{
    BOBUIableView view;
    QStandardItemModel model(1, 10);
    view.setModel(&model);
    QModelIndex last = model.index(0,9);
    view.show();

    view.openPersistentEditor(last);
    view.scrollTo(last);

    //let's see if the editor is moved to the right location
    //after adding a column
    model.setColumnCount(model.columnCount() + 1);
    QPointer<QLineEdit> editor = view.findChild<QLineEdit*>();
    QVERIFY(editor);
    QCOMPARE(editor->geometry(), view.visualRect(last));

    //let's see if the editor is moved to the right location
    //after removing a column
    view.scrollTo(model.index(0, model.columnCount()-1));
    model.setColumnCount(model.columnCount() - 1);
    QVERIFY(editor);
    QCOMPARE(editor->geometry(), view.visualRect(last));
}

void tst_BOBUIableView::task259308_scrollVerticalHeaderSwappedSections()
{
    QStandardItemModel model;
    model.setRowCount(50);
    model.setColumnCount(2);
    for (int row = 0; row < model.rowCount(); ++row)
        for (int col = 0; col < model.columnCount(); ++col) {
            const QModelIndex &idx = model.index(row, col);
            model.setData(idx, QVariant(row), BobUI::EditRole);
        }

    BOBUIableView tv;
    tv.setModel(&model);
    tv.show();
    tv.verticalHeader()->swapSections(0, model.rowCount() - 1);
    tv.setCurrentIndex(model.index(model.rowCount() - 1, 0));

    QVERIFY(BOBUIest::qWaitForWindowExposed(&tv));
    BOBUIest::keyClick(&tv, BobUI::Key_PageUp);   // PageUp won't scroll when at top
    BOBUIRY_COMPARE(tv.rowAt(0), tv.verticalHeader()->logicalIndex(0));

    int newRow = tv.rowAt(tv.viewport()->height());
    BOBUIest::keyClick(&tv, BobUI::Key_PageDown); // Scroll down and check current
    BOBUIRY_COMPARE(tv.currentIndex().row(), newRow);

    tv.setCurrentIndex(model.index(0, 0));
    BOBUIest::keyClick(&tv, BobUI::Key_PageDown); // PageDown won't scroll when at the bottom
    BOBUIRY_COMPARE(tv.rowAt(tv.viewport()->height() - 1), tv.verticalHeader()->logicalIndex(model.rowCount() - 1));
}

template <typename T>
struct ValueSaver {
    T &var, value;
    ValueSaver(T &v) : var(v), value(v) { }
    ~ValueSaver() { var = value; }
};

void tst_BOBUIableView::task191545_dragSelectRows()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QStandardItemModel model(10, 10);
    BOBUIableView table;
    table.setModel(&model);
    table.setSelectionBehavior(QAbstractItemView::SelectItems);
    table.setSelectionMode(QAbstractItemView::ExtendedSelection);
    table.setMinimumSize(1000, 400);
    table.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&table));

    ValueSaver<BobUI::KeyboardModifiers> saver(QApplicationPrivate::modifier_buttons);
    QApplicationPrivate::modifier_buttons = BobUI::ControlModifier;

    {
        QRect cellRect = table.visualRect(model.index(3, 0));
        QHeaderView *vHeader = table.verticalHeader();
        QWidget *vHeaderVp = vHeader->viewport();
        QPoint rowPos(cellRect.center());
        QMouseEvent rowPressEvent(QEvent::MouseButtonPress, rowPos, rowPos, vHeaderVp->mapToGlobal(rowPos),
                                  BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(vHeaderVp, &rowPressEvent);

        for (int i = 0; i < 4; ++i) {
            rowPos.setY(rowPos.y() + cellRect.height());
            QMouseEvent moveEvent(QEvent::MouseMove, rowPos, rowPos, vHeaderVp->mapToGlobal(rowPos),
                                  BobUI::NoButton, BobUI::LeftButton, BobUI::ControlModifier);
            QCoreApplication::sendEvent(vHeaderVp, &moveEvent);
        }
        QMouseEvent rowReleaseEvent(QEvent::MouseButtonRelease, rowPos, vHeaderVp->mapToGlobal(rowPos),
                                    BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(vHeaderVp, &rowReleaseEvent);

        for (int i = 0; i < 4; ++i) {
            QModelIndex index = model.index(3 + i, 0, table.rootIndex());
            QVERIFY(vHeader->selectionModel()->selectedRows().contains(index));
        }
    }

    {
        QRect cellRect = table.visualRect(model.index(0, 3));
        QHeaderView *hHeader = table.horizontalHeader();
        QWidget *hHeaderVp = hHeader->viewport();
        QPoint colPos((cellRect.left() + cellRect.right()) / 2, 5);
        QMouseEvent colPressEvent(QEvent::MouseButtonPress, colPos, hHeaderVp->mapToGlobal(colPos),
                                  BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(hHeaderVp, &colPressEvent);

        for (int i = 0; i < 4; ++i) {
            colPos.setX(colPos.x() + cellRect.width());
            QMouseEvent moveEvent(QEvent::MouseMove, colPos, hHeaderVp->mapToGlobal(colPos),
                                  BobUI::NoButton, BobUI::LeftButton, BobUI::ControlModifier);
            QCoreApplication::sendEvent(hHeaderVp, &moveEvent);
        }
        QMouseEvent colReleaseEvent(QEvent::MouseButtonRelease, colPos, hHeaderVp->mapToGlobal(colPos),
                                    BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(hHeaderVp, &colReleaseEvent);

        for (int i = 0; i < 4; ++i) {
            QModelIndex index = model.index(0, 3 + i, table.rootIndex());
            QVERIFY(hHeader->selectionModel()->selectedColumns().contains(index));
        }
    }

    {
        QRect cellRect = table.visualRect(model.index(2, 2));
        QWidget *tableVp = table.viewport();
        QPoint cellPos = cellRect.center();
        QMouseEvent cellPressEvent(QEvent::MouseButtonPress, cellPos, tableVp->mapToGlobal(cellPos),
                                   BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(tableVp, &cellPressEvent);

        for (int i = 0; i < 6; ++i) {
            cellPos.setX(cellPos.x() + cellRect.width());
            cellPos.setY(cellPos.y() + cellRect.height());
            QMouseEvent moveEvent(QEvent::MouseMove, cellPos, tableVp->mapToGlobal(cellPos),
                                  BobUI::NoButton, BobUI::LeftButton, BobUI::ControlModifier);
            QCoreApplication::sendEvent(tableVp, &moveEvent);
        }
        QMouseEvent cellReleaseEvent(QEvent::MouseButtonRelease, cellPos, tableVp->mapToGlobal(cellPos),
                                     BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(tableVp, &cellReleaseEvent);

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                QModelIndex index = model.index(2 + i, 2 + j, table.rootIndex());
                QVERIFY(table.selectionModel()->isSelected(index));
            }
        }
    }

    {
        QRect cellRect = table.visualRect(model.index(3, 3));
        QWidget *tableVp = table.viewport();
        QPoint cellPos = cellRect.center();
        QMouseEvent cellPressEvent(QEvent::MouseButtonPress, cellPos, tableVp->mapToGlobal(cellPos),
                                   BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(tableVp, &cellPressEvent);

        for (int i = 0; i < 6; ++i) {
            // cellPos might have been updated by scrolling, so refresh
            cellPos = table.visualRect(model.index(3+i, 3+i)).center();
            cellPos.setX(cellPos.x() + cellRect.width());
            cellPos.setY(cellPos.y() + cellRect.height());
            QMouseEvent moveEvent(QEvent::MouseMove, cellPos, tableVp->mapToGlobal(cellPos),
                                  BobUI::NoButton, BobUI::LeftButton, BobUI::ControlModifier);
            QCoreApplication::sendEvent(tableVp, &moveEvent);
        }
        QMouseEvent cellReleaseEvent(QEvent::MouseButtonRelease, cellPos, tableVp->mapToGlobal(cellPos),
                                     BobUI::LeftButton, BobUI::NoButton, BobUI::ControlModifier);
        QCoreApplication::sendEvent(tableVp, &cellReleaseEvent);

        BOBUIest::qWait(200);
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                QModelIndex index = model.index(3 + i, 3 + j, table.rootIndex());
                QVERIFY(!table.selectionModel()->isSelected(index));
            }
        }
    }
}

void tst_BOBUIableView::task234926_setHeaderSorting()
{
    QStringListModel model;
    QSortFilterProxyModel sfpm; // default QStandardItemModel does not support 'unsorted' state
    sfpm.setSourceModel(&model);
    const QStringList data({"orange", "apple", "banana", "lemon", "pumpkin"});
    QStringList sortedDataA = data;
    QStringList sortedDataD = data;
    std::sort(sortedDataA.begin(), sortedDataA.end());
    std::sort(sortedDataD.begin(), sortedDataD.end(), std::greater<QString>());
    model.setStringList(data);
    BOBUIableView view;
    view.setModel(&sfpm);

    BOBUIRY_COMPARE(model.stringList(), data);
    view.setSortingEnabled(true);
    view.sortByColumn(0, BobUI::AscendingOrder);
    for (int i = 0; i < sortedDataA.size(); ++i)
        QCOMPARE(view.model()->data(view.model()->index(i, 0)).toString(), sortedDataA.at(i));

    view.horizontalHeader()->setSortIndicator(0, BobUI::DescendingOrder);
    for (int i = 0; i < sortedDataD.size(); ++i)
        QCOMPARE(view.model()->data(view.model()->index(i, 0)).toString(), sortedDataD.at(i));

    QHeaderView *h = new QHeaderView(BobUI::Horizontal);
    h->setModel(&model);
    view.setHorizontalHeader(h);
    h->setSortIndicator(0, BobUI::AscendingOrder);
    for (int i = 0; i < sortedDataA.size(); ++i)
        QCOMPARE(view.model()->data(view.model()->index(i, 0)).toString(), sortedDataA.at(i));

    h->setSortIndicator(0, BobUI::DescendingOrder);
    for (int i = 0; i < sortedDataD.size(); ++i)
        QCOMPARE(view.model()->data(view.model()->index(i, 0)).toString(), sortedDataD.at(i));

    view.sortByColumn(-1, BobUI::AscendingOrder);
    QCOMPARE(view.horizontalHeader()->sortIndicatorSection(), -1);
    for (int i = 0; i < data.size(); ++i)
        QCOMPARE(view.model()->data(view.model()->index(i, 0)).toString(), data.at(i));
}

void tst_BOBUIableView::taskBOBUIBUG_5062_spansInconsistency()
{
    const int nRows = 5;
    const int nColumns = 5;

    BobUITestTableModel model(nRows, nColumns);
    BobUITestTableView view;
    view.setModel(&model);

    for (int i = 0; i < nRows; ++i)
       view.setSpan(i, 0, 1, nColumns);
    view.setSpan(2, 0, 1, 1);
    view.setSpan(3, 0, 1, 1);

    VERIFY_SPANS_CONSISTENCY(&view);
}

void tst_BOBUIableView::taskBOBUIBUG_4516_clickOnRichTextLabel()
{
    BOBUIableView view;
    QStandardItemModel model(5,5);
    view.setModel(&model);
    QLabel label("rich text");
    label.setTextFormat(BobUI::RichText);
    view.setIndexWidget(model.index(1,1), &label);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    view.setCurrentIndex(model.index(0,0));
    QCOMPARE(view.currentIndex(), model.index(0,0));

    BOBUIest::mouseClick(&label, BobUI::LeftButton);
    QCOMPARE(view.currentIndex(), model.index(1,1));
}


void tst_BOBUIableView::changeHeaderData()
{
    BOBUIableView view;
    QStandardItemModel model(5,5);
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QString text = "long long long text";
    const int textWidth = view.verticalHeader()->fontMetrics().horizontalAdvance(text);
    QVERIFY(view.verticalHeader()->width() < textWidth);

    model.setHeaderData(2, BobUI::Vertical, text);

    BOBUIRY_VERIFY(view.verticalHeader()->width() > textWidth);
}

#if BOBUI_CONFIG(wheelevent)
void tst_BOBUIableView::taskBOBUIBUG_5237_wheelEventOnHeader()
{
    BOBUIableView view;
    QStandardItemModel model(500,5);
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    int sbValueBefore = view.verticalScrollBar()->value();
    QHeaderView *header = view.verticalHeader();
    BOBUIest::mouseMove(header);
    QPoint pos = header->geometry().center();
    QWheelEvent wheelEvent(pos, header->viewport()->mapToGlobal(pos), QPoint(), QPoint(0, -720),
                           BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false);
    QApplication::sendEvent(header->viewport(), &wheelEvent);
    int sbValueAfter = view.verticalScrollBar()->value();
    QVERIFY(sbValueBefore != sbValueAfter);
}
#endif

class TestTableView : public BOBUIableView
{
    Q_OBJECT
public:
    TestTableView(QWidget *parent = nullptr) : BOBUIableView(parent)
    {
        connect(this, &BOBUIableView::entered, this, &TestTableView::openPersistentEditor);
    }
public slots:
    void onDataChanged()
    {
        for (int i = 0; i < model()->rowCount(); i++) {
            setRowHidden(i, model()->data(model()->index(i, 0)).toBool());
        }
    }
};


void tst_BOBUIableView::taskBOBUIBUG_8585_crashForNoGoodReason()
{
    QStandardItemModel model;
    model.insertColumn(0, QModelIndex());
    for (int i = 0; i < 20; i++)
        model.insertRow(i);

    TestTableView w;
    w.setMouseTracking(true);
    w.setModel(&model);
    connect(&model, &QStandardItemModel::dataChanged, &w, &TestTableView::onDataChanged);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    for (int i = 0; i < 10; i++)
    {
        BOBUIest::mouseMove(w.viewport(), QPoint(50, 20));
        w.model()->setData(w.indexAt(QPoint(50, 20)), true);
        BOBUIest::mouseMove(w.viewport(), QPoint(50, 25));
    }
}

class TableView7774 : public BOBUIableView
{
public:
    using BOBUIableView::visualRegionForSelection;
};

void tst_BOBUIableView::taskBOBUIBUG_7774_RtoLVisualRegionForSelection()
{
    TableView7774 view;
    QStandardItemModel model(5,5);
    view.setModel(&model);
    view.setLayoutDirection(BobUI::RightToLeft);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QItemSelectionRange range(model.index(2, 0), model.index(2, model.columnCount() - 1));
    QItemSelection selection;
    selection << range;
    QRegion region = view.visualRegionForSelection(selection);
    QVERIFY(!region.isEmpty());
    QCOMPARE(region.begin()[0], view.visualRect(range.topLeft()) | view.visualRect(range.bottomRight()));
}

void tst_BOBUIableView::taskBOBUIBUG_8777_scrollToSpans()
{
    BOBUIableWidget table(75,5);
    for (int i=0; i<50; i++)
        table.setSpan(2+i, 0, 1, 5);
    table.setCurrentCell(0,2);
    table.show();

    for (int i = 0; i < 45; ++i)
        BOBUIest::keyClick(&table, BobUI::Key_Down);

    QVERIFY(table.verticalScrollBar()->value() > 10);
}

void tst_BOBUIableView::taskBOBUIBUG_10169_sizeHintForRow()
{
    BobUITestTableView tableView;
    QStandardItemModel model(1, 3);
    model.setData(model.index(0, 0), "Word wrapping text goes here.");
    tableView.setModel(&model);
    tableView.verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    const int orderedHeight = tableView.sizeHintForRow(0);
    tableView.horizontalHeader()->moveSection(2, 0);
    const int reorderedHeight = tableView.sizeHintForRow(0);

    //the order of the columns shouldn't matter.
    QCOMPARE(orderedHeight, reorderedHeight);
}

void tst_BOBUIableView::viewOptions()
{
    BobUITestTableView view;
    QStyleOptionViewItem options;
    view.initViewItemOption(&options);
    QVERIFY(options.showDecorationSelected);
}

void tst_BOBUIableView::taskBOBUIBUG_30653_doItemsLayout()
{
    QWidget topLevel;
    BobUITestTableView view(&topLevel);

    BobUITestTableModel model(5, 5);
    view.setModel(&model);

    BobUITestItemDelegate delegate;
    delegate.hint = QSize(50, 50);
    view.setItemDelegate(&delegate);

    view.resizeRowsToContents();
    view.resizeColumnsToContents();

    // show two and half rows/cols
    int extraWidth = view.verticalHeader()->sizeHint().width() + view.verticalScrollBar()->sizeHint().width();
    int extraHeight = view.horizontalHeader()->sizeHint().height() + view.horizontalScrollBar()->sizeHint().height();
    view.resize(125 + extraWidth, 125 + extraHeight);

    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));

    // the offset after scrollToBottom() and doItemsLayout() should not differ
    // as the view content should stay aligned to the last section
    view.scrollToBottom();
    int scrollToBottomOffset = view.verticalHeader()->offset();
    view.doItemsLayout();
    int doItemsLayoutOffset = view.verticalHeader()->offset();

    QCOMPARE(scrollToBottomOffset, doItemsLayoutOffset);
}

void tst_BOBUIableView::taskBOBUIBUG_7232_AllowUserToControlSingleStep()
{
    // When we set the scrollMode to ScrollPerPixel it will adjust the scrollbars singleStep automatically
    // Setting a singlestep on a scrollbar should however imply that the user takes control (and it is not changed by geometry updates).
    // Setting a singlestep to -1 return to an automatic control of the singleStep.
    BOBUIableView t;
    t.setVerticalScrollBarPolicy(BobUI::ScrollBarAlwaysOn);
    t.setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOn);
    QStandardItemModel model(200, 200);
    t.setModel(&model);
    t.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&t));
    t.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    t.setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    t.setGeometry(200, 200, 200, 200);
    int vStep1 = t.verticalScrollBar()->singleStep();
    int hStep1 = t.horizontalScrollBar()->singleStep();
    QVERIFY(vStep1 > 1);
    QVERIFY(hStep1 > 1);

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

void tst_BOBUIableView::taskBOBUIBUG_50171_selectRowAfterSwapColumns()
{
    {
        BobUITestTableView tableView;
        BobUITestTableModel model(2, 3);
        tableView.setModel(&model);

        tableView.horizontalHeader()->swapSections(1, 2);
        tableView.horizontalHeader()->hideSection(0);
        tableView.selectRow(1);

        QItemSelectionModel* tableSelectionModel = tableView.selectionModel();
        QCOMPARE(tableSelectionModel->isRowSelected(1, QModelIndex()), true);
        QCOMPARE(tableSelectionModel->isSelected(tableView.model()->index(0, 0)), false);
        QCOMPARE(tableSelectionModel->isSelected(tableView.model()->index(0, 1)), false);
        QCOMPARE(tableSelectionModel->isSelected(tableView.model()->index(0, 2)), false);
    }

    {
        BobUITestTableView tableView;
        BobUITestTableModel model(3, 2);
        tableView.setModel(&model);

        tableView.verticalHeader()->swapSections(1, 2);
        tableView.verticalHeader()->hideSection(0);
        tableView.selectColumn(1);

        QItemSelectionModel* sModel = tableView.selectionModel();
        QCOMPARE(sModel->isColumnSelected(1, QModelIndex()), true);
        QCOMPARE(sModel->isSelected(tableView.model()->index(0, 0)), false);
        QCOMPARE(sModel->isSelected(tableView.model()->index(1, 0)), false);
        QCOMPARE(sModel->isSelected(tableView.model()->index(2, 0)), false);
    }
}

class DeselectTableWidget : public BOBUIableWidget
{
public:
    using BOBUIableWidget::BOBUIableWidget;
    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &,
                                                         const QEvent * = nullptr) const override
    {
        return QItemSelectionModel::Toggle;
    }
};

void tst_BOBUIableView::deselectRow()
{
    DeselectTableWidget tw(20, 20);
    tw.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tw));
    tw.hideColumn(0);
    QVERIFY(tw.isColumnHidden(0));
    tw.selectRow(1);
    QVERIFY(tw.selectionModel()->isRowSelected(1, QModelIndex()));
    tw.selectRow(1);
    // BOBUIBUG-79092 - deselection was not possible when column 0 was hidden
    QVERIFY(!tw.selectionModel()->isRowSelected(1, QModelIndex()));
}

class BOBUIableViewSelectCells : public BOBUIableView
{
public:
    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index,
                                                         const QEvent *) const override
    {
        return BOBUIableView::selectionCommand(index, shiftPressed ? &mouseEvent : nullptr);
    }
    QMouseEvent mouseEvent = QMouseEvent(QEvent::MouseButtonPress, QPointF(), QPointF(),
                                         BobUI::LeftButton, BobUI::LeftButton, BobUI::ShiftModifier);
    bool shiftPressed = false;
};

void tst_BOBUIableView::selectRowsAndCells()
{
    const auto checkRows = [](const QModelIndexList &mil)
    {
        QCOMPARE(mil.size(), 3);
        for (const auto &mi : mil)
            QVERIFY(mi.row() >= 1 && mi.row() <= 3);
    };
    BOBUIableViewSelectCells tw;
    BobUITestTableModel model(5, 1);
    tw.setSelectionBehavior(QAbstractItemView::SelectRows);
    tw.setSelectionMode(QAbstractItemView::ExtendedSelection);
    tw.setModel(&model);
    tw.show();

    tw.selectRow(1);
    tw.shiftPressed = true;
    tw.selectRow(2);
    tw.shiftPressed = false;
    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::ShiftModifier, tw.visualRect(model.index(3, 0)).center());
    checkRows(tw.selectionModel()->selectedRows());

    tw.clearSelection();
    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::NoModifier, tw.visualRect(model.index(3, 0)).center());
    tw.shiftPressed = true;
    tw.selectRow(1);
    checkRows(tw.selectionModel()->selectedRows());
}

void tst_BOBUIableView::selectColumnsAndCells()
{
    const auto checkColumns = [](const QModelIndexList &mil)
    {
        QCOMPARE(mil.size(), 3);
        for (const auto &mi : mil)
            QVERIFY(mi.column() >= 1 && mi.column() <= 3);
    };
    BOBUIableViewSelectCells tw;
    BobUITestTableModel model(1, 5);
    tw.setSelectionBehavior(QAbstractItemView::SelectColumns);
    tw.setSelectionMode(QAbstractItemView::ExtendedSelection);
    tw.setModel(&model);
    tw.show();

    tw.selectColumn(1);
    tw.shiftPressed = true;
    tw.selectColumn(2);
    tw.shiftPressed = false;
    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::ShiftModifier, tw.visualRect(model.index(0, 3)).center());
    checkColumns(tw.selectionModel()->selectedColumns());

    tw.clearSelection();
    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::NoModifier, tw.visualRect(model.index(0, 3)).center());
    tw.shiftPressed = true;
    tw.selectColumn(1);
    checkColumns(tw.selectionModel()->selectedColumns());
}

void tst_BOBUIableView::selectWithHeader_data()
{
    BOBUIest::addColumn<BobUI::Orientation>("orientation");

    BOBUIest::addRow("horizontal") << BobUI::Horizontal;
    BOBUIest::addRow("vertical") << BobUI::Vertical;
}

void tst_BOBUIableView::selectWithHeader()
{
    QFETCH(BobUI::Orientation, orientation);

    BOBUIableWidget view(10, 10);
    view.resize(200, 100);
    view.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    QHeaderView *header = nullptr;
    QPoint clickPos;
    QModelIndex lastIndex;

    switch (orientation) {
    case BobUI::Horizontal:
        header = view.horizontalHeader();
        clickPos.rx() = header->sectionPosition(0) + header->sectionSize(0) / 2;
        clickPos.ry() = header->height() / 2;
        lastIndex = view.model()->index(9, 0);
        break;
    case BobUI::Vertical:
        header = view.verticalHeader();
        clickPos.rx() = header->width() / 2;
        clickPos.ry() = header->sectionPosition(0) + header->sectionSize(0) / 2;
        lastIndex = view.model()->index(0, 9);
        break;
    }

    const auto isSelected = [&]{
        return orientation == BobUI::Horizontal
             ? view.selectionModel()->isColumnSelected(0)
             : view.selectionModel()->isRowSelected(0);
    };

    BOBUIest::mouseClick(header->viewport(), BobUI::LeftButton, {}, clickPos);
    QVERIFY(isSelected());
    BOBUIest::mouseClick(header->viewport(), BobUI::LeftButton, BobUI::ControlModifier, clickPos);
    QVERIFY(!isSelected());
    BOBUIest::mouseClick(header->viewport(), BobUI::LeftButton, {}, clickPos);
    QVERIFY(isSelected());
    view.scrollTo(lastIndex);
    BOBUIest::mouseClick(header->viewport(), BobUI::LeftButton, BobUI::ControlModifier, clickPos);
    QVERIFY(!isSelected());
}

void tst_BOBUIableView::resetDefaultSectionSize()
{
    // Create a table and change its default section size and then reset it.
    // This should be a no op so clicking on row 1 should select row 1 and not row
    // 0 as previously. BOBUIBUG-116013
    BOBUIableWidget view(10, 10);
    view.resize(300, 300);
    // We need to move this into section items mode...
    view.verticalHeader()->swapSections(0, 1);
    view.verticalHeader()->swapSections(0, 1);
    view.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    view.verticalHeader()->setDefaultSectionSize(120);
    view.verticalHeader()->resetDefaultSectionSize();
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    QCOMPARE(view.verticalHeader()->logicalIndexAt(9, 45), 1);
}

// This has nothing to do with BOBUIableView, but it's convenient to reuse the BobUITestTableModel
#if BOBUI_CONFIG(textmarkdownwriter)

// #define DEBUG_WRITE_OUTPUT

void tst_BOBUIableView::markdownWriter()
{
    BobUITestTableModel model(2, 3);
    QString md;
    {
        BOBUIextStream stream(&md);
        BOBUIextMarkdownWriter writer(stream, BOBUIextDocument::MarkdownDialectGitHub);
        writer.writeTable(&model);
    }

#ifdef DEBUG_WRITE_OUTPUT
    {
        QFile out("/tmp/table.md");
        out.open(QFile::WriteOnly);
        out.write(md.toUtf8());
        out.close();
    }
#endif

    QCOMPARE(md, QString::fromLatin1("|1      |2      |3      |\n|-------|-------|-------|\n|[0,0,0]|[0,1,0]|[0,2,0]|\n|[1,0,0]|[1,1,0]|[1,2,0]|\n"));
}
#endif

void tst_BOBUIableView::rowsInVerticalHeader()
{
    BobUITestTableModel model(0, 2);
    BOBUIableView view;
    view.setModel(&model);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));
    auto *verticalHeader = view.verticalHeader();
    QCOMPARE(verticalHeader->count(), 0);
    model.insertRows(2);
    QCOMPARE(verticalHeader->count(), 2);
}

BOBUIEST_MAIN(tst_BOBUIableView)
#include "tst_bobuiableview.moc"
