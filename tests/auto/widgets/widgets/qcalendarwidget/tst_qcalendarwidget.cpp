// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include <qcalendarwidget.h>
#include <bobuioolbutton.h>
#include <qspinbox.h>
#include <qmenu.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <qscopeguard.h>
#include <bobuiextformat.h>

class tst_QCalendarWidget : public QObject
{
    Q_OBJECT

private slots:
    void getSetCheck();
    void buttonClickCheck();

    void setTextFormat();
    void resetTextFormat();

    void setWeekdayFormat();
    void showPrevNext_data();
    void showPrevNext();

    void firstDayOfWeek();

    void contentsMargins();
};

// Testing get/set functions
void tst_QCalendarWidget::getSetCheck()
{
    QWidget topLevel;
    QCalendarWidget object(&topLevel);

    //horizontal header formats
    object.setHorizontalHeaderFormat(QCalendarWidget::NoHorizontalHeader);
    QCOMPARE(QCalendarWidget::NoHorizontalHeader, object.horizontalHeaderFormat());
    object.setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    QCOMPARE(QCalendarWidget::SingleLetterDayNames, object.horizontalHeaderFormat());
    object.setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
    QCOMPARE(QCalendarWidget::ShortDayNames, object.horizontalHeaderFormat());
    object.setHorizontalHeaderFormat(QCalendarWidget::LongDayNames);
    QCOMPARE(QCalendarWidget::LongDayNames, object.horizontalHeaderFormat());
    //vertical header formats
    object.setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
    QCOMPARE(QCalendarWidget::ISOWeekNumbers, object.verticalHeaderFormat());
    object.setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    QCOMPARE(QCalendarWidget::NoVerticalHeader, object.verticalHeaderFormat());
    //maximum Date
    QDate maxDate(2006, 7, 3);
    object.setMaximumDate(maxDate);
    QCOMPARE(maxDate, object.maximumDate());
    //minimum date
    QDate minDate(2004, 7, 3);
    object.setMinimumDate(minDate);
    QCOMPARE(minDate, object.minimumDate());
    //day of week
    object.setFirstDayOfWeek(BobUI::Thursday);
    QCOMPARE(BobUI::Thursday, object.firstDayOfWeek());
    //grid visible
    object.setGridVisible(true);
    QVERIFY(object.isGridVisible());
    object.setGridVisible(false);
    QVERIFY(!object.isGridVisible());
    //header visible
    object.setNavigationBarVisible(true);
    QVERIFY(object.isNavigationBarVisible());
    object.setNavigationBarVisible(false);
    QVERIFY(!object.isNavigationBarVisible());
    //selection mode
    QCOMPARE(QCalendarWidget::SingleSelection, object.selectionMode());
    object.setSelectionMode(QCalendarWidget::NoSelection);
    QCOMPARE(QCalendarWidget::NoSelection, object.selectionMode());
    object.setSelectionMode(QCalendarWidget::SingleSelection);
    QCOMPARE(QCalendarWidget::SingleSelection, object.selectionMode());
   //selected date
    QDate selectedDate(2005, 7, 3);
    QSignalSpy spy(&object, SIGNAL(selectionChanged()));
    object.setSelectedDate(selectedDate);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(selectedDate, object.selectedDate());
    //month and year
    object.setCurrentPage(2004, 1);
    QCOMPARE(1, object.monthShown());
    QCOMPARE(2004, object.yearShown());
    object.showNextMonth();
    QCOMPARE(2, object.monthShown());
    object.showPreviousMonth();
    QCOMPARE(1, object.monthShown());
    object.showNextYear();
    QCOMPARE(2005, object.yearShown());
    object.showPreviousYear();
    QCOMPARE(2004, object.yearShown());
    //date range
    minDate = QDate(2006,1,1);
    maxDate = QDate(2010,12,31);
    object.setDateRange(minDate, maxDate);
    QCOMPARE(maxDate, object.maximumDate());
    QCOMPARE(minDate, object.minimumDate());

    //date should not go beyond the minimum.
    selectedDate = minDate.addDays(-10);
    object.setSelectedDate(selectedDate);
    QCOMPARE(minDate, object.selectedDate());
    QVERIFY(selectedDate != object.selectedDate());
    object.clearMinimumDate();
    object.setSelectedDate(selectedDate);
    QCOMPARE(selectedDate, object.selectedDate());

    //date should not go beyond the maximum.
    selectedDate = maxDate.addDays(10);
    object.setSelectedDate(selectedDate);
    QCOMPARE(maxDate, object.selectedDate());
    QVERIFY(selectedDate != object.selectedDate());
    object.clearMaximumDate();
    object.setSelectedDate(selectedDate);
    QCOMPARE(selectedDate, object.selectedDate());

    //show today
    QDate today = QDate::currentDate();
    object.showToday();
    QCOMPARE(today.month(), object.monthShown());
    QCOMPARE(today.year(), object.yearShown());
    //slect a different date and move.
    object.setSelectedDate(minDate);
    object.showSelectedDate();
    QCOMPARE(minDate.month(), object.monthShown());
    QCOMPARE(minDate.year(), object.yearShown());
}

