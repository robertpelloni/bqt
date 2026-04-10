// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QRandomGenerator>
#include <QStack>
#include <QStandardItemModel>
#include <BOBUIest>
#include "viewstotest.cpp"

/*!
    See viewstotest.cpp for instructions on how to have your view tested with these tests.

    Each test such as visualRect have a _data() function which populate the BOBUIest data with
    tests specified by viewstotest.cpp and any extra data needed for that particular test.

    setupWithNoTestData() fills BOBUIest data with only the tests it is used by most tests.

    There are some basic qDebug statements sprikled about that might be helpfull for
    fixing your issues.
 */
class tst_QItemView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void nonDestructiveBasicTest_data();
    void nonDestructiveBasicTest();

    void spider_data();
    void spider();

    void resize_data();
    void resize();

    void visualRect_data();
    void visualRect();

    void indexAt_data();
    void indexAt();

    void scrollTo_data();
    void scrollTo();

    void moveCursor_data();
    void moveCursor();

private:
    void setupWithNoTestData();
    void populate();
    void walkScreen(QAbstractItemView *view);

    QAbstractItemView *view;
    QAbstractItemModel *treeModel;
    ViewsToTest *testViews;
};

/*!
 * Views should not make invalid requests, sense a model might not check all the bad cases.
 */
class CheckerModel : public QStandardItemModel
{
    Q_OBJECT

public:
    using QStandardItemModel::QStandardItemModel;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override
    {
        if (!index.isValid()) {
            qWarning("%s: index is not valid", Q_FUNC_INFO);
            return QVariant();
        }
        return QStandardItemModel::data(index, role);
    }

    BobUI::ItemFlags flags(const QModelIndex &index) const override
    {
        if (!index.isValid()) {
            qWarning("%s: index is not valid", Q_FUNC_INFO);
            return BobUI::ItemFlags();
        }
        if (index.row() == 2 || index.row() == rowCount() - 3
            || index.column() == 2 || index.column() == columnCount() - 3) {
            BobUI::ItemFlags f = QStandardItemModel::flags(index);
            f.setFlag(BobUI::ItemIsEnabled, false);
            return f;
        }
        return QStandardItemModel::flags(index);
    }

    QModelIndex parent(const QModelIndex &child) const override
    {
        if (!child.isValid()) {
            qWarning("%s: child index is not valid", Q_FUNC_INFO);
            return QModelIndex();
        }
        return QStandardItemModel::parent(child);
    }

    QVariant headerData(int section, BobUI::Orientation orientation,
                        int role = BobUI::DisplayRole) const override
    {
        if (orientation == BobUI::Horizontal
            && (section < 0 || section > columnCount())) {
            qWarning("%s: invalid section %d, must be in range 0..%d",
                     Q_FUNC_INFO, section, columnCount());
            return QVariant();
        }
        if (orientation == BobUI::Vertical
            && (section < 0 || section > rowCount())) {
            qWarning("%s: invalid section %d, must be in range 0..%d",
                     Q_FUNC_INFO, section, rowCount());
            return QVariant();
        }
        return QStandardItemModel::headerData(section, orientation, role);
    }

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = BobUI::EditRole) override
    {
        if (!index.isValid()) {
            qWarning("%s: index is not valid", Q_FUNC_INFO);
            return false;
        }
        return QStandardItemModel::setData(index, value, role);
    }

    void sort(int column, BobUI::SortOrder order = BobUI::AscendingOrder) override
    {
        if (column < 0 || column > columnCount())
            qWarning("%s: invalid column %d, must be in range 0..%d",
                     Q_FUNC_INFO, column, columnCount());
        else
            QStandardItemModel::sort(column, order);
    }

    QModelIndexList match(const QModelIndex &start, int role,
                          const QVariant &value, int hits = 1,
                          BobUI::MatchFlags flags = BobUI::MatchFlags(BobUI::MatchStartsWith | BobUI::MatchWrap)) const override
    {
        if (hits <= 0) {
            qWarning("%s: hits must be greater than zero", Q_FUNC_INFO);
            return QModelIndexList();
        }
        if (!value.isValid()) {
            qWarning("%s: value is not valid", Q_FUNC_INFO);
            return QModelIndexList();
        }
        return QAbstractItemModel::match(start, role, value, hits, flags);
    }

    bool setHeaderData(int section, BobUI::Orientation orientation,
                       const QVariant &value, int role = BobUI::EditRole) override
    {
        if (orientation == BobUI::Horizontal
            && (section < 0 || section > columnCount())) {
            qWarning("%s: invalid section %d, must be in range 0..%d",
                     Q_FUNC_INFO, section, columnCount());
            return false;
        }
        if (orientation == BobUI::Vertical
            && (section < 0 || section > rowCount())) {
            qWarning("%s: invalid section %d, must be in range 0..%d",
                     Q_FUNC_INFO, section, rowCount());
            return false;
        }
        return QAbstractItemModel::setHeaderData(section, orientation, value, role);
    }
};

