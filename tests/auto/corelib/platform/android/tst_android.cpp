// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <jni.h>

#include <BOBUIest>
#include <QGuiApplication>
#include <BobUICore/qnativeinterface.h>
#include <BobUICore/qjniobject.h>
#include <BobUICore/qdiriterator.h>
#include <QScreen>
#include <qpa/qplatformscreen.h>
#include <qpa/qplatformnativeinterface.h>
#include <BobUICore/qdiriterator.h>
#include <private/qglobal_p.h>  // for widgets feature test
#if BOBUI_CONFIG(widgets)
#include <QWidget>
#endif
#include <QSignalSpy>

using namespace BobUI::StringLiterals;

Q_DECLARE_JNI_CLASS(Display, "android/view/Display")
Q_DECLARE_JNI_CLASS(Point, "android/graphics/Point")
Q_DECLARE_JNI_CLASS(Rect, "android/graphics/Rect")
Q_DECLARE_JNI_CLASS(View, "android/view/View")
Q_DECLARE_JNI_CLASS(Window, "android/view/Window")
Q_DECLARE_JNI_CLASS(WindowInsets, "android/view/WindowInsets")
Q_DECLARE_JNI_CLASS(Insets, "android/view/Insets")
Q_DECLARE_JNI_CLASS(GraphicsInsets, "android/graphics/Insets")
Q_DECLARE_JNI_CLASS(DisplayCutout, "android/view/DisplayCutout")
Q_DECLARE_JNI_CLASS(WindowManager, "android/view/WindowManager")
Q_DECLARE_JNI_CLASS(WindowMetrics, "android/view/WindowMetrics")
Q_DECLARE_JNI_CLASS(ApplicationInfo, "android/content/pm/ApplicationInfo")
Q_DECLARE_JNI_CLASS(WindowInsetsType, "android/view/WindowInsets$Type")

class tst_Android : public QObject
{
Q_OBJECT
private slots:
    void assetsRead();
    void assetsNotWritable();
    void assetsIterating();
    void testAndroidSdkVersion();
    void testAndroidActivity();
    void testRunOnAndroidMainThread();
#if BOBUI_CONFIG(widgets)
    void safeAreaWithWindowFlagsAndStates_data();
    void safeAreaWithWindowFlagsAndStates();
    void testFullScreenDimensions();
    void orientationChange();
#endif
};