void tst_QCalendarWidget::buttonClickCheck()
{
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), BobUI::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    QCalendarWidget object;
    QSize size = object.sizeHint();
    object.setGeometry(0,0,size.width(), size.height());
    object.show();
    object.activateWindow();
    QVERIFY(BOBUIest::qWaitForWindowActive(&object));

    QDate selectedDate(2005, 1, 1);
    //click on the month buttons
    int month = object.monthShown();
    BOBUIoolButton *button = object.findChild<BOBUIoolButton *>("bobui_calendar_prevmonth");
    BOBUIest::mouseClick(button, BobUI::LeftButton);
    QCOMPARE(month > 1 ? month-1 : 12, object.monthShown());
    button = object.findChild<BOBUIoolButton *>("bobui_calendar_nextmonth");
    BOBUIest::mouseClick(button, BobUI::LeftButton);
    QCOMPARE(month, object.monthShown());

    button = object.findChild<BOBUIoolButton *>("bobui_calendar_yearbutton");
    BOBUIest::mouseClick(button, BobUI::LeftButton, BobUI::NoModifier, button->rect().center(), 2);
    QVERIFY(!button->isVisible());
    QSpinBox *spinbox = object.findChild<QSpinBox *>("bobui_calendar_yearedit");
    BOBUIest::keyClick(spinbox, '2');
    BOBUIest::keyClick(spinbox, '0');
    BOBUIest::keyClick(spinbox, '0');
    BOBUIest::keyClick(spinbox, '6');
    QWidget *widget = object.findChild<QWidget *>("bobui_calendar_calendarview");
    BOBUIest::mouseMove(widget);
    BOBUIest::mouseClick(widget, BobUI::LeftButton);
    QCOMPARE(2006, object.yearShown());
    BOBUIest::mouseClick(button, BobUI::LeftButton, BobUI::NoModifier, button->rect().center(), 2);
    BOBUIest::mouseMove(widget);
    BOBUIest::mouseClick(widget, BobUI::LeftButton);
    QCOMPARE(button->text(), "2006"); // Check that it is shown as a year should be
    object.setSelectedDate(selectedDate);
    object.showSelectedDate();
    BOBUIest::keyClick(widget, BobUI::Key_Down);
    QVERIFY(selectedDate != object.selectedDate());

    object.setDateRange(QDate(2006,1,1), QDate(2006,2,28));
    object.setSelectedDate(QDate(2006,1,1));
    object.showSelectedDate();
    button = object.findChild<BOBUIoolButton *>("bobui_calendar_prevmonth");
    BOBUIest::mouseClick(button, BobUI::LeftButton);
    QCOMPARE(1, object.monthShown());

    button = object.findChild<BOBUIoolButton *>("bobui_calendar_nextmonth");
    BOBUIest::mouseClick(button, BobUI::LeftButton);
    QCOMPARE(2, object.monthShown());
    BOBUIest::mouseClick(button, BobUI::LeftButton);
    QCOMPARE(2, object.monthShown());

}

