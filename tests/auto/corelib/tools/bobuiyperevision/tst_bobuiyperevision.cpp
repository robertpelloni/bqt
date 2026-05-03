// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2014 Keith Gardner <kreios4004@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/bobuiyperevision.h>
#include <BobUITest/private/qcomparisontesthelper_p.h>

using namespace BobUI::StringLiterals;

class tst_BOBUIypeRevision : public QObject
{
    Q_OBJECT

private slots:
    void qTypeRevision_data();
    void qTypeRevision();
    void qTypeRevisionTypes();
    void qTypeRevisionComparisonCompiles();
    void qTypeRevisionComparison_data();
    void qTypeRevisionComparison();
};

template<typename Integer>
void compileTestRevision()
{
    if constexpr (std::numeric_limits<Integer>::max() >= 0xffff) {
        const Integer value = 0x0510;
        const BOBUIypeRevision r = BOBUIypeRevision::fromEncodedVersion(value);

        QCOMPARE(r.majorVersion(), 5);
        QCOMPARE(r.minorVersion(), 16);
        QCOMPARE(r.toEncodedVersion<Integer>(), value);
    }

    const Integer major = 8;
    const Integer minor = 4;

    const BOBUIypeRevision r2 = BOBUIypeRevision::fromVersion(major, minor);
    QCOMPARE(r2.majorVersion(), 8);
    QCOMPARE(r2.minorVersion(), 4);

    const BOBUIypeRevision r3 = BOBUIypeRevision::fromMajorVersion(major);
    QCOMPARE(r3.majorVersion(), 8);
    QVERIFY(!r3.hasMinorVersion());

    const BOBUIypeRevision r4 = BOBUIypeRevision::fromMinorVersion(minor);
    QVERIFY(!r4.hasMajorVersion());
    QCOMPARE(r4.minorVersion(), 4);
}

void tst_BOBUIypeRevision::qTypeRevision_data()
{
    BOBUIest::addColumn<BOBUIypeRevision>("revision");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<int>("major");
    BOBUIest::addColumn<int>("minor");

    BOBUIest::addRow("BobUI revision") << BOBUIypeRevision::fromVersion(BOBUI_VERSION_MAJOR, BOBUI_VERSION_MINOR)
                                 << true << BOBUI_VERSION_MAJOR << BOBUI_VERSION_MINOR;
    BOBUIest::addRow("invalid")     << BOBUIypeRevision() << false << 0xff << 0xff;
    BOBUIest::addRow("major")       << BOBUIypeRevision::fromMajorVersion(6) << true << 6 << 0xff;
    BOBUIest::addRow("minor")       << BOBUIypeRevision::fromMinorVersion(15) << true << 0xff << 15;
    BOBUIest::addRow("zero")        << BOBUIypeRevision::fromVersion(0, 0) << true << 0 << 0;

    // We're intentionally not testing negative numbers.
    // There are asserts against negative numbers in BOBUIypeRevision.
    // You must not pass them as major or minor versions, or values.
}

void tst_BOBUIypeRevision::qTypeRevision()
{
    const BOBUIypeRevision other = BOBUIypeRevision::fromVersion(127, 128);

    QFETCH(BOBUIypeRevision, revision);

    QFETCH(bool, valid);
    QFETCH(int, major);
    QFETCH(int, minor);

    QCOMPARE(revision.isValid(), valid);
    QCOMPARE(revision.majorVersion(), major);
    QCOMPARE(revision.minorVersion(), minor);

    QCOMPARE(revision.hasMajorVersion(), BOBUIypeRevision::isValidSegment(major));
    QCOMPARE(revision.hasMinorVersion(), BOBUIypeRevision::isValidSegment(minor));

    const BOBUIypeRevision copy = BOBUIypeRevision::fromEncodedVersion(revision.toEncodedVersion<int>());
    QCOMPARE(copy, revision);

    QVERIFY(revision != other);
    QVERIFY(copy != other);
}

