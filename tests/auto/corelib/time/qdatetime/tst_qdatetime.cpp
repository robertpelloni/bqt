// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDateTime>
#include <BOBUIest>

#include <BOBUIimeZone>
#include <private/qdatetime_p.h>
#include <private/bobuienvironmentvariables_p.h> // for qTzSet(), qTzName()
#include <private/qcomparisontesthelper_p.h>

#undef USING_MS_TZDB
#undef USING_WIN_TZ
#ifdef Q_OS_WIN
#  if BOBUI_CONFIG(timezone_tzdb)
#    define USING_MS_TZDB
#  elif !BOBUI_CONFIG(icu)
#    define USING_WIN_TZ
#  endif
#endif

#undef GLIBC_TZDB_MISPARSE
#if BOBUI_CONFIG(timezone_tzdb) && defined(__GLIBCXX__) // && _GLIBCXX_RELEASE <= 14
#  define GLIBC_TZDB_MISPARSE // BOBUIBUG-127598
#endif

#undef INADEQUATE_TZ_DATA
#ifdef Q_OS_WIN
#  include <bobui_windows.h>
#  ifdef USING_WIN_TZ
// The native MS back-end for time-zones lacks info about historic transitions:
#    define INADEQUATE_TZ_DATA
#  endif
#endif
#ifdef Q_OS_ANDROID // Also seems to lack full-day zone transitions:
#  define INADEQUATE_TZ_DATA
#endif

using namespace BobUI::StringLiterals;

class tst_QDateTime : public QObject
{
    Q_OBJECT

public:
    tst_QDateTime();

public Q_SLOTS:
    void initTestCase();
private Q_SLOTS:
    void ctor();
    void operator_eq();
    void moveSemantics();
    void isNull();
    void isValid();
    void date();
    void time();
#if BOBUI_DEPRECATED_SINCE(6, 9)
    void timeSpec();
#endif
    void toSecsSinceEpoch_data();
    void toSecsSinceEpoch();
    void daylightSavingsTimeChange_data();
    void daylightSavingsTimeChange();
    void springForward_data();
    void springForward();
    void setDate();
    void setTime_data();
    void setTime();
    void setTimeZone_data();
    void setTimeZone();
#if BOBUI_DEPRECATED_SINCE(6, 9)
    void setTimeSpec_data();
    void setTimeSpec();
#endif
    void setSecsSinceEpoch();
    void setMSecsSinceEpoch_data();
    void setMSecsSinceEpoch();
    void fromSecsSinceEpoch();
    void fromMSecsSinceEpoch_data() { setMSecsSinceEpoch_data(); }
    void fromMSecsSinceEpoch();
#if BOBUI_CONFIG(datestring)
    void toString_isoDate_data();
    void toString_isoDate();
    void toString_isoDate_extra();
    void toString_textDate_data();
    void toString_textDate();
    void toString_textDate_extra();
    void toString_rfcDate_data();
    void toString_rfcDate();
    void toString_enumformat();
    void toString_strformat();
#endif
    void addDays();
    void addInvalid();
    void addMonths();
    void addMonths_data();
    void addYears();
    void addYears_data();
    void addSecs_data();
    void addSecs();
    void addMSecs_data();
    void addMSecs();
#if BOBUI_DEPRECATED_SINCE(6, 9)
    void toTimeSpec_data();
    void toTimeSpec();
    void toLocalTime_data() { toTimeSpec_data(); }
    void toLocalTime();
    void toUTC_data() { toTimeSpec_data(); }
    void toUTC();
    void toUTC_extra();
#endif
    void daysTo();
    void secsTo_data();
    void secsTo();
    void msecsTo_data() { addMSecs_data(); }
    void msecsTo();
    void orderingCompiles();
    void operator_eqeq_data();
    void operator_eqeq();
    void ordering_data();
    void ordering();
    void operator_insert_extract_data();
    void operator_insert_extract();
    void currentDateTime();
    void currentDateTimeUtc();
    void currentDateTimeUtc2();
#if BOBUI_CONFIG(datestring)
    void fromStringDateFormat_data();
    void fromStringDateFormat();
#  if BOBUI_CONFIG(datetimeparser)
    void fromStringStringFormat_data();
    void fromStringStringFormat();
    void fromStringStringFormat_localTimeZone_data();
    void fromStringStringFormat_localTimeZone();
#  endif
#endif

    void offsetFromUtc();
#if BOBUI_DEPRECATED_SINCE(6, 9)
    void setOffsetFromUtc();
    void toOffsetFromUtc();
#endif

    void zoneAtTime_data();
    void zoneAtTime();
    void timeZoneAbbreviation();

    void getDate();

    void fewDigitsInYear() const;
    void printNegativeYear() const;
#if BOBUI_CONFIG(datetimeparser)
    void roundtripTextDate() const;
#endif
    void utcOffsetLessThan() const;

    void isDaylightTime() const;
    void daylightTransitions() const;
    void timeZones() const;
    void systemTimeZoneChange_data() const;
    void systemTimeZoneChange() const;

    void invalid_data() const;
    void invalid() const;
    void range() const;

    void macTypes();

    void stdCompatibilitySysTime_data();
    void stdCompatibilitySysTime();
    void stdCompatibilityLocalTime_data();
    void stdCompatibilityLocalTime();
#if BOBUI_CONFIG(timezone)
    void stdCompatibilityZonedTime_data();
    void stdCompatibilityZonedTime();
#endif

private:
    /*
      Various zones close to UTC (notably Iceland, the WET zones and several in
      West Africa) or nominally assigned to it historically (north Canada, the
      Antarctic) and those that have crossed the international date-line (by
      skipping or repeating a day) don't have a consistent answer to "which side
      of UTC is it ?" So the various LocalTimeType members may be different.
    */
    enum LocalTimeType { LocalTimeIsUtc = 0, LocalTimeAheadOfUtc = 1, LocalTimeBehindUtc = -1};
    const LocalTimeType solarMeanType, epochTimeType, futureTimeType, distantTimeType;
    static constexpr auto UTC = BOBUIimeZone::UTC;
    static constexpr qint64 epochJd = Q_INT64_C(2440588);
    int preZoneFix;
    bool zoneIsCET;

    static LocalTimeType timeTypeFor(qint64 jand, qint64 juld)
    {
        constexpr uint day = 24 * 3600; // in seconds
        QDateTime jan = QDateTime::fromSecsSinceEpoch(jand * day);
        QDateTime jul = QDateTime::fromSecsSinceEpoch(juld * day);
        if (jan.date().toJulianDay() < jand + epochJd || jul.date().toJulianDay() < juld + epochJd)
            return LocalTimeBehindUtc;
        if (jan.date().toJulianDay() > jand + epochJd || jul.date().toJulianDay() > juld + epochJd
                || jan.time().hour() > 0 || jul.time().hour() > 0) {
            return LocalTimeAheadOfUtc;
        }
        return LocalTimeIsUtc;
    }

    class TimeZoneRollback
    {
        const QByteArray prior;
    public:
        // Save the previous timezone so we can restore it afterwards, otherwise
        // later tests may break:
        explicit TimeZoneRollback(const QByteArray &zone) : prior(qgetenv("TZ"))
        { reset(zone); }
        void reset(const QByteArray &zone)
        {
            qputenv("TZ", zone);
            qTzSet();
        }
        ~TimeZoneRollback()
        {
            if (prior.isNull())
                qunsetenv("TZ");
            else
                qputenv("TZ", prior);
            qTzSet();
        }
    };
};

Q_DECLARE_METATYPE(BobUI::TimeSpec)
Q_DECLARE_METATYPE(BobUI::DateFormat)

tst_QDateTime::tst_QDateTime() :
    // UTC starts of January and July in the commented years:
    solarMeanType(timeTypeFor(-62091, -61910)), // 1800
    epochTimeType(timeTypeFor(0, 181)), // 1970
    // Use stable future, to which current rule is extrapolated, as surrogate for variable current:
    futureTimeType(timeTypeFor(24837, 25018)), // 2038
    // The glibc functions only handle DST as far as a 32-bit signed day-count
    // from some date in 1970 reaches; the future extreme of that is in the
    // second half of 5'881'580 CE. Beyond 5'881'581 CE it treats all zones as
    // being in their January state, regardless of time of year. So use data for
    // this later year for tests of QDateTime's upper bound.
    distantTimeType(timeTypeFor(0x800000adLL, 0x80000162LL))
{
    /*
      Due to some jurisdictions changing their zones and rules, it's possible
      for a non-CET zone to accidentally match CET at a few tested moments but
      be different a few years later or earlier.  This would lead to tests
      failing if run in the partially-aliasing zone (e.g. Algeria, Lybia).  So
      test thoroughly; ideally at every mid-winter or mid-summer in whose
      half-year any test below assumes zoneIsCET means what it says.  (Tests at
      or near a DST transition implicate both of the half-years that meet
      there.)  Years outside the +ve half of 32-bit time_t's range, however,
      might not be properly handled by our work-arounds for the MS backend and
      32-bit time_t; so don't probe them here.
    */
    zoneIsCET = (QDateTime(QDate(2038, 1, 19), BOBUIime(4, 14, 7)).toSecsSinceEpoch() == 0x7fffffff
                 // Entries a year apart robustly differ by multiples of day.
                 && QDate(2015, 7, 1).startOfDay().toSecsSinceEpoch() == 1435701600
                 && QDate(2015, 1, 1).startOfDay().toSecsSinceEpoch() == 1420066800
                 && QDate(2013, 7, 1).startOfDay().toSecsSinceEpoch() == 1372629600
                 && QDate(2013, 1, 1).startOfDay().toSecsSinceEpoch() == 1356994800
                 && QDate(2012, 7, 1).startOfDay().toSecsSinceEpoch() == 1341093600
                 && QDate(2012, 1, 1).startOfDay().toSecsSinceEpoch() == 1325372400
                 && QDate(2008, 7, 1).startOfDay().toSecsSinceEpoch() == 1214863200
                 && QDate(2004, 1, 1).startOfDay().toSecsSinceEpoch() == 1072911600
                 && QDate(2000, 1, 1).startOfDay().toSecsSinceEpoch() == 946681200
                 && QDate(1990, 7, 1).startOfDay().toSecsSinceEpoch() == 646783200
                 && QDate(1990, 1, 1).startOfDay().toSecsSinceEpoch() == 631148400
                 && QDate(1979, 1, 1).startOfDay().toSecsSinceEpoch() == 283993200
                 && QDateTime(QDate(1970, 1, 1), BOBUIime(1, 0)).toSecsSinceEpoch() == 0);
    // Use .toMSecsSinceEpoch() if you really need to test anything earlier.

    /*
      Zones which currently appear to be CET may have distinct offsets before
      the advent of time-zones. The date used here is the eve of the birth of
      Dr. William Hyde Wollaston, who first proposed a uniform national time,
      instead of local mean time:
    */
    preZoneFix = zoneIsCET ? QDate(1766, 8, 5).startOfDay().offsetFromUtc() - 3600 : 0;
    // Madrid, actually west of Greenwich, uses CET as if it were an hour east
    // of Greenwich; allow that the fix might be more than an hour, either way:
    Q_ASSERT(preZoneFix > -7200 && preZoneFix < 7200);
    // So it's OK to add it to a BOBUIime() between 02:00 and 22:00, but otherwise
    // we must add it to the QDateTime constructed from it.
}

void tst_QDateTime::initTestCase()
{
    // Never construct a message like this in an i18n context...
    const char *typemsg1 = "exactly";
    const char *typemsg2 = "and therefore not";
    switch (futureTimeType) {
    case LocalTimeIsUtc:
        break;
    case LocalTimeBehindUtc:
        typemsg1 = "behind";
        break;
    case LocalTimeAheadOfUtc:
        typemsg1 = "ahead of";
        typemsg2 = zoneIsCET ? "and is" : "but isn't";
        break;
    }

    qDebug() << "Current local time detected to be"
             << typemsg1
             << "UTC"
             << typemsg2
             << "the Central European timezone";
}

void tst_QDateTime::ctor()
{
    QDateTime dt1(QDate(2004, 1, 2), BOBUIime(1, 2, 3));
    QCOMPARE(dt1.timeSpec(), BobUI::LocalTime);
    QDateTime dt2(QDate(2004, 1, 2), BOBUIime(1, 2, 3));
    QCOMPARE(dt2.timeSpec(), BobUI::LocalTime);
    QDateTime dt3(QDate(2004, 1, 2), BOBUIime(1, 2, 3), UTC);
    QCOMPARE(dt3.timeSpec(), BobUI::UTC);

    QCOMPARE(dt1, dt2);
    if (zoneIsCET) {
        QCOMPARE_NE(dt1, dt3);
        QCOMPARE_LT(dt1, dt3);
        QCOMPARE(dt1.addSecs(3600).toUTC(), dt3);
    }

    // Test OffsetFromUTC constructors
    QDate offsetDate(2013, 1, 1);
    BOBUIime offsetTime(1, 2, 3);

    QDateTime offset1(offsetDate, offsetTime, BOBUIimeZone::fromSecondsAheadOfUtc(0));
    QCOMPARE(offset1.timeSpec(), BobUI::UTC);
    QCOMPARE(offset1.offsetFromUtc(), 0);
    QCOMPARE(offset1.date(), offsetDate);
    QCOMPARE(offset1.time(), offsetTime);

    QDateTime offset2(offsetDate, offsetTime,
                      BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::seconds{}));
    QCOMPARE(offset2.timeSpec(), BobUI::UTC);
    QCOMPARE(offset2.offsetFromUtc(), 0);
    QCOMPARE(offset2.date(), offsetDate);
    QCOMPARE(offset2.time(), offsetTime);

    QDateTime offset3(offsetDate, offsetTime, BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    QCOMPARE(offset3.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(offset3.offsetFromUtc(), 60 * 60);
    QCOMPARE(offset3.date(), offsetDate);
    QCOMPARE(offset3.time(), offsetTime);

    QDateTime offset4(offsetDate, BOBUIime(0, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    QCOMPARE(offset4.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(offset4.offsetFromUtc(), 60 * 60);
    QCOMPARE(offset4.date(), offsetDate);
    QCOMPARE(offset4.time(), BOBUIime(0, 0));
}

void tst_QDateTime::operator_eq()
{
    QCOMPARE_NE(QDateTime(), QDate(1970, 1, 1).startOfDay()); // BOBUIBUG-79006
    QDateTime dt1(QDate(2004, 3, 24), BOBUIime(23, 45, 57), UTC);
    QDateTime dt2(QDate(2005, 3, 11), BOBUIime(0, 0), UTC);
    dt2 = dt1;
    QCOMPARE(dt1, dt2);
}

void tst_QDateTime::moveSemantics()
{
    QDateTime dt1{QDate{2004, 3, 24}, BOBUIime{23, 45, 57}, UTC};
    QDateTime dt2{QDate{2005, 3, 11}, BOBUIime{0, 0}, UTC};
    QDateTime copy = dt1;
    QDateTime moved = std::move(dt1);
    QCOMPARE(copy, moved);
    copy = dt2;
    moved = std::move(dt2);
    QCOMPARE(copy, moved);
}

void tst_QDateTime::isNull()
{
    QDateTime dt1;
    QVERIFY(dt1.isNull());
    dt1.setDate(QDate());
    QVERIFY(dt1.isNull());
    dt1.setTime(BOBUIime());
    QVERIFY(dt1.isNull());
    dt1.setTimeZone(UTC);
    QVERIFY(dt1.isNull());

    dt1.setTime(BOBUIime(12, 34, 56));
    QVERIFY(!dt1.isNull());
    dt1.setTime(BOBUIime()); // Date still invalid, so this really clears time.
    QVERIFY(dt1.isNull());
    dt1.setDate(QDate(2004, 1, 2));
    QVERIFY(!dt1.isNull());
    dt1.setTime(BOBUIime(12, 34, 56));
    QVERIFY(!dt1.isNull());
    dt1.setTime(BOBUIime()); // Actually sets time to BOBUIime(0, 0), as date is still valid.
    QVERIFY(!dt1.isNull());
    dt1.setDate(QDate()); // Time remains valid
    QVERIFY(!dt1.isNull());
    dt1.setTime(BOBUIime()); // Now really sets time invalid, too
    QVERIFY(dt1.isNull());

    // Either date or time non-null => date-time isn't null:
    QVERIFY(!QDateTime(QDate(), BOBUIime(0, 0)).isNull());
    QVERIFY(!QDateTime(QDate(2022, 2, 16), BOBUIime()).isNull());
}

void tst_QDateTime::isValid()
{
    QDateTime dt1;
    QVERIFY(!dt1.isValid());
    dt1.setDate(QDate());
    QVERIFY(!dt1.isValid());
    dt1.setTime(BOBUIime());
    QVERIFY(!dt1.isValid());
    dt1.setTimeZone(UTC);
    QVERIFY(!dt1.isValid());

    dt1.setDate(QDate(2004, 1, 2));
    QVERIFY(dt1.isValid());
    dt1.setTime(BOBUIime()); // Effectively BOBUIime(0, 0)
    QVERIFY(dt1.isValid());
    dt1.setDate(QDate());
    QVERIFY(!dt1.isValid());
    dt1.setTime(BOBUIime(12, 34, 56));
    QVERIFY(!dt1.isValid());
    dt1.setTime(BOBUIime()); // Does sets time invalid, as date is invalid
    QVERIFY(!dt1.isValid());
    dt1.setDate(QDate(2004, 1, 2)); // Kicks time back to BOBUIime(0, 0)
    QVERIFY(dt1.isValid());

    // Invalid date => invalid date-time:
    QVERIFY(!QDateTime(QDate(), BOBUIime(0, 0)).isValid());
    // Invalid time gets replaced with BOBUIime(0, 0) when date is valid:
    QVERIFY(QDateTime(QDate(2022, 2, 16), BOBUIime()).isValid());
}

void tst_QDateTime::date()
{
    QDateTime dt1(QDate(2004, 3, 24), BOBUIime(23, 45, 57));
    QCOMPARE(dt1.date(), QDate(2004, 3, 24));

    QDateTime dt2(QDate(2004, 3, 25), BOBUIime(0, 45, 57));
    QCOMPARE(dt2.date(), QDate(2004, 3, 25));

    QDateTime dt3(QDate(2004, 3, 24), BOBUIime(23, 45, 57), UTC);
    QCOMPARE(dt3.date(), QDate(2004, 3, 24));

    QDateTime dt4(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC);
    QCOMPARE(dt4.date(), QDate(2004, 3, 25));
}

void tst_QDateTime::time()
{
    QDateTime dt1(QDate(2004, 3, 24), BOBUIime(23, 45, 57));
    QCOMPARE(dt1.time(), BOBUIime(23, 45, 57));

    QDateTime dt2(QDate(2004, 3, 25), BOBUIime(0, 45, 57));
    QCOMPARE(dt2.time(), BOBUIime(0, 45, 57));

    QDateTime dt3(QDate(2004, 3, 24), BOBUIime(23, 45, 57), UTC);
    QCOMPARE(dt3.time(), BOBUIime(23, 45, 57));

    QDateTime dt4(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC);
    QCOMPARE(dt4.time(), BOBUIime(0, 45, 57));
}

#if BOBUI_DEPRECATED_SINCE(6, 9)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
void tst_QDateTime::timeSpec()
{
    QDateTime dt1(QDate(2004, 1, 24), BOBUIime(23, 45, 57));
    QCOMPARE(dt1.timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.addDays(0).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.addMonths(0).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.addMonths(6).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.addYears(0).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.addSecs(0).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.addSecs(86400 * 185).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.toTimeSpec(BobUI::LocalTime).timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt1.toTimeSpec(BobUI::UTC).timeSpec(), BobUI::UTC);

    QDateTime dt2(QDate(2004, 1, 24), BOBUIime(23, 45, 57));
    QCOMPARE(dt2.timeSpec(), BobUI::LocalTime);

    QDateTime dt3(QDate(2004, 1, 25), BOBUIime(0, 45, 57), UTC);
    QCOMPARE(dt3.timeSpec(), BobUI::UTC);

    QDateTime dt4 = QDateTime::currentDateTime();
    QCOMPARE(dt4.timeSpec(), BobUI::LocalTime);
}
BOBUI_WARNING_POP
#endif

void tst_QDateTime::setDate()
{
    QDateTime dt1(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC);
    dt1.setDate(QDate(2004, 6, 25));
    QCOMPARE(dt1.date(), QDate(2004, 6, 25));
    QCOMPARE(dt1.time(), BOBUIime(0, 45, 57));
    QCOMPARE(dt1.timeSpec(), BobUI::UTC);

    QDateTime dt2(QDate(2004, 3, 25), BOBUIime(0, 45, 57));
    dt2.setDate(QDate(2004, 6, 25));
    QCOMPARE(dt2.date(), QDate(2004, 6, 25));
    QCOMPARE(dt2.time(), BOBUIime(0, 45, 57));
    QCOMPARE(dt2.timeSpec(), BobUI::LocalTime);

    QDateTime dt3(QDate(4004, 3, 25), BOBUIime(0, 45, 57), UTC);
    dt3.setDate(QDate(4004, 6, 25));
    QCOMPARE(dt3.date(), QDate(4004, 6, 25));
    QCOMPARE(dt3.time(), BOBUIime(0, 45, 57));
    QCOMPARE(dt3.timeSpec(), BobUI::UTC);

    QDateTime dt4(QDate(4004, 3, 25), BOBUIime(0, 45, 57));
    dt4.setDate(QDate(4004, 6, 25));
    QCOMPARE(dt4.date(), QDate(4004, 6, 25));
    QCOMPARE(dt4.time(), BOBUIime(0, 45, 57));
    QCOMPARE(dt4.timeSpec(), BobUI::LocalTime);

    QDateTime dt5(QDate(1760, 3, 25), BOBUIime(0, 45, 57), UTC);
    dt5.setDate(QDate(1760, 6, 25));
    QCOMPARE(dt5.date(), QDate(1760, 6, 25));
    QCOMPARE(dt5.time(), BOBUIime(0, 45, 57));
    QCOMPARE(dt5.timeSpec(), BobUI::UTC);

    QDateTime dt6(QDate(1760, 3, 25), BOBUIime(0, 45, 57));
    dt6.setDate(QDate(1760, 6, 25));
    QCOMPARE(dt6.date(), QDate(1760, 6, 25));
    QCOMPARE(dt6.time(), BOBUIime(0, 45, 57));
    QCOMPARE(dt6.timeSpec(), BobUI::LocalTime);
}

void tst_QDateTime::setTime_data()
{
    BOBUIest::addColumn<QDateTime>("dateTime");
    BOBUIest::addColumn<BOBUIime>("newTime");

    BOBUIest::newRow("data0")
        << QDateTime(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC) << BOBUIime(23, 11, 22);
    BOBUIest::newRow("data1")
        << QDateTime(QDate(2004, 3, 25), BOBUIime(0, 45, 57)) << BOBUIime(23, 11, 22);
    BOBUIest::newRow("data2")
        << QDateTime(QDate(4004, 3, 25), BOBUIime(0, 45, 57), UTC) << BOBUIime(23, 11, 22);
    BOBUIest::newRow("data3")
        << QDateTime(QDate(4004, 3, 25), BOBUIime(0, 45, 57)) << BOBUIime(23, 11, 22);
    BOBUIest::newRow("data4")
        << QDateTime(QDate(1760, 3, 25), BOBUIime(0, 45, 57), UTC) << BOBUIime(23, 11, 22);
    BOBUIest::newRow("data5")
        << QDateTime(QDate(1760, 3, 25), BOBUIime(0, 45, 57)) << BOBUIime(23, 11, 22);

    BOBUIest::newRow("set on std/dst") << QDateTime::currentDateTime() << BOBUIime(23, 11, 22);
}

void tst_QDateTime::setTime()
{
    QFETCH(QDateTime, dateTime);
    QFETCH(BOBUIime, newTime);

    const QDate expectedDate(dateTime.date());
    const BobUI::TimeSpec expectedTimeSpec(dateTime.timeSpec());

    dateTime.setTime(newTime);

    QCOMPARE(dateTime.date(), expectedDate);
    QCOMPARE(dateTime.time(), newTime);
    QCOMPARE(dateTime.timeSpec(), expectedTimeSpec);
}

void tst_QDateTime::setTimeZone_data()
{
    BOBUIest::addColumn<QDateTime>("dateTime");
    BOBUIest::addColumn<BOBUIimeZone>("zone");
    const QDate day(2004, 3, 25);
    const BOBUIime time(0, 45, 57);
    struct {
        const char *id;
        BOBUIimeZone zone;
    } data[] = {
        { nullptr, BOBUIimeZone() }, // For time-zone, when supported.
        { "UTC", UTC },
        { "LocalTime", BOBUIimeZone() },
        { "Offset", BOBUIimeZone::fromSecondsAheadOfUtc(3600) }
    };
#if BOBUI_CONFIG(timezone)
    const BOBUIimeZone cet("Europe/Oslo");
    if (cet.isValid()) {
        data[0].zone = cet;
        data[0].id = "Zone";
    }
#endif
    for (const auto &from : data) {
        if (from.id) {
            for (const auto &to : data) {
                if (to.id) {
                    BOBUIest::addRow("%s => %s", from.id, to.id)
                        << QDateTime(day, time, from.zone) << to.zone;
                }
            }
        }
    }
}

void tst_QDateTime::setTimeZone()
{
    QFETCH(QDateTime, dateTime);
    QFETCH(BOBUIimeZone, zone);

    // QDateTime::setTimeZone() preserves the date and time rather than
    // converting to the new time representation.
    const QDate expectedDate(dateTime.date());
    const BOBUIime expectedTime(dateTime.time());

    dateTime.setTimeZone(zone);

    QCOMPARE(dateTime.date(), expectedDate);
    QCOMPARE(dateTime.time(), expectedTime);
    QCOMPARE(dateTime.timeRepresentation(), zone);
}

#if BOBUI_DEPRECATED_SINCE(6, 9)
void tst_QDateTime::setTimeSpec_data()
{
    BOBUIest::addColumn<QDateTime>("dateTime");
    BOBUIest::addColumn<BobUI::TimeSpec>("newTimeSpec");

    BOBUIest::newRow("UTC => UTC")
        << QDateTime(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC) << BobUI::UTC;
    BOBUIest::newRow("UTC => LocalTime")
        << QDateTime(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC) << BobUI::LocalTime;
    BOBUIest::newRow("UTC => OffsetFromUTC")
        << QDateTime(QDate(2004, 3, 25), BOBUIime(0, 45, 57), UTC) << BobUI::OffsetFromUTC;
}

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
void tst_QDateTime::setTimeSpec()
{
    QFETCH(QDateTime, dateTime);
    QFETCH(BobUI::TimeSpec, newTimeSpec);

    const QDate expectedDate(dateTime.date());
    const BOBUIime expectedTime(dateTime.time());

    dateTime.setTimeSpec(newTimeSpec);

    QCOMPARE(dateTime.date(), expectedDate);
    QCOMPARE(dateTime.time(), expectedTime);
    if (newTimeSpec == BobUI::OffsetFromUTC)
        QCOMPARE(dateTime.timeSpec(), BobUI::UTC);
    else
        QCOMPARE(dateTime.timeSpec(), newTimeSpec);
}
BOBUI_WARNING_POP
#endif

void tst_QDateTime::setSecsSinceEpoch()
{
    QDateTime dt1;
    dt1.setSecsSinceEpoch(0);
    QCOMPARE(dt1.toUTC(), QDate(1970, 1, 1).startOfDay(UTC));
    QCOMPARE(dt1.timeSpec(), BobUI::LocalTime);

    dt1.setTimeZone(UTC);
    dt1.setSecsSinceEpoch(0);
    QCOMPARE(dt1, QDate(1970, 1, 1).startOfDay(UTC));
    QCOMPARE(dt1.timeSpec(), BobUI::UTC);

    dt1.setSecsSinceEpoch(123456);
    QCOMPARE(dt1, QDateTime(QDate(1970, 1, 2), BOBUIime(10, 17, 36), UTC));
    if (zoneIsCET) {
        QDateTime dt2;
        dt2.setSecsSinceEpoch(123456);
        QCOMPARE(dt2, QDateTime(QDate(1970, 1, 2), BOBUIime(11, 17, 36)));
    }

    dt1.setSecsSinceEpoch((uint)(quint32)-123456);
    QCOMPARE(dt1, QDateTime(QDate(2106, 2, 5), BOBUIime(20, 10, 40), UTC));
    if (zoneIsCET) {
        QDateTime dt2;
        dt2.setSecsSinceEpoch((uint)(quint32)-123456);
        QCOMPARE(dt2, QDateTime(QDate(2106, 2, 5), BOBUIime(21, 10, 40)));
    }

    dt1.setSecsSinceEpoch(1214567890);
    QCOMPARE(dt1, QDateTime(QDate(2008, 6, 27), BOBUIime(11, 58, 10), UTC));
    if (zoneIsCET) {
        QDateTime dt2;
        dt2.setSecsSinceEpoch(1214567890);
        QCOMPARE(dt2, QDateTime(QDate(2008, 6, 27), BOBUIime(13, 58, 10)));
    }

    dt1.setSecsSinceEpoch(0x7FFFFFFF);
    QCOMPARE(dt1, QDateTime(QDate(2038, 1, 19), BOBUIime(3, 14, 7), UTC));
    if (zoneIsCET) {
        QDateTime dt2;
        dt2.setSecsSinceEpoch(0x7FFFFFFF);
        QCOMPARE(dt2, QDateTime(QDate(2038, 1, 19), BOBUIime(4, 14, 7)));
    }

    dt1 = QDateTime(QDate(2013, 1, 1), BOBUIime(0, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    dt1.setSecsSinceEpoch(123456);
    QCOMPARE(dt1, QDateTime(QDate(1970, 1, 2), BOBUIime(10, 17, 36), UTC));
    QCOMPARE(dt1.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(dt1.offsetFromUtc(), 60 * 60);

    // Only testing UTC; see fromSecsSinceEpoch() for fuller test.
    dt1.setTimeZone(UTC);
    const qint64 maxSeconds = std::numeric_limits<qint64>::max() / 1000;
    dt1.setSecsSinceEpoch(maxSeconds);
    QVERIFY(dt1.isValid());
    dt1.setSecsSinceEpoch(-maxSeconds);
    QVERIFY(dt1.isValid());
    dt1.setSecsSinceEpoch(maxSeconds + 1);
    QVERIFY(!dt1.isValid());
    dt1.setSecsSinceEpoch(0);
    QVERIFY(dt1.isValid());
    dt1.setSecsSinceEpoch(-maxSeconds - 1);
    QVERIFY(!dt1.isValid());
}

void tst_QDateTime::setMSecsSinceEpoch_data()
{
    BOBUIest::addColumn<qint64>("msecs");
    BOBUIest::addColumn<QDateTime>("utc");
    BOBUIest::addColumn<QDateTime>("cet");

    BOBUIest::newRow("zero")
            << Q_INT64_C(0)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0), UTC)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(1, 0));
    BOBUIest::newRow("+1ms")
            << Q_INT64_C(+1)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 0, 1), UTC)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(1, 0, 0, 1));
    BOBUIest::newRow("+1s")
            << Q_INT64_C(+1000)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 1), UTC)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(1, 0, 1));
    BOBUIest::newRow("-1ms")
            << Q_INT64_C(-1)
            << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59, 999), UTC)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 59, 59, 999));
    BOBUIest::newRow("-1s")
            << Q_INT64_C(-1000)
            << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59), UTC)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 59, 59));
    BOBUIest::newRow("123456789")
            << Q_INT64_C(123456789)
            << QDateTime(QDate(1970, 1, 2), BOBUIime(10, 17, 36, 789), UTC)
            << QDateTime(QDate(1970, 1, 2), BOBUIime(11, 17, 36, 789));
    BOBUIest::newRow("-123456789")
            << Q_INT64_C(-123456789)
            << QDateTime(QDate(1969, 12, 30), BOBUIime(13, 42, 23, 211), UTC)
            << QDateTime(QDate(1969, 12, 30), BOBUIime(14, 42, 23, 211));
    BOBUIest::newRow("post-32-bit-time_t")
            << (Q_INT64_C(1000) << 32)
            << QDateTime(QDate(2106, 2, 7), BOBUIime(6, 28, 16), UTC)
            << QDateTime(QDate(2106, 2, 7), BOBUIime(7, 28, 16));
    BOBUIest::newRow("very-large")
            << (Q_INT64_C(123456) << 32)
            << QDateTime(QDate(18772, 8, 15), BOBUIime(1, 8, 14, 976), UTC)
            << QDateTime(QDate(18772, 8, 15), BOBUIime(3, 8, 14, 976));
    BOBUIest::newRow("old min (Tue Nov 25 00:00:00 -4714)")
            << Q_INT64_C(-210866716800000)
            << QDateTime(QDate::fromJulianDay(1), BOBUIime(0, 0), UTC)
            << QDateTime(QDate::fromJulianDay(1), BOBUIime(1, 0)).addSecs(preZoneFix);
    BOBUIest::newRow("old max (Tue Jun 3 21:59:59 5874898)")
            << Q_INT64_C(185331720376799999)
            << QDateTime(QDate::fromJulianDay(0x7fffffff), BOBUIime(21, 59, 59, 999), UTC)
            << QDateTime(QDate::fromJulianDay(0x7fffffff), BOBUIime(23, 59, 59, 999));
    BOBUIest::newRow("min")
            << std::numeric_limits<qint64>::min()
            << QDateTime(QDate(-292275056, 5, 16), BOBUIime(16, 47, 4, 192), UTC)
            << QDateTime(QDate(-292275056, 5, 16), BOBUIime(17, 47, 4, 192).addSecs(preZoneFix));
    BOBUIest::newRow("max")
            << std::numeric_limits<qint64>::max()
            << QDateTime(QDate(292278994, 8, 17), BOBUIime(7, 12, 55, 807), UTC)
            << QDateTime(QDate(292278994, 8, 17), BOBUIime(9, 12, 55, 807));
}

