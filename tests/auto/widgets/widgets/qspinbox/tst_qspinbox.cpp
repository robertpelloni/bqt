// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qdebug.h>
#include <qapplication.h>
#include <limits.h>
#include <qspinbox.h>
#include <qlocale.h>
#include <qlineedit.h>
#include <qlayout.h>

#include <BOBUIest>
#include <QSpinBox>
#include <QWidget>
#include <QString>
#include <QValidator>
#include <QLineEdit>
#include <QObject>
#include <QStringList>
#include <QList>
#include <QLocale>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QSignalSpy>
#include <QStackedWidget>
#include <QDebug>
#include <QStyleOptionSpinBox>
#include <QStyle>
#include <QProxyStyle>
#include <QScreen>

#include <BobUIWidgets/private/qapplication_p.h>

#if BOBUI_CONFIG(shortcut)
#  include <QKeySequence>
#endif

class SpinBox : public QSpinBox
{
public:
    SpinBox(QWidget *parent = nullptr)
        : QSpinBox(parent)
    {}
    QString textFromValue(int v) const override
    {
        return QSpinBox::textFromValue(v);
    }
    QValidator::State validate(QString &text, int &pos) const override
    {
        return QSpinBox::validate(text, pos);
    }
    int valueFromText(const QString &text) const override
    {
        return QSpinBox::valueFromText(text);
    }
#if BOBUI_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override
    {
        QSpinBox::wheelEvent(event);
    }
#endif
    void initStyleOption(QStyleOptionSpinBox *option) const override
    {
        QSpinBox::initStyleOption(option);
    }

    QLineEdit *lineEdit() const { return QSpinBox::lineEdit(); }
};

class PressAndHoldStyle : public QProxyStyle
{
    Q_OBJECT
public:
    using QProxyStyle::QProxyStyle;

    int styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override
    {
        switch (hint) {
        case QStyle::SH_SpinBox_ClickAutoRepeatRate:
            return 5;
        case QStyle::SH_SpinBox_ClickAutoRepeatThreshold:
            return 10;
        default:
            return QProxyStyle::styleHint(hint, option, widget, returnData);
        }
    }
};

class StepModifierStyle : public QProxyStyle
{
    Q_OBJECT
public:
    using QProxyStyle::QProxyStyle;

    int styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override
    {
        switch (hint) {
        case QStyle::SH_SpinBox_StepModifier:
            return stepModifier;
        default:
            return QProxyStyle::styleHint(hint, option, widget, returnData);
        }
    }

    BobUI::KeyboardModifier stepModifier = BobUI::ControlModifier;
};

class SelectAllOnStepStyle : public QProxyStyle
{
public:
    SelectAllOnStepStyle(bool selectAll)
    : selectAll(selectAll)
    {}

    int styleHint(QStyle::StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData = nullptr) const override
    {
        switch (hint) {
        case QStyle::SH_SpinBox_SelectOnStep:
            return selectAll;
        default:
            return QProxyStyle::styleHint(hint, option, widget, returnData);
        }
    }

private:
    const bool selectAll;
};

class tst_QSpinBox : public QObject
{
    Q_OBJECT
public:
    tst_QSpinBox();
public slots:
    void init();
private slots:
    void getSetCheck();
    void setValue_data();
    void setValue();

    void setDisplayIntegerBase_data();
    void setDisplayIntegerBase();

    void setPrefixSuffix_data();
    void setPrefixSuffix();

    void setReadOnly();

    void setTracking_data();
    void setTracking();

    void locale_data();
    void locale();

    void setWrapping_data();
    void setWrapping();

    void setSpecialValueText_data();
    void setSpecialValueText();

    void setSingleStep_data();
    void setSingleStep();

    void setMinMax_data();
    void setMinMax();

    void editingFinished();

    void returnPressed();

    void valueFromTextAndValidate_data();
    void valueFromTextAndValidate();

    void removeAll();
    void startWithDash();

#if BOBUI_CONFIG(shortcut)
    void undoRedo();
#endif

    void specialValue();
    void textFromValue();

    void sizeHint();

    void integerOverflow();

    void taskBOBUIBUG_5008_textFromValueAndValidate();
    void lineEditReturnPressed();

    void positiveSign();

    void interpretOnLosingFocus();

    void setGroupSeparatorShown_data();
    void setGroupSeparatorShown();

    void wheelEvents_data();
    void wheelEvents();

    void adaptiveDecimalStep();

    void stepModifierKeys_data();
    void stepModifierKeys();

    void stepModifierButtons_data();
    void stepModifierButtons();

    void stepModifierPressAndHold_data();
    void stepModifierPressAndHold();

    void stepSelectAll_data();
    void stepSelectAll();

public slots:
    void textChangedHelper(const QString &);
    void valueChangedHelper(int);
private:
    QStringList actualTexts;
    QList<int> actualValues;
};

typedef QList<int> IntList;

Q_DECLARE_METATYPE(QLocale::Language)
Q_DECLARE_METATYPE(QLocale::Country)

static QLatin1String modifierToName(BobUI::KeyboardModifier modifier)
{
    switch (modifier) {
    case BobUI::NoModifier:
        return QLatin1String("No");
        break;
    case BobUI::ControlModifier:
        return QLatin1String("Ctrl");
        break;
    case BobUI::ShiftModifier:
        return QLatin1String("Shift");
        break;
    case BobUI::AltModifier:
        return QLatin1String("Alt");
        break;
    case BobUI::MetaModifier:
        return QLatin1String("Meta");
        break;
    default:
        qFatal("Unexpected keyboard modifier");
        return QLatin1String();
    }
}

// Testing get/set functions
void tst_QSpinBox::getSetCheck()
{
    QSpinBox obj1;
    QCOMPARE(obj1.inputMethodQuery(BobUI::ImHints), QVariant(int(BobUI::ImhDigitsOnly)));
    // int QSpinBox::singleStep()
    // void QSpinBox::setSingleStep(int)
    obj1.setSingleStep(0);
    QCOMPARE(0, obj1.singleStep());
    obj1.setSingleStep(INT_MIN);
    QCOMPARE(0, obj1.singleStep()); // Can't have negative steps => keep old value
    obj1.setSingleStep(INT_MAX);
    QCOMPARE(INT_MAX, obj1.singleStep());

    // int QSpinBox::minimum()
    // void QSpinBox::setMinimum(int)
    obj1.setMinimum(0);
    QCOMPARE(0, obj1.minimum());
    obj1.setMinimum(INT_MIN);
    QCOMPARE(INT_MIN, obj1.minimum());
    obj1.setMinimum(INT_MAX);
    QCOMPARE(INT_MAX, obj1.minimum());

    // int QSpinBox::maximum()
    // void QSpinBox::setMaximum(int)
    obj1.setMaximum(0);
    QCOMPARE(0, obj1.maximum());
    obj1.setMaximum(INT_MIN);
    QCOMPARE(INT_MIN, obj1.maximum());
    obj1.setMaximum(INT_MAX);
    QCOMPARE(INT_MAX, obj1.maximum());

    // int QSpinBox::value()
    // void QSpinBox::setValue(int)
    obj1.setValue(0);
    QCOMPARE(0, obj1.value());
    obj1.setValue(INT_MIN);
    QCOMPARE(INT_MIN, obj1.value());
    obj1.setValue(INT_MAX);
    QCOMPARE(INT_MAX, obj1.value());

    QDoubleSpinBox obj2;
    QCOMPARE(obj2.inputMethodQuery(BobUI::ImHints), QVariant(int(BobUI::ImhFormattedNumbersOnly)));
    // double QDoubleSpinBox::singleStep()
    // void QDoubleSpinBox::setSingleStep(double)
    obj2.setSingleStep(0.0);
    QCOMPARE(0.0, obj2.singleStep());
    obj2.setSingleStep(1.0);
    QCOMPARE(1.0, obj2.singleStep());

    // double QDoubleSpinBox::minimum()
    // void QDoubleSpinBox::setMinimum(double)
    obj2.setMinimum(1.0);
    QCOMPARE(1.0, obj2.minimum());
    obj2.setMinimum(0.0);
    QCOMPARE(0.0, obj2.minimum());
    obj2.setMinimum(-1.0);
    QCOMPARE(-1.0, obj2.minimum());

    // double QDoubleSpinBox::maximum()
    // void QDoubleSpinBox::setMaximum(double)
    obj2.setMaximum(-1.0);
    QCOMPARE(-1.0, obj2.maximum());
    obj2.setMaximum(0.0);
    QCOMPARE(0.0, obj2.maximum());
    obj2.setMaximum(1.0);
    QCOMPARE(1.0, obj2.maximum());

    // int QDoubleSpinBox::decimals()
    // void QDoubleSpinBox::setDecimals(int)
    obj2.setDecimals(0);
    QCOMPARE(0, obj2.decimals());
    obj2.setDecimals(INT_MIN);
    QCOMPARE(0, obj2.decimals()); // Range<0, 13>

    //obj2.setDecimals(INT_MAX);
    //QCOMPARE(13, obj2.decimals()); // Range<0, 13>
    obj2.setDecimals(128);
    QCOMPARE(obj2.decimals(), 128); // Range<0, 128>

    // double QDoubleSpinBox::value()
    // void QDoubleSpinBox::setValue(double)
    obj2.setValue(-1.0);
    QCOMPARE(-1.0, obj2.value());
    obj2.setValue(0.0);
    QCOMPARE(0.0, obj2.value());
    obj2.setValue(1.0);
    QCOMPARE(1.0, obj2.value());

    // Make sure we update line edit geometry when updating
    // buttons - see task 235747
    QRect oldEditGeometry = obj1.childrenRect();
    obj1.setButtonSymbols(QAbstractSpinBox::NoButtons);
    QVERIFY(obj1.childrenRect() != oldEditGeometry);
}

