// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUITest/private/bobuiesthelpers_p.h>
#include <qapplication.h>
#include <private/qguiapplication_p.h>
#include <QPushButton>
#include <QMainWindow>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <BOBUIoolBar>
#include <BOBUIoolButton>
#include <QStatusBar>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QScreen>
#include <QSpinBox>
#include <QSignalSpy>
#include <qdialog.h>

#include <qmenu.h>
#include <qstyle.h>
#include <QStyleHints>
#include <BOBUIimer>
#include <qdebug.h>
#include <BOBUIoolTip>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformintegration.h>

#include <BobUIWidgets/private/qapplication_p.h>
#include <BobUIWidgets/private/qmenu_p.h>

using namespace BOBUIestPrivate;

Q_DECLARE_METATYPE(BobUI::Key);
Q_DECLARE_METATYPE(BobUI::KeyboardModifiers);

struct MenuMetrics {
    int fw;
    int hmargin;
    int vmargin;
    int tearOffHeight;

    MenuMetrics(const QMenu *menu) {
        fw = menu->style()->pixelMetric(QStyle::PM_MenuPanelWidth, nullptr, menu);
        hmargin = menu->style()->pixelMetric(QStyle::PM_MenuHMargin, nullptr, menu);
        vmargin = menu->style()->pixelMetric(QStyle::PM_MenuVMargin, nullptr, menu);
        tearOffHeight = menu->style()->pixelMetric(QStyle::PM_MenuTearoffHeight, nullptr, menu);
    }
};

class tst_QMenu : public QObject
{
    Q_OBJECT

public:
    tst_QMenu();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
private slots:
    void getSetCheck();
    void addActionsAndClear();
    void addActionsConnect();

    void keyboardNavigation_data();
    void keyboardNavigation();
    void focus();
    void overrideMenuAction();
    void statusTip();
    void widgetActionFocus();
    void widgetActionFocusReason();
    void mouseActivation();
    void tearOff();
    void submenuTearOffDontClose();
    void layoutDirection();

    void task208001_stylesheet();
    void activeSubMenuPosition();
    void activeSubMenuPositionExec();
    void task242454_sizeHint();
    void task176201_clear();
    void task250673_activeMultiColumnSubMenuPosition();
    void task256918_setFont();
    void menuSizeHint();
    void task258920_mouseBorder();
    void setFixedWidth();
    void deleteActionInTriggered();
    void pushButtonPopulateOnAboutToShow();
    void BOBUIBUG7907_submenus_autoselect();
    void BOBUIBUG7411_submenus_activate();
    void BOBUIBUG30595_rtl_submenu();
    void BOBUIBUG20403_nested_popup_on_shortcut_trigger();
    void BOBUIBUG47515_widgetActionEnterLeave();
    void BOBUIBUG_89082_actionTipsHide();
    void BOBUIBUG8122_widgetActionCrashOnClose();
    void widgetActionTriggerClosesMenu();
    void widgetActionContextMenu();

    void transientParent();

    void BOBUIBUG_10735_crashWithDialog();
#ifdef Q_OS_MAC
    void BOBUIBUG_37933_ampersands_data();
    void BOBUIBUG_37933_ampersands();
#else
    void click_while_dismissing_submenu();
#endif
    void BOBUIBUG_56917_wideMenuSize();
    void BOBUIBUG_56917_wideMenuScreenNumber();
    void BOBUIBUG_56917_wideSubmenuScreenNumber();
    void menuSize_Scrolling_data();
    void menuSize_Scrolling();
    void tearOffMenuNotDisplayed();
    void BOBUIBUG_61039_menu_shortcuts();
    void screenOrientationChangedCloseMenu();
    void deleteWhenTriggered();

    void nestedTearOffDetached();
    void closeMenuOnClickIfMouseHasntMoved();
#if BOBUI_CONFIG(shortcut) && !defined(Q_OS_DARWIN)
    void dontSelectDisabledActionByShortcut();
#endif

    void invisibleActions();
    void execReturnsWidgetAction();

protected slots:
    void onActivated(QAction*);
    void onHighlighted(QAction*);
    void onStatusMessageChanged(const QString &);
    void onStatusTipTimer();
    void deleteAction(QAction *a) { delete a; }
private:
    void createActions();
    QMenu *menus[2], *lastMenu;
    enum { num_builtins = 10 };
    QAction *activated, *highlighted, *builtins[num_builtins];
    QString statustip;
    bool m_onStatusTipTimerExecuted;
};

// Testing get/set functions
void tst_QMenu::getSetCheck()
{
    QMenu obj1;
    // QAction * QMenu::defaultAction()
    // void QMenu::setDefaultAction(QAction *)
    QAction *var1 = new QAction(0);
    obj1.setDefaultAction(var1);
    QCOMPARE(var1, obj1.defaultAction());
    obj1.setDefaultAction((QAction *)0);
    QCOMPARE((QAction *)0, obj1.defaultAction());
    delete var1;

    // QAction * QMenu::activeAction()
    // void QMenu::setActiveAction(QAction *)
    QAction *var2 = new QAction(0);
    obj1.setActiveAction(var2);
    QCOMPARE(var2, obj1.activeAction());
    obj1.setActiveAction((QAction *)0);
    QCOMPARE((QAction *)0, obj1.activeAction());
    delete var2;
}

tst_QMenu::tst_QMenu()
    : m_onStatusTipTimerExecuted(false)
{
    QApplication::setEffectEnabled(BobUI::UI_FadeTooltip, false);
    QApplication::setEffectEnabled(BobUI::UI_AnimateMenu, false);
    QApplication::setEffectEnabled(BobUI::UI_AnimateTooltip, false);
}

void tst_QMenu::initTestCase()
{
    for (int i = 0; i < num_builtins; i++)
        builtins[i] = 0;
    for (int i = 0; i < 2; i++) {
        menus[i] = new QMenu;
        QObject::connect(menus[i], SIGNAL(triggered(QAction*)), this, SLOT(onActivated(QAction*)));
        QObject::connect(menus[i], SIGNAL(hovered(QAction*)), this, SLOT(onHighlighted(QAction*)));
    }
}

void tst_QMenu::cleanupTestCase()
{
    for (int i = 0; i < 2; i++)
        menus[i]->clear();
    for (int i = 0; i < num_builtins; i++) {
        bool menuAction = false;
        for (int j = 0; j < 2; ++j)
            if (menus[j]->menuAction() == builtins[i])
                menuAction = true;
        if (!menuAction)
            delete builtins[i];
    }
    delete menus[0];
    delete menus[1];
}

void tst_QMenu::init()
{
    activated = highlighted = 0;
    lastMenu = 0;
    m_onStatusTipTimerExecuted = false;
}

void tst_QMenu::createActions()
{
    if (!builtins[0])
        builtins[0] = new QAction("New", 0);
    menus[0]->addAction(builtins[0]);

    if (!builtins[1]) {
        builtins[1] = new QAction(0);
        builtins[1]->setSeparator(true);
    }
    menus[0]->addAction(builtins[1]);

    if (!builtins[2]) {
        builtins[2] = menus[1]->menuAction();
        builtins[2]->setText("&Open..");
        builtins[8] = new QAction("Close", 0);
        menus[1]->addAction(builtins[8]);
        builtins[9] = new QAction("Quit", 0);
        menus[1]->addAction(builtins[9]);
    }
    menus[0]->addAction(builtins[2]);

    if (!builtins[3])
        builtins[3] = new QAction("Open &as..", 0);
    menus[0]->addAction(builtins[3]);

    if (!builtins[4]) {
        builtins[4] = new QAction("Save", 0);
        builtins[4]->setEnabled(false);
    }
    menus[0]->addAction(builtins[4]);

    if (!builtins[5])
        builtins[5] = new QAction("Sa&ve as..", 0);
    menus[0]->addAction(builtins[5]);

    if (!builtins[6]) {
        builtins[6] = new QAction(0);
        builtins[6]->setSeparator(true);
    }
    menus[0]->addAction(builtins[6]);

    if (!builtins[7])
        builtins[7] = new QAction("Prin&t", 0);
    menus[0]->addAction(builtins[7]);
}

void tst_QMenu::onHighlighted(QAction *action)
{
    highlighted = action;
    lastMenu = qobject_cast<QMenu*>(sender());
}

void tst_QMenu::onActivated(QAction *action)
{
    activated = action;
    lastMenu = qobject_cast<QMenu*>(sender());
}

void tst_QMenu::onStatusMessageChanged(const QString &s)
{
    statustip=s;
}

void tst_QMenu::addActionsAndClear()
{
    QCOMPARE(menus[0]->actions().size(), 0);
    createActions();
    QCOMPARE(menus[0]->actions().size(), 8);
    menus[0]->clear();
    QCOMPARE(menus[0]->actions().size(), 0);
}

static void testFunction0() {}
static void testFunction1(bool) {}

template <typename T>
struct ImplicitlyConvertibleTo {
    T t;
    operator const T() const { return t; }
    operator T() { return t; }
};

void tst_QMenu::addActionsConnect()
{
    // almost exhaustive check of addAction() overloads:
    // (text), (icon, text), (icon, text, shortcut), (text, shortcut)
    // each with a good sample of ways to QObject::connect() to
    // QAction::triggered(bool)
    QMenu menu;

    // don't just pass QString etc - that'd be too easy (think QStringBuilder)
    ImplicitlyConvertibleTo<QString> text = {QLatin1String("bla")};
    ImplicitlyConvertibleTo<QIcon> icon;

    const auto check = [&](auto &...args) { // don't need to perfectly-forward, only lvalues passed
        menu.addAction(args...);

        menu.addAction(args..., &menu, SLOT(deleteLater()));
        menu.addAction(args..., &menu, &QMenu::deleteLater);
        menu.addAction(args..., testFunction0);
        menu.addAction(args..., &menu, testFunction0);
        menu.addAction(args..., testFunction1);
        menu.addAction(args..., &menu, testFunction1);
        menu.addAction(args..., [&](bool b) { menu.setEnabled(b); });
        menu.addAction(args..., &menu, [&](bool b) { menu.setEnabled(b); });

        menu.addAction(args..., &menu, SLOT(deleteLater()), BobUI::QueuedConnection);
        menu.addAction(args..., &menu, &QMenu::deleteLater, BobUI::QueuedConnection);
        // doesn't exist: menu.addAction(args..., testFunction0, BobUI::QueuedConnection);
        menu.addAction(args..., &menu, testFunction0, BobUI::QueuedConnection);
        // doesn't exist: menu.addAction(args..., testFunction1, BobUI::QueuedConnection);
        menu.addAction(args..., &menu, testFunction1, BobUI::QueuedConnection);
        // doesn't exist: menu.addAction(args..., [&](bool b) { menu.setEnabled(b); }, BobUI::QueuedConnection);
        menu.addAction(args..., &menu, [&](bool b) { menu.setEnabled(b); }, BobUI::QueuedConnection);
    };
    const auto check1 = [&](auto &arg, auto &...args) {
        check(arg, args...);
        check(std::as_const(arg), args...);
    };
    const auto check2 = [&](auto &arg1, auto &arg2, auto &...args) {
        check1(arg1, arg2, args...);
        check1(arg1, std::as_const(arg2), args...);
    };
    [[maybe_unused]]
    const auto check3 = [&](auto &arg1, auto &arg2, auto &arg3) {
        check2(arg1, arg2, arg3);
        check2(arg1, arg2, std::as_const(arg3));
    };

    check1(text);
    check2(icon, text);
#ifndef BOBUI_NO_SHORTCUT
    ImplicitlyConvertibleTo<QKeySequence> keySequence = {BobUI::CTRL | BobUI::Key_C};
    check2(text, keySequence);
    check3(icon, text, keySequence);
#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_WARNING_PUSH
    BOBUI_WARNING_DISABLE_DEPRECATED
    menu.addAction(text, &menu, SLOT(deleteLater()), keySequence);
    menu.addAction(text, &menu, &QMenu::deleteLater, keySequence);
    menu.addAction(text, testFunction0, keySequence);
    menu.addAction(text, &menu, testFunction0, keySequence);
    menu.addAction(text, testFunction1, keySequence);
    menu.addAction(text, &menu, testFunction1, keySequence);
    menu.addAction(icon, text, &menu, SLOT(deleteLater()), keySequence);
    menu.addAction(icon, text, &menu, &QMenu::deleteLater, keySequence);
    menu.addAction(icon, text, testFunction0, keySequence);
    menu.addAction(icon, text, &menu, testFunction0, keySequence);
    menu.addAction(icon, text, testFunction1, keySequence);
    menu.addAction(icon, text, &menu, testFunction1, keySequence);
    BOBUI_WARNING_POP
#endif
#endif // !BOBUI_NO_SHORTCUT
}

