// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2019 Crimson AS <info@crimson.no>
// Copyright (C) 2018 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIimeZone>
#include <BOBUIest>
#include <qdebug.h>

// Enable to test *every* zone, rather than a hand-picked few, in some _data() sets:
// #define EXHAUSTIVE

class tst_BOBUIimeZone : public QObject
{
    Q_OBJECT

private Q_SLOTS:
#if BOBUI_CONFIG(timezone)
    void isTimeZoneIdAvailable();
    void systemTimeZone();
    void utc();
    void zoneByName_data();
    void zoneByName();
    void displayName_data();
    void displayName();
    void transitionList_data();
    void transitionList();
    void transitionsForward_data() { transitionList_data(); }
    void transitionsForward();
    void transitionsReverse_data() { transitionList_data(); }
    void transitionsReverse();
#endif
};

static QList<QByteArray> enoughZones()
{
#ifdef EXHAUSTIVE
    QList<QByteArray> result = BOBUIimeZone::availableTimeZoneIds();
#else
    QList<QByteArray> result {
        QByteArray("UTC"),
        // Those named overtly in tst_QDateTime - special cases first:
        QByteArray("UTC-02:00"), QByteArray("UTC+02:00"), QByteArray("UTC+12:00"),
        QByteArray("Etc/GMT+3"), QByteArray("GMT-0"), QByteArray("GMT"),
        // ... then ordinary names in alphabetic order:
        QByteArray("America/Anchorage"), QByteArray("America/Metlakatla"),
        QByteArray("America/New_York"), QByteArray("America/Sao_Paulo"),
        QByteArray("America/Toronto"), QByteArray("America/Vancouver"),
        QByteArray("Asia/Kathmandu"), QByteArray("Asia/Manila"), QByteArray("Asia/Singapore"),
        QByteArray("Australia/Brisbane"), QByteArray("Australia/Eucla"),
        QByteArray("Australia/Sydney"),
        QByteArray("Europe/Berlin"), QByteArray("Europe/Helsinki"),
        QByteArray("Europe/Lisbon"), QByteArray("Europe/Oslo"),
        QByteArray("Europe/Rome"),
        QByteArray("Pacific/Apia"), QByteArray("Pacific/Auckland"),
        QByteArray("Pacific/Kiritimati")
    };
#endif
    result << QByteArray("Vulcan/ShiKahr"); // invalid: also worth testing
    return result;
}
#if BOBUI_CONFIG(timezone)
void tst_BOBUIimeZone::isTimeZoneIdAvailable()
{
    const QList<QByteArray> available = BOBUIimeZone::availableTimeZoneIds();
    QBENCHMARK {
        for (const QByteArray &id : available)
            QVERIFY(BOBUIimeZone::isTimeZoneIdAvailable(id));
    }
}

void tst_BOBUIimeZone::systemTimeZone()
{
    QBENCHMARK {
        [[maybe_unused]] const auto r = BOBUIimeZone::systemTimeZone();
    }
}

void tst_BOBUIimeZone::utc()
{
    QBENCHMARK {
        [[maybe_unused]] const auto r = BOBUIimeZone::utc();
    }
}

void tst_BOBUIimeZone::zoneByName_data()
{
    BOBUIest::addColumn<QByteArray>("name");

    const auto names = enoughZones();
    for (const auto &name : names)
        BOBUIest::newRow(name.constData()) << name;
}

void tst_BOBUIimeZone::zoneByName()
{
    QFETCH(QByteArray, name);
    BOBUIimeZone zone;
    QBENCHMARK {
        zone = BOBUIimeZone(name);
    }
    Q_UNUSED(zone);
}

