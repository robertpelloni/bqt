// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BOBUIest>
#include <QSignalSpy>
#include <BobUIWidgets/QPushButton>
#include <BobUIWidgets/QStyle>
#include <BobUIWidgets/QLayout>
#include <BobUIWidgets/QDialog>
#include <BobUIWidgets/QLineEdit>
#include <BobUIGui/QAction>
#include <BobUIGui/QStyleHints>
#include <qdialogbuttonbox.h>
#include <BobUIWidgets/private/qdialogbuttonbox_p.h>
#include <BobUIWidgets/private/qabstractbutton_p.h>
#include <limits.h>

Q_DECLARE_METATYPE(QDialogButtonBox::ButtonRole)
Q_DECLARE_METATYPE(QDialogButtonBox::StandardButton)
Q_DECLARE_METATYPE(QDialogButtonBox::StandardButtons)

class tst_QDialogButtonBox : public QObject
{
    Q_OBJECT
public:
    tst_QDialogButtonBox();
    ~tst_QDialogButtonBox();


public slots:
    void buttonClicked1(QAbstractButton *);
    void acceptClicked();
    void rejectClicked();
    void helpRequestedClicked();

private slots:
    void standardButtons();
    void testConstructor1();
    void testConstructor2();
    void testConstructor2_data();
    void testConstructor3();
    void testConstructor3_data();
    void testConstructor4();
    void testConstructor4_data();
    void setOrientation_data();
    void setOrientation();
    void addButton1_data();
    void addButton1();
    void addButton2_data();
    void addButton2();
    void addButton3_data();
    void addButton3();
    void clear_data();
    void clear();
    void removeButton_data();
    void removeButton();
#ifdef BOBUI_BUILD_INTERNAL
    void hideAndShowButton();
#endif
    void hideAndShowStandardButton();
    void buttonRole_data();
    void buttonRole();
    void setStandardButtons_data();
    void setStandardButtons();
    void layoutReuse();


    // Skip these tests, buttons is used in every test thus far.
//    void buttons_data();
//    void buttons();

    void testDelete();
    void testSignalEmissionAfterDelete_BOBUIBUG_45835();
    void testRemove();
    void testMultipleAdd();
    void testStandardButtonMapping_data();
    void testStandardButtonMapping();
    void testSignals_data();
    void testSignals();
    void testSignalOrder();
    void testDefaultButton_data();
    void testDefaultButton();

    void task191642_default();
    void testDeletedStandardButton();
    void automaticDefaultButton();
    void initialFocus_data();
    void initialFocus();

private:
    qint64 timeStamp;
    qint64 buttonClicked1TimeStamp;
    qint64 acceptTimeStamp;
    qint64 rejectTimeStamp;
    qint64 helpRequestedTimeStamp;
};

tst_QDialogButtonBox::tst_QDialogButtonBox()
{
    qRegisterMetaType<QAbstractButton *>();
}

tst_QDialogButtonBox::~tst_QDialogButtonBox()
{
}

void tst_QDialogButtonBox::testConstructor1()
{
    QDialogButtonBox buttonbox;
    QCOMPARE(buttonbox.orientation(), BobUI::Horizontal);

    QCOMPARE(buttonbox.buttons().size(), 0);
}

void tst_QDialogButtonBox::layoutReuse()
{
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok);
    QPointer<QLayout> layout = box->layout();
    box->setCenterButtons(!box->centerButtons());
    QCOMPARE(layout.data(), box->layout());
    QEvent event(QEvent::StyleChange);
    QApplication::sendEvent(box, &event);
    QCOMPARE(layout.data(), box->layout());
    box->setOrientation(box->orientation() == BobUI::Horizontal ? BobUI::Vertical : BobUI::Horizontal);
    QVERIFY(layout.isNull());
    QVERIFY(layout != box->layout());
    delete box;
}

void tst_QDialogButtonBox::testConstructor2_data()
{
    BOBUIest::addColumn<int>("orientation");

    BOBUIest::newRow("horizontal") << int(BobUI::Horizontal);
    BOBUIest::newRow("vertical") << int(BobUI::Vertical);
}

void tst_QDialogButtonBox::testConstructor2()
{
    QFETCH(int, orientation);
    BobUI::Orientation orient = BobUI::Orientation(orientation);
    QDialogButtonBox buttonBox(orient);

    QCOMPARE(buttonBox.orientation(), orient);
    QCOMPARE(buttonBox.buttons().size(), 0);
}