void tst_QMenu::mouseActivation()
{
    QWidget topLevel;
    topLevel.resize(300, 200);
    centerOnScreen(&topLevel);
    QMenu menu(&topLevel);
    topLevel.show();
    menu.addAction("Menu Action");
    menu.move(topLevel.geometry().topRight() + QPoint(50, 0));
    menu.show();
    BOBUIest::mouseClick(&menu, BobUI::LeftButton, {}, menu.rect().center(), 300);
    QVERIFY(!menu.isVisible());

    //context menus can always be accessed with right click except on windows
    menu.show();
    BOBUIest::mouseClick(&menu, BobUI::RightButton, {}, menu.rect().center(), 300);
    QVERIFY(!menu.isVisible());

#ifdef Q_OS_WIN
    //on windows normal mainwindow menus Can only be accessed with left mouse button
    QMenuBar menubar;
    QMenu submenu("Menu");
    submenu.addAction("action");
    QAction *action = menubar.addMenu(&submenu);
    menubar.move(topLevel.geometry().topRight() + QPoint(300, 0));
    menubar.show();


    BOBUIest::mouseClick(&menubar, BobUI::LeftButton, {}, menubar.actionGeometry(action).center(), 300);
    QVERIFY(submenu.isVisible());
    BOBUIest::mouseClick(&submenu, BobUI::LeftButton, {}, QPoint(5, 5), 300);
    QVERIFY(!submenu.isVisible());

    BOBUIest::mouseClick(&menubar, BobUI::LeftButton, {}, menubar.actionGeometry(action).center(), 300);
    QVERIFY(submenu.isVisible());
    BOBUIest::mouseClick(&submenu, BobUI::RightButton, {}, QPoint(5, 5), 300);
    QVERIFY(submenu.isVisible());
#endif
}

void tst_QMenu::keyboardNavigation_data()
{
    BOBUIest::addColumn<BobUI::Key>("key");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<int>("expected_action");
    BOBUIest::addColumn<int>("expected_menu");
    BOBUIest::addColumn<bool>("init");
    BOBUIest::addColumn<bool>("expected_activated");
    BOBUIest::addColumn<bool>("expected_highlighted");

    //test up and down (order is important here)
    BOBUIest::newRow("data0") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 0 << 0 << true << false << true;
    BOBUIest::newRow("data1") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 2 << 0 << false << false << true; //skips the separator
    BOBUIest::newRow("data2") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 3 << 0 << false << false << true;

    if (QApplication::style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled))
        BOBUIest::newRow("data3_noMac") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 4 << 0 << false << false << true;
    else
        BOBUIest::newRow("data3_Mac") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 5 << 0 << false << false << true;
    BOBUIest::newRow("data4") << BobUI::Key(BobUI::Key_Up) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 3 << 0 << false << false << true;
    BOBUIest::newRow("data5") << BobUI::Key(BobUI::Key_Up) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 2 << 0 << false << false << true;
    BOBUIest::newRow("data6") << BobUI::Key(BobUI::Key_Right) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 8 << 1 << false << false << true;
    BOBUIest::newRow("data7") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 9 << 1 << false << false << true;
    BOBUIest::newRow("data8") << BobUI::Key(BobUI::Key_Escape) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 2 << 0 << false << false << false;
    BOBUIest::newRow("data9") << BobUI::Key(BobUI::Key_Down) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 3 << 0 << false << false<< true;
    BOBUIest::newRow("data10") << BobUI::Key(BobUI::Key_Return) << BobUI::KeyboardModifiers(BobUI::NoModifier) << 3 << 0 << false << true << false;

    if (QGuiApplication::platformName().compare(QLatin1String("xcb"), BobUI::CaseInsensitive)) {
        // Test shortcuts.
        BOBUIest::newRow("shortcut0") << BobUI::Key(BobUI::Key_V) << BobUI::KeyboardModifiers(BobUI::AltModifier) << 5 << 0 << true << true << false;
    }
}

void tst_QMenu::keyboardNavigation()
{
    QFETCH(BobUI::Key, key);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(int, expected_action);
    QFETCH(int, expected_menu);
    QFETCH(bool, init);
    QFETCH(bool, expected_activated);
    QFETCH(bool, expected_highlighted);

    if (init) {
        lastMenu = menus[0];
        lastMenu->clear();
        createActions();
        lastMenu->popup(QPoint(0, 0));
    }

    BOBUIest::keyClick(lastMenu, key, modifiers);
    if (expected_activated) {
#ifdef Q_OS_MAC
        QEXPECT_FAIL("shortcut0", "Shortcut navication fails, see BOBUIBUG-23684", Continue);
#endif
        QCOMPARE(activated, builtins[expected_action]);
#ifndef Q_OS_MAC
        QEXPECT_FAIL("shortcut0", "BOBUIBUG-22449: QMenu doesn't remove highlight if a menu item is activated by a shortcut", Abort);
#endif
        QCOMPARE(menus[expected_menu]->activeAction(), nullptr);
    } else {
        QCOMPARE(menus[expected_menu]->activeAction(), builtins[expected_action]);
    }

    if (expected_highlighted)
        QCOMPARE(menus[expected_menu]->activeAction(), highlighted);
    else
        QCOMPARE(highlighted, nullptr);
}

#ifdef Q_OS_MAC
BOBUI_BEGIN_NAMESPACE
extern bool bobui_tab_all_widgets(); // from qapplication.cpp
BOBUI_END_NAMESPACE
#endif

void tst_QMenu::focus()
{
    QMenu menu;
    menu.addAction("One");
    menu.addAction("Two");
    menu.addAction("Three");

#ifdef Q_OS_MAC
    if (!bobui_tab_all_widgets())
        QSKIP("Computer is currently set up to NOT tab to all widgets,"
             " this test assumes you can tab to all widgets");
#endif

    QWidget window;
    window.resize(300, 200);
    QPushButton button("Push me", &window);
    centerOnScreen(&window);
    window.show();
    QApplicationPrivate::setActiveWindow(&window);

    QVERIFY(button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), (QWidget *)&button);
    QCOMPARE(QApplication::activeWindow(), &window);
    menu.move(window.geometry().topRight() + QPoint(50, 0));
    menu.show();
    QVERIFY(button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), (QWidget *)&button);
    QCOMPARE(QApplication::activeWindow(), &window);
    menu.hide();
    QVERIFY(button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), (QWidget *)&button);
    QCOMPARE(QApplication::activeWindow(), &window);
}

void tst_QMenu::overrideMenuAction()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    //test the override menu action by first creating an action to which we set its menu
    QMainWindow w;
    w.resize(300, 200);
    w.menuBar()->setNativeMenuBar(false);
    centerOnScreen(&w);

    QAction *aFileMenu = new QAction("&File", &w);
    w.menuBar()->addAction(aFileMenu);

    QMenu *m = new QMenu(&w);
    QAction *menuaction = m->menuAction();
    connect(m, SIGNAL(triggered(QAction*)), SLOT(onActivated(QAction*)));
    aFileMenu->setMenu(m); //this sets the override menu action for the QMenu
    QCOMPARE(m->menuAction(), aFileMenu);

    // On Mac and Windows CE, we need to create native key events to test menu
    // action activation, so skip this part of the test.
#if BOBUI_CONFIG(shortcut) && !defined(Q_OS_DARWIN)
    QAction *aQuit = new QAction("Quit", &w);
    aQuit->setShortcut(QKeySequence("Ctrl+X"));
    m->addAction(aQuit);

    w.show();
    w.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));
    QVERIFY(w.hasFocus());

    //test of the action inside the menu
    BOBUIest::keyClick(&w, BobUI::Key_X, BobUI::ControlModifier);
    BOBUIRY_COMPARE(activated, aQuit);

    //test if the menu still pops out
    BOBUIest::keyClick(&w, BobUI::Key_F, BobUI::AltModifier);
    BOBUIRY_VERIFY(m->isVisible());
#endif // BOBUI_CONFIG(shortcut) && !Q_OS_DARWIN

    delete aFileMenu;

    //after the deletion of the override menu action,
    //the menu should have its default menu action back
    QCOMPARE(m->menuAction(), menuaction);
}

void tst_QMenu::statusTip()
{
    //check that the statustip of actions inserted into the menu are displayed
    QMainWindow w;
    w.resize(300, 200);
    centerOnScreen(&w);
    connect(w.statusBar(), SIGNAL(messageChanged(QString)), SLOT(onStatusMessageChanged(QString)));; //creates the status bar
    BOBUIoolBar tb;
    QAction a("main action", &tb);
    a.setStatusTip("main action");
    QMenu m(&tb);
    QAction subact("sub action", &m);
    subact.setStatusTip("sub action");
    m.addAction(&subact);
    a.setMenu(&m);
    tb.addAction(&a);

    w.addToolBar(&tb);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    QRect rect1 = tb.actionGeometry(&a);
    BOBUIoolButton *btn = qobject_cast<BOBUIoolButton*>(tb.childAt(rect1.center()));

    QVERIFY(btn != nullptr);

    //because showMenu calls QMenu::exec, we need to use a singleshot
    //to continue the test
    BOBUIimer timer;
    timer.setSingleShot(true);
    connect(&timer, &BOBUIimer::timeout, this, &tst_QMenu::onStatusTipTimer);
    timer.setInterval(200);
    timer.start();
    btn->showMenu();
    QVERIFY(m_onStatusTipTimerExecuted);
    QVERIFY(statustip.isEmpty());
}

//2nd part of the test
void tst_QMenu::onStatusTipTimer()
{
    QMenu *menu = qobject_cast<QMenu*>(QApplication::activePopupWidget());
    QVERIFY(menu != 0);
    QVERIFY(menu->isVisible());
    BOBUIest::keyClick(menu, BobUI::Key_Down);

    //we store the statustip to press escape in any case
    //otherwise, if the test fails it blocks (never gets out of QMenu::exec
    const QString st=statustip;

    menu->close(); //goes out of the menu

    QCOMPARE(st, QString("sub action"));
    QVERIFY(!menu->isVisible());
    m_onStatusTipTimerExecuted = true;
}

