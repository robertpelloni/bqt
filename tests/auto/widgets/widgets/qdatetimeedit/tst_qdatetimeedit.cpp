// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qapplication.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qlocale.h>
#include <qlayout.h>
#include <qeventloop.h>
#include <qstyle.h>

#include <BOBUIest>
#include <QStyleOptionSpinBox>
#include <QDate>
#include <QDateTime>
#include <BOBUIime>
#include <QList>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QWidget>
#include <QLineEdit>
#include <QObject>
#include <QLocale>
#include <QString>
#include <QVariantList>
#include <QVariant>
#include <QApplication>
#include <QPoint>
#include <QVBoxLayout>
#include <QRect>
#include <QCursor>
#include <QEventLoop>
#include <QStyle>
#include <QStyleOptionComboBox>
#include <BOBUIimeEdit>
#include <QMetaType>
#include <QDebug>
#include <QWheelEvent>
#include <BOBUIest>
#include <QSignalSpy>
#include <BOBUIestEventList>
#include <QDateEdit>
#include <QProxyStyle>
#include <BOBUIimeZone>
#include <QScopeGuard>

#include <private/qdatetimeedit_p.h>

#include <BobUIWidgets/private/qapplication_p.h>

#ifdef Q_OS_WIN
# include <bobui_windows.h>
#endif


Q_DECLARE_METATYPE(BobUI::Key);
Q_DECLARE_METATYPE(BobUI::KeyboardModifiers);
Q_DECLARE_METATYPE(BobUI::KeyboardModifier);

class EditorDateEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    EditorDateEdit(QWidget *parent = nullptr) : QDateTimeEdit(parent) {}
    QLineEdit *lineEdit() { return QDateTimeEdit::lineEdit(); }
    friend class tst_QDateTimeEdit;
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

class tst_QDateTimeEdit : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void cachedDayTest();
    void getSetCheck();
    void constructor_qwidget();
    void constructor_qdatetime_data();
    void constructor_qdatetime();
    void constructor_qdate_data();
    void constructor_qdate();
    void constructor_bobuiime_data();
    void constructor_bobuiime();

    void sectionText_data();
    void sectionText();
    void dateTimeSignalChecking_data();
    void dateTimeSignalChecking();
    void mousePress();
    void stepHourAMPM_data();

    void stepHourAMPM();
    void displayedSections_data();
    void displayedSections();
    void currentSection_data();
    void currentSection();

    void setCurrentSection();
    void setCurrentSection_data();

    void minimumDate_data();
    void minimumDate();
    void maximumDate_data();
    void maximumDate();
    void clearMinimumDate_data();
    void clearMinimumDate();
    void clearMaximumDate_data();
    void clearMaximumDate();

    void minimumDateTime_data();
    void minimumDateTime();
    void maximumDateTime_data();
    void maximumDateTime();

    void clearMinimumDateTime_data();
    void clearMinimumDateTime();
    void clearMaximumDateTime_data();
    void clearMaximumDateTime();

    void displayFormat_data();
    void displayFormat();

    void specialValueText();
    void setRange_data();
    void setRange();
    void editingRanged_data();
    void editingRanged();

    void selectAndScrollWithKeys();
    void backspaceKey();
    void deleteKey();
    void tabKeyNavigation();
    void tabKeyNavigationWithPrefix();
    void tabKeyNavigationWithSuffix();
    void enterKey();

    void readOnly();

    void wrappingDate_data();
    void wrappingDate();

    void dateSignalChecking_data();
    void dateSignalChecking();

    void wrappingTime_data();
    void wrappingTime();

    void userKeyPress_Time_data();
    void userKeyPress_Time();

    void timeSignalChecking_data();
    void timeSignalChecking();

    void weirdCase();
    void newCase();
    void newCase2();
    void newCase3();
    void newCase4();
    void newCase5();
    void newCase6();

    void task98554();
    void task149097();

    void cursorPos();
    void calendarPopup();

    void hour12Test();
    void yyTest();
    void task108572();

    void task148725();
    void task148522();

    void setSelectedSection();
    void reverseTest();

    void ddMMMMyyyy();
    void wheelEvent_data();
    void wheelEvent();

    void specialValueCornerCase();
    void cursorPositionOnInit();

    void task118867();

    void nextPrevSection_data();
    void nextPrevSection();

    void dateEditTimeEditFormats();
#if BOBUI_DEPRECATED_SINCE(6, 10)
    void timeSpec_data();
    void timeSpec();
#endif
    void timeZoneBug();
    void timeZoneInit();
    void setDateTime_data();
    void setDateTime();

    void monthEdgeCase();
    void setLocale();

    void potentialYYValueBug();
    void textSectionAtEnd();

    void keypadAutoAdvance_data();
    void keypadAutoAdvance();

    void task196924();
    void focusNextPrevChild();

    void taskBOBUIBUG_12384_timeZoneShowTimeOnly();

    void deleteCalendarWidget();

    void setLocaleOnCalendarWidget();

#ifdef BOBUI_BUILD_INTERNAL
    void dateEditCorrectSectionSize_data();
    void dateEditCorrectSectionSize();
#endif

    void stepModifierKeys_data();
    void stepModifierKeys();

    void stepModifierButtons_data();
    void stepModifierButtons();

    void stepModifierPressAndHold_data();
    void stepModifierPressAndHold();

    void springForward_data();
    void springForward();

    void stepIntoDSTGap_data();
    void stepIntoDSTGap();

private:
    EditorDateEdit* testWidget;
    QWidget *testFocusWidget;
};

typedef QList<QDate> DateList;
typedef QList<BOBUIime> TimeList;
typedef QList<BobUI::Key> KeyList;

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

static QLatin1String sectionToName(const QDateTimeEdit::Section section)
{
    switch (section) {
    case QDateTimeEdit::SecondSection:
        return QLatin1String("Second");
    case QDateTimeEdit::MinuteSection:
        return QLatin1String("Minute");
    case QDateTimeEdit::HourSection:
        return QLatin1String("Hours");
    case QDateTimeEdit::DaySection:
        return QLatin1String("Day");
    case QDateTimeEdit::MonthSection:
        return QLatin1String("Month");
    case QDateTimeEdit::YearSection:
        return QLatin1String("Year");
    default:
        qFatal("Unexpected section");
        return QLatin1String();
    }
}

static QDate stepDate(QDate startDate, const QDateTimeEdit::Section section, const int steps)
{
    switch (section) {
    case QDateTimeEdit::DaySection:
        return startDate.addDays(steps);
    case QDateTimeEdit::MonthSection:
        return startDate.addMonths(steps);
    case QDateTimeEdit::YearSection:
        return startDate.addYears(steps);
    default:
        qFatal("Unexpected section");
        return QDate();
    }
}

static BOBUIime stepTime(BOBUIime startTime, const QDateTimeEdit::Section section, const int steps)
{
    switch (section) {
    case QDateTimeEdit::SecondSection:
        return startTime.addSecs(steps);
    case QDateTimeEdit::MinuteSection:
        return BOBUIime(startTime.hour(),
                     startTime.minute() + steps,
                     startTime.second());
    case QDateTimeEdit::HourSection:
        return BOBUIime(startTime.hour() + steps,
                     startTime.minute(),
                     startTime.second());
    default:
        qFatal("Unexpected section");
        return BOBUIime();
    }
}

// Testing get/set functions
void tst_QDateTimeEdit::getSetCheck()
{
    QDateTimeEdit obj1;
    QCOMPARE(obj1.inputMethodQuery(BobUI::ImHints), QVariant(int(BobUI::ImhPreferNumbers)));
    obj1.setDisplayFormat("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z AP");
    // Section QDateTimeEdit::currentSection()
    // void QDateTimeEdit::setCurrentSection(Section)
    obj1.setCurrentSection(QDateTimeEdit::NoSection);
    QVERIFY(obj1.currentSection() != QDateTimeEdit::NoSection);
    obj1.setCurrentSection(QDateTimeEdit::AmPmSection);
    QCOMPARE(QDateTimeEdit::AmPmSection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::MSecSection);
    QCOMPARE(QDateTimeEdit::MSecSection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::SecondSection);
    QCOMPARE(QDateTimeEdit::SecondSection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::MinuteSection);
    QCOMPARE(QDateTimeEdit::MinuteSection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::HourSection);
    QCOMPARE(QDateTimeEdit::HourSection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::DaySection);
    QCOMPARE(QDateTimeEdit::DaySection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::MonthSection);
    QCOMPARE(QDateTimeEdit::MonthSection, obj1.currentSection());
    obj1.setCurrentSection(QDateTimeEdit::YearSection);
    QCOMPARE(QDateTimeEdit::YearSection, obj1.currentSection());

    QDateEdit dateEdit;
    QCOMPARE(dateEdit.inputMethodQuery(BobUI::ImHints), QVariant(int(BobUI::ImhPreferNumbers)));
    BOBUIimeEdit timeEdit;
    QCOMPARE(timeEdit.inputMethodQuery(BobUI::ImHints), QVariant(int(BobUI::ImhPreferNumbers)));
}

void tst_QDateTimeEdit::initTestCase()
{
    QLocale system = QLocale::system();
    if (system.language() != QLocale::C && system.language() != QLocale::English)
        qWarning("Running under locale %s/%s -- this test may generate failures due to language differences",
                 qPrintable(QLocale::languageToString(system.language())),
                 qPrintable(QLocale::territoryToString(system.territory())));
    testWidget = new EditorDateEdit;
    testFocusWidget = new QWidget(nullptr);
    testFocusWidget->resize(200, 100);
    testFocusWidget->show();
}

void tst_QDateTimeEdit::cleanupTestCase()
{
    delete testFocusWidget;
    testFocusWidget = 0;
    delete testWidget;
    testWidget = 0;
}

void tst_QDateTimeEdit::init()
{
    QLocale::setDefault(QLocale(QLocale::C));
#if defined(Q_OS_WIN)
    SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
#endif
    testWidget->setDisplayFormat("dd/MM/yyyy"); // Nice default to have
    testWidget->setDateTime(QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0)));
    testWidget->show();
    testFocusWidget->move(-1000, -1000);
}

void tst_QDateTimeEdit::cleanup()
{
    testWidget->clearMinimumDateTime();
    testWidget->clearMaximumDateTime();
    testWidget->setTimeZone(BOBUIimeZone::LocalTime);
    testWidget->setSpecialValueText(QString());
    testWidget->setWrapping(false);
    // Restore the default.
    testWidget->setCalendarPopup(false);
}

void tst_QDateTimeEdit::constructor_qwidget()
{
    testWidget->hide();
    QDateTimeEdit dte(0);
    dte.show();
    QCOMPARE(dte.dateTime(), QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0, 0, 0)));
    QCOMPARE(dte.minimumDate(), QDate(1752, 9, 14));
    QCOMPARE(dte.minimumTime(), BOBUIime(0, 0, 0, 0));
    QCOMPARE(dte.maximumDate(), QDate(9999, 12, 31));
    QCOMPARE(dte.maximumTime(), BOBUIime(23, 59, 59, 999));
}

void tst_QDateTimeEdit::constructor_qdatetime_data()
{
    BOBUIest::addColumn<QDateTime>("parameter");

    BOBUIest::newRow("normal") << QDateTime(QDate(2004, 6, 16), BOBUIime(13, 46, 32, 764));
    BOBUIest::newRow("invalid") << QDateTime(QDate(9999, 99, 99), BOBUIime(13, 46, 32, 764));
}

void tst_QDateTimeEdit::constructor_qdatetime()
{
    QFETCH(QDateTime, parameter);
    testWidget->hide();

    QDateTimeEdit dte(parameter);
    dte.show();
    if (QByteArrayView(BOBUIest::currentDataTag()) == "invalid")
        QCOMPARE(dte.dateTime(), QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0)));
    else
        QCOMPARE(dte.dateTime(), parameter);
    QCOMPARE(dte.minimumDate(), QDate(1752, 9, 14));
    QCOMPARE(dte.minimumTime(), BOBUIime(0, 0));
    QCOMPARE(dte.maximumDate(), QDate(9999, 12, 31));
    QCOMPARE(dte.maximumTime(), BOBUIime(23, 59, 59, 999));
}

void tst_QDateTimeEdit::constructor_qdate_data()
{
    BOBUIest::addColumn<QDate>("parameter");

    BOBUIest::newRow("normal") << QDate(2004, 6, 16);
    BOBUIest::newRow("invalid") << QDate(9999, 99, 99);
}

void tst_QDateTimeEdit::constructor_qdate()
{
    QFETCH(QDate, parameter);
    testWidget->hide();

    {
        QDateTimeEdit dte(parameter);
        dte.show();
        if (QByteArrayView(BOBUIest::currentDataTag()) == "invalid")
            QCOMPARE(dte.dateTime(), QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0)));
        else
            QCOMPARE(dte.dateTime(), QDateTime(parameter, BOBUIime(0, 0)));
        QCOMPARE(dte.minimumDate(), QDate(1752, 9, 14));
        QCOMPARE(dte.minimumTime(), BOBUIime(0, 0));
        QCOMPARE(dte.maximumDate(), QDate(9999, 12, 31));
        QCOMPARE(dte.maximumTime(), BOBUIime(23, 59, 59, 999));
    }
    {
        QDateEdit dte(parameter);
        dte.show();
        if (QByteArrayView(BOBUIest::currentDataTag()) == "invalid")
            QCOMPARE(dte.date(), QDate(2000, 1, 1));
        else
            QCOMPARE(dte.date(), parameter);
        QCOMPARE(dte.minimumDate(), QDate(1752, 9, 14));
        QCOMPARE(dte.minimumTime(), BOBUIime(0, 0));
        QCOMPARE(dte.maximumDate(), QDate(9999, 12, 31));
        QCOMPARE(dte.maximumTime(), BOBUIime(23, 59, 59, 999));
    }
}

void tst_QDateTimeEdit::constructor_bobuiime_data()
{
    BOBUIest::addColumn<BOBUIime>("parameter");

    BOBUIest::newRow("normal") << BOBUIime(13, 46, 32, 764);
    BOBUIest::newRow("invalid") << BOBUIime(99, 99, 99, 5000);
}

void tst_QDateTimeEdit::constructor_bobuiime()
{
    QFETCH(BOBUIime, parameter);
    testWidget->hide();

    {
        QDateTimeEdit dte(parameter);
        dte.show();
        if (QByteArrayView(BOBUIest::currentDataTag()) == "invalid")
            QCOMPARE(dte.dateTime(), QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0)));
        else
            QCOMPARE(dte.dateTime(), QDateTime(QDate(2000, 1, 1), parameter));
        QCOMPARE(dte.minimumDate(), QDate(2000, 1, 1));
        QCOMPARE(dte.minimumTime(), BOBUIime(0, 0));
        QCOMPARE(dte.maximumDate(), QDate(2000, 1, 1));
        QCOMPARE(dte.maximumTime(), BOBUIime(23, 59, 59, 999));
    }
    {
        BOBUIimeEdit dte(parameter);
        dte.show();
        if (QByteArrayView(BOBUIest::currentDataTag()) == "invalid")
            QCOMPARE(dte.time(), BOBUIime(0, 0));
        else
            QCOMPARE(dte.time(), parameter);
        QCOMPARE(dte.minimumDate(), QDate(2000, 1, 1));
        QCOMPARE(dte.minimumTime(), BOBUIime(0, 0));
        QCOMPARE(dte.maximumDate(), QDate(2000, 1, 1));
        QCOMPARE(dte.maximumTime(), BOBUIime(23, 59, 59, 999));
    }
}

void tst_QDateTimeEdit::minimumDate_data()
{
    BOBUIest::addColumn<QDate>("minimumDate");
    BOBUIest::addColumn<QDate>("expectedMinDate");

    BOBUIest::newRow("normal-0") << QDate(2004, 5, 10) << QDate(2004, 5, 10);
    BOBUIest::newRow("normal-1") << QDate(2002, 3, 15) << QDate(2002, 3, 15);
    BOBUIest::newRow("normal-2") << QDate(9999, 12, 31) << QDate(9999, 12, 31);
    BOBUIest::newRow("normal-3") << QDate(1753, 1, 1) << QDate(1753, 1, 1);
    BOBUIest::newRow("invalid-0") << QDate(0, 0, 0) << QDate(1752, 9, 14);
    BOBUIest::newRow("old") << QDate(1492, 8, 3) << QDate(1492, 8, 3);
}

void tst_QDateTimeEdit::minimumDate()
{
    QFETCH(QDate, minimumDate);
    QFETCH(QDate, expectedMinDate);

    testWidget->setMinimumDate(minimumDate);
    QCOMPARE(testWidget->minimumDate(), expectedMinDate);
}

void tst_QDateTimeEdit::minimumDateTime_data()
{
    BOBUIest::addColumn<QDateTime>("minimumDateTime");
    BOBUIest::addColumn<QDateTime>("expectedMinDateTime");

    BOBUIest::newRow("normal-0") << QDateTime(QDate(2004, 5, 10), BOBUIime(2, 3, 14))
                              << QDateTime(QDate(2004, 5, 10), BOBUIime(2, 3, 14));

    BOBUIest::newRow("normal-1") << QDateTime(QDate(2005, 5, 10), BOBUIime(22, 33, 1))
                              << QDateTime(QDate(2005, 5, 10), BOBUIime(22, 33, 1));
    BOBUIest::newRow("normal-2") << QDateTime(QDate(2006, 5, 10), BOBUIime(13, 31, 23))
                              << QDateTime(QDate(2006, 5, 10), BOBUIime(13, 31, 23));
    BOBUIest::newRow("normal-3") << QDateTime(QDate(2007, 5, 10), BOBUIime(22, 23, 23))
                              << QDateTime(QDate(2007, 5, 10), BOBUIime(22, 23, 23));
    BOBUIest::newRow("normal-4") << QDateTime(QDate(2008, 5, 10), BOBUIime(2, 3, 1))
                              << QDateTime(QDate(2008, 5, 10), BOBUIime(2, 3, 1));
    BOBUIest::newRow("invalid-0") << QDateTime() << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0, 0));
    BOBUIest::newRow("old") << QDateTime(QDate(1492, 8, 3), BOBUIime(2, 3, 1))
                         << QDateTime(QDate(1492, 8, 3), BOBUIime(2, 3, 1));
}

void tst_QDateTimeEdit::minimumDateTime()
{
    QFETCH(QDateTime, minimumDateTime);
    QFETCH(QDateTime, expectedMinDateTime);

    testWidget->setMinimumDateTime(minimumDateTime);
    QCOMPARE(testWidget->minimumDateTime(), expectedMinDateTime);
}

void tst_QDateTimeEdit::maximumDateTime_data()
{
    BOBUIest::addColumn<QDateTime>("maximumDateTime");
    BOBUIest::addColumn<QDateTime>("expectedMinDateTime");

    BOBUIest::newRow("normal-0") << QDateTime(QDate(2004, 5, 10), BOBUIime(2, 3, 14))
                              << QDateTime(QDate(2004, 5, 10), BOBUIime(2, 3, 14));

    BOBUIest::newRow("normal-1") << QDateTime(QDate(2005, 5, 10), BOBUIime(22, 33, 1))
                              << QDateTime(QDate(2005, 5, 10), BOBUIime(22, 33, 1));
    BOBUIest::newRow("normal-2") << QDateTime(QDate(2006, 5, 10), BOBUIime(13, 31, 23))
                              << QDateTime(QDate(2006, 5, 10), BOBUIime(13, 31, 23));
    BOBUIest::newRow("normal-3") << QDateTime(QDate(2007, 5, 10), BOBUIime(22, 23, 23))
                              << QDateTime(QDate(2007, 5, 10), BOBUIime(22, 23, 23));
    BOBUIest::newRow("normal-4") << QDateTime(QDate(2008, 5, 10), BOBUIime(2, 3, 1))
                              << QDateTime(QDate(2008, 5, 10), BOBUIime(2, 3, 1));
    BOBUIest::newRow("invalid-0") << QDateTime() << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
}

void tst_QDateTimeEdit::maximumDateTime()
{
    QFETCH(QDateTime, maximumDateTime);
    QFETCH(QDateTime, expectedMinDateTime);

    testWidget->setMaximumDateTime(maximumDateTime);
    QCOMPARE(testWidget->maximumDateTime(), expectedMinDateTime);
}

