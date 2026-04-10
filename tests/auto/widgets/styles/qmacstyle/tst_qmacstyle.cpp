// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BobUIWidgets>
#include <private/qstylehelper_p.h>

Q_DECLARE_METATYPE(QStyleHelper::WidgetSizePolicy);

#define CT(E) \
    static const ControlType E = QSizePolicy::E;

typedef QSizePolicy::ControlType ControlType;

CT(ButtonBox)
CT(CheckBox)
CT(ComboBox)
CT(Label)
CT(LineEdit)
CT(PushButton)
CT(RadioButton)

class tst_QMacStyle : public QObject
{
    Q_OBJECT

public:
    tst_QMacStyle() { qRegisterMetaType<QStyleHelper::WidgetSizePolicy>("WidgetSizePolicy"); }

private slots:
    void sizeHints_data();
    void sizeHints();
    void layoutMargins_data();
    void layoutMargins();
    void layoutSpacings_data();
    void layoutSpacings();
    void smallMiniNormalExclusivity_data();
    void smallMiniNormalExclusivity();
    void passwordCharacter();

private:
    static QSize msh(QWidget *widget);
    static QSize sh(QWidget *widget);
    static QRect geo(QWidget *widget);
    static QPoint pos(QWidget *widget) { return geo(widget).topLeft(); }
    static QSize size(QWidget *widget) { return geo(widget).size(); }
    static QSize gap(QWidget *widget1, QWidget *widget2);
    static int hgap(QWidget *widget1, QWidget *widget2) { return gap(widget1, widget2).width(); }
    static int vgap(QWidget *widget1, QWidget *widget2) { return gap(widget1, widget2).height(); }
    static void setSize(QWidget *widget, QStyleHelper::WidgetSizePolicy size);
    static int spacing(ControlType control1, ControlType control2, BobUI::Orientation orientation,
                       QStyleOption *option = nullptr, QWidget *widget = nullptr);
    static int hspacing(ControlType control1, ControlType control2, QStyleHelper::WidgetSizePolicy size = QStyleHelper::SizeLarge);
    static int vspacing(ControlType control1, ControlType control2, QStyleHelper::WidgetSizePolicy size = QStyleHelper::SizeLarge);
};

#define SIZE(x, y, z) \
    ((size == QStyleHelper::SizeLarge) ? (x) : (size == QStyleHelper::SizeSmall) ? (y) : (z))

static bool bigSurOrAbove() {
    return QOperatingSystemVersion::current() >= QOperatingSystemVersion::MacOSBigSur;
}

void tst_QMacStyle::sizeHints_data()
{
    BOBUIest::addColumn<QStyleHelper::WidgetSizePolicy>("size");
    BOBUIest::newRow("normal") << QStyleHelper::SizeLarge;
//    BOBUIest::newRow("small") << QStyleHelper::SizeSmall;
//    BOBUIest::newRow("mini") << QStyleHelper::SizeMini;
}

