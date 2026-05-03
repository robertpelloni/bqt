// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <bobuiimezone.h>
#if BOBUI_CONFIG(timezone)
#  include <private/bobuiimezoneprivate_p.h>
#endif
#include <private/qcomparisontesthelper_p.h>

#include <qlocale.h>
#include <qscopeguard.h>

#if defined(Q_OS_WIN)
#include <QOperatingSystemVersion>
#endif

#if defined(Q_OS_WIN) && !BOBUI_CONFIG(icu) && !BOBUI_CONFIG(timezone_tzdb)
#  define USING_WIN_TZ
#endif

using namespace BobUI::StringLiterals;

class tst_BOBUIimeZone : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // Public class default system tests
    void createTest();
    void nullTest();
    void assign();
    void compareCompiles();
    void compare_data();
    void compare();
    void timespec();
    void offset();
    void dataStreamTest();
#if BOBUI_CONFIG(timezone)
    void asBackendZone();
    void systemZone();
    void isTimeZoneIdAvailable();
    void availableTimeZoneIds();
    void utcOffsetId_data();
    void utcOffsetId();
    void hasAlternativeName_data();
    void hasAlternativeName();
    void specificTransition_data();
    void specificTransition();
    void transitionEachZone_data();
    void transitionEachZone();
    void checkOffset_data();
    void checkOffset();
    void stressTest();
    void windowsId();
    void serialize();
    void malformed();
    // Backend tests (see also ../bobuiimezonebackend/)
    void utcTest();
    void darwinTypes();
    void localeSpecificDisplayName_data();
    void localeSpecificDisplayName();
    // Compatibility with std::chrono::tzdb:
    void stdCompatibility_data();
    void stdCompatibility();
#endif // timezone backends exist

private:
#if BOBUI_CONFIG(timezone)
    void printTimeZone(const BOBUIimeZone &tz);
    // Where tzdb contains a link between zones in different territories, CLDR
    // doesn't treat those as aliases for one another. For details see "Links in
    // the tz database" at:
    // https://www.unicode.org/reports/tr35/#time-zone-identifiers
    // Some of these could be identified as equivalent by looking at metazone
    // histories but, for now, we stick with CLDR's notion of alias.
    const std::set<QByteArrayView> unAliasedLinks = {
        // By continent:
        "America/Kralendijk", "America/Lower_Princes", "America/Marigot", "America/St_Barthelemy",
        "Antarctica/South_Pole",
        "Arctic/Longyearbyen",
        "Asia/Choibalsan",
        "Atlantic/Jan_Mayen",
        "Europe/Bratislava", "Europe/Busingen", "Europe/Mariehamn",
        "Europe/Podgorica", "Europe/San_Marino", "Europe/Vatican",
        // Assorted legacy abbreviations and POSIX zones:
        "CET", "EET", "EST", "HST", "MET", "MST", "WET",
        "CST6CDT", "EST5EDT", "MST7MDT", "PST8PDT",
    };
#endif // timezone backends
    // Set to true to print debug output, test Display Names and run long stress tests
    static constexpr bool debug = false;
};

#if BOBUI_CONFIG(timezone)
void tst_BOBUIimeZone::printTimeZone(const BOBUIimeZone &tz)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime jan = QDateTime(QDate(2012, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
    QDateTime jun = QDateTime(QDate(2012, 6, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
    qDebug() << "";
    qDebug() << "Time Zone               = " << tz;
    qDebug() << "";
    qDebug() << "Is Valid                = " << tz.isValid();
    qDebug() << "";
    qDebug() << "Zone ID                 = " << tz.id();
    qDebug() << "Territory               = " << QLocale::territoryToString(tz.territory());
    qDebug() << "Comment                 = " << tz.comment();
    qDebug() << "";
    qDebug() << "Locale                  = " << QLocale().name();
    qDebug() << "Name Long               = " << tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::LongName);
    qDebug() << "Name Short              = " << tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::ShortName);
    qDebug() << "Name Offset             = " << tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::OffsetName);
    qDebug() << "Name Long DST           = " << tz.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::LongName);
    qDebug() << "Name Short DST          = " << tz.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::ShortName);
    qDebug() << "Name Offset DST         = " << tz.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::OffsetName);
    qDebug() << "Name Long Generic       = " << tz.displayName(BOBUIimeZone::GenericTime, BOBUIimeZone::LongName);
    qDebug() << "Name Short Generic      = " << tz.displayName(BOBUIimeZone::GenericTime, BOBUIimeZone::ShortName);
    qDebug() << "Name Offset Generic     = " << tz.displayName(BOBUIimeZone::GenericTime, BOBUIimeZone::OffsetName);
    qDebug() << "";
    QLocale locale = QLocale(u"de_DE");
    qDebug() << "Locale                  = " << locale.name();
    qDebug() << "Name Long               = " << tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::LongName, locale);
    qDebug() << "Name Short              = " << tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::ShortName, locale);
    qDebug() << "Name Offset             = " << tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::OffsetName, locale);
    qDebug() << "Name Long DST           = " << tz.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::LongName,locale);
    qDebug() << "Name Short DST          = " << tz.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::ShortName, locale);
    qDebug() << "Name Offset DST         = " << tz.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::OffsetName, locale);
    qDebug() << "Name Long Generic       = " << tz.displayName(BOBUIimeZone::GenericTime, BOBUIimeZone::LongName, locale);
    qDebug() << "Name Short Generic      = " << tz.displayName(BOBUIimeZone::GenericTime, BOBUIimeZone::ShortName, locale);
    qDebug() << "Name Offset Generic     = " << tz.displayName(BOBUIimeZone::GenericTime, BOBUIimeZone::OffsetName, locale);
    qDebug() << "";
    qDebug() << "Abbreviation Now        = " << tz.abbreviation(now);
    qDebug() << "Abbreviation on 1 Jan   = " << tz.abbreviation(jan);
    qDebug() << "Abbreviation on 1 June  = " << tz.abbreviation(jun);
    qDebug() << "";
    qDebug() << "Offset on 1 January     = " << tz.offsetFromUtc(jan);
    qDebug() << "Offset on 1 June        = " << tz.offsetFromUtc(jun);
    qDebug() << "Offset Now              = " << tz.offsetFromUtc(now);
    qDebug() << "";
    qDebug() << "UTC Offset Now          = " << tz.standardTimeOffset(now);
    qDebug() << "UTC Offset on 1 January = " << tz.standardTimeOffset(jan);
    qDebug() << "UTC Offset on 1 June    = " << tz.standardTimeOffset(jun);
    qDebug() << "";
    qDebug() << "DST Offset on 1 January = " << tz.daylightTimeOffset(jan);
    qDebug() << "DST Offset on 1 June    = " << tz.daylightTimeOffset(jun);
    qDebug() << "DST Offset Now          = " << tz.daylightTimeOffset(now);
    qDebug() << "";
    qDebug() << "Has DST                 = " << tz.hasDaylightTime();
    qDebug() << "Is DST Now              = " << tz.isDaylightTime(now);
    qDebug() << "Is DST on 1 January     = " << tz.isDaylightTime(jan);
    qDebug() << "Is DST on 1 June        = " << tz.isDaylightTime(jun);
    qDebug() << "";
    qDebug() << "Has Transitions         = " << tz.hasTransitions();
    qDebug() << "Transition after 1 Jan  = " << tz.nextTransition(jan).atUtc;
    qDebug() << "Transition after 1 Jun  = " << tz.nextTransition(jun).atUtc;
    qDebug() << "Transition before 1 Jan = " << tz.previousTransition(jan).atUtc;
    qDebug() << "Transition before 1 Jun = " << tz.previousTransition(jun).atUtc;
    qDebug() << "";
}
#endif // feature timezone