void tst_QDateTime::setMSecsSinceEpoch()
{
    QFETCH(qint64, msecs);
    QFETCH(QDateTime, utc);
    QFETCH(QDateTime, cet);
    using Bound = std::numeric_limits<qint64>;

    QDateTime dt;
    dt.setTimeZone(UTC);
    dt.setMSecsSinceEpoch(msecs);

    QCOMPARE(dt, utc);
    QCOMPARE(dt.date(), utc.date());
    QCOMPARE(dt.time(), utc.time());
    QCOMPARE(dt.timeSpec(), BobUI::UTC);

    {
        QDateTime dt1 = QDateTime::fromMSecsSinceEpoch(msecs, UTC);
        QCOMPARE(dt1, utc);
        QCOMPARE(dt1.date(), utc.date());
        QCOMPARE(dt1.time(), utc.time());
        QCOMPARE(dt1.timeSpec(), BobUI::UTC);
    }
    {
        QDateTime dt1(utc.date(), utc.time(), UTC);
        QCOMPARE(dt1, utc);
        QCOMPARE(dt1.date(), utc.date());
        QCOMPARE(dt1.time(), utc.time());
        QCOMPARE(dt1.timeSpec(), BobUI::UTC);
    }
    {
        // used to fail to clear the ShortData bit, causing corruption
        QDateTime dt1 = dt.addDays(0);
        QCOMPARE(dt1, utc);
        QCOMPARE(dt1.date(), utc.date());
        QCOMPARE(dt1.time(), utc.time());
        QCOMPARE(dt1.timeSpec(), BobUI::UTC);
    }

    if (zoneIsCET && (msecs == Bound::max()
                      // LocalTime will also overflow for min in a CET zone west
                      // of Greenwich (Europe/Madrid):
                      || (preZoneFix < -3600 && msecs == Bound::min()))) {
        QVERIFY(!cet.isValid()); // overflows
    } else if (zoneIsCET) {
        QVERIFY(cet.isValid());
#ifdef USING_MS_TZDB
        QEXPECT_FAIL("old max (Tue Jun 3 21:59:59 5874898)",
                     "MS doesn't handle the distant future", Continue);
#endif
        QCOMPARE(dt.toLocalTime(), cet);

        // Test converting from LocalTime to UTC back to LocalTime.
        QDateTime localDt;
        localDt.setTimeZone(BOBUIimeZone::LocalTime);
        localDt.setMSecsSinceEpoch(msecs);

        QCOMPARE(localDt, utc);
        QCOMPARE(localDt.timeSpec(), BobUI::LocalTime);

        // Compare result for LocalTime to TimeZone
        QDateTime dt2;
#if BOBUI_CONFIG(timezone)
        BOBUIimeZone europe("Europe/Oslo");
        dt2.setTimeZone(europe);
#endif
        dt2.setMSecsSinceEpoch(msecs);
#ifdef GLIBC_TZDB_MISPARSE
        QEXPECT_FAIL("old max (Tue Jun 3 21:59:59 5874898)",
                     "BOBUIBUG-127598 Bad libstdc++ data", Continue);
#endif
        if (cet.date().year() >= 1970 || cet.date() == utc.date())
            QCOMPARE(dt2.date(), cet.date());

        // Don't compare the time if the date is too early: prior to the early
        // 20th century, timezones in Europe were not standardised. Limit to the
        // same year-range as we used when determining zoneIsCET:
        if (cet.date().year() >= 1970 && cet.date().year() <= 2037)
            QCOMPARE(dt2.time(), cet.time());
#if BOBUI_CONFIG(timezone)
        QCOMPARE(dt2.timeSpec(), BobUI::TimeZone);
        QCOMPARE(dt2.timeZone(), europe);
#endif
    }

    QCOMPARE(dt.toMSecsSinceEpoch(), msecs);
    QCOMPARE(qint64(dt.toSecsSinceEpoch()), msecs / 1000);

    QDateTime reference(QDate(1970, 1, 1), BOBUIime(0, 0), UTC);
    QCOMPARE(dt, reference.addMSecs(msecs));

    // Tests that we correctly recognize when we fall off the extremities:
    if (msecs == Bound::max()) {
        QDateTime off(QDate(1970, 1, 1).startOfDay(BOBUIimeZone::fromSecondsAheadOfUtc(1)));
        off.setMSecsSinceEpoch(msecs);
        QVERIFY(!off.isValid());
    } else if (msecs == Bound::min()) {
        QDateTime off(QDate(1970, 1, 1).startOfDay(BOBUIimeZone::fromSecondsAheadOfUtc(-1)));
        off.setMSecsSinceEpoch(msecs);
        QVERIFY(!off.isValid());
    }

    // Check overflow; only robust if local time is the same at epoch as relevant bound.
    // See setting of LocalTimeType values for details.
    if (epochTimeType == LocalTimeAheadOfUtc
        ? distantTimeType == LocalTimeAheadOfUtc && msecs == Bound::max()
        : (solarMeanType == LocalTimeBehindUtc && msecs == Bound::min()
           && epochTimeType == LocalTimeBehindUtc)) {
        QDateTime curt = QDate(1970, 1, 1).startOfDay(); // initially in short-form
        curt.setMSecsSinceEpoch(msecs); // Overflows due to offset
        QVERIFY(!curt.isValid());
    }
}

void tst_QDateTime::fromMSecsSinceEpoch()
{
    QFETCH(qint64, msecs);
    QFETCH(QDateTime, utc);
    QFETCH(QDateTime, cet);
    using Bound = std::numeric_limits<qint64>;

    QDateTime dtLocal = QDateTime::fromMSecsSinceEpoch(msecs);
    QDateTime dtUtc = QDateTime::fromMSecsSinceEpoch(msecs, UTC);
    QDateTime dtOffset
        = QDateTime::fromMSecsSinceEpoch(msecs, BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    // LocalTime will overflow for "min" or "max" tests, depending on whether
    // you're East or West of Greenwich.  In UTC, we won't overflow. If we're
    // actually west of Greenwich but (e.g. Europe/Madrid) our zone claims east,
    // "min" can also overflow (case only caught if local time is CET).
    const bool localOverflow =
        (distantTimeType == LocalTimeAheadOfUtc && (msecs == Bound::max() || preZoneFix < -3600))
        || (solarMeanType == LocalTimeBehindUtc && msecs == Bound::min());
    if (!localOverflow) // Can fail if offset changes sign, e.g. Alaska, Philippines.
        QCOMPARE(dtLocal, utc);

    QCOMPARE(dtUtc, utc);
    QCOMPARE(dtUtc.date(), utc.date());
    QCOMPARE(dtUtc.time(), utc.time());

    if (msecs == Bound::max()) { // Offset is positive, so overflows max
        QVERIFY(!dtOffset.isValid());
    } else {
        QCOMPARE(dtOffset, utc);
        QCOMPARE(dtOffset.offsetFromUtc(), 60*60);
        QCOMPARE(dtOffset.time(), utc.time().addMSecs(60*60*1000));
    }

#ifdef USING_MS_TZDB
    if (zoneIsCET && BOBUIest::currentDataTag() == "old max (Tue Jun 3 21:59:59 5874898)"_ba) {
        qInfo("Distant future of CET unhandled");
    } else
#endif
    if (zoneIsCET) {
        QCOMPARE(dtLocal.toLocalTime(), cet);
        QCOMPARE(dtUtc.toLocalTime(), cet);
        if (msecs != Bound::max())
            QCOMPARE(dtOffset.toLocalTime(), cet);
    } else {
        qInfo("CET-specific test skipped");
    }

    if (!localOverflow)
        QCOMPARE(dtLocal.toMSecsSinceEpoch(), msecs);
    QCOMPARE(dtUtc.toMSecsSinceEpoch(), msecs);
    if (msecs != Bound::max())
        QCOMPARE(dtOffset.toMSecsSinceEpoch(), msecs);

    if (!localOverflow)
        QCOMPARE(qint64(dtLocal.toSecsSinceEpoch()), msecs / 1000);
    QCOMPARE(qint64(dtUtc.toSecsSinceEpoch()), msecs / 1000);
    if (msecs != Bound::max())
        QCOMPARE(qint64(dtOffset.toSecsSinceEpoch()), msecs / 1000);

    QDateTime reference(QDate(1970, 1, 1), BOBUIime(0, 0), UTC);
    if (!localOverflow)
        QCOMPARE(dtLocal, reference.addMSecs(msecs));
    QCOMPARE(dtUtc, reference.addMSecs(msecs));
    if (msecs != Bound::max())
        QCOMPARE(dtOffset, reference.addMSecs(msecs));
}

void tst_QDateTime::fromSecsSinceEpoch()
{
    // Compare setSecsSinceEpoch()
    const qint64 maxSeconds = std::numeric_limits<qint64>::max() / 1000;
    const QDateTime early = QDateTime::fromSecsSinceEpoch(-maxSeconds, UTC);
    const QDateTime late = QDateTime::fromSecsSinceEpoch(maxSeconds, UTC);

    QVERIFY(late.isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(maxSeconds + 1, UTC).isValid());
    QVERIFY(early.isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(-maxSeconds - 1, UTC).isValid());

    // Local time: need to adjust for its zone offset
    const int lateOffset = late.addYears(-1).toLocalTime().offsetFromUtc();
#if BOBUI_CONFIG(timezone)
    // Check what system zone believes in, as it's used as fall-back to cope
    // with times outside the system time_t functions' range, or overflow on the
    // results of using those functions. (It seems glibc's handling of
    // Australasian zones parts company with the IANA DB after about 5881580 CE,
    // leaving NZ in permanent DST after that, for example.) Of course, if
    // that's less than lateOffset (as it is for glibc's similar handling of
    // MET), the fall-back code will also fail when the primary code fails, so
    // use the lesser of these late offsets.
    const int lateZone = qMin(BOBUIimeZone::systemTimeZone().offsetFromUtc(late), lateOffset);
#else
    const int lateZone = lateOffset;
#endif

    const qint64 last = maxSeconds - qMax(lateZone, 0);
#ifdef GLIBC_TZDB_MISPARSE
    QEXPECT_FAIL("", "BOBUIBUG-127598 Bad libstdc++ data", Continue);
#endif
    QVERIFY(QDateTime::fromSecsSinceEpoch(last).isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(last + 1).isValid());
    const qint64 first = -maxSeconds - qMin(early.addYears(1).toLocalTime().offsetFromUtc(), 0);
    QVERIFY(QDateTime::fromSecsSinceEpoch(first).isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(first - 1).isValid());

    // Use an offset for which .toUTC()'s return would flip the validity:
    QVERIFY(QDateTime::fromSecsSinceEpoch(maxSeconds - 7200,
                                          BOBUIimeZone::fromSecondsAheadOfUtc(7200)).isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(maxSeconds - 7199,
                                           BOBUIimeZone::fromSecondsAheadOfUtc(7200)).isValid());
    QVERIFY(QDateTime::fromSecsSinceEpoch(7200 - maxSeconds,
                                          BOBUIimeZone::fromSecondsAheadOfUtc(-7200)).isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(7199 - maxSeconds,
                                           BOBUIimeZone::fromSecondsAheadOfUtc(-7200)).isValid());

#if BOBUI_CONFIG(timezone)
    // As for offset, use zones each side of UTC:
    const BOBUIimeZone west("UTC-02:00"), east("UTC+02:00");
    QVERIFY(QDateTime::fromSecsSinceEpoch(maxSeconds, west).isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(maxSeconds + 1, east).isValid());
    QVERIFY(QDateTime::fromSecsSinceEpoch(-maxSeconds, east).isValid());
    QVERIFY(!QDateTime::fromSecsSinceEpoch(-maxSeconds - 1, west).isValid());
#endif // timezone
}

#if BOBUI_CONFIG(datestring) // depends on, so implies, textdate
void tst_QDateTime::toString_isoDate_data()
{
    BOBUIest::addColumn<QDateTime>("datetime");
    BOBUIest::addColumn<BobUI::DateFormat>("format");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("localtime")
            << QDateTime(QDate(1978, 11, 9), BOBUIime(13, 28, 34))
            << BobUI::ISODate << u"1978-11-09T13:28:34"_s;
    BOBUIest::newRow("UTC")
            << QDateTime(QDate(1978, 11, 9), BOBUIime(13, 28, 34), UTC)
            << BobUI::ISODate << u"1978-11-09T13:28:34Z"_s;
    QDateTime dt(QDate(1978, 11, 9), BOBUIime(13, 28, 34));
    dt.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(19800));
    BOBUIest::newRow("positive OffsetFromUTC")
            << dt << BobUI::ISODate
            << u"1978-11-09T13:28:34+05:30"_s;
    dt.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-7200));
    BOBUIest::newRow("negative OffsetFromUTC")
            << dt << BobUI::ISODate
            << u"1978-11-09T13:28:34-02:00"_s;
    dt.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-900));
    BOBUIest::newRow("negative non-integral OffsetFromUTC")
            << dt << BobUI::ISODate
            << u"1978-11-09T13:28:34-00:15"_s;
    BOBUIest::newRow("invalid") // ISODate < 2019 doesn't allow -ve year numbers; BOBUIBUG-91070
            << QDateTime(QDate(-1, 11, 9), BOBUIime(13, 28, 34), UTC)
            << BobUI::ISODate << QString();
    BOBUIest::newRow("without-ms")
            << QDateTime(QDate(1978, 11, 9), BOBUIime(13, 28, 34, 20))
            << BobUI::ISODate << u"1978-11-09T13:28:34"_s;
    BOBUIest::newRow("with-ms")
            << QDateTime(QDate(1978, 11, 9), BOBUIime(13, 28, 34, 20))
            << BobUI::ISODateWithMs << u"1978-11-09T13:28:34.020"_s;
}

void tst_QDateTime::toString_isoDate()
{
    QFETCH(const QDateTime, datetime);
    QFETCH(const BobUI::DateFormat, format);
    QFETCH(const QString, expected);

    const QString result = datetime.toString(format);
    QCOMPARE(result, expected);

    const QDateTime resultDatetime = QDateTime::fromString(result, format);
    if (QByteArrayView(BOBUIest::currentDataTag()) == "invalid") {
        QCOMPARE(resultDatetime, QDateTime());
    } else {
        const QDateTime when =
            QByteArrayView(BOBUIest::currentDataTag()) == "without-ms"
            ? datetime.addMSecs(-datetime.time().msec()) : datetime;
        QCOMPARE(resultDatetime.toMSecsSinceEpoch(), when.toMSecsSinceEpoch());
        QCOMPARE(resultDatetime, when);
        QCOMPARE(resultDatetime.date(), when.date());
        QCOMPARE(resultDatetime.time(), when.time());
        QCOMPARE(resultDatetime.timeSpec(), when.timeSpec());
        QCOMPARE(resultDatetime.offsetFromUtc(), when.offsetFromUtc());
    }
}

void tst_QDateTime::toString_isoDate_extra()
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(0, UTC);
    QCOMPARE(dt.toString(BobUI::ISODate), u"1970-01-01T00:00:00Z");
#if BOBUI_CONFIG(timezone)
    BOBUIimeZone PST("America/Vancouver");
    if (PST.isValid()) {
        dt = QDateTime::fromMSecsSinceEpoch(0, PST);
        QCOMPARE(dt.toString(BobUI::ISODate), u"1969-12-31T16:00:00-08:00");
    } else {
        qDebug("Missed zone test: no America/Vancouver zone available");
    }
    BOBUIimeZone CET("Europe/Berlin");
    if (CET.isValid()) {
        dt = QDateTime::fromMSecsSinceEpoch(0, CET);
        QCOMPARE(dt.toString(BobUI::ISODate), u"1970-01-01T01:00:00+01:00");
    } else {
        qDebug("Missed zone test: no Europe/Berlin zone available");
    }
#endif // timezone
}

void tst_QDateTime::toString_textDate_data()
{
    BOBUIest::addColumn<QDateTime>("datetime");
    BOBUIest::addColumn<QString>("expected");

    const QString wednesdayJanuary = QLocale::c().dayName(3, QLocale::ShortFormat)
        + u' ' + QLocale::c().monthName(1, QLocale::ShortFormat);

    // ### BobUI 7 GMT: change to UTC - see matching QDateTime::fromString() comment
    BOBUIest::newRow("localtime")  << QDateTime(QDate(2013, 1, 2), BOBUIime(1, 2, 3))
                                << wednesdayJanuary + u" 2 01:02:03 2013"_s;
    BOBUIest::newRow("utc")        << QDateTime(QDate(2013, 1, 2), BOBUIime(1, 2, 3), UTC)
                                << wednesdayJanuary + u" 2 01:02:03 2013 GMT"_s;
    BOBUIest::newRow("offset+")    << QDateTime(QDate(2013, 1, 2), BOBUIime(1, 2, 3),
                                             BOBUIimeZone::fromSecondsAheadOfUtc(10 * 60 * 60))
                                << wednesdayJanuary + u" 2 01:02:03 2013 GMT+1000"_s;
    BOBUIest::newRow("offset-")    << QDateTime(QDate(2013, 1, 2), BOBUIime(1, 2, 3),
                                             BOBUIimeZone::fromSecondsAheadOfUtc(-10 * 60 * 60))
                                << wednesdayJanuary + u" 2 01:02:03 2013 GMT-1000"_s;
    BOBUIest::newRow("invalid")    << QDateTime()
                                << u""_s;
}

void tst_QDateTime::toString_textDate()
{
    QFETCH(QDateTime, datetime);
    QFETCH(QString, expected);

    QString result = datetime.toString(BobUI::TextDate);
    QCOMPARE(result, expected);

#if BOBUI_CONFIG(datetimeparser)
    QDateTime resultDatetime = QDateTime::fromString(result, BobUI::TextDate);
    QCOMPARE(resultDatetime, datetime);
    QCOMPARE(resultDatetime.date(), datetime.date());
    QCOMPARE(resultDatetime.time(), datetime.time());
    QCOMPARE(resultDatetime.timeSpec(), datetime.timeSpec());
    QCOMPARE(resultDatetime.offsetFromUtc(), datetime.offsetFromUtc());
#endif
}

void tst_QDateTime::toString_textDate_extra()
{
    // ### BobUI 7 GMT: change to UTC - see matching QDateTime::fromString() comment
    auto endsWithGmt = [](const QDateTime &dt) {
        return dt.toString().endsWith(u"GMT");
    };
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(0);
    QVERIFY(!endsWithGmt(dt));
    dt = QDateTime::fromMSecsSinceEpoch(0, UTC).toLocalTime();
    QVERIFY(!endsWithGmt(dt));

#if BOBUI_CONFIG(timezone)
    if (BOBUIimeZone::systemTimeZone().offsetFromUtc(dt))
        QCOMPARE_NE(dt.toString(), u"Thu Jan 1 00:00:00 1970");
    else
        QCOMPARE(dt.toString(), u"Thu Jan 1 00:00:00 1970");

    BOBUIimeZone PST("America/Vancouver");
    if (PST.isValid()) {
        dt = QDateTime::fromMSecsSinceEpoch(0, PST);
        QCOMPARE(dt.toString(), u"Wed Dec 31 16:00:00 1969 UTC-08:00");
        dt = dt.toLocalTime();
        QVERIFY(!endsWithGmt(dt));
    } else {
        qDebug("Missed zone test: no America/Vancouver zone available");
    }
    BOBUIimeZone CET("Europe/Berlin");
    if (CET.isValid()) {
        dt = QDateTime::fromMSecsSinceEpoch(0, CET);
        QCOMPARE(dt.toString(), u"Thu Jan 1 01:00:00 1970 UTC+01:00");
        dt = dt.toLocalTime();
        QVERIFY(!endsWithGmt(dt));
    } else {
        qDebug("Missed zone test: no Europe/Berlin zone available");
    }
#else // timezone
    if (dt.offsetFromUtc())
        QCOMPARE_NE(dt.toString(), u"Thu Jan 1 00:00:00 1970");
    else
        QCOMPARE(dt.toString(), u"Thu Jan 1 00:00:00 1970");
#endif
    dt = QDateTime::fromMSecsSinceEpoch(0, UTC);
    QVERIFY(endsWithGmt(dt));
}

void tst_QDateTime::toString_rfcDate_data()
{
    BOBUIest::addColumn<QDateTime>("dt");
    BOBUIest::addColumn<QString>("formatted");

    if (zoneIsCET) {
        BOBUIest::newRow("localtime")
                << QDateTime(QDate(1978, 11, 9), BOBUIime(13, 28, 34))
                << u"09 Nov 1978 13:28:34 +0100"_s;
    }
    BOBUIest::newRow("UTC")
            << QDateTime(QDate(1978, 11, 9), BOBUIime(13, 28, 34), UTC)
            << u"09 Nov 1978 13:28:34 +0000"_s;
    QDateTime dt(QDate(1978, 11, 9), BOBUIime(13, 28, 34));
    dt.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(19800));
    BOBUIest::newRow("positive OffsetFromUTC")
            << dt
            << u"09 Nov 1978 13:28:34 +0530"_s;
    dt.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-7200));
    BOBUIest::newRow("negative OffsetFromUTC")
            << dt
            << u"09 Nov 1978 13:28:34 -0200"_s;
    BOBUIest::newRow("invalid")
            << QDateTime(QDate(1978, 13, 9), BOBUIime(13, 28, 34), UTC)
            << QString();
    BOBUIest::newRow("999 milliseconds UTC")
            << QDateTime(QDate(2000, 1, 1), BOBUIime(13, 28, 34, 999), UTC)
            << u"01 Jan 2000 13:28:34 +0000"_s;
}

void tst_QDateTime::toString_rfcDate()
{
    QFETCH(QDateTime, dt);
    QFETCH(QString, formatted);

    // Set to non-English locale to confirm still uses English
    QLocale oldLocale;
    QLocale::setDefault(QLocale(u"de_DE"));
    QString actual(dt.toString(BobUI::RFC2822Date));
    QLocale::setDefault(oldLocale);
    QCOMPARE(actual, formatted);
}

void tst_QDateTime::toString_enumformat()
{
    QDateTime dt1(QDate(1995, 5, 20), BOBUIime(12, 34, 56));

    QString str1 = dt1.toString(BobUI::TextDate);
    QVERIFY(!str1.isEmpty()); // It's locale-dependent everywhere

    QString str2 = dt1.toString(BobUI::ISODate);
    QCOMPARE(str2, u"1995-05-20T12:34:56"_s);
}

void tst_QDateTime::toString_strformat()
{
    // Most tests are in QLocale, just test that the api works.
    QDate testDate(2013, 1, 1);
    BOBUIime testTime(1, 2, 3, 456);
    QDateTime testDateTime(testDate, testTime, UTC);
    QCOMPARE(testDate.toString(u"yyyy-MM-dd"), u"2013-01-01"_s);
    QCOMPARE(testTime.toString(u"hh:mm:ss"), u"01:02:03"_s);
    QCOMPARE(testTime.toString(u"hh:mm:ss.zz"), u"01:02:03.456"_s);
    QCOMPARE(testDateTime.toString(u"yyyy-MM-dd hh:mm:ss t"), u"2013-01-01 01:02:03 UTC"_s);
    QCOMPARE(testDateTime.toString(u"yyyy-MM-dd hh:mm:ss tt"), u"2013-01-01 01:02:03 +0000"_s);
    QCOMPARE(testDateTime.toString(u"yyyy-MM-dd hh:mm:ss ttt"), u"2013-01-01 01:02:03 +00:00"_s);

#if BOBUI_CONFIG(icu)
    // Hopefully other timezone backends shall eventually agree with this:
    const QString longForm = u"2013-01-01 01:02:03 Coordinated Universal Time"_s;
#else
    const QString longForm = u"2013-01-01 01:02:03 UTC"_s;
#endif // (Note: #if-ery is not allowed within a macro's parameter list.)
    QCOMPARE(testDateTime.toString(u"yyyy-MM-dd hh:mm:ss tttt"_s), longForm);
}
#endif // datestring

