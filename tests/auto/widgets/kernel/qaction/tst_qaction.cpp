// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDialog>
#include <QMainWindow>
#include <BOBUIest>
#include <QSignalSpy>

#include <qapplication.h>
#include <qevent.h>
#include <qaction.h>
#include <qactiongroup.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <bobuioolbar.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformintegration.h>
#include <private/qguiapplication_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

class tst_QAction : public QObject
{
    Q_OBJECT

public:
    tst_QAction();

    void updateState(QActionEvent *e);

private slots:
    void init();
    void cleanup();
    void setUnknownFont();
    void actionEvent();
#if BOBUI_CONFIG(shortcut)
    void alternateShortcuts();
    void enabledVisibleInteraction();
#endif
    void task229128TriggeredSignalWhenInActiongroup();
#if BOBUI_CONFIG(shortcut)
    void repeat();
    void keysequence(); // BOBUIBUG-53381
    void disableShortcutsWithBlockedWidgets_data();
    void disableShortcutsWithBlockedWidgets();
    void shortcutFromKeyEvent(); // BOBUIBUG-48325
    void disableShortcutInMenuAction_data();
    void disableShortcutInMenuAction();
#endif

private:
    QEvent::Type m_lastEventType;
    const int m_keyboardScheme;
    QAction *m_lastAction;
};

tst_QAction::tst_QAction()
    : m_keyboardScheme(QGuiApplicationPrivate::platformTheme()->themeHint(QPlatformTheme::KeyboardScheme).toInt())
{
}

void tst_QAction::init()
{
    m_lastEventType = QEvent::None;
    m_lastAction = nullptr;
}

void tst_QAction::cleanup()
{
    QVERIFY(QApplication::topLevelWidgets().isEmpty());
}

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(tst_QAction *tst, QWidget *parent = nullptr) : QWidget(parent), m_test(tst)
    { setWindowTitle(BOBUIest::currentTestFunction()); }

protected:
    void actionEvent(QActionEvent *e) override { m_test->updateState(e); }

private:
    tst_QAction *m_test;
};

void tst_QAction::setUnknownFont() // BOBUIBUG-42728
{
    QAction action(nullptr);
    QFont font("DoesNotExist", 11);
    action.setFont(font);

    QMenu menu;
    menu.addAction(&action); // should not crash
}

void tst_QAction::updateState(QActionEvent *e)
{
    if (!e) {
        m_lastEventType = QEvent::None;
        m_lastAction = nullptr;
    } else {
        m_lastEventType = e->type();
        m_lastAction = e->action();
    }
}

void tst_QAction::actionEvent()
{
    QAction a(nullptr);
    a.setText("action text");

    // add action
    MyWidget testWidget(this);
    testWidget.show();
    testWidget.addAction(&a);
    qApp->processEvents();

    QCOMPARE(m_lastEventType, QEvent::ActionAdded);
    QCOMPARE(m_lastAction, &a);

    // change action
    a.setText("new action text");
    qApp->processEvents();

    QCOMPARE(m_lastEventType, QEvent::ActionChanged);
    QCOMPARE(m_lastAction, &a);

    // remove action
    testWidget.removeAction(&a);
    qApp->processEvents();

    QCOMPARE(m_lastEventType, QEvent::ActionRemoved);
    QCOMPARE(m_lastAction, &a);
}

#if BOBUI_CONFIG(shortcut)

void tst_QAction::alternateShortcuts()
{
    //test the alternate shortcuts (by adding more than 1 shortcut)

    MyWidget testWidget(this);
    testWidget.show();
    QApplicationPrivate::setActiveWindow(&testWidget);

    {
        QAction act(&testWidget);
        testWidget.addAction(&act);
        QList<QKeySequence> shlist = QList<QKeySequence>() << QKeySequence("CTRL+P") << QKeySequence("CTRL+A");
        act.setShortcuts(shlist);

        QSignalSpy spy(&act, &QAction::triggered);

        act.setAutoRepeat(true);
        BOBUIest::keyClick(&testWidget, BobUI::Key_A, BobUI::ControlModifier);
        QCOMPARE(spy.size(), 1); //act should have been triggered

        act.setAutoRepeat(false);
        BOBUIest::keyClick(&testWidget, BobUI::Key_A, BobUI::ControlModifier);
        QCOMPARE(spy.size(), 2); //act should have been triggered a 2nd time

        //end of the scope of the action, it will be destroyed and removed from wid
        //This action should also unregister its shortcuts
    }


    //this tests a crash (if the action did not unregister its alternate shortcuts)
    BOBUIest::keyClick(&testWidget, BobUI::Key_A, BobUI::ControlModifier);
}

