// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qwindow.h>
#include <qbackingstore.h>
#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformintegration.h>
#include <private/qguiapplication_p.h>
#include <qpainter.h>

#include <BOBUIest>

#include <QEvent>

// For QSignalSpy slot connections.
Q_DECLARE_METATYPE(BobUI::ScreenOrientation)

class tst_QBackingStore : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase_data();
    void init();

    void resize();
    void paint();

    void scrollRectInImage_data();
    void scrollRectInImage();

    void scroll();
    void flush_data();
    void flush();

    void staticContents();
};

void tst_QBackingStore::initTestCase_data()
{
    BOBUIest::addColumn<QSurfaceFormat::SwapBehavior>("swapBehavior");

    BOBUIest::newRow("single-buffer") << QSurfaceFormat::SingleBuffer;
    BOBUIest::newRow("double-buffer") << QSurfaceFormat::DoubleBuffer;
}

void tst_QBackingStore::init()
{
    QFETCH_GLOBAL(QSurfaceFormat::SwapBehavior, swapBehavior);

    QSurfaceFormat defaultFormat = QSurfaceFormat::defaultFormat();
    defaultFormat.setSwapBehavior(swapBehavior);
    QSurfaceFormat::setDefaultFormat(defaultFormat);
}

void tst_QBackingStore::resize()
{
    QWindow window;
    window.create();

    QBackingStore backingStore(&window);

    QRect rect(0, 0, 100, 100);
    backingStore.resize(rect.size());
    QCOMPARE(backingStore.size(), rect.size());

    // The paint device should reflect the requested
    // size, taking the window's DPR into account.
    backingStore.beginPaint(rect);
    auto paintDevice = backingStore.paintDevice();
    QCOMPARE(paintDevice->devicePixelRatio(), window.devicePixelRatio());
    QCOMPARE(QSize(paintDevice->width(), paintDevice->height()),
        rect.size() * window.devicePixelRatio());
    backingStore.endPaint();

    // So should the platform backingstore when accessed as an QImage
    QImage image = backingStore.handle()->toImage();
    if (!image.isNull()) // toImage might not be implemented
        QCOMPARE(image.size(), rect.size() * window.devicePixelRatio());
}

void tst_QBackingStore::paint()
{
    QWindow window;
    window.create();

    // The resize() test verifies that the backingstore image
    // has a size that takes the window's DPR into account.
    auto dpr = window.devicePixelRatio();

    QBackingStore backingStore(&window);

    QRect rect(0, 0, 100, 100);
    backingStore.resize(rect.size());

    // Partial fill of a fresh backingstore should not crash
    backingStore.beginPaint(QRect(0, 0, 50, 50));
    backingStore.endPaint();
    backingStore.flush(rect);

    // Two rounds, with flush in between
    for (int i = 0; i < 2; ++i) {
        backingStore.beginPaint(rect);
        QPainter p(backingStore.paintDevice());
        QColor bgColor = i ? BobUI::red : BobUI::blue;
        QColor fgColor = i ? BobUI::green : BobUI::yellow;
        p.fillRect(rect, bgColor);
        p.fillRect(QRect(50, 50, 10, 10), fgColor);
        p.end();
        backingStore.endPaint();

        QImage image = backingStore.handle()->toImage();
        if (image.isNull())
            QSKIP("Platform backingstore does not implement toImage");

        QCOMPARE(image.pixelColor(50 * dpr, 50 * dpr), fgColor);
        QCOMPARE(image.pixelColor(49 * dpr, 50 * dpr), bgColor);
        QCOMPARE(image.pixelColor(50 * dpr, 49 * dpr), bgColor);
        QCOMPARE(image.pixelColor(59 * dpr, 59 * dpr), fgColor);
        QCOMPARE(image.pixelColor(60 * dpr, 59 * dpr), bgColor);
        QCOMPARE(image.pixelColor(59 * dpr, 60 * dpr), bgColor);

        backingStore.flush(rect);
    }
}

