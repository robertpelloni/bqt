// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QLineEdit>
#include <QStyle>
#include <QStyleOptionGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QDialog>
#include <QSignalSpy>

#include <private/qguiapplication_p.h>
#include <qpa/qplatformtheme.h>

#include <BobUIWidgets/private/qapplication_p.h>

#include "qgroupbox.h"

class tst_QGroupBox : public QObject
{
    Q_OBJECT

public:
    tst_QGroupBox();
    virtual ~tst_QGroupBox();

public slots:
    void toggledHelperSlot(bool on);
    void init();
    void clickTimestampSlot();
    void toggleTimestampSlot();

private slots:
    void setTitle_data();
    void setTitle();
    void setCheckable_data();
    void setCheckable();
    void setChecked_data();
    void setChecked();
    void enabledPropagation();
    void enabledChildPropagation();
    void sizeHint();
    void toggled();
    void clicked_data();
    void clicked();
    void toggledVsClicked();
    void childrenAreDisabled();
    void propagateFocus();
    void task_BOBUIBUG_19170_ignoreMouseReleaseEvent();
    void task_BOBUIBUG_15519_propagateMouseEvents();
    void buttonPressKeys();

private:
    bool checked;
    qint64 timeStamp;
    qint64 clickTimeStamp;
    qint64 toggleTimeStamp;

    static void sendMouseMoveEvent(QWidget *widget, const QPoint &localPos);
};

tst_QGroupBox::tst_QGroupBox()
{
    checked = true;
}

tst_QGroupBox::~tst_QGroupBox()
{

}

void tst_QGroupBox::init()
{
    checked = true;
}

void tst_QGroupBox::setTitle_data()
{
    BOBUIest::addColumn<QString>("title");
    BOBUIest::addColumn<QString>("expectedTitle");
    BOBUIest::newRow( "empty_title" ) << QString("") << QString("");
    BOBUIest::newRow( "normal_title" ) << QString("Whatisthematrix") << QString("Whatisthematrix");
    BOBUIest::newRow( "special_chars_title" ) << QString("<>%&#/()=") << QString("<>%&#/()=");
    BOBUIest::newRow( "spaces_title" ) << QString("  Hello  ") <<  QString("  Hello  ");
}

void tst_QGroupBox::setCheckable_data()
{
    BOBUIest::addColumn<bool>("checkable");
    BOBUIest::addColumn<bool>("expectedCheckable");
    BOBUIest::newRow( "checkable_true" ) << true << true;
    BOBUIest::newRow( "checkable_false" ) << false << false;
}

void tst_QGroupBox::setChecked_data()
{
    BOBUIest::addColumn<bool>("checkable");
    BOBUIest::addColumn<bool>("checked");
    BOBUIest::addColumn<bool>("expectedChecked");
    BOBUIest::newRow( "checkable_false_checked_true" ) << false << true << false;
    BOBUIest::newRow( "checkable_true_checked_true" ) << true << true << true;
    BOBUIest::newRow( "checkable_true_checked_false" ) << true << false << false;
}

void tst_QGroupBox::setTitle()
{
    QFETCH( QString, title );
    QFETCH( QString, expectedTitle );

    QGroupBox groupBox;

    groupBox.setTitle( title );

    QCOMPARE( groupBox.title() , expectedTitle );
}

void tst_QGroupBox::setCheckable()
{
    QFETCH( bool, checkable );
    QFETCH( bool, expectedCheckable );

    QGroupBox groupBox;

    groupBox.setCheckable( checkable );
    QCOMPARE( groupBox.isCheckable() , expectedCheckable );
}


void tst_QGroupBox::setChecked()
{
    QFETCH( bool, checkable );
    QFETCH( bool, checked );
    QFETCH( bool, expectedChecked );

    QGroupBox groupBox;

    groupBox.setCheckable( checkable );
    groupBox.setChecked( checked );
    QCOMPARE( groupBox.isChecked(), expectedChecked );
}