void tst_QDateTimeEdit::maximumDate_data()
{
    BOBUIest::addColumn<QDate>("maximumDate");
    BOBUIest::addColumn<QDate>("expectedMaxDate");

    BOBUIest::newRow("normal-0") << QDate(2004, 05, 10) << QDate(2004, 5, 10);
    BOBUIest::newRow("normal-1") << QDate(2002, 03, 15) << QDate(2002, 3, 15);
    BOBUIest::newRow("normal-2") << QDate(9999, 12, 31) << QDate(9999, 12, 31);
    BOBUIest::newRow("normal-3") << QDate(1753, 1, 1) << QDate(1753, 1, 1);
    BOBUIest::newRow("invalid-0") << QDate(0, 0, 0) << QDate(9999, 12, 31);
}

void tst_QDateTimeEdit::maximumDate()
{
    QFETCH(QDate, maximumDate);
    QFETCH(QDate, expectedMaxDate);

    testWidget->setMaximumDate(maximumDate);
    QCOMPARE(testWidget->maximumDate(), expectedMaxDate);
}

void tst_QDateTimeEdit::clearMinimumDate_data()
{
    BOBUIest::addColumn<QDate>("minimumDate");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<QDate>("expectedMinDateAfterClear");

    BOBUIest::newRow("normal-0") << QDate(2004, 05, 10) << true << QDate(1752, 9, 14);
    BOBUIest::newRow("normal-1") << QDate(2002, 3, 15) << true << QDate(1752, 9, 14);
    BOBUIest::newRow("normal-2") << QDate(9999, 12, 31) << true << QDate(1752, 9, 14);
    BOBUIest::newRow("normal-3") << QDate(1753, 1, 1) << true << QDate(1752, 9, 14);
    BOBUIest::newRow("invalid-0") << QDate(0, 0, 0) << false << QDate(1752, 9, 14);
}

void tst_QDateTimeEdit::clearMinimumDate()
{
    QFETCH(QDate, minimumDate);
    QFETCH(bool, valid);
    QFETCH(QDate, expectedMinDateAfterClear);

    testWidget->setMinimumDate(minimumDate);
    if (valid)
        QCOMPARE(testWidget->minimumDate(), minimumDate);
    testWidget->clearMinimumDate();
    QCOMPARE(testWidget->minimumDate(), expectedMinDateAfterClear);
}

void tst_QDateTimeEdit::clearMinimumDateTime_data()
{
    BOBUIest::addColumn<QDateTime>("minimumDateTime");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<QDateTime>("expectedMinDateTimeAfterClear");

    BOBUIest::newRow("normal-0") << QDateTime(QDate(2004, 05, 10), BOBUIime(12, 12, 12))
                              << true << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0));
    BOBUIest::newRow("normal-1") << QDateTime(QDate(2002, 3, 15), BOBUIime(13, 13, 13))
                              << true << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0));
    BOBUIest::newRow("normal-2") << QDateTime(QDate(9999, 12, 31), BOBUIime(14, 14, 14))
                              << true << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0));
    BOBUIest::newRow("normal-3") << QDateTime(QDate(1753, 1, 1), BOBUIime(15, 15, 15))
                              << true << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0));
    BOBUIest::newRow("invalid-0") << QDateTime() << false << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0));
    BOBUIest::newRow("old") << QDateTime(QDate(1492, 8, 3), BOBUIime(2, 3, 1)) << true
                         << QDateTime(QDate(1752, 9, 14), BOBUIime(0, 0));
}

void tst_QDateTimeEdit::clearMinimumDateTime()
{
    QFETCH(QDateTime, minimumDateTime);
    QFETCH(bool, valid);
    QFETCH(QDateTime, expectedMinDateTimeAfterClear);

    testWidget->setMinimumDateTime(minimumDateTime);
    if (valid)
        QCOMPARE(testWidget->minimumDateTime(), minimumDateTime);
    testWidget->clearMinimumDateTime();
    QCOMPARE(testWidget->minimumDateTime(), expectedMinDateTimeAfterClear);
}

void tst_QDateTimeEdit::clearMaximumDateTime_data()
{
    BOBUIest::addColumn<QDateTime>("maximumDateTime");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<QDateTime>("expectedMinDateTimeAfterClear");

    BOBUIest::newRow("normal-0") << QDateTime(QDate(2004, 05, 10), BOBUIime(12, 12, 12))
                              << true << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
    BOBUIest::newRow("normal-1") << QDateTime(QDate(2002, 3, 15), BOBUIime(13, 13, 13))
                              << true << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
    BOBUIest::newRow("normal-2") << QDateTime(QDate(9999, 12, 31), BOBUIime(14, 14, 14))
                              << true << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
    BOBUIest::newRow("normal-3") << QDateTime(QDate(1753, 1, 1), BOBUIime(15, 15, 15))
                              << true << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
    BOBUIest::newRow("invalid-0") << QDateTime()
                               << false << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
}

void tst_QDateTimeEdit::clearMaximumDateTime()
{
    QFETCH(QDateTime, maximumDateTime);
    QFETCH(bool, valid);
    QFETCH(QDateTime, expectedMinDateTimeAfterClear);

    testWidget->setMaximumDateTime(maximumDateTime);
    if (valid)
        QCOMPARE(testWidget->maximumDateTime(), maximumDateTime);
    testWidget->clearMaximumDateTime();
    QCOMPARE(testWidget->maximumDateTime(), expectedMinDateTimeAfterClear);
}

void tst_QDateTimeEdit::clearMaximumDate_data()
{
    BOBUIest::addColumn<QDate>("maximumDate");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<QDate>("expectedMaxDateAfterClear");

    BOBUIest::newRow("normal-0") << QDate(2004, 05, 10) << true << QDate(9999, 12, 31);
    BOBUIest::newRow("normal-1") << QDate(2002, 03, 15) << true << QDate(9999, 12, 31);
    BOBUIest::newRow("normal-2") << QDate(9999, 12, 31) << true << QDate(9999, 12, 31);
    BOBUIest::newRow("normal-3") << QDate(2000, 1, 1) << true << QDate(9999, 12, 31);
    BOBUIest::newRow("invalid-0") << QDate(0, 0, 0) << false << QDate(9999, 12, 31);
}

void tst_QDateTimeEdit::clearMaximumDate()
{
    QFETCH(QDate, maximumDate);
    QFETCH(bool, valid);
    QFETCH(QDate, expectedMaxDateAfterClear);

    testWidget->setMaximumDate(maximumDate);
    if (valid)
        QCOMPARE(testWidget->maximumDate(), maximumDate);
    testWidget->clearMaximumDate();
    QCOMPARE(testWidget->maximumDate(), expectedMaxDateAfterClear);
}

void tst_QDateTimeEdit::displayFormat_data()
{
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QDateTime>("date");

    const QDateTime dt(QDate(2999, 12, 31), BOBUIime(3, 59, 59, 999));

    BOBUIest::newRow("valid-0") << QString("yyyy MM dd") << true << QString("2999 12 31") << dt;
    BOBUIest::newRow("valid-1") << QString("dd MM yyyy::ss:mm:hh") << true
                             << QString("31 12 2999::59:59:03") << dt;
    BOBUIest::newRow("valid-2") << QString("hh-dd-mm-MM-yy") << true << QString("03-31-59-12-99") << dt;
    BOBUIest::newRow("valid-3") << QString("ddd MM d yyyy::ss:mm:hh") << true
                             << QLocale::system().dayName(2, QLocale::ShortFormat) + " 12 31 2999::59:59:03" << dt;
    BOBUIest::newRow("valid-4") << QString("hh-dd-mm-MM-yyyy") << true << QString("03-31-59-12-2999") << dt;
    BOBUIest::newRow("invalid-0") << QString("yyyy.MM.yy") << true << QString("2999.12.99") << dt;
    BOBUIest::newRow("invalid-1") << QString("y") << false << QString() << dt;
    BOBUIest::newRow("invalid-2") << QString("") << false << QString() << dt;
    BOBUIest::newRow("quoted-1") << QString("'Midday is at:' dd") << true << QString("Midday is at: 31") << dt;
    BOBUIest::newRow("leading1") << QString("h:hh:hhh") << true << QString("3:03:033") << dt;
    BOBUIest::newRow("H1") << QString("HH:hh:ap") << true << QString("03:03:am") << dt;
    BOBUIest::newRow("H2") << QString("HH:hh:ap") << true << QString("23:11:pm")
                        << QDateTime(dt.date(), BOBUIime(23, 0, 0));
}

void tst_QDateTimeEdit::displayFormat()
{
    QFETCH(QString, format);
    QFETCH(bool, valid);
    QFETCH(QString, text);
    QFETCH(QDateTime, date);

    testWidget->setDateTime(date);

    QString compareFormat = format;
    if (!valid)
        compareFormat = testWidget->displayFormat();
    testWidget->setDisplayFormat(format);
    QCOMPARE(testWidget->displayFormat(), compareFormat);
    if (valid)
        QCOMPARE(testWidget->text(), text);
}

void tst_QDateTimeEdit::selectAndScrollWithKeys()
{
#ifdef Q_OS_MAC
    QSKIP("BOBUIBUG-23674");
    return;
#endif

    testWidget->setDate(QDate(2004, 05, 11));
    testWidget->setDisplayFormat("dd/MM/yyyy");
    testWidget->show();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("1"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/0"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/2"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/20"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/200"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/2004"));

    // Now the year part should be selected
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2005, 5, 11));
    QCOMPARE(testWidget->currentSection(), QDateTimeEdit::YearSection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2005"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2004, 5, 11));
    QCOMPARE(testWidget->currentSection(), QDateTimeEdit::YearSection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2004"));


#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("4"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("04"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("5/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("1/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11/05/2004"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif

    // Now the day part should be selected
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2004, 5, 12));
    QCOMPARE(testWidget->currentSection(), QDateTimeEdit::DaySection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("12"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2004, 5, 11));
    QCOMPARE(testWidget->currentSection(), QDateTimeEdit::DaySection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));

#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif

    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
    // Now the day part should be selected
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2004, 05, 12));
}

void tst_QDateTimeEdit::backspaceKey()
{
    testWidget->setDate(QDate(2004, 05, 11));
    testWidget->setDisplayFormat("d/MM/yyyy");
    testWidget->show();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    QCOMPARE(testWidget->text(), QString("11/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(testWidget->text(), QString("11/05/200"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/05/20"));
    // Check that moving into another field reverts it
    for (int i=0;i<3;i++)
        BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    QCOMPARE(testWidget->text(), QString("11/05/2004"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    for (int i=0;i<4;i++)
        BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ShiftModifier);

    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/05/"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/0/2004"));
    testWidget->interpretText();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/05/200"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/05/20"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/05/2"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/05/"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11/0/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("11//2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("1/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->text(), QString("1/05/2004"));
}

void tst_QDateTimeEdit::deleteKey()
{
    testWidget->setDate(QDate(2004, 05, 11));
    testWidget->setDisplayFormat("d/MM/yyyy");
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    QCOMPARE(testWidget->text(), QString("1/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    QCOMPARE(testWidget->text(), QString("/05/2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QCOMPARE(testWidget->text(), QString("1/05/2004"));
}

void tst_QDateTimeEdit::tabKeyNavigation()
{
    testWidget->setDate(QDate(2004, 05, 11));
    testWidget->setDisplayFormat("dd/MM/yyyy");
    testWidget->show();
    testWidget->setCurrentSection(QDateTimeEdit::DaySection);

    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
}

void tst_QDateTimeEdit::tabKeyNavigationWithPrefix()
{
    testWidget->setDate(QDate(2004, 05, 11));
    testWidget->setDisplayFormat("prefix dd/MM/yyyy");

    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
}

void tst_QDateTimeEdit::tabKeyNavigationWithSuffix()
{
    testWidget->setDate(QDate(2004, 05, 11));
    testWidget->setDisplayFormat("dd/MM/yyyy 'suffix'");

    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2004"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("05"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
}

void tst_QDateTimeEdit::enterKey()
{
    testWidget->setDate(QDate(2004, 5, 11));
    testWidget->setDisplayFormat("prefix d/MM/yyyy 'suffix'");
    testWidget->lineEdit()->setFocus();

#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QVERIFY(!testWidget->lineEdit()->hasSelectedText());
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QVERIFY(!testWidget->lineEdit()->hasSelectedText());
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
    BOBUIest::keyClick(testWidget, BobUI::Key_1);
    BOBUIest::keyClick(testWidget, BobUI::Key_5);

    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);

    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("15"));
    QCOMPARE(testWidget->date(), QDate(2004, 5, 15));

    BOBUIest::keyClick(testWidget, BobUI::Key_9);
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab, BobUI::ShiftModifier);
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("9"));
    QCOMPARE(testWidget->date(), QDate(2004, 5, 9));

    BOBUIest::keyClick(testWidget, BobUI::Key_0);
    BOBUIest::keyClick(testWidget, BobUI::Key_0);
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("9"));
    QCOMPARE(testWidget->date(), QDate(2004, 5, 9));

    // Current behaviour is that pressing the Enter key in a QDateTimeEdit
    // causes the dateChanged() signal to be emitted, even if the date
    // wasn't actually changed.  While this behaviour is questionable,
    // we include this test so a change to the behaviour can't go unnoticed.
    QSignalSpy enterSpy(testWidget, SIGNAL(dateChanged(QDate)));
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(enterSpy.size(), 1);
    QVariantList list = enterSpy.takeFirst();
    QCOMPARE(list.at(0).toDate(), QDate(2004, 5, 9));
}

