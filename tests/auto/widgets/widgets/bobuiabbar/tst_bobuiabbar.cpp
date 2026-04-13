// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QSignalSpy>
#include <QApplication>
#include <BOBUIabBar>
#include <QPushButton>
#include <QLabel>
#include <QStyle>
#include <QStyleOptionTab>
#include <QProxyStyle>
#include <BOBUIimer>
#include <QScreen>
#include <QWindow>

#include <BobUIWidgets/private/bobuiabbar_p.h>

using namespace BobUI::StringLiterals;

class TabBar;

class tst_BOBUIabBar : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIabBar();
    virtual ~tst_BOBUIabBar();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

private slots:
    void getSetCheck();
    void setIconSize();
    void setIconSize_data();

    void testCurrentChanged_data();
    void testCurrentChanged();

    void insertAtCurrentIndex();
    void insertAfterCurrentIndex();

    void removeTab_data();
    void removeTab();

    void hideTab_data();
    void hideTab();
    void hideAllTabs();
    void checkHiddenTab();

    void setElideMode_data();
    void setElideMode();
    void sizeHints();

    void setUsesScrollButtons_data();
    void setUsesScrollButtons();

    void removeLastTab();
    void removeLastVisibleTab();

    void closeButton();
    void requestCloseOnMiddleClick();

    void tabButton_data();
    void tabButton();

    void selectionBehaviorOnRemove_data();
    void selectionBehaviorOnRemove();

    void moveTab_data();
    void moveTab();

    void task251184_removeTab();
    void changeTitleWhileDoubleClickingTab();

    void taskBOBUIBUG_10052_widgetLayoutWhenMoving();

    void tabBarClicked();
    void autoHide();

    void mouseReleaseOutsideTabBar();

    void mouseWheel();
    void kineticWheel_data();
    void kineticWheel();
    void highResolutionWheel_data();
    void highResolutionWheel();

    void scrollButtons_data();
    void scrollButtons();

    void currentTabLargeFont();

    void hoverTab_data();
    void hoverTab();

    void resizeKeepsScroll_data();
    void resizeKeepsScroll();
    void changeTabTextKeepsScroll();
    void settingCurrentTabBeforeShowDoesntScroll();
    void checkPositionsAfterShapeChange();
    void checkScrollOffsetAfterTabRemoval();

private:
    void checkPositions(const TabBar &tabbar, const QList<int> &positions);
};

// Testing get/set functions
void tst_BOBUIabBar::getSetCheck()
{
    BOBUIabBar obj1;
    obj1.addTab("Tab1");
    obj1.addTab("Tab2");
    obj1.addTab("Tab3");
    obj1.addTab("Tab4");
    obj1.addTab("Tab5");
    // Shape BOBUIabBar::shape()
    // void BOBUIabBar::setShape(Shape)
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::RoundedNorth));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::RoundedNorth), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::RoundedSouth));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::RoundedSouth), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::RoundedWest));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::RoundedWest), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::RoundedEast));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::RoundedEast), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::TriangularNorth));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::TriangularNorth), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::TriangularSouth));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::TriangularSouth), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::TriangularWest));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::TriangularWest), obj1.shape());
    obj1.setShape(BOBUIabBar::Shape(BOBUIabBar::TriangularEast));
    QCOMPARE(BOBUIabBar::Shape(BOBUIabBar::TriangularEast), obj1.shape());

    // bool BOBUIabBar::drawBase()
    // void BOBUIabBar::setDrawBase(bool)
    obj1.setDrawBase(false);
    QCOMPARE(false, obj1.drawBase());
    obj1.setDrawBase(true);
    QCOMPARE(true, obj1.drawBase());

    // int BOBUIabBar::currentIndex()
    // void BOBUIabBar::setCurrentIndex(int)
    obj1.setCurrentIndex(0);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MIN);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MAX);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(4);
    QCOMPARE(4, obj1.currentIndex());
}

tst_BOBUIabBar::tst_BOBUIabBar()
{
}

tst_BOBUIabBar::~tst_BOBUIabBar()
{
}

void tst_BOBUIabBar::initTestCase()
{
}

void tst_BOBUIabBar::cleanupTestCase()
{
}

void tst_BOBUIabBar::init()
{
}

void tst_BOBUIabBar::setIconSize_data()
{
    BOBUIest::addColumn<int>("sizeToSet");
    BOBUIest::addColumn<int>("expectedWidth");

    const int iconDefault = qApp->style()->pixelMetric(QStyle::PM_TabBarIconSize);
    const int smallIconSize = qApp->style()->pixelMetric(QStyle::PM_SmallIconSize);
    const int largeIconSize = qApp->style()->pixelMetric(QStyle::PM_LargeIconSize);
    BOBUIest::newRow("default") << -1 << iconDefault;
    BOBUIest::newRow("zero") << 0 << 0;
    BOBUIest::newRow("same as default") << iconDefault << iconDefault;
    BOBUIest::newRow("large") << largeIconSize << largeIconSize;
    BOBUIest::newRow("small") << smallIconSize << smallIconSize;
}

void tst_BOBUIabBar::setIconSize()
{
    QFETCH(int, sizeToSet);
    QFETCH(int, expectedWidth);
    BOBUIabBar tabBar;
    tabBar.setIconSize(QSize(sizeToSet, sizeToSet));
    QCOMPARE(tabBar.iconSize().width(), expectedWidth);
}

void tst_BOBUIabBar::testCurrentChanged_data()
{
    BOBUIest::addColumn<int>("tabToSet");
    BOBUIest::addColumn<int>("expectedCount");

    BOBUIest::newRow("pressAntotherTab") << 1 << 2;
    BOBUIest::newRow("pressTheSameTab") << 0 << 1;
}

void tst_BOBUIabBar::testCurrentChanged()
{
    QFETCH(int, tabToSet);
    QFETCH(int, expectedCount);
    BOBUIabBar tabBar;
    QSignalSpy spy(&tabBar, SIGNAL(currentChanged(int)));
    tabBar.addTab("Tab1");
    tabBar.addTab("Tab2");
    QCOMPARE(tabBar.currentIndex(), 0);
    tabBar.setCurrentIndex(tabToSet);
    QCOMPARE(tabBar.currentIndex(), tabToSet);
    QCOMPARE(spy.size(), expectedCount);
}

class TabBar : public BOBUIabBar
{
public:
    using BOBUIabBar::initStyleOption;
    using BOBUIabBar::moveTab;
    using BOBUIabBar::BOBUIabBar;
    using BOBUIabBar::tabSizeHint;
};

void tst_BOBUIabBar::insertAtCurrentIndex()
{
    BOBUIabBar tabBar;
    tabBar.addTab("Tab1");
    QCOMPARE(tabBar.currentIndex(), 0);
    tabBar.insertTab(0, "Tab2");
    QCOMPARE(tabBar.currentIndex(), 1);
    tabBar.insertTab(0, "Tab3");
    QCOMPARE(tabBar.currentIndex(), 2);
    tabBar.insertTab(2, "Tab4");
    QCOMPARE(tabBar.currentIndex(), 3);
}

void tst_BOBUIabBar::insertAfterCurrentIndex()
{
    TabBar tabBar;

    tabBar.addTab("Tab10");
    checkPositions(tabBar, { QStyleOptionTab::OnlyOneTab });

    tabBar.addTab("Tab20");
    checkPositions(tabBar, { QStyleOptionTab::Beginning, QStyleOptionTab::End });

    tabBar.insertTab(1, "Tab15");
    checkPositions(tabBar,
                   { QStyleOptionTab::Beginning, QStyleOptionTab::Middle, QStyleOptionTab::End });

    tabBar.insertTab(3, "Tab30");
    checkPositions(tabBar,
                   { QStyleOptionTab::Beginning, QStyleOptionTab::Middle, QStyleOptionTab::Middle,
                     QStyleOptionTab::End });

    tabBar.insertTab(3, "Tab25");
    checkPositions(tabBar,
                   { QStyleOptionTab::Beginning, QStyleOptionTab::Middle, QStyleOptionTab::Middle,
                     QStyleOptionTab::Middle, QStyleOptionTab::End });
}