void tst_QItemView::init()
{
    testViews = new ViewsToTest();
    populate();
}

void tst_QItemView::cleanup()
{
    delete testViews;
    delete view;
    delete treeModel;
    view = nullptr;
    testViews = nullptr;
    treeModel = nullptr;
}

void tst_QItemView::setupWithNoTestData()
{
    ViewsToTest testViews;
    BOBUIest::addColumn<QString>("viewType");
    BOBUIest::addColumn<bool>("displays");
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("vscroll");
    BOBUIest::addColumn<QAbstractItemView::ScrollMode>("hscroll");
    for (int i = 0; i < testViews.tests.size(); ++i) {
        QString view = testViews.tests.at(i).viewType;
        QString test = view + " ScrollPerPixel";
        bool displayIndexes = (testViews.tests.at(i).display == ViewsToTest::DisplayRoot);
        BOBUIest::newRow(test.toLatin1().data()) << view << displayIndexes
                                              << QAbstractItemView::ScrollPerPixel
                                              << QAbstractItemView::ScrollPerPixel
                                              ;
    }
    for (int i = 0; i < testViews.tests.size(); ++i) {
        QString view = testViews.tests.at(i).viewType;
        QString test = view + " ScrollPerItem";
        bool displayIndexes = (testViews.tests.at(i).display == ViewsToTest::DisplayRoot);
        BOBUIest::newRow(test.toLatin1().data()) << view << displayIndexes
                                              << QAbstractItemView::ScrollPerItem
                                              << QAbstractItemView::ScrollPerItem
                                              ;
    }
}

void tst_QItemView::populate()
{
    treeModel = new CheckerModel;
    QModelIndex parent;
#if defined(Q_PROCESSOR_ARM)
    const int baseInsert = 4;
#else
    const int baseInsert = 26;
#endif
    for (int i = 0; i < 40; ++i) {
        const QString iS = QString::number(i);
        parent = treeModel->index(0, 0, parent);
        treeModel->insertRows(0, baseInsert + i, parent);
        treeModel->insertColumns(0, baseInsert + i, parent);
        // Fill in some values to make it easier to debug
        for (int x = 0; x < treeModel->rowCount(); ++x) {
            const QString xS = QString::number(x);
            for (int y = 0; y < treeModel->columnCount(); ++y) {
                QModelIndex index = treeModel->index(x, y, parent);
                treeModel->setData(index, xS + QLatin1Char('_') + QString::number(y) + QLatin1Char('_') + iS);
                treeModel->setData(index, QVariant(QColor(BobUI::blue)), BobUI::ForegroundRole);
            }
        }
    }
}

void tst_QItemView::nonDestructiveBasicTest_data()
{
    setupWithNoTestData();
}

/*!
    nonDestructiveBasicTest tries to call a number of the basic functions (not all)
    to make sure the view doesn't segfault, testing the functions that makes sense.
 */