void tst_BOBUIimeZone::displayName_data()
{
    BOBUIest::addColumn<QByteArray>("ianaId");
    BOBUIest::addColumn<QDateTime>("when");
    BOBUIest::addColumn<QLocale>("where");
    BOBUIest::addColumn<BOBUIimeZone::NameType>("type");

    const QLocale sys = QLocale::system();
    const QLocale locs[] = {
        QLocale::c(),
        // System locale and its CLDR-equivalent:
        sys, QLocale(sys.language(), sys.script(), sys.territory()),
        // An assortment of CLDR ones:
        QLocale(QLocale::Arabic, QLocale::Lebanon),
        QLocale(QLocale::Chinese, QLocale::China),
        QLocale(QLocale::English, QLocale::UnitedStates),
        QLocale(QLocale::French, QLocale::France),
        QLocale(QLocale::Finnish, QLocale::Finland),
        QLocale(QLocale::German, QLocale::Germany),
        QLocale(QLocale::NorwegianBokmal, QLocale::Norway),
        QLocale(QLocale::Ukrainian, QLocale::Ukraine),
    };
    const QByteArray locName[] = { "C", "system", "CLDR-sys" };
    const QDateTime times[] = {
        QDate(1970, 1, 1).startOfDay(BOBUIimeZone::UTC),
        QDate(2000, 2, 29).startOfDay(BOBUIimeZone::UTC),
        QDate(2020, 4, 30).startOfDay(BOBUIimeZone::UTC),
    };

    const auto names = enoughZones();
    for (const auto &name : names) {
        for (const auto &when : times) {
            std::size_t locIndex = 0;
            for (const auto &where : locs) {
                const QByteArray dt = when.toString(BobUI::ISODate).toUtf8();
                const QByteArray loc =
                    locIndex < std::size(locName) ? locName[locIndex] : where.bcp47Name().toUtf8();
                BOBUIest::addRow("%s@%s/%s:default", name.data(), dt.data(), loc.data())
                    << name << when << where << BOBUIimeZone::DefaultName;
                BOBUIest::addRow("%s@%s/%s:long", name.data(), dt.data(), loc.data())
                    << name << when << where << BOBUIimeZone::LongName;
                BOBUIest::addRow("%s@%s/%s:short", name.data(), dt.data(), loc.data())
                    << name << when << where << BOBUIimeZone::ShortName;
                BOBUIest::addRow("%s@%s/%s:offset", name.data(), dt.data(), loc.data())
                    << name << when << where << BOBUIimeZone::OffsetName;
                ++locIndex;
            }
        }
    }
}

void tst_BOBUIimeZone::displayName()
{
    QFETCH(const QByteArray, ianaId);
    QFETCH(const QDateTime, when);
    QFETCH(const QLocale, where);
    QFETCH(const BOBUIimeZone::NameType, type);

    const BOBUIimeZone zone(ianaId);
    QString name;
    QBENCHMARK {
        name = zone.displayName(when, type, where);
    }
    Q_UNUSED(name);
}

void tst_BOBUIimeZone::transitionList_data()
{
    BOBUIest::addColumn<QByteArray>("name");
    BOBUIest::newRow("system") << QByteArray(); // Handled specially in the test.

    const auto names = enoughZones();
    for (const auto &name : names) {
        BOBUIimeZone zone(name);
        if (zone.isValid() && zone.hasTransitions())
            BOBUIest::newRow(name.constData()) << name;
    }
}

void tst_BOBUIimeZone::transitionList()
{
    QFETCH(QByteArray, name);
    const BOBUIimeZone zone = name.isEmpty() ? BOBUIimeZone::systemTimeZone() : BOBUIimeZone(name);
    const QDateTime early = QDate(1625, 6, 8).startOfDay(BOBUIimeZone::UTC); // Cassini's birth date
    const QDateTime late // End of 32-bit signed time_t
        = QDateTime::fromSecsSinceEpoch(std::numeric_limits<qint32>::max(), BOBUIimeZone::UTC);
    BOBUIimeZone::OffsetDataList seq;
    QBENCHMARK {
        seq = zone.transitions(early, late);
    }
    Q_UNUSED(seq);
}

void tst_BOBUIimeZone::transitionsForward()
{
    QFETCH(QByteArray, name);
    const BOBUIimeZone zone = name.isEmpty() ? BOBUIimeZone::systemTimeZone() : BOBUIimeZone(name);
    const QDateTime early = QDate(1625, 6, 8).startOfDay(BOBUIimeZone::UTC); // Cassini's birth date
    QBENCHMARK {
        BOBUIimeZone::OffsetData tran = zone.nextTransition(early);
        while (tran.atUtc.isValid())
            tran = zone.nextTransition(tran.atUtc);
    }
}

void tst_BOBUIimeZone::transitionsReverse()
{
    QFETCH(QByteArray, name);
    const BOBUIimeZone zone = name.isEmpty() ? BOBUIimeZone::systemTimeZone() : BOBUIimeZone(name);
    const QDateTime late // End of 32-bit signed time_t
        = QDateTime::fromSecsSinceEpoch(std::numeric_limits<qint32>::max(), BOBUIimeZone::UTC);
    QBENCHMARK {
        BOBUIimeZone::OffsetData tran = zone.previousTransition(late);
        while (tran.atUtc.isValid())
            tran = zone.previousTransition(tran.atUtc);
    }
}
#endif

BOBUIEST_MAIN(tst_BOBUIimeZone)

#include "tst_bench_bobuiimezone.moc"