void tst_QCalendarWidget::setTextFormat()
{
    QCalendarWidget calendar;
    BOBUIextCharFormat format;
    format.setFontItalic(true);
    format.setForeground(BobUI::green);

    const QDate date(1984, 10, 20);
    calendar.setDateTextFormat(date, format);
    QCOMPARE(calendar.dateTextFormat(date), format);
}

void tst_QCalendarWidget::resetTextFormat()
{
    QCalendarWidget calendar;
    BOBUIextCharFormat format;
    format.setFontItalic(true);
    format.setForeground(BobUI::green);

    const QDate date(1984, 10, 20);
    calendar.setDateTextFormat(date, format);

    calendar.setDateTextFormat(QDate(), BOBUIextCharFormat());
    QCOMPARE(calendar.dateTextFormat(date), BOBUIextCharFormat());
}

void tst_QCalendarWidget::setWeekdayFormat()
{
    QCalendarWidget calendar;

    BOBUIextCharFormat format;
    format.setFontItalic(true);
    format.setForeground(BobUI::green);

    calendar.setWeekdayTextFormat(BobUI::Wednesday, format);

    // check the format of the a given month
    for (int i = 1; i <= 31; ++i) {
        const QDate date(1984, 10, i);
        const BobUI::DayOfWeek dayOfWeek = static_cast<BobUI::DayOfWeek>(date.dayOfWeek());
        if (dayOfWeek == BobUI::Wednesday)
            QCOMPARE(calendar.weekdayTextFormat(dayOfWeek), format);
        else
            QVERIFY(calendar.weekdayTextFormat(dayOfWeek) != format);
    }
}

typedef void (QCalendarWidget::*ShowFunc)();
Q_DECLARE_METATYPE(ShowFunc)

void tst_QCalendarWidget::showPrevNext_data()
{
    BOBUIest::addColumn<ShowFunc>("function");
    BOBUIest::addColumn<QDate>("dateOrigin");
    BOBUIest::addColumn<QDate>("expectedDate");

    BOBUIest::newRow("showNextMonth") << &QCalendarWidget::showNextMonth << QDate(1984,7,30) << QDate(1984,8,30);
    BOBUIest::newRow("showPrevMonth") << &QCalendarWidget::showPreviousMonth << QDate(1984,7,30) << QDate(1984,6,30);
    BOBUIest::newRow("showNextYear") << &QCalendarWidget::showNextYear << QDate(1984,7,30) << QDate(1985,7,30);
    BOBUIest::newRow("showPrevYear") << &QCalendarWidget::showPreviousYear << QDate(1984,7,30) << QDate(1983,7,30);

    BOBUIest::newRow("showNextMonth limit") << &QCalendarWidget::showNextMonth << QDate(2007,12,4) << QDate(2008,1,4);
    BOBUIest::newRow("showPreviousMonth limit") << &QCalendarWidget::showPreviousMonth << QDate(2006,1,23) << QDate(2005,12,23);

    BOBUIest::newRow("showNextMonth now") << &QCalendarWidget::showNextMonth << QDate() << QDate::currentDate().addMonths(1);
    BOBUIest::newRow("showNextYear now") << &QCalendarWidget::showNextYear << QDate() << QDate::currentDate().addYears(1);
    BOBUIest::newRow("showPrevieousMonth now") << &QCalendarWidget::showPreviousMonth << QDate() << QDate::currentDate().addMonths(-1);
    BOBUIest::newRow("showPreviousYear now") << &QCalendarWidget::showPreviousYear << QDate() << QDate::currentDate().addYears(-1);

    BOBUIest::newRow("showToday now") << &QCalendarWidget::showToday << QDate(2000,1,31) << QDate::currentDate();
    BOBUIest::newRow("showNextMonth 31") << &QCalendarWidget::showNextMonth << QDate(2000,1,31) << QDate(2000,2,28);
    BOBUIest::newRow("selectedDate") << &QCalendarWidget::showSelectedDate << QDate(2008,2,29) << QDate(2008,2,29);

}