void tst_QAction::keysequence()
{
    MyWidget testWidget(this);
    testWidget.show();
    QApplicationPrivate::setActiveWindow(&testWidget);

    {
        QAction act(&testWidget);
        testWidget.addAction(&act);

        QKeySequence ks(QKeySequence::SelectAll);

        act.setShortcut(ks);

        QSignalSpy spy(&act, &QAction::triggered);

        act.setAutoRepeat(true);
        BOBUIest::keySequence(&testWidget, ks);
        QCoreApplication::processEvents();
        QCOMPARE(spy.size(), 1); // act should have been triggered

        act.setAutoRepeat(false);
        BOBUIest::keySequence(&testWidget, ks);
        QCoreApplication::processEvents();
        QCOMPARE(spy.size(), 2); //act should have been triggered a 2nd time

        // end of the scope of the action, it will be destroyed and removed from widget
        // This action should also unregister its shortcuts
    }

    // this tests a crash (if the action did not unregister its alternate shortcuts)
    BOBUIest::keyClick(&testWidget, BobUI::Key_A, BobUI::ControlModifier);
}

void tst_QAction::enabledVisibleInteraction()
{
    MyWidget testWidget(this);
    testWidget.show();
    QApplicationPrivate::setActiveWindow(&testWidget);

    QAction act(nullptr);
    // check defaults
    QVERIFY(act.isEnabled());
    QVERIFY(act.isVisible());

    // !visible => !enabled
    act.setVisible(false);
    QVERIFY(!act.isEnabled());
    act.setVisible(true);
    QVERIFY(act.isEnabled());
    act.setEnabled(false);
    QVERIFY(act.isVisible());

    // check if shortcut is disabled if not visible
    testWidget.addAction(&act);
    act.setShortcut(QKeySequence("Ctrl+T"));
    QSignalSpy spy(&act, SIGNAL(triggered()));
    act.setEnabled(true);
    act.setVisible(false);
    BOBUIest::keyClick(&testWidget, BobUI::Key_T, BobUI::ControlModifier);
    QCOMPARE(spy.size(), 0); //act is not visible, so don't trigger
    act.setVisible(false);
    act.setEnabled(true);
    BOBUIest::keyClick(&testWidget, BobUI::Key_T, BobUI::ControlModifier);
    QCOMPARE(spy.size(), 0); //act is not visible, so don't trigger
    act.setVisible(true);
    act.setEnabled(true);
    BOBUIest::keyClick(&testWidget, BobUI::Key_T, BobUI::ControlModifier);
    QCOMPARE(spy.size(), 1); //act is visible and enabled, so trigger
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QAction::task229128TriggeredSignalWhenInActiongroup()
{
    QActionGroup ag(nullptr);
    QAction *action = new QAction("Test", &ag);
    QAction *checkedAction = new QAction("Test 2", &ag);
    ag.addAction(action);
    action->setCheckable(true);
    ag.addAction(checkedAction);
    checkedAction->setCheckable(true);
    checkedAction->setChecked(true);

    QSignalSpy actionSpy(checkedAction, QOverload<bool>::of(&QAction::triggered));
    QSignalSpy actionGroupSpy(&ag, QOverload<QAction*>::of(&QActionGroup::triggered));
    QCOMPARE(actionGroupSpy.size(), 0);
    QCOMPARE(actionSpy.size(), 0);
    checkedAction->trigger();
    // check that both the group and the action have emitted the signal
    QCOMPARE(actionGroupSpy.size(), 1);
    QCOMPARE(actionSpy.size(), 1);
}

#if BOBUI_CONFIG(shortcut)

void tst_QAction::repeat()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    MyWidget testWidget(this);
    testWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&testWidget));

    QAction act(&testWidget);
    testWidget.addAction(&act);
    act.setShortcut(QKeySequence(BobUI::Key_F));
    QSignalSpy spy(&act, SIGNAL(triggered()));

    act.setAutoRepeat(true);
    BOBUIest::keyPress(&testWidget, BobUI::Key_F);
    BOBUIest::keyRelease(&testWidget, BobUI::Key_F);
    QCOMPARE(spy.size(), 1);

    spy.clear();
    BOBUIest::keyPress(&testWidget, BobUI::Key_F);
    // repeat event
    BOBUIest::simulateEvent(&testWidget, true, BobUI::Key_F, BobUI::NoModifier, QString("f"), true);
    BOBUIest::simulateEvent(&testWidget, true, BobUI::Key_F, BobUI::NoModifier, QString("f"), true);
    BOBUIest::keyRelease(&testWidget, BobUI::Key_F);
    QCOMPARE(spy.size(), 3);

    spy.clear();
    act.setAutoRepeat(false);
    BOBUIest::keyPress(&testWidget, BobUI::Key_F);
    BOBUIest::simulateEvent(&testWidget, true, BobUI::Key_F, BobUI::NoModifier, QString("f"), true);
    BOBUIest::simulateEvent(&testWidget, true, BobUI::Key_F, BobUI::NoModifier, QString("f"), true);
    BOBUIest::keyRelease(&testWidget, BobUI::Key_F);
    QCOMPARE(spy.size(), 1);

    spy.clear();
    act.setAutoRepeat(true);
    BOBUIest::keyPress(&testWidget, BobUI::Key_F);
    BOBUIest::simulateEvent(&testWidget, true, BobUI::Key_F, BobUI::NoModifier, QString("f"), true);
    BOBUIest::keyRelease(&testWidget, BobUI::Key_F);
    QCOMPARE(spy.size(), 2);
}