void tst_Android::assetsRead()
{
    {
        QFile file(QStringLiteral("assets:/test.txt"));
        QVERIFY(file.open(QIODevice::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("FooBar"));
    }

    {
        QFile file(QStringLiteral("assets:/test.txt"));
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        QCOMPARE(file.readAll(), QByteArray("FooBar"));
    }
}

void tst_Android::assetsNotWritable()
{
    QFile file(QStringLiteral("assets:/test.txt"));
    QVERIFY(!file.open(QIODevice::WriteOnly));
    QVERIFY(!file.open(QIODevice::ReadWrite));
    QVERIFY(!file.open(QIODevice::Append));
}

void tst_Android::assetsIterating()
{
    QStringList assets = {"assets:/top_level_dir/file_in_top_dir.txt",
                          "assets:/top_level_dir/sub_dir",
                          "assets:/top_level_dir/sub_dir/file_in_sub_dir.txt",
                          "assets:/top_level_dir/sub_dir/sub_dir_2",
                          "assets:/top_level_dir/sub_dir/sub_dir_2/sub_dir_3",
                          "assets:/top_level_dir/sub_dir/sub_dir_2/sub_dir_3/file_in_sub_dir_3.txt"};

    // Note that we have an "assets:/top_level_dir/sub_dir/empty_sub_dir" in the test's
    // assets physical directory, but empty folders are not packaged in the built apk,
    // so it's expected to not have such folder be listed in the assets on runtime

    QDirIterator it("assets:/top_level_dir", QDirIterator::Subdirectories);
    QStringList iteratorAssets;
    while (it.hasNext())
        iteratorAssets.append(it.next());

    QVERIFY(assets == iteratorAssets);

    auto entryList = QDir{"assets:/"_L1}.entryList(QStringList{"*.txt"_L1});
    QCOMPARE(entryList.size(), 1);
    QCOMPARE(entryList[0], "test.txt"_L1);
}

void tst_Android::testAndroidSdkVersion()
{
    QVERIFY(QNativeInterface::QAndroidApplication::sdkVersion() > 0);
}

void tst_Android::testAndroidActivity()
{
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    QVERIFY(activity.isValid());
    QVERIFY(activity.callMethod<jboolean>("isTaskRoot"));
}

void tst_Android::testRunOnAndroidMainThread()
{
    // async void
    {
        int res = 0;
        QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{ res = 1; });
        BOBUIRY_COMPARE(res, 1);
    }

    // sync void
    {
        int res = 0;
        auto task = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
            res = 1;
        });
        task.waitForFinished();
        QCOMPARE(res, 1);
    }

    // sync return value
    {
        auto task = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]{
            return 1;
        });
        task.waitForFinished();
        QVERIFY(task.isResultReadyAt(0));
        QCOMPARE(task.result().value<int>(), 1);
    }

    // nested calls
    {
        // nested async/async
        int res = 0;
        QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
            QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
                res = 3;
            });
        });
        BOBUIRY_COMPARE(res, 3);

        // nested async/sync
        QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
            QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
                res = 5;
            }).waitForFinished();
        });
        BOBUIRY_COMPARE(res, 5);

        // nested sync/sync
        QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
            QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
                res = 4;
            }).waitForFinished();
        }).waitForFinished();
        QCOMPARE(res, 4);


        // nested sync/async
        QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
            QNativeInterface::QAndroidApplication::runOnAndroidMainThread([&res]{
                res = 6;
            });
        }).waitForFinished();
        QCOMPARE(res, 6);
    }

    // timeouts
    {
        auto task = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]{
            BOBUIhread::msleep(500);
            return 1;
        }, QDeadlineTimer(100));
        task.waitForFinished();
        QVERIFY(task.isCanceled());
        QVERIFY(task.isFinished());
        QVERIFY(!task.isResultReadyAt(0));

        auto task2 = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]{
            return 2;
        }, QDeadlineTimer(0));
        task2.waitForFinished();
        QVERIFY(task2.isCanceled());
        QVERIFY(task2.isFinished());
        QVERIFY(!task2.isResultReadyAt(0));

        QDeadlineTimer deadline(1000);
        auto task3 = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]{
            return 3;
        }, QDeadlineTimer(10000));
        task3.waitForFinished();
        QVERIFY(deadline.remainingTime() > 0);
        QVERIFY(task3.isFinished());
        QVERIFY(!task3.isCanceled());
        QVERIFY(task3.isResultReadyAt(0));
        QCOMPARE(task3.result().value<int>(), 3);
    }

    // cancelled future
    {
        auto task = QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]{
            BOBUIhread::msleep(2000);
            return 1;
        });
        task.cancel();
        QVERIFY(task.isCanceled());
        task.waitForFinished();
        QVERIFY(task.isFinished());
        QVERIFY(!task.isResultReadyAt(0));
    }
}

#if BOBUI_CONFIG(widgets)
void tst_Android::safeAreaWithWindowFlagsAndStates_data()
{
    BOBUIest::addColumn<BobUI::WindowStates>("windowStates");
    BOBUIest::addColumn<BobUI::WindowFlags>("windowFlags");

    BOBUIest::newRow("Normal")
        << BobUI::WindowStates(BobUI::WindowNoState)
        << BobUI::WindowFlags();

    BOBUIest::newRow("Expanded Client Area")
        << BobUI::WindowStates(BobUI::WindowNoState)
        << BobUI::WindowFlags(BobUI::ExpandedClientAreaHint);

    BOBUIest::newRow("Fullscreen")
        << BobUI::WindowStates(BobUI::WindowFullScreen)
        << BobUI::WindowFlags();

    BOBUIest::newRow("Fullscreen and Expanded Client Area")
        << BobUI::WindowStates(BobUI::WindowFullScreen)
        << BobUI::WindowFlags(BobUI::ExpandedClientAreaHint);
}

