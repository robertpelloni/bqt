// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <BobUICore/QDebug>
#include <BobUIWidgets/QGraphicsItem>
#include <BobUIWidgets/QGraphicsScene>
#include <BobUIWidgets/QGraphicsView>
#include <BobUIGui/QImage>
#include <BobUIGui/QPixmapCache>

#include "chiptester/chiptester.h"
//#define CALLGRIND_DEBUG
#ifdef CALLGRIND_DEBUG
#include "valgrind/callgrind.h"
#endif

static inline void processEvents()
{
    QPixmapCache::clear();
    QApplication::processEvents();
    QApplication::processEvents();
}

class TestView : public QGraphicsView
{
    Q_OBJECT
public:
    TestView() : QGraphicsView(), waiting(false), timerId(-1)
    {}

    void waitForPaintEvent(int timeout = 4000)
    {
        if (waiting)
            return;
        waiting = true;
        timerId = startTimer(timeout);
        eventLoop.exec();
        killTimer(timerId);
        timerId = -1;
        waiting = false;
    }

    void tryResize(int width, int height)
    {
        const QSize desktopSize = QGuiApplication::primaryScreen()->size();
        if (desktopSize.width() < width)
            width = desktopSize.width();
        if (desktopSize.height() < height)
            height = desktopSize.height();
        if (size() != QSize(width, height)) {
            resize(width, height);
            BOBUIest::qWait(250);
            processEvents();
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QGraphicsView::paintEvent(event);
        if (waiting)
            eventLoop.exit();
    }

    void timerEvent(BOBUIimerEvent *event) override
    {
        if (event->timerId() == timerId)
            eventLoop.exit();
    }

private:
    QEventLoop eventLoop;
    bool waiting;
    int timerId;
};

class tst_QGraphicsView : public QObject
{
    Q_OBJECT

public:
    tst_QGraphicsView();
    virtual ~tst_QGraphicsView();

public slots:
    void initTestCase();
    void init();
    void cleanup();

private slots:
    void construct();
    void paintSingleItem();
    void paintDeepStackingItems();
    void paintDeepStackingItems_clipped();
    void moveSingleItem();
    void mapPointToScene_data();
    void mapPointToScene();
    void mapPointFromScene_data();
    void mapPointFromScene();
    void mapRectToScene_data();
    void mapRectToScene();
    void mapRectFromScene_data();
    void mapRectFromScene();
    void chipTester_data();
    void chipTester();
    void deepNesting_data();
    void deepNesting();
    void imageRiver_data();
    void imageRiver();
    void textRiver_data();
    void textRiver();
    void moveItemCache_data();
    void moveItemCache();
    void paintItemCache_data();
    void paintItemCache();

private:
    TestView mView;
};

tst_QGraphicsView::tst_QGraphicsView()
{
}

tst_QGraphicsView::~tst_QGraphicsView()
{
}

void tst_QGraphicsView::initTestCase()
{
    mView.setFrameStyle(0);
    mView.setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
    mView.setVerticalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
    mView.tryResize(100, 100);
    mView.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&mView));
    BOBUIest::qWait(300);
    processEvents();
}

void tst_QGraphicsView::init()
{
    mView.setRenderHints(QPainter::RenderHints(0));
    mView.viewport()->setMouseTracking(false);
    mView.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    for (int i = 0; i < 3; ++i)
        processEvents();
}

void tst_QGraphicsView::cleanup()
{
}

void tst_QGraphicsView::construct()
{
    QBENCHMARK {
        QGraphicsView view;
    }
}

