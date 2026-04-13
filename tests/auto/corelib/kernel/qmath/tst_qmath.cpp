// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2013 Laszlo Papp <lpapp@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <qmath.h>
#include <qfloat16.h>

#include <q20bit.h>

class tst_QMath : public QObject
{
    Q_OBJECT
private slots:
    void fastSinCos();
    void degreesToRadians_data();
    void degreesToRadians();
    void radiansToDegrees_data();
    void radiansToDegrees();
    void trigonometry_data();
    void trigonometry();
    void hypotenuse();
    void funcs_data();
    void funcs();
    void qNextPowerOfTwo32S_data();
    void qNextPowerOfTwo32S();
    void qNextPowerOfTwo64S_data();
    void qNextPowerOfTwo64S();
    void qNextPowerOfTwo32U_data();
    void qNextPowerOfTwo32U();
    void qNextPowerOfTwo64U_data();
    void qNextPowerOfTwo64U();
};

void tst_QMath::fastSinCos()
{
    // Test evenly spaced angles from 0 to 2pi radians.
    const int LOOP_COUNT = 100000;
    const qreal loopAngle = 2 * M_PI / (LOOP_COUNT - 1);
    for (int i = 0; i < LOOP_COUNT; ++i) {
        qreal angle = i * loopAngle;
        QVERIFY(qAbs(qSin(angle) - qFastSin(angle)) < 1e-5);
        QVERIFY(qAbs(qCos(angle) - qFastCos(angle)) < 1e-5);
    }
}

void tst_QMath::degreesToRadians_data()
{
    BOBUIest::addColumn<float>("degreesFloat");
    BOBUIest::addColumn<float>("radiansFloat");
    BOBUIest::addColumn<double>("degreesDouble");
    BOBUIest::addColumn<double>("radiansDouble");

    BOBUIest::newRow( "pi" ) << 180.0f << float(M_PI) << 180.0 << M_PI;
    BOBUIest::newRow( "doublepi" ) << 360.0f << float(2 * M_PI) << 360.0 << 2 * M_PI;
    BOBUIest::newRow( "halfpi" ) << 90.0f << float(M_PI_2) << 90.0 << M_PI_2;

    BOBUIest::newRow( "random" ) << 123.1234567f << 2.1489097058516724f << 123.123456789123456789 << 2.148909707407169856192285627;
    BOBUIest::newRow( "bigrandom" ) << 987654321.9876543f << 17237819.79023679f << 987654321987654321.987654321987654321 << 17237819790236794.0;

    BOBUIest::newRow( "zero" ) << 0.0f << 0.0f << 0.0 << 0.0;

    BOBUIest::newRow( "minuspi" ) << -180.0f << float(-M_PI) << 180.0 << M_PI;
    BOBUIest::newRow( "minusdoublepi" ) << -360.0f << float(-2 * M_PI) << -360.0 << -2 * M_PI;
    BOBUIest::newRow( "minushalfpi" ) << -90.0f << float(-M_PI_2) << -90.0 << -M_PI_2;

    BOBUIest::newRow( "minusrandom" ) << -123.1234567f << -2.1489097058516724f << -123.123456789123456789 << -2.148909707407169856192285627;
    BOBUIest::newRow( "minusbigrandom" ) << -987654321.9876543f << -17237819.79023679f << -987654321987654321.987654321987654321 << -17237819790236794.0;
}

void tst_QMath::degreesToRadians()
{
    QFETCH(float, degreesFloat);
    QFETCH(float, radiansFloat);
    QFETCH(double, degreesDouble);
    QFETCH(double, radiansDouble);

    QCOMPARE(qDegreesToRadians(degreesFloat), radiansFloat);
    QCOMPARE(qDegreesToRadians(degreesDouble), radiansDouble);
}