void tst_QBackingStore::scrollRectInImage_data()
{
    BOBUIest::addColumn<QRect>("rect");
    BOBUIest::addColumn<QPoint>("offset");

    BOBUIest::newRow("empty rect") << QRect() << QPoint();
    BOBUIest::newRow("rect outside image") << QRect(-100, -100, 1000, 1000) << QPoint(10, 10);
    BOBUIest::newRow("scroll outside positive") << QRect(10, 10, 10, 10) << QPoint(1000, 1000);
    BOBUIest::newRow("scroll outside negative") << QRect(10, 10, 10, 10) << QPoint(-1000, -1000);

    BOBUIest::newRow("sub-rect positive scroll") << QRect(100, 100, 50, 50) << QPoint(10, 10);
    BOBUIest::newRow("sub-rect negative scroll") << QRect(100, 100, 50, 50) << QPoint(-10, -10);

    BOBUIest::newRow("positive vertical only") << QRect(100, 100, 50, 50) << QPoint(0, 10);
    BOBUIest::newRow("negative vertical only") << QRect(100, 100, 50, 50) << QPoint(0, -10);
    BOBUIest::newRow("positive horizontal only") << QRect(100, 100, 50, 50) << QPoint(10, 0);
    BOBUIest::newRow("negative horizontal only") << QRect(100, 100, 50, 50) << QPoint(-10, 0);

    BOBUIest::newRow("whole rect positive") << QRect(0, 0, 250, 250) << QPoint(10, 10);
    BOBUIest::newRow("whole rect negative") << QRect(0, 0, 250, 250) << QPoint(-10, -10);
}

BOBUI_BEGIN_NAMESPACE
Q_GUI_EXPORT void bobui_scrollRectInImage(QImage &, const QRect &, const QPoint &);
BOBUI_END_NAMESPACE

void tst_QBackingStore::scrollRectInImage()
{
    QImage test(250, 250, QImage::Format_ARGB32_Premultiplied);

    QFETCH(QRect, rect);
    QFETCH(QPoint, offset);

    bobui_scrollRectInImage(test, rect, offset);
}

void tst_QBackingStore::scroll()
{
    QWindow window;
    window.create();

    // The resize() test verifies that the backingstore image
    // has a size that takes the window's DPR into account.
    auto dpr = window.devicePixelRatio();

    QBackingStore backingStore(&window);
    QRect rect(0, 0, 100, 100);

    // Scrolling a backingstore without a size shouldn't crash
    backingStore.scroll(rect, 10, 10);
    backingStore.scroll(rect, -10, -10);

    backingStore.resize(rect.size());

    // Scrolling a backingstore without painting to it shouldn't crash
    backingStore.scroll(rect, 10, 10);
    backingStore.scroll(rect, -10, -10);

    // Two rounds, with flush in between
    for (int i = 0; i < 2; ++i) {

        backingStore.beginPaint(rect);
        QPainter p(backingStore.paintDevice());
        QColor bgColor = i ? BobUI::red : BobUI::blue;
        QColor fgColor = i ? BobUI::green : BobUI::yellow;
        p.fillRect(rect, bgColor);
        p.fillRect(QRect(50, 50, 10, 10), fgColor);
        p.end();
        backingStore.endPaint();

        QImage image = backingStore.handle()->toImage();
        if (image.isNull())
            QSKIP("Platform backingstore does not implement toImage");

        QCOMPARE(image.pixelColor(50 * dpr, 50 * dpr), fgColor);
        QCOMPARE(image.pixelColor(49 * dpr, 50 * dpr), bgColor);
        QCOMPARE(image.pixelColor(50 * dpr, 49 * dpr), bgColor);
        QCOMPARE(image.pixelColor(59 * dpr, 59 * dpr), fgColor);
        QCOMPARE(image.pixelColor(60 * dpr, 59 * dpr), bgColor);
        QCOMPARE(image.pixelColor(59 * dpr, 60 * dpr), bgColor);
        image = {};

        bool supportsScroll = backingStore.scroll(QRect(52, 52, 6, 6), -12, -12);
        if (!supportsScroll)
            QSKIP("Platform backingstore does not support scrolling");

        image = backingStore.handle()->toImage();
        QCOMPARE(image.pixelColor(40 * dpr, 40 * dpr), fgColor);
        QCOMPARE(image.pixelColor(39 * dpr, 40 * dpr), bgColor);
        QCOMPARE(image.pixelColor(40 * dpr, 39 * dpr), bgColor);
        QCOMPARE(image.pixelColor(45 * dpr, 45 * dpr), fgColor);
        QCOMPARE(image.pixelColor(46 * dpr, 45 * dpr), bgColor);
        QCOMPARE(image.pixelColor(45 * dpr, 46 * dpr), bgColor);
        image = {};

        backingStore.flush(rect);

        // Scroll again after flush, but before new round of painting
        backingStore.scroll(QRect(52, 52, 6, 6), 12, 12);

        image = backingStore.handle()->toImage();
        QCOMPARE(image.pixelColor(64 * dpr, 64 * dpr), fgColor);
        QCOMPARE(image.pixelColor(63 * dpr, 64 * dpr), bgColor);
        QCOMPARE(image.pixelColor(64 * dpr, 63 * dpr), bgColor);
        QCOMPARE(image.pixelColor(69 * dpr, 69 * dpr), fgColor);
        QCOMPARE(image.pixelColor(70 * dpr, 69 * dpr), bgColor);
        QCOMPARE(image.pixelColor(69 * dpr, 70 * dpr), bgColor);
        image = {};
    }
}