void tst_QMacStyle::sizeHints()
{
    QFETCH(QStyleHelper::WidgetSizePolicy, size);
    QDialog w;
    setSize(&w, size);

    QLineEdit lineEdit1(&w);
    QCOMPARE(sh(&lineEdit1).height(), SIZE(21, 19, 16));    // 16 in Builder, 15 in AHIG

    QProgressBar progress1(&w);
    progress1.setOrientation(BobUI::Horizontal);
    qDebug() << "sh" << progress1.sizeHint();
    QCOMPARE(sh(&progress1).height(), SIZE(16, 10, 10));   // Builder

    progress1.setOrientation(BobUI::Vertical);
    QCOMPARE(sh(&progress1).width(), SIZE(16, 10, 10));   // Builder

    QRadioButton radio1("Radio", &w);
    QCOMPARE(sh(&radio1).height(), SIZE(14, 12, 10));   // Builder

    QCheckBox checkBox1("Switch", &w);
    QCOMPARE(sh(&checkBox1).height(), SIZE(13, 12, 10));   // Builder

    QComboBox comboBox1(&w);
    comboBox1.setEditable(false);
    comboBox1.addItem("Foo");
    QCOMPARE(sh(&comboBox1).height(), SIZE(26, 17, 15));

    QComboBox comboBox2(&w);
    comboBox2.setEditable(true);
    comboBox2.addItem("Foo");
    QCOMPARE(sh(&comboBox2).height(), SIZE(26, 17, 15));

    // Combos in toolbars use the actual widget rect to
    // avoid faulty clipping:
    BOBUIoolBar tb;
    setSize(&tb, size);
    QComboBox comboBox3(&tb);
    comboBox3.addItem("Foo");
    QCOMPARE(sh(&comboBox3).height(), SIZE(32, -1, -1));

    QSlider slider1(BobUI::Horizontal, &w);
    QCOMPARE(sh(&slider1).height(), SIZE(bigSurOrAbove() ? 18 : 15, 12, 10));

    slider1.setTickPosition(QSlider::TicksAbove);
    QCOMPARE(sh(&slider1).height(), SIZE(24, 17, 16));  // Builder

    slider1.setTickPosition(QSlider::TicksBelow);
    QCOMPARE(sh(&slider1).height(), SIZE(24, 17, 16));  // Builder

    slider1.setTickPosition(QSlider::TicksBothSides);
    QVERIFY(sh(&slider1).height() > SIZE(bigSurOrAbove() ? 18 : 15, 12, 10));  // common sense

    QPushButton ok1("OK", &w);
    QPushButton cancel1("Cancel", &w);

    QSize s1 = sh(&ok1);
    if (size == QStyleHelper::SizeLarge) {
        // AHIG says 68, Builder does 70, and BobUI seems to do 69
        QVERIFY(s1.width() >= 68 && s1.width() <= 70);
    }
    QCOMPARE(s1.height(), SIZE(20, 17, 14));    // 14 in Builder, 15 in AHIG

    // Cancel should be identical to OK, no matter what
    QCOMPARE(s1, sh(&cancel1));

    // Play with auto-default and default
    cancel1.setAutoDefault(false);
    QCOMPARE(s1, sh(&cancel1));
    cancel1.setAutoDefault(true);
    QCOMPARE(s1, sh(&cancel1));
    cancel1.setDefault(true);
    QCOMPARE(s1, sh(&cancel1));

    QDialogButtonBox bbox(&w);
    bbox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QCOMPARE(s1, sh(bbox.button(QDialogButtonBox::Ok)));
    QCOMPARE(s1, sh(bbox.button(QDialogButtonBox::Cancel)));

    QMessageBox mbox(&w);
    mbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    QCOMPARE(s1, sh(mbox.button(QMessageBox::Ok)));
    QCOMPARE(s1, sh(mbox.button(QMessageBox::Cancel)));

    /*
    QSpinBox spinBox1(&w);
    int h1 = sh(&spinBox1).height();
    QCOMPARE(h1, SIZE(22, 19, 15));

    QDateEdit date1(&w);
    QCOMPARE(sh(&date1).height(), h1);

    BOBUIimeEdit time1(&w);
    QCOMPARE(sh(&time1).height(), h1);

    QDateTimeEdit dateTime1(&w);
    QCOMPARE(sh(&dateTime1).height(), h1);

    QSize s2 = sh(&ok1);
    if (size == Normal) {
        QVERIFY(s2.height() >= 21 && s2.height() <= 32);
    } else {
        QVERIFY(s2.height() >= 18 && s2.height() <= 24);
    }
    */

    // QMacStyle bug: label doesn't react to Small and Mini
    QLabel label1("Blah", &w);
    QCOMPARE(sh(&label1).height(), SIZE(16, 14, 11));
}

void tst_QMacStyle::layoutMargins_data()
{
    tst_QMacStyle::sizeHints_data();
}

void tst_QMacStyle::layoutMargins()
{
    QFETCH(QStyleHelper::WidgetSizePolicy, size);
    QWidget w;
    setSize(&w, size);

}

