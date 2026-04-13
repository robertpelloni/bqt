// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include <qapplication.h>
#include <qsplitter.h>
#include <qstyle.h>
#include <qfile.h>
#include <bobuiextstream.h>
#include <qlayout.h>
#include <qabstractscrollarea.h>
#include <qgraphicsview.h>
#include <qmdiarea.h>
#include <qscrollarea.h>
#include <bobuiextedit.h>
#include <bobuireeview.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qscreen.h>
#include <qproxystyle.h>
#include <qdebug.h> // for file error messages

BOBUI_FORWARD_DECLARE_CLASS(QSplitter)
BOBUI_FORWARD_DECLARE_CLASS(QWidget)
class tst_QSplitter : public QObject
{
    Q_OBJECT

public:
    tst_QSplitter();
    virtual ~tst_QSplitter();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
private slots:
    void getSetCheck();
    void setSizes();
    void setSizes_data();
    void saveAndRestoreState();
    void saveAndRestoreState_data();
    void saveState_data();
    void addWidget();
    void insertWidget();
    void setStretchFactor_data();
    void setStretchFactor();
    void testShowHide_data();
    void testShowHide();
    void testRemoval();
    void rubberBandNotInSplitter();
    void saveAndRestoreStateOfNotYetShownSplitter();
    void saveAndRestoreHandleWidth();
    void replaceWidget_data();
    void replaceWidget();
    void replaceWidgetWithSplitterChild_data();
    void replaceWidgetWithSplitterChild();
    void replaceWidgetWhileHidden_data();
    void replaceWidgetWhileHidden();
    void handleMinimumWidth();

    // task-specific tests below me:
    void task187373_addAbstractScrollAreas();
    void task187373_addAbstractScrollAreas_data();
    void task169702_sizes();
    void taskBOBUIBUG_4101_ensureOneNonCollapsedWidget_data();
    void taskBOBUIBUG_4101_ensureOneNonCollapsedWidget();
    void taskBOBUIBUG_102249_moveNonPressed();
    void setLayout();
    void autoAdd();

private:
    void removeThirdWidget();
    void addThirdWidget();
    QSplitter *splitter = nullptr;
    QWidget *w1;
    QWidget *w2;
    QWidget *w3;
};

// Testing get/set functions
void tst_QSplitter::getSetCheck()
{
    QSplitter obj1;
    // bool QSplitter::opaqueResize()
    // void QSplitter::setOpaqueResize(bool)
    bool styleHint = obj1.style()->styleHint(QStyle::SH_Splitter_OpaqueResize);
    QCOMPARE(styleHint, obj1.opaqueResize());
    obj1.setOpaqueResize(false);
    QCOMPARE(false, obj1.opaqueResize());
    obj1.setOpaqueResize(true);
    QCOMPARE(true, obj1.opaqueResize());
}

tst_QSplitter::tst_QSplitter()
    : w1(0), w2(0), w3(0)
{
}

tst_QSplitter::~tst_QSplitter()
{
}

void tst_QSplitter::initTestCase()
{
    splitter = new QSplitter(BobUI::Horizontal);
    w1 = new QWidget;
    w2 = new QWidget;
    splitter->addWidget(w1);
    splitter->addWidget(w2);
}

void tst_QSplitter::init()
{
    removeThirdWidget();
    w1->show();
    w2->show();
    w1->setMinimumSize(0, 0);
    w2->setMinimumSize(0, 0);
    splitter->setSizes(QList<int>() << 200 << 200);
    qApp->sendPostedEvents();
}

void tst_QSplitter::removeThirdWidget()
{
    delete w3;
    w3 = nullptr;
    int handleWidth = splitter->style()->pixelMetric(QStyle::PM_SplitterWidth);
    splitter->setFixedSize(400 + handleWidth, 400);
}

void tst_QSplitter::addThirdWidget()
{
    if (!w3) {
        w3 = new QWidget;
        splitter->addWidget(w3);
        int handleWidth = splitter->style()->pixelMetric(QStyle::PM_SplitterWidth);
        splitter->setFixedSize(600 + 2 * handleWidth, 400);
    }
}

void tst_QSplitter::cleanupTestCase()
{
    delete splitter;
}


typedef QList<int> IntList;

void tst_QSplitter::setSizes()
{
    QFETCH(IntList, minimumSizes);
    QFETCH(IntList, splitterSizes);
    QFETCH(IntList, collapsibleStates);
    QFETCH(bool, childrenCollapse);

    QCOMPARE(minimumSizes.size(), splitterSizes.size());
    if (minimumSizes.size() > 2)
        addThirdWidget();
    for (int i = 0; i < minimumSizes.size(); ++i) {
        QWidget *w = splitter->widget(i);
        w->setMinimumWidth(minimumSizes.at(i));
        splitter->setCollapsible(splitter->indexOf(w), collapsibleStates.at(i));
    }
    splitter->setChildrenCollapsible(childrenCollapse);
    splitter->setSizes(splitterSizes);
    BOBUIEST(splitter->sizes(), "expectedSizes");
}