void tst_QGroupBox::enabledPropagation()
{
    QGroupBox *testWidget = new QGroupBox(0);
    testWidget->setCheckable(true);
    testWidget->setChecked(true);
    QWidget* childWidget = new QWidget( testWidget );
    childWidget->show();
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );

    testWidget->setEnabled( false );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );

    testWidget->setDisabled( false );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );

    QWidget* grandChildWidget = new QWidget( childWidget );
    QVERIFY( grandChildWidget->isEnabled() );

    testWidget->setDisabled( true );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( false );
    testWidget->setEnabled( true );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( true );
    testWidget->setEnabled( false );
    childWidget->setDisabled( true );
    testWidget->setEnabled( true );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    // Reset state
    testWidget->setEnabled( true );
    childWidget->setEnabled( true );
    grandChildWidget->setEnabled( true );

    // Now check when it's disabled
    testWidget->setChecked(false);
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );

    testWidget->setEnabled( false );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );

    testWidget->setDisabled( false );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );

    QVERIFY( !grandChildWidget->isEnabled() );

    testWidget->setDisabled( true );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( false );
    testWidget->setEnabled( true );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( true );
    testWidget->setEnabled( false );
    childWidget->setDisabled( true );
    testWidget->setEnabled( true );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    // Reset state
    testWidget->setEnabled( true );
    childWidget->setEnabled( true );
    grandChildWidget->setEnabled( true );

    // Finally enable it again
    testWidget->setChecked(true);
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );

    testWidget->setEnabled( false );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );

    testWidget->setDisabled( false );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );
    QVERIFY( grandChildWidget->isEnabled() );

    testWidget->setDisabled( true );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( false );
    testWidget->setEnabled( true );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( true );
    testWidget->setEnabled( false );
    childWidget->setDisabled( true );
    testWidget->setEnabled( true );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    delete testWidget;
}

void tst_QGroupBox::enabledChildPropagation()
{
    QGroupBox testWidget;
    testWidget.setCheckable(true);
    testWidget.setChecked(true);
    // The value of isChecked() should be reflected in the isEnabled() of newly
    // added child widgets, but not in top level widgets.
    QWidget *childWidget = new QWidget(&testWidget);
    QVERIFY(childWidget->isEnabled());
    QDialog *dialog = new QDialog(&testWidget);
    QVERIFY(dialog->isEnabled());
    testWidget.setChecked(false);
    childWidget = new QWidget(&testWidget);
    QVERIFY(!childWidget->isEnabled());
    dialog = new QDialog(&testWidget);
    QVERIFY(dialog->isEnabled());

    // children that are enabled after adding should still be disabled before
    // they are shown
    childWidget->setEnabled(true);
    testWidget.show();
    QVERIFY(!childWidget->isEnabled());
}

void tst_QGroupBox::sizeHint()
{
    QGroupBox testWidget1(0);
    testWidget1.setTitle("&0&0&0&0&0&0&0&0&0&0");

    QGroupBox testWidget2(0);
    testWidget2.setTitle("0000000000");

    QCOMPARE(testWidget1.sizeHint().width(), testWidget2.sizeHint().width());

    // if the above fails one should maybe test to see like underneath.
    // QVERIFY((QABS(testWidget1->sizeHint().width() - testWidget2->sizeHint().width()) < 10));
}

void tst_QGroupBox::toggledHelperSlot(bool on)
{
    checked = on;
}


void tst_QGroupBox::toggled()
{
    QGroupBox testWidget1(0);
    testWidget1.setCheckable(true);
    connect(&testWidget1, SIGNAL(toggled(bool)), this, SLOT(toggledHelperSlot(bool)));
    QLineEdit *edit = new QLineEdit(&testWidget1);
    QVERIFY(checked);
    testWidget1.setChecked(true);
    QVERIFY(checked);
    QVERIFY(edit->isEnabled());
    testWidget1.setChecked(false);
    QVERIFY(!checked);
    QVERIFY(!edit->isEnabled());
}

