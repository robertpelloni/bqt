// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BobUIGui/QPainterPath>
#include <BobUIWidgets/qgraphicsscene.h>
#include <private/qgraphicsscenebsptreeindex_p.h>
#include <private/qgraphicssceneindex_p.h>
#include <private/qgraphicsscenelinearindex_p.h>
#include <BobUIWidgets/private/qapplication_p.h>

class tst_QGraphicsSceneIndex : public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();

private slots:
    void scatteredItems_data();
    void scatteredItems();
    void overlappedItems_data();
    void overlappedItems();
    void movingItems_data();
    void movingItems();
    void connectedToSceneRectChanged();
    void items();
    void boundingRectPointIntersection_data();
    void boundingRectPointIntersection();
    void removeItems();
    void clear();

private:
    void common_data();
    QGraphicsSceneIndex *createIndex(const QString &name);
};

void tst_QGraphicsSceneIndex::initTestCase()
{
}

void tst_QGraphicsSceneIndex::common_data()
{
    BOBUIest::addColumn<QString>("indexMethod");

    BOBUIest::newRow("BSP") << QString("bsp");
    BOBUIest::newRow("Linear") << QString("linear");
}

QGraphicsSceneIndex *tst_QGraphicsSceneIndex::createIndex(const QString &indexMethod)
{
    QGraphicsSceneIndex *index = nullptr;
    QGraphicsScene *scene = new QGraphicsScene();
    if (indexMethod == "bsp")
        index = new QGraphicsSceneBspTreeIndex(scene);

    if (indexMethod == "linear")
        index = new QGraphicsSceneLinearIndex(scene);

    return index;
}

void tst_QGraphicsSceneIndex::scatteredItems_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::scatteredItems()
{
    QFETCH(QString, indexMethod);

    QGraphicsScene scene;
    scene.setItemIndexMethod(indexMethod == "linear" ? QGraphicsScene::NoIndex : QGraphicsScene::BspTreeIndex);

    for (int i = 0; i < 10; ++i)
        scene.addRect(i*50, i*50, 40, 35);

    QCOMPARE(scene.items(QPointF(5, 5)).size(), 1);
    QCOMPARE(scene.items(QPointF(55, 55)).size(), 1);
    QCOMPARE(scene.items(QPointF(-100, -100)).size(), 0);

    QCOMPARE(scene.items(QRectF(0, 0, 10, 10)).size(), 1);
    QCOMPARE(scene.items(QRectF(0, 0, 1000, 1000)).size(), 10);
    QCOMPARE(scene.items(QRectF(-100, -1000, 0, 0)).size(), 0);
}

void tst_QGraphicsSceneIndex::overlappedItems_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::overlappedItems()
{
    QFETCH(QString, indexMethod);

    QGraphicsScene scene;
    scene.setItemIndexMethod(indexMethod == "linear" ? QGraphicsScene::NoIndex : QGraphicsScene::BspTreeIndex);

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            scene.addRect(i*50, j*50, 200, 200)->setPen(QPen(BobUI::black, 0));

    QCOMPARE(scene.items(QPointF(5, 5)).size(), 1);
    QCOMPARE(scene.items(QPointF(55, 55)).size(), 4);
    QCOMPARE(scene.items(QPointF(105, 105)).size(), 9);
    QCOMPARE(scene.items(QPointF(-100, -100)).size(), 0);

    QCOMPARE(scene.items(QRectF(0, 0, 1000, 1000)).size(), 100);
    QCOMPARE(scene.items(QRectF(-100, -1000, 0, 0)).size(), 0);
    QCOMPARE(scene.items(QRectF(0, 0, 200, 200)).size(), 16);
    QCOMPARE(scene.items(QRectF(0, 0, 100, 100)).size(), 4);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 100)).size(), 2);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 1000)).size(), 10);
}

void tst_QGraphicsSceneIndex::movingItems_data()
{
    common_data();
}

