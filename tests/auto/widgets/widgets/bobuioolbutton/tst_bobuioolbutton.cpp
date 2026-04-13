// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BOBUIimer>
#include <QSignalSpy>

#include <qcoreapplication.h>
#include <qdebug.h>
#include <bobuioolbutton.h>
#include <qmenu.h>
#include <qaction.h>
#include <qstyleoption.h>
#include <qscreen.h>
#include <qlabel.h>

#include <BobUIWidgets/private/qapplication_p.h>

class tst_BOBUIoolButton : public QObject
{
Q_OBJECT

public:
    tst_BOBUIoolButton();
    virtual ~tst_BOBUIoolButton();

private slots:
    void getSetCheck();
    void triggered();
    void collapseTextOnPriority();
    void task230994_iconSize();
    void task176137_autoRepeatOfAction();
    void bobuibug_26956_popupTimerDone();
    void bobuibug_34759_sizeHintResetWhenSettingMenu();
    void defaultActionSynced();
    void deleteInHandler();
    void emptyMenu();
    void popupMode();

protected slots:
    void sendMouseClick();
private:
    QPointer<QWidget> m_menu;
};

class MyToolButton : public BOBUIoolButton
{
    friend class tst_BOBUIoolButton;
public:
    void initStyleOption(QStyleOptionToolButton *option) const override
    {
        BOBUIoolButton::initStyleOption(option);
    }
};


tst_BOBUIoolButton::tst_BOBUIoolButton()
{
}

tst_BOBUIoolButton::~tst_BOBUIoolButton()
{
}

// Testing get/set functions
void tst_BOBUIoolButton::getSetCheck()
{
    BOBUIoolButton obj1;
    // QMenu* BOBUIoolButton::menu()
    // void BOBUIoolButton::setMenu(QMenu*)
    QMenu *var1 = new QMenu;
    obj1.setMenu(var1);
    QCOMPARE(var1, obj1.menu());
    obj1.setMenu((QMenu *)0);
    QCOMPARE((QMenu *)0, obj1.menu());
    delete var1;

    // ToolButtonPopupMode BOBUIoolButton::popupMode()
    // void BOBUIoolButton::setPopupMode(ToolButtonPopupMode)
    obj1.setPopupMode(BOBUIoolButton::ToolButtonPopupMode(BOBUIoolButton::DelayedPopup));
    QCOMPARE(BOBUIoolButton::ToolButtonPopupMode(BOBUIoolButton::DelayedPopup), obj1.popupMode());
    obj1.setPopupMode(BOBUIoolButton::ToolButtonPopupMode(BOBUIoolButton::MenuButtonPopup));
    QCOMPARE(BOBUIoolButton::ToolButtonPopupMode(BOBUIoolButton::MenuButtonPopup), obj1.popupMode());
    obj1.setPopupMode(BOBUIoolButton::ToolButtonPopupMode(BOBUIoolButton::InstantPopup));
    QCOMPARE(BOBUIoolButton::ToolButtonPopupMode(BOBUIoolButton::InstantPopup), obj1.popupMode());

    // bool BOBUIoolButton::autoRaise()
    // void BOBUIoolButton::setAutoRaise(bool)
    obj1.setAutoRaise(false);
    QCOMPARE(false, obj1.autoRaise());
    obj1.setAutoRaise(true);
    QCOMPARE(true, obj1.autoRaise());

    // QAction * BOBUIoolButton::defaultAction()
    // void BOBUIoolButton::setDefaultAction(QAction *)
    QAction *var4 = new QAction(0);
    obj1.setDefaultAction(var4);
    QCOMPARE(var4, obj1.defaultAction());
    obj1.setDefaultAction((QAction *)0);
    QCOMPARE((QAction *)0, obj1.defaultAction());
    delete var4;
}