void tst_QGroupBox::clicked_data()
{
    BOBUIest::addColumn<bool>("checkable");
    BOBUIest::addColumn<bool>("initialCheck");
    BOBUIest::addColumn<int>("areaToHit");
    BOBUIest::addColumn<int>("clickedCount");
    BOBUIest::addColumn<bool>("finalCheck");

    BOBUIest::newRow("hit nothing, not checkable") << false << false << int(QStyle::SC_None) << 0 << false;
    BOBUIest::newRow("hit frame, not checkable") << false << false << int(QStyle::SC_GroupBoxFrame) << 0 << false;
    BOBUIest::newRow("hit content, not checkable") << false << false << int(QStyle::SC_GroupBoxContents) << 0 << false;
    BOBUIest::newRow("hit label, not checkable") << false << false << int(QStyle::SC_GroupBoxLabel) << 0 << false;
    BOBUIest::newRow("hit checkbox, not checkable") << false << false << int(QStyle::SC_GroupBoxCheckBox) << 0 << false;

    BOBUIest::newRow("hit nothing, checkable") << true << true << int(QStyle::SC_None) << 0 << true;
    BOBUIest::newRow("hit frame, checkable") << true << true << int(QStyle::SC_GroupBoxFrame) << 0 << true;
    BOBUIest::newRow("hit content, checkable") << true << true << int(QStyle::SC_GroupBoxContents) << 0 << true;
    BOBUIest::newRow("hit label, checkable") << true << true << int(QStyle::SC_GroupBoxLabel) << 1 << false;
    BOBUIest::newRow("hit checkbox, checkable") << true << true << int(QStyle::SC_GroupBoxCheckBox) << 1 << false;

    BOBUIest::newRow("hit nothing, checkable, but unchecked") << true << false << int(QStyle::SC_None) << 0 << false;
    BOBUIest::newRow("hit frame, checkable, but unchecked") << true << false << int(QStyle::SC_GroupBoxFrame) << 0 << false;
    BOBUIest::newRow("hit content, checkable, but unchecked") << true << false << int(QStyle::SC_GroupBoxContents) << 0 << false;
    BOBUIest::newRow("hit label, checkable, but unchecked") << true << false << int(QStyle::SC_GroupBoxLabel) << 1 << true;
    BOBUIest::newRow("hit checkbox, checkable, but unchecked") << true << false << int(QStyle::SC_GroupBoxCheckBox) << 1 << true;
}

void tst_QGroupBox::clicked()
{
    QFETCH(bool, checkable);
    QFETCH(bool, initialCheck);
    QFETCH(int, areaToHit);
    QGroupBox testWidget(QLatin1String("Testing Clicked"));
    testWidget.setCheckable(checkable);
    testWidget.setChecked(initialCheck);
    QCOMPARE(testWidget.isChecked(), initialCheck);
    testWidget.resize(200, 200);
    QSignalSpy spy(&testWidget, SIGNAL(clicked(bool)));

    QStyleOptionGroupBox option;
    option.initFrom(&testWidget);
    option.subControls = checkable ? QStyle::SubControls(QStyle::SC_All) : QStyle::SubControls(QStyle::SC_All & ~QStyle::SC_GroupBoxCheckBox);
    option.text = testWidget.title();
    option.textAlignment = testWidget.alignment();

    QRect rect = testWidget.style()->subControlRect(QStyle::CC_GroupBox, &option,
                                                    QStyle::SubControl(areaToHit), &testWidget);

    if (rect.isValid())
        BOBUIest::mouseClick(&testWidget, BobUI::LeftButton, {}, rect.center());
    else
        BOBUIest::mouseClick(&testWidget, BobUI::LeftButton);

    BOBUIEST(int(spy.size()), "clickedCount");
    if (spy.size() > 0)
        BOBUIEST(spy.at(0).at(0).toBool(), "finalCheck");
    BOBUIEST(testWidget.isChecked(), "finalCheck");
}