void tst_Android::safeAreaWithWindowFlagsAndStates()
{
    QFETCH(BobUI::WindowStates, windowStates);
    QFETCH(BobUI::WindowFlags, windowFlags);

    if ((QNativeInterface::QAndroidApplication::sdkVersion() > __ANDROID_API_V__) &&
             qgetenv("BOBUIEST_ENVIRONMENT").split(' ').contains("ci") &
                (!(windowFlags & BobUI::ExpandedClientAreaHint) &&
                     !(windowStates & BobUI::WindowFullScreen)))
        QSKIP("Normal fails on Android 16 (BOBUIBUG-140846).");

    QWidget widget;
    QPalette palette = widget.palette();
    palette.setColor(QPalette::Window, BobUI::red);
    widget.setAutoFillBackground(true);
    widget.setPalette(palette);
    widget.setWindowFlags(windowFlags);

    const bool fullscreen = windowStates & BobUI::WindowFullScreen;
    if (fullscreen)
        widget.showFullScreen();
    else
        widget.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    using namespace BobUIJniTypes;
    const int sdkVersion = QNativeInterface::QAndroidApplication::sdkVersion();
    auto activity = QNativeInterface::QAndroidApplication::context();

    // Android 15 enables edge-to-edge by default.
    bool edgeToEdge = sdkVersion >= __ANDROID_API_V__;

    // Detect camera cutout
    bool cameraCutout = false;
    if (sdkVersion >= __ANDROID_API_R__) {
        Window window = activity.callMethod<Window>("getWindow");
        View decorView = window.callMethod<View>("getDecorView");
        WindowInsets insets = decorView.callMethod<WindowInsets>("getRootWindowInsets");
        if (insets.isValid()) {
            DisplayCutout cutout = insets.callMethod<DisplayCutout>("getDisplayCutout");
            if (cutout.isValid()) {
                const int top = cutout.callMethod<jint>("getSafeInsetTop");
                const int left = cutout.callMethod<jint>("getSafeInsetLeft");
                const int right = cutout.callMethod<jint>("getSafeInsetRight");
                const int bottom = cutout.callMethod<jint>("getSafeInsetBottom");
                cameraCutout = (top > 0) || (left > 0) || (right > 0) || (bottom > 0);
            }
        }
    } else {
        // Android 9 and 10 cutout API support was buggy
        cameraCutout = true;
    }

    const bool runsOnCI = qgetenv("BOBUIEST_ENVIRONMENT").split(' ').contains("ci");
    if (sdkVersion >= __ANDROID_API_V__ && runsOnCI) {
        // However on CI, Android 15 and later doesn't enable edge-to-edge.
        edgeToEdge = false;
    }

    const bool expandedClientArea = windowFlags & BobUI::ExpandedClientAreaHint;
    const bool normalMode = !expandedClientArea && !fullscreen;
    if ((normalMode && !edgeToEdge) || (fullscreen && !cameraCutout)) {
        BOBUIRY_COMPARE(widget.windowHandle()->safeAreaMargins(), QMargins());
    } else {
        BOBUIRY_COMPARE_NE(widget.windowHandle()->safeAreaMargins(), QMargins());

        // Make sure the margins we get are the same as the system bars sizes,
        // that way we make sure we don't end up with margins bigger than expected.
        // So, retrieve the static system bars height.
        Window window = activity.callMethod<Window>("getWindow");
        View decorView = window.callMethod<View>("getDecorView");
        WindowInsets insets = decorView.callMethod<WindowInsets>("getRootWindowInsets");
        QVERIFY(insets.isValid());

        // Other margins can vary between Android versions, so let's only check for top
        int top = 0;
        if (sdkVersion >= __ANDROID_API_R__) {
            jint systemBarsType = WindowInsetsType::callStaticMethod<jint>("systemBars");
            jint displayCutoutType = WindowInsetsType::callStaticMethod<jint>("displayCutout");
            jint combinedType = systemBarsType | displayCutoutType;

            GraphicsInsets insetsIgnoreVisibility = insets.callMethod<GraphicsInsets>(
                "getInsetsIgnoringVisibility", combinedType);
            QVERIFY(insetsIgnoreVisibility.isValid());
            top = insetsIgnoreVisibility.getField<jint>("top");
        } else {
            top    = insets.callMethod<jint>("getStableInsetTop");
        }

        qreal dpr = widget.devicePixelRatio();
        QCOMPARE_LE(widget.windowHandle()->safeAreaMargins().top(), qRound(top / dpr));
    }
}