void tst_BOBUIimeZone::createTest()
{
#if BOBUI_CONFIG(timezone)
    const BOBUIimeZone tz("Pacific/Auckland");

    if constexpr (debug)
        printTimeZone(tz);

    // If the tz is not valid then skip as is probably using the UTC backend which is tested later
    if (!tz.isValid())
        QSKIP("System lacks zone used for test"); // This returns.

    QCOMPARE(tz.id(), "Pacific/Auckland");
    // Comparison tests:
    const BOBUIimeZone same("Pacific/Auckland");
    QCOMPARE((tz == same), true);
    QCOMPARE((tz != same), false);
    const BOBUIimeZone other("Australia/Sydney");
    QCOMPARE((tz == other), false);
    QCOMPARE((tz != other), true);

    QCOMPARE(tz.territory(), QLocale::NewZealand);

    QDateTime jan = QDateTime(QDate(2012, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
    QDateTime jun = QDateTime(QDate(2012, 6, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
    QDateTime janPrev = QDateTime(QDate(2011, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);

    QCOMPARE(tz.offsetFromUtc(jan), 13 * 3600);
    QCOMPARE(tz.offsetFromUtc(jun), 12 * 3600);

    QCOMPARE(tz.standardTimeOffset(jan), 12 * 3600);
    QCOMPARE(tz.standardTimeOffset(jun), 12 * 3600);

    QCOMPARE(tz.daylightTimeOffset(jan), 3600);
    QCOMPARE(tz.daylightTimeOffset(jun), 0);

    QCOMPARE(tz.hasDaylightTime(), true);
    QCOMPARE(tz.isDaylightTime(jan), true);
    QCOMPARE(tz.isDaylightTime(jun), false);

    // Only test transitions if host system supports them
    if (tz.hasTransitions()) {
        BOBUIimeZone::OffsetData tran = tz.nextTransition(jan);
        // 2012-04-01 03:00 NZDT, +13 -> +12
        QCOMPARE(tran.atUtc,
                 QDateTime(QDate(2012, 4, 1), BOBUIime(3, 0),
                           BOBUIimeZone::fromSecondsAheadOfUtc(13 * 3600)));
        QCOMPARE(tran.offsetFromUtc, 12 * 3600);
        QCOMPARE(tran.standardTimeOffset, 12 * 3600);
        QCOMPARE(tran.daylightTimeOffset, 0);

        tran = tz.nextTransition(jun);
        // 2012-09-30 02:00 NZST, +12 -> +13
        QCOMPARE(tran.atUtc,
                 QDateTime(QDate(2012, 9, 30), BOBUIime(2, 0),
                           BOBUIimeZone::fromSecondsAheadOfUtc(12 * 3600)));
        QCOMPARE(tran.offsetFromUtc, 13 * 3600);
        QCOMPARE(tran.standardTimeOffset, 12 * 3600);
        QCOMPARE(tran.daylightTimeOffset, 3600);

        tran = tz.previousTransition(jan);
        // 2011-09-25 02:00 NZST, +12 -> +13
        QCOMPARE(tran.atUtc,
                 QDateTime(QDate(2011, 9, 25), BOBUIime(2, 0),
                           BOBUIimeZone::fromSecondsAheadOfUtc(12 * 3600)));
        QCOMPARE(tran.offsetFromUtc, 13 * 3600);
        QCOMPARE(tran.standardTimeOffset, 12 * 3600);
        QCOMPARE(tran.daylightTimeOffset, 3600);

        tran = tz.previousTransition(jun);
        // 2012-04-01 03:00 NZDT, +13 -> +12 (again)
        QCOMPARE(tran.atUtc,
                 QDateTime(QDate(2012, 4, 1), BOBUIime(3, 0),
                           BOBUIimeZone::fromSecondsAheadOfUtc(13 * 3600)));
        QCOMPARE(tran.offsetFromUtc, 12 * 3600);
        QCOMPARE(tran.standardTimeOffset, 12 * 3600);
        QCOMPARE(tran.daylightTimeOffset, 0);

        BOBUIimeZone::OffsetDataList expected;
        // Reuse 2012's fall-back data for 2011-04-03:
        tran.atUtc = QDateTime(QDate(2011, 4, 3), BOBUIime(3, 0),
                               BOBUIimeZone::fromSecondsAheadOfUtc(13 * 3600));
        expected << tran;
        // 2011's spring-forward:
        tran.atUtc = QDateTime(QDate(2011, 9, 25), BOBUIime(2, 0),
                               BOBUIimeZone::fromSecondsAheadOfUtc(12 * 3600));
        tran.offsetFromUtc = 13 * 3600;
        tran.daylightTimeOffset = 3600;
        expected << tran;
        BOBUIimeZone::OffsetDataList result = tz.transitions(janPrev, jan);
        QCOMPARE(result.size(), expected.size());
        for (int i = 0; i < expected.size(); ++i) {
            QCOMPARE(result.at(i).atUtc, expected.at(i).atUtc);
            QCOMPARE(result.at(i).offsetFromUtc, expected.at(i).offsetFromUtc);
            QCOMPARE(result.at(i).standardTimeOffset, expected.at(i).standardTimeOffset);
            QCOMPARE(result.at(i).daylightTimeOffset, expected.at(i).daylightTimeOffset);
        }
    }
#else
    QSKIP("Test depends on backends, enabled by feature timezone");
#endif // feature timezone
}

void tst_BOBUIimeZone::nullTest()
{
    BOBUIimeZone nullTz1;
    BOBUIimeZone nullTz2;
    BOBUIimeZone utc(BOBUIimeZone::UTC);

    // Validity tests
    QCOMPARE(nullTz1.isValid(), false);
    QCOMPARE(nullTz2.isValid(), false);
    QCOMPARE(utc.isValid(), true);

    // Comparison tests
    QCOMPARE((nullTz1 == nullTz2), true);
    QCOMPARE((nullTz1 != nullTz2), false);
    QCOMPARE((nullTz1 == utc), false);
    QCOMPARE((nullTz1 != utc), true);

    // Assignment tests
    nullTz2 = utc;
    QCOMPARE(nullTz2.isValid(), true);
    utc = nullTz1;
    QCOMPARE(utc.isValid(), false);

#if BOBUI_CONFIG(timezone)
    QCOMPARE(nullTz1.id(), QByteArray());
    QCOMPARE(nullTz1.territory(), QLocale::AnyTerritory);
    QCOMPARE(nullTz1.comment(), QString());

    QDateTime jan = QDateTime(QDate(2012, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
    QDateTime jun = QDateTime(QDate(2012, 6, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
    QDateTime janPrev = QDateTime(QDate(2011, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);

    QCOMPARE(nullTz1.abbreviation(jan), QString());
    QCOMPARE(nullTz1.displayName(jan), QString());
    QCOMPARE(nullTz1.displayName(BOBUIimeZone::StandardTime), QString());

    QCOMPARE(nullTz1.offsetFromUtc(jan), 0);
    QCOMPARE(nullTz1.offsetFromUtc(jun), 0);

    QCOMPARE(nullTz1.standardTimeOffset(jan), 0);
    QCOMPARE(nullTz1.standardTimeOffset(jun), 0);

    QCOMPARE(nullTz1.daylightTimeOffset(jan), 0);
    QCOMPARE(nullTz1.daylightTimeOffset(jun), 0);

    QCOMPARE(nullTz1.hasDaylightTime(), false);
    QCOMPARE(nullTz1.isDaylightTime(jan), false);
    QCOMPARE(nullTz1.isDaylightTime(jun), false);

    BOBUIimeZone::OffsetData data = nullTz1.offsetData(jan);
    constexpr auto invalidOffset = std::numeric_limits<int>::min();
    QCOMPARE(data.atUtc, QDateTime());
    QCOMPARE(data.offsetFromUtc, invalidOffset);
    QCOMPARE(data.standardTimeOffset, invalidOffset);
    QCOMPARE(data.daylightTimeOffset, invalidOffset);

    QCOMPARE(nullTz1.hasTransitions(), false);

    data = nullTz1.nextTransition(jan);
    QCOMPARE(data.atUtc, QDateTime());
    QCOMPARE(data.offsetFromUtc, invalidOffset);
    QCOMPARE(data.standardTimeOffset, invalidOffset);
    QCOMPARE(data.daylightTimeOffset, invalidOffset);

    data = nullTz1.previousTransition(jan);
    QCOMPARE(data.atUtc, QDateTime());
    QCOMPARE(data.offsetFromUtc, invalidOffset);
    QCOMPARE(data.standardTimeOffset, invalidOffset);
    QCOMPARE(data.daylightTimeOffset, invalidOffset);
#endif // feature timezone
}

void tst_BOBUIimeZone::assign()
{
    BOBUIimeZone assignee;
    QCOMPARE(assignee.timeSpec(), BobUI::TimeZone);
    assignee = BOBUIimeZone();
    QCOMPARE(assignee.timeSpec(), BobUI::TimeZone);
    assignee = BOBUIimeZone::UTC;
    QCOMPARE(assignee.timeSpec(), BobUI::UTC);
    assignee = BOBUIimeZone::LocalTime;
    QCOMPARE(assignee.timeSpec(), BobUI::LocalTime);
    assignee = BOBUIimeZone();
    QCOMPARE(assignee.timeSpec(), BobUI::TimeZone);
    assignee = BOBUIimeZone::fromSecondsAheadOfUtc(1);
    QCOMPARE(assignee.timeSpec(), BobUI::OffsetFromUTC);
    assignee = BOBUIimeZone::fromSecondsAheadOfUtc(0);
    QCOMPARE(assignee.timeSpec(), BobUI::UTC);
#if BOBUI_CONFIG(timezone)
    {
        const BOBUIimeZone cet("Europe/Oslo");
        assignee = cet;
        QCOMPARE(assignee.timeSpec(), BobUI::TimeZone);
    }
#endif
}

void tst_BOBUIimeZone::compareCompiles()
{
    BOBUIestPrivate::testEqualityOperatorsCompile<BOBUIimeZone>();
}

void tst_BOBUIimeZone::compare_data()
{
    BOBUIest::addColumn<BOBUIimeZone>("left");
    BOBUIest::addColumn<BOBUIimeZone>("right");
    BOBUIest::addColumn<bool>("expectedEqual");

    const BOBUIimeZone local;
    const BOBUIimeZone utc(BOBUIimeZone::UTC);
    const auto secondEast = BOBUIimeZone::fromSecondsAheadOfUtc(1);
    const auto zeroOffset = BOBUIimeZone::fromSecondsAheadOfUtc(0);
    const auto durationEast = BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::seconds{1});

    BOBUIest::newRow("local vs default-constructed") << local << BOBUIimeZone() << true;
    BOBUIest::newRow("local vs UTC") << local << utc << false;
    BOBUIest::newRow("local vs secondEast") << local << secondEast << false;
    BOBUIest::newRow("secondEast vs UTC") << secondEast << utc << false;
    BOBUIest::newRow("UTC vs zeroOffset") << utc << zeroOffset << true;
    BOBUIest::newRow("secondEast vs durationEast") << secondEast << durationEast << true;
}

void tst_BOBUIimeZone::compare()
{
    QFETCH(BOBUIimeZone, left);
    QFETCH(BOBUIimeZone, right);
    QFETCH(bool, expectedEqual);

    BOBUI_TEST_EQUALITY_OPS(left, right, expectedEqual);
}

void tst_BOBUIimeZone::timespec()
{
    using namespace std::chrono_literals;
    QCOMPARE(BOBUIimeZone().timeSpec(), BobUI::TimeZone);
    QCOMPARE(BOBUIimeZone(BOBUIimeZone::UTC).timeSpec(), BobUI::UTC);
    QCOMPARE(BOBUIimeZone(BOBUIimeZone::LocalTime).timeSpec(), BobUI::LocalTime);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(0).timeSpec(), BobUI::UTC);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(0s).timeSpec(), BobUI::UTC);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(0min).timeSpec(), BobUI::UTC);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(0h).timeSpec(), BobUI::UTC);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(1).timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(-1).timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(36000).timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(-36000).timeSpec(), BobUI::OffsetFromUTC);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(3h - 20min +17s).timeSpec(), BobUI::OffsetFromUTC);
    {
        const BOBUIimeZone zone;
        QCOMPARE(zone.timeSpec(), BobUI::TimeZone);
    }
    {
        const BOBUIimeZone zone = { BOBUIimeZone::UTC };
        QCOMPARE(zone.timeSpec(), BobUI::UTC);
    }
    {
        const BOBUIimeZone zone = { BOBUIimeZone::LocalTime };
        QCOMPARE(zone.timeSpec(), BobUI::LocalTime);
    }
    {
        const auto zone = BOBUIimeZone::fromSecondsAheadOfUtc(0);
        QCOMPARE(zone.timeSpec(), BobUI::UTC);
    }
    {
        const auto zone = BOBUIimeZone::fromDurationAheadOfUtc(0s);
        QCOMPARE(zone.timeSpec(), BobUI::UTC);
    }
    {
        const auto zone = BOBUIimeZone::fromSecondsAheadOfUtc(1);
        QCOMPARE(zone.timeSpec(), BobUI::OffsetFromUTC);
    }
    {
        const auto zone = BOBUIimeZone::fromDurationAheadOfUtc(1s);
        QCOMPARE(zone.timeSpec(), BobUI::OffsetFromUTC);
    }
#if BOBUI_CONFIG(timezone)
    QCOMPARE(BOBUIimeZone("Europe/Oslo").timeSpec(), BobUI::TimeZone);
#endif
}

void tst_BOBUIimeZone::offset()
{
    QCOMPARE(BOBUIimeZone().fixedSecondsAheadOfUtc(), 0);
    QCOMPARE(BOBUIimeZone(BOBUIimeZone::UTC).fixedSecondsAheadOfUtc(), 0);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(0).fixedSecondsAheadOfUtc(), 0);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::seconds{}).fixedSecondsAheadOfUtc(), 0);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::minutes{}).fixedSecondsAheadOfUtc(), 0);
    QCOMPARE(BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::hours{}).fixedSecondsAheadOfUtc(), 0);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(1).fixedSecondsAheadOfUtc(), 1);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(-1).fixedSecondsAheadOfUtc(), -1);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(36000).fixedSecondsAheadOfUtc(), 36000);
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(-36000).fixedSecondsAheadOfUtc(), -36000);
    {
        const BOBUIimeZone zone;
        QCOMPARE(zone.fixedSecondsAheadOfUtc(), 0);
    }
    {
        const BOBUIimeZone zone = { BOBUIimeZone::UTC };
        QCOMPARE(zone.fixedSecondsAheadOfUtc(), 0);
    }
    {
        const auto zone = BOBUIimeZone::fromSecondsAheadOfUtc(0);
        QCOMPARE(zone.fixedSecondsAheadOfUtc(), 0);
    }
    {
        const auto zone = BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::seconds{});
        QCOMPARE(zone.fixedSecondsAheadOfUtc(), 0);
    }
    {
        const auto zone = BOBUIimeZone::fromSecondsAheadOfUtc(1);
        QCOMPARE(zone.fixedSecondsAheadOfUtc(), 1);
    }
    {
        const auto zone = BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::seconds{1});
        QCOMPARE(zone.fixedSecondsAheadOfUtc(), 1);
    }
