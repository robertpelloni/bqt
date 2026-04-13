// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include <qabstractbutton.h>
#include <qaction.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qpixmap.h>
#include <qstyle.h>
#include <bobuioolbar.h>
#include <qwidgetaction.h>
#include <bobuioolbutton.h>
#include <qlineedit.h>
#if BOBUI_CONFIG(shortcut)
#  include <qkeysequence.h>
#endif
#include <qmenu.h>
#include <qlabel.h>
#include <private/bobuioolbarextension_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

BOBUI_FORWARD_DECLARE_CLASS(QAction)

class tst_BOBUIoolBar : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIoolBar();

public slots:
    void slot();
    void slot(QAction *action);

private slots:
    void isMovable();
    void allowedAreas();
    void orientation();
    void addAction();
    void addActionConnect();
    void insertAction();
    void addSeparator();
    void insertSeparator();
    void addWidget();
    void insertWidget();
    void actionGeometry();
    void toggleViewAction();
    void iconSize();
    void toolButtonStyle();
    void actionTriggered();
    void visibilityChanged();
    void actionOwnership();
    void widgetAction();
    void accel();

    void task191727_layout();
    void task197996_visibility();

    void extraCpuConsumption(); // BOBUIBUG-54676
    void iconSizeStyleSheet();
};


QAction *triggered = 0;

tst_BOBUIoolBar::tst_BOBUIoolBar()
{
    qRegisterMetaType<BobUI::Orientation>("BobUI::Orientation");
    qRegisterMetaType<BobUI::ToolBarAreas>("BobUI::ToolBarAreas");
    qRegisterMetaType<BobUI::ToolButtonStyle>("BobUI::ToolButtonStyle");
}

void tst_BOBUIoolBar::slot()
{ }

void tst_BOBUIoolBar::slot(QAction *action)
{ ::triggered = action; }

void tst_BOBUIoolBar::isMovable()
{
#define DO_TEST                                                 \
    do {                                                        \
        QVERIFY(tb.isMovable());                                 \
        tb.setMovable(false);                                   \
        QVERIFY(!tb.isMovable());                                \
        QCOMPARE(spy.count(), 1);                                \
        QCOMPARE(spy.at(0).value(0).toBool(), tb.isMovable());   \
        spy.clear();                                            \
        tb.setMovable(tb.isMovable());                          \
        QCOMPARE(spy.count(), 0);                                \
        spy.clear();                                            \
        tb.setMovable(true);                                    \
        QVERIFY(tb.isMovable());                                 \
        QCOMPARE(spy.count(), 1);                                \
        QCOMPARE(spy.at(0).value(0).toBool(), tb.isMovable());   \
        spy.clear();                                            \
        tb.setMovable(tb.isMovable());                          \
        QCOMPARE(spy.count(), 0);                                \
        spy.clear();                                            \
    } while (false)

    QMainWindow mw;
    BOBUIoolBar tb;

    QCOMPARE(tb.isMovable(), (bool)qApp->style()->styleHint(QStyle::SH_ToolBar_Movable));
    if (!tb.isMovable())
        tb.setMovable(true);

    QSignalSpy spy(&tb, SIGNAL(movableChanged(bool)));

    DO_TEST;
    mw.addToolBar(&tb);
    DO_TEST;
    mw.removeToolBar(&tb);
    DO_TEST;
}