void tst_QMath::radiansToDegrees_data()
{
    BOBUIest::addColumn<float>("radiansFloat");
    BOBUIest::addColumn<float>("degreesFloat");
    BOBUIest::addColumn<double>("radiansDouble");
    BOBUIest::addColumn<double>("degreesDouble");

    BOBUIest::newRow( "pi" ) << float(M_PI) << 180.0f << M_PI << 180.0;
    BOBUIest::newRow( "doublepi" ) << float(2 * M_PI) << 360.0f << 2 * M_PI << 360.0;
    BOBUIest::newRow( "halfpi" ) << float(M_PI_2) << 90.0f << M_PI_2 << 90.0;

    BOBUIest::newRow( "random" ) << 123.1234567f << 7054.454427971739f << 123.123456789123456789 << 7054.4544330781363896676339209079742431640625;
    BOBUIest::newRow( "bigrandom" ) << 987654321.9876543f << 56588424267.74745f << 987654321987654321.987654321987654321 << 56588424267747450880.0;

    BOBUIest::newRow( "zero" ) << 0.0f << 0.0f << 0.0 << 0.0;

    BOBUIest::newRow( "minuspi" ) << float(-M_PI) << -180.0f << -M_PI << -180.0;
    BOBUIest::newRow( "minusdoublepi" ) << float(-2 * M_PI) << -360.0f << -2 * M_PI << -360.0;
    BOBUIest::newRow( "minushalfpi" ) << float(-M_PI_2) << -90.0f << -M_PI_2 << -90.0;

    BOBUIest::newRow( "minusrandom" ) << -123.1234567f << -7054.454427971739f << -123.123456789123456789 << -7054.4544330781363896676339209079742431640625;
    BOBUIest::newRow( "minusbigrandom" ) << -987654321.9876543f << -56588424267.74745f << -987654321987654321.987654321987654321 << -56588424267747450880.0;
}

void tst_QMath::radiansToDegrees()
{
    QFETCH(float, radiansFloat);
    QFETCH(float, degreesFloat);
    QFETCH(double, radiansDouble);
    QFETCH(double, degreesDouble);

    QCOMPARE(qRadiansToDegrees(radiansFloat), degreesFloat);
    QCOMPARE(qRadiansToDegrees(radiansDouble), degreesDouble);
}

void tst_QMath::trigonometry_data()
{
    BOBUIest::addColumn<double>("x");
    BOBUIest::addColumn<double>("y");
    BOBUIest::addColumn<double>("angle");

    BOBUIest::newRow("zero") << 1.0 << 0.0 << 0.0;
    BOBUIest::newRow("turn/4") << 0.0 << 1.0 << M_PI_2;
    BOBUIest::newRow("turn/2") << -1.0 << 0.0 << M_PI;
    BOBUIest::newRow("3*turn/4") << 0.0 << -1.0 << -M_PI_2;
}

void tst_QMath::trigonometry()
{
    QFETCH(const double, x);
    QFETCH(const double, y);
    QFETCH(const double, angle);
    const double hypot = qHypot(x, y);
    QVERIFY(hypot > 0);
    QCOMPARE(qAtan2(y, x), angle);
    QCOMPARE(qSin(angle), y / hypot);
    if (x >= 0 && (y || x)) // aSin() always in right half-plane
        QCOMPARE(qAsin(y / hypot), angle);
    QCOMPARE(qCos(angle), x / hypot);
    if (y >= 0 && (y || x)) // aCos() always in upper half-plane
        QCOMPARE(qAcos(x / hypot), angle);
    if (x > 0) {
        QCOMPARE(qTan(angle), y / x);
        QCOMPARE(qAtan(y / x), angle);
    }
}