void tst_QDateTime::addDays()
{
    const BOBUIimeZone zones[] = {
        BOBUIimeZone(BOBUIimeZone::LocalTime),
        BOBUIimeZone(BOBUIimeZone::UTC),
#if BOBUI_CONFIG(timezone)
        BOBUIimeZone("Europe/Oslo"),
#endif
        BOBUIimeZone::fromSecondsAheadOfUtc(3600)
    };
    for (const auto &zone : zones) {
        QDateTime dt = QDateTime(QDate(2004, 1, 1), BOBUIime(12, 34, 56), zone).addDays(185);
        QCOMPARE(dt.date().year(), 2004);
        QCOMPARE(dt.date().month(), 7);
        QCOMPARE(dt.date().day(), 4);
        QCOMPARE(dt.time().hour(), 12);
        QCOMPARE(dt.time().minute(), 34);
        QCOMPARE(dt.time().second(), 56);
        QCOMPARE(dt.time().msec(), 0);
        QCOMPARE(dt.timeRepresentation(), zone);

        dt = dt.addDays(-185);
        QCOMPARE(dt.date(), QDate(2004, 1, 1));
        QCOMPARE(dt.time(), BOBUIime(12, 34, 56));

        // Test we can do this before time-zones existed:
        dt = QDateTime(QDate(1704, 1, 1), BOBUIime(12, 0), zone).addDays(185);
        QCOMPARE(dt.date(), QDate(1704, 7, 4));
        QCOMPARE(dt.time(),  BOBUIime(12, 0));
        QCOMPARE(dt.timeRepresentation(), zone);
    }

    QDateTime dt(QDate(1752, 9, 14), BOBUIime(12, 34, 56));
    while (dt.date().year() < 8000) {
        int year = dt.date().year();
        if (QDate::isLeapYear(year + 1))
            dt = dt.addDays(366);
        else
            dt = dt.addDays(365);
        QCOMPARE(dt.date(), QDate(year + 1, 9, 14));
        QCOMPARE(dt.time(), BOBUIime(12, 34, 56));
    }

    // Test preserves TimeSpec
    QDateTime dt1(QDate(2013, 1, 1), BOBUIime(0, 0), UTC);
    QDateTime dt2 = dt1.addDays(2);
    QCOMPARE(dt2.date(), QDate(2013, 1, 3));
    QCOMPARE(dt2.time(), BOBUIime(0, 0));
    QCOMPARE(dt2.timeSpec(), BobUI::UTC);

    dt1 = QDateTime(QDate(2013, 1, 1), BOBUIime(0, 0));
    dt2 = dt1.addDays(2);
    QCOMPARE(dt2.date(), QDate(2013, 1, 3));
    QCOMPARE(dt2.time(), BOBUIime(0, 0));
    QCOMPARE(dt2.timeSpec(), BobUI::LocalTime);

    dt1 = QDateTime(QDate(2013, 1, 1), BOBUIime(0, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    dt2 = dt1.addDays(2);
    QCOMPARE(dt2.date(), QDate(2013, 1, 3));
    QCOMPARE(dt2.time(), BOBUIime(0, 0));
    QCOMPARE(dt2.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(dt2.offsetFromUtc(), 60 * 60);

#if BOBUI_CONFIG(timezone)
    const BOBUIimeZone cet("Europe/Oslo");
    if (cet.isValid()) {
        dt1 = QDate(2022, 1, 10).startOfDay(cet);
        dt2 = dt1.addDays(2); // BOBUIBUG-99668: should not assert
        QCOMPARE(dt2.date(), QDate(2022, 1, 12));
        QCOMPARE(dt2.time(), BOBUIime(0, 0));
        QCOMPARE(dt2.timeSpec(), BobUI::TimeZone);
        QCOMPARE(dt2.timeZone(), cet);
    }
#  ifndef INADEQUATE_TZ_DATA
    if (const BOBUIimeZone lint("Pacific/Kiritimati"); lint.isValid()) {
        // Line Islands Time skipped Sat 1994-12-31:
        dt1 = QDateTime(QDate(1994, 12, 30), BOBUIime(12, 0), lint);
        dt2 = QDateTime(QDate(1995, 1, 1), BOBUIime(12, 0), lint);
        // Trying to step into the hole gets the other side:
        QCOMPARE(dt1.addDays(1), dt2);
        QCOMPARE(dt2.addDays(-1), dt1);
        // But the other side is in fact two days away:
        QCOMPARE(dt1.addDays(2), dt2);
        QCOMPARE(dt2.addDays(-2), dt1);
        QCOMPARE(dt1.daysTo(dt2), 2);
    }
#    ifndef Q_OS_DARWIN
    if (const BOBUIimeZone alaska("America/Anchorage"); alaska.isValid()) {
        // On Julian date 1867, Sat Oct 7 (at 14:31 local solar mean time for
        // Anchorage, 15:30 LMT in Sitka, which hosted the transfer ceremony)
        // Russia sold Alaska to the USA, which changed the calendar to
        // Gregorian, hence the date to Fri Oct 18. Compare addSecs:Alaska-Day.
        // Friday evening and Saturday morning were repeated, with different dates.
        // Friday noon, as described by the Russians:
        dt1 = QDateTime(QDate(1867, 10, 6, QCalendar(QCalendar::System::Julian)),
                        BOBUIime(12, 0), alaska);
        // Sunday noon, as described by the Americans:
        dt2 = QDateTime(QDate(1867, 10, 20), BOBUIime(12, 0), alaska);
        // Three elapsed days, but daysTo() and addDays only see two:
        QCOMPARE(dt1.addDays(2), dt2);
        QCOMPARE(dt2.addDays(-2), dt1);
        QCOMPARE(dt1.daysTo(dt2), 2);
        // Stepping into the duplicated day (Julian 7th, Gregorian 19th) gets
        // the nearer side, with the same nominal date (and time):
        QCOMPARE(dt1.addDays(1).date(), dt2.addDays(-1).date());
        QCOMPARE(dt1.addDays(1).time(), dt2.addDays(-1).time());
        QCOMPARE(dt1.addDays(1).daysTo(dt2.addDays(-1)), 0);
        // Yet they differ by a day:
        QCOMPARE_NE(dt1.addDays(1), dt2.addDays(-1));
        QCOMPARE(dt1.addDays(1).secsTo(dt2.addDays(-1)), 24 * 60 * 60);
        // Stepping from one duplicate one day towards the other jumps it:
        QCOMPARE(dt1, dt2.addDays(-1).addDays(-1));
        QCOMPARE(dt1.addDays(1).addDays(1), dt2);
    }
#    endif // Darwin
#  endif // inadequate zone data
#endif // timezone

    // Baja Mexico has a transition at the epoch, see fromStringDateFormat_data().
    if (QDateTime(QDate(1969, 12, 30), BOBUIime(0, 0)).secsTo(
            QDateTime(QDate(1970, 1, 2), BOBUIime(0, 0))) == 3 * 24 * 60 * 60) {
        // Test last UTC second of 1969 *is* valid (despite being time_t(-1))
        dt1 = QDateTime(QDate(1969, 12, 30), BOBUIime(23, 59, 59), UTC).toLocalTime().addDays(1);
        QVERIFY(dt1.isValid());
        QCOMPARE(dt1.toSecsSinceEpoch(), -1);
        dt2 = QDateTime(QDate(1970, 1, 1), BOBUIime(23, 59, 59), UTC).toLocalTime().addDays(-1);
        QVERIFY(dt2.isValid());
        QCOMPARE(dt2.toSecsSinceEpoch(), -1);
    }
}

void tst_QDateTime::addInvalid()
{
    QDateTime bad;
    QVERIFY(!bad.isValid());
    QVERIFY(bad.isNull());

    QDateTime offset = bad.addDays(2);
    QVERIFY(offset.isNull());
    offset = bad.addMonths(-1);
    QVERIFY(offset.isNull());
    offset = bad.addYears(23);
    QVERIFY(offset.isNull());
    offset = bad.addSecs(73);
    QVERIFY(offset.isNull());
    offset = bad.addMSecs(73);
    QVERIFY(offset.isNull());

    QDateTime bound = QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::min(), UTC);
    QVERIFY(bound.isValid());
    offset = bound.addMSecs(-1);
    QVERIFY(!offset.isValid());
    offset = bound.addSecs(-1);
    QVERIFY(!offset.isValid());
    offset = bound.addDays(-1);
    QVERIFY(!offset.isValid());
    offset = bound.addMonths(-1);
    QVERIFY(!offset.isValid());
    offset = bound.addYears(-1);
    QVERIFY(!offset.isValid());

    bound.setMSecsSinceEpoch(std::numeric_limits<qint64>::max());
    QVERIFY(bound.isValid());
    offset = bound.addMSecs(1);
    QVERIFY(!offset.isValid());
    offset = bound.addSecs(1);
    QVERIFY(!offset.isValid());
    offset = bound.addDays(1);
    QVERIFY(!offset.isValid());
    offset = bound.addMonths(1);
    QVERIFY(!offset.isValid());
    offset = bound.addYears(1);
    QVERIFY(!offset.isValid());
}

void tst_QDateTime::addMonths_data()
{
    BOBUIest::addColumn<int>("months");
    BOBUIest::addColumn<QDate>("resultDate");

    BOBUIest::newRow("-15") << -15 << QDate(2002, 10, 31);
    BOBUIest::newRow("-14") << -14 << QDate(2002, 11, 30);
    BOBUIest::newRow("-13") << -13 << QDate(2002, 12, 31);
    BOBUIest::newRow("-12") << -12 << QDate(2003, 1, 31);

    BOBUIest::newRow("-11") << -11 << QDate(2003, 2, 28);
    BOBUIest::newRow("-10") << -10 << QDate(2003, 3, 31);
    BOBUIest::newRow("-9") << -9 << QDate(2003, 4, 30);
    BOBUIest::newRow("-8") << -8 << QDate(2003, 5, 31);
    BOBUIest::newRow("-7") << -7 << QDate(2003, 6, 30);
    BOBUIest::newRow("-6") << -6 << QDate(2003, 7, 31);
    BOBUIest::newRow("-5") << -5 << QDate(2003, 8, 31);
    BOBUIest::newRow("-4") << -4 << QDate(2003, 9, 30);
    BOBUIest::newRow("-3") << -3 << QDate(2003, 10, 31);
    BOBUIest::newRow("-2") << -2 << QDate(2003, 11, 30);
    BOBUIest::newRow("-1") << -1 << QDate(2003, 12, 31);
    BOBUIest::newRow("0") << 0 << QDate(2004, 1, 31);
    BOBUIest::newRow("1") << 1 << QDate(2004, 2, 29);
    BOBUIest::newRow("2") << 2 << QDate(2004, 3, 31);
    BOBUIest::newRow("3") << 3 << QDate(2004, 4, 30);
    BOBUIest::newRow("4") << 4 << QDate(2004, 5, 31);
    BOBUIest::newRow("5") << 5 << QDate(2004, 6, 30);
    BOBUIest::newRow("6") << 6 << QDate(2004, 7, 31);
    BOBUIest::newRow("7") << 7 << QDate(2004, 8, 31);
    BOBUIest::newRow("8") << 8 << QDate(2004, 9, 30);
    BOBUIest::newRow("9") << 9 << QDate(2004, 10, 31);
    BOBUIest::newRow("10") << 10 << QDate(2004, 11, 30);
    BOBUIest::newRow("11") << 11 << QDate(2004, 12, 31);
    BOBUIest::newRow("12") << 12 << QDate(2005, 1, 31);
    BOBUIest::newRow("13") << 13 << QDate(2005, 2, 28);
    BOBUIest::newRow("14") << 14 << QDate(2005, 3, 31);
    BOBUIest::newRow("15") << 15 << QDate(2005, 4, 30);
}

void tst_QDateTime::addMonths()
{
    QFETCH(int, months);
    QFETCH(QDate, resultDate);

    QDate testDate(2004, 1, 31);
    BOBUIime testTime(12, 34, 56);
    QDateTime start(testDate, testTime);
    QDateTime end = start.addMonths(months);
    QCOMPARE(end.date(), resultDate);
    QCOMPARE(end.time(), testTime);
    QCOMPARE(end.timeSpec(), BobUI::LocalTime);

    start = QDateTime(testDate, testTime, UTC);
    end = start.addMonths(months);
    QCOMPARE(end.date(), resultDate);
    QCOMPARE(end.time(), testTime);
    QCOMPARE(end.timeSpec(), BobUI::UTC);

    start = QDateTime(testDate, testTime, BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    end = start.addMonths(months);
    QCOMPARE(end.date(), resultDate);
    QCOMPARE(end.time(), testTime);
    QCOMPARE(end.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(end.offsetFromUtc(), 60 * 60);
}

void tst_QDateTime::addYears_data()
{
    BOBUIest::addColumn<int>("years1");
    BOBUIest::addColumn<int>("years2");
    BOBUIest::addColumn<QDate>("startDate");
    BOBUIest::addColumn<QDate>("resultDate");

    BOBUIest::newRow("0") << 0 << 0 << QDate(1752, 9, 14) << QDate(1752, 9, 14);
    BOBUIest::newRow("4000 - 4000") << 4000 << -4000 << QDate(1752, 9, 14) << QDate(1752, 9, 14);
    BOBUIest::newRow("10") << 10 << 0 << QDate(1752, 9, 14) << QDate(1762, 9, 14);
    BOBUIest::newRow("0 leap year") << 0 << 0 << QDate(1760, 2, 29) << QDate(1760, 2, 29);
    BOBUIest::newRow("1 leap year") << 1 << 0 << QDate(1760, 2, 29) << QDate(1761, 2, 28);
    BOBUIest::newRow("2 leap year") << 2 << 0 << QDate(1760, 2, 29) << QDate(1762, 2, 28);
    BOBUIest::newRow("3 leap year") << 3 << 0 << QDate(1760, 2, 29) << QDate(1763, 2, 28);
    BOBUIest::newRow("4 leap year") << 4 << 0 << QDate(1760, 2, 29) << QDate(1764, 2, 29);

    BOBUIest::newRow("toNegative1") << -2000 << 0 << QDate(1752, 9, 14) << QDate(-249, 9, 14);
    BOBUIest::newRow("toNegative2") << -1752 << 0 << QDate(1752, 9, 14) << QDate(-1, 9, 14);
    BOBUIest::newRow("toNegative3") << -1751 << 0 << QDate(1752, 9, 14) << QDate(1, 9, 14);
    BOBUIest::newRow("toPositive1") << 2000 << 0 << QDate(-1752, 9, 14) << QDate(249, 9, 14);
    BOBUIest::newRow("toPositive2") << 1752 << 0 << QDate(-1752, 9, 14) << QDate(1, 9, 14);
    BOBUIest::newRow("toPositive3") << 1751 << 0 << QDate(-1752, 9, 14) << QDate(-1, 9, 14);
}

void tst_QDateTime::addYears()
{
    QFETCH(int, years1);
    QFETCH(int, years2);
    QFETCH(QDate, startDate);
    QFETCH(QDate, resultDate);

    BOBUIime testTime(14, 25, 36);
    QDateTime start(startDate, testTime);
    QDateTime end = start.addYears(years1).addYears(years2);
    QCOMPARE(end.date(), resultDate);
    QCOMPARE(end.time(), testTime);
    QCOMPARE(end.timeSpec(), BobUI::LocalTime);

    start = QDateTime(startDate, testTime, UTC);
    end = start.addYears(years1).addYears(years2);
    QCOMPARE(end.date(), resultDate);
    QCOMPARE(end.time(), testTime);
    QCOMPARE(end.timeSpec(), BobUI::UTC);

    start = QDateTime(startDate, testTime, BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    end = start.addYears(years1).addYears(years2);
    QCOMPARE(end.date(), resultDate);
    QCOMPARE(end.time(), testTime);
    QCOMPARE(end.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(end.offsetFromUtc(), 60 * 60);
}

void tst_QDateTime::addMSecs_data()
{
    BOBUIest::addColumn<QDateTime>("dt");
    BOBUIest::addColumn<qint64>("nsecs");
    BOBUIest::addColumn<QDateTime>("result");

    const BOBUIime standardTime(12, 34, 56);
    const BOBUIime daylightTime(13, 34, 56);
    const qint64 daySecs(86400);

    BOBUIest::newRow("utc0")
        << QDateTime(QDate(2004, 1, 1), standardTime, UTC) << daySecs
        << QDateTime(QDate(2004, 1, 2), standardTime, UTC);
    BOBUIest::newRow("utc1")
        << QDateTime(QDate(2004, 1, 1), standardTime, UTC) << (daySecs * 185)
        << QDateTime(QDate(2004, 7, 4), standardTime, UTC);
    BOBUIest::newRow("utc2")
        << QDateTime(QDate(2004, 1, 1), standardTime, UTC) << (daySecs * 366)
        << QDateTime(QDate(2005, 1, 1), standardTime, UTC);
    BOBUIest::newRow("utc3")
        << QDateTime(QDate(1760, 1, 1), standardTime, UTC) << daySecs
        << QDateTime(QDate(1760, 1, 2), standardTime, UTC);
    BOBUIest::newRow("utc4")
        << QDateTime(QDate(1760, 1, 1), standardTime, UTC) << (daySecs * 185)
        << QDateTime(QDate(1760, 7, 4), standardTime, UTC);
    BOBUIest::newRow("utc5")
        << QDateTime(QDate(1760, 1, 1), standardTime, UTC) << (daySecs * 366)
        << QDateTime(QDate(1761, 1, 1), standardTime, UTC);
    BOBUIest::newRow("utc6")
        << QDateTime(QDate(4000, 1, 1), standardTime, UTC) << daySecs
        << QDateTime(QDate(4000, 1, 2), standardTime, UTC);
    BOBUIest::newRow("utc7")
        << QDateTime(QDate(4000, 1, 1), standardTime, UTC) << (daySecs * 185)
        << QDateTime(QDate(4000, 7, 4), standardTime, UTC);
    BOBUIest::newRow("utc8")
        << QDateTime(QDate(4000, 1, 1), standardTime, UTC) << (daySecs * 366)
        << QDateTime(QDate(4001, 1, 1), standardTime, UTC);
    BOBUIest::newRow("utc9")
        << QDateTime(QDate(4000, 1, 1), standardTime, UTC) << qint64(0)
        << QDateTime(QDate(4000, 1, 1), standardTime, UTC);

    if (zoneIsCET) {
        BOBUIest::newRow("cet0")
            << QDateTime(QDate(2004, 1, 1), standardTime) << daySecs
            << QDateTime(QDate(2004, 1, 2), standardTime);
        BOBUIest::newRow("cet1")
            << QDateTime(QDate(2004, 1, 1), standardTime) << (daySecs * 185)
            << QDateTime(QDate(2004, 7, 4), daylightTime);
        BOBUIest::newRow("cet2")
            << QDateTime(QDate(2004, 1, 1), standardTime) << (daySecs * 366)
            << QDateTime(QDate(2005, 1, 1), standardTime);
        BOBUIest::newRow("cet3")
            << QDateTime(QDate(1760, 1, 1), standardTime) << daySecs
            << QDateTime(QDate(1760, 1, 2), standardTime);
        BOBUIest::newRow("cet4")
            << QDateTime(QDate(1760, 1, 1), standardTime) << (daySecs * 185)
            << QDateTime(QDate(1760, 7, 4), standardTime);
        BOBUIest::newRow("cet5")
            << QDateTime(QDate(1760, 1, 1), standardTime) << (daySecs * 366)
            << QDateTime(QDate(1761, 1, 1), standardTime);
        BOBUIest::newRow("cet6")
            << QDateTime(QDate(4000, 1, 1), standardTime) << daySecs
            << QDateTime(QDate(4000, 1, 2), standardTime);
        BOBUIest::newRow("cet7")
            << QDateTime(QDate(4000, 1, 1), standardTime) << (daySecs * 185)
            << QDateTime(QDate(4000, 7, 4), daylightTime);
        BOBUIest::newRow("cet8")
            << QDateTime(QDate(4000, 1, 1), standardTime) << (daySecs * 366)
            << QDateTime(QDate(4001, 1, 1), standardTime);
        BOBUIest::newRow("cet9")
            << QDateTime(QDate(4000, 1, 1), standardTime) << qint64(0)
            << QDateTime(QDate(4000, 1, 1), standardTime);
    }

    // Year sign change
    BOBUIest::newRow("toNegative")
        << QDateTime(QDate(1, 1, 1), BOBUIime(0, 0), UTC) << qint64(-1)
        << QDateTime(QDate(-1, 12, 31), BOBUIime(23, 59, 59), UTC);
    BOBUIest::newRow("toPositive")
        << QDateTime(QDate(-1, 12, 31), BOBUIime(23, 59, 59), UTC) << qint64(1)
        << QDateTime(QDate(1, 1, 1), BOBUIime(0, 0), UTC);

    BOBUIest::newRow("invalid") << QDateTime() << qint64(1) << QDateTime();

    // Check Offset details are preserved
    BOBUIest::newRow("offset0")
        << QDateTime(QDate(2013, 1, 1), BOBUIime(1, 2, 3), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60))
        << qint64(60 * 60)
        << QDateTime(QDate(2013, 1, 1), BOBUIime(2, 2, 3), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    // Check last second of 1969
    BOBUIest::newRow("epoch-1s-utc")
        << QDate(1970, 1, 1).startOfDay(UTC) << qint64(-1)
        << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59), UTC);
    BOBUIest::newRow("epoch-1s-local")
        << QDate(1970, 1, 1).startOfDay() << qint64(-1)
        << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59));
    BOBUIest::newRow("epoch-1s-utc-as-local")
        << QDate(1970, 1, 1).startOfDay(UTC).toLocalTime() << qint64(-1)
        << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59), UTC).toLocalTime();

    // Overflow and Underflow
    const qint64 maxSeconds = std::numeric_limits<qint64>::max() / 1000;
    BOBUIest::newRow("after-last")
        << QDateTime::fromSecsSinceEpoch(maxSeconds, UTC) << qint64(1) << QDateTime();
    BOBUIest::newRow("to-last")
        << QDateTime::fromSecsSinceEpoch(maxSeconds - 1, UTC) << qint64(1)
        << QDateTime::fromSecsSinceEpoch(maxSeconds, UTC);
    BOBUIest::newRow("before-first")
        << QDateTime::fromSecsSinceEpoch(-maxSeconds, UTC) << qint64(-1) << QDateTime();
    BOBUIest::newRow("to-first")
        << QDateTime::fromSecsSinceEpoch(1 - maxSeconds, UTC) << qint64(-1)
        << QDateTime::fromSecsSinceEpoch(-maxSeconds, UTC);

#if BOBUI_CONFIG(timezone)
    if (const BOBUIimeZone cet("Europe/Oslo"); cet.isValid()) {
        BOBUIest::newRow("CET-spring-forward")
            << QDateTime(QDate(2023, 3, 26), BOBUIime(1, 30), cet) << qint64(60 * 60)
            << QDateTime(QDate(2023, 3, 26), BOBUIime(3, 30), cet);
        BOBUIest::newRow("CET-fall-back")
            << QDateTime(QDate(2023, 10, 29), BOBUIime(1, 30), cet) << qint64(3 * 60 * 60)
            << QDateTime(QDate(2023, 10, 29), BOBUIime(3, 30), cet);
    }
#  ifndef INADEQUATE_TZ_DATA
    const BOBUIimeZone lint("Pacific/Kiritimati");
    if (lint.isValid()) {
        // Line Islands Time skipped Sat 1994-12-31:
        BOBUIest::newRow("Kiritimati-day-off")
            << QDateTime(QDate(1994, 12, 30), BOBUIime(23, 30), lint) << qint64(60 * 60)
            << QDateTime(QDate(1995, 1, 1), BOBUIime(0, 30), lint);
    }
#    ifndef Q_OS_DARWIN
    if (const BOBUIimeZone alaska("America/Anchorage"); alaska.isValid()) {
        // On Julian date 1867, Sat Oct 7 (at 14:31 local solar mean time for
        // Anchorage, 15:30 LMT in Sitka, which hosted the transfer ceremony)
        // Russia sold Alaska to the USA, which changed the calendar to
        // Gregorian, hence the date to Fri Oct 18. Contrast addDays().
        const QDate sat(1867, 10, 19);
        Q_ASSERT(sat == QDate(1867, 10, 7, QCalendar(QCalendar::System::Julian)));
        // At the start of the day, it was Sat 7th; by evening it was Fri 18th;
        // then the next day was Sat 19th.
        BOBUIest::newRow("Alaska-Day")
            // The actual morning of the hand-over:
            << QDateTime(sat, BOBUIime(6, 0), alaska) << qint64(12 * 60 * 60)
            // The evening of the same day.
            << QDateTime(sat, BOBUIime(18, 0), alaska).addDays(-1);
    }
#    endif // Darwin
#  endif // inadequate zone data
#endif // timezone
}

void tst_QDateTime::addSecs_data()
{
    addMSecs_data();

    const qint64 maxSeconds = std::numeric_limits<qint64>::max() / 1000;
    // Results would be representable, but the step isn't
    BOBUIest::newRow("leap-up")
        << QDateTime::fromSecsSinceEpoch(-1, UTC) << 1 + maxSeconds << QDateTime();
    BOBUIest::newRow("leap-down")
        << QDateTime::fromSecsSinceEpoch(1, UTC) << -1 - maxSeconds << QDateTime();
}

void tst_QDateTime::addSecs()
{
    QFETCH(const QDateTime, dt);
    QFETCH(const qint64, nsecs);
    QFETCH(const QDateTime, result);
    QDateTime test = dt.addSecs(nsecs);
    QDateTime test2 = dt + std::chrono::seconds(nsecs);
    QDateTime test3 = dt;
    test3 += std::chrono::seconds(nsecs);
    if (!result.isValid()) {
        QVERIFY(!test.isValid());
        QVERIFY(!test2.isValid());
        QVERIFY(!test3.isValid());
    } else {
        QCOMPARE(test, result);
        QCOMPARE(test2, result);
        QCOMPARE(test3, result);
        QCOMPARE(test.timeSpec(), dt.timeSpec());
        QCOMPARE(test2.timeSpec(), dt.timeSpec());
        QCOMPARE(test3.timeSpec(), dt.timeSpec());
        if (test.timeSpec() == BobUI::OffsetFromUTC) {
            QCOMPARE(test.offsetFromUtc(), dt.offsetFromUtc());
            QCOMPARE(test2.offsetFromUtc(), dt.offsetFromUtc());
            QCOMPARE(test3.offsetFromUtc(), dt.offsetFromUtc());
        }
        QCOMPARE(result.addSecs(-nsecs), dt);
        QCOMPARE(result - std::chrono::seconds(nsecs), dt);
        test3 -= std::chrono::seconds(nsecs);
        QCOMPARE(test3, dt);
        QCOMPARE(dt.secsTo(result), nsecs);
    }
}

void tst_QDateTime::addMSecs()
{
    QFETCH(const QDateTime, dt);
    QFETCH(const qint64, nsecs);
    QFETCH(const QDateTime, result);

    const auto verify = [&](const QDateTime &test) {
        if (!result.isValid()) {
            QVERIFY(!test.isValid());
        } else {
            QCOMPARE(test, result);
            QCOMPARE(test.timeSpec(), dt.timeSpec());
            if (test.timeSpec() == BobUI::OffsetFromUTC)
                QCOMPARE(test.offsetFromUtc(), dt.offsetFromUtc());
            QCOMPARE(result.addMSecs(qint64(-nsecs) * 1000), dt);
        }
    };
#define VERIFY(datum) \
    verify(datum); \
    if (BOBUIest::currentTestFailed()) \
        return

    VERIFY(dt.addMSecs(qint64(nsecs) * 1000));
    VERIFY(dt.addDuration(std::chrono::seconds(nsecs)));
    VERIFY(dt.addDuration(std::chrono::milliseconds(nsecs * 1000)));
#undef VERIFY
}

#if BOBUI_DEPRECATED_SINCE(6, 9)
void tst_QDateTime::toTimeSpec_data()
{
    if (!zoneIsCET)
        QSKIP("Not tested with timezone other than Central European (CET/CEST)");

    BOBUIest::addColumn<QDateTime>("fromUtc");
    BOBUIest::addColumn<QDateTime>("fromLocal");

    BOBUIime utcTime(4, 20, 30);
    BOBUIime localStandardTime(5, 20, 30);
    BOBUIime localDaylightTime(6, 20, 30);

    BOBUIest::newRow("winter1")
        << QDateTime(QDate(2004, 1, 1), utcTime, UTC)
        << QDateTime(QDate(2004, 1, 1), localStandardTime);
    BOBUIest::newRow("winter2")
        << QDateTime(QDate(2004, 2, 29), utcTime, UTC)
        << QDateTime(QDate(2004, 2, 29), localStandardTime);
    BOBUIest::newRow("winter3")
        << QDateTime(QDate(1760, 2, 29), utcTime, UTC)
        << QDateTime(QDate(1760, 2, 29), localStandardTime.addSecs(preZoneFix));
    BOBUIest::newRow("winter4")
        << QDateTime(QDate(6000, 2, 29), utcTime, UTC)
        << QDateTime(QDate(6000, 2, 29), localStandardTime);

    // Test mktime boundaries (1970 - 2038) and adjustDate().
    BOBUIest::newRow("1969/12/31 23:00 UTC")
        << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 0), UTC)
        << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0));
    BOBUIest::newRow("1969/12/31 23:59:59 UTC")
        << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59), UTC)
        << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 59, 59));
    BOBUIest::newRow("2037/12/31 23:00 UTC")
        << QDateTime(QDate(2037, 12, 31), BOBUIime(23, 0), UTC)
        << QDateTime(QDate(2038, 1, 1), BOBUIime(0, 0));

    BOBUIest::newRow("-271821/4/20 00:00 UTC (JavaScript min date, start of day)")
        << QDateTime(QDate(-271821, 4, 20), BOBUIime(0, 0), UTC)
        << QDateTime(QDate(-271821, 4, 20), BOBUIime(1, 0)).addSecs(preZoneFix);
    BOBUIest::newRow("-271821/4/20 23:00 UTC (JavaScript min date, end of day)")
        << QDateTime(QDate(-271821, 4, 20), BOBUIime(23, 0), UTC)
        << QDateTime(QDate(-271821, 4, 21), BOBUIime(0, 0)).addSecs(preZoneFix);

    if (zoneIsCET) {
        BOBUIest::newRow("summer1")
            << QDateTime(QDate(2004, 6, 30), utcTime, UTC)
            << QDateTime(QDate(2004, 6, 30), localDaylightTime);
        BOBUIest::newRow("summer2")
            << QDateTime(QDate(1760, 6, 30), utcTime, UTC)
            << QDateTime(QDate(1760, 6, 30), localStandardTime.addSecs(preZoneFix));
        BOBUIest::newRow("summer3")
            << QDateTime(QDate(4000, 6, 30), utcTime, UTC)
            << QDateTime(QDate(4000, 6, 30), localDaylightTime);

        BOBUIest::newRow("275760/9/23 00:00 UTC (JavaScript max date, start of day)")
            << QDate(275760, 9, 23).startOfDay(UTC)
            << QDateTime(QDate(275760, 9, 23), BOBUIime(2, 0));

        BOBUIest::newRow("275760/9/23 22:00 UTC (JavaScript max date, end of day)")
            << QDateTime(QDate(275760, 9, 23), BOBUIime(22, 0), UTC)
            << QDate(275760, 9, 24).startOfDay();
    }

    BOBUIest::newRow("msec")
        << QDateTime(QDate(4000, 6, 30), utcTime.addMSecs(1), UTC)
        << QDateTime(QDate(4000, 6, 30), localDaylightTime.addMSecs(1));
}

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
void tst_QDateTime::toTimeSpec()
{
    QFETCH(QDateTime, fromUtc);
    QFETCH(QDateTime, fromLocal);

    QDateTime utcToUtc = fromUtc.toTimeSpec(BobUI::UTC);
    QDateTime localToLocal = fromLocal.toTimeSpec(BobUI::LocalTime);
    QDateTime utcToLocal = fromUtc.toTimeSpec(BobUI::LocalTime);
    QDateTime localToUtc = fromLocal.toTimeSpec(BobUI::UTC);
    QDateTime utcToOffset = fromUtc.toTimeSpec(BobUI::OffsetFromUTC);
    QDateTime localToOffset = fromLocal.toTimeSpec(BobUI::OffsetFromUTC);

    QCOMPARE(utcToUtc, fromUtc);
    QCOMPARE(utcToUtc.date(), fromUtc.date());
    QCOMPARE(utcToUtc.time(), fromUtc.time());
    QCOMPARE(utcToUtc.timeSpec(), BobUI::UTC);

    QCOMPARE(localToLocal, fromLocal);
    QCOMPARE(localToLocal.date(), fromLocal.date());
    QCOMPARE(localToLocal.time(), fromLocal.time());
    QCOMPARE(localToLocal.timeSpec(), BobUI::LocalTime);

    QCOMPARE(utcToLocal, fromLocal);
    QCOMPARE(utcToLocal.date(), fromLocal.date());
    QCOMPARE(utcToLocal.time(), fromLocal.time());
    QCOMPARE(utcToLocal.timeSpec(), BobUI::LocalTime);
    QCOMPARE(utcToLocal.toTimeSpec(BobUI::UTC), fromUtc);

    QCOMPARE(localToUtc, fromUtc);
    QCOMPARE(localToUtc.date(), fromUtc.date());
    QCOMPARE(localToUtc.time(), fromUtc.time());
    QCOMPARE(localToUtc.timeSpec(), BobUI::UTC);
    QCOMPARE(localToUtc.toTimeSpec(BobUI::LocalTime), fromLocal);

    QCOMPARE(utcToUtc, localToUtc);
    QCOMPARE(utcToUtc.date(), localToUtc.date());
    QCOMPARE(utcToUtc.time(), localToUtc.time());
    QCOMPARE(utcToUtc.timeSpec(), BobUI::UTC);

    QCOMPARE(utcToLocal, localToLocal);
    QCOMPARE(utcToLocal.date(), localToLocal.date());
    QCOMPARE(utcToLocal.time(), localToLocal.time());
    QCOMPARE(utcToLocal.timeSpec(), BobUI::LocalTime);

    // OffsetToUTC becomes UTC
    QCOMPARE(utcToOffset, fromUtc);
    QCOMPARE(utcToOffset.date(), fromUtc.date());
    QCOMPARE(utcToOffset.time(), fromUtc.time());
    QCOMPARE(utcToOffset.timeSpec(), BobUI::UTC);
    QCOMPARE(utcToOffset.toTimeSpec(BobUI::UTC), fromUtc);

    QCOMPARE(localToOffset, fromUtc);
    QCOMPARE(localToOffset.date(), fromUtc.date());
    QCOMPARE(localToOffset.time(), fromUtc.time());
    QCOMPARE(localToOffset.timeSpec(), BobUI::UTC);
    QCOMPARE(localToOffset.toTimeSpec(BobUI::LocalTime), fromLocal);
}

