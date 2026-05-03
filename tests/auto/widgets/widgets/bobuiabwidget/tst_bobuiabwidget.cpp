// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <bobuiabwidget.h>
#include <bobuiabbar.h>
#include <qdebug.h>
#include <qapplication.h>
#include <qlabel.h>
#include <BobUIWidgets/qboxlayout.h>

#if defined(Q_OS_WIN)
#  include <bobui_windows.h>
#define Q_CHECK_PAINTEVENTS \
    if (::SwitchDesktop(::GetThreadDesktop(::GetCurrentThreadId())) == 0) \
        QSKIP("desktop is not visible, this test would fail");
#else
#define Q_CHECK_PAINTEVENTS
#endif

class BOBUIabWidgetChild:public BOBUIabWidget {
  public:
    BOBUIabWidgetChild():tabCount(0) {
        QVERIFY(tabBar() != NULL);
        QWidget *w = new QWidget;
        int index = addTab(w, "test");
          QCOMPARE(tabCount, 1);
          removeTab(index);
          QCOMPARE(tabCount, 0);

          // Test bad arguments
          // This will assert, so don't do it :)
          //setTabBar(NULL);
    };

  protected:
    virtual void tabInserted(int /*index */ ) override
    {
        tabCount++;
    };
    virtual void tabRemoved(int /*index */ ) override
    {
        tabCount--;
    };
    int tabCount;
};

class tst_BOBUIabWidget:public QObject {
  Q_OBJECT

private slots:
    void init();
    void cleanup();

    void getSetCheck();
    void testChild();
    void addRemoveTab();
    void tabPosition();
    void tabEnabled();
    void tabHidden();
    void checkHiddenTab();
    void tabText();
    void tabShape();
    void tabTooltip();
    void tabIcon();
    void indexOf();
    void currentWidget();
    void currentIndex();
    void cornerWidget();
    void removeTab();
    void clear();
    void keyboardNavigation();
    void paintEventCount();
    void minimumSizeHint();
    void heightForWidth_data();
    void heightForWidth();
    void tabBarClicked();
    void moveCurrentTab();
    void autoHide();

    void setCurrentBeforeShow_data();
    void setCurrentBeforeShow();

  private:
    int addPage();
    void removePage(int index);
    BOBUIabWidget *tw;
};

// Testing get/set functions
void tst_BOBUIabWidget::getSetCheck()
{
    BOBUIabWidget obj1;
    QWidget *w1 = new QWidget;
    QWidget *w2 = new QWidget;
    QWidget *w3 = new QWidget;
    QWidget *w4 = new QWidget;
    QWidget *w5 = new QWidget;

    obj1.addTab(w1, "Page 1");
    obj1.addTab(w2, "Page 2");
    obj1.addTab(w3, "Page 3");
    obj1.addTab(w4, "Page 4");
    obj1.addTab(w5, "Page 5");

    // TabShape BOBUIabWidget::tabShape()
    // void BOBUIabWidget::setTabShape(TabShape)
    obj1.setTabShape(BOBUIabWidget::TabShape(BOBUIabWidget::Rounded));
    QCOMPARE(BOBUIabWidget::TabShape(BOBUIabWidget::Rounded), obj1.tabShape());
    obj1.setTabShape(BOBUIabWidget::TabShape(BOBUIabWidget::Triangular));
    QCOMPARE(BOBUIabWidget::TabShape(BOBUIabWidget::Triangular), obj1.tabShape());

    // int BOBUIabWidget::currentIndex()
    // void BOBUIabWidget::setCurrentIndex(int)
    obj1.setCurrentIndex(0);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MIN);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MAX);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(4);
    QCOMPARE(4, obj1.currentIndex());

    // QWidget * BOBUIabWidget::currentWidget()
    // void BOBUIabWidget::setCurrentWidget(QWidget *)
    obj1.setCurrentWidget(w1);
    QCOMPARE(w1, obj1.currentWidget());
    obj1.setCurrentWidget(w5);
    QCOMPARE(w5, obj1.currentWidget());
    obj1.setCurrentWidget((QWidget *)0);
    QCOMPARE(w5, obj1.currentWidget()); // current not changed
}