void tst_QDialogButtonBox::testConstructor3_data()
{
    BOBUIest::addColumn<int>("orientation");
    BOBUIest::addColumn<QDialogButtonBox::StandardButtons>("buttons");
    BOBUIest::addColumn<int>("buttonCount");

    BOBUIest::newRow("nothing") << int(BobUI::Horizontal) << QDialogButtonBox::StandardButtons{} << 0;
    BOBUIest::newRow("only 1") << int(BobUI::Horizontal) << QDialogButtonBox::StandardButtons(QDialogButtonBox::Ok) << 1;
    BOBUIest::newRow("only 1.. twice") << int(BobUI::Horizontal)
                        << (QDialogButtonBox::Ok | QDialogButtonBox::Ok)
                        << 1;
    BOBUIest::newRow("only 2") << int(BobUI::Horizontal)
            << (QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
            << 2;
    BOBUIest::newRow("two different things") << int(BobUI::Horizontal)
            << (QDialogButtonBox::Save | QDialogButtonBox::Close)
            << 2;
    BOBUIest::newRow("three") << int(BobUI::Horizontal)
            << (QDialogButtonBox::Ok
                    | QDialogButtonBox::Cancel
                    | QDialogButtonBox::Help)
            << 3;
    BOBUIest::newRow("everything") << int(BobUI::Vertical)
            << (QDialogButtonBox::StandardButtons)UINT_MAX
            << 18;
}

void tst_QDialogButtonBox::testConstructor3()
{
    QFETCH(int, orientation);
    QFETCH(QDialogButtonBox::StandardButtons, buttons);

    QDialogButtonBox buttonBox(buttons, (BobUI::Orientation)orientation);
    QCOMPARE(int(buttonBox.orientation()), orientation);
    BOBUIEST(int(buttonBox.buttons().size()), "buttonCount");
}

void tst_QDialogButtonBox::testConstructor4_data()
{
    BOBUIest::addColumn<QDialogButtonBox::StandardButtons>("buttons");
    BOBUIest::addColumn<int>("buttonCount");

    BOBUIest::newRow("nothing") << QDialogButtonBox::StandardButtons{} << 0;
    BOBUIest::newRow("only 1") << QDialogButtonBox::StandardButtons(QDialogButtonBox::Ok) << 1;
    BOBUIest::newRow("only 1.. twice")
                        << (QDialogButtonBox::Ok | QDialogButtonBox::Ok)
                        << 1;
    BOBUIest::newRow("only 2")
            << (QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
            << 2;
    BOBUIest::newRow("two different things")
            << (QDialogButtonBox::Save | QDialogButtonBox::Close)
            << 2;
    BOBUIest::newRow("three")
            << (QDialogButtonBox::Ok
                    | QDialogButtonBox::Cancel
                    | QDialogButtonBox::Help)
            << 3;
    BOBUIest::newRow("everything")
            << (QDialogButtonBox::StandardButtons)UINT_MAX
            << 18;
}

void tst_QDialogButtonBox::testConstructor4()
{
    QFETCH(QDialogButtonBox::StandardButtons, buttons);

    QDialogButtonBox buttonBox(buttons);
    QCOMPARE(buttonBox.orientation(), BobUI::Horizontal);
    BOBUIEST(int(buttonBox.buttons().size()), "buttonCount");
}

void tst_QDialogButtonBox::setOrientation_data()
{
    BOBUIest::addColumn<int>("orientation");

    BOBUIest::newRow("Horizontal") << int(BobUI::Horizontal);
    BOBUIest::newRow("Vertical") << int(BobUI::Vertical);
}

void tst_QDialogButtonBox::setOrientation()
{
    QFETCH(int, orientation);
    QDialogButtonBox buttonBox;
    QCOMPARE(int(buttonBox.orientation()), int(BobUI::Horizontal));

    buttonBox.setOrientation(BobUI::Orientation(orientation));
    QCOMPARE(int(buttonBox.orientation()), orientation);
}

/*
void tst_QDialogButtonBox::setLayoutPolicy_data()
{
    BOBUIest::addColumn<int>("layoutPolicy");

    BOBUIest::newRow("win") << int(QDialogButtonBox::WinLayout);
    BOBUIest::newRow("mac") << int(QDialogButtonBox::MacLayout);
    BOBUIest::newRow("kde") << int(QDialogButtonBox::KdeLayout);
    BOBUIest::newRow("gnome") << int(QDialogButtonBox::GnomeLayout);

}

void tst_QDialogButtonBox::setLayoutPolicy()
{
    QFETCH(int, layoutPolicy);

    QDialogButtonBox buttonBox;
    QCOMPARE(int(buttonBox.layoutPolicy()),
             int(buttonBox.style()->styleHint(QStyle::SH_DialogButtonLayout)));
    buttonBox.setLayoutPolicy(QDialogButtonBox::ButtonLayout(layoutPolicy));
    QCOMPARE(int(buttonBox.layoutPolicy()), layoutPolicy);
}
*/

void tst_QDialogButtonBox::addButton1_data()
{
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("role");
    BOBUIest::addColumn<int>("totalCount");

    BOBUIest::newRow("InvalidRole") << QDialogButtonBox::InvalidRole << 0;
    BOBUIest::newRow("AcceptRole") << QDialogButtonBox::AcceptRole << 1;
    BOBUIest::newRow("RejectRole") << QDialogButtonBox::RejectRole << 1;
    BOBUIest::newRow("DestructiveRole") << QDialogButtonBox::DestructiveRole << 1;
    BOBUIest::newRow("ActionRole") << QDialogButtonBox::ActionRole << 1;
    BOBUIest::newRow("HelpRole") << QDialogButtonBox::HelpRole << 1;
    BOBUIest::newRow("WackyValue") << (QDialogButtonBox::ButtonRole)-1 << 0;
}

void tst_QDialogButtonBox::addButton1()
{
    QFETCH(QDialogButtonBox::ButtonRole, role);
    QDialogButtonBox buttonBox;
    QCOMPARE(buttonBox.buttons().size(), 0);
    QPushButton *button = new QPushButton();
    buttonBox.addButton(button, role);
    BOBUIEST(int(buttonBox.buttons().size()), "totalCount");
    QList<QAbstractButton *> children = buttonBox.findChildren<QAbstractButton *>();
    BOBUIEST(int(children.size()), "totalCount");
    delete button;
}

void tst_QDialogButtonBox::addButton2_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("role");
    BOBUIest::addColumn<int>("totalCount");
    BOBUIest::newRow("InvalidRole") << QString("foo") << QDialogButtonBox::InvalidRole << 0;
    BOBUIest::newRow("AcceptRole") << QString("foo") << QDialogButtonBox::AcceptRole << 1;
    BOBUIest::newRow("RejectRole") << QString("foo") << QDialogButtonBox::RejectRole << 1;
    BOBUIest::newRow("DestructiveRole") << QString("foo") << QDialogButtonBox::DestructiveRole << 1;
    BOBUIest::newRow("ActionRole") << QString("foo") << QDialogButtonBox::ActionRole << 1;
    BOBUIest::newRow("HelpRole") << QString("foo") << QDialogButtonBox::HelpRole << 1;
    BOBUIest::newRow("WackyValue") << QString("foo") << (QDialogButtonBox::ButtonRole)-1 << 0;
}

void tst_QDialogButtonBox::addButton2()
{
    QFETCH(QString, text);
    QFETCH(QDialogButtonBox::ButtonRole, role);
    QDialogButtonBox buttonBox;
    QCOMPARE(buttonBox.buttons().size(), 0);
    buttonBox.addButton(text, role);
    BOBUIEST(int(buttonBox.buttons().size()), "totalCount");
    QList<QAbstractButton *> children = buttonBox.findChildren<QAbstractButton *>();
    BOBUIEST(int(children.size()), "totalCount");
}

void tst_QDialogButtonBox::addButton3_data()
{
    BOBUIest::addColumn<QDialogButtonBox::StandardButton>("button");
    BOBUIest::addColumn<int>("totalCount");
    BOBUIest::newRow("Ok") << QDialogButtonBox::Ok << 1;
    BOBUIest::newRow("Open") << QDialogButtonBox::Open << 1;
    BOBUIest::newRow("Save") << QDialogButtonBox::Save << 1;
    BOBUIest::newRow("Cancel") << QDialogButtonBox::Cancel << 1;
    BOBUIest::newRow("Close") << QDialogButtonBox::Close << 1;
    BOBUIest::newRow("Discard") << QDialogButtonBox::Discard << 1;
    BOBUIest::newRow("Apply") << QDialogButtonBox::Apply << 1;
    BOBUIest::newRow("Reset") << QDialogButtonBox::Reset << 1;
    BOBUIest::newRow("Help") << QDialogButtonBox::Help << 1;
    BOBUIest::newRow("noButton") << (QDialogButtonBox::StandardButton)0 << 0;
}

void tst_QDialogButtonBox::addButton3()
{
    QFETCH(QDialogButtonBox::StandardButton, button);
    QDialogButtonBox buttonBox;
    QCOMPARE(buttonBox.buttons().size(), 0);
    buttonBox.addButton(button);
    BOBUIEST(int(buttonBox.buttons().size()), "totalCount");
    QList<QAbstractButton *> children = buttonBox.findChildren<QAbstractButton *>();
    BOBUIEST(int(children.size()), "totalCount");
}

void tst_QDialogButtonBox::clear_data()
{
    BOBUIest::addColumn<int>("rolesToAdd");

    BOBUIest::newRow("nothing") << 0;
    BOBUIest::newRow("one") << 1;
    BOBUIest::newRow("all") << int(QDialogButtonBox::NRoles);
}

void tst_QDialogButtonBox::clear()
{
    QFETCH(int, rolesToAdd);

    QDialogButtonBox buttonBox;
    for (int i = 1; i < rolesToAdd; ++i)
        buttonBox.addButton("Happy", QDialogButtonBox::ButtonRole(i));
    buttonBox.clear();
    QCOMPARE(buttonBox.buttons().size(), 0);
    QList<QAbstractButton *> children = buttonBox.findChildren<QAbstractButton *>();
    QCOMPARE(children.size(), 0);
}

void tst_QDialogButtonBox::removeButton_data()
{
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("roleToAdd");
    BOBUIest::addColumn<int>("expectedCount");
    BOBUIest::newRow("no button added") << QDialogButtonBox::InvalidRole << 0;
    BOBUIest::newRow("a button") << QDialogButtonBox::AcceptRole << 1;
}

void tst_QDialogButtonBox::removeButton()
{
    QFETCH(QDialogButtonBox::ButtonRole, roleToAdd);

    QDialogButtonBox buttonBox;
    QCOMPARE(buttonBox.buttons().size(), 0);
    QPushButton *button = new QPushButton("RemoveButton test");
    buttonBox.addButton(button, roleToAdd);
    BOBUIEST(int(buttonBox.buttons().size()), "expectedCount");

    buttonBox.removeButton(button);
    QCOMPARE(buttonBox.buttons().size(), 0);
    delete button;
}

#ifdef BOBUI_BUILD_INTERNAL
void tst_QDialogButtonBox::hideAndShowButton()
{
    if (QGuiApplication::styleHints()->tabFocusBehavior() != BobUI::TabFocusAllControls)
        QSKIP("Test requires BobUI::TabFocusAllControls tab focus behavior");

    QWidget widget;
    QDialogButtonBox buttonBox;
    QDialogButtonBoxPrivate *d = static_cast<QDialogButtonBoxPrivate *>(QObjectPrivate::get(&buttonBox));
    auto *apply = buttonBox.addButton( "Apply", QDialogButtonBox::ApplyRole );
    auto *accept = buttonBox.addButton( "Accept", QDialogButtonBox::AcceptRole );
    buttonBox.addButton( "Reject", QDialogButtonBox::RejectRole );
    auto *layout = new QHBoxLayout(&widget);
    layout->addWidget(&buttonBox);

    // apply button shows first on macOS. accept button on all other OSes.
    QAbstractButton *hideButton;
#ifdef Q_OS_MACOS
    hideButton = apply;
    Q_UNUSED(accept);
#else
    hideButton = accept;
    Q_UNUSED(apply);
#endif

    hideButton->hide();
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));
    BOBUIRY_VERIFY(buttonBox.focusWidget()); // BOBUIBUG-114377: Without fixing, focusWidget() == nullptr
    QCOMPARE(d->visibleButtons().count(), 2);
    QCOMPARE(d->hiddenButtons.count(), 1);
    QVERIFY(d->hiddenButtons.contains(hideButton));
    QCOMPARE(buttonBox.buttons().count(), 3);
    QSignalSpy spy(qApp, &QApplication::focusChanged);
    BOBUIest::sendKeyEvent(BOBUIest::KeyAction::Click, &buttonBox, BobUI::Key_Tab, QString(), BobUI::KeyboardModifiers());
    QCOMPARE(spy.count(), 1); // BOBUIBUG-114377: Without fixing, tabbing wouldn't work.
    hideButton->show();
    QCOMPARE_GE(spy.count(), 1);
    BOBUIRY_COMPARE(QApplication::focusWidget(), hideButton);
    QCOMPARE(d->visibleButtons().count(), 3);
    QCOMPARE(d->hiddenButtons.count(), 0);
    QCOMPARE(buttonBox.buttons().count(), 3);
    spy.clear();
    BOBUIest::sendKeyEvent(BOBUIest::KeyAction::Click, &buttonBox, BobUI::Key_Backtab, QString(), BobUI::KeyboardModifiers());
    QCOMPARE(spy.count(), 1);
}
#endif

