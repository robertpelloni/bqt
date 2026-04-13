// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qrasterwindow.h>
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformwindow.h>
#include <private/qguiapplication_p.h>
#include <private/qhighdpiscaling_p.h>
#include <private/qwindow_p.h>
#include <BobUIGui/QPainter>

#include <BOBUIest>
#include <BobUITest/private/bobuiesthelpers_p.h>
#include <QSignalSpy>
#include <QEvent>
#include <QStyleHints>

#if defined(Q_OS_QNX)
#include <QOpenGLContext>
#elif defined(Q_OS_WIN)
#  include <BobUICore/bobui_windows.h>
#endif

Q_LOGGING_CATEGORY(lcTests, "bobui.gui.tests")

static bool isPlatformEglFS()
{
    static const bool isEglFS = !QGuiApplication::platformName().compare(QLatin1String("eglfs"), BobUI::CaseInsensitive);
    return isEglFS;
}

class tst_QWindow: public QObject
{
    Q_OBJECT

private slots:
    void create();
    void setParent();
    void setVisible();
    void setVisibleThenCreate();
    void setVisibleFalseDoesNotCreateWindow();
    void eventOrderOnShow();
    void paintEvent();
    void resizeEventAfterResize();
    void exposeEventOnShrink_BOBUIBUG54040();
    void mapGlobal();
    void positioning();
    void framePositioning();
    void framePositioning_data();
    void framePositioningStableAfterDestroy();
    void geometryAfterWmUpdateAndDestroyCreate();
    void positioningDuringMinimized();
    void childWindowPositioning_data();
    void childWindowPositioning();
    void childWindowLevel();
    void platformSurface();
    void isExposed();
    void isActive();
    void testInputEvents();
    void touchToMouseTranslation();
    void touchToMouseTranslationForDevices();
    void mouseToTouchTranslation();
    void mouseToTouchLoop();
    void touchCancel();
    void touchCancelWithTouchToMouse();
    void touchInterruptedByPopup();
    void orientation();
    void sizes();
    void close();
    void activateAndClose();
    void mouseEventSequence();
    void windowModality();
    void inputReentrancy();
    void tabletEvents_data();
    void tabletEvents();
    void windowModality_BOBUIBUG27039();
    void visibility();
    void mask();
    void initialSize();
    void modalDialog();
    void modalDialogClosingOneOfTwoModal();
    void modalWithChildWindow();
    void modalWindowModallity();
    void modalWindowPosition();
    void modalCloseWhileBlocked();
#ifndef BOBUI_NO_CURSOR
    void modalWindowEnterEventOnHide_BOBUIBUG35109();
    void spuriousMouseMove();
#endif
    void windowsTransientChildren();
    void requestUpdate();
    void initTestCase();
    void stateChange_data();
    void stateChange();
    void flags();
    void cleanup();
    void testBlockingWindowShownAfterModalDialog();
    void generatedMouseMove();
    void keepPendingUpdateRequests();
    void activateDeactivateEvent();
    void qobject_castOnDestruction();
    void touchToMouseTranslationByPopup();
    void stateChangeSignal();
#ifndef BOBUI_NO_CURSOR
    void enterLeaveOnWindowShowHide_data();
    void enterLeaveOnWindowShowHide();
#endif
    void windowExposedAfterReparent();
    void childEvents();
    void parentEvents();

private:
    QPoint m_availableTopLeft;
    QSize m_testWindowSize;
    QPointingDevice *touchDevice = BOBUIest::createTouchDevice();
    QPointingDevice *touchDeviceWithMouseEmulation =
            BOBUIest::createTouchDevice(QInputDevice::DeviceType::TouchScreen,
                                     QInputDevice::Capability::Position | QInputDevice::Capability::MouseEmulation);
};

static bool isPlatformWayland()
{
    return QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive);
}

void tst_QWindow::initTestCase()
{
#ifdef Q_OS_ANDROID
    if (QNativeInterface::QAndroidApplication::sdkVersion() == 33)
        QSKIP("Is flaky on Android 13 / RHEL 8.6 and 8.8 (BOBUIQAINFRA-5606)");
#endif
    // Size of reference window, 200 for < 2000, scale up for larger screens
    // to avoid Windows warnings about minimum size for decorated windows.
    int width = 200;
    const QScreen *screen = QGuiApplication::primaryScreen();
    m_availableTopLeft = screen->availableGeometry().topLeft();
    const int screenWidth = screen->geometry().width();
    if (screenWidth > 2000)
        width = 100 * ((screenWidth + 500) / 1000);
    m_testWindowSize = QSize(width, width);

    // Make sure test runs consistently on all compositors by force-disabling window decorations
    if (isPlatformWayland())
        qputenv("BOBUI_WAYLAND_DISABLE_WINDOWDECORATION", "1");
}

void tst_QWindow::cleanup()
{
    QVERIFY(QGuiApplication::allWindows().isEmpty());
}

void tst_QWindow::create()
{
    QWindow a;
    QVERIFY2(!a.handle(), "QWindow should lazy init the platform window");

    a.create();
    QVERIFY2(a.handle(), "Explicitly creating a platform window should never fail");

    QWindow b;
    QWindow c(&b);
    b.create();
    QVERIFY(b.handle());
    QVERIFY2(!c.handle(), "Creating a parent window should not automatically create children");

    QWindow d;
    QWindow e(&d);
    e.create();
    QVERIFY(e.handle());
    QVERIFY2(d.handle(), "Creating a child window should automatically create parents");

    QWindow f;
    QWindow g(&f);
    f.create();
    QVERIFY(f.handle());
    QPlatformWindow *platformWindow = f.handle();
    g.create();
    QVERIFY(g.handle());
    QVERIFY2(f.handle() == platformWindow, "Creating a child window should not affect parent if already created");
}

void tst_QWindow::setParent()
{
    QWindow a;
    QWindow b(&a);
    QVERIFY2(b.parent() == &a, "Setting parent at construction time should work");
    QVERIFY2(a.children().contains(&b), "Parent should have child in list of children");

    QWindow c;
    QWindow d;
    d.setParent(&c);
    QVERIFY2(d.parent() == &c, "Setting parent after construction should work");
    QVERIFY2(c.children().contains(&d), "Parent should have child in list of children");

    a.create();
    b.setParent(nullptr);
    QVERIFY2(!b.handle(), "Making window top level shouild not automatically create it");

    QWindow e;
    c.create();
    e.setParent(&c);
    QVERIFY2(!e.handle(), "Making window a child of a created window should not automatically create it");

    QWindow f;
    QWindow g;
    g.create();
    QVERIFY(g.handle());
    g.setParent(&f);
    QVERIFY2(f.handle(), "Making a created window a child of a non-created window should automatically create it");
}

void tst_QWindow::setVisible()
{
    QWindow a;
    QWindow b(&a);
    a.setVisible(true);
    QVERIFY2(!b.handle(), "Making a top level window visible doesn't create its children");
    QVERIFY2(!b.isVisible(), "Making a top level window visible doesn't make its children visible");
    QVERIFY(BOBUIest::qWaitForWindowExposed(&a));

    QWindow c;
    QWindow d(&c);
    d.setVisible(true);
    QVERIFY2(!c.handle(), "Making a child window visible doesn't create parent window if parent is hidden");
    QVERIFY2(!c.isVisible(), "Making a child window visible doesn't make its parent visible");

    QVERIFY2(!d.handle(), "Making a child window visible doesn't create platform window if parent is hidden");

    c.create();
    QVERIFY(c.handle());
    QVERIFY2(d.handle(), "Creating a parent window should automatically create children if they are visible");
    QVERIFY2(!c.isVisible(), "Creating a parent window should not make it visible just because it has visible children");

    QWindow e;
    QWindow f(&e);
    f.setVisible(true);
    QVERIFY(!f.handle());
    QVERIFY(!e.handle());
    f.setParent(nullptr);
    QVERIFY2(f.handle(), "Making a visible but not created child window top level should create it");
    QVERIFY(BOBUIest::qWaitForWindowExposed(&f));

    QWindow g;
    QWindow h;
    QWindow i(&g);
    i.setVisible(true);
    h.setVisible(true);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&h));
    QVERIFY(!i.handle());
    QVERIFY(!g.handle());
    QVERIFY(h.handle());
    i.setParent(&h);
    QVERIFY2(i.handle(), "Making a visible but not created child window child of a created window should create it");
    QVERIFY(BOBUIest::qWaitForWindowExposed(&i));
}

class SurfaceCreatedWindow : public QWindow
{
    Q_OBJECT
public:
    using QWindow::QWindow;

    bool eventFilter(QObject *o, QEvent *e) override
    {
        if (e->type() == QEvent::PlatformSurface) {
            auto type = static_cast<QPlatformSurfaceEvent*>(e)->surfaceEventType();
            if (type == QPlatformSurfaceEvent::SurfaceCreated)
                ++surfaceCreatedEvents;
        }
        return QWindow::eventFilter(o, e);
    }

    int surfaceCreatedEvents = 0;
};

void tst_QWindow::setVisibleThenCreate()
{
    QWindow parent;
    parent.setObjectName("Parent");
    SurfaceCreatedWindow child(&parent);
    child.installEventFilter(&child);
    child.setObjectName("Child");
    child.setVisible(true);
    child.create();
    QCOMPARE(child.surfaceCreatedEvents, 1);
    parent.setVisible(true);
    QCOMPARE(child.surfaceCreatedEvents, 1);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&child));
}

void tst_QWindow::setVisibleFalseDoesNotCreateWindow()
{
    QWindow w;
    QVERIFY(!w.handle());
    w.setVisible(false);
    QVERIFY2(!w.handle(), "Hiding a non-created window doesn't create it");
    w.setVisible(true);
    QVERIFY2(w.handle(), "Showing a non-created window creates it");
}

void tst_QWindow::mapGlobal()
{
    QWindow a;
    QWindow b(&a);
    QWindow c(&b);

    a.setGeometry(10, 10, 300, 300);
    b.setGeometry(20, 20, 200, 200);
    c.setGeometry(40, 40, 100, 100);

    QCOMPARE(a.mapToGlobal(QPoint(100, 100)), QPoint(110, 110));
    auto delta = a.mapToGlobal(QPointF(100.5, 100.5)) - QPointF(110.5, 110.5);
    QVERIFY(qFuzzyIsNull(delta.manhattanLength()));

    QCOMPARE(b.mapToGlobal(QPoint(100, 100)), QPoint(130, 130));
    QCOMPARE(c.mapToGlobal(QPoint(100, 100)), QPoint(170, 170));

    QCOMPARE(a.mapFromGlobal(QPoint(100, 100)), QPoint(90, 90));
    delta = a.mapFromGlobal(QPointF(100.5, 100.5)) - QPointF(90.5, 90.5);
    QVERIFY(qFuzzyIsNull(delta.manhattanLength()));

    QCOMPARE(b.mapFromGlobal(QPoint(100, 100)), QPoint(70, 70));
    QCOMPARE(c.mapFromGlobal(QPoint(100, 100)), QPoint(30, 30));
}

class Window : public QWindow
{
public:
    Window(const BobUI::WindowFlags flags = BobUI::Window | BobUI::WindowTitleHint | BobUI::WindowMinMaxButtonsHint | BobUI::WindowCloseButtonHint)
        : QWindow(), lastReceivedWindowState(windowState())
    {
        reset();
        setFlags(flags);
#if defined(Q_OS_QNX)
        setSurfaceType(QSurface::OpenGLSurface);
#endif

#if !defined(Q_OS_MACOS)
        // FIXME: All platforms should send window-state change events, regardless
        // of the sync/async nature of the the underlying platform, but they don't.
        connect(this, &QWindow::windowStateChanged, [this]() {
            lastReceivedWindowState = windowState();
        });
#endif
    }

    void reset()
    {
        m_received.clear();
        m_framePositionsOnMove.clear();
    }

    bool event(QEvent *event) override
    {
        m_received[event->type()]++;
        m_order << event->type();
        switch (event->type()) {
        case QEvent::PlatformSurface:
            m_surfaceventType = static_cast<QPlatformSurfaceEvent *>(event)->surfaceEventType();
            break;

        case QEvent::Move:
            m_framePositionsOnMove << framePosition();
            break;

        case QEvent::WindowStateChange:
            lastReceivedWindowState = windowState();
            break;

        default:
            break;
        }

        return QWindow::event(event);
    }

    int received(QEvent::Type type)
    {
        return m_received.value(type, 0);
    }

    int eventIndex(QEvent::Type type)
    {
        return m_order.indexOf(type);
    }

    QPlatformSurfaceEvent::SurfaceEventType surfaceEventType() const
    {
        return m_surfaceventType;
    }

    QList<QPoint> m_framePositionsOnMove;
    BobUI::WindowStates lastReceivedWindowState;

private:
    QHash<QEvent::Type, int> m_received;
    QList<QEvent::Type> m_order;
    QPlatformSurfaceEvent::SurfaceEventType m_surfaceventType;
};

class ColoredWindow : public QRasterWindow {
public:
    explicit ColoredWindow(const QColor &color, QWindow *parent = nullptr) : QRasterWindow(parent), m_color(color) {}
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.fillRect(QRect(QPoint(0, 0), size()), m_color);
    }

private:
    const QColor m_color;
};

void tst_QWindow::eventOrderOnShow()
{
    // Some platforms enforce minimum widths for windows, which can cause extra resize
    // events, so set the width to suitably large value to avoid those.
    QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize);

    Window window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(geometry);
    window.show();
    QCoreApplication::processEvents();

    BOBUIRY_COMPARE(window.received(QEvent::Show), 1);
    BOBUIRY_COMPARE(window.received(QEvent::Resize), 1);
    BOBUIRY_VERIFY(window.isExposed());

    QVERIFY(window.eventIndex(QEvent::Show) < window.eventIndex(QEvent::Resize));
    QVERIFY(window.eventIndex(QEvent::Resize) < window.eventIndex(QEvent::Expose));
}

class PaintWindow : public Window
{
public:
    using Window::Window;

protected:
    void paintEvent(QPaintEvent *) override
    {
        // Handled, not calling base class
    }
};

void tst_QWindow::paintEvent()
{
    PaintWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(QRect(m_availableTopLeft, m_testWindowSize));
    window.show();

    BOBUIRY_VERIFY(window.received(QEvent::Expose));
    BOBUIRY_VERIFY(window.received(QEvent::Paint));
    QVERIFY(window.isExposed());

    // There is no defined order between paint and expose, so we don't test that

    window.reset();
    window.resize(m_testWindowSize * 2);

    BOBUIRY_VERIFY(window.received(QEvent::Paint));
    QVERIFY(!window.received(QEvent::Expose));

    window.reset();
    window.hide();

    BOBUIRY_VERIFY(window.received(QEvent::Expose));
    QVERIFY(!window.received(QEvent::Paint));
    QVERIFY(!window.isExposed());
}