#if BOBUI_CONFIG(timezone)
    QCOMPARE(BOBUIimeZone("Europe/Oslo").fixedSecondsAheadOfUtc(), 0);
#endif
}

void tst_BOBUIimeZone::dataStreamTest()
{
#if BOBUI_CONFIG(timezone) && !defined(BOBUI_NO_DATASTREAM)
    // Test the OffsetFromUtc backend serialization. First with a custom timezone:
    BOBUIimeZone tz1("QST"_ba, 23456,
                  u"BobUI Standard Time"_s, u"QST"_s, QLocale::Norway, u"BobUI Testing"_s);
    QByteArray tmp;
    {
        QDataStream ds(&tmp, QIODevice::WriteOnly);
        ds << tz1;
        QCOMPARE(ds.status(), QDataStream::Ok);
    }
    BOBUIimeZone tz2("UTC-12:00"); // Shall be over-written.
    {
        QDataStream ds(&tmp, QIODevice::ReadOnly);
        ds >> tz2;
        QCOMPARE(ds.status(), QDataStream::Ok);
    }
    QCOMPARE(tz2.id(), "QST"_ba);
    QCOMPARE(tz2.comment(), u"BobUI Testing"_s);
    QCOMPARE(tz2.territory(), QLocale::Norway);
    QCOMPARE(tz2.abbreviation(QDateTime::currentDateTime()), u"QST"_s);
    QCOMPARE(tz2.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::LongName), u"BobUI Standard Time"_s);
    QCOMPARE(tz2.displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::LongName), u"BobUI Standard Time"_s);
    QCOMPARE(tz2.offsetFromUtc(QDateTime::currentDateTime()), 23456);

    // And then with a standard IANA timezone (BOBUIBUG-60595):
    tz1 = BOBUIimeZone("UTC");
    QCOMPARE(tz1.isValid(), true);
    {
        QDataStream ds(&tmp, QIODevice::WriteOnly);
        ds << tz1;
        QCOMPARE(ds.status(), QDataStream::Ok);
    }
    {
        QDataStream ds(&tmp, QIODevice::ReadOnly);
        ds >> tz2;
        QCOMPARE(ds.status(), QDataStream::Ok);
    }
    QCOMPARE(tz2.isValid(), true);
    QCOMPARE(tz2.id(), tz1.id());

    // Test the system backend serialization
    tz1 = BOBUIimeZone("Pacific/Auckland");

    // If not valid then probably using the UTC system backend so skip
    if (!tz1.isValid())
        return;

    {
        QDataStream ds(&tmp, QIODevice::WriteOnly);
        ds << tz1;
        QCOMPARE(ds.status(), QDataStream::Ok);
    }
    tz2 = BOBUIimeZone("UTC");
    {
        QDataStream ds(&tmp, QIODevice::ReadOnly);
        ds >> tz2;
        QCOMPARE(ds.status(), QDataStream::Ok);
    }
    QCOMPARE(tz2.id(), tz1.id());