void tst_QDateTimeEdit::specialValueText()
{
    testWidget->setDisplayFormat("dd/MM/yyyy");
    testWidget->setDateRange(QDate(2000, 1, 1), QDate(2001, 1, 1));
    testWidget->setDate(QDate(2000, 1, 2));
    testWidget->setSpecialValueText("fOo");
    testWidget->setCurrentSection(QDateTimeEdit::DaySection);
    QCOMPARE(testWidget->date(), QDate(2000, 1, 2));
    QCOMPARE(testWidget->text(), QString("02/01/2000"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2000, 1, 1));
    QCOMPARE(testWidget->text(), QString("fOo"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2000, 1, 1));
    QCOMPARE(testWidget->text(), QString("fOo"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2000, 1, 2));
    QCOMPARE(testWidget->text(), QString("02/01/2000"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2000, 1, 1));
    QCOMPARE(testWidget->text(), QString("fOo"));

#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2000, 1, 2));
    QCOMPARE(testWidget->text(), QString("02/01/2000"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->text(), QString("fOo"));

#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("fO"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString("f"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString());
    BOBUIest::keyClick(testWidget, BobUI::Key_F);
    QCOMPARE(testWidget->text(), QString("f"));
    BOBUIest::keyClick(testWidget, BobUI::Key_O); // will automatically uppercase
    QCOMPARE(testWidget->text(), QString("fO"));
    BOBUIest::keyClick(testWidget, BobUI::Key_O);
    QCOMPARE(testWidget->text(), QString("fOo"));
}

void tst_QDateTimeEdit::setRange_data()
{
    BOBUIest::addColumn<BOBUIime>("minTime");
    BOBUIest::addColumn<BOBUIime>("maxTime");
    BOBUIest::addColumn<QDate>("minDate");
    BOBUIest::addColumn<QDate>("maxDate");
    BOBUIest::addColumn<QDateTime>("expectedMin");
    BOBUIest::addColumn<QDateTime>("expectedMax");

    const QDate cdt = QDate::currentDate();

    BOBUIest::newRow("data0") << BOBUIime(0, 0) << BOBUIime(14, 12, 0)
                           << cdt << cdt
                           << QDateTime(cdt, BOBUIime(0, 0))
                           << QDateTime(cdt, BOBUIime(14, 12, 0));

    BOBUIest::newRow("data1") << BOBUIime(10, 0) << BOBUIime(1, 12, 0) << cdt.addDays(-1)
                           << cdt
                           << QDateTime(cdt.addDays(-1), BOBUIime(10, 0))
                           << QDateTime(cdt, BOBUIime(1, 12, 0));
}

void tst_QDateTimeEdit::setRange()
{
    QFETCH(BOBUIime, minTime);
    QFETCH(BOBUIime, maxTime);
    QFETCH(QDate, minDate);
    QFETCH(QDate, maxDate);
    QFETCH(QDateTime, expectedMin);
    QFETCH(QDateTime, expectedMax);
    testWidget->hide();

    {
        QDateTimeEdit dte(0);
        dte.setTimeRange(minTime, maxTime);
        QCOMPARE(dte.minimumTime(), expectedMin.time());
        QCOMPARE(dte.maximumTime(), expectedMax.time());
        dte.setDateRange(minDate, maxDate);
        QCOMPARE(dte.minimumDate(), expectedMin.date());
        QCOMPARE(dte.minimumDateTime(), expectedMin);
        QCOMPARE(dte.maximumDate(), expectedMax.date());
        QCOMPARE(dte.maximumDateTime(), expectedMax);
        QCOMPARE(dte.minimumTime(), expectedMin.time());
        QCOMPARE(dte.maximumTime(), expectedMax.time());
        dte.setDateTimeRange(QDateTime(minDate, minTime), QDateTime(maxDate, maxTime));
        QCOMPARE(dte.minimumDate(), expectedMin.date());
        QCOMPARE(dte.minimumDateTime(), expectedMin);
        QCOMPARE(dte.maximumDate(), expectedMax.date());
        QCOMPARE(dte.maximumDateTime(), expectedMax);
        QCOMPARE(dte.minimumTime(), expectedMin.time());
        QCOMPARE(dte.maximumTime(), expectedMax.time());
    }
    {

        QDateTimeEdit dte2(0);
        dte2.setDateRange(minDate, maxDate);
        dte2.setTimeRange(minTime, maxTime);

        QCOMPARE(dte2.minimumDate(), expectedMin.date());
        QCOMPARE(dte2.maximumDate(), expectedMax.date());
        QCOMPARE(dte2.minimumTime(), expectedMin.time());
        QCOMPARE(dte2.maximumTime(), expectedMax.time());
    }

    {
        QDateTimeEdit dte3(0);
        dte3.setMinimumTime(minTime);
        dte3.setMaximumTime(maxTime);
        dte3.setMinimumDate(minDate);
        dte3.setMaximumDate(maxDate);

        QCOMPARE(dte3.minimumDate(), expectedMin.date());
        QCOMPARE(dte3.maximumDate(), expectedMax.date());
        QCOMPARE(dte3.minimumTime(), expectedMin.time());
        QCOMPARE(dte3.maximumTime(), expectedMax.time());
    }

    {
        QDateTimeEdit dte4(0);
        dte4.setMinimumDate(minDate);
        dte4.setMaximumDate(maxDate);
        dte4.setMinimumTime(minTime);
        dte4.setMaximumTime(maxTime);

        QCOMPARE(dte4.minimumDate(), expectedMin.date());
        QCOMPARE(dte4.maximumDate(), expectedMax.date());
        QCOMPARE(dte4.minimumTime(), expectedMin.time());
        QCOMPARE(dte4.maximumTime(), expectedMax.time());
    }
}

/*
    Test that a user can input a date into a ranged QDateTimeEdit or QDateEdit
    where a part of date is larger than the respective part of the maximum, or
    smaller than the respective part of the minimum of the range.

    This test is expected to fail unless keyboard tracking of the edit is set
    to off. Otherwise the changed-signal would be emitted with values outside
    of the allowed range as the user types.
*/
void tst_QDateTimeEdit::editingRanged_data()
{
    BOBUIest::addColumn<QDate>("minDate");
    BOBUIest::addColumn<BOBUIime>("minTime");
    BOBUIest::addColumn<QDate>("maxDate");
    BOBUIest::addColumn<BOBUIime>("maxTime");
    BOBUIest::addColumn<QString>("userInput");
    BOBUIest::addColumn<QDateTime>("expected");

    BOBUIest::addRow("trivial")
        << QDate(2010, 1, 1) << BOBUIime(9, 0)
        << QDate(2011, 12, 31) << BOBUIime(16, 0)
        << QString::fromLatin1("311220101600")
        << QDateTime(QDate(2010, 12, 31), BOBUIime(16, 0));

    BOBUIest::addRow("data0")
        << QDate(2010, 12, 30) << BOBUIime(16, 0)
        << QDate(2011, 1, 2) << BOBUIime(9, 0)
        << QString::fromLatin1("311220102359")
        << QDateTime(QDate(2010, 12, 31), BOBUIime(23, 59));

    BOBUIest::addRow("data1")
        << QDate(2010, 12, 30) << BOBUIime(16, 0)
        << QDate(2011, 1, 2) << BOBUIime(9, 0)
        << QString::fromLatin1("010120111823")
        << QDateTime(QDate(2011, 1, 1), BOBUIime(18, 23));

    BOBUIest::addRow("Out of range")
        << QDate(2010, 12, 30) << BOBUIime(16, 0)
        << QDate(2011, 1, 2) << BOBUIime(9, 0)
        << QString::fromLatin1("090920111823")
        << QDateTime(QDate(2011, 1, 2), BOBUIime(9, 0));

    BOBUIest::addRow("only date")
        << QDate(2010, 12, 30) << BOBUIime()
        << QDate(2011, 1, 2) << BOBUIime()
        << QString::fromLatin1("01012011")
        << QDateTime(QDate(2011, 1, 1), BOBUIime(), BOBUIimeZone::UTC);
}

void tst_QDateTimeEdit::editingRanged()
{
    QFETCH(QDate, minDate);
    QFETCH(BOBUIime, minTime);
    QFETCH(QDate, maxDate);
    QFETCH(BOBUIime, maxTime);
    QFETCH(QString, userInput);
    QFETCH(QDateTime, expected);

    QScopedPointer<QDateTimeEdit> edit;
    if (minTime.isValid()) {
        edit.reset(new QDateTimeEdit);
        edit->setDisplayFormat("dd.MM.yyyy hh:mm");
        edit->setDateTimeRange(QDateTime(minDate, minTime), QDateTime(maxDate, maxTime));
    } else {
        edit.reset(new QDateEdit);
        edit->setDisplayFormat("dd.MM.yyyy");
        edit->setDateRange(minDate, maxDate);
    }

    int callCount = 0;
    connect(edit.get(), &QDateTimeEdit::dateTimeChanged, [&](const QDateTime &dateTime) {
        ++callCount;
        if (minTime.isValid()) {
            QVERIFY(dateTime >= QDateTime(minDate, minTime));
            QVERIFY(dateTime <= QDateTime(maxDate, maxTime));
        } else {
            QVERIFY(dateTime.date() >= minDate);
            QVERIFY(dateTime.date() <= maxDate);
        }
    });

    edit->show();
    QVERIFY(BOBUIest::qWaitForWindowFocused(edit.get()));
    edit->setFocus();

    // with keyboard tracking, never get a signal with an out-of-range value
    edit->setKeyboardTracking(true);
    BOBUIest::keyClicks(edit.get(), userInput);
    BOBUIest::keyClick(edit.get(), BobUI::Key_Return);
    QVERIFY(callCount > 0);

    // QDateTimeEdit blocks these dates from being entered - see BOBUIBUG-65
    QEXPECT_FAIL("data0", "Can't enter this date", Continue);
    QEXPECT_FAIL("data1", "Can't enter this date", Continue);
    QEXPECT_FAIL("Out of range", "Can't enter this date", Continue);
    QEXPECT_FAIL("only date", "Can't enter this date", Continue);
    QCOMPARE(edit->dateTime(), expected);

    // reset
    edit->clearFocus();
    edit->setFocus();
    callCount = 0;

    edit->setKeyboardTracking(false);
    BOBUIest::keyClicks(edit.get(), userInput);
    BOBUIest::keyClick(edit.get(), BobUI::Key_Return);
    QCOMPARE(edit->dateTime(), expected);
    QCOMPARE(callCount, 1);
}

void tst_QDateTimeEdit::wrappingTime_data()
{
    BOBUIest::addColumn<bool>("startWithMin");
    BOBUIest::addColumn<BOBUIime>("minimumTime");
    BOBUIest::addColumn<BOBUIime>("maximumTime");
    BOBUIest::addColumn<uint>("section");
    BOBUIest::addColumn<BOBUIime>("newTime");

    BOBUIest::newRow("data0") << false << BOBUIime(0,0,0) << BOBUIime(2,2,2) << (uint)QDateTimeEdit::HourSection
                        << BOBUIime(0,2,2);
    BOBUIest::newRow("data1") << true << BOBUIime(0,0,0) << BOBUIime(2,2,2) << (uint)QDateTimeEdit::HourSection
                        << BOBUIime(2,0,0);
    BOBUIest::newRow("data2") << false << BOBUIime(0,0,0) << BOBUIime(2,2,2) << (uint)QDateTimeEdit::MinuteSection
                        << BOBUIime(2,0,2);
    BOBUIest::newRow("data3") << true << BOBUIime(0,0,0) << BOBUIime(2,2,2) << (uint)QDateTimeEdit::MinuteSection
                        << BOBUIime(0,59,0);
    BOBUIest::newRow("data4") << false << BOBUIime(0,0,0) << BOBUIime(2,2,2) << (uint)QDateTimeEdit::SecondSection
                        << BOBUIime(2,2,0);
    BOBUIest::newRow("data5") << true << BOBUIime(0,0,0) << BOBUIime(2,2,2) << (uint)QDateTimeEdit::SecondSection
                        << BOBUIime(0,0,59);
    BOBUIest::newRow("data6") << false << BOBUIime(1,1,1) << BOBUIime(22,22,22) << (uint)QDateTimeEdit::HourSection
                        << BOBUIime(1,22,22);
    BOBUIest::newRow("data7") << true << BOBUIime(1,1,1) << BOBUIime(22,22,22) << (uint)QDateTimeEdit::HourSection
                        << BOBUIime(22,1,1);
    BOBUIest::newRow("data8") << false << BOBUIime(1,1,1) << BOBUIime(22,22,22) << (uint)QDateTimeEdit::MinuteSection
                        << BOBUIime(22,0,22);
    BOBUIest::newRow("data9") << true << BOBUIime(1,1,1) << BOBUIime(22,22,22) << (uint)QDateTimeEdit::MinuteSection
                        << BOBUIime(1,59,1);
    BOBUIest::newRow("data10") << false << BOBUIime(1,1,1) << BOBUIime(22,22,22) << (uint)QDateTimeEdit::SecondSection
                         << BOBUIime(22,22,0);
    BOBUIest::newRow("data11") << true << BOBUIime(1,1,1) << BOBUIime(22,22,22) << (uint)QDateTimeEdit::SecondSection
                         << BOBUIime(1,1,59);
    BOBUIest::newRow("data12") << false << BOBUIime(1,1,1) << BOBUIime(1,2,1) << (uint)QDateTimeEdit::HourSection
                         << BOBUIime(1,2,1);
    BOBUIest::newRow("data13") << true << BOBUIime(1,1,1) << BOBUIime(1,2,1) << (uint)QDateTimeEdit::HourSection
                         << BOBUIime(1,1,1);
    BOBUIest::newRow("data14") << false << BOBUIime(1,1,1) << BOBUIime(1,2,1) << (uint)QDateTimeEdit::MinuteSection
                         << BOBUIime(1,1,1);
    BOBUIest::newRow("data15") << true << BOBUIime(1,1,1) << BOBUIime(1,2,1) << (uint)QDateTimeEdit::MinuteSection
                         << BOBUIime(1,2,1);
    BOBUIest::newRow("data16") << false << BOBUIime(1,1,1) << BOBUIime(1,2,1) << (uint)QDateTimeEdit::SecondSection
                         << BOBUIime(1,2,0);
    BOBUIest::newRow("data17") << true << BOBUIime(1,1,1) << BOBUIime(1,2,1) << (uint)QDateTimeEdit::SecondSection
                         << BOBUIime(1,1,59);
}

void tst_QDateTimeEdit::wrappingTime()
{
    QFETCH(bool, startWithMin);
    QFETCH(BOBUIime, minimumTime);
    QFETCH(BOBUIime, maximumTime);
    QFETCH(uint, section);
    QFETCH(BOBUIime, newTime);

    testWidget->setDisplayFormat("hh:mm:ss");
    testWidget->setMinimumTime(minimumTime);
    testWidget->setMaximumTime(maximumTime);
    testWidget->setWrapping(true);
    testWidget->setCurrentSection((QDateTimeEdit::Section)section);
    if (startWithMin) {
        testWidget->setTime(minimumTime);
        BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    } else {
        testWidget->setTime(maximumTime);
        BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    }
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->time(), newTime);
}

void tst_QDateTimeEdit::userKeyPress_Time_data()
{
    BOBUIest::addColumn<bool>("ampm");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<BOBUIime>("expected_time");

    // ***************** test the hours ***************

    // use up/down keys to change hour in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 10, 0, 0 );
        BOBUIest::newRow( "data0" ) << bool(true) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<5; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 6, 0, 0 );
        BOBUIest::newRow( "data1" ) << bool(true) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<10; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 1, 0, 0 );
        BOBUIest::newRow( "data2" ) << bool(true) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<12; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 23, 0, 0 );
        BOBUIest::newRow( "data3" ) << bool(true) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 12, 0, 0 );
        BOBUIest::newRow( "data4" ) << bool(true) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 13, 0, 0 );
        BOBUIest::newRow( "data5" ) << bool(true) << keys << expected;
    }

    // use up/down keys to change hour in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 10, 0, 0 );
        BOBUIest::newRow( "data6" ) << bool(false) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<5; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 6, 0, 0 );
        BOBUIest::newRow( "data7" ) << bool(false) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<10; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 1, 0, 0 );
        BOBUIest::newRow( "data8" ) << bool(false) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<12; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 23, 0, 0 );
        BOBUIest::newRow( "data9" ) << bool(false) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 12, 0, 0 );
        BOBUIest::newRow( "data10" ) << bool(false) << keys << expected;
    }
    {
        BOBUIestEventList keys;
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 13, 0, 0 );
        BOBUIest::newRow( "data11" ) << bool(false) << keys << expected;
    }

    // enter a one digit valid hour
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '5' );
        BOBUIime expected( 5, 0, 0 );
        BOBUIest::newRow( "data12" ) << bool(true) << keys << expected;
    }

    // entering a two digit valid hour
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '1' );
        keys.addKeyClick( '1' );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data13" ) << bool(true) << keys << expected;
    }

    // entering an invalid hour
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '2' );
        // the '5' creates an invalid hour (25) so it must be ignored
        keys.addKeyClick( '5' );
        BOBUIime expected( 2, 0, 0 );
        BOBUIest::newRow( "data14" ) << bool(true) << keys << expected;
    }

    // enter a value, in hour which causes a field change
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '0' );
        keys.addKeyClick( '2' );
        keys.addKeyClick( '1' );
        BOBUIime expected( 2, 1, 0 );
        BOBUIest::newRow( "data15" ) << bool(true) << keys << expected;
    }

    // enter a one digit valid hour in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '5' );
        BOBUIime expected( 5, 0, 0 );
        BOBUIest::newRow( "data16" ) << bool(false) << keys << expected;
    }

    // enter a two digit valid hour in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '1' );
        keys.addKeyClick( '1' );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data17" ) << bool(false) << keys << expected;
    }

    // enter a two digit valid hour (>12) in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '1' );
        keys.addKeyClick( '5' );
        BOBUIime expected( 15, 0, 0 );
        BOBUIest::newRow( "data18" ) << bool(false) << keys << expected;
    }

    // enter a two digit valid hour (>20) in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '2' );
        keys.addKeyClick( '1' );
        BOBUIime expected( 21, 0, 0 );
        BOBUIest::newRow( "data19" ) << bool(false) << keys << expected;
    }

    // enter a two digit invalid hour (>23) in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( '2' );
        keys.addKeyClick( '4' );
        BOBUIime expected( 2, 0, 0 );
        BOBUIest::newRow( "data20" ) << bool(false) << keys << expected;
    }

    // ***************** test the minutes ***************

    // use up/down keys to change the minutes in 12 hour mode
    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 2, 0 );
        BOBUIest::newRow( "data21" ) << bool(true) << keys << expected;
    }
    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<16; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 16, 0 );
        BOBUIest::newRow( "data22" ) << bool(true) << keys << expected;
    }
    { // test maximum value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<59; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 59, 0 );
        BOBUIest::newRow( "data23" ) << bool(true) << keys << expected;
    }
    { // test 'overflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<60; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data24" ) << bool(true) << keys << expected;
    }
    { // test 'underflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 59, 0 );
        BOBUIest::newRow( "data25" ) << bool(true) << keys << expected;
    }
    { // test valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 58, 0 );
        BOBUIest::newRow( "data26" ) << bool(true) << keys << expected;
    }

    // use up/down keys to change the minutes in 24 hour mode

    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 2, 0 );
        BOBUIest::newRow( "data27" ) << bool(false) << keys << expected;
    }
    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<16; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 16, 0 );
        BOBUIest::newRow( "data28" ) << bool(false) << keys << expected;
    }
    { // test maximum value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<59; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 59, 0 );
        BOBUIest::newRow( "data29" ) << bool(false) << keys << expected;
    }
    { // test 'overflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<60; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data30" ) << bool(false) << keys << expected;
    }
    { // test 'underflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 59, 0 );
        BOBUIest::newRow( "data31" ) << bool(false) << keys << expected;
    }
    { // test valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 58, 0 );
        BOBUIest::newRow( "data32" ) << bool(false) << keys << expected;
    }

    // enter a valid one digit minute in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Tab);
        keys.addKeyClick( '2' );
        BOBUIime expected( 11, 2, 0 );
        BOBUIest::newRow( "data33" ) << bool(true) << keys << expected;
    }

    // enter a valid two digit minutes in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Tab);
        keys.addKeyClick( '2' );
        keys.addKeyClick( '4' );
        BOBUIime expected( 11, 24, 0 );
        BOBUIest::newRow( "data34" ) << bool(true) << keys << expected;
    }

    // check the lower limit of the minutes in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Tab);
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data35" ) << bool(true) << keys << expected;
    }

    // check the upper limit of the minutes in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick(BobUI::Key_Tab);
        keys.addKeyClick( '5' );
        keys.addKeyClick( '9' );
        BOBUIime expected( 11, 59, 0 );
        BOBUIest::newRow( "data36" ) << bool(true) << keys << expected;
    }

    // enter an invalid two digit minutes in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '6' );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 6, 0 );
        BOBUIest::newRow( "data37" ) << bool(true) << keys << expected;
    }

    // test minutes in 24 hour motestWidget-> Behaviour should be exactly the same

    // enter a valid one digit minute in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '2' );
        BOBUIime expected( 11, 2, 0 );
        BOBUIest::newRow( "data38" ) << bool(false) << keys << expected;
    }

    // enter a valid two digit minutes in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '2' );
        keys.addKeyClick( '4' );
        BOBUIime expected( 11, 24, 0 );
        BOBUIest::newRow( "data39" ) << bool(false) << keys << expected;
    }

    // check the lower limit of the minutes in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data40" ) << bool(false) << keys << expected;
    }

    // check the upper limit of the minutes in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '5' );
        keys.addKeyClick( '9' );
        BOBUIime expected( 11, 59, 0 );
        BOBUIest::newRow( "data41" ) << bool(false) << keys << expected;
    }

    // enter an invalid two digit minutes in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '6' );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 6, 0 );
        BOBUIest::newRow( "data42" ) << bool(false) << keys << expected;
    }

    // ***************** test the seconds ***************

    // use up/down to edit the seconds...

    // use up/down keys to change the seconds in 12 hour mode
    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 2 );
        BOBUIest::newRow( "data43" ) << bool(true) << keys << expected;
    }
    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<16; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 16 );
        BOBUIest::newRow( "data44" ) << bool(true) << keys << expected;
    }
    { // test maximum value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<59; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 59 );
        BOBUIest::newRow( "data45" ) << bool(true) << keys << expected;
    }
    { // test 'overflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<60; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data46" ) << bool(true) << keys << expected;
    }
    { // test 'underflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 0, 59 );
        BOBUIest::newRow( "data47" ) << bool(true) << keys << expected;
    }
    { // test valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 0, 58 );
        BOBUIest::newRow( "data48" ) << bool(true) << keys << expected;
    }

    // use up/down keys to change the seconds in 24 hour mode

    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 2 );
        BOBUIest::newRow( "data49" ) << bool(false) << keys << expected;
    }
    { // test a valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<16; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 16 );
        BOBUIest::newRow( "data50" ) << bool(false) << keys << expected;
    }
    { // test maximum value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<59; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 59 );
        BOBUIest::newRow( "data51" ) << bool(false) << keys << expected;
    }
    { // test 'overflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<60; i++)
            keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data52" ) << bool(false) << keys << expected;
    }
    { // test 'underflow'
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 0, 59 );
        BOBUIest::newRow( "data53" ) << bool(false) << keys << expected;
    }
    { // test valid value
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        for (uint i=0; i<2; i++)
            keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 0, 58 );
        BOBUIest::newRow( "data54" ) << bool(false) << keys << expected;
    }

    // enter a valid one digit second in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '2' );
        BOBUIime expected( 11, 0, 2 );
        BOBUIest::newRow( "data55" ) << bool(true) << keys << expected;
    }

    // enter a valid two digit seconds in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '2' );
        keys.addKeyClick( '4' );
        BOBUIime expected( 11, 0, 24 );
        BOBUIest::newRow( "data56" ) << bool(true) << keys << expected;
    }

    // check the lower limit of the seconds in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data57" ) << bool(true) << keys << expected;
    }

    // check the upper limit of the seconds in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '5' );
        keys.addKeyClick( '9' );
        BOBUIime expected( 11, 0, 59 );
        BOBUIest::newRow( "data58" ) << bool(true) << keys << expected;
    }

    // enter an invalid two digit seconds in 12 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '6' );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 0, 6 );
        BOBUIest::newRow( "data59" ) << bool(true) << keys << expected;
    }

    // test seconds in 24 hour mode. Behaviour should be exactly the same

    // enter a valid one digit minute in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '2' );
        BOBUIime expected( 11, 0, 2 );
        BOBUIest::newRow( "data60" ) << bool(false) << keys << expected;
    }

    // enter a valid two digit seconds in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '2' );
        keys.addKeyClick( '4' );
        BOBUIime expected( 11, 0, 24 );
        BOBUIest::newRow( "data61" ) << bool(false) << keys << expected;
    }

    // check the lower limit of the seconds in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data62" ) << bool(false) << keys << expected;
    }

    // check the upper limit of the seconds in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '5' );
        keys.addKeyClick( '9' );
        BOBUIime expected( 11, 0, 59 );
        BOBUIest::newRow( "data63" ) << bool(false) << keys << expected;
    }

    // enter an invalid two digit seconds in 24 h mode
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( '6' );
        keys.addKeyClick( '0' );
        BOBUIime expected( 11, 0, 6 );
        BOBUIest::newRow( "data64" ) << bool(false) << keys << expected;
    }

    // Test the AMPM indicator
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Up );
        BOBUIime expected( 23, 0, 0 );
        BOBUIest::newRow( "data65" ) << bool(true) << keys << expected;
    }
    // Test the AMPM indicator
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 23, 0, 0 );
        BOBUIest::newRow( "data66" ) << bool(true) << keys << expected;
    }
    // Test the AMPM indicator
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Down );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data67" ) << bool(true) << keys << expected;
    }
    // Test the AMPM indicator
    {
        BOBUIestEventList keys;
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Tab );
        keys.addKeyClick( BobUI::Key_Up );
        keys.addKeyClick( BobUI::Key_Down );
        BOBUIime expected( 11, 0, 0 );
        BOBUIest::newRow( "data68" ) << bool(true) << keys << expected;
    }
}

void tst_QDateTimeEdit::userKeyPress_Time()
{
    QFETCH(bool, ampm);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(BOBUIime, expected_time);

    if (ampm)
        testWidget->setDisplayFormat("hh:mm:ss ap");
    else
        testWidget->setDisplayFormat("hh:mm:ss");

    testWidget->setTime(BOBUIime(11, 0, 0));
    testWidget->setFocus();

    testWidget->setWrapping(true);

    BOBUIest::keyClick(testWidget, BobUI::Key_Enter); // Make sure the first section is now focused
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("11"));
    keys.simulate(testWidget);
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);

    QCOMPARE(testWidget->time(), expected_time);
}