void tst_QMenu::widgetActionFocus()
{
    //test if the focus is correctly handled with a QWidgetAction
    QMenu m;
    QListWidget *l = new QListWidget(&m);
    for (int i = 1; i<3 ; i++)
        l->addItem(QStringLiteral("item" ) + QString::number(i));
    QWidgetAction *wa = new QWidgetAction(&m);
    wa->setDefaultWidget(l);
    m.addAction(wa);
    m.setActiveAction(wa);
    l->setFocus(); //to ensure it has primarily the focus
    QAction *menuitem1=m.addAction("menuitem1");
    QAction *menuitem2=m.addAction("menuitem2");

    m.popup(QPoint());

    QVERIFY(m.isVisible());
    QVERIFY(l->hasFocus());
    QVERIFY(l->currentItem());
    QCOMPARE(l->currentItem()->text(), QString("item1"));

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Down);
    QVERIFY(l->currentItem());
    QCOMPARE(l->currentItem()->text(), QString("item2"));

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Down);
    QVERIFY(m.hasFocus());
    QCOMPARE(m.activeAction(), menuitem1);

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Down);
    QVERIFY(m.hasFocus());
    QCOMPARE(m.activeAction(), menuitem2);

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Up);
    QVERIFY(m.hasFocus());
    QCOMPARE(m.activeAction(), menuitem1);

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Up);
    QVERIFY(l->hasFocus());
    QCOMPARE(m.activeAction(), (QAction *)wa);
}

void tst_QMenu::widgetActionFocusReason()
{
    class FocusReasonWidget : public QWidget
    {
    public:
        explicit FocusReasonWidget(QWidget *parent = nullptr)
            : QWidget(parent)
        {
            setFocusPolicy(BobUI::StrongFocus);
            // Give it some size so it doesn't skip over it.
            setFixedSize(QSize(10, 10));
        }
        BobUI::FocusReason focusInEventReason = BobUI::NoFocusReason;

    protected:
        void focusInEvent(QFocusEvent *event) override
        {
            QWidget::focusInEvent(event);
            focusInEventReason = event->reason();
        }
    };

    // test if the backtab/tab focus reason is correctly handled when going down and up.
    QWidget widget;
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));
    QMenu *menu = new QMenu(&widget);

    auto *w = new FocusReasonWidget(menu);

    QWidgetAction *wa = new QWidgetAction(menu);
    wa->setDefaultWidget(w);

    menu->setActiveAction(menu->addAction("Top"));
    menu->addAction(wa);
    menu->addAction("Bottom");

    menu->popup(QPoint());

    QVERIFY(menu->isVisible());
    QVERIFY(!w->hasFocus());

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Down);
    QVERIFY(w->hasFocus());
    QCOMPARE(w->focusInEventReason, BobUI::TabFocusReason);

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Down);
    QVERIFY(!w->hasFocus());

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Up);
    QVERIFY(w->hasFocus());
    QCOMPARE(w->focusInEventReason, BobUI::BacktabFocusReason);

    BOBUIest::keyClick(QApplication::focusWidget(), BobUI::Key_Up);
    QVERIFY(!w->hasFocus());
}

static QMenu *getTornOffMenu()
{
    for (QWidget *w : QApplication::allWidgets()) {
        if (w->isVisible() && w->inherits("BOBUIornOffMenu"))
            return static_cast<QMenu *>(w);
    }
    return nullptr;
}

void tst_QMenu::tearOff()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    QWidget widget;
    QScopedPointer<QMenu> menu(new QMenu(&widget));
    QVERIFY(!menu->isTearOffEnabled()); //default value
    menu->setTearOffEnabled(true);
    menu->setTitle(QLatin1String("Same &Menu"));
    menu->addAction("aaa");
    menu->addAction("bbb");
    QVERIFY(menu->isTearOffEnabled());

    widget.resize(300, 200);
    centerOnScreen(&widget);
    widget.show();
    widget.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));
    menu->popup(widget.geometry().topRight() + QPoint(50, 0));
    QVERIFY(BOBUIest::qWaitForWindowActive(menu.data()));
    QVERIFY(!menu->isTearOffMenuVisible());

    MenuMetrics mm(menu.data());
    const int tearOffOffset = mm.fw + mm.vmargin + mm.tearOffHeight / 2;

    BOBUIest::mouseClick(menu.data(), BobUI::LeftButton, {}, QPoint(10, tearOffOffset), 10);
    BOBUIRY_VERIFY(menu->isTearOffMenuVisible());
    QPointer<QMenu> torn = getTornOffMenu();
    QVERIFY(torn);
    QVERIFY(torn->isVisible());

    // Check menu title
    const QString cleanTitle = QPlatformTheme::removeMnemonics(menu->title()).trimmed();
    QCOMPARE(torn->windowTitle(), cleanTitle);

    // Change menu title and check again
    menu->setTitle(QLatin1String("Sample &Menu"));
    const QString newCleanTitle = QPlatformTheme::removeMnemonics(menu->title()).trimmed();
    QCOMPARE(torn->windowTitle(), newCleanTitle);

    // Clear menu title and check again
    menu->setTitle(QString());
    QCOMPARE(torn->windowTitle(), QString());

    menu->hideTearOffMenu();
    QVERIFY(!menu->isTearOffMenuVisible());
    QVERIFY(!torn->isVisible());

#ifndef BOBUI_NO_CURSOR
    // Test under-mouse positioning
    menu->showTearOffMenu();
    torn = getTornOffMenu();
    QVERIFY(torn);
    QVERIFY(torn->isVisible());
    QVERIFY(menu->isTearOffMenuVisible());
    // Some platforms include the window title bar in its geometry.
    BOBUIRY_COMPARE(torn->windowHandle()->position(), QCursor::pos());

    menu->hideTearOffMenu();
    QVERIFY(!menu->isTearOffMenuVisible());
    QVERIFY(!torn->isVisible());

    // Test custom positioning
    const QPoint &pos = QCursor::pos() / 2 + QPoint(10, 10);
    menu->showTearOffMenu(pos);
    torn = getTornOffMenu();
    QVERIFY(torn);
    QVERIFY(torn->isVisible());
    QVERIFY(menu->isTearOffMenuVisible());
    // Some platforms include the window title bar in its geometry.
    BOBUIRY_COMPARE(torn->windowHandle()->position(), pos);
#endif // BOBUI_NO_CURSOR
}

void tst_QMenu::submenuTearOffDontClose()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    QWidget widget;
    QMenu *menu = new QMenu(&widget);
    QVERIFY(!menu->isTearOffEnabled()); //default value
    menu->setTearOffEnabled(true);
    QVERIFY(menu->isTearOffEnabled());
    QMenu *submenu = new QMenu(&widget);
    submenu->addAction("aaa");
    submenu->addAction("bbb");
    QVERIFY(!submenu->isTearOffEnabled()); //default value
    submenu->setTearOffEnabled(true);
    QVERIFY(submenu->isTearOffEnabled());
    menu->addMenu(submenu);

    widget.resize(300, 200);
    centerOnScreen(&widget);
    widget.show();
    widget.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));
    // Show parent menu
    menu->popup(widget.geometry().topRight() + QPoint(50, 0));
    QVERIFY(BOBUIest::qWaitForWindowActive(menu));
    // Then its submenu
    const QRect submenuRect = menu->actionGeometry(menu->actions().at(0));
    const QPoint submenuPos(submenuRect.topLeft() + QPoint(3, 3));
    // Move then click to avoid the submenu moves from causing it to close
    BOBUIest::mouseMove(menu, submenuPos, 100);
    BOBUIest::mouseClick(menu, BobUI::LeftButton, {}, submenuPos, 100);
    QVERIFY(BOBUIest::qWaitFor([&]() { return submenu->window()->windowHandle(); }));
    QVERIFY(BOBUIest::qWaitForWindowActive(submenu));
    // Make sure we enter the submenu frame directly on the tear-off area
    BOBUIest::mouseMove(submenu, QPoint(10, 3), 100);
    if (submenu->style()->styleHint(QStyle::SH_Menu_SubMenuDontStartSloppyOnLeave)) {
        qWarning("Sloppy menu timer disabled by the style: %s", qPrintable(QApplication::style()->objectName()));
        // Submenu must get the enter event
        BOBUIRY_VERIFY(submenu->underMouse());
    } else {
        const int closeTimeout = submenu->style()->styleHint(QStyle::SH_Menu_SubMenuSloppyCloseTimeout);
        BOBUIest::qWait(closeTimeout + 100);
        // Menu must not disappear and it must get the enter event
        QVERIFY(submenu->isVisible());
        QVERIFY(submenu->underMouse());
    }
}

void tst_QMenu::layoutDirection()
{
    QMainWindow win;
    win.setLayoutDirection(BobUI::RightToLeft);
    win.resize(300, 200);
    centerOnScreen(&win);

    QMenu menu(&win);
    menu.addAction("foo");
    menu.move(win.geometry().topRight() + QPoint(50, 0));
    menu.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    QCOMPARE(menu.layoutDirection(), BobUI::RightToLeft);
    menu.close();

    menu.setParent(0);
    menu.move(win.geometry().topRight() + QPoint(50, 0));
    menu.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    QCOMPARE(menu.layoutDirection(), QApplication::layoutDirection());
    menu.close();

    //now the menubar
    QAction *action = win.menuBar()->addMenu(&menu);
    win.menuBar()->setActiveAction(action);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    QCOMPARE(menu.layoutDirection(), BobUI::RightToLeft);
}

void tst_QMenu::task208001_stylesheet()
{
    //test if it crash
    QMainWindow main;
    main.setStyleSheet("QMenu [title =\"File\"] { color: red;}");
    main.menuBar()->addMenu("File");
}

void tst_QMenu::activeSubMenuPosition()
{
    QPushButton lab("subMenuPosition test");

    QMenu *sub = new QMenu("Submenu", &lab);
    sub->addAction("Sub-Item1");
    QAction *subAction = sub->addAction("Sub-Item2");

    QMenu *main = new QMenu("Menu-Title", &lab);
    (void)main->addAction("Item 1");
    QAction *menuAction = main->addMenu(sub);
    (void)main->addAction("Item 3");
    (void)main->addAction("Item 4");

    main->setActiveAction(menuAction);
    sub->setActiveAction(subAction);
    main->popup(QPoint(200,200));

    QVERIFY(main->isVisible());
    QCOMPARE(main->activeAction(), menuAction);
    QVERIFY(sub->isVisible());
    QVERIFY(sub->pos() != QPoint(0,0));
    // well, it's enough to check the pos is not (0,0) but it's more safe
    // to check that submenu is to the right of the main menu too.
    QVERIFY(sub->pos().x() > main->pos().x());
    QCOMPARE(sub->activeAction(), subAction);
}

