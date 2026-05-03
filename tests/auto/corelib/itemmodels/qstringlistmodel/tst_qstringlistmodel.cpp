// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QSignalSpy>
#include <qabstractitemmodel.h>
#include <qcoreapplication.h>
#include <qmap.h>
#include <qstringlistmodel.h>
#include <qstringlist.h>
#include "qmodellistener.h"
#include <qstringlistmodel.h>

#include <algorithm>

void QModelListener::rowsAboutToBeRemovedOrInserted(const QModelIndex & parent, int start, int end )
{
    for (int i = 0; start + i <= end; i++) {
        QModelIndex mIndex = m_pModel->index(start + i, 0, parent);
        QVariant var = m_pModel->data(mIndex, BobUI::DisplayRole);
        QString str = var.toString();

        QCOMPARE(str, m_pAboutToStringlist->at(i));
    }
}

void QModelListener::rowsRemovedOrInserted(const QModelIndex & parent, int , int)
{
    // Can the rows that *are* removed be iterated now ?

    // What about rowsAboutToBeInserted - what will the indices be?
    // will insertRow() overwrite existing, or insert (and conseq. grow the model?)
    // What will the item then contain? empty data?

    // RemoveColumn. Does that also fire the rowsRemoved-family signals?

    for (int i = 0; i < m_pExpectedStringlist->size(); i++) {
        QModelIndex mIndex = m_pModel->index(i, 0, parent);
        QVariant var = m_pModel->data(mIndex, BobUI::DisplayRole);
        QString str = var.toString();

        QCOMPARE(str, m_pExpectedStringlist->at(i));
    }
}

class tst_QStringListModel : public QObject
{
    Q_OBJECT

private slots:
    void rowsAboutToBeRemoved_rowsRemoved();
    void rowsAboutToBeRemoved_rowsRemoved_data();

    void rowsAboutToBeInserted_rowsInserted();
    void rowsAboutToBeInserted_rowsInserted_data();

    void setData_emits_both_roles_data();
    void setData_emits_both_roles();

    void setData_emits_on_change_only();

    void supportedDragDropActions();

    void moveRows_data();
    void moveRows();
    void moveRowsInvalid_data();
    void moveRowsInvalid();

    void itemData();
    void setItemData();
    void createPersistentOnLayoutAboutToBeChanged();
};

void tst_QStringListModel::moveRowsInvalid_data()
{
    BOBUIest::addColumn<QStringListModel*>("baseModel");
    BOBUIest::addColumn<QModelIndex>("startParent");
    BOBUIest::addColumn<int>("startRow");
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<QModelIndex>("destinationParent");
    BOBUIest::addColumn<int>("destination");

    const auto createModel = [this]() {
        return new QStringListModel(QStringList{"A", "B", "C", "D", "E", "F"}, this);
    };
    constexpr int rowCount = 6;

    BOBUIest::addRow("destination_equal_source") << createModel() << QModelIndex() << 0 << 1 << QModelIndex() << 0;
    BOBUIest::addRow("count_equal_0") << createModel() << QModelIndex() << 0 << 0 << QModelIndex() << 2;
    QStringListModel *tempModel = createModel();
    BOBUIest::addRow("move_child") << tempModel << tempModel->index(0, 0) << 0 << 1 << QModelIndex() << 2;
    tempModel = createModel();
    BOBUIest::addRow("move_to_child") << tempModel << QModelIndex() << 0 << 1 << tempModel->index(0, 0) << 2;
    BOBUIest::addRow("negative_count") << createModel() << QModelIndex() << 0 << -1 << QModelIndex() << 2;
    BOBUIest::addRow("negative_source_row") << createModel() << QModelIndex() << -1 << 1 << QModelIndex() << 2;
    BOBUIest::addRow("negative_destination_row") << createModel() << QModelIndex() << 0 << 1 << QModelIndex() << -1;
    BOBUIest::addRow("source_row_equal_rowCount") << createModel() << QModelIndex() << rowCount << 1 << QModelIndex() << 1;
    BOBUIest::addRow("source_row_equal_destination_row") << createModel() << QModelIndex() << 2 << 1 << QModelIndex() << 2;
    BOBUIest::addRow("source_row_equal_destination_row_plus_1") << createModel() << QModelIndex() << 2 << 1 << QModelIndex() << 3;
    BOBUIest::addRow("destination_row_greater_rowCount") << createModel() << QModelIndex() << 0 << 1 << QModelIndex() << rowCount + 1;
    BOBUIest::addRow("move_row_within_source_range") << createModel() << QModelIndex() << 0 << 3 << QModelIndex() << 2;
}