tst_QSpinBox::tst_QSpinBox()
{
    QLocale::setDefault(QLocale::c());
}

void tst_QSpinBox::init()
{
#if BOBUI_CONFIG(cursor)
    // Ensure mouse cursor was not left by previous tests where widgets
    // will appear, as it could cause events and interfere with the tests.
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect availableGeometry = screen->availableGeometry();
    QCursor::setPos(availableGeometry.topLeft());
#endif
}

void tst_QSpinBox::setValue_data()
{
    BOBUIest::addColumn<int>("set");
    BOBUIest::addColumn<int>("expected");

    BOBUIest::newRow("data0") << 0 << 0;
    BOBUIest::newRow("data1") << 100 << 100;
    BOBUIest::newRow("data2") << -100 << -100;
    BOBUIest::newRow("data3") << INT_MIN << INT_MIN;
    BOBUIest::newRow("data4") << INT_MAX << INT_MAX;
}

void tst_QSpinBox::setValue()
{
    QFETCH(int, set);
    QFETCH(int, expected);
    QSpinBox spin(0);
    spin.setRange(INT_MIN, INT_MAX);
    spin.setValue(set);
    QCOMPARE(spin.value(), expected);
}

void tst_QSpinBox::setDisplayIntegerBase_data()
{
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<int>("base");
    BOBUIest::addColumn<QString>("string");

    BOBUIest::newRow("base 10") << 42 << 10 << "42";
    BOBUIest::newRow("base 2") << 42 << 2 << "101010";
    BOBUIest::newRow("base 8") << 42 << 8 << "52";
    BOBUIest::newRow("base 16") << 42 << 16 << "2a";
    BOBUIest::newRow("base 0") << 42 << 0 << "42";
    BOBUIest::newRow("base -4") << 42 << -4 << "42";
    BOBUIest::newRow("base 40") << 42 << 40 << "42";

    BOBUIest::newRow("negative base 10") << -42 << 10 << "-42";
    BOBUIest::newRow("negative base 2") << -42 << 2 << "-101010";
    BOBUIest::newRow("negative base 8") << -42 << 8 << "-52";
    BOBUIest::newRow("negative base 16") << -42 << 16 << "-2a";
    BOBUIest::newRow("negative base 0") << -42 << 0 << "-42";
    BOBUIest::newRow("negative base -4") << -42 << -4 << "-42";
    BOBUIest::newRow("negative base 40") << -42 << 40 << "-42";

    BOBUIest::newRow("0 base 10") << 0 << 10 << "0";
    BOBUIest::newRow("0 base 2") << 0 << 2 << "0";
    BOBUIest::newRow("0 base 8") << 0 << 8 << "0";
    BOBUIest::newRow("0 base 16") << 0 << 16 << "0";
    BOBUIest::newRow("0 base 0") << 0 << 0 << "0";
    BOBUIest::newRow("0 base -4") << 0 << -4 << "0";
    BOBUIest::newRow("0 base 40") << 0 << 40 << "0";
}

void tst_QSpinBox::setDisplayIntegerBase()
{
    QFETCH(int, value);
    QFETCH(int, base);
    QFETCH(QString, string);

    SpinBox spin;
    spin.setRange(INT_MIN, INT_MAX);

    spin.setValue(value);
    QCOMPARE(spin.lineEdit()->text(), QString::number(value));

    spin.setDisplayIntegerBase(base);
    QCOMPARE(spin.lineEdit()->text(), string);

    spin.setValue(0);
    QCOMPARE(spin.value(), 0);
    QCOMPARE(spin.lineEdit()->text(), QString::number(0, base));

    spin.lineEdit()->clear();
    BOBUIest::keyClicks(spin.lineEdit(), string);
    QCOMPARE(spin.value(), value);
}

void tst_QSpinBox::setPrefixSuffix_data()
{
    BOBUIest::addColumn<QString>("prefix");
    BOBUIest::addColumn<QString>("suffix");
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<QString>("expectedCleanText");
    BOBUIest::addColumn<bool>("show");

    BOBUIest::newRow("data0") << QString() << QString() << 10 << "10" << "10" << false;
    BOBUIest::newRow("data1") << QString() << "cm" << 10 << "10cm" << "10" << false;
    BOBUIest::newRow("data2") << "cm: " << QString() << 10 << "cm: 10" << "10" << false;
    BOBUIest::newRow("data3") << "length: " << "cm" << 10 << "length: 10cm" << "10" << false;

    BOBUIest::newRow("data4") << QString() << QString() << 10 << "10" << "10" << true;
    BOBUIest::newRow("data5") << QString() << "cm" << 10 << "10cm" << "10" << true;
    BOBUIest::newRow("data6") << "cm: " << QString() << 10 << "cm: 10" << "10" << true;
    BOBUIest::newRow("data7") << "length: " << "cm" << 10 << "length: 10cm" << "10" << true;
}

void tst_QSpinBox::setPrefixSuffix()
{
    QFETCH(QString, prefix);
    QFETCH(QString, suffix);
    QFETCH(int, value);
    QFETCH(QString, expectedText);
    QFETCH(QString, expectedCleanText);
    QFETCH(bool, show);

    QSpinBox spin;
    if (show) {
        spin.show();
        spin.setPrefix(QString());  // trigger a recalc of sizeHint
    }
    const QSize size1 = spin.sizeHint();
    spin.setPrefix(prefix);
    const QSize size2 = spin.sizeHint();
    spin.setSuffix(suffix);
    const QSize size3 = spin.sizeHint();
    spin.setValue(value);

    QCOMPARE(spin.prefix(), prefix);
    QCOMPARE(spin.suffix(), suffix);
    QCOMPARE(spin.text(), expectedText);
    QCOMPARE(spin.cleanText(), expectedCleanText);

    if (!suffix.isEmpty()) {
        QVERIFY(size2.width() < size3.width());
        spin.setSuffix(QString());
        QCOMPARE(spin.sizeHint(), size2);
    }
    if (!prefix.isEmpty()) {
        QVERIFY(size1.width() < size2.width());
        spin.setPrefix(QString());
        QCOMPARE(spin.sizeHint(), size1);
    }
}

