// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <QApplication>
#include <QHeaderView>
#include <QLineEdit>
#include <QScopeGuard>
#include <QScrollBar>
#include <QSignalSpy>
#include <QStyledItemDelegate>
#include <BOBUIreeWidget>
#include <BOBUIreeWidgetItemIterator>
#include <BOBUIest>

class tst_BOBUIreeWidget : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIreeWidget() = default;

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void getSetCheck();
    void addTopLevelItem();
    void currentItem_data();
    void currentItem();
    void editItem_data();
    void editItem();
    void takeItem_data();
    void takeItem();
    void removeChild_data();
    void removeChild();
    void setItemHidden();
    void setItemHidden2();
    void selectedItems_data();
    void selectedItems();
    void itemAssignment();
    void clone_data();
    void clone();
    void expand_data();
    void expand();
    void checkState_data();
    void checkState();
    void findItems_data();
    void findItems();
    void findItemsInColumn();
    void sortItems_data();
    void sortItems();
    void deleteItems_data();
    void deleteItems();
    void itemAboveOrBelow();
    void itemStreaming_data();
    void itemStreaming();
    void insertTopLevelItems_data();
    void insertTopLevelItems();
    void keyboardNavigation();
    void keyboardNavigationWithHidden();
    void scrollToItem();
    void setSortingEnabled();
    void match();
    void columnCount();
    void setHeaderLabels();
    void setHeaderItem();
    void itemWidget_data();
    void itemWidget();
    void insertItemsWithSorting_data();
    void insertItemsWithSorting();
    void insertExpandedItemsWithSorting_data();
    void insertExpandedItemsWithSorting();
    void changeDataWithSorting_data();
    void changeDataWithSorting();
    void changeDataWithStableSorting_data();
    void changeDataWithStableSorting();
    void sizeHint_data();
    void sizeHint();

    void sortedIndexOfChild_data();
    void sortedIndexOfChild();
    void defaultRowSizes();

    void task191552_rtl();
    void task203673_selection();
    void rootItemFlags();
    void task218661_setHeaderData();
    void task245280_sortChildren();
    void task253109_itemHeight();

    void nonEditableTristate();

    // BOBUIreeWidgetItem
    void itemOperatorLessThan();
    void addChild();
    void setData();
    void enableDisable();

    void expandAndCallapse();
    void itemData();
    void setDisabled();
    void setSpanned();
    void removeSelectedItem();
    void removeCurrentItem();
    void removeCurrentItem_task186451();
    void randomExpand();
    void crashTest();
    void sortAndSelect();

    void task206367_duplication();
    void selectionOrder();

    void setSelectionModel();
    void task217309();
    void setCurrentItemExpandsParent();
    void task239150_editorWidth();
    void setTextUpdate();
    void taskBOBUIBUG2844_visualItemRect();
    void setChildIndicatorPolicy();

    void taskBOBUIBUG_34717_collapseAtBottom();
    void task20345_sortChildren();
    void getMimeDataWithInvalidItem();
    void testVisualItemRect();
    void reparentHiddenItem();
    void persistentChildIndex();
    void createPersistentOnLayoutAboutToBeChanged();
    void createPersistentOnLayoutAboutToBeChangedAutoSort();
    void clearItemData();
#if BOBUI_CONFIG(draganddrop)
    void supportedDragActions();
#endif

public slots:
    void itemSelectionChanged();
    void emitDataChanged();

public:
    class PublicTreeWidget : public BOBUIreeWidget
    {
    public:
        using BOBUIreeWidget::indexFromItem;
        using BOBUIreeWidget::mimeData;
        using BOBUIreeWidget::sizeHintForColumn;
        void deleteCurrent() { delete currentItem(); }
    };

    class PublicTreeItem : public BOBUIreeWidgetItem
    {
    public:
        using BOBUIreeWidgetItem::BOBUIreeWidgetItem;
        using BOBUIreeWidgetItem::emitDataChanged;
    };

private:
    PublicTreeWidget *testWidget = nullptr;
};

// Testing get/set functions
void tst_BOBUIreeWidget::getSetCheck()
{
    BOBUIreeWidget obj1;
    // int BOBUIreeWidget::columnCount()
    // void BOBUIreeWidget::setColumnCount(int)
    obj1.setColumnCount(0);
    QCOMPARE(obj1.columnCount(), 0);

    obj1.setColumnCount(std::numeric_limits<int>::min());
    QCOMPARE(obj1.columnCount(), 0);

    //obj1.setColumnCount(INT_MAX);
    //QCOMPARE(obj1.columnCount(), INT_MAX);
    // Since setColumnCount allocates memory, there is no way this will succeed

    obj1.setColumnCount(100);
    QCOMPARE(obj1.columnCount(), 100);

    // BOBUIreeWidgetItem * BOBUIreeWidget::headerItem()
    // void BOBUIreeWidget::setHeaderItem(BOBUIreeWidgetItem *)
    BOBUIreeWidgetItem *var2 = new BOBUIreeWidgetItem();
    obj1.setHeaderItem(var2);
    QCOMPARE(obj1.headerItem(), var2);

    obj1.setHeaderItem(nullptr);
//    QCOMPARE(obj1.headerItem(), nullptr);

    // BOBUIreeWidgetItem * BOBUIreeWidget::currentItem()
    // void BOBUIreeWidget::setCurrentItem(BOBUIreeWidgetItem *)
    BOBUIreeWidgetItem *var3 = new BOBUIreeWidgetItem(&obj1);
    obj1.setCurrentItem(var3);
    QCOMPARE(obj1.currentItem(), var3);

    obj1.setCurrentItem(nullptr);
    QCOMPARE(obj1.currentItem(), nullptr);
}

using IntList = QList<int>;
using ListIntList = QList<IntList>;
using PersistentModelIndexVec = QList<QPersistentModelIndex>;
using TreeItem = BOBUIreeWidgetItem;
using TreeItemList = QList<TreeItem *>;

Q_DECLARE_METATYPE(BobUI::Orientation)
Q_DECLARE_METATYPE(BOBUIreeWidgetItem*)
Q_DECLARE_METATYPE(TreeItemList)

void tst_BOBUIreeWidget::initTestCase()
{
    qMetaTypeId<BobUI::Orientation>();
    qRegisterMetaType<BOBUIreeWidgetItem*>("BOBUIreeWidgetItem*");
    qRegisterMetaType<QList<QPersistentModelIndex>>("QList<QPersistentModelIndex>");
    qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint");

    testWidget = new PublicTreeWidget();
    testWidget->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(testWidget));
}

void tst_BOBUIreeWidget::cleanupTestCase()
{
    testWidget->hide();
    delete testWidget;
}

void tst_BOBUIreeWidget::init()
{
    testWidget->clear();
    testWidget->setColumnCount(2);
}

void tst_BOBUIreeWidget::cleanup()
{
}

TreeItem *operator<<(TreeItem *parent, const TreeItemList &children)
{
    for (TreeItem *child : children)
        parent->addChild(child);
    return parent;
}

static void populate(BOBUIreeWidget *widget, const TreeItemList &topLevelItems,
                     TreeItem *headerItem = nullptr)
{
    widget->clear();
    widget->setHeaderItem(headerItem);
    for (TreeItem *item : topLevelItems)
        widget->addTopLevelItem(item);
}

void tst_BOBUIreeWidget::addTopLevelItem()
{
    BOBUIreeWidget tree;
    QCOMPARE(tree.topLevelItemCount(), 0);

    // try to add 0
    tree.addTopLevelItem(nullptr);
    QCOMPARE(tree.topLevelItemCount(), 0);
    QCOMPARE(tree.indexOfTopLevelItem(nullptr), -1);

    // add one at a time
    QList<TreeItem *> tops;
    for (int i = 0; i < 10; ++i) {
        TreeItem *ti = new TreeItem();
        QCOMPARE(tree.indexOfTopLevelItem(ti), -1);
        tree.addTopLevelItem(ti);
        QCOMPARE(tree.topLevelItemCount(), i+1);
        QCOMPARE(tree.topLevelItem(i), ti);
        QCOMPARE(tree.topLevelItem(-1), nullptr);
        QCOMPARE(tree.indexOfTopLevelItem(ti), i);
        QCOMPARE(ti->parent(), nullptr);
        tree.addTopLevelItem(ti);
        QCOMPARE(tree.topLevelItemCount(), i+1);
        tops.append(ti);
    }

    // delete one at a time
    while (!tops.isEmpty()) {
        TreeItem *ti = tops.takeFirst();
        delete ti;
        QCOMPARE(tree.topLevelItemCount(), tops.size());
        for (int i = 0; i < tops.size(); ++i)
            QCOMPARE(tree.topLevelItem(i), tops.at(i));
    }

    // add none
    {
        int count = tree.topLevelItemCount();
        tree.addTopLevelItems(tops);
        QCOMPARE(tree.topLevelItemCount(), count);
    }

    // add many at a time
    {
        const int count = 10;
        for (int i = 0; i < 100; i += count) {
            tops.clear();
            for (int j = 0; j < count; ++j)
                tops << new TreeItem(QStringList(QString::number(j)));
            tree.addTopLevelItems(tops);
            QCOMPARE(tree.topLevelItemCount(), count + i);
            for (int j = 0; j < count; ++j)
                QCOMPARE(tree.topLevelItem(i+j), tops.at(j));

            tree.addTopLevelItems(tops);
            QCOMPARE(tree.topLevelItemCount(), count + i);
        }
    }

    // insert
    {
        tops.clear();
        for (int i = 0; i < 10; ++i)
            tops << new TreeItem();
        int count = tree.topLevelItemCount();
        tree.insertTopLevelItems(count, tops);
        QCOMPARE(tree.topLevelItemCount(), count + 10);
    }

    // invalid insert
    {
        tops.clear();
        const auto sg = qScopeGuard([&] { qDeleteAll(std::exchange(tops, {})); });
        for (int i = 0; i < 10; ++i)
            tops << new TreeItem();
        int count = tree.topLevelItemCount();
        tree.insertTopLevelItems(100000, tops); // should be a no-op
        QCOMPARE(tree.topLevelItemCount(), count);
    }
}

void tst_BOBUIreeWidget::currentItem_data()
{
    BOBUIest::addColumn<TreeItemList>("topLevelItems");

    BOBUIest::newRow("only top-level items, 2 columns")
        << (TreeItemList()
            << new TreeItem({"a", "b"})
            << new TreeItem({"c", "d"}));
    TreeItemList lst;
    lst << (new TreeItem({"a", "b"})
        << (TreeItemList()
            << new TreeItem({"c", "d"})
            << new TreeItem({"c", "d"})
            )
            )
        << (new TreeItem({"e", "f"})
            << (TreeItemList()
                << new TreeItem({"g", "h"})
                << new TreeItem({"g", "h"})
                )
            );
    BOBUIest::newRow("hierarchy, 2 columns") << lst;
}

void tst_BOBUIreeWidget::currentItem()
{
    QFETCH(TreeItemList, topLevelItems);

    BOBUIreeWidget tree;
    tree.show();
    populate(&tree, topLevelItems, new TreeItem({"1", "2"}));
    BOBUIreeWidgetItem *previous = nullptr;
    for (int x = 0; x < 2; ++x) {
        tree.setSelectionBehavior(x ? QAbstractItemView::SelectItems
                                  : QAbstractItemView::SelectRows);
        QSignalSpy currentItemChangedSpy(
            &tree, &BOBUIreeWidget::currentItemChanged);
        QSignalSpy itemSelectionChangedSpy(
            &tree, &BOBUIreeWidget::itemSelectionChanged);

        BOBUIreeWidgetItemIterator it(&tree);
        // do all items
        while (BOBUIreeWidgetItem *item = (*it++)) {
            tree.setCurrentItem(item);
            QCOMPARE(tree.currentItem(), item);

            QCOMPARE(currentItemChangedSpy.size(), 1);
            QVariantList args = currentItemChangedSpy.takeFirst();
            QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
            QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(1)), previous);

            QCOMPARE(itemSelectionChangedSpy.size(), 1);
            itemSelectionChangedSpy.clear();

            previous = item;
            // do all columns
            for (int col = 0; col < item->columnCount(); ++col) {
                tree.setCurrentItem(item, col);
                QCOMPARE(tree.currentItem(), item);
                QCOMPARE(tree.currentColumn(), col);

                if (!currentItemChangedSpy.isEmpty()) {
                    // ### we get a currentItemChanged() when what really
                    // changed was just currentColumn(). Should it be like this?
                    QCOMPARE(currentItemChangedSpy.size(), 1);
                    QVariantList args = currentItemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(1)), item);
                    if (tree.selectionBehavior() == QAbstractItemView::SelectItems) {
                        QCOMPARE(itemSelectionChangedSpy.size(), 1);
                        itemSelectionChangedSpy.clear();
                    } else {
                        QCOMPARE(itemSelectionChangedSpy.size(), 0);
                    }
                }
            }
        }
    }

    // can't set the headerItem to be the current item
    tree.setCurrentItem(tree.headerItem());
    QCOMPARE(tree.currentItem(), nullptr);
}

void tst_BOBUIreeWidget::editItem_data()
{
    BOBUIest::addColumn<TreeItemList>("topLevelItems");

    {
        TreeItemList list;
        for (int i = 0; i < 10; i++) {
            TreeItem *item = new TreeItem(QStringList() << "col1" << "col2");
            if ((i & 1) == 0)
                item->setFlags(item->flags() | BobUI::ItemIsEditable);
            else
                item->setFlags(item->flags() & ~BobUI::ItemIsEditable);
            list << item;
        }
        BOBUIest::newRow("2 columns, only even items editable")
            << list;
    }
}

void tst_BOBUIreeWidget::editItem()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(TreeItemList, topLevelItems);

    BOBUIreeWidget tree;
    populate(&tree, topLevelItems, new TreeItem(QStringList() << "1" << "2"));
    tree.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&tree));

    QSignalSpy itemChangedSpy(&tree, &BOBUIreeWidget::itemChanged);

    BOBUIreeWidgetItemIterator it(&tree);
    while (BOBUIreeWidgetItem *item = (*it++)) {
        for (int col = 0; col < item->columnCount(); ++col) {
            if (!(item->flags() & BobUI::ItemIsEditable))
                BOBUIest::ignoreMessage(BobUIWarningMsg, "edit: editing failed");
            tree.editItem(item, col);
            QCoreApplication::processEvents();
            QCoreApplication::processEvents();
            QLineEdit *editor = tree.findChild<QLineEdit*>();
            if (editor) {
                QVERIFY(item->flags() & BobUI::ItemIsEditable);
                QCOMPARE(editor->selectedText(), editor->text());
                BOBUIest::keyClick(editor, BobUI::Key_A);
                BOBUIest::keyClick(editor, BobUI::Key_Enter);
                QCoreApplication::processEvents();
                QCOMPARE(itemChangedSpy.size(), 1);
                QVariantList args = itemChangedSpy.takeFirst();
                QCOMPARE(qvariant_cast<BOBUIreeWidgetItem *>(args.at(0)), item);
                QCOMPARE(qvariant_cast<int>(args.at(1)), col);
            } else {
                QVERIFY(!(item->flags() & BobUI::ItemIsEditable));
            }
        }
    }
}

void tst_BOBUIreeWidget::takeItem_data()
{
    BOBUIest::addColumn<int>("index");
    BOBUIest::addColumn<bool>("topLevel");
    BOBUIest::addColumn<bool>("outOfBounds");

    BOBUIest::newRow("First, topLevel") << 0 << true << false;
    BOBUIest::newRow("Last, topLevel") << 2 << true << false;
    BOBUIest::newRow("Middle, topLevel") << 1 << true << false;
    BOBUIest::newRow("Out of bounds, toplevel, (index: -1)") << -1 << true << true;
    BOBUIest::newRow("Out of bounds, toplevel, (index: 3)") << 3 << true << true;

    BOBUIest::newRow("First, child of topLevel") << 0 << false << false;
    BOBUIest::newRow("Last, child of topLevel") << 2 << false << false;
    BOBUIest::newRow("Middle, child of topLevel") << 1 << false << false;
    BOBUIest::newRow("Out of bounds, child of toplevel, (index: -1)") << -1 << false << true;
    BOBUIest::newRow("Out of bounds, child of toplevel, (index: 3)") << 3 << false << true;
}

void tst_BOBUIreeWidget::takeItem()
{
    QFETCH(int, index);
    QFETCH(bool, topLevel);
    QFETCH(bool, outOfBounds);

    for (int i = 0; i < 3; ++i) {
        BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(testWidget);
        top->setText(0, QStringLiteral("top") + QString::number(i));
        for (int j = 0; j < 3; ++j) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(top);
            child->setText(0, QStringLiteral("child") + QString::number(j));
        }
    }

    QCOMPARE(testWidget->topLevelItemCount(), 3);
    QCOMPARE(testWidget->topLevelItem(0)->childCount(), 3);

    if (topLevel) {
        int count = testWidget->topLevelItemCount();
        BOBUIreeWidgetItem *item = testWidget->takeTopLevelItem(index);
        if (outOfBounds) {
            QCOMPARE(item, nullptr);
            QCOMPARE(count, testWidget->topLevelItemCount());
        } else {
            QCOMPARE(item->text(0), QStringLiteral("top") + QString::number(index));
            QCOMPARE(count-1, testWidget->topLevelItemCount());
            delete item;
        }
    } else {
        int count = testWidget->topLevelItem(0)->childCount();
        BOBUIreeWidgetItem *item = testWidget->topLevelItem(0)->takeChild(index);
        if (outOfBounds) {
            QCOMPARE(item, nullptr);
            QCOMPARE(count, testWidget->topLevelItem(0)->childCount());
        } else {
            QCOMPARE(item->text(0), QStringLiteral("child") + QString::number(index));
            QCOMPARE(count-1, testWidget->topLevelItem(0)->childCount());
            delete item;
        }
    }
}