void tst_BOBUIoolBar::allowedAreas()
{
    BOBUIoolBar tb;

    QSignalSpy spy(&tb, SIGNAL(allowedAreasChanged(BobUI::ToolBarAreas)));

    // default
    QCOMPARE((int)tb.allowedAreas(), (int)BobUI::AllToolBarAreas);
    QVERIFY(tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::BottomToolBarArea));

    // a single dock window area
    tb.setAllowedAreas(BobUI::LeftToolBarArea);
    QCOMPARE((int)tb.allowedAreas(), (int)BobUI::LeftToolBarArea);
    QVERIFY(tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    tb.setAllowedAreas(BobUI::RightToolBarArea);
    QCOMPARE((int)tb.allowedAreas(), (int)BobUI::RightToolBarArea);
    QVERIFY(!tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    tb.setAllowedAreas(BobUI::TopToolBarArea);
    QCOMPARE((int)tb.allowedAreas(), (int)BobUI::TopToolBarArea);
    QVERIFY(!tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    tb.setAllowedAreas(BobUI::BottomToolBarArea);
    QCOMPARE((int)tb.allowedAreas(), (int)BobUI::BottomToolBarArea);
    QVERIFY(!tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    // multiple dock window areas
    tb.setAllowedAreas(BobUI::TopToolBarArea | BobUI::BottomToolBarArea);
    QCOMPARE(tb.allowedAreas(), BobUI::TopToolBarArea | BobUI::BottomToolBarArea);
    QVERIFY(!tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    tb.setAllowedAreas(BobUI::LeftToolBarArea | BobUI::RightToolBarArea);
    QCOMPARE(tb.allowedAreas(), BobUI::LeftToolBarArea | BobUI::RightToolBarArea);
    QVERIFY(tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    tb.setAllowedAreas(BobUI::TopToolBarArea | BobUI::LeftToolBarArea);
    QCOMPARE(tb.allowedAreas(), BobUI::TopToolBarArea | BobUI::LeftToolBarArea);
    QVERIFY(tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);

    tb.setAllowedAreas(BobUI::BottomToolBarArea | BobUI::RightToolBarArea);
    QCOMPARE(tb.allowedAreas(), BobUI::BottomToolBarArea | BobUI::RightToolBarArea);
    QVERIFY(!tb.isAreaAllowed(BobUI::LeftToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::RightToolBarArea));
    QVERIFY(!tb.isAreaAllowed(BobUI::TopToolBarArea));
    QVERIFY(tb.isAreaAllowed(BobUI::BottomToolBarArea));
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::ToolBarAreas *>(spy.at(0).value(0).constData()),
            tb.allowedAreas());
    spy.clear();
    tb.setAllowedAreas(tb.allowedAreas());
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIoolBar::orientation()
{
    BOBUIoolBar tb;
    QCOMPARE(tb.orientation(), BobUI::Horizontal);

    QSignalSpy spy(&tb, SIGNAL(orientationChanged(BobUI::Orientation)));

    tb.setOrientation(BobUI::Vertical);
    QCOMPARE(tb.orientation(), BobUI::Vertical);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::Orientation *>(spy.at(0).value(0).constData()),
            tb.orientation());
    spy.clear();
    tb.setOrientation(tb.orientation());
    QCOMPARE(spy.size(), 0);

    tb.setOrientation(BobUI::Horizontal);
    QCOMPARE(tb.orientation(), BobUI::Horizontal);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::Orientation *>(spy.at(0).value(0).constData()),
            tb.orientation());
    spy.clear();
    tb.setOrientation(tb.orientation());
    QCOMPARE(spy.size(), 0);

    tb.setOrientation(BobUI::Vertical);
    QCOMPARE(tb.orientation(), BobUI::Vertical);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::Orientation *>(spy.at(0).value(0).constData()),
            tb.orientation());
    spy.clear();
    tb.setOrientation(tb.orientation());
    QCOMPARE(spy.size(), 0);

    tb.setOrientation(BobUI::Horizontal);
    QCOMPARE(tb.orientation(), BobUI::Horizontal);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::Orientation *>(spy.at(0).value(0).constData()),
            tb.orientation());
    spy.clear();
    tb.setOrientation(tb.orientation());
    QCOMPARE(spy.size(), 0);

    tb.setOrientation(BobUI::Vertical);
    QCOMPARE(tb.orientation(), BobUI::Vertical);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(*static_cast<const BobUI::Orientation *>(spy.at(0).value(0).constData()),
            tb.orientation());
    spy.clear();
    tb.setOrientation(tb.orientation());
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIoolBar::addAction()
{
    BOBUIoolBar tb;

    {
        QAction action(0);

        QCOMPARE(tb.actions().size(), 0);
        tb.addAction(&action);
        QCOMPARE(tb.actions().size(), 1);
        QCOMPARE(tb.actions()[0], &action);

        tb.clear();
        QCOMPARE(tb.actions().size(), 0);
    }

    {
        QString text = "text";
        QPixmap pm(32, 32);
        pm.fill(BobUI::blue);
        QIcon icon = pm;

        QAction *action1 = tb.addAction(text);
        QCOMPARE(text, action1->text());

        QAction *action2 = tb.addAction(icon, text);
        QCOMPARE(icon, action2->icon());
        QCOMPARE(text, action2->text());

        QAction *action3 = tb.addAction(text, this, SLOT(slot()));
        QCOMPARE(text, action3->text());

        QAction *action4 = tb.addAction(icon, text, this, SLOT(slot()));
        QCOMPARE(icon, action4->icon());
        QCOMPARE(text, action4->text());

        QCOMPARE(tb.actions().size(), 4);
        QCOMPARE(tb.actions()[0], action1);
        QCOMPARE(tb.actions()[1], action2);
        QCOMPARE(tb.actions()[2], action3);
        QCOMPARE(tb.actions()[3], action4);

        tb.clear();
        QCOMPARE(tb.actions().size(), 0);
    }
}

static void testFunction() { }

void tst_BOBUIoolBar::addActionConnect()
{
    BOBUIoolBar tb;
    const QString text = QLatin1String("bla");
    const QIcon icon;
    tb.addAction(text, &tb, SLOT(deleteLater()));
    tb.addAction(text, &tb, &QMenu::deleteLater);
    tb.addAction(text, testFunction);
    tb.addAction(text, &tb, testFunction);
    tb.addAction(icon, text, &tb, SLOT(deleteLater()));
    tb.addAction(icon, text, &tb, &QMenu::deleteLater);
    tb.addAction(icon, text, testFunction);
    tb.addAction(icon, text, &tb, testFunction);
}

void tst_BOBUIoolBar::insertAction()
{
    BOBUIoolBar tb;
    QAction action1(0);
    QAction action2(0);
    QAction action3(0);
    QAction action4(0);

    QCOMPARE(tb.actions().size(), 0);
    tb.insertAction(0, &action1);
    tb.insertAction(&action1, &action2);
    tb.insertAction(&action2, &action3);
    tb.insertAction(&action3, &action4);
    QCOMPARE(tb.actions().size(), 4);
    QCOMPARE(tb.actions()[0], &action4);
    QCOMPARE(tb.actions()[1], &action3);
    QCOMPARE(tb.actions()[2], &action2);
    QCOMPARE(tb.actions()[3], &action1);

    tb.clear();
    QCOMPARE(tb.actions().size(), 0);
}

void tst_BOBUIoolBar::addSeparator()
{
    BOBUIoolBar tb;

    QAction action1(0);
    QAction action2(0);

    tb.addAction(&action1);
    QAction *sep = tb.addSeparator();
    tb.addAction(&action2);

    QCOMPARE(tb.actions().size(), 3);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], sep);
    QCOMPARE(tb.actions()[2], &action2);

    tb.clear();
    QCOMPARE(tb.actions().size(), 0);
}