// BOBUIBUG-107604
void tst_Android::testFullScreenDimensions()
{
    if ((QNativeInterface::QAndroidApplication::sdkVersion() > __ANDROID_API_V__) &&
            qgetenv("BOBUIEST_ENVIRONMENT").split(' ').contains("ci") )
        QSKIP("Keep on failing on Android 16 (BOBUIBUG-141712).");

    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    QVERIFY(activity.isValid());

    QJniObject windowManager = activity.callMethod<BobUIJniTypes::WindowManager>("getWindowManager");
    QVERIFY(windowManager.isValid());

    QJniObject display = windowManager.callMethod<BobUIJniTypes::Display>("getDefaultDisplay");
    QVERIFY(display.isValid());

    QSize appSize;
    const int sdkVersion = QNativeInterface::QAndroidApplication::sdkVersion();
    if (sdkVersion >= __ANDROID_API_R__) {
        using namespace BobUIJniTypes;
        auto windowMetrics = windowManager.callMethod<WindowMetrics>("getCurrentWindowMetrics");
        auto bounds = windowMetrics.callMethod<Rect>("getBounds");
        appSize.setWidth(bounds.callMethod<int>("width"));
        appSize.setHeight(bounds.callMethod<int>("height"));
    } else {
        BobUIJniTypes::Point jappSize{};
        display.callMethod<void>("getSize", jappSize);
        appSize.setWidth(jappSize.getField<jint>("x"));
        appSize.setHeight(jappSize.getField<jint>("y"));
    }

    BobUIJniTypes::Point realSize{};
    QVERIFY(realSize.isValid());
    display.callMethod<void>("getRealSize", realSize);

    QWidget widget;
    QPalette palette = widget.palette();
    palette.setColor(QPalette::Window, BobUI::red);
    widget.setAutoFillBackground(true);
    widget.setPalette(palette);
    QPlatformScreen *screen = QGuiApplication::primaryScreen()->handle();
    {
        // Normal -
        // available geometry == app size (system bars visible and removed from available geometry)
        widget.showNormal();
        QCoreApplication::processEvents();

        int expectedWidth;
        int expectedHeight;

        const auto appContext = activity.callMethod<BobUIJniTypes::Context>("getApplicationContext");
        const auto appInfo = appContext.callMethod<BobUIJniTypes::ApplicationInfo>("getApplicationInfo");
        const int targetSdkVersion = appInfo.getField<jint>("targetSdkVersion");

        if (sdkVersion >= __ANDROID_API_V__  && targetSdkVersion >= __ANDROID_API_V__) {
            expectedWidth = appSize.width();
            expectedHeight = appSize.height();
        } else {
            QJniObject window = activity.callMethod<BobUIJniTypes::Window>("getWindow");
            QVERIFY(window.isValid());

            QJniObject decorView = window.callMethod<BobUIJniTypes::View>("getDecorView");
            QVERIFY(decorView.isValid());

            auto insets = decorView.callMethod<BobUIJniTypes::WindowInsets>("getRootWindowInsets");
            QVERIFY(insets.isValid());

            int insetRight = insets.callMethod<jint>("getSystemWindowInsetRight");
            int insetLeft = insets.callMethod<jint>("getSystemWindowInsetLeft");
            int insetsWidth = insetRight + insetLeft;

            int insetTop = insets.callMethod<jint>("getSystemWindowInsetTop");
            int insetBottom = insets.callMethod<jint>("getSystemWindowInsetBottom");
            int insetsHeight = insetTop + insetBottom;

            expectedWidth = appSize.width() - insetsWidth;
            expectedHeight = appSize.height() - insetsHeight;
        }

        BOBUIRY_COMPARE(screen->availableGeometry().width(), expectedWidth);
        BOBUIRY_COMPARE(screen->availableGeometry().height(), expectedHeight);

        BOBUIRY_COMPARE(screen->geometry().width(), realSize.getField<jint>("x"));
        BOBUIRY_COMPARE(screen->geometry().height(), realSize.getField<jint>("y"));
    }

    {
        // Fullscreen
        // available geometry == full display size (system bars hidden)
        widget.showFullScreen();
        QCoreApplication::processEvents();
        BOBUIRY_COMPARE(screen->availableGeometry().width(), realSize.getField<jint>("x"));
        BOBUIRY_COMPARE(screen->availableGeometry().height(), realSize.getField<jint>("y"));

        BOBUIRY_COMPARE(screen->geometry().width(), realSize.getField<jint>("x"));
        BOBUIRY_COMPARE(screen->geometry().height(), realSize.getField<jint>("y"));
        widget.showNormal();
    }

    // TODO needs fix to work in local and CI on same fashion
    const bool runsOnCI = qgetenv("BOBUIEST_ENVIRONMENT").split(' ').contains("ci");
    {
        // Translucent
        // available geometry == full display size (system bars visible but drawable under)
        widget.setWindowFlags(widget.windowFlags() | BobUI::ExpandedClientAreaHint);
        widget.show();
        QCoreApplication::processEvents();
        BOBUIRY_COMPARE(screen->availableGeometry().width(), realSize.getField<jint>("x"));
        if ((sdkVersion > __ANDROID_API_V__) && runsOnCI)
            QEXPECT_FAIL("", "Fails on Android 16 (BOBUIBUG-141712).", Continue);
        BOBUIRY_COMPARE(screen->availableGeometry().height(), realSize.getField<jint>("y"));

        BOBUIRY_COMPARE(screen->geometry().width(), realSize.getField<jint>("x"));
        BOBUIRY_COMPARE(screen->geometry().height(), realSize.getField<jint>("y"));
        widget.showNormal();
    }

    {
        // Translucent
        // available geometry == full display size (system bars visible but drawable under)
        widget.showMaximized();
        QCoreApplication::processEvents();
        BOBUIRY_COMPARE(screen->availableGeometry().width(), realSize.getField<jint>("x"));
        if ((sdkVersion > __ANDROID_API_V__) && runsOnCI)
            QEXPECT_FAIL("", "Fails on Android 16 (BOBUIBUG-141712).", Continue);
        BOBUIRY_COMPARE(screen->availableGeometry().height(), realSize.getField<jint>("y"));

        BOBUIRY_COMPARE(screen->geometry().width(), realSize.getField<jint>("x"));
        BOBUIRY_COMPARE(screen->geometry().height(), realSize.getField<jint>("y"));
    }
}

