// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include "qbuttongroup.h"
#include <qaction.h>
#include <qapplication.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <bobuioolbutton.h>
#ifdef Q_OS_MAC
#include <qsettings.h>
#endif

#include <BobUIWidgets/private/qapplication_p.h>

class SpecialRadioButton: public QRadioButton
{
public:
    SpecialRadioButton(QWidget *parent) : QRadioButton(parent)
    { }

protected:
    void focusInEvent(QFocusEvent *) override
    {
        QCoreApplication::postEvent(this, new QKeyEvent(QEvent::KeyPress,
                                                        BobUI::Key_Down, BobUI::NoModifier));
    }
};

class TestPushButton : public QPushButton
{
public:
    TestPushButton(QWidget *parent = nullptr)
    : QPushButton(parent)
    {}
    TestPushButton(const QString &title, QWidget *parent = nullptr)
    : QPushButton(title, parent)
    {}

protected:
    bool hitButton(const QPoint &pos) const override
    {
        return rect().contains(pos);
    }
};

#include <qbuttongroup.h>

class tst_QButtonGroup : public QObject
{
Q_OBJECT

private slots:
    void arrowKeyNavigation();
    void keyNavigationPushButtons();
    void exclusive();
    void exclusiveWithActions();
    void testSignals();
    void checkedButton();

    void task106609();

    void autoIncrementId();

    void task209485_removeFromGroupInEventHandler_data();
    void task209485_removeFromGroupInEventHandler();
};

BOBUI_BEGIN_NAMESPACE
extern bool Q_GUI_EXPORT bobui_tab_all_widgets();
BOBUI_END_NAMESPACE


void tst_QButtonGroup::arrowKeyNavigation()
{
    if (!bobui_tab_all_widgets())
        QSKIP("This test requires full keyboard control to be enabled.");

    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QDialog dlg(0);
    QHBoxLayout layout(&dlg);
    QGroupBox g1("1", &dlg);
    QHBoxLayout g1layout(&g1);
    QRadioButton bt1("Radio1", &g1);
    TestPushButton pb("PB", &g1);
    QLineEdit le(&g1);
    QRadioButton bt2("Radio2", &g1);
    g1layout.addWidget(&bt1);
    g1layout.addWidget(&pb);
    g1layout.addWidget(&le);
    g1layout.addWidget(&bt2);

    // create a mixed button group with radion buttons and push
    // buttons. Not very useful, but it tests borderline cases wrt
    // focus handling.
    QButtonGroup bgrp1(&g1);
    bgrp1.addButton(&bt1);
    bgrp1.addButton(&pb);
    bgrp1.addButton(&bt2);

    QGroupBox g2("2", &dlg);
    QVBoxLayout g2layout(&g2);
    // we don't need a button group here, because radio buttons are
    // auto exclusive, i.e. they group themselves in he same parent
    // widget.
    QRadioButton bt3("Radio3", &g2);
    QRadioButton bt4("Radio4", &g2);
    g2layout.addWidget(&bt3);
    g2layout.addWidget(&bt4);

    layout.addWidget(&g1);
    layout.addWidget(&g2);

    dlg.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&dlg));

    bt1.setFocus();

    BOBUIRY_VERIFY(bt1.hasFocus());

    BOBUIest::keyClick(&bt1, BobUI::Key_Right);
    QVERIFY(pb.hasFocus());
    BOBUIest::keyClick(&pb, BobUI::Key_Right);
    QVERIFY(bt2.hasFocus());
    BOBUIest::keyClick(&bt2, BobUI::Key_Right);
    QVERIFY(bt2.hasFocus());
    BOBUIest::keyClick(&bt2, BobUI::Key_Left);
    QVERIFY(pb.hasFocus());
    BOBUIest::keyClick(&pb, BobUI::Key_Left);
    QVERIFY(bt1.hasFocus());

    BOBUIest::keyClick(&bt1, BobUI::Key_Tab);
    QVERIFY(pb.hasFocus());
    BOBUIest::keyClick(&pb, BobUI::Key_Tab);

    QVERIFY(le.hasFocus());
    QCOMPARE(le.selectedText(), le.text());
    BOBUIest::keyClick(&le, BobUI::Key_Tab);

    QVERIFY(bt2.hasFocus());
    BOBUIest::keyClick(&bt2, BobUI::Key_Tab);
    QVERIFY(bt3.hasFocus());

    BOBUIest::keyClick(&bt3, BobUI::Key_Down);
    QVERIFY(bt4.hasFocus());
    BOBUIest::keyClick(&bt4, BobUI::Key_Down);
    QVERIFY(bt4.hasFocus());

    BOBUIest::keyClick(&bt4, BobUI::Key_Up);
    QVERIFY(bt3.hasFocus());
    BOBUIest::keyClick(&bt3, BobUI::Key_Up);
    QVERIFY(bt3.hasFocus());
}