void tst_QSplitter::setSizes_data()
{
    BOBUIest::addColumn<IntList>("minimumSizes");
    BOBUIest::addColumn<IntList>("splitterSizes");
    BOBUIest::addColumn<IntList>("expectedSizes");
    BOBUIest::addColumn<IntList>("collapsibleStates");
    BOBUIest::addColumn<bool>("childrenCollapse");

    QFile file(QFINDTESTDATA("setSizes3.dat"));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file, reason:" << file.errorString();
        return;
    }
    BOBUIextStream ts(&file);
    ts.setIntegerBase(10);

    QString dataName;
    IntList minimumSizes;
    IntList splitterSizes;
    IntList expectedSizes;
    IntList collapsibleStates;
    int childrenCollapse;
    while (!ts.atEnd()) {
        int i1, i2, i3;
        minimumSizes.clear();
        splitterSizes.clear();
        expectedSizes.clear();
        collapsibleStates.clear();
        ts >> dataName;
        ts >> i1 >> i2 >> i3;
        minimumSizes << i1 << i2 << i3;
        ts >> i1 >> i2 >> i3;
        splitterSizes << i1 << i2 << i3;
        ts >> i1 >> i2 >> i3;
        expectedSizes << i1 << i2 << i3;
        ts >> i1 >> i2 >> i3;
        collapsibleStates << i1 << i2 << i3;
        ts >> childrenCollapse;
        BOBUIest::newRow(dataName.toLocal8Bit()) << minimumSizes << splitterSizes << expectedSizes << collapsibleStates << bool(childrenCollapse);
        ts.skipWhiteSpace();
    }
}

void tst_QSplitter::saveAndRestoreState_data()
{
    saveState_data();
}

void tst_QSplitter::saveAndRestoreState()
{
    QFETCH(IntList, initialSizes);
    splitter->setSizes(initialSizes);
    QApplication::instance()->sendPostedEvents();

    QSplitter *splitter2 = new QSplitter(splitter->orientation() == BobUI::Horizontal ?
                                         BobUI::Vertical : BobUI::Horizontal);
    for (int i = 0; i < splitter->count(); ++i) {
        splitter2->addWidget(new QWidget());
    }
    splitter2->resize(splitter->size());
    splitter2->setChildrenCollapsible(!splitter->childrenCollapsible());
    splitter2->setOpaqueResize(!splitter->opaqueResize());
    splitter2->setHandleWidth(splitter->handleWidth()+3);

    QByteArray ba = splitter->saveState();
    QVERIFY(splitter2->restoreState(ba));

    QCOMPARE(splitter2->orientation(), splitter->orientation());
    QCOMPARE(splitter2->handleWidth(), splitter->handleWidth());
    QCOMPARE(splitter2->opaqueResize(), splitter->opaqueResize());
    QCOMPARE(splitter2->childrenCollapsible(), splitter->childrenCollapsible());

    QList<int> l1 = splitter->sizes();
    QList<int> l2 = splitter2->sizes();
    QCOMPARE(l1.size(), l2.size());
    for (int i = 0; i < splitter->sizes().size(); ++i) {
        QCOMPARE(l2.at(i), l1.at(i));
    }

    // destroy version and magic number
    for (int i = 0; i < ba.size(); ++i)
        ba[i] = ~ba.at(i);
    QVERIFY(!splitter2->restoreState(ba));

    delete splitter2;
}

void tst_QSplitter::saveAndRestoreStateOfNotYetShownSplitter()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QSplitter *spl = new QSplitter;
    QLabel *l1 = new QLabel;
    QLabel *l2 = new QLabel;
    spl->addWidget(l1);
    spl->addWidget(l2);

    QByteArray ba = spl->saveState();
    spl->restoreState(ba);
    spl->show();
    QVERIFY(BOBUIest::qWaitForWindowActive(spl));

    QCOMPARE(l1->geometry().isValid(), true);
    QCOMPARE(l2->geometry().isValid(), true);

    delete spl;
}

class TestSplitterStyle : public QProxyStyle
{
public:
    TestSplitterStyle() : handleWidth(5) {}
    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override
    {
        if (metric == QStyle::PM_SplitterWidth)
            return handleWidth;
        else
            return QProxyStyle::pixelMetric(metric, option, widget);
    }
    int handleWidth;
};

void tst_QSplitter::saveAndRestoreHandleWidth()
{
    TestSplitterStyle style;
    style.handleWidth = 5;
    QSplitter spl;
    spl.setStyle(&style);

    QCOMPARE(spl.handleWidth(), style.handleWidth);
    style.handleWidth = 10;
    QCOMPARE(spl.handleWidth(), style.handleWidth);
    QByteArray ba = spl.saveState();
    spl.setHandleWidth(20);
    QCOMPARE(spl.handleWidth(), 20);
    spl.setHandleWidth(-1);
    QCOMPARE(spl.handleWidth(), style.handleWidth);
    spl.setHandleWidth(15);
    QCOMPARE(spl.handleWidth(), 15);
    spl.restoreState(ba);
    QCOMPARE(spl.handleWidth(), style.handleWidth);
}