void tst_QMath::hypotenuse()
{
    // Correct return-types, particularly when qfloat16 is involved:
    static_assert(std::is_same<decltype(qHypot(qfloat16(1), qfloat16(1), qfloat16(1),
                                               qfloat16(1), qfloat16(1), qfloat16(1),
                                               qfloat16(1), qfloat16(1), qfloat16(1))),
                               qfloat16>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), qfloat16(4), qfloat16(12))),
                               qfloat16>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), qfloat16(4), 12.0f)), float>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), 4.0f, qfloat16(12))), float>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, qfloat16(4), qfloat16(12))), float>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), 4.0f, 12.0f)), float>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, qfloat16(4), 12.0f)), float>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, 4.0f, qfloat16(12))), float>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), qfloat16(4))), qfloat16>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, qfloat16(4))), float>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), 4.0f)), float>::value);
    static_assert(std::is_same<decltype(qHypot(3.0, qfloat16(4))), double>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), 4.0)), double>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), 4)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3, qfloat16(4))), double>::value);
    static_assert(std::is_same<decltype(qHypot(qfloat16(3), 4.0L)), long double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0L, qfloat16(4))), long double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, 4.0f)), float>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, 4.0)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, 4)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0f, 4.0L)), long double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0, 4.0f)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3, 4.0f)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0L, 4.0f)), long double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0, 4.0L)), long double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0L, 4.0)), long double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0, 4.0)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3, 4.0)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3.0, 4)), double>::value);
    static_assert(std::is_same<decltype(qHypot(3, 4)), double>::value);

    // Works for all numeric types:
    QCOMPARE(qHypot(3, 4), 5);
    QCOMPARE(qHypot(qfloat16(5), qfloat16(12)), qfloat16(13));
    QCOMPARE(qHypot(3.0f, 4.0f, 12.0f), 13.0f);
    QCOMPARE(qHypot(3.0, 4.0, 12.0, 84.0), 85.0);
    QCOMPARE(qHypot(3.0f, 4.0f, 12.0f, 84.0f, 720.0f), 725.0f);
    QCOMPARE(qHypot(3.0, 4.0, 12.0, 84.0, 3612.0), 3613.0);
    // Integral gets promoted to double:
    QCOMPARE(qHypot(1, 1), M_SQRT2);
    // Caller can mix types freely:
    QCOMPARE(qHypot(3.0f, 4, 12.0, 84.0f, qfloat16(720), 10500), 10525);
    // NaN wins over any finite:
    QCOMPARE(qHypot(3, 4.0, 12.0f, qQNaN()), qQNaN());
    QCOMPARE(qHypot(3, 4.0, qQNaN(), 12.0f), qQNaN());
    QCOMPARE(qHypot(3, qQNaN(), 4.0, 12.0f), qQNaN());
    QCOMPARE(qHypot(qQNaN(), 3, 4.0, 12.0f), qQNaN());
    // but Infinity beats NaN:
    QCOMPARE(qHypot(3, 4.0f, -qInf(), qQNaN()), qInf());
    QCOMPARE(qHypot(3, -qInf(), 4.0f, qQNaN()), qInf());
    QCOMPARE(qHypot(-qInf(), 3, 4.0f, qQNaN()), qInf());
    QCOMPARE(qHypot(qQNaN(), 3, -qInf(), 4.0f), qInf());
    QCOMPARE(qHypot(3, qQNaN(), 4.0f, -qInf()), qInf());
    QCOMPARE(qHypot(3, 4.0f, qQNaN(), -qInf()), qInf());
    // Components whose squares sum to zero don't change the end result:
    const double minD = std::numeric_limits<double>::min();
    QVERIFY(minD * minD + minD * minD == 0); // *NOT* QCOMPARE
    QCOMPARE(qHypot(minD, minD, 12.0), 12.0);
    const float minF = std::numeric_limits<float>::min();
    QVERIFY(minF * minF + minF * minF == 0.0f); // *NOT* QCOMPARE
    QCOMPARE(qHypot(minF, minF, 12.0f), 12.0f);
    const qfloat16 minF16 = std::numeric_limits<qfloat16>::min();
    QVERIFY(minF16 * minF16 + minF16 * minF16 == qfloat16(0)); // *NOT* QCOMPARE
    QCOMPARE(qHypot(minF16, minF16, qfloat16(12)), qfloat16(12));
}