// BOBUIBUG-49588, BOBUIBUG-48396: activeSubMenuPositionExec() is the same as
// activeSubMenuPosition(), but uses QMenu::exec(), which produces a different
// sequence of events. Verify that the sub menu is positioned to the right of the
// main menu.
class SubMenuPositionExecMenu : public QMenu
{
    Q_OBJECT
public:
    SubMenuPositionExecMenu() : QMenu("Menu-Title"), m_timerId(-1), m_timerTick(0)
    {
        addAction("Item 1");
        m_subMenu = addMenu("Submenu");
        m_subAction = m_subMenu->addAction("Sub-Item1");
        setActiveAction(m_subMenu->menuAction());
    }

protected:
    void showEvent(QShowEvent *e) override
    {
        QVERIFY(m_subMenu->isVisible());
        QVERIFY2(m_subMenu->x() > x(),
                 (QByteArray::number(m_subMenu->x()) + ' ' + QByteArray::number(x())).constData());
        m_timerId = startTimer(50);
        QMenu::showEvent(e);
    }

    void timerEvent(BOBUIimerEvent *e) override
    {
        if (e->timerId() == m_timerId) {
            switch (m_timerTick++) {
            case 0:
                m_subMenu->close();
                break;
            case 1:
                close();
                break;
            }
        }
    }

private:
    int m_timerId;
    int m_timerTick;
    QMenu *m_subMenu;
    QAction *m_subAction;
};

void tst_QMenu::activeSubMenuPositionExec()
{
#ifdef Q_OS_ANDROID
    // BOBUIBUG-87424
    QSKIP("Android: This hangs. Figure out why.");
#endif
    SubMenuPositionExecMenu menu;
    menu.exec(QGuiApplication::primaryScreen()->availableGeometry().center());
}

void tst_QMenu::task242454_sizeHint()
{
#ifdef Q_OS_ANDROID
    // BOBUIBUG-87424
    QSKIP("Android: This hangs. Figure out why.");
#endif
    QMenu menu;
    QString s = QLatin1String("foo\nfoo\nfoo\nfoo");
    menu.addAction(s);
    QVERIFY(menu.sizeHint().width() > menu.fontMetrics().boundingRect(QRect(), BobUI::TextSingleLine, s).width());
}

class Menu : public QMenu
{
    Q_OBJECT
public slots:
    void clear()
    {
        QMenu::clear();
    }
};

void tst_QMenu::task176201_clear()
{
    //this test used to crash
    Menu menu;
    QAction *action = menu.addAction("test");
    menu.connect(action, SIGNAL(triggered()), SLOT(clear()));
    menu.popup(QPoint());
    BOBUIest::mouseClick(&menu, BobUI::LeftButton, {}, menu.rect().center());
}

void tst_QMenu::task250673_activeMultiColumnSubMenuPosition()
{
    class MyMenu : public QMenu
    {
    public:
        int columnCount() const { return QMenu::columnCount(); }
    };

    QMenu sub;

    if (sub.style()->styleHint(QStyle::SH_Menu_Scrollable, 0, &sub)) {
        //the style prevents the menus from getting columns
        QSKIP("the style doesn't support multiple columns, it makes the menu scrollable");
    }

    sub.addAction("Sub-Item1");
    QAction *subAction = sub.addAction("Sub-Item2");

    MyMenu main;
    main.addAction("Item 1");
    QAction *menuAction = main.addMenu(&sub);
    main.popup(QPoint(200,200));

    uint i = 2;
    while (main.columnCount() < 2) {
        main.addAction(QLatin1String("Item ") + QString::number(i));
        ++i;
        QVERIFY(i<1000);
    }
    main.setActiveAction(menuAction);
    sub.setActiveAction(subAction);

    QVERIFY(main.isVisible());
    QCOMPARE(main.activeAction(), menuAction);
    QVERIFY(sub.isVisible());
    QVERIFY(sub.pos().x() > main.pos().x());

    const int subMenuOffset = main.style()->pixelMetric(QStyle::PM_SubMenuOverlap, 0, &main);
    QVERIFY((sub.geometry().left() - subMenuOffset + 5) < main.geometry().right());
}

void tst_QMenu::task256918_setFont()
{
    QMenu menu;
    QAction *action = menu.addAction("foo");
    QFont f;
    f.setPointSize(30);
    action->setFont(f);
    centerOnScreen(&menu, QSize(120, 40));
    menu.show(); //ensures that the actiongeometry are calculated
    QVERIFY(menu.actionGeometry(action).height() > f.pointSize());
}

void tst_QMenu::menuSizeHint()
{
    QMenu menu;
    //this is a list of arbitrary strings so that we check the geometry
    for (auto str : {"trer", "ezrfgtgvqd", "sdgzgzerzerzer", "eerzertz", "er"})
        menu.addAction(str);

    const QMargins cm = menu.contentsMargins();
    const int panelWidth = menu.style()->pixelMetric(QStyle::PM_MenuPanelWidth, 0, &menu);
    const int hmargin = menu.style()->pixelMetric(QStyle::PM_MenuHMargin, 0, &menu),
    vmargin = menu.style()->pixelMetric(QStyle::PM_MenuVMargin, 0, &menu);

    int maxWidth =0;
    QRect result;
    for (QAction *action : menu.actions()) {
        maxWidth = qMax(maxWidth, menu.actionGeometry(action).width());
        result |= menu.actionGeometry(action);
        QCOMPARE(result.x(), cm.left() + hmargin + panelWidth);
        QCOMPARE(result.y(), cm.top() + vmargin + panelWidth);
    }

    QStyleOption opt(0);
    opt.rect = menu.rect();
    opt.state = QStyle::State_None;

    QSize resSize = QSize(result.x(), result.y()) + result.size() + QSize(hmargin + cm.right() + panelWidth, vmargin + cm.top() + panelWidth);

    resSize = menu.style()->sizeFromContents(QStyle::CT_Menu, &opt, resSize, &menu);

    QCOMPARE(resSize, menu.sizeHint());
}

class Menu258920 : public QMenu
{
    Q_OBJECT
public slots:
    void paintEvent(QPaintEvent *e) override
    {
        QMenu::paintEvent(e);
        painted = true;
    }

public:
    bool painted;
};

// Mouse move related signals for Windows Mobile unavailable
void tst_QMenu::task258920_mouseBorder()
{
    const QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    Menu258920 menu;
    QCursor::setPos(screenGeometry.topLeft());
    if (!BOBUIest::qWaitFor([screenGeometry]{ return QCursor::pos() == screenGeometry.topLeft(); }))
        QSKIP("Can't move cursor out of the way");
    // For styles which inherit from QWindowsStyle, styleHint(QStyle::SH_Menu_MouseTracking) is true.
    menu.setMouseTracking(true);
    QAction *action = menu.addAction("test");

    const QPoint center = screenGeometry.center();
    menu.popup(center);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    QRect actionRect = menu.actionGeometry(action);
    const QPoint actionCenter = actionRect.center();
    BOBUIest::mouseMove(&menu, actionCenter - QPoint(-10, 0));
    BOBUIest::mouseMove(&menu, actionCenter);
    BOBUIest::mouseMove(&menu, actionCenter + QPoint(10, 0));
    BOBUIRY_COMPARE(action, menu.activeAction());
    menu.painted = false;
    BOBUIest::mouseMove(&menu, QPoint(actionRect.center().x(), actionRect.bottom() + 1));
    BOBUIRY_COMPARE(static_cast<QAction*>(0), menu.activeAction());
    BOBUIRY_VERIFY(menu.painted);
}

void tst_QMenu::setFixedWidth()
{
    QMenu menu;
    menu.addAction("action");
    menu.setFixedWidth(300);
    //the sizehint should reflect the minimumwidth because the action will try to
    //get as much space as possible
    QCOMPARE(menu.sizeHint().width(), menu.minimumWidth());
}

void tst_QMenu::deleteActionInTriggered()
{
    // should not crash
    QMenu m;
    QObject::connect(&m, SIGNAL(triggered(QAction*)), this, SLOT(deleteAction(QAction*)));
    QPointer<QAction> a = m.addAction("action");
    a.data()->trigger();
    QVERIFY(!a);
}

class PopulateOnAboutToShowTestMenu : public QMenu {
    Q_OBJECT
public:
    explicit PopulateOnAboutToShowTestMenu(QWidget *parent = nullptr);

public slots:
    void populateMenu();
};

PopulateOnAboutToShowTestMenu::PopulateOnAboutToShowTestMenu(QWidget *parent) : QMenu(parent)
{
    connect(this, SIGNAL(aboutToShow()), this, SLOT(populateMenu()));
}

void PopulateOnAboutToShowTestMenu::populateMenu()
{
    // just adds 3 dummy actions and a separator.
    addAction("Foo");
    addAction("Bar");
    addAction("FooBar");
    addSeparator();
}

static inline QByteArray msgGeometryIntersects(const QRect &r1, const QRect &r2)
{
    QString result;
    QDebug(&result) << r1 << "intersects" << r2;
    return result.toLocal8Bit();
}

void tst_QMenu::pushButtonPopulateOnAboutToShow()
{
#ifdef Q_OS_MACOS
    QSKIP("Popup menus may partially overlap the button on macOS, and that's okey");
#endif

    QPushButton b("Test PushButton");
    b.setWindowFlags(BobUI::FramelessWindowHint | BobUI::X11BypassWindowManagerHint);

    QMenu *buttonMenu= new PopulateOnAboutToShowTestMenu(&b);
    b.setMenu(buttonMenu);
    const QScreen *scr = QGuiApplication::screenAt(b.pos());
    b.show();
    const QRect screen = scr->geometry();

    QRect desiredGeometry = b.geometry();
    desiredGeometry.moveTopLeft(QPoint(screen.x() + 10, screen.bottom() - b.height() - 5));

    b.setGeometry(desiredGeometry);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&b));

    if (b.geometry() != desiredGeometry) {
        // We are trying to put the button very close to the edge of the screen,
        // explicitly to test behavior when the popup menu goes off the screen.
        // However a modern window manager is quite likely to reject this requested geometry
        // (kwin in kde4 does, for example, since the button would probably appear behind
        // or partially behind the taskbar).
        // Your best bet is to run this test _without_ a WM.
        QSKIP("Your window manager won't allow a window against the bottom of the screen");
    }

    BOBUIest::mouseClick(&b, BobUI::LeftButton, BobUI::NoModifier, b.rect().center());
    QVERIFY(BOBUIest::qWaitForWindowExposed(buttonMenu));
    BOBUIest::qWait(300);
    buttonMenu->hide();
    QVERIFY2(!buttonMenu->geometry().intersects(b.geometry()), msgGeometryIntersects(buttonMenu->geometry(), b.geometry()));

    // note: we're assuming that, if we previously got the desired geometry, we'll get it here too
    b.move(10, screen.bottom()-buttonMenu->height()-5);
    BOBUIest::mouseClick(&b, BobUI::LeftButton, BobUI::NoModifier, b.rect().center());
    QVERIFY(BOBUIest::qWaitForWindowExposed(buttonMenu));
    BOBUIest::qWait(300);
    buttonMenu->hide();
    QVERIFY2(!buttonMenu->geometry().intersects(b.geometry()), msgGeometryIntersects(buttonMenu->geometry(), b.geometry()));
}

void tst_QMenu::BOBUIBUG7907_submenus_autoselect()
{
    QMenu menu("Test Menu");
    QMenu set1("Setting1");
    QMenu set2("Setting2");
    QMenu subset("Subsetting");
    subset.addAction("Values");
    set1.addMenu(&subset);
    menu.addMenu(&set1);
    menu.addMenu(&set2);
    centerOnScreen(&menu, QSize(120, 100));
    menu.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    BOBUIest::mouseClick(&menu, BobUI::LeftButton, BobUI::NoModifier, QPoint(5,5) );
    QVERIFY(!subset.isVisible());
}