void tst_BOBUIoolBar::insertSeparator()
{
    BOBUIoolBar tb;

    QAction action1(0);
    QAction action2(0);

    tb.addAction(&action1);
    tb.addAction(&action2);
    QAction *sep = tb.insertSeparator(&action2);

    QCOMPARE(tb.actions().size(), 3);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], sep);
    QCOMPARE(tb.actions()[2], &action2);

    tb.clear();
    QCOMPARE(tb.actions().size(), 0);
}

void tst_BOBUIoolBar::addWidget()
{
    BOBUIoolBar tb;
    QWidget w(&tb);

    QAction action1(0);
    QAction action2(0);

    tb.addAction(&action1);
    QAction *widget = tb.addWidget(&w);
    tb.addAction(&action2);

    QCOMPARE(tb.actions().size(), 3);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], widget);
    QCOMPARE(tb.actions()[2], &action2);

    // it should be possible to reuse the action returned by
    // addWidget() to place the widget somewhere else in the toolbar
    tb.removeAction(widget);
    QCOMPARE(tb.actions().size(), 2);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], &action2);

    tb.addAction(widget);
    QCOMPARE(tb.actions().size(), 3);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], &action2);
    QCOMPARE(tb.actions()[2], widget);

    tb.clear();
    QCOMPARE(tb.actions().size(), 0);
}

void tst_BOBUIoolBar::insertWidget()
{
    BOBUIoolBar tb;
    QWidget w(&tb);

    QAction action1(0);
    QAction action2(0);

    tb.addAction(&action1);
    tb.addAction(&action2);
    QAction *widget = tb.insertWidget(&action2, &w);

    QCOMPARE(tb.actions().size(), 3);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], widget);
    QCOMPARE(tb.actions()[2], &action2);

    // it should be possible to reuse the action returned by
    // addWidget() to place the widget somewhere else in the toolbar
    tb.removeAction(widget);
    QCOMPARE(tb.actions().size(), 2);
    QCOMPARE(tb.actions()[0], &action1);
    QCOMPARE(tb.actions()[1], &action2);

    tb.insertAction(&action1, widget);
    QCOMPARE(tb.actions().size(), 3);
    QCOMPARE(tb.actions()[0], widget);
    QCOMPARE(tb.actions()[1], &action1);
    QCOMPARE(tb.actions()[2], &action2);

    tb.clear();
    QCOMPARE(tb.actions().size(), 0);

    {
        BOBUIoolBar tb;
        QPointer<QWidget> widget = new QWidget;
        QAction *action = tb.addWidget(widget);
        QCOMPARE(action->parent(), &tb);

        BOBUIoolBar tb2;
        tb.removeAction(action);
        tb2.addAction(action);
        QVERIFY(widget && widget->parent() == &tb2);
        QCOMPARE(action->parent(), &tb2);
    }
}