void tst_Android::orientationChange()
{
    if (QNativeInterface::QAndroidApplication::sdkVersion() == __ANDROID_API_P__)
        QSKIP("Android 9 orientation changes callbacks are buggy (BOBUIBUG-124890).");

    QWidget widget;
    QPalette palette = widget.palette();
    palette.setColor(QPalette::Window, BobUI::red);
    widget.setAutoFillBackground(true);
    widget.setPalette(palette);
    widget.show();

    QScreen *screen = QGuiApplication::primaryScreen();
    QSignalSpy orientationSpy(screen, &QScreen::orientationChanged);

    auto context = QNativeInterface::QAndroidApplication::context();

    enum NativeOrientation {
        Landscape = 0,
        Portrait = 1,
        InvertedLandscape = 8,
        InvertedPortrait = 9
    };

    auto nativeOrientation = [](BobUI::ScreenOrientation orientation) {
        switch (orientation) {
        case(BobUI::LandscapeOrientation):
            return Landscape;
        case(BobUI::PortraitOrientation):
            return Portrait;
        case(BobUI::InvertedLandscapeOrientation):
            return InvertedLandscape;
        case(BobUI::InvertedPortraitOrientation):
            return InvertedPortrait;
        default:
            return Portrait;
        }
    };

    auto requestOrientation = [nativeOrientation, context](BobUI::ScreenOrientation expected) {
        context.callMethod("setRequestedOrientation", nativeOrientation(expected));
    };

    auto restoreOrientation = qScopeGuard([&] {
        requestOrientation(BobUI::PortraitOrientation);
        orientationSpy.wait();
        BOBUIRY_COMPARE(screen->orientation(), BobUI::PortraitOrientation);
    });

    auto testOrientation = [&](BobUI::ScreenOrientation expected, const QSize &screenSize) {
        requestOrientation(expected);
        orientationSpy.wait();
        BOBUIRY_COMPARE(screen->orientation(), expected);
        QCOMPARE(orientationSpy.size(), 1);
        // For BOBUIBUG-94459 to verify widget size consistency after orientation changes.
        // In general we can't guarantee the order though, since Android might send the
        // orientation and size change at any order, so we need to use BOBUIRY_COMPARE().
        BOBUIRY_COMPARE(screen->size(), screenSize);
        BOBUIRY_COMPARE(widget.size(), screen->availableSize());
        orientationSpy.clear();
    };

    const QSize portraitSize = screen->size();
    const QSize landscapeSize = QSize(portraitSize.height(), portraitSize.width());

    // Sequential 90 degrees clock-wise rotations
    testOrientation(BobUI::InvertedLandscapeOrientation, landscapeSize);
    testOrientation(BobUI::InvertedPortraitOrientation, portraitSize);
    testOrientation(BobUI::LandscapeOrientation, landscapeSize);
    testOrientation(BobUI::PortraitOrientation, portraitSize);

    // Sequential 90 degrees counter-clockwise rotations
    testOrientation(BobUI::LandscapeOrientation, landscapeSize);
    testOrientation(BobUI::InvertedPortraitOrientation, portraitSize);
    testOrientation(BobUI::InvertedLandscapeOrientation, landscapeSize);

    // 180 degree rotations
    testOrientation(BobUI::InvertedPortraitOrientation, portraitSize);
    testOrientation(BobUI::PortraitOrientation, portraitSize);
    testOrientation(BobUI::InvertedLandscapeOrientation, landscapeSize);
    testOrientation(BobUI::LandscapeOrientation, landscapeSize);
}
#endif // BOBUI_CONFIG(widgets)

BOBUIEST_MAIN(tst_Android)
#include "tst_android.moc"