void tst_QSplitter::saveState_data()
{
    BOBUIest::addColumn<IntList>("initialSizes");
    BOBUIest::addColumn<bool>("hideWidget1");
    BOBUIest::addColumn<bool>("hideWidget2");
    BOBUIest::addColumn<QByteArray>("finalBa");

    BOBUIest::newRow("ok0") << (IntList() << 200 << 200) << bool(false) << bool(false) << QByteArray("[200,200]");
    BOBUIest::newRow("ok1") << (IntList() << 300 << 100) << bool(false) << bool(false) << QByteArray("[300,100]");
    BOBUIest::newRow("ok2") << (IntList() << 100 << 300) << bool(false) << bool(false) << QByteArray("[100,300]");
    BOBUIest::newRow("ok3") << (IntList() << 200 << 200) << bool(false) << bool(true) << QByteArray("[200,H]");
    BOBUIest::newRow("ok4") << (IntList() << 200 << 200) << bool(true) << bool(false) << QByteArray("[H,200]");
    BOBUIest::newRow("ok5") << (IntList() << 200 << 200) << bool(false) << bool(false) << QByteArray("[200,200]");
    BOBUIest::newRow("ok6") << (IntList() << 200 << 200) << bool(false) << bool(false) << QByteArray("[200,200]");
    BOBUIest::newRow("ok7") << (IntList() << 200 << 200) << bool(false) << bool(false) << QByteArray("[200,200]");
    BOBUIest::newRow("ok8") << (IntList() << 200 << 200) << bool(true) << bool(true) << QByteArray("[H,H]");
}

void tst_QSplitter::addWidget()
{
    QSplitter split;

    // Simple case
    QWidget *widget1 = new QWidget;
    QWidget *widget2 = new QWidget;
    split.addWidget(widget1);
    split.addWidget(widget2);
    QCOMPARE(split.count(), 2);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.indexOf(widget2), 1);
    QCOMPARE(split.widget(0), widget1);
    QCOMPARE(split.widget(1), widget2);


    // Implicit Add
    QWidget *widget3 = new QWidget(&split);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget3), 2);
    QCOMPARE(split.widget(2), widget3);

    // Try and add it again
    split.addWidget(widget3);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget3), 2);
    QCOMPARE(split.widget(2), widget3);

    // Add a widget that is already in the splitter
    split.addWidget(widget1);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget1), 2);
    QCOMPARE(split.widget(0), widget2);
    QCOMPARE(split.widget(1), widget3);
    QCOMPARE(split.widget(2), widget1);

    // Change a widget's parent
    widget2->setParent(0);
    QCOMPARE(split.count(), 2);
    QCOMPARE(split.indexOf(widget2), -1);


    // Add the widget in again.
    split.addWidget(widget2);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget2), 2);
    QCOMPARE(split.widget(0), widget3);
    QCOMPARE(split.widget(1), widget1);
    QCOMPARE(split.widget(2), widget2);

    // Delete a widget
    delete widget1;
    QCOMPARE(split.count(), 2);
    QCOMPARE(split.indexOf(widget1), -1); // Nasty
    QCOMPARE(split.widget(0), widget3);
    QCOMPARE(split.widget(1), widget2);

    delete widget2;
}