void tst_BOBUIoolBar::actionGeometry()
{
    BOBUIoolBar tb;

    QAction action1(0);
    QAction action2(0);
    QAction action3(0);
    QAction action4(0);

    tb.addAction(&action1);
    tb.addAction(&action2);
    tb.addAction(&action3);
    tb.addAction(&action4);

    tb.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tb));

    QList<BOBUIoolBarExtension *> extensions = tb.findChildren<BOBUIoolBarExtension *>();

    QRect rect01;
    QRect rect02;
    QRect rect03;
    QRect rect04;
    QMenu *popupMenu = 0;

    if (extensions.size() != 0)
    {
        BOBUIoolBarExtension *extension = extensions.at(0);
        if (extension->isVisible()) {
            QRect rect0 = extension->geometry();
            BOBUIest::mouseClick( extension, BobUI::LeftButton, {}, rect0.center(), -1 );
            QApplication::processEvents();
            popupMenu = qobject_cast<QMenu *>(extension->menu());
            rect01 = popupMenu->actionGeometry(&action1);
            rect02 = popupMenu->actionGeometry(&action2);
            rect03 = popupMenu->actionGeometry(&action3);
            rect04 = popupMenu->actionGeometry(&action4);
        }
    }

    QRect rect1 = tb.actionGeometry(&action1);
    QRect rect2 = tb.actionGeometry(&action2);
    QRect rect3 = tb.actionGeometry(&action3);
    QRect rect4 = tb.actionGeometry(&action4);

    QVERIFY(rect1.isValid());
    QVERIFY(!rect1.isNull());
    QVERIFY(!rect1.isEmpty());

    QVERIFY(rect2.isValid());
    QVERIFY(!rect2.isNull());
    QVERIFY(!rect2.isEmpty());

    QVERIFY(rect3.isValid());
    QVERIFY(!rect3.isNull());
    QVERIFY(!rect3.isEmpty());

    QVERIFY(rect4.isValid());
    QVERIFY(!rect4.isNull());
    QVERIFY(!rect4.isEmpty());

    if (rect01.isValid())
        QCOMPARE(popupMenu->actionAt(rect01.center()), &action1);
    else
        QCOMPARE(tb.actionAt(rect1.center()), &action1);

    if (rect02.isValid())
        QCOMPARE(popupMenu->actionAt(rect02.center()), &action2);
    else
        QCOMPARE(tb.actionAt(rect2.center()), &action2);

    if (rect03.isValid())
        QCOMPARE(popupMenu->actionAt(rect03.center()), &action3);
    else
        QCOMPARE(tb.actionAt(rect3.center()), &action3);

    if (rect04.isValid())
        QCOMPARE(popupMenu->actionAt(rect04.center()), &action4);
    else
        QCOMPARE(tb.actionAt(rect4.center()), &action4);
}

void tst_BOBUIoolBar::toggleViewAction()
{
    {
        BOBUIoolBar tb;
        QAction *toggleViewAction = tb.toggleViewAction();
        QVERIFY(tb.isHidden());
        toggleViewAction->trigger();
        QVERIFY(!tb.isHidden());
        toggleViewAction->trigger();
        QVERIFY(tb.isHidden());
    }

    {
        QMainWindow mw;
        BOBUIoolBar tb(&mw);
        mw.addToolBar(&tb);
        mw.show();
        QAction *toggleViewAction = tb.toggleViewAction();
        QVERIFY(!tb.isHidden());
        toggleViewAction->trigger();
        QVERIFY(tb.isHidden());
        toggleViewAction->trigger();
        QVERIFY(!tb.isHidden());
        toggleViewAction->trigger();
        QVERIFY(tb.isHidden());
    }
}