void tst_QSpinBox::textChangedHelper(const QString &text)
{
    actualTexts << text;
}

void tst_QSpinBox::valueChangedHelper(int value)
{
    actualValues << value;
}

class ReadOnlyChangeTracker: public QSpinBox
{
public:
    ReadOnlyChangeTracker(QWidget *parent = nullptr) : QSpinBox(parent) {}

    void changeEvent(QEvent *ev) override
    {
        if (ev->type() == QEvent::ReadOnlyChange)
            ++readOnlyChangeEventCount;
    }
    int readOnlyChangeEventCount = 0;
};

void tst_QSpinBox::setReadOnly()
{
    ReadOnlyChangeTracker spin(0);
    spin.show();
    BOBUIest::keyClick(&spin, BobUI::Key_Up);
    QCOMPARE(spin.value(), 1);
    spin.setReadOnly(true);
    QCOMPARE(spin.readOnlyChangeEventCount, 1);
    BOBUIest::keyClick(&spin, BobUI::Key_Up);
    QCOMPARE(spin.value(), 1);
    spin.stepBy(1);
    QCOMPARE(spin.value(), 2);
    spin.setReadOnly(false);
    QCOMPARE(spin.readOnlyChangeEventCount, 2);
    BOBUIest::keyClick(&spin, BobUI::Key_Up);
    QCOMPARE(spin.value(), 3);
}
void tst_QSpinBox::setTracking_data()
{
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<QStringList>("texts");
    BOBUIest::addColumn<bool>("tracking");

    BOBUIestEventList keys;
    QStringList texts1;
    QStringList texts2;

#ifdef Q_OS_MAC
    keys.addKeyClick(BobUI::Key_Right, BobUI::ControlModifier);
#else
    keys.addKeyClick(BobUI::Key_End);
#endif
    keys.addKeyClick('7');
    keys.addKeyClick('9');
    keys.addKeyClick(BobUI::Key_Enter);
    keys.addKeyClick(BobUI::Key_Enter);
    keys.addKeyClick(BobUI::Key_Enter);
    texts1 << "07" << "079" << "79" << "79" << "79";
    texts2 << "79";
    BOBUIest::newRow("data1") << keys << texts1 << true;
    BOBUIest::newRow("data2") << keys << texts2 << false;
}

void tst_QSpinBox::setTracking()
{
    actualTexts.clear();
    QFETCH(BOBUIestEventList, keys);
    QFETCH(QStringList, texts);
    QFETCH(bool, tracking);

    QSpinBox spin(0);
    spin.setKeyboardTracking(tracking);
    spin.show();
    connect(&spin, &QSpinBox::textChanged, this, &tst_QSpinBox::textChangedHelper);

    keys.simulate(&spin);
    QCOMPARE(actualTexts, texts);
}

void tst_QSpinBox::setWrapping_data()
{
    BOBUIest::addColumn<bool>("wrapping");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("startValue");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<IntList>("expected");

    BOBUIestEventList keys;
    IntList values;
    keys.addKeyClick(BobUI::Key_Up);
    values << 10;
    keys.addKeyClick(BobUI::Key_Up);
    BOBUIest::newRow("data0") << false << 0 << 10 << 9 << keys << values;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_Up);
    values << 10;
    keys.addKeyClick(BobUI::Key_Up);
    values << 0;
    BOBUIest::newRow("data1") << true << 0 << 10 << 9 << keys << values;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_Delete); // doesn't emit because lineedit is empty so intermediate
    keys.addKeyClick('1');
    keys.addKeyClick(BobUI::Key_Down);
    keys.addKeyClick(BobUI::Key_Down);
    values << 1 << 0;
    BOBUIest::newRow("data2") << false << 0 << 10 << 9 << keys << values;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_Delete);
    keys.addKeyClick('1');
    keys.addKeyClick(BobUI::Key_Down);
    keys.addKeyClick(BobUI::Key_Down);
    values << 1 << 0 << 10;
    BOBUIest::newRow("data3") << true << 0 << 10 << 9 << keys << values;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_PageDown);
    keys.addKeyClick(BobUI::Key_Down);
    values << 0;
    BOBUIest::newRow("data4") << false << 0 << 10 << 6 << keys << values;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_PageDown);
    keys.addKeyClick(BobUI::Key_Down);
    values << 0 << 10;
    BOBUIest::newRow("data5") << true << 0 << 10 << 6 << keys << values;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_PageUp);
    keys.addKeyClick(BobUI::Key_PageDown);
    keys.addKeyClick(BobUI::Key_Down);
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_PageDown);
    keys.addKeyClick(BobUI::Key_PageDown);
    values << 10 << 0 << 10 << 0 << 10 << 0;
    BOBUIest::newRow("data6") << true << 0 << 10 << 6 << keys << values;

}


void tst_QSpinBox::setWrapping()
{
    QFETCH(bool, wrapping);
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(int, startValue);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(IntList, expected);

    QSpinBox spin(0);
    QVERIFY(!spin.wrapping());
    spin.setMinimum(minimum);
    spin.setMaximum(maximum);
    spin.setValue(startValue);
    spin.setWrapping(wrapping);
    spin.show();
    actualValues.clear();
    connect(&spin, SIGNAL(valueChanged(int)), this, SLOT(valueChangedHelper(int)));

    keys.simulate(&spin);

    QCOMPARE(actualValues.size(), expected.size());
    for (int i=0; i<qMin(actualValues.size(), expected.size()); ++i) {
        QCOMPARE(actualValues.at(i), expected.at(i));
    }
}

void tst_QSpinBox::setSpecialValueText_data()
{
    BOBUIest::addColumn<QString>("specialValueText");
    BOBUIest::addColumn<int>("minimum");
    BOBUIest::addColumn<int>("maximum");
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<QString>("expected");
    BOBUIest::addColumn<bool>("show");

    BOBUIest::newRow("data0") << QString() << 0 << 10 << 1 << "1" << false;
    BOBUIest::newRow("data1") << QString() << 0 << 10 << 1 << "1" << true;
    BOBUIest::newRow("data2") << "foo" << 0 << 10 << 0 << "foo" << false;
    BOBUIest::newRow("data3") << "foo" << 0 << 10 << 0 << "foo" << true;
}

void tst_QSpinBox::setSpecialValueText()
{
    QFETCH(QString, specialValueText);
    QFETCH(int, minimum);
    QFETCH(int, maximum);
    QFETCH(int, value);
    QFETCH(QString, expected);
    QFETCH(bool, show);

    QSpinBox spin(0);
    spin.setSpecialValueText(specialValueText);
    QCOMPARE(spin.specialValueText(), specialValueText);
    spin.setMinimum(minimum);
    spin.setMaximum(maximum);
    spin.setValue(value);
    if (show)
        spin.show();

    QCOMPARE(spin.text(), expected);
}

void tst_QSpinBox::setSingleStep_data()
{
    BOBUIest::addColumn<int>("singleStep");
    BOBUIest::addColumn<int>("startValue");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<IntList>("expected");
    BOBUIest::addColumn<bool>("show");

    BOBUIestEventList keys;
    IntList values;
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Down);
    keys.addKeyClick(BobUI::Key_Up);
    values << 11 << 10 << 11;
    BOBUIest::newRow("data0") << 1 << 10 << keys << values << false;
    BOBUIest::newRow("data1") << 1 << 10 << keys << values << true;

    keys.clear();
    values.clear();
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Down);
    keys.addKeyClick(BobUI::Key_Up);
    values << 12 << 10 << 12;
    BOBUIest::newRow("data2") << 2 << 10 << keys << values << false;
    BOBUIest::newRow("data3") << 2 << 10 << keys << values << true;
}