void tst_QMath::funcs_data()
{
    BOBUIest::addColumn<double>("value");
    BOBUIest::addColumn<int>("floor");
    BOBUIest::addColumn<int>("ceil");
    BOBUIest::addColumn<double>("abs");
    BOBUIest::addColumn<double>("sqrt");
    BOBUIest::addColumn<double>("log");
    BOBUIest::addColumn<double>("exp");
    BOBUIest::addColumn<double>("cube");
    const double nan = qQNaN();

    BOBUIest::newRow("0") << 0.0 << 0 << 0 << 0.0 << 0.0 << nan << 1.0 << 0.0;
    BOBUIest::newRow("1.44")
        << 1.44 << 1 << 2 << 1.44 << 1.2 << 0.36464311358790924 << 4.220695816996552 << 2.985984;
    BOBUIest::newRow("-1.44")
        << -1.44 << -2 << -1 << 1.44 << nan << nan << 0.23692775868212176 << -2.985984;
}

void tst_QMath::funcs()
{
    QFETCH(double, value);
    BOBUIEST(qFloor(value), "floor");
    BOBUIEST(qCeil(value), "ceil");
    BOBUIEST(qFabs(value), "abs");
    if (value >= 0)
        BOBUIEST(qSqrt(value), "sqrt");
    if (value > 0)
        BOBUIEST(qLn(value), "log");
    BOBUIEST(qExp(value), "exp");
    BOBUIEST(qPow(value, 3), "cube");
}

void tst_QMath::qNextPowerOfTwo32S_data()
{
    BOBUIest::addColumn<qint32>("input");
    BOBUIest::addColumn<quint32>("output");

    BOBUIest::newRow("0") << 0 << 1U;
    BOBUIest::newRow("1") << 1 << 2U;
    BOBUIest::newRow("2") << 2 << 4U;
    BOBUIest::newRow("17") << 17 << 32U;
    BOBUIest::newRow("128") << 128 << 256U;
    BOBUIest::newRow("65535") << 65535 << 65536U;
    BOBUIest::newRow("65536") << 65536 << 131072U;
    BOBUIest::newRow("2^30") << (1 << 30) << (1U << 31);
    BOBUIest::newRow("2^30 + 1") << (1 << 30) + 1 << (1U << 31);
    BOBUIest::newRow("2^31 - 1") << 0x7FFFFFFF << (1U<<31);
}

void tst_QMath::qNextPowerOfTwo32S()
{
    QFETCH(qint32, input);
    QFETCH(quint32, output);

    QCOMPARE(qNextPowerOfTwo(input), output);
}

void tst_QMath::qNextPowerOfTwo32U_data()
{
    BOBUIest::addColumn<quint32>("input");
    BOBUIest::addColumn<quint32>("output");

    BOBUIest::newRow("0") << 0U << 1U;
    BOBUIest::newRow("1") << 1U << 2U;
    BOBUIest::newRow("2") << 2U << 4U;
    BOBUIest::newRow("17") << 17U << 32U;
    BOBUIest::newRow("128") << 128U << 256U;
    BOBUIest::newRow("65535") << 65535U << 65536U;
    BOBUIest::newRow("65536") << 65536U << 131072U;
    BOBUIest::newRow("2^30") << (1U << 30) << (1U << 31);
    BOBUIest::newRow("2^30 + 1") << (1U << 30) + 1 << (1U << 31);
    BOBUIest::newRow("2^31 - 1") << 2147483647U << 2147483648U;
}