void tst_BOBUIabWidget::init()
{
    tw = new BOBUIabWidget(0);
    QCOMPARE(tw->count(), 0);
    QCOMPARE(tw->currentIndex(), -1);
    QVERIFY(!tw->currentWidget());
}

void tst_BOBUIabWidget::cleanup()
{
    delete tw;
    tw = 0;
}

void tst_BOBUIabWidget::testChild()
{
    BOBUIabWidgetChild t;
}

#define LABEL "TEST"
#define TIP "TIP"
int tst_BOBUIabWidget::addPage()
{
    QWidget *w = new QWidget();
    return tw->addTab(w, LABEL);
}

void tst_BOBUIabWidget::removePage(int index)
{
    QWidget *w = tw->widget(index);
    tw->removeTab(index);
    delete w;
}

/**
 * Tests:
 * addTab(...) which really calls -> insertTab(...)
 * widget(...)
 * removeTab(...);
 * If this fails then many others probably will too.
 */
void tst_BOBUIabWidget::addRemoveTab()
{
    // Test bad arguments
    tw->addTab(NULL, LABEL);
    QCOMPARE(tw->count(), 0);
    tw->removeTab(-1);
    QCOMPARE(tw->count(), 0);
    QVERIFY(!tw->widget(-1));

    QWidget *w = new QWidget();
    int index = tw->addTab(w, LABEL);
    // return value
    QCOMPARE(tw->indexOf(w), index);

    QCOMPARE(tw->count(), 1);
    QCOMPARE(tw->widget(index), w);
    QCOMPARE(tw->tabText(index), QString(LABEL));

    removePage(index);
    QCOMPARE(tw->count(), 0);
}

void tst_BOBUIabWidget::tabPosition()
{
    tw->setTabPosition(BOBUIabWidget::North);
    QCOMPARE(tw->tabPosition(), BOBUIabWidget::North);
    tw->setTabPosition(BOBUIabWidget::South);
    QCOMPARE(tw->tabPosition(), BOBUIabWidget::South);
    tw->setTabPosition(BOBUIabWidget::East);
    QCOMPARE(tw->tabPosition(), BOBUIabWidget::East);
    tw->setTabPosition(BOBUIabWidget::West);
    QCOMPARE(tw->tabPosition(), BOBUIabWidget::West);
}

void tst_BOBUIabWidget::tabEnabled()
{
    // Test bad arguments
    QVERIFY(!tw->isTabEnabled(-1));
    tw->setTabEnabled(-1, false);

    int index = addPage();

    tw->setTabEnabled(index, true);
    QVERIFY(tw->isTabEnabled(index));
    QVERIFY(tw->widget(index)->isEnabled());
    tw->setTabEnabled(index, false);
    QVERIFY(!tw->isTabEnabled(index));
    QVERIFY(!tw->widget(index)->isEnabled());
    tw->setTabEnabled(index, true);
    QVERIFY(tw->isTabEnabled(index));
    QVERIFY(tw->widget(index)->isEnabled());

    removePage(index);
}

void tst_BOBUIabWidget::tabHidden()
{
    // Test bad arguments
    QVERIFY(!tw->isTabVisible(-1));
    tw->setTabVisible(-1, false);
    QVERIFY(!tw->isTabVisible(tw->count()));
    tw->setTabVisible(tw->count(), false);

    const int index = addPage();

    tw->setTabVisible(index, true);
    QVERIFY(tw->isTabVisible(index));
    tw->setTabVisible(index, false);
    QVERIFY(!tw->isTabVisible(index));
    tw->setTabVisible(index, true);
    QVERIFY(tw->isTabVisible(index));

    removePage(index);

    for (int i = 0; i < tw->count(); ++i) {
        QVERIFY(tw->isTabVisible(i));
    }
}