void tst_BOBUIoolBar::iconSize()
{
    {
        BOBUIoolBar tb;

        QSignalSpy spy(&tb, SIGNAL(iconSizeChanged(QSize)));

        // the default is determined by the style
        const int metric = tb.style()->pixelMetric(QStyle::PM_ToolBarIconSize);
        const QSize defaultIconSize = QSize(metric, metric);
        const QSize smallIconSize = QSize(metric / 2, metric / 2);
        const QSize largeIconSize = QSize(metric * 2, metric * 2);

        QCOMPARE(tb.iconSize(), defaultIconSize);
        tb.setIconSize(defaultIconSize);
        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(spy.size(), 0);

        spy.clear();
        tb.setIconSize(largeIconSize);
        QCOMPARE(tb.iconSize(), largeIconSize);
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.first().first().toSize(), largeIconSize);

        // no-op
        spy.clear();
        tb.setIconSize(largeIconSize);
        QCOMPARE(tb.iconSize(), largeIconSize);
        QCOMPARE(spy.size(), 0);

        spy.clear();
        tb.setIconSize(defaultIconSize);
        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.first().first().toSize(), defaultIconSize);

        // no-op
        spy.clear();
        tb.setIconSize(defaultIconSize);
        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(spy.size(), 0);

        spy.clear();
        tb.setIconSize(smallIconSize);
        QCOMPARE(tb.iconSize(), smallIconSize);
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.first().first().toSize(), smallIconSize);

        // no-op
        spy.clear();
        tb.setIconSize(smallIconSize);
        QCOMPARE(tb.iconSize(), smallIconSize);
        QCOMPARE(spy.size(), 0);

        // setting the icon size to an invalid QSize will reset the
        // iconSize property to the default
        tb.setIconSize(QSize());
        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(spy.size(), 1);
        QCOMPARE(spy.first().first().toSize(), defaultIconSize);
        spy.clear();
    }

    {
        QMainWindow mw;
        BOBUIoolBar tb;
        QSignalSpy mwSpy(&mw, SIGNAL(iconSizeChanged(QSize)));
        QSignalSpy tbSpy(&tb, SIGNAL(iconSizeChanged(QSize)));

        // the default is determined by the style
        const int metric = tb.style()->pixelMetric(QStyle::PM_ToolBarIconSize);
        const QSize defaultIconSize = QSize(metric, metric);
        const QSize smallIconSize = QSize(metric / 2, metric / 2);
        const QSize largeIconSize = QSize(metric * 2, metric * 2);

        mw.setIconSize(smallIconSize);

        // explicitly set it to the default
        tb.setIconSize(defaultIconSize);
        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(tbSpy.size(), 0);

        mw.addToolBar(&tb);

        // tb icon size should not change since it has been explicitly set
        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(tbSpy.size(), 0);

        mw.setIconSize(largeIconSize);

        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(tbSpy.size(), 0);

        mw.setIconSize(defaultIconSize);

        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(tbSpy.size(), 0);

        mw.setIconSize(smallIconSize);

        QCOMPARE(tb.iconSize(), defaultIconSize);
        QCOMPARE(tbSpy.size(), 0);

        // resetting to the default should cause the toolbar to take
        // on the mainwindow's icon size
        tb.setIconSize(QSize());
        QCOMPARE(tb.iconSize(), smallIconSize);
        QCOMPARE(tbSpy.size(), 1);
        QCOMPARE(tbSpy.first().first().toSize(), smallIconSize);
        tbSpy.clear();
    }
}

void tst_BOBUIoolBar::toolButtonStyle()
{
    {
        BOBUIoolBar tb;

        QSignalSpy spy(&tb, SIGNAL(toolButtonStyleChanged(BobUI::ToolButtonStyle)));

        // no-op
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        tb.setToolButtonStyle(BobUI::ToolButtonIconOnly);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(spy.size(), 0);

        tb.setToolButtonStyle(BobUI::ToolButtonTextOnly);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonTextOnly);
        QCOMPARE(spy.size(), 1);
        spy.clear();

        // no-op
        tb.setToolButtonStyle(BobUI::ToolButtonTextOnly);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonTextOnly);
        QCOMPARE(spy.size(), 0);

        tb.setToolButtonStyle(BobUI::ToolButtonIconOnly);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(spy.size(), 1);
        spy.clear();

        // no-op
        tb.setToolButtonStyle(BobUI::ToolButtonIconOnly);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(spy.size(), 0);

        tb.setToolButtonStyle(BobUI::ToolButtonTextBesideIcon);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonTextBesideIcon);
        QCOMPARE(spy.size(), 1);
        spy.clear();

        // no-op
        tb.setToolButtonStyle(BobUI::ToolButtonTextBesideIcon);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonTextBesideIcon);
        QCOMPARE(spy.size(), 0);

        tb.setToolButtonStyle(BobUI::ToolButtonTextUnderIcon);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonTextUnderIcon);
        QCOMPARE(spy.size(), 1);
        spy.clear();

        // no-op
        tb.setToolButtonStyle(BobUI::ToolButtonTextUnderIcon);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonTextUnderIcon);
        QCOMPARE(spy.size(), 0);

        tb.setToolButtonStyle(BobUI::ToolButtonFollowStyle);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonFollowStyle);
        QCOMPARE(spy.size(), 1);
    }

    {
        QMainWindow mw;
        BOBUIoolBar tb;
        QSignalSpy mwSpy(&mw, SIGNAL(toolButtonStyleChanged(BobUI::ToolButtonStyle)));
        QSignalSpy tbSpy(&tb, SIGNAL(toolButtonStyleChanged(BobUI::ToolButtonStyle)));

        mw.setToolButtonStyle(BobUI::ToolButtonTextBesideIcon);

        // explicitly set the tb to the default
        tb.setToolButtonStyle(BobUI::ToolButtonIconOnly);
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(tbSpy.size(), 0);

        mw.addToolBar(&tb);

        // tb icon size should not change since it has been explicitly set
        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(tbSpy.size(), 0);

        mw.setToolButtonStyle(BobUI::ToolButtonIconOnly);

        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(tbSpy.size(), 0);

        mw.setToolButtonStyle(BobUI::ToolButtonTextOnly);

        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(tbSpy.size(), 0);

        mw.setToolButtonStyle(BobUI::ToolButtonTextUnderIcon);

        QCOMPARE(tb.toolButtonStyle(), BobUI::ToolButtonIconOnly);
        QCOMPARE(tbSpy.size(), 0);

        // note: there is no way to clear the explicitly set tool
        // button style... once you explicitly set it, the toolbar
        // will never follow the mainwindow again
    }
}