void tst_BOBUIabBar::removeTab_data()
{
    BOBUIest::addColumn<int>("currentIndex");
    BOBUIest::addColumn<int>("deleteIndex");
    BOBUIest::addColumn<int>("spyCount");
    BOBUIest::addColumn<int>("finalIndex");

    BOBUIest::newRow("deleteEnd") << 0 << 2 << 0 << 0;
    BOBUIest::newRow("deleteEndWithIndexOnEnd") << 2 << 2 << 1 << 1;
    BOBUIest::newRow("deleteMiddle") << 2 << 1 << 1 << 1;
    BOBUIest::newRow("deleteMiddleOnMiddle") << 1 << 1 << 1 << 1;
}
void tst_BOBUIabBar::removeTab()
{
    BOBUIabBar tabbar;

    QFETCH(int, currentIndex);
    QFETCH(int, deleteIndex);
    tabbar.addTab("foo");
    tabbar.addTab("bar");
    tabbar.addTab("baz");
    tabbar.setCurrentIndex(currentIndex);
    QSignalSpy spy(&tabbar, SIGNAL(currentChanged(int)));
    tabbar.removeTab(deleteIndex);
    BOBUIEST(int(spy.size()), "spyCount");
    BOBUIEST(tabbar.currentIndex(), "finalIndex");
}

void tst_BOBUIabBar::hideTab_data()
{
    BOBUIest::addColumn<int>("currentIndex");
    BOBUIest::addColumn<int>("hideIndex");
    BOBUIest::addColumn<int>("spyCount");
    BOBUIest::addColumn<int>("finalIndex");

    BOBUIest::newRow("hideEnd") << 0 << 2 << 0 << 0;
    BOBUIest::newRow("hideEndWithIndexOnEnd") << 2 << 2 << 1 << 1;
    BOBUIest::newRow("hideMiddle") << 2 << 1 << 0 << 2;
    BOBUIest::newRow("hideMiddleOnMiddle") << 1 << 1 << 1 << 2;
    BOBUIest::newRow("hideFirst") << 2 << 0 << 0 << 2;
    BOBUIest::newRow("hideFirstOnFirst") << 0 << 0 << 1 << 1;
}

void tst_BOBUIabBar::hideTab()
{
    BOBUIabBar tabbar;

    QFETCH(int, currentIndex);
    QFETCH(int, hideIndex);
    tabbar.addTab("foo");
    tabbar.addTab("bar");
    tabbar.addTab("baz");
    tabbar.setCurrentIndex(currentIndex);
    QSignalSpy spy(&tabbar, &BOBUIabBar::currentChanged);
    tabbar.setTabVisible(hideIndex, false);
    BOBUIEST(int(spy.size()), "spyCount");
    BOBUIEST(tabbar.currentIndex(), "finalIndex");
}

void tst_BOBUIabBar::hideAllTabs()
{
    TabBar tabbar;

    tabbar.addTab("foo");
    tabbar.addTab("bar");
    tabbar.addTab("baz");
    tabbar.setCurrentIndex(0);
    checkPositions(tabbar,
                   { QStyleOptionTab::Beginning, QStyleOptionTab::Middle, QStyleOptionTab::End });

    // Check we don't crash trying to hide an unexistant tab
    QSize prevSizeHint = tabbar.sizeHint();
    tabbar.setTabVisible(3, false);
    checkPositions(tabbar,
                   { QStyleOptionTab::Beginning, QStyleOptionTab::Middle, QStyleOptionTab::End });
    QCOMPARE(tabbar.currentIndex(), 0);
    QSize sizeHint = tabbar.sizeHint();
    QVERIFY(sizeHint.width() == prevSizeHint.width());
    prevSizeHint = sizeHint;

    tabbar.setTabVisible(1, false);
    checkPositions(tabbar, { QStyleOptionTab::Beginning, QStyleOptionTab::End });
    QCOMPARE(tabbar.currentIndex(), 0);
    sizeHint = tabbar.sizeHint();
    QVERIFY(sizeHint.width() < prevSizeHint.width());
    prevSizeHint = sizeHint;

    tabbar.setTabVisible(2, false);
    checkPositions(tabbar, { QStyleOptionTab::OnlyOneTab });
    QCOMPARE(tabbar.currentIndex(), 0);
    sizeHint = tabbar.sizeHint();
    QVERIFY(sizeHint.width() < prevSizeHint.width());
    prevSizeHint = sizeHint;

    tabbar.setTabVisible(0, false);
    // We cannot set currentIndex < 0
    QCOMPARE(tabbar.currentIndex(), 0);
    sizeHint = tabbar.sizeHint();
    QVERIFY(sizeHint.width() < prevSizeHint.width());
}

void tst_BOBUIabBar::checkHiddenTab()
{
    BOBUIabBar tabbar;

    tabbar.addTab("foo");
    tabbar.addTab("bar");
    tabbar.addTab("baz");
    tabbar.setCurrentIndex(0);
    tabbar.setTabVisible(1, false);

    QKeyEvent keyRight(QKeyEvent::KeyPress, BobUI::Key_Right, BobUI::NoModifier);
    QVERIFY(QApplication::sendEvent(&tabbar, &keyRight));
    QCOMPARE(tabbar.currentIndex(), 2);

    QKeyEvent keyLeft(QKeyEvent::KeyPress, BobUI::Key_Left, BobUI::NoModifier);
    QVERIFY(QApplication::sendEvent(&tabbar, &keyLeft));
    QCOMPARE(tabbar.currentIndex(), 0);
}

void tst_BOBUIabBar::setElideMode_data()
{
    BOBUIest::addColumn<int>("tabElideMode");
    BOBUIest::addColumn<int>("expectedMode");

    BOBUIest::newRow("default") << -128 << qApp->style()->styleHint(QStyle::SH_TabBar_ElideMode);
    BOBUIest::newRow("explicit default") << qApp->style()->styleHint(QStyle::SH_TabBar_ElideMode)
                                      << qApp->style()->styleHint(QStyle::SH_TabBar_ElideMode);
    BOBUIest::newRow("None") << int(BobUI::ElideNone) << int(BobUI::ElideNone);
    BOBUIest::newRow("Left") << int(BobUI::ElideLeft) << int(BobUI::ElideLeft);
    BOBUIest::newRow("Center") << int(BobUI::ElideMiddle) << int(BobUI::ElideMiddle);
    BOBUIest::newRow("Right") << int(BobUI::ElideRight) << int(BobUI::ElideRight);
}

void tst_BOBUIabBar::setElideMode()
{
    QFETCH(int, tabElideMode);
    BOBUIabBar tabBar;
    if (tabElideMode != -128)
        tabBar.setElideMode(BobUI::TextElideMode(tabElideMode));
    BOBUIEST(int(tabBar.elideMode()), "expectedMode");
    // Make sure style sheet does not override user set mode
    tabBar.setStyleSheet("QWidget { background-color: #ABA8A6;}");
    BOBUIEST(int(tabBar.elideMode()), "expectedMode");
}

void tst_BOBUIabBar::sizeHints()
{
    BOBUIabBar tabBar;
    tabBar.setFont(QFont("Arial", 10));

    // No eliding and no scrolling -> tabbar becomes very wide
    tabBar.setUsesScrollButtons(false);
    tabBar.setElideMode(BobUI::ElideNone);
    tabBar.addTab("tab 01");

    // Fetch the minimum size hint width and make sure that we create enough
    // tabs.
    int minimumSizeHintWidth = tabBar.minimumSizeHint().width();
    for (int i = 0; i <= 700 / minimumSizeHintWidth; ++i)
        tabBar.addTab(QString("tab 0%1").arg(i+2));

    //qDebug() << tabBar.minimumSizeHint() << tabBar.sizeHint();
    QVERIFY(tabBar.minimumSizeHint().width() > 700);
    QVERIFY(tabBar.sizeHint().width() > 700);

    // Scrolling enabled -> no reason to become very wide
    tabBar.setUsesScrollButtons(true);
    QVERIFY(tabBar.minimumSizeHint().width() < 200);
    QVERIFY(tabBar.sizeHint().width() > 700); // unchanged

    // Eliding enabled -> no reason to become very wide
    tabBar.setUsesScrollButtons(false);
    tabBar.setElideMode(BobUI::ElideRight);

    // The sizeHint is very much dependent on the screen DPI value
    // so we can not really predict it.
    int tabBarMinSizeHintWidth = tabBar.minimumSizeHint().width();
    int tabBarSizeHintWidth = tabBar.sizeHint().width();
    QVERIFY(tabBarMinSizeHintWidth < tabBarSizeHintWidth);
    QVERIFY(tabBarSizeHintWidth > 700); // unchanged

    tabBar.addTab("This is tab with a very long title");
    QVERIFY(tabBar.minimumSizeHint().width() > tabBarMinSizeHintWidth);
    QVERIFY(tabBar.sizeHint().width() > tabBarSizeHintWidth);
}