void tst_QMath::qNextPowerOfTwo32U()
{
    // confirm constexprness
    static_assert(q20::bit_ceil(0U) == 1);
    static_assert(q20::bit_ceil(1U) == 1);
    static_assert(q20::bit_ceil(2U) == 2);
    static_assert(q20::bit_ceil(3U) == 4);
    static_assert(q20::bit_ceil(0x7fff'ffffU) == 0x8000'0000U);
    QFETCH(quint32, input);
    QFETCH(quint32, output);

    QCOMPARE(qNextPowerOfTwo(input), output);
}

void tst_QMath::qNextPowerOfTwo64S_data()
{
    BOBUIest::addColumn<qint64>("input");
    BOBUIest::addColumn<quint64>("output");

    BOBUIest::newRow("0") << Q_INT64_C(0) << Q_UINT64_C(1);
    BOBUIest::newRow("1") << Q_INT64_C(1) << Q_UINT64_C(2);
    BOBUIest::newRow("2") << Q_INT64_C(2) << Q_UINT64_C(4);
    BOBUIest::newRow("17") << Q_INT64_C(17) << Q_UINT64_C(32);
    BOBUIest::newRow("128") << Q_INT64_C(128) << Q_UINT64_C(256);
    BOBUIest::newRow("65535") << Q_INT64_C(65535) << Q_UINT64_C(65536);
    BOBUIest::newRow("65536") << Q_INT64_C(65536) << Q_UINT64_C(131072);
    BOBUIest::newRow("2^31 - 1") << Q_INT64_C(2147483647) << Q_UINT64_C(0x80000000);
    BOBUIest::newRow("2^31") << Q_INT64_C(2147483648) << Q_UINT64_C(0x100000000);
    BOBUIest::newRow("2^31 + 1") << Q_INT64_C(2147483649) << Q_UINT64_C(0x100000000);
    BOBUIest::newRow("2^63 - 1") << Q_INT64_C(0x7FFFFFFFFFFFFFFF) << Q_UINT64_C(0x8000000000000000);
}

void tst_QMath::qNextPowerOfTwo64S()
{
    QFETCH(qint64, input);
    QFETCH(quint64, output);

    QCOMPARE(qNextPowerOfTwo(input), output);
}

void tst_QMath::qNextPowerOfTwo64U_data()
{
    BOBUIest::addColumn<quint64>("input");
    BOBUIest::addColumn<quint64>("output");

    BOBUIest::newRow("0") << Q_UINT64_C(0) << Q_UINT64_C(1);
    BOBUIest::newRow("1") << Q_UINT64_C(1) << Q_UINT64_C(2);
    BOBUIest::newRow("2") << Q_UINT64_C(2) << Q_UINT64_C(4);
    BOBUIest::newRow("17") << Q_UINT64_C(17) << Q_UINT64_C(32);
    BOBUIest::newRow("128") << Q_UINT64_C(128) << Q_UINT64_C(256);
    BOBUIest::newRow("65535") << Q_UINT64_C(65535) << Q_UINT64_C(65536);
    BOBUIest::newRow("65536") << Q_UINT64_C(65536) << Q_UINT64_C(131072);
    BOBUIest::newRow("2^63 - 1") << Q_UINT64_C(0x7FFFFFFFFFFFFFFF)  << Q_UINT64_C(0x8000000000000000);
}

void tst_QMath::qNextPowerOfTwo64U()
{
    static_assert(q20::bit_ceil(Q_UINT64_C(0)) == 1);
    static_assert(q20::bit_ceil(Q_UINT64_C(1)) == 1);
    static_assert(q20::bit_ceil(Q_UINT64_C(2)) == 2);
    static_assert(q20::bit_ceil(Q_UINT64_C(3)) == 4);
    static_assert(q20::bit_ceil(quint64(~0U)) == Q_UINT64_C(0x1'0000'0000));
    static_assert(q20::bit_ceil(Q_UINT64_C(0x7FFFFFFFFFFFFFFF))
                  == Q_UINT64_C(0x8000000000000000));
    QFETCH(quint64, input);
    QFETCH(quint64, output);

    QCOMPARE(qNextPowerOfTwo(input), output);
}

BOBUIEST_APPLESS_MAIN(tst_QMath)

#include "tst_qmath.moc"