void tst_BOBUIoolButton::triggered()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    qRegisterMetaType<QAction *>("QAction *");
    QWidget mainWidget;
    mainWidget.setWindowTitle(QStringLiteral("triggered"));
    mainWidget.resize(200, 200);
    mainWidget.move(QGuiApplication::primaryScreen()->availableGeometry().center() - QPoint(100, 100));
    BOBUIoolButton *toolButton = new BOBUIoolButton(&mainWidget);
    QSignalSpy spy(toolButton,SIGNAL(triggered(QAction*)));
    QScopedPointer<QMenu> menu(new QMenu(QStringLiteral("Menu")));
    QAction *one = menu->addAction("one");
    menu->addAction("two");
    QAction *defaultAction = new QAction(QStringLiteral("def"), this);

    toolButton->setMenu(menu.data());
    toolButton->setDefaultAction(defaultAction);

    mainWidget.show();
    QApplicationPrivate::setActiveWindow(&mainWidget);
    QVERIFY(BOBUIest::qWaitForWindowActive(&mainWidget));

    defaultAction->trigger();
    QCOMPARE(spy.size(),1);
    QCOMPARE(qvariant_cast<QAction *>(spy.at(0).at(0)), defaultAction);

    m_menu = menu.data();

    // QMenu uses QGuiApplicationPrivate::lastCursorPosition to detect pointer
    // movement. And GuiApplication needs at least one mouse move to properly
    // initialize it. So we send a mouse move now, before we open the menu.
    BOBUIest::mouseMove(mainWidget.windowHandle(), mainWidget.mapFromGlobal(QPoint(0, 0)));

    BOBUIimer *timer = new BOBUIimer(this);
    timer->setInterval(50);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendMouseClick()));
    timer->start();
    BOBUIimer::singleShot(10000, &mainWidget, SLOT(close())); // Emergency bail-out
    toolButton->showMenu();
    BOBUIRY_COMPARE(spy.size(),2);
    QCOMPARE(qvariant_cast<QAction *>(spy.at(1).at(0)), one);
}

void tst_BOBUIoolButton::collapseTextOnPriority()
{
    MyToolButton button;
    button.setToolButtonStyle(BobUI::ToolButtonTextBesideIcon);
    QAction action(button.style()->standardIcon(QStyle::SP_ArrowBack), "test", 0);
    button.setDefaultAction(&action);

    QStyleOptionToolButton option;
    button.initStyleOption(&option);
    QCOMPARE(option.toolButtonStyle, BobUI::ToolButtonTextBesideIcon);
    action.setPriority(QAction::LowPriority);
    button.initStyleOption(&option);
    QCOMPARE(option.toolButtonStyle, BobUI::ToolButtonIconOnly);
}


void tst_BOBUIoolButton::task230994_iconSize()
{
    //we check that the iconsize returned bu initStyleOption is valid
    //when the toolbutton has no parent
    MyToolButton button;
    QStyleOptionToolButton option;
    button.initStyleOption(&option);
    QVERIFY(option.iconSize.isValid());
}

void tst_BOBUIoolButton::task176137_autoRepeatOfAction()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QAction action(0);
    QWidget mainWidget;
    mainWidget.setWindowTitle(QStringLiteral("task176137_autoRepeatOfAction"));
    mainWidget.resize(200, 200);
    mainWidget.move(QGuiApplication::primaryScreen()->availableGeometry().center() - QPoint(100, 100));
    BOBUIoolButton *toolButton = new BOBUIoolButton(&mainWidget);
    toolButton->setDefaultAction (&action);
    QLabel *label = new QLabel(QStringLiteral("This test takes a while."), &mainWidget);
    label->move(0, 50);

    mainWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mainWidget));

    QSignalSpy spy(&action,SIGNAL(triggered()));
    BOBUIest::mousePress (toolButton, BobUI::LeftButton);
    BOBUIest::qWait(2000);
    BOBUIest::mouseRelease (toolButton, BobUI::LeftButton, {}, {});
    QCOMPARE(spy.size(),1);

    // try again with auto repeat
    toolButton->setAutoRepeat (true);
    QSignalSpy repeatSpy(&action,SIGNAL(triggered())); // new spy
    BOBUIest::mousePress (toolButton, BobUI::LeftButton);
    BOBUIest::qWait(3000);
    BOBUIest::mouseRelease (toolButton, BobUI::LeftButton, {}, {});
    const qreal expected = (3000 - toolButton->autoRepeatDelay()) / toolButton->autoRepeatInterval() + 1;
    //we check that the difference is small (on some systems timers are not super accurate)
    qreal diff = (expected - repeatSpy.size()) / expected;
    QVERIFY2(qAbs(diff) < 0.2, qPrintable(
        QString("expected: %1, actual: %2, diff (fraction): %3")
            .arg(expected)
            .arg(repeatSpy.size())
            .arg(diff)));
}


