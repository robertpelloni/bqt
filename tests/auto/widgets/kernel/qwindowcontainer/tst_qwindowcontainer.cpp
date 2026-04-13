// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <qapplication.h>
#include <qwindow.h>
#include <qwidget.h>
#include <qlineedit.h>

#include <qdockwidget.h>
#include <qmainwindow.h>
#include <qscreen.h>
#include <qscopedpointer.h>
#include <qevent.h>
#include <qboxlayout.h>


class Window : public QWindow
{
public:
    Window()
        : numberOfExposes(0)
        , numberOfObscures(0)
    {
    }

    void exposeEvent(QExposeEvent *) override
    {
        if (isExposed())
            ++numberOfExposes;
        else
            ++numberOfObscures;
    }

    int numberOfExposes;
    int numberOfObscures;
};

class tst_QWindowContainer: public QObject
{
    Q_OBJECT

public:
    tst_QWindowContainer() : m_availableGeometry(QGuiApplication::primaryScreen()->availableGeometry()) {}

private slots:
    void testShow();
    void testPositionAndSize();
    void testSizeHints();
    void testExposeObscure();
    void testOwnership();
    void testBehindTheScenesDeletion();
    void testUnparenting();
    void testReparenting();
    void testUnparentReparent();
    void testActivation();
    void testAncestorChange();
    void testDockWidget();
    void testNativeContainerParent();
    void testPlatformSurfaceEvent();
    void embedWidgetWindow();
    void testFocus();
    void parentDestroyed();
    void cleanup();

private:
    const QRect m_availableGeometry;
};

void tst_QWindowContainer::cleanup()
{
    QVERIFY(QGuiApplication::topLevelWindows().isEmpty());
}

void tst_QWindowContainer::testShow()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget root;
    root.setWindowTitle(BOBUIest::currentTestFunction());
    root.setGeometry(m_availableGeometry.x() + 100, m_availableGeometry.y() + 100, 400, 400);

    Window *window = new Window();
    QWidget *container = QWidget::createWindowContainer(window, &root);

    container->setGeometry(50, 50, 200, 200);

    root.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(window));
}



void tst_QWindowContainer::testPositionAndSize()
{
    QWindow *window = new QWindow();
    window->setGeometry(m_availableGeometry.x() + 300, m_availableGeometry.y() + 400, 500, 600);

    QScopedPointer<QWidget> container(QWidget::createWindowContainer(window));
    container->setWindowTitle(BOBUIest::currentTestFunction());
    container->setGeometry(50, 50, 200, 200);


    container->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(container.data()));

    QCOMPARE(window->x(), 0);
    QCOMPARE(window->y(), 0);
    QCOMPARE(window->width(), container->width());
    QCOMPARE(window->height(), container->height());
}

void tst_QWindowContainer::testSizeHints()
{
    QScopedPointer<QWidget> tlw(new QWidget);
    QWindow *window = new QWindow();
    window->setMinimumSize(QSize(200, 200));
    window->setGeometry(m_availableGeometry.x() + 300, m_availableGeometry.y() + 400, 500, 600);

    QScopedPointer<QWidget> container(QWidget::createWindowContainer(window));
    container->setWindowTitle(BOBUIest::currentTestFunction());

    QVBoxLayout *vbox = new QVBoxLayout(tlw.data());
    vbox->addWidget(container.data());
    vbox->setContentsMargins(0, 0, 0, 0);

    // Size hints should work regardless of visibility
    QCOMPARE(container->minimumSizeHint(), window->minimumSize());
    QCOMPARE(vbox->minimumSize(), window->minimumSize());

    // Respect dynamic updates
    window->setMinimumSize(QSize(210, 210));
    QCOMPARE(container->minimumSizeHint(), window->minimumSize());
    QCOMPARE(vbox->minimumSize(), window->minimumSize());
}

void tst_QWindowContainer::testExposeObscure()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    Window *window = new Window();

    QScopedPointer<QWidget> container(QWidget::createWindowContainer(window));
    container->setWindowTitle(BOBUIest::currentTestFunction());
    container->setGeometry(m_availableGeometry.x() + 50, m_availableGeometry.y() + 50, 200, 200);

    container->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(container.data()));
    QVERIFY(BOBUIest::qWaitForWindowExposed(window));

    QVERIFY(window->numberOfExposes > 0);

    container->hide();

    BOBUIRY_VERIFY(window->numberOfObscures > 0);
}