void tst_QDateTimeEdit::wrappingDate_data()
{
    BOBUIest::addColumn<bool>("startWithMin");
    BOBUIest::addColumn<QDate>("minimumDate");
    BOBUIest::addColumn<QDate>("maximumDate");
    BOBUIest::addColumn<uint>("section");
    BOBUIest::addColumn<QDate>("newDate");

    BOBUIest::newRow("data0") << false << QDate(1999, 1, 1) << QDate(1999, 1, 31) << (uint)QDateTimeEdit::DaySection
                        << QDate(1999, 1, 1);
    BOBUIest::newRow("data1") << true << QDate(1999, 1, 1) << QDate(1999, 1, 31) << (uint)QDateTimeEdit::DaySection
                        << QDate(1999, 1, 31);
    BOBUIest::newRow("data2") << false << QDate(1999, 1, 1) << QDate(1999, 1, 31) << (uint)QDateTimeEdit::MonthSection
                        << QDate(1999, 1, 31);
    BOBUIest::newRow("data3") << true << QDate(1999, 1, 1) << QDate(1999, 1, 31) << (uint)QDateTimeEdit::MonthSection
                        << QDate(1999, 1, 1);
    BOBUIest::newRow("data4") << false << QDate(1999, 1, 1) << QDate(1999, 1, 31) << (uint)QDateTimeEdit::YearSection
                        << QDate(1999, 1, 31);
    BOBUIest::newRow("data5") << true << QDate(1999, 1, 1) << QDate(1999, 1, 31) << (uint)QDateTimeEdit::YearSection
                        << QDate(1999, 1, 1);
    BOBUIest::newRow("data6") << false << QDate(1999, 1, 1) << QDate(2000, 1, 31) << (uint)QDateTimeEdit::DaySection
                        << QDate(2000, 1, 1);
    BOBUIest::newRow("data7") << true << QDate(1999, 1, 1) << QDate(2000, 1, 31) << (uint)QDateTimeEdit::DaySection
                        << QDate(1999, 1, 31);
    BOBUIest::newRow("data8") << false << QDate(1999, 1, 1) << QDate(2000, 1, 31) << (uint)QDateTimeEdit::MonthSection
                        << QDate(2000, 1, 31);
    BOBUIest::newRow("data9") << true << QDate(1999, 1, 1) << QDate(2000, 1, 31) << (uint)QDateTimeEdit::MonthSection
                        << QDate(1999, 12, 1);
    BOBUIest::newRow("data10") << false << QDate(1999, 1, 1) << QDate(2000, 1, 31) << (uint)QDateTimeEdit::YearSection
                         << QDate(1999, 1, 31);
    BOBUIest::newRow("data11") << true << QDate(1999, 1, 1) << QDate(2000, 1, 31) << (uint)QDateTimeEdit::YearSection
                         << QDate(2000, 1, 1);
}

void tst_QDateTimeEdit::wrappingDate()
{
    QFETCH(bool, startWithMin);
    QFETCH(QDate, minimumDate);
    QFETCH(QDate, maximumDate);
    QFETCH(uint, section);
    QFETCH(QDate, newDate);

    testWidget->setDisplayFormat("dd/MM/yyyy");
    testWidget->setMinimumDate(minimumDate);
    testWidget->setMaximumDate(maximumDate);
    testWidget->setWrapping(true);
    testWidget->setCurrentSection((QDateTimeEdit::Section)section);

    if (startWithMin) {
        testWidget->setDate(minimumDate);
        BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    } else {
        testWidget->setDate(maximumDate);
        BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    }
    if (testWidget->currentSection() == QDateTimeEdit::MonthSection)
        QCOMPARE(testWidget->date(), newDate);
}

void tst_QDateTimeEdit::dateSignalChecking_data()
{
    BOBUIest::addColumn<QDate>("originalDate");
    BOBUIest::addColumn<QDate>("newDate");
    BOBUIest::addColumn<int>("timesEmitted");

    BOBUIest::newRow("data0") << QDate(2004, 06, 22) << QDate(2004, 07, 23) << 1;
    BOBUIest::newRow("data1") << QDate(2004, 06, 22) << QDate(2004, 06, 22) << 0;
}

void tst_QDateTimeEdit::dateSignalChecking()
{
    QFETCH(QDate, originalDate);
    QFETCH(QDate, newDate);
    QFETCH(int, timesEmitted);

    testWidget->setDate(originalDate);

    QSignalSpy dateSpy(testWidget, SIGNAL(dateChanged(QDate)));
    QSignalSpy dateTimeSpy(testWidget, SIGNAL(dateTimeChanged(QDateTime)));
    QSignalSpy timeSpy(testWidget, SIGNAL(timeChanged(BOBUIime)));

    testWidget->setDate(newDate);
    QCOMPARE(dateSpy.size(), timesEmitted);

    if (timesEmitted > 0) {
        QList<QVariant> list = dateSpy.takeFirst();
        QDate d;
        d = qvariant_cast<QDate>(list.at(0));
        QCOMPARE(d, newDate);
    }
    QCOMPARE(dateTimeSpy.size(), timesEmitted);
    QCOMPARE(timeSpy.size(), 0);
}

void tst_QDateTimeEdit::timeSignalChecking_data()
{
    BOBUIest::addColumn<BOBUIime>("originalTime");
    BOBUIest::addColumn<BOBUIime>("newTime");
    BOBUIest::addColumn<int>("timesEmitted");

    BOBUIest::newRow("data0") << BOBUIime(15, 55, 00) << BOBUIime(15, 17, 12) << 1;
    BOBUIest::newRow("data1") << BOBUIime(15, 55, 00) << BOBUIime(15, 55, 00) << 0;
}

void tst_QDateTimeEdit::timeSignalChecking()
{
    QFETCH(BOBUIime, originalTime);
    QFETCH(BOBUIime, newTime);
    QFETCH(int, timesEmitted);

    testWidget->setTime(originalTime);

    testWidget->setDisplayFormat("hh:mm:ss");
    QSignalSpy dateSpy(testWidget, SIGNAL(dateChanged(QDate)));
    QSignalSpy dateTimeSpy(testWidget, SIGNAL(dateTimeChanged(QDateTime)));
    QSignalSpy timeSpy(testWidget, SIGNAL(timeChanged(BOBUIime)));

    testWidget->setTime(newTime);
    QCOMPARE(timeSpy.size(), timesEmitted);

    if (timesEmitted > 0) {
        QList<QVariant> list = timeSpy.takeFirst();
        BOBUIime t;
        t = qvariant_cast<BOBUIime>(list.at(0));
        QCOMPARE(t, newTime);
    }
    QCOMPARE(dateTimeSpy.size(), timesEmitted);
    QCOMPARE(dateSpy.size(), 0);
}

void tst_QDateTimeEdit::dateTimeSignalChecking_data()
{
    BOBUIest::addColumn<QDateTime>("originalDateTime");
    BOBUIest::addColumn<QDateTime>("newDateTime");
    BOBUIest::addColumn<int>("timesDateEmitted");
    BOBUIest::addColumn<int>("timesTimeEmitted");
    BOBUIest::addColumn<int>("timesDateTimeEmitted");

    BOBUIest::newRow("data0") << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 0))
                        << QDateTime(QDate(2004, 7, 23), BOBUIime(15, 17, 12))
                        << 1 << 1 << 1;
    BOBUIest::newRow("data1") << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 0))
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 17, 12))
                        << 0 << 1 << 1;
    BOBUIest::newRow("data2") << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 0))
                        << QDateTime(QDate(2004, 7, 23), BOBUIime(15, 55, 0))
                        << 1 << 0 << 1;
    BOBUIest::newRow("data3") << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 0))
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 0))
                        << 0 << 0 << 0;
}

void tst_QDateTimeEdit::dateTimeSignalChecking()
{
    QFETCH(QDateTime, originalDateTime);
    QFETCH(QDateTime, newDateTime);
    QFETCH(int, timesDateEmitted);
    QFETCH(int, timesTimeEmitted);
    QFETCH(int, timesDateTimeEmitted);

    testWidget->setDisplayFormat("dd/MM/yyyy hh:mm:ss");
    testWidget->setDateTime(originalDateTime);

    QSignalSpy dateSpy(testWidget, SIGNAL(dateChanged(QDate)));
    QSignalSpy timeSpy(testWidget, SIGNAL(timeChanged(BOBUIime)));
    QSignalSpy dateTimeSpy(testWidget, SIGNAL(dateTimeChanged(QDateTime)));

    testWidget->setDateTime(newDateTime);
    QCOMPARE(dateSpy.size(), timesDateEmitted);
    if (timesDateEmitted > 0) {
        QCOMPARE(timesDateEmitted, 1);
        QList<QVariant> list = dateSpy.takeFirst();
        QDate d;
        d = qvariant_cast<QDate>(list.at(0));
        QCOMPARE(d, newDateTime.date());
    }
    QCOMPARE(timeSpy.size(), timesTimeEmitted);
    if (timesTimeEmitted > 0) {
        QList<QVariant> list = timeSpy.takeFirst();
        BOBUIime t;
        t = qvariant_cast<BOBUIime>(list.at(0));
        QCOMPARE(t, newDateTime.time());
    }
    QCOMPARE(dateTimeSpy.size(), timesDateTimeEmitted);
    if (timesDateTimeEmitted > 0) {
        QList<QVariant> list = dateTimeSpy.takeFirst();
        QDateTime dt;
        dt = qvariant_cast<QDateTime>(list.at(0));
        QCOMPARE(dt, newDateTime);
    }
}

void tst_QDateTimeEdit::sectionText_data()
{
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<QDateTime>("dateTime");
    BOBUIest::addColumn<uint>("section");
    BOBUIest::addColumn<QString>("sectionText");

    BOBUIest::newRow("data0") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::NoSection << QString();
    BOBUIest::newRow("data1") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::AmPmSection << QString("pm");
    BOBUIest::newRow("data2") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::MSecSection << QString("789");
    BOBUIest::newRow("data3") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::SecondSection << QString("03");
    BOBUIest::newRow("data4") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::MinuteSection << QString("55");
    BOBUIest::newRow("data5") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::HourSection << QString("03");
    BOBUIest::newRow("data6") << QString("dd/MM/yyyy hh:mm:ss zzz")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::HourSection << QString("15");
    BOBUIest::newRow("data7") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::DaySection << QString("22");
    BOBUIest::newRow("data8") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::MonthSection << QString("06");
    BOBUIest::newRow("data9") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                        << (uint)QDateTimeEdit::YearSection << QString("2004");
    BOBUIest::newRow("data10") << QString("dd/MM/yyyy hh:mm:ss zzz AP")
                         << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                         << (uint)QDateTimeEdit::AmPmSection << QString("PM");
    BOBUIest::newRow("data11") << QString("dd/MM/yyyy hh:mm:ss ap")
                         << QDateTime(QDate(2004, 6, 22), BOBUIime(15, 55, 3, 789))
                         << (uint)QDateTimeEdit::MSecSection << QString();
}

void tst_QDateTimeEdit::sectionText()
{
    QFETCH(QString, format);
    QFETCH(QDateTime, dateTime);
    QFETCH(uint, section);
    QFETCH(QString, sectionText);

    testWidget->setDisplayFormat(format);
    testWidget->setDateTime(dateTime);
    QCOMPARE(testWidget->sectionText(QDateTimeEdit::Section(section)), sectionText);

    QApplication::setLayoutDirection(BobUI::RightToLeft);
    const QScopeGuard resetLayoutDirection([]() {
            QApplication::setLayoutDirection(BobUI::LeftToRight);
        });
    testWidget->setDisplayFormat(format);
    QCOMPARE(format, testWidget->displayFormat());
    testWidget->setDateTime(dateTime);
    QCOMPARE(testWidget->sectionText(QDateTimeEdit::Section(section)), sectionText);
}

void tst_QDateTimeEdit::mousePress()
{
    testWidget->setDate(QDate(2004, 6, 23));
    testWidget->setCurrentSection(QDateTimeEdit::YearSection);
    QCOMPARE(testWidget->currentSection(), QDateTimeEdit::YearSection);

    // Ask the SC_SpinBoxUp button location from style
    QStyleOptionSpinBox so;
    so.rect = testWidget->rect();
    QRect rectUp = testWidget->style()->subControlRect(QStyle::CC_SpinBox, &so, QStyle::SC_SpinBoxUp, testWidget);

    // Send mouseClick to center of SC_SpinBoxUp
    BOBUIest::mouseClick(testWidget, BobUI::LeftButton, {}, rectUp.center());
    QCOMPARE(testWidget->date().year(), 2005);
}