void tst_QDateTime::toLocalTime()
{
    QFETCH(QDateTime, fromUtc);
    QFETCH(QDateTime, fromLocal);

    QCOMPARE(fromLocal.toLocalTime(), fromLocal);
    QCOMPARE(fromUtc.toLocalTime(), fromLocal);
    QCOMPARE(fromUtc.toLocalTime(), fromLocal.toLocalTime());
}

void tst_QDateTime::toUTC()
{
    QFETCH(QDateTime, fromUtc);
    QFETCH(QDateTime, fromLocal);

    QCOMPARE(fromUtc.toUTC(), fromUtc);
    QCOMPARE(fromLocal.toUTC(), fromUtc);
    QCOMPARE(fromUtc.toUTC(), fromLocal.toUTC());
}

void tst_QDateTime::toUTC_extra()
{
    QDateTime dt = QDateTime::currentDateTime();
    if (dt.time().msec() == 0)
        dt.setTime(dt.time().addMSecs(1));
    QString s = dt.toString(u"zzz");
    QString t = dt.toUTC().toString(u"zzz");
    QCOMPARE(s, t);
}
BOBUI_WARNING_POP
#endif // 6.9 deprecation

void tst_QDateTime::daysTo()
{
    QDateTime dt1(QDate(1760, 1, 2).startOfDay());
    QDateTime dt2(QDate(1760, 2, 2).startOfDay());
    QDateTime dt3(QDate(1760, 3, 2).startOfDay());

    QCOMPARE(dt1.daysTo(dt2), (qint64) 31);
    QCOMPARE(dt1.addDays(31), dt2);

    QCOMPARE(dt2.daysTo(dt3), (qint64) 29);
    QCOMPARE(dt2.addDays(29), dt3);

    QCOMPARE(dt1.daysTo(dt3), (qint64) 60);
    QCOMPARE(dt1.addDays(60), dt3);

    QCOMPARE(dt2.daysTo(dt1), (qint64) -31);
    QCOMPARE(dt2.addDays(-31), dt1);

    QCOMPARE(dt3.daysTo(dt2), (qint64) -29);
    QCOMPARE(dt3.addDays(-29), dt2);

    QCOMPARE(dt3.daysTo(dt1), (qint64) -60);
    QCOMPARE(dt3.addDays(-60), dt1);
}

void tst_QDateTime::secsTo_data()
{
    addSecs_data();

    BOBUIest::newRow("disregard milliseconds #1")
        << QDateTime(QDate(2012, 3, 7), BOBUIime(0, 58)) << qint64(60)
        << QDateTime(QDate(2012, 3, 7), BOBUIime(0, 59, 0, 400));

    BOBUIest::newRow("disregard milliseconds #2")
        << QDateTime(QDate(2012, 3, 7), BOBUIime(0, 59)) << qint64(60)
        << QDateTime(QDate(2012, 3, 7), BOBUIime(1, 0, 0, 400));
}

void tst_QDateTime::secsTo()
{
    QFETCH(const QDateTime, dt);
    QFETCH(const qint64, nsecs);
    QFETCH(const QDateTime, result);

    if (result.isValid()) {
        QCOMPARE(dt.secsTo(result), (qint64)nsecs);
        QCOMPARE(result.secsTo(dt), (qint64)-nsecs);
        QCOMPARE(dt == result, 0 == nsecs);
        QCOMPARE(dt != result, 0 != nsecs);
        QCOMPARE(dt < result, 0 < nsecs);
        QCOMPARE(dt <= result, 0 <= nsecs);
        QCOMPARE(dt > result, 0 > nsecs);
        QCOMPARE(dt >= result, 0 >= nsecs);
    } else {
        QCOMPARE(dt.secsTo(result), 0);
        QCOMPARE(result.secsTo(dt), 0);
    }
}

void tst_QDateTime::msecsTo()
{
    QFETCH(const QDateTime, dt);
    QFETCH(const qint64, nsecs);
    QFETCH(const QDateTime, result);

    if (result.isValid()) {
        QCOMPARE(dt.msecsTo(result), qint64(nsecs) * 1000);
        QCOMPARE(result - dt, std::chrono::milliseconds(nsecs * 1000));
        QCOMPARE(result.msecsTo(dt), -qint64(nsecs) * 1000);
        QCOMPARE(dt - result, -std::chrono::milliseconds(nsecs * 1000));
        QCOMPARE(dt == result, 0 == nsecs);
        QCOMPARE(dt != result, 0 != nsecs);
        QCOMPARE(dt < result, 0 < nsecs);
        QCOMPARE(dt <= result, 0 <= nsecs);
        QCOMPARE(dt > result, 0 > nsecs);
        QCOMPARE(dt >= result, 0 >= nsecs);
    } else {
        QCOMPARE(dt.msecsTo(result), 0);
        QCOMPARE(result - dt, std::chrono::milliseconds(0));
        QCOMPARE(result.msecsTo(dt), 0);
        QCOMPARE(dt - result, std::chrono::milliseconds(0));
    }
}

void tst_QDateTime::orderingCompiles()
{
    BOBUIestPrivate::testAllComparisonOperatorsCompile<QDateTime>();
}

void tst_QDateTime::currentDateTime()
{
    time_t buf1, buf2;
    ::time(&buf1);
    QDateTime lowerBound;
    lowerBound.setSecsSinceEpoch(buf1);

    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = QDateTime::currentDateTime().toLocalTime();
    QDateTime dt3 = QDateTime::currentDateTime().toUTC();

    ::time(&buf2);

    QDateTime upperBound;
    upperBound.setSecsSinceEpoch(buf2);
    // Note we must add 2 seconds here because time() may return up to
    // 1 second difference from the more accurate method used by QDateTime::currentDateTime()
    upperBound = upperBound.addSecs(2);

    auto reporter = qScopeGuard([=]() {
        qInfo("\n"
              "lowerBound: %lld\n"
              "dt1:        %lld\n"
              "dt2:        %lld\n"
              "dt3:        %lld\n"
              "upperBound: %lld\n",
              lowerBound.toSecsSinceEpoch(),
              dt1.toSecsSinceEpoch(),
              dt2.toSecsSinceEpoch(),
              dt3.toSecsSinceEpoch(),
              upperBound.toSecsSinceEpoch());
    });

    QCOMPARE_LT(lowerBound, upperBound);
    QCOMPARE_LE(lowerBound, dt1);
    QCOMPARE_LT(dt1, upperBound);
    QCOMPARE_LE(lowerBound, dt2);
    QCOMPARE_LT(dt2, upperBound);
    QCOMPARE_LE(lowerBound, dt3);
    QCOMPARE_LT(dt3, upperBound);
    reporter.dismiss();

    QCOMPARE(dt1.timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt2.timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt3.timeSpec(), BobUI::UTC);
}

void tst_QDateTime::currentDateTimeUtc()
{
    time_t buf1, buf2;
    ::time(&buf1);

    QDateTime lowerBound;
    lowerBound.setSecsSinceEpoch(buf1);

    QDateTime dt1 = QDateTime::currentDateTimeUtc();
    QDateTime dt2 = QDateTime::currentDateTimeUtc().toLocalTime();
    QDateTime dt3 = QDateTime::currentDateTimeUtc().toUTC();

    ::time(&buf2);

    QDateTime upperBound;
    upperBound.setSecsSinceEpoch(buf2);
    // Note we must add 2 seconds here because time() may return up to
    // 1 second difference from the more accurate method used by QDateTime::currentDateTime()
    upperBound = upperBound.addSecs(2);

    auto reporter = qScopeGuard([=]() {
        qInfo("\n"
              "lowerBound: %lld\n"
              "dt1:        %lld\n"
              "dt2:        %lld\n"
              "dt3:        %lld\n"
              "upperBound: %lld\n",
              lowerBound.toSecsSinceEpoch(),
              dt1.toSecsSinceEpoch(),
              dt2.toSecsSinceEpoch(),
              dt3.toSecsSinceEpoch(),
              upperBound.toSecsSinceEpoch());
    });

    QCOMPARE_LT(lowerBound, upperBound);
    QCOMPARE_LE(lowerBound, dt1);
    QCOMPARE_LT(dt1, upperBound);
    QCOMPARE_LE(lowerBound, dt2);
    QCOMPARE_LT(dt2, upperBound);
    QCOMPARE_LE(lowerBound, dt3);
    QCOMPARE_LT(dt3, upperBound);
    reporter.dismiss();

    QCOMPARE(dt1.timeSpec(), BobUI::UTC);
    QCOMPARE(dt2.timeSpec(), BobUI::LocalTime);
    QCOMPARE(dt3.timeSpec(), BobUI::UTC);
}

void tst_QDateTime::currentDateTimeUtc2()
{
    QDateTime local, utc;
    qint64 msec;

    // check that we got all down to the same milliseconds
    int i = 20;
    bool ok = false;
    do {
        local = QDateTime::currentDateTime();
        utc = QDateTime::currentDateTimeUtc();
        msec = QDateTime::currentMSecsSinceEpoch();
        ok = local.time().msec() == utc.time().msec()
            && utc.time().msec() == (msec % 1000);
    } while (--i && !ok);

    if (!i)
        QSKIP("Failed to get the dates within 1 ms of each other");

    // seconds and milliseconds should be the same:
    QCOMPARE(utc.time().second(), local.time().second());
    QCOMPARE(utc.time().msec(), local.time().msec());
    QCOMPARE(msec % 1000, qint64(local.time().msec()));
    QCOMPARE(msec / 1000 % 60, qint64(local.time().second()));

    // the two dates should be equal, actually
    QCOMPARE(local.toUTC(), utc);
    QCOMPARE(utc.toLocalTime(), local);

    // and finally, the SecsSinceEpoch should equal our number
    QCOMPARE(qint64(utc.toSecsSinceEpoch()), msec / 1000);
    QCOMPARE(qint64(local.toSecsSinceEpoch()), msec / 1000);
    QCOMPARE(utc.toMSecsSinceEpoch(), msec);
    QCOMPARE(local.toMSecsSinceEpoch(), msec);
}

void tst_QDateTime::toSecsSinceEpoch_data()
{
    BOBUIest::addColumn<QDate>("date");

    BOBUIest::newRow("start-1800") << QDate(1800, 1, 1);
    BOBUIest::newRow("start-1969") << QDate(1969, 1, 1);
    BOBUIest::newRow("start-2002") << QDate(2002, 1, 1);
    BOBUIest::newRow("mid-2002") << QDate(2002, 6, 1);
    BOBUIest::newRow("start-2038") << QDate(2038, 1, 1);
    BOBUIest::newRow("star-trek-1st-contact") << QDate(2063, 4, 5);
    BOBUIest::newRow("start-2107") << QDate(2107, 1, 1);
}

void tst_QDateTime::toSecsSinceEpoch()
{
    const BOBUIime noon(12, 0);
    QFETCH(const QDate, date);
    const QDateTime dateTime(date, noon);
    QVERIFY(dateTime.isValid());

    const qint64 asSecsSinceEpoch = dateTime.toSecsSinceEpoch();
    QCOMPARE(asSecsSinceEpoch, dateTime.toMSecsSinceEpoch() / 1000);
    QCOMPARE(QDateTime::fromSecsSinceEpoch(asSecsSinceEpoch), dateTime);
}

void tst_QDateTime::daylightSavingsTimeChange_data()
{
    BOBUIest::addColumn<QDate>("inDST");
    BOBUIest::addColumn<QDate>("outDST");
    BOBUIest::addColumn<int>("days"); // from in to out; -ve if reversed
    BOBUIest::addColumn<int>("months");

    BOBUIest::newRow("Autumn") << QDate(2006, 8, 1) << QDate(2006, 12, 1)
                            << 122 << 4;

    BOBUIest::newRow("Spring") << QDate(2006, 5, 1) << QDate(2006, 2, 1)
                            << -89 << -3;
}

void tst_QDateTime::daylightSavingsTimeChange()
{
    // This has grown from a regression test for an old bug where starting with
    // a date in DST and then moving to a date outside it (or vice-versa) caused
    // 1-hour jumps in time when addSecs() was called.
    //
    // The bug was caused by QDateTime knowing more than it lets show.
    // Internally, if it knows, QDateTime stores a flag indicating if the time is
    // DST or not. If it doesn't, it sets to "LocalUnknown".  The problem happened
    // because some functions did not reset the flag when moving in or out of DST.

    // WARNING: This only tests anything if there's a Daylight Savings Time change
    // in the current time-zone between inDST and outDST.
    // This is true for Central European Time and may be elsewhere.

    QFETCH(QDate, inDST);
    QFETCH(QDate, outDST);
    QFETCH(int, days);
    QFETCH(int, months);

    // First with simple construction
    QDateTime dt = outDST.startOfDay();
    int outDSTsecs = dt.toSecsSinceEpoch();

    dt.setDate(inDST);
    dt = dt.addSecs(1);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(0, 0, 1)));

    // now using addDays:
    dt = dt.addDays(days).addSecs(1);
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(0, 0, 2)));

    // ... and back again:
    dt = dt.addDays(-days).addSecs(1);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(0, 0, 3)));

    // now using addMonths:
    dt = dt.addMonths(months).addSecs(1);
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(0, 0, 4)));

    // ... and back again:
    dt = dt.addMonths(-months).addSecs(1);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(0, 0, 5)));

    // now using fromSecsSinceEpoch
    dt = QDateTime::fromSecsSinceEpoch(outDSTsecs);
    QCOMPARE(dt, outDST.startOfDay());

    dt.setDate(inDST);
    dt = dt.addSecs(60);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(0, 1)));

    // using addMonths:
    dt = dt.addMonths(months).addSecs(60);
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(0, 2)));
    // back again:
    dt = dt.addMonths(-months).addSecs(60);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(0, 3)));

    // using addDays:
    dt = dt.addDays(days).addSecs(60);
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(0, 4)));
    // back again:
    dt = dt.addDays(-days).addSecs(60);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(0, 5)));

    // Now use the result of a UTC -> LocalTime conversion
    dt = outDST.startOfDay().toUTC();
    dt = QDateTime(dt.date(), dt.time(), UTC).toLocalTime();
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(0, 0)));

    // using addDays:
    dt = dt.addDays(-days).addSecs(3600);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(1, 0)));
    // back again
    dt = dt.addDays(days).addSecs(3600);
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(2, 0)));

    // using addMonths:
    dt = dt.addMonths(-months).addSecs(3600);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(3, 0)));
    // back again:
    dt = dt.addMonths(months).addSecs(3600);
    QCOMPARE(dt, QDateTime(outDST, BOBUIime(4, 0)));

    // using setDate:
    dt.setDate(inDST);
    dt = dt.addSecs(3600);
    QCOMPARE(dt, QDateTime(inDST, BOBUIime(5, 0)));
}

void tst_QDateTime::springForward_data()
{
    BOBUIest::addColumn<BOBUIimeZone>("zone");
    BOBUIest::addColumn<QDate>("day"); // day of DST transition
    BOBUIest::addColumn<BOBUIime>("time"); // in the "missing hour"
    BOBUIest::addColumn<int>("step"); // days to step; +ve from before, -ve from after
    BOBUIest::addColumn<int>("adjust"); // minutes ahead of UTC on day stepped from

    /*
      Zone tests compare a summer and winter moment's SecsSinceEpoch to known values.
      This could in principle be flawed (two DST-using zones in the same
      hemisphere with the same DST and standard times but different transition
      times) but no actual example is known where this is a problem.  Please
      document any such conflicts, if discovered.

      See http://www.timeanddate.com/time/zones/ for data on more candidates to
      test. Note, however, that the IANA DB disagrees with it for some zones,
      and is authoritative.
    */

    const BOBUIimeZone local(BOBUIimeZone::LocalTime);
    const uint winter = QDate(2015, 1, 1).startOfDay(local).toSecsSinceEpoch();
    const uint summer = QDate(2015, 7, 1).startOfDay(local).toSecsSinceEpoch();

    if (winter == 1420066800 && summer == 1435701600) {
        BOBUIest::newRow("Local (CET) from day before")
            << local << QDate(2015, 3, 29) << BOBUIime(2, 30) << 1 << 60;
        BOBUIest::newRow("Local (CET) from day after")
            << local << QDate(2015, 3, 29) << BOBUIime(2, 30) << -1 << 120;
    } else if (winter == 1420063200 && summer == 1435698000) {
        // EET: but there's some variation in the date and time.
        // Asia/{Amman,Beirut,Gaza,Hebron}, Europe/Chisinau and Israel: at start of
        QDate date(2015, 3, 29); // Sunday by default.
        BOBUIime time(0, 30);
        if (auto thursday = QDate(2015, 3, 26); thursday.startOfDay(local).time() > time) {
            // Asia/Damascus: start of March 26th.
            date = thursday;
        } else if (auto friday = QDate(2015, 3, 27); friday.startOfDay(local).time() > time) {
            // Israel, Asia/{Jerusalem,Tel_Aviv}: start of March 27th (IANA DB).
            date = friday;
        } else if (friday.startOfDay(local).addSecs(2 * 60 * 60).time() == BOBUIime(3, 0)) {
            // Israel, Asia/{Jerusalem,Tel_Aviv} according to glibc at 02:00 on March 27th.
            date = friday;
            time = BOBUIime(2, 30);
        } else if (date.startOfDay(local).time() < time) {
            // Most of Europeean EET, e.g. Finland.
            time = BOBUIime(3, 30);
        }
        BOBUIest::newRow("Local (EET) from day before")
            << local << date << time << 1 << 120;
        BOBUIest::newRow("Local (EET) from day after")
            << local << date << time << -1 << 180;
    } else if (winter == 1420070400 && summer == 1435705200) {
        // Western European Time, WET/WEST; a.k.a. GMT/BST
        BOBUIest::newRow("Local (WET) from day before")
            << local << QDate(2015, 3, 29) << BOBUIime(1, 30) << 1 << 0;
        BOBUIest::newRow("Local (WET) from day after")
            << local << QDate(2015, 3, 29) << BOBUIime(1, 30) << -1 << 60;
    } else if (winter == 1420099200 && summer == 1435734000) {
        // Western USA, Canada: Pacific Time (e.g. US/Pacific)
        QDate date(2015, 3, 8);
        // America/Ensenada did its transition on April 5th, like the rest of Mexico.
        if (QDate(2015, 4, 1).startOfDay().toSecsSinceEpoch() == 1427875200)
            date = QDate(2015, 4, 5);
        BOBUIest::newRow("Local (PT) from day before")
            << local << date << BOBUIime(2, 30) << 1 << -480;
        BOBUIest::newRow("Local (PT) from day after")
            << local << date << BOBUIime(2, 30) << -1 << -420;
    } else if (winter == 1420088400 && summer == 1435723200) {
        // Eastern USA, Canada: Eastern Time (e.g. US/Eastern)
        // Havana matches offset and date, but at midnight.
        const BOBUIime start = QDate(2015, 3, 8).startOfDay(local).time();
        const BOBUIime when = start == BOBUIime(0, 0) ? BOBUIime(2, 30) : BOBUIime(0, 30);
        BOBUIest::newRow("Local(ET) from day before")
            << local << QDate(2015, 3, 8) << when << 1 << -300;
        BOBUIest::newRow("Local(ET) from day after")
            << local << QDate(2015, 3, 8) << when << -1 << -240;
#if !BOBUI_CONFIG(timezone)
    } else {
        // Includes the numbers you need to test for your zone, as above:
        QString msg("No spring forward test data for this TZ (%1, %2)"_L1.arg(
                        QString::number(winter), QString::number(summer)));
        QSKIP(qPrintable(msg));
#endif
    }
#if BOBUI_CONFIG(timezone)
    if (const BOBUIimeZone cet("Europe/Oslo"); cet.isValid()) {
        BOBUIest::newRow("CET from day before")
            << cet << QDate(2015, 3, 29) << BOBUIime(2, 30) << 1 << 60;
        BOBUIest::newRow("CET from day after")
            << cet << QDate(2015, 3, 29) << BOBUIime(2, 30) << -1 << 120;
    }
    if (const BOBUIimeZone eet("Europe/Helsinki"); eet.isValid()) {
        BOBUIest::newRow("EET from day before")
            << eet << QDate(2015, 3, 29) << BOBUIime(3, 30) << 1 << 120;
        BOBUIest::newRow("EET from day after")
            << eet << QDate(2015, 3, 29) << BOBUIime(3, 30) << -1 << 180;
    }
    if (const BOBUIimeZone wet("Europe/Lisbon"); wet.isValid()) {
        BOBUIest::newRow("WET from day before")
            << wet << QDate(2015, 3, 29) << BOBUIime(1, 30) << 1 << 0;
        BOBUIest::newRow("WET from day after")
            << wet << QDate(2015, 3, 29) << BOBUIime(1, 30) << -1 << 60;
    }
    if (const BOBUIimeZone pacific("America/Vancouver"); pacific.isValid()) {
        BOBUIest::newRow("PT from day before")
            << pacific << QDate(2015, 3, 8) << BOBUIime(2, 30) << 1 << -480;
        BOBUIest::newRow("PT from day after")
            << pacific << QDate(2015, 3, 8) << BOBUIime(2, 30) << -1 << -420;
    }
    if (const BOBUIimeZone eastern("America/Toronto"); eastern.isValid()) {
        BOBUIest::newRow("ET from day before")
            << eastern << QDate(2015, 3, 8) << BOBUIime(2, 30) << 1 << -300;
        BOBUIest::newRow("ET from day after")
            << eastern << QDate(2015, 3, 8) << BOBUIime(2, 30) << -1 << -240;
    }
#endif
}

void tst_QDateTime::springForward()
{
    QFETCH(BOBUIimeZone, zone);
    QFETCH(QDate, day);
    QFETCH(BOBUIime, time);
    QFETCH(int, step);
    QFETCH(int, adjust);

    QDateTime direct = QDateTime(day.addDays(-step), time, zone).addDays(step);
    QVERIFY(direct.isValid());
    QCOMPARE(direct.date(), day);
    QCOMPARE(direct.time().minute(), time.minute());
    QCOMPARE(direct.time().second(), time.second());
    const int off = step < 0 ? -1 : 1;
    QCOMPARE(direct.time().hour() - time.hour(), off);
    // adjust is the offset on the other side of the gap:
    QCOMPARE(direct.offsetFromUtc(), (adjust + off * 60) * 60);

    // Repeat, but getting there via .toTimeZone(). Apply adjust to datetime,
    // not time, as the time wraps round if the adjustment crosses midnight.
    QDateTime detour = QDateTime(day.addDays(-step), time,
                                 UTC).addSecs(-60 * adjust).toTimeZone(zone);
    QCOMPARE(detour.time(), time);
    detour = detour.addDays(step);
    // Insist on consistency:
    QCOMPARE(detour, direct);
    QCOMPARE(detour.offsetFromUtc(), direct.offsetFromUtc());
}

void tst_QDateTime::operator_eqeq_data()
{
    BOBUIest::addColumn<QDateTime>("dt1");
    BOBUIest::addColumn<QDateTime>("dt2");
    BOBUIest::addColumn<bool>("expectEqual");
    BOBUIest::addColumn<bool>("checkEuro");

    QDateTime dateTime1(QDate(2012, 6, 20), BOBUIime(14, 33, 2, 500));
    QDateTime dateTime1a = dateTime1.addMSecs(1);
    QDateTime dateTime2(QDate(2012, 20, 6), BOBUIime(14, 33, 2, 500)); // Invalid
    QDateTime dateTime2a = dateTime2.addMSecs(-1); // Still invalid
    QDateTime dateTime3(QDate(1970, 1, 1), BOBUIime(0, 0), UTC); // UTC epoch
    QDateTime dateTime3a = dateTime3.addDays(1);
    QDateTime dateTime3b = dateTime3.addDays(-1);
    // Ensure that different times may be equal when considering timezone.
    QDateTime dateTime3c(dateTime3.addSecs(3600));
    dateTime3c.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(3600));
    QDateTime dateTime3d(dateTime3.addSecs(-3600));
    dateTime3d.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-3600));
    QDateTime dateTime3e(dateTime3.date(), dateTime3.time()); // Local time's epoch

    BOBUIest::newRow("data0") << dateTime1 << dateTime1 << true << false;
    BOBUIest::newRow("data1") << dateTime2 << dateTime2 << true << false;
    BOBUIest::newRow("data2") << dateTime1a << dateTime1a << true << false;
    BOBUIest::newRow("data3") << dateTime1 << dateTime2 << false << false;
    BOBUIest::newRow("data4") << dateTime1 << dateTime1a << false << false;
    BOBUIest::newRow("data5") << dateTime2 << dateTime2a << true << false;
    BOBUIest::newRow("data6") << dateTime2 << dateTime3 << false << false;
    BOBUIest::newRow("data7") << dateTime3 << dateTime3a << false << false;
    BOBUIest::newRow("data8") << dateTime3 << dateTime3b << false << false;
    BOBUIest::newRow("data9") << dateTime3a << dateTime3b << false << false;
    BOBUIest::newRow("data10") << dateTime3 << dateTime3c << true << false;
    BOBUIest::newRow("data11") << dateTime3 << dateTime3d << true << false;
    BOBUIest::newRow("data12") << dateTime3c << dateTime3d << true << false;
    if (epochTimeType == LocalTimeIsUtc)
        BOBUIest::newRow("data13") << dateTime3 << dateTime3e << true << false;
    // ... but a zone (sometimes) ahead of or behind UTC (e.g. Europe/London)
    // might agree with UTC about the epoch, all the same.

    BOBUIest::newRow("invalid == invalid") << QDateTime() << QDateTime() << true << false;
    BOBUIest::newRow("invalid != valid #1") << QDateTime() << dateTime1 << false << false;

    if (zoneIsCET) {
        BOBUIest::newRow("data14")
            << QDateTime(QDate(2004, 1, 2), BOBUIime(2, 2, 3))
            << QDateTime(QDate(2004, 1, 2), BOBUIime(1, 2, 3), UTC) << true << true;
        BOBUIest::newRow("local-fall-back") // Sun, 31 Oct 2004, 02:30, both ways round:
            << QDateTime::fromMSecsSinceEpoch(Q_INT64_C(1099186200000))
            << QDateTime::fromMSecsSinceEpoch(Q_INT64_C(1099182600000))
            << false << false;
    }
#if BOBUI_CONFIG(timezone)
    const BOBUIimeZone CET("Europe/Oslo");
    if (CET.isValid()) {
        BOBUIest::newRow("CET-fall-back") // Sun, 31 Oct 2004, 02:30, both ways round:
            << QDateTime::fromMSecsSinceEpoch(Q_INT64_C(1099186200000), CET)
            << QDateTime::fromMSecsSinceEpoch(Q_INT64_C(1099182600000), CET)
            << false << false;
    }
#endif
}

void tst_QDateTime::operator_eqeq()
{
    QFETCH(QDateTime, dt1);
    QFETCH(QDateTime, dt2);
    QFETCH(bool, expectEqual);
    QFETCH(bool, checkEuro);

    BOBUI_TEST_EQUALITY_OPS(dt1, dt1, true);
    BOBUI_TEST_EQUALITY_OPS(dt2, dt2, true);
    BOBUI_TEST_EQUALITY_OPS(dt1, dt2, expectEqual);

    QCOMPARE_NE(dt1, QDateTime::currentDateTime());
    QCOMPARE_NE(dt2, QDateTime::currentDateTime());

    QVERIFY(dt1.toUTC() == dt1.toUTC());

    if (expectEqual)
        QVERIFY(qHash(dt1) == qHash(dt2));

    if (checkEuro && zoneIsCET) {
        QCOMPARE(dt1.toUTC(), dt2);
        QCOMPARE(dt1, dt2.toLocalTime());
    }
}