void tst_QCalendarWidget::showPrevNext()
{
    QFETCH(ShowFunc, function);
    QFETCH(QDate, dateOrigin);
    QFETCH(QDate, expectedDate);
#ifdef Q_OS_ANDROID
    QSKIP("Crashes sometimes on Android emulator, figure out why (BOBUIBUG-102258)");
#endif

    QCalendarWidget calWidget;
    calWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&calWidget));
    if(!dateOrigin.isNull()) {
        calWidget.setSelectedDate(dateOrigin);
        calWidget.setCurrentPage(dateOrigin.year(), dateOrigin.month());

        QCOMPARE(calWidget.yearShown(), dateOrigin.year());
        QCOMPARE(calWidget.monthShown(), dateOrigin.month());
    } else {
        QCOMPARE(calWidget.yearShown(), QDate::currentDate().year());
        QCOMPARE(calWidget.monthShown(), QDate::currentDate().month());
    }

    (calWidget.*function)();

    QCOMPARE(calWidget.yearShown(), expectedDate.year());
    QCOMPARE(calWidget.monthShown(), expectedDate.month());

    // BOBUIBUG-4058
    BOBUIoolButton *button = calWidget.findChild<BOBUIoolButton *>("bobui_calendar_prevmonth");
    BOBUIest::mouseClick(button, BobUI::LeftButton);
    expectedDate = expectedDate.addMonths(-1);
    QCOMPARE(calWidget.yearShown(), expectedDate.year());
    QCOMPARE(calWidget.monthShown(), expectedDate.month());

    if(!dateOrigin.isNull()) {
        //the selectedDate should not have changed
        QCOMPARE(calWidget.selectedDate(), dateOrigin);
    }
}

void tst_QCalendarWidget::firstDayOfWeek()
{
    // Ensure the default locale is chosen.
    QWidget parent;
    QCalendarWidget calendar;
    QLocale locale;
    QCOMPARE(calendar.firstDayOfWeek(), locale.firstDayOfWeek());

    const auto restoreDefault = qScopeGuard([locale]() {
        QLocale::setDefault(locale);
    });
    QLocale germanLocale(QLocale::German);
    QLocale::setDefault(germanLocale);
    QCalendarWidget germanLocaleCalendar;
    QCOMPARE(germanLocaleCalendar.firstDayOfWeek(), germanLocale.firstDayOfWeek());

    // Ensure calling setLocale works as well.
    QLocale frenchLocale(QLocale::French);
    calendar.setLocale(frenchLocale);
    QCOMPARE(calendar.firstDayOfWeek(), frenchLocale.firstDayOfWeek());

    // Ensure that widget-specific locale takes precedence over default.
    QLocale::setDefault(QLocale::English);
    QCOMPARE(calendar.firstDayOfWeek(), frenchLocale.firstDayOfWeek());

    // Ensure that setting the locale of parent widget has an effect.
    calendar.setParent(&parent);
    QLocale hausaLocale(QLocale::Hausa);
    parent.setLocale(hausaLocale);
    QCOMPARE(calendar.firstDayOfWeek(), hausaLocale.firstDayOfWeek());

    // Ensure that widget-specific locale takes precedence over parent.
    calendar.setLocale(germanLocale);
    // Sanity check...
    QCOMPARE(calendar.locale(), germanLocale);
    QCOMPARE(parent.locale(), hausaLocale);
    QCOMPARE(calendar.firstDayOfWeek(), germanLocale.firstDayOfWeek());
}

void tst_QCalendarWidget::contentsMargins()
{
    QCalendarWidget calendar1;
    QCalendarWidget calendar2;
    calendar2.setContentsMargins(10, 5, 20, 30);
    QCOMPARE(calendar1.minimumSizeHint() + QSize(30, 35), calendar2.minimumSizeHint());
}

BOBUIEST_MAIN(tst_QCalendarWidget)
#include "tst_qcalendarwidget.moc"