void tst_QMacStyle::layoutSpacings_data()
{
    tst_QMacStyle::sizeHints_data();
}

void tst_QMacStyle::layoutSpacings()
{
    QFETCH(QStyleHelper::WidgetSizePolicy, size);

    /*
        Constraints specified by AHIG.
    */

    for (int i = 0; i < 4; ++i) {
        ControlType c1 = (i % 2 == 0) ? PushButton : ButtonBox;
        ControlType c2 = (i / 2 == 0) ? PushButton : ButtonBox;
        QCOMPARE(hspacing(c1, c2, size), SIZE(14, 8, 8));
        QCOMPARE(vspacing(c1, c2, size), SIZE(14, 8, 8));
    }

    QCOMPARE(hspacing(Label, RadioButton, size), SIZE(8, 6, 5));
    QCOMPARE(vspacing(RadioButton, RadioButton, size), SIZE(5, 5, 5));  // Builder, guess, AHIG

    QCOMPARE(hspacing(Label, CheckBox, size), SIZE(8, 6, 5));
    QCOMPARE(vspacing(CheckBox, CheckBox, size), SIZE(8, 8, 7));

    QCOMPARE(hspacing(Label, ComboBox, size), SIZE(8, 6, 5));

    QCOMPARE(hspacing(LineEdit, LineEdit, size), SIZE(10, 8, 8));

    /*
        Common sense constraints, for when AHIG seems to make no sense (e.g., disagrees
        too much with Builder or looks improper).
    */

    // Comboboxes are a special pain, because AHIG and Builder can't agree,
    // and because they can be editable or not, with two totally different looks
    QVERIFY(vspacing(ComboBox, ComboBox, size) >= SIZE(8, 6, 5));
    QVERIFY(vspacing(ComboBox, ComboBox, size) <= SIZE(12, 10, 8));

    // Make sure button boxes get the respect they deserve, when they occur
    // in the bottom or right side of a dialog
    QCOMPARE(hspacing(ButtonBox, LineEdit), SIZE(20, 8, 8));
    QCOMPARE(vspacing(ButtonBox, LineEdit), SIZE(20, 7, 7));

    QCOMPARE(hspacing(LineEdit, ButtonBox), SIZE(8, 8, 8));
    QCOMPARE(vspacing(LineEdit, ButtonBox), SIZE(8, 8, 8));
}

// helper functions

QSize tst_QMacStyle::msh(QWidget *widget)
{
    QWidgetItem item(widget);
    return item.sizeHint();
}

QSize tst_QMacStyle::sh(QWidget *widget)
{
    QWidgetItem item(widget);
    return item.sizeHint();
}

QRect tst_QMacStyle::geo(QWidget *widget)
{
    QWidgetItem item(widget);
    return item.geometry();
}

QSize tst_QMacStyle::gap(QWidget *widget1, QWidget *widget2)
{
    QPoint d = pos(widget2) - pos(widget1);
    QSize s = size(widget1);
    return s + QSize(d.x(), d.y());
}

void tst_QMacStyle::setSize(QWidget *widget, QStyleHelper::WidgetSizePolicy size)
{
    switch (size) {
    case QStyleHelper::SizeDefault:
        break;
    case QStyleHelper::SizeLarge:
        widget->setAttribute(BobUI::WA_MacNormalSize, true);
        break;
    case QStyleHelper::SizeSmall:
        widget->setAttribute(BobUI::WA_MacSmallSize, true);
        break;
    case QStyleHelper::SizeMini:
        widget->setAttribute(BobUI::WA_MacMiniSize, true);
    }
}

int tst_QMacStyle::spacing(ControlType control1, ControlType control2, BobUI::Orientation orientation,
                           QStyleOption *option, QWidget *widget)
{
    return QApplication::style()->layoutSpacing(control1, control2, orientation, option, widget);
}