void tst_QWindow::resizeEventAfterResize()
{
    // Some platforms enforce minimum widths for windows, which can cause extra resize
    // events, so set the width to suitably large value to avoid those.
    QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize * 2);

    Window window;
    window.setGeometry(geometry);
    window.showNormal();

    BOBUIRY_COMPARE(window.received(QEvent::Resize), 1);

    // BOBUIBUG-32706
    // Make sure we get a resizeEvent after calling resize
    window.resize(m_testWindowSize);

    if (isPlatformEglFS())
        QEXPECT_FAIL("", "eglfs windows are fullscreen by default.", Continue);

    BOBUIRY_COMPARE(window.received(QEvent::Resize), 2);
}

void tst_QWindow::exposeEventOnShrink_BOBUIBUG54040()
{
    if (isPlatformEglFS())
        QSKIP("", "eglfs windows are fullscreen by default.", Continue);
    Window window;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.setTitle(BOBUIest::currentTestFunction());
    window.showNormal();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    int exposeCount = window.received(QEvent::Expose);
    window.resize(window.width(), window.height() - 5);
    BOBUIRY_VERIFY(window.received(QEvent::Expose) > exposeCount);

    exposeCount = window.received(QEvent::Expose);
    window.resize(window.width() - 5, window.height());
    BOBUIRY_VERIFY(window.received(QEvent::Expose) > exposeCount);

    exposeCount = window.received(QEvent::Expose);
    window.resize(window.width() - 5, window.height() - 5);
    BOBUIRY_VERIFY(window.received(QEvent::Expose) > exposeCount);
}

// Compare a window position that may go through scaling in the platform plugin with fuzz.
static inline bool qFuzzyCompareWindowPosition(const QPoint &p1, const QPoint p2, int fuzz)
{
    return (p1 - p2).manhattanLength() <= fuzz;
}

static inline bool qFuzzyCompareWindowSize(const QSize &s1, const QSize &s2, int fuzz)
{
    const int manhattanLength = qAbs(s1.width() - s2.width()) + qAbs(s1.height() - s2.height());
    return manhattanLength <= fuzz;
}

static inline bool qFuzzyCompareWindowGeometry(const QRect &r1, const QRect &r2, int fuzz)
{
    return qFuzzyCompareWindowPosition(r1.topLeft(), r2.topLeft(), fuzz)
        && qFuzzyCompareWindowSize(r1.size(), r2.size(), fuzz);
}

static QString msgPointMismatch(const QPoint &p1, const QPoint p2)
{
    QString result;
    QDebug(&result) << p1 << "!=" << p2 << ", manhattanLength=" << (p1 - p2).manhattanLength();
    return result;
}

static QString msgRectMismatch(const QRect &r1, const QRect &r2)
{
    QString result;
    QDebug(&result) << r1 << "!=" << r2;
    return result;
}

void tst_QWindow::positioning()
{
#ifdef Q_OS_ANDROID
    QSKIP("Fails on Android. BOBUIBUG-105201");
#endif
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(
                QPlatformIntegration::NonFullScreenWindows)) {
        QSKIP("This platform does not support non-fullscreen windows");
    }

    if (isPlatformWayland())
        QSKIP("Wayland: This fails. See BOBUIBUG-68660.");

    // Some platforms enforce minimum widths for windows, which can cause extra resize
    // events, so set the width to suitably large value to avoid those.
    const QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize);

    Window window;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(20, 20), m_testWindowSize));
    window.setFramePosition(m_availableTopLeft + QPoint(40, 40)); // Move window around before show, size must not change.
    QCOMPARE(window.geometry().size(), m_testWindowSize);
    window.setGeometry(geometry);
    QCOMPARE(window.geometry(), geometry);
    //  explicitly use non-fullscreen show. show() can be fullscreen on some platforms
    window.showNormal();

    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    QMargins originalMargins = window.frameMargins();

    QCOMPARE(window.position(), window.framePosition() + QPoint(originalMargins.left(), originalMargins.top()));
    QVERIFY(window.frameGeometry().contains(window.geometry()));

    QPoint originalPos = window.position();
    QPoint originalFramePos = window.framePosition();

    window.reset();
    window.setWindowState(BobUI::WindowFullScreen);
    BOBUIRY_COMPARE(window.lastReceivedWindowState, BobUI::WindowFullScreen);

    BOBUIRY_VERIFY(window.received(QEvent::Resize) > 0);

    window.reset();
    window.setWindowState(BobUI::WindowNoState);
    BOBUIRY_COMPARE(window.lastReceivedWindowState, BobUI::WindowNoState);

    BOBUIRY_VERIFY(window.received(QEvent::Resize) > 0);

    BOBUIRY_COMPARE(originalPos, window.position());
    BOBUIRY_COMPARE(originalFramePos, window.framePosition());
    BOBUIRY_COMPARE(originalMargins, window.frameMargins());
}

void tst_QWindow::framePositioning_data()
{
    BOBUIest::addColumn<bool>("showBeforePositioning");

    BOBUIest::newRow("before show") << false;
    BOBUIest::newRow("after show") << true;
}

void tst_QWindow::framePositioning()
{
#ifdef Q_OS_ANDROID
    QSKIP("Fails on Android. BOBUIBUG-105201");
#endif
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(
                QPlatformIntegration::NonFullScreenWindows)) {
        QSKIP("This platform does not support non-fullscreen windows");
    }
    if (isPlatformWayland())
        QSKIP("Wayland: This fails. See BOBUIBUG-68660.");

    QFETCH(bool, showBeforePositioning);

    Window window;
    const QScreen *screen = window.screen();
    const QRect availableGeometry = screen->availableGeometry();
    const QPoint screenCenter = availableGeometry.center();

    const QPoint oldFramePos = window.framePosition();
    QMargins originalMargins;

    if (showBeforePositioning) {
        window.showNormal();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&window));
        // Needed on OpenSuse. The window manager (KWin) sets the frame margins after exposure. See
        // BOBUIBUG-131368.
        QVERIFY(BOBUIest::qWaitFor([&window]{ return !window.frameMargins().isNull(); }));
        originalMargins = window.frameMargins();
        window.setFramePosition(screenCenter);
    } else {
        window.setFramePosition(screenCenter);
        window.showNormal();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&window));
    }

    BOBUIRY_VERIFY(window.received(QEvent::Move));
    const int fuzz = int(QHighDpiScaling::factor(&window));
    if (!qFuzzyCompareWindowPosition(window.framePosition(), screenCenter, fuzz)) {
        qDebug() << "About to fail auto-test. Here is some additional information:";
        qDebug() << "window.framePosition() == " << window.framePosition();
        qDebug() << "old frame position == " << oldFramePos;
        qDebug() << "We received " << window.received(QEvent::Move) << " move events";
        qDebug() << "frame positions after each move event:" << window.m_framePositionsOnMove;
    }
    BOBUIRY_VERIFY2(qFuzzyCompareWindowPosition(window.framePosition(), screenCenter, fuzz),
                 qPrintable(msgPointMismatch(window.framePosition(), screenCenter)));

    if (showBeforePositioning) {
        // Repositioning should not affect existing margins
        BOBUIRY_COMPARE(originalMargins, window.frameMargins());
        QCOMPARE(window.position(), window.framePosition() + QPoint(originalMargins.left(), originalMargins.top()));
    }

    // Check that regular positioning still works

    const QPoint screenCenterAdjusted = screenCenter + QPoint(50, 50);
    window.reset();
    window.setPosition(screenCenterAdjusted);
    BOBUIRY_VERIFY(window.received(QEvent::Move));
    BOBUIRY_COMPARE(screenCenterAdjusted, window.position());
}

void tst_QWindow::framePositioningStableAfterDestroy()
{
    QWindow window;
    window.setFramePosition(QPoint(100, 100));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    const bool frameOk = BOBUIest::qWaitFor([&]{return window.geometry() != window.frameGeometry(); });
    if (!frameOk)
        qCritical() << "Frame geometry failed to update";

    const QPoint stablePosition = window.position();
    const QPoint stableFramePosition = window.framePosition();

    window.destroy();
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));
    BOBUIRY_COMPARE(window.position(), stablePosition);
    BOBUIRY_COMPARE(window.framePosition(), stableFramePosition);
}

void tst_QWindow::geometryAfterWmUpdateAndDestroyCreate()
{
    if (isPlatformWayland())
        QSKIP("A window can't be moved programmatically on Wayland");

    QWindow window;
    window.setFlag(BobUI::FramelessWindowHint);
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    if (window.windowState() != BobUI::WindowNoState)
        QSKIP("Default window state is not BobUI::WindowNoState");

    const QRect geometryAfterShow = window.geometry();

    // Check that the geometry is retained for create/destroy/create,
    // if the user has moved and resized the window via the window-manager
    // (i.e. no explicit setGeometry calls from user code).
    QRect modifiedGeometry = geometryAfterShow.translated(42, 42);
    modifiedGeometry.setSize(modifiedGeometry.size() + QSize(42, 42));
    QWindowSystemInterface::handleGeometryChange<QWindowSystemInterface::SynchronousDelivery>(
        &window, modifiedGeometry);

    window.destroy();
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    BOBUIRY_COMPARE(window.geometry(), modifiedGeometry);
}

void tst_QWindow::positioningDuringMinimized()
{
    // BOBUIBUG-39544, setting a geometry in minimized state should work as well.
    if (QGuiApplication::platformName().compare("windows", BobUI::CaseInsensitive) != 0
        && QGuiApplication::platformName().compare("cocoa", BobUI::CaseInsensitive) != 0)
        QSKIP("Not supported on this platform");
    Window window;
    window.setTitle(QStringLiteral("positioningDuringMinimized"));
    const QRect initialGeometry(m_availableTopLeft + QPoint(100, 100), m_testWindowSize);
    window.setGeometry(initialGeometry);
    window.showNormal();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));
    QCOMPARE(window.geometry(), initialGeometry);
    window.setWindowState(BobUI::WindowMinimized);
    QCOMPARE(window.geometry(), initialGeometry);
    const QRect newGeometry(initialGeometry.topLeft() + QPoint(50, 50), initialGeometry.size() + QSize(50, 50));
    window.setGeometry(newGeometry);
    BOBUIRY_COMPARE(window.geometry(), newGeometry);
    window.setWindowState(BobUI::WindowNoState);
    BOBUIRY_COMPARE(window.geometry(), newGeometry);
}

void tst_QWindow::childWindowPositioning_data()
{
    BOBUIest::addColumn<bool>("showInsteadOfCreate");

    BOBUIest::newRow("create") << false;
    BOBUIest::newRow("show") << true;
}

void tst_QWindow::childWindowPositioning()
{
    if (isPlatformEglFS())
        QSKIP("eglfs does not support child windows.");
    const QPoint topLeftOrigin(0, 0);

    ColoredWindow topLevelWindowFirst(BobUI::green);
    topLevelWindowFirst.setObjectName("topLevelWindowFirst");
    ColoredWindow childWindowAfter(BobUI::yellow, &topLevelWindowFirst);
    childWindowAfter.setObjectName("childWindowAfter");

    topLevelWindowFirst.setFramePosition(m_availableTopLeft);
    childWindowAfter.setFramePosition(topLeftOrigin);

    ColoredWindow topLevelWindowAfter(BobUI::green);
    topLevelWindowAfter.setObjectName("topLevelWindowAfter");
    ColoredWindow childWindowFirst(BobUI::yellow, &topLevelWindowAfter);
    childWindowFirst.setObjectName("childWindowFirst");

    topLevelWindowAfter.setFramePosition(m_availableTopLeft);
    childWindowFirst.setFramePosition(topLeftOrigin);

    QFETCH(bool, showInsteadOfCreate);

    QWindow *windows[] = {&topLevelWindowFirst, &childWindowAfter, &childWindowFirst, &topLevelWindowAfter};
    for (QWindow *window : windows) {
        if (showInsteadOfCreate)
            window->showNormal();
        else
            window->create();
    }

    if (showInsteadOfCreate) {
        QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevelWindowFirst));
        QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevelWindowAfter));
    }

    // Creation order shouldn't affect the geometry
    // Use try compare since on X11 the window manager may still re-position the window after expose
    BOBUIRY_COMPARE(topLevelWindowFirst.geometry(), topLevelWindowAfter.geometry());
    BOBUIRY_COMPARE(childWindowAfter.geometry(), childWindowFirst.geometry());

    // Creation order shouldn't affect the child ending up at 0,0
    QCOMPARE(childWindowFirst.framePosition(), topLeftOrigin);
    QCOMPARE(childWindowAfter.framePosition(), topLeftOrigin);
}

void tst_QWindow::childWindowLevel()
{
    ColoredWindow topLevel(BobUI::green);
    topLevel.setObjectName("topLevel");
    ColoredWindow yellowChild(BobUI::yellow, &topLevel);
    yellowChild.setObjectName("yellowChild");
    ColoredWindow redChild(BobUI::red, &topLevel);
    redChild.setObjectName("redChild");
    ColoredWindow blueChild(BobUI::blue, &topLevel);
    blueChild.setObjectName("blueChild");

    const QObjectList &siblings = topLevel.children();

    QCOMPARE(siblings.constFirst(), &yellowChild);
    QCOMPARE(siblings.constLast(), &blueChild);

    yellowChild.raise();
    QCOMPARE(siblings.constLast(), &yellowChild);

    blueChild.lower();
    QCOMPARE(siblings.constFirst(), &blueChild);
}

// BOBUIBUG-49709: Verify that the normal geometry is correctly restored
// when executing a sequence of window state changes. So far, Windows
// only where state changes have immediate effect.

typedef QList<BobUI::WindowState> WindowStateList;

Q_DECLARE_METATYPE(WindowStateList)

void tst_QWindow::stateChange_data()
{
    BOBUIest::addColumn<WindowStateList>("stateSequence");

    BOBUIest::newRow("normal->min->normal") <<
        (WindowStateList() << BobUI::WindowMinimized << BobUI::WindowNoState);
    BOBUIest::newRow("normal->maximized->normal") <<
        (WindowStateList() << BobUI::WindowMaximized << BobUI::WindowNoState);
    BOBUIest::newRow("normal->fullscreen->normal") <<
        (WindowStateList() << BobUI::WindowFullScreen << BobUI::WindowNoState);
    BOBUIest::newRow("normal->maximized->fullscreen->normal") <<
        (WindowStateList() << BobUI::WindowMaximized << BobUI::WindowFullScreen << BobUI::WindowNoState);
}

void tst_QWindow::stateChange()
{
    QFETCH(WindowStateList, stateSequence);

    if (QGuiApplication::platformName().compare(QLatin1String("windows"), BobUI::CaseInsensitive))
        QSKIP("Windows-only test");

    Window window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()) + QLatin1Char(' ') + QLatin1String(BOBUIest::currentDataTag()));
    const QRect normalGeometry(m_availableTopLeft + QPoint(40, 40), m_testWindowSize);
    window.setGeometry(normalGeometry);
    //  explicitly use non-fullscreen show. show() can be fullscreen on some platforms
    window.showNormal();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));
    for (BobUI::WindowState state : std::as_const(stateSequence)) {
        window.setWindowState(state);
        QCoreApplication::processEvents();
    }
    const QRect geometry = window.geometry();
    const int fuzz = int(QHighDpiScaling::factor(&window));
    QVERIFY2(qFuzzyCompareWindowGeometry(geometry, normalGeometry, fuzz),
             qPrintable(msgRectMismatch(geometry, normalGeometry)));
}