void tst_QAction::disableShortcutsWithBlockedWidgets_data()
{
    BOBUIest::addColumn<BobUI::ShortcutContext>("shortcutContext");
    BOBUIest::addColumn<BobUI::WindowModality>("windowModality");

    BOBUIest::newRow("application modal dialog should block window shortcut.")
        << BobUI::WindowShortcut << BobUI::ApplicationModal;

    BOBUIest::newRow("application modal dialog should block application shortcut.")
        << BobUI::ApplicationShortcut << BobUI::ApplicationModal;

    BOBUIest::newRow("window modal dialog should block application shortcut.")
        << BobUI::ApplicationShortcut << BobUI::WindowModal;

    BOBUIest::newRow("window modal dialog should block window shortcut.")
        << BobUI::WindowShortcut << BobUI::WindowModal;
}


void tst_QAction::disableShortcutsWithBlockedWidgets()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    QMainWindow window;

    QFETCH(BobUI::ShortcutContext, shortcutContext);
    QAction action(&window);
    window.addAction(&action);
    action.setShortcut(QKeySequence(BobUI::Key_1));
    action.setShortcutContext(shortcutContext);

    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QDialog dialog(&window);
    QFETCH(BobUI::WindowModality, windowModality);
    dialog.setWindowModality(windowModality);

    dialog.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dialog));

    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    QSignalSpy spy(&action, &QAction::triggered);
    BOBUIest::keyPress(&window, BobUI::Key_1);
    QCOMPARE(spy.size(), 0);
}

class ShortcutOverrideWidget : public QWidget
{
public:
    using QWidget::QWidget;
    int shortcutOverrideCount = 0;
protected:
    bool event(QEvent *e) override
    {
        if (e->type() == QEvent::ShortcutOverride)
            ++shortcutOverrideCount;
        return QWidget::event(e);
    }
};

// Test that a key press event sent with sendEvent() still gets handled as a possible
// ShortcutOverride event first before passing it on as a normal KeyEvent.
void tst_QAction::shortcutFromKeyEvent()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    ShortcutOverrideWidget testWidget;
    QAction action;
    action.setShortcut(BobUI::Key_1);
    testWidget.addAction(&action);
    testWidget.show();
    QSignalSpy spy(&action, &QAction::triggered);
    QVERIFY(spy.isValid());
    QVERIFY(BOBUIest::qWaitForWindowActive(&testWidget));
    QCOMPARE(testWidget.shortcutOverrideCount, 0);

    // Don't use the BOBUIest::keyPress approach as this will take the
    // shortcut route for us
    QKeyEvent e(QEvent::KeyPress, BobUI::Key_1, BobUI::NoModifier);
    QApplication::sendEvent(&testWidget, &e);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(testWidget.shortcutOverrideCount, 1);
}

/*
    Ignore actions in menus whose menu action has been hidden or disabled.
    The menu entry will not be in the menu bar or parent menu, so the action
    is not reachable through interactive means. BOBUIBUG-25743
*/
void tst_QAction::disableShortcutInMenuAction_data()
{
    BOBUIest::addColumn<QByteArray>("property");

    BOBUIest::addRow("visible") << QByteArray("visible");
    BOBUIest::addRow("enabled") << QByteArray("enabled");
}

void tst_QAction::disableShortcutInMenuAction()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("QWindow::requestActivate() is not supported.");

    QFETCH(QByteArray, property);

    QMainWindow mw;
    QMenu *testMenu = mw.menuBar()->addMenu("Test");
    QAction *testAction = testMenu->addAction("Test Action");
    testAction->setShortcut(BobUI::ControlModifier | BobUI::Key_A);
    BOBUIoolBar *toolBar = new BOBUIoolBar;
    mw.addToolBar(toolBar);

    mw.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mw));

    int expectedTriggerCount = 0;
    QSignalSpy spy(testAction, &QAction::triggered);

    QKeyEvent event(QEvent::KeyPress, BobUI::Key_A, BobUI::ControlModifier);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), ++expectedTriggerCount);

    testMenu->menuAction()->setProperty(property, false);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), expectedTriggerCount);

    testMenu->menuAction()->setProperty(property, true);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), ++expectedTriggerCount);

    // If the action lives somewhere else, then keep firing even
    // if the menu has been hidden or disabled.
    toolBar->addAction(testAction);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), ++expectedTriggerCount);

    testMenu->menuAction()->setProperty(property, false);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), ++expectedTriggerCount);

    // unless all other widgets in which the action lives have
    // been hidden...
    toolBar->hide();
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), expectedTriggerCount);

    // ... or disabled
    toolBar->show();
    toolBar->setEnabled(false);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), expectedTriggerCount);

    // back to normal
    toolBar->setEnabled(true);
    QApplication::sendEvent(&mw, &event);
    QCOMPARE(spy.size(), ++expectedTriggerCount);
}

#endif // BOBUI_CONFIG(shortcut)

BOBUIEST_MAIN(tst_QAction)
#include "tst_qaction.moc"