void tst_BOBUIoolBar::actionTriggered()
{
    BOBUIoolBar tb;
    connect(&tb, SIGNAL(actionTriggered(QAction*)), SLOT(slot(QAction*)));

    QAction action1(0);
    QAction action2(0);
    QAction action3(0);
    QAction action4(0);

    tb.addAction(&action1);
    tb.addAction(&action2);
    tb.addAction(&action3);
    tb.addAction(&action4);

    tb.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&tb));

    QList<BOBUIoolBarExtension *> extensions = tb.findChildren<BOBUIoolBarExtension *>();

    QRect rect01;
    QRect rect02;
    QRect rect03;
    QRect rect04;
    QMenu *popupMenu = 0;

    if (extensions.size() != 0)
    {
        BOBUIoolBarExtension *extension = extensions.at(0);
        if (extension->isVisible()) {
            QRect rect0 = extension->geometry();
            BOBUIest::mouseClick( extension, BobUI::LeftButton, {}, rect0.center(), -1 );
            QApplication::processEvents();
            popupMenu = qobject_cast<QMenu *>(extension->menu());
            rect01 = popupMenu->actionGeometry(&action1);
            rect02 = popupMenu->actionGeometry(&action2);
            rect03 = popupMenu->actionGeometry(&action3);
            rect04 = popupMenu->actionGeometry(&action4);
        }
    }

    QRect rect1 = tb.actionGeometry(&action1);
    QRect rect2 = tb.actionGeometry(&action2);
    QRect rect3 = tb.actionGeometry(&action3);
    QRect rect4 = tb.actionGeometry(&action4);

    QAbstractButton *button1 = 0;
    QAbstractButton *button2 = 0;
    QAbstractButton *button3 = 0;
    QAbstractButton *button4 = 0;

    if (!rect01.isValid()) {
        button1 = qobject_cast<QAbstractButton *>(tb.childAt(rect1.center()));
        QVERIFY(button1 != 0);
    }
    if (!rect02.isValid()) {
        button2 = qobject_cast<QAbstractButton *>(tb.childAt(rect2.center()));
        QVERIFY(button2 != 0);
    }
    if (!rect03.isValid()) {
        button3 = qobject_cast<QAbstractButton *>(tb.childAt(rect3.center()));
        QVERIFY(button3 != 0);
    }
    if (!rect04.isValid()) {
        button4 = qobject_cast<QAbstractButton *>(tb.childAt(rect4.center()));
        QVERIFY(button4 != 0);
    }

    ::triggered = 0;
    if (!rect01.isValid())
        BOBUIest::mouseClick(button1, BobUI::LeftButton);
    else
        BOBUIest::mouseClick(popupMenu, BobUI::LeftButton, {}, rect01.center(), -1 );
    QCOMPARE(::triggered, &action1);

    ::triggered = 0;
    if (!rect02.isValid())
        BOBUIest::mouseClick(button2, BobUI::LeftButton);
    else
        BOBUIest::mouseClick(popupMenu, BobUI::LeftButton, {}, rect02.center(), -1 );
    QCOMPARE(::triggered, &action2);

    ::triggered = 0;
    if (!rect03.isValid())
        BOBUIest::mouseClick(button3, BobUI::LeftButton);
    else
        BOBUIest::mouseClick(popupMenu, BobUI::LeftButton, {}, rect03.center(), -1 );
    QCOMPARE(::triggered, &action3);

    ::triggered = 0;
    if (!rect04.isValid())
        BOBUIest::mouseClick(button4, BobUI::LeftButton);
    else
        BOBUIest::mouseClick(popupMenu, BobUI::LeftButton, {}, rect04.center(), -1 );
    QCOMPARE(::triggered, &action4);
}