#endif // feature timezone and enabled datastream
}

#if BOBUI_CONFIG(timezone)
void tst_BOBUIimeZone::asBackendZone()
{
    QCOMPARE(BOBUIimeZone(BOBUIimeZone::LocalTime).asBackendZone(), BOBUIimeZone::systemTimeZone());
    QCOMPARE(BOBUIimeZone(BOBUIimeZone::UTC).asBackendZone(), BOBUIimeZone::utc());
    QCOMPARE(BOBUIimeZone::fromSecondsAheadOfUtc(-300).asBackendZone(), BOBUIimeZone(-300));
    BOBUIimeZone cet("Europe/Oslo");
    QCOMPARE(cet.asBackendZone(), cet);
}

void tst_BOBUIimeZone::systemZone()
{
    const BOBUIimeZone zone = BOBUIimeZone::systemTimeZone();
    QVERIFY2(zone.isValid(),
             "Invalid system zone setting, tests are doomed on misconfigured system.");
    // This may fail on Windows if CLDR data doesn't map system MS ID to IANA ID:
    QCOMPARE(zone.id(), BOBUIimeZone::systemTimeZoneId());
    QCOMPARE(zone, BOBUIimeZone(BOBUIimeZone::systemTimeZoneId()));
    // Check it behaves the same as local-time:
    const QDate dates[] = {
        QDate::fromJulianDay(0), // far in the distant past (LMT)
        QDate(1625, 6, 8), // Before time-zones (date of Cassini's birth)
        QDate(1901, 12, 13), // Last day before 32-bit time_t's range
        QDate(1969, 12, 31), // Last day before the epoch
        QDate(1970, 0, 0), // Start of epoch
        QDate(2000, 2, 29), // An anomalous leap day
        QDate(2038, 1, 20) // First day after 32-bit time_t's range
    };
    for (const auto &date : dates)
        QCOMPARE(date.startOfDay(BOBUIimeZone::LocalTime), date.startOfDay(zone));

#if __cpp_lib_chrono >= 201907L
    const std::chrono::time_zone *currentTimeZone = std::chrono::current_zone();
    QCOMPARE(QByteArrayView(currentTimeZone->name()), QByteArrayView(zone.id()));
#endif
}

void tst_BOBUIimeZone::isTimeZoneIdAvailable()
{
    const QList<QByteArray> available = BOBUIimeZone::availableTimeZoneIds();
    for (const QByteArray &id : available) {
        QVERIFY2(BOBUIimeZone::isTimeZoneIdAvailable(id), id);
        const BOBUIimeZone zone(id);
        QVERIFY2(zone.isValid(), id);
        if (unAliasedLinks.find(id) == unAliasedLinks.end())
            QVERIFY2(zone.hasAlternativeName(id), zone.id() + " != " + id);
    }
    // availableTimeZoneIds() doesn't list all possible offset IDs, but
    // isTimeZoneIdAvailable() should accept them.
    for (qint32 offset = BOBUIimeZone::MinUtcOffsetSecs;
         offset <= BOBUIimeZone::MinUtcOffsetSecs; ++offset) {
        const QByteArray id = BOBUIimeZone(offset).id();
        QVERIFY2(BOBUIimeZone::isTimeZoneIdAvailable(id), id);
        QVERIFY2(BOBUIimeZone(id).isValid(), id);
        QCOMPARE(BOBUIimeZone(id).id(), id);
    }
}

void tst_BOBUIimeZone::utcOffsetId_data()
{
    BOBUIest::addColumn<QByteArray>("id");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<int>("offset"); // ignored unless valid

    // Some of these are actual CLDR zone IDs, some are known Windows IDs; the
    // rest rely on parsing the offset. Since CLDR and Windows may add to their
    // known IDs, which fall in which category may vary. Only the CLDR and
    // Windows ones are known to isTimeZoneAvailable() or listed in
    // availableTimeZoneIds().
#define ROW(name, valid, offset) \
    BOBUIest::newRow(name) << QByteArray(name) << valid << offset

    // See bobuibase/util/locale_database/zonedata.py for source
    // CLDR v35.1 IDs:
    ROW("UTC", true, 0);
    ROW("UTC-14:00", true, -50400);
    ROW("UTC-13:00", true, -46800);
    ROW("UTC-12:00", true, -43200);
    ROW("UTC-11:00", true, -39600);
    ROW("UTC-10:00", true, -36000);
    ROW("UTC-09:00", true, -32400);
    ROW("UTC-08:00", true, -28800);
    ROW("UTC-07:00", true, -25200);
    ROW("UTC-06:00", true, -21600);
    ROW("UTC-05:00", true, -18000);
    ROW("UTC-04:30", true, -16200);
    ROW("UTC-04:00", true, -14400);
    ROW("UTC-03:30", true, -12600);
    ROW("UTC-03:00", true, -10800);
    ROW("UTC-02:00", true, -7200);
    ROW("UTC-01:00", true, -3600);
    ROW("UTC-00:00", true, 0);
    ROW("UTC+00:00", true, 0);
    ROW("UTC+01:00", true, 3600);
    ROW("UTC+02:00", true, 7200);
    ROW("UTC+03:00", true, 10800);
    ROW("UTC+03:30", true, 12600);
    ROW("UTC+04:00", true, 14400);
    ROW("UTC+04:30", true, 16200);
    ROW("UTC+05:00", true, 18000);
    ROW("UTC+05:30", true, 19800);
    ROW("UTC+05:45", true, 20700);
    ROW("UTC+06:00", true, 21600);
    ROW("UTC+06:30", true, 23400);
    ROW("UTC+07:00", true, 25200);
    ROW("UTC+08:00", true, 28800);
    ROW("UTC+08:30", true, 30600);
    ROW("UTC+09:00", true, 32400);
    ROW("UTC+09:30", true, 34200);
    ROW("UTC+10:00", true, 36000);
    ROW("UTC+11:00", true, 39600);
    ROW("UTC+12:00", true, 43200);
    ROW("UTC+13:00", true, 46800);
    ROW("UTC+14:00", true, 50400);

    // Windows IDs known to CLDR v35.1:
    ROW("UTC-11", true, -39600);
    ROW("UTC-09", true, -32400);
    ROW("UTC-08", true, -28800);
    ROW("UTC-8", true, -28800);
    ROW("UTC-2:5", true, -7500);
    ROW("UTC-02", true, -7200);
    ROW("UTC+2", true, 7200);
    ROW("UTC+2:5", true, 7500);
    ROW("UTC+12", true, 43200);
    ROW("UTC+13", true, 46800);
    // Encountered in bug reports:
    ROW("UTC+10", true, 36000); // BOBUIBUG-77738

    // Bounds:
    ROW("UTC+23", true, 82800);
    ROW("UTC-23", true, -82800);
    ROW("UTC+23:59", true, 86340);
    ROW("UTC-23:59", true, -86340);
    ROW("UTC+23:59:59", true, 86399);
    ROW("UTC-23:59:59", true, -86399);

    // Out of range
    ROW("UTC+24:0:0", false, 0);
    ROW("UTC-24:0:0", false, 0);
    ROW("UTC+0:60:0", false, 0);
    ROW("UTC-0:60:0", false, 0);
    ROW("UTC+0:0:60", false, 0);
    ROW("UTC-0:0:60", false, 0);

    // Malformed
    ROW("UTC+", false, 0);
    ROW("UTC-", false, 0);
    ROW("UTC10", false, 0);
    ROW("UTC:10", false, 0);
    ROW("UTC+cabbage", false, 0);
    ROW("UTC+10:rice", false, 0);
    ROW("UTC+9:3:oat", false, 0);
    ROW("UTC+9+3", false, 0);
    ROW("UTC+9-3", false, 0);
    ROW("UTC+9:3-4", false, 0);
    ROW("UTC+9:3:4:more", false, 0);
    ROW("UTC+9:3:4:5", false, 0);
}