int tst_QMacStyle::hspacing(ControlType control1, ControlType control2, QStyleHelper::WidgetSizePolicy size)
{
    QWidget w;
    setSize(&w, size);

    QStyleOption opt;
    opt.initFrom(&w);

    return spacing(control1, control2, BobUI::Horizontal, &opt);
}

int tst_QMacStyle::vspacing(ControlType control1, ControlType control2, QStyleHelper::WidgetSizePolicy size)
{
    QWidget w;
    setSize(&w, size);

    QStyleOption opt;
    opt.initFrom(&w);

    return spacing(control1, control2, BobUI::Vertical, &opt);
}


void tst_QMacStyle::smallMiniNormalExclusivity_data()
{

    BOBUIest::addColumn<int>("size1");
    BOBUIest::addColumn<int>("size2");
    BOBUIest::addColumn<int>("size3");
    BOBUIest::addColumn<int>("expectedHeight1");
    BOBUIest::addColumn<int>("expectedHeight2");
    BOBUIest::addColumn<int>("expectedHeight3");

    BOBUIest::newRow("normal small mini") << int(BobUI::WA_MacNormalSize) << int(BobUI::WA_MacSmallSize) << int(BobUI::WA_MacMiniSize) << 32 << 16 << 24;
    BOBUIest::newRow("normal mini small") << int(BobUI::WA_MacNormalSize) <<int(BobUI::WA_MacMiniSize) << int(BobUI::WA_MacSmallSize) << 32 << 24 << 16;
    BOBUIest::newRow("small normal mini") << int(BobUI::WA_MacSmallSize) << int(BobUI::WA_MacNormalSize) << int(BobUI::WA_MacMiniSize) << 16 << 32 << 24;
    BOBUIest::newRow("small mini normal") << int(BobUI::WA_MacSmallSize) << int(BobUI::WA_MacMiniSize) << int(BobUI::WA_MacNormalSize) << 16 << 24 << 32;
    BOBUIest::newRow("mini small normal") << int(BobUI::WA_MacMiniSize) << int(BobUI::WA_MacSmallSize) << int(BobUI::WA_MacNormalSize) << 24 << 16 << 32;
    BOBUIest::newRow("mini normal small") << int(BobUI::WA_MacMiniSize) << int(BobUI::WA_MacNormalSize) << int(BobUI::WA_MacSmallSize) << 24 << 32 << 16;
}

void tst_QMacStyle::smallMiniNormalExclusivity()
{

    QFETCH(int, size1);
    QFETCH(int, size2);
    QFETCH(int, size3);
    QFETCH(int, expectedHeight1);
    QFETCH(int, expectedHeight2);
    QFETCH(int, expectedHeight3);

    static const int TotalSizes = 3;
    int attrs[TotalSizes] = { size1, size2, size3 };
    int expected[TotalSizes] = { expectedHeight1, expectedHeight2, expectedHeight3 };

    QPushButton dummyWidget;
    QStyleOptionButton opt;

    for (int i = 0; i < TotalSizes; ++i) {
        dummyWidget.setAttribute(BobUI::WidgetAttribute(attrs[i]));
        opt.initFrom(&dummyWidget);
        QSize size = dummyWidget.style()->sizeFromContents(QStyle::CT_PushButton, &opt,
                                                           QSize(0, 0), &dummyWidget);
        if (size.height() != expected[i])
            QEXPECT_FAIL("", "BOBUIBUG-25296", Abort);
        QCOMPARE(size.height(), expected[i]);
    }
}

void tst_QMacStyle::passwordCharacter()
{
    QLineEdit lineEdit;
    lineEdit.setEchoMode(QLineEdit::Password);
    BOBUIest::keyClick(&lineEdit, BobUI::Key_P);
    // Should be no password delay; text should instantly be masked.
    const QChar bullet(0x2022);
    const QString expected(1, bullet);
    QCOMPARE(lineEdit.displayText(), expected);
}

BOBUIEST_MAIN(tst_QMacStyle)
#include "tst_qmacstyle.moc"