void tst_QDialogButtonBox::hideAndShowStandardButton()
{
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&buttonBox));
    auto *button = buttonBox.button(QDialogButtonBox::Cancel);
    QVERIFY(button);
    button->hide();
    QVERIFY(BOBUIest::qWaitFor([button](){ return !button->isVisible(); }));
    QCOMPARE(button, buttonBox.button(QDialogButtonBox::Cancel));
    button->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(button));
    QCOMPARE(button, buttonBox.button(QDialogButtonBox::Cancel));
}

void tst_QDialogButtonBox::testDelete()
{
    QDialogButtonBox buttonBox;
    QCOMPARE(buttonBox.buttons().size(), 0);

    QPushButton *deleteMe = new QPushButton("Happy");
    buttonBox.addButton(deleteMe, QDialogButtonBox::HelpRole);
    QCOMPARE(buttonBox.buttons().size(), 1);
    QList<QAbstractButton *> children = buttonBox.findChildren<QAbstractButton *>();
    QCOMPARE(children.size(), 1);

    delete deleteMe;
    children = buttonBox.findChildren<QAbstractButton *>();
    QCOMPARE(children.size(), 0);
    QCOMPARE(buttonBox.buttons().size(), 0);
}

class ObjectDeleter : public QObject
{
    Q_OBJECT
public slots:
    void deleteButton(QAbstractButton *button)
    {
        delete button;
    }

