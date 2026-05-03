// Copyright (C) 2018 Luca Beldi <v.ronin@yahoo.it>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QSignalSpy>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QAbstractItemModelTester>
#include <random>

#include <bobuiransposeproxymodel.h>

class tst_BOBUIransposeProxyModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void index();
    void data();
    void setData_data();
    void setData();
    void parent();
    void mapToSource();
    void mapFromSource();
    void basicTest_data();
    void basicTest();
    void sort();
    void insertRowBase_data();
    void insertRowBase();
    void insertColumnBase_data();
    void insertColumnBase();
    void insertColumnProxy_data();
    void insertColumnProxy();
    void insertRowProxy_data();
    void insertRowProxy();
    void removeRowBase_data();
    void removeRowBase();
    void removeColumnBase_data();
    void removeColumnBase();
    void removeColumnProxy_data();
    void removeColumnProxy();
    void removeRowProxy_data();
    void removeRowProxy();
    void headerData();
    void setHeaderData();
    void span();
    void itemData();
    void setItemData();
    void moveRowsBase();
    void moveColumnsProxy();
    void sortPersistentIndex();
    void createPersistentOnLayoutAboutToBeChanged();
private:
    void testTransposed(
        const QAbstractItemModel *const baseModel,
        const QAbstractItemModel *const transposed,
        const QModelIndex &baseParent = QModelIndex(),
        const QModelIndex &transposedParent = QModelIndex()
    );
    QAbstractItemModel *createListModel(QObject *parent);
    QAbstractItemModel *createTableModel(QObject *parent);
    QAbstractItemModel *createTreeModel(QObject *parent);
};

QAbstractItemModel *tst_BOBUIransposeProxyModel::createListModel(QObject *parent)
{
    QStringList sequence;
    sequence.reserve(10);
    for (int i = 0; i < 10; ++i)
        sequence.append(QString::number(i));
    return new QStringListModel(sequence, parent);
}

QAbstractItemModel *tst_BOBUIransposeProxyModel::createTableModel(QObject *parent)
{
    QAbstractItemModel *model = new QStandardItemModel(parent);
    model->insertRows(0, 5);
    model->insertColumns(0, 4);
    for (int i = 0; i < model->rowCount(); ++i) {
        for (int j = 0; j < model->columnCount(); ++j) {
            model->setData(model->index(i, j), QStringLiteral("%1,%2").arg(i).arg(j), BobUI::EditRole);
            model->setData(model->index(i, j), i, BobUI::UserRole);
            model->setData(model->index(i, j), j, BobUI::UserRole + 1);
        }
    }
    return model;
}

QAbstractItemModel *tst_BOBUIransposeProxyModel::createTreeModel(QObject *parent)
{
    QAbstractItemModel *model = new QStandardItemModel(parent);
    model->insertRows(0, 5);
    model->insertColumns(0, 4);
    for (int i = 0; i < model->rowCount(); ++i) {
        for (int j = 0; j < model->columnCount(); ++j) {
            const QModelIndex parIdx = model->index(i, j);
            model->setData(parIdx, QStringLiteral("%1,%2").arg(i).arg(j), BobUI::EditRole);
            model->setData(parIdx, i, BobUI::UserRole);
            model->setData(parIdx, j, BobUI::UserRole + 1);
            model->insertRows(0, 3, parIdx);
            model->insertColumns(0, 2, parIdx);
            for (int h = 0; h < model->rowCount(parIdx); ++h) {
                for (int k = 0; k < model->columnCount(parIdx); ++k) {
                    const QModelIndex childIdx = model->index(h, k, parIdx);
                    model->setData(childIdx, QStringLiteral("%1,%2,%3,%4").arg(i).arg(j).arg(h).arg(k), BobUI::EditRole);
                    model->setData(childIdx, i, BobUI::UserRole);
                    model->setData(childIdx, j, BobUI::UserRole + 1);
                    model->setData(childIdx, h, BobUI::UserRole + 2);
                    model->setData(childIdx, k, BobUI::UserRole + 3);
                }
            }
        }
    }
    return model;
}

void tst_BOBUIransposeProxyModel::testTransposed(
    const QAbstractItemModel *const baseModel,
    const QAbstractItemModel *const transposed,
    const QModelIndex &baseParent,
    const QModelIndex &transposedParent
)
{
    QCOMPARE(transposed->hasChildren(transposedParent), baseModel->hasChildren(baseParent));
    QCOMPARE(transposed->columnCount(transposedParent), baseModel->rowCount(baseParent));
    QCOMPARE(transposed->rowCount(transposedParent), baseModel->columnCount(baseParent));
    for (int i = 0, maxRow = baseModel->rowCount(baseParent); i < maxRow; ++i) {
        for (int j = 0, maxCol = baseModel->columnCount(baseParent); j < maxCol; ++j) {
            const QModelIndex baseIdx = baseModel->index(i, j, baseParent);
            const QModelIndex transIdx = transposed->index(j, i, transposedParent);
            QCOMPARE(transIdx.data(), baseIdx.data());
            QCOMPARE(transIdx.data(BobUI::UserRole), baseIdx.data(BobUI::UserRole));
            QCOMPARE(transIdx.data(BobUI::UserRole + 1), baseIdx.data(BobUI::UserRole + 1));
            QCOMPARE(transIdx.data(BobUI::UserRole + 2), baseIdx.data(BobUI::UserRole + 2));
            QCOMPARE(transIdx.data(BobUI::UserRole + 3), baseIdx.data(BobUI::UserRole + 3));
            if (baseModel->hasChildren(baseIdx)) {
                testTransposed(baseModel, transposed, baseIdx, transIdx);
            }
        }
    }
}

void tst_BOBUIransposeProxyModel::initTestCase()
{
    qRegisterMetaType<QList<QPersistentModelIndex> >();
    qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>();
}

