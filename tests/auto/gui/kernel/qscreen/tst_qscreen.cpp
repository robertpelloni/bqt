// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qpainter.h>
#include <qrasterwindow.h>
#include <qscreen.h>
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qplatformintegration.h>
#include <private/qguiapplication_p.h>
#include <private/qhighdpiscaling_p.h>

#include <BOBUIest>
#include <QSignalSpy>

class tst_QScreen: public QObject
{
    Q_OBJECT

private slots:
    void angleBetween_data();
    void angleBetween();
    void transformBetween_data();
    void transformBetween();
    void orientationChange();

    void grabWindow_data();
    void grabWindow();
};

void tst_QScreen::angleBetween_data()
{
    BOBUIest::addColumn<uint>("oa");
    BOBUIest::addColumn<uint>("ob");
    BOBUIest::addColumn<int>("expected");

    BOBUIest::newRow("Portrait Portrait")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::PortraitOrientation)
        << 0;

    BOBUIest::newRow("Portrait Landscape")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::LandscapeOrientation)
        << 270;

    BOBUIest::newRow("Portrait InvertedPortrait")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::InvertedPortraitOrientation)
        << 180;

    BOBUIest::newRow("Portrait InvertedLandscape")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::InvertedLandscapeOrientation)
        << 90;

    BOBUIest::newRow("InvertedLandscape InvertedPortrait")
        << uint(BobUI::InvertedLandscapeOrientation)
        << uint(BobUI::InvertedPortraitOrientation)
        << 90;

    BOBUIest::newRow("InvertedLandscape Landscape")
        << uint(BobUI::InvertedLandscapeOrientation)
        << uint(BobUI::LandscapeOrientation)
        << 180;

    BOBUIest::newRow("Landscape Primary")
        << uint(BobUI::LandscapeOrientation)
        << uint(BobUI::PrimaryOrientation)
        << QGuiApplication::primaryScreen()->angleBetween(BobUI::LandscapeOrientation, QGuiApplication::primaryScreen()->primaryOrientation());
}

void tst_QScreen::angleBetween()
{
    QFETCH( uint, oa );
    QFETCH( uint, ob );
    QFETCH( int, expected );

    BobUI::ScreenOrientation a = BobUI::ScreenOrientation(oa);
    BobUI::ScreenOrientation b = BobUI::ScreenOrientation(ob);

    QCOMPARE(QGuiApplication::primaryScreen()->angleBetween(a, b), expected);
    QCOMPARE(QGuiApplication::primaryScreen()->angleBetween(b, a), (360 - expected) % 360);
}

void tst_QScreen::transformBetween_data()
{
    BOBUIest::addColumn<uint>("oa");
    BOBUIest::addColumn<uint>("ob");
    BOBUIest::addColumn<QRect>("rect");
    BOBUIest::addColumn<BOBUIransform>("expected");

    QRect rect(0, 0, 480, 640);

    BOBUIest::newRow("Portrait Portrait")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::PortraitOrientation)
        << rect
        << BOBUIransform();

    BOBUIest::newRow("Portrait Landscape")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::LandscapeOrientation)
        << rect
        << BOBUIransform(0, -1, 1, 0, 0, rect.height());

    BOBUIest::newRow("Portrait InvertedPortrait")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::InvertedPortraitOrientation)
        << rect
        << BOBUIransform(-1, 0, 0, -1, rect.width(), rect.height());

    BOBUIest::newRow("Portrait InvertedLandscape")
        << uint(BobUI::PortraitOrientation)
        << uint(BobUI::InvertedLandscapeOrientation)
        << rect
        << BOBUIransform(0, 1, -1, 0, rect.width(), 0);

    BOBUIest::newRow("InvertedLandscape InvertedPortrait")
        << uint(BobUI::InvertedLandscapeOrientation)
        << uint(BobUI::InvertedPortraitOrientation)
        << rect
        << BOBUIransform(0, 1, -1, 0, rect.width(), 0);

    BOBUIest::newRow("InvertedLandscape Landscape")
        << uint(BobUI::InvertedLandscapeOrientation)
        << uint(BobUI::LandscapeOrientation)
        << rect
        << BOBUIransform(-1, 0, 0, -1, rect.width(), rect.height());

    BOBUIest::newRow("Landscape Primary")
        << uint(BobUI::LandscapeOrientation)
        << uint(BobUI::PrimaryOrientation)
        << rect
        << QGuiApplication::primaryScreen()->transformBetween(BobUI::LandscapeOrientation, QGuiApplication::primaryScreen()->primaryOrientation(), rect);
}