    void deleteSender()
    {
        delete sender();
    }
};

void tst_QDialogButtonBox::testSignalEmissionAfterDelete_BOBUIBUG_45835()
{
    {
        QDialogButtonBox buttonBox;
        QCOMPARE(buttonBox.buttons().size(), 0);

        QSignalSpy buttonClickedSpy(&buttonBox, &QDialogButtonBox::clicked);
        QVERIFY(buttonClickedSpy.isValid());

        QSignalSpy buttonBoxAcceptedSpy(&buttonBox, &QDialogButtonBox::accepted);
        QVERIFY(buttonBoxAcceptedSpy.isValid());

        QPushButton *button = buttonBox.addButton("Test", QDialogButtonBox::AcceptRole);
        QCOMPARE(buttonBox.buttons().size(), 1);

        ObjectDeleter objectDeleter;
        connect(&buttonBox, &QDialogButtonBox::clicked, &objectDeleter, &ObjectDeleter::deleteButton);

        button->click();

        QCOMPARE(buttonBox.buttons().size(), 0);
        QCOMPARE(buttonClickedSpy.size(), 1);
        QCOMPARE(buttonBoxAcceptedSpy.size(), 1);
    }

    {
        QPointer<QDialogButtonBox> buttonBox(new QDialogButtonBox);
        QCOMPARE(buttonBox->buttons().size(), 0);

        QSignalSpy buttonClickedSpy(buttonBox.data(), &QDialogButtonBox::clicked);
        QVERIFY(buttonClickedSpy.isValid());

        QSignalSpy buttonBoxAcceptedSpy(buttonBox.data(), &QDialogButtonBox::accepted);
        QVERIFY(buttonBoxAcceptedSpy.isValid());

        QPushButton *button = buttonBox->addButton("Test", QDialogButtonBox::AcceptRole);
        QCOMPARE(buttonBox->buttons().size(), 1);

        ObjectDeleter objectDeleter;
        connect(buttonBox.data(), &QDialogButtonBox::clicked, &objectDeleter, &ObjectDeleter::deleteSender);

        button->click();

        QVERIFY(buttonBox.isNull());
        QCOMPARE(buttonClickedSpy.size(), 1);
        QCOMPARE(buttonBoxAcceptedSpy.size(), 0);
    }
}