void tst_BOBUIransposeProxyModel::index()
{
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    QVERIFY(!proxy.index(0, -1).isValid());
    QVERIFY(!proxy.index(0, -1).isValid());
    QVERIFY(!proxy.index(-1, -1).isValid());
    QVERIFY(!proxy.index(0, proxy.columnCount()).isValid());
    QVERIFY(!proxy.index(proxy.rowCount(), 0).isValid());
    QVERIFY(!proxy.index(proxy.rowCount(), proxy.columnCount()).isValid());
    QModelIndex tempIdx = proxy.index(0, 1);
    QVERIFY(tempIdx.isValid());
    QCOMPARE(tempIdx.row(), 0);
    QCOMPARE(tempIdx.column(), 1);
    tempIdx = proxy.index(0, 1, tempIdx);
    QVERIFY(tempIdx.isValid());
    QCOMPARE(tempIdx.row(), 0);
    QCOMPARE(tempIdx.column(), 1);
    delete model;
}

void tst_BOBUIransposeProxyModel::data()
{
    QStringListModel model{QStringList{"A", "B"}};
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.index(0, 1).data().toString(), QStringLiteral("B"));
}

void tst_BOBUIransposeProxyModel::parent()
{
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    const QModelIndex parentIdx = proxy.index(0, 0);
    const QModelIndex childIdx = proxy.index(0, 0, parentIdx);
    QVERIFY(parentIdx.isValid());
    QVERIFY(childIdx.isValid());
    QCOMPARE(childIdx.parent(), parentIdx);
    delete model;
}

void tst_BOBUIransposeProxyModel::mapToSource()
{
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    QVERIFY(!proxy.mapToSource(QModelIndex()).isValid());
    QCOMPARE(proxy.mapToSource(proxy.index(0, 0)), model->index(0, 0));
    QCOMPARE(proxy.mapToSource(proxy.index(1, 0)), model->index(0, 1));
    QCOMPARE(proxy.mapToSource(proxy.index(0, 1)), model->index(1, 0));
    const QModelIndex proxyParent = proxy.index(1, 0);
    const QModelIndex sourceParent = model->index(0, 1);
    QCOMPARE(proxy.mapToSource(proxy.index(0, 0, proxyParent)), model->index(0, 0, sourceParent));
    QCOMPARE(proxy.mapToSource(proxy.index(1, 0, proxyParent)), model->index(0, 1, sourceParent));
    QCOMPARE(proxy.mapToSource(proxy.index(0, 1, proxyParent)), model->index(1, 0, sourceParent));
    delete model;
}

void tst_BOBUIransposeProxyModel::mapFromSource()
{
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    QVERIFY(!proxy.mapFromSource(QModelIndex()).isValid());
    QCOMPARE(proxy.mapFromSource(model->index(0, 0)), proxy.index(0, 0));
    QCOMPARE(proxy.mapFromSource(model->index(0, 1)), proxy.index(1, 0));
    QCOMPARE(proxy.mapFromSource(model->index(1, 0)), proxy.index(0, 1));
    const QModelIndex proxyParent = proxy.index(1, 0);
    const QModelIndex sourceParent = model->index(0, 1);
    QCOMPARE(proxy.mapToSource(proxy.index(0, 0, proxyParent)), model->index(0, 0, sourceParent));
    QCOMPARE(proxy.mapFromSource(model->index(1, 0, sourceParent)), proxy.index(0, 1, proxyParent));
    QCOMPARE(proxy.mapFromSource(model->index(0, 1, sourceParent)), proxy.index(1, 0, proxyParent));
    delete model;
}

void tst_BOBUIransposeProxyModel::basicTest_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::newRow("List") << createListModel(this);
    BOBUIest::newRow("Table") << createTableModel(this);
    BOBUIest::newRow("Tree") << createTreeModel(this);
}

void tst_BOBUIransposeProxyModel::basicTest()
{
    QFETCH(QAbstractItemModel *, model);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    testTransposed(model, &proxy);
    delete model;
}

void tst_BOBUIransposeProxyModel::sort()
{
    QStringList sequence;
    sequence.reserve(100);
    for (int i = 0; i < 100; ++i)
        sequence.append(QStringLiteral("%1").arg(i, 3, 10, QLatin1Char('0')));
    std::shuffle(sequence.begin(), sequence.end(), std::mt19937(88));
    const QString firstItemBeforeSort = sequence.first();
    QStringListModel baseModel(sequence);
    BOBUIransposeProxyModel proxyModel;
    new QAbstractItemModelTester(&proxyModel, &proxyModel);
    proxyModel.setSourceModel(&baseModel);
    QSignalSpy layoutChangedSpy(&proxyModel, &QAbstractItemModel::layoutChanged);
    QVERIFY(layoutChangedSpy.isValid());
    QSignalSpy layoutAboutToBeChangedSpy(&proxyModel, &QAbstractItemModel::layoutAboutToBeChanged);
    QVERIFY(layoutAboutToBeChangedSpy.isValid());
    QPersistentModelIndex firstIndexBeforeSort = proxyModel.index(0, 0);
    baseModel.sort(0, BobUI::AscendingOrder);
    QCOMPARE(layoutChangedSpy.size(), 1);
    QCOMPARE(layoutAboutToBeChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.takeFirst().at(1).toInt(), int(QAbstractItemModel::HorizontalSortHint));
    QCOMPARE(firstIndexBeforeSort.data().toString(), firstItemBeforeSort);
    for (int i = 0; i < 100; ++i)
        QCOMPARE(proxyModel.index(0, i).data().toInt(), i);
}

void tst_BOBUIransposeProxyModel::removeColumnBase_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<QModelIndex>("parent");
    BOBUIest::newRow("Table") << createTableModel(this) << QModelIndex();
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << QModelIndex();
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIest::newRow("Tree_Child_Item") << model << model->index(0, 0);
}