void tst_QSpinBox::setSingleStep()
{
    QFETCH(int, singleStep);
    QFETCH(int, startValue);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(IntList, expected);
    QFETCH(bool, show);

    QSpinBox spin(0);
    actualValues.clear();
    spin.setSingleStep(singleStep);
    QCOMPARE(spin.singleStep(), singleStep);
    spin.setValue(startValue);
    if (show)
        spin.show();
    connect(&spin, SIGNAL(valueChanged(int)), this, SLOT(valueChangedHelper(int)));

    QCOMPARE(actualValues.size(), 0);
    keys.simulate(&spin);
    QCOMPARE(actualValues.size(), expected.size());
    for (int i=0; i<qMin(actualValues.size(), expected.size()); ++i) {
        QCOMPARE(actualValues.at(i), expected.at(i));
    }
}

void tst_QSpinBox::setMinMax_data()
{
    BOBUIest::addColumn<int>("startValue");
    BOBUIest::addColumn<int>("mini");
    BOBUIest::addColumn<int>("maxi");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<int>("expected");
    BOBUIest::addColumn<bool>("show");

    BOBUIestEventList keys;
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Up);
    keys.addKeyClick(BobUI::Key_Up);
    BOBUIest::newRow("data0") << 1 << INT_MIN << 2 << keys << 2 << false;
    BOBUIest::newRow("data1") << 1 << INT_MIN << 2 << keys << 2 << true;

    keys.clear();
    BOBUIest::newRow("data2") << 2 << INT_MAX - 2 << INT_MAX << keys << INT_MAX - 2 << false;
    BOBUIest::newRow("data3") << 2 << INT_MAX - 2 << INT_MAX << keys << INT_MAX - 2 << true;
}

void tst_QSpinBox::setMinMax()
{
    QFETCH(int, startValue);
    QFETCH(int, mini);
    QFETCH(int, maxi);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(int, expected);
    QFETCH(bool, show);

    QSpinBox spin(0);
    spin.setValue(startValue);
    spin.setMinimum(mini);
    spin.setMaximum(maxi);
    QCOMPARE(spin.minimum(), mini);
    QCOMPARE(spin.maximum(), maxi);
    if (show)
        spin.show();
    keys.simulate(&spin);
    QCOMPARE(spin.value(), expected);
}

void tst_QSpinBox::valueFromTextAndValidate_data()
{
    const int Intermediate = QValidator::Intermediate;
    const int Invalid = QValidator::Invalid;
    const int Acceptable = QValidator::Acceptable;

    BOBUIest::addColumn<QString>("txt");
    BOBUIest::addColumn<int>("state");
    BOBUIest::addColumn<int>("mini");
    BOBUIest::addColumn<int>("maxi");
    BOBUIest::addColumn<QString>("expectedText"); // if empty we don't check

    BOBUIest::newRow("data0") << QString("2") << Intermediate << 3 << 5 << QString();
    BOBUIest::newRow("data1") << QString() << Intermediate << 0 << 100 << QString();
    BOBUIest::newRow("data2") << QString("asd") << Invalid << 0 << 100 << QString();
    BOBUIest::newRow("data3") << QString("2") << Acceptable << 0 << 100 << QString();
    BOBUIest::newRow("data4") << QString() << Intermediate << 0 << 1 << QString();
    BOBUIest::newRow("data5") << QString() << Invalid << 0 << 0 << QString();
    BOBUIest::newRow("data6") << QString("5") << Intermediate << 2004 << 2005 << QString();
    BOBUIest::newRow("data7") << QString("50") << Intermediate << 2004 << 2005 << QString();
    BOBUIest::newRow("data8") << QString("205") << Intermediate << 2004 << 2005 << QString();
    BOBUIest::newRow("data9") << QString("2005") << Acceptable << 2004 << 2005 << QString();
    BOBUIest::newRow("data10") << QString("3") << Intermediate << 2004 << 2005 << QString();
    BOBUIest::newRow("data11") << QString("-") << Intermediate << -20 << -10 << QString();
    BOBUIest::newRow("data12") << QString("-1") << Intermediate << -20 << -10 << QString();
    BOBUIest::newRow("data13") << QString("-5") << Intermediate << -20 << -10 << QString();
    BOBUIest::newRow("data14") << QString("-5") << Intermediate << -20 << -16 << QString();
    BOBUIest::newRow("data15") << QString("-2") << Intermediate << -20 << -16 << QString();
    BOBUIest::newRow("data16") << QString("2") << Invalid << -20 << -16 << QString();
    BOBUIest::newRow("data17") << QString() << Intermediate << -20 << -16 << QString();
    BOBUIest::newRow("data18") << QString("  22") << Acceptable << 0 << 1000 << QString("22");
    BOBUIest::newRow("data19") << QString("22  ") << Acceptable << 0 << 1000 << QString("22");
    BOBUIest::newRow("data20") << QString("  22  ") << Acceptable << 0 << 1000 << QString("22");
    BOBUIest::newRow("data21") << QString("2 2") << Invalid << 0 << 1000 << QString();
}

static QString stateName(int state)
{
    switch (state) {
    case QValidator::Acceptable: return QString("Acceptable");
    case QValidator::Intermediate: return QString("Intermediate");
    case QValidator::Invalid: return QString("Invalid");
    default: break;
    }
    qWarning("%s %d: this should never happen", __FILE__, __LINE__);
    return QString();
}

void tst_QSpinBox::valueFromTextAndValidate()
{
    QFETCH(QString, txt);
    QFETCH(int, state);
    QFETCH(int, mini);
    QFETCH(int, maxi);
    QFETCH(QString, expectedText);

    SpinBox sb(0);
    sb.show();
    sb.setRange(mini, maxi);
    int unused = 0;
    QCOMPARE(stateName(sb.validate(txt, unused)), stateName(state));
    if (!expectedText.isEmpty())
        QCOMPARE(txt, expectedText);
}

void tst_QSpinBox::locale_data()
{
    BOBUIest::addColumn<QLocale>("loc");
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<QString>("textFromVal");
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("valFromText");

    BOBUIest::newRow("data0") << QLocale(QLocale::NorwegianBokmal, QLocale::Norway) << 1234 << QString("1234") << QString("2345") << 2345;
    BOBUIest::newRow("data1") << QLocale(QLocale::German, QLocale::Germany) << 1234 << QString("1234") << QString("2345") << 2345;
}

void tst_QSpinBox::locale()
{
    QFETCH(QLocale, loc);
    QFETCH(int, value);
    QFETCH(QString, textFromVal);
    QFETCH(QString, text);
    QFETCH(int, valFromText);

    const auto restoreDefault = qScopeGuard([prior = QLocale()]() {
        QLocale::setDefault(prior);
    });
    QLocale::setDefault(loc);
    SpinBox box;
    box.setMaximum(100000);
    box.setValue(value);
    QCOMPARE(box.cleanText(), textFromVal);

    box.lineEdit()->setText(text);
    QCOMPARE(box.cleanText(), text);
    box.interpretText();

    QCOMPARE(box.value(), valFromText);
}