class PlatformWindowFilter : public QObject
{
    Q_OBJECT
public:
    explicit PlatformWindowFilter(Window *window) : m_window(window) {}

    bool eventFilter(QObject *o, QEvent *e) override
    {
        // Check that the platform surface events are delivered synchronously.
        // If they are, the native platform surface should always exist when we
        // receive a QPlatformSurfaceEvent
        if (e->type() == QEvent::PlatformSurface && o == m_window) {
            m_alwaysExisted &= (m_window->handle() != nullptr);
        }
        return false;
    }

    bool surfaceExisted() const { return m_alwaysExisted; }

private:
    Window *m_window;
    bool m_alwaysExisted = true;
};

void tst_QWindow::platformSurface()
{
    QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize);

    Window window;
    PlatformWindowFilter filter(&window);
    window.installEventFilter(&filter);

    window.setGeometry(geometry);
    QCOMPARE(window.geometry(), geometry);
    window.create();

    BOBUIRY_COMPARE(window.received(QEvent::PlatformSurface), 1);
    BOBUIRY_COMPARE(window.surfaceEventType(), QPlatformSurfaceEvent::SurfaceCreated);
    BOBUIRY_VERIFY(window.handle() != nullptr);

    window.destroy();
    BOBUIRY_COMPARE(window.received(QEvent::PlatformSurface), 2);
    BOBUIRY_COMPARE(window.surfaceEventType(), QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed);
    BOBUIRY_VERIFY(!window.handle());

    // Check for synchronous delivery of platform surface events and that the platform
    // surface always existed upon event delivery
    BOBUIRY_VERIFY(filter.surfaceExisted());
}

void tst_QWindow::isExposed()
{
    QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize);

    Window window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(geometry);
    QCOMPARE(window.geometry(), geometry);
    window.show();
    QCoreApplication::processEvents();

    BOBUIRY_VERIFY(window.received(QEvent::Expose) > 0);
    BOBUIRY_VERIFY(window.isExposed());

    window.hide();

    QCoreApplication::processEvents();
    BOBUIRY_VERIFY(window.received(QEvent::Expose) > 1);
    BOBUIRY_VERIFY(!window.isExposed());
}


void tst_QWindow::isActive()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    Window window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    // Some platforms enforce minimum widths for windows, which can cause extra resize
    // events, so set the width to suitably large value to avoid those.
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QCoreApplication::processEvents();

    BOBUIRY_VERIFY(window.isExposed());
#if defined(Q_OS_QNX) // We either need to create a eglSurface or a create a backing store
                      // and then post the window in order for screen to show the window
    QOpenGLContext context;
    context.create();
    context.makeCurrent(&window);
    context.swapBuffers(&window);
#endif
    BOBUIRY_COMPARE(window.received(QEvent::Resize), 1);
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &window);
    QVERIFY(window.isActive());

    if (isPlatformWayland())
        QSKIP("A nested window or a subsurface in wayland terms can't get focus.");

    Window child;
    child.setParent(&window);
    child.setGeometry(10, 10, 20, 20);
    child.show();

    BOBUIRY_VERIFY(child.isExposed());

    child.requestActivate();

    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &child);
    QVERIFY(child.isActive());

    // parent shouldn't receive new resize events from child being shown
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.received(QEvent::Resize), 1);
    BOBUIRY_COMPARE(window.received(QEvent::FocusIn), 1);
    BOBUIRY_COMPARE(window.received(QEvent::FocusOut), 1);
    BOBUIRY_COMPARE(child.received(QEvent::FocusIn), 1);

    // child has focus
    QVERIFY(window.isActive());

    // test focus back to parent and then back to child (BOBUIBUG-39362)
    // also verify the cumulative FocusOut and FocusIn counts
    // activate parent
    window.requestActivate();
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &window);
    QVERIFY(window.isActive());
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(child.received(QEvent::FocusOut), 1);
    BOBUIRY_COMPARE(window.received(QEvent::FocusIn), 2);

    // activate child again
    child.requestActivate();
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &child);
    QVERIFY(child.isActive());
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.received(QEvent::FocusOut), 2);
    BOBUIRY_COMPARE(child.received(QEvent::FocusIn), 2);

    Window dialog;
    dialog.setTransientParent(&window);
    dialog.setGeometry(QRect(m_availableTopLeft + QPoint(110, 100), m_testWindowSize));
    dialog.show();

    dialog.requestActivate();

    BOBUIRY_VERIFY(dialog.isExposed());
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(dialog.received(QEvent::Resize), 1);
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &dialog);
    QVERIFY(dialog.isActive());

    // transient child has focus
    QVERIFY(window.isActive());

    // parent is active
    QVERIFY(child.isActive());

    window.requestActivate();

    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &window);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(dialog.received(QEvent::FocusOut), 1);
    // We should be checking for exactly three, but since this is a try-compare _loop_, we might
    // loose and regain focus multiple times in the event of a system popup. This has been observed
    // to fail on Windows, see BOBUIBUG-77769.
    BOBUIRY_VERIFY2(window.received(QEvent::FocusIn) >= 3,
                 qPrintable(
                 QStringLiteral("Expected more than three focus in events, received: %1")
                 .arg(window.received(QEvent::FocusIn))));

    QVERIFY(window.isActive());

    // transient parent has focus
    QVERIFY(dialog.isActive());

    // parent has focus
    QVERIFY(child.isActive());
}

class InputTestWindow : public ColoredWindow
{
public:
    void keyPressEvent(QKeyEvent *event) override
    {
        keyPressCode = event->key();
    }
    void keyReleaseEvent(QKeyEvent *event) override
    {
        keyReleaseCode = event->key();
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        qCDebug(lcTests) << event;
        mouseDevice = event->pointingDevice();
        if (ignoreMouse) {
            event->ignore();
        } else {
            QCOMPARE(event->isBeginEvent(), true);
            QCOMPARE(event->isUpdateEvent(), false);
            QCOMPARE(event->isEndEvent(), false);
            QCOMPARE(event->points().first().state(), QEventPoint::State::Pressed);
            ++mousePressedCount;
            mouseSequenceSignature += 'p';
            mousePressButton = event->button();
            mousePressScreenPos = event->globalPosition();
            mousePressLocalPos = event->position();
            if (spinLoopWhenPressed)
                QCoreApplication::processEvents();
        }
        if (closeOnTap)
            this->close();

    }
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        qCDebug(lcTests) << event;
        if (ignoreMouse) {
            event->ignore();
        } else {
            QCOMPARE(event->isBeginEvent(), false);
            QCOMPARE(event->isUpdateEvent(), false);
            QCOMPARE(event->isEndEvent(), true);
            QCOMPARE(event->points().first().state(), QEventPoint::State::Released);
            ++mouseReleasedCount;
            mouseSequenceSignature += 'r';
            mouseReleaseButton = event->button();
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        qCDebug(lcTests) << event;
        mouseMoveDevice = event->pointingDevice();
        buttonStateInGeneratedMove = event->buttons();
        if (ignoreMouse) {
            event->ignore();
        } else {
            QCOMPARE(event->isBeginEvent(), false);
            QCOMPARE(event->isUpdateEvent(), true);
            QCOMPARE(event->isEndEvent(), false);
            QCOMPARE(event->points().first().state(), QEventPoint::State::Updated);
            ++mouseMovedCount;
            mouseMoveButton = event->button();
            mouseMoveScreenPos = event->globalPosition();
        }
    }
    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        qCDebug(lcTests) << event;
        if (ignoreMouse) {
            event->ignore();
        } else {
            QCOMPARE(event->isBeginEvent(), false);
            QCOMPARE(event->isUpdateEvent(), true);
            QCOMPARE(event->isEndEvent(), false);
            QCOMPARE(event->points().first().state(), QEventPoint::State::Stationary);
            ++mouseDoubleClickedCount;
            mouseSequenceSignature += 'd';
        }
    }
    void touchEvent(BOBUIouchEvent *event) override
    {
        qCDebug(lcTests) << event;
        touchDevice = event->pointingDevice();
        if (ignoreTouch) {
            event->ignore();
            return;
        }
        touchEventType = event->type();
        QList<BOBUIouchEvent::TouchPoint> points = event->points();
        for (int i = 0; i < points.size(); ++i) {
            const auto &point = points.at(i);
            switch (point.state()) {
            case QEventPoint::State::Pressed:
                ++touchPressedCount;
                if (spinLoopWhenPressed)
                    QCoreApplication::processEvents();
                if (i == 0) {
                    touchPressLocalPos = point.position();
                    touchPressGlobalPos = point.globalPosition();
                }
                if (closeOnTap)
                    this->close();
                break;
            case QEventPoint::State::Released:
                ++touchReleasedCount;
                break;
            case QEventPoint::State::Updated:
                ++touchMovedCount;
                break;
            default:
                break;
            }
        }
    }
    bool event(QEvent *e) override
    {
        switch (e->type()) {
        case QEvent::Enter:
            qCDebug(lcTests) << e;
            ++enterEventCount;
            break;
        case QEvent::Leave:
            qCDebug(lcTests) << e;
            ++leaveEventCount;
            break;
        default:
            break;
        }
        return ColoredWindow::event(e);
    }
    void resetCounters()
    {
        mousePressedCount = mouseReleasedCount = mouseMovedCount = mouseDoubleClickedCount = 0;
        mouseSequenceSignature.clear();
        touchPressedCount = touchReleasedCount = touchMovedCount = 0;
        enterEventCount = leaveEventCount = 0;
    }

    explicit InputTestWindow(const QColor &color = BobUI::white, QWindow *parent = nullptr)
        : ColoredWindow(color, parent) {}

    int keyPressCode = 0, keyReleaseCode = 0;
    int mousePressButton = 0, mouseReleaseButton = 0, mouseMoveButton = 0;
    int mousePressedCount = 0, mouseReleasedCount = 0, mouseMovedCount = 0, mouseDoubleClickedCount = 0;
    QString mouseSequenceSignature;
    QPointF mousePressScreenPos, mouseMoveScreenPos, mousePressLocalPos;
    QPointF touchPressGlobalPos, touchPressLocalPos;
    int touchPressedCount = 0, touchReleasedCount = 0, touchMovedCount = 0;
    QEvent::Type touchEventType = QEvent::None;
    int enterEventCount = 0, leaveEventCount = 0;

    bool ignoreMouse = false, ignoreTouch = false;

    bool spinLoopWhenPressed = false;
    BobUI::MouseButtons buttonStateInGeneratedMove;

    const QPointingDevice *mouseMoveDevice = nullptr;
    const QPointingDevice *mouseDevice = nullptr;
    const QPointingDevice *touchDevice = nullptr;

    bool closeOnTap = false;
};

static void simulateMouseClick(QWindow *target, const QPointF &local, const QPointF &global)
{
    QWindowSystemInterface::handleMouseEvent(target, local, global,
                                             BobUI::LeftButton, BobUI::LeftButton, QEvent::MouseButtonPress);
    QWindowSystemInterface::handleMouseEvent(target, local, global,
                                             {}, BobUI::LeftButton, QEvent::MouseButtonRelease);
}

static void simulateMouseClick(QWindow *target, ulong &timeStamp,
                               const QPointF &local, const QPointF &global)
{
    QWindowSystemInterface::handleMouseEvent(target, timeStamp++, local, global,
                                             BobUI::LeftButton, BobUI::LeftButton, QEvent::MouseButtonPress);
    QWindowSystemInterface::handleMouseEvent(target, timeStamp++, local, global,
                                             {}, BobUI::LeftButton, QEvent::MouseButtonRelease);
}

void tst_QWindow::testInputEvents()
{
    InputTestWindow window;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.showNormal();
    BOBUIRY_VERIFY(window.isActive());
    QVERIFY(BOBUIestPrivate::ensurePositionTopLeft(&window));

    BOBUIest::keyClick(&window, BobUI::Key_A, BobUI::NoModifier);
    QCoreApplication::processEvents();
    QCOMPARE(window.keyPressCode, int(BobUI::Key_A));
    QCOMPARE(window.keyReleaseCode, int(BobUI::Key_A));

    QPointF local(12, 34);
    BOBUIest::mouseClick(&window, BobUI::LeftButton, BobUI::NoModifier, local.toPoint());
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressButton, int(BobUI::LeftButton));
    QCOMPARE(window.mouseReleaseButton, int(BobUI::LeftButton));
    QCOMPARE(window.mousePressLocalPos, local);

    QList<QWindowSystemInterface::TouchPoint> points;
    QWindowSystemInterface::TouchPoint tp1, tp2;
    tp1.id = 1;
    tp1.state = QEventPoint::State::Pressed;
    tp1.area = QRect(10, 10, 4, 4);
    tp2.id = 2;
    tp2.state = QEventPoint::State::Pressed;
    tp2.area = QRect(20, 20, 4, 4);
    points << tp1 << tp2;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    points[0].state = QEventPoint::State::Released;
    points[1].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchPressedCount, 2);
    BOBUIRY_COMPARE(window.touchReleasedCount, 2);

    // Now with null pointer as window. local param should not be utilized:
    // handleMouseEvent() with tlw == 0 means the event is in global coords only.
    window.mousePressButton = window.mouseReleaseButton = 0;
    const QPointF nonWindowGlobal(window.geometry().topRight() + QPoint(200, 50)); // not inside the window
    const QPointF deviceNonWindowGlobal = QHighDpi::toNativePixels(nonWindowGlobal, window.screen());
    simulateMouseClick(nullptr, deviceNonWindowGlobal, deviceNonWindowGlobal);
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressButton, 0);
    QCOMPARE(window.mouseReleaseButton, 0);
    const QPointF windowGlobal = window.mapToGlobal(local.toPoint());
    const QPointF deviceWindowGlobal = QHighDpi::toNativePixels(windowGlobal, window.screen());
    simulateMouseClick(nullptr, deviceWindowGlobal, deviceWindowGlobal);
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressButton, int(BobUI::LeftButton));
    QCOMPARE(window.mouseReleaseButton, int(BobUI::LeftButton));
    QCOMPARE(window.mousePressScreenPos, windowGlobal);
    QCOMPARE(window.mousePressLocalPos, local); // the local we passed was bogus, verify that qGuiApp calculated the proper one
}