void tst_BOBUIabBar::setUsesScrollButtons_data()
{
    BOBUIest::addColumn<int>("usesArrows");
    BOBUIest::addColumn<bool>("expectedArrows");

    BOBUIest::newRow("default") << -128 << !qApp->style()->styleHint(QStyle::SH_TabBar_PreferNoArrows);
    BOBUIest::newRow("explicit default")
                        << int(!qApp->style()->styleHint(QStyle::SH_TabBar_PreferNoArrows))
                        << !qApp->style()->styleHint(QStyle::SH_TabBar_PreferNoArrows);
    BOBUIest::newRow("No") << int(false) << false;
    BOBUIest::newRow("Yes") << int(true) << true;
}

void tst_BOBUIabBar::setUsesScrollButtons()
{
    QFETCH(int, usesArrows);
    BOBUIabBar tabBar;
    if (usesArrows != -128)
        tabBar.setUsesScrollButtons(usesArrows);
    BOBUIEST(tabBar.usesScrollButtons(), "expectedArrows");

    // Make sure style sheet does not override user set mode
    tabBar.setStyleSheet("QWidget { background-color: #ABA8A6;}");
    BOBUIEST(tabBar.usesScrollButtons(), "expectedArrows");
}

void tst_BOBUIabBar::removeLastTab()
{
    BOBUIabBar tabbar;
    QSignalSpy spy(&tabbar, SIGNAL(currentChanged(int)));
    int index = tabbar.addTab("foo");
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), index);
    spy.clear();

    tabbar.removeTab(index);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), -1);
    spy.clear();
}

void tst_BOBUIabBar::removeLastVisibleTab()
{
    BOBUIabBar tabbar;
    tabbar.setSelectionBehaviorOnRemove(BOBUIabBar::SelectionBehavior::SelectRightTab);

    int invisible = tabbar.addTab("invisible");
    int visible = tabbar.addTab("visible");
    tabbar.setCurrentIndex(visible);
    tabbar.adjustSize();

    tabbar.setTabVisible(invisible, false);
    {
        QSignalSpy spy(&tabbar, SIGNAL(currentChanged(int)));
        tabbar.removeTab(visible);
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.at(0).at(0).toInt(), -1);
        QCOMPARE(tabbar.currentIndex(), -1);
    }

    tabbar.setSelectionBehaviorOnRemove(BOBUIabBar::SelectionBehavior::SelectLeftTab);
    visible = tabbar.insertTab(0, "visible");
    ++invisible;
    QVERIFY(!tabbar.isTabVisible(invisible));
    tabbar.setCurrentIndex(visible);
    {
        QSignalSpy spy(&tabbar, SIGNAL(currentChanged(int)));
        tabbar.removeTab(visible);
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.at(0).at(0).toInt(), -1);
        QCOMPARE(tabbar.currentIndex(), -1);
    }
}

void tst_BOBUIabBar::closeButton()
{
    BOBUIabBar tabbar;
    QCOMPARE(tabbar.tabsClosable(), false);
    tabbar.setTabsClosable(true);
    QCOMPARE(tabbar.tabsClosable(), true);
    tabbar.addTab("foo");

    BOBUIabBar::ButtonPosition closeSide = (BOBUIabBar::ButtonPosition)tabbar.style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0, &tabbar);
    BOBUIabBar::ButtonPosition otherSide = (closeSide == BOBUIabBar::LeftSide ? BOBUIabBar::RightSide : BOBUIabBar::LeftSide);
    QVERIFY(tabbar.tabButton(0, otherSide) == 0);
    QVERIFY(tabbar.tabButton(0, closeSide) != 0);

    QAbstractButton *button = static_cast<QAbstractButton*>(tabbar.tabButton(0, closeSide));
    QVERIFY(button);
    QSignalSpy spy(&tabbar, SIGNAL(tabCloseRequested(int)));
    button->click();
    QCOMPARE(tabbar.count(), 1);
    QCOMPARE(spy.size(), 1);
}

void tst_BOBUIabBar::requestCloseOnMiddleClick()
{
    BOBUIabBar tabbar;
    tabbar.addTab("foo");
    tabbar.addTab("bar");
    QCOMPARE(tabbar.count(), 2);

    QSignalSpy spy(&tabbar, SIGNAL(tabCloseRequested(int)));

    QCOMPARE(tabbar.tabsClosable(), false);
    BOBUIest::mouseClick(&tabbar, BobUI::MiddleButton, {}, tabbar.tabRect(0).center());
    QCOMPARE(spy.size(), 0);

    tabbar.setTabsClosable(true);
    QCOMPARE(tabbar.tabsClosable(), true);
    BOBUIest::mouseClick(&tabbar, BobUI::MiddleButton, {}, tabbar.tabRect(0).center());
    QCOMPARE(spy.size(), 1);

    BOBUIest::mouseClick(&tabbar, BobUI::MiddleButton, {}, tabbar.rect().bottomRight() * 1.1);
    QCOMPARE(spy.size(), 1);
}

Q_DECLARE_METATYPE(BOBUIabBar::ButtonPosition)
void tst_BOBUIabBar::tabButton_data()
{
    BOBUIest::addColumn<BOBUIabBar::ButtonPosition>("position");

    BOBUIest::newRow("left") << BOBUIabBar::LeftSide;
    BOBUIest::newRow("right") << BOBUIabBar::RightSide;
}

// BOBUIabBar::setTabButton(index, closeSide, closeButton);
void tst_BOBUIabBar::tabButton()
{
    QFETCH(BOBUIabBar::ButtonPosition, position);
    BOBUIabBar::ButtonPosition otherSide = (position == BOBUIabBar::LeftSide ? BOBUIabBar::RightSide : BOBUIabBar::LeftSide);

    BOBUIabBar tabbar;
    tabbar.resize(500, 200);
    tabbar.show();
    BOBUIRY_VERIFY(tabbar.isVisible());

    tabbar.setTabButton(-1, position, 0);
    QVERIFY(tabbar.tabButton(-1, position) == 0);
    QVERIFY(tabbar.tabButton(0, position) == 0);

    tabbar.addTab("foo");
    QCOMPARE(tabbar.count(), 1);
    tabbar.setTabButton(0, position, 0);
    QVERIFY(tabbar.tabButton(0, position) == 0);

    QPushButton *button = new QPushButton;
    button->show();
    button->setText("hi");
    button->resize(10, 10);
    BOBUIRY_VERIFY(button->isVisible());
    BOBUIRY_VERIFY(button->isVisible());

    tabbar.setTabButton(0, position, button);

    QCOMPARE(tabbar.tabButton(0, position), static_cast<QWidget *>(button));
    BOBUIRY_VERIFY(!button->isHidden());
    QVERIFY(tabbar.tabButton(0, otherSide) == 0);
    QCOMPARE(button->parent(), static_cast<QObject *>(&tabbar));
    QVERIFY(button->pos() != QPoint(0, 0));

    QPushButton *button2 = new QPushButton;
    tabbar.setTabButton(0, position, button2);
    QVERIFY(button->isHidden());
}

