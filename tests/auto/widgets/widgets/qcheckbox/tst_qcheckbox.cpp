// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <QApplication>
#include <QPixmap>
#include <QDateTime>
#include <QCheckBox>

class tst_QCheckBox : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void setChecked();
    void setCheckedSignal();
    void setTriState();
    void setText_data();
    void setText();
    void isToggleButton();
    void setDown();
    void setAutoRepeat();
    void toggle();
    void pressed();
    void toggled();
    void checkStateChanged();
    void foregroundRole();
    void minimumSizeHint();
};

void tst_QCheckBox::initTestCase()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");
}

// ***************************************************

void tst_QCheckBox::setChecked()
{
    QCheckBox testWidget;
    testWidget.setChecked(true);
    QVERIFY(testWidget.isChecked());
    QVERIFY(testWidget.isChecked());
    QVERIFY(testWidget.checkState() == BobUI::Checked);

    testWidget.setChecked(false);
    QVERIFY(!testWidget.isChecked());
    QVERIFY(!testWidget.isChecked());
    QVERIFY(testWidget.checkState() == BobUI::Unchecked);

    testWidget.setChecked(false);
    BOBUIest::keyClick(&testWidget, ' ');
    QVERIFY(testWidget.isChecked());

    BOBUIest::keyClick(&testWidget, ' ');
    QVERIFY(!testWidget.isChecked());
}

void tst_QCheckBox::setCheckedSignal()
{
    QCheckBox testWidget;
    testWidget.setCheckState(BobUI::Unchecked);
    QSignalSpy checkStateChangedSpy(&testWidget, &QCheckBox::checkStateChanged);
    testWidget.setCheckState(BobUI::Checked);
    testWidget.setCheckState(BobUI::Checked);
    BOBUIRY_COMPARE(checkStateChangedSpy.size(), 1);   // get signal only once
    QCOMPARE(testWidget.checkState(), BobUI::Checked);
    testWidget.setCheckState(BobUI::Unchecked);
    testWidget.setCheckState(BobUI::Unchecked);
    BOBUIRY_COMPARE(checkStateChangedSpy.size(), 2);   // get signal only once
    QCOMPARE(testWidget.checkState(), BobUI::Unchecked);
    testWidget.setCheckState(BobUI::PartiallyChecked);
    testWidget.setCheckState(BobUI::PartiallyChecked);
    BOBUIRY_COMPARE(checkStateChangedSpy.size(), 3);   // get signal only once
    QCOMPARE(testWidget.checkState(), BobUI::PartiallyChecked);
}

void tst_QCheckBox::setTriState()
{
    QCheckBox testWidget;
    testWidget.setTristate(true);
    QVERIFY(testWidget.isTristate());
    QVERIFY(testWidget.checkState() == BobUI::Unchecked);

    testWidget.setCheckState(BobUI::PartiallyChecked);
    QVERIFY(testWidget.checkState() == BobUI::PartiallyChecked);

    testWidget.setChecked(true);
    QVERIFY(testWidget.isChecked());
    QVERIFY(testWidget.checkState() == BobUI::Checked);

    testWidget.setChecked(false);
    QVERIFY(!testWidget.isChecked());
    QVERIFY(testWidget.checkState() == BobUI::Unchecked);

    testWidget.setCheckState(BobUI::PartiallyChecked);
    QVERIFY(testWidget.checkState() == BobUI::PartiallyChecked);

    testWidget.setTristate(false);
    QVERIFY(!testWidget.isTristate());

    testWidget.setCheckState(BobUI::PartiallyChecked);
    QVERIFY(testWidget.checkState() == BobUI::PartiallyChecked);

    testWidget.setChecked(true);
    QVERIFY(testWidget.checkState() == BobUI::Checked);

    testWidget.setChecked(false);
    QVERIFY(testWidget.checkState() == BobUI::Unchecked);
}

void tst_QCheckBox::setText_data()
{
    BOBUIest::addColumn<QString>("s1");

    BOBUIest::newRow("data0" ) << QString("This is a text");
    BOBUIest::newRow("data1" ) << QString("A");
    BOBUIest::newRow("data2" ) << QString("ABCDEFG ");
    BOBUIest::newRow("data3" ) << QString("Text\nwith a cr-lf");
    BOBUIest::newRow("data4" ) << QString("");
}

void tst_QCheckBox::setText()
{
    QCheckBox testWidget;
    QFETCH(QString, s1);
    testWidget.setText(s1);
    QCOMPARE(testWidget.text(), s1);
}

void tst_QCheckBox::setDown()
{
    QCheckBox testWidget;
    testWidget.setDown(true);
    QVERIFY(testWidget.isDown());

    testWidget.setDown(false);
    QVERIFY(!testWidget.isDown());
}

void tst_QCheckBox::setAutoRepeat()
{
    QCheckBox testWidget;
    // setAutoRepeat has no effect on toggle buttons
    testWidget.setAutoRepeat(true);
    QVERIFY(testWidget.isCheckable());
}