void tst_QWindow::touchToMouseTranslation()
{
    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.ignoreTouch = true;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QList<QWindowSystemInterface::TouchPoint> points;
    QWindowSystemInterface::TouchPoint tp1, tp2;
    const QRectF pressArea(101, 102, 4, 4);
    const QRectF moveArea(105, 108, 4, 4);
    tp1.id = 1;
    tp1.state = QEventPoint::State::Pressed;
    tp1.area = QHighDpi::toNativePixels(pressArea, &window);
    tp2.id = 2;
    tp2.state = QEventPoint::State::Pressed;
    points << tp1 << tp2;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    // Now an update but with changed list order. The mouse event should still
    // be generated from the point with id 1.
    tp1.id = 2;
    tp1.state = QEventPoint::State::Stationary;
    tp2.id = 1;
    tp2.state = QEventPoint::State::Updated;
    tp2.area = QHighDpi::toNativePixels(moveArea, &window);
    points.clear();
    points << tp1 << tp2;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    points[0].state = QEventPoint::State::Released;
    points[1].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();

    BOBUIRY_COMPARE(window.mousePressButton, int(BobUI::LeftButton));
    BOBUIRY_COMPARE(window.mouseReleaseButton, int(BobUI::LeftButton));
    BOBUIRY_COMPARE(window.mousePressScreenPos, pressArea.center());
    BOBUIRY_COMPARE(window.mouseMoveScreenPos, moveArea.center());
    QCOMPARE(window.mouseDevice, window.touchDevice);
    QCOMPARE(window.mouseDevice->type(), QInputDevice::DeviceType::TouchScreen);

    window.mousePressButton = 0;
    window.mouseReleaseButton = 0;

    window.ignoreTouch = false;

    points[0].state = QEventPoint::State::Pressed;
    points[1].state = QEventPoint::State::Pressed;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    points[0].state = QEventPoint::State::Released;
    points[1].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();

    // no new mouse events should be generated since the input window handles the touch events
    BOBUIRY_COMPARE(window.mousePressButton, 0);
    BOBUIRY_COMPARE(window.mouseReleaseButton, 0);

    QCoreApplication::setAttribute(BobUI::AA_SynthesizeMouseForUnhandledTouchEvents, false);

    window.ignoreTouch = true;
    points[0].state = QEventPoint::State::Pressed;
    points[1].state = QEventPoint::State::Pressed;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    points[0].state = QEventPoint::State::Released;
    points[1].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();

    QCoreApplication::setAttribute(BobUI::AA_SynthesizeMouseForUnhandledTouchEvents, true);

    // mouse event synthesis was disabled when the events were sent above
    BOBUIRY_COMPARE(window.mousePressButton, 0);
    BOBUIRY_COMPARE(window.mouseReleaseButton, 0);

    // but now, mouse event synthesis is enabled
    points.clear();
    points.append(tp2);
    points[0].state = QEventPoint::State::Pressed;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    points.clear();
    points.append(tp1);
    points[0].state = QEventPoint::State::Pressed;
    const int mouseMoveCountWas = window.mouseMovedCount;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.mousePressButton, 1);
    QCOMPARE(window.mouseDevice, touchDevice);
    // tp1's position is different than tp2's, so
    // QGuiApplicationPrivate::processMouseEvent() sent a synth-mouse move
    QCOMPARE(window.mouseMoveDevice, touchDevice);
    QCOMPARE(window.mouseMovedCount, mouseMoveCountWas + 1);

    points.clear();
    points.append(tp2);
    points[0].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    points.clear();
    points.append(tp1);
    points[0].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.mouseReleaseButton, 1);
}

void tst_QWindow::touchToMouseTranslationForDevices()
{
    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.ignoreTouch = true;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QPoint touchPoint(10, 10);

    BOBUIest::touchEvent(&window, touchDevice).press(0, touchPoint, &window);
    BOBUIest::touchEvent(&window, touchDevice).release(0, touchPoint, &window);
    QCoreApplication::processEvents();

    QCOMPARE(window.mousePressedCount, 1);
    QCOMPARE(window.mouseReleasedCount, 1);

    window.resetCounters();

    BOBUIest::touchEvent(&window, touchDeviceWithMouseEmulation).press(0, touchPoint, &window);
    BOBUIest::touchEvent(&window, touchDeviceWithMouseEmulation).release(0, touchPoint, &window);
    QCoreApplication::processEvents();

    QCOMPARE(window.mousePressedCount, 0);
    QCOMPARE(window.mouseReleasedCount, 0);
}

void tst_QWindow::mouseToTouchTranslation()
{
    QCoreApplication::setAttribute(BobUI::AA_SynthesizeTouchForUnhandledMouseEvents, true);

    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.ignoreMouse = true;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    const QPoint localPos(10, 10);
    BOBUIest::mouseClick(&window, BobUI::LeftButton, {}, localPos);
    QCoreApplication::processEvents();

    QCoreApplication::setAttribute(BobUI::AA_SynthesizeTouchForUnhandledMouseEvents, false);

    BOBUIRY_COMPARE(window.touchPressedCount, 1);
    BOBUIRY_COMPARE(window.touchReleasedCount, 1);
    QCOMPARE(window.mouseDevice, window.touchDevice);
    if (isPlatformWayland())
        QEXPECT_FAIL("", "Wayland: This fails. See BOBUIBUG-100887.", Abort);
    QCOMPARE(window.touchDevice->type(), QInputDevice::DeviceType::Mouse);
    QCOMPARE(window.touchPressLocalPos.toPoint(), localPos);
    QCOMPARE(window.touchPressGlobalPos.toPoint(), window.mapToGlobal(localPos));

    QCoreApplication::setAttribute(BobUI::AA_SynthesizeTouchForUnhandledMouseEvents, true);

    window.ignoreMouse = false;

    BOBUIest::mouseClick(&window, BobUI::LeftButton, {}, localPos);
    QCoreApplication::processEvents();

    QCoreApplication::setAttribute(BobUI::AA_SynthesizeTouchForUnhandledMouseEvents, false);

    // no new touch events should be generated since the input window handles the mouse events
    BOBUIRY_COMPARE(window.touchPressedCount, 1);
    BOBUIRY_COMPARE(window.touchReleasedCount, 1);

    window.ignoreMouse = true;

    BOBUIest::mouseClick(&window, BobUI::LeftButton, {}, localPos);
    QCoreApplication::processEvents();

    // touch event synthesis disabled
    BOBUIRY_COMPARE(window.touchPressedCount, 1);
    BOBUIRY_COMPARE(window.touchReleasedCount, 1);
}

void tst_QWindow::mouseToTouchLoop()
{
    // make sure there's no infinite loop when synthesizing both ways
    QCoreApplication::setAttribute(BobUI::AA_SynthesizeTouchForUnhandledMouseEvents, true);
    QCoreApplication::setAttribute(BobUI::AA_SynthesizeMouseForUnhandledTouchEvents, true);

    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));

    window.ignoreMouse = true;
    window.ignoreTouch = true;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    BOBUIest::mouseClick(&window, BobUI::LeftButton, {}, QPoint(10, 10));
    QCoreApplication::processEvents();

    QCoreApplication::setAttribute(BobUI::AA_SynthesizeTouchForUnhandledMouseEvents, false);
    QCoreApplication::setAttribute(BobUI::AA_SynthesizeMouseForUnhandledTouchEvents, true);
}

void tst_QWindow::touchCancel()
{
    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QList<QWindowSystemInterface::TouchPoint> points;
    QWindowSystemInterface::TouchPoint tp1;
    tp1.id = 1;

    // Start a touch.
    tp1.state = QEventPoint::State::Pressed;
    tp1.area = QRect(10, 10, 4, 4);
    points << tp1;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchEventType, QEvent::TouchBegin);
    BOBUIRY_COMPARE(window.touchPressedCount, 1);

    // Cancel the active touch sequence.
    QWindowSystemInterface::handleTouchCancelEvent(&window, touchDevice);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchEventType, QEvent::TouchCancel);

    // Send a move -> will not be delivered due to the cancellation.
    BOBUIRY_COMPARE(window.touchMovedCount, 0);
    points[0].state = QEventPoint::State::Updated;
    tp1.area.adjust(2, 2, 2, 2);
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchMovedCount, 0);

    // Likewise. The only allowed transition is TouchCancel -> TouchBegin.
    BOBUIRY_COMPARE(window.touchReleasedCount, 0);
    points[0].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchReleasedCount, 0);

    // Start a new sequence -> from this point on everything should go through normally.
    points[0].state = QEventPoint::State::Pressed;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchEventType, QEvent::TouchBegin);
    BOBUIRY_COMPARE(window.touchPressedCount, 2);

    points[0].state = QEventPoint::State::Updated;
    tp1.area.adjust(2, 2, 2, 2);
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchMovedCount, 1);

    points[0].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchReleasedCount, 1);
}

void tst_QWindow::touchCancelWithTouchToMouse()
{
    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.ignoreTouch = true;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QList<QWindowSystemInterface::TouchPoint> points;
    QWindowSystemInterface::TouchPoint tp1;
    tp1.id = 1;

    tp1.state = QEventPoint::State::Pressed;
    const QRect area(100, 100, 4, 4);
    tp1.area = QHighDpi::toNativePixels(area, &window);
    points << tp1;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.mousePressButton, int(BobUI::LeftButton));
    const int fuzz = 2 * int(QHighDpiScaling::factor(&window));
    BOBUIRY_VERIFY2(qFuzzyCompareWindowPosition(window.mousePressScreenPos.toPoint(), area.center(), fuzz),
                 qPrintable(msgPointMismatch(window.mousePressScreenPos.toPoint(), area.center())));

    // Cancel the touch. Should result in a mouse release for windows that have
    // have an active touch-to-mouse sequence.
    QWindowSystemInterface::handleTouchCancelEvent(nullptr, touchDevice);
    QCoreApplication::processEvents();

    BOBUIRY_COMPARE(window.mouseReleaseButton, int(BobUI::LeftButton));

    // Now change the window to accept touches.
    window.mousePressButton = window.mouseReleaseButton = 0;
    window.ignoreTouch = false;

    // Send a touch, there will be no mouse event generated.
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.mousePressButton, 0);

    // Cancel the touch. It should not result in a mouse release with this window.
    QWindowSystemInterface::handleTouchCancelEvent(nullptr, touchDevice);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.mouseReleaseButton, 0);
}

void tst_QWindow::touchInterruptedByPopup()
{
    if (isPlatformWayland())
        QSKIP("Wayland: need real user action like a button press, key press, or touch down event.");

    InputTestWindow window;
    window.setObjectName("main");
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QList<QWindowSystemInterface::TouchPoint> points;
    QWindowSystemInterface::TouchPoint tp1;
    tp1.id = 1;

    // Start a touch.
    tp1.state = QEventPoint::State::Pressed;
    tp1.area = QRect(10, 10, 4, 4);
    points << tp1;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchEventType, QEvent::TouchBegin);
    BOBUIRY_COMPARE(window.touchPressedCount, 1);

    // Launch a popup window
    InputTestWindow popup;
    popup.setObjectName("popup");
    popup.setFlags(BobUI::Popup);
    popup.setModality(BobUI::WindowModal);
    popup.resize(m_testWindowSize /  2);
    popup.setTransientParent(&window);
    popup.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&popup));

    // Send a move -> will not be delivered to the original window
    // (TODO verify where it is forwarded, after we've defined that)
    BOBUIRY_COMPARE(window.touchMovedCount, 0);
    points[0].state = QEventPoint::State::Updated;
    tp1.area.adjust(2, 2, 2, 2);
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchMovedCount, 0);

    // Send a touch end -> will not be delivered to the original window
    BOBUIRY_COMPARE(window.touchReleasedCount, 0);
    points[0].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    BOBUIRY_COMPARE(window.touchReleasedCount, 0);
}

void tst_QWindow::orientation()
{
    qRegisterMetaType<BobUI::ScreenOrientation>("BobUI::ScreenOrientation");

    QWindow window;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.create();

    window.reportContentOrientationChange(BobUI::PortraitOrientation);
    QCOMPARE(window.contentOrientation(), BobUI::PortraitOrientation);

    window.reportContentOrientationChange(BobUI::PrimaryOrientation);
    QCOMPARE(window.contentOrientation(), BobUI::PrimaryOrientation);

    QSignalSpy spy(&window, SIGNAL(contentOrientationChanged(BobUI::ScreenOrientation)));
    window.reportContentOrientationChange(BobUI::LandscapeOrientation);
    QCOMPARE(spy.size(), 1);
}

void tst_QWindow::sizes()
{
    QWindow window;

    QSignalSpy minimumWidthSpy(&window, SIGNAL(minimumWidthChanged(int)));
    QSignalSpy minimumHeightSpy(&window, SIGNAL(minimumHeightChanged(int)));
    QSignalSpy maximumWidthSpy(&window, SIGNAL(maximumWidthChanged(int)));
    QSignalSpy maximumHeightSpy(&window, SIGNAL(maximumHeightChanged(int)));

    QSize oldMaximum = window.maximumSize();

    window.setMinimumWidth(10);
    QCOMPARE(window.minimumWidth(), 10);
    QCOMPARE(window.minimumHeight(), 0);
    QCOMPARE(window.minimumSize(), QSize(10, 0));
    QCOMPARE(window.maximumSize(), oldMaximum);
    QCOMPARE(minimumWidthSpy.size(), 1);
    QCOMPARE(minimumHeightSpy.size(), 0);
    QCOMPARE(maximumWidthSpy.size(), 0);
    QCOMPARE(maximumHeightSpy.size(), 0);

    window.setMinimumHeight(10);
    QCOMPARE(window.minimumWidth(), 10);
    QCOMPARE(window.minimumHeight(), 10);
    QCOMPARE(window.minimumSize(), QSize(10, 10));
    QCOMPARE(window.maximumSize(), oldMaximum);
    QCOMPARE(minimumWidthSpy.size(), 1);
    QCOMPARE(minimumHeightSpy.size(), 1);
    QCOMPARE(maximumWidthSpy.size(), 0);
    QCOMPARE(maximumHeightSpy.size(), 0);

    window.setMaximumWidth(100);
    QCOMPARE(window.maximumWidth(), 100);
    QCOMPARE(window.maximumHeight(), oldMaximum.height());
    QCOMPARE(window.minimumSize(), QSize(10, 10));
    QCOMPARE(window.maximumSize(), QSize(100, oldMaximum.height()));
    QCOMPARE(minimumWidthSpy.size(), 1);
    QCOMPARE(minimumHeightSpy.size(), 1);
    QCOMPARE(maximumWidthSpy.size(), 1);
    QCOMPARE(maximumHeightSpy.size(), 0);

    window.setMaximumHeight(100);
    QCOMPARE(window.maximumWidth(), 100);
    QCOMPARE(window.maximumHeight(), 100);
    QCOMPARE(window.minimumSize(), QSize(10, 10));
    QCOMPARE(window.maximumSize(), QSize(100, 100));
    QCOMPARE(minimumWidthSpy.size(), 1);
    QCOMPARE(minimumHeightSpy.size(), 1);
    QCOMPARE(maximumWidthSpy.size(), 1);
    QCOMPARE(maximumHeightSpy.size(), 1);

    // test if min and max limits will change the size
    QVERIFY(window.minimumWidth() < 50 && window.maximumWidth() > 80);
    QVERIFY(window.minimumHeight() < 50 && window.maximumHeight() > 80);
    window.resize(50, 50);
    QCOMPARE(window.size(), QSize(50, 50));
    window.setMinimumSize(QSize(60, 60));
    QCOMPARE(window.size(), QSize(60, 60));
    window.resize(80, 80);
    window.setMaximumSize(QSize(70, 70));
    QCOMPARE(window.size(), QSize(70, 70));

    // BOBUIBUG-113233
    // test for an invalid min/max pair
    window.setMinimumSize(QSize(80, 80)); // current maximumSize = QSize(70, 70)
    QCOMPARE(window.size(), QSize(70, 70));
    window.setMaximumSize(QSize(90, 90));
    QCOMPARE(window.size(), QSize(80, 80));
}