typedef QList<int> IntList;
Q_DECLARE_METATYPE(BOBUIabBar::SelectionBehavior)
#define ONE(x) (IntList() << x)
void tst_BOBUIabBar::selectionBehaviorOnRemove_data()
{
    BOBUIest::addColumn<BOBUIabBar::SelectionBehavior>("selectionBehavior");
    BOBUIest::addColumn<int>("tabs");
    BOBUIest::addColumn<IntList>("select");
    BOBUIest::addColumn<IntList>("remove");
    BOBUIest::addColumn<int>("expected");

    //                                               Count            select remove current
    BOBUIest::newRow("left-1") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 0) << ONE(0) << 0;

    BOBUIest::newRow("left-2") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 0) << ONE(1) << 0; // not removing current
    BOBUIest::newRow("left-3") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 0) << ONE(2) << 0; // not removing current
    BOBUIest::newRow("left-4") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 1) << ONE(0) << 0; // not removing current
    BOBUIest::newRow("left-5") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 1) << ONE(1) << 0;
    BOBUIest::newRow("left-6") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 1) << ONE(2) << 1;
    BOBUIest::newRow("left-7") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 2) << ONE(0) << 1; // not removing current
    BOBUIest::newRow("left-8") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 2) << ONE(1) << 1; // not removing current
    BOBUIest::newRow("left-9") << BOBUIabBar::SelectLeftTab << 3 << (IntList() << 2) << ONE(2) << 1;

    BOBUIest::newRow("right-1") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 0) << ONE(0) << 0;
    BOBUIest::newRow("right-2") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 0) << ONE(1) << 0; // not removing current
    BOBUIest::newRow("right-3") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 0) << ONE(2) << 0; // not removing current
    BOBUIest::newRow("right-4") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 1) << ONE(0) << 0; // not removing current
    BOBUIest::newRow("right-5") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 1) << ONE(1) << 1;
    BOBUIest::newRow("right-6") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 1) << ONE(2) << 1; // not removing current
    BOBUIest::newRow("right-7") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 2) << ONE(0) << 1; // not removing current
    BOBUIest::newRow("right-8") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 2) << ONE(1) << 1; // not removing current
    BOBUIest::newRow("right-9") << BOBUIabBar::SelectRightTab << 3 << (IntList() << 2) << ONE(2) << 1;

    BOBUIest::newRow("previous-0") << BOBUIabBar::SelectPreviousTab << 3 << (IntList()) << ONE(0) << 0;
    BOBUIest::newRow("previous-1") << BOBUIabBar::SelectPreviousTab << 3 << (IntList()) << ONE(1) << 0; // not removing current
    BOBUIest::newRow("previous-2") << BOBUIabBar::SelectPreviousTab << 3 << (IntList()) << ONE(2) << 0; // not removing current

    BOBUIest::newRow("previous-3") << BOBUIabBar::SelectPreviousTab << 3 << (IntList() << 2) << ONE(0) << 1; // not removing current
    BOBUIest::newRow("previous-4") << BOBUIabBar::SelectPreviousTab << 3 << (IntList() << 2) << ONE(1) << 1; // not removing current
    BOBUIest::newRow("previous-5") << BOBUIabBar::SelectPreviousTab << 3 << (IntList() << 2) << ONE(2) << 0;

    // go back one
    BOBUIest::newRow("previous-6") << BOBUIabBar::SelectPreviousTab << 4 << (IntList() << 0 << 2 << 3 << 1) << (IntList() << 1) << 2;
    // go back two
    BOBUIest::newRow("previous-7") << BOBUIabBar::SelectPreviousTab << 4 << (IntList() << 0 << 2 << 3 << 1) << (IntList() << 1 << 2) << 1;
    // go back three
    BOBUIest::newRow("previous-8") << BOBUIabBar::SelectPreviousTab << 4 << (IntList() << 0 << 2 << 3 << 1) << (IntList() << 1 << 2 << 1) << 0;

    // pick from the middle
    BOBUIest::newRow("previous-9") << BOBUIabBar::SelectPreviousTab << 4 << (IntList() << 0 << 2 << 3 << 1) << (IntList() << 2 << 1) << 1;

    // every other one
    BOBUIest::newRow("previous-10") << BOBUIabBar::SelectPreviousTab << 7 << (IntList() << 0 << 2 << 4 << 6) << (IntList() << 6 << 4) << 2;

    // BOBUIBUG-94352
    BOBUIest::newRow("BOBUIBUG-94352") << BOBUIabBar::SelectPreviousTab << 4 << (IntList() << 3) << (IntList() << 2 << 2) << 0;

}

void tst_BOBUIabBar::selectionBehaviorOnRemove()
{
    QFETCH(BOBUIabBar::SelectionBehavior, selectionBehavior);
    QFETCH(int, tabs);
    QFETCH(IntList, select);
    QFETCH(IntList, remove);
    QFETCH(int, expected);

    BOBUIabBar tabbar;
    tabbar.setSelectionBehaviorOnRemove(selectionBehavior);
    while(--tabs >= 0)
        tabbar.addTab(QString::number(tabs));
    QCOMPARE(tabbar.currentIndex(), 0);
    while(!select.isEmpty())
        tabbar.setCurrentIndex(select.takeFirst());
    while(!remove.isEmpty())
        tabbar.removeTab(remove.takeFirst());
    QVERIFY(tabbar.count() > 0);
    QCOMPARE(tabbar.currentIndex(), expected);
}

Q_DECLARE_METATYPE(BOBUIabBar::Shape)
void tst_BOBUIabBar::moveTab_data()
{
    BOBUIest::addColumn<BOBUIabBar::Shape>("shape");
    BOBUIest::addColumn<int>("tabs");
    BOBUIest::addColumn<int>("from");
    BOBUIest::addColumn<int>("to");

    BOBUIest::newRow("null-0") << BOBUIabBar::RoundedNorth << 0 << -1 << -1;
    BOBUIest::newRow("null-1") << BOBUIabBar::RoundedEast  << 0 << -1 << -1;
    BOBUIest::newRow("null-2") << BOBUIabBar::RoundedEast  << 1 << 0 << 0;

    BOBUIest::newRow("two-0") << BOBUIabBar::RoundedNorth << 2 << 0 << 1;
    BOBUIest::newRow("two-1") << BOBUIabBar::RoundedNorth << 2 << 1 << 0;

    BOBUIest::newRow("five-0") << BOBUIabBar::RoundedNorth << 5 << 1 << 3; // forward
    BOBUIest::newRow("five-1") << BOBUIabBar::RoundedNorth << 5 << 3 << 1; // reverse

    BOBUIest::newRow("five-2") << BOBUIabBar::RoundedNorth << 5 << 0 << 4; // forward
    BOBUIest::newRow("five-3") << BOBUIabBar::RoundedNorth << 5 << 1 << 4; // forward
    BOBUIest::newRow("five-4") << BOBUIabBar::RoundedNorth << 5 << 3 << 4; // forward
}

void tst_BOBUIabBar::moveTab()
{
    QFETCH(BOBUIabBar::Shape, shape);
    QFETCH(int, tabs);
    QFETCH(int, from);
    QFETCH(int, to);

    TabBar bar;
    bar.setShape(shape);
    while(--tabs >= 0)
        bar.addTab(QString::number(tabs));
    bar.moveTab(from, to);
}


class MyTabBar : public BOBUIabBar
{
    Q_OBJECT
public slots:
    void onCurrentChanged()
    {
        //we just want this to be done once
        disconnect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged()));
        removeTab(0);
    }
};

void tst_BOBUIabBar::task251184_removeTab()
{
    MyTabBar bar;
    bar.addTab("bar1");
    bar.addTab("bar2");
    QCOMPARE(bar.count(), 2);
    QCOMPARE(bar.currentIndex(), 0);

    bar.connect(&bar, SIGNAL(currentChanged(int)), SLOT(onCurrentChanged()));
    bar.setCurrentIndex(1);

    QCOMPARE(bar.count(), 1);
    QCOMPARE(bar.currentIndex(), 0);
    QCOMPARE(bar.tabText(bar.currentIndex()), QString("bar2"));
}


class TitleChangeTabBar : public BOBUIabBar
{
    Q_OBJECT

    BOBUIimer timer;
    int count;

public:
    TitleChangeTabBar(QWidget * parent = nullptr) : BOBUIabBar(parent), count(0)
    {
        setMovable(true);
        addTab("0");
        connect(&timer, SIGNAL(timeout()), this, SLOT(updateTabText()));
        timer.start(1);
    }

public slots:
    void updateTabText()
    {
        count++;
        setTabText(0, QString::number(count));
    }
};

void tst_BOBUIabBar::changeTitleWhileDoubleClickingTab()
{
    TitleChangeTabBar bar;
    QPoint tabPos = bar.tabRect(0).center();

    for(int i=0; i < 10; i++)
        BOBUIest::mouseDClick(&bar, BobUI::LeftButton, {}, tabPos);
}

class Widget10052 : public QWidget
{
public:
    Widget10052(QWidget *parent) : QWidget(parent), moved(false)
    { }

    void moveEvent(QMoveEvent *e) override
    {
        moved = e->oldPos() != e->pos();
        QWidget::moveEvent(e);
    }

    bool moved;
};

void tst_BOBUIabBar::taskBOBUIBUG_10052_widgetLayoutWhenMoving()
{
    BOBUIabBar tabBar;
    tabBar.insertTab(0, "My first tab");
    Widget10052 w1(&tabBar);
    tabBar.setTabButton(0, BOBUIabBar::RightSide, &w1);
    tabBar.insertTab(1, "My other tab");
    Widget10052 w2(&tabBar);
    tabBar.setTabButton(1, BOBUIabBar::RightSide, &w2);

    tabBar.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabBar));
    w1.moved = w2.moved = false;
    tabBar.moveTab(0, 1);
    BOBUIRY_VERIFY(w1.moved);
    QVERIFY(w2.moved);
}