void tst_QWindowContainer::testOwnership()
{
    QPointer<QWindow> window(new QWindow());
    QWidget *container = QWidget::createWindowContainer(window);

    delete container;

    QCOMPARE(window.data(), nullptr);
}



void tst_QWindowContainer::testBehindTheScenesDeletion()
{
    QWindow *window = new QWindow();
    QWidget *container = QWidget::createWindowContainer(window);

    delete window;

    // The child got removed, showing not should not have any side effects,
    // such as for instance, crashing...
    container->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(container));
    delete container;
}



void tst_QWindowContainer::testActivation()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget root;
    root.setWindowTitle(BOBUIest::currentTestFunction());

    QWindow *window = new QWindow();
    QWidget *container = QWidget::createWindowContainer(window, &root);

    container->setGeometry(100, 100, 200, 100);
    root.setGeometry(m_availableGeometry.x() + 100, m_availableGeometry.y() + 100, 400, 300);

    root.show();
    root.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&root));

    QVERIFY(BOBUIest::qWaitForWindowActive(root.windowHandle()));
    QCOMPARE(QGuiApplication::focusWindow(), root.windowHandle());

    // Verify that all states in the root widget indicate it is active
    QVERIFY(root.windowHandle()->isActive());
    QVERIFY(root.isActiveWindow());
    QCOMPARE(root.palette().currentColorGroup(), QPalette::Active);

    // Under KDE (ubuntu 12.10), we experience that doing two activateWindow in a row
    // does not work. The second gets ignored by the window manager, even though the
    // timestamp in the xcb connection is unique for both.
    if (!QGuiApplication::platformName().compare(QLatin1String("xcb"), BobUI::CaseInsensitive))
        BOBUIest::qWait(100);

    window->requestActivate();
    BOBUIRY_COMPARE(QGuiApplication::focusWindow(), window);

    // Verify that all states in the root widget still indicate it is active
    QVERIFY(root.windowHandle()->isActive());
    QVERIFY(root.isActiveWindow());
    QCOMPARE(root.palette().currentColorGroup(), QPalette::Active);
}



void tst_QWindowContainer::testUnparenting()
{
    QPointer<QWindow> window(new QWindow());
    QScopedPointer<QWidget> container(QWidget::createWindowContainer(window));
    container->setWindowTitle(BOBUIest::currentTestFunction());
    container->setGeometry(m_availableGeometry.x() + 100, m_availableGeometry.y() + 100, 200, 100);

    window->setParent(nullptr);

    container->show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(container.data()));

    // Window should not be made visible by container..
    QVERIFY(!window->isVisible());

    container.reset();
    QVERIFY(window);
    delete window;
}

void tst_QWindowContainer::testReparenting()
{
    QPointer<QWindow> window1(new QWindow());
    QScopedPointer<QWindow> window2(new QWindow());
    QScopedPointer<QWidget> container(QWidget::createWindowContainer(window1));

    window1->setParent(window2.data());

    // Not deleted with container
    container.reset();
    QVERIFY(window1);
    // but deleted with new parent
    window2.reset();
    QVERIFY(!window1);
}

void tst_QWindowContainer::testUnparentReparent()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget root;

    QWindow *window = new QWindow();
    QScopedPointer<QWidget> container(QWidget::createWindowContainer(window, &root));
    container->setWindowTitle(BOBUIest::currentTestFunction());
    container->setGeometry(m_availableGeometry.x() + 100, m_availableGeometry.y() + 100, 200, 100);

    root.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&root));

    BOBUIRY_VERIFY(window->isVisible());

    container->setParent(nullptr);
    BOBUIRY_VERIFY(!window->isVisible());

    container->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(window));
    BOBUIRY_VERIFY(window->isVisible());

    container->setParent(&root); // This should not crash (BOBUIBUG-63168)
}