void tst_QSplitter::insertWidget()
{
    QSplitter split;
    QWidget *widget1 = new QWidget;
    QWidget *widget2 = new QWidget;
    QWidget *widget3 = new QWidget;

    split.insertWidget(0, widget1);
    QCOMPARE(split.count(), 1);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.widget(0), widget1);

    split.insertWidget(0, widget2);
    QCOMPARE(split.count(), 2);
    QCOMPARE(split.indexOf(widget1), 1);
    QCOMPARE(split.indexOf(widget2), 0);
    QCOMPARE(split.widget(0), widget2);
    QCOMPARE(split.widget(1), widget1);

    split.insertWidget(1, widget3);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget1), 2);
    QCOMPARE(split.indexOf(widget2), 0);
    QCOMPARE(split.indexOf(widget3), 1);
    QCOMPARE(split.widget(0), widget2);
    QCOMPARE(split.widget(1), widget3);
    QCOMPARE(split.widget(2), widget1);

    delete widget3;
    QCOMPARE(split.count(), 2);
    QCOMPARE(split.indexOf(widget1), 1);
    QCOMPARE(split.indexOf(widget2), 0);
    QCOMPARE(split.widget(0), widget2);
    QCOMPARE(split.widget(1), widget1);

    widget3 = new QWidget;
    split.insertWidget(split.count() + 1, widget3);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget1), 1);
    QCOMPARE(split.indexOf(widget2), 0);
    QCOMPARE(split.indexOf(widget3), 2);
    QCOMPARE(split.widget(0), widget2);
    QCOMPARE(split.widget(1), widget1);
    QCOMPARE(split.widget(2), widget3);


    // Try it again,
    split.insertWidget(split.count() + 1, widget3);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget1), 1);
    QCOMPARE(split.indexOf(widget2), 0);
    QCOMPARE(split.indexOf(widget3), 2);
    QCOMPARE(split.widget(0), widget2);
    QCOMPARE(split.widget(1), widget1);
    QCOMPARE(split.widget(2), widget3);

    // Try to move widget2 to a bad place
    split.insertWidget(-1, widget2);
    QCOMPARE(split.count(), 3);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.indexOf(widget2), 2);
    QCOMPARE(split.indexOf(widget3), 1);
    QCOMPARE(split.widget(0), widget1);
    QCOMPARE(split.widget(1), widget3);
    QCOMPARE(split.widget(2), widget2);

    QWidget *widget4 = new QWidget(&split);
    QCOMPARE(split.count(), 4);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.indexOf(widget2), 2);
    QCOMPARE(split.indexOf(widget3), 1);
    QCOMPARE(split.indexOf(widget4), 3);
    QCOMPARE(split.widget(0), widget1);
    QCOMPARE(split.widget(1), widget3);
    QCOMPARE(split.widget(2), widget2);
    QCOMPARE(split.widget(3), widget4);

    QWidget *widget5 = new QWidget(&split);
    QCOMPARE(split.count(), 5);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.indexOf(widget2), 2);
    QCOMPARE(split.indexOf(widget3), 1);
    QCOMPARE(split.indexOf(widget4), 3);
    QCOMPARE(split.indexOf(widget5), 4);
    QCOMPARE(split.widget(0), widget1);
    QCOMPARE(split.widget(1), widget3);
    QCOMPARE(split.widget(2), widget2);
    QCOMPARE(split.widget(3), widget4);
    QCOMPARE(split.widget(4), widget5);

    split.insertWidget(2, widget4);
    QCOMPARE(split.count(), 5);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.indexOf(widget2), 3);
    QCOMPARE(split.indexOf(widget3), 1);
    QCOMPARE(split.indexOf(widget4), 2);
    QCOMPARE(split.indexOf(widget5), 4);
    QCOMPARE(split.widget(0), widget1);
    QCOMPARE(split.widget(1), widget3);
    QCOMPARE(split.widget(2), widget4);
    QCOMPARE(split.widget(3), widget2);
    QCOMPARE(split.widget(4), widget5);

    split.insertWidget(1, widget5);
    QCOMPARE(split.count(), 5);
    QCOMPARE(split.indexOf(widget1), 0);
    QCOMPARE(split.indexOf(widget2), 4);
    QCOMPARE(split.indexOf(widget3), 2);
    QCOMPARE(split.indexOf(widget4), 3);
    QCOMPARE(split.indexOf(widget5), 1);
    QCOMPARE(split.widget(0), widget1);
    QCOMPARE(split.widget(1), widget5);
    QCOMPARE(split.widget(2), widget3);
    QCOMPARE(split.widget(3), widget4);
    QCOMPARE(split.widget(4), widget2);
}

void tst_QSplitter::setStretchFactor_data()
{
    BOBUIest::addColumn<int>("orientation");
    BOBUIest::addColumn<int>("widgetIndex");
    BOBUIest::addColumn<int>("stretchFactor");
    BOBUIest::addColumn<int>("expectedHStretch");
    BOBUIest::addColumn<int>("expectedVStretch");

    BOBUIest::newRow("ok01") << int(BobUI::Horizontal) << 1 << 2 << 2 << 2;
    BOBUIest::newRow("ok02") << int(BobUI::Horizontal) << 2 << 0 << 0 << 0;
    BOBUIest::newRow("ok03") << int(BobUI::Horizontal) << 3 << 1 << 1 << 1;
    BOBUIest::newRow("ok04") << int(BobUI::Horizontal) << 0 << 7 << 7 << 7;
    BOBUIest::newRow("ok05") << int(BobUI::Vertical) << 0 << 0 << 0 << 0;
    BOBUIest::newRow("ok06") << int(BobUI::Vertical) << 1 << 1 << 1 << 1;
    BOBUIest::newRow("ok07") << int(BobUI::Vertical) << 2 << 2 << 2 << 2;
    BOBUIest::newRow("ok08") << int(BobUI::Vertical) << 3 << 5 << 5 << 5;
    BOBUIest::newRow("ok09") << int(BobUI::Vertical) << -1 << 5 << 0 << 0;
}

void tst_QSplitter::setStretchFactor()
{
    QFETCH(int, orientation);
    BobUI::Orientation orient = BobUI::Orientation(orientation);
    QSplitter split(orient);
    QWidget *w = new QWidget;
    split.addWidget(w);
    w = new QWidget;
    split.addWidget(w);
    w = new QWidget;
    split.addWidget(w);
    w = new QWidget;
    split.addWidget(w);

    QFETCH(int, widgetIndex);
    QFETCH(int, stretchFactor);
    w = split.widget(widgetIndex);
    QSizePolicy sp;
    if (w) {
        QCOMPARE(sp.horizontalStretch(), 0);
        QCOMPARE(sp.verticalStretch(), 0);
    }
    split.setStretchFactor(widgetIndex, stretchFactor);
    if (w)
        sp = w->sizePolicy();
    BOBUIEST(sp.horizontalStretch(), "expectedHStretch");
    BOBUIEST(sp.verticalStretch(), "expectedVStretch");
}