void tst_QDateTimeEdit::stepHourAMPM_data()
{
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<KeyList>("keys");
    BOBUIest::addColumn<TimeList>("expected");
    BOBUIest::addColumn<BOBUIime>("start");
    BOBUIest::addColumn<BOBUIime>("min");
    BOBUIest::addColumn<BOBUIime>("max");

    {
        KeyList keys;
        TimeList expected;
        keys << BobUI::Key_Up;
        expected << BOBUIime(1, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(2, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(3, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(4, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(5, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(6, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(7, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(8, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(9, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(10, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(11, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(12, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(13, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(14, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(15, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(16, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(17, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(18, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(19, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(20, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(21, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(22, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(23, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(22, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(21, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(20, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(19, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(18, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(17, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(16, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(15, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(14, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(13, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(12, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(11, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(10, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(9, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(8, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(7, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(6, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(5, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(4, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(3, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(2, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(1, 0, 0);
        keys << BobUI::Key_Down;
        expected << BOBUIime(0, 0, 0);

        BOBUIest::newRow("hh 1") << QString("hh") << keys << expected << BOBUIime(0, 0)
                              << BOBUIime(0, 0) << BOBUIime(23, 59, 59);
        BOBUIest::newRow("hh:ap 1") << QString("hh:ap") << keys << expected
                                 << BOBUIime(0, 0) << BOBUIime(0, 0)
                                 << BOBUIime(23, 59, 59);

        BOBUIest::newRow("HH:ap 2") << QString("HH:ap") << keys << expected
                                 << BOBUIime(0, 0) << BOBUIime(0, 0)
                                 << BOBUIime(23, 59, 59);
    }
    {
        KeyList keys;
        TimeList expected;
        keys << BobUI::Key_Down;
        expected << BOBUIime(2, 0, 0);
        BOBUIest::newRow("hh 2") << QString("hh") << keys << expected << BOBUIime(0, 0) << BOBUIime(2, 0, 0) << BOBUIime(23, 59, 59);
        BOBUIest::newRow("hh:ap 2") << QString("hh:ap") << keys << expected << BOBUIime(0, 0) << BOBUIime(2, 0, 0) << BOBUIime(23, 59, 59);
    }
    {
        KeyList keys;
        TimeList expected;
        keys << BobUI::Key_Up;
        expected << BOBUIime(23, 0, 0);
        keys << BobUI::Key_Up;
        expected << BOBUIime(23, 0, 0);
        BOBUIest::newRow("hh 3") << QString("hh") << keys << expected << BOBUIime(0, 0) << BOBUIime(22, 0, 0)
                              << BOBUIime(23, 59, 59);
        BOBUIest::newRow("hh:ap 3") << QString("hh:ap") << keys << expected << BOBUIime(0, 0)
                                 << BOBUIime(22, 0, 0) << BOBUIime(23, 59, 59);
    }
    {
        KeyList keys;
        TimeList expected;
        keys << BobUI::Key_Up;
        expected << BOBUIime(15, 31, 0);
        BOBUIest::newRow("hh:mm:ap 3") << QString("hh:mm:ap") << keys << expected << BOBUIime(15, 31, 0)
                                    << BOBUIime(9, 0, 0) << BOBUIime(16, 0, 0);
        BOBUIest::newRow("hh:mm 3") << QString("hh:mm") << keys << expected << BOBUIime(15, 31, 0)
                                 << BOBUIime(9, 0, 0) << BOBUIime(16, 0, 0);
    }
}

void tst_QDateTimeEdit::stepHourAMPM()
{
    QFETCH(QString, format);
    QFETCH(KeyList, keys);
    QFETCH(TimeList, expected);
    QFETCH(BOBUIime, start);
    QFETCH(BOBUIime, min);
    QFETCH(BOBUIime, max);

    testWidget->setDisplayFormat(format);
    testWidget->setTime(start);
    testWidget->setMinimumTime(min);
    testWidget->setMaximumTime(max);
    if (keys.size() != expected.size()) {
        qWarning("%s:%d Test broken", __FILE__, __LINE__);
        QCOMPARE(keys.size(), expected.size());
    }

    for (int i=0; i<keys.size(); ++i) {
        BOBUIest::keyClick(testWidget, keys.at(i));
        QCOMPARE(testWidget->time(), expected.at(i));
    }
}

void tst_QDateTimeEdit::displayedSections_data()
{
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<uint>("section");

    BOBUIest::newRow("data0") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)(QDateTimeEdit::DaySection | QDateTimeEdit::MonthSection
                                  | QDateTimeEdit::YearSection | QDateTimeEdit::HourSection
                                  | QDateTimeEdit::MinuteSection | QDateTimeEdit::SecondSection
                                  | QDateTimeEdit::MSecSection | QDateTimeEdit::AmPmSection);
    BOBUIest::newRow("data1") << QString("dd/yyyy hh:mm:ss zzz ap")
                        << (uint)(QDateTimeEdit::DaySection
                                  | QDateTimeEdit::YearSection | QDateTimeEdit::HourSection
                                  | QDateTimeEdit::MinuteSection | QDateTimeEdit::SecondSection
                                  | QDateTimeEdit::MSecSection | QDateTimeEdit::AmPmSection);
    BOBUIest::newRow("data2") << QString("dd/MM/yyyy mm zzz ap")
                        << (uint)(QDateTimeEdit::DaySection | QDateTimeEdit::MonthSection
                                  | QDateTimeEdit::YearSection
                                  | QDateTimeEdit::MinuteSection
                                  | QDateTimeEdit::MSecSection | QDateTimeEdit::AmPmSection);
    BOBUIest::newRow("data3") << QString("dd/MM/yyyy")
                        << (uint)(QDateTimeEdit::DaySection | QDateTimeEdit::MonthSection
                                  | QDateTimeEdit::YearSection);
    BOBUIest::newRow("data4") << QString("hh:mm:ss zzz ap")
                        << (uint)(QDateTimeEdit::HourSection
                                  | QDateTimeEdit::MinuteSection | QDateTimeEdit::SecondSection
                                  | QDateTimeEdit::MSecSection | QDateTimeEdit::AmPmSection);
    BOBUIest::newRow("data5") << QString("dd ap")
                        << (uint)(QDateTimeEdit::DaySection | QDateTimeEdit::AmPmSection);
    BOBUIest::newRow("data6") << QString("zzz")
                        << (uint)QDateTimeEdit::MSecSection;
}

void tst_QDateTimeEdit::displayedSections()
{
    QFETCH(QString, format);
    QFETCH(uint, section);

    testWidget->setDisplayFormat(format);
    QCOMPARE(QDateTimeEdit::Sections(section), testWidget->displayedSections());
}

void tst_QDateTimeEdit::currentSection_data()
{
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<uint>("section");
    BOBUIest::addColumn<uint>("currentSection");

    // First is deliberate, this way we can make sure that it is not reset by specifying no section.
    BOBUIest::newRow("data0") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::NoSection << (uint)QDateTimeEdit::YearSection;
    BOBUIest::newRow("data1") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::AmPmSection << (uint)QDateTimeEdit::AmPmSection;
    BOBUIest::newRow("data2") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::MSecSection << (uint)QDateTimeEdit::MSecSection;
    BOBUIest::newRow("data3") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::SecondSection << (uint)QDateTimeEdit::SecondSection;
    BOBUIest::newRow("data4") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::MinuteSection << (uint)QDateTimeEdit::MinuteSection;
    BOBUIest::newRow("data5") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::HourSection << (uint)QDateTimeEdit::HourSection;
    BOBUIest::newRow("data6") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::DaySection << (uint)QDateTimeEdit::DaySection;
    BOBUIest::newRow("data7") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::MonthSection << (uint)QDateTimeEdit::MonthSection;
    BOBUIest::newRow("data8") << QString("dd/MM/yyyy hh:mm:ss zzz ap")
                        << (uint)QDateTimeEdit::YearSection << (uint)QDateTimeEdit::YearSection;
    BOBUIest::newRow("data9") << QString("dd/MM/yyyy hh:mm:ss zzz AP")
                        << (uint)QDateTimeEdit::AmPmSection << (uint)QDateTimeEdit::AmPmSection;
    BOBUIest::newRow("data10") << QString("dd/MM/yyyy hh:mm:ss ap")
                         << (uint)QDateTimeEdit::MSecSection << (uint)QDateTimeEdit::DaySection;
}

void tst_QDateTimeEdit::currentSection()
{
    QFETCH(QString, format);
    QFETCH(uint, section);
    QFETCH(uint, currentSection);

    testWidget->setDisplayFormat(format);
    if ((QDateTimeEdit::Section)section == QDateTimeEdit::NoSection)
        testWidget->setCurrentSection(QDateTimeEdit::YearSection); // Ensure it's not reset (see above)
    testWidget->setCurrentSection((QDateTimeEdit::Section)section);
    QCOMPARE((QDateTimeEdit::Section)currentSection, testWidget->currentSection());
}

void tst_QDateTimeEdit::readOnly()
{
    testWidget->hide();
    QDateTimeEdit dt(QDate(2000, 2, 1));
    dt.setDisplayFormat("yyyy.MM.dd");
    dt.show();
    dt.setCurrentSection(QDateTimeEdit::DaySection);
    BOBUIest::keyClick(&dt, BobUI::Key_Up);
    QCOMPARE(dt.date(), QDate(2000, 2, 2));
    dt.setReadOnly(true);
    BOBUIest::keyClick(&dt, BobUI::Key_Up);
    QCOMPARE(dt.date(), QDate(2000, 2, 2));
    dt.stepBy(1); // stepBy should still work
    QCOMPARE(dt.date(), QDate(2000, 2, 3));
    dt.setReadOnly(false);
    BOBUIest::keyClick(&dt, BobUI::Key_Up);
    QCOMPARE(dt.date(), QDate(2000, 2, 4));
}

void tst_QDateTimeEdit::weirdCase()
{
    testWidget->lineEdit()->setCursorPosition(0);
    testWidget->setDateRange(QDate(2005, 1, 1), QDate(2010, 12, 31));
    testWidget->setDisplayFormat("dd//MM//yyyy");
    testWidget->setDate(testWidget->minimumDate());
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);
    QVERIFY(!testWidget->lineEdit()->hasSelectedText());
    QCOMPARE(testWidget->lineEdit()->cursorPosition(), 0);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    QVERIFY(!testWidget->lineEdit()->hasSelectedText());
    QCOMPARE(testWidget->lineEdit()->cursorPosition(), 8);

    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    QCOMPARE(testWidget->text(), QString("01//01//005"));
    BOBUIest::keyClick(testWidget, BobUI::Key_4);
    QCOMPARE(testWidget->text(), QString("01//01//005"));
}

void tst_QDateTimeEdit::newCase()
{
    const auto locale = QLocale::system();
    if (locale.monthName(6, QLocale::ShortFormat) != "Jun" ||
            locale.monthName(7, QLocale::ShortFormat) != "Jul" ||
            locale.monthName(6, QLocale::LongFormat) != "June" ||
            locale.monthName(7, QLocale::LongFormat) != "July") {
        QSKIP("This test only works in English");
    }

    testWidget->setDisplayFormat("MMMM'a'MbMMMcMM");
    testWidget->setDate(QDate(2005, 6, 1));
    QCOMPARE(testWidget->text(), QString("Junea6bJunc06"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->text(), QString("Julya7bJulc07"));
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("July"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString());
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Right);
    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    BOBUIest::keyClick(testWidget, BobUI::Key_Left);

    QCOMPARE(testWidget->text(), QString("Jula7bJulc07"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Delete);
    QCOMPARE(testWidget->text(), QString("Jua7bJulc07"));
    BOBUIest::keyClick(testWidget, BobUI::Key_N);
    QCOMPARE(testWidget->text(), QString("Juna7bJulc07"));
    BOBUIest::keyClick(testWidget, BobUI::Key_E);
    QCOMPARE(testWidget->text(), QString("Junea6bJunc06"));
}

void tst_QDateTimeEdit::newCase2()
{
    testWidget->setDisplayFormat("MMMM yyyy-MM-dd MMMM");
    testWidget->setDate(QDate(2005, 8, 8));
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->text(), QString(" 2005-08-08 ") + QLocale::system().monthName(8, QLocale::LongFormat));
}

void tst_QDateTimeEdit::newCase3()
{
    if (!QLocale::system().monthName(1, QLocale::LongFormat).startsWith("Januar"))
        QSKIP("This test does not work in this locale");

    testWidget->setDisplayFormat("dd MMMM yyyy");
    testWidget->setDate(QDate(2000, 1, 1));
    testWidget->setGeometry(QRect(QPoint(0, 0), testWidget->sizeHint()));
    testWidget->setCurrentSection(QDateTimeEdit::MonthSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    BOBUIest::keyClick(testWidget, BobUI::Key_J);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("01 J 2000"));
    QCOMPARE(testWidget->lineEdit()->cursorPosition(), 4);
    BOBUIest::keyClick(testWidget, BobUI::Key_A);
    BOBUIest::keyClick(testWidget, BobUI::Key_N);
    BOBUIest::keyClick(testWidget, BobUI::Key_U);
    BOBUIest::keyClick(testWidget, BobUI::Key_A);
    BOBUIest::keyClick(testWidget, BobUI::Key_R);
}

void tst_QDateTimeEdit::cursorPos()
{
    if (QLocale::system().monthName(1, QLocale::LongFormat) != "January")
        QSKIP("This test only works in English");

    testWidget->setDisplayFormat("dd MMMM yyyy");
    //testWidget->setGeometry(0, 0, 200, 200);
    testWidget->setCurrentSection(QDateTimeEdit::MonthSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    QCOMPARE(testWidget->lineEdit()->cursorPosition(), 10);
    BOBUIest::keyClick(testWidget, BobUI::Key_J);
    BOBUIest::keyClick(testWidget, BobUI::Key_A);
    BOBUIest::keyClick(testWidget, BobUI::Key_N);
    BOBUIest::keyClick(testWidget, BobUI::Key_U);
    BOBUIest::keyClick(testWidget, BobUI::Key_A);
    BOBUIest::keyClick(testWidget, BobUI::Key_R);
    //QCursor::setPos(20, 20);
    //QEventLoop l;
    //l.exec();
    BOBUIest::keyClick(testWidget, BobUI::Key_Y);
    QCOMPARE(testWidget->lineEdit()->cursorPosition(), 11);
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    BOBUIest::keyClick(testWidget, BobUI::Key_3);
    BOBUIest::keyClick(testWidget, BobUI::Key_1);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(testWidget->lineEdit()->cursorPosition(), 3);
}

void tst_QDateTimeEdit::newCase4()
{
    testWidget->setDisplayFormat("hh:mm");
    testWidget->setMinimumTime(BOBUIime(3, 3, 0));
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    BOBUIest::keyClick(testWidget, BobUI::Key_0);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("0:03"));
    BOBUIest::keyClick(testWidget, BobUI::Key_2);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("0:03"));
    BOBUIest::keyClick(testWidget, BobUI::Key_4);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("04:03"));
}

void tst_QDateTimeEdit::newCase5()
{
    testWidget->setDisplayFormat("yyyy-MM-dd hh:mm:ss zzz 'ms'");
    testWidget->setDateTime(QDateTime(QDate(2005, 10, 7), BOBUIime(17, 44, 13, 100)));
    testWidget->show();
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("2005-10-07 17:44:13 100 ms"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Backtab, BobUI::ShiftModifier);

    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    BOBUIest::keyClick(testWidget, BobUI::Key_1);
    BOBUIest::keyClick(testWidget, BobUI::Key_2);
    BOBUIest::keyClick(testWidget, BobUI::Key_4);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("2005-10-07 17:44:13 124 ms"));

    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("2005-10-07 17:44:13 12 ms"));
}

void tst_QDateTimeEdit::newCase6()
{
    testWidget->setDisplayFormat("d-yyyy-MM-dd");
    testWidget->setDate(QDate(2005, 10, 7));
    testWidget->show();
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("7-2005-10-07"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    BOBUIest::keyClick(testWidget, BobUI::Key_1);
    BOBUIest::keyClick(testWidget, BobUI::Key_2);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("12-2005-10-12"));
}

void tst_QDateTimeEdit::task98554()
{
    testWidget->setDisplayFormat("mm.ss.zzz(ms)");
    testWidget->setTime(BOBUIime(0, 0, 9));
    testWidget->setCurrentSection(QDateTimeEdit::SecondSection);
    testWidget->show();
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("00.09.000(09)"));
    QCOMPARE(testWidget->time(), BOBUIime(0, 0, 9, 0));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("00.10.000(010)"));
    QCOMPARE(testWidget->time(), BOBUIime(0, 0, 10, 0));
}

static QList<int> makeList(int val1, int val2, int val3)
{
    QList<int> ret;
    ret << val1 << val2 << val3;
    return ret;
}

void tst_QDateTimeEdit::setCurrentSection_data()
{
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<QDateTime>("dateTime");
    BOBUIest::addColumn<QList<int> >("setCurrentSections");
    BOBUIest::addColumn<QList<int> >("expectedCursorPositions");

    BOBUIest::newRow("Day") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                         << makeList(QDateTimeEdit::DaySection, QDateTimeEdit::DaySection, QDateTimeEdit::DaySection)
                         << makeList(24, 0, 24);
    BOBUIest::newRow("Month") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                           << makeList(QDateTimeEdit::MonthSection, QDateTimeEdit::MonthSection, QDateTimeEdit::MonthSection)
                           << makeList(3, 26, 3);
    BOBUIest::newRow("Year") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                          << makeList(QDateTimeEdit::YearSection, QDateTimeEdit::YearSection, QDateTimeEdit::YearSection)
                          << makeList(6, 28, 6);
    BOBUIest::newRow("Hour") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                          << makeList(QDateTimeEdit::HourSection, QDateTimeEdit::HourSection, QDateTimeEdit::HourSection)
                          << makeList(11, 31, 11);
    BOBUIest::newRow("Minute") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                            << makeList(QDateTimeEdit::MinuteSection, QDateTimeEdit::MinuteSection, QDateTimeEdit::MinuteSection)
                            << makeList(14, 33, 14);
    BOBUIest::newRow("Second") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                            << makeList(QDateTimeEdit::SecondSection, QDateTimeEdit::SecondSection, QDateTimeEdit::SecondSection)
                            << makeList(17, 35, 17);
    BOBUIest::newRow("MSec") << QString("dd/MM/yyyy hh:mm:ss.zzz d/M/yy h:m:s.z") << QDateTime(QDate(2001, 1, 1), BOBUIime(1, 2, 3, 4))
                          << makeList(QDateTimeEdit::MSecSection, QDateTimeEdit::MSecSection, QDateTimeEdit::MSecSection)
                          << makeList(20, 37, 20);
}

void tst_QDateTimeEdit::setCurrentSection()
{
    QFETCH(QString, format);
    QFETCH(QDateTime, dateTime);
    QFETCH(QList<int>, setCurrentSections);
    QFETCH(QList<int>, expectedCursorPositions);

    QCOMPARE(setCurrentSections.size(), expectedCursorPositions.size());
    testWidget->setDisplayFormat(format);
    testWidget->setDateTime(dateTime);
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif

    testWidget->resize(400, 100);
    for (int i=0; i<setCurrentSections.size(); ++i) {
        testWidget->setCurrentSection((QDateTimeEdit::Section)setCurrentSections.at(i));
        QCOMPARE(testWidget->currentSection(), (QDateTimeEdit::Section)setCurrentSections.at(i));
        QCOMPARE(testWidget->lineEdit()->cursorPosition(), expectedCursorPositions.at(i));
    }
}

void tst_QDateTimeEdit::setSelectedSection()
{
    testWidget->setDisplayFormat("mm.ss.zzz('ms') m");
    testWidget->setTime(BOBUIime(0, 0, 9));
    testWidget->show();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_Home);
#endif
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QVERIFY(!testWidget->lineEdit()->hasSelectedText());
    testWidget->setSelectedSection(QDateTimeEdit::MinuteSection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("00"));
    testWidget->setCurrentSection(QDateTimeEdit::MinuteSection);
    testWidget->setSelectedSection(QDateTimeEdit::MinuteSection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("0"));
    testWidget->setSelectedSection(QDateTimeEdit::SecondSection);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("09"));
    testWidget->setSelectedSection(QDateTimeEdit::NoSection);
    QVERIFY(!testWidget->lineEdit()->hasSelectedText());
}

void tst_QDateTimeEdit::calendarPopup()
{
    {
        QDateEdit edit;
        QVERIFY(!edit.calendarWidget());
        edit.setCalendarPopup(true);
        QVERIFY(edit.calendarWidget());
    }

    {
        BOBUIimeEdit edit;
        QVERIFY(!edit.calendarWidget());
        edit.setCalendarPopup(true);
        QVERIFY(!edit.calendarWidget());
    }

    {
        QDateEdit edit;
        QVERIFY(!edit.calendarWidget());
        QCalendarWidget *cw = new QCalendarWidget;
        edit.setCalendarWidget(cw);
        QVERIFY(!edit.calendarWidget());
        edit.setCalendarPopup(true);
        edit.setCalendarWidget(cw);
        QCOMPARE(edit.calendarWidget(), cw);
        edit.setDateRange(QDate(1980, 1, 5), QDate(1980, 2, 11));
        QCOMPARE(cw->minimumDate(), edit.minimumDate());
        QCOMPARE(cw->maximumDate(), edit.maximumDate());
        edit.setDate(QDate(1980, 1, 31));
        QCOMPARE(edit.date(), cw->selectedDate());
        cw->setSelectedDate(QDate(1980, 1, 30));
        QCOMPARE(edit.date(), cw->selectedDate());
    }

    testWidget->setDisplayFormat("dd/MM/yyyy");
    testWidget->setDateTime(QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0)));
    testWidget->show();
    testWidget->setCalendarPopup(true);
    QCOMPARE(testWidget->calendarPopup(), true);
    QStyle *style = testWidget->style();
    QStyleOptionComboBox opt;
    opt.initFrom(testWidget);
    opt.editable = true;
    opt.subControls = QStyle::SC_ComboBoxArrow;
    QRect rect = style->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxArrow, testWidget);
    BOBUIest::mouseClick(testWidget, BobUI::LeftButton, {},
                      QPoint(rect.left() + rect.width() / 2, rect.top() + rect.height() / 2));
    QWidget *wid = testWidget->findChild<QWidget *>("bobui_datetimedit_calendar");
    QVERIFY(wid != 0);
    testWidget->hide();

    BOBUIimeEdit timeEdit;
    timeEdit.setCalendarPopup(true);
    timeEdit.show();

    opt.initFrom(&timeEdit);
    opt.subControls = QStyle::SC_ComboBoxArrow;
    rect = style->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxArrow, &timeEdit);
    BOBUIest::mouseClick(&timeEdit, BobUI::LeftButton, {},
                      QPoint(rect.left() + rect.width() / 2, rect.top() + rect.height() / 2));
    QWidget *wid2 = timeEdit.findChild<QWidget *>("bobui_datetimedit_calendar");
    QVERIFY(!wid2);
    timeEdit.hide();


    QDateEdit dateEdit;
    dateEdit.setCalendarPopup(true);
    dateEdit.setReadOnly(true);
    dateEdit.show();

    opt.initFrom(&dateEdit);
    opt.subControls = QStyle::SC_ComboBoxArrow;
    rect = style->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxArrow, &dateEdit);
    BOBUIest::mouseClick(&dateEdit, BobUI::LeftButton, {},
                      QPoint(rect.left() + rect.width() / 2, rect.top() + rect.height() / 2));
    QWidget *wid3 = dateEdit.findChild<QWidget *>("bobui_datetimedit_calendar");
    QVERIFY(!wid3);
    dateEdit.hide();
}

class RestoreLayoutDirectioner
{
public:
    RestoreLayoutDirectioner(BobUI::LayoutDirection was)
        : old(was)
    {}

    ~RestoreLayoutDirectioner()
    {
        QApplication::setLayoutDirection(old);
    }
private:
    const BobUI::LayoutDirection old;
};

void tst_QDateTimeEdit::reverseTest()
{
    const RestoreLayoutDirectioner restorer(QApplication::layoutDirection());
    QApplication::setLayoutDirection(BobUI::RightToLeft);
    testWidget->setDisplayFormat("dd/MM/yyyy");
    testWidget->setDate(QDate(2001, 3, 30));
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("2001/03/30"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(testWidget->currentSection(), QDateTimeEdit::DaySection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2001, 3, 31));
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("2001/03/31"));
}

void tst_QDateTimeEdit::hour12Test()
{
    testWidget->setDisplayFormat("hh a");
    testWidget->setTime(BOBUIime(0, 0, 0));
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("12 am"));
    for (int i=0; i<11; ++i) {
        BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    }
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("11 am"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("12 pm"));
    for (int i=0; i<11; ++i) {
        BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    }
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("11 pm"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("11 pm"));
    for (int i=0; i<12; ++i) {
        BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    }
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("11 am"));
    BOBUIest::keyClick(testWidget, BobUI::Key_1);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("1 am"));
    BOBUIest::keyClick(testWidget, BobUI::Key_3);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("1 am"));
}

void tst_QDateTimeEdit::yyTest()
{
    testWidget->setDisplayFormat("dd-MMM-yy");
    testWidget->setDateRange(QDate(2005, 1, 1), QDate(2010, 12, 31));
    testWidget->setDate(testWidget->minimumDate());
    testWidget->setTime(BOBUIime(12, 0, 0)); // Mid-day to avoid DST artefacts.
    testWidget->setCurrentSection(QDateTimeEdit::YearSection);

    QString jan = QLocale::system().monthName(1, QLocale::ShortFormat);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-05");
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-06");
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-07");
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-08");
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-09");
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-10");
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-10");
    testWidget->setWrapping(true);
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->lineEdit()->displayText(), "01-" + jan + "-05");

    testWidget->setDateRange(QDate(100, 1, 1), QDate(8000, 12, 31));
    testWidget->setDate(QDate(2000, 1, 1));
    testWidget->setCurrentSection(QDateTimeEdit::YearSection);
    testWidget->setWrapping(false);
    for (int i=0; i<10; ++i) {
        for (int j=0; j<50; ++j) {
            testWidget->stepBy(-1);
        }
        testWidget->stepBy(-50);
        QCOMPARE(testWidget->sectionText(QDateTimeEdit::YearSection), QString("00"));
        QCOMPARE(testWidget->date(), QDate(2000 - ((i + 1) * 100), 1, 1));
    }
}

void tst_QDateTimeEdit::task108572()
{
    testWidget->setDisplayFormat("hh:mm:ss.zzz");
    testWidget->setTime(BOBUIime(0, 1, 2, 0));
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("00:01:02.000"));

    testWidget->setCurrentSection(QDateTimeEdit::MSecSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("000"));
    BOBUIest::keyClick(testWidget, BobUI::Key_2);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("00:01:02.2"));
    BOBUIest::keyClick(testWidget, BobUI::Key_Return);
    QCOMPARE(testWidget->lineEdit()->displayText(), QString("00:01:02.200"));
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("200"));
    QCOMPARE(testWidget->time(), BOBUIime(0, 1, 2, 200));
}

void tst_QDateTimeEdit::task149097()
{
    QSignalSpy dtSpy(testWidget, SIGNAL(dateTimeChanged(QDateTime)));
    QSignalSpy dSpy(testWidget, SIGNAL(dateChanged(QDate)));
    QSignalSpy tSpy(testWidget, SIGNAL(timeChanged(BOBUIime)));

    testWidget->setDisplayFormat("yyyy/MM/dd hh:mm:ss");
    testWidget->setDateTime(QDateTime(QDate(2001, 02, 03), BOBUIime(5, 1, 2)));
//    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(dtSpy.size(), 1);
    QCOMPARE(dSpy.size(), 1);
    QCOMPARE(tSpy.size(), 1);
    testWidget->setCurrentSection(QDateTimeEdit::YearSection);
    testWidget->stepBy(1);

    QCOMPARE(dtSpy.size(), 2);
    QCOMPARE(dSpy.size(), 2);
    QCOMPARE(tSpy.size(), 1);

    testWidget->setCurrentSection(QDateTimeEdit::MinuteSection);
    testWidget->stepBy(1);

    QCOMPARE(dtSpy.size(), 3);
    QCOMPARE(dSpy.size(), 2);
    QCOMPARE(tSpy.size(), 2);
}

void tst_QDateTimeEdit::task148725()
{
    testWidget->setDisplayFormat("dd/MM");
    testWidget->setDate(QDate(2001, 2, 27));
    testWidget->stepBy(1);
    QCOMPARE(testWidget->date(), QDate(2001, 2, 28));
    testWidget->stepBy(1);
    QCOMPARE(testWidget->date(), QDate(2001, 2, 28));
    testWidget->setWrapping(true);
    testWidget->stepBy(1);
    QCOMPARE(testWidget->date(), QDate(2001, 2, 1));
}

void tst_QDateTimeEdit::task148522()
{
    BOBUIimeEdit edit;
    const QDateTime dt(QDate(2000, 12, 12), BOBUIime(12, 13, 14, 15));
    edit.setDateTime(dt);
    QCOMPARE(edit.dateTime(), dt);
}

void tst_QDateTimeEdit::ddMMMMyyyy()
{
    testWidget->setDisplayFormat("dd.MMMM.yyyy");
    testWidget->setDate(QDate(2000, 1, 1));
    testWidget->setCurrentSection(QDateTimeEdit::YearSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Enter);
    QCOMPARE(testWidget->lineEdit()->selectedText(), QString("2000"));
#ifdef Q_OS_MAC
    BOBUIest::keyClick(testWidget, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(testWidget, BobUI::Key_End);
#endif
    BOBUIest::keyClick(testWidget, BobUI::Key_Backspace);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(testWidget->lineEdit()->text(), "01." + QLocale::system().monthName(1, QLocale::LongFormat) + ".200");
}

void tst_QDateTimeEdit::wheelEvent_data()
{
#if BOBUI_CONFIG(wheelevent)
    BOBUIest::addColumn<QPoint>("angleDelta");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<BobUI::MouseEventSource>("source");
    BOBUIest::addColumn<QDateTimeEdit::Section>("section");
    BOBUIest::addColumn<QDate>("startDate");
    BOBUIest::addColumn<DateList>("expectedDates");

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

    const auto sections = {QDateTimeEdit::DaySection,
                           QDateTimeEdit::MonthSection,
                           QDateTimeEdit::YearSection};

    for (auto fraction : fractions) {
        for (auto up : directions) {

            const QDate startDate(2000, up ? 2 : 12, 17);

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

                        for (const auto section : sections) {

                            DateList expectedDates;
                            if (fraction)
                                expectedDates << startDate;

                            const auto expectedDate = stepDate(startDate, section, steps);
                            if (!expectedDate.isValid())
                                continue;

                            expectedDates << expectedDate;

                            const QLatin1String sectionName = sectionToName(section);

                            BOBUIest::addRow("%s%s%s%sWith%sKeyboardModifier%s",
                                          fraction ? "half" : "full",
                                          up ? "Up" : "Down",
                                          stepModifierName.latin1(),
                                          sectionName.latin1(),
                                          modifierName.latin1(),
                                          sourceName.latin1())
                                    << angleDelta
                                    << static_cast<int>(stepModifier)
                                    << modifiers
                                    << source
                                    << section
                                    << startDate
                                    << expectedDates;
                        }
                    }
                }
            }
        }
    }
#else
    QSKIP("Built with --no-feature-wheelevent");
#endif
}

void tst_QDateTimeEdit::wheelEvent()
{
#if BOBUI_CONFIG(wheelevent)
    QFETCH(QPoint, angleDelta);
    QFETCH(int, stepModifier);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(BobUI::MouseEventSource, source);
    QFETCH(QDateTimeEdit::Section, section);
    QFETCH(QDate, startDate);
    QFETCH(DateList, expectedDates);

    EditorDateEdit edit;
    edit.setDate(startDate);
    edit.setCurrentSection(section);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> style(
                new StepModifierStyle);
    style->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    edit.setStyle(style.data());

    QWheelEvent event(QPointF(), QPointF(), QPoint(), angleDelta,
                      BobUI::NoButton, modifiers, BobUI::NoScrollPhase, false, source);

    QCOMPARE(edit.date(), startDate);
    for (QDate expected : expectedDates) {
        qApp->sendEvent(&edit, &event);
        QCOMPARE(edit.date(), expected);
    }
#else
    QSKIP("Built with --no-feature-wheelevent");
#endif // BOBUI_CONFIG(wheelevent)
}

void tst_QDateTimeEdit::specialValueCornerCase()
{
    // if you set minimum to value it won't update since value won't
    // be bounded to anything. If you have a specialValueText it needs
    // to call updateEdit to make sure the text is changed

    QDateTimeEdit edit;
    edit.setSpecialValueText("foobar");
    edit.setMinimumDate(edit.date());
    QCOMPARE(edit.minimumDate(), edit.date());
    QCOMPARE(edit.text(), QString("foobar"));
}

void tst_QDateTimeEdit::cursorPositionOnInit()
{
    {
        EditorDateEdit edit;
        edit.setDisplayFormat("dd/MM");
        edit.show();
        QCOMPARE(edit.lineEdit()->cursorPosition(), 0);
    }

    {
        EditorDateEdit edit;
        edit.setDisplayFormat("dd/MM");
        edit.setSpecialValueText("special");
        edit.setMinimumDate(edit.date());
        edit.show();
        QCOMPARE(edit.lineEdit()->cursorPosition(), 7);
        // ### legacy behavior. Keep it like this rather than changing
        // ### but add a test none-the-less
    }
}

void tst_QDateTimeEdit::task118867()
{
    EditorDateEdit edit;
    edit.setDisplayFormat("hh:mm");
    edit.setMinimumTime(BOBUIime(5, 30));
    edit.setMaximumTime(BOBUIime(6, 30));
    QCOMPARE(edit.text(), QString("05:30"));
    edit.lineEdit()->setCursorPosition(5);
    BOBUIest::keyClick(&edit, BobUI::Key_Backspace);
    QCOMPARE(edit.text(), QString("05:3"));
    BOBUIest::keyClick(&edit, BobUI::Key_Backspace);
    QCOMPARE(edit.text(), QString("05:"));
    BOBUIest::keyClick(&edit, BobUI::Key_1);
    QCOMPARE(edit.text(), QString("05:"));
    BOBUIest::keyClick(&edit, BobUI::Key_2);
    QCOMPARE(edit.text(), QString("05:"));
    BOBUIest::keyClick(&edit, BobUI::Key_3);
    QCOMPARE(edit.text(), QString("05:3"));
    BOBUIest::keyClick(&edit, BobUI::Key_3);
    QCOMPARE(edit.text(), QString("05:33"));
}

void tst_QDateTimeEdit::nextPrevSection_data()
{
    BOBUIest::addColumn<BobUI::Key>("key");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<QString>("selectedText");

    BOBUIest::newRow("tab") << BobUI::Key_Tab << (BobUI::KeyboardModifiers)BobUI::NoModifier << QString("56");
    BOBUIest::newRow("backtab") << BobUI::Key_Backtab << (BobUI::KeyboardModifiers)BobUI::NoModifier << QString("12");
    BOBUIest::newRow("shift-tab") << BobUI::Key_Tab << (BobUI::KeyboardModifiers)BobUI::ShiftModifier << QString("12");
    BOBUIest::newRow("/") << BobUI::Key_Slash << (BobUI::KeyboardModifiers)BobUI::NoModifier << QString("56");
    BOBUIest::newRow("b") << BobUI::Key_B << (BobUI::KeyboardModifiers)BobUI::NoModifier << QString("56");
    BOBUIest::newRow("c") << BobUI::Key_C << (BobUI::KeyboardModifiers)BobUI::NoModifier << QString("56");

    // 1. mac doesn't do these,
    // 2. some WinCE devices do not have modifiers
#if !defined(Q_OS_DARWIN)
    BOBUIest::newRow("ctrl-right") << BobUI::Key_Right << (BobUI::KeyboardModifiers)BobUI::ControlModifier << QString("56");
    BOBUIest::newRow("ctrl-left") << BobUI::Key_Left << (BobUI::KeyboardModifiers)BobUI::ControlModifier << QString("12");
#endif
}

void tst_QDateTimeEdit::nextPrevSection()
{
    QFETCH(BobUI::Key, key);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(QString, selectedText);

    EditorDateEdit edit;
    edit.setDisplayFormat("hh/mm/bc9ss");
    edit.setTime(BOBUIime(12, 34, 56));
    edit.show();
    edit.setSelectedSection(QDateTimeEdit::MinuteSection);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("34")); // selftest
    BOBUIest::keyClick(&edit, key, modifiers);
    QCOMPARE(edit.lineEdit()->selectedText(), selectedText);
}

void tst_QDateTimeEdit::dateEditTimeEditFormats()
{
    BOBUIimeEdit t;
    t.setDisplayFormat("hh yyyy");
    QCOMPARE(t.displayedSections(), QDateTimeEdit::HourSection);

    QDateEdit d;
    d.setDisplayFormat("hh yyyy");
    QCOMPARE(d.displayedSections(), QDateTimeEdit::YearSection);
}

#if BOBUI_DEPRECATED_SINCE(6, 10)
void tst_QDateTimeEdit::timeSpec_data()
{
    BOBUIest::addColumn<bool>("useSetProperty");
    BOBUIest::newRow("setProperty") << true;
    BOBUIest::newRow("setTimeSpec") << false;
}

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
void tst_QDateTimeEdit::timeSpec()
{
    QFETCH(bool, useSetProperty);

    QDateTimeEdit edit;
    QCOMPARE(edit.dateTime().timeSpec(), edit.timeSpec());
    QCOMPARE(edit.minimumDateTime().timeSpec(), edit.timeSpec());
    QCOMPARE(edit.maximumDateTime().timeSpec(), edit.timeSpec());
    if (useSetProperty) {
        edit.setProperty("timeSpec", BobUI::UTC);
    } else {
        edit.setTimeSpec(BobUI::UTC);
    }
    QCOMPARE(edit.minimumDateTime().timeSpec(), edit.timeSpec());
    QCOMPARE(edit.maximumDateTime().timeSpec(), edit.timeSpec());
    QCOMPARE(edit.dateTime().timeSpec(), edit.timeSpec());
    if (useSetProperty) {
        edit.setProperty("timeSpec", BobUI::LocalTime);
    } else {
        edit.setTimeSpec(BobUI::LocalTime);
    }
    const QDateTime dt = edit.dateTime();
    QCOMPARE(edit.timeSpec(), BobUI::LocalTime);
    const QDateTime utc = dt.toUTC();
    if (dt.time() != utc.time()) {
        const QDateTime min(QDate(1999, 1, 1), BOBUIime(1, 0));
        edit.setMinimumDateTime(min);
        QCOMPARE(edit.minimumTime(), min.time());
        if (useSetProperty) {
            edit.setProperty("timeSpec", BobUI::UTC);
        } else {
            edit.setTimeSpec(BobUI::UTC);
        }
        QVERIFY(edit.minimumTime() != min.time());
        QVERIFY(edit.minimumDateTime().timeSpec() != min.timeSpec());
        QCOMPARE(edit.minimumDateTime().toSecsSinceEpoch(), min.toSecsSinceEpoch());
    } else {
        QSKIP("Not tested in the GMT timezone");
    }
}
BOBUI_WARNING_POP
#endif // test deprecated timeSpec property

void tst_QDateTimeEdit::timeZoneBug()
{
    testWidget->setTimeZone(BOBUIimeZone::UTC);
    testWidget->setDisplayFormat("hh:mm");
    testWidget->setTime(BOBUIime(2, 2));
    const QString oldText = testWidget->text();
    const QDateTime oldDateTime = testWidget->dateTime();
    BOBUIest::keyClick(testWidget, BobUI::Key_Tab);
    QCOMPARE(oldDateTime, testWidget->dateTime());
    QCOMPARE(oldText, testWidget->text());
}

void tst_QDateTimeEdit::timeZoneInit()
{
    QDateTime utc(QDate(2000, 1, 1), BOBUIime(12, 0), BOBUIimeZone::UTC);
    QDateTimeEdit widget(utc);
    QCOMPARE(widget.dateTime(), utc);
}

void tst_QDateTimeEdit::setDateTime_data()
{
    const QDateTime localNoon(QDate(2019, 12, 24), BOBUIime(12, 0));
    const BOBUIimeZone UTC(BOBUIimeZone::UTC), local(BOBUIimeZone::LocalTime);
    BOBUIest::addColumn<BOBUIimeZone>("zone");
    BOBUIest::addColumn<QDateTime>("store");
    BOBUIest::addColumn<QDateTime>("expect");
    BOBUIest::newRow("LocalTime/LocalTime") << local << localNoon << localNoon;
    BOBUIest::newRow("LocalTime/UTC") << local << localNoon.toUTC() << localNoon;
    BOBUIest::newRow("UTC/LocalTime") << UTC << localNoon << localNoon.toUTC();
    BOBUIest::newRow("UTC/UTC") << UTC << localNoon.toUTC() << localNoon.toUTC();
}

void tst_QDateTimeEdit::setDateTime()
{
    QFETCH(const BOBUIimeZone, zone);
    QFETCH(const QDateTime, store);
    QFETCH(const QDateTime, expect);
    QDateTimeEdit editor;
    editor.setTimeZone(zone);
    editor.setDateTime(store);
    QCOMPARE(editor.dateTime(), expect);
}

void tst_QDateTimeEdit::cachedDayTest()
{
    testWidget->setDisplayFormat("MM/dd");
    testWidget->setDate(QDate(2007, 1, 30));
    testWidget->setCurrentSection(QDateTimeEdit::DaySection);
    //BOBUIest::keyClick(testWidget->lineEdit(), BobUI::Key_Up); // this doesn't work
    //on Mac. BobUI Test bug? ###
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    testWidget->setCurrentSection(QDateTimeEdit::MonthSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2007, 2, 28));
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2007, 3, 31));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2007, 2, 28));
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2007, 1, 31));

    testWidget->setCurrentSection(QDateTimeEdit::DaySection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Down);
    QCOMPARE(testWidget->date(), QDate(2007, 1, 30));
    testWidget->setCurrentSection(QDateTimeEdit::MonthSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2007, 2, 28));
    testWidget->setCurrentSection(QDateTimeEdit::MonthSection);
    BOBUIest::keyClick(testWidget, BobUI::Key_Up);
    QCOMPARE(testWidget->date(), QDate(2007, 3, 30));
}

void tst_QDateTimeEdit::monthEdgeCase()
{
    EditorDateEdit edit;
    edit.setLocale(QLocale("fr_FR"));
    edit.setDisplayFormat("MMM d");
    edit.setDate(QDate(2000, 1, 1));
    QCOMPARE(edit.text(), QString("janv. 1"));
    edit.lineEdit()->setCursorPosition(5);
    BOBUIest::keyClick(&edit, BobUI::Key_Backspace);
    QCOMPARE(edit.text(), QString("janv 1"));
}

class RestoreLocaler
{
public:
    RestoreLocaler()
        : old(QLocale())
    {}

    ~RestoreLocaler()
    {
        QLocale::setDefault(old);
    }
private:
    const QLocale old;
};

void tst_QDateTimeEdit::setLocale()
{
    RestoreLocaler r;
    QLocale::setDefault(QLocale("en_US"));
    {
        EditorDateEdit edit;
        edit.setDisplayFormat("MMMM d");
        edit.setDate(QDate(2000, 1, 1));
        QCOMPARE(edit.text(), QString("January 1"));
    }
    QLocale::setDefault(QLocale("no_NO"));
    {
        EditorDateEdit edit;
        edit.setDisplayFormat("MMMM d");
        edit.setDate(QDate(2000, 1, 1));
        QCOMPARE(edit.text().toLower(), QString("januar 1"));
        // I get Januar on Mac and januar on linux
    }
    QLocale::setDefault(QLocale("en_US"));
    {
        EditorDateEdit edit;
        edit.setDisplayFormat("MMMM d");
        edit.setDate(QDate(2000, 1, 1));
        QCOMPARE(edit.text(), QString("January 1"));
        edit.setLocale(QLocale("no_NO"));
        QCOMPARE(edit.text().toLower(), QString("januar 1"));
        edit.setLocale(QLocale("no_NO"));
    }
}

void tst_QDateTimeEdit::potentialYYValueBug()
{
    EditorDateEdit edit;
    edit.setDisplayFormat("dd/MM/yy");
    QCOMPARE(edit.minimumDate(), QDate(1752, 9, 14));
    edit.setDate(edit.minimumDate());
    edit.lineEdit()->setFocus();

#ifdef Q_OS_MAC
    BOBUIest::keyClick(&edit, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(&edit, BobUI::Key_End);
#endif
    BOBUIest::keyClick(&edit, BobUI::Key_Backspace);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(edit.text(), QString("14/09/5"));
}

void tst_QDateTimeEdit::textSectionAtEnd()
{
    EditorDateEdit edit;
    edit.setDisplayFormat("MMMM");
    edit.setDate(QDate(2000, 1, 1));
    edit.lineEdit()->setFocus();
#ifdef Q_OS_MAC
    BOBUIest::keyClick(&edit, BobUI::Key_Right, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(&edit, BobUI::Key_End);
#endif
    BOBUIest::keyClick(&edit, BobUI::Key_Backspace);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "BOBUIBUG-23674", Abort);
#endif
    QCOMPARE(edit.text(), QString("Januar"));
}

void tst_QDateTimeEdit::keypadAutoAdvance_data()
{
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::newRow("None") << (BobUI::KeyboardModifiers)BobUI::NoModifier;
    BOBUIest::newRow("Keypad") << (BobUI::KeyboardModifiers)BobUI::KeypadModifier;
    // BOBUIBUG-7842: Using KeyPad with shift (numlock off)
    BOBUIest::newRow("Keypad+Shift") << (BobUI::KeyboardModifiers)(BobUI::KeypadModifier|BobUI::ShiftModifier);
}

void tst_QDateTimeEdit::keypadAutoAdvance()
{
    QFETCH(BobUI::KeyboardModifiers, modifiers);

    EditorDateEdit edit;
    edit.setDate(QDate(2000, 2, 1));
    edit.setDisplayFormat("dd/MM");
#ifdef Q_OS_MAC
    BOBUIest::keyClick(&edit, BobUI::Key_Left, BobUI::ControlModifier);
#else
    BOBUIest::keyClick(&edit, BobUI::Key_Home);
#endif
    BOBUIest::keyClick(&edit, BobUI::Key_Return);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("01"));
    BOBUIest::keyClick(&edit, BobUI::Key_1, modifiers);
    BOBUIest::keyClick(&edit, BobUI::Key_2, modifiers);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("02"));
}

void tst_QDateTimeEdit::task196924()
{
    EditorDateEdit edit;
    edit.setDisplayFormat("dd/M/yyyy");
    edit.setDate(QDate(2345, 6, 17));
    QCOMPARE(edit.text(), QString("17/6/2345"));
    BOBUIest::keyClick(&edit, BobUI::Key_Tab);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("6"));
    BOBUIest::keyClick(&edit, BobUI::Key_3);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("2345"));
    QCOMPARE(edit.text(), QString("17/3/2345"));

    edit.setDisplayFormat("dd/MM/yyyy");
    edit.setDate(QDate(2345, 6, 17));
    edit.lineEdit()->setCursorPosition(0);
    QCOMPARE(edit.text(), QString("17/06/2345"));
    BOBUIest::keyClick(&edit, BobUI::Key_Tab);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("06"));
    BOBUIest::keyClick(&edit, BobUI::Key_3);
    QCOMPARE(edit.lineEdit()->selectedText(), QString("2345"));
    QCOMPARE(edit.text(), QString("17/03/2345"));
}

void tst_QDateTimeEdit::focusNextPrevChild()
{
    EditorDateEdit edit;
    edit.setDisplayFormat("dd/MM/yyyy");

    edit.show();
    edit.setFocus();
    edit.setCurrentSection(QDateTimeEdit::DaySection);

    QCOMPARE(edit.currentSection(), QDateTimeEdit::DaySection);
    edit.focusNextPrevChild(true);
    QCOMPARE(edit.currentSection(), QDateTimeEdit::MonthSection);
}

void tst_QDateTimeEdit::taskBOBUIBUG_12384_timeZoneShowTimeOnly()
{
    QDateTime time = QDateTime::fromString("20100723 04:02:40", "yyyyMMdd hh:mm:ss");
    time.setTimeZone(BOBUIimeZone::UTC);

    EditorDateEdit edit;
    edit.setDisplayFormat("hh:mm:ss");
    edit.setTimeZone(BOBUIimeZone::UTC);
    edit.setDateTime(time);

    QCOMPARE(edit.minimumTime(), BOBUIime(0, 0, 0, 0));
    QCOMPARE(edit.maximumTime(), BOBUIime(23, 59, 59, 999));
    QCOMPARE(edit.time(), time.time());
}

void tst_QDateTimeEdit::deleteCalendarWidget()
{
    {
        // setup
        QDateEdit edit;
        QVERIFY(!edit.calendarWidget());
        edit.setCalendarPopup(true);
        QVERIFY(edit.calendarWidget());
        edit.calendarWidget()->setObjectName("cw1");

        // delete
        delete edit.calendarWidget();

        // it should create a new widget
        QVERIFY(edit.calendarWidget());
        QVERIFY(edit.calendarWidget()->objectName() != QLatin1String("cw1"));
    }
}

void tst_QDateTimeEdit::setLocaleOnCalendarWidget()
{
    QDateEdit dateEdit;
    QList<QLocale> allLocales = QLocale::matchingLocales(
                QLocale::AnyLanguage,
                QLocale::AnyScript,
                QLocale::AnyTerritory);
    QLocale c = QLocale::c();
    dateEdit.setCalendarPopup(true);
    dateEdit.setLocale(c);
    for (const QLocale& l : allLocales) {
        dateEdit.setLocale(l);
        const QLocale locCal = dateEdit.calendarWidget()->locale();
        const QLocale locEdit = dateEdit.locale();
        QCOMPARE(locCal.name(), locEdit.name());
        QVERIFY(locCal == locEdit);
    }
}

#ifdef BOBUI_BUILD_INTERNAL

using KeyPair = std::pair<BobUI::Key, BobUI::KeyboardModifier>;
typedef QList<KeyPair> KeyPairList;

Q_DECLARE_METATYPE(KeyPair)
/*
When a QDateEdit has its display format set to 'yyyy/MM/dd', its day
set to 31 and its month set to 2, it will display 291 as the day until
the cursor is moved or the focus changed. This is because
QDateTimeParser::parse calls sectionSize() for the day section, which
returns 1 when it should return 2.

This test verifies that QDateTimeEditPrivate has the correct text,
which is the text that is displayed until the cursor is moved or the
focus changed.
*/

void tst_QDateTimeEdit::dateEditCorrectSectionSize_data()
{
    BOBUIest::addColumn<QLocale>("locale");
    BOBUIest::addColumn<QDate>("defaultDate");
    BOBUIest::addColumn<QString>("displayFormat");
    BOBUIest::addColumn<KeyPairList>("keyPresses");
    BOBUIest::addColumn<QDate>("expectedDate");
    BOBUIest::addColumn<QString>("expectedDisplayString");

    const auto key = [](BobUI::Key key, BobUI::KeyboardModifier modifier = BobUI::NoModifier) {
        return KeyPair(key, modifier);
    };
    const QDate y2kStart(2000, 1, 1), start2001(2001, 1, 1);
    const QLocale ozzy(QLocale::English, QLocale::Australia);
    const KeyPairList thirtyUpKeypresses(30, key(BobUI::Key_Up));

    // Make day the current section, set day to 31st (invalid for february),
    // move to month field, set month to february (2).
    KeyPairList threeDigitDayIssueKeypresses;
    threeDigitDayIssueKeypresses << key(BobUI::Key_Tab) << key(BobUI::Key_Tab)
        << key(BobUI::Key_3) << key(BobUI::Key_1) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_2);

    // Same as above, except day-year-month format.
    KeyPairList threeDigitDayIssueKeypresses_DayYearMonth;
    threeDigitDayIssueKeypresses_DayYearMonth << key(BobUI::Key_3) << key(BobUI::Key_1) << key(BobUI::Key_Tab)
        << key(BobUI::Key_2);

    // Same as threeDigitDayIssueKeypresses, except doesn't require the day to be corrected.
    KeyPairList threeDigitDayIssueKeypresses_Nofixday;
    threeDigitDayIssueKeypresses_Nofixday << key(BobUI::Key_Tab) << key(BobUI::Key_Tab)
        << key(BobUI::Key_2) << key(BobUI::Key_8) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_2);

    // Set day to 31st (invalid for february), set month to february (2).
    KeyPairList reverseThreeDigitDayIssueKeypresses;
    reverseThreeDigitDayIssueKeypresses
        << key(BobUI::Key_3) << key(BobUI::Key_1) << key(BobUI::Key_2);

    // Make day the current section, set day to 31st, move to month field, set month to november (11).
    KeyPairList threeDigitDayIssueKeypresses_TwoDigitMonth;
    threeDigitDayIssueKeypresses_TwoDigitMonth << key(BobUI::Key_Tab) << key(BobUI::Key_Tab) << key(BobUI::Key_3)
        << key(BobUI::Key_1) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_1) << key(BobUI::Key_1);

    // Make day the current section, set day to 3rd, move to month field, set month to february (2).
    KeyPairList threeDigitDayIssueKeypresses_OneDigitDay;
    threeDigitDayIssueKeypresses_OneDigitDay << key(BobUI::Key_Tab) << key(BobUI::Key_Tab)
        << key(BobUI::Key_3) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_2);

    // Make day the current section, set day to 31st (invalid for february), move to month field,
    // set month to february (2).
    KeyPairList threeDigitDayIssueKeypresses_ShortMonthName;
    threeDigitDayIssueKeypresses_ShortMonthName << key(BobUI::Key_Tab) << key(BobUI::Key_Tab)
        << key(BobUI::Key_3) << key(BobUI::Key_1) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_Up);

    // Make day the current section, set day to 31st (Monday), move to month field, set month to february (2).
    // Will probably never see this display format in a QDateTimeEdit, but it's good to test it anyway.
    KeyPairList threeDigitDayIssueKeypresses_DayName;
    threeDigitDayIssueKeypresses_DayName << key(BobUI::Key_Tab) << key(BobUI::Key_Tab) << thirtyUpKeypresses
        << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_2);

    KeyPairList threeDigitDayIssueKeypresses_DayName_DayYearMonth;
    threeDigitDayIssueKeypresses_DayName_DayYearMonth << thirtyUpKeypresses << key(BobUI::Key_Tab)
        << key(BobUI::Key_Tab) << key(BobUI::Key_2);

    KeyPairList threeDigitDayIssueKeypresses_DayName_YearDayMonth;
    threeDigitDayIssueKeypresses_DayName_YearDayMonth << key(BobUI::Key_Tab) << thirtyUpKeypresses
        << key(BobUI::Key_Tab) << key(BobUI::Key_2);

    KeyPairList threeDigitDayIssueKeypresses_DayName_DayMonthYear;
    threeDigitDayIssueKeypresses_DayName_DayMonthYear << thirtyUpKeypresses << key(BobUI::Key_Tab)
        << key(BobUI::Key_2);

    KeyPairList threeDigitDayIssueKeypresses_DayName_MonthDayYear;
    threeDigitDayIssueKeypresses_DayName_MonthDayYear << key(BobUI::Key_Tab) << thirtyUpKeypresses
        << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_2);

    // Make day the current section, set day to 31st (invalid for february), move to month field,
    // set month to february (2).
    KeyPairList threeDigitDayIssueKeypresses_YearDayMonth;
    threeDigitDayIssueKeypresses_YearDayMonth << key(BobUI::Key_Tab) << key(BobUI::Key_3) << key(BobUI::Key_1)
        << key(BobUI::Key_Tab) << key(BobUI::Key_2);

    // Make day the current section, set day to 31st, move to month field, set month to february (2).
    KeyPairList threeDigitDayIssueKeypresses_MonthDayYear;
    threeDigitDayIssueKeypresses_MonthDayYear << key(BobUI::Key_Tab) << key(BobUI::Key_3) << key(BobUI::Key_1)
        << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_2);

    // Same as above, except month-year-day format.
    KeyPairList threeDigitDayIssueKeypresses_MonthYearDay;
    threeDigitDayIssueKeypresses_MonthYearDay << key(BobUI::Key_Tab) << key(BobUI::Key_Tab) << key(BobUI::Key_3)
        << key(BobUI::Key_1) << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_Tab, BobUI::ShiftModifier)
        << key(BobUI::Key_2);

    // Same as above, except short month name.
    KeyPairList threeDigitDayIssueKeypresses_ShortMonthName_MonthYearDay;
    threeDigitDayIssueKeypresses_ShortMonthName_MonthYearDay << key(BobUI::Key_Tab) << key(BobUI::Key_Tab)
        << key(BobUI::Key_3) << key(BobUI::Key_1) << key(BobUI::Key_Tab, BobUI::ShiftModifier)
        << key(BobUI::Key_Tab, BobUI::ShiftModifier) << key(BobUI::Key_Up);

    KeyPairList shortAndLongNameIssueKeypresses;
    shortAndLongNameIssueKeypresses << key(BobUI::Key_Tab) << key(BobUI::Key_3) << key(BobUI::Key_1) << key(BobUI::Key_Up);

    // When day-of-week is specified, rather than day-of-month, changing month
    // cares more about preserving day-of-week than day-of-month, so Jan/31 ->
    // Feb picks 28th even in a leap year, as that's exactly four weeks later.
    BOBUIest::newRow("no fixday, leap, yy/M/dddd")
        << ozzy << y2kStart << QString::fromLatin1("yy/M/dddd")
        << threeDigitDayIssueKeypresses_DayName
        << QDate(2000, 2, 28) << QString::fromLatin1("00/2/Monday");

    BOBUIest::newRow("no fixday, leap, yy/M/ddd")
        << ozzy << y2kStart << QString::fromLatin1("yy/M/ddd")
        << threeDigitDayIssueKeypresses_DayName
        << QDate(2000, 2, 28) << QString::fromLatin1("00/2/Mon");

    BOBUIest::newRow("no fixday, leap, yy/MM/dddd")
        << ozzy << y2kStart << QString::fromLatin1("yy/MM/dddd")
        << threeDigitDayIssueKeypresses_DayName
        << QDate(2000, 2, 28) << QString::fromLatin1("00/02/Monday");

    BOBUIest::newRow("fixday, leap, yy/MM/dd")
        << ozzy << y2kStart << QString::fromLatin1("yy/MM/dd")
        << threeDigitDayIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("00/02/29");

    BOBUIest::newRow("fixday, leap, yy/MM/d")
        << ozzy << y2kStart << QString::fromLatin1("yy/MM/d")
        << threeDigitDayIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("00/02/29");

    BOBUIest::newRow("fixday, leap, yyyy/M/d")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/M/d")
        << threeDigitDayIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/2/29");

    BOBUIest::newRow("no fixday, yyyy/M/d")
        << ozzy << start2001 << QString::fromLatin1("yyyy/M/d")
        << threeDigitDayIssueKeypresses_Nofixday
        << QDate(2001, 2, 28) << QString::fromLatin1("2001/2/28");

    BOBUIest::newRow("fixday, leap, 2-digit month, yyyy/M/dd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/M/dd")
        << threeDigitDayIssueKeypresses_TwoDigitMonth
        << QDate(2000, 11, 30) << QString::fromLatin1("2000/11/30");

    BOBUIest::newRow("no fixday, leap, 1-digit day, yyyy/M/dd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/M/dd")
        << threeDigitDayIssueKeypresses_OneDigitDay
        << QDate(2000, 2, 3) << QString::fromLatin1("2000/2/03");

    BOBUIest::newRow("fixday, leap, yyyy/MM/dd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/MM/dd")
        << threeDigitDayIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/02/29");

    BOBUIest::newRow("no fixday, yyyy/MM/dd")
        << ozzy << start2001 << QString::fromLatin1("yyyy/MM/dd")
        << threeDigitDayIssueKeypresses_Nofixday
        << QDate(2001, 2, 28) << QString::fromLatin1("2001/02/28");

    BOBUIest::newRow("fixday, leap, 2-digit month, yyyy/MM/dd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/MM/dd")
        << threeDigitDayIssueKeypresses_TwoDigitMonth
        << QDate(2000, 11, 30) << QString::fromLatin1("2000/11/30");

    BOBUIest::newRow("no fixday, leap, yyyy/M/dddd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/M/dddd")
        << threeDigitDayIssueKeypresses_DayName
        << QDate(2000, 2, 28) << QString::fromLatin1("2000/2/Monday");

    BOBUIest::newRow("no fixday, leap, yyyy/MM/dddd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/MM/dddd")
        << threeDigitDayIssueKeypresses_DayName
        << QDate(2000, 2, 28) << QString::fromLatin1("2000/02/Monday");

    BOBUIest::newRow("fixday, leap, yyyy/dd/MM")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/dd/MM")
        << threeDigitDayIssueKeypresses_YearDayMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/29/02");

    BOBUIest::newRow("fixday, leap, yyyy/dd/M")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/dd/M")
        << threeDigitDayIssueKeypresses_YearDayMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/29/2");

    BOBUIest::newRow("fixday, leap, yyyy/d/M")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/d/M")
        << threeDigitDayIssueKeypresses_YearDayMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/29/2");

    BOBUIest::newRow("fixday, leap, yyyy/MMM/dd")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/MMM/dd")
        << threeDigitDayIssueKeypresses_ShortMonthName
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/Feb/29");

    BOBUIest::newRow("fixday, leap, yyyy/MMM/d")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/MMM/d")
        << threeDigitDayIssueKeypresses_ShortMonthName
        << QDate(2000, 2, 29) << QString::fromLatin1("2000/Feb/29");

    BOBUIest::newRow("fixday, leap, yy/MMM/dd")
        << ozzy << y2kStart << QString::fromLatin1("yy/MMM/dd")
        << threeDigitDayIssueKeypresses_ShortMonthName
        << QDate(2000, 2, 29) << QString::fromLatin1("00/Feb/29");

    BOBUIest::newRow("fixday, leap, yyyy/dddd/M")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/dddd/M")
        << threeDigitDayIssueKeypresses_DayName_YearDayMonth
        << QDate(2000, 2, 28) << QString::fromLatin1("2000/Monday/2");

    BOBUIest::newRow("fixday, leap, yyyy/dddd/MM")
        << ozzy << y2kStart << QString::fromLatin1("yyyy/dddd/MM")
        << threeDigitDayIssueKeypresses_DayName_YearDayMonth
        << QDate(2000, 2, 28) << QString::fromLatin1("2000/Monday/02");

    BOBUIest::newRow("fixday, leap, d/M/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("d/M/yyyy")
        << reverseThreeDigitDayIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("29/2/2000");

    BOBUIest::newRow("fixday, leap, dd/MM/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("dd/MM/yyyy")
        << reverseThreeDigitDayIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("29/02/2000");

    BOBUIest::newRow("fixday, dd/MM/yyyy")
        << ozzy << start2001 << QString::fromLatin1("dd/MM/yyyy")
        << reverseThreeDigitDayIssueKeypresses
        << QDate(2001, 2, 28) << QString::fromLatin1("28/02/2001");

    BOBUIest::newRow("fixday, leap, dddd/MM/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("dddd/MM/yyyy")
        << threeDigitDayIssueKeypresses_DayName_DayMonthYear
        << QDate(2000, 2, 28) << QString::fromLatin1("Monday/02/2000");

    BOBUIest::newRow("fixday, leap, d/yy/M")
        << ozzy << y2kStart << QString::fromLatin1("d/yy/M")
        << threeDigitDayIssueKeypresses_DayYearMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("29/00/2");

    BOBUIest::newRow("fixday, leap, d/yyyy/M")
        << ozzy << y2kStart << QString::fromLatin1("d/yyyy/M")
        << threeDigitDayIssueKeypresses_DayYearMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("29/2000/2");

    BOBUIest::newRow("fixday, leap, d/yyyy/MM")
        << ozzy << y2kStart << QString::fromLatin1("d/yyyy/MM")
        << threeDigitDayIssueKeypresses_DayYearMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("29/2000/02");

    BOBUIest::newRow("fixday, leap, dd/yy/MM")
        << ozzy << y2kStart << QString::fromLatin1("dd/yy/MM")
        << threeDigitDayIssueKeypresses_DayYearMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("29/00/02");

    BOBUIest::newRow("fixday, leap, dd/yyyy/M")
        << ozzy << y2kStart << QString::fromLatin1("dd/yyyy/M")
        << threeDigitDayIssueKeypresses_DayYearMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("29/2000/2");

    BOBUIest::newRow("fixday, leap, dd/yyyy/MM")
        << ozzy << y2kStart << QString::fromLatin1("dd/yyyy/MM")
        << threeDigitDayIssueKeypresses_DayYearMonth
        << QDate(2000, 2, 29) << QString::fromLatin1("29/2000/02");

    BOBUIest::newRow("fixday, leap, dddd/yy/M")
        << ozzy << y2kStart << QString::fromLatin1("dddd/yy/M")
        << threeDigitDayIssueKeypresses_DayName_DayYearMonth
        << QDate(2000, 2, 28) << QString::fromLatin1("Monday/00/2");

    BOBUIest::newRow("fixday, leap, dddd/yy/MM")
        << ozzy << y2kStart << QString::fromLatin1("dddd/yy/MM")
        << threeDigitDayIssueKeypresses_DayName_DayYearMonth
        << QDate(2000, 2, 28) << QString::fromLatin1("Monday/00/02");

    BOBUIest::newRow("fixday, leap, M/d/yy")
        << ozzy << y2kStart << QString::fromLatin1("M/d/yy")
        << threeDigitDayIssueKeypresses_MonthDayYear
        << QDate(2000, 2, 29) << QString::fromLatin1("2/29/00");

    BOBUIest::newRow("fixday, leap, M/d/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("M/d/yyyy")
        << threeDigitDayIssueKeypresses_MonthDayYear
        << QDate(2000, 2, 29) << QString::fromLatin1("2/29/2000");

    BOBUIest::newRow("fixday, leap, M/dd/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("M/dd/yyyy")
        << threeDigitDayIssueKeypresses_MonthDayYear
        << QDate(2000, 2, 29) << QString::fromLatin1("2/29/2000");

    BOBUIest::newRow("fixday, leap, M/dddd/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("M/dddd/yyyy")
        << threeDigitDayIssueKeypresses_DayName_MonthDayYear
        << QDate(2000, 2, 28) << QString::fromLatin1("2/Monday/2000");

    BOBUIest::newRow("fixday, leap, MM/dd/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("MM/dd/yyyy")
        << threeDigitDayIssueKeypresses_MonthDayYear
        << QDate(2000, 2, 29) << QString::fromLatin1("02/29/2000");

    BOBUIest::newRow("fixday, leap, MM/dddd/yyyy")
        << ozzy << y2kStart << QString::fromLatin1("MM/dddd/yyyy")
        << threeDigitDayIssueKeypresses_DayName_MonthDayYear
        << QDate(2000, 2, 28) << QString::fromLatin1("02/Monday/2000");

    BOBUIest::newRow("fixday, leap, M/yyyy/dd")
        << ozzy << y2kStart << QString::fromLatin1("M/yyyy/dd")
        << threeDigitDayIssueKeypresses_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("2/2000/29");

    BOBUIest::newRow("fixday, leap, M/yy/dd")
        << ozzy << y2kStart << QString::fromLatin1("M/yy/dd")
        << threeDigitDayIssueKeypresses_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("2/00/29");

    BOBUIest::newRow("fixday, leap, M/yy/d")
        << ozzy << y2kStart << QString::fromLatin1("M/yy/d")
        << threeDigitDayIssueKeypresses_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("2/00/29");

    BOBUIest::newRow("fixday, leap, MM/yyyy/dd")
        << ozzy << y2kStart << QString::fromLatin1("MM/yyyy/dd")
        << threeDigitDayIssueKeypresses_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("02/2000/29");

    BOBUIest::newRow("fixday, leap, MMM/yy/d")
        << ozzy << y2kStart << QString::fromLatin1("MMM/yy/d")
        << threeDigitDayIssueKeypresses_ShortMonthName_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("Feb/00/29");

    BOBUIest::newRow("fixday, leap, MMM/yyyy/d")
        << ozzy << y2kStart << QString::fromLatin1("MMM/yyyy/d")
        << threeDigitDayIssueKeypresses_ShortMonthName_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("Feb/2000/29");

    BOBUIest::newRow("fixday, MMM/yyyy/d")
        << ozzy << start2001 << QString::fromLatin1("MMM/yyyy/d")
        << threeDigitDayIssueKeypresses_ShortMonthName_MonthYearDay
        << QDate(2001, 2, 28) << QString::fromLatin1("Feb/2001/28");

    BOBUIest::newRow("fixday, leap, MMM/yyyy/dd")
        << ozzy << y2kStart << QString::fromLatin1("MMM/yyyy/dd")
        << threeDigitDayIssueKeypresses_ShortMonthName_MonthYearDay
        << QDate(2000, 2, 29) << QString::fromLatin1("Feb/2000/29");

    BOBUIest::newRow("fixday, leap, dddd, dd. MMMM yyyy")
        << ozzy << y2kStart << QString::fromLatin1("dddd, dd. MMMM yyyy")
        << shortAndLongNameIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("Tuesday, 29. February 2000");

    BOBUIest::newRow("fixday, leap, german, dddd, dd. MMMM yyyy")
        << QLocale(QLocale::German, QLocale::Germany) << y2kStart
        << QString::fromLatin1("dddd, dd. MMMM yyyy") << shortAndLongNameIssueKeypresses
        << QDate(2000, 2, 29) << QString::fromLatin1("Dienstag, 29. Februar 2000");
}

void tst_QDateTimeEdit::dateEditCorrectSectionSize()
{
    QFETCH(const QLocale, locale);
    QFETCH(const QDate, defaultDate);
    QFETCH(const QString, displayFormat);
    QFETCH(const KeyPairList, keyPresses);
    QFETCH(const QDate, expectedDate);
    QFETCH(const QString, expectedDisplayString);

    QDateEdit edit;
    edit.setLocale(locale);
    edit.setDate(defaultDate);
    edit.setDisplayFormat(displayFormat);
    edit.show();
    edit.setFocus();
    // Day-of-week tests rely on advance through week advancing the
    // day-of-month, so not stopping at the locale's first day of the week:
    edit.setWrapping(true);
    // For some reason, we need to set the selected section for the dd/MM/yyyy tests,
    // otherwise the 3 is inserted at the front of 01/01/2000 (301/01/2000), instead of the
    // selected text being replaced. This is not an issue for the yyyy/MM/dd format though...
    edit.setSelectedSection(edit.sectionAt(0));

    for (const KeyPair &keyPair : keyPresses)
        BOBUIest::keyClick(&edit, keyPair.first, keyPair.second);

    QCOMPARE(edit.date(), expectedDate);
    QDateTimeEditPrivate* edit_d_ptr(static_cast<QDateTimeEditPrivate*>(bobui_widget_private(&edit)));
    QCOMPARE(edit_d_ptr->QDateTimeParser::displayText(), expectedDisplayString);
}
#endif

void tst_QDateTimeEdit::stepModifierKeys_data()
{
    BOBUIest::addColumn<QDate>("startDate");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<QDateTimeEdit::Section>("section");
    BOBUIest::addColumn<BOBUIestEventList>("keys");
    BOBUIest::addColumn<QDate>("expectedDate");

    const auto keyList = {BobUI::Key_Up, BobUI::Key_Down};

    const auto modifierList = {BobUI::NoModifier,
                                   BobUI::ShiftModifier,
                                   BobUI::ControlModifier,
                                   BobUI::AltModifier,
                                   BobUI::MetaModifier};

    const auto validStepModifierList = {BobUI::NoModifier,
                                        BobUI::ControlModifier,
                                        BobUI::ShiftModifier};

    const auto sections = {QDateTimeEdit::DaySection,
                           QDateTimeEdit::MonthSection,
                           QDateTimeEdit::YearSection};

    for (auto key : keyList) {

        const bool up = key == BobUI::Key_Up;
        Q_ASSERT(up || key == BobUI::Key_Down);

        const QDate startDate(2000, up ? 2 : 12, 17);

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

                for (const auto section : sections) {

                    const auto expectedDate = stepDate(startDate, section, steps);
                    if (!expectedDate.isValid())
                        continue;

                    const auto sectionName = sectionToName(section);

                    BOBUIest::addRow("%s%s%sWith%sKeyboardModifier",
                                  up ? "up" : "down",
                                  stepModifierName.latin1(),
                                  sectionName.latin1(),
                                  modifierName.latin1())
                            << startDate
                            << static_cast<int>(stepModifier)
                            << section
                            << keys
                            << expectedDate;
                }
            }
        }
    }
}

void tst_QDateTimeEdit::stepModifierKeys()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(QDate, startDate);
    QFETCH(int, stepModifier);
    QFETCH(QDateTimeEdit::Section, section);
    QFETCH(BOBUIestEventList, keys);
    QFETCH(QDate, expectedDate);

    // This can interfere with our stuff.
    testWidget->hide();

    QDateTimeEdit edit(0);
    edit.setDate(startDate);
    edit.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&edit));
    edit.setCurrentSection(section);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> style(
                new StepModifierStyle);
    style->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    edit.setStyle(style.data());

    QCOMPARE(edit.date(), startDate);
    keys.simulate(&edit);
    QCOMPARE(edit.date(), expectedDate);
}

void tst_QDateTimeEdit::stepModifierButtons_data()
{
    BOBUIest::addColumn<QStyle::SubControl>("subControl");
    BOBUIest::addColumn<int>("stepModifier");
    BOBUIest::addColumn<BobUI::KeyboardModifiers>("modifiers");
    BOBUIest::addColumn<QDateTimeEdit::Section>("section");
    BOBUIest::addColumn<BOBUIime>("startTime");
    BOBUIest::addColumn<BOBUIime>("expectedTime");

    const auto subControls = {QStyle::SC_SpinBoxUp, QStyle::SC_SpinBoxDown};

    const auto modifierList = {BobUI::NoModifier,
                                   BobUI::ShiftModifier,
                                   BobUI::ControlModifier,
                                   BobUI::AltModifier,
                                   BobUI::MetaModifier};

    const auto validStepModifierList = {BobUI::NoModifier,
                                        BobUI::ControlModifier,
                                        BobUI::ShiftModifier};

    const auto sections = {QDateTimeEdit::SecondSection,
                           QDateTimeEdit::MinuteSection,
                           QDateTimeEdit::HourSection};

    const BOBUIime startTime(12, 36, 24);

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

                for (const auto section : sections) {

                    const auto expectedTime = stepTime(startTime, section, steps);
                    if (!expectedTime.isValid())
                        continue;

                    const auto sectionName = sectionToName(section);

                    BOBUIest::addRow("%s%s%sWith%sKeyboardModifier",
                                  up ? "up" : "down",
                                  stepModifierName.latin1(),
                                  sectionName.latin1(),
                                  modifierName.latin1())
                            << subControl
                            << static_cast<int>(stepModifier)
                            << modifiers
                            << section
                            << startTime
                            << expectedTime;
                }
            }
        }
    }
}