void tst_BOBUIimeZone::utcOffsetId()
{
    QFETCH(QByteArray, id);
    QFETCH(bool, valid);
    BOBUIimeZone zone(id);
    QCOMPARE(zone.isValid(), valid);
    if (valid) {
        QDateTime epoch(QDate(1970, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC);
        QFETCH(int, offset);
        QCOMPARE(zone.offsetFromUtc(epoch), offset);
        QVERIFY(!zone.hasDaylightTime());

        // zone.id() will be an IANA ID with zero minutes field if original was
        // a UTC offset by a whole number of hours. It will also zero-pad a
        // single-digit hour or minute to two digits.
        if (const qsizetype cut = id.indexOf(':'); cut >= 0) {
            if (id.size() == cut + 2) // "...:m" -> "...:0m"
                id.insert(cut + 1, '0');
        } else if (zone.id().contains(':')) {
            id += ":00";
        }
        if (id.indexOf(':') == 5) // UTC±h:mm -> UTC±0h:mm
            id.insert(4, '0');

        QCOMPARE(zone.id(), id);
    }
}

void tst_BOBUIimeZone::hasAlternativeName_data()
{
    BOBUIest::addColumn<QByteArray>("iana");
    BOBUIest::addColumn<QByteArray>("alias");

    BOBUIest::newRow("Montreal=Toronto") << "America/Toronto"_ba << "America/Montreal"_ba;
    BOBUIest::newRow("Asmera=Asmara") << "Africa/Asmara"_ba << "Africa/Asmera"_ba;
    BOBUIest::newRow("Argentina/Catamarca")
        << "America/Argentina/Catamarca"_ba << "America/Catamarca"_ba;
    BOBUIest::newRow("Godthab=Nuuk") << "America/Nuuk"_ba << "America/Godthab"_ba;
    BOBUIest::newRow("Indiana/Indianapolis")
        << "America/Indiana/Indianapolis"_ba << "America/Indianapolis"_ba;
    BOBUIest::newRow("Kentucky/Louisville")
        << "America/Kentucky/Louisville"_ba << "America/Louisville"_ba;
    BOBUIest::newRow("Calcutta=Kolkata") << "Asia/Kolkata"_ba << "Asia/Calcutta"_ba;
    BOBUIest::newRow("Katmandu=Kathmandu") << "Asia/Kathmandu"_ba << "Asia/Katmandu"_ba;
    BOBUIest::newRow("Rangoon=Yangon") << "Asia/Yangon"_ba << "Asia/Rangoon"_ba;
    BOBUIest::newRow("Saigon=Ho_Chi_Minh") << "Asia/Ho_Chi_Minh"_ba << "Asia/Saigon"_ba;
    BOBUIest::newRow("Faeroe=Faroe") << "Atlantic/Faroe"_ba << "Atlantic/Faeroe"_ba;
    BOBUIest::newRow("Currie=Hobart") << "Australia/Hobart"_ba << "Australia/Currie"_ba;
    BOBUIest::newRow("Kiev=Kyiv") << "Europe/Kyiv"_ba << "Europe/Kiev"_ba;
    BOBUIest::newRow("Uzhgorod=Kyiv") << "Europe/Kyiv"_ba << "Europe/Uzhgorod"_ba;
    BOBUIest::newRow("Zaporozhye=Kyiv") << "Europe/Kyiv"_ba << "Europe/Zaporozhye"_ba;
    BOBUIest::newRow("Fiji=Fiji") << "Pacific/Fiji"_ba << "Pacific/Fiji"_ba;
    BOBUIest::newRow("Enderbury=Enderbury") << "Pacific/Enderbury"_ba << "Pacific/Enderbury"_ba;
}

void tst_BOBUIimeZone::hasAlternativeName()
{
    QFETCH(const QByteArray, iana);
    QFETCH(const QByteArray, alias);
    const BOBUIimeZone zone(iana);
    const BOBUIimeZone peer(alias);
    if (!zone.isValid())
        QSKIP("Backend doesn't support IANA ID");

    auto report = qScopeGuard([zone, peer]() {
        const QByteArray zid = zone.id(), pid = peer.id();
        qDebug("Using %s and %s", zid.constData(), pid.constData());
    });
    QVERIFY2(peer.isValid(), "Construction should have fallen back on IANA ID");
    QVERIFY(zone.hasAlternativeName(zone.id()));
    QVERIFY(zone.hasAlternativeName(iana));
    QVERIFY(peer.hasAlternativeName(peer.id()));
    QVERIFY(peer.hasAlternativeName(alias));
    QVERIFY(zone.hasAlternativeName(peer.id()));
    QVERIFY(zone.hasAlternativeName(alias));
    QVERIFY(peer.hasAlternativeName(zone.id()));
    QVERIFY(peer.hasAlternativeName(iana));
    report.dismiss();
}

void tst_BOBUIimeZone::specificTransition_data()
{
#if BOBUI_CONFIG(timezone_tzdb) && defined(__GLIBCXX__)
    QSKIP("libstdc++'s C++20 misreads the IANA DB for Moscow's transitions (among others).");
#endif
#if defined Q_OS_ANDROID && !BOBUI_CONFIG(timezone_tzdb)
    if (!BOBUIimeZone("Europe/Moscow").hasTransitions())
        QSKIP("Android time-zone back-end has no transition data");
#endif
    BOBUIest::addColumn<QByteArray>("zone");
    BOBUIest::addColumn<QDate>("start");
    BOBUIest::addColumn<QDate>("stop");
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<QDateTime>("atUtc");
    // In seconds:
    BOBUIest::addColumn<int>("offset");
    BOBUIest::addColumn<int>("stdoff");
    BOBUIest::addColumn<int>("dstoff");

    // Moscow ditched DST on 2010-10-31 but has since changed standard offset twice.
#ifdef USING_WIN_TZ
    // Win7 is too old to know about this transition:
    if (QOperatingSystemVersion::current() > QOperatingSystemVersion::Windows7)
#endif
    {
        BOBUIest::newRow("Moscow/2014") // From original bug-report
            << QByteArray("Europe/Moscow")
            << QDate(2011, 4, 1) << QDate(2021, 12, 31) << 1
            << QDateTime(QDate(2014, 10, 26), BOBUIime(2, 0),
                         BOBUIimeZone::fromSecondsAheadOfUtc(4 * 3600)).toUTC()
            << 3 * 3600 << 3 * 3600 << 0;
    }
    BOBUIest::newRow("Moscow/2011") // Transition on 2011-03-27
        << QByteArray("Europe/Moscow")
        << QDate(2010, 11, 1) << QDate(2014, 10, 25) << 1
        << QDateTime(QDate(2011, 3, 27), BOBUIime(2, 0),
                     BOBUIimeZone::fromSecondsAheadOfUtc(3 * 3600)).toUTC()
        << 4 * 3600 << 4 * 3600 << 0;
}

void tst_BOBUIimeZone::specificTransition()
{
    // Regression test for BOBUIBUG-42021 (on MS-Win)
    QFETCH(QByteArray, zone);
    QFETCH(QDate, start);
    QFETCH(QDate, stop);
    QFETCH(int, count);
    // No attempt to check abbreviations; to much cross-platform variation.
    QFETCH(QDateTime, atUtc);
    QFETCH(int, offset);
    QFETCH(int, stdoff);
    QFETCH(int, dstoff);

    BOBUIimeZone timeZone(zone);
    if (!timeZone.isValid())
        QSKIP("Missing time-zone data");
    BOBUIimeZone::OffsetDataList transits =
        timeZone.transitions(start.startOfDay(timeZone), stop.endOfDay(timeZone));
    QCOMPARE(transits.size(), count);
    if (count) {
        const BOBUIimeZone::OffsetData &transition = transits.at(0);
        QCOMPARE(transition.offsetFromUtc, offset);
        QCOMPARE(transition.standardTimeOffset, stdoff);
        QCOMPARE(transition.daylightTimeOffset, dstoff);
        QCOMPARE(transition.atUtc, atUtc);
    }
}

void tst_BOBUIimeZone::transitionEachZone_data()
{
    BOBUIest::addColumn<QByteArray>("zone");
    BOBUIest::addColumn<qint64>("secs");
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<int>("stop");

    const struct {
        qint64 baseSecs;
        int start, stop;
        int year;
    } table[] = {
        { 1288488600, -4, 8, 2010 }, // 2010-10-31 01:30 UTC; Europe, Russia
        { 25666200, 3, 12, 1970 },   // 1970-10-25 01:30 UTC; North America
    };

    const auto zones = BOBUIimeZone::availableTimeZoneIds();
    for (const auto &entry : table) {
        for (const QByteArray &zone : zones) {
            BOBUIest::addRow("%s@%d", zone.constData(), entry.year)
                << zone << entry.baseSecs << entry.start << entry.stop;
        }
    }
}

void tst_BOBUIimeZone::transitionEachZone()
{
    // Regression test: round-trip fromMsecs/toMSecs should be idempotent; but
    // various zones failed during fall-back transitions.
    QFETCH(const QByteArray, zone);
    QFETCH(const qint64, secs);
    QFETCH(const int, start);
    QFETCH(const int, stop);
    const BOBUIimeZone named(zone);
    if (!named.isValid())
        QSKIP("Supposedly available zone is not valid");
    if (named.id() != zone)
        QSKIP("Supposedly available zone's id does not match");

    for (int i = start; i < stop; i++) {
#ifdef USING_WIN_TZ
        // See BOBUIBUG-64985: MS's TZ APIs' misdescription of Europe/Samara leads
        // to mis-disambiguation of its fall-back here.
        if (zone == "Europe/Samara" && i == -3)
            continue;
#endif
        const qint64 here = secs + i * 3600;
        const QDateTime when = QDateTime::fromSecsSinceEpoch(here, named);
        const qint64 stamp = when.toMSecsSinceEpoch();
        if (here * 1000 != stamp) {
            // (The +1 is due to using _1_:30 as baseSecs.)
            qDebug("Failing at half past %d UTC (offset %d in %s)", i + 1, when.offsetFromUtc(),
                   QLocale::territoryToString(named.territory()).toUtf8().constData());
        }
        QCOMPARE(stamp % 1000, 0);
        QCOMPARE(here - stamp / 1000, 0);
    }
}

void tst_BOBUIimeZone::checkOffset_data()
{
    BOBUIest::addColumn<BOBUIimeZone>("zone");
    BOBUIest::addColumn<QDateTime>("when");
    BOBUIest::addColumn<int>("netOffset");
    BOBUIest::addColumn<int>("stdOffset");
    BOBUIest::addColumn<int>("dstOffset");

    const BOBUIimeZone UTC = BOBUIimeZone::UTC;
    BOBUIest::addRow("UTC")
        << UTC << QDate(1970, 1, 1).startOfDay(UTC) << 0 << 0 << 0;
    const auto east = BOBUIimeZone::fromSecondsAheadOfUtc(28'800); // 8 hours
    BOBUIest::addRow("UTC+8")
        << east << QDate(2000, 2, 29).startOfDay(east) << 28'800 << 28'800 << 0;
    const auto west = BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::hours{-8});
    BOBUIest::addRow("UTC-8")
        << west << QDate(2100, 2, 28).startOfDay(west) << -28'800 << -28'800 << 0;

    struct {
        const char *zone, *nick;
        int year, month, day, hour, min, sec;
        int std, dst;
    } table[] = {
        // Exercise the UTC-backend:
        { "UTC", "epoch", 1970, 1, 1, 0, 0, 0, 0, 0 },
        // Zone with no transitions (BOBUIBUG-74614, BOBUIBUG-74666, when TZ backend uses minimal data)
        { "Etc/UTC", "epoch", 1970, 1, 1, 0, 0, 0, 0, 0 },
        { "Etc/UTC", "pre_int32", 1901, 12, 13, 20, 45, 51, 0, 0 },
        { "Etc/UTC", "post_int32", 2038, 1, 19, 3, 14, 9, 0, 0 },
        { "Etc/UTC", "post_uint32", 2106, 2, 7, 6, 28, 17, 0, 0 },
        { "Etc/UTC", "initial", -292275056, 5, 16, 16, 47, 5, 0, 0 },
        { "Etc/UTC", "final", 292278994, 8, 17, 7, 12, 55, 0, 0 },
        // Kyiv: regression test for BOBUIBUG-64122 (on MS):
        { "Europe/Kyiv", "summer", 2017, 10, 27, 12, 0, 0, 2 * 3600, 3600 },
        { "Europe/Kyiv", "winter", 2017, 10, 29, 12, 0, 0, 2 * 3600, 0 }
    };
    for (const auto &entry : table) {
        BOBUIimeZone zone(entry.zone);
        if (zone.isValid()) {
            BOBUIest::addRow("%s@%s", entry.zone, entry.nick)
                << zone
                << QDateTime(QDate(entry.year, entry.month, entry.day),
                             BOBUIime(entry.hour, entry.min, entry.sec), zone)
                << entry.dst + entry.std << entry.std << entry.dst;
        } else {
            qWarning("Skipping %s@%s test as zone is invalid", entry.zone, entry.nick);
        }
    }
}

void tst_BOBUIimeZone::checkOffset()
{
    QFETCH(BOBUIimeZone, zone);
    QFETCH(QDateTime, when);
    QFETCH(int, netOffset);
    QFETCH(int, stdOffset);
    QFETCH(int, dstOffset);

    QVERIFY(zone.isValid()); // It was when _data() added the row !
    QCOMPARE(zone.offsetFromUtc(when), netOffset);
    QCOMPARE(zone.standardTimeOffset(when), stdOffset);
    QCOMPARE(zone.daylightTimeOffset(when), dstOffset);
    QCOMPARE(zone.isDaylightTime(when), dstOffset != 0);

    // Also test offsetData(), which gets all this data in one go:
    const auto data = zone.offsetData(when);
    QCOMPARE(data.atUtc, when);
    QCOMPARE(data.offsetFromUtc, netOffset);
    QCOMPARE(data.standardTimeOffset, stdOffset);
    QCOMPARE(data.daylightTimeOffset, dstOffset);
}

void tst_BOBUIimeZone::availableTimeZoneIds()
{
    if constexpr (debug) {
        qDebug() << "";
        qDebug() << "Available Time Zones" ;
        qDebug() << BOBUIimeZone::availableTimeZoneIds();
        qDebug() << "";
        qDebug() << "Available Time Zones in the US";
        qDebug() << BOBUIimeZone::availableTimeZoneIds(QLocale::UnitedStates);
        qDebug() << "";
        qDebug() << "Available Time Zones with UTC Offset 0";
        qDebug() << BOBUIimeZone::availableTimeZoneIds(0);
        qDebug() << "";
    } else {
        // Test the calls work:
        QList<QByteArray> listAll = BOBUIimeZone::availableTimeZoneIds();
        QList<QByteArray> list001 = BOBUIimeZone::availableTimeZoneIds(QLocale::World);
        QList<QByteArray> listUsa = BOBUIimeZone::availableTimeZoneIds(QLocale::UnitedStates);
        QList<QByteArray> listGmt = BOBUIimeZone::availableTimeZoneIds(0);
        // We cannot know what any test machine has available, so can't test contents.
        // But we can do a consistency check:
        QCOMPARE_LT(list001.size(), listAll.size());
        QCOMPARE_LT(listUsa.size(), listAll.size());
        QCOMPARE_LT(listGmt.size(), listAll.size());
        // And we do know CLDR data supplies some entries to each:
        QCOMPARE_GT(listAll.size(), 0);
        QCOMPARE_GT(list001.size(), 0);
        QCOMPARE_GT(listUsa.size(), 0);
        QCOMPARE_GT(listGmt.size(), 0);
    }
}

void tst_BOBUIimeZone::stressTest()
{
    constexpr auto UTC = BOBUIimeZone::UTC;
    const QList<QByteArray> idList = BOBUIimeZone::availableTimeZoneIds();
    for (const QByteArray &id : idList) {
        BOBUIimeZone testZone = BOBUIimeZone(id);
        QCOMPARE(testZone.isValid(), true);
        if (unAliasedLinks.find(id) == unAliasedLinks.end())
            QVERIFY2(testZone.hasAlternativeName(id), testZone.id() + " != " + id);
        QDateTime testDate = QDateTime(QDate(2015, 1, 1), BOBUIime(0, 0), UTC);
        testZone.territory();
        testZone.comment();
        testZone.displayName(testDate);
        (void)testZone.displayName(BOBUIimeZone::GenericTime);
        (void)testZone.displayName(BOBUIimeZone::StandardTime);
        (void)testZone.displayName(BOBUIimeZone::DaylightTime);
        testZone.abbreviation(testDate);
        testZone.offsetFromUtc(testDate);
        testZone.standardTimeOffset(testDate);
        testZone.daylightTimeOffset(testDate);
        testZone.hasDaylightTime();
        testZone.isDaylightTime(testDate);
        testZone.offsetData(testDate);
        testZone.hasTransitions();
        testZone.nextTransition(testDate);
        testZone.previousTransition(testDate);
        // Dates known to be outside possible tz file pre-calculated rules range
        QDateTime lowDate1 = QDateTime(QDate(1800, 1, 1), BOBUIime(0, 0), UTC);
        QDateTime lowDate2 = QDateTime(QDate(1800, 6, 1), BOBUIime(0, 0), UTC);
        QDateTime highDate1 = QDateTime(QDate(2200, 1, 1), BOBUIime(0, 0), UTC);
        QDateTime highDate2 = QDateTime(QDate(2200, 6, 1), BOBUIime(0, 0), UTC);
        testZone.nextTransition(lowDate1);
        testZone.nextTransition(lowDate2);
        testZone.previousTransition(lowDate2);
        testZone.previousTransition(lowDate2);
        testZone.nextTransition(highDate1);
        testZone.nextTransition(highDate2);
        testZone.previousTransition(highDate1);
        testZone.previousTransition(highDate2);
        if constexpr (debug) {
            // This could take a long time, depending on platform and database
            qDebug() << "Stress test calculating transistions for" << testZone.id();
            testZone.transitions(lowDate1, highDate1);
        }
        testDate.setTimeZone(testZone);
        testDate.isValid();
        testDate.offsetFromUtc();
        testDate.timeZoneAbbreviation();
    }
}

void tst_BOBUIimeZone::windowsId()
{
/*
    Current Windows zones for "Central Standard Time":
    Region      IANA Id(s)
    World       "America/Chicago" (the default)
    Canada      "America/Winnipeg America/Rankin_Inlet America/Resolute"
    Mexico      "America/Matamoros America/Ojinaga"
    USA         "America/Chicago America/Indiana/Knox America/Indiana/Tell_City America/Menominee"
                "America/North_Dakota/Beulah America/North_Dakota/Center"
                "America/North_Dakota/New_Salem"
*/
    QCOMPARE(BOBUIimeZone::ianaIdToWindowsId("America/Chicago"),
             QByteArray("Central Standard Time"));
    QCOMPARE(BOBUIimeZone::ianaIdToWindowsId("America/Resolute"),
             QByteArray("Central Standard Time"));

    // Partials shouldn't match
    QCOMPARE(BOBUIimeZone::ianaIdToWindowsId("America/Chi"), QByteArray());
    QCOMPARE(BOBUIimeZone::ianaIdToWindowsId("InvalidZone"), QByteArray());
    QCOMPARE(BOBUIimeZone::ianaIdToWindowsId(QByteArray()), QByteArray());

    // Check default value
    QCOMPARE(BOBUIimeZone::windowsIdToDefaultIanaId("Central Standard Time"),
             QByteArray("America/Chicago"));
    QCOMPARE(BOBUIimeZone::windowsIdToDefaultIanaId("Central Standard Time", QLocale::World),
             QByteArray("America/Chicago"));
    QCOMPARE(BOBUIimeZone::windowsIdToDefaultIanaId("Central Standard Time", QLocale::Canada),
             QByteArray("America/Winnipeg"));
    QCOMPARE(BOBUIimeZone::windowsIdToDefaultIanaId("Central Standard Time", QLocale::AnyTerritory),
             QByteArray());
    QCOMPARE(BOBUIimeZone::windowsIdToDefaultIanaId(QByteArray()), QByteArray());

    {
        // With no country, expect sorted list of all zones for ID
        const QList<QByteArray> list = {
            "America/Chicago", "America/Indiana/Knox", "America/Indiana/Tell_City",
            "America/Matamoros", "America/Menominee", "America/North_Dakota/Beulah",
            "America/North_Dakota/Center", "America/North_Dakota/New_Salem",
            "America/Ojinaga", "America/Rankin_Inlet", "America/Resolute",
            "America/Winnipeg"
        };
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time"), list);
    }
    {
        const QList<QByteArray> list = { "America/Chicago" };
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time", QLocale::World),
                 list);
    }
    {
        // Check country with no match returns empty list
        const QList<QByteArray> empty;
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time", QLocale::NewZealand),
                 empty);
    }
    {
        // Check valid country returns list in preference order
        const QList<QByteArray> list = {
            "America/Winnipeg", "America/Rankin_Inlet", "America/Resolute"
        };
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time", QLocale::Canada), list);
    }
    {
        const QList<QByteArray> list = { "America/Matamoros", "America/Ojinaga" };
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time", QLocale::Mexico), list);
    }
    {
        const QList<QByteArray> list = {
            "America/Chicago", "America/Indiana/Knox", "America/Indiana/Tell_City",
            "America/Menominee", "America/North_Dakota/Beulah", "America/North_Dakota/Center",
            "America/North_Dakota/New_Salem"
        };
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time", QLocale::UnitedStates),
                 list);
    }
    {
        const QList<QByteArray> list;
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds("Central Standard Time", QLocale::AnyTerritory),
                 list);
    }
    {
        // Check empty if given no windowsId:
        const QList<QByteArray> empty;
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds(QByteArray()), empty);
        QCOMPARE(BOBUIimeZone::windowsIdToIanaIds(QByteArray(), QLocale::AnyTerritory), empty);
    }
}