void tst_BOBUIabBar::tabBarClicked()
{
    BOBUIabBar tabBar;
    tabBar.addTab("0");
    QSignalSpy clickSpy(&tabBar, SIGNAL(tabBarClicked(int)));
    QSignalSpy doubleClickSpy(&tabBar, SIGNAL(tabBarDoubleClicked(int)));

    QCOMPARE(clickSpy.size(), 0);
    QCOMPARE(doubleClickSpy.size(), 0);

    BobUI::MouseButton button = BobUI::LeftButton;
    while (button <= BobUI::MaxMouseButton) {
        const QPoint tabPos = tabBar.tabRect(0).center();

        BOBUIest::mouseClick(&tabBar, button, {}, tabPos);
        QCOMPARE(clickSpy.size(), 1);
        QCOMPARE(clickSpy.takeFirst().takeFirst().toInt(), 0);
        QCOMPARE(doubleClickSpy.size(), 0);

        BOBUIest::mouseDClick(&tabBar, button, {}, tabPos);
        QCOMPARE(clickSpy.size(), 1);
        QCOMPARE(clickSpy.takeFirst().takeFirst().toInt(), 0);
        QCOMPARE(doubleClickSpy.size(), 1);
        QCOMPARE(doubleClickSpy.takeFirst().takeFirst().toInt(), 0);
        BOBUIest::mouseRelease(&tabBar, button, {}, tabPos);

        const QPoint barPos(tabBar.tabRect(0).right() + 5, tabBar.tabRect(0).center().y());

        BOBUIest::mouseClick(&tabBar, button, {}, barPos);
        QCOMPARE(clickSpy.size(), 1);
        QCOMPARE(clickSpy.takeFirst().takeFirst().toInt(), -1);
        QCOMPARE(doubleClickSpy.size(), 0);

        BOBUIest::mouseDClick(&tabBar, button, {}, barPos);
        QCOMPARE(clickSpy.size(), 1);
        QCOMPARE(clickSpy.takeFirst().takeFirst().toInt(), -1);
        QCOMPARE(doubleClickSpy.size(), 1);
        QCOMPARE(doubleClickSpy.takeFirst().takeFirst().toInt(), -1);
        BOBUIest::mouseRelease(&tabBar, button, {}, barPos);

        button = BobUI::MouseButton(button << 1);
    }
}

void tst_BOBUIabBar::autoHide()
{
    BOBUIabBar tabBar;
    QVERIFY(!tabBar.autoHide());
    QVERIFY(!tabBar.isVisible());
    tabBar.show();
    QVERIFY(tabBar.isVisible());
    tabBar.addTab("0");
    QVERIFY(tabBar.isVisible());
    tabBar.removeTab(0);
    QVERIFY(tabBar.isVisible());

    tabBar.setAutoHide(true);
    QVERIFY(!tabBar.isVisible());
    tabBar.addTab("0");
    QVERIFY(!tabBar.isVisible());
    tabBar.addTab("1");
    QVERIFY(tabBar.isVisible());
    tabBar.removeTab(0);
    QVERIFY(!tabBar.isVisible());
    tabBar.removeTab(0);
    QVERIFY(!tabBar.isVisible());

    tabBar.setAutoHide(false);
    QVERIFY(tabBar.isVisible());
}

void tst_BOBUIabBar::mouseReleaseOutsideTabBar()
{
    class RepaintChecker : public QObject
    {
    public:
        bool repainted = false;
        QRect rectToBeRepainted;
        bool eventFilter(QObject *, QEvent *event) override
        {
            if (event->type() == QEvent::Paint &&
                static_cast<QPaintEvent *>(event)->rect().contains(rectToBeRepainted)) {
                repainted = true;
            }
            return false;
        }
    } repaintChecker;

    BOBUIabBar tabBar;
    tabBar.installEventFilter(&repaintChecker);
    tabBar.addTab("    ");
    tabBar.addTab("    ");
    tabBar.show();
    if (!BOBUIest::qWaitForWindowExposed(&tabBar))
        QSKIP("Window failed to show, skipping test");

    QRect tabRect = tabBar.tabRect(1);
    QPoint tabCenter = tabRect.center();
    repaintChecker.rectToBeRepainted = tabRect;
    // if a press repaints the tab...
    BOBUIest::mousePress(&tabBar, BobUI::LeftButton, {}, tabCenter);
    const bool pressRepainted = BOBUIest::qWaitFor([&]{ return repaintChecker.repainted; }, 250);

    // ... then releasing the mouse outside the tabbar should repaint it as well
    repaintChecker.repainted = false;
    BOBUIest::mouseRelease(&tabBar, BobUI::LeftButton, {}, tabCenter + QPoint(tabCenter.x(), tabCenter.y() + tabRect.height()));
    BOBUIRY_COMPARE(repaintChecker.repainted, pressRepainted);
}

void tst_BOBUIabBar::checkPositions(const TabBar &tabbar, const QList<int> &positions)
{
    QStyleOptionTab option;
    int iPos = 0;
    for (int i = 0; i < tabbar.count(); ++i) {
        if (!tabbar.isTabVisible(i))
            continue;
        tabbar.initStyleOption(&option, i);
        QCOMPARE(option.position, positions.at(iPos++));
    }
}

#if BOBUI_CONFIG(wheelevent)

class TabBarScrollingProxyStyle : public QProxyStyle
{
public:
    TabBarScrollingProxyStyle(const QString &defStyle = {})
        : QProxyStyle(defStyle), scrolling(true)
    { }