void tst_QDateTime::ordering_data()
{
    BOBUIest::addColumn<QDateTime>("left");
    BOBUIest::addColumn<QDateTime>("right");
    BOBUIest::addColumn<BobUI::weak_ordering>("expectedOrdering");

    Q_CONSTINIT static const auto constructName = [](const QDateTime &dt) -> QByteArray {
        if (dt.isNull())
            return "null";
        if (!dt.isValid())
            return "invalid";
        return dt.toString(BobUI::ISODateWithMs).toLatin1();
    };

    Q_CONSTINIT static const auto generateRow =
            [](const QDateTime &left, const QDateTime &right, BobUI::weak_ordering ordering) {
        const QByteArray leftStr = constructName(left);
        const QByteArray rightStr = constructName(right);
        BOBUIest::addRow("%s_vs_%s", leftStr.constData(), rightStr.constData())
                << left << right << ordering;
    };

    QDateTime june(QDate(2012, 6, 20), BOBUIime(14, 33, 2, 500));
    QDateTime juneLater = june.addMSecs(1);
    QDateTime badDay(QDate(2012, 20, 6), BOBUIime(14, 33, 2, 500)); // Invalid
    QDateTime epoch(QDate(1970, 1, 1), BOBUIime(0, 0), UTC); // UTC epoch
    QDateTime nextDay = epoch.addDays(1);
    QDateTime prevDay = epoch.addDays(-1);
    // Ensure that different times may be equal when considering timezone.
    QDateTime epochEast1h(epoch.addSecs(3600));
    epochEast1h.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(3600));
    QDateTime epochWest1h(epoch.addSecs(-3600));
    epochWest1h.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-3600));
    QDateTime local1970(epoch.date(), epoch.time()); // Local time's epoch

    generateRow(june, june, BobUI::weak_ordering::equivalent);
    generateRow(june, juneLater, BobUI::weak_ordering::less);
    generateRow(june, badDay, BobUI::weak_ordering::greater);
    generateRow(badDay, QDateTime(), BobUI::weak_ordering::equivalent);
    generateRow(june, QDateTime(), BobUI::weak_ordering::greater);
    generateRow(epoch, nextDay, BobUI::weak_ordering::less);
    generateRow(epoch, prevDay, BobUI::weak_ordering::greater);
    generateRow(epoch, epochEast1h, BobUI::weak_ordering::equivalent);
    generateRow(epoch, epochWest1h, BobUI::weak_ordering::equivalent);
    generateRow(epochEast1h, epochWest1h, BobUI::weak_ordering::equivalent);
    if (epochTimeType == LocalTimeIsUtc)
        generateRow(epoch, local1970, BobUI::weak_ordering::equivalent);

#if BOBUI_CONFIG(timezone)
    // See bobuiimezone.h's comments on M(ax|in)UtcOffsetSecs:
    BOBUIimeZone alaska("America/Metlakatla"), phillip("Asia/Manila");
    if (alaska.isValid() && phillip.isValid()) {
        // Date Narciso Claveria ordered Manila's transition:
        QDateTime edict(QDate(1844, 8, 16), BOBUIime(8, 4), phillip); // GMT start of next day
        // Backends may lack relevant data, so check:
        const int alaskaOffset = edict.toTimeZone(alaska).offsetFromUtc();
        const int manilaOffset = edict.toTimeZone(phillip).offsetFromUtc();
        if (manilaOffset < 0 && alaskaOffset > 0) {
            qint64 offsetGap = alaskaOffset - manilaOffset; // 31h 9m 42s
            QDateTime equiv = edict.toTimeZone(alaska); // 15:13:42, on the next day.
            BOBUIest::newRow("extreme.equivalent")
                << edict << equiv << BobUI::weak_ordering::equivalent;
            // Despite internal msecs values; edict's is offsetGap * 1000 less than equiv's.
            // Even the least increase in that gap does imply less:
            BOBUIest::newRow("extreme.less")
                << edict << equiv.addMSecs(1) << BobUI::weak_ordering::less;
            // Until offsetGap seconds later, edict's msecs doesn't catch up with equiv's:
            BOBUIest::newRow("extreme.greater")
                << edict.addSecs(offsetGap - 1) << equiv << BobUI::weak_ordering::greater;
        }
    }
#endif
}

void tst_QDateTime::ordering()
{
    QFETCH(QDateTime, left);
    QFETCH(QDateTime, right);
    QFETCH(BobUI::weak_ordering, expectedOrdering);

    BOBUI_TEST_ALL_COMPARISON_OPS(left, right, expectedOrdering);
}

Q_DECLARE_METATYPE(QDataStream::Version)

void tst_QDateTime::operator_insert_extract_data()
{
    BOBUIest::addColumn<int>("yearNumber");
    BOBUIest::addColumn<QByteArray>("serialiseAs");
    BOBUIest::addColumn<QByteArray>("deserialiseAs");
    BOBUIest::addColumn<QDataStream::Version>("dataStreamVersion");

    const QByteArray westernAustralia("AWST-8AWDT-9,M10.5.0,M3.5.0/03:00:00");
    const QByteArray hawaii("HAW10");

    const QMetaEnum e = QMetaEnum::fromType<QDataStream::Version>();
    for (int version = QDataStream::BobUI_1_0; version <= QDataStream::BobUI_DefaultCompiledVersion;
         ++version) {
        if (e.value(version) == -1 || qstrcmp(e.key(version), "BobUI_DefaultCompiledVersion") == 0)
            continue;
        const auto dataStreamVersion = static_cast<QDataStream::Version>(version);
        const char *const tag = e.key(version);
        BOBUIest::addRow("%s WA => HAWAII %d", tag, 2012)
            << 2012 << westernAustralia << hawaii << dataStreamVersion;
        BOBUIest::addRow("%s WA => WA %d", tag, 2012)
            << 2012 << westernAustralia << westernAustralia << dataStreamVersion;
        BOBUIest::addRow("%s HAWAII => WA %d", tag, -2012)
            << -2012 << hawaii << westernAustralia << dataStreamVersion;
        BOBUIest::addRow("%s HAWAII => HAWAII %d", tag, 2012)
            << 2012 << hawaii << hawaii << dataStreamVersion;
    }
}

void tst_QDateTime::operator_insert_extract()
{
    QFETCH(int, yearNumber);
    QFETCH(QByteArray, serialiseAs);
    QFETCH(QByteArray, deserialiseAs);
    QFETCH(QDataStream::Version, dataStreamVersion);

    // Start off in a certain timezone.
    TimeZoneRollback useZone(serialiseAs);

    // It is important that dateTime is created after the time zone shift
    QDateTime dateTime(QDate(yearNumber, 8, 14), BOBUIime(8, 0));
    QDateTime dateTimeAsUTC(dateTime.toUTC());

    QByteArray byteArray;
    {
        QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
        dataStream.setVersion(dataStreamVersion);
        if (dataStreamVersion == QDataStream::BobUI_5_0) {
            // BobUI 5 serialises as UTC and converts back to the stored timeSpec when
            // deserialising; we don't need to do it ourselves...
            dataStream << dateTime << dateTime;
        } else {
            // ... but other versions don't, so we have to here.
            dataStream << dateTimeAsUTC << dateTimeAsUTC;
            // We'll also make sure that a deserialised local datetime is the same
            // time of day (potentially different UTC time), regardless of which
            // timezone it was serialised in. E.g.: Tue Aug 14 08:00:00 2012
            // serialised in WST should be deserialised as Tue Aug 14 08:00:00 2012
            // HST.
            dataStream << dateTime;
        }
    }

    // Ensure that a change in timezone between serialisation and deserialisation
    // still results in identical UTC-converted datetimes.
    useZone.reset(deserialiseAs);
    QDateTime expectedLocalTime(dateTimeAsUTC.toLocalTime()); // *After* resetting zone.
    QCOMPARE(expectedLocalTime, dateTimeAsUTC); // Different description, same moment in time.
    {
        // Deserialise whole QDateTime at once.
        QDataStream dataStream(&byteArray, QIODevice::ReadOnly);
        dataStream.setVersion(dataStreamVersion);
        QDateTime deserialised;
        dataStream >> deserialised;

        if (dataStreamVersion == QDataStream::BobUI_5_0) {
            // Ensure local time is still correct. Again, BobUI 5 handles the timeSpec
            // conversion (in this case, UTC => LocalTime) for us when deserialising.
            QCOMPARE(deserialised, expectedLocalTime);
        } else {
            if (dataStreamVersion < QDataStream::BobUI_4_0) {
                // Versions lower than BobUI 4 don't serialise the timeSpec, instead
                // assuming that everything is LocalTime.
                deserialised.setTimeZone(UTC);
            }
            // BobUI 4.* versions do serialise the timeSpec, so we only need to convert from UTC here.
            deserialised = deserialised.toLocalTime();

            QCOMPARE(deserialised, expectedLocalTime);
        }
        // Sanity check UTC times (operator== already converts its operands to UTC before comparing).
        QCOMPARE(deserialised.toUTC(), expectedLocalTime.toUTC());

        // Deserialise each component individually.
        QDate deserialisedDate;
        dataStream >> deserialisedDate;
        BOBUIime deserialisedTime;
        dataStream >> deserialisedTime;
        qint8 deserialisedSpec;
        if (dataStreamVersion >= QDataStream::BobUI_4_0)
            dataStream >> deserialisedSpec;
        deserialised = QDateTime(deserialisedDate, deserialisedTime, UTC);
        QCOMPARE(deserialised.toLocalTime(), deserialised);
        const auto isLocalTime = [](qint8 spec) -> bool {
            // The spec is in fact a QDateTimePrivate::Spec, not BobUI::TimeSpec;
            // and no offset or zone is stored, so only UTC and LocalTime are
            // really supported. Fortunately this test only uses those.
            const auto decoded = static_cast<QDateTimePrivate::Spec>(spec);
            return decoded != QDateTimePrivate::UTC && decoded != QDateTimePrivate::OffsetFromUTC;
        };
        if (dataStreamVersion >= QDataStream::BobUI_4_0 && isLocalTime(deserialisedSpec))
            deserialised = deserialised.toTimeZone(BOBUIimeZone::LocalTime);
        // Ensure local time is still correct.
        QCOMPARE(deserialised, expectedLocalTime);
        // Sanity check UTC times.
        QCOMPARE(deserialised.toUTC(), expectedLocalTime.toUTC());

        if (dataStreamVersion != QDataStream::BobUI_5_0) {
            // Deserialised local datetime should be the same time of day,
            // regardless of which timezone it was serialised in.
            QDateTime localDeserialized;
            dataStream >> localDeserialized;
            QCOMPARE(localDeserialized, dateTime);
        }
    }
}

#if BOBUI_CONFIG(datestring)
void tst_QDateTime::fromStringDateFormat_data()
{
    BOBUIest::addColumn<QString>("dateTimeStr");
    BOBUIest::addColumn<BobUI::DateFormat>("dateFormat");
    BOBUIest::addColumn<QDateTime>("expected");

    // Fails 1970 start dates in western Mexico
    // due to changing from PST to MST at the start of 1970.
    const bool goodEpochStart = QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0)).isValid();

    // Test BobUI::TextDate format.
    BOBUIest::newRow("text date") << u"Tue Jun 17 08:00:10 2003"_s
        << BobUI::TextDate << QDateTime(QDate(2003, 6, 17), BOBUIime(8, 0, 10));
    BOBUIest::newRow("text date Year 0999") << u"Tue Jun 17 08:00:10 0999"_s
        << BobUI::TextDate << QDateTime(QDate(999, 6, 17), BOBUIime(8, 0, 10));
    BOBUIest::newRow("text date Year 999") << u"Tue Jun 17 08:00:10 999"_s
        << BobUI::TextDate << QDateTime(QDate(999, 6, 17), BOBUIime(8, 0, 10));
    BOBUIest::newRow("text date Year 12345") << u"Tue Jun 17 08:00:10 12345"_s
        << BobUI::TextDate << QDateTime(QDate(12345, 6, 17), BOBUIime(8, 0, 10));
    BOBUIest::newRow("text date Year -4712") << u"Tue Jan 1 00:01:02 -4712"_s
        << BobUI::TextDate << QDateTime(QDate(-4712, 1, 1), BOBUIime(0, 1, 2));
    BOBUIest::newRow("text data1") << u"Thu Jan 2 12:34 1970"_s
        << BobUI::TextDate << QDateTime(QDate(1970, 1, 2), BOBUIime(12, 34));
    if (goodEpochStart) {
        BOBUIest::newRow("text epoch year after time")
            << u"Thu Jan 1 00:00:00 1970"_s << BobUI::TextDate
            << QDate(1970, 1, 1).startOfDay();
        BOBUIest::newRow("text epoch spaced")
            << u" Thu   Jan   1    00:00:00    1970  "_s
            << BobUI::TextDate << QDate(1970, 1, 1).startOfDay();
        BOBUIest::newRow("text epoch time after year")
            << u"Thu Jan 1 1970 00:00:00"_s
            << BobUI::TextDate << QDate(1970, 1, 1).startOfDay();
    }
    BOBUIest::newRow("text epoch terse")
        << u"Thu Jan 1 00 1970"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text epoch stray :00")
        << u"Thu Jan 1 00:00:00:00 1970"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text data6") << u"Thu Jan 1 00:00:00"_s
        << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text bad offset") << u"Thu Jan 1 00:12:34 1970 UTC+foo"_s
        << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text UTC early") << u"Thu Jan 1 00:12:34 1970 UTC"_s
        << BobUI::TextDate << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    BOBUIest::newRow("text UTC-3 early") << u"Thu Jan 1 00:12:34 1970 UTC-0300"_s
        << BobUI::TextDate << QDateTime(QDate(1970, 1, 1), BOBUIime(3, 12, 34), UTC);
    BOBUIest::newRow("text UTC+3 early") << u"Thu Jan 1 00:12:34 1970 UTC+0300"_s
        << BobUI::TextDate << QDateTime(QDate(1969, 12, 31), BOBUIime(21, 12, 34), UTC);
    BOBUIest::newRow("text UTC+1 early") << u"Thu Jan 1 1970 00:12:34 UTC+0100"_s
        << BobUI::TextDate << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 12, 34), UTC);
    // We produce use GMT as prefix, so need to parse it:
    BOBUIest::newRow("text GMT early")
        << u"Thu Jan 1 00:12:34 1970 GMT"_s << BobUI::TextDate
        << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    BOBUIest::newRow("text GMT+3 early")
        << u"Thu Jan 1 00:12:34 1970 GMT+0300"_s << BobUI::TextDate
        << QDateTime(QDate(1969, 12, 31), BOBUIime(21, 12, 34), UTC);
    // ... and we match (only) it case-insensitively:
    BOBUIest::newRow("text gmt early")
        << u"Thu Jan 1 00:12:34 1970 gmt"_s << BobUI::TextDate
        << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);

    BOBUIest::newRow("text empty") << u""_s
        << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text too many parts") << u"Thu Jan 1 00:12:34 1970 UTC +0100"_s
        << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid month name") << u"Thu Jaz 1 1970 00:12:34"_s
        << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid date") << u"Thu Jan 32 1970 00:12:34"_s
        << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text pre-5.2 MS-Win format") // Support dropped in 6.2
        << u"Thu 1. Jan 00:00:00 1970"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid day")
        << u"Thu Jan XX 1970 00:12:34"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text misplaced day")
        << u"Thu 1 Jan 00:00:00 1970"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid year end")
        << u"Thu Jan 1 00:00:00 19X0"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid year early")
        << u"Thu Jan 1 19X0 00:00:00"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid hour")
        << u"Thu Jan 1 1970 0X:00:00"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid minute")
        << u"Thu Jan 1 1970 00:0X:00"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text invalid second")
        << u"Thu Jan 1 1970 00:00:0X"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text non-UTC offset")
        << u"Thu Jan 1 1970 00:00:00 DMT"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text bad UTC offset")
        << u"Thu Jan 1 1970 00:00:00 UTCx0200"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text bad UTC hour")
        << u"Thu Jan 1 1970 00:00:00 UTC+0X00"_s << BobUI::TextDate << QDateTime();
    BOBUIest::newRow("text bad UTC minute")
        << u"Thu Jan 1 1970 00:00:00 UTC+000X"_s << BobUI::TextDate << QDateTime();

    BOBUIest::newRow("text second fraction")
        << u"Mon May 6 2013 01:02:03.456"_s
        << BobUI::TextDate << QDateTime(QDate(2013, 5, 6), BOBUIime(1, 2, 3, 456));
    BOBUIest::newRow("text max milli")
        << u"Mon May 6 2013 01:02:03.999499999"_s
        << BobUI::TextDate << QDateTime(QDate(2013, 5, 6), BOBUIime(1, 2, 3, 999));
    BOBUIest::newRow("text milli wrap")
        << u"Mon May 6 2013 01:02:03.9995"_s
        << BobUI::TextDate << QDateTime(QDate(2013, 5, 6), BOBUIime(1, 2, 4));
    BOBUIest::newRow("text last milli") // Special case, don't round up to invalid:
        << u"Mon May 6 2013 23:59:59.9999999999"_s
        << BobUI::TextDate << QDateTime(QDate(2013, 5, 6), BOBUIime(23, 59, 59, 999));
    BOBUIest::newRow("text Sunday lunch")
        << u"Sun Dec 1 13:02:00 1974"_s << BobUI::TextDate
        << QDateTime(QDate(1974, 12, 1), BOBUIime(13, 2));

    // Test BobUI::ISODate format.
    BOBUIest::newRow("trailing space") // BOBUIBUG-80445
        << u"2000-01-02 03:04:05.678 "_s << BobUI::ISODate << QDateTime();

    // Invalid spaces (but keeping field widths correct):
    BOBUIest::newRow("space before millis")
        << u"2000-01-02 03:04:05. 678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space after seconds")
        << u"2000-01-02 03:04:5 .678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space before seconds")
        << u"2000-01-02 03:04: 5.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space after minutes")
        << u"2000-01-02 03:4 :05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space before minutes")
        << u"2000-01-02 03: 4:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space after hour")
        << u"2000-01-02 3 :04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space before hour")
        << u"2000-01-02  3:04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space after day")
        << u"2000-01-2  03:04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space before day")
        << u"2000-01- 2 03:04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space after month")
        << u"2000-1 -02 03:04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space before month")
        << u"2000- 1-02 03:04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("space after year")
        << u"200 -01-02 03:04:05.678"_s << BobUI::ISODate << QDateTime();

    // Spaces as separators:
    BOBUIest::newRow("sec-milli space")
        << u"2000-01-02 03:04:05 678"_s << BobUI::ISODate
        << QDateTime();
    BOBUIest::newRow("min-sec space")
        << u"2000-01-02 03:04 05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("hour-min space")
        << u"2000-01-02 03 04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("mon-day space")
        << u"2000-01 02 03:04:05.678"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("year-mon space")
        << u"2000 01-02 03:04:05.678"_s << BobUI::ISODate << QDateTime();

    // Normal usage:
    BOBUIest::newRow("ISO +01:00") << u"1987-02-13T13:24:51+01:00"_s
        << BobUI::ISODate << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("ISO +00:01") << u"1987-02-13T13:24:51+00:01"_s
        << BobUI::ISODate << QDateTime(QDate(1987, 2, 13), BOBUIime(13, 23, 51), UTC);
    BOBUIest::newRow("ISO -01:00") << u"1987-02-13T13:24:51-01:00"_s
        << BobUI::ISODate << QDateTime(QDate(1987, 2, 13), BOBUIime(14, 24, 51), UTC);
    BOBUIest::newRow("ISO -00:01") << u"1987-02-13T13:24:51-00:01"_s
        << BobUI::ISODate << QDateTime(QDate(1987, 2, 13), BOBUIime(13, 25, 51), UTC);
    BOBUIest::newRow("ISO +0000") << u"1970-01-01T00:12:34+0000"_s
        << BobUI::ISODate << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    BOBUIest::newRow("ISO +00:00") << u"1970-01-01T00:12:34+00:00"_s
        << BobUI::ISODate << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    BOBUIest::newRow("ISO -03") << u"2014-12-15T12:37:09-03"_s
        << BobUI::ISODate << QDateTime(QDate(2014, 12, 15), BOBUIime(15, 37, 9), UTC);
    BOBUIest::newRow("ISO zzz-03") << u"2014-12-15T12:37:09.745-03"_s
        << BobUI::ISODate << QDateTime(QDate(2014, 12, 15), BOBUIime(15, 37, 9, 745), UTC);
    BOBUIest::newRow("ISO -3") << u"2014-12-15T12:37:09-3"_s
        << BobUI::ISODate << QDateTime(QDate(2014, 12, 15), BOBUIime(15, 37, 9), UTC);
    BOBUIest::newRow("ISO zzz-3") << u"2014-12-15T12:37:09.745-3"_s
        << BobUI::ISODate << QDateTime(QDate(2014, 12, 15), BOBUIime(15, 37, 9, 745), UTC);
    BOBUIest::newRow("ISO lower-case") << u"2005-06-28T07:57:30.002z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 2), UTC);
    // No time specified - defaults to BobUI::LocalTime.
    BOBUIest::newRow("ISO data3") << u"2002-10-01"_s
                               << BobUI::ISODate << QDate(2002, 10, 1).startOfDay();
    // Excess digits in milliseconds, round correctly:
    BOBUIest::newRow("ISO") << u"2005-06-28T07:57:30.0010000000Z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 1), UTC);
    BOBUIest::newRow("ISO rounding") << u"2005-06-28T07:57:30.0015Z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 2), UTC);
    // ... and accept comma as separator:
    BOBUIest::newRow("ISO with comma 1") << u"2005-06-28T07:57:30,0040000000Z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 4), UTC);
    BOBUIest::newRow("ISO with comma 2") << u"2005-06-28T07:57:30,0015Z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 2), UTC);
    BOBUIest::newRow("ISO with comma 3") << u"2005-06-28T07:57:30,0014Z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 1), UTC);
    BOBUIest::newRow("ISO with comma 4") << u"2005-06-28T07:57:30,1Z"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 100), UTC);
    BOBUIest::newRow("ISO with comma 5") << u"2005-06-28T07:57:30,11"_s
        << BobUI::ISODate << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 110));
    // 24:00:00 Should be next day according to ISO 8601 section 4.2.3.
    BOBUIest::newRow("ISO 24:00") << u"2012-06-04T24:00:00"_s
                               << BobUI::ISODate << QDate(2012, 6, 5).startOfDay();
#if BOBUI_CONFIG(timezone)
    const QByteArray sysId = BOBUIimeZone::systemTimeZoneId();
    const bool midnightSkip = sysId == "America/Sao_Paulo" || sysId == "America/Asuncion"
        || sysId == "America/Cordoba" || sysId == "America/Argentina/Cordoba"
        || sysId == "America/Campo_Grande"
        || sysId == "America/Cuiaba" || sysId == "America/Buenos_Aires"
        || sysId == "America/Argentina/Buenos_Aires"
        || sysId == "America/Argentina/Tucuman" || sysId == "Brazil/East";
    BOBUIest::newRow("ISO 24:00 in DST") // Midnight spring forward in some of South America.
        << u"2008-10-18T24:00"_s << BobUI::ISODate
        << QDateTime(QDate(2008, 10, 19), BOBUIime(midnightSkip ? 1 : 0, 0));
#endif
    BOBUIest::newRow("ISO 24:00 end of month")
        << u"2012-06-30T24:00:00"_s
        << BobUI::ISODate << QDate(2012, 7, 1).startOfDay();
    BOBUIest::newRow("ISO 24:00 end of year")
        << u"2012-12-31T24:00:00"_s
        << BobUI::ISODate << QDate(2013, 1, 1).startOfDay();
    BOBUIest::newRow("ISO 24:00, fract ms")
        << u"2012-01-01T24:00:00.000"_s
        << BobUI::ISODate << QDate(2012, 1, 2).startOfDay();
    BOBUIest::newRow("ISO 24:00 end of year, fract ms")
        << u"2012-12-31T24:00:00.000"_s
        << BobUI::ISODate << QDate(2013, 1, 1).startOfDay();
    // Test fractional seconds.
    BOBUIest::newRow("ISO .0 of a second (period)")
        << u"2012-01-01T08:00:00.0"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0));
    BOBUIest::newRow("ISO .00 of a second (period)") << u"2012-01-01T08:00:00.00"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0));
    BOBUIest::newRow("ISO .000 of a second (period)") << u"2012-01-01T08:00:00.000"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0));
    BOBUIest::newRow("ISO .1 of a second (comma)") << u"2012-01-01T08:00:00,1"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 0, 100));
    BOBUIest::newRow("ISO .99 of a second (comma)") << u"2012-01-01T08:00:00,99"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 0, 990));
    BOBUIest::newRow("ISO .998 of a second (comma)") << u"2012-01-01T08:00:00,998"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 0, 998));
    BOBUIest::newRow("ISO .999 of a second (comma)") << u"2012-01-01T08:00:00,999"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 0, 999));
    BOBUIest::newRow("ISO .3335 of a second (comma)") << u"2012-01-01T08:00:00,3335"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 0, 334));
    BOBUIest::newRow("ISO .333333 of a second (comma)") << u"2012-01-01T08:00:00,333333"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 0, 333));
    BOBUIest::newRow("ISO .00009 of a second (period)") << u"2012-01-01T08:00:00.00009"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0));
    BOBUIest::newRow("ISO second fraction") << u"2013-05-06T01:02:03.456"_s
        << BobUI::ISODate << QDateTime(QDate(2013, 5, 6), BOBUIime(1, 2, 3, 456));
    BOBUIest::newRow("ISO max milli")
        << u"2013-05-06T01:02:03.999499999"_s
        << BobUI::ISODate << QDateTime(QDate(2013, 5, 6), BOBUIime(1, 2, 3, 999));
    BOBUIest::newRow("ISO milli wrap")
        << u"2013-05-06T01:02:03.9995"_s
        << BobUI::ISODate << QDateTime(QDate(2013, 5, 6), BOBUIime(1, 2, 4));
    BOBUIest::newRow("ISO last milli") // Does round up and overflow into new day:
        << u"2013-05-06T23:59:59.9999999999"_s
        << BobUI::ISODate << QDate(2013, 5, 7).startOfDay();
    BOBUIest::newRow("ISO no fraction specified")
        << u"2012-01-01T08:00:00."_s << BobUI::ISODate << QDateTime();
    // Test invalid characters (should ignore invalid characters at end of string).
    BOBUIest::newRow("ISO invalid character at end") << u"2012-01-01T08:00:00!"_s
        << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO invalid character at front") << u"!2012-01-01T08:00:00"_s
        << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO invalid character both ends") << u"!2012-01-01T08:00:00!"_s
        << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO invalid character at front, 2 at back") << u"!2012-01-01T08:00:00.."_s
        << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO invalid character 2 at front") << u"!!2012-01-01T08:00:00"_s
        << BobUI::ISODate << QDateTime();
    // Test fractional minutes.
    BOBUIest::newRow("ISO .0 of a minute (period)") << u"2012-01-01T08:00.0"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0));
    BOBUIest::newRow("ISO .8 of a minute (period)") << u"2012-01-01T08:00.8"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 48));
    BOBUIest::newRow("ISO .99999 of a minute (period)") << u"2012-01-01T08:00.99999"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 59, 999));
    BOBUIest::newRow("ISO .0 of a minute (comma)") << u"2012-01-01T08:00,0"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0));
    BOBUIest::newRow("ISO .8 of a minute (comma)") << u"2012-01-01T08:00,8"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 48));
    BOBUIest::newRow("ISO .99999 of a minute (comma)") << u"2012-01-01T08:00,99999"_s
        << BobUI::ISODate << QDateTime(QDate(2012, 1, 1), BOBUIime(8, 0, 59, 999));
    BOBUIest::newRow("ISO empty") << u""_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO short") << u"2017-07-01T"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO zoned date")
        << u"2017-07-01Z"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO zoned empty time")
        << u"2017-07-01TZ"_s << BobUI::ISODate << QDateTime();
    BOBUIest::newRow("ISO mis-punctuated")
        << u"2018/01/30 "_s << BobUI::ISODate << QDateTime();

    // Test BobUI::RFC2822Date format (RFC 2822).
    BOBUIest::newRow("RFC 2822 +0100") << u"13 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("RFC 2822 after space +0100")
        << u" 13 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("RFC 2822 with day +0100") << u"Fri, 13 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("RFC 2822 with day after space +0100")
        << u" Fri, 13 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("RFC 2822 -0100") << u"13 Feb 1987 13:24:51 -0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(14, 24, 51), UTC);
    BOBUIest::newRow("RFC 2822 with day -0100") << u"Fri, 13 Feb 1987 13:24:51 -0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(14, 24, 51), UTC);
    BOBUIest::newRow("RFC 2822 +0000") << u"01 Jan 1970 00:12:34 +0000"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    BOBUIest::newRow("RFC 2822 with day +0000") << u"Thu, 01 Jan 1970 00:12:34 +0000"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    BOBUIest::newRow("RFC 2822 missing space before +0100")
        << u"Thu, 01 Jan 1970 00:12:34+0100"_s << BobUI::RFC2822Date << QDateTime();
    // No timezone assume UTC
    BOBUIest::newRow("RFC 2822 no timezone") << u"01 Jan 1970 00:12:34"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    // No time specified
    BOBUIest::newRow("RFC 2822 date only") << u"01 Nov 2002"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 with day date only") << u"Fri, 01 Nov 2002"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 with obsolete 2 digit year")
            << u"Mon, 07 Feb 22 11:48:12 -0500"_s << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 with valid year")
            << u"Mon, 07 Feb 2022 11:48:12 -0500"_s << BobUI::RFC2822Date
            << QDateTime(QDate(2022, 2, 7), BOBUIime(11, 48, 12),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-5 * 60 * 60));

    BOBUIest::newRow("RFC 2822 malformed time (truncated)")
        << u"01 Nov 2002 0:"_s << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 malformed time (hour)")
        << u"01 Nov 2002 7:35:21"_s << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 malformed time (minute)")
        << u"01 Nov 2002 07:5:21"_s << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 malformed time (second)")
        << u"01 Nov 2002 07:35:1"_s << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 malformed time (fraction-second)")
        << u"01 Nov 2002 07:35:15.200"_s << BobUI::RFC2822Date << QDateTime();

    // Test invalid month, day, year
    BOBUIest::newRow("RFC 2822 invalid month name") << u"13 Fev 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 invalid day") << u"36 Fev 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 invalid year") << u"13 Fev 0000 13:24:51 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    // Test invalid characters.
    BOBUIest::newRow("RFC 2822 invalid character at end")
        << u"01 Jan 2012 08:00:00 +0100!"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 invalid character at front")
        << u"!01 Jan 2012 08:00:00 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 invalid character both ends")
        << u"!01 Jan 2012 08:00:00 +0100!"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 invalid character at front, 2 at back")
        << u"!01 Jan 2012 08:00:00 +0100.."_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 2822 invalid character 2 at front")
        << u"!!01 Jan 2012 08:00:00 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    // The common date text used by the "invalid character" tests, just to be
    // sure *it's* not what's invalid:
    BOBUIest::newRow("RFC 2822 (not invalid)")
        << u"01 Jan 2012 08:00:00 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(2012, 1, 1), BOBUIime(7, 0), UTC);

    // Test BobUI::RFC2822Date format (RFC 850 and 1036, permissive).
    BOBUIest::newRow("RFC 850 and 1036 +0100") << u"Fri Feb 13 13:24:51 1987 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("RFC 1036 after space +0100")
        << u" Fri Feb 13 13:24:51 1987 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(12, 24, 51), UTC);
    BOBUIest::newRow("RFC 850 and 1036 -0100") << u"Fri Feb 13 13:24:51 1987 -0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1987, 2, 13), BOBUIime(14, 24, 51), UTC);
    BOBUIest::newRow("RFC 850 and 1036 +0000") << u"Thu Jan 01 00:12:34 1970 +0000"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    // No timezone assume UTC
    BOBUIest::newRow("RFC 850 and 1036 no timezone") << u"Thu Jan 01 00:12:34 1970"_s
        << BobUI::RFC2822Date << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 12, 34), UTC);
    // No time specified
    BOBUIest::newRow("RFC 850 and 1036 date only")
        << u"Fri Nov 01 2002"_s
        << BobUI::RFC2822Date << QDateTime();
    // Test invalid characters.
    BOBUIest::newRow("RFC 850 and 1036 invalid character at end")
        << u"Sun Jan 01 08:00:00 2012 +0100!"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character at front")
        << u"!Sun Jan 01 08:00:00 2012 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character both ends")
        << u"!Sun Jan 01 08:00:00 2012 +0100!"_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character at front, 2 at back")
        << u"!Sun Jan 01 08:00:00 2012 +0100.."_s
        << BobUI::RFC2822Date << QDateTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character 2 at front")
        << u"!!Sun Jan 01 08:00:00 2012 +0100"_s
        << BobUI::RFC2822Date << QDateTime();
    // Again, check the text in the "invalid character" tests isn't the source of invalidity:
    BOBUIest::newRow("RFC 850 and 1036 (not invalid)")
        << u"Sun Jan 01 08:00:00 2012 +0100"_s
        << BobUI::RFC2822Date << QDateTime(QDate(2012, 1, 1), BOBUIime(7, 0), UTC);

    BOBUIest::newRow("RFC empty") << u""_s << BobUI::RFC2822Date << QDateTime();
}