void tst_BOBUIreeWidget::removeChild_data()
{
    BOBUIest::addColumn<int>("childCount");
    BOBUIest::addColumn<int>("removeAt");

    BOBUIest::newRow("10 remove 3") << 10 << 3;
}

void tst_BOBUIreeWidget::removeChild()
{
    QFETCH(int, childCount);
    QFETCH(int, removeAt);

    const QScopedPointer<BOBUIreeWidgetItem> root(new BOBUIreeWidgetItem);
    for (int i = 0; i < childCount; ++i)
        new BOBUIreeWidgetItem(root.data(), QStringList(QString::number(i)));

    QCOMPARE(root->childCount(), childCount);
    for (int j = 0; j < childCount; ++j)
        QCOMPARE(root->child(j)->text(0), QString::number(j));

    const QScopedPointer<BOBUIreeWidgetItem> remove(root->child(removeAt));
    root->removeChild(remove.data());

    QCOMPARE(root->childCount(), childCount - 1);
    for (int k = 0; k < childCount; ++k) {
        if (k == removeAt)
            QCOMPARE(remove->text(0), QString::number(k));
        else if (k < removeAt)
            QCOMPARE(root->child(k)->text(0), QString::number(k));
        else if (k > removeAt)
            QCOMPARE(root->child(k - 1)->text(0), QString::number(k));
    }
}

void tst_BOBUIreeWidget::setItemHidden()
{
    BOBUIreeWidgetItem *parent = new BOBUIreeWidgetItem(testWidget);
    parent->setText(0, "parent");
    BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(parent);
    child->setText(0, "child");
    QVERIFY(child->parent());

    testWidget->expandItem(parent);
    testWidget->scrollToItem(child);

    QVERIFY(testWidget->visualItemRect(parent).isValid()
           && testWidget->viewport()->rect().intersects(testWidget->visualItemRect(parent)));
    QVERIFY(testWidget->visualItemRect(child).isValid()
           && testWidget->viewport()->rect().intersects(testWidget->visualItemRect(child)));

    QVERIFY(!parent->isHidden());
    QVERIFY(!child->isHidden());

    parent->setHidden(true);

    QVERIFY(!(testWidget->visualItemRect(parent).isValid()
             && testWidget->viewport()->rect().intersects(testWidget->visualItemRect(parent))));
    QVERIFY(!(testWidget->visualItemRect(child).isValid()
             && testWidget->viewport()->rect().intersects(testWidget->visualItemRect(child))));

    QVERIFY(parent->isHidden());
    QVERIFY(!child->isHidden());

    // From task 78670 (This caused an core dump)
    // Check if we can set an item visible if it already is visible.
    parent->setHidden(false);
    parent->setHidden(false);
    QVERIFY(!parent->isHidden());


    // hide, hide and then unhide.
    parent->setHidden(true);
    parent->setHidden(true);
    parent->setHidden(false);
    QVERIFY(!parent->isHidden());
}


void tst_BOBUIreeWidget::setItemHidden2()
{
    // From Task 78587
    const QStringList hl({"ID", "Desc"});
    testWidget->setColumnCount(hl.size());
    testWidget->setHeaderLabels(hl);
    testWidget->setSortingEnabled(true);

    BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(testWidget);
    top->setText(0, "ItemList");
    for (int i = 1; i <= 4; i++) {
        auto leaf = new BOBUIreeWidgetItem(top);
        leaf->setText(0, QString::number(i));
        leaf->setText(1, QStringLiteral("Item %1").arg(i));
    }

    if (testWidget->topLevelItemCount() > 0) {
        top = testWidget->topLevelItem(0);
        top->setExpanded(true);
    }

    if (testWidget->topLevelItemCount() > 0) {
        top = testWidget->topLevelItem(0);
        for (int i = 0; i < top->childCount(); i++) {
            auto leaf = top->child(i);
            if (leaf->text(0).toInt() % 2 == 0) {
                if (!leaf->isHidden())
                    leaf->setHidden(true);
            }
        }
    }
}


void tst_BOBUIreeWidget::selectedItems_data()
{
    BOBUIest::addColumn<int>("topLevel");
    BOBUIest::addColumn<int>("children");
    BOBUIest::addColumn<bool>("closeTopLevel");
    BOBUIest::addColumn<ListIntList>("selectedItems");
    BOBUIest::addColumn<ListIntList>("hiddenItems");
    BOBUIest::addColumn<ListIntList>("expectedItems");

    ListIntList selectedItems;
    ListIntList hiddenItems;
    ListIntList expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems
        << (IntList()
            << 0);
    expectedItems
        << (IntList() << 0);
    BOBUIest::newRow("2 top with 2 children, closed, top0 selected, no hidden")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems
        << (IntList()
            << 0 << 0);
    expectedItems
        << (IntList() << 0 << 0);
    BOBUIest::newRow("2 top with 2 children, closed, top0child0 selected, no hidden")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems
        << (IntList()
            << 0 << 0);
    expectedItems
        << (IntList()
            << 0 << 0);
    BOBUIest::newRow("2 top with 2 children, open, top0child0 selected, no hidden")
        << 2 << 2 << false << selectedItems << hiddenItems << expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems << (IntList() << 0);
    hiddenItems << (IntList() << 0);
    BOBUIest::newRow("2 top with 2 children, closed, top0 selected, top0 hidden")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems << (IntList() << 0 << 0);
    hiddenItems << (IntList() << 0);
    expectedItems << (IntList() << 0 << 0);
    BOBUIest::newRow("2 top with 2 children, closed, top0child0 selected, top0 hidden")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems
        << (IntList() << 0)
        << (IntList() << 0 << 0)
        << (IntList() << 0 << 1)
        << (IntList() << 1)
        << (IntList() << 1 << 0)
        << (IntList() << 1 << 1);
    expectedItems
        << (IntList() << 0)
        << (IntList() << 0 << 0)
        << (IntList() << 0 << 1)
        << (IntList() << 1)
        << (IntList() << 1 << 0)
        << (IntList() << 1 << 1);
    BOBUIest::newRow("2 top with 2 children, closed, all selected, no hidden")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;


    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems
        << (IntList() << 0)
        << (IntList() << 0 << 0)
        << (IntList() << 0 << 1)
        << (IntList() << 1)
        << (IntList() << 1 << 0)
        << (IntList() << 1 << 1);
    hiddenItems
        << (IntList() << 0);
    expectedItems
        //<< (IntList() << 0)
        << (IntList() << 0 << 0)
        << (IntList() << 0 << 1)
        << (IntList() << 1)
        << (IntList() << 1 << 0)
        << (IntList() << 1 << 1);
    BOBUIest::newRow("2 top with 2 children, closed, all selected, top0 hidden")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;

    selectedItems.clear(); hiddenItems.clear(); expectedItems.clear();
    selectedItems
        << (IntList() << 0)
        << (IntList() << 0 << 0)
        << (IntList() << 0 << 1)
        << (IntList() << 1)
        << (IntList() << 1 << 0)
        << (IntList() << 1 << 1);
    hiddenItems
        << (IntList() << 0 << 1)
        << (IntList() << 1);
    expectedItems
        << (IntList() << 0)
        << (IntList() << 0 << 0)
        //<< (IntList() << 0 << 1)
        //<< (IntList() << 1)
        << (IntList() << 1 << 0)
        << (IntList() << 1 << 1);

    BOBUIest::newRow("2 top with 2 children, closed, all selected, top0child1 and top1")
        << 2 << 2 << true << selectedItems << hiddenItems << expectedItems;

}

void tst_BOBUIreeWidget::selectedItems()
{
    QFETCH(int, topLevel);
    QFETCH(int, children);
    QFETCH(bool, closeTopLevel);
    QFETCH(const ListIntList, selectedItems);
    QFETCH(const ListIntList, hiddenItems);
    QFETCH(const ListIntList, expectedItems);

    // create items
    for (int t = 0; t < topLevel; ++t) {
        BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(testWidget);
        const QString topS = QLatin1String("top") + QString::number(t);
        top->setText(0, topS);
        for (int c = 0; c < children; ++c) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(top);
            child->setText(0, topS + QLatin1String("child") + QString::number(c));
        }
    }

    // set selected
    for (const auto &itemPath : selectedItems) {
        BOBUIreeWidgetItem *item = nullptr;
        for (int index : itemPath) {
            if (!item)
                item = testWidget->topLevelItem(index);
            else
                item = item->child(index);
        }
        item->setSelected(true);
    }

    // hide rows
    for (const auto &itemPath : hiddenItems) {
        BOBUIreeWidgetItem *item = nullptr;
        for (int index : itemPath) {
            if (!item)
                item = testWidget->topLevelItem(index);
            else
                item = item->child(index);
        }
        item->setHidden(true);
    }

    // open/close toplevel
    for (int i = 0; i < testWidget->topLevelItemCount(); ++i) {
        if (closeTopLevel)
            testWidget->collapseItem(testWidget->topLevelItem(i));
        else
            testWidget->expandItem(testWidget->topLevelItem(i));
    }

    // check selectedItems
    const auto sel = testWidget->selectedItems();
    QCOMPARE(sel.size(), expectedItems.size());
    for (const auto &itemPath : expectedItems) {
        BOBUIreeWidgetItem *item = nullptr;
        for (int index : itemPath) {
            if (!item)
                item = testWidget->topLevelItem(index);
            else
                item = item->child(index);
        }
        if (item)
            QVERIFY(sel.contains(item));
    }

    // compare isSelected
    for (int t = 0; t < testWidget->topLevelItemCount(); ++t) {
        BOBUIreeWidgetItem *top = testWidget->topLevelItem(t);
        if (top->isSelected() && !top->isHidden())
            QVERIFY(sel.contains(top));
        for (int c = 0; c < top->childCount(); ++c) {
            BOBUIreeWidgetItem *child = top->child(c);
            if (child->isSelected() && !child->isHidden())
                QVERIFY(sel.contains(child));
        }
    }

    // unselect
    for (const auto &itemPath : selectedItems) {
        BOBUIreeWidgetItem *item = nullptr;
        for (int index : itemPath) {
            if (!item)
                item = testWidget->topLevelItem(index);
            else
                item = item->child(index);
        }
        item->setSelected(false);
    }
    QCOMPARE(testWidget->selectedItems().size(), 0);
}

void tst_BOBUIreeWidget::itemAssignment()
{
    // create item with children and parent but not insert in the view
    BOBUIreeWidgetItem grandParent;
    BOBUIreeWidgetItem *parent = new BOBUIreeWidgetItem(&grandParent);
    parent->setText(0, "foo");
    parent->setText(1, "bar");
    for (int i = 0; i < 5; ++i) {
        BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(parent);
        child->setText(0, "bingo");
        child->setText(1, "bango");
    }
    QCOMPARE(parent->parent(), &grandParent);
    QVERIFY(!parent->treeWidget());
    QCOMPARE(parent->columnCount(), 2);
    QCOMPARE(parent->text(0), QString("foo"));
    QCOMPARE(parent->childCount(), 5);
    QCOMPARE(parent->child(0)->parent(), parent);

    // create item which is inserted in the widget
    BOBUIreeWidgetItem item(testWidget);
    item.setText(0, "baz");
    QVERIFY(!item.parent());
    QCOMPARE(item.treeWidget(), testWidget);
    QCOMPARE(item.columnCount(), 1);
    QCOMPARE(item.text(0), QString("baz"));
    QCOMPARE(item.childCount(), 0);

    // assign and test
    *parent = item;
    QCOMPARE(parent->parent(), &grandParent);
    QVERIFY(!parent->treeWidget());
    QCOMPARE(parent->columnCount(), 1);
    QCOMPARE(parent->text(0), QString("baz"));
    QCOMPARE(parent->childCount(), 5);
    QCOMPARE(parent->child(0)->parent(), parent);
}

void tst_BOBUIreeWidget::clone_data()
{
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<int>("topLevelIndex");
    BOBUIest::addColumn<int>("childIndex");
    BOBUIest::addColumn<QStringList>("topLevelText");
    BOBUIest::addColumn<QStringList>("childText");
    BOBUIest::addColumn<bool>("cloneChild");

    BOBUIest::newRow("clone parent with child") << 0 << 0 << 0
                                          << (QStringList() << "some text")
                                          << (QStringList() << "more text")
                                          << false;

    BOBUIest::newRow("clone child") << 0 << 0 << 0
                              << (QStringList() << "some text")
                              << (QStringList() << "more text")
                              << true;
}

void tst_BOBUIreeWidget::clone()
{
    QFETCH(int, column);
    QFETCH(int, topLevelIndex);
    QFETCH(int, childIndex);
    QFETCH(const QStringList, topLevelText);
    QFETCH(const QStringList, childText);
    QFETCH(bool, cloneChild);

    for (const QString &tl : topLevelText) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(testWidget);
        item->setText(column, tl);
        for (const QString &cl : childText) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(item);
            child->setText(column, cl);
        }
    }

    BOBUIreeWidgetItem *original = testWidget->topLevelItem(topLevelIndex);
    BOBUIreeWidgetItem *copy = original->clone();
    QCOMPARE(copy->text(column), original->text(column));
    QCOMPARE(copy->childCount(), original->childCount());
    QVERIFY(!copy->parent());
    QVERIFY(!copy->treeWidget());

    BOBUIreeWidgetItem *originalChild = original->child(childIndex);
    BOBUIreeWidgetItem *copiedChild = cloneChild ? originalChild->clone() : copy->child(childIndex);
    QVERIFY(copiedChild != originalChild);
    QCOMPARE(copiedChild->text(column), originalChild->text(column));
    QCOMPARE(copiedChild->childCount(), originalChild->childCount());
    QCOMPARE(copiedChild->parent(), cloneChild ? nullptr : copy);
    QVERIFY(!copiedChild->treeWidget());
    if (cloneChild)
        delete copiedChild;
    delete copy;
}

void tst_BOBUIreeWidget::expand_data()
{
    BOBUIest::addColumn<int>("topLevelIndex");
    BOBUIest::addColumn<int>("topLevelCount");
    BOBUIest::addColumn<int>("childIndex");
    BOBUIest::addColumn<int>("childCount");

    BOBUIest::newRow("the only test data for now") << 0 << 1 << 0 << 1;
}

void tst_BOBUIreeWidget::expand()
{
    QFETCH(int, topLevelIndex);
    QFETCH(int, topLevelCount);
    QFETCH(int, childIndex);
    QFETCH(int, childCount);

    for (int i = 0; i < topLevelCount; ++i) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(testWidget);
        for (int j = 0; j < childCount; ++j)
            new BOBUIreeWidgetItem(item);
    }

    BOBUIreeWidgetItem *topLevelItem = testWidget->topLevelItem(topLevelIndex);
    BOBUIreeWidgetItem *childItem = topLevelItem->child(childIndex);

    QVERIFY(!topLevelItem->isExpanded());
    topLevelItem->setExpanded(true);
    QVERIFY(topLevelItem->isExpanded());

    QVERIFY(!childItem->isExpanded());
    childItem->setExpanded(true);
    QVERIFY(childItem->isExpanded());

    QVERIFY(topLevelItem->isExpanded());
    topLevelItem->setExpanded(false);
    QVERIFY(!topLevelItem->isExpanded());

    QVERIFY(childItem->isExpanded());
    childItem->setExpanded(false);
    QVERIFY(!childItem->isExpanded());
}

void tst_BOBUIreeWidget::checkState_data()
{
}

void tst_BOBUIreeWidget::checkState()
{
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(testWidget);
    item->setCheckState(0, BobUI::Unchecked);
    BOBUIreeWidgetItem *firstChild = new BOBUIreeWidgetItem(item);
    firstChild->setCheckState(0, BobUI::Unchecked);
    BOBUIreeWidgetItem *seccondChild = new BOBUIreeWidgetItem(item);
    seccondChild->setCheckState(0, BobUI::Unchecked);

    QCOMPARE(item->checkState(0), BobUI::Unchecked);
    QCOMPARE(firstChild->checkState(0), BobUI::Unchecked);
    QCOMPARE(seccondChild->checkState(0), BobUI::Unchecked);

    firstChild->setCheckState(0, BobUI::Checked);
    QCOMPARE(item->checkState(0), BobUI::Unchecked);
    QCOMPARE(firstChild->checkState(0), BobUI::Checked);
    QCOMPARE(seccondChild->checkState(0), BobUI::Unchecked);

    item->setFlags(item->flags()|BobUI::ItemIsAutoTristate);
    QCOMPARE(item->checkState(0), BobUI::PartiallyChecked);
    QCOMPARE(firstChild->checkState(0), BobUI::Checked);
    QCOMPARE(seccondChild->checkState(0), BobUI::Unchecked);

    seccondChild->setCheckState(0, BobUI::Checked);
    QCOMPARE(item->checkState(0), BobUI::Checked);
    QCOMPARE(firstChild->checkState(0), BobUI::Checked);
    QCOMPARE(seccondChild->checkState(0), BobUI::Checked);

    firstChild->setCheckState(0, BobUI::Unchecked);
    seccondChild->setCheckState(0, BobUI::Unchecked);
    QCOMPARE(item->checkState(0), BobUI::Unchecked);
    QCOMPARE(firstChild->checkState(0), BobUI::Unchecked);
    QCOMPARE(seccondChild->checkState(0), BobUI::Unchecked);

    // Can't force the state to PartiallyChecked; state comes from children
    item->setCheckState(0, BobUI::PartiallyChecked);
    QCOMPARE(item->checkState(0), BobUI::Unchecked);
    QCOMPARE(firstChild->checkState(0), BobUI::Unchecked);
    QCOMPARE(seccondChild->checkState(0), BobUI::Unchecked);
}