void tst_QMenu::BOBUIBUG7411_submenus_activate()
{
    QMenu menu("Test Menu");
    QAction *act = menu.addAction("foo");
    QMenu sub1("&sub1");
    sub1.addAction("foo");
    sub1.setTitle("&sub1");
    QAction *act1 = menu.addMenu(&sub1);
    centerOnScreen(&menu, QSize(120, 100));
    menu.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    menu.setActiveAction(act);
    BOBUIest::keyPress(&menu, BobUI::Key_Down);
    QCOMPARE(menu.activeAction(), act1);
    QVERIFY(!sub1.isVisible());
    BOBUIest::keyPress(&menu, BobUI::Key_S);
    BOBUIRY_VERIFY(sub1.isVisible());
}

static bool isPlatformWayland()
{
    return !QGuiApplication::platformName().compare(QLatin1String("wayland"), BobUI::CaseInsensitive);
}

void tst_QMenu::BOBUIBUG30595_rtl_submenu()
{
    if (isPlatformWayland())
        QSKIP("Creating xdg_popups on Wayland requires real input events. Positions would be off.");

    QMenu menu("Test Menu");
    menu.setLayoutDirection(BobUI::RightToLeft);
    QMenu sub("&sub");
    sub.addAction("bar");
    sub.setTitle("&sub");
    menu.addMenu(&sub);
    centerOnScreen(&menu, QSize(120, 40));
    menu.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    BOBUIest::mouseClick(&menu, BobUI::LeftButton, BobUI::NoModifier, QPoint(5,5) );
    BOBUIRY_VERIFY(sub.isVisible());
    QVERIFY2(sub.pos().x() < menu.pos().x(), QByteArray::number(sub.pos().x()) + QByteArrayLiteral(" not less than ") + QByteArray::number(menu.pos().x()));
}

void tst_QMenu::BOBUIBUG20403_nested_popup_on_shortcut_trigger()
{
    QMenu menu("Test Menu");
    QMenu sub1("&sub1");
    QMenu subsub1("&subsub1");
    subsub1.addAction("foo");
    sub1.addMenu(&subsub1);
    menu.addMenu(&sub1);
    centerOnScreen(&menu, QSize(120, 100));
    menu.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    BOBUIest::keyPress(&menu, BobUI::Key_S);
    BOBUIest::qWait(100); // 20ms delay with previous behavior
    BOBUIRY_VERIFY(sub1.isVisible());
    QVERIFY(!subsub1.isVisible());
}

#ifndef Q_OS_MACOS
void tst_QMenu::click_while_dismissing_submenu()
{
    if (isPlatformWayland())
        QSKIP("Wayland: Creating (grabbing) popups requires real mouse events.");

    QMenu menu("Test Menu");
    QAction *action = menu.addAction("action");
    QMenu sub("&sub");
    sub.addAction("subaction");
    menu.addMenu(&sub);
    centerOnScreen(&menu, QSize(120, 100));
    menu.show();
    QSignalSpy spy(action, &QAction::triggered);
    QSignalSpy menuShownSpy(&sub, &QMenu::aboutToShow);
    QSignalSpy menuHiddenSpy(&sub, &QMenu::aboutToHide);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    QWindow *menuWindow = menu.windowHandle();
    QVERIFY(menuWindow);
    //go over the submenu, press, move and release over the top level action
    //this opens the submenu, move two times to emulate user interaction (d->motions > 0 in QMenu)
    BOBUIest::mouseMove(menuWindow, menu.rect().center() + QPoint(0,2));
    BOBUIest::mouseMove(menuWindow, menu.rect().center() + QPoint(1,3), 60);
    QVERIFY(menuShownSpy.wait());
    QVERIFY(sub.isVisible());
    QVERIFY(BOBUIest::qWaitForWindowExposed(&sub));
    //press over the submenu entry
    BOBUIest::mousePress(menuWindow, BobUI::LeftButton, {}, menu.rect().center() + QPoint(0, 2), 300);
    //move over the main action
    BOBUIest::mouseMove(menuWindow, menu.rect().center() - QPoint(0,2));
    QVERIFY(menuHiddenSpy.wait());
    //the submenu must have been hidden for the bug to be triggered
    QVERIFY(!sub.isVisible());
    BOBUIest::mouseRelease(menuWindow, BobUI::LeftButton, {}, menu.rect().center() - QPoint(0, 2), 300);
    QCOMPARE(spy.size(), 1);
}
#endif

class MyWidget : public QWidget
{
public:
    MyWidget(QWidget *parent) :
        QWidget(parent),
        move(0), enter(0), leave(0)
    {
        setMinimumSize(100, 100);
        setMouseTracking(true);
    }

    bool event(QEvent *e) override
    {
        switch (e->type()) {
        case QEvent::MouseMove:
            ++move;
            break;
        case QEvent::Enter:
            ++enter;
            break;
        case QEvent::Leave:
            ++leave;
            break;
        default:
            break;
        }
        return QWidget::event(e);
    }

    int move, enter, leave;
};

void tst_QMenu::BOBUIBUG47515_widgetActionEnterLeave()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");
    if (QGuiApplication::platformName() == QLatin1String("cocoa"))
        QSKIP("This test is meaningless on macOS, for additional info see BOBUIBUG-63031");

    const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    QRect geometry(QPoint(), availableGeometry.size() / 3);
    geometry.moveCenter(availableGeometry.center());
    QPoint pointOutsideMenu = geometry.bottomRight() - QPoint(5, 5);

    QMainWindow topLevel;
    topLevel.setGeometry(geometry);

    QMenuBar *menuBar = topLevel.menuBar();
    menuBar->setNativeMenuBar(false);
    QMenu *menu = menuBar->addMenu("Menu1");
    QMenu *submenu = menu->addMenu("Menu2");

    QWidgetAction *menuAction = new QWidgetAction(menu);
    MyWidget *w1 = new MyWidget(menu);
    menuAction->setDefaultWidget(w1);

    QWidgetAction *submenuAction = new QWidgetAction(submenu);
    MyWidget *w2 = new MyWidget(submenu);
    submenuAction->setDefaultWidget(w2);

    QAction *nextMenuAct = menu->addMenu(submenu);

    menu->addAction(menuAction);
    submenu->addAction(submenuAction);

    topLevel.show();
    topLevel.setWindowTitle(BOBUIest::currentTestFunction());
    QVERIFY(BOBUIest::qWaitForWindowActive(&topLevel));
    QWindow *topLevelWindow = topLevel.windowHandle();
    QVERIFY(topLevelWindow);

    // Root menu: Click on menu bar to open menu1
    {
        const QPoint menuActionPos = menuBar->mapTo(&topLevel, menuBar->actionGeometry(menu->menuAction()).center());
        BOBUIest::mouseClick(topLevelWindow, BobUI::LeftButton, BobUI::KeyboardModifiers(), menuActionPos);
        QVERIFY(BOBUIest::qWaitForWindowExposed(menu));

        w1->enter = 0;
        w1->leave = 0;
        QPoint w1Center = topLevel.mapFromGlobal(w1->mapToGlobal(w1->rect().center()));
        BOBUIest::mouseMove(topLevelWindow, w1Center);
        QVERIFY(w1->isVisible());
        BOBUIRY_COMPARE(w1->leave, 0);
        BOBUIRY_COMPARE(w1->enter, 1);

        // Check whether leave event is not delivered on mouse move
        w1->move = 0;
        BOBUIest::mouseMove(topLevelWindow, w1Center + QPoint(1, 1));
        BOBUIRY_COMPARE(w1->move, 1);
        BOBUIRY_COMPARE(w1->leave, 0);
        BOBUIRY_COMPARE(w1->enter, 1);

        BOBUIest::mouseMove(topLevelWindow, topLevel.mapFromGlobal(pointOutsideMenu));
        BOBUIRY_COMPARE(w1->leave, 1);
        BOBUIRY_COMPARE(w1->enter, 1);
    }

    // Submenu
    {
        menu->setActiveAction(nextMenuAct);
        QVERIFY(BOBUIest::qWaitForWindowExposed(submenu));

        QPoint w2Center = topLevel.mapFromGlobal(w2->mapToGlobal(w2->rect().center()));
        BOBUIest::mouseMove(topLevelWindow, w2Center);

        QVERIFY(w2->isVisible());
        BOBUIRY_COMPARE(w2->leave, 0);
        BOBUIRY_COMPARE(w2->enter, 1);

        // Check whether leave event is not delivered on mouse move
        w2->move = 0;
        BOBUIest::mouseMove(topLevelWindow, w2Center + QPoint(1, 1));
        BOBUIRY_COMPARE(w2->move, 1);
        BOBUIRY_COMPARE(w2->leave, 0);
        BOBUIRY_COMPARE(w2->enter, 1);

        BOBUIest::mouseMove(topLevelWindow, topLevel.mapFromGlobal(pointOutsideMenu));
        BOBUIRY_COMPARE(w2->leave, 1);
        BOBUIRY_COMPARE(w2->enter, 1);
    }
}

void tst_QMenu::BOBUIBUG_89082_actionTipsHide()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    QWidget widget;
    QMenu *menu = new QMenu(&widget);
    menu->addAction("aaa");
    menu->addAction("bbb");
    menu->addAction("ccc");
    menu->addAction("ddd");
    menu->addAction("eee");
    menu->addAction("fff");
    menu->setToolTipsVisible(true);

    auto menuActTip = menu->actions().first();
    QString tipFullName = "actionTip-this-is-a-long-action-and-show-the-full-name-by-tip";
    QFontMetrics m_fm = QFontMetrics(QAction().font());
    const QString &&elidedName = m_fm.elidedText(tipFullName, BobUI::ElideRight, 50);
    menuActTip->setText(elidedName);
    if (elidedName != tipFullName)
        menuActTip->setToolTip(tipFullName);

    widget.resize(300, 200);
    centerOnScreen(&widget);
    widget.show();
#if BOBUI_CONFIG(cursor)
    QCursor::setPos(widget.screen()->availableGeometry().topLeft() + QPoint(10, 10));
#endif
    widget.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));
    menu->popup(widget.geometry().topRight() + QPoint(50, 0));
    QVERIFY(BOBUIest::qWaitForWindowExposed(menu));
    auto menuWindow = menu->windowHandle();
    QVERIFY(menuWindow != nullptr);

    auto actionZero = menu->actions().at(0);
    auto actionOne = menu->actions().at(1);
    auto actionFive = menu->actions().at(5);
    const QRect submenuRect0 = menu->actionGeometry(actionZero);
    const QPoint submenuPos0(submenuRect0.topLeft() + QPoint(3, 3));

    const QRect submenuRect1 = menu->actionGeometry(actionOne);
    const QPoint submenuPos1(submenuRect1.topLeft() + QPoint(3, 3));

    const QRect submenuRect5 = menu->actionGeometry(actionFive);
    const QPoint submenuPos5(submenuRect5.topLeft() + QPoint(10, 3));

    BOBUIest::mouseMove(menuWindow, submenuPos1);
    BOBUIest::mouseMove(menuWindow, submenuPos0); //show the tip
    BOBUIRY_COMPARE_WITH_TIMEOUT(BOBUIoolTip::text(), tipFullName, 1000);

    //Move to the fifth action without prompting
    BOBUIest::mouseMove(menuWindow, submenuPos5);
    //The previous tip was hidden, but now is a new tip to get text, So there should be no content
    BOBUIRY_COMPARE_WITH_TIMEOUT(BOBUIoolTip::text(), QString(), 1000);
}