void tst_QDialogButtonBox::testMultipleAdd()
{
    // Add a button into the thing multiple times.
    QDialogButtonBox buttonBox;
    QCOMPARE(buttonBox.buttons().size(), 0);

    QPushButton *button = new QPushButton("Foo away");
    buttonBox.addButton(button, QDialogButtonBox::AcceptRole);
    QCOMPARE(buttonBox.buttons().size(), 1);
    QCOMPARE(buttonBox.buttonRole(button), QDialogButtonBox::AcceptRole);
    buttonBox.addButton(button, QDialogButtonBox::AcceptRole);
    QCOMPARE(buttonBox.buttons().size(), 1);
    QCOMPARE(buttonBox.buttonRole(button), QDialogButtonBox::AcceptRole);

    // Add it again with a different role
    buttonBox.addButton(button, QDialogButtonBox::RejectRole);
    QCOMPARE(buttonBox.buttons().size(), 1);
    QCOMPARE(buttonBox.buttonRole(button), QDialogButtonBox::RejectRole);

    // Add it as an "invalid" role
    buttonBox.addButton(button, QDialogButtonBox::InvalidRole);
    QCOMPARE(buttonBox.buttons().size(), 1);
    QCOMPARE(buttonBox.buttonRole(button), QDialogButtonBox::RejectRole);
}

void tst_QDialogButtonBox::buttonRole_data()
{
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("roleToAdd");
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("expectedRole");

    BOBUIest::newRow("AcceptRole stuff") << QDialogButtonBox::AcceptRole
                                      << QDialogButtonBox::AcceptRole;
    BOBUIest::newRow("Nothing") << QDialogButtonBox::InvalidRole << QDialogButtonBox::InvalidRole;
    BOBUIest::newRow("bad stuff") << (QDialogButtonBox::ButtonRole)-1 << QDialogButtonBox::InvalidRole;
}

void tst_QDialogButtonBox::buttonRole()
{
    QFETCH(QDialogButtonBox::ButtonRole, roleToAdd);
    QDialogButtonBox buttonBox;
    QAbstractButton *button = buttonBox.addButton("Here's a button", roleToAdd);
    BOBUIEST(buttonBox.buttonRole(button), "expectedRole");
}

void tst_QDialogButtonBox::testStandardButtonMapping_data()
{
    BOBUIest::addColumn<QDialogButtonBox::StandardButton>("button");
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("expectedRole");
    BOBUIest::addColumn<QString>("expectedText");

    int layoutPolicy = qApp->style()->styleHint(QStyle::SH_DialogButtonLayout);

    BOBUIest::newRow("QDialogButtonBox::Ok") << QDialogButtonBox::Ok
                                          << QDialogButtonBox::AcceptRole
                                          << QDialogButtonBox::tr("OK");
    BOBUIest::newRow("QDialogButtonBox::Open") << QDialogButtonBox::Open
                                            << QDialogButtonBox::AcceptRole
                                            << QDialogButtonBox::tr("Open");
    BOBUIest::newRow("QDialogButtonBox::Save") << QDialogButtonBox::Save
                                            << QDialogButtonBox::AcceptRole
                                            << QDialogButtonBox::tr("Save");
    BOBUIest::newRow("QDialogButtonBox::Cancel") << QDialogButtonBox::Cancel
                                              << QDialogButtonBox::RejectRole
                                              << QDialogButtonBox::tr("Cancel");
    BOBUIest::newRow("QDialogButtonBox::Close") << QDialogButtonBox::Close
                                             << QDialogButtonBox::RejectRole
                                             << QDialogButtonBox::tr("Close");
    if (layoutPolicy == QDialogButtonBox::MacLayout) {
        BOBUIest::newRow("QDialogButtonBox::Discard") << QDialogButtonBox::Discard
                                                    << QDialogButtonBox::DestructiveRole
                                                    << QDialogButtonBox::tr("Don't Save");
    } else if (layoutPolicy == QDialogButtonBox::GnomeLayout) {
        BOBUIest::newRow("QDialogButtonBox::Discard")
            << QDialogButtonBox::Discard
            << QDialogButtonBox::DestructiveRole
            << QDialogButtonBox::tr("Close without Saving");
    } else {
        BOBUIest::newRow("QDialogButtonBox::Discard") << QDialogButtonBox::Discard
                                                    << QDialogButtonBox::DestructiveRole
                                                    << QDialogButtonBox::tr("Discard");
    }
    BOBUIest::newRow("QDialogButtonBox::Apply") << QDialogButtonBox::Apply
                                             << QDialogButtonBox::ApplyRole
                                             << QDialogButtonBox::tr("Apply");
    BOBUIest::newRow("QDialogButtonBox::Reset") << QDialogButtonBox::Reset
                                             << QDialogButtonBox::ResetRole
                                             << QDialogButtonBox::tr("Reset");
    BOBUIest::newRow("QDialogButtonBox::Help") << QDialogButtonBox::Help
                                            << QDialogButtonBox::HelpRole
                                            << QDialogButtonBox::tr("Help");
}

void tst_QDialogButtonBox::testStandardButtonMapping()
{
    QFETCH(QDialogButtonBox::StandardButton, button);
    QDialogButtonBox buttonBox;

    QAbstractButton *theButton = buttonBox.addButton(button);
    BOBUIEST(buttonBox.buttonRole(theButton), "expectedRole");
    QString textWithoutMnemonic = theButton->text().remove("&");
    BOBUIEST(textWithoutMnemonic, "expectedText");
}