void tst_QSplitter::testShowHide_data()
{
    BOBUIest::addColumn<bool>("hideWidget1");
    BOBUIest::addColumn<bool>("hideWidget2");
    BOBUIest::addColumn<QList<int> >("finalValues");
    BOBUIest::addColumn<bool>("handleVisible");

    QSplitter *split = new QSplitter(BobUI::Horizontal);
    BOBUIest::newRow("hideNone") << false << false << (QList<int>() << 200 << 200) << true;
    BOBUIest::newRow("hide2") << false << true << (QList<int>() << 400 + split->handleWidth() << 0) << false;
    BOBUIest::newRow("hide1") << true << false << (QList<int>() << 0 << 400 + split->handleWidth()) << false;
    BOBUIest::newRow("hideall") << true << true << (QList<int>() << 0 << 0) << false;
    delete split;
}

void tst_QSplitter::testShowHide()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(bool, hideWidget1);
    QFETCH(bool, hideWidget2);

    QSplitter *split = new QSplitter(BobUI::Horizontal);

    QWidget topLevel;
    QWidget widget(&topLevel);
    widget.resize(400 + split->handleWidth(), 200);
    QVBoxLayout *lay=new QVBoxLayout(&widget);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    split->addWidget(new QWidget);
    split->addWidget(new QWidget);
    split->setSizes(QList<int>() << 200 << 200);
    lay->addWidget(split);
    widget.setLayout(lay);
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&topLevel));

    widget.hide();
    split->widget(0)->setHidden(hideWidget1);
    split->widget(1)->setHidden(hideWidget2);
    widget.show();
    BOBUIest::qWait(100);

    BOBUIEST(split->sizes(), "finalValues");
    BOBUIEST(split->handle(1)->isVisible(), "handleVisible");
}

void tst_QSplitter::testRemoval()
{

    // This test relies on the internal structure of QSplitter That is, that
    // there is a handle before every splitter, but sometimes that handle is
    // hidden. But definiately when something is removed the front handle
    // should not be visible.

    QSplitter split;
    split.addWidget(new QWidget);
    split.addWidget(new QWidget);
    split.show();
    BOBUIest::qWait(100);

    QCOMPARE(split.handle(0)->isVisible(), false);
    QSplitterHandle *handle = split.handle(1);
    QCOMPARE(handle->isVisible(), true);

    delete split.widget(0);
    QSplitterHandle *sameHandle = split.handle(0);
    QCOMPARE(handle, sameHandle);
    QCOMPARE(sameHandle->isVisible(), false);
}

class MyFriendlySplitter : public QSplitter
{
public:
    MyFriendlySplitter(QWidget *parent = nullptr) : QSplitter(parent) {}
    void setRubberBand(int pos) { QSplitter::setRubberBand(pos); }

    void moveSplitter(int pos, int index) { QSplitter::moveSplitter(pos, index); }

    friend class tst_QSplitter;
};

class EventCounterSpy : public QObject
{
public:
    EventCounterSpy(QWidget *obj) : objectToWatch(obj)
    { }

    ~EventCounterSpy()
    {
        removeEventFilter();
    }

    void installEventFilter()
    {
        if (needRemoveEventFilter)
            return;
        needRemoveEventFilter = true;
        qApp->installEventFilter(this);
    }

    void removeEventFilter()
    {
        if (!needRemoveEventFilter)
            return;
        needRemoveEventFilter = false;
        qApp->removeEventFilter(this);
    }

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        // Watch for events in the parent widget and all its children
        if (watched == objectToWatch || watched->parent() == objectToWatch) {
            if (event->type() == QEvent::Resize)
                resizeCount++;
            else if (event->type() == QEvent::Paint)
                paintCount++;
        }

        return QObject::eventFilter(watched, event);
    }

    int resizeCount = 0;
    int paintCount = 0;
    bool needRemoveEventFilter = false;
    QObject *objectToWatch;
};

void tst_QSplitter::replaceWidget_data()
{
    BOBUIest::addColumn<int>("index");
    BOBUIest::addColumn<bool>("visible");
    BOBUIest::addColumn<bool>("collapsed");

    BOBUIest::newRow("negative index") << -1 << true << false;
    BOBUIest::newRow("index too large") << 80 << true << false;
    BOBUIest::newRow("visible, not collapsed") << 3 << true << false;
    BOBUIest::newRow("visible, collapsed") << 3 << true << true;
    BOBUIest::newRow("not visible, not collapsed") << 3 << false << false;
    BOBUIest::newRow("not visible, collapsed") << 3 << false << true;
}