    int styleHint(StyleHint hint, const QStyleOption *option = 0,
                  const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const override
    {
        switch (hint) {
        case QStyle::SH_TabBar_AllowWheelScrolling:
            return scrolling;
        case SH_TabBar_ElideMode:
            return BobUI::ElideNone;
        default:
            break;
        }

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }

    bool scrolling;
};

void tst_BOBUIabBar::mouseWheel()
{
    TabBar tabbar;

    // apply custom style to the tabbar, which can toggle tabbar scrolling behavior
    TabBarScrollingProxyStyle proxyStyle;
    tabbar.setStyle(&proxyStyle);

    // make tabbar with three tabs, select the middle one
    tabbar.addTab("one");
    tabbar.addTab("two");
    tabbar.addTab("three");
    int startIndex = 1;
    tabbar.setCurrentIndex(startIndex);

    const auto systemId = QPointingDevice::primaryPointingDevice()->systemId() + 1;
    QPointingDevice clickyWheel("test clicky wheel", systemId, QInputDevice::DeviceType::Mouse,
                                QPointingDevice::PointerType::Generic,
                                QInputDevice::Capability::Position | QInputDevice::Capability::Scroll,
                                1, 3);

    // define scroll event
    const QPoint wheelPoint = tabbar.rect().bottomRight();
    QWheelEvent event(wheelPoint, tabbar.mapToGlobal(wheelPoint),
                      QPoint(), QPoint(0, QWheelEvent::DefaultDeltasPerStep),
                      BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false,
                      BobUI::MouseEventSynthesizedByApplication, &clickyWheel);

    // disable scrolling, send scroll event, confirm that tab did not change
    proxyStyle.scrolling = false;
    QVERIFY(QApplication::sendEvent(&tabbar, &event));
    QVERIFY(tabbar.currentIndex() == startIndex);

    // enable scrolling, send scroll event, confirm that tab changed
    proxyStyle.scrolling = true;
    QVERIFY(QApplication::sendEvent(&tabbar, &event));
    QVERIFY(tabbar.currentIndex() != startIndex);
}

void tst_BOBUIabBar::kineticWheel_data()
{
    BOBUIest::addColumn<BOBUIabBar::Shape>("tabShape");

    BOBUIest::addRow("North") << BOBUIabBar::RoundedNorth;
    BOBUIest::addRow("East") << BOBUIabBar::RoundedEast;
    BOBUIest::addRow("South") << BOBUIabBar::RoundedSouth;
    BOBUIest::addRow("West") << BOBUIabBar::RoundedWest;
}

void tst_BOBUIabBar::kineticWheel()
{
    const auto systemId = QPointingDevice::primaryPointingDevice()->systemId() + 1;
    QPointingDevice pixelPad("test pixel pad", systemId, QInputDevice::DeviceType::TouchPad,
                             QPointingDevice::PointerType::Generic,
                             QInputDevice::Capability::Position | QInputDevice::Capability::PixelScroll,
                             1, 3);

    QFETCH(BOBUIabBar::Shape, tabShape);
    QWidget window;
    TabBar tabbar(&window);
    // Since the macOS style makes sure that all tabs are always visible, we
    // replace it with the windows style for this test, and use the proxy that
    // makes sure that scrolling is enabled and that tab texts are not elided.
    QString defaultStyle;
    if (QApplication::style()->name() == QStringLiteral("macos"))
        defaultStyle = "windows";
    TabBarScrollingProxyStyle proxyStyle(defaultStyle);
    tabbar.setStyle(&proxyStyle);

    tabbar.addTab("long tab text 1");
    tabbar.addTab("long tab text 2");
    tabbar.addTab("long tab text 3");

    // Make sure we don't have enough space for the tabs and need to scroll
    const int tabbarLength = tabbar.tabRect(0).width() * 2;

    tabbar.setShape(tabShape);
    const bool horizontal = tabShape == BOBUIabBar::RoundedNorth
                         || tabShape == BOBUIabBar::RoundedSouth;
    if (horizontal)
        tabbar.setFixedWidth(tabbarLength);
    else
        tabbar.setFixedHeight(tabbarLength);

    // start with the middle tab, BOBUIabBar will scroll to make it visible
    const int startIndex = 1;
    tabbar.setCurrentIndex(startIndex);

    window.setMinimumSize(tabbarLength, tabbarLength);
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&window));

    const auto *leftButton = tabbar.findChild<QAbstractButton*>(u"ScrollLeftButton"_s);
    const auto *rightButton = tabbar.findChild<QAbstractButton*>(u"ScrollRightButton"_s);
    QVERIFY(leftButton && rightButton);
    QVERIFY(leftButton->isEnabled() && rightButton->isEnabled());

    // Figure out if any of the buttons is laid out to be in front of the tabs.
    // We can't use setUsesScrollButtons(false), as then several styles will enforce
    // a minimum size for the tab bar.
    const bool leftInFront = ((horizontal && leftButton->pos().x() < tabbar.rect().center().x())
                           || (!horizontal && leftButton->pos().y() < tabbar.rect().center().y()));
    const bool rightInFront = ((horizontal && rightButton->pos().x() < tabbar.rect().center().x())
                            || (!horizontal && rightButton->pos().y() < tabbar.rect().center().y()));
    QPoint leftEdge;
    QPoint rightEdge;
    if (leftInFront && rightInFront) { // both on the left
        leftEdge = rightButton->geometry().bottomRight();
        rightEdge = tabbar.rect().bottomRight();
    } else if (leftInFront && !rightInFront) {
        leftEdge = leftButton->geometry().bottomRight();
        rightEdge = rightButton->geometry().topLeft();
    } else { // both on the right
        leftEdge = QPoint(0, 0);
        rightEdge = leftButton->geometry().topLeft();
    }
    // make sure the point is inside tabbar rect
    const auto tabbarCenter = tabbar.geometry().center();
    if (horizontal) {
        leftEdge = QPoint(leftEdge.x() + 10, tabbarCenter.y());
        rightEdge = QPoint(rightEdge.x() - 10, tabbarCenter.y());
    } else {
        leftEdge = QPoint(tabbarCenter.x(), leftEdge.y() + 10);
        rightEdge = QPoint(tabbarCenter.x(), rightEdge.y() - 10);
    }

    QCOMPARE(tabbar.tabAt(leftEdge), 0);
    QCOMPARE(tabbar.tabAt(rightEdge), 1);

    const QPoint delta = horizontal ? QPoint(10, 0) : QPoint(0, 10);
    const QPoint wheelPoint = tabbar.rect().center();

    bool accepted = true;
    BobUI::ScrollPhase phase = BobUI::ScrollBegin;
    // scroll all the way to the end
    while (accepted) {
        QWheelEvent event(wheelPoint, tabbar.mapToGlobal(wheelPoint), -delta, -delta,
                          BobUI::NoButton, BobUI::NoModifier, phase, false,
                          BobUI::MouseEventSynthesizedByApplication, &pixelPad);
        if (phase == BobUI::ScrollBegin)
            phase = BobUI::ScrollUpdate;
        QApplication::sendEvent(&tabbar, &event);
        accepted = event.isAccepted();
    }
    QCOMPARE(tabbar.tabAt(leftEdge), 1);
    QCOMPARE(tabbar.tabAt(rightEdge), 2);
    QVERIFY(leftButton->isEnabled());
    QVERIFY(!rightButton->isEnabled());
    // kinetic wheel events don't change the current index
    QVERIFY(tabbar.currentIndex() == startIndex);

    // scroll all the way to the beginning
    accepted = true;
    while (accepted) {
        QWheelEvent event(wheelPoint, tabbar.mapToGlobal(wheelPoint), delta, delta,
                          BobUI::NoButton, BobUI::NoModifier, phase, false,
                          BobUI::MouseEventSynthesizedByApplication, &pixelPad);
        QApplication::sendEvent(&tabbar, &event);
        accepted = event.isAccepted();
    }

    QCOMPARE(tabbar.tabAt(leftEdge), 0);
    QCOMPARE(tabbar.tabAt(rightEdge), 1);
    QVERIFY(!leftButton->isEnabled());
    QVERIFY(rightButton->isEnabled());
    // kinetic wheel events don't change the current index
    QVERIFY(tabbar.currentIndex() == startIndex);

    // make tabs small so that we have enough space, and verify sure we can't scroll
    tabbar.setTabText(0, "A");
    tabbar.setTabText(1, "B");
    tabbar.setTabText(2, "C");
    QVERIFY(tabbar.sizeHint().width() <= tabbar.width() && tabbar.sizeHint().height() <= tabbar.height());

    {
        QWheelEvent event(wheelPoint, tabbar.mapToGlobal(wheelPoint), -delta, -delta,
                          BobUI::NoButton, BobUI::NoModifier, phase, false,
                          BobUI::MouseEventSynthesizedByApplication, &pixelPad);
        QApplication::sendEvent(&tabbar, &event);
        QVERIFY(!event.isAccepted());
    }

    {
        QWheelEvent event(wheelPoint, tabbar.mapToGlobal(wheelPoint), delta, delta,
                          BobUI::NoButton, BobUI::NoModifier, phase, false,
                          BobUI::MouseEventSynthesizedByApplication, &pixelPad);
        QApplication::sendEvent(&tabbar, &event);
        QVERIFY(!event.isAccepted());
    }
}

void tst_BOBUIabBar::highResolutionWheel_data()
{
    BOBUIest::addColumn<int>("angleDelta");
    // Smallest angleDelta for a Logitech MX Master 3 with Linux/X11/Libinput
    BOBUIest::addRow("increment index") << -16;
    BOBUIest::addRow("decrement index") << 16;
}

void tst_BOBUIabBar::highResolutionWheel()
{
    TabBar tabbar;
    TabBarScrollingProxyStyle proxyStyle;
    tabbar.setStyle(&proxyStyle);

    tabbar.addTab("tab1");
    tabbar.addTab("tab2");
    QFETCH(int, angleDelta);
    // Negative values increment, positive values decrement
    int startIndex = angleDelta < 0 ? 0 : 1;
    tabbar.setCurrentIndex(startIndex);

    const auto systemId = QPointingDevice::primaryPointingDevice()->systemId() + 1;
    QPointingDevice hiResWheel(
            "test high resolution wheel", systemId, QInputDevice::DeviceType::Mouse,
            QPointingDevice::PointerType::Generic,
            QInputDevice::Capability::Position | QInputDevice::Capability::Scroll, 1, 3);

    const QPoint wheelPoint = tabbar.rect().bottomRight();
    QWheelEvent event(wheelPoint, tabbar.mapToGlobal(wheelPoint), QPoint(),
                      QPoint(angleDelta, angleDelta), BobUI::NoButton, BobUI::NoModifier,
                      BobUI::NoScrollPhase, false, BobUI::MouseEventSynthesizedByApplication,
                      &hiResWheel);

    proxyStyle.scrolling = true;
    for (int accumulated = 0; accumulated < QWheelEvent::DefaultDeltasPerStep;
         accumulated += qAbs(angleDelta)) {
        // verify that nothing has changed until the threshold has been reached
        QVERIFY(tabbar.currentIndex() == startIndex);
        QVERIFY(QApplication::sendEvent(&tabbar, &event));
    }
    QVERIFY(tabbar.currentIndex() != startIndex);
}

#endif // BOBUI_CONFIG(wheelevent)