void tst_QMenu::BOBUIBUG8122_widgetActionCrashOnClose()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");
    if (QGuiApplication::platformName() == QLatin1String("cocoa"))
        QSKIP("See BOBUIBUG-63031");

    const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    QRect geometry(QPoint(), availableGeometry.size() / 3);
    geometry.moveCenter(availableGeometry.center());
    QPoint pointOutsideMenu = geometry.bottomRight() - QPoint(5, 5);

    QMainWindow topLevel;
    topLevel.setGeometry(geometry);

    auto menuBar = topLevel.menuBar();
    auto menu = menuBar->addMenu("Menu");
    auto wAct = new QWidgetAction(menu);
    auto spinBox1 = new QSpinBox(menu);
    wAct->setDefaultWidget(spinBox1);
    menu->addAction(wAct);
    auto subMenu = menu->addMenu("Submenu");
    auto nextMenuAct = menu->addMenu(subMenu);
    auto wAct2 = new QWidgetAction(menu);
    auto spinBox2 = new QSpinBox(menu);
    wAct2->setDefaultWidget(spinBox2);
    subMenu->addAction(wAct2);
    QObject::connect(spinBox2, &QSpinBox::editingFinished, menu, &QMenu::hide);

    topLevel.show();
    topLevel.setWindowTitle(BOBUIest::currentTestFunction());
    QVERIFY(BOBUIest::qWaitForWindowActive(&topLevel));
    QWindow *topLevelWindow = topLevel.windowHandle();
    QVERIFY(topLevelWindow);

    const QPoint menuActionPos = menuBar->mapTo(&topLevel, menuBar->actionGeometry(menu->menuAction()).center());
    BOBUIest::mouseClick(topLevelWindow, BobUI::LeftButton, BobUI::KeyboardModifiers(), menuActionPos);
    QVERIFY(BOBUIest::qWaitForWindowExposed(menu));

    QPoint w1Center = topLevel.mapFromGlobal(spinBox1->mapToGlobal(spinBox1->rect().center()));
    BOBUIest::mouseClick(topLevelWindow, BobUI::LeftButton, BobUI::KeyboardModifiers(), w1Center);
    menu->setActiveAction(nextMenuAct);
    QVERIFY(BOBUIest::qWaitForWindowExposed(subMenu));

    QPoint w2Center = topLevel.mapFromGlobal(spinBox2->mapToGlobal(spinBox2->rect().center()));
    BOBUIest::mouseClick(topLevelWindow, BobUI::LeftButton, BobUI::KeyboardModifiers(), w2Center);
    BOBUIest::mouseMove(topLevelWindow, topLevel.mapFromGlobal(pointOutsideMenu));
    BOBUIRY_VERIFY(menu->isHidden());
}

/*!
    Test that a QWidgetAction that fires closes the menus that it is in.
*/
void tst_QMenu::widgetActionTriggerClosesMenu()
{
    class ButtonAction : public QWidgetAction
    {
    public:
        ButtonAction()
        : QWidgetAction(nullptr)
        {}

        void click()
        {
            if (pushButton)
                pushButton->click();
        }

    protected:
        QWidget *createWidget(QWidget *parent) override
        {
            QPushButton *button = new QPushButton(QLatin1String("Button"), parent);
            connect(button, &QPushButton::clicked, this, &QAction::trigger);

            if (!pushButton)
                pushButton = button;
            return button;
        }

    private:
        QPointer<QPushButton> pushButton;
    };

    QMenu menu;
    QMenu submenu;

    int menuTriggeredCount = 0;
    int menuAboutToHideCount = 0;
    QAction *actionTriggered = nullptr;

    connect(&menu, &QMenu::triggered, this, [&](QAction *action){
        ++menuTriggeredCount;
        actionTriggered = action;
    });
    connect (&menu, &QMenu::aboutToHide, this, [&](){
        ++menuAboutToHideCount;
    });

    QAction regularAction(QLatin1String("Action"));
    ButtonAction widgetAction;

    submenu.addAction(&regularAction);
    submenu.addAction(&widgetAction);

    menu.addMenu(&submenu);
    menu.addAction(&regularAction);
    menu.addAction(&widgetAction);

    menu.popup(QPoint(200,200));
    submenu.popup(QPoint(250,250));
    if (!BOBUIest::qWaitForWindowExposed(&menu) || !BOBUIest::qWaitForWindowExposed(&submenu))
        QSKIP("Failed to show menus, aborting test");

    regularAction.trigger();
    QVERIFY(menu.isVisible());
    QVERIFY(submenu.isVisible());
    QCOMPARE(menuTriggeredCount, 1);
    QCOMPARE(actionTriggered, &regularAction);
    menuTriggeredCount = 0;
    actionTriggered = nullptr;

    widgetAction.click();
    QVERIFY(!menu.isVisible());
    QVERIFY(!submenu.isVisible());
    QCOMPARE(menuTriggeredCount, 1);
    QCOMPARE(menuAboutToHideCount, 1);
    QCOMPARE(actionTriggered, &widgetAction);
}

void tst_QMenu::widgetActionContextMenu() // BOBUIBUG-134757
{
    QPushButton openButton("open");
    QMenu *menu = new QMenu(&openButton);
    QVBoxLayout *layout = new QVBoxLayout;
    QWidgetAction widgetAction(menu);
    QWidget menuWidget(menu);
    QPlainTextEdit edit;
    openButton.setMenu(menu);
    menuWidget.setLayout(layout);
    widgetAction.setDefaultWidget(&menuWidget);
    menu->addAction(&widgetAction);
    layout->addWidget(&edit);

    openButton.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&openButton));

    // Click the QPushButton to open its menu
    BOBUIest::mouseClick(&openButton, BobUI::LeftButton);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menuWidget));
    QWindow *popupWindow = edit.window()->windowHandle();
    QVERIFY(popupWindow);
    QCOMPARE(QApplication::activePopupWidget(), menu);

    // Right-click the QPlainTextEdit to open its context menu
    BOBUIest::mouseClick(popupWindow, BobUI::RightButton);
    QVERIFY(qobject_cast<QMenu *>(QApplication::activePopupWidget()));
    QCOMPARE_NE(QApplication::activePopupWidget(), menu);
}

void tst_QMenu::transientParent()
{
    QMainWindow window;
    window.resize(480, 320);
    window.menuBar()->setNativeMenuBar(false);
    centerOnScreen(&window);

    QMenu *fileMenu = new QMenu("&File", &window);
    QAction *exitAct = new QAction("Exit", &window);
    fileMenu->addAction(exitAct);

    QMenu *editMenu = new QMenu("&Edit", &window);
    QAction *undoAct = new QAction("Undo", &window);
    editMenu->addAction(undoAct);

    QMenuBar *menuBar = new QMenuBar;
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    window.setMenuBar(menuBar);

    QMenu *bookmarksMenu = new QMenu(&window);
    bookmarksMenu->addAction("This is KDE!");

    QMenu *contextMenu = new QMenu(&window);
    QAction *bookmarksAction = contextMenu->addAction("&Bookmarks");
    bookmarksAction->setMenu(bookmarksMenu);

    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));
    QWindow *topLevel = window.windowHandle();
    QVERIFY(topLevel);

    // Show the standalone bookmarks menu. It should be a child of the main window.
    bookmarksMenu->popup(window.geometry().center());
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(bookmarksMenu));
    QVERIFY(bookmarksMenu->windowHandle());
    QCOMPARE(bookmarksMenu->windowHandle()->transientParent(), topLevel);
    bookmarksMenu->close();

    // Show the bookmarks menu attached to the context menu. Even though the bookmarks menu is
    // a child of the main window, its transient parent will be the context menu.
    contextMenu->popup(window.geometry().center());
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(contextMenu));
    QVERIFY(contextMenu->windowHandle());
    QCOMPARE(contextMenu->windowHandle()->transientParent(), topLevel);

    contextMenu->setActiveAction(bookmarksAction);
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(bookmarksMenu));
    QVERIFY(bookmarksMenu->windowHandle());
    QCOMPARE(bookmarksMenu->windowHandle()->transientParent(), contextMenu->windowHandle());
    contextMenu->close();

    // Show the standalone bookmarks menu. Its transient parent will be the main window again.
    bookmarksMenu->popup(window.geometry().center());
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(bookmarksMenu));
    QVERIFY(bookmarksMenu->windowHandle());
    QCOMPARE(bookmarksMenu->windowHandle()->transientParent(), topLevel);
    bookmarksMenu->close();

    // On Mac, we need to create native key events to test menu
    // action activation, so skip this part of the test.
#if BOBUI_CONFIG(shortcut) && !defined(Q_OS_DARWIN)
    window.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&window));
    QVERIFY(window.hasFocus());

    BOBUIest::keyPress(&window, BobUI::Key_F, BobUI::AltModifier);
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(fileMenu));
    QVERIFY(fileMenu->windowHandle());
    QCOMPARE(fileMenu->windowHandle()->transientParent(), topLevel);
    BOBUIest::keyRelease(fileMenu, BobUI::Key_F, BobUI::AltModifier);

    BOBUIest::keyPress(fileMenu, BobUI::Key_E, BobUI::AltModifier);
    BOBUIRY_VERIFY(BOBUIest::qWaitForWindowExposed(editMenu));
    QVERIFY(editMenu->windowHandle());
    QCOMPARE(editMenu->windowHandle()->transientParent(), topLevel);
    BOBUIest::keyRelease(editMenu, BobUI::Key_E, BobUI::AltModifier);
#endif // BOBUI_CONFIG(shortcut) && !Q_OS_DARWIN
}

class MyMenu : public QMenu
{
    Q_OBJECT
public:
    MyMenu() : m_currentIndex(0)
    {
        for (int i = 0; i < 2; ++i)
            dialogActions[i] = addAction(QLatin1String("dialog ") + QString::number(i), dialogs + i, SLOT(exec()));
    }

    void activateAction(int index)
    {
        m_currentIndex = index;
        popup(QPoint());
        QVERIFY(BOBUIest::qWaitForWindowExposed(this));
        setActiveAction(dialogActions[index]);
        BOBUIimer::singleShot(500, this, SLOT(checkVisibility()));
        BOBUIest::keyClick(this, BobUI::Key_Enter); //activation
    }

public slots:
    void activateLastAction()
    {
        activateAction(1);
    }

    void checkVisibility()
    {
        BOBUIRY_VERIFY(dialogs[m_currentIndex].isVisible());
        if (m_currentIndex == 1) {
            QApplication::closeAllWindows(); //this is the end of the test
        }
    }

private:
    QAction *dialogActions[2];
    QDialog dialogs[2];
    int m_currentIndex;
};

void tst_QMenu::BOBUIBUG_10735_crashWithDialog()
{
    MyMenu menu;

    BOBUIimer::singleShot(1000, &menu, SLOT(activateLastAction()));
    menu.activateAction(0);
}