/*
    Test that tab and arrow key navigation through buttons
    in an invisible button group works as expected. Tabbing
    into the group should give focus to the checked button,
    and arrow navigation should change the checked button and
    move focus.
*/
void tst_QButtonGroup::keyNavigationPushButtons()
{
    if (!bobui_tab_all_widgets())
        QSKIP("This test requires full keyboard control to be enabled.");

    QDialog dlg(nullptr);
    QLineEdit *le1 = new QLineEdit;
    le1->setObjectName("le1");
    QPushButton *pb1 = new QPushButton("Exclusive 1");
    pb1->setObjectName("pb1");
    pb1->setCheckable(true);
    pb1->setChecked(true);
    QPushButton *pb2 = new QPushButton("Exclusive 2");
    pb2->setObjectName("pb2");
    pb2->setCheckable(true);
    QPushButton *pb3 = new QPushButton("Exclusive 3");
    pb3->setObjectName("pb3");
    pb3->setCheckable(true);
    QLineEdit *le2 = new QLineEdit;
    le2->setObjectName("le2");

    QVBoxLayout* layout = new QVBoxLayout(&dlg);
    layout->addWidget(le1);
    layout->addWidget(pb1);
    layout->addWidget(pb2);
    layout->addWidget(pb3);
    layout->addWidget(le2);

    QButtonGroup *buttonGroup = new QButtonGroup(&dlg);
    buttonGroup->addButton(pb1);
    buttonGroup->addButton(pb2);
    buttonGroup->addButton(pb3);

    dlg.show();
    QVERIFY(BOBUIest::qWaitForWindowFocused(&dlg));

    QVERIFY2(le1->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Tab);
    QVERIFY2(pb1->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    QVERIFY2(pb1->isChecked(), qPrintable(buttonGroup->checkedButton()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Down);
    QVERIFY2(pb2->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    QVERIFY2(pb2->isChecked(), qPrintable(buttonGroup->checkedButton()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Down);
    QVERIFY2(pb3->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    QVERIFY2(pb3->isChecked(), qPrintable(buttonGroup->checkedButton()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Up);
    QVERIFY2(pb2->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    QVERIFY2(pb2->isChecked(), qPrintable(buttonGroup->checkedButton()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Tab);
    QVERIFY2(le2->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Backtab);
    QVERIFY2(pb2->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
    QVERIFY2(pb2->isChecked(), qPrintable(buttonGroup->checkedButton()->objectName()));
    BOBUIest::keyClick(qApp->focusWidget(), BobUI::Key_Backtab);
    QVERIFY2(le1->hasFocus(), qPrintable(qApp->focusWidget()->objectName()));
}

void tst_QButtonGroup::exclusiveWithActions()
{
    QDialog dlg(0);
    QHBoxLayout layout(&dlg);
    QAction *action1 = new QAction("Action 1", &dlg);
    action1->setCheckable(true);
    QAction *action2 = new QAction("Action 2", &dlg);
    action2->setCheckable(true);
    QAction *action3 = new QAction("Action 3", &dlg);
    action3->setCheckable(true);
    BOBUIoolButton *toolButton1 = new BOBUIoolButton(&dlg);
    BOBUIoolButton *toolButton2 = new BOBUIoolButton(&dlg);
    BOBUIoolButton *toolButton3 = new BOBUIoolButton(&dlg);
    toolButton1->setDefaultAction(action1);
    toolButton2->setDefaultAction(action2);
    toolButton3->setDefaultAction(action3);
    layout.addWidget(toolButton1);
    layout.addWidget(toolButton2);
    layout.addWidget(toolButton3);
    QButtonGroup *buttonGroup = new QButtonGroup( &dlg );
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(toolButton1, 1);
    buttonGroup->addButton(toolButton2, 2);
    buttonGroup->addButton(toolButton3, 3);
    dlg.show();

    BOBUIest::mouseClick(toolButton1, BobUI::LeftButton);
    QVERIFY(toolButton1->isChecked());
    QVERIFY(action1->isChecked());
    QVERIFY(!toolButton2->isChecked());
    QVERIFY(!toolButton3->isChecked());
    QVERIFY(!action2->isChecked());
    QVERIFY(!action3->isChecked());

    BOBUIest::mouseClick(toolButton2, BobUI::LeftButton);
    QVERIFY(toolButton2->isChecked());
    QVERIFY(action2->isChecked());
    QVERIFY(!toolButton1->isChecked());
    QVERIFY(!toolButton3->isChecked());
    QVERIFY(!action1->isChecked());
    QVERIFY(!action3->isChecked());

    BOBUIest::mouseClick(toolButton3, BobUI::LeftButton);
    QVERIFY(toolButton3->isChecked());
    QVERIFY(action3->isChecked());
    QVERIFY(!toolButton1->isChecked());
    QVERIFY(!toolButton2->isChecked());
    QVERIFY(!action1->isChecked());
    QVERIFY(!action2->isChecked());

    BOBUIest::mouseClick(toolButton2, BobUI::LeftButton);
    QVERIFY(toolButton2->isChecked());
    QVERIFY(action2->isChecked());
    QVERIFY(!toolButton1->isChecked());
    QVERIFY(!toolButton3->isChecked());
    QVERIFY(!action1->isChecked());
    QVERIFY(!action3->isChecked());
}

void tst_QButtonGroup::exclusive()
{
    QDialog dlg(0);
    QHBoxLayout layout(&dlg);
    TestPushButton *pushButton1 = new TestPushButton(&dlg);
    TestPushButton *pushButton2 = new TestPushButton(&dlg);
    TestPushButton *pushButton3 = new TestPushButton(&dlg);
    pushButton1->setCheckable(true);
    pushButton2->setCheckable(true);
    pushButton3->setCheckable(true);
    layout.addWidget(pushButton1);
    layout.addWidget(pushButton2);
    layout.addWidget(pushButton3);
    QButtonGroup *buttonGroup = new QButtonGroup( &dlg );
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(pushButton1, 1);
    buttonGroup->addButton(pushButton2, 2);
    buttonGroup->addButton(pushButton3, 3);
    dlg.show();

    BOBUIest::mouseClick(pushButton1, BobUI::LeftButton);
    QVERIFY(pushButton1->isChecked());
    QVERIFY(!pushButton2->isChecked());
    QVERIFY(!pushButton3->isChecked());

    BOBUIest::mouseClick(pushButton2, BobUI::LeftButton);
    QVERIFY(pushButton2->isChecked());
    QVERIFY(!pushButton1->isChecked());
    QVERIFY(!pushButton3->isChecked());

    BOBUIest::mouseClick(pushButton3, BobUI::LeftButton);
    QVERIFY(pushButton3->isChecked());
    QVERIFY(!pushButton1->isChecked());
    QVERIFY(!pushButton2->isChecked());

    BOBUIest::mouseClick(pushButton2, BobUI::LeftButton);
    QVERIFY(pushButton2->isChecked());
    QVERIFY(!pushButton1->isChecked());
    QVERIFY(!pushButton3->isChecked());
}

void tst_QButtonGroup::testSignals()
{
    QButtonGroup buttons;
    TestPushButton pb1;
    TestPushButton pb2;
    TestPushButton pb3;
    buttons.addButton(&pb1);
    buttons.addButton(&pb2, 23);
    buttons.addButton(&pb3);

    qRegisterMetaType<QAbstractButton *>("QAbstractButton *");
    QSignalSpy clickedSpy(&buttons, SIGNAL(buttonClicked(QAbstractButton*)));
    QSignalSpy clickedIdSpy(&buttons, SIGNAL(idClicked(int)));
    QSignalSpy pressedSpy(&buttons, SIGNAL(buttonPressed(QAbstractButton*)));
    QSignalSpy pressedIdSpy(&buttons, SIGNAL(idPressed(int)));
    QSignalSpy releasedSpy(&buttons, SIGNAL(buttonReleased(QAbstractButton*)));
    QSignalSpy releasedIdSpy(&buttons, SIGNAL(idReleased(int)));

    pb1.animateClick();
    BOBUIestEventLoop::instance().enterLoop(1);

    QCOMPARE(clickedSpy.size(), 1);
    QCOMPARE(clickedIdSpy.size(), 1);

    int expectedId = -2;

    QCOMPARE(clickedIdSpy.takeFirst().at(0).toInt(), expectedId);
    QCOMPARE(pressedSpy.size(), 1);
    QCOMPARE(pressedIdSpy.size(), 1);
    QCOMPARE(pressedIdSpy.takeFirst().at(0).toInt(), expectedId);
    QCOMPARE(releasedSpy.size(), 1);
    QCOMPARE(releasedIdSpy.size(), 1);
    QCOMPARE(releasedIdSpy.takeFirst().at(0).toInt(), expectedId);

    clickedSpy.clear();
    clickedIdSpy.clear();
    pressedSpy.clear();
    pressedIdSpy.clear();
    releasedSpy.clear();
    releasedIdSpy.clear();

    pb2.animateClick();
    BOBUIestEventLoop::instance().enterLoop(1);

    QCOMPARE(clickedSpy.size(), 1);
    QCOMPARE(clickedIdSpy.size(), 1);
    QCOMPARE(clickedIdSpy.takeFirst().at(0).toInt(), 23);
    QCOMPARE(pressedSpy.size(), 1);
    QCOMPARE(pressedIdSpy.size(), 1);
    QCOMPARE(pressedIdSpy.takeFirst().at(0).toInt(), 23);
    QCOMPARE(releasedSpy.size(), 1);
    QCOMPARE(releasedIdSpy.size(), 1);
    QCOMPARE(releasedIdSpy.takeFirst().at(0).toInt(), 23);


    QSignalSpy toggledSpy(&buttons, SIGNAL(buttonToggled(QAbstractButton*, bool)));
    QSignalSpy toggledIdSpy(&buttons, SIGNAL(idToggled(int, bool)));

    pb1.setCheckable(true);
    pb2.setCheckable(true);
    pb1.toggle();
    QCOMPARE(toggledSpy.size(), 1);
    QCOMPARE(toggledIdSpy.size(), 1);

    pb2.toggle();
    QCOMPARE(toggledSpy.size(), 3);     // equals 3 since pb1 and pb2 are both toggled
    QCOMPARE(toggledIdSpy.size(), 3);

    pb1.setCheckable(false);
    pb2.setCheckable(false);
    pb1.toggle();
    QCOMPARE(toggledSpy.size(), 3);
    QCOMPARE(toggledIdSpy.size(), 3);
}

void tst_QButtonGroup::task106609()
{
    // task is:
    // sometimes, only one of the two signals in QButtonGroup get emitted
    // they get emitted when using the mouse, but when using the keyboard, only one is
    //

    QDialog dlg(0);
    QButtonGroup *buttons = new QButtonGroup(&dlg);
    QVBoxLayout *vbox = new QVBoxLayout(&dlg);

    SpecialRadioButton *radio1 = new SpecialRadioButton(&dlg);
    radio1->setText("radio1");
    SpecialRadioButton *radio2 = new SpecialRadioButton(&dlg);
    radio2->setText("radio2");
    QRadioButton *radio3 = new QRadioButton(&dlg);
    radio3->setText("radio3");

    buttons->addButton(radio1, 1);
    vbox->addWidget(radio1);
    buttons->addButton(radio2, 2);
    vbox->addWidget(radio2);
    buttons->addButton(radio3, 3);
    vbox->addWidget(radio3);
    dlg.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&dlg));

    qRegisterMetaType<QAbstractButton*>("QAbstractButton*");
    QSignalSpy spy1(buttons, SIGNAL(buttonClicked(QAbstractButton*)));

    BOBUIRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget*>(&dlg));

    radio1->setFocus();
    radio1->setChecked(true);
    BOBUIestEventLoop::instance().enterLoop(1);

    QCOMPARE(spy1.size(), 2);
}

void tst_QButtonGroup::checkedButton()
{
    QButtonGroup buttons;
    buttons.setExclusive(false);
    TestPushButton pb1;
    pb1.setCheckable(true);
    TestPushButton pb2;
    pb2.setCheckable(true);
    buttons.addButton(&pb1);
    buttons.addButton(&pb2, 23);

    QVERIFY(!buttons.checkedButton());
    pb1.setChecked(true);
    QCOMPARE(buttons.checkedButton(), &pb1);
    pb2.setChecked(true);
    QCOMPARE(buttons.checkedButton(), &pb2);
    pb2.setChecked(false);
    QCOMPARE(buttons.checkedButton(), &pb1);
    pb1.setChecked(false);
    QVERIFY(!buttons.checkedButton());

    buttons.setExclusive(true);
    QVERIFY(!buttons.checkedButton());
    pb1.setChecked(true);
    QCOMPARE(buttons.checkedButton(), &pb1);
    pb2.setChecked(true);
    QCOMPARE(buttons.checkedButton(), &pb2);
    // checked button cannot be unchecked
    pb2.setChecked(false);
    QCOMPARE(buttons.checkedButton(), &pb2);
}

void tst_QButtonGroup::task209485_removeFromGroupInEventHandler_data()
{
    BOBUIest::addColumn<bool>("deleteButton");
    BOBUIest::addColumn<int>("signalCount");
    BOBUIest::newRow("buttonPress 1") << true << 1;
    BOBUIest::newRow("buttonPress 2") << false << 1;
}

void tst_QButtonGroup::task209485_removeFromGroupInEventHandler()
{
    QFETCH(bool, deleteButton);
    QFETCH(int, signalCount);
    qRegisterMetaType<QAbstractButton *>("QAbstractButton *");

    TestPushButton button;
    QButtonGroup group;
    group.addButton(&button);

    if (deleteButton) {
        QObject::connect(&group, &QButtonGroup::buttonClicked,
                         &button, [&] { group.removeButton(&button); });
    }

    QSignalSpy spy1(&group, SIGNAL(buttonClicked(QAbstractButton*)));

    // NOTE: Reintroducing the bug of this task will cause the following line to crash:
    BOBUIest::mouseClick(&button, BobUI::LeftButton);

    QCOMPARE(spy1.size(), signalCount);
}

void tst_QButtonGroup::autoIncrementId()
{
    QDialog dlg(0);
    QButtonGroup *buttons = new QButtonGroup(&dlg);
    QVBoxLayout *vbox = new QVBoxLayout(&dlg);

    QRadioButton *radio1 = new QRadioButton(&dlg);
    radio1->setText("radio1");
    QRadioButton *radio2 = new QRadioButton(&dlg);
    radio2->setText("radio2");
    QRadioButton *radio3 = new QRadioButton(&dlg);
    radio3->setText("radio3");

    buttons->addButton(radio1, 2);
    vbox->addWidget(radio1);
    buttons->addButton(radio2, -1);
    vbox->addWidget(radio2);
    buttons->addButton(radio3);
    vbox->addWidget(radio3);

    radio1->setChecked(true);

    QCOMPARE(buttons->id(radio1), 2);
    QCOMPARE(buttons->id(radio2), -2);
    QCOMPARE(buttons->id(radio3), -3);

    dlg.show();
}

BOBUIEST_MAIN(tst_QButtonGroup)
#include "tst_qbuttongroup.moc"