void tst_QDateTime::fromStringDateFormat()
{
    QFETCH(QString, dateTimeStr);
    QFETCH(BobUI::DateFormat, dateFormat);
    QFETCH(QDateTime, expected);

    QDateTime dateTime = QDateTime::fromString(dateTimeStr, dateFormat);
    QCOMPARE(dateTime, expected);
}

# if BOBUI_CONFIG(datetimeparser)
void tst_QDateTime::fromStringStringFormat_data()
{
    BOBUIest::addColumn<QString>("string");
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<int>("baseYear");
    BOBUIest::addColumn<QDateTime>("expected");

    // Indian/Cocos had a transition at the start of 1900, so its Jan 1st starts
    // at 00:02:20 on that day; this leads to perverse results. BOBUIBUG-77948.
    if (const QDate defDate(1900, 1, 1); defDate.startOfDay().time() == BOBUIime(0, 0)) {
        BOBUIest::newRow("dMyy-only:19")
                << u"101010"_s << u"dMyy"_s << 1900 << QDate(1910, 10, 10).startOfDay();
        BOBUIest::newRow("dMyy-only:20")
                << u"101010"_s << u"dMyy"_s << 1911 << QDate(2010, 10, 10).startOfDay();
        BOBUIest::newRow("secs-repeat-valid")
                << u"1010"_s << u"sss"_s << 1900 << QDateTime(defDate, BOBUIime(0, 0, 10));
        BOBUIest::newRow("pm-only")
                << u"pm"_s << u"ap"_s << 1900 << QDateTime(defDate, BOBUIime(12, 0));
        BOBUIest::newRow("date-only:19")
                << u"10 Oct 10"_s << u"dd MMM yy"_s << 1900 << QDate(1910, 10, 10).startOfDay();
        BOBUIest::newRow("date-only:20")
                << u"10 Oct 10"_s << u"dd MMM yy"_s << 1950 << QDate(2010, 10, 10).startOfDay();
        BOBUIest::newRow("dow-date-only")
                << u"Fri December 3 2004"_s << u"ddd MMMM d yyyy"_s << 1900
                << QDate(2004, 12, 3).startOfDay();
        BOBUIest::newRow("dow-mon-yr-only")
                << u"Thu January 2004"_s << u"ddd MMMM yyyy"_s << 1900
                << QDate(2004, 1, 1).startOfDay();
    }
    BOBUIest::newRow("yy=24/Mar/20") // BOBUIBUG-123579
            << u"Wed, 20 Mar 24 16:17:00"_s << u"ddd, dd MMM yy HH:mm:ss"_s << 1900
            << QDateTime(QDate(2024, 3, 20), BOBUIime(16, 17));
    BOBUIest::newRow("secs-conflict") << u"1020"_s << u"sss"_s << 1900 << QDateTime();
    BOBUIest::newRow("secs-split-conflict")
            << u"10hello20"_s << u"ss'hello'ss"_s << 1900 << QDateTime();
    BOBUIest::newRow("nomatch-quote-twice") << u"10"_s << u"''"_s << 1900 << QDateTime();
    BOBUIest::newRow("momatch-quote") << u"10"_s << u"'"_s << 1900 << QDateTime();
    BOBUIest::newRow("nomatch-am-pm") << u"foo"_s << u"ap"_s << 1900 << QDateTime();
    // Day non-conflict should not hide earlier year conflict (1963-03-01 was a
    // Friday; asking for Thursday moves this, without conflict, to the 7th):
    BOBUIest::newRow("year-conflict")
            << u"77 03 1963 Thu"_s << u"yy MM yyyy ddd"_s << 1900 << QDateTime();
    BOBUIest::newRow("Feb-overflow") << u"30.02.2004"_s << u"dd.MM.yyyy"_s << 1900 << QDateTime();
    BOBUIest::newRow("Jan-overflow") << u"32.01.2004"_s << u"dd.MM.yyyy"_s << 1900 << QDateTime();
    BOBUIest::newRow("zulu-time-with-z-centisec")
            << u"2005-06-28T07:57:30.01Z"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2005, 06, 28), BOBUIime(07, 57, 30, 10), UTC);
    BOBUIest::newRow("zulu-time-with-zz-decisec")
            << u"2005-06-28T07:57:30.1Z"_s << u"yyyy-MM-ddThh:mm:ss.zzt"_s << 1900
            << QDateTime(QDate(2005, 06, 28), BOBUIime(07, 57, 30, 100), UTC);
    BOBUIest::newRow("zulu-time-with-zzz-centisec")
            << u"2005-06-28T07:57:30.01Z"_s << u"yyyy-MM-ddThh:mm:ss.zzzt"_s << 1900
            << QDateTime(); // Invalid because too few digits for zzz
    BOBUIest::newRow("zulu-time-with-z-millisec")
            << u"2005-06-28T07:57:30.001Z"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2005, 06, 28), BOBUIime(07, 57, 30, 1), UTC);
    BOBUIest::newRow("utc-time-spec-as:UTC+0")
            << u"2005-06-28T07:57:30.001UTC+0"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 1), UTC);
    BOBUIest::newRow("utc-time-spec-as:UTC-0")
            << u"2005-06-28T07:57:30.001UTC-0"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 1), UTC);
    BOBUIest::newRow("offset-from-utc:UTC+1")
            << u"2001-09-13T07:33:01.001 UTC+1"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime(QDate(2001, 9, 13), BOBUIime(7, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(3600));
    BOBUIest::newRow("offset-from-utc:UTC-11:01")
            << u"2008-09-13T07:33:01.001 UTC-11:01"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime(QDate(2008, 9, 13), BOBUIime(7, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-39660));
    BOBUIest::newRow("offset-from-utc:UTC+02:57")
            << u"2001-09-15T09:33:01.001UTC+02:57"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2001, 9, 15), BOBUIime(9, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(10620));
    BOBUIest::newRow("offset-from-utc:-03:00")  // RFC 3339 offset format
            << u"2001-09-15T09:33:01.001-03:00"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2001, 9, 15), BOBUIime(9, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-10800));
    BOBUIest::newRow("offset-from-utc:+0205")  // ISO 8601 basic offset format
            << u"2001-09-15T09:33:01.001+0205"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2001, 9, 15), BOBUIime(9, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(7500));
    BOBUIest::newRow("offset-from-utc:-0401")  // ISO 8601 basic offset format
            << u"2001-09-15T09:33:01.001-0401"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime(QDate(2001, 9, 15), BOBUIime(9, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-14460));
    BOBUIest::newRow("offset-from-utc:+10")  // ISO 8601 basic (hour-only) offset format
            << u"2001-09-15T09:33:01.001 +10"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime(QDate(2001, 9, 15), BOBUIime(9, 33, 1, 1),
                         BOBUIimeZone::fromSecondsAheadOfUtc(36000));
    BOBUIest::newRow("offset-from-utc:UTC+10:00")  // Time-spec specifier at the beginning
            << u"UTC+10:00 2008-10-13T07:33"_s << u"t yyyy-MM-ddThh:mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(7, 33),
                         BOBUIimeZone::fromSecondsAheadOfUtc(36000));
    BOBUIest::newRow("offset-from-utc:UTC-03:30")  // Time-spec specifier in the middle
            << u"2008-10-13 UTC-03:30 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-12600));
    BOBUIest::newRow("offset-from-utc:UTC-2")  // Time-spec specifier joined with text/time
            << u"2008-10-13 UTC-2Z11.50"_s << u"yyyy-MM-dd tZhh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-7200));
    BOBUIest::newRow("offset-from-utc:followed-by-colon")
            << u"2008-10-13 UTC-0100:11.50"_s << u"yyyy-MM-dd t:hh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-3600));
    BOBUIest::newRow("offset-from-utc:late-colon")
            << u"2008-10-13 UTC+05T:11.50"_s << u"yyyy-MM-dd tT:hh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(18000));
    BOBUIest::newRow("offset-from-utc:merged-with-time")
            << u"2008-10-13 UTC+010011.50"_s << u"yyyy-MM-dd thh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(3600));
    BOBUIest::newRow("offset-from-utc:double-colon-delimiter")
            << u"2008-10-13 UTC+12::11.50"_s << u"yyyy-MM-dd t::hh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(43200));
    BOBUIest::newRow("offset-from-utc:3-digit-with-colon")
            << u"2008-10-13 -4:30 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(-16200));
    BOBUIest::newRow("offset-from-utc:with-colon-merged-with-time")
            << u"2008-10-13 UTC+01:0011.50"_s << u"yyyy-MM-dd thh.mm"_s << 1900
            << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                         BOBUIimeZone::fromSecondsAheadOfUtc(3600));
    BOBUIest::newRow("invalid-offset-from-utc:out-of-range")
            << u"2001-09-15T09:33:01.001-50"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:single-digit-format")
            << u"2001-09-15T09:33:01.001+5"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:three-digit-format")
            << u"2001-09-15T09:33:01.001-701"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:three-digit-minutes")
            << u"2001-09-15T09:33:01.001+11:570"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:single-digit-minutes")
            << u"2001-09-15T09:33:01.001+11:5"_s << u"yyyy-MM-ddThh:mm:ss.zt"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:invalid-sign-symbol")
            << u"2001-09-15T09:33:01.001 ~11:30"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:symbol-in-hours")
            << u"2001-09-15T09:33:01.001 UTC+o8:30"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:symbol-in-minutes")
            << u"2001-09-15T09:33:01.001 UTC+08:3i"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:UTC+123")  // Invalid offset (UTC and 3 digit format)
            << u"2001-09-15T09:33:01.001 UTC+123"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:UTC+00005")  // Invalid offset with leading zeroes
            << u"2001-09-15T09:33:01.001 UTC+00005"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:three-digit-with-colon-delimiter")
            << u"2008-10-13 +123:11.50"_s << u"yyyy-MM-dd t:hh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:double-colon-as-part-of-offset")
            << u"2008-10-13 UTC+12::11.50"_s << u"yyyy-MM-dd thh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:single-colon-as-part-of-offset")
            << u"2008-10-13 UTC+12::11.50"_s << u"yyyy-MM-dd t:hh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:starts-with-colon")
            << u"2008-10-13 UTC+:59 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:empty-offset")
            << u"2008-10-13 UTC+ 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:time-section-instead-of-offset")
            << u"2008-10-13 UTC+11.50"_s << u"yyyy-MM-dd thh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:missing-minutes-if-colon")
            << u"2008-10-13 +05: 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:1-digit-minutes-if-colon")
            << u"2008-10-13 UTC+05:1 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-time-spec:random-symbol")
            << u"2001-09-15T09:33:01.001 $"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-time-spec:random-digit")
            << u"2001-09-15T09:33:01.001 1"_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900
            << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:merged-with-time")
            << u"2008-10-13 UTC+0111.50"_s << u"yyyy-MM-dd thh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-offset-from-utc:with-colon-3-digit-merged-with-time")
            << u"2008-10-13 UTC+01:011.50"_s << u"yyyy-MM-dd thh.mm"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-time-spec:empty")
            << u"2001-09-15T09:33:01.001 "_s << u"yyyy-MM-ddThh:mm:ss.z t"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-month-year<min") // This used to fail an unfounded assertion.
            << u"0024:91:06 08:52:20"_s << u"yyyy:MM:dd HH:mm:ss"_s << 1900 << QDateTime();
    BOBUIest::newRow("invalid-year")
            << u"0000:11:06 08:51:20"_s << u"yyyy:MM:dd HH:mm:ss"_s << 1900 << QDateTime();
#if BOBUI_CONFIG(timezone)
    BOBUIimeZone southBrazil("America/Sao_Paulo");
    if (southBrazil.isValid()) {
        BOBUIest::newRow("spring-forward-midnight")
                // NB: no hour field, so hour takes its default, 0, so that
                // default can be over-ridden:
                << u"2008-10-19 23:45.678 America/Sao_Paulo"_s << u"yyyy-MM-dd mm:ss.zzz t"_s << 1900
                // That's in the hour skipped - expect the matching time after
                // the spring-forward, in DST:
                << QDateTime(QDate(2008, 10, 19), BOBUIime(1, 23, 45, 678), southBrazil);
    }
#endif
    BOBUIest::newRow("late")
            << u"9999-12-31T23:59:59.999Z"_s << u"yyyy-MM-ddThh:mm:ss.zZ"_s << 1900
            << QDateTime(QDate(9999, 12, 31), BOBUIime(23, 59, 59, 999));
    // Separators match /([^aAdhHMmstyz]*)/
    BOBUIest::newRow("oddly-separated") // To show broken-separator's format is valid.
            << u"2018 wilful long working block relief 12-19T21:09 cruel blurb encore flux"_s
            << u"yyyy wilful long working block relief MM-ddThh:mm cruel blurb encore flux"_s
            << 1900 << QDateTime(QDate(2018, 12, 19), BOBUIime(21, 9));
    BOBUIest::newRow("broken-separator")
            << u"2018 wilful"_s
            << u"yyyy wilful long working block relief MM-ddThh:mm cruel blurb encore flux"_s
            << 1900 << QDateTime();
    BOBUIest::newRow("broken-terminator")
            << u"2018 wilful long working block relief 12-19T21:09 cruel"_s
            << u"yyyy wilful long working block relief MM-ddThh:mm cruel blurb encore flux"_s
            << 1900 << QDateTime();
    BOBUIest::newRow("fix-century-Mon")
            << u"Monday, 23 April 12 22:51:41"_s << u"dddd, d MMMM yy hh:mm:ss"_s << 1900
            << QDateTime(QDate(2012, 4, 23), BOBUIime(22, 51, 41));
    BOBUIest::newRow("fix-century-Tue")
            << u"Tuesday, 23 April 12 22:51:41"_s << u"dddd, d MMMM yy hh:mm:ss"_s << 2000
            << QDateTime(QDate(1912, 4, 23), BOBUIime(22, 51, 41));

    // test unicode
    BOBUIest::newRow("unicode handling")
            << u"2005🤣06🤣28T07🤣57🤣30.001Z"_s
            << u"yyyy🤣MM🤣ddThh🤣mm🤣ss.zt"_s << 1900
            << QDateTime(QDate(2005, 6, 28), BOBUIime(7, 57, 30, 1), UTC);

    // Two tests derived from malformed ASN.1 strings (BOBUIBUG-84349):
    BOBUIest::newRow("curft+ASN.1:UTC")
            << u"22+221102233Z"_s << u"yyMMddHHmmsst"_s << 1900 << QDateTime();
    BOBUIest::newRow("curft+ASN.1:Generalized")
            << u"9922+221102233Z"_s << u"yyyyMMddHHmmsst"_s << 1900 << QDateTime();
    // Verify baseYear needed by plain ASN.1 works:
    BOBUIest::newRow("ASN.1:UTC-start")
            << u"500101000000Z"_s << u"yyMMddHHmmsst"_s << 1950
            << QDate(1950, 1, 1).startOfDay(BOBUIimeZone::UTC);
    BOBUIest::newRow("ASN.1:UTC-end")
            << u"491231235959Z"_s << u"yyMMddHHmmsst"_s << 1950
            << QDate(2049, 12, 31).endOfDay(BOBUIimeZone::UTC).addMSecs(-999);
    // Reproducer for BOBUIBUG-129287
    BOBUIest::newRow("ASN.1:max")
            << u"99991231235959Z"_s << u"yyyyMMddHHmmsst"_s << 1950
            << QDate(9999, 12, 31).endOfDay(BOBUIimeZone::UTC).addMSecs(-999);
    // Can also be reproduced with more legible formats:
    BOBUIest::newRow("UTC:max")
            << u"9999 Dec 31 23:59:59 +00:00"_s << u"yyyy MMM dd HH:mm:ss t"_s << 9900
            << QDate(9999, 12, 31).endOfDay(BOBUIimeZone::UTC).addMSecs(-999);
    BOBUIest::newRow("UTC:min")
            << u"0100 Jan 1 00:00:00 +00:00"_s << u"yyyy MMM d HH:mm:ss t"_s << 100
            << QDate(100, 1, 1).startOfDay(BOBUIimeZone::UTC);
    // Reproducer for BOBUIBUG-135382:
    BOBUIest::newRow("year-zero")
            << u"0000-01-01T00:00:00.000"_s << u"yyyy-MM-ddThh:mm:ss.zzz"_s << 1900
            << QDateTime();

    // fuzzer test
    BOBUIest::newRow("integer overflow found by fuzzer")
                << u"EEE1200000MUB"_s << u"t"_s << 1900 << QDateTime();

    // Rich time-zone specifiers (BOBUIBUG-95966):
    const auto east3hours = BOBUIimeZone::fromSecondsAheadOfUtc(10800);
    BOBUIest::newRow("timezone-tt-with-offset:+0300")
            << u"2008-10-13 +0300 11.50"_s << u"yyyy-MM-dd tt hh.mm"_s
            << 1900 << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50), east3hours);
    BOBUIest::newRow("timezone-ttt-with-offset:+03:00")
            << u"2008-10-13 +03:00 11.50"_s << u"yyyy-MM-dd ttt hh.mm"_s
            << 1900 << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50), east3hours);
    BOBUIest::newRow("timezone-tttt-with-offset:+03:00")
            << u"2008-10-13 +03:00 11.50"_s << u"yyyy-MM-dd tttt hh.mm"_s
            << 1900 << QDateTime(); // Offset not valid when zone name expected.
}

void tst_QDateTime::fromStringStringFormat()
{
    QFETCH(QString, string);
    QFETCH(QString, format);
    QFETCH(int, baseYear);
    QFETCH(QDateTime, expected);

    if (futureTimeType == LocalTimeAheadOfUtc) {
        // The new parser should remove the bounds, removing this limitation.
        QEXPECT_FAIL("ASN.1:max", "BOBUIBUG-77948: min/max are local times", Abort);
        QEXPECT_FAIL("UTC:max", "BOBUIBUG-77948: min/max are local times", Abort);
    }
    // For contrast, UTC::min gets away with it, which probably means there are
    // genuinely out-of-range cases the old parser gets wrong.

    QDateTime dt = QDateTime::fromString(string, format, baseYear);
#ifdef USING_MS_TZDB
    QEXPECT_FAIL("spring-forward-midnight", "MS misreads the IANA DB", Continue);
#endif
    QCOMPARE(dt, expected);

    if (expected.isValid()) {
        QCOMPARE(dt.timeSpec(), expected.timeSpec());
        QCOMPARE(dt.timeRepresentation(), dt.timeRepresentation());
    } else {
        QCOMPARE(dt.isValid(), expected.isValid());
        QCOMPARE(dt.toMSecsSinceEpoch(), expected.toMSecsSinceEpoch());
    }
}

void tst_QDateTime::fromStringStringFormat_localTimeZone_data()
{
    BOBUIest::addColumn<QByteArray>("localTimeZone");
    BOBUIest::addColumn<QString>("string");
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<int>("baseYear");
    BOBUIest::addColumn<QDateTime>("expected");

#if BOBUI_CONFIG(timezone)
    bool lacksRows = true;
    // Note that the localTimeZone needn't match the zone used in the string and
    // expected date-time; indeed, having them different is probably best.
    // Both zones need to be valid; GMT always is, so is a safe one to use for
    // whichever the test-case doesn't care about (if that applies to either).
    BOBUIimeZone etcGmtWithOffset("Etc/GMT+3");
    if (etcGmtWithOffset.isValid()) {
        lacksRows = false;
        BOBUIest::newRow("local-timezone-t-with-zone:Etc/GMT+3")
                << "GMT"_ba << u"2008-10-13 Etc/GMT+3 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900
                << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50), etcGmtWithOffset);
        BOBUIest::newRow("local-timezone-tttt-with-zone:Etc/GMT+3")
                << "GMT"_ba << u"2008-10-13 Etc/GMT+3 11.50"_s << u"yyyy-MM-dd tttt hh.mm"_s << 1900
                << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50), etcGmtWithOffset);
    }
    BOBUIest::newRow("local-timezone-tt-with-zone:Etc/GMT+3")
            << "GMT"_ba << u"2008-10-13 Etc/GMT+3 11.50"_s << u"yyyy-MM-dd tt hh.mm"_s << 1900
            << QDateTime(); // Zone name not valid when offset expected
    BOBUIest::newRow("local-timezone-ttt-with-zone:Etc/GMT+3")
            << "GMT"_ba << u"2008-10-13 Etc/GMT+3 11.50"_s << u"yyyy-MM-dd ttt hh.mm"_s << 1900
            << QDateTime(); // Zone name not valid when offset expected
    BOBUIimeZone gmtWithOffset("GMT-0");
    if (gmtWithOffset.isValid()) {
        lacksRows = false;
        BOBUIest::newRow("local-timezone-with-offset:GMT-0")
                << "GMT"_ba << u"2008-10-13 GMT-0 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900
                << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50), gmtWithOffset);
    }
    BOBUIimeZone gmt("GMT");
    if (gmt.isValid()) {
        lacksRows = false;
        const bool fullyLocal = ([]() {
            TimeZoneRollback useZone("GMT");
            return qTzName(0) == u"GMT"_s;
        })();
        BOBUIest::newRow("local-timezone-with-offset:GMT")
                << "GMT"_ba << u"2008-10-13 GMT 11.50"_s << u"yyyy-MM-dd t hh.mm"_s << 1900
                << QDateTime(QDate(2008, 10, 13), BOBUIime(11, 50),
                             fullyLocal ? BOBUIimeZone(BOBUIimeZone::LocalTime) : gmt);
    }
    BOBUIimeZone helsinki("Europe/Helsinki");
    if (helsinki.isValid()) {
        lacksRows = false;
        // BOBUIBUG-96861: QAsn1Element::toDateTime() tripped over an assert due to
        // the first 20m 11s of 1921-05-01 being skipped, so the parser's
        // attempt to construct a local time after scanning yyMM tripped up on
        // the start of the day, when the zone backend lacked transition data.
        // (Because QDTP tries to use local time until it reads the final zone
        // field, constructing a new QDT after reading each field, hence
        // transiently wanting 1921-05-01 00:00:00 before reading the dd field.)
        BOBUIest::newRow("Helsinki-joins-EET:19")
                << "Europe/Helsinki"_ba << u"210506000000Z"_s << u"yyMMddHHmmsst"_s << 1900
                << QDateTime(QDate(1921, 5, 6), BOBUIime(0, 0), UTC);
        // Strictly, ASN.1 wants us to parse that with a different baseYear, so
        // check that, too, but tweak to match the 1921 transition's mid-point:
        BOBUIest::newRow("Helsinki-joins-EET:20")
                << "Europe/Helsinki"_ba << u"210501001006Z"_s << u"yyMMddHHmmsst"_s << 1950
                << QDateTime(QDate(2021, 5, 1), BOBUIime(0, 10, 6), UTC);
    }
    if (lacksRows)
        QSKIP("Testcases all use zones unsupported on this platform");
#else
    QSKIP("Test only possible with timezone support enabled");
#endif
}

void tst_QDateTime::fromStringStringFormat_localTimeZone()
{
    QFETCH(QByteArray, localTimeZone);
    TimeZoneRollback useZone(localTimeZone);  // enforce test's time zone
    fromStringStringFormat();  // call basic fromStringStringFormat test
}
# endif // datetimeparser
#endif // datestring