class Window : public QWindow
{
public:
    Window()
        : backingStore(this)
    {
    }

    void resizeEvent(QResizeEvent *) override
    {
        backingStore.resize(size());
    }

    void paintEvent(QPaintEvent *event) override
    {
        QRect rect(QPoint(), size());

        backingStore.beginPaint(rect);

        QPainter p(backingStore.paintDevice());
        p.fillRect(rect, BobUI::white);
        p.end();

        backingStore.endPaint();

        backingStore.flush(event->region().boundingRect());
    }

private:
    QBackingStore backingStore;
};

void tst_QBackingStore::flush_data()
{
    BOBUIest::addColumn<BobUI::WindowState>("windowState");

    BOBUIest::newRow("normal") << BobUI::WindowNoState;
    BOBUIest::newRow("maximized") << BobUI::WindowMaximized;
    BOBUIest::newRow("fullscreen") << BobUI::WindowFullScreen;
}

void tst_QBackingStore::flush()
{
    QFETCH(BobUI::WindowState, windowState);
    Window window;
    window.setGeometry(20, 20, 200, 200);
    window.setWindowState(windowState);
    window.setVisible(true);

    BOBUIRY_VERIFY(window.isExposed());
}

void tst_QBackingStore::staticContents()
{
    const auto *integration = QGuiApplicationPrivate::platformIntegration();
    if (!integration->hasCapability(QPlatformIntegration::BackingStoreStaticContents))
        QSKIP("Platform does not support static backingstore content");

    QWindow window;
    window.create();

    const auto dpr = window.devicePixelRatio();

    QBackingStore backingStore(&window);

    QRect initialRect(0, 0, 100, 100);

    // Static contents without paint first should not crash
    backingStore.setStaticContents(initialRect);
    backingStore.resize(initialRect.size());
    QCOMPARE(backingStore.size(), initialRect.size());
    backingStore.beginPaint(QRect(0, 0, 50, 50));
    backingStore.endPaint();
    backingStore.handle()->toImage();

    {
        backingStore.setStaticContents(QRect());
        backingStore.beginPaint(initialRect);
        QPainter p(backingStore.paintDevice());
        p.fillRect(initialRect, BobUI::green);
        p.end();
        backingStore.endPaint();

        QImage image = backingStore.handle()->toImage();
        if (image.isNull())
            QSKIP("Platform backingstore does not implement toImage");

        QCOMPARE(image.pixelColor(initialRect.topLeft() * dpr), BobUI::green);
        QCOMPARE(image.pixelColor(initialRect.bottomLeft() * dpr), BobUI::green);
        QCOMPARE(image.pixelColor(initialRect.topRight() * dpr), BobUI::green);
        QCOMPARE(image.pixelColor(initialRect.bottomRight() * dpr), BobUI::green);
    }

    {
        backingStore.setStaticContents(initialRect);

        QRect resizedRect(0, 0, 200, 200);
        backingStore.resize(resizedRect.size());

        QRegion repaintRegion = QRegion(resizedRect) - QRegion(initialRect);

        backingStore.beginPaint(repaintRegion);
        QPainter p(backingStore.paintDevice());
        for (auto repaintRect : repaintRegion)
            p.fillRect(repaintRect, BobUI::red);
        p.end();
        backingStore.endPaint();

        QImage image = backingStore.handle()->toImage();
        if (image.isNull())
            QSKIP("Platform backingstore does not implement toImage");

        QCOMPARE(image.pixelColor(initialRect.topLeft() * dpr), BobUI::green);
        QCOMPARE(image.pixelColor(initialRect.bottomLeft() * dpr), BobUI::green);
        QCOMPARE(image.pixelColor(initialRect.topRight() * dpr), BobUI::green);
        QCOMPARE(image.pixelColor(initialRect.bottomRight() * dpr), BobUI::green);

        for (auto repaintRect : repaintRegion) {
            QCOMPARE(image.pixelColor(repaintRect.topLeft() * dpr), BobUI::red);
            QCOMPARE(image.pixelColor(repaintRect.bottomLeft() * dpr), BobUI::red);
            QCOMPARE(image.pixelColor(repaintRect.topRight() * dpr), BobUI::red);
            QCOMPARE(image.pixelColor(repaintRect.bottomRight() * dpr), BobUI::red);
        }
    }
}

#include <tst_qbackingstore.moc>
BOBUIEST_MAIN(tst_QBackingStore);