void tst_QItemView::nonDestructiveBasicTest()
{
    QFETCH(QString, viewType);
    QFETCH(QAbstractItemView::ScrollMode, vscroll);
    QFETCH(QAbstractItemView::ScrollMode, hscroll);

    view = testViews->createView(viewType);
    QVERIFY(view);
    view->setVerticalScrollMode(vscroll);
    view->setHorizontalScrollMode(hscroll);

    // setSelectionModel() will assert
    //view->setSelectionModel(0);
    // setItemDelegate() will assert
    //view->setItemDelegate(0);

    // setSelectionMode
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    QCOMPARE(view->selectionMode(), QAbstractItemView::SingleSelection);
    view->setSelectionMode(QAbstractItemView::ContiguousSelection);
    QCOMPARE(view->selectionMode(), QAbstractItemView::ContiguousSelection);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QCOMPARE(view->selectionMode(), QAbstractItemView::ExtendedSelection);
    view->setSelectionMode(QAbstractItemView::MultiSelection);
    QCOMPARE(view->selectionMode(), QAbstractItemView::MultiSelection);
    view->setSelectionMode(QAbstractItemView::NoSelection);
    QCOMPARE(view->selectionMode(), QAbstractItemView::NoSelection);

    // setSelectionBehavior
    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    QCOMPARE(view->selectionBehavior(), QAbstractItemView::SelectItems);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    QCOMPARE(view->selectionBehavior(), QAbstractItemView::SelectRows);
    view->setSelectionBehavior(QAbstractItemView::SelectColumns);
    QCOMPARE(view->selectionBehavior(), QAbstractItemView::SelectColumns);

    // setEditTriggers
    view->setEditTriggers(QAbstractItemView::EditKeyPressed);
    QCOMPARE(view->editTriggers(), QAbstractItemView::EditKeyPressed);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QCOMPARE(view->editTriggers(), QAbstractItemView::NoEditTriggers);
    view->setEditTriggers(QAbstractItemView::CurrentChanged);
    QCOMPARE(view->editTriggers(), QAbstractItemView::CurrentChanged);
    view->setEditTriggers(QAbstractItemView::DoubleClicked);
    QCOMPARE(view->editTriggers(), QAbstractItemView::DoubleClicked);
    view->setEditTriggers(QAbstractItemView::SelectedClicked);
    QCOMPARE(view->editTriggers(), QAbstractItemView::SelectedClicked);
    view->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    QCOMPARE(view->editTriggers(), QAbstractItemView::AnyKeyPressed);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    QCOMPARE(view->editTriggers(), QAbstractItemView::AllEditTriggers);

    // setAutoScroll
    view->setAutoScroll(false);
    QCOMPARE(view->hasAutoScroll(), false);
    view->setAutoScroll(true);
    QCOMPARE(view->hasAutoScroll(), true);

    // setTabKeyNavigation
    view->setTabKeyNavigation(false);
    QCOMPARE(view->tabKeyNavigation(), false);
    view->setTabKeyNavigation(true);
    QCOMPARE(view->tabKeyNavigation(), true);
#if BOBUI_CONFIG(draganddrop)
    // setDropIndicatorShown
    view->setDropIndicatorShown(false);
    QCOMPARE(view->showDropIndicator(), false);
    view->setDropIndicatorShown(true);
    QCOMPARE(view->showDropIndicator(), true);

    // setDragEnabled
    view->setDragEnabled(false);
    QCOMPARE(view->dragEnabled(), false);
    view->setDragEnabled(true);
    QCOMPARE(view->dragEnabled(), true);
#endif

    // setAlternatingRowColors
    view->setAlternatingRowColors(false);
    QCOMPARE(view->alternatingRowColors(), false);
    view->setAlternatingRowColors(true);
    QCOMPARE(view->alternatingRowColors(), true);

    // setIconSize
    view->setIconSize(QSize(16, 16));
    QCOMPARE(view->iconSize(), QSize(16, 16));
    view->setIconSize(QSize(32, 32));
    QCOMPARE(view->iconSize(), QSize(32, 32));
    // Should this happen?
    view->setIconSize(QSize(-1, -1));
    QCOMPARE(view->iconSize(), QSize(-1, -1));

    QCOMPARE(view->currentIndex(), QModelIndex());
    QCOMPARE(view->rootIndex(), QModelIndex());

    view->keyboardSearch("");
    view->keyboardSearch("foo");
    view->keyboardSearch("1");

    QCOMPARE(view->visualRect(QModelIndex()), QRect());

    view->scrollTo(QModelIndex());

    QCOMPARE(view->sizeHintForIndex(QModelIndex()), QSize());
    QCOMPARE(view->indexAt(QPoint(-1, -1)), QModelIndex());

    if (!view->model()){
        QCOMPARE(view->indexAt(QPoint(10, 10)), QModelIndex());
        QCOMPARE(view->sizeHintForRow(0), -1);
        QCOMPARE(view->sizeHintForColumn(0), -1);
    } else if (view->itemDelegate()){
        view->sizeHintForRow(0);
        view->sizeHintForColumn(0);
    }
    view->openPersistentEditor(QModelIndex());
    view->closePersistentEditor(QModelIndex());

    view->reset();
    view->setRootIndex(QModelIndex());
    view->doItemsLayout();
    view->selectAll();
    // edit() causes warning by default
    //view->edit(QModelIndex());
    view->clearSelection();
    view->setCurrentIndex(QModelIndex());
}