class CloseOnCloseEventWindow : public QWindow
{
public:
    inline static int closeEvents;
    CloseOnCloseEventWindow() { closeEvents = 0; }

protected:
    void closeEvent(QCloseEvent *e) override
    {
        if (++closeEvents > 1)
            return;

        close();
        e->accept();
    }
};

void tst_QWindow::close()
{
    {
        QWindow a;
        QWindow b;
        QWindow c(&a);

        a.show();
        b.show();

        // we can not close a non top level window
        QVERIFY(!c.close());
        QVERIFY(a.close());
        QVERIFY(b.close());
    }

    // Verify that closing a QWindow deletes its platform window,
    // independent of API used to close the window.
    {
        // Close with QWindow::close
        {
            QWindow w;
            w.create();
            QVERIFY(w.handle());
            w.close();
            QVERIFY(!w.handle());
        }

        // Close with QWindowSystemInterface::handleCloseEvent();
        {
            QWindow w;
            w.create();
            QVERIFY(w.handle());
            QWindowSystemInterface::handleCloseEvent(&w);
            QCoreApplication::processEvents();
            QVERIFY(!w.handle());
        }
    }

    // Verify that closing a QWindow deletes the platform window for
    // child windows
    {
        QWindow w;
        QWindow c(&w);
        w.create();
        c.create();
        QVERIFY(w.handle());
        QVERIFY(c.handle());
        w.close();
        QVERIFY(!w.handle());
        QVERIFY(!c.handle());
    }

    // Verify that re-creating closed windows is possble.
    {
        // Re-create top-level window
        {
            QWindow w;
            w.create();
            QVERIFY(w.handle());
            w.close();
            QVERIFY(!w.handle());
            w.create();
            QVERIFY(w.handle());
        }

        // Re-create top-level window with child window
        {
            QWindow w;
            QWindow c(&w);
            c.create();
            QVERIFY(w.handle());
            QVERIFY(c.handle());
            w.close();
            QVERIFY(!w.handle());
            QVERIFY(!c.handle());
            c.create();
            QVERIFY(w.handle());
            QVERIFY(c.handle());
        }
    }

    {
        // A QWidget will call close() from the destructor, and
        // we allow widgets deleting itself in the closeEvent,
        // so we need to guard against close being called recursively.
        CloseOnCloseEventWindow w;
        w.create();
        w.close();
        QCOMPARE(CloseOnCloseEventWindow::closeEvents, 1);
    }
}

void tst_QWindow::activateAndClose()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    for (int i = 0; i < 10; ++i) {
       QWindow window;
       window.setTitle(QLatin1String(BOBUIest::currentTestFunction()) + QString::number(i));
#if defined(Q_OS_QNX)
       window.setSurfaceType(QSurface::OpenGLSurface);
#endif
       // qWaitForWindowActive will block for the duration of
       // of the timeout if the window is at 0,0
       window.setGeometry(QGuiApplication::primaryScreen()->availableGeometry().adjusted(1, 1, -1, -1));
       window.showNormal();
#if defined(Q_OS_QNX) // We either need to create a eglSurface or a create a backing store
                      // and then post the window in order for screen to show the window
       QVERIFY(BOBUIest::qWaitForWindowExposed(&window));
       QOpenGLContext context;
       context.create();
       context.makeCurrent(&window);
       context.swapBuffers(&window);
#endif
       window.requestActivate();
       QVERIFY(BOBUIest::qWaitForWindowActive(&window));
       QCOMPARE(QGuiApplication::focusWindow(), &window);
    }
}

void tst_QWindow::mouseEventSequence()
{
    const auto doubleClickInterval = ulong(QGuiApplication::styleHints()->mouseDoubleClickInterval());

    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    ulong timestamp = 0;
    QPointF local(12, 34);
    const QPointF deviceLocal = QHighDpi::toNativePixels(local, &window);

    simulateMouseClick(&window, timestamp, deviceLocal, deviceLocal);
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressedCount, 1);
    QCOMPARE(window.mouseReleasedCount, 1);
    QCOMPARE(window.mouseDoubleClickedCount, 0);
    QCOMPARE(window.mouseSequenceSignature, QLatin1String("pr"));

    window.resetCounters();
    timestamp += doubleClickInterval;

    // A double click must result in press, release, press, doubleclick, release.
    // Check that no unexpected event suppression occurs and that the order is correct.
    simulateMouseClick(&window, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressedCount, 2);
    QCOMPARE(window.mouseReleasedCount, 2);
    QCOMPARE(window.mouseDoubleClickedCount, 1);
    QCOMPARE(window.mouseSequenceSignature, QLatin1String("prpdr"));

    timestamp += doubleClickInterval;
    window.resetCounters();

    // Triple click = double + single click
    simulateMouseClick(&window, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressedCount, 3);
    QCOMPARE(window.mouseReleasedCount, 3);
    QCOMPARE(window.mouseDoubleClickedCount, 1);
    QCOMPARE(window.mouseSequenceSignature, QLatin1String("prpdrpr"));

    timestamp += doubleClickInterval;
    window.resetCounters();

    // Two double clicks.
    simulateMouseClick(&window, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressedCount, 4);
    QCOMPARE(window.mouseReleasedCount, 4);
    QCOMPARE(window.mouseDoubleClickedCount, 2);
    QCOMPARE(window.mouseSequenceSignature, QLatin1String("prpdrprpdr"));

    timestamp += doubleClickInterval;
    window.resetCounters();

    // Four clicks, none of which qualifies as a double click.
    simulateMouseClick(&window, timestamp, local, local);
    timestamp += doubleClickInterval;
    simulateMouseClick(&window, timestamp, local, local);
    timestamp += doubleClickInterval;
    simulateMouseClick(&window, timestamp, local, local);
    timestamp += doubleClickInterval;
    simulateMouseClick(&window, timestamp, local, local);
    timestamp += doubleClickInterval;
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressedCount, 4);
    QCOMPARE(window.mouseReleasedCount, 4);
    QCOMPARE(window.mouseDoubleClickedCount, 0);
    QCOMPARE(window.mouseSequenceSignature, QLatin1String("prprprpr"));

    // Test double click across windows
    InputTestWindow windowNew;
    windowNew.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    windowNew.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&windowNew));

    timestamp += doubleClickInterval;
    windowNew.resetCounters();
    window.resetCounters();

    simulateMouseClick(&windowNew, timestamp, local, local);
    simulateMouseClick(&window, timestamp, local, local);
    QCoreApplication::processEvents();
    QCOMPARE(windowNew.mousePressedCount, 1);
    QCOMPARE(windowNew.mouseReleasedCount, 1);
    QCOMPARE(windowNew.mouseDoubleClickedCount, 0);
    QCOMPARE(windowNew.mouseSequenceSignature, QLatin1String("pr"));
    QCOMPARE(window.mousePressedCount, 1);
    QCOMPARE(window.mouseReleasedCount, 1);
    QCOMPARE(window.mouseDoubleClickedCount, 0);
    QCOMPARE(window.mouseSequenceSignature, QLatin1String("pr"));
}

void tst_QWindow::windowModality()
{
    qRegisterMetaType<BobUI::WindowModality>("BobUI::WindowModality");

    QWindow window;
    QSignalSpy spy(&window, SIGNAL(modalityChanged(BobUI::WindowModality)));

    QCOMPARE(window.modality(), BobUI::NonModal);
    window.setModality(BobUI::NonModal);
    QCOMPARE(window.modality(), BobUI::NonModal);
    QCOMPARE(spy.size(), 0);

    window.setModality(BobUI::WindowModal);
    QCOMPARE(window.modality(), BobUI::WindowModal);
    QCOMPARE(spy.size(), 1);
    window.setModality(BobUI::WindowModal);
    QCOMPARE(window.modality(), BobUI::WindowModal);
    QCOMPARE(spy.size(), 1);

    window.setModality(BobUI::ApplicationModal);
    QCOMPARE(window.modality(), BobUI::ApplicationModal);
    QCOMPARE(spy.size(), 2);
    window.setModality(BobUI::ApplicationModal);
    QCOMPARE(window.modality(), BobUI::ApplicationModal);
    QCOMPARE(spy.size(), 2);

    window.setModality(BobUI::NonModal);
    QCOMPARE(window.modality(), BobUI::NonModal);
    QCOMPARE(spy.size(), 3);
}