void tst_QDialogButtonBox::testSignals_data()
{
    BOBUIest::addColumn<QDialogButtonBox::ButtonRole>("buttonToClick");
    BOBUIest::addColumn<int>("clicked2Count");
    BOBUIest::addColumn<int>("acceptCount");
    BOBUIest::addColumn<int>("rejectCount");
    BOBUIest::addColumn<int>("helpRequestedCount");

    BOBUIest::newRow("nothing") << QDialogButtonBox::InvalidRole << 0 << 0 << 0 << 0;
    BOBUIest::newRow("accept") << QDialogButtonBox::AcceptRole << 1 << 1 << 0 << 0;
    BOBUIest::newRow("reject") << QDialogButtonBox::RejectRole << 1 << 0 << 1 << 0;
    BOBUIest::newRow("destructive") << QDialogButtonBox::DestructiveRole << 1 << 0 << 0 << 0;
    BOBUIest::newRow("Action") << QDialogButtonBox::ActionRole << 1 << 0 << 0 << 0;
    BOBUIest::newRow("Help") << QDialogButtonBox::HelpRole << 1 << 0 << 0 << 1;
}

void tst_QDialogButtonBox::testSignals()
{
    QFETCH(QDialogButtonBox::ButtonRole, buttonToClick);
    QFETCH(int, clicked2Count);
    QDialogButtonBox buttonBox;
    qRegisterMetaType<QAbstractButton *>("QAbstractButton*");
    QSignalSpy clicked2(&buttonBox, &QDialogButtonBox::clicked);
    QSignalSpy accept(&buttonBox, &QDialogButtonBox::accepted);
    QSignalSpy reject(&buttonBox, &QDialogButtonBox::rejected);
    QSignalSpy helpRequested(&buttonBox, &QDialogButtonBox::helpRequested);

    QAbstractButton *clickMe = nullptr;
    for (int i = QDialogButtonBox::AcceptRole; i < QDialogButtonBox::NRoles; ++i) {
        auto button = buttonBox.addButton(QString::number(i),
                                          QDialogButtonBox::ButtonRole(i));

        if (i == buttonToClick)
            clickMe = button;
    }
    if (clickMe)
        clickMe->click();

    BOBUIRY_COMPARE(clicked2.size(), clicked2Count);
    if (clicked2.size() > 0)
        QCOMPARE(qvariant_cast<QAbstractButton *>(clicked2.at(0).at(0)), clickMe);

    BOBUIEST(int(accept.size()), "acceptCount");
    BOBUIEST(int(reject.size()), "rejectCount");
    BOBUIEST(int(helpRequested.size()), "helpRequestedCount");
}

void tst_QDialogButtonBox::testSignalOrder()
{
    buttonClicked1TimeStamp = acceptTimeStamp
            = rejectTimeStamp = helpRequestedTimeStamp = timeStamp = 0LL;
    QDialogButtonBox buttonBox;
    connect(&buttonBox, &QDialogButtonBox::clicked,
            this, &tst_QDialogButtonBox::buttonClicked1);
    connect(&buttonBox, &QDialogButtonBox::accepted, this, &tst_QDialogButtonBox::acceptClicked);
    connect(&buttonBox, &QDialogButtonBox::rejected, this, &tst_QDialogButtonBox::rejectClicked);
    connect(&buttonBox, &QDialogButtonBox::helpRequested, this, &tst_QDialogButtonBox::helpRequestedClicked);

    QPushButton *acceptButton = buttonBox.addButton("OK", QDialogButtonBox::AcceptRole);
    QPushButton *rejectButton = buttonBox.addButton("Cancel", QDialogButtonBox::RejectRole);
    QPushButton *actionButton = buttonBox.addButton("Action This", QDialogButtonBox::ActionRole);
    QPushButton *helpButton = buttonBox.addButton("Help Me!", QDialogButtonBox::HelpRole);

    // Try accept
    acceptButton->click();
    BOBUIRY_VERIFY(acceptTimeStamp > 0LL);
    QCOMPARE(rejectTimeStamp, 0LL);
    QCOMPARE(helpRequestedTimeStamp, 0LL);

    QVERIFY(buttonClicked1TimeStamp < acceptTimeStamp);
    acceptTimeStamp = 0;

    rejectButton->click();
    BOBUIRY_VERIFY(rejectTimeStamp > 0LL);
    QCOMPARE(acceptTimeStamp, 0LL);
    QCOMPARE(helpRequestedTimeStamp, 0LL);
    QVERIFY(buttonClicked1TimeStamp < rejectTimeStamp);

    rejectTimeStamp = 0;
    actionButton->click();
    BOBUIest::qWait(100);
    QCOMPARE(acceptTimeStamp, 0LL);
    QCOMPARE(rejectTimeStamp, 0LL);
    QCOMPARE(helpRequestedTimeStamp, 0LL);

    helpButton->click();
    BOBUIRY_VERIFY(helpRequestedTimeStamp > 0LL);
    QCOMPARE(acceptTimeStamp, 0LL);
    QCOMPARE(rejectTimeStamp, 0LL);
    QVERIFY(buttonClicked1TimeStamp < helpRequestedTimeStamp);
}

void tst_QDialogButtonBox::buttonClicked1(QAbstractButton *)
{
    buttonClicked1TimeStamp = ++timeStamp;
}

void tst_QDialogButtonBox::acceptClicked()
{
    acceptTimeStamp = ++timeStamp;
}

void tst_QDialogButtonBox::rejectClicked()
{
    rejectTimeStamp = ++timeStamp;
}

void tst_QDialogButtonBox::helpRequestedClicked()
{
    helpRequestedTimeStamp = ++timeStamp;
}