void tst_QItemView::spider_data()
{
    setupWithNoTestData();
}

void touch(QWidget *widget, BobUI::KeyboardModifier modifier, BobUI::Key keyPress)
{
    int width = widget->width();
    int height = widget->height();
    for (int i = 0; i < 5; ++i) {
        BOBUIest::mouseClick(widget, BobUI::LeftButton, modifier,
                          QPoint(QRandomGenerator::global()->bounded(width), QRandomGenerator::global()->bounded(height)));
        BOBUIest::mouseDClick(widget, BobUI::LeftButton, modifier,
                           QPoint(QRandomGenerator::global()->bounded(width), QRandomGenerator::global()->bounded(height)));
        QPoint press(QRandomGenerator::global()->bounded(width), QRandomGenerator::global()->bounded(height));
        QPoint releasePoint(QRandomGenerator::global()->bounded(width), QRandomGenerator::global()->bounded(height));
        BOBUIest::mousePress(widget, BobUI::LeftButton, modifier, press);
        BOBUIest::mouseMove(widget, releasePoint);
        if (QRandomGenerator::global()->bounded(1) == 0)
            BOBUIest::mouseRelease(widget, BobUI::LeftButton, {}, releasePoint);
        else
            BOBUIest::mouseRelease(widget, BobUI::LeftButton, modifier, releasePoint);
        BOBUIest::keyClick(widget, keyPress);
    }
}

/*!
    This is a basic stress testing application that tries a few basics such as clicking around
    the screen, and key presses.

    The main goal is to catch any easy segfaults, not to test every case.
  */
void tst_QItemView::spider()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(QString, viewType);
    QFETCH(QAbstractItemView::ScrollMode, vscroll);
    QFETCH(QAbstractItemView::ScrollMode, hscroll);

    view = testViews->createView(viewType);
    QVERIFY(view);
    view->setVerticalScrollMode(vscroll);
    view->setHorizontalScrollMode(hscroll);
    view->setModel(treeModel);
    view->show();
    QVERIFY(BOBUIest::qWaitForWindowActive(view));
    touch(view->viewport(), BobUI::NoModifier, BobUI::Key_Left);
    touch(view->viewport(), BobUI::ShiftModifier, BobUI::Key_Enter);
    touch(view->viewport(), BobUI::ControlModifier, BobUI::Key_Backspace);
    touch(view->viewport(), BobUI::AltModifier, BobUI::Key_Up);
}

void tst_QItemView::resize_data()
{
    setupWithNoTestData();
}

/*!
    The main goal is to catch any infinite loops from layouting
  */
void tst_QItemView::resize()
{
    QSKIP("This test needs to be re-thought out, it takes too long and doesn't really catch the problem.");

    QFETCH(QString, viewType);
    QFETCH(QAbstractItemView::ScrollMode, vscroll);
    QFETCH(QAbstractItemView::ScrollMode, hscroll);

    view = testViews->createView(viewType);
    QVERIFY(view);
    view->setVerticalScrollMode(vscroll);
    view->setHorizontalScrollMode(hscroll);
    view->setModel(treeModel);
    view->show();

    for (int w = 100; w < 400; w += 10) {
        for (int h = 100; h < 400; h += 10) {
            view->resize(w, h);
            BOBUIest::qWait(1);
            QCoreApplication::processEvents();
        }
    }
}