void tst_QSpinBox::editingFinished()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QWidget testFocusWidget;
    testFocusWidget.setObjectName(QLatin1String("tst_qspinbox"));
    testFocusWidget.setWindowTitle(objectName());
    testFocusWidget.resize(200, 100);

    QVBoxLayout *layout = new QVBoxLayout(&testFocusWidget);
    QSpinBox *box = new QSpinBox(&testFocusWidget);
    layout->addWidget(box);
    QSpinBox *box2 = new QSpinBox(&testFocusWidget);
    layout->addWidget(box2);

    testFocusWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&testFocusWidget));
    box->activateWindow();
    box->setFocus();

    BOBUIRY_COMPARE(qApp->focusWidget(), (QWidget *)box);

    QSignalSpy editingFinishedSpy1(box, SIGNAL(editingFinished()));
    QSignalSpy editingFinishedSpy2(box2, SIGNAL(editingFinished()));

    box->setFocus();
    BOBUIest::keyClick(box, BobUI::Key_Up);
    BOBUIest::keyClick(box, BobUI::Key_Up);

    QCOMPARE(editingFinishedSpy1.size(), 0);
    QCOMPARE(editingFinishedSpy2.size(), 0);

    BOBUIest::keyClick(box2, BobUI::Key_Up);
    BOBUIest::keyClick(box2, BobUI::Key_Up);
    box2->setFocus();
    QCOMPARE(editingFinishedSpy1.size(), 1);
    box->setFocus();
    QCOMPARE(editingFinishedSpy1.size(), 1);
    QCOMPARE(editingFinishedSpy2.size(), 1);
    BOBUIest::keyClick(box, BobUI::Key_Up);
    QCOMPARE(editingFinishedSpy1.size(), 1);
    QCOMPARE(editingFinishedSpy2.size(), 1);
    BOBUIest::keyClick(box, BobUI::Key_Enter);
    QCOMPARE(editingFinishedSpy1.size(), 2);
    QCOMPARE(editingFinishedSpy2.size(), 1);
    BOBUIest::keyClick(box, BobUI::Key_Return);
    QCOMPARE(editingFinishedSpy1.size(), 3);
    QCOMPARE(editingFinishedSpy2.size(), 1);
    box2->setFocus();
    QCOMPARE(editingFinishedSpy1.size(), 4);
    QCOMPARE(editingFinishedSpy2.size(), 1);
    BOBUIest::keyClick(box2, BobUI::Key_Enter);
    QCOMPARE(editingFinishedSpy1.size(), 4);
    QCOMPARE(editingFinishedSpy2.size(), 2);
    BOBUIest::keyClick(box2, BobUI::Key_Return);
    QCOMPARE(editingFinishedSpy1.size(), 4);
    QCOMPARE(editingFinishedSpy2.size(), 3);

    testFocusWidget.hide();
    QCOMPARE(editingFinishedSpy1.size(), 4);
    QCOMPARE(editingFinishedSpy2.size(), 4);

    //task203285
    editingFinishedSpy1.clear();
    testFocusWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&testFocusWidget));
    box->setKeyboardTracking(false);
    QApplicationPrivate::setActiveWindow(&testFocusWidget);
    testFocusWidget.activateWindow();
    box->setFocus();
    BOBUIRY_VERIFY(box->hasFocus());
    box->setValue(0);
    BOBUIest::keyClick(box, '2');
    QCOMPARE(box->text(), QLatin1String("20"));
    box2->setFocus();
    BOBUIRY_VERIFY(qApp->focusWidget() != box);
    QCOMPARE(box->text(), QLatin1String("20"));
    QCOMPARE(editingFinishedSpy1.size(), 1);
}

void tst_QSpinBox::returnPressed()
{
    QSpinBox spinBox;
    QSignalSpy spyCurrentChanged(&spinBox, &QSpinBox::returnPressed);
    spinBox.show();
    BOBUIest::keyClick(&spinBox, BobUI::Key_Return);
    QCOMPARE(spyCurrentChanged.size(), 1);
}

void tst_QSpinBox::removeAll()
{
    SpinBox spin(0);
    spin.setPrefix("foo");
    spin.setSuffix("bar");
    spin.setValue(2);
    spin.show();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(&spin, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(&spin, BobUI::Key_Home);
#endif

#ifdef Q_OS_MAC
    BOBUIest::keyClick(&spin, BobUI::Key_Right, BobUI::ControlModifier|BobUI::ShiftModifier);
#else
    BOBUIest::keyClick(&spin, BobUI::Key_End, BobUI::ShiftModifier);
#endif

    QCOMPARE(spin.lineEdit()->selectedText(), QString("foo2bar"));
    BOBUIest::keyClick(&spin, BobUI::Key_1);
    QCOMPARE(spin.text(), QString("foo1bar"));
}

void tst_QSpinBox::startWithDash()
{
    SpinBox spin(0);
    spin.show();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(&spin, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(&spin, BobUI::Key_Home);
#endif
    QCOMPARE(spin.text(), QString("0"));
    BOBUIest::keyClick(&spin, BobUI::Key_Minus);
    QCOMPARE(spin.text(), QString("0"));
}

#if BOBUI_CONFIG(shortcut)

void tst_QSpinBox::undoRedo()
{
    //test undo/redo feature (in conjunction with the "undoRedoEnabled" property)
    SpinBox spin(0);
    spin.show();

    //the undo/redo is disabled by default

    QCOMPARE(spin.value(), 0); //this is the default value
    QVERIFY(!spin.lineEdit()->isUndoAvailable());
    QVERIFY(!spin.lineEdit()->isRedoAvailable());

    spin.lineEdit()->selectAll(); //ensures everything is selected and will be cleared by typing a key
    BOBUIest::keyClick(&spin, BobUI::Key_1); //we put 1 into the spinbox
    QCOMPARE(spin.value(), 1);
    QVERIFY(spin.lineEdit()->isUndoAvailable());

    //testing CTRL+Z (undo)
    int val = QKeySequence(QKeySequence::Undo)[0].toCombined();
    BobUI::KeyboardModifiers mods = (BobUI::KeyboardModifiers)(val & BobUI::KeyboardModifierMask);
    BOBUIest::keyClick(&spin, val & ~mods, mods);

    QCOMPARE(spin.value(), 0);
    QVERIFY(!spin.lineEdit()->isUndoAvailable());
    QVERIFY(spin.lineEdit()->isRedoAvailable());

    //testing CTRL+Y (redo)
    val = QKeySequence(QKeySequence::Redo)[0].toCombined();
    mods = (BobUI::KeyboardModifiers)(val & BobUI::KeyboardModifierMask);
    BOBUIest::keyClick(&spin, val & ~mods, mods);
    QCOMPARE(spin.value(), 1);
    QVERIFY(!spin.lineEdit()->isRedoAvailable());
    QVERIFY(spin.lineEdit()->isUndoAvailable());

    spin.setValue(55);
    QVERIFY(!spin.lineEdit()->isUndoAvailable());
    QVERIFY(!spin.lineEdit()->isRedoAvailable());

    BOBUIest::keyClick(&spin, BobUI::Key_Return);
    BOBUIest::keyClick(&spin, '1');
    QVERIFY(spin.lineEdit()->isUndoAvailable());
    QVERIFY(!spin.lineEdit()->isRedoAvailable());
    spin.lineEdit()->undo();
    QCOMPARE(spin.value(), 55);
    QVERIFY(!spin.lineEdit()->isUndoAvailable());
    QVERIFY(spin.lineEdit()->isRedoAvailable());
    spin.lineEdit()->redo();
    QCOMPARE(spin.value(), 1);
    QVERIFY(spin.lineEdit()->isUndoAvailable());
    QVERIFY(!spin.lineEdit()->isRedoAvailable());
}

#endif // BOBUI_CONFIG(shortcut)

void tst_QSpinBox::specialValue()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QString specialText="foo";

    QWidget topWidget;
    QVBoxLayout layout(&topWidget);
    SpinBox spin(&topWidget);
    layout.addWidget(&spin);
    SpinBox box2(&topWidget);
    layout.addWidget(&box2);

    spin.setSpecialValueText(specialText);
    spin.setMinimum(0);
    spin.setMaximum(100);
    spin.setValue(50);
    topWidget.show();
    //make sure we have the focus (even if editingFinished fails)
    topWidget.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&topWidget));
    spin.setFocus();

    BOBUIest::keyClick(&spin, BobUI::Key_Return);
    BOBUIest::keyClick(&spin, '0');
    QCOMPARE(spin.text(), QString("0"));
    BOBUIest::keyClick(&spin, BobUI::Key_Return);
    QCOMPARE(spin.text(), specialText);

    spin.setValue(50);
    BOBUIest::keyClick(&spin, BobUI::Key_Return);
    BOBUIest::keyClick(&spin, '0');
    QCOMPARE(spin.text(), QString("0"));
    BOBUIest::keyClick(spin.lineEdit(), BobUI::Key_Tab);
    QCOMPARE(spin.text(), specialText);

    spin.setValue(50);
    spin.setFocus();
    BOBUIest::keyClick(&spin, BobUI::Key_Return);
    BOBUIest::keyClick(&spin, '0');
    QCOMPARE(spin.text(), QString("0"));
    box2.setFocus();
    QCOMPARE(spin.text(), specialText);
}