#ifdef Q_OS_MAC
void tst_QMenu::BOBUIBUG_37933_ampersands_data()
{
    BOBUIest::addColumn<QString>("title");
    BOBUIest::addColumn<QString>("visibleTitle");
    BOBUIest::newRow("simple") << QString("Test") << QString("Test");
    BOBUIest::newRow("ampersand") << QString("&Test") << QString("Test");
    BOBUIest::newRow("double_ampersand") << QString("&Test && more") << QString("Test & more");
    BOBUIest::newRow("ampersand_in_parentheses") << QString("Test(&T) (&&) more") << QString("Test (&) more");
    BOBUIest::newRow("ampersand_in_parentheses_after_space") << QString("Test (&T)") << QString("Test");
    BOBUIest::newRow("ampersand_in_parentheses_after_spaces") << QString("Test  (&T)") << QString("Test");
    BOBUIest::newRow("ampersand_in_parentheses_before_space") << QString("Test(&T) ") << QString("Test ");
    BOBUIest::newRow("only_ampersand_in_parentheses") << QString("(&T)") << QString("");
    BOBUIest::newRow("only_ampersand_in_parentheses_after_space") << QString(" (&T)") << QString("");
    BOBUIest::newRow("parentheses_after_space") << QString(" (Dummy)") << QString(" (Dummy)");
    BOBUIest::newRow("ampersand_after_space") << QString("About &BobUI Project") << QString("About BobUI Project");
}

void tst_qmenu_BOBUIBUG_37933_ampersands();

void tst_QMenu::BOBUIBUG_37933_ampersands()
{
    // external in .mm file
    tst_qmenu_BOBUIBUG_37933_ampersands();
}
#endif

void tst_QMenu::BOBUIBUG_56917_wideMenuSize()
{
    // menu shouldn't to take on full screen height when menu width is larger than screen width
    QMenu menu;
    QString longString;
    longString.fill(QLatin1Char('Q'), 3000);
    menu.addAction(longString);
    QSize menuSizeHint = menu.sizeHint();
    menu.popup(QPoint());
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
    QVERIFY(menu.isVisible());
    QVERIFY(menu.height() <= menuSizeHint.height());
}

void tst_QMenu::BOBUIBUG_56917_wideMenuScreenNumber()
{
    if (QApplication::styleHints()->showIsFullScreen())
        QSKIP("The platform defaults to windows being fullscreen.");
    // menu must appear on the same screen where show action is triggered
    QString longString;
    longString.fill(QLatin1Char('Q'), 3000);

    const QList<QScreen *> screens = QGuiApplication::screens();
    for (QScreen *screen : screens) {
        QMenu menu;
        menu.addAction(longString);
        menu.popup(screen->geometry().center());
        QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
        QVERIFY(menu.isVisible());
        QCOMPARE(QGuiApplication::screenAt(menu.pos()), screen);
    }
}

void tst_QMenu::BOBUIBUG_56917_wideSubmenuScreenNumber()
{
    if (QApplication::styleHints()->showIsFullScreen())
        QSKIP("The platform defaults to windows being fullscreen.");
    // submenu must appear on the same screen where its parent menu is shown
    QString longString;
    longString.fill(QLatin1Char('Q'), 3000);

    const QList<QScreen *> screens = QGuiApplication::screens();
    for (QScreen *screen : screens) {
        QMenu menu;
        QMenu submenu("Submenu");
        submenu.addAction(longString);
        QAction *action = menu.addMenu(&submenu);
        menu.popup(screen->geometry().center());
        QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));
        QVERIFY(menu.isVisible());
        BOBUIest::mouseClick(&menu, BobUI::LeftButton, {}, menu.actionGeometry(action).center());
        BOBUIest::qWait(100);
        QVERIFY(BOBUIest::qWaitForWindowExposed(&submenu));
        QVERIFY(submenu.isVisible());
        QCOMPARE(QGuiApplication::screenAt(submenu.pos()), screen);
    }
}

void tst_QMenu::menuSize_Scrolling_data()
{
    BOBUIest::addColumn<int>("numItems");
    BOBUIest::addColumn<int>("topMargin");
    BOBUIest::addColumn<int>("bottomMargin");
    BOBUIest::addColumn<int>("leftMargin");
    BOBUIest::addColumn<int>("rightMargin");
    BOBUIest::addColumn<int>("topPadding");
    BOBUIest::addColumn<int>("bottomPadding");
    BOBUIest::addColumn<int>("leftPadding");
    BOBUIest::addColumn<int>("rightPadding");
    BOBUIest::addColumn<int>("border");
    BOBUIest::addColumn<bool>("scrollable");
    BOBUIest::addColumn<bool>("tearOff");

    // test data
    // a single column and non-scrollable menu with contents margins + border
    BOBUIest::newRow("data0") << 5 << 2 << 2 << 2 << 2 << 0 << 0 << 0 << 0 << 2 << false << false;
    // a single column and non-scrollable menu with paddings + border
    BOBUIest::newRow("data1") << 5 << 0 << 0 << 0 << 0 << 2 << 2 << 2 << 2 << 2 << false << false;
    // a single column and non-scrollable menu with contents margins + paddings + border
    BOBUIest::newRow("data2") << 5 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << false << false;
    // a single column and non-scrollable menu with contents margins + paddings + border + tear-off
    BOBUIest::newRow("data3") << 5 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << false << true;
    // a multi-column menu with contents margins + border
    BOBUIest::newRow("data4") << 80 << 2 << 2 << 2 << 2 << 0 << 0 << 0 << 0 << 2 << false << false;
    // a multi-column menu with paddings + border
    BOBUIest::newRow("data5") << 80 << 0  << 0 << 0 << 0 << 2 << 2 << 2 << 2 << 2 << false << false;
    // a multi-column menu with contents margins + paddings + border
    BOBUIest::newRow("data6") << 80 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << false << false;
    // a multi-column menu with contents margins + paddings + border + tear-off
    BOBUIest::newRow("data7") << 80 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << false << true;
    // a scrollable menu with contents margins + border
    BOBUIest::newRow("data8") << 80 << 2 << 2 << 2 << 2 << 0 << 0 << 0 << 0 << 2 << true << false;
    // a scrollable menu with paddings + border
    BOBUIest::newRow("data9") << 80 << 0 << 0 << 0 << 0 << 2 << 2 << 2 << 2 << 2 << true << false;
    // a scrollable menu with contents margins + paddings + border
    BOBUIest::newRow("data10") << 80 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << true << false;
    // a scrollable menu with contents margins + paddings + border + tear-off
    BOBUIest::newRow("data11") << 80 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << true << true;
}

void tst_QMenu::menuSize_Scrolling()
{
    class TestMenu : public QMenu
    {
    public:
        struct ContentsMargins
        {
            ContentsMargins(int l, int t, int r, int b)
                : left(l), top(t), right(r), bottom(b) {}
            int left;
            int top;
            int right;
            int bottom;
        };

        struct MenuPaddings
        {
            MenuPaddings(int l, int t, int r, int b)
                : left(l), top(t), right(r), bottom(b) {}
            int left;
            int top;
            int right;
            int bottom;
        };

        TestMenu(int numItems, const ContentsMargins &margins, const MenuPaddings &paddings,
                 int border, bool scrollable, bool tearOff)
            : QMenu("Test Menu"),
              m_numItems(numItems),
              m_scrollable(scrollable),
              m_tearOff(tearOff)
        {
            init(margins, paddings, border);
        }

        ~TestMenu() {}

    private:
        void showEvent(QShowEvent *e) override
        {
            QVERIFY(actions().size() == m_numItems);

            int hmargin = style()->pixelMetric(QStyle::PM_MenuHMargin, nullptr, this);
            int fw = style()->pixelMetric(QStyle::PM_MenuPanelWidth, nullptr, this);
            const QMargins cm = contentsMargins();
            QRect lastItem = actionGeometry(actions().at(actions().size() - 1));
            QSize s = size();
            if (!QGuiApplication::platformName().compare(QLatin1String("minimal"), BobUI::CaseInsensitive)
                || !QGuiApplication::platformName().compare(QLatin1String("offscreen"), BobUI::CaseInsensitive)) {
                qWarning("Skipping test on minimal/offscreen platforms - BOBUIBUG-73522");
                QMenu::showEvent(e);
                return;
            }

            QCOMPARE( s.width(), lastItem.right() + fw + hmargin + cm.right() + 1);
            QMenu::showEvent(e);
        }

        void init(const ContentsMargins &margins, const MenuPaddings &paddings, int border)
        {
            setLayoutDirection(BobUI::LeftToRight);

            setTearOffEnabled(m_tearOff);
            setContentsMargins(margins.left, margins.top, margins.right, margins.bottom);
            QString cssStyle("QMenu {menu-scrollable: ");
            cssStyle += (m_scrollable ? QString::number(1) : QString::number(0));
            cssStyle += "; border: ";
            cssStyle += QString::number(border);
            cssStyle += "px solid black; padding: ";
            cssStyle += QString::number(paddings.top);
            cssStyle += "px ";
            cssStyle += QString::number(paddings.right);
            cssStyle += "px ";
            cssStyle += QString::number(paddings.bottom);
            cssStyle += "px ";
            cssStyle += QString::number(paddings.left);
            cssStyle += "px;}";
            setStyleSheet(cssStyle);
            for (int i = 1; i <= m_numItems; i++)
                addAction("MenuItem " + QString::number(i));
        }

    private:
        int m_numItems;
        bool m_scrollable;
        bool m_tearOff;
    };

    QFETCH(int, numItems);
    QFETCH(int, topMargin);
    QFETCH(int, bottomMargin);
    QFETCH(int, leftMargin);
    QFETCH(int, rightMargin);
    QFETCH(int, topPadding);
    QFETCH(int, bottomPadding);
    QFETCH(int, leftPadding);
    QFETCH(int, rightPadding);
    QFETCH(int, border);
    QFETCH(bool, scrollable);
    QFETCH(bool, tearOff);

    qApp->setAttribute(BobUI::AA_DontUseNativeMenuBar);

    TestMenu::ContentsMargins margins(leftMargin, topMargin, rightMargin, bottomMargin);
    TestMenu::MenuPaddings paddings(leftPadding, topPadding, rightPadding, bottomPadding);
    TestMenu menu(numItems, margins, paddings, border, scrollable, tearOff);
    menu.popup(QPoint(0,0));
    centerOnScreen(&menu);
    QVERIFY(BOBUIest::qWaitForWindowExposed(&menu));

    QList<QAction *> actions = menu.actions();
    QCOMPARE(actions.size(), numItems);

    MenuMetrics mm(&menu);
    BOBUIest::keyClick(&menu, BobUI::Key_Home);
    BOBUIRY_COMPARE(menu.actionGeometry(actions.first()).y(), mm.fw + mm.vmargin + topMargin + (tearOff ? mm.tearOffHeight : 0));
    QCOMPARE(menu.actionGeometry(actions.first()).x(), mm.fw + mm.hmargin + leftMargin);

    if (!scrollable)
        return;

    BOBUIest::keyClick(&menu, BobUI::Key_End);
    BOBUIRY_COMPARE(menu.actionGeometry(actions.last()).right(),
                 menu.width() - mm.fw - mm.hmargin - leftMargin - 1);
    QCOMPARE(menu.actionGeometry(actions.last()).bottom(),
             menu.height() - mm.fw - mm.vmargin - bottomMargin - 1);
}