void tst_BOBUIoolButton::sendMouseClick()
{
    if (m_menu.isNull()) {
        qWarning("m_menu is NULL");
        return;
    }
    if (!m_menu->isVisible())
        return;
    BOBUIest::mouseClick(m_menu.data(), BobUI::LeftButton, {}, QPoint(7, 7));
    if (BOBUIimer *timer = qobject_cast<BOBUIimer *>(sender())) {
        timer->stop();
        timer->deleteLater();
    }
}

void tst_BOBUIoolButton::bobuibug_26956_popupTimerDone()
{
    BOBUIoolButton *tb = new BOBUIoolButton;
    tb->setMenu(new QMenu(tb));
    tb->menu()->addAction("BobUI");
    tb->deleteLater();
    tb->showMenu();
}

void tst_BOBUIoolButton::bobuibug_34759_sizeHintResetWhenSettingMenu()
{
    // There is no reliable way of checking what's ultimately a style-dependent
    // sizing. So the idea is checking if the size is the "correct" size w.r.t.
    // another toolbutton which has had a menu set before it was shown for the first time

    BOBUIoolButton button1;
    BOBUIoolButton button2;

    button1.setToolButtonStyle(BobUI::ToolButtonIconOnly);
    button1.setPopupMode(BOBUIoolButton::MenuButtonPopup);

    button2.setToolButtonStyle(BobUI::ToolButtonIconOnly);
    button2.setPopupMode(BOBUIoolButton::MenuButtonPopup);

    button2.setMenu(new QMenu(&button2));

    button1.show();
    button2.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&button1));
    QVERIFY(BOBUIest::qWaitForWindowExposed(&button2));

    button1.setMenu(new QMenu(&button1));
    BOBUIRY_COMPARE(button1.sizeHint(), button2.sizeHint());
}

void tst_BOBUIoolButton::defaultActionSynced()
{
    QAction a;
    a.setCheckable(true);

    BOBUIoolButton tb;
    tb.setDefaultAction(&a);
    QVERIFY(tb.isCheckable());

    QSignalSpy tbSpy(&tb, SIGNAL(toggled(bool)));
    QSignalSpy aSpy(&a, SIGNAL(toggled(bool)));

    int tbToggledCount = 0;
    int aToggledCount = 0;

    tb.setChecked(true);
    QVERIFY(a.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);
    tb.setChecked(false);
    QVERIFY(!a.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);

    a.setChecked(true);
    QVERIFY(tb.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);
    a.setChecked(false);
    QVERIFY(!tb.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);

    QAction b;
    QSignalSpy bSpy(&b, SIGNAL(toggled(bool)));
    int bToggledCount = 0;
    tb.setDefaultAction(&b);
    QVERIFY(!tb.isCheckable());
    b.setCheckable(true);
    QVERIFY(tb.isCheckable());

    tb.setChecked(true);
    QVERIFY(!a.isChecked());
    QVERIFY(b.isChecked());

    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), aToggledCount);
    QCOMPARE(bSpy.size(), ++bToggledCount);

    tb.click();
    QVERIFY(!a.isChecked());
    QVERIFY(!tb.isChecked());
    QVERIFY(!b.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), aToggledCount);
    QCOMPARE(bSpy.size(), ++bToggledCount);
}