void tst_BOBUIabWidget::checkHiddenTab()
{
    tw->addTab(new QWidget(), "foo");
    tw->addTab(new QWidget(), "bar");
    tw->addTab(new QWidget(), "baz");
    QCOMPARE(tw->count(), 3);
    tw->setCurrentIndex(0);
    tw->setTabVisible(1, false);

    QKeyEvent keyTab(QKeyEvent::KeyPress, BobUI::Key_Tab, BobUI::ControlModifier);
    QVERIFY(QApplication::sendEvent(tw, &keyTab));
    QCOMPARE(tw->currentIndex(), 2);
    QVERIFY(QApplication::sendEvent(tw, &keyTab));
    QCOMPARE(tw->currentIndex(), 0);
    QVERIFY(QApplication::sendEvent(tw, &keyTab));
    QCOMPARE(tw->currentIndex(), 2);

    QKeyEvent keyBacktab(QKeyEvent::KeyPress, BobUI::Key_Backtab, BobUI::ControlModifier);
    QVERIFY(QApplication::sendEvent(tw, &keyBacktab));
    QCOMPARE(tw->currentIndex(), 0);
    QVERIFY(QApplication::sendEvent(tw, &keyBacktab));
    QCOMPARE(tw->currentIndex(), 2);
    QVERIFY(QApplication::sendEvent(tw, &keyBacktab));
    QCOMPARE(tw->currentIndex(), 0);
}

void tst_BOBUIabWidget::tabText()
{
    // Test bad arguments
    QCOMPARE(tw->tabText(-1), QString(""));
    tw->setTabText(-1, LABEL);

    int index = addPage();

    tw->setTabText(index, "new");
    QCOMPARE(tw->tabText(index), QString("new"));
    tw->setTabText(index, LABEL);
    QCOMPARE(tw->tabText(index), QString(LABEL));

    removePage(index);
}

void tst_BOBUIabWidget::tabShape()
{
    int index = addPage();

    tw->setTabShape(BOBUIabWidget::Rounded);
    QCOMPARE(tw->tabShape(), BOBUIabWidget::Rounded);
    tw->setTabShape(BOBUIabWidget::Triangular);
    QCOMPARE(tw->tabShape(), BOBUIabWidget::Triangular);
    tw->setTabShape(BOBUIabWidget::Rounded);
    QCOMPARE(tw->tabShape(), BOBUIabWidget::Rounded);

    removePage(index);
}

void tst_BOBUIabWidget::tabTooltip()
{
    // Test bad arguments
    QCOMPARE(tw->tabToolTip(-1), QString(""));
    tw->setTabText(-1, TIP);

    int index = addPage();

    tw->setTabToolTip(index, "tip");
    QCOMPARE(tw->tabToolTip(index), QString("tip"));
    tw->setTabToolTip(index, TIP);
    QCOMPARE(tw->tabToolTip(index), QString(TIP));

    removePage(index);
}

void tst_BOBUIabWidget::tabIcon()
{
    // Test bad arguments
    QVERIFY(tw->tabToolTip(-1).isNull());
    tw->setTabIcon(-1, QIcon());

    int index = addPage();

    QIcon icon;
    tw->setTabIcon(index, icon);
    QVERIFY(tw->tabIcon(index).isNull());

    removePage(index);
}

void tst_BOBUIabWidget::indexOf()
{
    // Test bad arguments
    QCOMPARE(tw->indexOf(NULL), -1);

    int index = addPage();
    QWidget *w = tw->widget(index);
    QCOMPARE(tw->indexOf(w), index);

    removePage(index);
}