void tst_BOBUIreeWidget::findItems_data()
{
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<QStringList>("topLevelText");
    BOBUIest::addColumn<QStringList>("childText");
    BOBUIest::addColumn<QString>("pattern");
    BOBUIest::addColumn<int>("resultCount");
    BOBUIest::addColumn<QStringList>("resultText");

    BOBUIest::newRow("find in toplevel")
        << 0
        << (QStringList() << "This is a text" << "This is another" << "This is the one")
        << (QStringList() << "A child" << "This is not the one" << "And yet another child")
        << "This is the one"
        << 1
        << (QStringList() << "This is the one");

    BOBUIest::newRow("find child")
        << 0
        << (QStringList() << "This is a text" << "This is another" << "This is the one")
        << (QStringList() << "A child" << "This is not the one" << "And yet another child")
        << "A child"
        << 3 // once for each branch
        << (QStringList() << "A child");

}

void tst_BOBUIreeWidget::findItems()
{
    QFETCH(int, column);
    QFETCH(const QStringList, topLevelText);
    QFETCH(const QStringList, childText);
    QFETCH(QString, pattern);
    QFETCH(int, resultCount);
    QFETCH(const QStringList, resultText);

    for (const QString &tl : topLevelText) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(testWidget);
        item->setText(column, tl);
        for (const QString &cl : childText) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(item);
            child->setText(column, cl);
        }
    }

    QList<BOBUIreeWidgetItem*> result = testWidget->findItems(pattern,
                                                           BobUI::MatchExactly|BobUI::MatchRecursive);
    QCOMPARE(result.size(), resultCount);

    for (int k = 0; k < result.size() && k < resultText.size(); ++k)
        QCOMPARE(result.at(k)->text(column), resultText.at(k));
}

void tst_BOBUIreeWidget::findItemsInColumn()
{
    // Create 5 root items.
    for (int i = 0; i < 5; i++)
        new BOBUIreeWidgetItem(testWidget, QStringList() << QString::number(i));

    // Create a child with two columns for each root item.
    for (int i = 0; i < 5; i++) {
        BOBUIreeWidgetItem * const  parent = testWidget->topLevelItem(i);
        new BOBUIreeWidgetItem(parent, QStringList() << QString::number(i * 10) << QString::number(i * 100));
    }

    // Recursively search column one for 400.
    QList<BOBUIreeWidgetItem*> items = testWidget->findItems("400", BobUI::MatchExactly|BobUI::MatchRecursive, 1);
    QCOMPARE(items.size(), 1);
}

void tst_BOBUIreeWidget::sortItems_data()
{
    BOBUIest::addColumn<int>("column");
    BOBUIest::addColumn<BobUI::SortOrder>("order");
    BOBUIest::addColumn<QStringList>("topLevelText");
    BOBUIest::addColumn<QStringList>("childText");
    BOBUIest::addColumn<QStringList>("topLevelResult");
    BOBUIest::addColumn<QStringList>("childResult");
    BOBUIest::addColumn<IntList>("expectedTopRows");
    BOBUIest::addColumn<IntList>("expectedChildRows");

    BOBUIest::newRow("ascending order")
        << 0
        << BobUI::AscendingOrder
        << (QStringList() << "c" << "d" << "a" << "b")
        << (QStringList() << "e" << "h" << "g" << "f")
        << (QStringList() << "a" << "b" << "c" << "d")
        << (QStringList() << "e" << "f" << "g" << "h")
        << (IntList() << 2 << 3 << 0 << 1)
        << (IntList() << 0 << 3 << 2 << 1);

    BOBUIest::newRow("descending order")
        << 0
        << BobUI::DescendingOrder
        << (QStringList() << "c" << "d" << "a" << "b")
        << (QStringList() << "e" << "h" << "g" << "f")
        << (QStringList() << "d" << "c" << "b" << "a")
        << (QStringList() << "h" << "g" << "f" << "e")
        << (IntList() << 1 << 0 << 3 << 2)
        << (IntList() << 3 << 0 << 1 << 2);
}

void tst_BOBUIreeWidget::sortItems()
{
    QFETCH(int, column);
    QFETCH(BobUI::SortOrder, order);
    QFETCH(QStringList, topLevelText);
    QFETCH(QStringList, childText);
    QFETCH(QStringList, topLevelResult);
    QFETCH(QStringList, childResult);
    QFETCH(IntList, expectedTopRows);
    QFETCH(IntList, expectedChildRows);
    testWidget->setSortingEnabled(false);

    for (const QString &tl : topLevelText) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(testWidget);
        item->setText(column, tl);
        for (const QString &cl : childText) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(item);
            child->setText(column, cl);
        }
    }

    QAbstractItemModel *model = testWidget->model();
    PersistentModelIndexVec tops;
    for (int r = 0; r < model->rowCount(QModelIndex()); ++r)
        tops.push_back(model->index(r, 0, QModelIndex()));
    PersistentModelIndexVec children;
    for (int s = 0; s < model->rowCount(tops.constFirst()); ++s)
        children.push_back(model->index(s, 0, tops.constFirst()));

    testWidget->sortItems(column, order);
    QCOMPARE(testWidget->sortColumn(), column);

    for (int k = 0; k < topLevelResult.size(); ++k) {
        BOBUIreeWidgetItem *item = testWidget->topLevelItem(k);
        QCOMPARE(item->text(column), topLevelResult.at(k));
        for (int l = 0; l < childResult.size(); ++l)
            QCOMPARE(item->child(l)->text(column), childResult.at(l));
    }

    for (int m = 0; m < tops.size(); ++m)
        QCOMPARE(tops.at(m).row(), expectedTopRows.at(m));
    for (int n = 0; n < children.size(); ++n)
        QCOMPARE(children.at(n).row(), expectedChildRows.at(n));
}

void tst_BOBUIreeWidget::deleteItems_data()
{
    BOBUIest::addColumn<int>("topLevelCount");
    BOBUIest::addColumn<int>("childCount");
    BOBUIest::addColumn<int>("grandChildCount");

    BOBUIest::addColumn<int>("deleteTopLevelCount");
    BOBUIest::addColumn<int>("deleteChildCount");
    BOBUIest::addColumn<int>("deleteGrandChildCount");

    BOBUIest::addColumn<int>("expectedTopLevelCount");
    BOBUIest::addColumn<int>("expectedChildCount");
    BOBUIest::addColumn<int>("expectedGrandChildCount");

    BOBUIest::addColumn<int>("persistentRow");
    BOBUIest::addColumn<int>("persistentColumn");
    BOBUIest::addColumn<bool>("persistentIsValid");

    BOBUIest::newRow("start with 10, delete 1")
        << 10 << 10 << 10
        << 1 << 1 << 1
        << 9 << 9 << 9
        << 0 << 0 << false;
    BOBUIest::newRow("start with 10, delete 5")
        << 10 << 10 << 10
        << 5 << 5 << 5
        << 5 << 5 << 5
        << 0 << 0 << false;
    BOBUIest::newRow("mixed")
        << 10 << 13 << 7
        << 3 << 7 << 4
        << 7 << 6 << 3
        << 0 << 0 << false;
    BOBUIest::newRow("all")
        << 10 << 10 << 10
        << 10 << 10 << 10
        << 0 << 0 << 0
        << 0 << 0 << false;
}

void tst_BOBUIreeWidget::deleteItems()
{
    QFETCH(int, topLevelCount);
    QFETCH(int, childCount);
    QFETCH(int, grandChildCount);

    QFETCH(int, deleteTopLevelCount);
    QFETCH(int, deleteChildCount);
    QFETCH(int, deleteGrandChildCount);

    QFETCH(int, expectedTopLevelCount);
    QFETCH(int, expectedChildCount);
    QFETCH(int, expectedGrandChildCount);

    QFETCH(int, persistentRow);
    QFETCH(int, persistentColumn);
    QFETCH(bool, persistentIsValid);

    for (int i = 0; i < topLevelCount; ++i) {
        BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(testWidget);
        for (int j = 0; j < childCount; ++j) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(top);
            for (int k = 0; k < grandChildCount; ++k) {
                new BOBUIreeWidgetItem(child);
            }
        }
    }

    QPersistentModelIndex persistent = testWidget->model()->index(persistentRow,
                                                                  persistentColumn);
    QVERIFY(persistent.isValid());

    BOBUIreeWidgetItem *top = testWidget->topLevelItem(0);
    BOBUIreeWidgetItem *child = top->child(0);

    for (int n = 0; n < deleteGrandChildCount; ++n)
        delete child->child(0);
    QCOMPARE(child->childCount(), expectedGrandChildCount);

    for (int m = 0; m < deleteChildCount; ++m)
        delete top->child(0);
    QCOMPARE(top->childCount(), expectedChildCount);

    for (int l = 0; l < deleteTopLevelCount; ++l)
        delete testWidget->topLevelItem(0);
    QCOMPARE(testWidget->topLevelItemCount(), expectedTopLevelCount);

    QCOMPARE(persistent.isValid(), persistentIsValid);
}


void tst_BOBUIreeWidget::itemAboveOrBelow()
{
    BOBUIreeWidget tw;
    tw.setColumnCount(1);
    BOBUIreeWidgetItem *twi = new BOBUIreeWidgetItem(&tw, QStringList() << "Test");
    BOBUIreeWidgetItem *twi2 = new BOBUIreeWidgetItem(&tw, QStringList() << "Test 2");
    BOBUIreeWidgetItem *twi3 = new BOBUIreeWidgetItem(&tw, QStringList() << "Test 3");
    tw.show();
    QCOMPARE(tw.itemAbove(twi2), twi);
    QCOMPARE(tw.itemBelow(twi2), twi3);
}

void tst_BOBUIreeWidget::itemStreaming_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QString>("toolTip");
    BOBUIest::addColumn<int>("column");

    BOBUIest::newRow("Data") << "item text" << "tool tip text" << 0;
}

void tst_BOBUIreeWidget::itemStreaming()
{
    QFETCH(QString, text);
    QFETCH(QString, toolTip);
    QFETCH(int, column);

    BOBUIreeWidgetItem item(testWidget);
    QCOMPARE(item.text(column), QString());
    QCOMPARE(item.toolTip(column), QString());

    item.setText(column, text);
    item.setToolTip(column, toolTip);
    QCOMPARE(item.text(column), text);
    QCOMPARE(item.toolTip(column), toolTip);

    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out << item;

    BOBUIreeWidgetItem item2(testWidget);
    QCOMPARE(item2.text(column), QString());
    QCOMPARE(item2.toolTip(column), QString());

    QVERIFY(!buffer.isEmpty());

    QDataStream in(&buffer, QIODevice::ReadOnly);
    in >> item2;
    QCOMPARE(item2.text(column), text);
    QCOMPARE(item2.toolTip(column), toolTip);
}

void tst_BOBUIreeWidget::insertTopLevelItems_data()
{
    BOBUIest::addColumn<QStringList>("initialText");
    BOBUIest::addColumn<QStringList>("insertText");
    BOBUIest::addColumn<int>("insertTopLevelIndex");
    BOBUIest::addColumn<int>("expectedTopLevelIndex");
    BOBUIest::addColumn<int>("insertChildIndex");
    BOBUIest::addColumn<int>("expectedChildIndex");

    const QStringList initial{ "foo", "bar" };
    const QStringList insert{ "baz" };

    BOBUIest::newRow("Insert at count") << initial << insert
                                     << initial.size() << initial.size()
                                     << initial.size() << initial.size();
    BOBUIest::newRow("Insert in the middle") << initial << insert
                                          << (initial.size() / 2) << (initial.size() / 2)
                                          << (initial.size() / 2) << (initial.size() / 2);
    BOBUIest::newRow("Insert less than 0") << initial << insert
                                        << -1 << -1
                                        << -1 << -1;
    BOBUIest::newRow("Insert beyond count") << initial << insert
                                         << initial.size() + 1 << -1
                                         << initial.size() + 1 << -1;
}

void tst_BOBUIreeWidget::insertTopLevelItems()
{
    QFETCH(QStringList, initialText);
    QFETCH(QStringList, insertText);
    QFETCH(int, insertTopLevelIndex);
    QFETCH(int, expectedTopLevelIndex);
    QFETCH(int, insertChildIndex);
    QFETCH(int, expectedChildIndex);
    testWidget->setSortingEnabled(false);

    { // insert the initial items
        QCOMPARE(testWidget->topLevelItemCount(), 0);
        for (int i = 0; i < initialText.size(); ++i) {
            BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(QStringList(initialText.at(i)));
            testWidget->addTopLevelItem(top);
            QCOMPARE(testWidget->indexOfTopLevelItem(top), i);
        }
        QCOMPARE(testWidget->topLevelItemCount(), initialText.size());
    }

    { // test adding children
        BOBUIreeWidgetItem *topLevel = testWidget->topLevelItem(0);
        for (int i = 0; i < initialText.size(); ++i)
            topLevel->addChild(new BOBUIreeWidgetItem(QStringList(initialText.at(i))));
        QCOMPARE(topLevel->childCount(), initialText.size());
    }

    { // test adding more top level items
        BOBUIreeWidgetItem *topsy = new BOBUIreeWidgetItem(QStringList(insertText.at(0)));
        testWidget->insertTopLevelItem(insertTopLevelIndex, topsy);
        if (expectedTopLevelIndex == -1) {
            QCOMPARE(testWidget->topLevelItemCount(), initialText.size());
            delete topsy;
        } else {
            BOBUIreeWidgetItem *item = testWidget->topLevelItem(expectedTopLevelIndex);
            QVERIFY(item != nullptr);
            QCOMPARE(item->text(0), insertText.at(0));
            QCOMPARE(testWidget->indexOfTopLevelItem(item), expectedTopLevelIndex);
        }
    }

    { // test adding more children
        BOBUIreeWidgetItem *topLevel = testWidget->topLevelItem(0);
        QVERIFY(topLevel != nullptr);
        BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(QStringList(insertText.at(0)));
        topLevel->insertChild(insertChildIndex, child);
        if (expectedChildIndex == -1) {
            QCOMPARE(topLevel->childCount(), initialText.size());
            delete child;
        } else {
            BOBUIreeWidgetItem *item = topLevel->child(expectedChildIndex);
            QVERIFY(item != nullptr);
            QCOMPARE(item->text(0), insertText.at(0));
        }
    }
}

static void fillTreeWidget(BOBUIreeWidgetItem *parent, int rows)
{
    const int columns = parent->treeWidget()->columnCount();
    for (int r = 0; r < rows; ++r) {
        const QString prefix = QLatin1String("[r:") + QString::number(r) + QLatin1String(",c:");
        BOBUIreeWidgetItem *w = new BOBUIreeWidgetItem(parent);
        for (int c = 0; c < columns; ++c)
            w->setText(c, prefix + QString::number(c) + QLatin1Char(']'));
        fillTreeWidget(w, rows - r - 1);
    }
}

static void fillTreeWidget(BOBUIreeWidget *tree, int rows)
{
    for (int r = 0; r < rows; ++r) {
        BOBUIreeWidgetItem *w = new BOBUIreeWidgetItem();
        const QString prefix = QLatin1String("[r:") + QString::number(r) + QLatin1String(",c:");
        for (int c = 0; c < tree->columnCount(); ++c)
            w->setText(c, prefix + QString::number(c) + QLatin1Char(']'));
        tree->insertTopLevelItem(r, w);
        fillTreeWidget(w, rows - r - 1);
    }
}