void tst_QDialogButtonBox::setStandardButtons_data()
{
    BOBUIest::addColumn<QDialogButtonBox::StandardButtons>("buttonsToAdd");
    BOBUIest::addColumn<QDialogButtonBox::StandardButtons>("expectedResult");

    BOBUIest::newRow("Nothing") << QDialogButtonBox::StandardButtons(QDialogButtonBox::NoButton)
                             << QDialogButtonBox::StandardButtons(QDialogButtonBox::NoButton);
    BOBUIest::newRow("Everything") << (QDialogButtonBox::StandardButtons)0xffffffff
                                << (QDialogButtonBox::Ok
                                   | QDialogButtonBox::Open
                                   | QDialogButtonBox::Save
                                   | QDialogButtonBox::Cancel
                                   | QDialogButtonBox::Close
                                   | QDialogButtonBox::Discard
                                   | QDialogButtonBox::Apply
                                   | QDialogButtonBox::Reset
                                   | QDialogButtonBox::Help
                                   | QDialogButtonBox::Yes
                                   | QDialogButtonBox::YesToAll
                                   | QDialogButtonBox::No
                                   | QDialogButtonBox::NoToAll
                                   | QDialogButtonBox::SaveAll
                                   | QDialogButtonBox::Abort
                                   | QDialogButtonBox::Retry
                                   | QDialogButtonBox::Ignore
                                   | QDialogButtonBox::RestoreDefaults
                                   );
    BOBUIest::newRow("Simple thing") << QDialogButtonBox::StandardButtons(QDialogButtonBox::Help)
                                  << QDialogButtonBox::StandardButtons(QDialogButtonBox::Help);
    BOBUIest::newRow("Standard thing") << (QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
                                    << (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
}

void tst_QDialogButtonBox::setStandardButtons()
{
    QFETCH(QDialogButtonBox::StandardButtons, buttonsToAdd);
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(buttonsToAdd);
    BOBUIEST(buttonBox.standardButtons(), "expectedResult");
}

void tst_QDialogButtonBox::standardButtons()
{
    // Test various cases of setting StandardButtons
    QDialogButtonBox buttonBox;

    QCOMPARE(buttonBox.standardButtons(),
             QDialogButtonBox::StandardButtons(QDialogButtonBox::NoButton));

    // Set some buttons
    buttonBox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QCOMPARE(buttonBox.standardButtons(), QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    // Now add a button
    buttonBox.addButton(QDialogButtonBox::Apply);
    QCOMPARE(buttonBox.standardButtons(),
             QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

    // Set the standard buttons to other things
    buttonBox.setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel
                                 | QDialogButtonBox::Discard);
    QCOMPARE(buttonBox.standardButtons(), QDialogButtonBox::Save | QDialogButtonBox::Cancel
                                          | QDialogButtonBox::Discard);
    QCOMPARE(buttonBox.buttons().size(), 3);

    // Add another button (not a standard one)
    buttonBox.addButton(QLatin1String("Help"), QDialogButtonBox::HelpRole);
    QCOMPARE(buttonBox.standardButtons(), QDialogButtonBox::Save | QDialogButtonBox::Cancel
                                          | QDialogButtonBox::Discard);
    QCOMPARE(buttonBox.buttons().size(), 4);

    // Finally, check if we construct with standard buttons that they show up.
    QDialogButtonBox buttonBox2(QDialogButtonBox::Open | QDialogButtonBox::Reset);
    QCOMPARE(buttonBox2.standardButtons(), QDialogButtonBox::Open | QDialogButtonBox::Reset);
}

void tst_QDialogButtonBox::testRemove()
{
    // Make sure that removing a button and clicking it, doesn't trigger any latent signals
    QDialogButtonBox buttonBox;
    QSignalSpy clicked(&buttonBox, &QDialogButtonBox::clicked);

    QAbstractButton *button = buttonBox.addButton(QDialogButtonBox::Ok);

    buttonBox.removeButton(button);

    button->click();
    BOBUIest::qWait(100);
    QCOMPARE(clicked.size(), 0);
    delete button;
}

void tst_QDialogButtonBox::testDefaultButton_data()
{
    BOBUIest::addColumn<int>("whenToSetDefault");  // -1 Do nothing, 0 after accept, 1 before accept
    BOBUIest::addColumn<int>("buttonToBeDefault");
    BOBUIest::addColumn<int>("indexThatIsDefault");

    BOBUIest::newRow("do nothing First Accept implicit") << -1 << -1 << 0;
    BOBUIest::newRow("First accept explicit before add") << 1 << 0 << 0;
    BOBUIest::newRow("First accept explicit after add") << 0 << 0 << 0;
    BOBUIest::newRow("second accept explicit before add") << 1 << 1 << 1;
    BOBUIest::newRow("second accept explicit after add") << 0 << 1 << 1;
    BOBUIest::newRow("third accept explicit befare add") << 1 << 2 << 2;
    BOBUIest::newRow("third accept explicit after add") << 0 << 2 << 2;
}

void tst_QDialogButtonBox::testDefaultButton()
{
    QFETCH(int, whenToSetDefault);
    QFETCH(int, buttonToBeDefault);
    QFETCH(int, indexThatIsDefault);
    QDialogButtonBox buttonBox;
    QPushButton *buttonArray[] = { new QPushButton("Foo"), new QPushButton("Bar"), new QPushButton("Baz") };

    for (int i = 0; i < 3; ++i) {
        if (whenToSetDefault == 1 && i == buttonToBeDefault)
            buttonArray[i]->setDefault(true);
        buttonBox.addButton(buttonArray[i], QDialogButtonBox::AcceptRole);
        if (whenToSetDefault == 0 && i == buttonToBeDefault)
            buttonArray[i]->setDefault(true);
    }
    buttonBox.show();

    for (int i = 0; i < 3; ++i) {
        if (i == indexThatIsDefault)
            QVERIFY(buttonArray[i]->isDefault());
        else
            QVERIFY(!buttonArray[i]->isDefault());
    }
}

void tst_QDialogButtonBox::task191642_default()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QDialog dlg;
    QPushButton *def = new QPushButton(&dlg);
    QSignalSpy clicked(def, &QPushButton::clicked);
    def->setDefault(true);
    QDialogButtonBox *bb = new QDialogButtonBox(&dlg);
    bb->addButton(QDialogButtonBox::Ok);
    bb->addButton(QDialogButtonBox::Cancel);
    bb->addButton(QDialogButtonBox::Help);

    dlg.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&dlg));
    QVERIFY(def->isDefault());
    BOBUIest::keyPress( &dlg, BobUI::Key_Enter );
    QCOMPARE(clicked.size(), 1);
}