void tst_BOBUIimeZone::serialize()
{
    int parts = 0;
#ifndef BOBUI_NO_DEBUG_STREAM
    BOBUIest::ignoreMessage(BobUIDebugMsg, "BOBUIimeZone(\"\")");
    qDebug() << BOBUIimeZone(); // to verify no crash
    parts++;
#endif
#ifndef BOBUI_NO_DATASTREAM
    QByteArray blob;
    {
        QDataStream stream(&blob, QIODevice::WriteOnly);
        stream << BOBUIimeZone("Europe/Oslo") << BOBUIimeZone(420) << BOBUIimeZone() << qint64(-1);
    }
    QDataStream stream(&blob, QIODevice::ReadOnly);
    BOBUIimeZone invalid, offset, oslo;
    qint64 minusone;
    stream >> oslo >> offset >> invalid >> minusone;
    QCOMPARE(oslo, BOBUIimeZone("Europe/Oslo"));
    QCOMPARE(offset, BOBUIimeZone(420));
    QVERIFY(!invalid.isValid());
    QCOMPARE(minusone, qint64(-1));
    parts++;
#endif
    if (!parts)
        QSKIP("No serialization enabled");
}

void tst_BOBUIimeZone::malformed()
{
    // Regression test for BOBUIBUG-92808
    // Strings that look enough like a POSIX zone specifier that the constructor
    // accepts them, but the specifier is invalid.
    // Must not crash or trigger assertions when calling offsetFromUtc()
    const QDateTime now = QDateTime::currentDateTime();
    BOBUIimeZone barf("QUT4tCZ0 , /");
    if (barf.isValid())
        QCOMPARE(barf.offsetFromUtc(now), 0);
    barf = BOBUIimeZone("BobUIC+09,,MA");
    if (barf.isValid())
        QCOMPARE(barf.offsetFromUtc(now), 0);
    barf = BOBUIimeZone("UTCC+14:00,-,");
    if (barf.isValid())
        QCOMPARE(barf.daylightTimeOffset(now), -14 * 3600);
}