void tst_BOBUIreeWidget::keyboardNavigation()
{
    int rows = 8;

    fillTreeWidget(testWidget, rows);

    const QList<BobUI::Key> keymoves {
        BobUI::Key_Down,  BobUI::Key_Right, BobUI::Key_Left,  BobUI::Key_Down, BobUI::Key_Down, BobUI::Key_Down,
        BobUI::Key_Down,  BobUI::Key_Right, BobUI::Key_Up,    BobUI::Key_Left, BobUI::Key_Left, BobUI::Key_Up,
        BobUI::Key_Down,  BobUI::Key_Up,    BobUI::Key_Up,    BobUI::Key_Up,   BobUI::Key_Up,   BobUI::Key_Up,
        BobUI::Key_Up,    BobUI::Key_Down,  BobUI::Key_Right, BobUI::Key_Down, BobUI::Key_Down, BobUI::Key_Down,
        BobUI::Key_Right, BobUI::Key_Down,  BobUI::Key_Down,  BobUI::Key_Left, BobUI::Key_Left, BobUI::Key_Up,
        BobUI::Key_Down,  BobUI::Key_Up,    BobUI::Key_Up,    BobUI::Key_Up,   BobUI::Key_Left, BobUI::Key_Down,
        BobUI::Key_Right, BobUI::Key_Right, BobUI::Key_Right, BobUI::Key_Left, BobUI::Key_Left, BobUI::Key_Right,
        BobUI::Key_Left
    };

    int row = 0;
    BOBUIreeWidgetItem *item = testWidget->topLevelItem(0);
    testWidget->setCurrentItem(item);
    QCOMPARE(testWidget->currentItem(), item);
    QCoreApplication::processEvents();

    QScrollBar *scrollBar = testWidget->horizontalScrollBar();
    for (const BobUI::Key key : keymoves) {
        int valueBeforeClick = scrollBar->value();
        const bool checkScroll = (valueBeforeClick >= scrollBar->singleStep());
        BOBUIest::keyClick(testWidget, key);
        QCoreApplication::processEvents();

        switch (key) {
        case BobUI::Key_Up:
            if (row > 0) {
                if (item->parent())
                    item = item->parent()->child(row - 1);
                else
                    item = testWidget->topLevelItem(row - 1);
                row -= 1;
            } else if (item->parent()) {
                item = item->parent();
                row = item->parent() ? item->parent()->indexOfChild(item) : testWidget->indexOfTopLevelItem(item);
            }
            break;
        case BobUI::Key_Down:
            if (item->isExpanded()) {
                row = 0;
                item = item->child(row);
            } else {
                row = qMin(rows - 1, row + 1);
                if (item->parent())
                    item = item->parent()->child(row);
                else
                    item = testWidget->topLevelItem(row);
            }
            break;
        case BobUI::Key_Left:
            if (checkScroll) {
                QVERIFY(item->isExpanded());
                QCOMPARE(scrollBar->value(), valueBeforeClick - scrollBar->singleStep());
            }
            // windows style right will walk to the parent
            if (testWidget->currentItem() != item) {
                QCOMPARE(testWidget->currentItem(), item->parent());
                item = testWidget->currentItem();
                row = item->parent() ? item->parent()->indexOfChild(item) : testWidget->indexOfTopLevelItem(item);
            }
            break;
        case BobUI::Key_Right:
            if (checkScroll)
                QCOMPARE(scrollBar->value(), valueBeforeClick + scrollBar->singleStep());
            // windows style right will walk to the first child
            if (testWidget->currentItem() != item) {
                QCOMPARE(testWidget->currentItem()->parent(), item);
                row = item->indexOfChild(testWidget->currentItem());
                item = testWidget->currentItem();
                QCOMPARE(row, 0);
            }
            break;
        default:
            QFAIL(qPrintable(QStringLiteral("Unexpected key: %1").arg(key)));
        }

        BOBUIreeWidgetItem *current = testWidget->currentItem();
        QCOMPARE(current->text(0), QLatin1String("[r:") + QString::number(row) + QLatin1String(",c:0]"));
        if (current->parent())
            QCOMPARE(current->parent()->indexOfChild(current), row);
        else
            QCOMPARE(testWidget->indexOfTopLevelItem(current), row);
    }
}

void tst_BOBUIreeWidget::keyboardNavigationWithHidden()
{
    BOBUIreeWidget tw;
    for (int i = 0; i < 1000; ++i)
        tw.addTopLevelItem(new BOBUIreeWidgetItem({QString::number(i), QStringLiteral("second col")}));
    // BOBUIBUG-34832 - when first/last item is hidden,
    // Key_PageUp/Down/Home/End will not work as expected.
    tw.topLevelItem(0)->setHidden(true);
    tw.topLevelItem(tw.model()->rowCount() - 1)->setHidden(true);
    // PageUp
    tw.setCurrentIndex(tw.model()->index(2, 0));
    QCOMPARE(tw.currentIndex(), tw.model()->index(2, 0));
    BOBUIest::keyClick(tw.viewport(), BobUI::Key_PageUp);
    QCOMPARE(tw.currentIndex(), tw.model()->index(1, 0));
    // PageDown
    tw.setCurrentIndex(tw.model()->index(tw.model()->rowCount() - 3, 0));
    QCOMPARE(tw.currentIndex(), tw.model()->index(tw.model()->rowCount() - 3, 0));
    BOBUIest::keyClick(tw.viewport(), BobUI::Key_PageDown);
    QCOMPARE(tw.currentIndex(), tw.model()->index(tw.model()->rowCount() - 2, 0));
    // Key_Home
    BOBUIest::keyClick(tw.viewport(), BobUI::Key_Home);
    QCOMPARE(tw.currentIndex(), tw.model()->index(1, 0));
    // Key_End
    BOBUIest::keyClick(tw.viewport(), BobUI::Key_End);
    QCOMPARE(tw.currentIndex(), tw.model()->index(tw.model()->rowCount() - 2, 0));
}

void tst_BOBUIreeWidget::scrollToItem()
{
    // Check if all parent nodes of the item found are expanded.
    // Reported in task #78761
    BOBUIreeWidgetItem *search = nullptr;
    for (int i = 0; i < 2; ++i) {
        BOBUIreeWidgetItem *bar = new BOBUIreeWidgetItem(testWidget);
        bar->setText(0, QString::number(i));

        for (int j = 0; j < 2; ++j) {
            BOBUIreeWidgetItem *foo = new BOBUIreeWidgetItem(bar);
            foo->setText(0, bar->text(0) + QString::number(j));

            for (int k = 0; k < 2; ++k) {
                search = new BOBUIreeWidgetItem(foo);
                search->setText(0, foo->text(0) + QString::number(k));
            }
        }
    }

    testWidget->setHeaderLabels(QStringList() << "foo");
    testWidget->scrollToItem(search);
    QCOMPARE(search->text(0), QLatin1String("111"));

    BOBUIreeWidgetItem *par = search->parent();
    QVERIFY(par->isExpanded());
    par = par->parent();
    QVERIFY(par->isExpanded());
}

// From task #85413
void tst_BOBUIreeWidget::setSortingEnabled()
{
    const QStringList hl{ "ID" };
    testWidget->setColumnCount(hl.size());
    testWidget->setHeaderLabels(hl);

    BOBUIreeWidgetItem *item1 = new BOBUIreeWidgetItem(testWidget);
    BOBUIreeWidgetItem *item2 = new BOBUIreeWidgetItem(testWidget);

    testWidget->setSortingEnabled(true);
    QCOMPARE(testWidget->isSortingEnabled(), true);
    QCOMPARE(testWidget->isSortingEnabled(), testWidget->header()->isSortIndicatorShown());
    QCOMPARE(testWidget->topLevelItem(0), item1);
    QCOMPARE(testWidget->topLevelItem(1), item2);

    // Make sure we do it twice
    testWidget->setSortingEnabled(true);
    QCOMPARE(testWidget->isSortingEnabled(), true);
    QCOMPARE(testWidget->isSortingEnabled(), testWidget->header()->isSortIndicatorShown());

    testWidget->setSortingEnabled(false);
    QCOMPARE(testWidget->isSortingEnabled(), false);
    QCOMPARE(testWidget->isSortingEnabled(), testWidget->header()->isSortIndicatorShown());

    testWidget->setSortingEnabled(false);
    QCOMPARE(testWidget->isSortingEnabled(), false);
    QCOMPARE(testWidget->isSortingEnabled(), testWidget->header()->isSortIndicatorShown());

    // And back again so that we make sure that we test the transition from false to true
    testWidget->setSortingEnabled(true);
    QCOMPARE(testWidget->isSortingEnabled(), true);
    QCOMPARE(testWidget->isSortingEnabled(), testWidget->header()->isSortIndicatorShown());

    testWidget->setSortingEnabled(true);
    QCOMPARE(testWidget->isSortingEnabled(), true);
    QCOMPARE(testWidget->isSortingEnabled(), testWidget->header()->isSortIndicatorShown());

    testWidget->setSortingEnabled(false);
}

void tst_BOBUIreeWidget::addChild()
{
    BOBUIreeWidget tree;
    for (int x = 0; x < 2; ++x) {
        BOBUIreeWidget *view = x ? &tree : static_cast<BOBUIreeWidget*>(nullptr);
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(view);
        QCOMPARE(item->childCount(), 0);

        // try to add 0
        item->addChild(nullptr);
        QCOMPARE(item->childCount(), 0);
        QCOMPARE(item->indexOfChild(nullptr), -1);

        // add one at a time
        QList<BOBUIreeWidgetItem*> children;
        for (int i = 0; i < 10; ++i) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem();
            item->addChild(child);
            QCOMPARE(item->childCount(), i+1);
            QCOMPARE(item->child(i), child);
            QCOMPARE(item->indexOfChild(child), i);
            QCOMPARE(child->parent(), item);
            QCOMPARE(child->treeWidget(), view);
            item->addChild(child);
            QCOMPARE(item->childCount(), i+1);
            children.append(child);
        }

        // take them all
        QList<BOBUIreeWidgetItem*> taken = item->takeChildren();
        QCOMPARE(taken, children);
        QCOMPARE(item->childCount(), 0);
        for (int i = 0; i < taken.size(); ++i) {
            QCOMPARE(taken.at(i)->parent(), nullptr);
            QCOMPARE(taken.at(i)->treeWidget(), nullptr);
            item->addChild(taken.at(i)); // re-add
        }

        // delete one at a time
        while (!children.isEmpty()) {
            BOBUIreeWidgetItem *ti = children.takeFirst();
            delete ti;
            QCOMPARE(item->childCount(), children.size());
            for (int i = 0; i < children.size(); ++i)
                QCOMPARE(item->child(i), children.at(i));
        }

        // add none
        {
            int count = item->childCount();
            item->addChildren(QList<BOBUIreeWidgetItem*>());
            QCOMPARE(item->childCount(), count);
        }

        // add many at a time
        const int count = 10;
        for (int i = 0; i < 100; i += count) {
            QList<BOBUIreeWidgetItem*> list;
            for (int j = 0; j < count; ++j)
                list << new BOBUIreeWidgetItem(QStringList(QString::number(j)));
            item->addChildren(list);
            QCOMPARE(item->childCount(), count + i);
            for (int j = 0; j < count; ++j) {
                QCOMPARE(item->child(i+j), list.at(j));
                QCOMPARE(item->child(i+j)->parent(), item);
            }

            item->addChildren(list);
            QCOMPARE(item->childCount(), count + i);
        }

        if (!view)
            delete item;
    }
}

void tst_BOBUIreeWidget::setData()
{
    {
        BOBUIreeWidgetItem *headerItem = new BOBUIreeWidgetItem();
        headerItem->setText(0, "Item1");
        testWidget->setHeaderItem(headerItem);

        QSignalSpy headerDataChangedSpy(
            testWidget->model(), &QAbstractItemModel::headerDataChanged);
        QSignalSpy dataChangedSpy(
            testWidget->model(), &QAbstractItemModel::dataChanged);
        QSignalSpy itemChangedSpy(
            testWidget, &BOBUIreeWidget::itemChanged);
        headerItem->setText(0, "test");
        QCOMPARE(dataChangedSpy.size(), 0);
        QCOMPARE(headerDataChangedSpy.size(), 1);
        QCOMPARE(itemChangedSpy.size(), 0); // no itemChanged() signal for header item

        headerItem->setData(-1, -1, QVariant());
    }

    {
        QSignalSpy itemChangedSpy(
            testWidget, &BOBUIreeWidget::itemChanged);
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem();
        testWidget->addTopLevelItem(item);
        for (int x = 0; x < 2; ++x) {
            for (int i = 1; i <= 2; ++i) {
                for (int j = 0; j < 5; ++j) {
                    QVariantList args;
                    const QString iS = QString::number(i);
                    const QString text = QLatin1String("text ") + iS;
                    item->setText(j, text);
                    QCOMPARE(item->text(j), text);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setText(j, text);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    QPixmap pixmap(32, 32);
                    pixmap.fill((i == 1) ? BobUI::red : BobUI::green);
                    QIcon icon(pixmap);
                    item->setIcon(j, icon);
                    QCOMPARE(item->icon(j), icon);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setIcon(j, icon);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);

                    const QString toolTip = QLatin1String("toolTip ") + iS;
                    item->setToolTip(j, toolTip);
                    QCOMPARE(item->toolTip(j), toolTip);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setToolTip(j, toolTip);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    const QString statusTip = QLatin1String("statusTip ") + iS;
                    item->setStatusTip(j, statusTip);
                    QCOMPARE(item->statusTip(j), statusTip);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setStatusTip(j, statusTip);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    const QString whatsThis = QLatin1String("whatsThis ") + iS;
                    item->setWhatsThis(j, whatsThis);
                    QCOMPARE(item->whatsThis(j), whatsThis);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setWhatsThis(j, whatsThis);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    QSize sizeHint(64*i, 48*i);
                    item->setSizeHint(j, sizeHint);
                    QCOMPARE(item->sizeHint(j), sizeHint);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setSizeHint(j, sizeHint);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    QFont font;
                    item->setFont(j, font);
                    QCOMPARE(item->font(j), font);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setFont(j, font);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    BobUI::Alignment textAlignment((i == 1)
                                                ? BobUI::AlignLeft|BobUI::AlignVCenter
                                                : BobUI::AlignRight);
                    item->setTextAlignment(j, textAlignment);
                    QCOMPARE(item->textAlignment(j), int(textAlignment));
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setTextAlignment(j, textAlignment);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    QColor backgroundColor((i == 1) ? BobUI::blue : BobUI::yellow);
                    item->setBackground(j, backgroundColor);
                    QCOMPARE(item->background(j).color(), backgroundColor);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setBackground(j, backgroundColor);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    const QColor foregroundColor((i == 1) ? BobUI::green : BobUI::cyan);
                    item->setForeground(j, foregroundColor);
                    QCOMPARE(item->foreground(j), foregroundColor);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setForeground(j, foregroundColor);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    BobUI::CheckState checkState((i == 1) ? BobUI::PartiallyChecked : BobUI::Checked);
                    item->setCheckState(j, checkState);
                    QCOMPARE(item->checkState(j), checkState);
                    QCOMPARE(itemChangedSpy.size(), 1);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setCheckState(j, checkState);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    QCOMPARE(item->text(j), text);
                    QCOMPARE(item->icon(j), icon);
                    QCOMPARE(item->toolTip(j), toolTip);
                    QCOMPARE(item->statusTip(j), statusTip);
                    QCOMPARE(item->whatsThis(j), whatsThis);
                    QCOMPARE(item->sizeHint(j), sizeHint);
                    QCOMPARE(item->font(j), font);
                    QCOMPARE(item->textAlignment(j), int(textAlignment));
                    QCOMPARE(item->background(j).color(), backgroundColor);
                    QCOMPARE(item->foreground(j), foregroundColor);
                    QCOMPARE(item->checkState(j), checkState);

                    QCOMPARE(qvariant_cast<QString>(item->data(j, BobUI::DisplayRole)), text);
                    QCOMPARE(qvariant_cast<QIcon>(item->data(j, BobUI::DecorationRole)), icon);
                    QCOMPARE(qvariant_cast<QString>(item->data(j, BobUI::ToolTipRole)), toolTip);
                    QCOMPARE(qvariant_cast<QString>(item->data(j, BobUI::StatusTipRole)), statusTip);
                    QCOMPARE(qvariant_cast<QString>(item->data(j, BobUI::WhatsThisRole)), whatsThis);
                    QCOMPARE(qvariant_cast<QSize>(item->data(j, BobUI::SizeHintRole)), sizeHint);
                    QCOMPARE(qvariant_cast<QFont>(item->data(j, BobUI::FontRole)), font);
                    QCOMPARE(qvariant_cast<int>(item->data(j, BobUI::TextAlignmentRole)), int(textAlignment));
                    QCOMPARE(qvariant_cast<QBrush>(item->data(j, BobUI::BackgroundRole)), QBrush(backgroundColor));
                    QCOMPARE(qvariant_cast<QColor>(item->data(j, BobUI::ForegroundRole)), foregroundColor);
                    QCOMPARE(qvariant_cast<int>(item->data(j, BobUI::CheckStateRole)), int(checkState));

                    item->setBackground(j, pixmap);
                    QCOMPARE(item->background(j).texture(), pixmap);
                    QCOMPARE(qvariant_cast<QBrush>(item->data(j, BobUI::BackgroundRole)).texture(), pixmap);
                    args = itemChangedSpy.takeFirst();
                    QCOMPARE(qvariant_cast<BOBUIreeWidgetItem*>(args.at(0)), item);
                    QCOMPARE(qvariant_cast<int>(args.at(1)), j);
                    item->setBackground(j, pixmap);
                    QCOMPARE(itemChangedSpy.size(), 0);

                    item->setData(j, BobUI::DisplayRole, QVariant());
                    item->setData(j, BobUI::DecorationRole, QVariant());
                    item->setData(j, BobUI::ToolTipRole, QVariant());
                    item->setData(j, BobUI::StatusTipRole, QVariant());
                    item->setData(j, BobUI::WhatsThisRole, QVariant());
                    item->setData(j, BobUI::SizeHintRole, QVariant());
                    item->setData(j, BobUI::FontRole, QVariant());
                    item->setData(j, BobUI::TextAlignmentRole, QVariant());
                    item->setData(j, BobUI::BackgroundRole, QVariant());
                    item->setData(j, BobUI::ForegroundRole, QVariant());
                    item->setData(j, BobUI::CheckStateRole, QVariant());
                    QCOMPARE(itemChangedSpy.size(), 11);
                    itemChangedSpy.clear();

                    QCOMPARE(item->data(j, BobUI::DisplayRole).toString(), QString());
                    QCOMPARE(item->data(j, BobUI::DecorationRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::ToolTipRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::StatusTipRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::WhatsThisRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::SizeHintRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::FontRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::TextAlignmentRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::BackgroundRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::ForegroundRole), QVariant());
                    QCOMPARE(item->data(j, BobUI::CheckStateRole), QVariant());
                }
            }
        }

        // ### add more data types here

        item->setData(0, BobUI::DisplayRole, 5);
        QCOMPARE(item->data(0, BobUI::DisplayRole).userType(), QMetaType::Int);

        item->setData(0, BobUI::DisplayRole, "test");
        QCOMPARE(item->data(0, BobUI::DisplayRole).userType(), QMetaType::QString);

        item->setData(0, BobUI::DisplayRole, 0.4);
        QCOMPARE(item->data(0, BobUI::DisplayRole).userType(), QMetaType::Double);

        delete item;
    }
}