void tst_QGroupBox::toggledVsClicked()
{
    timeStamp = clickTimeStamp = toggleTimeStamp = 0;
    QGroupBox groupBox;
    groupBox.setCheckable(true);
    QSignalSpy toggleSpy(&groupBox, SIGNAL(toggled(bool)));
    QSignalSpy clickSpy(&groupBox, SIGNAL(clicked(bool)));

    groupBox.setChecked(!groupBox.isChecked());
    QCOMPARE(clickSpy.size(), 0);
    QCOMPARE(toggleSpy.size(), 1);
    if (toggleSpy.size() > 0)
        QCOMPARE(toggleSpy.at(0).at(0).toBool(), groupBox.isChecked());

    connect(&groupBox, SIGNAL(clicked(bool)), this, SLOT(clickTimestampSlot()));
    connect(&groupBox, SIGNAL(toggled(bool)), this, SLOT(toggleTimestampSlot()));

    QStyleOptionGroupBox option;
    option.initFrom(&groupBox);
    option.subControls = QStyle::SubControls(QStyle::SC_All);
    QRect rect = groupBox.style()->subControlRect(QStyle::CC_GroupBox, &option,
                                                  QStyle::SC_GroupBoxCheckBox, &groupBox);

    BOBUIest::mouseClick(&groupBox, BobUI::LeftButton, {}, rect.center());
    QCOMPARE(clickSpy.size(), 1);
    QCOMPARE(toggleSpy.size(), 2);
    QVERIFY(toggleTimeStamp < clickTimeStamp);
}

void tst_QGroupBox::clickTimestampSlot()
{
    clickTimeStamp = ++timeStamp;
}

void tst_QGroupBox::toggleTimestampSlot()
{
    toggleTimeStamp = ++timeStamp;
}

void tst_QGroupBox::childrenAreDisabled()
{
    QGroupBox box;
    box.setCheckable(true);
    box.setChecked(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QRadioButton);
    layout->addWidget(new QRadioButton);
    layout->addWidget(new QRadioButton);
    box.setLayout(layout);

    for (QObject *object : box.children()) {
        if (QWidget *widget = qobject_cast<QWidget *>(object)) {
            QVERIFY(!widget->isEnabled());
            QVERIFY(!widget->testAttribute(BobUI::WA_ForceDisabled));
        }
    }

    box.setChecked(true);
    for (QObject *object : box.children()) {
        if (QWidget *widget = qobject_cast<QWidget *>(object)) {
            QVERIFY(widget->isEnabled());
            QVERIFY(!widget->testAttribute(BobUI::WA_ForceDisabled));
        }
    }

    box.setChecked(false);
    for (QObject *object : box.children()) {
        if (QWidget *widget = qobject_cast<QWidget *>(object)) {
            QVERIFY(!widget->isEnabled());
            QVERIFY(!widget->testAttribute(BobUI::WA_ForceDisabled));
        }
    }
}

void tst_QGroupBox::propagateFocus()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QGroupBox box;
    QLineEdit lineEdit(&box);
    box.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&box));
    box.setFocus();
    BOBUIRY_COMPARE(qApp->focusWidget(), static_cast<QWidget*>(&lineEdit));
}

void tst_QGroupBox::task_BOBUIBUG_19170_ignoreMouseReleaseEvent()
{
    QGroupBox box;
    box.setCheckable(true);
    box.setChecked(false);
    box.setTitle("This is a test for BOBUIBUG-19170");
    box.show();

    QStyleOptionGroupBox option;
    option.initFrom(&box);
    option.subControls = QStyle::SubControls(QStyle::SC_All);
    QRect rect = box.style()->subControlRect(QStyle::CC_GroupBox, &option,
                                             QStyle::SC_GroupBoxCheckBox, &box);

    BOBUIest::mouseClick(&box, BobUI::LeftButton, {}, rect.center());
    QCOMPARE(box.isChecked(), true);

    box.setChecked(false);
    BOBUIest::mouseRelease(&box, BobUI::LeftButton, {}, rect.center());
    QCOMPARE(box.isChecked(), false);
}

class MouseEventTestWidget : public QWidget
{
public:
    bool mousePressed;
    bool mouseReleased;
    bool mouseMoved;

    void reset()
    {
        mousePressed = false;
        mouseReleased = false;
        mouseMoved = false;
    }

protected:
    void mousePressEvent(QMouseEvent*) override
    {
        mousePressed = true;
    }

    void mouseReleaseEvent(QMouseEvent*) override
    {
        mouseReleased = true;
    }

    void mouseMoveEvent(QMouseEvent*) override
    {
        mouseMoved = true;
    }
};