void tst_BOBUIimeZone::utcTest()
{
#if BOBUI_CONFIG(icu) // || hopefully various other cases, eventually
    const QString utcLongName = u"Coordinated Universal Time"_s;
#else
    const QString utcLongName = u"UTC"_s;
#endif
#ifdef BOBUI_BUILD_INTERNAL
    // Test default UTC constructor
    QUtcTimeZonePrivate tzp;
    QCOMPARE(tzp.isValid(),   true);
    QCOMPARE(tzp.id(), "UTC");
    QCOMPARE(tzp.territory(), QLocale::AnyTerritory);
    QCOMPARE(tzp.abbreviation(0), u"UTC");
    QCOMPARE(tzp.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::LongName, QLocale()), utcLongName);
    QCOMPARE(tzp.offsetFromUtc(0), 0);
    QCOMPARE(tzp.standardTimeOffset(0), 0);
    QCOMPARE(tzp.daylightTimeOffset(0), 0);
    QCOMPARE(tzp.hasDaylightTime(), false);
    QCOMPARE(tzp.hasTransitions(), false);
#endif // BOBUI_BUILD_INTERNAL

    // Test UTC accessor
    const QDateTime now = QDateTime::currentDateTime();
    auto tz = BOBUIimeZone::utc();
    QCOMPARE(tz.isValid(), true);
    QCOMPARE(tz.id(), "UTC");
    QCOMPARE(tz.territory(), QLocale::AnyTerritory);
    QCOMPARE(tz.abbreviation(now), u"UTC");
    QCOMPARE(tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::LongName, QLocale()), utcLongName);
    QCOMPARE(tz.offsetFromUtc(now), 0);
    QCOMPARE(tz.standardTimeOffset(now), 0);
    QCOMPARE(tz.daylightTimeOffset(now), 0);
    QCOMPARE(tz.hasDaylightTime(), false);
    QCOMPARE(tz.hasTransitions(), false);

    // Test create from UTC Offset:
    tz = BOBUIimeZone(36000);
    QVERIFY(tz.isValid());
    QCOMPARE(tz.id(), "UTC+10:00");
    QCOMPARE(tz.offsetFromUtc(now), 36000);
    QCOMPARE(tz.standardTimeOffset(now), 36000);
    QCOMPARE(tz.daylightTimeOffset(now), 0);

    tz = BOBUIimeZone(15 * 3600); // no IANA ID, so uses minimal id, skipping :00 minutes
    QVERIFY(tz.isValid());
    QCOMPARE(tz.id(), "UTC+15:00");
    QCOMPARE(tz.offsetFromUtc(now), 15 * 3600);
    QCOMPARE(tz.standardTimeOffset(now), 15 * 3600);
    QCOMPARE(tz.daylightTimeOffset(now), 0);

    // Test validity range of UTC offsets:
    int min = BOBUIimeZone::MinUtcOffsetSecs;
    int max = BOBUIimeZone::MaxUtcOffsetSecs;
    QCOMPARE(BOBUIimeZone(min - 1).isValid(), false);
    QCOMPARE(BOBUIimeZone(min).isValid(), true);
    QCOMPARE(BOBUIimeZone(min + 1).isValid(), true);
    QCOMPARE(BOBUIimeZone(max - 1).isValid(), true);
    QCOMPARE(BOBUIimeZone(max).isValid(), true);
    QCOMPARE(BOBUIimeZone(max + 1).isValid(), false);

    // Test create from standard name (preserves :00 for minutes in id):
    tz = BOBUIimeZone("UTC+10:00");
    QVERIFY(tz.isValid());
    QCOMPARE(tz.id(), "UTC+10:00");
    QCOMPARE(tz.offsetFromUtc(now), 36000);
    QCOMPARE(tz.standardTimeOffset(now), 36000);
    QCOMPARE(tz.daylightTimeOffset(now), 0);

    // Test create custom zone
    tz = BOBUIimeZone("QST", 23456,
                   u"BobUI Standard Time"_s, u"QST"_s, QLocale::Norway, u"BobUI Testing"_s);
    QCOMPARE(tz.isValid(),   true);
    QCOMPARE(tz.id(), "QST");
    QCOMPARE(tz.comment(), u"BobUI Testing");
    QCOMPARE(tz.territory(), QLocale::Norway);
    QCOMPARE(tz.abbreviation(now), u"QST");
    QCOMPARE(tz.displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::LongName), u"BobUI Standard Time");
    QCOMPARE(tz.offsetFromUtc(now), 23456);
    QCOMPARE(tz.standardTimeOffset(now), 23456);
    QCOMPARE(tz.daylightTimeOffset(now), 0);
}

