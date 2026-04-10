// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>


#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>


class tst_QRadioButton : public QObject
{
Q_OBJECT
public:
    tst_QRadioButton(){};
    virtual ~tst_QRadioButton(){};

private slots:
#if BOBUI_CONFIG(shortcut)
    void task190739_focus();
#endif
    void minimumSizeHint();

private:
};

#if BOBUI_CONFIG(shortcut)

void tst_QRadioButton::task190739_focus()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget widget;
    QPushButton button1(&widget);
    button1.setText("button1");
    QLineEdit edit(&widget);
    edit.setFocus();

    QRadioButton radio1(&widget);
    radio1.setText("radio1");
    radio1.setFocusPolicy(BobUI::TabFocus);
    radio1.setShortcut(QKeySequence("Ctrl+O"));

    QVBoxLayout layout(&widget);
    layout.addWidget(&button1);
    layout.addWidget(&edit);
    layout.addWidget(&radio1);

    widget.show();
    widget.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));

    QVERIFY(edit.hasFocus());
    QVERIFY(!radio1.isChecked());

    BOBUIest::keyClick(&edit, BobUI::Key_O, BobUI::ControlModifier, 20);
    BOBUIRY_VERIFY(radio1.isChecked());
    QVERIFY(edit.hasFocus());
    QVERIFY(!radio1.hasFocus());
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QRadioButton::minimumSizeHint()
{
    QRadioButton button(tr("QRadioButtons sizeHint is the same as it's minimumSizeHint"));
    QCOMPARE(button.sizeHint(), button.minimumSizeHint());
}


BOBUIEST_MAIN(tst_QRadioButton)
#include "tst_qradiobutton.moc"