void tst_QSplitter::replaceWidget()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(int, index);
    QFETCH(bool, visible);
    QFETCH(bool, collapsed);

    // Setup
    MyFriendlySplitter sp;
    const int count = 7;
    for (int i = 0; i < count; i++) {
        // We use labels instead of plain widgets to
        // make it easier to fix eventual regressions.
        QLabel *w = new QLabel(QString::asprintf("WIDGET #%d", i));
        sp.addWidget(w);
    }
    sp.setWindowTitle(QString::asprintf("index %d, visible %d, collapsed %d", index, visible, collapsed));
    sp.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&sp));

    // Configure splitter
    QWidget *oldWidget = sp.widget(index);
    BOBUIest::qWait(100); // Make sure we record the right original size (after the window manager adds the frame)
    const QRect oldGeom = oldWidget ? oldWidget->geometry() : QRect();
    if (oldWidget) {
        // Collapse first, then hide, if necessary
        if (collapsed) {
            sp.setCollapsible(index, true);
            sp.moveSplitter(oldWidget->x() + 1, index + 1);
        }
        if (!visible)
            oldWidget->hide();
    }

    // Replace widget
    BOBUIest::qWait(100); // Flush event queue
    const QList<int> sizes = sp.sizes();
    // Shorter label: The important thing is to ensure we can set
    // the same size on the new widget. Because of QLabel's sizing
    // constraints (they can expand but not shrink) the easiest is
    // to set a shorter label.
    QLabel *newWidget = new QLabel(QLatin1String("NEW"));

    EventCounterSpy ef(&sp);
    ef.installEventFilter();
    const QWidget *res = sp.replaceWidget(index, newWidget);
    std::unique_ptr<const QWidget> reaper{res};
    BOBUIest::qWait(100); // Give visibility and resizing some time

    // Check
    if (index < 0 || index >= count) {
        // check before reset() may delete *res: C++/Coverity are picky with zapped pointers
        const bool resWasNull = !res;
        reaper.reset(newWidget); // newWidget wasn't added, so we need to delete
        QVERIFY(resWasNull);
        QVERIFY(!newWidget->parentWidget());
        QCOMPARE(ef.resizeCount, 0);
        QCOMPARE(ef.paintCount, 0);
    } else {
        QCOMPARE(res, oldWidget);
        QVERIFY(!res->parentWidget());
        QVERIFY(!res->isVisible());
        const int expectedResizeCount = visible ? 1 : 0; // new widget only
        const int expectedPaintCount = visible && !collapsed ? 2 : 0; // splitter and new widget
        BOBUIRY_COMPARE(ef.resizeCount, expectedResizeCount);
        BOBUIRY_COMPARE(ef.paintCount, expectedPaintCount);
        QCOMPARE(newWidget->parentWidget(), &sp);
        QCOMPARE(newWidget->isVisible(), visible);
        if (visible && !collapsed)
            QCOMPARE(newWidget->geometry(), oldGeom);
        QCOMPARE(newWidget->size().isEmpty(), !visible || collapsed);
    }
    QCOMPARE(sp.count(), count);
    QCOMPARE(sp.sizes(), sizes);
}

void tst_QSplitter::replaceWidgetWithSplitterChild_data()
{
    BOBUIest::addColumn<int>("srcIndex");
    BOBUIest::addColumn<int>("dstIndex");

    BOBUIest::newRow("replace with null widget") << -2 << 3;
    BOBUIest::newRow("replace with itself") << 3 << 3;
    BOBUIest::newRow("replace with sibling, after recalc") << 1 << 4;
    BOBUIest::newRow("replace with sibling, before recalc") << -1 << 4;
}

void tst_QSplitter::replaceWidgetWithSplitterChild()
{
    QFETCH(int, srcIndex);
    QFETCH(int, dstIndex);

    // Setup
    MyFriendlySplitter sp;
    const int count = 7;
    for (int i = 0; i < count; i++) {
        // We use labels instead of plain widgets to
        // make it easier to fix eventual regressions.
        QLabel *w = new QLabel(QString::asprintf("WIDGET #%d", i));
        sp.addWidget(w);
    }
    sp.setWindowTitle(QLatin1String(BOBUIest::currentTestFunction()) + QLatin1Char(' ') + QLatin1String(BOBUIest::currentDataTag()));
    sp.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&sp));

    BOBUIest::qWait(100); // Flush event queue before new widget creation
    const QList<int> sizes = sp.sizes();
    QWidget *sibling = srcIndex == -1 ? (new QLabel("<b>NEW</b>", &sp)) : sp.widget(srcIndex);

    EventCounterSpy ef(&sp);
    ef.installEventFilter();
    const QWidget *res = sp.replaceWidget(dstIndex, sibling);
    BOBUIest::qWait(100); // Give visibility and resizing some time

    QVERIFY(!res);
    if (srcIndex == -1) {
        // Create and replace before recalc. The sibling is scheduled to be
        // added after replaceWidget(), when QSplitter receives a child event.
        BOBUIRY_VERIFY(ef.resizeCount > 0);
        BOBUIRY_VERIFY(ef.paintCount > 0);
        QCOMPARE(sp.count(), count + 1);
        QCOMPARE(sp.sizes().mid(0, count), sizes);
        QCOMPARE(sp.sizes().last(), sibling->width());
    } else {
        // No-op for the rest
        QCOMPARE(ef.resizeCount, 0);
        QCOMPARE(ef.paintCount, 0);
        QCOMPARE(sp.count(), count);
        QCOMPARE(sp.sizes(), sizes);
    }
}