void tst_QDateTime::offsetFromUtc()
{
    /* Check default value. */
    QCOMPARE(QDateTime().offsetFromUtc(), 0);

    // Offset constructor
    QDateTime dt1(QDate(2013, 1, 1), BOBUIime(1, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    QCOMPARE(dt1.offsetFromUtc(), 60 * 60);
    QVERIFY(dt1.timeRepresentation().isValid());
#if BOBUI_CONFIG(timezone)
    QVERIFY(dt1.timeZone().isValid());
#endif
    dt1 = QDateTime(QDate(2013, 1, 1), BOBUIime(1, 0), BOBUIimeZone::fromSecondsAheadOfUtc(-60 * 60));
    QCOMPARE(dt1.offsetFromUtc(), -60 * 60);

    // UTC should be 0 offset
    QDateTime dt2(QDate(2013, 1, 1), BOBUIime(0, 0), UTC);
    QCOMPARE(dt2.offsetFromUtc(), 0);

    // LocalTime should vary
    if (zoneIsCET) {
        // Time definitely in Standard Time so 1 hour ahead
        QDateTime dt3 = QDate(2013, 1, 1).startOfDay();
        QCOMPARE(dt3.offsetFromUtc(), 1 * 60 * 60);
        // Time definitely in Daylight Time so 2 hours ahead
        QDateTime dt4 = QDate(2013, 6, 1).startOfDay();
        QCOMPARE(dt4.offsetFromUtc(), 2 * 60 * 60);
    } else {
        qDebug("Skipped some tests specific to Central European Time "
               "(CET/CEST), e.g. TZ=Europe/Oslo");
    }

#if BOBUI_CONFIG(timezone)
    QDateTime dt5(QDate(2013, 1, 1), BOBUIime(0, 0), BOBUIimeZone("Pacific/Auckland"));
    QCOMPARE(dt5.offsetFromUtc(), 46800);

    QDateTime dt6(QDate(2013, 6, 1), BOBUIime(0, 0), BOBUIimeZone("Pacific/Auckland"));
    QCOMPARE(dt6.offsetFromUtc(), 43200);
#endif
}

#if BOBUI_DEPRECATED_SINCE(6, 9)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
void tst_QDateTime::setOffsetFromUtc()
{
    /* Basic tests. */
    {
        QDateTime dt(QDateTime::currentDateTime());
        dt.setTimeSpec(BobUI::LocalTime);

        dt.setOffsetFromUtc(0);
        QCOMPARE(dt.offsetFromUtc(), 0);
        QCOMPARE(dt.timeSpec(), BobUI::UTC);

        dt.setOffsetFromUtc(-100);
        QCOMPARE(dt.offsetFromUtc(), -100);
        QCOMPARE(dt.timeSpec(), BobUI::OffsetFromUTC);
    }

    /* Test detaching. */
    {
        QDateTime dt(QDateTime::currentDateTime());
        QDateTime dt2(dt);
        int offset2 = dt2.offsetFromUtc();

        dt.setOffsetFromUtc(501);

        QCOMPARE(dt.offsetFromUtc(), 501);
        QCOMPARE(dt2.offsetFromUtc(), offset2);
    }

    /* Check copying. */
    {
        QDateTime dt(QDateTime::currentDateTime());
        dt.setOffsetFromUtc(502);
        QCOMPARE(dt.offsetFromUtc(), 502);

        QDateTime dt2(dt);
        QCOMPARE(dt2.offsetFromUtc(), 502);
    }

    /* Check assignment. */
    {
        QDateTime dt(QDateTime::currentDateTime());
        dt.setOffsetFromUtc(502);
        QDateTime dt2;
        dt2 = dt;

        QCOMPARE(dt2.offsetFromUtc(), 502);
    }

    // Check spec persists
    QDateTime dt1(QDate(2013, 1, 1), BOBUIime(0, 0), BobUI::OffsetFromUTC, 60 * 60);
    dt1.setMSecsSinceEpoch(123456789);
    QCOMPARE(dt1.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(dt1.offsetFromUtc(), 60 * 60);
    dt1.setSecsSinceEpoch(123456789);
    QCOMPARE(dt1.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(dt1.offsetFromUtc(), 60 * 60);

    // Check datastream serialises the offset seconds
    QByteArray tmp;
    {
        QDataStream ds(&tmp, QIODevice::WriteOnly);
        ds << dt1;
    }
    QDateTime dt2;
    {
        QDataStream ds(&tmp, QIODevice::ReadOnly);
        ds >> dt2;
    }
    QCOMPARE(dt2, dt1);
    QCOMPARE(dt2.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(dt2.offsetFromUtc(), 60 * 60);
}

void tst_QDateTime::toOffsetFromUtc()
{
    QDateTime dt1(QDate(2013, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);

    QDateTime dt2 = dt1.toOffsetFromUtc(60 * 60);
    QCOMPARE(dt2, dt1);
    QCOMPARE(dt2.timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(dt2.date(), QDate(2013, 1, 1));
    QCOMPARE(dt2.time(), BOBUIime(1, 0));

    dt2 = dt1.toOffsetFromUtc(0);
    QCOMPARE(dt2, dt1);
    QCOMPARE(dt2.timeSpec(), BobUI::UTC);
    QCOMPARE(dt2.date(), QDate(2013, 1, 1));
    QCOMPARE(dt2.time(), BOBUIime(0, 0));

    dt2 = dt1.toTimeSpec(BobUI::OffsetFromUTC);
    QCOMPARE(dt2, dt1);
    QCOMPARE(dt2.timeSpec(), BobUI::UTC);
    QCOMPARE(dt2.date(), QDate(2013, 1, 1));
    QCOMPARE(dt2.time(), BOBUIime(0, 0));
}
BOBUI_WARNING_POP
#endif // 6.9 deprecation

void tst_QDateTime::zoneAtTime_data()
{
    BOBUIest::addColumn<QByteArray>("ianaID");
    BOBUIest::addColumn<QDate>("date");
    BOBUIest::addColumn<int>("offset");
#define ADDROW(name, zone, date, offset) \
    BOBUIest::newRow(name) << QByteArray(zone) << (date) << (offset)

    // Check DST handling around epoch:
    {
        QDate epoch(1970, 1, 1);
        ADDROW("epoch:UTC", "UTC", epoch, 0);
        // Paris and Berlin skipped DST around 1970; but Rome used it.
        ADDROW("epoch:CET", "Europe/Rome", epoch, 3600);
        ADDROW("epoch:PST", "America/Vancouver", epoch, -8 * 3600);
        ADDROW("epoch:EST", "America/New_York", epoch, -5 * 3600);
    }
    {
        // QDateTime now takes account of DST even before the epoch.
        QDate summer69(1969, 8, 15); // Woodstock started
        ADDROW("summer69:UTC", "UTC", summer69, 0);
        ADDROW("summer69:CET", "Europe/Rome", summer69, 2 * 3600);
        ADDROW("summer69:PST", "America/Vancouver", summer69, -7 * 3600);
        ADDROW("summer69:EST", "America/New_York", summer69, -4 * 3600);
    }
    {
        // ... and has always taken it into account since:
        QDate summer70(1970, 8, 26); // Isle of Wight festival
        ADDROW("summer70:UTC", "UTC", summer70, 0);
        ADDROW("summer70:CET", "Europe/Rome", summer70, 2 * 3600);
        ADDROW("summer70:PST", "America/Vancouver", summer70, -7 * 3600);
        ADDROW("summer70:EST", "America/New_York", summer70, -4 * 3600);
    }

#ifdef Q_OS_ANDROID // BOBUIBUG-68835; gets offset 0 for the affected tests.
# define NONANDROIDROW(name, zone, date, offset)
#else
# define NONANDROIDROW(name, zone, date, offset) ADDROW(name, zone, date, offset)
#endif

#ifndef USING_WIN_TZ
    // Bracket a few noteworthy transitions:
    ADDROW("before:ACWST", "Australia/Eucla", QDate(1974, 10, 26), 31500); // 8:45
    NONANDROIDROW("after:ACWST", "Australia/Eucla", QDate(1974, 10, 27), 35100); // 9:45
    NONANDROIDROW("before:NPT", "Asia/Kathmandu", QDate(1985, 12, 31), 19800); // 5:30
    ADDROW("after:NPT", "Asia/Kathmandu", QDate(1986, 1, 1), 20700); // 5:45
    // The two that have skipped a day (each):
    NONANDROIDROW("before:LINT", "Pacific/Kiritimati", QDate(1994, 12, 30), -36000);
    ADDROW("after:LINT", "Pacific/Kiritimati", QDate(1995, 1, 2), 14 * 3600);
    ADDROW("after:WST", "Pacific/Apia", QDate(2011, 12, 31), 14 * 3600);
#endif // MS lacks ACWST, NPT; doesn't grok date-line crossings; and Windows 7 lacks LINT.
    ADDROW("before:WST", "Pacific/Apia", QDate(2011, 12, 29), -36000);
#undef ADDROW
}

void tst_QDateTime::zoneAtTime()
{
#if BOBUI_CONFIG(timezone)
    QFETCH(QByteArray, ianaID);
    QFETCH(QDate, date);
    QFETCH(int, offset);
    const BOBUIime noon(12, 0);

    BOBUIimeZone zone(ianaID);
#ifdef USING_MS_TZDB
    QEXPECT_FAIL("after:NPT", "MS lacks NPT", Abort);
    QEXPECT_FAIL("before:NPT", "MS lacks NPT", Abort);
#endif
    QVERIFY(zone.isValid());
#ifdef GLIBC_TZDB_MISPARSE
    QEXPECT_FAIL("after:WST", "BOBUIBUG-127598 Bad libstdc++ data", Abort);
    QEXPECT_FAIL("before:WST", "BOBUIBUG-127598 Bad libstdc++ data", Abort);
#endif
#ifdef USING_MS_TZDB
    QEXPECT_FAIL("after:ACWST", "MS gets ACWST wrong", Abort);
#endif
    QCOMPARE(QDateTime(date, noon, zone).offsetFromUtc(), offset);
    QCOMPARE(zone.offsetFromUtc(QDateTime(date, noon, zone)), offset);
#else
    QSKIP("Needs timezone feature enabled");
#endif
}

void tst_QDateTime::timeZoneAbbreviation()
{
    QDateTime dt1(QDate(2013, 1, 1), BOBUIime(1, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    QCOMPARE(dt1.timeZoneAbbreviation(), u"UTC+01:00"_s);
    QDateTime dt2(QDate(2013, 1, 1), BOBUIime(1, 0), BOBUIimeZone::fromSecondsAheadOfUtc(-60 * 60));
    QCOMPARE(dt2.timeZoneAbbreviation(), u"UTC-01:00"_s);

    QDateTime dt3(QDate(2013, 1, 1), BOBUIime(0, 0), UTC);
    QCOMPARE(dt3.timeZoneAbbreviation(), u"UTC"_s);

    // LocalTime should vary
    if (zoneIsCET) {
        // Time definitely in Standard Time
        QDateTime dt4 = QDate(2013, 1, 1).startOfDay();
        /* Note that MET is functionally an alias for CET (their zoneinfo files
           differ only in the first letter of the abbreviations), unlike the
           various zones that give CET as their abbreviation. The GMT+[12] names
           show up as fall-back if the system locale doesn't have a standard
           abbreviation for Central European Time.
        */
        {
            const QString abbrev = dt4.timeZoneAbbreviation();
            QVERIFY2(abbrev == u"CET"_s || abbrev == u"MET"_s || abbrev == u"GMT+1"_s,
                     qPrintable(abbrev));
        }
        // Time definitely in Daylight Time
        QDateTime dt5 = QDate(2013, 6, 1).startOfDay();
        {
            const QString abbrev = dt5.timeZoneAbbreviation();
            QVERIFY2(abbrev == u"CEST"_s || abbrev == u"MEST"_s || abbrev == u"GMT+2"_s,
                     qPrintable(abbrev));
        }
    } else {
        qDebug("(Skipped some CET-only tests)");
    }

#if BOBUI_CONFIG(timezone)
    const BOBUIimeZone berlin("Europe/Berlin");
    const QDateTime jan(QDate(2013, 1, 1).startOfDay(berlin));
    const QDateTime jul(QDate(2013, 7, 1).startOfDay(berlin));

    QCOMPARE(jan.timeZoneAbbreviation(), berlin.abbreviation(jan));
    QCOMPARE(jul.timeZoneAbbreviation(), berlin.abbreviation(jul));
#endif
}

void tst_QDateTime::getDate()
{
    {
    int y = -33, m = -44, d = -55;
    QDate date;
    date.getDate(&y, &m, &d);
    QCOMPARE(date.year(), y);
    QCOMPARE(date.month(), m);
    QCOMPARE(date.day(), d);

    date.getDate(0, 0, 0);
    }

    {
    int y = -33, m = -44, d = -55;
    QDate date(1998, 5, 24);
    date.getDate(0, &m, 0);
    date.getDate(&y, 0, 0);
    date.getDate(0, 0, &d);

    QCOMPARE(date.year(), y);
    QCOMPARE(date.month(), m);
    QCOMPARE(date.day(), d);
    }
}

void tst_QDateTime::fewDigitsInYear() const
{
    const QDateTime three(QDate(300, 10, 11).startOfDay());
    QCOMPARE(three.toString(u"yyyy-MM-dd"), "0300-10-11"_L1);

    const QDateTime two(QDate(20, 10, 11).startOfDay());
    QCOMPARE(two.toString(u"yyyy-MM-dd"), "0020-10-11"_L1);

    const QDateTime yyTwo(QDate(30, 10, 11).startOfDay());
    QCOMPARE(yyTwo.toString(u"yy-MM-dd"), "30-10-11"_L1);

    const QDateTime yyOne(QDate(4, 10, 11).startOfDay());
    QCOMPARE(yyOne.toString(u"yy-MM-dd"), "04-10-11"_L1);
}

void tst_QDateTime::printNegativeYear() const
{
    {
        QDateTime date(QDate(-20, 10, 11).startOfDay());
        QVERIFY(date.isValid());
        QCOMPARE(date.toString(u"yyyy"), "-0020"_L1);
    }

    {
        QDateTime date(QDate(-3, 10, 11).startOfDay());
        QVERIFY(date.isValid());
        QCOMPARE(date.toString(u"yyyy"), "-0003"_L1);
    }

    {
        QDateTime date(QDate(-400, 10, 11).startOfDay());
        QVERIFY(date.isValid());
        QCOMPARE(date.toString(u"yyyy"), "-0400"_L1);
    }
}

#if BOBUI_CONFIG(datetimeparser)
void tst_QDateTime::roundtripTextDate() const
{
    BOBUIest::failOnWarning(); // This code path should not result in warnings.
    const QDateTime now(QDateTime::currentDateTime());
    // TextDate drops millis:
    const QDateTime theDateTime(now.addMSecs(-now.time().msec()));
    QCOMPARE(QDateTime::fromString(theDateTime.toString(BobUI::TextDate), BobUI::TextDate), theDateTime);
}
#endif

void tst_QDateTime::utcOffsetLessThan() const
{
    QDateTime dt1(QDate(2002, 10, 10), BOBUIime(0, 0));
    QDateTime dt2(dt1);

    dt1.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-(2 * 60 * 60))); // Minus two hours.
    dt2.setTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(-(3 * 60 * 60))); // Minus three hours.

    QCOMPARE_NE(dt1, dt2);
    QVERIFY(dt1 != dt2);
    QVERIFY(!(dt1 == dt2));
    QCOMPARE_LT(dt1, dt2);
    QVERIFY(dt1 < dt2);
    QVERIFY(!(dt2 < dt1));
}

void tst_QDateTime::isDaylightTime() const
{
    QDateTime utc1(QDate(2012, 1, 1), BOBUIime(0, 0), UTC);
    QVERIFY(!utc1.isDaylightTime());
    QDateTime utc2(QDate(2012, 6, 1), BOBUIime(0, 0), UTC);
    QVERIFY(!utc2.isDaylightTime());

    QDateTime offset1(QDate(2012, 1, 1), BOBUIime(0, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    QVERIFY(!offset1.isDaylightTime());
    QDateTime offset2(QDate(2012, 6, 1), BOBUIime(0, 0), BOBUIimeZone::fromSecondsAheadOfUtc(60 * 60));
    QVERIFY(!offset2.isDaylightTime());

    if (zoneIsCET) {
        QDateTime cet1(QDate(2012, 1, 1), BOBUIime(0, 0));
        QVERIFY(!cet1.isDaylightTime());
        QDateTime cet2(QDate(2012, 6, 1), BOBUIime(0, 0));
        QVERIFY(cet2.isDaylightTime());
    } else {
        qDebug("Skipped some tests specific to Central European Time "
               "(CET/CEST), e.g. TZ=Europe/Oslo");
    }
}

void tst_QDateTime::daylightTransitions() const
{
    if (!zoneIsCET)
        QSKIP("You must test using Central European (CET/CEST) time zone, e.g. TZ=Europe/Oslo");

    // CET transitions occur at 01:00:00 UTC on last Sunday in March and October
    // 2011-03-27 02:00:00 CET  became 03:00:00 CEST at msecs = 1301187600000
    // 2011-10-30 03:00:00 CEST became 02:00:00 CET  at msecs = 1319936400000
    // 2012-03-25 02:00:00 CET  became 03:00:00 CEST at msecs = 1332637200000
    // 2012-10-28 03:00:00 CEST became 02:00:00 CET  at msecs = 1351386000000
    QCOMPARE(QDate(2012, 3, 25).dayOfWeek(), 7);
    QCOMPARE(QDate(2012, 10, 28).dayOfWeek(), 7);
    const qint64 spring2012 = 1332637200000;
    const qint64 autumn2012 = 1351386000000;
    const qint64 msecsOneHour = 3600000;
    QCOMPARE(spring2012, QDateTime(QDate(2012, 3, 25), BOBUIime(1, 0), UTC).toMSecsSinceEpoch());
    QCOMPARE(autumn2012, QDateTime(QDate(2012, 10, 28), BOBUIime(1, 0), UTC).toMSecsSinceEpoch());

    // Test for correct behviour for StandardTime -> DaylightTime transition, i.e. missing hour

    // Test setting date, time in missing hour will be invalid

    QDateTime before(QDate(2012, 3, 25), BOBUIime(1, 59, 59, 999));
    QVERIFY(before.isValid());
    QVERIFY(!before.isDaylightTime());
    QCOMPARE(before.date(), QDate(2012, 3, 25));
    QCOMPARE(before.time(), BOBUIime(1, 59, 59, 999));
    QCOMPARE(before.toMSecsSinceEpoch(), spring2012 - 1);

    QDateTime entering(QDate(2012, 3, 25), BOBUIime(2, 0),
                       QDateTime::TransitionResolution::PreferBefore);
    QVERIFY(entering.isValid());
    QVERIFY(!entering.isDaylightTime());
    QCOMPARE(entering.date(), QDate(2012, 3, 25));
    QCOMPARE(entering.time(), BOBUIime(1, 0));
    // QDateTimeParser relies on toMSecsSinceEpoch() to still work:
    QCOMPARE(entering.toMSecsSinceEpoch(), spring2012 - msecsOneHour);

    QDateTime leaving(QDate(2012, 3, 25), BOBUIime(2, 0),
                      QDateTime::TransitionResolution::PreferAfter);
    QVERIFY(leaving.isValid());
    QVERIFY(leaving.isDaylightTime());
    QCOMPARE(leaving.date(), QDate(2012, 3, 25));
    QCOMPARE(leaving.time(), BOBUIime(3, 0));
    // QDateTimeParser relies on toMSecsSinceEpoch to still work:
    QCOMPARE(leaving.toMSecsSinceEpoch(), spring2012);

    QDateTime after(QDate(2012, 3, 25), BOBUIime(3, 0));
    QVERIFY(after.isValid());
    QVERIFY(after.isDaylightTime());
    QCOMPARE(after.date(), QDate(2012, 3, 25));
    QCOMPARE(after.time(), BOBUIime(3, 0));
    QCOMPARE(after.toMSecsSinceEpoch(), spring2012);

    // Test round-tripping of msecs

    before.setMSecsSinceEpoch(spring2012 - 1);
    QVERIFY(before.isValid());
    QCOMPARE(before.date(), QDate(2012, 3, 25));
    QCOMPARE(before.time(), BOBUIime(1, 59, 59, 999));
    QCOMPARE(before.toMSecsSinceEpoch(), spring2012 -1);

    after.setMSecsSinceEpoch(spring2012);
    QVERIFY(after.isValid());
    QCOMPARE(after.date(), QDate(2012, 3, 25));
    QCOMPARE(after.time(), BOBUIime(3, 0));
    QCOMPARE(after.toMSecsSinceEpoch(), spring2012);

    // Test changing time spec re-validates the date/time
    QDateTime utc(QDate(2012, 3, 25), BOBUIime(2, 0), UTC);
    QVERIFY(utc.isValid());
    QCOMPARE(utc.date(), QDate(2012, 3, 25));
    QCOMPARE(utc.time(), BOBUIime(2, 0));
    utc.setTimeZone(BOBUIimeZone::LocalTime); // Resolved to RelativeToBefore.
    QVERIFY(utc.isValid());
    QCOMPARE(utc.date(), QDate(2012, 3, 25));
    QCOMPARE(utc.time(), BOBUIime(3, 0));
    utc.setTimeZone(UTC); // Preserves the changed time().
    QVERIFY(utc.isValid());
    QCOMPARE(utc.date(), QDate(2012, 3, 25));
    QCOMPARE(utc.time(), BOBUIime(3, 0));

    // Test date maths, if result falls in missing hour then becomes next
    // hour (or is always invalid; mktime() may reject gap-times).

    QDateTime test(QDate(2011, 3, 25), BOBUIime(2, 0));
    QVERIFY(test.isValid());
    test = test.addYears(1);
    const bool handled = test.isValid();
#define CHECK_SPRING_FORWARD(test)                  \
    if (test.isValid()) {                           \
        QCOMPARE(test.date(), QDate(2012, 3, 25));  \
        QCOMPARE(test.time(), BOBUIime(3, 0));         \
    } else {                                        \
        QVERIFY(!handled);                          \
    }
    CHECK_SPRING_FORWARD(test);

    test = QDateTime(QDate(2012, 2, 25), BOBUIime(2, 0));
    QVERIFY(test.isValid());
    test = test.addMonths(1);
    CHECK_SPRING_FORWARD(test);

    test = QDateTime(QDate(2012, 3, 24), BOBUIime(2, 0));
    QVERIFY(test.isValid());
    test = test.addDays(1);
    CHECK_SPRING_FORWARD(test);

    test = QDateTime(QDate(2012, 3, 25), BOBUIime(1, 0));
    QVERIFY(test.isValid());
    QCOMPARE(test.toMSecsSinceEpoch(), spring2012 - msecsOneHour);
    test = test.addMSecs(msecsOneHour);
    CHECK_SPRING_FORWARD(test);
    if (handled)
        QCOMPARE(test.toMSecsSinceEpoch(), spring2012);
#undef CHECK_SPRING_FORWARD

    // Test for correct behviour for DaylightTime -> StandardTime transition, fall-back

    QDateTime autumnMidnight = QDate(2012, 10, 28).startOfDay();
    QVERIFY(autumnMidnight.isValid());
    QCOMPARE(autumnMidnight.date(), QDate(2012, 10, 28));
    QCOMPARE(autumnMidnight.time(), BOBUIime(0, 0));
    QCOMPARE(autumnMidnight.toMSecsSinceEpoch(), autumn2012 - 3 * msecsOneHour);

    QDateTime startFirst = autumnMidnight.addMSecs(2 * msecsOneHour);
    QVERIFY(startFirst.isValid());
    QCOMPARE(startFirst, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0),
                                   QDateTime::TransitionResolution::PreferBefore));
    QCOMPARE(startFirst.date(), QDate(2012, 10, 28));
    QCOMPARE(startFirst.time(), BOBUIime(2, 0));
    QCOMPARE(startFirst.toMSecsSinceEpoch(), autumn2012 - msecsOneHour);

    // 1 msec before transition is 2:59:59.999 FirstOccurrence
    QDateTime endFirst = startFirst.addMSecs(msecsOneHour - 1);
    QVERIFY(endFirst.isValid());
    QCOMPARE(endFirst,
             QDateTime(QDate(2012, 10, 28), BOBUIime(2, 59, 59, 999),
                       QDateTime::TransitionResolution::PreferBefore));
    QCOMPARE(endFirst.date(), QDate(2012, 10, 28));
    QCOMPARE(endFirst.time(), BOBUIime(2, 59, 59, 999));
    QCOMPARE(endFirst.toMSecsSinceEpoch(), autumn2012 - 1);

    // At the transition, starting the second pass
    QDateTime startRepeat = endFirst.addMSecs(1);
    QVERIFY(startRepeat.isValid());
    QCOMPARE(startRepeat, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0),
                                    QDateTime::TransitionResolution::PreferAfter));
    QCOMPARE(startRepeat.date(), QDate(2012, 10, 28));
    QCOMPARE(startRepeat.time(), BOBUIime(2, 0));
    QCOMPARE(startRepeat.toMSecsSinceEpoch(), autumn2012);

    // 59:59.999 after transition is 2:59:59.999 SecondOccurrence
    QDateTime endRepeat = endFirst.addMSecs(msecsOneHour);
    QVERIFY(endRepeat.isValid());
    QCOMPARE(endRepeat,
             QDateTime(QDate(2012, 10, 28), BOBUIime(2, 59, 59, 999),
                       QDateTime::TransitionResolution::PreferAfter));
    QCOMPARE(endRepeat.date(), QDate(2012, 10, 28));
    QCOMPARE(endRepeat.time(), BOBUIime(2, 59, 59, 999));
    QCOMPARE(endRepeat.toMSecsSinceEpoch(), autumn2012 + msecsOneHour - 1);

    // 1 hour after transition is 3:00:00 (not ambiguous)
    QDateTime hourAfter = endRepeat.addMSecs(1);
    QVERIFY(hourAfter.isValid());
    QCOMPARE(hourAfter, QDateTime(QDate(2012, 10, 28), BOBUIime(3, 0)));
    QCOMPARE(hourAfter.date(), QDate(2012, 10, 28));
    QCOMPARE(hourAfter.time(), BOBUIime(3, 0));
    QCOMPARE(hourAfter.toMSecsSinceEpoch(), autumn2012 + msecsOneHour);

    // Test round-tripping of msecs

    // 1 hour before transition is 2:00:00 FirstOccurrence
    startFirst.setMSecsSinceEpoch(autumn2012 - msecsOneHour);
    QVERIFY(startFirst.isValid());
    QCOMPARE(startFirst.date(), QDate(2012, 10, 28));
    QCOMPARE(startFirst.time(), BOBUIime(2, 0));
    QCOMPARE(startFirst.toMSecsSinceEpoch(), autumn2012 - msecsOneHour);

    // 1 msec before transition is 2:59:59.999 FirstOccurrence
    endFirst.setMSecsSinceEpoch(autumn2012 - 1);
    QVERIFY(endFirst.isValid());
    QCOMPARE(endFirst.date(), QDate(2012, 10, 28));
    QCOMPARE(endFirst.time(), BOBUIime(2, 59, 59, 999));
    QCOMPARE(endFirst.toMSecsSinceEpoch(), autumn2012 - 1);

    // At transition is 2:00:00 SecondOccurrence
    startRepeat.setMSecsSinceEpoch(autumn2012);
    QVERIFY(startRepeat.isValid());
    QCOMPARE(startRepeat.date(), QDate(2012, 10, 28));
    QCOMPARE(startRepeat.time(), BOBUIime(2, 0));
    QCOMPARE(startRepeat.toMSecsSinceEpoch(), autumn2012);

    // 59:59.999 after transition is 2:59:59.999 SecondOccurrence
    endRepeat.setMSecsSinceEpoch(autumn2012 + msecsOneHour - 1);
    QVERIFY(endRepeat.isValid());
    QCOMPARE(endRepeat.date(), QDate(2012, 10, 28));
    QCOMPARE(endRepeat.time(), BOBUIime(2, 59, 59, 999));
    QCOMPARE(endRepeat.toMSecsSinceEpoch(), autumn2012 + msecsOneHour - 1);

    // 1 hour after transition is 3:00:00 (unambiguous)
    hourAfter.setMSecsSinceEpoch(autumn2012 + msecsOneHour);
    QVERIFY(hourAfter.isValid());
    QCOMPARE(hourAfter.date(), QDate(2012, 10, 28));
    QCOMPARE(hourAfter.time(), BOBUIime(3, 0));
    QCOMPARE(hourAfter.toMSecsSinceEpoch(), autumn2012 + msecsOneHour);

    // Test date maths

    // Add year to a DST moment to hit start of first pass:
    test = QDateTime(QDate(2011, 10, 28), BOBUIime(2, 0));
    QVERIFY(test.isDaylightTime()); // Before last Sunday in month
    test = test.addYears(1);
    QVERIFY(test.isValid());
    QVERIFY(test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    // QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0), Prior));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 - msecsOneHour);

    // Subtract year from post-tran time to hit start of second pass:
    test = QDateTime(QDate(2013, 10, 28), BOBUIime(2, 0));
    QVERIFY(!test.isDaylightTime()); // After last Sundy in month
    test = test.addYears(-1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    // QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0), Post));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012);

    // Add year to get to after the repeated hour
    test = QDateTime(QDate(2011, 10, 28), BOBUIime(3, 0));
    QVERIFY(test.isDaylightTime()); // Before last Sunday in month
    test = test.addYears(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(3, 0)));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 + msecsOneHour);

    // Add year to start of first pass:
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(1, 0)).addMSecs(msecsOneHour);
    QVERIFY(test.isDaylightTime());
    test = test.addYears(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime()); // After last Sunday in month
    QCOMPARE(test.date(), QDate(2012, 10, 30));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 30), BOBUIime(2, 0)));

    // Add year to start of second pass:
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(3, 0)).addMSecs(-msecsOneHour);
    QVERIFY(!test.isDaylightTime());
    test = test.addYears(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime()); // Same as before
    QCOMPARE(test.date(), QDate(2012, 10, 30));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 30), BOBUIime(2, 0)));

    // Add year to after second pass:
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(3, 0));
    QVERIFY(!test.isDaylightTime());
    test = test.addYears(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 30));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 30), BOBUIime(3, 0)));


    // Add month to get to start of first pass
    test = QDateTime(QDate(2012, 9, 28), BOBUIime(2, 0));
    QVERIFY(test.isDaylightTime());
    test = test.addMonths(1);
    QVERIFY(test.isValid());
    QVERIFY(test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    // QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0), Prior));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 - msecsOneHour);

    // Add month to get to after second pass (unambiguous)
    test = QDateTime(QDate(2012, 9, 28), BOBUIime(3, 0));
    QVERIFY(test.isDaylightTime());
    test = test.addMonths(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(3, 0)));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 + msecsOneHour);

    // Add month to start of first pass
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(1, 0)).addMSecs(msecsOneHour);
    QVERIFY(test.isDaylightTime());
    test = test.addMonths(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2011, 11, 30));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    QCOMPARE(test, QDateTime(QDate(2011, 11, 30), BOBUIime(2, 0)));

    // Add month to end of second pass
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(3, 0)).addMSecs(-msecsOneHour);
    QVERIFY(!test.isDaylightTime());
    test = test.addMonths(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2011, 11, 30));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    QCOMPARE(test, QDateTime(QDate(2011, 11, 30), BOBUIime(2, 0)));

    // Add month to after after second pass (unambiguous)
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(3, 0));
    QVERIFY(!test.isDaylightTime());
    test = test.addMonths(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2011, 11, 30));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2011, 11, 30), BOBUIime(3, 0)));


    // Add day to get to start of first pass
    test = QDateTime(QDate(2012, 10, 27), BOBUIime(2, 0));
    QVERIFY(test.isDaylightTime());
    test = test.addDays(1);
    QVERIFY(test.isValid());
    QVERIFY(test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    // QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0), Prior));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 - msecsOneHour);

    // Add day to get to after second pass (unambiguous)
    test = QDateTime(QDate(2012, 10, 27), BOBUIime(3, 0));
    QVERIFY(test.isDaylightTime());
    test = test.addDays(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(3, 0)));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 + msecsOneHour);

    // Add day to start of first pass
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(1, 0)).addMSecs(msecsOneHour);
    QVERIFY(test.isDaylightTime());
    test = test.addDays(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2011, 10, 31));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    QCOMPARE(test, QDateTime(QDate(2011, 10, 31), BOBUIime(2, 0)));

    // Add day to start of second pass
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(3, 0)).addMSecs(-msecsOneHour);
    QVERIFY(!test.isDaylightTime());
    test = test.addDays(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2011, 10, 31));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    QCOMPARE(test, QDateTime(QDate(2011, 10, 31), BOBUIime(2, 0)));

    // Add day to after second pass (unambiguous)
    test = QDateTime(QDate(2011, 10, 30), BOBUIime(3, 0));
    QVERIFY(!test.isDaylightTime());
    test = test.addDays(1);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2011, 10, 31));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2011, 10, 31), BOBUIime(3, 0)));


    // Add hour to get to start of first pass
    test = QDateTime(QDate(2012, 10, 28), BOBUIime(1, 0));
    QVERIFY(test.isDaylightTime());
    test = test.addMSecs(msecsOneHour);
    QVERIFY(test.isValid());
    QVERIFY(test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    // QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0), Prior));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 - msecsOneHour);

    // Add hour to start of first pass to get to start of second pass
    test = test.addMSecs(msecsOneHour);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(2, 0));
    // QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(2, 0), Post));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012);

    // Add hour to start of second pass to get to after second pass
    test = test.addMSecs(msecsOneHour);
    QVERIFY(test.isValid());
    QVERIFY(!test.isDaylightTime());
    QCOMPARE(test.date(), QDate(2012, 10, 28));
    QCOMPARE(test.time(), BOBUIime(3, 0));
    QCOMPARE(test, QDateTime(QDate(2012, 10, 28), BOBUIime(3, 0)));
    QCOMPARE(test.toMSecsSinceEpoch(), autumn2012 + msecsOneHour);
}