void tst_QSpinBox::textFromValue()
{
    SpinBox spinBox;
    QCOMPARE(spinBox.textFromValue(INT_MIN), QString::number(INT_MIN));
}

class sizeHint_SpinBox : public QSpinBox
{
public:
    QSize sizeHint() const override
    {
        ++sizeHintRequests;
        return QSpinBox::sizeHint();
    }
    mutable int sizeHintRequests;
};

void tst_QSpinBox::sizeHint()
{
    QWidget widget;
    QHBoxLayout *layout = new QHBoxLayout(&widget);

    sizeHint_SpinBox *spinBox = new sizeHint_SpinBox;
    layout->addWidget(spinBox);
    // Make sure all layout requests posted by the QHBoxLayout constructor and addWidget
    // are processed before the widget is shown
    QCoreApplication::sendPostedEvents(&widget, QEvent::LayoutRequest);
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    // Prefix
    spinBox->sizeHintRequests = 0;
    spinBox->setPrefix(QLatin1String("abcdefghij"));
    BOBUIRY_VERIFY(spinBox->sizeHintRequests > 0);

    // Suffix
    spinBox->sizeHintRequests = 0;
    spinBox->setSuffix(QLatin1String("abcdefghij"));
    BOBUIRY_VERIFY(spinBox->sizeHintRequests > 0);

    // Range
    spinBox->sizeHintRequests = 0;
    spinBox->setRange(0, 1234567890);
    spinBox->setValue(spinBox->maximum());
    BOBUIRY_VERIFY(spinBox->sizeHintRequests > 0);
}

void tst_QSpinBox::taskBOBUIBUG_5008_textFromValueAndValidate()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    class DecoratedSpinBox : public QSpinBox
    {
    public:
        DecoratedSpinBox()
        {
            setLocale(QLocale::French);
            setMaximum(100000000);
            setValue(1000000);
        }

        QLineEdit *lineEdit() const
        {
            return QSpinBox::lineEdit();
        }

        //we use the French delimiters here
        QString textFromValue (int value) const override
        {
            return locale().toString(value);
        }

    } spinbox;
    spinbox.show();
    spinbox.activateWindow();
    spinbox.setFocus();
    QVERIFY(BOBUIest::qWaitForWindowActive(&spinbox));
    QVERIFY(spinbox.hasFocus());
    BOBUIRY_COMPARE(static_cast<QWidget *>(&spinbox), QApplication::activeWindow());
    QCOMPARE(spinbox.text(), spinbox.locale().toString(spinbox.value()));
    spinbox.lineEdit()->setCursorPosition(2); //just after the first thousand separator
    BOBUIest::keyClick(static_cast<QWidget *>(0), BobUI::Key_0); // let's insert a 0
    QCOMPARE(spinbox.value(), 10000000); //it's been multiplied by 10
    spinbox.clearFocus(); //make sure the value is correctly formatted
    QCOMPARE(spinbox.text(), spinbox.locale().toString(spinbox.value()));
}

void tst_QSpinBox::integerOverflow()
{
    QSpinBox sb;
    sb.setRange(INT_MIN, INT_MAX);

    sb.setValue(INT_MAX - 1);
    sb.stepUp();
    QCOMPARE(sb.value(), INT_MAX);
    sb.stepUp();
    QCOMPARE(sb.value(), INT_MAX);

    sb.setValue(INT_MIN + 1);
    sb.stepDown();
    QCOMPARE(sb.value(), INT_MIN);
    sb.stepDown();
    QCOMPARE(sb.value(), INT_MIN);

    sb.setValue(0);
    QCOMPARE(sb.value(), 0);
    sb.setSingleStep(INT_MAX);
    sb.stepUp();
    QCOMPARE(sb.value(), INT_MAX);
    sb.stepDown();
    QCOMPARE(sb.value(), 0);
    sb.stepDown();
    QCOMPARE(sb.value(), INT_MIN + 1);
    sb.stepDown();
    QCOMPARE(sb.value(), INT_MIN);
}

void tst_QSpinBox::lineEditReturnPressed()
{
    SpinBox spinBox;
    QSignalSpy spyCurrentChanged(spinBox.lineEdit(), SIGNAL(returnPressed()));
    spinBox.show();
    BOBUIest::keyClick(&spinBox, BobUI::Key_Return);
    QCOMPARE(spyCurrentChanged.size(), 1);
}

void tst_QSpinBox::positiveSign()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QSpinBox spinBox;
    spinBox.setRange(-20, 20);
    spinBox.setValue(-20);
    spinBox.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&spinBox));

    BOBUIest::keyClick(&spinBox, BobUI::Key_End, BobUI::ShiftModifier);
    BOBUIest::keyClick(&spinBox, BobUI::Key_Plus, BobUI::ShiftModifier);
    BOBUIest::keyClick(&spinBox, BobUI::Key_2);
    BOBUIest::keyClick(&spinBox, BobUI::Key_0);
    QCOMPARE(spinBox.text(), QLatin1String("+20"));
}

void tst_QSpinBox::interpretOnLosingFocus()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    // BOBUIBUG-55249: When typing an invalid value after QSpinBox::clear(),
    // it should be fixed up on losing focus.

    static const int minimumValue = 10;
    static const int maximumValue = 20;

    QWidget widget;
    widget.setWindowTitle(BOBUIest::currentTestFunction());
    QVBoxLayout *layout = new QVBoxLayout(&widget);
    QLineEdit *focusDummy = new QLineEdit("focusDummy", &widget);
    layout->addWidget(focusDummy);
    SpinBox *spinBox = new SpinBox(&widget);
    spinBox->setRange(minimumValue, maximumValue);
    spinBox->setValue(minimumValue);
    layout->addWidget(spinBox);
    spinBox->clear();
    spinBox->setFocus();
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&widget));
    BOBUIest::keyClick(spinBox, BobUI::Key_1); // Too small
    focusDummy->setFocus();
    QCOMPARE(spinBox->value(), minimumValue);
    QCOMPARE(spinBox->lineEdit()->text().toInt(), minimumValue);
}

void tst_QSpinBox::setGroupSeparatorShown_data()
{
    BOBUIest::addColumn<QLocale::Language>("lang");
    BOBUIest::addColumn<QLocale::Territory>("country");

    BOBUIest::newRow("data0") << QLocale::English << QLocale::UnitedStates;
    BOBUIest::newRow("data1") << QLocale::Swedish << QLocale::Sweden;
    BOBUIest::newRow("data2") << QLocale::German << QLocale::Germany;
    BOBUIest::newRow("data3") << QLocale::Georgian << QLocale::Georgia;
    BOBUIest::newRow("data3") << QLocale::Macedonian << QLocale::Macedonia;
}

void tst_QSpinBox::setGroupSeparatorShown()
{
    QFETCH(QLocale::Language, lang);
    QFETCH(QLocale::Territory, country);

    const auto restoreDefault = qScopeGuard([prior = QLocale()]() {
        QLocale::setDefault(prior);
    });
    QLocale loc(lang, country);
    QLocale::setDefault(loc);
    SpinBox spinBox;
    spinBox.setMaximum(99999);
    spinBox.setValue(13000);
    spinBox.setGroupSeparatorShown(true);
    QCOMPARE(spinBox.lineEdit()->text(), spinBox.locale().toString(13000));
    QCOMPARE(spinBox.isGroupSeparatorShown(), true);
    QCOMPARE(spinBox.textFromValue(23421),spinBox.locale().toString(23421));

    spinBox.setGroupSeparatorShown(false);
    QCOMPARE(spinBox.lineEdit()->text(), QStringLiteral("13000"));
    QCOMPARE(spinBox.isGroupSeparatorShown(), false);

    spinBox.setMaximum(72000);
    spinBox.lineEdit()->setText(spinBox.locale().toString(32000));
    QCOMPARE(spinBox.value()+1000, 33000);

    spinBox.lineEdit()->setText(QStringLiteral("32000"));
    QCOMPARE(spinBox.value()+1000, 33000);

    spinBox.lineEdit()->setText(QStringLiteral("32,000"));
    QCOMPARE(spinBox.value()+1000, 33000);
}