void tst_QWindow::inputReentrancy()
{
    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.spinLoopWhenPressed = true;

    window.setGeometry(QRect(m_availableTopLeft + QPoint(80, 80), m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    // Queue three events.
    QPointF local(12, 34);
    QWindowSystemInterface::handleMouseEvent(&window, local, local, BobUI::LeftButton,
                                             BobUI::LeftButton, QEvent::MouseButtonPress);
    local += QPointF(2, 2);
    QWindowSystemInterface::handleMouseEvent(&window, local, local,
                                             BobUI::LeftButton, {}, QEvent::MouseMove);
    QWindowSystemInterface::handleMouseEvent(&window, local, local, {},
                                             BobUI::LeftButton, QEvent::MouseButtonRelease);
    // Process them. However, the event handler for the press will also call
    // processEvents() so the move and release will be delivered before returning
    // from mousePressEvent(). The point is that no events should get lost.
    QCoreApplication::processEvents();
    QCOMPARE(window.mousePressButton, int(BobUI::LeftButton));
    QCOMPARE(window.mouseReleaseButton, int(BobUI::LeftButton));
    QCOMPARE(window.mousePressedCount, 1);
    // The mouse press may have generated a synthetic move in QGuiApplicationPrivate::processMouseEvent()
    QVERIFY(window.mouseMovedCount == 1 || window.mouseMovedCount == 2);
    QCOMPARE(window.mouseReleasedCount, 1);

    // Now the same for touch.
    QList<QWindowSystemInterface::TouchPoint> points;
    QWindowSystemInterface::TouchPoint tp1;
    tp1.id = 1;
    tp1.state = QEventPoint::State::Pressed;
    tp1.area = QRectF(10, 10, 4, 4);
    points << tp1;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    points[0].state = QEventPoint::State::Updated;
    points[0].area = QRectF(20, 20, 8, 8);
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    points[0].state = QEventPoint::State::Released;
    QWindowSystemInterface::handleTouchEvent(&window, touchDevice, points);
    QCoreApplication::processEvents();
    QCOMPARE(window.touchPressedCount, 1);
    QCOMPARE(window.touchMovedCount, 1);
    QCOMPARE(window.touchReleasedCount, 1);
}

#if BOBUI_CONFIG(tabletevent)
struct PointerEvent {
    QEvent::Type type;
    BobUI::MouseButton button;
    const QPointingDevice *device;
    QPointF local;
    QPointF global;

    bool operator==(const PointerEvent &other) const
    {
        return type == other.type && button == other.button && device == other.device &&
                global == other.global && local == other.local;
    }
};

QDebug operator<< (QDebug d, const PointerEvent& pe)
{
    QDebugStateSaver saver(d);
    d.space() << "PtrEvt {" << pe.type << pe.button << pe.local << pe.global << pe.device << '}';
    return d;
}

class TabletTestWindow : public QWindow
{
public:
    void tabletEvent(BOBUIabletEvent *ev) override
    {
        ev->setAccepted(acceptTabletEvent);
    }

    bool event(QEvent *ev) override
    {
        if (ev->type() == QEvent::MouseButtonDblClick) {
            ++doubleClickCount;
        } else if (ev->isSinglePointEvent()) {
            auto *spe = static_cast<QSinglePointEvent *>(ev);
            singlePointEvents << PointerEvent {
                                 spe->type(), spe->button(), spe->pointingDevice(),
                                 spe->position(), spe->globalPosition() };
        }
        return QWindow::event(ev);
    }

    QList<PointerEvent> singlePointEvents;
    int  doubleClickCount = 0;
    bool acceptTabletEvent = false;

    bool eventFilter(QObject *obj, QEvent *ev) override
    {
        if (ev->type() == QEvent::TabletEnterProximity
                || ev->type() == QEvent::TabletLeaveProximity) {
            BOBUIabletEvent *te = static_cast<BOBUIabletEvent *>(ev);
            singlePointEvents << PointerEvent {
                                 te->type(), te->button(), te->pointingDevice(),
                                 te->position(), te->globalPosition() };
        }
        return QWindow::eventFilter(obj, ev);
    }
};
#endif

void tst_QWindow::tabletEvents_data()
{
    BOBUIest::addColumn<bool>("guiSynthMouse");
    BOBUIest::addColumn<bool>("acceptTabletEvent");

    BOBUIest::newRow("nosynth-accept") << false << true;
    BOBUIest::newRow("synth-accept") << true << true;
    BOBUIest::newRow("nosynth-ignore") << false << false;
    BOBUIest::newRow("synth-ignore") << true << false;;
}

void tst_QWindow::tabletEvents()
{
#if BOBUI_CONFIG(tabletevent)
    QFETCH(bool, guiSynthMouse);
    QFETCH(bool, acceptTabletEvent);
    qApp->setAttribute(BobUI::AA_SynthesizeMouseForUnhandledTabletEvents, guiSynthMouse);

    // the fake USB tablet device is "plugged in"
    QPointingDevice tabletDevice("macow", 0xbeef, QInputDevice::DeviceType::Unknown, QPointingDevice::PointerType::Generic,
                                 QInputDevice::Capability::Position, 1, 0);
    QWindowSystemInterface::registerInputDevice(&tabletDevice);

    TabletTestWindow window;
    window.acceptTabletEvent = acceptTabletEvent;
    window.setGeometry(QRect(m_availableTopLeft + QPoint(10, 10), m_testWindowSize));
    qGuiApp->installEventFilter(&window);
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    const QPoint local(10, 10);
    const QPoint global = window.mapToGlobal(local);
    const QPoint deviceLocal = QHighDpi::toNativeLocalPosition(local, &window);
    const QPoint deviceGlobal = QHighDpi::toNativePixels(global, window.screen());
    ulong timestamp = 1234;

    // The stylus is just now seen for the first time, as it comes into proximity.
    // Its QObject-parent will be the tablet device.
    QPointingDevice tabletStylus("macow stylus eraser", 0xe6a5e6, QInputDevice::DeviceType::Stylus, QPointingDevice::PointerType::Eraser,
                                 QInputDevice::Capability::Position | QInputDevice::Capability::Pressure, 1, 3, QString(),
                                 QPointingDeviceUniqueId::fromNumericId(42), &tabletDevice);
    QWindowSystemInterface::registerInputDevice(&tabletStylus);
    QWindowSystemInterface::handleTabletEnterLeaveProximityEvent(&window, timestamp++, &tabletStylus, true, local, global);
    QCoreApplication::processEvents();
    qCDebug(lcTests) << "expect TabletEnterProximity:" << window.singlePointEvents;
    BOBUIRY_COMPARE_GE(window.singlePointEvents.size(), 1);
    // the positions given to handleTabletEnterLeaveProximityEvent are not currently delivered (BOBUIBUG-111400)
    QCOMPARE(window.singlePointEvents.last(), PointerEvent({ QEvent::TabletEnterProximity, BobUI::NoButton, &tabletStylus, {}, {} }));
    window.singlePointEvents.clear();

    // the eraser is pressed into contact with the tablet surface
    QWindowSystemInterface::handleTabletEvent(&window, timestamp++, &tabletStylus, deviceLocal, deviceGlobal,
                                              BobUI::LeftButton, 0.5, 1, 2, 0.1, 0, 0, {});
    QCoreApplication::processEvents();
    qCDebug(lcTests) << "eraser pressed:" << window.singlePointEvents;
    if (guiSynthMouse && !acceptTabletEvent) {
        // expect synth-mouse events after the ignored tablet event,
        // all from the same stylus device at the same position
        // TODO why does this not work on QNX and some ARM Linux platforms?
        if (!BOBUIest::qWaitFor([&window]{ return window.singlePointEvents.size() == 3; }))
            QSKIP("Failed to receive synth-mouse after tablet event on this platform (BOBUIBUG-129998)");
        QCOMPARE(window.singlePointEvents.size(), 3);
        QCOMPARE(window.singlePointEvents.at(0).type, QEvent::TabletPress);
        QCOMPARE(window.singlePointEvents.at(1).type, QEvent::MouseMove);
        QCOMPARE(window.singlePointEvents.at(2).type, QEvent::MouseButtonPress);
        for (int i = 0; i < window.singlePointEvents.size(); ++i) {
            QCOMPARE(window.singlePointEvents.at(i).device, &tabletStylus);
            QCOMPARE(window.singlePointEvents.at(i).local.toPoint(), local);
            QCOMPARE(window.singlePointEvents.at(i).global.toPoint(), global);
        }
    } else {
        BOBUIRY_COMPARE(window.singlePointEvents.size(), 1);
        QCOMPARE(window.singlePointEvents.first().type, QEvent::TabletPress);
        QCOMPARE(window.singlePointEvents.first().local.toPoint(), local);
        QCOMPARE(window.singlePointEvents.first().global.toPoint(), global);
    }

    // now it's lifted
    QWindowSystemInterface::handleTabletEvent(&window, timestamp++, &tabletStylus, deviceLocal, deviceGlobal,
                                              BobUI::NoButton, 0, 3, 4, 0.11, 2, 1, {});
    QCoreApplication::processEvents();
    qCDebug(lcTests) << "eraser lifted:" << window.singlePointEvents;
    if (guiSynthMouse && !acceptTabletEvent) {
        // expect synth-mouse events after the ignored tablet event,
        // all from the same stylus device at the same position
#ifndef Q_OS_QNX
        BOBUIRY_COMPARE(window.singlePointEvents.size(), 5);
        for (int i = 2; i < window.singlePointEvents.size(); ++i) {
            QCOMPARE(window.singlePointEvents.at(i).device, &tabletStylus);
            QCOMPARE(window.singlePointEvents.at(i).local.toPoint(), local);
            QCOMPARE(window.singlePointEvents.at(i).global.toPoint(), global);
        }
        QCOMPARE(window.singlePointEvents.at(window.singlePointEvents.size() - 2).type, QEvent::TabletRelease);
        QCOMPARE(window.singlePointEvents.at(window.singlePointEvents.size() - 1).type, QEvent::MouseButtonRelease);
#endif
    } else {
        BOBUIRY_COMPARE(window.singlePointEvents.size(), 2);
        QCOMPARE(window.singlePointEvents.at(1).type, QEvent::TabletRelease);
        QCOMPARE(window.singlePointEvents.at(1).local.toPoint(), local);
        QCOMPARE(window.singlePointEvents.at(1).global.toPoint(), global);
    }
    QCOMPARE(window.doubleClickCount, 0);

    // and is taken away (goes out of proxmity)
    QWindowSystemInterface::handleTabletEnterLeaveProximityEvent(&window, timestamp, &tabletStylus, false, local, global);
    QCoreApplication::processEvents();
    // the positions given to handleTabletEnterLeaveProximityEvent are not currently delivered (BOBUIBUG-111400)
    BOBUIRY_COMPARE(window.singlePointEvents.last(), PointerEvent({ QEvent::TabletLeaveProximity, BobUI::NoButton, &tabletStylus, {}, {} }));
    QCOMPARE(window.doubleClickCount, 0);
#endif
}

void tst_QWindow::windowModality_BOBUIBUG27039()
{
    QWindow parent;
    parent.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    parent.setGeometry(QRect(m_availableTopLeft + QPoint(10, 10), m_testWindowSize));
    parent.show();

    InputTestWindow modalA;
    modalA.setTransientParent(&parent);
    modalA.setGeometry(QRect(m_availableTopLeft + QPoint(20, 10), m_testWindowSize));
    modalA.setModality(BobUI::ApplicationModal);
    modalA.show();

    InputTestWindow modalB;
    modalB.setTransientParent(&parent);
    modalA.setGeometry(QRect(m_availableTopLeft + QPoint(30, 10), m_testWindowSize));
    modalB.setModality(BobUI::ApplicationModal);
    modalB.show();

    QPoint local(5, 5);
    BOBUIest::mouseClick(&modalA, BobUI::LeftButton, {}, local);
    BOBUIest::mouseClick(&modalB, BobUI::LeftButton, {}, local);
    QCoreApplication::processEvents();

    // modal A should be blocked since it was shown first, but modal B should not be blocked
    QCOMPARE(modalB.mousePressedCount, 1);
    QCOMPARE(modalA.mousePressedCount, 0);

    modalB.hide();
    BOBUIest::mouseClick(&modalA, BobUI::LeftButton, {}, local);
    QCoreApplication::processEvents();

    // modal B has been hidden, modal A should be unblocked again
    QCOMPARE(modalA.mousePressedCount, 1);
}

void tst_QWindow::visibility()
{
    qRegisterMetaType<BobUI::WindowModality>("QWindow::Visibility");

    Window window;
    QSignalSpy spy(&window, SIGNAL(visibilityChanged(QWindow::Visibility)));

    window.setVisibility(QWindow::AutomaticVisibility);
    QVERIFY(window.isVisible());
    QVERIFY(window.visibility() != QWindow::Hidden);
    QVERIFY(window.visibility() != QWindow::AutomaticVisibility);
    QCOMPARE(spy.size(), 1);
    spy.clear();

    window.setVisibility(QWindow::Hidden);
    QVERIFY(!window.isVisible());
    QCOMPARE(window.visibility(), QWindow::Hidden);
    QCOMPARE(spy.size(), 1);
    spy.clear();

    window.setVisibility(QWindow::FullScreen);
    QVERIFY(window.isVisible());
    QCOMPARE(window.windowState(), BobUI::WindowFullScreen);
    QCOMPARE(window.visibility(), QWindow::FullScreen);
    QCOMPARE(spy.size(), 1);
    BOBUIRY_COMPARE(window.lastReceivedWindowState, BobUI::WindowFullScreen);
    spy.clear();

    window.setWindowState(BobUI::WindowNoState);
    QCOMPARE(window.visibility(), QWindow::Windowed);
    QCOMPARE(spy.size(), 1);
    BOBUIRY_COMPARE(window.lastReceivedWindowState, BobUI::WindowNoState);
    spy.clear();

    window.setVisible(false);
    QCOMPARE(window.visibility(), QWindow::Hidden);
    QCOMPARE(spy.size(), 1);
    spy.clear();
}

void tst_QWindow::mask()
{
    QRegion mask = QRect(10, 10, 800 - 20, 600 - 20);

    {
        QWindow window;
        window.resize(800, 600);
        QCOMPARE(window.mask(), QRegion());

        window.create();
        window.setMask(mask);
        QCOMPARE(window.mask(), mask);
    }

    {
        QWindow window;
        window.resize(800, 600);
        QCOMPARE(window.mask(), QRegion());

        window.setMask(mask);
        QCOMPARE(window.mask(), mask);
        window.create();
        QCOMPARE(window.mask(), mask);
    }

}

void tst_QWindow::initialSize()
{
    QSize defaultSize(0,0);
    {
    Window w;
    w.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    w.showNormal();
    BOBUIRY_VERIFY(w.width() > 0);
    BOBUIRY_VERIFY(w.height() > 0);
    defaultSize = QSize(w.width(), w.height());
    }
    {
    Window w;
    w.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    w.setWidth(m_testWindowSize.width());
    w.showNormal();

    if (isPlatformEglFS())
        QEXPECT_FAIL("", "eglfs windows are fullscreen by default.", Continue);

    BOBUIRY_COMPARE(w.width(), m_testWindowSize.width());
    BOBUIRY_VERIFY(w.height() > 0);
    }
    {
    Window w;
    w.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    const QSize testSize(m_testWindowSize.width(), 42);
    w.resize(testSize);
    w.showNormal();

    const QSize expectedSize = testSize;
    if (isPlatformEglFS())
        QEXPECT_FAIL("", "eglfs windows are fullscreen by default.", Continue);
    BOBUIRY_COMPARE(w.size(), expectedSize);
    }
}

static bool isPlatformOffscreenOrMinimal()
{
    return ((QGuiApplication::platformName() == QLatin1String("offscreen"))
             || (QGuiApplication::platformName() == QLatin1String("minimal")));
}

void tst_QWindow::modalDialog()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    if (QGuiApplication::platformName() == QLatin1String("cocoa"))
        QSKIP("Test fails due to BOBUIBUG-61965, and is slow due to BOBUIBUG-61964");

    QWindow normalWindow;
    normalWindow.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    normalWindow.setFramePosition(m_availableTopLeft + QPoint(80, 80));
    normalWindow.resize(m_testWindowSize);
    normalWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&normalWindow));

    QWindow dialog;
    dialog.setFramePosition(m_availableTopLeft + QPoint(200, 200));
    dialog.resize(m_testWindowSize);
    dialog.setModality(BobUI::ApplicationModal);
    dialog.setFlags(BobUI::Dialog);
    dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));

    normalWindow.requestActivate();

    QGuiApplication::sync();
    QGuiApplication::processEvents();

    if (isPlatformOffscreenOrMinimal()) {
        qWarning("Focus stays in normalWindow on offscreen/minimal platforms");
        BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &normalWindow);
        return;
    }

    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &dialog);
}

void tst_QWindow::modalDialogClosingOneOfTwoModal()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    QWindow normalWindow;
    normalWindow.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    normalWindow.setFramePosition(m_availableTopLeft + QPoint(80, 80));
    normalWindow.resize(m_testWindowSize);
    normalWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&normalWindow));

    QWindow first_dialog;
    first_dialog.setFramePosition(m_availableTopLeft + QPoint(200, 200));
    first_dialog.resize(m_testWindowSize);
    first_dialog.setModality(BobUI::ApplicationModal);
    first_dialog.setFlags(BobUI::Dialog);
    first_dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&first_dialog));

    {
        QWindow second_dialog;
        second_dialog.setFramePosition(m_availableTopLeft + QPoint(300, 300));
        second_dialog.resize(m_testWindowSize);
        second_dialog.setModality(BobUI::ApplicationModal);
        second_dialog.setFlags(BobUI::Dialog);
        second_dialog.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&second_dialog));

        BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &second_dialog);

        second_dialog.close();
    }

    QGuiApplication::sync();
    QGuiApplication::processEvents();

    if (isPlatformOffscreenOrMinimal()) {
        qWarning("Focus is lost when closing modal dialog on offscreen/minimal platforms");
        BOBUIRY_COMPARE(QGuiApplication::focusWindow(), nullptr);
        return;
    }

    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &first_dialog);
}

void tst_QWindow::modalWithChildWindow()
{
    if (isPlatformWayland())
        QSKIP("A nested window or a subsurface in wayland terms can't get focus.");

    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    QWindow normalWindow;
    normalWindow.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    normalWindow.setFramePosition(m_availableTopLeft + QPoint(80, 80));
    normalWindow.resize(m_testWindowSize);
    normalWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&normalWindow));

    QWindow tlw_dialog;
    tlw_dialog.setFramePosition(m_availableTopLeft + QPoint(200, 200));
    tlw_dialog.resize(m_testWindowSize);
    tlw_dialog.setModality(BobUI::ApplicationModal);
    tlw_dialog.setFlags(BobUI::Dialog);
    tlw_dialog.create();

    QWindow sub_window(&tlw_dialog);
    sub_window.resize(200,300);
    sub_window.show();

    tlw_dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tlw_dialog));
    QVERIFY(BOBUIest::qWaitForWindowExposed(&sub_window));

    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &tlw_dialog);

    sub_window.requestActivate();
    QGuiApplication::sync();
    QGuiApplication::processEvents();
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &sub_window);
}

void tst_QWindow::modalWindowModallity()
{
    if (isPlatformWayland() && qgetenv("XDG_CURRENT_DESKTOP").toLower().contains("ubuntu:gnome"))
        QSKIP("Wayland: This will trigger a 'X is ready' system notification in GNOME.");
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    QWindow normal_window;
    normal_window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    normal_window.setFramePosition(m_availableTopLeft + QPoint(80, 80));
    normal_window.resize(m_testWindowSize);
    normal_window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&normal_window));

    QWindow parent_to_modal;
    parent_to_modal.setFramePosition(normal_window.geometry().topRight() + QPoint(100, 0));
    parent_to_modal.resize(m_testWindowSize);
    parent_to_modal.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&parent_to_modal));
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &parent_to_modal);

    QWindow modal_dialog;
    modal_dialog.resize(m_testWindowSize);
    modal_dialog.setFramePosition(normal_window.geometry().bottomLeft() + QPoint(0, 100));
    modal_dialog.setModality(BobUI::WindowModal);
    modal_dialog.setFlags(BobUI::Dialog);
    modal_dialog.setTransientParent(&parent_to_modal);
    modal_dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&modal_dialog));
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &modal_dialog);

    normal_window.requestActivate();
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), &normal_window);

}

void tst_QWindow::modalWindowPosition()
{
    if (isPlatformWayland())
        QSKIP("Window position not queryable on Wayland");

    QWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(QRect(m_availableTopLeft + QPoint(100, 100), m_testWindowSize));
    // Allow for any potential resizing due to constraints
    QRect origGeo = window.geometry();
    window.setModality(BobUI::WindowModal);
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    if (isPlatformEglFS())
        QEXPECT_FAIL("", "eglfs windows are fullscreen by default.", Continue);

    QCOMPARE(window.geometry(), origGeo);
}

void tst_QWindow::modalCloseWhileBlocked()
{
    QWindow first;
    first.setModality(BobUI::ApplicationModal);
    first.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&first));

    QWindow second;
    second.setModality(BobUI::ApplicationModal);
    second.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&first));

    first.close();
    BOBUIRY_VERIFY(!first.isVisible());
}