void tst_BOBUIoolBar::visibilityChanged()
{
    QMainWindow mw;
    BOBUIoolBar tb;
    QSignalSpy spy(&tb, SIGNAL(visibilityChanged(bool)));

    mw.addToolBar(&tb);
    mw.show();

    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
    spy.clear();

    tb.hide();
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    spy.clear();

    tb.hide();
    QCOMPARE(spy.size(), 0);

    tb.show();
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
    spy.clear();

    tb.show();
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIoolBar::actionOwnership()
{
    {
        BOBUIoolBar *tb1 = new BOBUIoolBar;
        BOBUIoolBar *tb2 = new BOBUIoolBar;

        QPointer<QAction> action = tb1->addAction("test");
        QCOMPARE(action->parent(), tb1);

        tb2->addAction(action);
        QCOMPARE(action->parent(), tb1);

        delete tb1;
        QVERIFY(!action);
        delete tb2;
    }
    {
        BOBUIoolBar *tb1 = new BOBUIoolBar;
        BOBUIoolBar *tb2 = new BOBUIoolBar;

        QPointer<QAction> action = tb1->addAction("test");
        QCOMPARE(action->parent(), tb1);

        tb1->removeAction(action);
        QCOMPARE(action->parent(), tb1);

        tb2->addAction(action);
        QCOMPARE(action->parent(), tb1);

        delete tb1;
        QVERIFY(!action);
        delete tb2;
    }
}

void tst_BOBUIoolBar::widgetAction()
{
    // ensure that a QWidgetAction without widget behaves like a normal action
    BOBUIoolBar tb;
    QWidgetAction *a = new QWidgetAction(0);
    a->setIconText("Blah");

    tb.addAction(a);
    QWidget *w = tb.widgetForAction(a);
    QVERIFY(w);
    BOBUIoolButton *button = qobject_cast<BOBUIoolButton *>(w);
    QVERIFY(button);
    QCOMPARE(a->iconText(), button->text());

    delete a;
}

#ifdef Q_OS_MAC
BOBUI_BEGIN_NAMESPACE
extern void bobui_set_sequence_auto_mnemonic(bool b);
BOBUI_END_NAMESPACE
#endif

void tst_BOBUIoolBar::accel()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

#ifdef Q_OS_MAC
    bobui_set_sequence_auto_mnemonic(true);
#endif
    QMainWindow mw;
    BOBUIoolBar *toolBar = mw.addToolBar("test");
    QAction *action = toolBar->addAction("&test");
    action->setIconText(action->text()); // we really want that mnemonic in the button!

    QSignalSpy spy(action, SIGNAL(triggered(bool)));

    mw.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mw));

    BOBUIest::keyClick(&mw, BobUI::Key_T, BobUI::AltModifier);

    BOBUIRY_COMPARE(spy.size(), 1);
#ifdef Q_OS_MAC
    bobui_set_sequence_auto_mnemonic(false);
#endif
}

void tst_BOBUIoolBar::task191727_layout()
{
    QMainWindow mw;
    BOBUIoolBar *toolBar = mw.addToolBar("test");
    toolBar->addAction("one");
    QAction *action = toolBar->addAction("two");

    QLineEdit *lineedit = new QLineEdit;
    lineedit->setMaximumWidth(50);
    toolBar->addWidget(lineedit);

    mw.resize(400, 400);
    mw.show();

    QWidget *actionwidget = toolBar->widgetForAction(action);
    QVERIFY(qAbs(lineedit->pos().x() - (actionwidget->geometry().right() + 1 + toolBar->layout()->spacing())) < 2);
}

void tst_BOBUIoolBar::task197996_visibility()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QMainWindow mw;
    BOBUIoolBar *toolBar = new BOBUIoolBar(&mw);

    mw.addToolBar(toolBar);
    toolBar->addAction(new QAction("Foo", &mw));
    QAction *pAction = new QAction("Test", &mw);
    toolBar->addAction(pAction);

    pAction->setVisible(false);
    toolBar->setVisible(false);

    toolBar->setVisible(true);
    pAction->setVisible(true);

    mw.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mw));

    QVERIFY(toolBar->widgetForAction(pAction)->isVisible());

    toolBar->setVisible(false);
    pAction->setVisible(false);

    QVERIFY(!toolBar->widgetForAction(pAction)->isVisible());

    toolBar->setVisible(true);
    pAction->setVisible(true);

    BOBUIRY_VERIFY(toolBar->widgetForAction(pAction)->isVisible());
}