void tst_QScreen::transformBetween()
{
    QFETCH( uint, oa );
    QFETCH( uint, ob );
    QFETCH( QRect, rect );
    QFETCH( BOBUIransform, expected );

    BobUI::ScreenOrientation a = BobUI::ScreenOrientation(oa);
    BobUI::ScreenOrientation b = BobUI::ScreenOrientation(ob);

    QCOMPARE(QGuiApplication::primaryScreen()->transformBetween(a, b, rect), expected);
}

void tst_QScreen::orientationChange()
{
    qRegisterMetaType<BobUI::ScreenOrientation>("BobUI::ScreenOrientation");

    QScreen *screen = QGuiApplication::primaryScreen();
    QSignalSpy spy(screen, SIGNAL(orientationChanged(BobUI::ScreenOrientation)));
    int expectedSignalCount = 0;

    QWindowSystemInterface::handleScreenOrientationChange(screen, BobUI::LandscapeOrientation);
    QWindowSystemInterface::flushWindowSystemEvents();
    BOBUIRY_COMPARE(screen->orientation(), BobUI::LandscapeOrientation);
    QCOMPARE(spy.size(), ++expectedSignalCount);

    QWindowSystemInterface::handleScreenOrientationChange(screen, BobUI::PortraitOrientation);
    QWindowSystemInterface::flushWindowSystemEvents();
    BOBUIRY_COMPARE(screen->orientation(), BobUI::PortraitOrientation);
    QCOMPARE(spy.size(), ++expectedSignalCount);

    QWindowSystemInterface::handleScreenOrientationChange(screen, BobUI::InvertedLandscapeOrientation);
    QWindowSystemInterface::flushWindowSystemEvents();
    BOBUIRY_COMPARE(screen->orientation(), BobUI::InvertedLandscapeOrientation);
    QCOMPARE(spy.size(), ++expectedSignalCount);

    QWindowSystemInterface::handleScreenOrientationChange(screen, BobUI::InvertedPortraitOrientation);
    QWindowSystemInterface::flushWindowSystemEvents();
    BOBUIRY_COMPARE(screen->orientation(), BobUI::InvertedPortraitOrientation);
    QCOMPARE(spy.size(), ++expectedSignalCount);

    QWindowSystemInterface::handleScreenOrientationChange(screen, BobUI::LandscapeOrientation);
    QWindowSystemInterface::flushWindowSystemEvents();
    BOBUIRY_COMPARE(screen->orientation(), BobUI::LandscapeOrientation);
    QCOMPARE(spy.size(), ++expectedSignalCount);
}

void tst_QScreen::grabWindow_data()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::ScreenWindowGrabbing)
            || (QGuiApplication::platformName().toLower() == QStringLiteral("xcb") && !qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")))
        QSKIP("This platform does not support grabbing windows on screen.");
    BOBUIest::addColumn<int>("screenIndex");
    BOBUIest::addColumn<QByteArray>("screenName");
    BOBUIest::addColumn<bool>("grabWindow");
    BOBUIest::addColumn<QRect>("windowRect");
    BOBUIest::addColumn<QRect>("grabRect");

    int screenIndex = 0;
    for (const auto screen : QGuiApplication::screens()) {
        const QByteArray screenName = screen->name().toUtf8();
        const QRect availableGeometry = screen->availableGeometry();
        const QPoint topLeft = availableGeometry.topLeft() + QPoint(20, 20);
        BOBUIest::addRow("%s - Window", screenName.data())
            << screenIndex << screenName << true << QRect(topLeft, QSize(200, 200)) << QRect(0, 0, -1, -1);
        BOBUIest::addRow("%s - Window Section", screenName.data())
            << screenIndex << screenName << true << QRect(topLeft, QSize(200, 200)) << QRect(50, 50, 100, 100);
        BOBUIest::addRow("%s - Screen", screenName.data())
            << screenIndex << screenName << false << QRect(topLeft, QSize(200, 200)) << QRect(0, 0, -1, -1);
        BOBUIest::addRow("%s - Screen Section", screenName.data())
            << screenIndex << screenName << false << QRect(topLeft, QSize(200, 200)) << QRect(topLeft, QSize(200, 200));

        ++screenIndex;
    }
}