void tst_BOBUIabWidget::currentWidget()
{
    // Test bad arguments
    tw->setCurrentWidget(NULL);
    QVERIFY(!tw->currentWidget());

    int index = addPage();
    QWidget *w = tw->widget(index);
    QCOMPARE(tw->currentWidget(), w);
    QCOMPARE(tw->currentIndex(), index);

    tw->setCurrentWidget(NULL);
    QCOMPARE(tw->currentWidget(), w);
    QCOMPARE(tw->currentIndex(), index);

    int index2 = addPage();
    QWidget *w2 = tw->widget(index2);
    Q_UNUSED(w2);
    QCOMPARE(tw->currentWidget(), w);
    QCOMPARE(tw->currentIndex(), index);

    removePage(index2);
    removePage(index);
}

/**
 * setCurrentWidget(..) calls setCurrentIndex(..)
 * currentChanged(..) SIGNAL
 */
void tst_BOBUIabWidget::currentIndex()
{
    // Test bad arguments
    QSignalSpy spy(tw, SIGNAL(currentChanged(int)));
    QCOMPARE(tw->currentIndex(), -1);
    tw->setCurrentIndex(-1);
    QCOMPARE(tw->currentIndex(), -1);
    QCOMPARE(spy.size(), 0);

    int firstIndex = addPage();
    tw->setCurrentIndex(firstIndex);
    QCOMPARE(tw->currentIndex(), firstIndex);
    QCOMPARE(spy.size(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), firstIndex);

    int index = addPage();
    QCOMPARE(tw->currentIndex(), firstIndex);
    tw->setCurrentIndex(index);
    QCOMPARE(tw->currentIndex(), index);
    QCOMPARE(spy.size(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), index);

    removePage(index);
    QCOMPARE(tw->currentIndex(), firstIndex);
    QCOMPARE(spy.size(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), firstIndex);

    removePage(firstIndex);
    QCOMPARE(tw->currentIndex(), -1);
    QCOMPARE(spy.size(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), -1);
}

void tst_BOBUIabWidget::cornerWidget()
{
    // Test bad arguments
    tw->setCornerWidget(NULL, BobUI::TopRightCorner);

    QVERIFY(!tw->cornerWidget(BobUI::TopLeftCorner));
    QVERIFY(!tw->cornerWidget(BobUI::TopRightCorner));
    QVERIFY(!tw->cornerWidget(BobUI::BottomLeftCorner));
    QVERIFY(!tw->cornerWidget(BobUI::BottomRightCorner));

    QWidget *w = new QWidget(0);
    tw->setCornerWidget(w, BobUI::TopLeftCorner);
    QCOMPARE(w->parent(), (QObject *)tw);
    QCOMPARE(tw->cornerWidget(BobUI::TopLeftCorner), w);
    tw->setCornerWidget(w, BobUI::TopRightCorner);
    QCOMPARE(tw->cornerWidget(BobUI::TopRightCorner), w);
    tw->setCornerWidget(w, BobUI::BottomLeftCorner);
    QCOMPARE(tw->cornerWidget(BobUI::BottomLeftCorner), w);
    tw->setCornerWidget(w, BobUI::BottomRightCorner);
    QCOMPARE(tw->cornerWidget(BobUI::BottomRightCorner), w);

    tw->setCornerWidget(0, BobUI::TopRightCorner);
    QVERIFY(!tw->cornerWidget(BobUI::TopRightCorner));
    QCOMPARE(w->isHidden(), true);
}

//test that the BOBUIabWidget::count() is correct at the moment the currentChanged signal is emit
class RemoveTabObject : public QObject
{
    Q_OBJECT
    public:
        RemoveTabObject(BOBUIabWidget *_tw) : tw(_tw), count(-1) {
            connect(tw, SIGNAL(currentChanged(int)), this, SLOT(currentChanged()));
        }

        BOBUIabWidget *tw;
        int count;
    public slots:
        void currentChanged() { count = tw->count(); }
};