void tst_QDateTimeEdit::stepModifierButtons()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(QStyle::SubControl, subControl);
    QFETCH(int, stepModifier);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(QDateTimeEdit::Section, section);
    QFETCH(BOBUIime, startTime);
    QFETCH(BOBUIime, expectedTime);

    testWidget->hide();

    EditorDateEdit edit;
    edit.setDisplayFormat("HH:mm:ss");
    edit.setTime(startTime);
    edit.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&edit));
    edit.setCurrentSection(section);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> style(
                new StepModifierStyle);
    style->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    edit.setStyle(style.data());

    QStyleOptionSpinBox spinBoxStyleOption;
    edit.initStyleOption(&spinBoxStyleOption);

    const QRect buttonRect = edit.style()->subControlRect(
                QStyle::CC_SpinBox, &spinBoxStyleOption, subControl, &edit);

    QCOMPARE(edit.time(), startTime);
    BOBUIest::mouseClick(&edit, BobUI::LeftButton, modifiers, buttonRect.center());
    QCOMPARE(edit.time(), expectedTime);
}

void tst_QDateTimeEdit::stepModifierPressAndHold_data()
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

void tst_QDateTimeEdit::stepModifierPressAndHold()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QFETCH(QStyle::SubControl, subControl);
    QFETCH(int, stepModifier);
    QFETCH(BobUI::KeyboardModifiers, modifiers);
    QFETCH(int, expectedStepModifier);

    // Some west African zones (e.g. Niamey, Conakry) changed from 1 hour west
    // of GMT to GMT at the start of 1960; and spy.size() can get as high as 4,
    // causing the expectedDate below, when expectedStepModifier is -10, to land
    // in a transition gap for these zones, if we use Jan 1st; so use Jan 2nd.
    const QDate startDate(2000, 1, 2);

    testWidget->hide();

    EditorDateEdit edit;
    edit.setDate(startDate);

    QScopedPointer<StepModifierStyle, QScopedPointerDeleteLater> stepModifierStyle(
                new StepModifierStyle(new PressAndHoldStyle));
    stepModifierStyle->stepModifier = static_cast<BobUI::KeyboardModifier>(stepModifier);
    edit.setStyle(stepModifierStyle.data());

    QSignalSpy spy(&edit, &EditorDateEdit::dateChanged);

    edit.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&edit));
    edit.setCurrentSection(QDateTimeEdit::YearSection);

    QStyleOptionSpinBox spinBoxStyleOption;
    edit.initStyleOption(&spinBoxStyleOption);

    const QRect buttonRect = edit.style()->subControlRect(
                QStyle::CC_SpinBox, &spinBoxStyleOption, subControl, &edit);

    BOBUIest::mousePress(&edit, BobUI::LeftButton, modifiers, buttonRect.center());
    BOBUIRY_VERIFY(spy.size() >= 3);
    BOBUIest::mouseRelease(&edit, BobUI::LeftButton, modifiers, buttonRect.center());

    const auto value = spy.last().at(0);
    QVERIFY(value.userType() == QMetaType::QDate);
    const QDate expectedDate = startDate.addYears(spy.size() *
                                                  expectedStepModifier);
    QCOMPARE(value.toDate(), expectedDate);
}