void tst_QGraphicsSceneIndex::movingItems()
{
    QFETCH(QString, indexMethod);

    QGraphicsScene scene;
    scene.setItemIndexMethod(indexMethod == "linear" ? QGraphicsScene::NoIndex : QGraphicsScene::BspTreeIndex);

    for (int i = 0; i < 10; ++i)
        scene.addRect(i*50, i*50, 40, 35);

    QGraphicsRectItem *box = scene.addRect(0, 0, 10, 10);
    QCOMPARE(scene.items(QPointF(5, 5)).size(), 2);
    QCOMPARE(scene.items(QPointF(-1, -1)).size(), 0);
    QCOMPARE(scene.items(QRectF(0, 0, 5, 5)).size(), 2);

    box->setPos(10, 10);
    QCOMPARE(scene.items(QPointF(9, 9)).size(), 1);
    QCOMPARE(scene.items(QPointF(15, 15)).size(), 2);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 1)).size(), 1);

    box->setPos(-5, -5);
    QCOMPARE(scene.items(QPointF(-1, -1)).size(), 1);
    QCOMPARE(scene.items(QRectF(0, 0, 1, 1)).size(), 2);

    QCOMPARE(scene.items(QRectF(0, 0, 1000, 1000)).size(), 11);
}

void tst_QGraphicsSceneIndex::connectedToSceneRectChanged()
{

    class MyScene : public QGraphicsScene
    {
    public:
        using QGraphicsScene::receivers;
    };

    MyScene scene; // Uses QGraphicsSceneBspTreeIndex by default.
    QCOMPARE(scene.receivers(SIGNAL(sceneRectChanged(QRectF))), 1);

    scene.setItemIndexMethod(QGraphicsScene::NoIndex); // QGraphicsSceneLinearIndex
    QCOMPARE(scene.receivers(SIGNAL(sceneRectChanged(QRectF))), 1);
}

void tst_QGraphicsSceneIndex::items()
{
    QGraphicsScene scene;
    QGraphicsItem *item1 = scene.addRect(0, 0, 10, 10);
    QGraphicsItem *item2 = scene.addRect(10, 10, 10, 10);
    QCOMPARE(scene.items().size(), 2);

    // Move from unindexed items into bsp tree.
    BOBUIest::qWait(50);
    QCOMPARE(scene.items().size(), 2);

    // Add untransformable item.
    QGraphicsItem *item3 = new QGraphicsRectItem(QRectF(20, 20, 10, 10));
    item3->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    scene.addItem(item3);
    QCOMPARE(scene.items().size(), 3);

    // Move from unindexed items into untransformable items.
    BOBUIest::qWait(50);
    QCOMPARE(scene.items().size(), 3);

    // Move from untransformable items into unindexed items.
    item3->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    QCOMPARE(scene.items().size(), 3);
    BOBUIest::qWait(50);
    QCOMPARE(scene.items().size(), 3);

    // Make all items untransformable.
    item1->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    item2->setParentItem(item1);
    item3->setParentItem(item2);
    QCOMPARE(scene.items().size(), 3);

    // Move from unindexed items into untransformable items.
    BOBUIest::qWait(50);
    QCOMPARE(scene.items().size(), 3);
}

class CustomShapeItem : public QGraphicsItem
{
public:
    CustomShapeItem(const QPainterPath &shape) : QGraphicsItem(0), mShape(shape) {}

    QPainterPath shape() const override { return mShape; }
    QRectF boundingRect() const override { return mShape.boundingRect(); }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}
private:
    QPainterPath mShape;
};

Q_DECLARE_METATYPE(BobUI::ItemSelectionMode)
Q_DECLARE_METATYPE(QPainterPath)