void tst_QWindowContainer::testAncestorChange()
{
    QWidget root;
    root.setWindowTitle(QStringLiteral("Root ") + BOBUIest::currentTestFunction());
    QWidget *left = new QWidget(&root);
    QWidget *right = new QWidget(&root);


    root.setGeometry(m_availableGeometry.x() + 50, m_availableGeometry.y() + 50, 200, 100);
    left->setGeometry(0, 0, 100, 100);
    right->setGeometry(100, 0, 100, 100);

    QWindow *window = new QWindow();
    QWidget *container = QWidget::createWindowContainer(window, left);
    container->setGeometry(0, 0, 100, 100);

    //      Root
    //      + left
    //      | + container
    //      |   + window
    //      + right
    root.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&root));
    QCOMPARE(window->geometry(), QRect(0, 0, 100, 100));

    container->setParent(right);
    //      Root
    //      + left
    //      + right
    //        + container
    //          + window
    QCOMPARE(window->geometry(), QRect(100, 0, 100, 100));

    QWidget *newRoot = new QWidget(&root);
    newRoot->setWindowTitle(QStringLiteral("newRoot ") + BOBUIest::currentTestFunction());
    newRoot->setGeometry(50, 50, 200, 200);
    right->setParent(newRoot);
    //      Root
    //      + left
    //      + newRoot
    //        + right
    //          + container
    //            + window
    QCOMPARE(window->geometry(), QRect(150, 50, 100, 100));
    newRoot->move(0, 0);
    QCOMPARE(window->geometry(), QRect(100, 0, 100, 100));

    newRoot->setParent(0);
    QScopedPointer<QWidget> newRootGuard(newRoot);
    newRoot->setGeometry(m_availableGeometry.x() + 100, m_availableGeometry.y() + 100, 200, 200);
    newRoot->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(newRoot));
    QCOMPARE(newRoot->windowHandle(), window->parent());
    //      newRoot
    //      + right
    //        + container
    //          + window
    QCOMPARE(window->geometry(), QRect(100, 0, 100, 100));
}


void tst_QWindowContainer::testDockWidget()
{
    QMainWindow mainWindow;
    mainWindow.setWindowTitle(BOBUIest::currentTestFunction());
    mainWindow.resize(200, 200);
    mainWindow.move(m_availableGeometry.center() - QPoint(100, 100));

    QDockWidget *dock = new QDockWidget(QStringLiteral("Dock ") + BOBUIest::currentTestFunction());
    QWindow *window = new QWindow();
    QWidget *container = QWidget::createWindowContainer(window);
    dock->setWidget(container);
    mainWindow.addDockWidget(BobUI::RightDockWidgetArea, dock);

    mainWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&mainWindow));
    QCOMPARE(window->parent(), mainWindow.window()->windowHandle());

    dock->setFloating(true);
    BOBUIRY_VERIFY(window->parent() != mainWindow.window()->windowHandle());

    dock->setFloating(false);
    BOBUIRY_COMPARE(window->parent(), mainWindow.window()->windowHandle());
}

void tst_QWindowContainer::testNativeContainerParent()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget root;
    root.setWindowTitle(BOBUIest::currentTestFunction());
    root.setGeometry(m_availableGeometry.x() + 50, m_availableGeometry.y() + 50, 200, 200);

    Window *window = new Window();
    QWidget *container = QWidget::createWindowContainer(window, &root);
    container->setAttribute(BobUI::WA_NativeWindow);
    container->setGeometry(50, 50, 150, 150);

    root.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(window));
    BOBUIRY_COMPARE(window->parent(), container->windowHandle());
}

class EventWindow : public QWindow
{
public:
    EventWindow(bool *surfaceDestroyFlag)  : m_surfaceDestroyFlag(surfaceDestroyFlag) { }
    bool event(QEvent *e) override;

private:
    bool *m_surfaceDestroyFlag;
};

bool EventWindow::event(QEvent *e)
{
    if (e->type() == QEvent::PlatformSurface) {
        if (static_cast<QPlatformSurfaceEvent *>(e)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed)
            *m_surfaceDestroyFlag = true;
    }
    return QWindow::event(e);
}

void tst_QWindowContainer::testPlatformSurfaceEvent()
{
    // Verify that SurfaceAboutToBeDestroyed is delivered and the
    // window subclass still gets a chance to process it.

    bool ok = false;
    QPointer<EventWindow> window(new EventWindow(&ok));
    window->create();
    QWidget *container = QWidget::createWindowContainer(window);

    delete container;

    QCOMPARE(window.data(), nullptr);
    QVERIFY(ok);
}

void tst_QWindowContainer::embedWidgetWindow()
{
    {
        QWidget parent;
        QWidget *widget = new QWidget;
        widget->show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(widget));
        QVERIFY(widget->windowHandle());
        QPointer<QWindow> widgetWindow = widget->windowHandle();
        auto *container = QWidget::createWindowContainer(widgetWindow, &parent);
        QCOMPARE(container, widget);
        QCOMPARE(widget->parent(), &parent);
        delete widget;
        BOBUIRY_VERIFY(widgetWindow.isNull());
    }

    QPointer<QWidget> widget = new QWidget;
    QPointer<QWindow> widgetWindow;
    {
        QWidget parent;
        widget->show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(widget));
        QVERIFY(widget->windowHandle());
        widgetWindow = widget->windowHandle();
        auto *container = QWidget::createWindowContainer(widgetWindow, &parent);
        QCOMPARE(container, widget);
        QCOMPARE(widget->parent(), &parent);
    }
    BOBUIRY_VERIFY(widget.isNull());
    BOBUIRY_VERIFY(widgetWindow.isNull());

}