#if BOBUI_CONFIG(timezone)
/*
    The following tests verify correct handling of the spring forward gap; which
    hour is skipped, and on which day, depends on the local time zone. We try to
    make it reasonably robust by discovering the first day of spring in a given
    year, but we won't try to handle every situation.

    If this function returns an invalid QDateTime, then the tests should be skipped.
*/
static QDateTime findSpring(int year, const BOBUIimeZone &timeZone)
{
    if (!timeZone.hasTransitions())
        return QDateTime();

    // Southern hemisphere spring is after midsummer
    const QDateTime midSummer = QDate(year, 6, 21).startOfDay(timeZone);
    const BOBUIimeZone::OffsetData transition =
        midSummer.isDaylightTime() ? timeZone.previousTransition(midSummer)
                                   : timeZone.nextTransition(midSummer);
    const QDateTime spring = transition.atUtc.toTimeZone(timeZone);
    // there might have been DST at some point, but not in the year we care about
    if (spring.date().year() != year || !spring.isDaylightTime())
        return QDateTime();

    return spring;
};

// Number of missing seconds between a day before and a day after when.
// If when is the time of a spring-forward transition, this is the width of its gap.
static int missingSecondsNear(const QDateTime &when)
{
    return 2 * 24 * 60 * 60 - when.addDays(-1).secsTo(when.addDays(1));
}
#endif