void tst_BOBUIabWidget::removeTab()
{
    tw->show();
    QCOMPARE(tw->count(), 0);
    RemoveTabObject ob(tw);
    tw->addTab(new QLabel("1"), "1");
    QCOMPARE(ob.count, 1);
    tw->addTab(new QLabel("2"), "2");
    tw->addTab(new QLabel("3"), "3");
    tw->addTab(new QLabel("4"), "4");
    tw->addTab(new QLabel("5"), "5");
    QCOMPARE(ob.count, 1);
    QCOMPARE(tw->count(), 5);

    tw->setCurrentIndex(4);
    QCOMPARE(ob.count,5);
    tw->removeTab(4);
    QCOMPARE(ob.count, 4);
    QCOMPARE(tw->count(), 4);
    QCOMPARE(tw->currentIndex(), 3);

    tw->setCurrentIndex(1);
    tw->removeTab(1);
    QCOMPARE(ob.count, 3);
    QCOMPARE(tw->count(), 3);
    QCOMPARE(tw->currentIndex(), 1);

    delete tw->widget(1);
    QCOMPARE(tw->count(), 2);
    QCOMPARE(ob.count, 2);
    QCOMPARE(tw->currentIndex(), 1);
    delete tw->widget(1);
    QCOMPARE(tw->count(), 1);
    QCOMPARE(ob.count, 1);
    tw->removeTab(0);
    QCOMPARE(tw->count(), 0);
    QCOMPARE(ob.count, 0);
}

void tst_BOBUIabWidget::clear()
{
    tw->addTab(new QWidget, "1");
    tw->addTab(new QWidget, "2");
    tw->addTab(new QWidget, "3");
    tw->addTab(new QWidget, "4");
    tw->addTab(new QWidget, "5");
    tw->setCurrentIndex(4);
    tw->clear();
    QCOMPARE(tw->count(), 0);
    QCOMPARE(tw->currentIndex(), -1);
}

void tst_BOBUIabWidget::keyboardNavigation()
{
    int firstIndex = addPage();
    addPage();
    addPage();
    tw->setCurrentIndex(firstIndex);
    QCOMPARE(tw->currentIndex(), firstIndex);

    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier);
    QCOMPARE(tw->currentIndex(), 1);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier);
    QCOMPARE(tw->currentIndex(), 2);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier);
    QCOMPARE(tw->currentIndex(), 0);
    tw->setTabEnabled(1, false);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier);
    QCOMPARE(tw->currentIndex(), 2);

    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier | BobUI::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 0);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier | BobUI::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 2);
    tw->setTabEnabled(1, true);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier | BobUI::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 1);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier | BobUI::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 0);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier | BobUI::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 2);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier);
    QCOMPARE(tw->currentIndex(), 0);

    // Disable all and try to go to the next. It should not move anywhere, and more importantly
    // it should not loop forever. (a naive "search for the first enabled tabbar") implementation
    // might do that)
    tw->setTabEnabled(0, false);
    tw->setTabEnabled(1, false);
    tw->setTabEnabled(2, false);
    BOBUIest::keyClick(tw, BobUI::Key_Tab, BobUI::ControlModifier);
    // TODO: Disabling the current tab will move current tab to the next,
    // but what if next tab is also disabled. We should look into this.
    QVERIFY(tw->currentIndex() < 3 && tw->currentIndex() >= 0);
}

class PaintCounter : public QWidget
{
public:
    PaintCounter() :count(0) { setAttribute(BobUI::WA_OpaquePaintEvent); }
    int count;
protected:
    void paintEvent(QPaintEvent*) override
    {
        ++count;
    }
};