class BOBUIreeWidgetDataChanged : public BOBUIreeWidget
{
    Q_OBJECT
public:
    using BOBUIreeWidget::BOBUIreeWidget;

    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QList<int> &roles) override
    {
        BOBUIreeWidget::dataChanged(topLeft, bottomRight, roles);
        currentRoles = roles;
    }
    QList<int> currentRoles;
};

void tst_BOBUIreeWidget::itemData()
{
    BOBUIreeWidgetDataChanged widget;
    BOBUIreeWidgetItem item(&widget);
    widget.setColumnCount(2);
    item.setFlags(item.flags() | BobUI::ItemIsEditable);
    item.setData(0, BobUI::DisplayRole,  QString("0"));
    QCOMPARE(widget.currentRoles, QList<int>({ BobUI::DisplayRole, BobUI::EditRole }));
    item.setData(0, BobUI::CheckStateRole, BobUI::PartiallyChecked);
    QCOMPARE(widget.currentRoles, QList<int> { BobUI::CheckStateRole });
    for (int i = 0; i < 4; ++i) {
        item.setData(0, BobUI::UserRole + i, QString::number(i + 1));
        QCOMPARE(widget.currentRoles, QList<int> { BobUI::UserRole + i });
    }
    QMap<int, QVariant> flags = widget.model()->itemData(widget.model()->index(0, 0));
    QCOMPARE(flags.size(), 6);
    for (int i = 0; i < 4; ++i)
        QCOMPARE(flags[BobUI::UserRole + i].toString(), QString::number(i + 1));
    flags = widget.model()->itemData(widget.model()->index(0, 1));
    QCOMPARE(flags.size(), 0);

    item.setBackground(0, QBrush(BobUI::red));
    item.setForeground(0, QBrush(BobUI::green));
    item.setSizeHint(0, QSize(10, 10));
    QCOMPARE(item.data(0, BobUI::BackgroundRole), QVariant(QBrush(BobUI::red)));
    QCOMPARE(item.data(0, BobUI::ForegroundRole), QVariant(QBrush(BobUI::green)));
    QCOMPARE(item.data(0, BobUI::SizeHintRole), QVariant(QSize(10, 10)));
    // an empty brush should result in a QVariant()
    item.setBackground(0, QBrush());
    item.setForeground(0, QBrush());
    item.setSizeHint(0, QSize());
    QCOMPARE(item.data(0, BobUI::BackgroundRole), QVariant());
    QCOMPARE(item.data(0, BobUI::ForegroundRole), QVariant());
    QCOMPARE(item.data(0, BobUI::SizeHintRole), QVariant());
}

void tst_BOBUIreeWidget::enableDisable()
{
    const QScopedPointer<BOBUIreeWidgetItem> itm(new BOBUIreeWidgetItem);
    for (int i = 0; i < 10; ++i)
        new BOBUIreeWidgetItem(itm.data());

    // make sure all items are enabled
    QVERIFY(itm->flags() & BobUI::ItemIsEnabled);
    for (int j = 0; j < itm->childCount(); ++j)
        QVERIFY(itm->child(j)->flags() & BobUI::ItemIsEnabled);

    // disable root and make sure they are all disabled
    itm->setFlags(itm->flags() & ~BobUI::ItemIsEnabled);
    QVERIFY(!(itm->flags() & BobUI::ItemIsEnabled));
    for (int k = 0; k < itm->childCount(); ++k)
        QVERIFY(!(itm->child(k)->flags() & BobUI::ItemIsEnabled));

    // disable a child and make sure they are all still disabled
    itm->child(5)->setFlags(itm->child(5)->flags() & ~BobUI::ItemIsEnabled);
    QVERIFY(!(itm->flags() & BobUI::ItemIsEnabled));
    for (int l = 0; l < itm->childCount(); ++l)
        QVERIFY(!(itm->child(l)->flags() & BobUI::ItemIsEnabled));

    // enable root and make sure all items except one are enabled
    itm->setFlags(itm->flags() | BobUI::ItemIsEnabled);
    QVERIFY(itm->flags() & BobUI::ItemIsEnabled);
    for (int m = 0; m < itm->childCount(); ++m)
        if (m == 5)
            QVERIFY(!(itm->child(m)->flags() & BobUI::ItemIsEnabled));
        else
            QVERIFY(itm->child(m)->flags() & BobUI::ItemIsEnabled);
}

void tst_BOBUIreeWidget::match()
{
    BOBUIreeWidget tree;
    QModelIndexList list = tree.model()->match(QModelIndex(), BobUI::DisplayRole, QString());

    QVERIFY(list.isEmpty());
}

void tst_BOBUIreeWidget::columnCount()
{
    int columnCountBefore = testWidget->columnCount();
    testWidget->setColumnCount(-1);
    QCOMPARE(testWidget->columnCount(), columnCountBefore);
}

void tst_BOBUIreeWidget::setHeaderLabels()
{
    QStringList list = QString("a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z").split(QLatin1Char(','));
    testWidget->setHeaderLabels(list);
    QCOMPARE(testWidget->header()->count(), list.size());
}

