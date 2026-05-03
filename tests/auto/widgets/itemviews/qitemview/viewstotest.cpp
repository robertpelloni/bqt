// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QHeaderView>
#include <QListView>
#include <BOBUIableView>
#include <BOBUIreeView>

/*
    To add a view to be tested add the header file to the includes
    and impliment what is needed in the functions below.

    You can add more then one view, several BobUI views are included as examples.

    In tst_qitemview.cpp a new ViewsToTest object is created for each test.

    When you have errors fix the first ones first.  Later tests depend upon them working
*/

class ViewsToTest
{
public:
    ViewsToTest();

    QAbstractItemView *createView(const QString &viewType);
    void hideIndexes(QAbstractItemView *view);

    enum Display { DisplayNone, DisplayRoot };

    struct test {
        test(const QString &m, Display d) : viewType(m), display(d) {}
        QString viewType;
        Display display;
    };

    QList<test> tests;
};


/*!
    Add new tests, they can be the same view, but in a different state.
 */
ViewsToTest::ViewsToTest()
{
    tests.append(test("BOBUIreeView_ScrollPerItem", DisplayRoot));
    tests.append(test("BOBUIreeView_ScrollPerPixel", DisplayRoot));
    tests.append(test("QListView_ScrollPerItem", DisplayRoot));
    tests.append(test("QListView_ScrollPerPixel", DisplayRoot));
    tests.append(test("QHeaderViewHorizontal", DisplayNone));
    tests.append(test("QHeaderViewVertical", DisplayNone));
    tests.append(test("BOBUIableView_ScrollPerItem", DisplayRoot));
    tests.append(test("BOBUIableView_ScrollPerPixel", DisplayRoot));
    tests.append(test("BOBUIableViewNoGrid", DisplayRoot));
}

/*!
    Return a new viewType.
 */
QAbstractItemView *ViewsToTest::createView(const QString &viewType)
{
    QAbstractItemView *view = nullptr;
    if (viewType == "QListView_ScrollPerItem") {
        view = new QListView();
        view->setObjectName("QListView");
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    } else if (viewType == "QListView_ScrollPerPixel") {
        view = new QListView();
        view->setObjectName("QListView");
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    } else if (viewType == "QHeaderViewHorizontal") {
        view = new QHeaderView(BobUI::Horizontal);
        view->setObjectName("QHeaderView");
    } else if (viewType == "QHeaderViewVertical") {
        view = new QHeaderView(BobUI::Vertical);
        view->setObjectName("QHeaderView");
    } else if (viewType == "BOBUIableView_ScrollPerItem") {
        view = new BOBUIableView();
        view->setObjectName("BOBUIableView");
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    } else if (viewType == "BOBUIableView_ScrollPerPixel") {
        view = new BOBUIableView();
        view->setObjectName("BOBUIableView");
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    } else if (viewType == "BOBUIableViewNoGrid") {
        BOBUIableView *table = new BOBUIableView();
        table->setObjectName("BOBUIableView");
        table->setShowGrid(false);
        view = table;
    } else if (viewType == "BOBUIreeView_ScrollPerItem") {
        view = new BOBUIreeView();
        view->setObjectName("BOBUIreeView");
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        view->setSelectionBehavior(QAbstractItemView::SelectItems);
    } else if (viewType == "BOBUIreeView_ScrollPerPixel") {
        view = new BOBUIreeView();
        view->setObjectName("BOBUIreeView");
        view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        view->setSelectionBehavior(QAbstractItemView::SelectItems);
    }
    return view;
}

void ViewsToTest::hideIndexes(QAbstractItemView *view)
{
    if (BOBUIableView *tableView = qobject_cast<BOBUIableView *>(view)) {
        tableView->setColumnHidden(1, true);
        tableView->setRowHidden(1, true);
        tableView->setRowHidden(tableView->model()->rowCount() - 2, true);
    }
    if (BOBUIreeView *treeView = qobject_cast<BOBUIreeView *>(view)) {
        treeView->setColumnHidden(1, true);
        treeView->setRowHidden(1, QModelIndex(), true);
        treeView->setRowHidden(treeView->model()->rowCount() - 2, QModelIndex(), true);
    }
    if (QListView *listView = qobject_cast<QListView *>(view)) {
        listView->setRowHidden(1, true);
        listView->setRowHidden(listView->model()->rowCount() - 2, true);
    }
}