void tst_QItemView::visualRect_data()
{
    setupWithNoTestData();
}

void tst_QItemView::visualRect()
{
    QFETCH(QString, viewType);
    QFETCH(QAbstractItemView::ScrollMode, vscroll);
    QFETCH(QAbstractItemView::ScrollMode, hscroll);

    view = testViews->createView(viewType);
    QVERIFY(view);
    view->setVerticalScrollMode(vscroll);
    view->setHorizontalScrollMode(hscroll);
    QCOMPARE(view->visualRect(QModelIndex()), QRect());

    // Add model
    view->setModel(treeModel);
    QCOMPARE(view->visualRect(QModelIndex()), QRect());

    QModelIndex topIndex = treeModel->index(0,0);

    QFETCH(bool, displays);
    if (!displays){
        QCOMPARE(view->visualRect(topIndex), QRect());
        return;
    }

    QVERIFY(view->visualRect(topIndex) != QRect());
    view->show();
    QVERIFY(view->visualRect(topIndex) != QRect());

    QCOMPARE(topIndex, view->indexAt(view->visualRect(topIndex).center()));
    QCOMPARE(topIndex, view->indexAt(view->visualRect(topIndex).bottomLeft()));
    QCOMPARE(topIndex, view->indexAt(view->visualRect(topIndex).bottomRight()));
    QCOMPARE(topIndex, view->indexAt(view->visualRect(topIndex).topLeft()));
    QCOMPARE(topIndex, view->indexAt(view->visualRect(topIndex).topRight()));

    testViews->hideIndexes(view);
    QModelIndex hiddenIndex = treeModel->index(1, 0);
    QCOMPARE(view->visualRect(hiddenIndex), QRect());
}

void tst_QItemView::walkScreen(QAbstractItemView *view)
{
    QModelIndex hiddenIndex = view->model() ? view->model()->index(1, 0) : QModelIndex();
    int width = view->width();
    int height = view->height();
    for (int w = 0; w < width; ++w)
    {
        for (int h = 0; h < height; ++h)
        {
            QPoint point(w, h);
            QModelIndex index = view->indexAt(point);

            // If we have no model then we should *never* get a valid index
            if (!view->model() || !view->isVisible())
                QVERIFY(!index.isValid());
            // index should not be the hidden one
            if (hiddenIndex.isValid())
                QVERIFY(hiddenIndex != index);
            // If we are valid then check the visualRect for that index
            if (index.isValid()){
                QRect visualRect = view->visualRect(index);
                if (!visualRect.contains(point))
                    qDebug() << point << visualRect;
                QVERIFY(visualRect.contains(point));
            }
        }
    }
}

void walkIndex(const QModelIndex &index, const QAbstractItemView *view)
{
    const QRect visualRect = view->visualRect(index);
    const int width = visualRect.width();
    const int height = visualRect.height();

    if (width == 0 || height == 0)
        return;

    const auto widths = (width < 2) ? QList<int>({ 0, 1 })
                                    : QList<int>({ 0, 1, width / 2, width - 2, width - 1 });
    const auto heights = (height < 2) ? QList<int>({ 0, 1 })
                                      : QList<int>({ 0, 1, height / 2, height - 2, height - 1 });
    for (int w : widths)
    {
        for (int h : heights)
        {
            const QPoint point(visualRect.x() + w, visualRect.y() + h);
            const auto idxAt = view->indexAt(point);
            if (idxAt != index)
                qDebug() << "index" << index << "visualRect" << visualRect << point << view->indexAt(point);
            QCOMPARE(idxAt, index);
        }
    }

}

/*!
    A model that returns an index of parent X should also return X when asking
    for the parent of the index.

    This recursive function does pretty extensive testing on the whole model in an
    effort to catch edge cases.

    This function assumes that rowCount(), columnCount() and index() work.  If they have
    a bug it will point it out, but the above tests should have already found the basic bugs
    because it is easier to figure out the problem in those tests then this one.
 */