void tst_BOBUIreeWidget::setHeaderItem()
{
    testWidget->setHeaderItem(nullptr);
    BOBUIreeWidgetItem *headerItem = new BOBUIreeWidgetItem();

    testWidget->setColumnCount(0);
    QCOMPARE(testWidget->header()->count(), 0);
    QCOMPARE(testWidget->columnCount(), 0);

    headerItem->setText(0, "0");
    headerItem->setText(1, "1");
    testWidget->setHeaderItem(headerItem);
    QCOMPARE(testWidget->headerItem(), headerItem);
    QCOMPARE(headerItem->treeWidget(), static_cast<BOBUIreeWidget *>(testWidget));

    QCOMPARE(testWidget->header()->count(), 2);
    QCOMPARE(testWidget->columnCount(), 2);

    headerItem->setText(2, "2");
    QCOMPARE(testWidget->header()->count(), 3);
    QCOMPARE(testWidget->columnCount(), 3);

    delete headerItem;
    testWidget->setColumnCount(3);
    testWidget->setColumnCount(5);
    QCOMPARE(testWidget->model()->headerData(0, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("1"));
    QCOMPARE(testWidget->model()->headerData(1, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("2"));
    QCOMPARE(testWidget->model()->headerData(2, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("3"));
    QCOMPARE(testWidget->model()->headerData(3, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("4"));
    QCOMPARE(testWidget->model()->headerData(4, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("5"));

    headerItem = new BOBUIreeWidgetItem();
    testWidget->setHeaderItem(headerItem);
    testWidget->model()->insertColumns(0, 5, QModelIndex());
    QCOMPARE(testWidget->model()->headerData(0, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("1"));
    QCOMPARE(testWidget->model()->headerData(1, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("2"));
    QCOMPARE(testWidget->model()->headerData(2, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("3"));
    QCOMPARE(testWidget->model()->headerData(3, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("4"));
    QCOMPARE(testWidget->model()->headerData(4, BobUI::Horizontal, BobUI::DisplayRole).toString(), QString("5"));
}

void tst_BOBUIreeWidget::itemWidget_data()
{
    editItem_data();
}

void tst_BOBUIreeWidget::itemWidget()
{
    QFETCH(TreeItemList, topLevelItems);

    BOBUIreeWidget tree;
    populate(&tree, topLevelItems, new TreeItem({"1", "2"}));
    tree.show();

    for (int x = 0; x < 2; ++x) {
        BOBUIreeWidgetItemIterator it(&tree);
        while (BOBUIreeWidgetItem *item = (*it++)) {
            for (int col = 0; col < item->columnCount(); ++col) {
                if (x == 0) {
                    QCOMPARE(tree.itemWidget(item, col), nullptr);
                    QWidget *editor = new QLineEdit();
                    tree.setItemWidget(item, col, editor);
                    QCOMPARE(tree.itemWidget(item, col), editor);
                    tree.removeItemWidget(item, col);
                    QCOMPARE(tree.itemWidget(item, col), nullptr);
                } else {
                    // ### should you really be able to open a persistent
                    //     editor for an item that isn't editable??
                    tree.openPersistentEditor(item, col);
                    QWidget *editor = tree.findChild<QLineEdit*>();
                    QVERIFY(editor != nullptr);
                    tree.closePersistentEditor(item, col);
                }
            }
        }
    }
}

void tst_BOBUIreeWidget::insertItemsWithSorting_data()
{
    BOBUIest::addColumn<BobUI::SortOrder>("sortOrder");
    BOBUIest::addColumn<QStringList>("initialItems");
    BOBUIest::addColumn<QStringList>("insertItems");
    BOBUIest::addColumn<QStringList>("expectedItems");
    BOBUIest::addColumn<IntList>("expectedRows");

    BOBUIest::newRow("() + (a) = (a)")
        << BobUI::AscendingOrder
        << QStringList()
        << (QStringList() << "a")
        << (QStringList() << "a")
        << IntList();
    BOBUIest::newRow("() + (c, b, a) = (a, b, c)")
        << BobUI::AscendingOrder
        << QStringList()
        << (QStringList() << "c" << "b" << "a")
        << (QStringList() << "a" << "b" << "c")
        << IntList();
    BOBUIest::newRow("() + (a, b, c) = (c, b, a)")
        << BobUI::DescendingOrder
        << QStringList()
        << (QStringList() << "a" << "b" << "c")
        << (QStringList() << "c" << "b" << "a")
        << IntList();
    BOBUIest::newRow("(a) + (b) = (a, b)")
        << BobUI::AscendingOrder
        << QStringList("a")
        << (QStringList() << "b")
        << (QStringList() << "a" << "b")
        << (IntList() << 0);
    BOBUIest::newRow("(a) + (b) = (b, a)")
        << BobUI::DescendingOrder
        << QStringList("a")
        << (QStringList() << "b")
        << (QStringList() << "b" << "a")
        << (IntList() << 1);
    BOBUIest::newRow("(a, c, b) + (d) = (a, b, c, d)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "b")
        << (QStringList() << "d")
        << (QStringList() << "a" << "b" << "c" << "d")
        << (IntList() << 0 << 1 << 2);
    BOBUIest::newRow("(b, c, a) + (d) = (d, c, b, a)")
        << BobUI::DescendingOrder
        << (QStringList() << "b" << "c" << "a")
        << (QStringList() << "d")
        << (QStringList() << "d" << "c" << "b" << "a")
        << (IntList() << 1 << 2 << 3);
    {
        IntList ascendingRows;
        IntList reverseRows;
        QStringList ascendingItems;
        QStringList reverseItems;
        for (char i = 'a'; i <= 'z'; ++i) {
            ascendingItems << QString(1, QLatin1Char(i));
            reverseItems << QString(1, QLatin1Char('z' - i + 'a'));
            ascendingRows << i - 'a';
            reverseRows << 'z' - i + 'a';
        }
        BOBUIest::newRow("() + (sorted items) = (sorted items)")
            << BobUI::AscendingOrder
            << QStringList()
            << ascendingItems
            << ascendingItems
            << IntList();
        BOBUIest::newRow("(sorted items) + () = (sorted items)")
            << BobUI::AscendingOrder
            << ascendingItems
            << QStringList()
            << ascendingItems
            << ascendingRows;
        BOBUIest::newRow("() + (ascending items) = (reverse items)")
            << BobUI::DescendingOrder
            << QStringList()
            << ascendingItems
            << reverseItems
            << IntList();
        BOBUIest::newRow("(reverse items) + () = (ascending items)")
            << BobUI::AscendingOrder
            << reverseItems
            << QStringList()
            << ascendingItems
            << ascendingRows;
        BOBUIest::newRow("(reverse items) + () = (reverse items)")
            << BobUI::DescendingOrder
            << reverseItems
            << QStringList()
            << reverseItems
            << ascendingRows;
    }
}

void tst_BOBUIreeWidget::insertItemsWithSorting()
{
    QFETCH(BobUI::SortOrder, sortOrder);
    QFETCH(const QStringList, initialItems);
    QFETCH(const QStringList, insertItems);
    QFETCH(const QStringList, expectedItems);
    QFETCH(IntList, expectedRows);

    for (int method = 0; method < 5; ++method) {
        BOBUIreeWidget w;
        w.setSortingEnabled(true);
        w.sortItems(0, sortOrder);
        for (const QString &initialItem : initialItems)
          w.addTopLevelItem(new BOBUIreeWidgetItem({initialItem}));

        QAbstractItemModel *model = w.model();
        PersistentModelIndexVec persistent;
        for (int j = 0; j < model->rowCount(QModelIndex()); ++j)
            persistent << model->index(j, 0, QModelIndex());

        switch (method) {
            case 0:
                // insert using item constructor
                for (const QString &txt : insertItems)
                    new BOBUIreeWidgetItem(&w, { txt });
                break;
            case 1:
            {
                // insert using insertTopLevelItems()
                QList<BOBUIreeWidgetItem*> lst;
                for (const QString &txt : insertItems)
                    lst << new BOBUIreeWidgetItem({ txt });
                w.insertTopLevelItems(0, lst);
                break;
            }
            case 2:
                // insert using insertTopLevelItem()
                for (const QString &txt : insertItems)
                    w.insertTopLevelItem(0, new BOBUIreeWidgetItem({ txt }));
                break;
            case 3:
            {
                // insert using addTopLevelItems()
                QList<BOBUIreeWidgetItem*> lst;
                for (const QString &txt : insertItems)
                    lst << new BOBUIreeWidgetItem({ txt });
                w.addTopLevelItems(lst);
                break;
            }
            case 4:
                // insert using addTopLevelItem()
                for (const QString &txt : insertItems)
                    w.addTopLevelItem(new BOBUIreeWidgetItem({ txt }));
                break;
        }
        QCOMPARE(w.topLevelItemCount(), expectedItems.size());
        for (int i = 0; i < w.topLevelItemCount(); ++i)
            QCOMPARE(w.topLevelItem(i)->text(0), expectedItems.at(i));

        for (int k = 0; k < persistent.size(); ++k)
            QCOMPARE(persistent.at(k).row(), expectedRows.at(k));
    }
}

void tst_BOBUIreeWidget::insertExpandedItemsWithSorting_data()
{
    BOBUIest::addColumn<QStringList>("parentTexts");
    BOBUIest::addColumn<QStringList>("childTexts");
    BOBUIest::addColumn<QStringList>("parentResult");
    BOBUIest::addColumn<QStringList>("childResult");
    BOBUIest::newRow("test 1")
        << (QStringList() << "c" << "d" << "a" << "b")
        << (QStringList() << "e" << "h" << "g" << "f")
        << (QStringList() << "d" << "c" << "b" << "a")
        << (QStringList() << "h" << "g" << "f" << "e");
}

// From Task 134978
void tst_BOBUIreeWidget::insertExpandedItemsWithSorting()
{
    QFETCH(const QStringList, parentTexts);
    QFETCH(const QStringList, childTexts);
    QFETCH(const QStringList, parentResult);
    QFETCH(const QStringList, childResult);

    // create a tree with autosorting enabled
    PublicTreeWidget tree;
    tree.setSortingEnabled(true);

    // insert expanded items in unsorted order
    QList<BOBUIreeWidgetItem *> items;
    for (const QString &text : parentTexts) {
        BOBUIreeWidgetItem *parent = new BOBUIreeWidgetItem(&tree, {text});
        parent->setExpanded(true);
        QVERIFY(parent->isExpanded());
        items << parent;
        for (const QString &text : childTexts) {
            BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(parent, {text});
            items << child;
        }
        QCOMPARE(parent->childCount(), childTexts.size());
        QVERIFY(parent->isExpanded());
    }
    QCOMPARE(tree.model()->rowCount(), parentTexts.size());

    // verify that the items are still expanded
    for (const BOBUIreeWidgetItem *item : std::as_const(items)) {
        if (item->childCount() > 0)
            QVERIFY(item->isExpanded());
        QModelIndex idx = tree.indexFromItem(item);
        QVERIFY(idx.isValid());
        //QRect rect = tree.visualRect(idx);
        //QVERIFY(rect.isValid());
        // ### it is not guarantied that the index is in the viewport
    }

    // verify that the tree is sorted
    QAbstractItemModel *model = tree.model();
    PersistentModelIndexVec parents;
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
        parents.push_back(model->index(i, 0, QModelIndex()));
    PersistentModelIndexVec children;
    for (int i = 0; i < model->rowCount(parents.constFirst()); ++i)
        children.push_back(model->index(i, 0, parents.constFirst()));
    for (int i = 0; i < parentResult.size(); ++i) {
        BOBUIreeWidgetItem *item = tree.topLevelItem(i);
        QCOMPARE(item->text(0), parentResult.at(i));
        for (int j = 0; j < childResult.size(); ++j)
            QCOMPARE(item->child(j)->text(0), childResult.at(j));
    }
}

void tst_BOBUIreeWidget::changeDataWithSorting_data()
{
    BOBUIest::addColumn<BobUI::SortOrder>("sortOrder");
    BOBUIest::addColumn<QStringList>("initialItems");
    BOBUIest::addColumn<int>("itemIndex");
    BOBUIest::addColumn<QString>("newValue");
    BOBUIest::addColumn<QStringList>("expectedItems");
    BOBUIest::addColumn<IntList>("expectedRows");
    BOBUIest::addColumn<bool>("reorderingExpected");

    BOBUIest::newRow("change a to b in (a)")
        << BobUI::AscendingOrder
        << (QStringList() << "a")
        << 0 << "b"
        << (QStringList() << "b")
        << (IntList() << 0)
        << false;
    BOBUIest::newRow("change a to b in (a, c)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c")
        << 0 << "b"
        << (QStringList() << "b" << "c")
        << (IntList() << 0 << 1)
        << false;
    BOBUIest::newRow("change a to c in (a, b)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "b")
        << 0 << "c"
        << (QStringList() << "b" << "c")
        << (IntList() << 1 << 0)
        << true;
    BOBUIest::newRow("change c to a in (c, b)")
        << BobUI::DescendingOrder
        << (QStringList() << "c" << "b")
        << 0 << "a"
        << (QStringList() << "b" << "a")
        << (IntList() << 1 << 0)
        << true;
    BOBUIest::newRow("change e to i in (a, c, e, g)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "e" << "g")
        << 2 << "i"
        << (QStringList() << "a" << "c" << "g" << "i")
        << (IntList() << 0 << 1 << 3 << 2)
        << true;
    BOBUIest::newRow("change e to a in (c, e, g, i)")
        << BobUI::AscendingOrder
        << (QStringList() << "c" << "e" << "g" << "i")
        << 1 << "a"
        << (QStringList() << "a" << "c" << "g" << "i")
        << (IntList() << 1 << 0 << 2 << 3)
        << true;
    BOBUIest::newRow("change e to f in (c, e, g, i)")
        << BobUI::AscendingOrder
        << (QStringList() << "c" << "e" << "g" << "i")
        << 1 << "f"
        << (QStringList() << "c" << "f" << "g" << "i")
        << (IntList() << 0 << 1 << 2 << 3)
        << false;
}

void tst_BOBUIreeWidget::changeDataWithSorting()
{
    QFETCH(BobUI::SortOrder, sortOrder);
    QFETCH(const QStringList, initialItems);
    QFETCH(int, itemIndex);
    QFETCH(const QString, newValue);
    QFETCH(const QStringList, expectedItems);
    QFETCH(const IntList, expectedRows);
    QFETCH(bool, reorderingExpected);

    BOBUIreeWidget w;
    w.setSortingEnabled(true);
    w.sortItems(0, sortOrder);
    for (const QString &str : initialItems)
      w.addTopLevelItem(new BOBUIreeWidgetItem({ str }));

    QAbstractItemModel *model = w.model();
    PersistentModelIndexVec persistent;
    for (int j = 0; j < model->rowCount(QModelIndex()); ++j)
        persistent << model->index(j, 0, QModelIndex());

    QSignalSpy dataChangedSpy(model, &QAbstractItemModel::dataChanged);
    QSignalSpy layoutChangedSpy(model, &QAbstractItemModel::layoutChanged);

    BOBUIreeWidgetItem *item = w.topLevelItem(itemIndex);
    item->setText(0, newValue);
    for (int i = 0; i < expectedItems.size(); ++i) {
        QCOMPARE(w.topLevelItem(i)->text(0), expectedItems.at(i));
        for (const QPersistentModelIndex &p : std::as_const(persistent)) {
            if (p.row() == i) // the same toplevel row
                QCOMPARE(p.internalPointer(), static_cast<void *>(w.topLevelItem(i)));
        }
    }

    for (int k = 0; k < persistent.size(); ++k)
        QCOMPARE(persistent.at(k).row(), expectedRows.at(k));

    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.size(), reorderingExpected ? 1 : 0);
}

void tst_BOBUIreeWidget::changeDataWithStableSorting_data()
{
    BOBUIest::addColumn<BobUI::SortOrder>("sortOrder");
    BOBUIest::addColumn<QStringList>("initialItems");
    BOBUIest::addColumn<int>("itemIndex");
    BOBUIest::addColumn<QString>("newValue");
    BOBUIest::addColumn<QStringList>("expectedItems");
    BOBUIest::addColumn<IntList>("expectedRows");
    BOBUIest::addColumn<bool>("reorderingExpected");
    BOBUIest::addColumn<bool>("forceChange");

    BOBUIest::newRow("change a to c in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 0 << "c"
        << (QStringList() << "c" << "c" << "c" << "c" << "e")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << false;
    BOBUIest::newRow("change e to c in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 4 << "c"
        << (QStringList() << "a" << "c" << "c" << "c" << "c")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << false;
    BOBUIest::newRow("change 1st c to c in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 1 << "c"
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << true;
    BOBUIest::newRow("change 2nd c to c in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 2 << "c"
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << true;
    BOBUIest::newRow("change 3rd c to c in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 3 << "c"
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << true;
    BOBUIest::newRow("change 1st c to c in (e, c, c, c, a)")
        << BobUI::DescendingOrder
        << (QStringList() << "e" << "c" << "c" << "c" << "a")
        << 1 << "c"
        << (QStringList() << "e" << "c" << "c" << "c" << "a")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << true;
    BOBUIest::newRow("change 2nd c to c in (e, c, c, c, a)")
        << BobUI::DescendingOrder
        << (QStringList() << "e" << "c" << "c" << "c" << "a")
        << 2 << "c"
        << (QStringList() << "e" << "c" << "c" << "c" << "a")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << true;
    BOBUIest::newRow("change 3rd c to c in (e, c, c, c, a)")
        << BobUI::DescendingOrder
        << (QStringList() << "e" << "c" << "c" << "c" << "a")
        << 3 << "c"
        << (QStringList() << "e" << "c" << "c" << "c" << "a")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << true;
    BOBUIest::newRow("change 1st c to b in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 1 << "b"
        << (QStringList() << "a" << "b" << "c" << "c" << "e")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << false;
    BOBUIest::newRow("change 2nd c to b in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 2 << "b"
        << (QStringList() << "a" << "b" << "c" << "c" << "e")
        << (IntList() << 0 << 2 << 1 << 3 << 4)
        << true
        << false;
    BOBUIest::newRow("change 3rd c to b in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 3 << "b"
        << (QStringList() << "a" << "b" << "c" << "c" << "e")
        << (IntList() << 0 << 2 << 3 << 1 << 4)
        << true
        << false;
    BOBUIest::newRow("change 1st c to d in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 1 << "d"
        << (QStringList() << "a" << "c" << "c" << "d" << "e")
        << (IntList() << 0 << 3 << 1 << 2 << 4)
        << true
        << false;
    BOBUIest::newRow("change 2nd c to d in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 2 << "d"
        << (QStringList() << "a" << "c" << "c" << "d" << "e")
        << (IntList() << 0 << 1 << 3 << 2 << 4)
        << true
        << false;
    BOBUIest::newRow("change 3rd c to d in (a, c, c, c, e)")
        << BobUI::AscendingOrder
        << (QStringList() << "a" << "c" << "c" << "c" << "e")
        << 3 << "d"
        << (QStringList() << "a" << "c" << "c" << "d" << "e")
        << (IntList() << 0 << 1 << 2 << 3 << 4)
        << false
        << false;
}

void tst_BOBUIreeWidget::changeDataWithStableSorting()
{
    QFETCH(BobUI::SortOrder, sortOrder);
    QFETCH(const QStringList, initialItems);
    QFETCH(int, itemIndex);
    QFETCH(const QString, newValue);
    QFETCH(const QStringList, expectedItems);
    QFETCH(const IntList, expectedRows);
    QFETCH(bool, reorderingExpected);
    QFETCH(bool, forceChange);

    BOBUIreeWidget w;
    w.setSortingEnabled(true);
    w.sortItems(0, sortOrder);
    for (const QString &str : initialItems)
      w.addTopLevelItem(new PublicTreeItem({ str }));

    QAbstractItemModel *model = w.model();
    PersistentModelIndexVec persistent;
    for (int j = 0; j < model->rowCount(QModelIndex()); ++j)
        persistent << model->index(j, 0, QModelIndex());

    QSignalSpy dataChangedSpy(model, &QAbstractItemModel::dataChanged);
    QSignalSpy layoutChangedSpy(model, &QAbstractItemModel::layoutChanged);

    auto *item = static_cast<PublicTreeItem *>(w.topLevelItem(itemIndex));
    item->setText(0, newValue);
    if (forceChange)
        item->emitDataChanged();
    for (int i = 0; i < expectedItems.size(); ++i) {
        QCOMPARE(w.topLevelItem(i)->text(0), expectedItems.at(i));
        for (const QPersistentModelIndex &p : std::as_const(persistent)) {
            if (p.row() == i) // the same toplevel row
                QCOMPARE(p.internalPointer(), static_cast<void *>(w.topLevelItem(i)));
        }
    }

    for (int k = 0; k < persistent.size(); ++k)
        QCOMPARE(persistent.at(k).row(), expectedRows.at(k));

    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.size(), reorderingExpected ? 1 : 0);
}

void tst_BOBUIreeWidget::sizeHint_data()
{
    BOBUIest::addColumn<BobUI::ScrollBarPolicy>("scrollBarPolicy");
    BOBUIest::addColumn<QSize>("viewSize");
    BOBUIest::newRow("ScrollBarAlwaysOn") << BobUI::ScrollBarAlwaysOn << QSize();
    BOBUIest::newRow("ScrollBarAlwaysOff") << BobUI::ScrollBarAlwaysOff << QSize();
    // make sure the scrollbars are shown by resizing the view to 40x40
    BOBUIest::newRow("ScrollBarAsNeeded (40x40)") << BobUI::ScrollBarAsNeeded << QSize(40, 40);
    BOBUIest::newRow("ScrollBarAsNeeded (1000x1000)") << BobUI::ScrollBarAsNeeded << QSize(1000, 1000);
}

void tst_BOBUIreeWidget::sizeHint()
{
    QFETCH(BobUI::ScrollBarPolicy, scrollBarPolicy);
    QFETCH(QSize, viewSize);

    const QString defaultStyle = QApplication::style()->name();
    QApplication::setStyle("fusion");
    const auto resetStyle = qScopeGuard([defaultStyle]{
        QApplication::setStyle(defaultStyle);
    });

    BOBUIreeWidget view;
    view.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view.setVerticalScrollBarPolicy(scrollBarPolicy);
    view.setHorizontalScrollBarPolicy(scrollBarPolicy);
    view.setColumnCount(2);
    for (int i = 0 ; i < view.columnCount(); ++i)
        view.addTopLevelItem(new BOBUIreeWidgetItem(QStringList{"foo","bar"}));

    view.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&view));

    if (viewSize.isValid()) {
        view.resize(viewSize);
        view.setColumnWidth(0, 100);
        BOBUIRY_COMPARE(view.size(), viewSize);
    }

    QApplication::processEvents(); // execute delayed layouts
    auto sizeHint = view.sizeHint();
    view.hide();
    QCOMPARE(view.sizeHint(), sizeHint);

    view.header()->hide();
    view.show();
    sizeHint = view.sizeHint();
    view.hide();
    QCOMPARE(view.sizeHint(), sizeHint);
}

void tst_BOBUIreeWidget::itemOperatorLessThan()
{
    BOBUIreeWidget tw;
    tw.setColumnCount(2);
    {
        BOBUIreeWidgetItem item1(&tw);
        BOBUIreeWidgetItem item2(&tw);
        QCOMPARE(item1 < item2, false);
        item1.setText(1, "a");
        item2.setText(1, "b");
        QCOMPARE(item1 < item2, false);
        item1.setText(0, "a");
        item2.setText(0, "b");
        QCOMPARE(item1 < item2, true);
        tw.sortItems(1, BobUI::AscendingOrder);
        item1.setText(0, "b");
        item2.setText(0, "a");
        QCOMPARE(item1 < item2, true);
        tw.sortItems(0, BobUI::AscendingOrder);
        item1.setData(0, BobUI::DisplayRole, 11);
        item2.setData(0, BobUI::DisplayRole, 2);
        QCOMPARE(item1 < item2, false);
    }
}

void tst_BOBUIreeWidget::sortedIndexOfChild_data()
{
    BOBUIest::addColumn<BobUI::SortOrder>("sortOrder");
    BOBUIest::addColumn<QStringList>("itemTexts");
    BOBUIest::addColumn<IntList>("expectedIndexes");

    BOBUIest::newRow("three ascending")
        << BobUI::AscendingOrder
        << (QStringList{"A", "B", "C"})
        << (IntList{0, 1, 2});


    BOBUIest::newRow("three descending")
        << BobUI::DescendingOrder
        << (QStringList{"A", "B", "C"})
        << (IntList{2, 1, 0});
}

void tst_BOBUIreeWidget::sortedIndexOfChild()
{
    QFETCH(BobUI::SortOrder, sortOrder);
    QFETCH(const QStringList, itemTexts);
    QFETCH(const IntList, expectedIndexes);

    BOBUIreeWidget tw;
    QList<BOBUIreeWidgetItem *> itms;
    auto *top = new BOBUIreeWidgetItem(&tw, {"top"});

    for (const QString &str : itemTexts)
      itms << new BOBUIreeWidgetItem(top, {str});

    tw.sortItems(0, sortOrder);
    tw.expandAll();

    QCOMPARE(itms.size(), expectedIndexes.size());
    for (int j = 0; j < expectedIndexes.size(); ++j)
        QCOMPARE(top->indexOfChild(itms.at(j)), expectedIndexes.at(j));
}

void tst_BOBUIreeWidget::expandAndCallapse()
{
    BOBUIreeWidget tw;
    BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(&tw, QStringList() << "top");
    BOBUIreeWidgetItem *p = nullptr;
    for (int i = 0; i < 10; ++i) {
        p = new BOBUIreeWidgetItem(top, QStringList(QString::number(i)));
        for (int j = 0; j < 10; ++j)
            new BOBUIreeWidgetItem(p, QStringList(QString::number(j)));
    }
    QSignalSpy spy0(&tw, &BOBUIreeWidget::itemExpanded);
    QSignalSpy spy1(&tw, &BOBUIreeWidget::itemCollapsed);


    tw.expandItem(p);
    tw.collapseItem(p);
    tw.expandItem(p);
    tw.expandItem(top);
    tw.collapseItem(top);
    tw.collapseItem(top);

    QCOMPARE(spy0.size(), 3);
    QCOMPARE(spy1.size(), 2);
}

void tst_BOBUIreeWidget::setDisabled()
{
    BOBUIreeWidget w;
    BOBUIreeWidgetItem *i1 = new BOBUIreeWidgetItem();
    BOBUIreeWidgetItem *i2 = new BOBUIreeWidgetItem(i1);
    BOBUIreeWidgetItem *i3 = new BOBUIreeWidgetItem(i1);

    BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(&w);
    top->setDisabled(true);
    top->addChild(i1);
    QCOMPARE(i1->isDisabled(), true);
    QCOMPARE(i2->isDisabled(), true);
    QCOMPARE(i3->isDisabled(), true);

    i1 = top->takeChild(0);
    QCOMPARE(i1->isDisabled(), false);
    QCOMPARE(i2->isDisabled(), false);
    QCOMPARE(i3->isDisabled(), false);

    top->addChild(i1);
    QCOMPARE(i1->isDisabled(), true);
    QCOMPARE(i2->isDisabled(), true);
    QCOMPARE(i3->isDisabled(), true);

    top->setDisabled(false);
    QCOMPARE(i1->isDisabled(), false);
    QCOMPARE(i2->isDisabled(), false);
    QCOMPARE(i3->isDisabled(), false);



    QList<BOBUIreeWidgetItem*> children;
    children.append(new BOBUIreeWidgetItem());
    children.append(new BOBUIreeWidgetItem());
    children.append(new BOBUIreeWidgetItem());
    {
        const QScopedPointer<BOBUIreeWidgetItem> taken(top->takeChild(0));
        QCOMPARE(taken.data(), i1);
    }

    top->addChildren(children);
    QCOMPARE(top->child(0)->isDisabled(), false);
    QCOMPARE(top->child(1)->isDisabled(), false);
    QCOMPARE(top->child(1)->isDisabled(), false);

    top->setDisabled(true);
    QCOMPARE(top->child(0)->isDisabled(), true);
    QCOMPARE(top->child(1)->isDisabled(), true);
    QCOMPARE(top->child(1)->isDisabled(), true);

    struct Deleter {
        QList<BOBUIreeWidgetItem *> items;
        explicit Deleter(QList<BOBUIreeWidgetItem *> items) : items(std::move(items)) {}
        ~Deleter() { qDeleteAll(items); }
    };

    const Deleter takenChildren(top->takeChildren());
    QCOMPARE(takenChildren.items[0]->isDisabled(), false);
    QCOMPARE(takenChildren.items[1]->isDisabled(), false);
    QCOMPARE(takenChildren.items[1]->isDisabled(), false);
}

void tst_BOBUIreeWidget::setSpanned()
{
    BOBUIreeWidget w;
    BOBUIreeWidgetItem *i1 = new BOBUIreeWidgetItem();
    QScopedPointer<BOBUIreeWidgetItem> i2(new BOBUIreeWidgetItem());

    BOBUIreeWidgetItem *top = new BOBUIreeWidgetItem(&w);
    top->addChild(i1);

    top->setFirstColumnSpanned(true);
    QCOMPARE(top->isFirstColumnSpanned(), true);
    QCOMPARE(i1->isFirstColumnSpanned(), false);
    QCOMPARE(i2->isFirstColumnSpanned(), false);

    top->setFirstColumnSpanned(false);
    i1->setFirstColumnSpanned(true);
    i2->setFirstColumnSpanned(true);
    QCOMPARE(top->isFirstColumnSpanned(), false);
    QCOMPARE(i1->isFirstColumnSpanned(), true);
    QCOMPARE(i2->isFirstColumnSpanned(), false);
}

void tst_BOBUIreeWidget::removeSelectedItem()
{
    const QScopedPointer <BOBUIreeWidget> w(new BOBUIreeWidget);
    w->setSortingEnabled(true);

    BOBUIreeWidgetItem *first = new BOBUIreeWidgetItem();
    first->setText(0, QLatin1String("D"));
    w->addTopLevelItem(first);

    BOBUIreeWidgetItem *itm = new BOBUIreeWidgetItem();
    itm->setText(0, QLatin1String("D"));
    w->addTopLevelItem(itm);

    itm = new BOBUIreeWidgetItem();
    itm->setText(0, QLatin1String("C"));
    w->addTopLevelItem(itm);
    itm->setSelected(true);

    itm = new BOBUIreeWidgetItem();
    itm->setText(0, QLatin1String("A"));
    w->addTopLevelItem(itm);

    //w->show();

    QItemSelectionModel *selModel = w->selectionModel();
    QCOMPARE(selModel->hasSelection(), true);
    QCOMPARE(selModel->selectedRows().size(), 1);

    const QScopedPointer<BOBUIreeWidgetItem> taken(w->takeTopLevelItem(2));
    QCOMPARE(taken->text(0), QLatin1String("C"));

    QCOMPARE(selModel->hasSelection(), false);
    QCOMPARE(selModel->selectedRows().size(), 0);
    QItemSelection sel = selModel->selection();
    QCOMPARE(selModel->isSelected(w->model()->index(0,0)), false);
}

void tst_BOBUIreeWidget::removeCurrentItem()
{
    PublicTreeWidget widget;
    connect(widget.selectionModel(),
            &QItemSelectionModel::currentChanged,
            &widget, &PublicTreeWidget::clear);
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(&widget);
    widget.setCurrentItem(item);
    widget.deleteCurrent();
}

void tst_BOBUIreeWidget::removeCurrentItem_task186451()
{
    PublicTreeWidget widget;
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(&widget, {"1"});
    BOBUIreeWidgetItem *item2 = new BOBUIreeWidgetItem(&widget, {"2"});
    widget.setCurrentItem(item);
    widget.deleteCurrent();

    QVERIFY(item2->isSelected());
    QCOMPARE(item2, widget.currentItem());
}

void tst_BOBUIreeWidget::randomExpand()
{
    BOBUIreeWidget tree;
    BOBUIreeWidgetItem *item1 = new BOBUIreeWidgetItem(&tree);
    BOBUIreeWidgetItem *item3 = new BOBUIreeWidgetItem(&tree, item1);
    new BOBUIreeWidgetItem(item1);
    new BOBUIreeWidgetItem(item3);

    tree.expandAll();

    /*
        item1
         \- item2
        item3
         \- item4
    */

    for (int i = 0; i < 100; i++) {
        auto newItem1 = new BOBUIreeWidgetItem(&tree, item1);
        newItem1->setExpanded(true);
        QCOMPARE(newItem1->isExpanded(), true);

        BOBUIreeWidgetItem *x = new BOBUIreeWidgetItem();
        QCOMPARE(newItem1->isExpanded(), true);
        newItem1->addChild(x);

        QCOMPARE(newItem1->isExpanded(), true);
    }
}

void tst_BOBUIreeWidget::crashTest()
{
    BOBUIreeWidget tree;
    tree.setColumnCount(1);
    tree.show();

    BOBUIreeWidgetItem *item1 = new BOBUIreeWidgetItem(&tree);
    item1->setText(0, "item1");
    item1->setExpanded(true);
    BOBUIreeWidgetItem *item2 = new BOBUIreeWidgetItem(item1);
    item2->setText(0, "item2");

    BOBUIreeWidgetItem *item3 = new BOBUIreeWidgetItem(&tree, item1);
    item3->setText(0, "item3");
    item3->setExpanded(true);
    BOBUIreeWidgetItem *item4 = new BOBUIreeWidgetItem(item3);
    item4->setText(0, "item4");

    BOBUIreeWidgetItem *item5 = new BOBUIreeWidgetItem(&tree, item3);
    item5->setText(0, "item5");
    item5->setExpanded(true);
    BOBUIreeWidgetItem *item6 = new BOBUIreeWidgetItem(item5);
    item6->setText(0, "item6");

    for (int i = 0; i < 1000; i++) {
        BOBUIreeWidgetItem *newItem1 = new BOBUIreeWidgetItem(&tree, item1);
        newItem1->setText(0, "newItem");
        BOBUIreeWidgetItem *newItem2 = new BOBUIreeWidgetItem(newItem1);
        newItem2->setText(0, "subItem1");
        BOBUIreeWidgetItem *newItem3 = new BOBUIreeWidgetItem(newItem1, newItem2);
        newItem3->setText(0, "subItem2");
        delete item3;
        item3 = newItem1;
    }
    QCoreApplication::processEvents();
}

class CrashWidget : public BOBUIreeWidget
{
public:
    CrashWidget(QWidget *parent = nullptr) : BOBUIreeWidget(parent)
    {
        setSortingEnabled(true);
        timerId = startTimer(10);
    }
    int i = 0;
protected:
    void timerEvent(BOBUIimerEvent * event) override
    {
        if (event->timerId() == timerId) {
            auto newItem = new BOBUIreeWidgetItem({QString::number(i++)});
            m_list.append(newItem);
            insertTopLevelItem(0, newItem);
            while (m_list.size() > 10)
                delete m_list.takeFirst();
        }
        BOBUIreeWidget::timerEvent(event);
    }
private:
    int timerId;
    QList<BOBUIreeWidgetItem *> m_list;
};

void tst_BOBUIreeWidget::sortAndSelect()
{
    CrashWidget w;
    w.resize(1, 1);
    w.show();
    while (w.i < 100) {
        QApplication::processEvents();
        if (w.i & 16) {
            QPoint pt = w.viewport()->rect().center();
            BOBUIest::mouseClick(w.viewport(), BobUI::LeftButton, BobUI::NoModifier, pt);
        }
    }
    QVERIFY(true);
}

void tst_BOBUIreeWidget::defaultRowSizes()
{
    const QScopedPointer<BOBUIreeWidget> tw(new BOBUIreeWidget);
    tw->setIconSize(QSize(50, 50));
    tw->setColumnCount(6);
    for (int i = 0; i < 10; ++i) {
        auto it = new BOBUIreeWidgetItem(tw.data());
        for (int j = 0; j < tw->columnCount() - 1; ++j)
            it->setText(j, "This is a test");
        auto sp = static_cast<QStyle::StandardPixmap>(i + QStyle::SP_TitleBarMenuButton);
        QPixmap icon = tw->style()->standardPixmap(sp);

        if (icon.isNull())
            QSKIP("No pixmap found on current style, skipping this test.");
        it->setIcon(tw->columnCount() - 1,
                    icon.scaled(tw->iconSize()));
    }
    tw->resize(100,100);
    tw->show();
    QApplication::processEvents();

    QRect visualRect = tw->visualItemRect(tw->topLevelItem(0));
    QVERIFY(visualRect.height() >= 50);
}

void tst_BOBUIreeWidget::task191552_rtl()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BobUI::LayoutDirection oldDir = QGuiApplication::layoutDirection();
    QGuiApplication::setLayoutDirection(BobUI::RightToLeft);

    BOBUIreeWidget tw;
    tw.setColumnCount(1);
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(&tw);
    item->setText(0, "item 1");
    item->setCheckState(0, BobUI::Checked);
    QCOMPARE(item->checkState(0), BobUI::Checked);
    tw.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&tw));
    QStyleOptionViewItem opt;
    opt.initFrom(&tw);
    opt.rect = tw.visualItemRect(item);
    // mimic QStyledItemDelegate::initStyleOption logic
    opt.features = QStyleOptionViewItem::HasDisplay | QStyleOptionViewItem::HasCheckIndicator;
    opt.checkState = BobUI::Checked;
    opt.widget = &tw;
    const QRect checkRect = tw.style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &opt, &tw);
    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::NoModifier, checkRect.center());
    QCOMPARE(item->checkState(0), BobUI::Unchecked);

    QGuiApplication::setLayoutDirection(oldDir);
}

void tst_BOBUIreeWidget::task203673_selection()
{
    //we try to change the selection by rightclick + ctrl
    //it should do anything when using ExtendedSelection

    BOBUIreeWidget tw;
    tw.setColumnCount(1);
    BOBUIreeWidgetItem *item1 = new BOBUIreeWidgetItem(&tw);
    item1->setText(0, "item 1");
    tw.setSelectionMode(BOBUIreeView::ExtendedSelection);

    QPoint center = tw.visualItemRect(item1).center();
    QCOMPARE(item1->isSelected(), false);

    BOBUIest::mouseClick(tw.viewport(), BobUI::RightButton, BobUI::ControlModifier, center);
    QCOMPARE(item1->isSelected(), false);

    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::ControlModifier, center);
    QCOMPARE(item1->isSelected(), true);

    BOBUIest::mouseClick(tw.viewport(), BobUI::RightButton, BobUI::ControlModifier, center);
    QCOMPARE(item1->isSelected(), true); //it shouldn't change

    BOBUIest::mouseClick(tw.viewport(), BobUI::LeftButton, BobUI::ControlModifier, center);
    QCOMPARE(item1->isSelected(), false);
}


void tst_BOBUIreeWidget::rootItemFlags()
{
    BOBUIreeWidget tw;
    tw.setColumnCount(1);
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(&tw);
    item->setText(0, "item 1");

    QVERIFY(tw.invisibleRootItem()->flags() & BobUI::ItemIsDropEnabled);

    tw.invisibleRootItem()->setFlags(tw.invisibleRootItem()->flags() & ~BobUI::ItemIsDropEnabled);

    QVERIFY(!(tw.invisibleRootItem()->flags() & BobUI::ItemIsDropEnabled));
}

void tst_BOBUIreeWidget::task218661_setHeaderData()
{
    //We check that setting header data out of bounds returns false
    //and doesn't increase the size of the model
    BOBUIreeWidget tw;
    tw.setColumnCount(1);
    QCOMPARE(tw.columnCount(), 1);

    QCOMPARE(tw.model()->setHeaderData(99999, BobUI::Horizontal, QVariant()), false);

    QCOMPARE(tw.columnCount(), 1);
}

void tst_BOBUIreeWidget::task245280_sortChildren()
{
    BOBUIreeWidget tw;
    tw.setColumnCount(2);

    BOBUIreeWidgetItem top(&tw);
    top.setText(0,"Col 0");
    top.setText(1,"Col 1");
    BOBUIreeWidgetItem item1(&top);
    item1.setText(0,"X");
    item1.setText(1,"0");
    BOBUIreeWidgetItem item2(&top);
    item2.setText(0,"A");
    item2.setText(1,"4");
    BOBUIreeWidgetItem item3(&top);
    item3.setText(0,"E");
    item3.setText(1,"1");
    BOBUIreeWidgetItem item4(&top);
    item4.setText(0,"Z");
    item4.setText(1,"3");
    BOBUIreeWidgetItem item5(&top);
    item5.setText(0,"U");
    item5.setText(1,"2");
    tw.expandAll();
    tw.show();
    top.sortChildren(1,BobUI::AscendingOrder);

    for (int i = 0; i < top.childCount(); ++i)
        QCOMPARE(top.child(i)->text(1), QString::number(i));
}

void tst_BOBUIreeWidget::task253109_itemHeight()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIreeWidget treeWidget;
    treeWidget.setColumnCount(1);
    treeWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&treeWidget));

    BOBUIreeWidgetItem item(&treeWidget);
    class MyWidget : public QWidget
    {
        QSize sizeHint() const override { return QSize(200, 100); }
    } w;
    treeWidget.setItemWidget(&item, 0, &w);

    BOBUIRY_COMPARE(w.geometry(), treeWidget.visualItemRect(&item));
}

void tst_BOBUIreeWidget::task206367_duplication()
{
    QWidget topLevel;
    // Explicitly set the font size because it is dpi dependent on some platforms
    QFont font;
    font.setPixelSize(40);
    topLevel.setFont(font);
    BOBUIreeWidget treeWidget(&topLevel);
    topLevel.show();
    treeWidget.resize(200, 200);
    treeWidget.setHeaderHidden(true);

    treeWidget.setSortingEnabled(true);
    BOBUIreeWidgetItem* rootItem = new BOBUIreeWidgetItem(&treeWidget, QStringList("root"));
    for (int nFile = 0; nFile < 2; nFile++ )  {
        BOBUIreeWidgetItem* itemFile = new BOBUIreeWidgetItem(rootItem, {QString::number(nFile)});
        for (int nRecord = 0; nRecord < 2; nRecord++)
            new BOBUIreeWidgetItem(itemFile,  {QString::number(nRecord)});
        itemFile->setExpanded(true);
    }
    rootItem->setExpanded(true);

    //there should be enough room for 2x2 items.  If there is a scrollbar, it means the items are duplicated
    BOBUIRY_VERIFY(!treeWidget.verticalScrollBar()->isVisible());
}

void tst_BOBUIreeWidget::itemSelectionChanged()
{
    QVERIFY(testWidget);
    if (testWidget->topLevelItem(0))
        QVERIFY(testWidget->topLevelItem(0)->isSelected());
}

void tst_BOBUIreeWidget::selectionOrder()
{
    testWidget->setColumnCount(1);
    QList<BOBUIreeWidgetItem *> items;
    for (int i = 0; i < 10; ++i) {
        items.append(new BOBUIreeWidgetItem(static_cast<BOBUIreeWidget *>(nullptr),
                                         {QStringLiteral("item: %1").arg(i)}));
    }
    testWidget->insertTopLevelItems(0, items);

    QModelIndex idx = testWidget->indexFromItem(items.at(0));
    connect(testWidget, &BOBUIreeWidget::itemSelectionChanged,
            this, &tst_BOBUIreeWidget::itemSelectionChanged);
    testWidget->selectionModel()->select(idx, QItemSelectionModel::SelectCurrent);
    disconnect(testWidget, &BOBUIreeWidget::itemSelectionChanged,
               this, &tst_BOBUIreeWidget::itemSelectionChanged);
}

void tst_BOBUIreeWidget::setSelectionModel()
{
    BOBUIreeWidget tree;
    for(int i = 0; i < 3; ++i)
        new BOBUIreeWidgetItem(&tree, QStringList(QString::number(i)));
    QItemSelectionModel selection(tree.model());
    selection.select(tree.model()->index(1, 0), QItemSelectionModel::Select);
    tree.setSelectionModel(&selection);
    QCOMPARE(tree.topLevelItem(1)->isSelected(), true);
}

void tst_BOBUIreeWidget::task217309()
{
    BOBUIreeWidgetItem item;
    item.setFlags(item.flags() | BobUI::ItemIsAutoTristate);
    BOBUIreeWidgetItem subitem1;
    subitem1.setFlags(subitem1.flags() | BobUI::ItemIsAutoTristate);
    BOBUIreeWidgetItem subitem2;
    subitem2.setFlags(subitem2.flags() | BobUI::ItemIsAutoTristate);
    item.addChild(&subitem1);
    item.addChild(&subitem2);
    subitem1.setCheckState(0, BobUI::Checked);
    subitem2.setCheckState(0, BobUI::Unchecked);

    QVERIFY(item.data(0, BobUI::CheckStateRole) == BobUI::PartiallyChecked);

    subitem2.setCheckState(0, BobUI::PartiallyChecked);
    QVERIFY(item.data(0, BobUI::CheckStateRole) == BobUI::PartiallyChecked);

    subitem2.setCheckState(0, BobUI::Checked);
    QVERIFY(item.data(0, BobUI::CheckStateRole) == BobUI::Checked);
}

void tst_BOBUIreeWidget::nonEditableTristate()
{
    // A tree with checkable items, the parent is tristate
    BOBUIreeWidget tree;
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem;
    tree.insertTopLevelItem(0, item);
    item->setFlags(item->flags() | BobUI::ItemIsAutoTristate);
    item->setCheckState(0, BobUI::Unchecked);
    BOBUIreeWidgetItem *subitem1 = new BOBUIreeWidgetItem(item);
    subitem1->setCheckState(0, BobUI::Unchecked);
    BOBUIreeWidgetItem *subitem2 = new BOBUIreeWidgetItem(item);
    subitem2->setCheckState(0, BobUI::Unchecked);
    QCOMPARE(int(item->checkState(0)), int(BobUI::Unchecked));
    tree.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tree));

    // Test clicking on the parent item, it should become Checked (not PartiallyChecked)
    QStyleOptionViewItem option;
    option.rect = tree.visualRect(tree.model()->index(0, 0));
    option.state |= QStyle::State_Enabled;
    option.features |= QStyleOptionViewItem::HasCheckIndicator | QStyleOptionViewItem::HasDisplay;
    option.checkState = item->checkState(0);

    auto appStyle = QApplication::style();
    const int checkMargin = appStyle->pixelMetric(
          QStyle::PM_FocusFrameHMargin, nullptr, nullptr) + 1;
    QPoint pos = appStyle->subElementRect(
          QStyle::SE_ItemViewItemCheckIndicator, &option, nullptr).center();
    pos.rx() += checkMargin;
    BOBUIest::mouseClick(tree.viewport(), BobUI::LeftButton, BobUI::NoModifier, pos);
    QCOMPARE(item->checkState(0), BobUI::Checked);

    // Click again, it should become Unchecked.
    BOBUIest::mouseClick(tree.viewport(), BobUI::LeftButton, BobUI::NoModifier, pos);
    QCOMPARE(item->checkState(0), BobUI::Unchecked);
}

void tst_BOBUIreeWidget::emitDataChanged()
{
    BOBUIreeWidget tree;
    QSignalSpy spy(&tree, &BOBUIreeWidget::itemChanged);
    auto item = new PublicTreeItem;
    tree.insertTopLevelItem(0, item);
    item->emitDataChanged();
    QCOMPARE(spy.size(), 1);
}

void tst_BOBUIreeWidget::setCurrentItemExpandsParent()
{
    BOBUIreeWidget w;
    w.setColumnCount(1);
    BOBUIreeWidgetItem *i1 = new BOBUIreeWidgetItem(&w, {"parent"});
    BOBUIreeWidgetItem *i2 = new BOBUIreeWidgetItem(i1, {"child"});
    QVERIFY(!i2->isExpanded());
    QVERIFY(!w.currentItem());
    w.setCurrentItem(i2);
    QVERIFY(!i2->isExpanded());
    QCOMPARE(w.currentItem(), i2);
}

void tst_BOBUIreeWidget::task239150_editorWidth()
{
    //we check that an item with no text will get an editor with a correct size
    BOBUIreeWidget tree;

    QStyleOptionFrame opt;
    opt.initFrom(&tree);
    const int minWidth = tree.style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(0, 0)
                                                        , nullptr).width();

    {
        BOBUIreeWidgetItem item;
        item.setFlags(BobUI::ItemIsSelectable | BobUI::ItemIsEditable | BobUI::ItemIsEnabled );
        tree.addTopLevelItem(&item);
        QVERIFY(tree.itemWidget(&item, 0) == nullptr);
        tree.editItem(&item);
        QVERIFY(tree.itemWidget(&item, 0));
        QVERIFY(tree.itemWidget(&item, 0)->width() >= minWidth);
    }

    //now let's test it with an item with a lot of text
    {
        BOBUIreeWidgetItem item;
        item.setText(0, "foooooooooooooooooooooooo");
        item.setFlags(BobUI::ItemIsSelectable | BobUI::ItemIsEditable | BobUI::ItemIsEnabled );
        tree.addTopLevelItem(&item);
        QVERIFY(tree.itemWidget(&item, 0) == nullptr);
        tree.editItem(&item);
        QVERIFY(tree.itemWidget(&item, 0));
        QVERIFY(tree.itemWidget(&item, 0)->width() >= minWidth + tree.fontMetrics().horizontalAdvance(item.text(0)));
    }
}



void tst_BOBUIreeWidget::setTextUpdate()
{
    BOBUIreeWidget treeWidget;
    treeWidget.setColumnCount(2);

    class MyItemDelegate : public QStyledItemDelegate
    {
    public:
        using QStyledItemDelegate::QStyledItemDelegate;
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
        {
            numPaints++;
            QStyledItemDelegate::paint(painter, option, index);
        }

        mutable int numPaints = 0;
    } delegate;

    treeWidget.setItemDelegate(&delegate);
    treeWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&treeWidget));
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem({ "variable1", "0" });
    treeWidget.insertTopLevelItem(0, item);
    BOBUIRY_VERIFY(delegate.numPaints > 0);
    delegate.numPaints = 0;

    item->setText(1, "42");
    BOBUIRY_VERIFY(delegate.numPaints > 0);
}

void tst_BOBUIreeWidget::taskBOBUIBUG2844_visualItemRect()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    PublicTreeWidget tree;
    tree.resize(150, 100);
    tree.setColumnCount(3);
    BOBUIreeWidgetItem item(&tree);

    QRect rectCol0 = tree.visualRect(tree.indexFromItem(&item, 0));
    QRect rectCol1 = tree.visualRect(tree.indexFromItem(&item, 1));
    QRect rectCol2 = tree.visualRect(tree.indexFromItem(&item, 2));

    QCOMPARE(tree.visualItemRect(&item), rectCol0 | rectCol2);
    tree.setColumnHidden(2, true);
    QCOMPARE(tree.visualItemRect(&item), rectCol0 | rectCol1);
}

void tst_BOBUIreeWidget::setChildIndicatorPolicy()
{
    BOBUIreeWidget treeWidget;
    treeWidget.setColumnCount(1);

    class MyItemDelegate : public QStyledItemDelegate
    {
    public:
        using QStyledItemDelegate::QStyledItemDelegate;
        void paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
        {
            numPaints++;
            QCOMPARE(!(option.state & QStyle::State_Children), !expectChildren);
            QStyledItemDelegate::paint(painter, option, index);
        }
        mutable int numPaints = 0;
        bool expectChildren = false;
    } delegate;

    treeWidget.setItemDelegate(&delegate);
    treeWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&treeWidget));
    QCoreApplication::processEvents(); // Process all queued paint events

    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(QStringList("Hello"));
    treeWidget.insertTopLevelItem(0, item);
    BOBUIRY_VERIFY(delegate.numPaints > 0);

    delegate.numPaints = 0;
    delegate.expectChildren = true;
    item->setChildIndicatorPolicy(BOBUIreeWidgetItem::ShowIndicator);
    BOBUIRY_COMPARE(delegate.numPaints, 1);

    delegate.numPaints = 0;
    delegate.expectChildren = false;
    item->setChildIndicatorPolicy(BOBUIreeWidgetItem::DontShowIndicatorWhenChildless);
    BOBUIRY_COMPARE(delegate.numPaints, 1);

    delegate.numPaints = 0;
    delegate.expectChildren = true;
    new BOBUIreeWidgetItem(item);
    BOBUIRY_COMPARE(delegate.numPaints, 1);

    delegate.numPaints = 0;
    delegate.expectChildren = false;
    item->setChildIndicatorPolicy(BOBUIreeWidgetItem::DontShowIndicator);
    BOBUIRY_COMPARE(delegate.numPaints, 1);
}

// From BOBUIBUG_34717 (BOBUIreeWidget crashes when scrolling to the end
// of an expanded tree, then collapse all)
// The test passes simply if it doesn't crash.
void tst_BOBUIreeWidget::taskBOBUIBUG_34717_collapseAtBottom()
{
    PublicTreeWidget treeWidget;
    treeWidget.header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeWidget.setColumnCount(2);
    BOBUIreeWidgetItem *mainItem = new BOBUIreeWidgetItem(&treeWidget, { "Root" });
    for (int i = 0; i < 200; ++i) {
        BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(mainItem, { "Item" });
        new BOBUIreeWidgetItem(item, { "Child", "1" });
        new BOBUIreeWidgetItem(item, { "Child", "2" });
        new BOBUIreeWidgetItem(item, { "Child", "3" });
    }
    treeWidget.show();
    treeWidget.expandAll();
    treeWidget.scrollToBottom();
    treeWidget.collapseAll();

    treeWidget.setAnimated(true);
    treeWidget.expandAll();
    treeWidget.scrollToBottom();
    mainItem->setExpanded(false);

    QVERIFY(treeWidget.sizeHintForColumn(1) >= 0);
}

void tst_BOBUIreeWidget::task20345_sortChildren()
{
    if (!QGuiApplication::platformName().compare(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This causes a crash triggered by setVisible(false)");

    // This test case is considered successful if it is executed (no crash in sorting)
    BOBUIreeWidget tw;
    tw.setColumnCount(3);
    tw.headerItem()->setText(0, "Col 0");
    tw.headerItem()->setText(1, "Col 1");
    tw.header()->setSortIndicator(0, BobUI::AscendingOrder);
    tw.setSortingEnabled(true);
    tw.show();

    auto rootItem = new BOBUIreeWidgetItem(&tw, QStringList("a"));
    auto childItem = new BOBUIreeWidgetItem(rootItem);
    childItem->setText(1, "3");
    childItem = new BOBUIreeWidgetItem(rootItem);
    childItem->setText(1, "1");
    childItem = new BOBUIreeWidgetItem(rootItem);
    childItem->setText(1, "2");

    tw.setCurrentItem(tw.topLevelItem(0));

    BOBUIreeWidgetItem *curItem = tw.currentItem();
    int childCount = curItem->childCount() + 1;

    BOBUIreeWidgetItem *newItem = new BOBUIreeWidgetItem(curItem);
    newItem->setText(1, QString::number(childCount));
    rootItem->sortChildren(1, BobUI::AscendingOrder);
    QVERIFY(1);
}

void tst_BOBUIreeWidget::getMimeDataWithInvalidItem()
{
    PublicTreeWidget w;
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIreeWidget::mimeData: Null-item passed");
    QMimeData *md = w.mimeData(QList<BOBUIreeWidgetItem*>() << nullptr);
    QVERIFY(!md);
}

// visualItemRect returned a wrong rect when the columns were moved
// (-> logical index != visual index). see BOBUIBUG-28733
void tst_BOBUIreeWidget::testVisualItemRect()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    BOBUIreeWidget tw;
    tw.setColumnCount(2);
    BOBUIreeWidgetItem *item = new BOBUIreeWidgetItem(&tw);
    item->setText(0, "text 0");
    item->setText(1, "text 1");

    static const int sectionSize = 30;
    tw.header()->setStretchLastSection(false);
    tw.header()->setMinimumSectionSize(sectionSize);
    tw.header()->resizeSection(0, sectionSize);
    tw.header()->resizeSection(1, sectionSize);
    tw.setRootIsDecorated(false);
    tw.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tw));

    QRect r = tw.visualItemRect(item);
    QCOMPARE(r.width(), sectionSize * 2);   // 2 columns
    tw.header()->moveSection(1, 0);
    r = tw.visualItemRect(item);
    QCOMPARE(r.width(), sectionSize * 2);   // 2 columns
    tw.hideColumn(0);
    r = tw.visualItemRect(item);
    QCOMPARE(r.width(), sectionSize);
}