void tst_BOBUIabWidget::paintEventCount()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    Q_CHECK_PAINTEVENTS

    PaintCounter *tab1 = new PaintCounter;
    PaintCounter *tab2 = new PaintCounter;

    tw->addTab(tab1, "one");
    tw->addTab(tab2, "two");

    QCOMPARE(tab1->count, 0);
    QCOMPARE(tab2->count, 0);
    QCOMPARE(tw->currentIndex(), 0);

    tw->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(tw));
    // Wait for extra paint events that happen at least on macOS
    BOBUIest::qWait(1000);

    // Mac, Windows and Windows CE get multiple repaints on the first show, so use those as a starting point.
    static const int MaxInitialPaintCount =
#if defined(Q_OS_WIN)
        2;
#elif defined(Q_OS_MAC)
        5;
#else
        2;
#endif
    QVERIFY(tab1->count <= MaxInitialPaintCount);
    QCOMPARE(tab2->count, 0);

    const int initalPaintCount = tab1->count;

    tw->setCurrentIndex(1);

    BOBUIest::qWait(100);

    QCOMPARE(tab1->count, initalPaintCount);
    QCOMPARE(tab2->count, 1);

    tw->setCurrentIndex(0);

    BOBUIest::qWait(100);

    QCOMPARE(tab1->count, initalPaintCount + 1);
    QCOMPARE(tab2->count, 1);
}

void tst_BOBUIabWidget::minimumSizeHint()
{
    BOBUIabWidget tw;
    QWidget *page = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout;

    QLabel *label = new QLabel(QLatin1String("XXgypq lorem ipsum must be long, must be long. lorem ipsumMMMW"));
    lay->addWidget(label);

    page->setLayout(lay);

    tw.addTab(page, QLatin1String("page1"));

    tw.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tw));
    tw.resize(tw.minimumSizeHint());

    QSize minSize = label->minimumSizeHint();
    QSize actSize = label->geometry().size();
    QVERIFY(minSize.width() <= actSize.width());
    QVERIFY(minSize.height() <= actSize.height());
}

void tst_BOBUIabWidget::heightForWidth_data()
{
    BOBUIest::addColumn<int>("tabPosition");
    BOBUIest::newRow("West") << int(BOBUIabWidget::West);
    BOBUIest::newRow("North") << int(BOBUIabWidget::North);
    BOBUIest::newRow("East") << int(BOBUIabWidget::East);
    BOBUIest::newRow("South") << int(BOBUIabWidget::South);
}

void tst_BOBUIabWidget::heightForWidth()
{
    QFETCH(int, tabPosition);

    QWidget *window = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(window);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    BOBUIabWidget *tabWid = new BOBUIabWidget(window);
    QWidget *w = new QWidget;
    tabWid->addTab(w, QLatin1String("HFW page"));
    tabWid->setTabPosition(BOBUIabWidget::TabPosition(tabPosition));
    QVBoxLayout *lay2 = new QVBoxLayout(w);
    QLabel *label = new QLabel("Label with wordwrap turned on makes it trade height for width."
                               " Make it a really long text so that it spans on several lines"
                               " when the label is on its narrowest."
                               " I don't like to repeat myself."
                               " I don't like to repeat myself."
                               " I don't like to repeat myself."
                               " I don't like to repeat myself."
                               );
    label->setWordWrap(true);
    lay2->addWidget(label);
    lay2->setContentsMargins(0, 0, 0, 0);

    lay->addWidget(tabWid);
    int h = window->heightForWidth(160);
    window->resize(160, h);
    window->show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(window));
    QVERIFY(label->height() >= label->heightForWidth(label->width()));

    delete window;
}

void tst_BOBUIabWidget::tabBarClicked()
{
    BOBUIabWidget tabWidget;
    tabWidget.addTab(new QWidget(&tabWidget), "0");
    QSignalSpy clickSpy(&tabWidget, SIGNAL(tabBarClicked(int)));
    QSignalSpy doubleClickSpy(&tabWidget, SIGNAL(tabBarDoubleClicked(int)));

    QCOMPARE(clickSpy.size(), 0);
    QCOMPARE(doubleClickSpy.size(), 0);

    BOBUIabBar &tabBar = *tabWidget.tabBar();
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

        button = BobUI::MouseButton(button << 1);
    }
}