/*!
    Test that typing in a time that is invalid due to spring forward gap
    produces reasonable results.
*/
void tst_QDateTimeEdit::springForward_data()
{
#if BOBUI_CONFIG(timezone)
    BOBUIest::addColumn<QDateTime>("start");
    BOBUIest::addColumn<QAbstractSpinBox::CorrectionMode>("correctionMode");
    BOBUIest::addColumn<BOBUIime>("inputTime");
    BOBUIest::addColumn<QDateTime>("expected");

    const BOBUIimeZone timeZone = BOBUIimeZone::systemTimeZone();
    if (!timeZone.hasDaylightTime())
        QSKIP("This test needs to run in a timezone that observes DST!");

    const QDateTime springTransition = findSpring(2019, timeZone);
    if (!springTransition.isValid())
        QSKIP("Failed to obtain valid spring forward datetime for 2019!");

    const QDate springDate = springTransition.date();
    const int gapWidth = missingSecondsNear(springTransition);
    if (gapWidth <= 0)
        QSKIP("Spring forward transition did not actually skip any time!");

    const BOBUIime springGap = springTransition.time().addSecs(-gapWidth);
    const BOBUIime springGapMiddle = springTransition.time().addSecs(-gapWidth / 2);
    const QByteArray startGapTime = springGap.toString("hh:mm").toLocal8Bit();
    const QByteArray midGapTime = springGapMiddle.toString("hh:mm").toLocal8Bit();

    BOBUIest::addRow("forward to %s, correct to previous", startGapTime.data())
        << QDateTime(springDate, springGap.addSecs(-gapWidth))
        << QAbstractSpinBox::CorrectToPreviousValue
        << springGap
        << QDateTime(springDate, springGap.addSecs(-gapWidth));

    BOBUIest::addRow("back to %s, correct to previous", startGapTime.data())
        << springTransition
        << QAbstractSpinBox::CorrectToPreviousValue
        << springGap
        << springTransition;

    BOBUIest::addRow("forward to %s, correct to nearest", midGapTime.data())
        << QDateTime(springDate, springGap.addSecs(-gapWidth))
        << QAbstractSpinBox::CorrectToNearestValue
        << springGapMiddle
        << springTransition;

    BOBUIest::addRow("back to %s, correct to nearest", midGapTime.data())
        << springTransition
        << QAbstractSpinBox::CorrectToNearestValue
        << springGapMiddle
        << springTransition;

    BOBUIest::addRow("jump to %s, correct to nearest", midGapTime.data())
        << QDateTime(QDate(1980, 5, 10), springGap)
        << QAbstractSpinBox::CorrectToNearestValue
        << springGapMiddle
        << springTransition;
#else
    QSKIP("Needs timezone feature enabled");
#endif
}

void tst_QDateTimeEdit::springForward()
{
#if BOBUI_CONFIG(timezone)
    QFETCH(QDateTime, start);
    QFETCH(QAbstractSpinBox::CorrectionMode, correctionMode);
    QFETCH(BOBUIime, inputTime);
    QFETCH(QDateTime, expected);

    QDateTimeEdit edit;
    edit.setDisplayFormat(QLatin1String("dd.MM.yyyy hh:mm"));
    edit.setCorrectionMode(correctionMode);

    // we always want to start with a valid time
    QVERIFY(start.isValid());
    edit.setDateTime(start);

    edit.setSelectedSection(QDateTimeEdit::DaySection);
    const QDate date = expected.date();
    const QString day = QString::number(date.day()).rightJustified(2, u'0');
    const QString month = QString::number(date.month()).rightJustified(2, u'0');
    const QString year = QString::number(date.year());
    const QString hour = QString::number(inputTime.hour()).rightJustified(2, u'0');
    const QString minute = QString::number(inputTime.minute()).rightJustified(2, u'0');
    BOBUIest::keyClicks(&edit, day);
    BOBUIest::keyClicks(&edit, month);
    BOBUIest::keyClicks(&edit, year);
    BOBUIest::keyClicks(&edit, hour);
    BOBUIest::keyClicks(&edit, minute);
    BOBUIest::keyClick(&edit, BobUI::Key_Return, {});

    QCOMPARE(edit.dateTime(), expected);
#endif
}

/*!
    Test that using the up/down spinners to modify a valid time into a time that
    is invalid due to daylight-saving changes produces reasonable results.

    2007 is a year in which the DST transition in most tested places was not on the
    last or first day of the month, which allows us to test the various steps.
*/
void tst_QDateTimeEdit::stepIntoDSTGap_data()
{
#if BOBUI_CONFIG(timezone)
    BOBUIest::addColumn<QDateTime>("start");
    BOBUIest::addColumn<QDateTimeEdit::Section>("section");
    BOBUIest::addColumn<int>("steps");
    BOBUIest::addColumn<QDateTime>("end");

    const BOBUIimeZone timeZone = BOBUIimeZone::systemTimeZone();
    if (!timeZone.hasDaylightTime())
        QSKIP("This test needs to run in a timezone that observes DST!");

    const QDateTime springTransition = findSpring(2007, timeZone);
    if (!springTransition.isValid())
        QSKIP("Failed to obtain valid spring forward datetime for 2007!");

    const QDate spring = springTransition.date();
    const int gapWidth = missingSecondsNear(springTransition);
    if (gapWidth <= 0)
        QSKIP("Spring forward transition did not actually skip any time!");

    const BOBUIime springGap = springTransition.time().addSecs(-gapWidth);
    const QByteArray springTime = springGap.toString("hh:mm").toLocal8Bit();

    // change hour (can't change day):
    if (springGap.hour() != 0) {
        BOBUIest::addRow("hour up into %s gap", springTime.data())
            << QDateTime(spring, springGap.addSecs(-3600))
            << QDateTimeEdit::HourSection
            << +1
            << springTransition;

        // 3:00:10 into 2:00:10 should get us to 1:00:10
        BOBUIest::addRow("hour down into %s gap", springTime.data())
            << QDateTime(spring, springGap.addSecs(gapWidth + 10))
            << QDateTimeEdit::HourSection
            << -1
            << QDateTime(spring, springGap.addSecs(-3590));
    }

    // change day
    if (spring.day() != 1) {
        // today's 2:05 is tomorrow's 3:05
        BOBUIest::addRow("day up into %s gap", springTime.data())
            << QDateTime(spring.addDays(-1), springGap.addSecs(300))
            << QDateTimeEdit::DaySection
            << +1
            << springTransition.addSecs(300);
    }

    if (spring.day() != spring.daysInMonth()) {
        BOBUIest::addRow("day down into %s gap", springTime.data())
            << QDateTime(spring.addDays(1), springGap)
            << QDateTimeEdit::DaySection
            << -1
            << springTransition;
    }

    // change month
    // Previous month may well be February, so lack the day-of-month that
    // matches spring (e.g. Asia/Jerusalem, March 30).
    if (QDate prior = spring.addMonths(-1); prior.day() == spring.day()) {
        BOBUIest::addRow("month up into %s gap", springTime.data())
            << QDateTime(prior, springGap) << QDateTimeEdit::MonthSection << +1 << springTransition;
    }
    // America/{Jujuy,Cordoba,Catamarca} did a 2007 Dec 30th 00:00 spring
    // forward; and QDTE month steps won't change the year.
    if (QDate prior = spring.addMonths(1);
        prior.year() == spring.year() && prior.day() == spring.day()) {
        BOBUIest::addRow("month down into %s gap", springTime.data())
            << QDateTime(prior, springGap) << QDateTimeEdit::MonthSection << -1 << springTransition;
    }

    // change year
    // Some zones (e.g. Asia/Baghdad) do transitions on a fixed date; for these,
    // the springGap moment is invalid every year, so skip this test.
    if (QDateTime prior = QDateTime(spring.addYears(-1), springGap); prior.isValid()) {
        BOBUIest::addRow("year up into %s gap", springTime.data())
            << prior << QDateTimeEdit::YearSection << +1 << springTransition;
    }
    if (QDateTime later(spring.addYears(1), springGap); later.isValid()) {
        BOBUIest::addRow("year down into %s gap", springTime.data())
            << later << QDateTimeEdit::YearSection << -1 << springTransition;
    }
#else
    QSKIP("Needs timezone feature enabled");
#endif
}

void tst_QDateTimeEdit::stepIntoDSTGap()
{
#if BOBUI_CONFIG(timezone)
    QFETCH(QDateTime, start);
    QFETCH(QDateTimeEdit::Section, section);
    QFETCH(int, steps);
    QFETCH(QDateTime, end);

    QDateTimeEdit edit;
    edit.setDisplayFormat(QLatin1String("dd.MM.yyyy hh:mm"));

    // we always want to start with a valid time
    QVERIFY(start.isValid());
    edit.setDateTime(start);

    edit.setSelectedSection(section);

    // we want to end with a valid value
    QVERIFY(end.isValid());

    const auto stepCount = qAbs(steps);
    for (int step = 0; step < stepCount; ++step)
        BOBUIest::keyClick(&edit, steps > 0 ? BobUI::Key_Up : BobUI::Key_Down, {});

    QCOMPARE(edit.dateTime(), end);
#endif
}

BOBUIEST_MAIN(tst_QDateTimeEdit)
#include "tst_qdatetimeedit.moc"