void tst_QSpinBox::wheelEvents_data()
{
#if BOBUI_CONFIG(wheelevent)
    BOBUIest::addColumn<QPoint>("angleDelta");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifier");
    BOBUIest::addColumn<BobUI::MouseEventSource>("source");
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<IntList>("expectedValues");

    const auto fractions = {false, true};

    const auto directions = {true, false};

    const auto modifierList = {BobUI::NoModifier,
                               BobUI::ShiftModifier,
                               BobUI::ControlModifier,
                               BobUI::AltModifier,
                               BobUI::MetaModifier};

    const auto validStepModifierList = {BobUI::NoModifier,
                                        BobUI::ControlModifier,
                                        BobUI::ShiftModifier};

    const auto sources = {BobUI::MouseEventNotSynthesized,
                          BobUI::MouseEventSynthesizedBySystem,
                          BobUI::MouseEventSynthesizedByBobUI,
                          BobUI::MouseEventSynthesizedByApplication};

    const int startValue = 0;

    for (auto fraction : fractions) {
        for (auto up : directions) {

            const int units = (fraction ? 60 : 120) * (up ? 1 : -1);

            for (auto modifier : modifierList) {

                const BobUI::KeyboardModifiers modifiers(modifier);

                const auto modifierName = modifierToName(modifier);
                if (modifierName.isEmpty())
                    continue;

                for (auto stepModifier : validStepModifierList) {

                    const auto stepModifierName = modifierToName(stepModifier);
                    if (stepModifierName.isEmpty())
                        continue;

                    const int steps = (modifier & stepModifier ? 10 : 1)
                            * (up ? 1 : -1);

                    for (auto source : sources) {

#ifdef Q_OS_MACOS
                        QPoint angleDelta;
                        if ((modifier & BobUI::ShiftModifier) &&
                                source == BobUI::MouseEventNotSynthesized) {
                            // On macOS the Shift modifier converts vertical
                            // mouse wheel events to horizontal.
                            angleDelta = { units, 0 };
                        } else {
                            // However, this is not the case for trackpad scroll
                            // events.
                            angleDelta = { 0, units };
                        }
#else
                        const QPoint angleDelta(0, units);
#endif

                        QLatin1String sourceName;
                        switch (source) {
                        case BobUI::MouseEventNotSynthesized:
                            sourceName = QLatin1String("NotSynthesized");
                            break;
                        case BobUI::MouseEventSynthesizedBySystem:
                            sourceName = QLatin1String("SynthesizedBySystem");
                            break;
                        case BobUI::MouseEventSynthesizedByBobUI:
                            sourceName = QLatin1String("SynthesizedByBobUI");
                            break;
                        case BobUI::MouseEventSynthesizedByApplication:
                            sourceName = QLatin1String("SynthesizedByApplication");
                            break;
                        default:
                            qFatal("Unexpected wheel event source");
                            continue;
                        }

                        IntList expectedValues;
                        if (fraction)
                            expectedValues << startValue;
                        expectedValues << startValue + steps;

                        BOBUIest::addRow("%s%s%sWith%sKeyboardModifier%s",
                                      fraction ? "half" : "full",
                                      up ? "Up" : "Down",
                                      stepModifierName.latin1(),
                                      modifierName.latin1(),
                                      sourceName.latin1())
                                << angleDelta
                                << static_cast<int>(stepModifier)
                                << modifiers
                                << source
                                << startValue
                                << expectedValues;
                    }
                }
            }
        }
    }
#else
    QSKIP("Built with --no-feature-wheelevent");
#endif
}

void tst_QSpinBox::wheelEvents()
{
#if BOBUI_CONFIG(wheelevent)
    QFETCH(QPoint, angleDelta);
    QFETCH(int, stepModifier);
    QFETCH(BobUI::KeyboardModifiers, modifier);
    QFETCH(BobUI::MouseEventSource, source);
    QFETCH(int, start);
    QFETCH(IntList, expectedValues);

    SpinBox spinBox;
    spinBox.setRange(-20, 20);
    spinBox.setValue(start);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> style(
                new StepModifierStyle);
    style->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    spinBox.setStyle(style.data());

    QWheelEvent event(QPointF(), QPointF(), QPoint(), angleDelta,
                      BobUI::NoButton, modifier, BobUI::NoScrollPhase, false, source);
    for (int expected : expectedValues) {
        qApp->sendEvent(&spinBox, &event);
        QCOMPARE(spinBox.value(), expected);
    }
#else
    QSKIP("Built with --no-feature-wheelevent");
#endif
}

void tst_QSpinBox::adaptiveDecimalStep()
{
    SpinBox spinBox;
    spinBox.setRange(-100000, 100000);
    spinBox.setStepType(SpinBox::StepType::AdaptiveDecimalStepType);

    // Positive values

    spinBox.setValue(0);

    // Go from 0 to 100
    for (int i = 0; i < 100; i++) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(1);
    }

    // Go from 100 to 1000
    for (int i = 100; i < 1000; i += 10) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(1);
    }

    // Go from 1000 to 10000
    for (int i = 1000; i < 10000; i += 100) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(1);
    }

    // Test decreasing the values now

    // Go from 10000 down to 1000
    for (int i = 10000; i > 1000; i -= 100) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(-1);
    }

    // Go from 1000 down to 100
    for (int i = 1000; i > 100; i -= 10) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(-1);
    }

    // Negative values

    spinBox.setValue(0);

    // Go from 0 to -100
    for (int i = 0; i > -100; i--) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(-1);
    }

    // Go from -100 to -1000
    for (int i = -100; i > -1000; i -= 10) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(-1);
    }

    // Go from 1000 to 10000
    for (int i = -1000; i > -10000; i -= 100) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(-1);
    }

    // Test increasing the values now

    // Go from -10000 up to -1000
    for (int i = -10000; i < -1000; i += 100) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(1);
    }

    // Go from -1000 up to -100
    for (int i = -1000; i < -100; i += 10) {
        QCOMPARE(spinBox.value(), i);
        spinBox.stepBy(1);
    }
}

void tst_QSpinBox::stepModifierKeys_data()
{
    BOBUIest::addColumn<int>("startValue");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<int>("expectedValue");

    const auto keyList = {BobUI::Key_Up, BobUI::Key_Down};

    const auto modifierList = {BobUI::NoModifier,
                               BobUI::ShiftModifier,
                               BobUI::ControlModifier,
                               BobUI::AltModifier,
                               BobUI::MetaModifier};

    const auto validStepModifierList = {BobUI::NoModifier,
                                        BobUI::ControlModifier,
                                        BobUI::ShiftModifier};

    for (auto key : keyList) {

        const bool up = key == BobUI::Key_Up;
        Q_ASSERT(up || key == BobUI::Key_Down);

        const int startValue = up ? 0.0 : 10.0;

        for (auto modifier : modifierList) {

            BOBUIestEventList keys;
            keys.addKeyClick(key, modifier);

            const auto modifierName = modifierToName(modifier);
            if (modifierName.isEmpty())
                continue;

            for (auto stepModifier : validStepModifierList) {

                const auto stepModifierName = modifierToName(stepModifier);
                if (stepModifierName.isEmpty())
                    continue;

                const int steps = (modifier & stepModifier ? 10 : 1)
                        * (up ? 1 : -1);

                const int expectedValue = startValue + steps;

                BOBUIest::addRow("%s%sWith%sKeyboardModifier",
                              up ? "up" : "down",
                              stepModifierName.latin1(),
                              modifierName.latin1())
                        << startValue
                        << static_cast<int>(stepModifier)
                        << keys
                        << expectedValue;
            }
        }
    }
}