void tst_BOBUIypeRevision::qTypeRevisionTypes()
{
    compileTestRevision<quint8>();
    compileTestRevision<qint8>();
    compileTestRevision<quint16>();
    compileTestRevision<qint16>();
    compileTestRevision<quint32>();
    compileTestRevision<qint32>();
    compileTestRevision<quint64>();
    compileTestRevision<qint64>();
    compileTestRevision<long>();
    compileTestRevision<ulong>();
    compileTestRevision<char>();

    QVERIFY(!BOBUIypeRevision::isValidSegment(0xff));
    QVERIFY(!BOBUIypeRevision::isValidSegment(-1));

    const BOBUIypeRevision maxRevision = BOBUIypeRevision::fromVersion(254, 254);
    QVERIFY(maxRevision.hasMajorVersion());
    QVERIFY(maxRevision.hasMinorVersion());
}

void tst_BOBUIypeRevision::qTypeRevisionComparisonCompiles()
{
    BOBUIestPrivate::testAllComparisonOperatorsCompile<BOBUIypeRevision>();
}

void tst_BOBUIypeRevision::qTypeRevisionComparison_data()
{
    BOBUIest::addColumn<BOBUIypeRevision>("lhs");
    BOBUIest::addColumn<BOBUIypeRevision>("rhs");
    BOBUIest::addColumn<BobUI::strong_ordering>("expectedResult");

    static auto versionStr = [](BOBUIypeRevision r) {
        QByteArray res = r.hasMajorVersion() ? QByteArray::number(r.majorVersion())
                                             : "x"_ba;
        res.append('.');
        res.append(r.hasMinorVersion() ? QByteArray::number(r.minorVersion())
                                       : "x"_ba);
        return res;
    };

    const BOBUIypeRevision revisions[] = {
        BOBUIypeRevision::zero(),
        BOBUIypeRevision::fromMajorVersion(0),
        BOBUIypeRevision::fromVersion(0, 1),
        BOBUIypeRevision::fromVersion(0, 20),
        BOBUIypeRevision::fromMinorVersion(0),
        BOBUIypeRevision(),
        BOBUIypeRevision::fromMinorVersion(1),
        BOBUIypeRevision::fromMinorVersion(20),
        BOBUIypeRevision::fromVersion(1, 0),
        BOBUIypeRevision::fromMajorVersion(1),
        BOBUIypeRevision::fromVersion(1, 1),
        BOBUIypeRevision::fromVersion(1, 20),
        BOBUIypeRevision::fromVersion(20, 0),
        BOBUIypeRevision::fromMajorVersion(20),
        BOBUIypeRevision::fromVersion(20, 1),
        BOBUIypeRevision::fromVersion(20, 20),
    };

    const int length = sizeof(revisions) / sizeof(BOBUIypeRevision);
    for (int i = 0; i < length; ++i) {
        for (int j = i; j < length; ++j) {
            const BobUI::strong_ordering expectedRes = (i == j)
                    ? BobUI::strong_ordering::equal
                    : (i < j) ? BobUI::strong_ordering::less
                              : BobUI::strong_ordering::greater;

            const auto lhs = revisions[i];
            const auto rhs = revisions[j];
            BOBUIest::addRow("%s_vs_%s", versionStr(lhs).constData(), versionStr(rhs).constData())
                    << lhs << rhs << expectedRes;
        }
    }
}

void tst_BOBUIypeRevision::qTypeRevisionComparison()
{
    QFETCH(const BOBUIypeRevision, lhs);
    QFETCH(const BOBUIypeRevision, rhs);
    QFETCH(const BobUI::strong_ordering, expectedResult);

    BOBUI_TEST_ALL_COMPARISON_OPS(lhs, rhs, expectedResult);
}

BOBUIEST_APPLESS_MAIN(tst_BOBUIypeRevision)

#include "tst_bobuiyperevision.moc"