void tst_QSplitter::replaceWidgetWhileHidden_data()
{
    BOBUIest::addColumn<bool>("splitterVisible");
    BOBUIest::addColumn<bool>("widgetVisible");

    BOBUIest::addRow("visibleToVisible") << true << true;
    BOBUIest::addRow("hiddenToVisible") << true << false;
    BOBUIest::addRow("visibleToHidden") << false << true;
    BOBUIest::addRow("hiddenToHidden") << false << false;
}

void tst_QSplitter::replaceWidgetWhileHidden()
{
    QFETCH(bool, splitterVisible);
    QFETCH(bool, widgetVisible);

    MyFriendlySplitter splitter;

    splitter.addWidget(new QLabel("One"));
    splitter.addWidget(new QLabel("Two"));

    if (splitterVisible) {
        splitter.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&splitter));
    }
    QWidget *newWidget = new QLabel("Three");
    if (!widgetVisible)
        newWidget->hide();

    const bool wasExplicitHide = !widgetVisible && newWidget->testAttribute(BobUI::WA_WState_ExplicitShowHide);
    const std::unique_ptr<QWidget> reaper{splitter.replaceWidget(1, newWidget)};

    QCOMPARE(!widgetVisible && newWidget->testAttribute(BobUI::WA_WState_ExplicitShowHide), wasExplicitHide);

    if (!splitterVisible) {
        splitter.show();
        QVERIFY(BOBUIest::qWaitForWindowExposed(&splitter));
    }
    QCOMPARE(widgetVisible, newWidget->isVisible());
}

void tst_QSplitter::handleMinimumWidth()
{
    MyFriendlySplitter split;
    split.addWidget(new QLabel("Number Wan"));
    split.addWidget(new QLabel("Number Too"));

    split.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&split));
    for (int i = 0; i < 10; i++) {
        split.setHandleWidth(i);
        BOBUIest::qWait(100); // resizing
        QCOMPARE(split.handle(1)->width(), qMax(4 + (i & 1), i));
    }

    split.setOrientation(BobUI::Vertical);
    BOBUIest::qWait(100);
    for (int i = 0; i < 10; i++) {
        split.setHandleWidth(i);
        BOBUIest::qWait(100); // resizing
        QCOMPARE(split.handle(1)->height(), qMax(4 + (i & 1), i));
    }
}

void tst_QSplitter::rubberBandNotInSplitter()
{
    MyFriendlySplitter split;
    split.addWidget(new QWidget);
    split.addWidget(new QWidget);
    split.setOpaqueResize(false);
    QCOMPARE(split.count(), 2);
    split.setRubberBand(2);
    QCOMPARE(split.count(), 2);
}

void tst_QSplitter::task187373_addAbstractScrollAreas_data()
{
    BOBUIest::addColumn<QByteArray>("className");
    BOBUIest::addColumn<bool>("addInConstructor");
    BOBUIest::addColumn<bool>("addOutsideConstructor");

    QList<QByteArray> classNames{
        "QGraphicsView",
        "QMdiArea",
        "QScrollArea",
        "BOBUIextEdit",
        "BOBUIreeView"
    };

    for (const auto &className : std::as_const(classNames)) {
        BOBUIest::newRow(qPrintable(className + " 1")) << className << false << true;
        BOBUIest::newRow(qPrintable(className + " 2")) << className << true << false;
        BOBUIest::newRow(qPrintable(className + " 3")) << className << true << true;
    }
}

static QAbstractScrollArea *task187373_createScrollArea(
    QSplitter *splitter, const QByteArray &className, bool addInConstructor)
{
    if (className == "QGraphicsView")
        return new QGraphicsView(addInConstructor ? splitter : 0);
    if (className == "QMdiArea")
        return new QMdiArea(addInConstructor ? splitter : 0);
    if (className == "QScrollArea")
        return new QScrollArea(addInConstructor ? splitter : 0);
    if (className == "BOBUIextEdit")
        return new BOBUIextEdit(addInConstructor ? splitter : 0);
    if (className == "BOBUIreeView")
        return new BOBUIreeView(addInConstructor ? splitter : 0);
    return 0;
}

void tst_QSplitter::task187373_addAbstractScrollAreas()
{
    QFETCH(QByteArray, className);
    QFETCH(bool, addInConstructor);
    QFETCH(bool, addOutsideConstructor);
    QVERIFY(addInConstructor || addOutsideConstructor);

    QSplitter splitter;
    splitter.show();
    QVERIFY(splitter.isVisible());

    QAbstractScrollArea *w = task187373_createScrollArea(&splitter, className, addInConstructor);
    QVERIFY(w);
    if (addOutsideConstructor)
        splitter.addWidget(w);

    BOBUIRY_VERIFY(w->isVisible());
    QVERIFY(!w->isHidden());
    QVERIFY(w->viewport()->isVisible());
    QVERIFY(!w->viewport()->isHidden());
}