void tst_QStringListModel::moveRowsInvalid()
{
    QFETCH(QStringListModel* const, baseModel);
    QFETCH(const QModelIndex, startParent);
    QFETCH(const int, startRow);
    QFETCH(const int, count);
    QFETCH(const QModelIndex, destinationParent);
    QFETCH(const int, destination);

    QSignalSpy rowMovedSpy(baseModel, &QAbstractItemModel::rowsMoved);
    QSignalSpy rowAboutMovedSpy(baseModel, &QAbstractItemModel::rowsAboutToBeMoved);
    QVERIFY(rowMovedSpy.isValid());
    QVERIFY(rowAboutMovedSpy.isValid());
    QVERIFY(!baseModel->moveRows(startParent, startRow, count, destinationParent, destination));
    QCOMPARE(rowMovedSpy.size(), 0);
    QCOMPARE(rowAboutMovedSpy.size(), 0);
    delete baseModel;
}

void tst_QStringListModel::moveRows_data()
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

void tst_QStringListModel::moveRows()
{
    QFETCH(const int, startRow);
    QFETCH(const int, count);
    QFETCH(const int, destination);
    QFETCH(const QStringList, expected);
    QStringListModel baseModel(QStringList{"A", "B", "C", "D", "E", "F"});
    QSignalSpy rowMovedSpy(&baseModel, &QAbstractItemModel::rowsMoved);
    QSignalSpy rowAboutMovedSpy(&baseModel, &QAbstractItemModel::rowsAboutToBeMoved);
    QVERIFY(baseModel.moveRows(QModelIndex(), startRow, count, QModelIndex(), destination));
    QCOMPARE(baseModel.stringList(), expected);
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

void tst_QStringListModel::rowsAboutToBeRemoved_rowsRemoved_data()
{
    BOBUIest::addColumn<QStringList>("input");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<QStringList>("aboutto");
    BOBUIest::addColumn<QStringList>("res");

    QStringList strings0;   strings0    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto0;   aboutto0    << "Two" << "Three";
    QStringList res0;       res0        << "One" << "Four" << "Five";
    BOBUIest::newRow( "data0" )   << strings0 << 1 << 2 << aboutto0 << res0;

    QStringList strings1;   strings1    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto1;   aboutto1    << "One" << "Two";
    QStringList res1;       res1        << "Three" << "Four" << "Five";
    BOBUIest::newRow( "data1" )   << strings1 << 0 << 2 << aboutto1 << res1;

    QStringList strings2;   strings2    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto2;   aboutto2    << "Four" << "Five";
    QStringList res2;       res2        << "One" << "Two" << "Three";
    BOBUIest::newRow( "data2" )   << strings2 << 3 << 2 << aboutto2 << res2;

    QStringList strings3;   strings3    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto3;   aboutto3    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList res3;
    BOBUIest::newRow( "data3" )   << strings3 << 0 << 5 << aboutto3 << res3;

    /*
     * Keep this, template to add more data
    QStringList strings2;   strings2    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto2;   aboutto2    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList res2;       res2        << "One" << "Two" << "Three" << "Four" << "Five";
    BOBUIest::newRow( "data2" )   << strings2 << 0 << 5 << aboutto2 << res2;
*/

}

void tst_QStringListModel::rowsAboutToBeRemoved_rowsRemoved()
{
    QFETCH(QStringList, input);
    QFETCH(int, row);
    QFETCH(int, count);
    QFETCH(QStringList, aboutto);
    QFETCH(QStringList, res);

    QStringListModel model(input);
    QModelListener listener(&aboutto, &res, &model);
    connect(&model, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
            &listener, SLOT(rowsAboutToBeRemovedOrInserted(QModelIndex,int,int)));

    connect(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            &listener, SLOT(rowsRemovedOrInserted(QModelIndex,int,int)));

    model.removeRows(row, count);
    // At this point, control goes to our connected slots inn this order:
    // 1. rowsAboutToBeRemovedOrInserted
    // 2. rowsRemovedOrInserted
    // Control returns here
}

void tst_QStringListModel::rowsAboutToBeInserted_rowsInserted_data()
{
    BOBUIest::addColumn<QStringList>("input");
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<QStringList>("aboutto");
    BOBUIest::addColumn<QStringList>("res");

    QStringList strings0;   strings0    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto0;   aboutto0    << "Two" << "Three";
    QStringList res0;       res0        << "One" << "" << "" << "Two" << "Three" << "Four" << "Five";
    BOBUIest::newRow( "data0" )   << strings0 << 1 << 2 << aboutto0 << res0;

    QStringList strings1;   strings1    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto1;   aboutto1    << "One" << "Two";
    QStringList res1;       res1        << "" << "" << "One" << "Two" << "Three" << "Four" << "Five";
    BOBUIest::newRow( "data1" )   << strings1 << 0 << 2 << aboutto1 << res1;

    QStringList strings2;   strings2    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto2;   aboutto2    << "Four" << "Five";
    QStringList res2;       res2        << "One" << "Two" << "Three" << "" << "" << "Four" << "Five";
    BOBUIest::newRow( "data2" )   << strings2 << 3 << 2 << aboutto2 << res2;

    QStringList strings3;   strings3    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto3;   aboutto3    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList res3;       res3        << "" << "" << "" << "" << "" << "One" << "Two" << "Three" << "Four" << "Five";
    BOBUIest::newRow( "data3" )   << strings3 << 0 << 5 << aboutto3 << res3;

    /*
     * Keep this, template to add more data
    QStringList strings2;   strings2    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList aboutto2;   aboutto2    << "One" << "Two" << "Three" << "Four" << "Five";
    QStringList res2;       res2        << "One" << "Two" << "Three" << "Four" << "Five";
    BOBUIest::newRow( "data2" )   << strings2 << 0 << 5 << aboutto2 << res2;
*/

}

void tst_QStringListModel::rowsAboutToBeInserted_rowsInserted()
{
    QFETCH(QStringList, input);
    QFETCH(int, row);
    QFETCH(int, count);
    QFETCH(QStringList, aboutto);
    QFETCH(QStringList, res);

    QStringListModel model(input);
    QModelListener listener(&aboutto, &res, &model);
    connect(&model, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
            &listener, SLOT(rowsAboutToBeRemovedOrInserted(QModelIndex,int,int)));

    connect(&model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            &listener, SLOT(rowsRemovedOrInserted(QModelIndex,int,int)));

    model.insertRows(row, count);
    // At this point, control goes to our connected slots inn this order:
    // 1. rowsAboutToBeRemovedOrInserted
    // 2. rowsRemovedOrInserted
    // Control returns here
}

void tst_QStringListModel::setData_emits_both_roles_data()
{
    BOBUIest::addColumn<int>("row");
    BOBUIest::addColumn<QString>("data");
    BOBUIest::addColumn<int>("role");

#define ROW(row, string, role) \
    BOBUIest::newRow(#row " -> " string) << row << QString(string) << int(BobUI::role)
    ROW(0, "1", EditRole);
    ROW(1, "2", DisplayRole);
#undef ROW
}

template <class C>
C sorted(C c)
{
    std::sort(c.begin(), c.end());
    return c;
}

void tst_QStringListModel::setData_emits_both_roles()
{
    QFETCH(int, row);
    QFETCH(QString, data);
    QFETCH(int, role);

    QStringListModel model(QStringList() << "one" << "two");
    QList<int> expected;
    expected.reserve(2);
    expected.append(BobUI::DisplayRole);
    expected.append(BobUI::EditRole);

    QSignalSpy spy(&model, &QAbstractItemModel::dataChanged);
    QVERIFY(spy.isValid());
    model.setData(model.index(row, 0), data, role);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(sorted(spy.at(0).at(2).value<QList<int> >()),
             expected);
}

void tst_QStringListModel::itemData()
{
    QStringListModel testModel{ QStringList {
        QStringLiteral("One"),
        QStringLiteral("Two"),
        QStringLiteral("Three"),
        QStringLiteral("Four"),
        QStringLiteral("Five")
    }};
    QMap<int, QVariant> compareMap;
    QCOMPARE(testModel.itemData(QModelIndex()), compareMap);
    compareMap.insert(BobUI::DisplayRole, QStringLiteral("Two"));
    compareMap.insert(BobUI::EditRole, QStringLiteral("Two"));
    QCOMPARE(testModel.itemData(testModel.index(1, 0)), compareMap);
}

void tst_QStringListModel::setItemData()
{
    QStringListModel testModel{ QStringList {
        QStringLiteral("One"),
        QStringLiteral("Two"),
        QStringLiteral("Three"),
        QStringLiteral("Four"),
        QStringLiteral("Five")
    }};
    QSignalSpy dataChangedSpy(&testModel, &QAbstractItemModel::dataChanged);
    QModelIndex changeIndex = testModel.index(1, 0);
    const QList<int> changeRoles{BobUI::DisplayRole, BobUI::EditRole};
    const QString changedString("Changed");
    QMap<int, QVariant> newItemData{std::make_pair<int>(BobUI::DisplayRole, changedString)};
    // invalid index does nothing and returns false
    QVERIFY(!testModel.setItemData(QModelIndex(), newItemData));
    // valid data is set, return value is true and dataChanged is emitted once
    QVERIFY(testModel.setItemData(changeIndex, newItemData));
    QCOMPARE(changeIndex.data(BobUI::DisplayRole).toString(), changedString);
    QCOMPARE(changeIndex.data(BobUI::EditRole).toString(), changedString);
    QCOMPARE(dataChangedSpy.size(), 1);
    QVariantList dataChangedArguments = dataChangedSpy.takeFirst();
    QCOMPARE(dataChangedArguments.at(0).value<QModelIndex>(), changeIndex);
    QCOMPARE(dataChangedArguments.at(1).value<QModelIndex>(), changeIndex);
    QCOMPARE(dataChangedArguments.at(2).value<QList<int> >(), changeRoles);
    // Unsupported roles do nothing return false
    newItemData.clear();
    newItemData.insert(BobUI::UserRole, changedString);
    QVERIFY(!testModel.setItemData(changeIndex, newItemData));
    QCOMPARE(dataChangedSpy.size(), 0);
    // If some but not all the roles are supported it returns false and does nothing
    newItemData.insert(BobUI::EditRole, changedString);
    changeIndex = testModel.index(2, 0);
    QVERIFY(!testModel.setItemData(changeIndex, newItemData));
    QCOMPARE(changeIndex.data(BobUI::DisplayRole).toString(), QStringLiteral("Three"));
    QCOMPARE(changeIndex.data(BobUI::EditRole).toString(), QStringLiteral("Three"));
    QCOMPARE(dataChangedSpy.size(), 0);
    // BobUI::EditRole and BobUI::DisplayRole are both set, BobUI::EditRole takes precedence
    newItemData.clear();
    newItemData.insert(BobUI::EditRole, changedString);
    newItemData.insert(BobUI::DisplayRole, QStringLiteral("Ignored"));
    changeIndex = testModel.index(3, 0);
    QVERIFY(testModel.setItemData(changeIndex, newItemData));
    QCOMPARE(changeIndex.data(BobUI::DisplayRole).toString(), changedString);
    QCOMPARE(changeIndex.data(BobUI::EditRole).toString(), changedString);
    QCOMPARE(dataChangedSpy.size(), 1);
    dataChangedArguments = dataChangedSpy.takeFirst();
    QCOMPARE(dataChangedArguments.at(0).value<QModelIndex>(), changeIndex);
    QCOMPARE(dataChangedArguments.at(1).value<QModelIndex>(), changeIndex);
    QCOMPARE(dataChangedArguments.at(2).value<QList<int> >(), changeRoles);
}

void tst_QStringListModel::setData_emits_on_change_only()
{
    QStringListModel model(QStringList{QStringLiteral("one"), QStringLiteral("two")});
    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);
    QVERIFY(dataChangedSpy.isValid());
    const QModelIndex modelIdx = model.index(0, 0);
    const QString newStringData = QStringLiteral("test");
    QVERIFY(model.setData(modelIdx, newStringData));
    QCOMPARE(dataChangedSpy.size(), 1);
    const QList<QVariant> spyList = dataChangedSpy.takeFirst();
    QCOMPARE(spyList.at(0).value<QModelIndex>(), modelIdx);
    QCOMPARE(spyList.at(1).value<QModelIndex>(), modelIdx);
    const QList<int> expectedRoles{BobUI::DisplayRole, BobUI::EditRole};
    QCOMPARE(spyList.at(2).value<QList<int> >(), expectedRoles);
    QVERIFY(model.setData(modelIdx, newStringData));
    QVERIFY(dataChangedSpy.isEmpty());
}

void tst_QStringListModel::supportedDragDropActions()
{
    QStringListModel model;
    QCOMPARE(model.supportedDragActions(), BobUI::CopyAction | BobUI::MoveAction);
    QCOMPARE(model.supportedDropActions(), BobUI::CopyAction | BobUI::MoveAction);
}

void tst_QStringListModel::createPersistentOnLayoutAboutToBeChanged() // BOBUIBUG-93466
{
    QStringListModel model(QStringList{QStringLiteral("1"), QStringLiteral("2"), QStringLiteral("3")});
    QList<QPersistentModelIndex> idxList;
    QSignalSpy layoutAboutToBeChangedSpy(&model, &QAbstractItemModel::layoutAboutToBeChanged);
    QSignalSpy layoutChangedSpy(&model, &QAbstractItemModel::layoutChanged);
    connect(&model, &QAbstractItemModel::layoutAboutToBeChanged, this, [&idxList, &model](){
        idxList.clear();
        for (int row = 0; row < 3; ++row)
            idxList << QPersistentModelIndex(model.index(row, 0));
    });
    connect(&model, &QAbstractItemModel::layoutChanged, this, [&idxList](){
        QCOMPARE(idxList.size(), 3);
        QCOMPARE(idxList.at(0).row(), 1);
        QCOMPARE(idxList.at(0).column(), 0);
        QCOMPARE(idxList.at(0).data().toString(), QStringLiteral("1"));
        QCOMPARE(idxList.at(1).row(), 0);
        QCOMPARE(idxList.at(1).column(), 0);
        QCOMPARE(idxList.at(1).data().toString(), QStringLiteral("0"));
        QCOMPARE(idxList.at(2).row(), 2);
        QCOMPARE(idxList.at(2).column(), 0);
        QCOMPARE(idxList.at(2).data().toString(), QStringLiteral("3"));
    });
    model.setData(model.index(1, 0), QStringLiteral("0"));
    model.sort(0);
    QCOMPARE(layoutAboutToBeChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.size(), 1);
}

BOBUIEST_MAIN(tst_QStringListModel)
#include "tst_qstringlistmodel.moc"