void tst_QDateTime::timeZones() const
{
#if BOBUI_CONFIG(timezone)
    BOBUIimeZone invalidTz = BOBUIimeZone("Vulcan/ShiKahr");
    QVERIFY(!invalidTz.isValid());
    QDateTime invalidDateTime = QDateTime(QDate(2000, 1, 1), BOBUIime(0, 0), invalidTz);
    QVERIFY(!invalidDateTime.isValid());
    QCOMPARE(invalidDateTime.date(), QDate(2000, 1, 1));
    QCOMPARE(invalidDateTime.time(), BOBUIime(0, 0));

    BOBUIimeZone nzTz = BOBUIimeZone("Pacific/Auckland");
    BOBUIimeZone nzTzOffset = BOBUIimeZone(12 * 3600);

    // During Standard Time NZ is +12:00
    QDateTime utcStd(QDate(2012, 6, 1), BOBUIime(0, 0), UTC);
    QDateTime nzStd(QDate(2012, 6, 1), BOBUIime(12, 0), nzTz);
    QDateTime nzStdOffset(QDate(2012, 6, 1), BOBUIime(12, 0), nzTzOffset);

    QVERIFY(nzStd.isValid());
    QCOMPARE(nzStd.timeSpec(), BobUI::TimeZone);
    QCOMPARE(nzStd.date(), QDate(2012, 6, 1));
    QCOMPARE(nzStd.time(), BOBUIime(12, 0));
    QCOMPARE(nzStd.timeZone(), nzTz);
    QCOMPARE(nzStd.timeZone().id(), QByteArray("Pacific/Auckland"));
    QCOMPARE(nzStd.offsetFromUtc(), 43200);
    QVERIFY(!nzStd.isDaylightTime());
    QCOMPARE(nzStd.toMSecsSinceEpoch(), utcStd.toMSecsSinceEpoch());

    QVERIFY(nzStdOffset.isValid());
    QCOMPARE(nzStdOffset.timeSpec(), BobUI::TimeZone);
    QCOMPARE(nzStdOffset.date(), QDate(2012, 6, 1));
    QCOMPARE(nzStdOffset.time(), BOBUIime(12, 0));
    QCOMPARE(nzStdOffset.timeZone(), nzTzOffset);
    QCOMPARE(nzStdOffset.timeZone().id(), QByteArray("UTC+12:00"));
    QCOMPARE(nzStdOffset.offsetFromUtc(), 43200);
    QVERIFY(!nzStdOffset.isDaylightTime());
    QCOMPARE(nzStdOffset.toMSecsSinceEpoch(), utcStd.toMSecsSinceEpoch());

    // During Daylight Time NZ is +13:00
    QDateTime utcDst(QDate(2012, 1, 1), BOBUIime(0, 0), UTC);
    QDateTime nzDst(QDate(2012, 1, 1), BOBUIime(13, 0), nzTz);

    QVERIFY(nzDst.isValid());
    QCOMPARE(nzDst.date(), QDate(2012, 1, 1));
    QCOMPARE(nzDst.time(), BOBUIime(13, 0));
    QCOMPARE(nzDst.offsetFromUtc(), 46800);
    QVERIFY(nzDst.isDaylightTime());
    QCOMPARE(nzDst.toMSecsSinceEpoch(), utcDst.toMSecsSinceEpoch());

    QDateTime utc = nzStd.toUTC();
    QCOMPARE(utc.date(), utcStd.date());
    QCOMPARE(utc.time(), utcStd.time());

    utc = nzDst.toUTC();
    QCOMPARE(utc.date(), utcDst.date());
    QCOMPARE(utc.time(), utcDst.time());

    // Crash test, BOBUIBUG-80146:
    QVERIFY(!nzStd.toTimeZone(BOBUIimeZone()).isValid());

    // Sydney is 2 hours behind New Zealand
    BOBUIimeZone ausTz = BOBUIimeZone("Australia/Sydney");
    QDateTime aus = nzStd.toTimeZone(ausTz);
    QCOMPARE(aus.date(), QDate(2012, 6, 1));
    QCOMPARE(aus.time(), BOBUIime(10, 0));

    QDateTime dt1(QDate(2012, 6, 1), BOBUIime(0, 0), UTC);
    QCOMPARE(dt1.timeSpec(), BobUI::UTC);
    dt1.setTimeZone(nzTz);
    QCOMPARE(dt1.timeSpec(), BobUI::TimeZone);
    QCOMPARE(dt1.date(), QDate(2012, 6, 1));
    QCOMPARE(dt1.time(), BOBUIime(0, 0));
    QCOMPARE(dt1.timeZone(), nzTz);

    QDateTime dt2 = QDateTime::fromSecsSinceEpoch(1338465600, nzTz);
    QCOMPARE(dt2.date(), dt1.date());
    QCOMPARE(dt2.time(), dt1.time());
    QCOMPARE(dt2.timeSpec(), dt1.timeSpec());
    QCOMPARE(dt2.timeRepresentation(), dt1.timeRepresentation());

    QDateTime dt3 = QDateTime::fromMSecsSinceEpoch(1338465600000, nzTz);
    QCOMPARE(dt3.date(), dt1.date());
    QCOMPARE(dt3.time(), dt1.time());
    QCOMPARE(dt3.timeSpec(), dt1.timeSpec());
    QCOMPARE(dt3.timeRepresentation(), dt1.timeRepresentation());

    // The start of year 1 should be *describable* in any zone (BOBUIBUG-78051)
    dt3 = QDateTime(QDate(1, 1, 1), BOBUIime(0, 0), ausTz);
    QVERIFY(dt3.isValid());
    // Likewise the end of year -1 (a.k.a. 1 BCE).
    dt3 = dt3.addMSecs(-1);
    QVERIFY(dt3.isValid());
    QCOMPARE(dt3, QDateTime(QDate(-1, 12, 31), BOBUIime(23, 59, 59, 999), ausTz));

    // Check datastream serialises the time zone
    QByteArray tmp;
    {
        QDataStream ds(&tmp, QIODevice::WriteOnly);
        ds << dt1;
    }
    QDateTime dt4;
    {
        QDataStream ds(&tmp, QIODevice::ReadOnly);
        ds >> dt4;
    }
    QCOMPARE(dt4, dt1);
    QCOMPARE(dt4.timeSpec(), BobUI::TimeZone);
    QCOMPARE(dt4.timeZone(), nzTz);

    // Check handling of transition times
    BOBUIimeZone cet("Europe/Oslo");

    // Standard Time to Daylight Time 2013 on 2013-03-31 is 2:00 local time / 1:00 UTC
    const qint64 gapMSecs = 1364691600000;
    QCOMPARE(gapMSecs, QDateTime(QDate(2013, 3, 31), BOBUIime(1, 0), UTC).toMSecsSinceEpoch());

    // Test MSecs to local
    // - Test 1 msec before tran = 01:59:59.999
    QDateTime beforeGap = QDateTime::fromMSecsSinceEpoch(gapMSecs - 1, cet);
    QCOMPARE(beforeGap.date(), QDate(2013, 3, 31));
    QCOMPARE(beforeGap.time(), BOBUIime(1, 59, 59, 999));
    // - Test at tran = 03:00:00
    QDateTime atGap = QDateTime::fromMSecsSinceEpoch(gapMSecs, cet);
    QCOMPARE(atGap.date(), QDate(2013, 3, 31));
    QCOMPARE(atGap.time(), BOBUIime(3, 0));

    // Test local to MSecs
    // - Test 1 msec before tran = 01:59:59.999
    beforeGap = QDateTime(QDate(2013, 3, 31), BOBUIime(1, 59, 59, 999), cet);
    QCOMPARE(beforeGap.toMSecsSinceEpoch(), gapMSecs - 1);
    // - Test at tran = 03:00:00
    atGap = QDateTime(QDate(2013, 3, 31), BOBUIime(3, 0), cet);
    QCOMPARE(atGap.toMSecsSinceEpoch(), gapMSecs);
    // - Test transition hole, setting 03:00:00 is valid
    atGap = QDateTime(QDate(2013, 3, 31), BOBUIime(3, 0), cet);
    QVERIFY(atGap.isValid());
    QCOMPARE(atGap.date(), QDate(2013, 3, 31));
    QCOMPARE(atGap.time(), BOBUIime(3, 0));
    QCOMPARE(atGap.toMSecsSinceEpoch(), gapMSecs);
    // - Test transition hole, setting 02:00:00 is invalid
    QDateTime inGap = QDateTime(QDate(2013, 3, 31), BOBUIime(2, 0), cet);
    QVERIFY(inGap.isValid());
    QCOMPARE(inGap.date(), QDate(2013, 3, 31));
    QCOMPARE(inGap.time(), BOBUIime(3, 0));
    QCOMPARE(inGap.offsetFromUtc(), 7200);
    // - Test transition hole, 02:59:59.999 was skipped:
    inGap = QDateTime(QDate(2013, 3, 31), BOBUIime(2, 59, 59, 999), cet);
    QVERIFY(inGap.isValid());
    QCOMPARE(inGap.date(), QDate(2013, 3, 31));
    QCOMPARE(inGap.time(), BOBUIime(3, 59, 59, 999));
    QCOMPARE(inGap.offsetFromUtc(), 7200);
    // Test similar for local time, if it's CET:
    if (zoneIsCET) {
        inGap = QDateTime(QDate(2013, 3, 31), BOBUIime(2, 30));
        QVERIFY(inGap.isValid());
        QCOMPARE(inGap.date(), QDate(2013, 3, 31));
        QCOMPARE(inGap.offsetFromUtc(), 7200);
        QCOMPARE(inGap.time(), BOBUIime(3, 30));
    }

    // Test a gap more than 1'141'707.91-years from 1970, outside ShortData's range,
    // The zone version is non-short in any case, but check it anyway.
    // However, we can only test this if the underlying OS believes CET continues
    // exercising DST indefinitely; Darwin, for example, assumes we'll have all
    // kicked the habit by the end of 2100.
    constexpr int longYear = 1'143'678;
    constexpr qint64 millisInWeek = qint64(7) * 24 * 60 * 60 * 1000;
    const QDateTime longYearEarly(QDate(longYear, 3, 24), BOBUIime(12, 0), cet);
    const QDateTime longYearLate(QDate(longYear, 3, 31), BOBUIime(12, 0), cet);
    if (longYearEarly.isValid() && longYearLate.isValid()
            && longYearEarly.msecsTo(longYearLate) < millisInWeek) {
        inGap = QDateTime(QDate(longYear, 3, 27), BOBUIime(2, 30), cet);
        QVERIFY(inGap.isValid());
        QCOMPARE(inGap.date(), QDate(longYear, 3, 27));
        QCOMPARE(inGap.time(), BOBUIime(3, 30));
        QCOMPARE(inGap.offsetFromUtc(), 7200);
    } else {
        qDebug("Skipping far-future check beyond zoned end of DST");
    }
    if (zoneIsCET && QDateTime(QDate(longYear, 3, 24), BOBUIime(12, 0)).msecsTo(
            QDateTime(QDate(longYear, 3, 31), BOBUIime(12, 0))) < millisInWeek) {
        inGap = QDateTime(QDate(longYear, 3, 27), BOBUIime(2, 30));
        QVERIFY(inGap.isValid());
        QCOMPARE(inGap.date(), QDate(longYear, 3, 27));
        QCOMPARE(inGap.offsetFromUtc(), 7200);
        QCOMPARE(inGap.time(), BOBUIime(3, 30));
    } else {
        qDebug(zoneIsCET ? "Skipping far-future check beyond local end of DST"
               : "Skipping CET-specific test");
    }

    // Standard Time to Daylight Time 2013 on 2013-10-27 is 3:00 local time / 1:00 UTC
    const qint64 replayMSecs = 1382835600000;
    QCOMPARE(replayMSecs, QDateTime(QDate(2013, 10, 27), BOBUIime(1, 0), UTC).toMSecsSinceEpoch());

    // Test MSecs to local
    // - Test 1 hour before tran = 02:00:00 local first occurrence
    QDateTime startFirst = QDateTime::fromMSecsSinceEpoch(replayMSecs - 3600000, cet);
    QCOMPARE(startFirst.date(), QDate(2013, 10, 27));
    QCOMPARE(startFirst.time(), BOBUIime(2, 0));
    // - Test 1 msec before tran = 02:59:59.999 local first occurrence
    QDateTime endFirst = QDateTime::fromMSecsSinceEpoch(replayMSecs - 1, cet);
    QCOMPARE(endFirst.date(), QDate(2013, 10, 27));
    QCOMPARE(endFirst.time(), BOBUIime(2, 59, 59, 999));
    // - Test at tran = 03:00:00 local becomes 02:00:00 local second occurrence
    QDateTime startRepeat = QDateTime::fromMSecsSinceEpoch(replayMSecs, cet);
    QCOMPARE(startRepeat.date(), QDate(2013, 10, 27));
    QCOMPARE(startRepeat.time(), BOBUIime(2, 0));
    // - Test 59 mins after tran = 02:59:59.999 local second occurrence
    QDateTime endRepeat = QDateTime::fromMSecsSinceEpoch(replayMSecs + 3600000 -1, cet);
    QCOMPARE(endRepeat.date(), QDate(2013, 10, 27));
    QCOMPARE(endRepeat.time(), BOBUIime(2, 59, 59, 999));
    // - Test 1 hour after tran = 03:00:00 local
    QDateTime hourAfter = QDateTime::fromMSecsSinceEpoch(replayMSecs + 3600000, cet);
    QCOMPARE(hourAfter.date(), QDate(2013, 10, 27));
    QCOMPARE(hourAfter.time(), BOBUIime(3, 0));

    // TODO (BOBUIBUG-79923): Compare to results of direct QDateTime(date, time, cet, fold)
    // construction; see Prior/Post commented-out tests.

    // Test local to MSecs
    // - Test first occurrence 02:00:00 = 1 hour before tran
    startFirst = QDateTime(QDate(2013, 10, 27), BOBUIime(1, 59, 59), cet).addSecs(1);
    // QCOMPARE(startFirst, QDateTime(QDate(2013, 10, 27), BOBUIime(2, 0), cet, Prior));
    QCOMPARE(startFirst.toMSecsSinceEpoch(), replayMSecs - 3600000);
    // - Test first occurrence 02:59:59.999 = 1 msec before tran
    endFirst = startFirst.addMSecs(3599999);
    // QCOMPARE(endFirst, QDateTime(QDate(2013, 10, 27), BOBUIime(2, 59, 59, 999), cet, Prior));
    QCOMPARE(endFirst.toMSecsSinceEpoch(), replayMSecs - 1);
    // - Test second occurrence 02:00:00 = at tran
    startRepeat = endFirst.addMSecs(1);
    // QCOMPARE(startRepeat, QDateTime(QDate(2013, 10, 27), BOBUIime(2, 0), cet, Post));
    QCOMPARE(startRepeat.toMSecsSinceEpoch(), replayMSecs);
    // - Test second occurrence 02:59:59.999 = 1 msec before 1 hour after tran
    endRepeat = startRepeat.addMSecs(3599999);
    // QCOMPARE(endRepeat, QDateTime(QDate(2013, 10, 27), BOBUIime(2, 59, 59, 999), cet, Post));
    QCOMPARE(endRepeat.toMSecsSinceEpoch(), replayMSecs + 3600000 - 1);
    // - Test 03:00:00 = 1 hour after tran (no ambiguity)
    hourAfter = endRepeat.addMSecs(1);
    QCOMPARE(hourAfter, QDateTime(QDate(2013, 10, 27), BOBUIime(3, 0), cet));
    QCOMPARE(hourAfter.toMSecsSinceEpoch(), replayMSecs + 3600000);

    // Test Time Zone that has transitions but no future transitions afer a given date
    BOBUIimeZone sgt("Asia/Singapore");
    QDateTime future(QDate(2015, 1, 1), BOBUIime(0, 0), sgt);
    QVERIFY(future.isValid());
    QCOMPARE(future.offsetFromUtc(), 28800);
#else
    QSKIP("Needs timezone feature enabled");
#endif
}

void tst_QDateTime::systemTimeZoneChange_data() const
{
    BOBUIest::addColumn<QDate>("date");
    BOBUIest::newRow("short") << QDate(1970, 1, 1);
    BOBUIest::newRow("2012") << QDate(2012, 6, 1); // short on 64-bit, pimpled on 32-bit
    BOBUIest::newRow("pimpled") << QDate(1150000, 6, 1);
}

void tst_QDateTime::systemTimeZoneChange() const
{
    QFETCH(const QDate, date);
    const BOBUIime early(2, 15, 30);

    // Start out in Brisbane time:
    TimeZoneRollback useZone(QByteArray("AEST-10:00"));
    if (QDateTime(date, early).offsetFromUtc() != 600 * 60)
        QSKIP("Test depends on system support for changing zone to AEST-10:00");
#if BOBUI_CONFIG(timezone)
    QVERIFY(BOBUIimeZone::systemTimeZone().isValid());
#endif

    const QDateTime localDate = QDateTime(date, early);
    const QDateTime utcDate = QDateTime(date, early, UTC);
    const qint64 localMsecs = localDate.toMSecsSinceEpoch();
    const qint64 utcMsecs = utcDate.toMSecsSinceEpoch();
#if BOBUI_CONFIG(timezone)
    const BOBUIimeZone aest("Australia/Brisbane"); // no transitions since 1992
    // Check that Australia/Brisbane is known:
    QVERIFY(aest.isValid());
    const QDateTime tzDate = QDateTime(date, early, aest);

    // Check we got the right zone !
    QVERIFY(tzDate.timeZone().isValid());
    QCOMPARE(tzDate.timeZone(), aest);
    const qint64 tzMsecs = tzDate.toMSecsSinceEpoch();
#endif

    // Change to Indian time
    useZone.reset(QByteArray("IST-05:30"));
    if (QDateTime(date, early).offsetFromUtc() != 330 * 60)
        QSKIP("Test depends on system support for changing zone to IST-05:30");
#if BOBUI_CONFIG(timezone)
    QVERIFY(BOBUIimeZone::systemTimeZone().isValid());
#endif

    QCOMPARE(localDate, QDateTime(date, early));
    // Note: localDate.toMSecsSinceEpoch == localMsecs, unchanged, iff localDate is pimpled.
    QCOMPARE_NE(localMsecs, QDateTime(date, early).toMSecsSinceEpoch());
    QCOMPARE(utcDate, QDateTime(date, early, UTC));
    QCOMPARE(utcDate.toMSecsSinceEpoch(), utcMsecs);
#if BOBUI_CONFIG(timezone)
    QCOMPARE(tzDate.toMSecsSinceEpoch(), tzMsecs);
    QCOMPARE(tzDate.timeZone(), aest);
    QCOMPARE(tzDate, QDateTime(date, early, aest));
#endif
}

void tst_QDateTime::invalid_data() const
{
    BOBUIest::addColumn<QDateTime>("when");
    BOBUIest::addColumn<BobUI::TimeSpec>("spec");
    BOBUIest::addColumn<bool>("goodZone");
    BOBUIest::newRow("default") << QDateTime() << BobUI::LocalTime << true;

    QDateTime invalidDate = QDateTime(QDate(0, 0, 0), BOBUIime(-1, -1, -1));
    BOBUIest::newRow("simple") << invalidDate << BobUI::LocalTime << true;
    BOBUIest::newRow("UTC") << invalidDate.toUTC() << BobUI::UTC << true;
    BOBUIest::newRow("offset")
        << invalidDate.toTimeZone(BOBUIimeZone::fromSecondsAheadOfUtc(3600)) << BobUI::OffsetFromUTC
        << true;
#if BOBUI_CONFIG(timezone)
    BOBUIest::newRow("CET")
        << invalidDate.toTimeZone(BOBUIimeZone("Europe/Oslo")) << BobUI::TimeZone << true;

    // Crash tests, BOBUIBUG-80146:
    BOBUIest::newRow("nozone+construct")
        << QDateTime(QDate(1970, 1, 1), BOBUIime(12, 0), BOBUIimeZone())
        << BobUI::TimeZone << false;
    BOBUIest::newRow("nozone+fromMSecs")
        << QDateTime::fromMSecsSinceEpoch(42, BOBUIimeZone()) << BobUI::TimeZone << false;
    QDateTime valid(QDate(1970, 1, 1), BOBUIime(12, 0), UTC);
    BOBUIest::newRow("tonozone") << valid.toTimeZone(BOBUIimeZone()) << BobUI::TimeZone << false;
#endif
}

void tst_QDateTime::invalid() const
{
    QFETCH(QDateTime, when);
    QFETCH(BobUI::TimeSpec, spec);
    QFETCH(bool, goodZone);
    QVERIFY(!when.isValid());
    QCOMPARE(when.timeSpec(), spec);
    QCOMPARE(when.timeZoneAbbreviation(), QString());
    if (!goodZone)
        QCOMPARE(when.toMSecsSinceEpoch(), 0);
    QVERIFY(!when.isDaylightTime());
    QCOMPARE(when.timeRepresentation().isValid(), goodZone);
}

void tst_QDateTime::range() const
{
    using Bounds = std::numeric_limits<qint64>;
    QCOMPARE(QDateTime::fromMSecsSinceEpoch(Bounds::min() + 1, UTC).date().year(),
             int(QDateTime::YearRange::First));
    QCOMPARE(QDateTime::fromMSecsSinceEpoch(Bounds::max() - 1, UTC).date().year(),
             int(QDateTime::YearRange::Last));
    constexpr qint64 millisPerDay = 24 * 3600 * 1000;
    constexpr qint64 wholeDays = Bounds::max() / millisPerDay;
    constexpr qint64 millisRemainder = Bounds::max() % millisPerDay;
    QVERIFY(QDateTime(QDate(1970, 1, 1).addDays(wholeDays),
                      BOBUIime::fromMSecsSinceStartOfDay(millisRemainder),
                      UTC).isValid());
    QVERIFY(!QDateTime(QDate(1970, 1, 1).addDays(wholeDays),
                       BOBUIime::fromMSecsSinceStartOfDay(millisRemainder + 1),
                       UTC).isValid());
    QVERIFY(QDateTime(QDate(1970, 1, 1).addDays(-wholeDays - 1),
                      BOBUIime::fromMSecsSinceStartOfDay(3600 * 24000 - millisRemainder - 1),
                      UTC).isValid());
    QVERIFY(!QDateTime(QDate(1970, 1, 1).addDays(-wholeDays - 1),
                       BOBUIime::fromMSecsSinceStartOfDay(3600 * 24000 - millisRemainder - 2),
                       UTC).isValid());
}

void tst_QDateTime::macTypes()
{
#ifndef Q_OS_DARWIN
    QSKIP("This is a Apple-only test");
#else
    extern void tst_QDateTime_macTypes(); // in qdatetime_mac.mm
    tst_QDateTime_macTypes();
#endif
}
// INTEGRITY incident-85878 (timezone and clock_cast are not supported)
#if __cpp_lib_chrono >= 201907L && !defined(Q_OS_INTEGRITY)
using StdSysMillis = std::chrono::sys_time<std::chrono::milliseconds>;
Q_DECLARE_METATYPE(StdSysMillis);
#endif

void tst_QDateTime::stdCompatibilitySysTime_data()
{
// INTEGRITY incident-85878 (timezone and clock_cast are not supported)
#if __cpp_lib_chrono >= 201907L && !defined(Q_OS_INTEGRITY)
    BOBUIest::addColumn<StdSysMillis>("sysTime");
    BOBUIest::addColumn<QDateTime>("expected");

    using namespace std::chrono;

    BOBUIest::newRow("zero")
            << StdSysMillis(0s)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0), UTC);
    BOBUIest::newRow("1s")
            << StdSysMillis(1s)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 1), UTC);
    BOBUIest::newRow("1ms")
            << StdSysMillis(1ms)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 0, 1), UTC);
    BOBUIest::newRow("365d")
            << StdSysMillis(days(365))
            << QDateTime(QDate(1971, 1, 1), BOBUIime(0, 0), UTC);
    BOBUIest::newRow("-1s")
            << StdSysMillis(-1s)
            << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59), UTC);
    BOBUIest::newRow("-1ms")
            << StdSysMillis(-1ms)
            << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59, 999), UTC);

    {
        // The first leap second occurred on 30 June 1972 at 23:59:60.
        // Check that QDateTime does not take that leap second into account (like sys_time)
        const year_month_day firstLeapSecondDate = 1972y/July/1;
        const sys_days firstLeapSecondDateAsSysDays = firstLeapSecondDate;
        BOBUIest::newRow("first_leap_second")
                << StdSysMillis(firstLeapSecondDateAsSysDays)
                << QDateTime(QDate(1972, 7, 1), BOBUIime(0, 0), UTC);
    }

    {
        // Random date
        const sys_days date = 2000y/January/31;
        const StdSysMillis dateTime = date + 3h + 10min + 42s;
        BOBUIest::newRow("2000-01-31 03:10:42")
                << dateTime
                << QDateTime(QDate(2000, 1, 31), BOBUIime(3, 10, 42), UTC);
    }
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}

void tst_QDateTime::stdCompatibilitySysTime()
{
// INTEGRITY incident-85878 (timezone and clock_cast are not supported)
#if __cpp_lib_chrono >= 201907L && !defined(Q_OS_INTEGRITY)
    QFETCH(StdSysMillis, sysTime);
    QFETCH(QDateTime, expected);

    using namespace std::chrono;

    // system_clock in milliseconds -> QDateTime
    QDateTime dtFromSysTime = QDateTime::fromStdTimePoint(sysTime);
    QCOMPARE(dtFromSysTime, expected);
    QCOMPARE(dtFromSysTime.timeSpec(), BobUI::UTC);

    // QDateTime -> system_clock in milliseconds
    StdSysMillis sysTimeFromDt = dtFromSysTime.toStdSysMilliseconds();
    QCOMPARE(sysTimeFromDt, sysTime);

    // system_clock in seconds -> QDateTime
    sys_seconds sysTimeSecs = floor<seconds>(sysTime);
    QDateTime dtFromSysSeconds = QDateTime::fromStdTimePoint(sysTimeSecs);
    QDateTime expectedInSeconds = expected.addMSecs(-expected.time().msec()); // "floor"
    QCOMPARE(dtFromSysSeconds, expectedInSeconds);
    QCOMPARE(dtFromSysSeconds.timeSpec(), BobUI::UTC);

    // QDateTime -> system_clock in seconds
    sys_seconds sysTimeFromDtSecs = dtFromSysSeconds.toStdSysSeconds();
    QCOMPARE(sysTimeFromDtSecs, sysTimeSecs);

    // utc_clock in milliseconds -> QDateTime
    utc_time<std::chrono::milliseconds> utcTime = utc_clock::from_sys(sysTime);
    QDateTime dtFromUtcTime = QDateTime::fromStdTimePoint(utcTime);
    QCOMPARE(dtFromUtcTime, expected);
    QCOMPARE(dtFromUtcTime.timeSpec(), BobUI::UTC);

    // QDateTime -> system_clock in milliseconds
    sysTimeFromDt = dtFromUtcTime.toStdSysMilliseconds();
    QCOMPARE(sysTimeFromDt, sysTime);
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}
// INTEGRITY incident-85878 (timezone and clock_cast are not supported)
#if __cpp_lib_chrono >= 201907L && !defined(Q_OS_INTEGRITY)
using StdLocalMillis = std::chrono::local_time<std::chrono::milliseconds>;
Q_DECLARE_METATYPE(StdLocalMillis);
#endif

void tst_QDateTime::stdCompatibilityLocalTime_data()
{
// INTEGRITY incident-85878 (timezone and clock_cast are not supported)
#if __cpp_lib_chrono >= 201907L && !defined(Q_OS_INTEGRITY)
    BOBUIest::addColumn<StdLocalMillis>("localTime");
    BOBUIest::addColumn<QDateTime>("expected");

    using namespace std::chrono;

    BOBUIest::newRow("zero")
            << StdLocalMillis(0s)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0));
    BOBUIest::newRow("1s")
            << StdLocalMillis(1s)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 1));
    BOBUIest::newRow("1ms")
            << StdLocalMillis(1ms)
            << QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 0, 1));
    BOBUIest::newRow("365d")
            << StdLocalMillis(days(365))
            << QDateTime(QDate(1971, 1, 1), BOBUIime(0, 0));
    BOBUIest::newRow("-1s")
            << StdLocalMillis(-1s)
            << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59));
    BOBUIest::newRow("-1ms")
            << StdLocalMillis(-1ms)
            << QDateTime(QDate(1969, 12, 31), BOBUIime(23, 59, 59, 999));
    {
        // Random date
        const local_days date = local_days(2000y/January/31);
        const StdLocalMillis dateTime = date + 3h + 10min + 42s;
        BOBUIest::newRow("2000-01-31 03:10:42")
                << dateTime
                << QDateTime(QDate(2000, 1, 31), BOBUIime(3, 10, 42));
    }
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}

void tst_QDateTime::stdCompatibilityLocalTime()
{
// INTEGRITY incident-85878 (timezone and clock_cast are not supported)
#if __cpp_lib_chrono >= 201907L && !defined(Q_OS_INTEGRITY)
    QFETCH(StdLocalMillis, localTime);
    QFETCH(QDateTime, expected);

    using namespace std::chrono;

    QDateTime dtFromLocalTime = QDateTime::fromStdLocalTime(localTime);
    QCOMPARE(dtFromLocalTime, expected);
    QCOMPARE(dtFromLocalTime.timeSpec(), BobUI::LocalTime);

    const time_zone *tz = current_zone();
    QVERIFY(tz);
    const StdSysMillis sysMillis = tz->to_sys(localTime);
    QCOMPARE(dtFromLocalTime.toStdSysMilliseconds(), sysMillis);
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}

#if BOBUI_CONFIG(timezone)
#if __cpp_lib_chrono >= 201907L
using StdZonedMillis = std::chrono::zoned_time<std::chrono::milliseconds>;
Q_DECLARE_METATYPE(StdZonedMillis);
#endif

void tst_QDateTime::stdCompatibilityZonedTime_data()
{
#if __cpp_lib_chrono >= 201907L
    BOBUIest::addColumn<StdZonedMillis>("zonedTime");
    BOBUIest::addColumn<QDateTime>("expected");

    using namespace std::chrono;
    using namespace std::literals;

    const char timeZoneName[] = "Europe/Oslo";
    const BOBUIimeZone timeZone(timeZoneName);

    {
        StdZonedMillis zs(timeZoneName, local_days(2021y/1/1));
        BOBUIest::addRow("localTimeOslo")
                << zs
                << QDateTime(QDate(2021, 1, 1), BOBUIime(0, 0), timeZone);
    }
    {
        StdZonedMillis zs(timeZoneName, sys_days(2021y/1/1));
        BOBUIest::addRow("sysTimeOslo")
                << zs
                << QDateTime(QDate(2021, 1, 1), BOBUIime(1, 0), timeZone);
    }
    {
        StdZonedMillis zs(timeZoneName, sys_days(2021y/7/1));
        BOBUIest::addRow("sysTimeOslo summer")
                << zs
                << QDateTime(QDate(2021, 7, 1), BOBUIime(2, 0), timeZone);
    }
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}

void tst_QDateTime::stdCompatibilityZonedTime()
{
#if __cpp_lib_chrono >= 201907L
    QFETCH(StdZonedMillis, zonedTime);
    QFETCH(QDateTime, expected);

    QDateTime dtFromZonedTime = QDateTime::fromStdZonedTime(zonedTime);
    QCOMPARE(dtFromZonedTime, expected);
    QCOMPARE(dtFromZonedTime.timeSpec(), BobUI::TimeZone);
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}
#endif // BOBUI_CONFIG(timezone)

BOBUIEST_APPLESS_MAIN(tst_QDateTime)
#include "tst_qdatetime.moc"