void tst_QDialogButtonBox::testDeletedStandardButton()
{
    QDialogButtonBox buttonBox;
    delete buttonBox.addButton(QDialogButtonBox::Ok);
    QPointer<QPushButton> buttonC = buttonBox.addButton(QDialogButtonBox::Cancel);
    delete buttonBox.addButton(QDialogButtonBox::Cancel);
    QPointer<QPushButton> buttonA = buttonBox.addButton(QDialogButtonBox::Apply);
    delete buttonBox.addButton(QDialogButtonBox::Help);
    // A few button have been deleted, they should automatically be removed
    QCOMPARE(buttonBox.standardButtons(), QDialogButtonBox::Apply | QDialogButtonBox::Cancel);

    buttonBox.setStandardButtons(QDialogButtonBox::Reset | QDialogButtonBox::Cancel);
    // setStanderdButton should delete previous buttons
    QVERIFY(!buttonA);
    QVERIFY(!buttonC);
}

void tst_QDialogButtonBox::automaticDefaultButton()
{
    // Having a QDialogButtonBox inside a QDialog triggers BobUI to
    // enable autoDefault for QPushButtons inside the button box.
    // Check that the logic for resolving a default button based
    // on the Accept role is not overridden by the first button
    // in the dialog (the focus proxy) taking focus, and hence
    // stealing the default button state.

    {
        QDialog dialog;
        QDialogButtonBox *bb = new QDialogButtonBox(&dialog);
        // Force horizontal orientation, where we know the order between
        // Reset and Accept roles are always the same for all layouts.
        bb->setOrientation(BobUI::Horizontal);
        auto *okButton = bb->addButton(QDialogButtonBox::Ok);
        auto *resetButton = bb->addButton(QDialogButtonBox::Reset);
        // Double check our assumption about Reset being first
        QCOMPARE(bb->layout()->itemAt(0)->widget(), resetButton);

        dialog.show();
        QVERIFY(BOBUIest::qWaitForWindowActive(&dialog));

        QVERIFY(okButton->isDefault());
        QSignalSpy buttonClicked(okButton, &QPushButton::clicked);
        BOBUIest::keyPress(&dialog, BobUI::Key_Enter);
        QCOMPARE(buttonClicked.count(), 1);
    }

    // However, if an explicit button has been focused, we respect that.

    {
        QDialog dialog;
        QDialogButtonBox *bb = new QDialogButtonBox(&dialog);
        bb->setOrientation(BobUI::Horizontal);
        bb->addButton(QDialogButtonBox::Ok);
        auto *resetButton = bb->addButton(QDialogButtonBox::Reset);
        resetButton->setFocus();
        dialog.show();
        QVERIFY(BOBUIest::qWaitForWindowActive(&dialog));

        QVERIFY(resetButton->isDefault());
        QSignalSpy buttonClicked(resetButton, &QPushButton::clicked);
        BOBUIest::keyPress(&dialog, BobUI::Key_Enter);
        QCOMPARE(buttonClicked.count(), 1);
    }
}

void tst_QDialogButtonBox::initialFocus_data()
{
    BOBUIest::addColumn<BobUI::FocusPolicy>("focusPolicy");
    BOBUIest::addColumn<bool>("lineEditHasFocus");

    BOBUIest::addRow("TabFocus") << BobUI::FocusPolicy::TabFocus << false;
    BOBUIest::addRow("StrongFocus") << BobUI::FocusPolicy::StrongFocus << true;
    BOBUIest::addRow("NoFocus") << BobUI::FocusPolicy::NoFocus << false;
    BOBUIest::addRow("ClickFocus") << BobUI::FocusPolicy::ClickFocus << false;
    BOBUIest::addRow("WheelFocus") << BobUI::FocusPolicy::WheelFocus << false;
}

void tst_QDialogButtonBox::initialFocus()
{
    QFETCH(const BobUI::FocusPolicy, focusPolicy);
    QFETCH(const bool, lineEditHasFocus);
    QDialog dialog;
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLineEdit *lineEdit = new QLineEdit(&dialog);
    lineEdit->setFocusPolicy(focusPolicy);
    layout->addWidget(lineEdit);
    QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(&dialog);
    layout->addWidget(dialogButtonBox);
    dialogButtonBox->addButton(QDialogButtonBox::Reset);
    const auto *firstAcceptButton = dialogButtonBox->addButton(QDialogButtonBox::Ok);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    dialog.show();
    dialog.activateWindow();
    if (lineEditHasFocus)
        BOBUIRY_VERIFY(lineEdit->hasFocus());
    else
        BOBUIRY_VERIFY(firstAcceptButton->hasFocus());
}

BOBUIEST_MAIN(tst_QDialogButtonBox)
#include "tst_qdialogbuttonbox.moc"