void tst_QScreen::grabWindow()
{
    QFETCH(int, screenIndex);
    QFETCH(QByteArray, screenName);
    QFETCH(bool, grabWindow);
    QFETCH(QRect, windowRect);
    QFETCH(QRect, grabRect);

    class Window : public QRasterWindow
    {
    public:
        Window(QScreen *scr)
        : image(scr->size(), QImage::Format_ARGB32_Premultiplied)
        {
            setFlags(BobUI::CustomizeWindowHint|BobUI::FramelessWindowHint|BobUI::WindowStaysOnTopHint);
            setScreen(scr);
            image.setDevicePixelRatio(scr->devicePixelRatio());
        }
        QImage image;

    protected:
        void resizeEvent(QResizeEvent *e) override
        {
            const QSize sz = e->size();
            image = image.scaled(sz * image.devicePixelRatio());
            QPainter painter(&image);
            painter.fillRect(0, 0, sz.width(), sz.height(), BobUI::black);
            painter.setPen(QPen(BobUI::red, 2));
            painter.drawLine(0, 0, sz.width(), sz.height());
            painter.drawLine(0, sz.height(), sz.width(), 0);
            painter.drawRect(0, 0, sz.width(), sz.height());
        }
        void paintEvent(QPaintEvent *) override
        {
            QPainter painter(this);
            painter.drawImage(0, 0, image);
        }
    };
    const auto screens = QGuiApplication::screens();
    double highestDpr = 0;
    for (auto screen : screens)
        highestDpr = qMax(highestDpr, screen->devicePixelRatio());

    QScreen *screen = screens.at(screenIndex);
    QCOMPARE(screen->name().toUtf8(), screenName);
    const double screenDpr = screen->devicePixelRatio();

    if (QHighDpiScaling::isActive()) {
        const float rawFactor = QHighDpiScaling::factor(screen);
        const float roundedFactor = qRound(rawFactor);
        if (!qFuzzyCompare(roundedFactor, rawFactor))
            QSKIP("HighDPI enabled with non-integer factor. Skip due to possible rounding errors.");
    }

    Window window(screen);
    window.setGeometry(windowRect);
#ifndef Q_OS_ANDROID
    window.show();
#else
    window.showNormal();
#endif

    if (!BOBUIest::qWaitForWindowExposed(&window))
        QSKIP("Failed to expose window - aborting");

    // this is necessary because of scrolling effects combined with potential slowness of VMs
    BOBUIest::qWait(1500);

    QSize expectedGrabSize = grabRect.isValid()
                           ? grabRect.size()
                           : (grabWindow ?  windowRect.size() : screen->size());
    // we ask for pixel coordinates, but will get a pixmap with device-specific DPR
    expectedGrabSize *= screen->devicePixelRatio();

    // the painted image will always be in the screen's DPR
    QImage paintedImage = window.image;
    QCOMPARE(paintedImage.devicePixelRatio(), screenDpr);

    const QPixmap pixmap = screen->grabWindow(grabWindow
                         ? window.winId()
                         : 0, grabRect.x(), grabRect.y(), grabRect.width(), grabRect.height());

    QImage grabbedImage = pixmap.toImage();
    const QSize grabbedSize = grabbedImage.size();
    QCOMPARE(grabbedSize, expectedGrabSize);

    QPoint pixelOffset = QPoint(0, 0);
    if (!grabRect.isValid()) {
        if (grabWindow) {
            // if we grab the entire window, then the grabbed image should be as large as the window
            QCOMPARE(grabbedImage.size(), paintedImage.size());
        } else {
            // if we grab the entire screen, then the grabbed image should be as large as the screen
            QCOMPARE(grabbedImage.size(), screen->size() * screenDpr);
            pixelOffset = window.geometry().topLeft() - screen->geometry().topLeft();
            grabbedImage = grabbedImage.copy(QRect(pixelOffset * screenDpr, window.geometry().size() * screenDpr));
        }
    } else if (grabWindow) {
        // if we grab the section, compare with the corresponding section from the painted image
        const QRect sectionRect = QRect(grabRect.topLeft() * screenDpr,
                                        grabRect.size() * screenDpr);
        paintedImage = paintedImage.copy(sectionRect);
    }
    QCOMPARE(grabbedImage.size(), paintedImage.size());

    // The two images might differ in format, or DPR, so instead of comparing them,
    // sample a few pixels, with appropriate distance from the edge so that any
    // compositor effect will not affect the pixel comparison.
    for (auto point : {
                       QPoint(5, 5),
                       QPoint(5, 15),
                       QPoint(paintedImage.width() - 5, paintedImage.height() - 5),
                       QPoint(paintedImage.width() - 5, paintedImage.height() - 10)
                      }) {
        QCOMPARE(grabbedImage.pixelColor(point), paintedImage.pixelColor(point));
    }
}

#include <tst_qscreen.moc>
BOBUIEST_MAIN(tst_QScreen);