void tst_QGraphicsView::paintSingleItem()
{
    QGraphicsScene scene(0, 0, 100, 100);
    scene.addRect(0, 0, 10, 10);

    mView.setScene(&scene);
    mView.tryResize(100, 100);
    processEvents();

    QImage image(100, 100, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    QBENCHMARK {
        mView.viewport()->render(&painter);
    }
}

#define DEEP_STACKING_COUNT 85

void tst_QGraphicsView::paintDeepStackingItems()
{
    QGraphicsScene scene(0, 0, 100, 100);
    QGraphicsRectItem *item = scene.addRect(0, 0, 10, 10);
    QGraphicsRectItem *lastRect = item;
    for (int i = 0; i < DEEP_STACKING_COUNT; ++i) {
        QGraphicsRectItem *rect = scene.addRect(0, 0, 10, 10);
        rect->setPos(1, 1);
        rect->setParentItem(lastRect);
        lastRect = rect;
    }

    mView.setScene(&scene);
    mView.tryResize(100, 100);
    processEvents();

    QImage image(100, 100, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    QBENCHMARK {
        mView.viewport()->render(&painter);
    }
}

void tst_QGraphicsView::paintDeepStackingItems_clipped()
{
    QGraphicsScene scene(0, 0, 100, 100);
    QGraphicsRectItem *item = scene.addRect(0, 0, 10, 10);
    item->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    QGraphicsRectItem *lastRect = item;
    for (int i = 0; i < DEEP_STACKING_COUNT; ++i) {
        QGraphicsRectItem *rect = scene.addRect(0, 0, 10, 10);
        rect->setPos(1, 1);
        rect->setParentItem(lastRect);
        lastRect = rect;
    }

    mView.setScene(&scene);
    mView.tryResize(100, 100);
    processEvents();

    QImage image(100, 100, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    QBENCHMARK {
        mView.viewport()->render(&painter);
    }
}

void tst_QGraphicsView::moveSingleItem()
{
    QGraphicsScene scene(0, 0, 100, 100);
    QGraphicsRectItem *item = scene.addRect(0, 0, 10, 10);

    mView.setScene(&scene);
    mView.tryResize(100, 100);
    processEvents();

    int n = 1;
    QBENCHMARK {
        item->setPos(25 * n, 25 * n);
        mView.waitForPaintEvent();
        n = n ? 0 : 1;
    }
}

void tst_QGraphicsView::mapPointToScene_data()
{
    BOBUIest::addColumn<BOBUIransform>("transform");
    BOBUIest::addColumn<QPoint>("point");

    BOBUIest::newRow("null") << BOBUIransform() << QPoint();
    BOBUIest::newRow("identity  QPoint(100, 100)") << BOBUIransform() << QPoint(100, 100);
    BOBUIest::newRow("rotate    QPoint(100, 100)") << BOBUIransform().rotate(90) << QPoint(100, 100);
    BOBUIest::newRow("scale     QPoint(100, 100)") << BOBUIransform().scale(5, 5) << QPoint(100, 100);
    BOBUIest::newRow("translate QPoint(100, 100)") << BOBUIransform().translate(5, 5) << QPoint(100, 100);
    BOBUIest::newRow("shear     QPoint(100, 100)") << BOBUIransform().shear(1.5, 1.5) << QPoint(100, 100);
    BOBUIest::newRow("perspect  QPoint(100, 100)") << BOBUIransform().rotate(45, BobUI::XAxis) << QPoint(100, 100);
}

void tst_QGraphicsView::mapPointToScene()
{
    QFETCH(BOBUIransform, transform);
    QFETCH(QPoint, point);

    QGraphicsView view;
    view.setTransform(transform);
    processEvents();

    QBENCHMARK {
        view.mapToScene(point);
    }
}

void tst_QGraphicsView::mapPointFromScene_data()
{
    BOBUIest::addColumn<BOBUIransform>("transform");
    BOBUIest::addColumn<QPointF>("point");

    BOBUIest::newRow("null") << BOBUIransform() << QPointF();
    BOBUIest::newRow("identity  QPointF(100, 100)") << BOBUIransform() << QPointF(100, 100);
    BOBUIest::newRow("rotate    QPointF(100, 100)") << BOBUIransform().rotate(90) << QPointF(100, 100);
    BOBUIest::newRow("scale     QPointF(100, 100)") << BOBUIransform().scale(5, 5) << QPointF(100, 100);
    BOBUIest::newRow("translate QPointF(100, 100)") << BOBUIransform().translate(5, 5) << QPointF(100, 100);
    BOBUIest::newRow("shear     QPointF(100, 100)") << BOBUIransform().shear(1.5, 1.5) << QPointF(100, 100);
    BOBUIest::newRow("perspect  QPointF(100, 100)") << BOBUIransform().rotate(45, BobUI::XAxis) << QPointF(100, 100);
}

void tst_QGraphicsView::mapPointFromScene()
{
    QFETCH(BOBUIransform, transform);
    QFETCH(QPointF, point);

    QGraphicsView view;
    view.setTransform(transform);
    processEvents();

    QBENCHMARK {
        view.mapFromScene(point);
    }
}

void tst_QGraphicsView::mapRectToScene_data()
{
    BOBUIest::addColumn<BOBUIransform>("transform");
    BOBUIest::addColumn<QRect>("rect");

    BOBUIest::newRow("null") << BOBUIransform() << QRect();
    BOBUIest::newRow("identity  QRect(0, 0, 100, 100)") << BOBUIransform() << QRect(0, 0, 100, 100);
    BOBUIest::newRow("rotate    QRect(0, 0, 100, 100)") << BOBUIransform().rotate(90) << QRect(0, 0, 100, 100);
    BOBUIest::newRow("scale     QRect(0, 0, 100, 100)") << BOBUIransform().scale(5, 5) << QRect(0, 0, 100, 100);
    BOBUIest::newRow("translate QRect(0, 0, 100, 100)") << BOBUIransform().translate(5, 5) << QRect(0, 0, 100, 100);
    BOBUIest::newRow("shear     QRect(0, 0, 100, 100)") << BOBUIransform().shear(1.5, 1.5) << QRect(0, 0, 100, 100);
    BOBUIest::newRow("perspect  QRect(0, 0, 100, 100)") << BOBUIransform().rotate(45, BobUI::XAxis) << QRect(0, 0, 100, 100);
}

void tst_QGraphicsView::mapRectToScene()
{
    QFETCH(BOBUIransform, transform);
    QFETCH(QRect, rect);

    QGraphicsView view;
    view.setTransform(transform);
    processEvents();

    QBENCHMARK {
        view.mapToScene(rect);
    }
}

void tst_QGraphicsView::mapRectFromScene_data()
{
    BOBUIest::addColumn<BOBUIransform>("transform");
    BOBUIest::addColumn<QRectF>("rect");

    BOBUIest::newRow("null") << BOBUIransform() << QRectF();
    BOBUIest::newRow("identity  QRectF(0, 0, 100, 100)") << BOBUIransform() << QRectF(0, 0, 100, 100);
    BOBUIest::newRow("rotate    QRectF(0, 0, 100, 100)") << BOBUIransform().rotate(90) << QRectF(0, 0, 100, 100);
    BOBUIest::newRow("scale     QRectF(0, 0, 100, 100)") << BOBUIransform().scale(5, 5) << QRectF(0, 0, 100, 100);
    BOBUIest::newRow("translate QRectF(0, 0, 100, 100)") << BOBUIransform().translate(5, 5) << QRectF(0, 0, 100, 100);
    BOBUIest::newRow("shear     QRectF(0, 0, 100, 100)") << BOBUIransform().shear(1.5, 1.5) << QRectF(0, 0, 100, 100);
    BOBUIest::newRow("perspect  QRectF(0, 0, 100, 100)") << BOBUIransform().rotate(45, BobUI::XAxis) << QRectF(0, 0, 100, 100);
}

void tst_QGraphicsView::mapRectFromScene()
{
    QFETCH(BOBUIransform, transform);
    QFETCH(QRectF, rect);

    QGraphicsView view;
    view.setTransform(transform);
    processEvents();

    QBENCHMARK {
        view.mapFromScene(rect);
    }
}

void tst_QGraphicsView::chipTester_data()
{
    BOBUIest::addColumn<bool>("antialias");
    BOBUIest::addColumn<int>("operation");
    BOBUIest::newRow("rotate") << false << 0;
    BOBUIest::newRow("rotate, antialias") << true << 0;
    BOBUIest::newRow("zoom") << false << 1;
    BOBUIest::newRow("zoom, antialias") << true << 1;
    BOBUIest::newRow("translate") << false << 2;
    BOBUIest::newRow("translate, antialias") << true << 2;
}

void tst_QGraphicsView::chipTester()
{
    QFETCH(bool, antialias);
    QFETCH(int, operation);

    ChipTester tester;
    tester.setAntialias(antialias);
    tester.setOperation(ChipTester::Operation(operation));
    tester.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tester));
    BOBUIest::qWait(250);
    processEvents();

    QBENCHMARK {
        tester.runBenchmark();
    }
}

static void addChildHelper(QGraphicsItem *parent, int n, bool rotate)
{
    if (!n)
        return;
    QGraphicsRectItem *item = new QGraphicsRectItem(QRectF(0, 0, 50, 50), parent);
    item->setPos(10, 10);
    if (rotate)
        item->setTransform(BOBUIransform().rotate(10), true);
    addChildHelper(item, n - 1, rotate);
}

void tst_QGraphicsView::deepNesting_data()
{
    BOBUIest::addColumn<bool>("rotate");
    BOBUIest::addColumn<bool>("bsp");

    BOBUIest::newRow("bsp, no transform") << false << true;
    BOBUIest::newRow("bsp, rotation") << true << true;
    BOBUIest::newRow("no transform") << false << false;
    BOBUIest::newRow("rotation") << true << false;
}

void tst_QGraphicsView::deepNesting()
{
    QFETCH(bool, rotate);
    QFETCH(bool, bsp);

    QGraphicsScene scene;
    for (int y = 0; y < 15; ++y) {
        for (int x = 0; x < 15; ++x) {
            QGraphicsItem *item1 = scene.addRect(QRectF(0, 0, 50, 50));
            if (rotate)
                item1->setTransform(BOBUIransform().rotate(10), true);
            item1->setPos(x * 25, y * 25);
            addChildHelper(item1, 30, rotate);
        }
    }
    scene.setItemIndexMethod(bsp ? QGraphicsScene::BspTreeIndex : QGraphicsScene::NoIndex);
    scene.setSceneRect(scene.sceneRect());

    mView.setRenderHint(QPainter::Antialiasing);
    mView.setScene(&scene);
    mView.tryResize(600, 600);
    (void)scene.items(QPointF(0, 0));
    processEvents();

    QBENCHMARK {
#ifdef CALLGRIND_DEBUG
        CALLGRIND_START_INSTRUMENTATION
#endif
        mView.viewport()->update();
        mView.waitForPaintEvent();
#ifdef CALLGRIND_DEBUG
        CALLGRIND_STOP_INSTRUMENTATION
#endif
    }
}

class AnimatedPixmapItem : public QGraphicsPixmapItem
{
public:
    AnimatedPixmapItem(int x, int y, bool rot, bool scal, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent), rotateFactor(0), scaleFactor(0)
    {
        rotate = rot;
        scale = scal;
        xspeed = x;
        yspeed = y;
    }

protected:
    void advance(int i) override
    {
        if (!i)
            return;
        int x = int(pos().x()) + pixmap().width();
        x += xspeed;
        x = (x % (300 + pixmap().width() * 2)) - pixmap().width();
        int y = int(pos().y()) + pixmap().width();
        y += yspeed;
        y = (y % (300 + pixmap().width() * 2)) - pixmap().width();
        setPos(x, y);

        int rot = rotateFactor;
        int sca = scaleFactor;
        if (rotate)
            rotateFactor = 1 + (rot + xspeed) % 360;
        if (scale)
            scaleFactor = 1 + (sca + yspeed) % 50;

        if (rotate || scale) {
            qreal s = 0.5 + scaleFactor / 50.0;
            setTransform(BOBUIransform().rotate(rotateFactor).scale(s, s));
        }
    }

private:
    int xspeed;
    int yspeed;
    int rotateFactor;
    int scaleFactor;
    bool rotate;
    bool scale;
};

void tst_QGraphicsView::imageRiver_data()
{
    BOBUIest::addColumn<int>("direction");
    BOBUIest::addColumn<bool>("rotation");
    BOBUIest::addColumn<bool>("scale");
    BOBUIest::newRow("horizontal") << 0 << false << false;
    BOBUIest::newRow("vertical") << 1 << false << false;
    BOBUIest::newRow("both") << 2 << false << false;
    BOBUIest::newRow("horizontal rot") << 0 << true << false;
    BOBUIest::newRow("horizontal scale") << 0 << false << true;
    BOBUIest::newRow("horizontal rot + scale") << 0 << true << true;
}

void tst_QGraphicsView::imageRiver()
{
    QFETCH(int, direction);
    QFETCH(bool, rotation);
    QFETCH(bool, scale);

    QGraphicsScene scene(0, 0, 300, 300);

    QPixmap pix(":/images/designer.png");
    QVERIFY(!pix.isNull());

    QList<QGraphicsItem *> items;
    QFile file(":/random.data");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QDataStream str(&file);
    for (int i = 0; i < 50; ++i) {
        AnimatedPixmapItem *item = 0;
        if (direction == 0) item = new AnimatedPixmapItem((i % 4) + 1, 0, rotation, scale);
        if (direction == 1) item = new AnimatedPixmapItem(0, (i % 4) + 1, rotation, scale);
        if (direction == 2) item = new AnimatedPixmapItem((i % 4) + 1, (i % 4) + 1, rotation, scale);
        item->setPixmap(pix);
        int rnd1, rnd2;
        str >> rnd1 >> rnd2;
        item->setPos(-pix.width() + rnd1 % (300 + pix.width()),
                     -pix.height() + rnd2 % (300 + pix.height()));
        scene.addItem(item);
    }
    scene.setSceneRect(0, 0, 300, 300);

    mView.setScene(&scene);
    mView.tryResize(300, 300);
    processEvents();

    QBENCHMARK {
#ifdef CALLGRIND_DEBUG
        CALLGRIND_START_INSTRUMENTATION
#endif
        for (int i = 0; i < 50; ++i) {
            scene.advance();
            mView.waitForPaintEvent();
        }
#ifdef CALLGRIND_DEBUG
        CALLGRIND_STOP_INSTRUMENTATION
#endif
    }
}

class AnimatedTextItem : public QGraphicsSimpleTextItem
{
public:
    AnimatedTextItem(int x, int y, bool rot, bool scal, QGraphicsItem *parent = nullptr)
        : QGraphicsSimpleTextItem(parent), rotateFactor(0), scaleFactor(25)
    {
        setText("River of text");
        rotate = rot;
        scale = scal;
        xspeed = x;
        yspeed = y;
    }

protected:
    void advance(int i) override
    {
        if (!i)
            return;
        QRect r = boundingRect().toRect();
        int x = int(pos().x()) + r.width();
        x += xspeed;
        x = (x % (300 + r.width() * 2)) - r.width();
        int y = int(pos().y()) + r.width();
        y += yspeed;
        y = (y % (300 + r.width() * 2)) - r.width();
        setPos(x, y);

        int rot = rotateFactor;
        int sca = scaleFactor;
        if (rotate)
            rotateFactor = 1 + (rot + xspeed) % 360;
        if (scale)
            scaleFactor = 1 + (sca + yspeed) % 50;

        if (rotate || scale) {
            qreal s = 0.5 + scaleFactor / 50.0;
            setTransform(BOBUIransform().rotate(rotateFactor).scale(s, s));
        }
    }

private:
    int xspeed;
    int yspeed;
    int rotateFactor;
    int scaleFactor;
    bool rotate;
    bool scale;
};

void tst_QGraphicsView::textRiver_data()
{
    BOBUIest::addColumn<int>("direction");
    BOBUIest::addColumn<bool>("rotation");
    BOBUIest::addColumn<bool>("scale");
    BOBUIest::newRow("horizontal") << 0 << false << false;
    BOBUIest::newRow("vertical") << 1 << false << false;
    BOBUIest::newRow("both") << 2 << false << false;
    BOBUIest::newRow("horizontal rot") << 0 << true << false;
    BOBUIest::newRow("horizontal scale") << 0 << false << true;
    BOBUIest::newRow("horizontal rot + scale") << 0 << true << true;
}

void tst_QGraphicsView::textRiver()
{
    QFETCH(int, direction);
    QFETCH(bool, rotation);
    QFETCH(bool, scale);

    QGraphicsScene scene(0, 0, 300, 300);

    QPixmap pix(":/images/designer.png");
    QVERIFY(!pix.isNull());

    QList<QGraphicsItem *> items;
    QFile file(":/random.data");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QDataStream str(&file);
    for (int i = 0; i < 50; ++i) {
        AnimatedTextItem *item = 0;
        if (direction == 0) item = new AnimatedTextItem((i % 4) + 1, 0, rotation, scale);
        if (direction == 1) item = new AnimatedTextItem(0, (i % 4) + 1, rotation, scale);
        if (direction == 2) item = new AnimatedTextItem((i % 4) + 1, (i % 4) + 1, rotation, scale);
        int rnd1, rnd2;
        str >> rnd1 >> rnd2;
        item->setPos(-pix.width() + rnd1 % (300 + pix.width()),
                     -pix.height() + rnd2 % (300 + pix.height()));
        item->setAcceptDrops(false);
        item->setAcceptHoverEvents(false);
        scene.addItem(item);
    }
    scene.setSceneRect(0, 0, 300, 300);

    mView.setScene(&scene);
    mView.tryResize(300, 300);
    processEvents();

    QBENCHMARK {
#ifdef CALLGRIND_DEBUG
        CALLGRIND_START_INSTRUMENTATION
#endif
        for (int i = 0; i < 50; ++i) {
            scene.advance();
            mView.waitForPaintEvent();
        }
#ifdef CALLGRIND_DEBUG
        CALLGRIND_STOP_INSTRUMENTATION
#endif
    }
}

class AnimatedPixmapCacheItem : public QGraphicsPixmapItem
{
public:
    AnimatedPixmapCacheItem(int x, int y, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent)
    {
        xspeed = x;
        yspeed = y;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override
    {
        QGraphicsPixmapItem::paint(painter,option,widget);
        //We just want to wait, and we don't want to process the event loop with qWait
        BOBUIest::qSleep(3);
    }
protected:
    void advance(int i) override
    {
        if (!i)
            return;
        int x = int(pos().x()) + pixmap().width();
        x += xspeed;
        x = (x % (300 + pixmap().width() * 2)) - pixmap().width();
        int y = int(pos().y()) + pixmap().width();
        y += yspeed;
        y = (y % (300 + pixmap().width() * 2)) - pixmap().width();
        setPos(x, y);
    }

private:
    int xspeed;
    int yspeed;
};

void tst_QGraphicsView::moveItemCache_data()
{
    BOBUIest::addColumn<int>("direction");
    BOBUIest::addColumn<bool>("rotation");
    BOBUIest::addColumn<int>("cacheMode");
    BOBUIest::newRow("Horizontal movement : ItemCoordinate Cache") << 0 << false << (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Horizontal movement : DeviceCoordinate Cache") << 0 << false << (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Horizontal movement : No Cache") << 0 << false << (int)QGraphicsItem::NoCache;
    BOBUIest::newRow("Vertical +  Horizontal movement : ItemCoordinate Cache") << 2 << false <<  (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Vertical +  Horizontal movement : DeviceCoordinate Cache") << 2 << false <<  (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Vertical +  Horizontal movement : No Cache") << 2 << false << (int)QGraphicsItem::NoCache;
    BOBUIest::newRow("Horizontal movement + Rotation : ItemCoordinate Cache") << 0 << true << (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Horizontal movement + Rotation : DeviceCoordinate Cache") << 0 << true << (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Horizontal movement + Rotation : No Cache") << 0 << true << (int)QGraphicsItem::NoCache;
}

void tst_QGraphicsView::moveItemCache()
{
    QFETCH(int, direction);
    QFETCH(bool, rotation);
    QFETCH(int, cacheMode);

    QGraphicsScene scene(0, 0, 300, 300);

    QPixmap pix(":/images/wine.jpeg");
    QVERIFY(!pix.isNull());

    QList<QGraphicsItem *> items;
    QFile file(":/random.data");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QDataStream str(&file);
    for (int i = 0; i < 5; ++i) {
        AnimatedPixmapCacheItem *item = 0;
        if (direction == 0) item = new AnimatedPixmapCacheItem((i % 4) + 1, 0);
        if (direction == 1) item = new AnimatedPixmapCacheItem(0, (i % 4) + 1);
        if (direction == 2) item = new AnimatedPixmapCacheItem((i % 4) + 1, (i % 4) + 1);
        item->setPixmap(pix);
        item->setCacheMode((QGraphicsItem::CacheMode)cacheMode);
        if (rotation)
            item->setTransform(BOBUIransform().rotate(45));
        int rnd1, rnd2;
        str >> rnd1 >> rnd2;
        item->setPos(-pix.width() + rnd1 % (400 + pix.width()),
                     -pix.height() + rnd2 % (400 + pix.height()));
        scene.addItem(item);
    }
    scene.setSceneRect(0, 0, 400, 400);

    mView.setScene(&scene);
    mView.tryResize(400, 400);
    processEvents();

    QBENCHMARK {
#ifdef CALLGRIND_DEBUG
        CALLGRIND_START_INSTRUMENTATION
#endif
        for (int i = 0; i < 5; ++i) {
            scene.advance();
            mView.waitForPaintEvent();
        }
#ifdef CALLGRIND_DEBUG
        CALLGRIND_STOP_INSTRUMENTATION
#endif
    }
}

class UpdatedPixmapCacheItem : public QGraphicsPixmapItem
{
public:
    UpdatedPixmapCacheItem(bool partial, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent), partial(partial)
    {
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override
    {
        QGraphicsPixmapItem::paint(painter,option,widget);
    }
protected:
    void advance(int i) override
    {
        Q_UNUSED(i);
        if (partial)
            update(QRectF(boundingRect().center().x(), boundingRect().center().x(), 30, 30));
        else
            update();
    }

private:
    bool partial;
};

void tst_QGraphicsView::paintItemCache_data()
{
    BOBUIest::addColumn<bool>("updatePartial");
    BOBUIest::addColumn<bool>("rotation");
    BOBUIest::addColumn<int>("cacheMode");
    BOBUIest::newRow("Partial Update : ItemCoordinate Cache") << true << false << (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Partial Update : DeviceCoordinate Cache") << true << false << (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Partial Update : No Cache") << true << false << (int)QGraphicsItem::NoCache;
    BOBUIest::newRow("Full Update : ItemCoordinate Cache") << false << false << (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Full Update : DeviceCoordinate Cache") << false << false << (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Full Update : No Cache") << false << false << (int)QGraphicsItem::NoCache;
    BOBUIest::newRow("Partial Update : ItemCoordinate Cache item rotated") << true << true << (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Partial Update : DeviceCoordinate Cache item rotated") << true << true << (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Partial Update : No Cache item rotated") << true << true << (int)QGraphicsItem::NoCache;
    BOBUIest::newRow("Full Update : ItemCoordinate Cache item rotated") << false  << true << (int)QGraphicsItem::ItemCoordinateCache;
    BOBUIest::newRow("Full Update : DeviceCoordinate Cache item rotated") << false << true << (int)QGraphicsItem::DeviceCoordinateCache;
    BOBUIest::newRow("Full Update : No Cache item rotated") << false << true <<(int)QGraphicsItem::NoCache;
}

void tst_QGraphicsView::paintItemCache()
{
    QFETCH(bool, updatePartial);
    QFETCH(bool, rotation);
    QFETCH(int, cacheMode);

    QGraphicsScene scene(0, 0, 300, 300);

    QPixmap pix(":/images/wine.jpeg");
    QVERIFY(!pix.isNull());

    QList<QGraphicsItem *> items;
    QFile file(":/random.data");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QDataStream str(&file);
    UpdatedPixmapCacheItem *item = new UpdatedPixmapCacheItem(updatePartial);
    item->setPixmap(pix);
    item->setCacheMode((QGraphicsItem::CacheMode)cacheMode);
    if (rotation)
        item->setTransform(BOBUIransform().rotate(45));
    item->setPos(-100, -100);
    scene.addItem(item);

    QPixmap pix2(":/images/wine-big.jpeg");
    item = new UpdatedPixmapCacheItem(updatePartial);
    item->setPixmap(pix2);
    item->setCacheMode((QGraphicsItem::CacheMode)cacheMode);
    if (rotation)
        item->setTransform(BOBUIransform().rotate(45));
    item->setPos(0, 0);
    scene.addItem(item);
    scene.setSceneRect(-100, -100, 600, 600);

    mView.tryResize(600, 600);
    mView.setScene(&scene);
    processEvents();

    QBENCHMARK {
#ifdef CALLGRIND_DEBUG
        CALLGRIND_START_INSTRUMENTATION
#endif
        for (int i = 0; i < 5; ++i) {
            scene.advance();
            mView.waitForPaintEvent();
        }
#ifdef CALLGRIND_DEBUG
        CALLGRIND_STOP_INSTRUMENTATION
#endif
    }
}

BOBUIEST_MAIN(tst_QGraphicsView)
#include "tst_qgraphicsview.moc"