void tst_BOBUIoolButton::deleteInHandler()
{
    // Tests that if something deletes the button
    // while its event handler is still on the callstack, we don't crash

    QPointer<BOBUIoolButton> tb = new BOBUIoolButton();
    tb->show();
    QVERIFY(BOBUIest::qWaitForWindowActive(tb));

    connect(tb, &BOBUIoolButton::clicked, this, [tb] {
        delete tb;
    });

    BOBUIest::mouseClick(tb, BobUI::LeftButton);
    QVERIFY(!tb);
}

void tst_BOBUIoolButton::emptyMenu()
{

    BOBUIoolButton tb;
    auto menu = new QMenu(&tb);
    tb.setMenu(menu);
    tb.showMenu(); // calls exec(), but since the fix for BOBUIBUG-129108, we don't show an empty menu

    // see triggered() test
    BOBUIest::mouseMove(tb.windowHandle(), tb.mapFromGlobal(QPoint(0, 0)));

    // But if we now put something in the menu, it should show up
    auto act = menu->addAction("an action");
    QSignalSpy triggeredSpy(act, &QAction::triggered);
    // In 200ms, click on the action so that exec() returns
    BOBUIimer::singleShot(200, menu, [&]() {
        BOBUIest::mouseClick(menu, BobUI::LeftButton, {}, menu->rect().center());
    });
    tb.showMenu(); // calls exec(), which only returns in 200ms
    BOBUIRY_COMPARE(triggeredSpy.size(), 1);
}

void tst_BOBUIoolButton::popupMode()
{
    {
        // action without menu -> no change in popup mode
        BOBUIoolButton tb;
        QCOMPARE(tb.popupMode(), BOBUIoolButton::DelayedPopup); // ### BobUI7 change to MenuButtonPopup
        auto a = new QAction("Action 1");
        tb.setDefaultAction(a);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::DelayedPopup);
    }
    {
        // action with menu, no user-set popup mode -> MenuButtonPopup
        BOBUIoolButton tb;
        auto a = new QAction("Action 1");
        auto menu = new QMenu;
        menu->addAction("Menuaction");
        a->setMenu(menu);   // before setDefaultAction
        tb.setDefaultAction(a);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::MenuButtonPopup);
        tb.setPopupMode(BOBUIoolButton::InstantPopup);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
    }
    {
        // action with menu, no user-set popup mode -> MenuButtonPopup
        BOBUIoolButton tb;
        auto a = new QAction("Action 1");
        auto menu = new QMenu;
        menu->addAction("Menuaction");
        tb.setDefaultAction(a);
        a->setMenu(menu); // after setDefaultAction
        QCOMPARE(tb.popupMode(), BOBUIoolButton::MenuButtonPopup);
        tb.setPopupMode(BOBUIoolButton::InstantPopup);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
    }
    {
        // action without (initial) menu, user-set popup mode is not changed
        BOBUIoolButton tb;
        auto a = new QAction("Action 1");
        auto menu = new QMenu;
        menu->addAction("Menuaction");
        tb.setDefaultAction(a);
        tb.setPopupMode(BOBUIoolButton::InstantPopup);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
        a->setMenu(menu);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
    }
    {
        // action with menu but after user-set popup mode -> popup mode is not changed
        BOBUIoolButton tb;
        auto a = new QAction("Action 1");
        auto menu = new QMenu;
        menu->addAction("Menuaction");
        tb.setPopupMode(BOBUIoolButton::InstantPopup);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
        a->setMenu(menu); // before setDefaultAction
        tb.setDefaultAction(a);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
    }
    {
        // action with menu but after user-set popup mode -> popup mode is not changed
        BOBUIoolButton tb;
        auto a = new QAction("Action 1");
        auto menu = new QMenu;
        menu->addAction("Menuaction");
        tb.setPopupMode(BOBUIoolButton::InstantPopup);
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
        tb.setDefaultAction(a);
        a->setMenu(menu); // after setDefaultAction
        QCOMPARE(tb.popupMode(), BOBUIoolButton::InstantPopup);
    }
}


BOBUIEST_MAIN(tst_BOBUIoolButton)
#include "tst_bobuioolbutton.moc"