void tst_BOBUIabBar::scrollButtons_data()
{
    BOBUIest::addColumn<BOBUIabWidget::TabPosition>("tabPosition");
    BOBUIest::addColumn<BobUI::LayoutDirection>("layoutDirection");

    for (auto ld : {BobUI::LeftToRight, BobUI::RightToLeft}) {
        const char *ldStr = ld == BobUI::LeftToRight ? "LTR" : "RTL";
        BOBUIest::addRow("North, %s", ldStr) << BOBUIabWidget::North << ld;
        BOBUIest::addRow("South, %s", ldStr) << BOBUIabWidget::South << ld;
        BOBUIest::addRow("West, %s", ldStr) << BOBUIabWidget::West << ld;
        BOBUIest::addRow("East, %s", ldStr) << BOBUIabWidget::East << ld;
    }
}

void tst_BOBUIabBar::scrollButtons()
{
    QFETCH(BOBUIabWidget::TabPosition, tabPosition);
    QFETCH(BobUI::LayoutDirection, layoutDirection);

    QWidget window;
    BOBUIabWidget tabWidget(&window);
    tabWidget.setLayoutDirection(layoutDirection);
    tabWidget.setTabPosition(tabPosition);
    tabWidget.setElideMode(BobUI::ElideNone);
    tabWidget.setUsesScrollButtons(true);

    const int tabCount = 5;
    for (int i = 0; i < tabCount; ++i)
    {
        const QString num = QString::number(i);
        tabWidget.addTab(new QLabel(num), num + " - Really long tab name to force arrows");
    }
    tabWidget.move(0, 0);
    tabWidget.resize(tabWidget.minimumSizeHint());
    window.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&window));

    auto *leftB = tabWidget.tabBar()->findChild<QAbstractButton*>(u"ScrollLeftButton"_s);
    auto *rightB = tabWidget.tabBar()->findChild<QAbstractButton*>(u"ScrollRightButton"_s);

    QVERIFY(leftB->isVisible());
    QVERIFY(!leftB->isEnabled());
    QVERIFY(rightB->isVisible());
    QVERIFY(rightB->isEnabled());
    QVERIFY(!tabWidget.tabBar()->tabRect(1).intersects(tabWidget.tabBar()->rect()));

    int index = 0;
    for (; index < tabWidget.count(); ++index) {
        QCOMPARE(leftB->isEnabled(), index > 0);
        QCOMPARE(rightB->isEnabled(), index < tabWidget.count() - 1);
        QVERIFY(tabWidget.tabBar()->tabRect(index).intersects(tabWidget.tabBar()->rect()));
        QCOMPARE(tabWidget.tabBar()->tabAt(tabWidget.tabBar()->rect().center()), index);
        if (rightB->isEnabled())
            rightB->click();
    }
    for (--index; index >= 0; --index) {
        QCOMPARE(leftB->isEnabled(), index >= 0);
        QCOMPARE(rightB->isEnabled(), index < tabWidget.count() - 1);

        QVERIFY(tabWidget.tabBar()->tabRect(index).intersects(tabWidget.tabBar()->rect()));
        if (leftB->isEnabled())
            leftB->click();
    }
    QVERIFY(!leftB->isEnabled());
}

void tst_BOBUIabBar::currentTabLargeFont()
{
    TabBar tabBar;
    tabBar.setStyleSheet(R"(
        BOBUIabBar::tab::selected {
            font-size: 24pt;
        }
    )");

    tabBar.addTab("Tab Item 1");
    tabBar.addTab("Tab Item 2");
    tabBar.addTab("Tab Item 3");

    tabBar.setCurrentIndex(0);
    tabBar.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabBar));

    QList<QRect> oldTabRects;
    oldTabRects << tabBar.tabRect(0) << tabBar.tabRect(1) << tabBar.tabRect(2);
    tabBar.setCurrentIndex(1);
    QList<QRect> newTabRects;
    newTabRects << tabBar.tabRect(0) << tabBar.tabRect(1) << tabBar.tabRect(2);
    QVERIFY(oldTabRects != newTabRects);
}

void tst_BOBUIabBar::hoverTab_data()
{
    // Move the cursor away from the widget as not to interfere.
    // skip this test if we can't
    const QPoint topLeft = QGuiApplication::primaryScreen()->availableGeometry().topLeft();
    const QPoint cursorPos = topLeft + QPoint(10, 10);
    QCursor::setPos(cursorPos);
    if (!BOBUIest::qWaitFor([cursorPos]{ return QCursor::pos() == cursorPos; }, 500))
        QSKIP("Can't move mouse");

    BOBUIest::addColumn<bool>("documentMode");
    BOBUIest::addRow("normal mode") << true;
    BOBUIest::addRow("document mode") << true;
}

void tst_BOBUIabBar::hoverTab()
{
    QFETCH(bool, documentMode);
    QWidget topLevel;
    class TabBar : public BOBUIabBar
    {
    public:
        using BOBUIabBar::BOBUIabBar;
        void initStyleOption(QStyleOptionTab *option, int tabIndex) const override
        {
            BOBUIabBar::initStyleOption(option, tabIndex);
            styleOptions[tabIndex] = *option;
        }
        mutable QHash<int, QStyleOptionTab> styleOptions;
    } tabbar(&topLevel);

    tabbar.setDocumentMode(documentMode);
    tabbar.addTab("A");
    tabbar.addTab("B");
    tabbar.addTab("C");
    tabbar.addTab("D");

    tabbar.move(0,0);
    const QSize size = tabbar.sizeHint();
    const auto center = topLevel.screen()->availableGeometry().center();
    topLevel.move(center - QPoint{size.width(), size.height()} / 2);
    topLevel.setMinimumSize(size);
    tabbar.ensurePolished();

    // some styles set those flags, some don't. If not, use a style sheet
    if (!(tabbar.testAttribute(BobUI::WA_Hover) || tabbar.hasMouseTracking())) {
        tabbar.setStyleSheet(R"(
            BOBUIabBar::tab { background: blue; }
            BOBUIabBar::tab::hover { background: yellow; }
            BOBUIabBar::tab::selected { background: red; }
        )");
    }

    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&topLevel));
    auto *window = topLevel.windowHandle();

    auto pos = tabbar.mapToParent(tabbar.tabRect(0).center());
    BOBUIest::mouseMove(window, pos);
    BOBUIRY_VERIFY(tabbar.styleOptions[0].state & QStyle::State_Selected);
    BOBUIRY_COMPARE(tabbar.styleOptions[1].state & QStyle::State_MouseOver, QStyle::State_None);
    BOBUIRY_COMPARE(tabbar.styleOptions[2].state & QStyle::State_MouseOver, QStyle::State_None);
    BOBUIRY_COMPARE(tabbar.styleOptions[3].state & QStyle::State_MouseOver, QStyle::State_None);

    pos = tabbar.mapToParent(tabbar.tabRect(1).center());
    BOBUIest::mouseMove(window, pos);
    BOBUIRY_COMPARE(tabbar.styleOptions[1].state & QStyle::State_MouseOver, QStyle::State_MouseOver);
    QCOMPARE(tabbar.styleOptions[2].state & QStyle::State_MouseOver, QStyle::State_None);
    QCOMPARE(tabbar.styleOptions[3].state & QStyle::State_MouseOver, QStyle::State_None);

    pos = tabbar.mapToParent(tabbar.tabRect(2).center());
    BOBUIest::mouseMove(window, pos);
    BOBUIRY_COMPARE(tabbar.styleOptions[2].state & QStyle::State_MouseOver, QStyle::State_MouseOver);
    QCOMPARE(tabbar.styleOptions[1].state & QStyle::State_MouseOver, QStyle::State_None);
    QCOMPARE(tabbar.styleOptions[3].state & QStyle::State_MouseOver, QStyle::State_None);

    // removing tab 2 lays the tabs out so that they stretch across the
    // tab bar; tab 1 is now where the cursor was. What matters is that a
    // different tab is now hovered (rather than none).
    tabbar.removeTab(2);
    BOBUIRY_COMPARE(tabbar.styleOptions[1].state & QStyle::State_MouseOver, QStyle::State_MouseOver);
    QCOMPARE(tabbar.styleOptions[2].state & QStyle::State_MouseOver, QStyle::State_None);

    // inserting a tab at index 2 again should paint the new tab hovered
    tabbar.insertTab(2, "X");
    BOBUIRY_COMPARE(tabbar.styleOptions[2].state & QStyle::State_MouseOver, QStyle::State_MouseOver);
    QCOMPARE(tabbar.styleOptions[1].state & QStyle::State_MouseOver, QStyle::State_None);
}