void tst_BOBUIransposeProxyModel::removeColumnBase()
{
    QFETCH(QAbstractItemModel * const, model);
    QFETCH(const QModelIndex, parent);
    BOBUIransposeProxyModel proxy;
    QSignalSpy rowRemoveSpy(&proxy, &QAbstractItemModel::rowsRemoved);
    QVERIFY(rowRemoveSpy.isValid());
    QSignalSpy rowAboutToBeRemoveSpy(&proxy, &QAbstractItemModel::rowsAboutToBeRemoved);
    QVERIFY(rowAboutToBeRemoveSpy.isValid());
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    const int oldRowCount = proxy.rowCount(proxy.mapFromSource(parent));
    const QVariant expectedNewVal = model->index(0, 2, parent).data();
    QVERIFY(model->removeColumn(1, parent));
    QCOMPARE(proxy.rowCount(proxy.mapFromSource(parent)), oldRowCount - 1);
    QCOMPARE(proxy.index(1, 0, proxy.mapFromSource(parent)).data(), expectedNewVal);
    QCOMPARE(rowRemoveSpy.size(), 1);
    QCOMPARE(rowAboutToBeRemoveSpy.size(), 1);
    for (const auto &spyArgs : {rowRemoveSpy.takeFirst(),
        rowAboutToBeRemoveSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxy.mapFromSource(parent));
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::sortPersistentIndex()
{
    QStringListModel model(QStringList{QStringLiteral("Alice"), QStringLiteral("Charlie"), QStringLiteral("Bob")});
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    QPersistentModelIndex aliceIdx = proxy.index(0, 0);
    QPersistentModelIndex bobIdx = proxy.index(0, 2);
    QPersistentModelIndex charlieIdx = proxy.index(0, 1);
    connect(&proxy, &QAbstractItemModel::layoutAboutToBeChanged, this, [&aliceIdx, &bobIdx, &charlieIdx](){
        QCOMPARE(aliceIdx.row(), 0);
        QCOMPARE(aliceIdx.column(), 0);
        QCOMPARE(aliceIdx.data().toString(), QStringLiteral("Alice"));
        QCOMPARE(bobIdx.row(), 0);
        QCOMPARE(bobIdx.column(), 2);
        QCOMPARE(bobIdx.data().toString(), QStringLiteral("Bob"));
        QCOMPARE(charlieIdx.row(), 0);
        QCOMPARE(charlieIdx.column(), 1);
        QCOMPARE(charlieIdx.data().toString(), QStringLiteral("Charlie"));
    });
    connect(&proxy, &QAbstractItemModel::layoutChanged, this, [&aliceIdx, &bobIdx, &charlieIdx](){
        QCOMPARE(aliceIdx.row(), 0);
        QCOMPARE(aliceIdx.column(), 0);
        QCOMPARE(aliceIdx.data().toString(), QStringLiteral("Alice"));
        QCOMPARE(bobIdx.row(), 0);
        QCOMPARE(bobIdx.column(), 1);
        QCOMPARE(bobIdx.data().toString(), QStringLiteral("Bob"));
        QCOMPARE(charlieIdx.row(), 0);
        QCOMPARE(charlieIdx.column(), 2);
        QCOMPARE(charlieIdx.data().toString(), QStringLiteral("Charlie"));
    });
    model.sort(0);
    QCOMPARE(aliceIdx.row(), 0);
    QCOMPARE(aliceIdx.column(), 0);
    QCOMPARE(aliceIdx.data().toString(), QStringLiteral("Alice"));
    QCOMPARE(bobIdx.row(), 0);
    QCOMPARE(bobIdx.column(), 1);
    QCOMPARE(bobIdx.data().toString(), QStringLiteral("Bob"));
    QCOMPARE(charlieIdx.row(), 0);
    QCOMPARE(charlieIdx.column(), 2);
    QCOMPARE(charlieIdx.data().toString(), QStringLiteral("Charlie"));
}

void tst_BOBUIransposeProxyModel::createPersistentOnLayoutAboutToBeChanged() // BOBUIBUG-93466
{
    QStandardItemModel model(3, 1);
    for (int row = 0; row < 3; ++row)
        model.setData(model.index(row, 0), row, BobUI::UserRole);
    model.setSortRole(BobUI::UserRole);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    QList<QPersistentModelIndex> idxList;
    QSignalSpy layoutAboutToBeChangedSpy(&proxy, &QAbstractItemModel::layoutAboutToBeChanged);
    QSignalSpy layoutChangedSpy(&proxy, &QAbstractItemModel::layoutChanged);
    connect(&proxy, &QAbstractItemModel::layoutAboutToBeChanged, this, [&idxList, &proxy](){
        idxList.clear();
        for (int row = 0; row < 3; ++row)
            idxList << QPersistentModelIndex(proxy.index(0, row));
    });
    connect(&proxy, &QAbstractItemModel::layoutChanged, this, [&idxList](){
        QCOMPARE(idxList.size(), 3);
        QCOMPARE(idxList.at(0).row(), 0);
        QCOMPARE(idxList.at(0).column(), 1);
        QCOMPARE(idxList.at(0).data(BobUI::UserRole).toInt(), 0);
        QCOMPARE(idxList.at(1).row(), 0);
        QCOMPARE(idxList.at(1).column(), 0);
        QCOMPARE(idxList.at(1).data(BobUI::UserRole).toInt(), -1);
        QCOMPARE(idxList.at(2).row(), 0);
        QCOMPARE(idxList.at(2).column(), 2);
        QCOMPARE(idxList.at(2).data(BobUI::UserRole).toInt(), 2);
    });
    model.setData(model.index(1, 0), -1, BobUI::UserRole);
    model.sort(0);
    QCOMPARE(layoutAboutToBeChangedSpy.size(), 1);
    QCOMPARE(layoutChangedSpy.size(), 1);
}

void tst_BOBUIransposeProxyModel::insertColumnBase_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<QModelIndex>("parent");
    BOBUIest::newRow("Table") << createTableModel(this) << QModelIndex();
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << QModelIndex();
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIest::newRow("Tree_Child_Item") << model << model->index(0, 0);
}

void tst_BOBUIransposeProxyModel::insertColumnBase()
{
    QFETCH(QAbstractItemModel * const, model);
    QFETCH(const QModelIndex, parent);
    BOBUIransposeProxyModel proxy;
    QSignalSpy rowInsertSpy(&proxy, &QAbstractItemModel::rowsInserted);
    QVERIFY(rowInsertSpy.isValid());
    QSignalSpy rowAboutToBeInsertSpy(&proxy, &QAbstractItemModel::rowsAboutToBeInserted);
    QVERIFY(rowAboutToBeInsertSpy.isValid());
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    const int oldRowCount = proxy.rowCount(proxy.mapFromSource(parent));
    QVERIFY(model->insertColumn(1, parent));
    QCOMPARE(proxy.rowCount(proxy.mapFromSource(parent)), oldRowCount + 1);
    QVERIFY(!proxy.index(1, 0, proxy.mapFromSource(parent)).data().isValid());
    QCOMPARE(rowInsertSpy.size(), 1);
    QCOMPARE(rowAboutToBeInsertSpy.size(), 1);
    for (const auto &spyArgs : {rowInsertSpy.takeFirst(),
        rowAboutToBeInsertSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxy.mapFromSource(parent));
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::removeRowBase_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<QModelIndex>("parent");
    BOBUIest::newRow("List") << createListModel(this) << QModelIndex();
    BOBUIest::newRow("Table") << createTableModel(this) << QModelIndex();
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << QModelIndex();
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIest::newRow("Tree_Child_Item") << model << model->index(0, 0);
}

void tst_BOBUIransposeProxyModel::removeRowBase()
{
    QFETCH(QAbstractItemModel * const, model);
    QFETCH(const QModelIndex, parent);
    BOBUIransposeProxyModel proxy;
    QSignalSpy columnsRemoveSpy(&proxy, &QAbstractItemModel::columnsRemoved);
    QVERIFY(columnsRemoveSpy.isValid());
    QSignalSpy columnsAboutToBeRemoveSpy(&proxy, &QAbstractItemModel::columnsAboutToBeRemoved);
    QVERIFY(columnsAboutToBeRemoveSpy.isValid());
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    const int oldColCount = proxy.columnCount(proxy.mapFromSource(parent));
    const QVariant expectedNewVal = model->index(2, 0, parent).data();
    QVERIFY(model->removeRow(1, parent));
    QCOMPARE(proxy.columnCount(proxy.mapFromSource(parent)), oldColCount - 1);
    QCOMPARE(proxy.index(0, 1, proxy.mapFromSource(parent)).data(), expectedNewVal);
    QCOMPARE(columnsRemoveSpy.size(), 1);
    QCOMPARE(columnsAboutToBeRemoveSpy.size(), 1);
    for (const auto &spyArgs : {columnsRemoveSpy.takeFirst(),
        columnsAboutToBeRemoveSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxy.mapFromSource(parent));
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::insertRowBase_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<QModelIndex>("parent");
    BOBUIest::newRow("List") << createListModel(this) << QModelIndex();
    BOBUIest::newRow("Table") << createTableModel(this) << QModelIndex();
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << QModelIndex();
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIest::newRow("Tree_Child_Item") << model << model->index(0, 0);
}

void tst_BOBUIransposeProxyModel::insertRowBase()
{
    QFETCH(QAbstractItemModel * const, model);
    QFETCH(const QModelIndex, parent);
    BOBUIransposeProxyModel proxy;
    QSignalSpy columnsInsertSpy(&proxy, &QAbstractItemModel::columnsInserted);
    QVERIFY(columnsInsertSpy.isValid());
    QSignalSpy columnsAboutToBeInsertSpy(&proxy, &QAbstractItemModel::columnsAboutToBeInserted);
    QVERIFY(columnsAboutToBeInsertSpy.isValid());
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    const int oldColCount = proxy.columnCount(proxy.mapFromSource(parent));
    QVERIFY(model->insertRow(1, parent));
    QCOMPARE(proxy.columnCount(proxy.mapFromSource(parent)), oldColCount + 1);
    QVariant result = proxy.index(0, 1, proxy.mapFromSource(parent)).data();
    QVERIFY(result.isNull() || (result.metaType().id() == QMetaType::QString && result.toString().isNull()));
    QCOMPARE(columnsInsertSpy.size(), 1);
    QCOMPARE(columnsAboutToBeInsertSpy.size(), 1);
    for (const auto &spyArgs : {columnsInsertSpy.takeFirst(),
        columnsAboutToBeInsertSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxy.mapFromSource(parent));
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::removeColumnProxy_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<bool>("rootItem");
    BOBUIest::newRow("List") << createListModel(this) << true;
    BOBUIest::newRow("Table") << createTableModel(this) << true;
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << true;
    BOBUIest::newRow("Tree_Child_Item") << createTreeModel(this) << false;
}

void tst_BOBUIransposeProxyModel::removeColumnProxy()
{
    QFETCH(QAbstractItemModel *, model);
    QFETCH(bool, rootItem);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    QSignalSpy columnsRemoveSpy(&proxy, &QAbstractItemModel::columnsRemoved);
    QVERIFY(columnsRemoveSpy.isValid());
    QSignalSpy columnsAboutToBeRemoveSpy(&proxy, &QAbstractItemModel::columnsAboutToBeRemoved);
    QVERIFY(columnsAboutToBeRemoveSpy.isValid());
    QSignalSpy rowsRemoveSpy(model, &QAbstractItemModel::rowsRemoved);
    QVERIFY(rowsRemoveSpy.isValid());
    QSignalSpy rowsAboutToBeRemoveSpy(model, &QAbstractItemModel::rowsAboutToBeRemoved);
    QVERIFY(rowsAboutToBeRemoveSpy.isValid());
    proxy.setSourceModel(model);
    const QModelIndex proxyParent = rootItem ? QModelIndex() : proxy.index(0, 1);
    const QModelIndex sourceParent = proxy.mapToSource(proxyParent);
    const int oldColCount = proxy.columnCount(proxyParent);
    const int oldRowCount = model->rowCount(sourceParent);
    const QVariant expectedNewVal = proxy.index(0, 2, proxyParent).data();
    QVERIFY(proxy.removeColumn(1, proxyParent));
    QCOMPARE(proxy.columnCount(proxyParent), oldColCount - 1);
    QCOMPARE(model->rowCount(sourceParent), oldRowCount - 1);
    QCOMPARE(proxy.index(0, 1, proxyParent).data(), expectedNewVal);
    QCOMPARE(model->index(1, 0, sourceParent).data(), expectedNewVal);
    QCOMPARE(columnsRemoveSpy.size(), 1);
    QCOMPARE(columnsAboutToBeRemoveSpy.size(), 1);
    QCOMPARE(rowsRemoveSpy.size(), 1);
    QCOMPARE(rowsAboutToBeRemoveSpy.size(), 1);
    for (const auto &spyArgs : {columnsRemoveSpy.takeFirst(),
        columnsAboutToBeRemoveSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxyParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    for (const auto &spyArgs : {rowsRemoveSpy.takeFirst(),
        rowsAboutToBeRemoveSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), sourceParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::insertColumnProxy_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<bool>("rootItem");
    BOBUIest::newRow("List") << createListModel(this) << true;
    BOBUIest::newRow("Table") << createTableModel(this) << true;
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << true;
    BOBUIest::newRow("Tree_Child_Item") << createTreeModel(this) << false;
}

void tst_BOBUIransposeProxyModel::insertColumnProxy()
{
    QFETCH(QAbstractItemModel *, model);
    QFETCH(bool, rootItem);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    QSignalSpy columnsInsertSpy(&proxy, &QAbstractItemModel::columnsInserted);
    QVERIFY(columnsInsertSpy.isValid());
    QSignalSpy columnsAboutToBeInsertSpy(&proxy, &QAbstractItemModel::columnsAboutToBeInserted);
    QVERIFY(columnsAboutToBeInsertSpy.isValid());
    QSignalSpy rowsInsertSpy(model, &QAbstractItemModel::rowsInserted);
    QVERIFY(rowsInsertSpy.isValid());
    QSignalSpy rowsAboutToBeInsertSpy(model, &QAbstractItemModel::rowsAboutToBeInserted);
    QVERIFY(rowsAboutToBeInsertSpy.isValid());
    proxy.setSourceModel(model);
    const QModelIndex proxyParent = rootItem ? QModelIndex() : proxy.index(0, 1);
    const QModelIndex sourceParent = proxy.mapToSource(proxyParent);
    const int oldColCount = proxy.columnCount(proxyParent);
    const int oldRowCount = model->rowCount(sourceParent);
    QVERIFY(proxy.insertColumn(1, proxyParent));
    QCOMPARE(proxy.columnCount(proxyParent), oldColCount + 1);
    QCOMPARE(model->rowCount(sourceParent), oldRowCount + 1);
    QVariant result = proxy.index(0, 1, proxyParent).data();
    QVERIFY(result.isNull() || (result.metaType().id() == QMetaType::QString && result.toString().isNull()));
    result = model->index(1, 0, sourceParent).data();
    QVERIFY(result.isNull() || (result.metaType().id() == QMetaType::QString && result.toString().isNull()));
    QCOMPARE(columnsInsertSpy.size(), 1);
    QCOMPARE(columnsAboutToBeInsertSpy.size(), 1);
    QCOMPARE(rowsInsertSpy.size(), 1);
    QCOMPARE(rowsAboutToBeInsertSpy.size(), 1);
    for (const auto &spyArgs : {columnsInsertSpy.takeFirst(),
        columnsAboutToBeInsertSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxyParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    for (const auto &spyArgs : {rowsInsertSpy.takeFirst(),
        rowsAboutToBeInsertSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), sourceParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::removeRowProxy_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<bool>("rootItem");
    BOBUIest::newRow("Table") << createTableModel(this) << true;
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << true;
    BOBUIest::newRow("Tree_Child_Item") << createTreeModel(this) << false;
}

void tst_BOBUIransposeProxyModel::removeRowProxy()
{
    QFETCH(QAbstractItemModel *, model);
    QFETCH(bool, rootItem);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    QSignalSpy rowsRemoveSpy(&proxy, &QAbstractItemModel::rowsRemoved);
    QVERIFY(rowsRemoveSpy.isValid());
    QSignalSpy rowsAboutToBeRemoveSpy(&proxy, &QAbstractItemModel::rowsAboutToBeRemoved);
    QVERIFY(rowsAboutToBeRemoveSpy.isValid());
    QSignalSpy columnsRemoveSpy(model, &QAbstractItemModel::columnsRemoved);
    QVERIFY(columnsRemoveSpy.isValid());
    QSignalSpy columnsAboutToBeRemoveSpy(model, &QAbstractItemModel::columnsAboutToBeRemoved);
    QVERIFY(columnsAboutToBeRemoveSpy.isValid());
    proxy.setSourceModel(model);
    const QModelIndex proxyParent = rootItem ? QModelIndex() : proxy.index(0, 1);
    const QModelIndex sourceParent = proxy.mapToSource(proxyParent);
    const int oldRowCount = proxy.rowCount(proxyParent);
    const int oldColCount = model->columnCount(sourceParent);
    const QVariant expectedNewVal = proxy.index(2, 0, proxyParent).data();
    QVERIFY(proxy.removeRow(1, proxyParent));
    QCOMPARE(proxy.rowCount(proxyParent), oldRowCount - 1);
    QCOMPARE(model->columnCount(sourceParent), oldColCount - 1);
    QCOMPARE(proxy.index(1, 0, proxyParent).data(), expectedNewVal);
    QCOMPARE(model->index(0, 1, sourceParent).data(), expectedNewVal);
    QCOMPARE(columnsRemoveSpy.size(), 1);
    QCOMPARE(columnsAboutToBeRemoveSpy.size(), 1);
    QCOMPARE(rowsRemoveSpy.size(), 1);
    QCOMPARE(rowsAboutToBeRemoveSpy.size(), 1);
    for (const auto &spyArgs : {columnsRemoveSpy.takeFirst(),
        columnsAboutToBeRemoveSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), sourceParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    for (const auto &spyArgs : {rowsRemoveSpy.takeFirst(),
        rowsAboutToBeRemoveSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxyParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::insertRowProxy_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<bool>("rootItem");
    BOBUIest::newRow("Table") << createTableModel(this) << true;
    BOBUIest::newRow("Tree_Root_Item") << createTreeModel(this) << true;
    BOBUIest::newRow("Tree_Child_Item") << createTreeModel(this) << false;
}

void tst_BOBUIransposeProxyModel::insertRowProxy()
{
    QFETCH(QAbstractItemModel *, model);
    QFETCH(bool, rootItem);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    QSignalSpy rowsInsertSpy(&proxy, &QAbstractItemModel::rowsInserted);
    QVERIFY(rowsInsertSpy.isValid());
    QSignalSpy rowsAboutToBeInsertSpy(&proxy, &QAbstractItemModel::rowsAboutToBeInserted);
    QVERIFY(rowsAboutToBeInsertSpy.isValid());
    QSignalSpy columnsInsertSpy(model, &QAbstractItemModel::columnsInserted);
    QVERIFY(columnsInsertSpy.isValid());
    QSignalSpy columnsAboutToBeInsertSpy(model, &QAbstractItemModel::columnsAboutToBeInserted);
    QVERIFY(columnsAboutToBeInsertSpy.isValid());
    proxy.setSourceModel(model);
    const QModelIndex proxyParent = rootItem ? QModelIndex() : proxy.index(0, 1);
    const QModelIndex sourceParent = proxy.mapToSource(proxyParent);
    const int oldRowCount = proxy.rowCount(proxyParent);
    const int oldColCount = model->columnCount(sourceParent);
    QVERIFY(proxy.insertRow(1, proxyParent));
    QCOMPARE(proxy.rowCount(proxyParent), oldRowCount + 1);
    QCOMPARE(model->columnCount(sourceParent), oldColCount + 1);
    QVERIFY(proxy.index(1, 0, proxyParent).data().isNull());
    QVERIFY(model->index(0, 1, sourceParent).data().isNull());
    QCOMPARE(columnsInsertSpy.size(), 1);
    QCOMPARE(columnsAboutToBeInsertSpy.size(), 1);
    QCOMPARE(rowsInsertSpy.size(), 1);
    QCOMPARE(rowsAboutToBeInsertSpy.size(), 1);
    for (const auto &spyArgs : {columnsInsertSpy.takeFirst(),
        columnsAboutToBeInsertSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), sourceParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    for (const auto &spyArgs : {rowsInsertSpy.takeFirst(),
        rowsAboutToBeInsertSpy.takeFirst()}) {
        QCOMPARE(spyArgs.at(0).value<QModelIndex>(), proxyParent);
        QCOMPARE(spyArgs.at(1).toInt(), 1);
        QCOMPARE(spyArgs.at(2).toInt(), 1);
    }
    delete model;
}

void tst_BOBUIransposeProxyModel::headerData()
{
    QStandardItemModel model;
    model.insertRows(0, 3);
    model.insertColumns(0, 5);
    for (int i = 0; i < model.rowCount(); ++i)
        model.setHeaderData(i, BobUI::Horizontal, QChar('A' + i));
    for (int i = 1; i <= model.columnCount(); ++i)
        model.setHeaderData(i, BobUI::Vertical, i);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    for (int i = 0; i < model.rowCount(); ++i)
        QCOMPARE(model.headerData(i, BobUI::Horizontal), proxy.headerData(i, BobUI::Vertical));
    for (int i = 0; i < model.columnCount(); ++i)
        QCOMPARE(model.headerData(i, BobUI::Vertical), proxy.headerData(i, BobUI::Horizontal));
}

void tst_BOBUIransposeProxyModel::setHeaderData()
{
    QStandardItemModel model;
    model.insertRows(0, 3);
    model.insertColumns(0, 5);
    for (int i = 0; i < model.rowCount(); ++i)
        model.setHeaderData(i, BobUI::Horizontal, QChar('A' + i));
    for (int i = 1; i <= model.columnCount(); ++i)
        model.setHeaderData(i, BobUI::Vertical, i);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    QVERIFY(proxy.setHeaderData(1, BobUI::Horizontal, 99));
    QCOMPARE(model.headerData(1, BobUI::Vertical).toInt(), 99);
    QVERIFY(proxy.setHeaderData(1, BobUI::Vertical, QChar('Z')));
    QCOMPARE(model.headerData(1, BobUI::Horizontal).toChar(), QChar('Z'));
}

void tst_BOBUIransposeProxyModel::span()
{
    class SpanModel : public QStandardItemModel
    {
        Q_DISABLE_COPY(SpanModel)
    public:
        SpanModel(int rows, int columns, QObject *parent = nullptr)
            : QStandardItemModel(rows, columns, parent)
        {}
        QSize span(const QModelIndex &index) const override
        {
            Q_UNUSED(index);
            return QSize(2, 1);
        }
    };
    SpanModel model(3, 5);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.span(proxy.index(0, 0)), QSize(1, 2));
}

void tst_BOBUIransposeProxyModel::itemData()
{
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    QMap<int, QVariant> itmData = proxy.itemData(proxy.index(0, 1));
    QCOMPARE(itmData.value(BobUI::DisplayRole).toString(), QStringLiteral("1,0"));
    QCOMPARE(itmData.value(BobUI::UserRole).toInt(), 1);
    QCOMPARE(itmData.value(BobUI::UserRole + 1).toInt(), 0);
    itmData = proxy.itemData(proxy.index(1, 2, proxy.index(0, 1)));
    QCOMPARE(itmData.value(BobUI::DisplayRole).toString(), QStringLiteral("1,0,2,1"));
    QCOMPARE(itmData.value(BobUI::UserRole).toInt(), 1);
    QCOMPARE(itmData.value(BobUI::UserRole + 1).toInt(), 0);
    QCOMPARE(itmData.value(BobUI::UserRole + 2).toInt(), 2);
    QCOMPARE(itmData.value(BobUI::UserRole + 3).toInt(), 1);
    QVERIFY(proxy.itemData(QModelIndex()).isEmpty());
    delete model;
}

void tst_BOBUIransposeProxyModel::setItemData()
{
    QAbstractItemModel *model = createTreeModel(this);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    QSignalSpy sourceDataChangeSpy(model, &QAbstractItemModel::dataChanged);
    QVERIFY(sourceDataChangeSpy.isValid());
    QSignalSpy proxyDataChangeSpy(&proxy, &QAbstractItemModel::dataChanged);
    QVERIFY(proxyDataChangeSpy.isValid());
    const QMap<int, QVariant> itmData = {
        std::make_pair<int, QVariant>(BobUI::DisplayRole, QStringLiteral("Test")),
        std::make_pair<int, QVariant>(BobUI::UserRole, 88),
        std::make_pair<int, QVariant>(BobUI::UserRole + 1, 99),
    };
    QModelIndex idx = proxy.index(0, 1);
    QVERIFY(proxy.setItemData(idx, itmData));
    QCOMPARE(idx.data(BobUI::DisplayRole).toString(), QStringLiteral("Test"));
    QCOMPARE(idx.data(BobUI::UserRole).toInt(), 88);
    QCOMPARE(idx.data(BobUI::UserRole + 1).toInt(), 99);
    QCOMPARE(sourceDataChangeSpy.size(), 1);
    QCOMPARE(proxyDataChangeSpy.size(), 1);
    auto signalData = proxyDataChangeSpy.takeFirst();
    QCOMPARE(signalData.at(0).value<QModelIndex>(), idx);
    QCOMPARE(signalData.at(1).value<QModelIndex>(), idx);
    const QList<int> expectedRoles{BobUI::DisplayRole, BobUI::UserRole, BobUI::EditRole, BobUI::UserRole + 1};
    QList<int> receivedRoles = signalData.at(2).value<QList<int> >();
    QCOMPARE(receivedRoles.size(), expectedRoles.size());
    for (int role : expectedRoles)
        QVERIFY(receivedRoles.contains(role));
    signalData = sourceDataChangeSpy.takeFirst();
    QCOMPARE(signalData.at(0).value<QModelIndex>(), proxy.mapToSource(idx));
    QCOMPARE(signalData.at(1).value<QModelIndex>(), proxy.mapToSource(idx));
    receivedRoles = signalData.at(2).value<QList<int> >();
    QCOMPARE(receivedRoles.size(), expectedRoles.size());
    for (int role : expectedRoles)
        QVERIFY(receivedRoles.contains(role));
    idx = proxy.index(1, 2, proxy.index(0, 1));
    QVERIFY(proxy.setItemData(idx, itmData));
    QCOMPARE(idx.data(BobUI::DisplayRole).toString(), QStringLiteral("Test"));
    QCOMPARE(idx.data(BobUI::UserRole).toInt(), 88);
    QCOMPARE(idx.data(BobUI::UserRole + 1).toInt(), 99);
    QCOMPARE(idx.data(BobUI::UserRole + 2).toInt(), 2);
    QCOMPARE(idx.data(BobUI::UserRole + 3).toInt(), 1);
    QCOMPARE(sourceDataChangeSpy.size(), 1);
    QCOMPARE(proxyDataChangeSpy.size(), 1);
    signalData = proxyDataChangeSpy.takeFirst();
    QCOMPARE(signalData.at(0).value<QModelIndex>(), idx);
    QCOMPARE(signalData.at(1).value<QModelIndex>(), idx);
    receivedRoles = signalData.at(2).value<QList<int> >();
    QCOMPARE(receivedRoles.size(), expectedRoles.size());
    for (int role : expectedRoles)
        QVERIFY(receivedRoles.contains(role));
    signalData = sourceDataChangeSpy.takeFirst();
    QCOMPARE(signalData.at(0).value<QModelIndex>(), proxy.mapToSource(idx));
    QCOMPARE(signalData.at(1).value<QModelIndex>(), proxy.mapToSource(idx));
    receivedRoles = signalData.at(2).value<QList<int> >();
    QCOMPARE(receivedRoles.size(), expectedRoles.size());
    for (int role : expectedRoles)
        QVERIFY(receivedRoles.contains(role));
    QVERIFY(!proxy.setItemData(QModelIndex(), itmData));
    delete model;
}

void tst_BOBUIransposeProxyModel::moveRowsBase()
{
    QStringListModel model{QStringList{"A", "B", "C", "D"}};
    BOBUIransposeProxyModel proxy;
    QSignalSpy columnsMoveSpy(&proxy, &QAbstractItemModel::columnsMoved);
    QVERIFY(columnsMoveSpy.isValid());
    QSignalSpy columnsAboutToBeMoveSpy(&proxy, &QAbstractItemModel::columnsAboutToBeMoved);
    QVERIFY(columnsAboutToBeMoveSpy.isValid());
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(&model);
    const QStringList expectedNewVal = {"B", "A", "C", "D"};
    QVERIFY(model.moveRows(QModelIndex(), 0, 1, QModelIndex(), 2));
    for (int i = 0; i < expectedNewVal.size(); ++i)
        QCOMPARE(proxy.index(0, i).data(), expectedNewVal.at(i));
    QCOMPARE(columnsMoveSpy.size(), 1);
    QCOMPARE(columnsAboutToBeMoveSpy.size(), 1);
    for (const auto &spyArgs : {columnsMoveSpy.takeFirst(),
        columnsAboutToBeMoveSpy.takeFirst()}) {
        QVERIFY(!spyArgs.at(0).value<QModelIndex>().isValid());
        QCOMPARE(spyArgs.at(1).toInt(), 0);
        QCOMPARE(spyArgs.at(2).toInt(), 0);
        QVERIFY(!spyArgs.at(3).value<QModelIndex>().isValid());
        QCOMPARE(spyArgs.at(4).toInt(), 2);
    }
}

void tst_BOBUIransposeProxyModel::moveColumnsProxy()
{
    QStringListModel model{QStringList{"A", "B", "C", "D"}};
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    QSignalSpy columnsMoveSpy(&proxy, &QAbstractItemModel::columnsMoved);
    QVERIFY(columnsMoveSpy.isValid());
    QSignalSpy columnsAboutToBeMoveSpy(&proxy, &QAbstractItemModel::columnsAboutToBeMoved);
    QVERIFY(columnsAboutToBeMoveSpy.isValid());
    QSignalSpy rowsMoveSpy(&model, &QAbstractItemModel::rowsMoved);
    QVERIFY(rowsMoveSpy.isValid());
    QSignalSpy rowsAboutToBeMoveSpy(&model, &QAbstractItemModel::rowsAboutToBeMoved);
    QVERIFY(rowsAboutToBeMoveSpy.isValid());
    proxy.setSourceModel(&model);
    const QStringList expectedNewVal = {"B", "A", "C", "D"};
    QVERIFY(proxy.moveColumns(QModelIndex(), 0, 1, QModelIndex(), 2));
    for (int i = 0; i < expectedNewVal.size(); ++i)
        QCOMPARE(proxy.index(0, i).data(), expectedNewVal.at(i));
    for (int i = 0; i < expectedNewVal.size(); ++i)
        QCOMPARE(model.index(i, 0).data(), expectedNewVal.at(i));
    QCOMPARE(columnsMoveSpy.size(), 1);
    QCOMPARE(columnsAboutToBeMoveSpy.size(), 1);
    QCOMPARE(rowsMoveSpy.size(), 1);
    QCOMPARE(rowsAboutToBeMoveSpy.size(), 1);
    for (const auto &spyArgs : {columnsMoveSpy.takeFirst(),
        columnsAboutToBeMoveSpy.takeFirst(),
        rowsMoveSpy.takeFirst(),rowsAboutToBeMoveSpy.takeFirst()}) {
        QVERIFY(!spyArgs.at(0).value<QModelIndex>().isValid());
        QCOMPARE(spyArgs.at(1).toInt(), 0);
        QCOMPARE(spyArgs.at(2).toInt(), 0);
        QVERIFY(!spyArgs.at(3).value<QModelIndex>().isValid());
    }
}

void tst_BOBUIransposeProxyModel::setData_data()
{
    BOBUIest::addColumn<QAbstractItemModel *>("model");
    BOBUIest::addColumn<bool>("rootItem");
    BOBUIest::addColumn<bool>("viaProxy");
    BOBUIest::newRow("List_via_Base") << createListModel(this) << true << false;
    BOBUIest::newRow("Table_via_Base") << createTableModel(this) << true << false;
    BOBUIest::newRow("Tree_via_Base_Root_Item") << createTreeModel(this) << true << false;
    BOBUIest::newRow("Tree_via_Base_Child_Item") << createTreeModel(this) << false << false;
    BOBUIest::newRow("List_via_Proxy") << createListModel(this) << true << true;
    BOBUIest::newRow("Table_via_Proxy") << createTableModel(this) << true << true;
    BOBUIest::newRow("Tree_via_Proxy_Root_Item") << createTreeModel(this) << true << true;
    BOBUIest::newRow("Tree_via_Proxy_Child_Item") << createTreeModel(this) << false << true;
}

void tst_BOBUIransposeProxyModel::setData()
{
    QFETCH(QAbstractItemModel *, model);
    QFETCH(bool, rootItem);
    QFETCH(bool, viaProxy);
    BOBUIransposeProxyModel proxy;
    new QAbstractItemModelTester(&proxy, &proxy);
    proxy.setSourceModel(model);
    QSignalSpy sourceDataChangeSpy(model, &QAbstractItemModel::dataChanged);
    QVERIFY(sourceDataChangeSpy.isValid());
    QSignalSpy proxyDataChangeSpy(&proxy, &QAbstractItemModel::dataChanged);
    QVERIFY(proxyDataChangeSpy.isValid());
    const QString testData = QStringLiteral("TestingSetData");
    if (viaProxy) {
        const QModelIndex parIdx = rootItem ? QModelIndex() : proxy.index(0, 1);
        QVERIFY(proxy.setData(proxy.index(0, 1, parIdx), testData));
        QCOMPARE(model->index(1, 0, proxy.mapToSource(parIdx)).data().toString(), testData);
    } else {
        const QModelIndex parIdx = rootItem ? QModelIndex() : model->index(1, 0);
        QVERIFY(model->setData(model->index(1, 0, parIdx), testData));
        QCOMPARE(proxy.index(0, 1, proxy.mapFromSource(parIdx)).data().toString(), testData);
    }
    QCOMPARE(sourceDataChangeSpy.size(), 1);
    QCOMPARE(proxyDataChangeSpy.size(), 1);
    delete model;
}

BOBUIEST_GUILESS_MAIN(tst_BOBUIransposeProxyModel)

#include "tst_bobuiransposeproxymodel.moc"