void tst_QMenu::tearOffMenuNotDisplayed()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");
    QWidget widget;
    QScopedPointer<QMenu> menu(new QMenu(&widget));
    menu->setTearOffEnabled(true);
    QVERIFY(menu->isTearOffEnabled());

    menu->setStyleSheet("QMenu { menu-scrollable: 1 }");
    for (int i = 0; i < 80; i++)
        menu->addAction(QString::number(i));

    widget.resize(300, 200);
    centerOnScreen(&widget);
    widget.show();
    widget.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));
    menu->popup(widget.geometry().topRight() + QPoint(50, 0));
    QVERIFY(BOBUIest::qWaitForWindowActive(menu.data()));
    QVERIFY(!menu->isTearOffMenuVisible());

    MenuMetrics mm(menu.data());
    const int tearOffOffset = mm.fw + mm.vmargin + mm.tearOffHeight / 2;

    BOBUIest::mouseClick(menu.data(), BobUI::LeftButton, {}, QPoint(10, tearOffOffset), 10);
    BOBUIRY_VERIFY(menu->isTearOffMenuVisible());
    QPointer<QMenu> torn = getTornOffMenu();
    QVERIFY(torn);
    QVERIFY(torn->isVisible());
    QVERIFY(torn->minimumWidth() >=0 && torn->minimumWidth() < QWIDGETSIZE_MAX);

    menu->hideTearOffMenu();
    QVERIFY(!menu->isTearOffMenuVisible());
    QVERIFY(!torn->isVisible());
}

void tst_QMenu::BOBUIBUG_61039_menu_shortcuts()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    QObject reaper;

    QAction *actionKamen = new QAction("Action Kamen", &reaper);
#if BOBUI_CONFIG(shortcut)
    actionKamen->setShortcut(QKeySequence(QLatin1String("K")));
#endif
    QAction *actionJoe = new QAction("Action Joe", &reaper);
#if BOBUI_CONFIG(shortcut)
    actionJoe->setShortcut(QKeySequence(QLatin1String("Ctrl+J")));
#endif

    QMenu menu;
    menu.addAction(actionKamen);
    menu.addAction(actionJoe);
    QVERIFY(!menu.platformMenu());

    QWidget widget;
    widget.addAction(menu.menuAction());
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));

    QSignalSpy actionKamenSpy(actionKamen, &QAction::triggered);
    BOBUIest::keyClick(&widget, BobUI::Key_K);
    BOBUIRY_COMPARE(actionKamenSpy.size(), 1);

    QSignalSpy actionJoeSpy(actionJoe, &QAction::triggered);
    BOBUIest::keyClick(&widget, BobUI::Key_J, BobUI::ControlModifier);
    BOBUIRY_COMPARE(actionJoeSpy.size(), 1);
}

void tst_QMenu::screenOrientationChangedCloseMenu()
{
     QMenu menu;
     menu.addAction("action1");
     menu.show();

     BOBUIRY_COMPARE(menu.isVisible(),true);

     BobUI::ScreenOrientation orientation = menu.screen()->orientation() == BobUI::PortraitOrientation ? BobUI::LandscapeOrientation : BobUI::PortraitOrientation;
     QScreenOrientationChangeEvent event(menu.screen(), orientation);
     QCoreApplication::sendEvent(QCoreApplication::instance(), &event);

     BOBUIRY_COMPARE(menu.isVisible(),false);
}

/*
    Verify that deleting the menu in a slot connected to an
    action's triggered signal doesn't crash.
    BOBUIBUG-106718
*/
void tst_QMenu::deleteWhenTriggered()
{
    QPointer<QMenu> menu = new QMenu;
    QAction *action = menu->addAction("Action", [&menu]{
        delete menu;
    });
    menu->popup(QGuiApplication::primaryScreen()->availableGeometry().center());
    menu->setActiveAction(action);
    BOBUIest::keyClick(menu, BobUI::Key_Return);
    BOBUIRY_VERIFY(!menu);
}

/*
    QMenu uses the caused-stack to create the parent/child relationship
    for tear-off menus. Since BOBUIornOffMenu set the DeleteOnClose flag, closing a
    tear-off in the parent chain will result in a null-pointer in the caused-stack.
    Verify that we don't crash when traversing the chain, as reported in BOBUIBUG-112217.

    The test has to open the submenus by hovering of the menu action, otherwise
    the caused-stack remains empty and the issue doesn't reproduce. Due to QMenu's
    timing and "sloppiness", we need to move the mouse within the action, with some
    waiting and event processing in between to trigger the opening of the submenu.
    If this fails we skip, as we then can't test what we are trying to test.
*/
void tst_QMenu::nestedTearOffDetached()
{
    // Since BOBUIornOffMenu is not declared in qmenuprivate.h we can't access the
    // object even through QMenuPrivate. So use an event filter to watch out for
    // a BOBUIornOffMenu showing.
    class TearOffWatcher : public QObject
    {
    public:
        QMenu *tornOffMenu = nullptr;
    protected:
        bool eventFilter(QObject *receiver, QEvent *event) override
        {
            if (event->type() == QEvent::Show && receiver->inherits("BOBUIornOffMenu"))
                tornOffMenu = qobject_cast<QMenu *>(receiver);
            return QObject::eventFilter(receiver, event);
        }
    } watcher;
    qApp->installEventFilter(&watcher);

    QWidget widget;
    QMenu *menu = new QMenu("Context", &widget);

    MenuMetrics mm(menu);
    const int tearOffOffset = mm.fw + mm.vmargin + mm.tearOffHeight / 2;

    QMenu *subMenu = menu->addMenu("SubMenu");
    menu->setTearOffEnabled(true);
    QMenu *subSubMenu = subMenu->addMenu("SubSubMenu");
    subMenu->setTearOffEnabled(true);
    subSubMenu->addAction("Action!");
    subSubMenu->setTearOffEnabled(true);

    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    // open and tear off context menu
    menu->popup(widget.geometry().center());
    BOBUIest::mouseClick(menu, BobUI::LeftButton, {}, QPoint(menu->width() / 2, tearOffOffset));

    QMenu *menuTorn = watcher.tornOffMenu;
    watcher.tornOffMenu = nullptr;
    QVERIFY(menuTorn);
    QVERIFY(BOBUIest::qWaitForWindowExposed(menuTorn));

    // open second menu and tear-off
    BOBUIest::mouseMove(menuTorn, menuTorn->actionGeometry(subMenu->menuAction()).topLeft());
    BOBUIest::qWait(100);
    BOBUIest::mouseMove(menuTorn, menuTorn->actionGeometry(subMenu->menuAction()).center());
    if (!BOBUIest::qWaitFor([subMenu]{ return subMenu->isVisible(); }))
        QSKIP("Menu failed to show, skipping test");

    BOBUIest::mouseClick(subMenu, BobUI::LeftButton, {}, QPoint(subMenu->width() / 2, tearOffOffset));
    menuTorn = watcher.tornOffMenu;
    QVERIFY(menuTorn);
    QVERIFY(BOBUIest::qWaitForWindowExposed(menuTorn));
    // close the top level tear off
    menu->hideTearOffMenu();
    // open third menu and tear-off
    BOBUIest::mouseMove(menuTorn, menuTorn->actionGeometry(subSubMenu->menuAction()).topLeft());
    BOBUIest::qWait(100);
    BOBUIest::mouseMove(menuTorn, menuTorn->actionGeometry(subSubMenu->menuAction()).center());
    BOBUIRY_VERIFY(subSubMenu->isVisible());
    BOBUIest::mouseClick(subSubMenu, BobUI::LeftButton, {}, QPoint(subSubMenu->width() / 2, tearOffOffset));
}

/*!
    Test that a menu will close if you do a mouse click on top of
    it without having moved the mouse.
    (BOBUIBUG-128359).
*/
void tst_QMenu::closeMenuOnClickIfMouseHasntMoved()
{
    QWidget w;
    w.resize(100, 100);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    QMenu contextMenu;
    for (int i = 0; i < 5; ++i) {
        QAction *action = contextMenu.addAction(QStringLiteral("action"));
        connect(action, &QAction::triggered, []{ QFAIL("No menu item should trigger"); });
    }

    const QPoint pos = w.rect().center();
    const QPoint globalPos = w.mapToGlobal(pos);
    // Move the mouse inside the window
    BOBUIest::mouseMove(&w, pos);
    // Move the menu a bit up, so that a menu item falls underneath the
    // mouse (similar to the code attached to the bug report: BOBUIBUG-128359).
    contextMenu.popup(globalPos - QPoint(0, 20));
    QVERIFY(BOBUIest::qWaitForWindowExposed(&contextMenu));
    // Do a mouse click without having moved the cursor. This
    // should close the menu, even if it's underneath the mouse.
    BOBUIest::mouseClick(&contextMenu, BobUI::RightButton, {}, contextMenu.mapFromGlobal(pos));
}

void tst_QMenu::invisibleActions()
{
    QWidget window;
    window.resize(100, 100);
    window.show();

    const QPoint globalPos = window.mapToGlobal(window.rect().center());

    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    QMenu contextMenu;
    QList<QAction *> actions;
    for (int i = 0; i < 5; ++i)
        actions << contextMenu.addAction("action");
    QVERIFY(contextMenu.sizeHint().isValid());

    contextMenu.popup(globalPos);
    QVERIFY(contextMenu.isVisible());

    contextMenu.close();

    for (const auto &action : actions)
        action->setVisible(false);

    contextMenu.popup(globalPos);
    QVERIFY(!contextMenu.isVisible());
    QVERIFY(!contextMenu.exec());

    // a QMenu might not have any (visible) actions, but still have contents
    QPushButton *buttonInMenu = new QPushButton(&contextMenu);
    buttonInMenu->show();

    contextMenu.popup(globalPos);
    QVERIFY(contextMenu.isVisible());
}

#if BOBUI_CONFIG(shortcut) && !defined(Q_OS_DARWIN)
void tst_QMenu::dontSelectDisabledActionByShortcut()
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::WindowActivation))
        QSKIP("Window activation is not supported");

    QMainWindow w;
    auto mb = w.menuBar();
    auto m = mb->addMenu("me&nu");
    auto first = m->addAction("disabled &o");
    auto second = m->addAction(QStringLiteral("enabled &o"));

    QSignalSpy spy(second, &QAction::triggered);
    first->setDisabled(true);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));
    BOBUIest::keyClick(&w, BobUI::Key_N, BobUI::AltModifier);
    BOBUIest::keyClick(m, BobUI::Key_O, BobUI::NoModifier);
    QCOMPARE(spy.count(), 1);
}
#endif

void tst_QMenu::execReturnsWidgetAction()
{
    QWidget window;

    QMenu menu(&window);
    QWidgetAction *widgetAction = new QWidgetAction(&menu);
    QPushButton *menuButton = new QPushButton("Button", &menu);
    widgetAction->setDefaultWidget(menuButton);
    QObject::connect(menuButton, &QPushButton::clicked, widgetAction, &QAction::trigger);

    menu.addAction("First");
    menu.addAction(widgetAction);
    menu.addAction("Last");

    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    BOBUIimer::singleShot(0, menuButton, &QPushButton::click);
    QCOMPARE(menu.exec(window.geometry().center()), widgetAction);
}

BOBUIEST_MAIN(tst_QMenu)
#include "tst_qmenu.moc"