void checkChildren(const QAbstractItemView *currentView, const QModelIndex &parent = QModelIndex(), int currentDepth = 0)
{
    QAbstractItemModel *currentModel = currentView->model();

    int rows = currentModel->rowCount(parent);
    int columns = currentModel->columnCount(parent);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            QModelIndex index = currentModel->index(r, c, parent);
            walkIndex(index, currentView);
            if (BOBUIest::currentTestFailed())
                return;

            // recursivly go down
            if (currentModel->hasChildren(index) && currentDepth < 2) {
                checkChildren(currentView, index, ++currentDepth);
                // Because this is recursive we will return at the first failure rather then
                // reporting it over and over
                if (BOBUIest::currentTestFailed())
                    return;
            }
        }
    }
}


void tst_QItemView::indexAt_data()
{
    setupWithNoTestData();
}

void tst_QItemView::indexAt()
{
    QFETCH(QString, viewType);
    QFETCH(QAbstractItemView::ScrollMode, vscroll);
    QFETCH(QAbstractItemView::ScrollMode, hscroll);

    view = testViews->createView(viewType);
    QVERIFY(view);
    view->setVerticalScrollMode(vscroll);
    view->setHorizontalScrollMode(hscroll);
    view->show();
    view->setModel(treeModel);
    checkChildren(view);

    QModelIndex index = view->model()->index(0, 0);
    while (view->model()->hasChildren(index))
        index = view->model()->index(0, 0, index);
    QCOMPARE(view->model()->hasChildren(index), false);
    QVERIFY(index.isValid());
    view->setRootIndex(index);
    //qDebug() << view->indexAt(QPoint(view->width()/2, view->height()/2)) << view->rootIndex();
    QPoint p(1, view->height()/2);
    QModelIndex idx = view->indexAt(p);
    QCOMPARE(idx, QModelIndex());
}

void tst_QItemView::scrollTo_data()
{
    setupWithNoTestData();
}

void tst_QItemView::scrollTo()
{
    QFETCH(QString, viewType);
    QFETCH(QAbstractItemView::ScrollMode, vscroll);
    QFETCH(QAbstractItemView::ScrollMode, hscroll);

    view = testViews->createView(viewType);
    QVERIFY(view);
    view->setVerticalScrollMode(vscroll);
    view->setHorizontalScrollMode(hscroll);
    view->setModel(treeModel);
    view->show();

    QModelIndex parent;
    for (int row = 0; row < treeModel->rowCount(parent); ++row) {
        for (int column = 0; column < treeModel->columnCount(parent); ++column) {
            QModelIndex idx = treeModel->index(row, column, parent);
            view->scrollTo(idx);
            QRect rect = view->visualRect(idx);
            view->scrollTo(idx);
            QCOMPARE(rect, view->visualRect(idx));
        }
    }

    QModelIndex idx = treeModel->index(0, 0, parent);
    view->scrollTo(idx);
    QRect rect = view->visualRect(idx);
    view->scrollToBottom();
    view->scrollTo(idx);
    QCOMPARE(rect, view->visualRect(idx));
}

void tst_QItemView::moveCursor_data()
{
    setupWithNoTestData();
}

struct Event
{
    Event(BobUI::Key k, const QModelIndex &s, const QModelIndex &e, const QString &n)
        : key(k), start(s), end(e), name(n){}
    BobUI::Key key;
    QModelIndex start;
    QModelIndex end;
    QString name;
};