void tst_BOBUIreeWidget::reparentHiddenItem()
{
    BOBUIreeWidgetItem *parent = new BOBUIreeWidgetItem(testWidget);
    parent->setText(0, "parent");
    BOBUIreeWidgetItem *otherParent = new BOBUIreeWidgetItem(testWidget);
    otherParent->setText(0, "other parent");
    BOBUIreeWidgetItem *child = new BOBUIreeWidgetItem(parent);
    child->setText(0, "child");
    BOBUIreeWidgetItem *grandChild = new BOBUIreeWidgetItem(child);
    grandChild->setText(0, "grandchild");
    QVERIFY(child->parent());
    QVERIFY(grandChild->parent());

    testWidget->expandItem(parent);
    testWidget->expandItem(otherParent);
    testWidget->expandItem(child);

    QVERIFY(!parent->isHidden());
    QVERIFY(!child->isHidden());
    QVERIFY(!grandChild->isHidden());

    grandChild->setHidden(true);

    QVERIFY(grandChild->isHidden());
    parent->removeChild(child);
    otherParent->addChild(child);
    QVERIFY(grandChild->isHidden());
}

void tst_BOBUIreeWidget::persistentChildIndex() // BOBUIBUG-90030
{
    BOBUIreeWidget tree;
    BOBUIreeWidgetItem *toplevel = new BOBUIreeWidgetItem(QStringList{QStringLiteral("toplevel")});
    tree.addTopLevelItem(toplevel);
    QModelIndex firstIndex = tree.model()->index(0, 0);
    BOBUIreeWidgetItem *child1 = new BOBUIreeWidgetItem(QStringList{QStringLiteral("child1")});
    BOBUIreeWidgetItem *child2 = new BOBUIreeWidgetItem(QStringList{QStringLiteral("child2")});
    toplevel->addChildren({child1, child2});
    QPersistentModelIndex persistentIdx = tree.model()->index(1, 0, firstIndex);
    QCOMPARE(persistentIdx.data().toString(), QStringLiteral("child2"));
    tree.model()->removeRows(0, 1, firstIndex);
    QCOMPARE(persistentIdx.data().toString(), QStringLiteral("child2"));
}