class ShowHideEventCounter : public QObject
{
public:
    using QObject::QObject;

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (qobject_cast<QLineEdit*>(watched) && !event->spontaneous()) {
            if (event->type() == QEvent::Show)
                ++m_showEventsCount;

            if (event->type() == QEvent::Hide)
                ++m_hideEventsCount;
        }

        return QObject::eventFilter(watched, event);
    }

    uint showEventsCount() const { return m_showEventsCount; }
    uint hideEventsCount() const { return m_hideEventsCount; }

private:
    uint m_showEventsCount = 0;
    uint m_hideEventsCount = 0;
};

void tst_BOBUIoolBar::extraCpuConsumption()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QMainWindow mainWindow;

    auto tb = new BOBUIoolBar(&mainWindow);
    tb->setMovable(false);

    auto extensions = tb->findChildren<BOBUIoolBarExtension *>();
    QVERIFY(!extensions.isEmpty());

    auto extensionButton = extensions.at(0);
    QVERIFY(extensionButton);

    tb->addWidget(new QLabel("Lorem ipsum dolor sit amet"));

    auto le = new QLineEdit;
    le->setClearButtonEnabled(true);
    le->setText("Lorem ipsum");
    tb->addWidget(le);

    mainWindow.addToolBar(tb);
    mainWindow.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mainWindow));

    auto eventCounter = new ShowHideEventCounter(&mainWindow);
    le->installEventFilter(eventCounter);

    auto defaultSize = mainWindow.size();

    // Line edit should be hidden now and extension button should be displayed
    for (double p = 0.7; extensionButton->isHidden() || qFuzzyCompare(p, 0.); p -= 0.01) {
        mainWindow.resize(int(defaultSize.width() * p), defaultSize.height());
    }
    QVERIFY(!extensionButton->isHidden());

    // Line edit should be visible, but smaller
    for (double p = 0.75; !extensionButton->isHidden() || qFuzzyCompare(p, 1.); p += 0.01) {
        mainWindow.resize(int(defaultSize.width() * p), defaultSize.height());
    }
    QVERIFY(extensionButton->isHidden());

    // Dispatch all pending events
    qApp->sendPostedEvents();
    qApp->processEvents();

    QCOMPARE(eventCounter->showEventsCount(), eventCounter->hideEventsCount());
    QCOMPARE(eventCounter->showEventsCount(), uint(1));
    QCOMPARE(eventCounter->hideEventsCount(), uint(1));
}

void tst_BOBUIoolBar::iconSizeStyleSheet()
{
    auto resetStyleSheet = qScopeGuard([]{
        qApp->setStyleSheet({});
    });

    QMainWindow mw;
    BOBUIoolBar tb1;
    BOBUIoolBar tb2;
    mw.addToolBar(&tb1);
    mw.addToolBar(&tb2);

    const QString styleSheet = "BOBUIoolBar { icon-size: %1px; }";

    const int targetIconSize1 = 48;
    const int targetIconSize2 = 58;
    const int targetIconSize3 = 68;
    const int targetIconSize4 = 78;

    // Set iconSize via stylesheet using icon-size property first time
    qApp->setStyleSheet(styleSheet.arg(targetIconSize1));
    BOBUIRY_COMPARE(tb1.iconSize(), QSize(targetIconSize1, targetIconSize1));
    BOBUIRY_COMPARE(tb2.iconSize(), QSize(targetIconSize1, targetIconSize1));

    qApp->setStyleSheet(styleSheet.arg(targetIconSize2));
    BOBUIRY_COMPARE(tb1.iconSize(), QSize(targetIconSize2, targetIconSize2));
    BOBUIRY_COMPARE(tb2.iconSize(), QSize(targetIconSize2, targetIconSize2));

    // Set tb2's icon size explicitly via setIconSize()
    tb2.setIconSize(QSize(targetIconSize3, targetIconSize3));
    QCOMPARE(tb1.iconSize(), QSize(targetIconSize2, targetIconSize2));
    QCOMPARE(tb2.iconSize(), QSize(targetIconSize3, targetIconSize3));

    // Set iconSize via stylesheet using icon-size property after setting using setIconSize()
    qApp->setStyleSheet(styleSheet.arg(targetIconSize4));
    // setIconSize has precedence over iconSize set via stylesheet using icon-size property
    BOBUIRY_COMPARE(tb1.iconSize(), QSize(targetIconSize4, targetIconSize4));
    QCOMPARE(tb2.iconSize(), QSize(targetIconSize3, targetIconSize3));
}

BOBUIEST_MAIN(tst_BOBUIoolBar)
#include "tst_bobuioolbar.moc"