void tst_QGraphicsSceneIndex::boundingRectPointIntersection_data()
{
    BOBUIest::addColumn<QPainterPath>("itemShape");
    BOBUIest::addColumn<BobUI::ItemSelectionMode>("mode");

    BOBUIest::newRow("zero shape - intersects rect") << QPainterPath() << BobUI::IntersectsItemBoundingRect;
    BOBUIest::newRow("zero shape - contains rect") << QPainterPath() << BobUI::ContainsItemBoundingRect;

    QPainterPath triangle;
    triangle.moveTo(50, 0);
    triangle.lineTo(0, 50);
    triangle.lineTo(100, 50);
    triangle.lineTo(50, 0);
    BOBUIest::newRow("triangle shape - intersects rect") << triangle << BobUI::IntersectsItemBoundingRect;
    BOBUIest::newRow("triangle shape - contains rect") << triangle << BobUI::ContainsItemBoundingRect;

    QPainterPath rect;
    rect.addRect(QRectF(0, 0, 100, 100));
    BOBUIest::newRow("rectangle shape - intersects rect") << rect << BobUI::IntersectsItemBoundingRect;
    BOBUIest::newRow("rectangle shape - contains rect") << rect << BobUI::ContainsItemBoundingRect;
}

void tst_QGraphicsSceneIndex::boundingRectPointIntersection()
{
    QFETCH(QPainterPath, itemShape);
    QFETCH(BobUI::ItemSelectionMode, mode);

    QGraphicsScene scene;
    CustomShapeItem *item = new CustomShapeItem(itemShape);
    scene.addItem(item);
    QList<QGraphicsItem*> items = scene.items(QPointF(0, 0), mode, BobUI::AscendingOrder);
    QVERIFY(!items.isEmpty());
    QCOMPARE(items.first(), item);
}

class RectWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    RectWidget(QGraphicsItem *parent = nullptr) : QGraphicsWidget(parent)
    {
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem * /* option */, QWidget * /* widget */) override
    {
        painter->setBrush(brush);
        painter->drawRect(boundingRect());
    }
public:
    QBrush brush;
};

void tst_QGraphicsSceneIndex::removeItems()
{
     QGraphicsScene scene;

    RectWidget *parent = new RectWidget;
    parent->brush = QBrush(QColor(BobUI::magenta));
    parent->setGeometry(250, 250, 400, 400);

    RectWidget *widget = new RectWidget(parent);
    widget->brush = QBrush(QColor(BobUI::blue));
    widget->setGeometry(10, 10, 200, 200);

    RectWidget *widgetChild1 = new RectWidget(widget);
    widgetChild1->brush = QBrush(QColor(BobUI::green));
    widgetChild1->setGeometry(20, 20, 100, 100);

    RectWidget *widgetChild2 = new RectWidget(widgetChild1);
    widgetChild2->brush = QBrush(QColor(BobUI::yellow));
    widgetChild2->setGeometry(25, 25, 50, 50);

    scene.addItem(parent);

    QGraphicsView view(&scene);
    view.resize(600, 600);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));

    scene.removeItem(widgetChild1);

    delete widgetChild1;

    //We move the parent
    scene.items(QRectF(295, 295, 50, 50));

    //This should not crash
}

void tst_QGraphicsSceneIndex::clear()
{
    class MyItem : public QGraphicsItem
    {
    public:
        MyItem(QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), numPaints(0) {}
        int numPaints;
    protected:
        QRectF boundingRect() const override { return QRectF(0, 0, 10, 10); }
        void paint(QPainter * /* painter */, const QStyleOptionGraphicsItem *, QWidget *) override
        { ++numPaints; }
    };

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 100, 100);
    scene.addItem(new MyItem);

    QGraphicsView view(&scene);
    view.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&view));
    scene.clear();

    // Make sure the index is re-generated after QGraphicsScene::clear();
    // otherwise no items will be painted.
    MyItem *item = new MyItem;
    scene.addItem(item);
    qApp->processEvents();
    BOBUIRY_VERIFY(item->numPaints > 0);
}

BOBUIEST_MAIN(tst_QGraphicsSceneIndex)
#include "tst_qgraphicssceneindex.moc"