void tst_BOBUIreeWidget::clearItemData()
{
    BOBUIreeWidget tree;
    QAbstractItemModel* model = tree.model();
    QVERIFY(model->insertColumn(0));
    QVERIFY(model->insertRow(0));
    const QModelIndex parentIdx = model->index(0, 0);
    QVERIFY(model->insertColumn(0, parentIdx));
    QVERIFY(model->insertRow(0, parentIdx));
    const QModelIndex childIdx = model->index(0, 0, parentIdx);
    model->setData(parentIdx, QStringLiteral("parent"));
    model->setData(parentIdx, QStringLiteral("parent"), BobUI::UserRole);
    model->setData(childIdx, QStringLiteral("child"));
    QSignalSpy dataChangeSpy(model, &QAbstractItemModel::dataChanged);
    QVERIFY(dataChangeSpy.isValid());
    QVERIFY(!model->clearItemData(QModelIndex()));
    QCOMPARE(dataChangeSpy.size(), 0);
    QVERIFY(model->clearItemData(parentIdx));
    QVERIFY(!model->data(parentIdx).isValid());
    QVERIFY(!model->data(parentIdx, BobUI::UserRole).isValid());
    QCOMPARE(dataChangeSpy.size(), 1);
    QList<QVariant> dataChangeArgs = dataChangeSpy.takeFirst();
    QCOMPARE(dataChangeArgs.at(0).value<QModelIndex>(), parentIdx);
    QCOMPARE(dataChangeArgs.at(1).value<QModelIndex>(), parentIdx);
    QVERIFY(dataChangeArgs.at(2).value<QList<int>>().isEmpty());
    QVERIFY(model->clearItemData(parentIdx));
    QCOMPARE(dataChangeSpy.size(), 0);
    QVERIFY(model->clearItemData(childIdx));
    QVERIFY(!model->data(childIdx).isValid());
    QCOMPARE(dataChangeSpy.size(), 1);
    dataChangeArgs = dataChangeSpy.takeFirst();
    QCOMPARE(dataChangeArgs.at(0).value<QModelIndex>(), childIdx);
    QCOMPARE(dataChangeArgs.at(1).value<QModelIndex>(), childIdx);
    QVERIFY(dataChangeArgs.at(2).value<QList<int>>().isEmpty());
}

void tst_BOBUIreeWidget::createPersistentOnLayoutAboutToBeChanged() // BOBUIBUG-93466
{
    BOBUIreeWidget widget;
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

void tst_BOBUIreeWidget::createPersistentOnLayoutAboutToBeChangedAutoSort() // BOBUIBUG-93466
{
    BOBUIreeWidget widget;
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

#if BOBUI_CONFIG(draganddrop)
class MoveOnlyTreeWidget : public BOBUIreeWidget
{
    Q_OBJECT
public:
    using BOBUIreeWidget::BOBUIreeWidget;
    BobUI::DropActions supportedDropActions() const override { return BobUI::MoveAction; }
};

void tst_BOBUIreeWidget::supportedDragActions()
{
    MoveOnlyTreeWidget treeWidget;
    QCOMPARE(treeWidget.model()->supportedDropActions(), BobUI::MoveAction);
    // For BobUI < 6.8 compatibility reasons, supportedDragActions defaults to supportedDropActions
    QCOMPARE(treeWidget.model()->supportedDragActions(), BobUI::MoveAction);

    treeWidget.setSupportedDragActions(BobUI::CopyAction);
    QCOMPARE(treeWidget.model()->supportedDragActions(), BobUI::CopyAction);
}
#endif // BOBUI_CONFIG(draganddrop)

BOBUIEST_MAIN(tst_BOBUIreeWidget)
#include "tst_bobuireewidget.moc"