void tst_QItemView::moveCursor()
{
    QFETCH(QString, viewType);
    view = testViews->createView(viewType);
    QVERIFY(view);
    if (view->objectName() == "QHeaderView")
        return;

    view->setModel(treeModel);
    testViews->hideIndexes(view);
    view->resize(100, 100);

    QModelIndex invalidIndex = QModelIndex();
    QModelIndex firstRow = treeModel->index(0, 0);
    QModelIndex hiddenRowT = treeModel->index(1, 0);
    QModelIndex disabledRowT = treeModel->index(2, 0);
    QModelIndex secondRow = treeModel->index(3, 0);

    QModelIndex secondToLastRow = treeModel->index(treeModel->rowCount() - 4, 0);
    QModelIndex disabledRowB = treeModel->index(treeModel->rowCount() - 3, 0);
    QModelIndex hiddenRowB = treeModel->index(treeModel->rowCount() - 2, 0);
    QModelIndex lastRow = treeModel->index(treeModel->rowCount() - 1, 0);

    QStack<Event> events;

    events.push(Event(BobUI::Key_Up, invalidIndex,      firstRow, "inv, first"));
    events.push(Event(BobUI::Key_Up, hiddenRowT,        firstRow, "hid, first"));
    events.push(Event(BobUI::Key_Up, disabledRowT,      firstRow, "dis, first"));
    events.push(Event(BobUI::Key_Up, firstRow,          firstRow, "first, first"));
    events.push(Event(BobUI::Key_Up, secondRow,         firstRow, "sec, first"));
    events.push(Event(BobUI::Key_Up, hiddenRowB,        firstRow, "hidB, first"));
    events.push(Event(BobUI::Key_Up, disabledRowB,      secondToLastRow, "disB, secLast"));
    events.push(Event(BobUI::Key_Up, lastRow,           secondToLastRow, "last, secLast"));

    events.push(Event(BobUI::Key_Down, invalidIndex,    firstRow, "inv, first"));
    events.push(Event(BobUI::Key_Down, hiddenRowT,      firstRow, "hid, first"));
    events.push(Event(BobUI::Key_Down, disabledRowT,    secondRow, "dis, sec"));
    events.push(Event(BobUI::Key_Down, firstRow,        secondRow, "first, sec"));
    events.push(Event(BobUI::Key_Down, secondToLastRow, lastRow, "secLast, last" ));
    events.push(Event(BobUI::Key_Down, disabledRowB,    lastRow, "disB, last"));
    events.push(Event(BobUI::Key_Down, hiddenRowB,      firstRow, "hidB, first"));
    events.push(Event(BobUI::Key_Down, lastRow,         lastRow, "last, last"));

    events.push(Event(BobUI::Key_Home, invalidIndex,    firstRow, "inv, first"));
    events.push(Event(BobUI::Key_End, invalidIndex,     firstRow, "inv, first"));

    if (view->objectName() == "BOBUIableView") {
        // In a table we move to the first/last column
        events.push(Event(BobUI::Key_Home, hiddenRowT,      firstRow, "hid, first"));
        events.push(Event(BobUI::Key_Home, disabledRowT,    disabledRowT, "dis, dis"));
        events.push(Event(BobUI::Key_Home, firstRow,        firstRow, "first, first"));
        events.push(Event(BobUI::Key_Home, secondRow,       secondRow, "sec, sec"));
        events.push(Event(BobUI::Key_Home, disabledRowB,    disabledRowB, "disB, disB"));
        events.push(Event(BobUI::Key_Home, hiddenRowB,      firstRow, "hidB, first"));
        events.push(Event(BobUI::Key_Home, secondToLastRow, secondToLastRow, "secLast, secLast"));
        events.push(Event(BobUI::Key_Home, lastRow,         lastRow, "last, last"));

        int col = treeModel->columnCount() - 1;
        events.push(Event(BobUI::Key_End, hiddenRowT,      firstRow, "hidT, hidT"));
        events.push(Event(BobUI::Key_End, disabledRowT,    disabledRowT, "disT, disT"));
        events.push(Event(BobUI::Key_End, firstRow,        firstRow.sibling(firstRow.row(), col), "first, first_C"));
        events.push(Event(BobUI::Key_End, secondRow,       secondRow.sibling(secondRow.row(), col), "sec, sec_C"));
        events.push(Event(BobUI::Key_End, disabledRowB,    disabledRowB, "disB, disB"));
        events.push(Event(BobUI::Key_End, hiddenRowB,      firstRow, "hidB, hidB"));
        events.push(Event(BobUI::Key_End, secondToLastRow, secondToLastRow.sibling(secondToLastRow.row(), col), "secLast, secLast_C"));
        events.push(Event(BobUI::Key_End, lastRow,         lastRow.sibling(lastRow.row(), col), "last, last_C"));
    } else {
        events.push(Event(BobUI::Key_Home, hiddenRowT,      firstRow, "hid, first"));
        events.push(Event(BobUI::Key_Home, disabledRowT,    firstRow, "dis, first"));
        events.push(Event(BobUI::Key_Home, firstRow,        firstRow, "first, first"));
        events.push(Event(BobUI::Key_Home, secondRow,       firstRow, "sec, first"));
        events.push(Event(BobUI::Key_Home, disabledRowB,    firstRow, "disB, first"));
        events.push(Event(BobUI::Key_Home, hiddenRowB,      firstRow, "hidB, first"));
        events.push(Event(BobUI::Key_Home, secondToLastRow, firstRow, "sec, first"));
        events.push(Event(BobUI::Key_Home, lastRow,         firstRow, "last, first"));

        events.push(Event(BobUI::Key_End, hiddenRowT,       firstRow, "hid, last"));
        events.push(Event(BobUI::Key_End, disabledRowT,     lastRow, "dis, last"));
        events.push(Event(BobUI::Key_End, firstRow,         lastRow, "first, last"));
        events.push(Event(BobUI::Key_End, secondRow,        lastRow, "sec, last"));
        events.push(Event(BobUI::Key_End, disabledRowB,     lastRow, "disB, last"));
        events.push(Event(BobUI::Key_End, hiddenRowB,       firstRow, "hidB, last"));
        events.push(Event(BobUI::Key_End, secondToLastRow,  lastRow, "sec, last"));
        events.push(Event(BobUI::Key_End, lastRow,          lastRow, "last, last"));
    }

    events.push(Event(BobUI::Key_PageDown, invalidIndex,    firstRow, "inv, first"));
    events.push(Event(BobUI::Key_PageDown, firstRow,        QModelIndex(), "first, x"));
    events.push(Event(BobUI::Key_PageDown, secondRow,       QModelIndex(), "sec, x"));
    events.push(Event(BobUI::Key_PageDown, hiddenRowT,      QModelIndex(), "hid, x"));
    events.push(Event(BobUI::Key_PageDown, disabledRowT,    QModelIndex(), "dis, x"));
    events.push(Event(BobUI::Key_PageDown, disabledRowB,    lastRow, "disB, last"));
    events.push(Event(BobUI::Key_PageDown, hiddenRowB,      lastRow, "hidB, last"));
    events.push(Event(BobUI::Key_PageDown, secondToLastRow, lastRow, "secLast, last"));
    events.push(Event(BobUI::Key_PageDown, lastRow,         lastRow, "last, last"));

    events.push(Event(BobUI::Key_PageUp, invalidIndex,    firstRow, "inv, first"));
    events.push(Event(BobUI::Key_PageUp, firstRow,        firstRow, "first, first"));
    events.push(Event(BobUI::Key_PageUp, secondRow,       firstRow, "sec, first"));
    events.push(Event(BobUI::Key_PageUp, secondToLastRow, QModelIndex(), "secLast, x"));
    events.push(Event(BobUI::Key_PageUp, lastRow,         QModelIndex(), "last, x"));

    if (view->objectName() == "BOBUIableView") {
        events.push(Event(BobUI::Key_Left, firstRow,                      firstRow, "first_0, first"));
        events.push(Event(BobUI::Key_Left, firstRow.sibling(0, 1),        firstRow, "first_1, first"));
        events.push(Event(BobUI::Key_Left, firstRow.sibling(0, 2),        firstRow, "first_2, first"));
        events.push(Event(BobUI::Key_Left, firstRow.sibling(0, 3),        firstRow, "first_3, first"));
        events.push(Event(BobUI::Key_Left, secondRow,        secondRow, "sec, sec"));

        events.push(Event(BobUI::Key_Right, firstRow,                      firstRow.sibling(0, 3), "first, first_3"));
        events.push(Event(BobUI::Key_Right, firstRow.sibling(0, 1),        firstRow, "first_1, first"));
        events.push(Event(BobUI::Key_Right, firstRow.sibling(0, 2),        firstRow.sibling(0, 3), "first_2, first_3"));
        events.push(Event(BobUI::Key_Right, firstRow.sibling(0, treeModel->columnCount()-1), firstRow.sibling(0, treeModel->columnCount()-1), "first_3, sec"));
    }
}

BOBUIEST_MAIN(tst_QItemView)
#include "tst_qitemview.moc"