#ifndef BOBUI_NO_CURSOR
void tst_QWindow::modalWindowEnterEventOnHide_BOBUIBUG35109()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    if (isPlatformOffscreenOrMinimal())
        QSKIP("Can't test window focusing on offscreen/minimal");

    if (isPlatformEglFS() || isPlatformWayland())
        QSKIP("QCursor::setPos() is not supported on this platform");

    const QPoint center = QGuiApplication::primaryScreen()->availableGeometry().center();

    const int childOffset = 16;
    const QPoint rootPos = center - QPoint(m_testWindowSize.width(),
                                           m_testWindowSize.height())/2;
    const QPoint modalPos = rootPos + QPoint(childOffset * 5,
                                             childOffset * 5);
    const QPoint cursorPos = rootPos - QPoint(80, 80);

    // Test whether tlw can receive the enter event
    {
        QCursor::setPos(cursorPos);
        QCoreApplication::processEvents();

        InputTestWindow root;
        root.setTitle(__FUNCTION__);
        root.setGeometry(QRect(rootPos, m_testWindowSize));
        root.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&root));
        root.requestActivate();
        QVERIFY(BOBUIest::qWaitForWindowActive(&root));

        // Move the mouse over the root window, but not over the modal window.
        QCursor::setPos(rootPos + QPoint(childOffset * 5 / 2,
                                         childOffset * 5 / 2));

        // Wait for the enter event. It must be delivered here, otherwise second
        // compare can PASS because of this event even after "resetCounters()".
        BOBUIRY_COMPARE(root.enterEventCount, 1);
        BOBUIRY_COMPARE(root.leaveEventCount, 0);

        QWindow modal;
        modal.setTitle(QLatin1String("Modal - ") + __FUNCTION__);
        modal.setTransientParent(&root);
        modal.resize(m_testWindowSize/2);
        modal.setFramePosition(modalPos);
        modal.setModality(BobUI::ApplicationModal);
        modal.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&modal));
        QVERIFY(BOBUIest::qWaitForWindowActive(&modal));

        QCoreApplication::processEvents();
        BOBUIRY_COMPARE(root.leaveEventCount, 1);

        root.resetCounters();
        modal.close();

        // Check for the enter event
        BOBUIRY_COMPARE(root.enterEventCount, 1);
    }

    // Test whether child window can receive the enter event
    {
        QCursor::setPos(cursorPos);
        QCoreApplication::processEvents();

        QWindow root;
        root.setTitle(__FUNCTION__);
        root.setGeometry(QRect(rootPos, m_testWindowSize));

        QWindow childLvl1;
        childLvl1.setParent(&root);
        childLvl1.setGeometry(childOffset,
                              childOffset,
                              m_testWindowSize.width() - childOffset,
                              m_testWindowSize.height() - childOffset);

        InputTestWindow childLvl2;
        childLvl2.setParent(&childLvl1);
        childLvl2.setGeometry(childOffset,
                              childOffset,
                              childLvl1.width() - childOffset,
                              childLvl1.height() - childOffset);

        root.show();
        childLvl1.show();
        childLvl2.show();

        QVERIFY(BOBUIest::qWaitForWindowExposed(&root));
        root.requestActivate();
        QVERIFY(BOBUIest::qWaitForWindowActive(&root));
        QVERIFY(childLvl1.isVisible());
        QVERIFY(childLvl2.isVisible());

        // Move the mouse over the child window, but not over the modal window.
        // Be sure that the value is almost left-top of second child window for
        // checking proper position mapping.
        QCursor::setPos(rootPos + QPoint(childOffset * 5 / 2,
                                         childOffset * 5 / 2));

        // Wait for the enter event. It must be delivered here, otherwise second
        // compare can PASS because of this event even after "resetCounters()".
        BOBUIRY_COMPARE(childLvl2.enterEventCount, 1);
        BOBUIRY_COMPARE(childLvl2.leaveEventCount, 0);

        QWindow modal;
        modal.setTitle(QLatin1String("Modal - ") + __FUNCTION__);
        modal.setTransientParent(&root);
        modal.resize(m_testWindowSize/2);
        modal.setFramePosition(modalPos);
        modal.setModality(BobUI::ApplicationModal);
        modal.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&modal));
        QVERIFY(BOBUIest::qWaitForWindowActive(&modal));

        QCoreApplication::processEvents();
        BOBUIRY_COMPARE(childLvl2.leaveEventCount, 1);

        childLvl2.resetCounters();
        modal.close();

        // Check for the enter event
        BOBUIRY_COMPARE(childLvl2.enterEventCount, 1);
    }

    // Test whether tlw can receive the enter event if mouse is over the invisible child windnow
    {
        QCursor::setPos(cursorPos);
        QCoreApplication::processEvents();

        InputTestWindow root;
        root.setTitle(__FUNCTION__);
        root.setGeometry(QRect(rootPos, m_testWindowSize));

        QWindow child;
        child.setParent(&root);
        child.setGeometry(QRect(QPoint(), m_testWindowSize));

        root.show();

        QVERIFY(BOBUIest::qWaitForWindowExposed(&root));
        QVERIFY(BOBUIest::qWaitForWindowActive(&root));
        QVERIFY(!child.isVisible());

        // Move the mouse over the child window, but not over the modal window.
        QCursor::setPos(rootPos + QPoint(childOffset * 5 / 2,
                                         childOffset * 5 / 2));

        // Wait for the enter event. It must be delivered here, otherwise second
        // compare can PASS because of this event even after "resetCounters()".
        BOBUIRY_COMPARE(root.enterEventCount, 1);
        BOBUIRY_COMPARE(root.leaveEventCount, 0);

        QWindow modal;
        modal.setTitle(QLatin1String("Modal - ") + __FUNCTION__);
        modal.setTransientParent(&root);
        modal.resize(m_testWindowSize/2);
        modal.setFramePosition(modalPos);
        modal.setModality(BobUI::ApplicationModal);
        modal.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&modal));
        QVERIFY(BOBUIest::qWaitForWindowActive(&modal));

        QCoreApplication::processEvents();
        BOBUIRY_COMPARE(root.leaveEventCount, 1);

        root.resetCounters();
        modal.close();

        // Check for the enter event
        BOBUIRY_COMPARE(root.enterEventCount, 1);
    }
}

// Verify that no spurious mouse move events are received. On Windows, there is
// no enter event, the OS sends mouse move events instead. Test that the QPA
// plugin properly suppresses those since they can interfere with tests.
// Simulate a main window setup with a modal dialog on top, keep the cursor
// in the center and check that no mouse events are recorded.
void tst_QWindow::spuriousMouseMove()
{
    const QString &platformName = QGuiApplication::platformName();
    if (platformName == QLatin1String("offscreen") || platformName == QLatin1String("cocoa"))
        QSKIP("No enter events sent");
    if (platformName == QLatin1String("wayland"))
        QSKIP("Setting mouse cursor position is not possible on Wayland");
    if (isPlatformEglFS())
        QSKIP("QCursor::setPos() is not supported on this platform");
    const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    const QPoint center = screenGeometry.center();
    QCursor::setPos(center);
    QRect windowGeometry(QPoint(), 2 * m_testWindowSize);
    windowGeometry.moveCenter(center);
    BOBUIRY_COMPARE(QCursor::pos(), center);
    InputTestWindow topLevel;
    topLevel.setTitle(BOBUIest::currentTestFunction());
    topLevel.setGeometry(windowGeometry);
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));
    BOBUIRY_VERIFY(topLevel.enterEventCount > 0);
    InputTestWindow dialog(BobUI::yellow);
    dialog.setTransientParent(&topLevel);
    dialog.setTitle("Dialog " + topLevel.title());
    dialog.setModality(BobUI::ApplicationModal);
    windowGeometry.setSize(m_testWindowSize);
    windowGeometry.moveCenter(center);
    dialog.setGeometry(windowGeometry);
    dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));
    BOBUIRY_VERIFY(dialog.enterEventCount > 0);
    dialog.setVisible(false);
    QCOMPARE(dialog.mousePressedCount, 0);
    QCOMPARE(dialog.mouseReleasedCount, 0);
    QCOMPARE(dialog.mouseMovedCount, 0);
    QCOMPARE(dialog.mouseDoubleClickedCount, 0);
    topLevel.setVisible(false);
    QCOMPARE(topLevel.mousePressedCount, 0);
    QCOMPARE(topLevel.mouseReleasedCount, 0);
    QCOMPARE(topLevel.mouseMovedCount, 0);
    QCOMPARE(topLevel.mouseDoubleClickedCount, 0);
}
#endif // !BOBUI_NO_CURSOR

static bool isNativeWindowVisible(const QWindow *window)
{
#if defined(Q_OS_WIN)
    return IsWindowVisible(reinterpret_cast<HWND>(window->winId()));
#else
    Q_UNIMPLEMENTED();
    return window->isVisible();
#endif
}

void tst_QWindow::windowsTransientChildren()
{
    if (QGuiApplication::platformName().compare(QStringLiteral("windows"), BobUI::CaseInsensitive))
        QSKIP("Windows only test");

    ColoredWindow mainWindow(BobUI::yellow);
    mainWindow.setGeometry(QRect(m_availableTopLeft + QPoint(100, 100), m_testWindowSize));
    mainWindow.setTitle(QStringLiteral("Main"));
    ColoredWindow child(BobUI::blue, &mainWindow);
    child.setGeometry(QRect(QPoint(0, 0), m_testWindowSize / 2));

    ColoredWindow dialog(BobUI::red);
    dialog.setGeometry(QRect(m_availableTopLeft + QPoint(200, 200), m_testWindowSize));
    dialog.setTitle(QStringLiteral("Dialog"));
    dialog.setTransientParent(&mainWindow);

    mainWindow.show();
    child.show();
    dialog.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));
    mainWindow.setWindowState(BobUI::WindowMinimized);
    QVERIFY(!isNativeWindowVisible(&dialog));
    dialog.hide();
    mainWindow.setWindowState(BobUI::WindowNoState);
    // BOBUIBUG-40696, transient children hidden by BobUI should not be re-shown by Windows.
    QVERIFY(!isNativeWindowVisible(&dialog));
    QVERIFY(isNativeWindowVisible(&child)); // Real children should be visible.
}

void tst_QWindow::requestUpdate()
{
    QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize);

    Window window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(geometry);
    window.show();
    QCoreApplication::processEvents();
    BOBUIRY_VERIFY(window.isExposed());

    QCOMPARE(window.received(QEvent::UpdateRequest), 0);

    window.requestUpdate();
    BOBUIRY_COMPARE(window.received(QEvent::UpdateRequest), 1);

    window.requestUpdate();
    BOBUIRY_COMPARE(window.received(QEvent::UpdateRequest), 2);
}

void tst_QWindow::flags()
{
    Window window;
    QSignalSpy spy(&window, SIGNAL(flagsChanged(BobUI::WindowFlags)));

    const auto baseFlags = window.flags();
    window.setFlags(window.flags() | BobUI::FramelessWindowHint);
    QCOMPARE(window.flags(), baseFlags | BobUI::FramelessWindowHint);
    QCOMPARE(spy.size(), 1);
    window.setFlags(window.flags());
    QCOMPARE(spy.size(), 1);

    window.setFlag(BobUI::WindowStaysOnTopHint, true);
    QCOMPARE(window.flags(), baseFlags | BobUI::FramelessWindowHint | BobUI::WindowStaysOnTopHint);
    QCOMPARE(spy.size(), 2);
    window.setFlags(window.flags());
    QCOMPARE(spy.size(), 2);

    window.setFlag(BobUI::FramelessWindowHint, false);
    QCOMPARE(window.flags(), baseFlags | BobUI::WindowStaysOnTopHint);
    QCOMPARE(spy.size(), 3);
    window.setFlags(window.flags());
    QCOMPARE(spy.size(), 3);
}

class EventWindow : public QWindow
{
public:
    bool gotBlocked = false;

protected:
    bool event(QEvent *e) override
    {
        if (e->type() == QEvent::WindowBlocked)
            gotBlocked = true;
        return QWindow::event(e);
    }
};

void tst_QWindow::testBlockingWindowShownAfterModalDialog()
{
    EventWindow normalWindow;
    normalWindow.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    normalWindow.setFramePosition(m_availableTopLeft + QPoint(80, 80));
    normalWindow.resize(m_testWindowSize);
    normalWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&normalWindow));
    QVERIFY(!normalWindow.gotBlocked);

    QWindow dialog;
    dialog.setFramePosition(m_availableTopLeft + QPoint(200, 200));
    dialog.resize(m_testWindowSize);
    dialog.setModality(BobUI::ApplicationModal);
    dialog.setFlags(BobUI::Dialog);
    dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));
    QVERIFY(normalWindow.gotBlocked);

    EventWindow normalWindowAfter;
    normalWindowAfter.setFramePosition(m_availableTopLeft + QPoint(80, 80));
    normalWindowAfter.resize(m_testWindowSize);
    QVERIFY(!normalWindowAfter.gotBlocked);
    normalWindowAfter.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&normalWindowAfter));
    QVERIFY(normalWindowAfter.gotBlocked);
}

void tst_QWindow::generatedMouseMove()
{
    InputTestWindow w;
    w.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    w.setGeometry(QRect(m_availableTopLeft + QPoint(100, 100), m_testWindowSize));
    w.setFlags(w.flags() | BobUI::FramelessWindowHint); // ### FIXME: BOBUIBUG-63542
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
    QPoint point(10, 10);
    QPoint step(2, 2);

    QVERIFY(w.mouseMovedCount == 0);
    BOBUIest::mouseMove(&w, point);
    QVERIFY(w.mouseMovedCount == 1);
    // A press event that does not change position should not generate mouse move
    BOBUIest::mousePress(&w, BobUI::LeftButton, BobUI::KeyboardModifiers(), point);
    BOBUIest::mousePress(&w, BobUI::RightButton, BobUI::KeyboardModifiers(), point);

    QVERIFY(w.mouseMovedCount == 1);

    // Verify that a move event is generated for a mouse release event that changes position
    point += step;
    BOBUIest::mouseRelease(&w, BobUI::LeftButton,BobUI::KeyboardModifiers(), point);
    QVERIFY(w.mouseMovedCount == 2);
    QVERIFY(w.buttonStateInGeneratedMove == (BobUI::LeftButton | BobUI::RightButton));
    point += step;
    BOBUIest::mouseRelease(&w, BobUI::RightButton, BobUI::KeyboardModifiers(), point);
    QVERIFY(w.mouseMovedCount == 3);
    QVERIFY(w.buttonStateInGeneratedMove == BobUI::RightButton);

    // Verify that a move event is generated for a mouse press event that changes position
    point += step;
    BOBUIest::mousePress(&w, BobUI::LeftButton, BobUI::KeyboardModifiers(), point);
    QVERIFY(w.mouseMovedCount == 4);
    QVERIFY(w.buttonStateInGeneratedMove == BobUI::NoButton);
    point += step;
    BOBUIest::mousePress(&w, BobUI::RightButton, BobUI::KeyboardModifiers(), point);
    QVERIFY(w.mouseMovedCount == 5);
    QVERIFY(w.buttonStateInGeneratedMove == BobUI::LeftButton);

    // A release event that does not change position should not generate mouse move
    BOBUIest::mouseRelease(&w, BobUI::RightButton, BobUI::KeyboardModifiers(), point);
    BOBUIest::mouseRelease(&w, BobUI::LeftButton, BobUI::KeyboardModifiers(), point);
    QVERIFY(w.mouseMovedCount == 5);
}

void tst_QWindow::keepPendingUpdateRequests()
{
    QRect geometry(m_availableTopLeft + QPoint(80, 80), m_testWindowSize);

    Window window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.setGeometry(geometry);
    window.show();
    QCoreApplication::processEvents();
    BOBUIRY_VERIFY(window.isExposed());

    window.requestUpdate();
    window.close();
    window.setVisible(true);

    QPlatformWindow *platformWindow = window.handle();
    QVERIFY(platformWindow);

    QVERIFY(platformWindow->hasPendingUpdateRequest());
    BOBUIRY_VERIFY(!platformWindow->hasPendingUpdateRequest());
}