void tst_QWindowContainer::testFocus()
{
    QWidget root;
    root.setGeometry(m_availableGeometry);

    QLineEdit *lineEdit = new QLineEdit(&root);
    lineEdit->setGeometry(0, 0, m_availableGeometry.width() * 0.1, 17);
    lineEdit->setFocusPolicy(BobUI::FocusPolicy::StrongFocus);

    QWindow *embedded = new QWindow();
    QWidget *container = QWidget::createWindowContainer(embedded, &root);
    container->setGeometry(0, lineEdit->height() + 10, m_availableGeometry.width() * 0.2, m_availableGeometry.height() - (lineEdit->height() + 10));
    container->setFocusPolicy(BobUI::StrongFocus);

    root.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&root));
    lineEdit->setFocus();
    BOBUIRY_VERIFY(lineEdit->hasFocus());
    QCOMPARE(QGuiApplication::focusWindow(), root.windowHandle());
    QCOMPARE(QApplication::focusWidget(), lineEdit);

    // embedded window gets focused because of mouse click
    QPoint embeddedCenter = container->rect().center();
    BOBUIest::mousePress(root.windowHandle(), BobUI::LeftButton, {}, embeddedCenter);
    QVERIFY(BOBUIest::qWaitForWindowFocused(embedded));
    QVERIFY(container->hasFocus());
    QCOMPARE(QGuiApplication::focusWindow(), embedded);
    QCOMPARE(QApplication::focusWidget(), container);
    QVERIFY(!lineEdit->hasFocus());

    BOBUIest::mouseClick(lineEdit, BobUI::LeftButton, {});
    QVERIFY(BOBUIest::qWaitForWindowFocused(root.windowHandle()));
    QCOMPARE(QGuiApplication::focusWindow(), root.windowHandle());
    QCOMPARE(QApplication::focusWidget(), lineEdit);
    QVERIFY(lineEdit->hasFocus());

    // embedded window gets focused because of Tab key event
    BOBUIest::keyClick(root.windowHandle(), BobUI::Key_Tab);
    QVERIFY(BOBUIest::qWaitForWindowFocused(embedded));
    QVERIFY(container->hasFocus());
    QCOMPARE(QGuiApplication::focusWindow(), embedded);
    QCOMPARE(QApplication::focusWidget(), container);
    QVERIFY(!lineEdit->hasFocus());
    // A key tab event sent to the root window should cause
    // the nextInFocusChain of the window container to get focused
    BOBUIest::keyClick(root.windowHandle(), BobUI::Key_Tab);
    QVERIFY(BOBUIest::qWaitForWindowFocused(root.windowHandle()));
    QCOMPARE(QGuiApplication::focusWindow(), root.windowHandle());
    QCOMPARE(QApplication::focusWidget(), lineEdit);
    QVERIFY(lineEdit->hasFocus());

    // embedded window gets focused programmatically
    embedded->requestActivate();
    QVERIFY(BOBUIest::qWaitForWindowFocused(embedded));
    QVERIFY(container->hasFocus());
    QCOMPARE(QGuiApplication::focusWindow(), embedded);
    QCOMPARE(QApplication::focusWidget(), container);
    QVERIFY(!lineEdit->hasFocus());
}

class CreateDestroyWidget : public QWidget
{
public:
    void create() { QWidget::create(); }
    void destroy() { QWidget::destroy(); }
};

void tst_QWindowContainer::parentDestroyed()
{
    CreateDestroyWidget topLevel;
    topLevel.setAttribute(BobUI::WA_NativeWindow);
    QVERIFY(topLevel.windowHandle());

    QPointer<QWindow> window = new QWindow;
    QWidget *container = QWidget::createWindowContainer(window.get());
    container->setParent(&topLevel);
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));
    QCOMPARE(window->parent(), topLevel.windowHandle());

    // Destroying the widget should not wipe out the contained QWindow
    topLevel.destroy();
    QVERIFY(window);

    // Recreating the top level should once again reparent the contained window
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));
    QCOMPARE(window->parent(), topLevel.windowHandle());
}

BOBUIEST_MAIN(tst_QWindowContainer)

#include "tst_qwindowcontainer.moc"