void tst_QSpinBox::stepModifierKeys()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(int, startValue);
    QFETCH(int, stepModifier);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(int, expectedValue);

    QSpinBox spin(0);
    spin.setValue(startValue);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> style(
                new StepModifierStyle);
    style->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    spin.setStyle(style.data());

    spin.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&spin));

    QCOMPARE(spin.value(), startValue);
    keys.simulate(&spin);
    QCOMPARE(spin.value(), expectedValue);
}

void tst_QSpinBox::stepModifierButtons_data()
{
    BOBUIest::addColumn<QStyle::SubControl>("subControl");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<int>("startValue");
    BOBUIest::addColumn<int>("expectedValue");

    const auto subControls = {QStyle::SC_SpinBoxUp, QStyle::SC_SpinBoxDown};

    const auto modifierList = {BobUI::NoModifier,
                               BobUI::ShiftModifier,
                               BobUI::ControlModifier,
                               BobUI::AltModifier,
                               BobUI::MetaModifier};

    const auto validStepModifierList = {BobUI::NoModifier,
                                        BobUI::ControlModifier,
                                        BobUI::ShiftModifier};

    for (auto subControl : subControls) {

        const bool up = subControl == QStyle::SC_SpinBoxUp;
        Q_ASSERT(up || subControl == QStyle::SC_SpinBoxDown);

        const int startValue = up ? 0 : 10;

        for (auto modifier : modifierList) {

            const BobUI::KeyboardModifiers modifiers(modifier);

            const auto modifierName = modifierToName(modifier);
            if (modifierName.isEmpty())
                continue;

            for (auto stepModifier : validStepModifierList) {

                const auto stepModifierName = modifierToName(stepModifier);
                if (stepModifierName.isEmpty())
                    continue;

                const int steps = (modifier & stepModifier ? 10 : 1)
                        * (up ? 1 : -1);

                const int expectedValue = startValue + steps;

                BOBUIest::addRow("%s%sWith%sKeyboardModifier",
                              up ? "up" : "down",
                              stepModifierName.latin1(),
                              modifierName.latin1())
                        << subControl
                        << static_cast<int>(stepModifier)
                        << modifiers
                        << startValue
                        << expectedValue;
            }
        }
    }
}

void tst_QSpinBox::stepModifierButtons()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(QStyle::SubControl, subControl);
    QFETCH(int, stepModifier);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(int, startValue);
    QFETCH(int, expectedValue);

    SpinBox spin(0);
    spin.setRange(-20, 20);
    spin.setValue(startValue);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> style(
                new StepModifierStyle);
    style->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    spin.setStyle(style.data());

    spin.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&spin));

    QStyleOptionSpinBox spinBoxStyleOption;
    spin.initStyleOption(&spinBoxStyleOption);

    const QRect buttonRect = spin.style()->subControlRect(
                QStyle::CC_SpinBox, &spinBoxStyleOption, subControl, &spin);

    QCOMPARE(spin.value(), startValue);
    BOBUIest::mouseClick(&spin, BobUI::LeftButton, modifiers, buttonRect.center());
    QCOMPARE(spin.value(), expectedValue);
}

void tst_QSpinBox::stepModifierPressAndHold_data()
{
    BOBUIest::addColumn<QStyle::SubControl>("subControl");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<int>("expectedStepModifier");

    const auto subControls = {QStyle::SC_SpinBoxUp, QStyle::SC_SpinBoxDown};

    const auto modifierList = {BobUI::NoModifier,
                               BobUI::ShiftModifier,
                               BobUI::ControlModifier,
                               BobUI::AltModifier,
                               BobUI::MetaModifier};

    const auto validStepModifierList = {BobUI::NoModifier,
                                        BobUI::ControlModifier,
                                        BobUI::ShiftModifier};

    for (auto subControl : subControls) {

        const bool up = subControl == QStyle::SC_SpinBoxUp;
        Q_ASSERT(up || subControl == QStyle::SC_SpinBoxDown);

        for (auto modifier : modifierList) {

            const BobUI::KeyboardModifiers modifiers(modifier);

            const auto modifierName = modifierToName(modifier);
            if (modifierName.isEmpty())
                continue;

            for (auto stepModifier : validStepModifierList) {

                const auto stepModifierName = modifierToName(stepModifier);
                if (stepModifierName.isEmpty())
                    continue;

                const int steps = (modifier & stepModifier ? 10 : 1)
                        * (up ? 1 : -1);

                BOBUIest::addRow("%s%sWith%sKeyboardModifier",
                              up ? "up" : "down",
                              stepModifierName.latin1(),
                              modifierName.latin1())
                        << subControl
                        << static_cast<int>(stepModifier)
                        << modifiers
                        << steps;
            }
        }
    }
}

void tst_QSpinBox::stepModifierPressAndHold()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(QStyle::SubControl, subControl);
    QFETCH(int, stepModifier);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(int, expectedStepModifier);

    SpinBox spin(0);
    spin.setRange(-100, 100);
    spin.setValue(0);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> stepModifierStyle(
                new StepModifierStyle(new PressAndHoldStyle));
    stepModifierStyle->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    spin.setStyle(stepModifierStyle.data());

    QSignalSpy spy(&spin, &SpinBox::valueChanged);

    spin.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&spin));

    QStyleOptionSpinBox spinBoxStyleOption;
    spin.initStyleOption(&spinBoxStyleOption);

    const QRect buttonRect = spin.style()->subControlRect(
                QStyle::CC_SpinBox, &spinBoxStyleOption, subControl, &spin);

    // TODO: remove debug output when BOBUIBUG-69492 is fixed
    qDebug() << "QGuiApplication::focusWindow():" << QGuiApplication::focusWindow();
    qDebug() << "QGuiApplication::topLevelWindows():" << QGuiApplication::topLevelWindows();
    BOBUIest::mousePress(&spin, BobUI::LeftButton, modifiers, buttonRect.center());
    BOBUIRY_VERIFY2(spy.size() >= 3, qPrintable(QString::fromLatin1(
        "Expected valueChanged() to be emitted 3 or more times, but it was only emitted %1 times").arg(spy.size())));
    BOBUIest::mouseRelease(&spin, BobUI::LeftButton, modifiers, buttonRect.center());

    const auto value = spy.last().at(0);
    QVERIFY(value.metaType().id() == QMetaType::Int);
    QCOMPARE(value.toInt(), spy.size() * expectedStepModifier);
}

void tst_QSpinBox::stepSelectAll_data()
{
    BOBUIest::addColumn<bool>("stepShouldSelectAll");
    BOBUIest::addColumn<QStringList>("selectedText");

    BOBUIest::addRow("select all") << true << QStringList{"1", "0", "5", "4", "9"};
    BOBUIest::addRow("don't select all") << false << QStringList{{}, {}, {}, {}, "94"};
}

void tst_QSpinBox::stepSelectAll()
{
    QFETCH(bool, stepShouldSelectAll);
    QFETCH(QStringList, selectedText);
    SelectAllOnStepStyle style(stepShouldSelectAll);

    SpinBox spinBox;
    spinBox.setStyle(&style);

    QCOMPARE(spinBox.lineEdit()->selectedText(), QString());

    auto it = selectedText.cbegin();
    spinBox.stepUp();
    QCOMPARE(spinBox.lineEdit()->selectedText(), *(it++));
    spinBox.lineEdit()->deselect();
    spinBox.stepDown();
    QCOMPARE(spinBox.lineEdit()->selectedText(), *(it++));
    spinBox.lineEdit()->deselect();
    spinBox.stepBy(5);
    QCOMPARE(spinBox.lineEdit()->selectedText(), *(it++));
    spinBox.lineEdit()->deselect();
    BOBUIest::keyClick(&spinBox, BobUI::Key_Down);
    QCOMPARE(spinBox.lineEdit()->selectedText(), *(it++));
    BOBUIest::keyClicks(&spinBox, "9");
    QCOMPARE(spinBox.lineEdit()->selectedText(), QString());
    QCOMPARE(spinBox.lineEdit()->text(), *(it++));
}

BOBUIEST_MAIN(tst_QSpinBox)
#include "tst_qspinbox.moc"