void tst_QWindow::activateDeactivateEvent()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    class Window : public QWindow
    {
    public:
        using QWindow::QWindow;

        int activateCount = 0;
        int deactivateCount = 0;
    protected:
        bool event(QEvent *e) override
        {
            switch (e->type()) {
            case QEvent::WindowActivate:
                ++activateCount;
                break;
            case QEvent::WindowDeactivate:
                ++deactivateCount;
                break;
            default:
                break;
            }
            return QWindow::event(e);
        }
    };

    Window w1;
    Window w2;

    w1.show();
    w1.requestActivate();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w1));
    QCOMPARE(w1.activateCount, 1);
    QCOMPARE(w1.deactivateCount, 0);

    w2.show();
    w2.requestActivate();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w2));
    QCOMPARE(w1.deactivateCount, 1);
    QCOMPARE(w2.activateCount, 1);
}

// Test that in a slot connected to destroyed() the emitter is
// is no longer a QWindow.
void tst_QWindow::qobject_castOnDestruction()
{
    QWindow window;
    QObject::connect(&window, &QObject::destroyed, [](QObject *object)
    {
        QVERIFY(!qobject_cast<QWindow *>(object));
        QVERIFY(!dynamic_cast<QWindow *>(object));
        QVERIFY(!object->isWindowType());
    });
}

void tst_QWindow::touchToMouseTranslationByPopup()
{
    if (isPlatformWayland())
        QSKIP("Wayland: need real user action like a button press, key press, or touch down event.");

    InputTestWindow window;
    window.setTitle(QLatin1String(BOBUIest::currentTestFunction()));
    window.ignoreTouch = true;
    window.setGeometry(QRect(m_availableTopLeft, m_testWindowSize));
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    InputTestWindow popupWindow;
    popupWindow.setGeometry(QRect(m_availableTopLeft + QPoint(20, 20),
                                  QSize(m_testWindowSize.width(), m_testWindowSize.height() / 2)));
    popupWindow.setFlag(BobUI::Popup);
    popupWindow.setTransientParent(&window);
    popupWindow.ignoreTouch = true;
    popupWindow.closeOnTap = true;
    popupWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&popupWindow));

    BOBUIest::touchEvent(&popupWindow, touchDevice).press(0, {1, 1}, &window);
    QVERIFY(!popupWindow.isVisible());

    // Omit touchpoint 0: because the popup was closed, touchpoint0.release is not sent.
    const QPoint tp1(50, 1);
    BOBUIest::touchEvent(&window, touchDevice).press(1, tp1, &window);
    BOBUIRY_COMPARE(window.mousePressButton, int(BobUI::LeftButton));
    BOBUIest::touchEvent(&window, touchDevice).release(1, tp1, &window);
    BOBUIRY_COMPARE(window.mouseReleaseButton, int(BobUI::LeftButton));
}

// Test that windowStateChanged is not emitted on noop change (BOBUIBUG-102478)
void tst_QWindow::stateChangeSignal()
{
    // Test only for Windows, Linux and macOS
#if !defined(Q_OS_LINUX) && !defined(Q_OS_WINDOWS) && !defined(Q_OS_DARWIN)
    QSKIP("Singular windowStateChanged signal emission is guaranteed for Linux, Windows and macOS only.\n"
          "On other operating systems, the signal may be emitted twice.");
#endif
    QWindow w;
    Q_ASSERT(connect (&w, &QWindow::windowStateChanged, [](BobUI::WindowState s){qCDebug(lcTests) << "State change to" << s;}));
    QSignalSpy spy(&w, SIGNAL(windowStateChanged(BobUI::WindowState)));
    unsigned short signalCount = 0;
    QList<BobUI::WindowState> effectiveStates;
    Q_ASSERT(connect(&w, &QWindow::windowStateChanged, [&effectiveStates](BobUI::WindowState state)
            { effectiveStates.append(state); }));
    // Part 1:
    // => test signal emission on programmatic state changes
    QCOMPARE(w.windowState(), BobUI::WindowNoState);
    // - wait for target state to be set
    // - wait for signal spy to have reached target count
    // - extract state from signal and compare to target
#define CHECK_STATE(State)\
    QVERIFY(BOBUIest::qWaitFor([&w](){return (w.windowState() == State); }));\
    CHECK_SIGNAL(State)
#define CHECK_SIGNAL(State)\
    BOBUIRY_COMPARE(spy.count(), signalCount);\
    if (signalCount > 0) {\
        QVariantList list = spy.at(signalCount - 1).toList();\
        QCOMPARE(list.count(), 1);\
        bool ok;\
        const int stateInt = list.at(0).toInt(&ok);\
        QVERIFY(ok);\
        const BobUI::WindowState newState = static_cast<BobUI::WindowState>(stateInt);\
        QCOMPARE(newState, State);\
    }
    // Check initialization
    CHECK_STATE(BobUI::WindowNoState);
    // showMaximized after init
    // expected behavior: signal emitted once with state == WindowMaximized
    ++signalCount;
    w.showMaximized();
    CHECK_STATE(BobUI::WindowMaximized);
    // setWindowState to normal
    // expected behavior: signal emitted once with state == WindowNoState
    ++signalCount;
    w.setWindowState(BobUI::WindowNoState);
    CHECK_STATE(BobUI::WindowNoState);
    // redundant setWindowState to normal - except windows, where the no-op is counted
    // expected behavior: No emits.
    // On Windows, a no-op state change causes a no-op resize and repaint, leading to a
    // no-op state change and singal emission.
#ifdef Q_OS_WINDOWS
    ++signalCount;
    ++signalCount;
#endif
    w.setWindowState(BobUI::WindowNoState);
    CHECK_STATE(BobUI::WindowNoState);
    // setWindowState to minimized
    // expected behavior: signal emitted once with state == WindowMinimized
    ++signalCount;
    w.showMinimized();
    CHECK_STATE(BobUI::WindowMinimized);
    // setWindowState to Normal
    // expected behavior: signal emitted once with state == WindowNoState
    ++signalCount;
    w.showNormal();
    CHECK_STATE(BobUI::WindowNoState);
    /*
     - Testcase showFullScreen is omitted: Depending on window manager,
     WindowFullScreen can be mapped to WindowMaximized
     - Transition from WindowMinimized to WindowMaximized is omitted:
     WindowNoState to WindowMaximized
     */
    // Part 2:
    // => test signal emission on simulated user interaction
    // To test the code path, inject state change events into the QPA event queue.
    // Test the signal emission only, not the window's actual visible state.

    // Flush pending events and clear
    QCoreApplication::processEvents();
    spy.clear();
    effectiveStates.clear();
    signalCount = 0;
    // Maximize window
    QWindowSystemInterface::handleWindowStateChanged(&w, BobUI::WindowMaximized, w.windowState());
    ++signalCount;
    CHECK_SIGNAL(BobUI::WindowMaximized);
    // Normalize window
    QWindowSystemInterface::handleWindowStateChanged(&w, BobUI::WindowNoState, w.windowState());
    ++signalCount;
    CHECK_SIGNAL(BobUI::WindowNoState);
    // Minimize window
    QWindowSystemInterface::handleWindowStateChanged(&w, BobUI::WindowMinimized, w.windowState());
    ++signalCount;
    CHECK_SIGNAL(BobUI::WindowMinimized);
}

#ifndef BOBUI_NO_CURSOR
void tst_QWindow::enterLeaveOnWindowShowHide_data()
{
    BOBUIest::addColumn<BobUI::WindowType>("windowType");
    BOBUIest::addRow("dialog") << BobUI::Dialog;
    BOBUIest::addRow("popup") << BobUI::Popup;
}

/*!
    Verify that we get enter and leave events if the window under the mouse
    opens and closes a modal dialog or popup. QWindow might get multiple
    events in a row, as the various QPA plugins need to use different techniques
    to synthesize events if the native platform doesn't provide them for us.
*/
void tst_QWindow::enterLeaveOnWindowShowHide()
{
    if (isPlatformWayland())
        QSKIP("Can't set cursor position and qWaitForWindowActive on Wayland");

    if (isPlatformEglFS())
        QSKIP("QCursor::setPos() is not supported on this platform");

    QFETCH(BobUI::WindowType, windowType);

    class Window : public QWindow
    {
    public:
        int numEnterEvents = 0;
        int numLeaveEvents = 0;
        QPoint enterPosition;
    protected:
        bool event(QEvent *e) override
        {
            switch (e->type()) {
            case QEvent::Enter:
                ++numEnterEvents;
                enterPosition = static_cast<QEnterEvent*>(e)->position().toPoint();
                break;
            case QEvent::Leave:
                ++numLeaveEvents;
                break;
            default:
                break;
            }
            return QWindow::event(e);
        }
    };

    int expectedEnter = 0;
    int expectedLeave = 0;

    Window window;
    const QRect screenGeometry = window.screen()->availableGeometry();
    const QPoint cursorPos = screenGeometry.topLeft() + QPoint(50, 50);
    window.setGeometry(QRect(cursorPos - QPoint(50, 50), screenGeometry.size() / 4));
    QCursor::setPos(cursorPos);

    if (!BOBUIest::qWaitFor([&]{ return window.geometry().contains(QCursor::pos()); }))
        QSKIP("We can't move the cursor");

    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    ++expectedEnter;
    BOBUIRY_COMPARE_WITH_TIMEOUT(window.numEnterEvents, expectedEnter, 250);
    QCOMPARE(window.enterPosition, window.mapFromGlobal(QCursor::pos()));

    QWindow secondary;
    secondary.setFlag(windowType);
    secondary.setModality(BobUI::WindowModal);
    secondary.setTransientParent(&window);
    secondary.setPosition(cursorPos + QPoint(50, 50));
    secondary.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&secondary));
    ++expectedLeave;
    BOBUIRY_VERIFY(window.numLeaveEvents >= expectedLeave);
    secondary.close();
    ++expectedEnter;
    BOBUIRY_VERIFY(window.numEnterEvents >= expectedEnter);
    QCOMPARE(window.enterPosition, window.mapFromGlobal(QCursor::pos()));
}
#endif

void tst_QWindow::windowExposedAfterReparent()
{
    QWindow parent;
    QWindow child(&parent);
    child.show();
    parent.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&parent));
    QVERIFY(BOBUIest::qWaitForWindowExposed(&child));

    // Close the child before reparenting it to ensure it is correctly converted
    // to a toplevel window by the window manager.
    child.close();
    child.setParent(nullptr);
    child.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&child));

    child.setParent(&parent);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&child));
}

struct ParentWindow : public QWindow
{
    bool event(QEvent *event) override
    {
        [&]() -> void {
            if (event->type() == QEvent::ChildWindowAdded
             || event->type() == QEvent::ChildWindowRemoved) {
                // We should not receive child events after the window has been destructed
                QVERIFY(this->isWindowType());

                auto *parentWindow = this;
                auto *childEvent = static_cast<QChildWindowEvent*>(event);
                auto *childWindow = childEvent->child();

                if (event->type() == QEvent::ChildWindowAdded) {
                    QVERIFY(childWindow->parent());
                    QVERIFY(parentWindow->isAncestorOf(childWindow));
                    if (childWindow->handle())
                        QVERIFY(childWindow->handle()->parent() == parentWindow->handle());

                } else {
                    QVERIFY(!childWindow->parent());
                    QVERIFY(!parentWindow->isAncestorOf(childWindow));
                    if (childWindow->handle())
                        QVERIFY(childWindow->handle()->parent() != parentWindow->handle());
                }
            }
        }();

        return QWindow::event(event);
    }
};

void tst_QWindow::childEvents()
{
    ParentWindow parent;

    {
        // ChildAdded via constructor
        QWindow constructorChild(&parent);
        if (BOBUIest::currentTestFailed()) return;
        // ChildRemoved via destructor
    }

    if (BOBUIest::currentTestFailed()) return;

    // ChildAdded and ChildRemoved via setParent
    QWindow child;
    child.setParent(&parent);
    if (BOBUIest::currentTestFailed()) return;
    child.setParent(nullptr);
    if (BOBUIest::currentTestFailed()) return;

    parent.create();
    child.create();

    // ChildAdded and ChildRemoved after creation
    child.setParent(&parent);
    if (BOBUIest::currentTestFailed()) return;
    child.setParent(nullptr);
    if (BOBUIest::currentTestFailed()) return;
}

struct ChildWindowPrivate;
struct ChildWindow : public QWindow
{
    ChildWindow(QWindow *parent = nullptr);
};

struct ChildWindowPrivate : public QWindowPrivate
{
    ChildWindowPrivate() : QWindowPrivate()
    {
        receiveParentEvents = true;
    }
};

ChildWindow::ChildWindow(QWindow *parent)
    : QWindow(*new ChildWindowPrivate, parent)
{}

struct ParentEventTester : public QObject
{
    bool eventFilter(QObject *object, QEvent *event) override
    {
        [&]() -> void {
            if (event->type() == QEvent::ParentWindowAboutToChange
             || event->type() == QEvent::ParentWindowChange) {
                // We should not receive parent events after the window has been destructed
                QVERIFY(object->isWindowType());
                auto *window = static_cast<QWindow*>(object);

                if (event->type() == QEvent::ParentWindowAboutToChange) {
                    QVERIFY(window->parent() != nextExpectedParent);
                    if (window->handle()) {
                        QVERIFY(window->handle()->parent() !=
                            (nextExpectedParent ? nextExpectedParent->handle() : nullptr));
                    }
                } else {
                    QVERIFY(window->parent() == nextExpectedParent);
                    if (window->handle()) {
                        QVERIFY(window->handle()->parent() ==
                            (nextExpectedParent ? nextExpectedParent->handle() : nullptr));
                    }
                }
            }
        }();

        return QObject::eventFilter(object, event);
    }

    QWindow *nextExpectedParent = nullptr;
};



void tst_QWindow::parentEvents()
{
    QWindow parent;

    {
        ParentEventTester tester;

        {
            // We can't hook in early enough to get the parent change during
            // QObject construction.
            ChildWindow child(&parent);

            // But we can observe the one during destruction
            child.installEventFilter(&tester);
            tester.nextExpectedParent = nullptr;
        }
    }
    if (BOBUIest::currentTestFailed()) return;

    ParentEventTester tester;
    ChildWindow child;
    child.installEventFilter(&tester);

    tester.nextExpectedParent = &parent;
    child.setParent(&parent);
    if (BOBUIest::currentTestFailed()) return;

    tester.nextExpectedParent = nullptr;
    child.setParent(nullptr);
    if (BOBUIest::currentTestFailed()) return;

    parent.create();
    child.create();

    tester.nextExpectedParent = &parent;
    child.setParent(&parent);
    if (BOBUIest::currentTestFailed()) return;

    tester.nextExpectedParent = nullptr;
    child.setParent(nullptr);
    if (BOBUIest::currentTestFailed()) return;
}

#include <tst_qwindow.moc>
BOBUIEST_MAIN(tst_QWindow)