void tst_BOBUIabBar::resizeKeepsScroll_data()
{
    BOBUIest::addColumn<BOBUIabBar::Shape>("tabShape");
    BOBUIest::addColumn<bool>("expanding");

    BOBUIest::addRow("North, expanding") << BOBUIabBar::RoundedNorth << true;
    BOBUIest::addRow("East, expanding") << BOBUIabBar::RoundedEast << true;
    BOBUIest::addRow("South, expanding") << BOBUIabBar::RoundedSouth << true;
    BOBUIest::addRow("West, expanding") << BOBUIabBar::RoundedWest << true;

    BOBUIest::addRow("North, not expanding") << BOBUIabBar::RoundedNorth << false;
    BOBUIest::addRow("South, not expanding") << BOBUIabBar::RoundedSouth << false;
}

void tst_BOBUIabBar::resizeKeepsScroll()
{
    QFETCH(BOBUIabBar::Shape, tabShape);
    QFETCH(const bool, expanding);

    BOBUIabBar tabBar;
    TabBarScrollingProxyStyle proxyStyle;
    tabBar.setStyle(&proxyStyle);

    for (int i = 0; i < 10; ++i)
        tabBar.addTab(u"Tab Number %1"_s.arg(i));

    tabBar.setShape(tabShape);
    tabBar.setUsesScrollButtons(true);
    tabBar.setExpanding(expanding);

    // resize to half
    const QSize fullSize = tabBar.sizeHint();
    const bool horizontal = fullSize.width() > fullSize.height();
    if (horizontal)
        tabBar.resize(fullSize.width() / 2, fullSize.height());
    else
        tabBar.resize(fullSize.width(), fullSize.height() / 2);

    tabBar.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabBar));

    const auto getScrollOffset = [&]() -> int {
        return static_cast<BOBUIabBarPrivate *>(QObjectPrivate::get(&tabBar))->scrollOffset;
    };

    // select a tab outside, this will scroll
    tabBar.setCurrentIndex(6);
    // the first tab is now scrolled out
    const int scrollOffset = getScrollOffset();
    QCOMPARE_GT(scrollOffset, 0);
    // the current index is now fully visible, with margin on both sides
    tabBar.setCurrentIndex(5);

    // make the tab bar a bit larger, by the width of a tab
    if (horizontal)
        tabBar.resize(tabBar.width() + tabBar.tabRect(5).width(), tabBar.height());
    else
        tabBar.resize(tabBar.width(), tabBar.height() + tabBar.tabRect(5).height());

    // this should not change the scroll
    QCOMPARE(getScrollOffset(), scrollOffset);

    // make the tab bar large enough to fit everything with extra space
    tabBar.resize(fullSize + QSize(50, 50));

    // there should be no scroll
    QCOMPARE(getScrollOffset(), 0);

    for (int i = 0; i < tabBar.count(); ++i) {
        tabBar.setCurrentIndex(i);
        QCOMPARE(getScrollOffset(), 0);
    }
}

void tst_BOBUIabBar::changeTabTextKeepsScroll()
{
    BOBUIabBar tabBar;
    TabBarScrollingProxyStyle proxyStyle;
    tabBar.setStyle(&proxyStyle);

    for (int i = 0; i < 6; ++i)
        tabBar.addTab(u"Tab Number %1"_s.arg(i));

    const QSize fullSize = tabBar.sizeHint();
    tabBar.resize(fullSize.width() / 2, fullSize.height());

    tabBar.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabBar));

    const auto getScrollOffset = [&]() -> int {
        return static_cast<BOBUIabBarPrivate *>(QObjectPrivate::get(&tabBar))->scrollOffset;
    };

    tabBar.setCurrentIndex(3);
    const int scrollOffset = getScrollOffset();
    tabBar.setTabText(3, "New title");
    QCOMPARE(getScrollOffset(), scrollOffset);
}

void tst_BOBUIabBar::settingCurrentTabBeforeShowDoesntScroll()
{
    BOBUIabBar tabBar;
    TabBarScrollingProxyStyle proxyStyle;
    tabBar.setStyle(&proxyStyle);

    for (int i = 0; i < 6; ++i)
        tabBar.addTab(u"Tab Number %1"_s.arg(i));

    const auto getScrollOffset = [&]() -> int {
        return static_cast<BOBUIabBarPrivate *>(QObjectPrivate::get(&tabBar))->scrollOffset;
    };

    tabBar.setCurrentIndex(5);

    // changing the current index while the tab bar isn't visible shouldn't scroll yet
    QCOMPARE(getScrollOffset(), 0);

    // now show the tab bar with a size that's too small to fit the current index
    const QSize fullSize = tabBar.sizeHint();
    tabBar.resize(fullSize.width() / 2, fullSize.height());

    tabBar.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabBar));

    // this should scroll
    QCOMPARE_GT(getScrollOffset(), 0);
}

void tst_BOBUIabBar::checkPositionsAfterShapeChange()
{
    class TabWidget : public BOBUIabWidget
    {
    public:
        using BOBUIabWidget::BOBUIabWidget;
        using BOBUIabWidget::setTabBar;
    };

    class TabBar : public BOBUIabBar
    {
    public:
        using BOBUIabBar::initStyleOption;
        void resizeEvent(QResizeEvent *e) override
        {
            BOBUIabBar::resizeEvent(e);
            resized = true;
        }
        bool resized = false;
    };

    TabWidget tabWidget;
    auto *tabBar = new TabBar;
    tabWidget.setTabBar(tabBar);
    for (int i = 0; i < 3; ++i)
        tabWidget.addTab(new QWidget, u"Tab %1"_s.arg(i));
    tabWidget.setTabPosition(BOBUIabWidget::North);
    tabWidget.setCurrentIndex(2);
    tabWidget.resize(300, 300);
    tabWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabWidget));

    tabBar->resized = false;
    tabWidget.setTabPosition(BOBUIabWidget::East);
    QVERIFY(BOBUIest::qWaitFor([&]() { return tabBar->resized; }));
    QStyleOptionTab opt;
    tabBar->initStyleOption(&opt, 2);
    QVERIFY(opt.rect.top() > 0);
}

void tst_BOBUIabBar::checkScrollOffsetAfterTabRemoval()
{
    BOBUIabWidget tabWidget;
    BOBUIabBar *tabBar = tabWidget.tabBar();
    for (int i = 0; i < 10; ++i)
        tabWidget.addTab(new QWidget, u"Tab %1"_s.arg(i));
    tabWidget.setTabPosition(BOBUIabWidget::North);
    tabWidget.resize(300, 300);
    tabWidget.setCurrentIndex(0);
    tabWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabWidget));

    auto *rightButton = tabBar->findChild<QAbstractButton *>(u"ScrollRightButton"_s);
    auto *leftButton = tabBar->findChild<QAbstractButton *>(u"ScrollLeftButton"_s);
    QVERIFY(leftButton);
    QVERIFY(rightButton);
    QVERIFY(rightButton->isEnabled());
    QVERIFY(!leftButton->isEnabled());
    // scroll to the right
    tabBar->setCurrentIndex(9);
    QVERIFY(!rightButton->isEnabled());
    QVERIFY(leftButton->isEnabled());
    // scroll to the center
    tabBar->setCurrentIndex(2);
    QVERIFY(rightButton->isEnabled());
    QVERIFY(leftButton->isEnabled());

    const auto getScrollOffset = [&]() -> int {
        return static_cast<BOBUIabBarPrivate *>(QObjectPrivate::get(tabBar))->scrollOffset;
    };
    // the scroll offset should not change when a tab right outside
    // the scroll rect is removed
    auto oldOffset = getScrollOffset();
    tabWidget.removeTab(9);
    QCOMPARE(getScrollOffset(), oldOffset);
    // the scroll offset must change when a tab left outside
    // the scroll rect is removed
    oldOffset = getScrollOffset();
    tabWidget.removeTab(0);
    QVERIFY(getScrollOffset() < oldOffset);

    // the scroll offset must change when there is empty
    // place in the right after tab removal
    oldOffset = getScrollOffset();
    QVERIFY(oldOffset > 0);
    for (int i : { 7, 6, 5, 4, 3 })
        tabWidget.removeTab(i);
    QCOMPARE(getScrollOffset(), 0);
    QVERIFY(!rightButton->isVisible());
    QVERIFY(!leftButton->isVisible());
}

BOBUIEST_MAIN(tst_BOBUIabBar)
#include "tst_bobuiabbar.moc"