void tst_QCheckBox::toggle()
{
    QCheckBox testWidget;
    bool cur_state;
    cur_state = testWidget.isChecked();
    testWidget.toggle();
    QVERIFY(cur_state != testWidget.isChecked());

    cur_state = testWidget.isChecked();
    testWidget.toggle();
    QVERIFY(cur_state != testWidget.isChecked());

    cur_state = testWidget.isChecked();
    testWidget.toggle();
    QVERIFY(cur_state != testWidget.isChecked());
}

void tst_QCheckBox::pressed()
{
    QCheckBox testWidget;
    int press_count = 0;
    int release_count = 0;
    connect(&testWidget, &QCheckBox::pressed, this, [&]() { ++press_count; });
    connect(&testWidget, &QCheckBox::released, this, [&]() { ++release_count; });
    testWidget.setDown(false);
    QVERIFY(!testWidget.isChecked());

    BOBUIest::keyPress(&testWidget, BobUI::Key_Space);
    QVERIFY(press_count == 1);
    QVERIFY(release_count == 0);
    QVERIFY(!testWidget.isChecked());

    BOBUIest::keyRelease(&testWidget, BobUI::Key_Space);
    QVERIFY(press_count == 1);
    QVERIFY(release_count == 1);
    QVERIFY(testWidget.isChecked());
}

void tst_QCheckBox::toggled()
{
    QCheckBox testWidget;
    int toggle_count = 0;
    int click_count = 0;
    connect(&testWidget, &QCheckBox::toggled, this, [&]() { ++toggle_count; });
    connect(&testWidget, &QCheckBox::clicked, this, [&]() { ++click_count; });

    testWidget.toggle();
    QCOMPARE(toggle_count, 1);

    testWidget.toggle();
    QCOMPARE(toggle_count, 2);

    testWidget.toggle();
    QCOMPARE(toggle_count, 3);

    QCOMPARE(click_count, 0);
}

void tst_QCheckBox::checkStateChanged()
{
    QCheckBox testWidget;
    QCOMPARE(testWidget.checkState(), BobUI::Unchecked);

    BobUI::CheckState cur_state = BobUI::Unchecked;
    QSignalSpy checkStateChangedSpy(&testWidget, &QCheckBox::checkStateChanged);
#if BOBUI_DEPRECATED_SINCE(6, 9)
    BOBUI_IGNORE_DEPRECATIONS(
    QSignalSpy stateChangedSpy(&testWidget, &QCheckBox::stateChanged);
    )
#endif
    connect(&testWidget, &QCheckBox::checkStateChanged, this, [&](auto state) { cur_state = state; });
    testWidget.setChecked(true);
    BOBUIRY_COMPARE(checkStateChangedSpy.size(), 1);
#if BOBUI_DEPRECATED_SINCE(6, 9)
    QCOMPARE(stateChangedSpy.size(), 1);
#endif
    QCOMPARE(cur_state, BobUI::Checked);
    QCOMPARE(testWidget.checkState(), BobUI::Checked);

    testWidget.setChecked(false);
    BOBUIRY_COMPARE(checkStateChangedSpy.size(), 2);
#if BOBUI_DEPRECATED_SINCE(6, 9)
    QCOMPARE(stateChangedSpy.size(), 2);
#endif
    QCOMPARE(cur_state, BobUI::Unchecked);
    QCOMPARE(testWidget.checkState(), BobUI::Unchecked);

    testWidget.setCheckState(BobUI::PartiallyChecked);
    BOBUIRY_COMPARE(checkStateChangedSpy.size(), 3);
#if BOBUI_DEPRECATED_SINCE(6, 9)
    QCOMPARE(stateChangedSpy.size(), 3);
#endif
    QCOMPARE(cur_state, BobUI::PartiallyChecked);
    QCOMPARE(testWidget.checkState(), BobUI::PartiallyChecked);

    testWidget.setCheckState(BobUI::PartiallyChecked);
    QCoreApplication::processEvents();
    QCOMPARE(checkStateChangedSpy.size(), 3);
#if BOBUI_DEPRECATED_SINCE(6, 9)
    QCOMPARE(stateChangedSpy.size(), 3);
#endif
}

void tst_QCheckBox::isToggleButton()
{
    QCheckBox testWidget;
    QVERIFY(testWidget.isCheckable());
}

void tst_QCheckBox::foregroundRole()
{
    QCheckBox testWidget;
    QCOMPARE(testWidget.foregroundRole(), QPalette::WindowText);
}

void tst_QCheckBox::minimumSizeHint()
{
    QCheckBox testWidget;
    QCheckBox box(tr("CheckBox's sizeHint is the same as it's minimumSizeHint"));
    QCOMPARE(box.sizeHint(), box.minimumSizeHint());
}

BOBUIEST_MAIN(tst_QCheckBox)
#include "tst_qcheckbox.moc"