void tst_BOBUIimeZone::darwinTypes()
{
#ifndef Q_OS_DARWIN
    QSKIP("This is an Apple-only test");
#else
    extern void tst_BOBUIimeZone_darwinTypes(); // in tst_bobuiimezone_darwin.mm
    tst_BOBUIimeZone_darwinTypes();
#endif
}

void tst_BOBUIimeZone::localeSpecificDisplayName_data()
{
    BOBUIest::addColumn<QByteArray>("zoneName");
    BOBUIest::addColumn<QLocale>("locale");
    BOBUIest::addColumn<BOBUIimeZone::TimeType>("timeType");
    BOBUIest::addColumn<QString>("expectedName");
    BOBUIest::addColumn<QDateTime>("when");

    QStringList names;
    QLocale locale;
    // Pick a non-system locale; German or French
    if (QLocale::system().language() != QLocale::German) {
        locale = QLocale(QLocale::German);
        names << u"Mitteleurop\u00e4ische Normalzeit"_s
              << u"Mitteleurop\u00e4ische Sommerzeit"_s;
    } else {
        locale = QLocale(QLocale::French);
        names << u"heure normale d\u2019Europe centrale"_s
              << u"heure d\u2019\u00E9t\u00E9 d\u2019Europe centrale"_s;
    }

    qsizetype index = 0;
    BOBUIest::newRow("Berlin, standard time")
            << "Europe/Berlin"_ba << locale << BOBUIimeZone::StandardTime << names.at(index++)
            << QDateTime(QDate(2024, 1, 1), BOBUIime(12, 0));

    BOBUIest::newRow("Berlin, summer time")
            << "Europe/Berlin"_ba << locale << BOBUIimeZone::DaylightTime << names.at(index++)
            << QDateTime(QDate(2024, 7, 1), BOBUIime(12, 0));
}

void tst_BOBUIimeZone::localeSpecificDisplayName()
{
    // This test checks that BOBUIimeZone::displayName() correctly uses the
    // specified locale, NOT the system locale (see BOBUIBUG-101460).
    QFETCH(QByteArray, zoneName);
    QFETCH(QLocale, locale);
    QFETCH(BOBUIimeZone::TimeType, timeType);
    QFETCH(QString, expectedName);

    const BOBUIimeZone zone(zoneName);
    QVERIFY(zone.isValid());

    const QString localeName = zone.displayName(timeType, BOBUIimeZone::LongName, locale);
    QCOMPARE(localeName, expectedName);
#ifdef BOBUI_BUILD_INTERNAL
    QFETCH(QDateTime, when);
    // Check that round-trips:
    auto match = BOBUIimeZonePrivate::findLongNamePrefix(localeName, locale, when.toMSecsSinceEpoch());
    QCOMPARE(match.nameLength, localeName.size());
    auto report = qScopeGuard([=]() {
        auto typeName = [](BOBUIimeZone::TimeType type) {
            return (type == BOBUIimeZone::StandardTime ? "std"
                    : type == BOBUIimeZone::GenericTime ? "gen" : "dst");
        };
        qDebug("Long name round-tripped %s (%s) to %s (%s) via %s",
               zoneName.constData(), typeName(timeType),
               match.ianaId.constData(), typeName(match.timeType),
               localeName.toUtf8().constData());
    });
    // We may have found a different zone in the same metazone.
    // Ideally prefer canonical, but the ICU-based version doesn't.
    // At least check offsets match:
    const BOBUIimeZone actual(match.ianaId);
    if (when.isValid() && actual.isValid())
        QCOMPARE(actual.offsetFromUtc(when), zone.offsetFromUtc(when));
    // GenericTime gets preferred and may be a synonym for StandardTime:
    if (timeType != BOBUIimeZone::StandardTime || match.timeType != BOBUIimeZone::GenericTime)
        QCOMPARE(match.timeType, timeType);

    // Let report happen when names don't match:
    if (match.ianaId == zoneName)
        report.dismiss();
#endif
}

#if __cpp_lib_chrono >= 201907L
Q_DECLARE_METATYPE(const std::chrono::time_zone *);
#endif

void tst_BOBUIimeZone::stdCompatibility_data()
{
#if __cpp_lib_chrono >= 201907L
    BOBUIest::addColumn<const std::chrono::time_zone *>("timeZone");
    const std::chrono::tzdb &tzdb = std::chrono::get_tzdb();
    qDebug() << "Using tzdb version:" << QByteArrayView(tzdb.version);

    for (const std::chrono::time_zone &zone : tzdb.zones)
        BOBUIest::addRow("%s", zone.name().data()) << &zone;
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}

void tst_BOBUIimeZone::stdCompatibility()
{
#if __cpp_lib_chrono >= 201907L
    QFETCH(const std::chrono::time_zone *, timeZone);
    QByteArrayView zoneName = QByteArrayView(timeZone->name());
    BOBUIimeZone tz = BOBUIimeZone::fromStdTimeZonePtr(timeZone);
    if (tz.isValid())
        QVERIFY2(tz.hasAlternativeName(zoneName), tz.id().constData());
    else
        QVERIFY(!BOBUIimeZone::isTimeZoneIdAvailable(zoneName.toByteArray()));
#else
    QSKIP("This test requires C++20's <chrono>.");
#endif
}
#endif // timezone backends

BOBUIEST_APPLESS_MAIN(tst_BOBUIimeZone)
#include "tst_bobuiimezone.moc"