void tst_BOBUIabWidget::moveCurrentTab()
{
    BOBUIabWidget tabWidget;
    QWidget* firstTab = new QWidget(&tabWidget);
    QWidget* secondTab = new QWidget(&tabWidget);
    tabWidget.addTab(firstTab, "0");
    tabWidget.addTab(secondTab, "1");

    QCOMPARE(tabWidget.currentIndex(), 0);
    QCOMPARE(tabWidget.currentWidget(), firstTab);

    tabWidget.setCurrentIndex(1);

    QCOMPARE(tabWidget.currentIndex(), 1);
    QCOMPARE(tabWidget.currentWidget(), secondTab);

    tabWidget.tabBar()->moveTab(1, 0);

    QCOMPARE(tabWidget.currentIndex(), 0);
    QCOMPARE(tabWidget.currentWidget(), secondTab);
}

void tst_BOBUIabWidget::autoHide()
{
    BOBUIabWidget tabWidget;
    QWidget* firstTab = new QWidget(&tabWidget);
    tabWidget.addTab(firstTab, "0");
    const auto sizeHint1 = tabWidget.sizeHint();
    const auto minSizeHint1 = tabWidget.minimumSizeHint();
    const auto heightForWidth1 = tabWidget.heightForWidth(20);

    QWidget* secondTab = new QWidget(&tabWidget);
    tabWidget.addTab(secondTab, "1");
    const auto sizeHint2 = tabWidget.sizeHint();
    const auto minSizeHint2 = tabWidget.minimumSizeHint();
    const auto heightForWidth2 = tabWidget.heightForWidth(20);

    tabWidget.setTabBarAutoHide(true);
    // size should not change
    QCOMPARE(sizeHint2, tabWidget.sizeHint());
    QCOMPARE(minSizeHint2, tabWidget.minimumSizeHint());
    QCOMPARE(heightForWidth2, tabWidget.heightForWidth(20));

    tabWidget.removeTab(1);
    // this size should change now since the tab should be hidden
    QVERIFY(sizeHint1.height() > tabWidget.sizeHint().height());
    QVERIFY(minSizeHint1.height() > tabWidget.sizeHint().height());
    QVERIFY(heightForWidth1 > tabWidget.heightForWidth(20));
}

void tst_BOBUIabWidget::setCurrentBeforeShow_data()
{
    BOBUIest::addColumn<BOBUIabWidget::TabPosition>("tabPosition");
    BOBUIest::newRow("West") << BOBUIabWidget::West;
    BOBUIest::newRow("North") << BOBUIabWidget::North;
    BOBUIest::newRow("East") << BOBUIabWidget::East;
    BOBUIest::newRow("South") << BOBUIabWidget::South;
}

void tst_BOBUIabWidget::setCurrentBeforeShow()
{
    QFETCH(BOBUIabWidget::TabPosition, tabPosition);

    BOBUIabWidget tabWidget;
    tabWidget.setTabPosition(tabPosition);

    QPixmap pm(50, 50);
    pm.fill(BobUI::red);
    const QIcon icon(pm);
    for (int i = 0; i < 4; ++i)
        tabWidget.addTab(new QWidget, icon, QString("Tab %1").arg(i));

    // the tab widget has space for the entire tab bar
    tabWidget.resize(tabWidget.tabBar()->sizeHint() + QSize(50, 50));
    tabWidget.setCurrentIndex(2);
    tabWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tabWidget));

    QCOMPARE_GE(tabWidget.tabBar()->tabRect(0).x(), 0);
    QCOMPARE_GE(tabWidget.tabBar()->tabRect(0).y(), 0);

    BOBUIest::qWait(2000);
}

BOBUIEST_MAIN(tst_BOBUIabWidget)
#include "tst_bobuiabwidget.moc"