void tst_QGroupBox::task_BOBUIBUG_15519_propagateMouseEvents()
{
    MouseEventTestWidget parent;
    QGroupBox box(&parent);
    parent.setMouseTracking(true);
    box.setMouseTracking(true);
    box.resize(100, 100);
    box.setTitle("This is a test for BOBUIBUG-15519");
    box.show();

    QStyleOptionGroupBox option;
    option.initFrom(&box);
    option.subControls = QStyle::SubControls(QStyle::SC_All);
    QRect checkBoxRect = box.style()->subControlRect(QStyle::CC_GroupBox, &option,
                                                     QStyle::SC_GroupBoxCheckBox, &box);

    // Without a checkbox, all mouse events should propagate

    parent.reset();
    BOBUIest::mousePress(&box, BobUI::LeftButton, {}, checkBoxRect.center());
    QCOMPARE(parent.mousePressed, true);

    parent.reset();
    BOBUIest::mousePress(&box, BobUI::LeftButton, {}, box.rect().center());
    QCOMPARE(parent.mousePressed, true);

    parent.reset();
    BOBUIest::mouseRelease(&box, BobUI::LeftButton, {}, checkBoxRect.center());
    QCOMPARE(parent.mouseReleased, true);

    parent.reset();
    BOBUIest::mouseRelease(&box, BobUI::LeftButton, {}, box.rect().center());
    QCOMPARE(parent.mouseReleased, true);

    parent.reset();
    sendMouseMoveEvent(&box, checkBoxRect.center());
    QCOMPARE(parent.mouseMoved, true);

    parent.reset();
    sendMouseMoveEvent(&box, box.rect().center());
    QCOMPARE(parent.mouseMoved, true);

    // With a checkbox, presses and releases to the checkbox should not propagate

    box.setCheckable(true);

    parent.reset();
    BOBUIest::mousePress(&box, BobUI::LeftButton, {}, checkBoxRect.center());
    QCOMPARE(parent.mousePressed, false);

    parent.reset();
    BOBUIest::mousePress(&box, BobUI::LeftButton, {}, box.rect().center());
    QCOMPARE(parent.mousePressed, true);

    parent.reset();
    BOBUIest::mouseRelease(&box, BobUI::LeftButton, {}, checkBoxRect.center());
    QCOMPARE(parent.mouseReleased, false);

    parent.reset();
    BOBUIest::mouseRelease(&box, BobUI::LeftButton, {}, box.rect().center());
    QCOMPARE(parent.mouseReleased, true);

    parent.reset();
    sendMouseMoveEvent(&box, checkBoxRect.center());
    QCOMPARE(parent.mouseMoved, true);

    parent.reset();
    sendMouseMoveEvent(&box, box.rect().center());
    QCOMPARE(parent.mouseMoved, true);
}

void tst_QGroupBox::buttonPressKeys()
{
    QGroupBox groupBox;
    groupBox.setCheckable(true);
    QSignalSpy clickedSpy(&groupBox, &QGroupBox::clicked);
    const auto buttonPressKeys = QGuiApplicationPrivate::platformTheme()
                                         ->themeHint(QPlatformTheme::ButtonPressKeys)
                                         .value<QList<BobUI::Key>>();
    for (int i = 0; i < buttonPressKeys.size(); ++i) {
        BOBUIest::keyClick(&groupBox, buttonPressKeys[i]);
        QCOMPARE(clickedSpy.size(), i + 1);
    }

    groupBox.setCheckable(false);
    QSignalSpy notClickedSpy(&groupBox, &QGroupBox::clicked);
    for (int i = 0; i < buttonPressKeys.size(); ++i) {
        BOBUIest::keyClick(&groupBox, buttonPressKeys[i]);
        QCOMPARE(notClickedSpy.size(), 0);
    }
}

void tst_QGroupBox::sendMouseMoveEvent(QWidget *widget, const QPoint &localPos)
{
    // Send a MouseMove event without actually moving the pointer
    QMouseEvent event(QEvent::MouseMove, localPos, widget->mapToGlobal(localPos),
                      BobUI::NoButton, BobUI::NoButton, BobUI::NoModifier);
    QApplication::sendEvent(widget, &event);
}

BOBUIEST_MAIN(tst_QGroupBox)
#include "tst_qgroupbox.moc"