//! A simple BOBUIextEdit which can switch between two different size states
class MyTextEdit : public BOBUIextEdit
{
    public:
        MyTextEdit(const QString & text, QWidget* parent = NULL)
            : BOBUIextEdit(text, parent) ,  m_iFactor(1)
        {
            setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        }
        virtual QSize minimumSizeHint () const override
        {
            return QSize(200, 200) * m_iFactor;
        }
        virtual QSize sizeHint() const override
        {
            return QSize(390, 390) * m_iFactor;
        }
        int m_iFactor;
};

void tst_QSplitter::task169702_sizes()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget topLevel;
    // Create two nested (non-collapsible) splitters
    QSplitter* outerSplitter = new QSplitter(BobUI::Vertical, &topLevel);
    outerSplitter->setChildrenCollapsible(false);
    QSplitter* splitter = new QSplitter(BobUI::Horizontal, outerSplitter);
    splitter->setChildrenCollapsible(false);

    // populate the outer splitter
    outerSplitter->addWidget(new BOBUIextEdit("Foo"));
    outerSplitter->addWidget(splitter);
    outerSplitter->setStretchFactor(0, 1);
    outerSplitter->setStretchFactor(1, 0);

    // populate the inner splitter
    MyTextEdit* testW = new MyTextEdit("TextEdit with size restriction");
    splitter->addWidget(testW);
    splitter->addWidget(new BOBUIextEdit("Bar"));

    outerSplitter->setGeometry(100, 100, 500, 500);
    topLevel.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&topLevel));

    testW->m_iFactor++;
    testW->updateGeometry();

    //Make sure the minimimSizeHint is respected
    BOBUIRY_COMPARE(testW->size().height(), testW->minimumSizeHint().height());
}

void tst_QSplitter::taskBOBUIBUG_4101_ensureOneNonCollapsedWidget_data()
{
    BOBUIest::addColumn<bool>("testingHide");

    BOBUIest::newRow("last non collapsed hidden") << true;
    BOBUIest::newRow("last non collapsed deleted") << false;
}

void tst_QSplitter::taskBOBUIBUG_4101_ensureOneNonCollapsedWidget()
{
    QFETCH(bool, testingHide);

    MyFriendlySplitter s;
    QLabel *l = nullptr;
    for (int i = 0; i < 5; ++i) {
        l = new QLabel(QString("Label ") + QChar('A' + i));
        l->setAlignment(BobUI::AlignCenter);
        s.addWidget(l);
        s.moveSplitter(0, i);  // Collapse all the labels except the last one.
    }

    s.show();
    if (testingHide)
        l->hide();
    else
        delete l;
    BOBUIRY_VERIFY(s.sizes().at(0) > 0);
}

void tst_QSplitter::taskBOBUIBUG_102249_moveNonPressed()
{
    QSplitter s;
    s.setOpaqueResize(true);
    s.addWidget(new QWidget());
    s.addWidget(new QWidget());
    s.show();

    QSignalSpy spyMove(&s, &QSplitter::splitterMoved);
    QPointF posOutOfWidget = QPointF(30, 30);
    QMouseEvent me(QEvent::MouseMove,
                   posOutOfWidget, s.mapToGlobal(posOutOfWidget),
                   BobUI::NoButton, BobUI::MouseButtons(BobUI::LeftButton),
                   BobUI::NoModifier);
    qApp->sendEvent(s.handle(0), &me);
    QCOMPARE(spyMove.size(), 0);
}

void tst_QSplitter::setLayout()
{
    QSplitter splitter;
    QVBoxLayout layout;
    BOBUIest::ignoreMessage(BobUIWarningMsg, "Adding a QLayout to a QSplitter is not supported.");
    splitter.setLayout(&layout);
    // It will work, but we don't recommend it...
    QCOMPARE(splitter.layout(), &layout);
}

void tst_QSplitter::autoAdd()
{
    QSplitter splitter;
    splitter.setWindowTitle("autoAdd");
    splitter.setMinimumSize(QSize(200, 200));
    splitter.move(QGuiApplication::primaryScreen()->availableGeometry().center() - QPoint(100, 100));
    splitter.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&splitter));
    // Constructing a child widget on the splitter should
    // automatically add and show it.
    QWidget *childWidget = new QWidget(&splitter);
    QCOMPARE(splitter.count(), 1);
    BOBUIRY_VERIFY(childWidget->isVisible());
    // Deleting should automatically remove it
    delete childWidget;
    QCOMPARE(splitter.count(), 0);
    // BOBUIBUG-40132, top level windows should not be affected by this.
    QDialog *dialog = new QDialog(&splitter);
    QCOMPARE(splitter.count(), 0);
    QCoreApplication::processEvents();
    QVERIFY(!dialog->isVisible());
}

BOBUIEST_MAIN(tst_QSplitter)
#include "tst_qsplitter.moc"
