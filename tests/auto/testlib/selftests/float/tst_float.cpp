// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BobUICore/qfloat16.h>
#include <BOBUIest>
#include <QDebug>

#include <BobUITest/private/qemulationdetector_p.h>

#include <fenv.h>

// Test proper handling of floating-point types
class tst_float: public QObject
{
    Q_OBJECT
public:
    tst_float();
private slots:
    void doubleComparisons() const;
    void doubleComparisons_data() const;
    void floatComparisons() const;
    void floatComparisons_data() const;
    void float16Comparisons() const;
    void float16Comparisons_data() const;
    void compareFloatTests() const;
    void compareFloatTests_data() const;
    void compareQListDouble() const;
};

template<typename F> F negate(F f)
{
    // ISO C says we should just use the unary minus operator to negate any
    // input, but we do it explicitly here to survive a too-smart compiler in
    // fast-math mode.

    using U = typename QIntegerForSizeof<F>::Unsigned;
    auto u = qFromUnaligned<U>(&f);
    u |= U(1) << (std::numeric_limits<U>::digits - 1);
    return qFromUnaligned<F>(&u);
}

template<typename F> F makeNan(typename QIntegerForSizeof<F>::Signed i, bool is_quiet = true)
{
    using U = typename QIntegerForSizeof<F>::Unsigned;
    constexpr U MantissaMask = (U(1) << (std::numeric_limits<F>::digits - 1)) - 1;
    constexpr U SignBit = U(1) << (std::numeric_limits<U>::digits - 1);
    constexpr U ExponentMask = U(~(MantissaMask | SignBit));
    Q_ASSERT(i);

    U u = i;
    if (i < 0) {
        // transform two's complement into sign-magnitude
        u = -i;
        u |= SignBit;
    }
    Q_ASSERT((u & ExponentMask) == 0);
    u |= ExponentMask;
    // the is_quiet bit is the MSB of the significand
    u |= U(is_quiet) << (std::numeric_limits<F>::digits - 2);

    return qFromUnaligned<F>(&u);
}

template<typename F>
static void nonFinite_data(F zero, F one)
{
    using Bounds = std::numeric_limits<F>;

    // QCOMPARE special-cases non-finite values
    if (Bounds::has_quiet_NaN) {
        const F nan = Bounds::quiet_NaN();
        BOBUIest::newRow("should PASS: NaN == NaN") << nan << nan;
        BOBUIest::newRow("should PASS: NaN == -NaN") << nan << negate(nan);
        BOBUIest::newRow("should FAIL: NaN != 0") << nan << zero;
        BOBUIest::newRow("should FAIL: 0 != NaN") << zero << nan;
        BOBUIest::newRow("should FAIL: NaN != 1") << nan << one;
        BOBUIest::newRow("should FAIL: 1 != NaN") << one << nan;
        BOBUIest::newRow("should FAIL: -NaN != 0") << negate(nan) << zero;
        BOBUIest::newRow("should FAIL: -NaN != -0") << negate(nan) << negate(zero);

        BOBUIest::newRow("should PASS: NaN == NaN(1)") << nan << makeNan<F>(1);
        BOBUIest::newRow("should PASS: NaN(1) == NaN") << makeNan<F>(1) << nan;
        BOBUIest::newRow("should PASS: NaN == -NaN(1)") << nan << makeNan<F>(-1);
        BOBUIest::newRow("should PASS: -NaN(1) == NaN") << makeNan<F>(-1) << nan;
        BOBUIest::newRow("should FAIL: NaN(1) != 0") << makeNan<F>(1) << zero;
        BOBUIest::newRow("should FAIL: 0 != NaN(1)") << zero << makeNan<F>(1);
        BOBUIest::newRow("should FAIL: NaN(1) != 1") << makeNan<F>(1) << one;
        BOBUIest::newRow("should FAIL: 1 != NaN(1)") << one << makeNan<F>(1);

        if (Bounds::has_signaling_NaN) {
            // make SNaN with the highest and lowest bits set
            const F snanMsb = makeNan<F>(Q_INT64_C(1) << (Bounds::digits - 3), false);
            const F snanLsb = makeNan<F>(1, false);
            BOBUIest::newRow("should PASS: SNaN == SNaN") << snanMsb << snanMsb;
            BOBUIest::newRow("should PASS: SNaN == -SNaN") << snanMsb << negate(snanMsb);
            BOBUIest::newRow("should PASS: SNaN == NaN") << snanMsb << nan;
            BOBUIest::newRow("should PASS: NaN == SNaN") << nan << snanMsb;

            BOBUIest::newRow("should FAIL: SNaN != 0") << snanMsb << zero;
            BOBUIest::newRow("should FAIL: 0 != sNaN") << zero << snanMsb;
            BOBUIest::newRow("should FAIL: SNaN != 1") << snanMsb << one;
            BOBUIest::newRow("should FAIL: 1 != SNaN") << one << snanMsb;
            BOBUIest::newRow("should FAIL: -SNaN != 0") << negate(snanMsb) << zero;
            BOBUIest::newRow("should FAIL: -SNaN != -0") << negate(snanMsb) << negate(zero);

            BOBUIest::newRow("should PASS: SNaN == SNaN(1)") << snanMsb << snanLsb;
            BOBUIest::newRow("should PASS: SNaN(1) == sNaN") << snanLsb << snanMsb;
            BOBUIest::newRow("should PASS: SNaN == -SNaN(1)") << snanMsb << negate(snanLsb);
            BOBUIest::newRow("should PASS: -SNaN(1) == sNaN") << negate(snanLsb) << snanMsb;
            BOBUIest::newRow("should PASS: NaN(1) == SNaN(1)") << makeNan<F>(1) << snanLsb;
            BOBUIest::newRow("should PASS: SNaN(1) == NaN(1)") << snanLsb << makeNan<F>(1);
        }
    }

    if (Bounds::has_infinity) {
        const F uge = Bounds::infinity();
        BOBUIest::newRow("should PASS: inf == inf") << uge << uge;
        BOBUIest::newRow("should PASS: -inf == -inf") << -uge << -uge;
        BOBUIest::newRow("should FAIL: inf != -inf") << uge << -uge;
        BOBUIest::newRow("should FAIL: -inf != inf") << -uge << uge;
        if (Bounds::has_quiet_NaN) {
            const F nan = Bounds::quiet_NaN();
            BOBUIest::newRow("should FAIL: inf != nan") << uge << nan;
            BOBUIest::newRow("should FAIL: nan != inf") << nan << uge;
            BOBUIest::newRow("should FAIL: -inf != nan") << -uge << nan;
            BOBUIest::newRow("should FAIL: nan != -inf") << nan << -uge;
        }
        BOBUIest::newRow("should FAIL: inf != 0") << uge << zero;
        BOBUIest::newRow("should FAIL: 0 != inf") << zero << uge;
        BOBUIest::newRow("should FAIL: -inf != 0") << -uge << zero;
        BOBUIest::newRow("should FAIL: 0 != -inf") << zero << -uge;
        BOBUIest::newRow("should FAIL: inf != -0") << uge << negate(zero);
        BOBUIest::newRow("should FAIL: -0 != inf") << negate(zero) << uge;
        BOBUIest::newRow("should FAIL: inf != 1") << uge << one;
        BOBUIest::newRow("should FAIL: 1 != inf") << one << uge;
        BOBUIest::newRow("should FAIL: -inf != 1") << -uge << one;
        BOBUIest::newRow("should FAIL: 1 != -inf") << one << -uge;

        const F big = Bounds::max();
        BOBUIest::newRow("should FAIL: inf != max") << uge << big;
        BOBUIest::newRow("should FAIL: inf != -max") << uge << -big;
        BOBUIest::newRow("should FAIL: max != inf") << big << uge;
        BOBUIest::newRow("should FAIL: -max != inf") << -big << uge;
        BOBUIest::newRow("should FAIL: -inf != max") << -uge << big;
        BOBUIest::newRow("should FAIL: -inf != -max") << -uge << -big;
        BOBUIest::newRow("should FAIL: max != -inf") << big << -uge;
        BOBUIest::newRow("should FAIL: -max != -inf") << -big << -uge;
    }
}

tst_float::tst_float()
{
    fenv_t env;
    feholdexcept(&env);     // ensure that we don't consume exceptions
}

void tst_float::doubleComparisons() const
{
    QFETCH(double, operandLeft);
    QFETCH(double, operandRight);

    QCOMPARE(operandLeft, operandRight);
}

void tst_float::doubleComparisons_data() const
{
    BOBUIest::addColumn<double>("operandLeft");
    BOBUIest::addColumn<double>("operandRight");
    double zero(0.), one(1.);

    BOBUIest::newRow("should FAIL 1") << one << 3.;
    BOBUIest::newRow("should PASS 1") << zero << zero;
    BOBUIest::newRow("should PASS: 0 == -0") << zero << negate(zero);
    BOBUIest::newRow("should FAIL 2") << 1.e-7 << 3.e-7;

    // QCOMPARE() uses qFuzzyCompare(), which succeeds if doubles differ by no
    // more than 1e-12 times the smaller value; but QCOMPARE() also considers
    // values equal if qFuzzyIsNull() is true for both, so all doubles smaller
    // than 1e-12 are equal.  Thus QCOMPARE(1e12-2, 1e12-1) should fail, while
    // QCOMPARE(1e12+1, 1e12+2) should pass, as should QCOMPARE(1e-12-2e-24,
    // 1e-12-1e-24), despite the values differing by more than one part in 1e12.

    BOBUIest::newRow("should PASS 2") << 1e12 + one << 1e12 + 2.;
    BOBUIest::newRow("should FAIL 3") << 1e12 - one << 1e12 - 2.;
    BOBUIest::newRow("should PASS 3") << 1e-12 << -1e-12;
    // ... but rounding makes that a bit unrelaible when scaled close to the bounds.
    BOBUIest::newRow("should FAIL 4") << 1e-12 + 1e-24 << 1e-12 - 1e-24;
    BOBUIest::newRow("should PASS 4") << 1e307 + 1e295 << 1e307 + 2e295;
    BOBUIest::newRow("should FAIL 5") << 1e307 - 1e295 << 1e307 - 3e295;

    nonFinite_data(zero, one);
}

void tst_float::floatComparisons() const
{
    QFETCH(float, operandLeft);
    QFETCH(float, operandRight);

    QCOMPARE(operandLeft, operandRight);
}

void tst_float::floatComparisons_data() const
{
    BOBUIest::addColumn<float>("operandLeft");
    BOBUIest::addColumn<float>("operandRight");
    float zero(0.f), one(1.f);

    BOBUIest::newRow("should FAIL 1") << one << 3.f;
    BOBUIest::newRow("should PASS 1") << zero << zero;
    BOBUIest::newRow("should PASS: 0 == -0") << zero << negate(zero);
    BOBUIest::newRow("should FAIL 2") << 1.e-5f << 3.e-5f;

    // QCOMPARE() uses qFuzzyCompare(), which succeeds if the floats differ by
    // no more than 1e-5 times the smaller value; but QCOMPARE() also considers
    // values equal if qFuzzyIsNull is true for both, so all floats smaller than
    // 1e-5 are equal.  Thus QCOMPARE(1e5-2, 1e5-1) should fail, while
    // QCOMPARE(1e5+1, 1e5+2) should pass, as should QCOMPARE(1e-5-2e-10,
    // 1e-5-1e-10), despite the values differing by more than one part in 1e5.

    BOBUIest::newRow("should PASS 2") << 1e5f + one << 1e5f + 2.f;
    BOBUIest::newRow("should FAIL 3") << 1e5f - one << 1e5f - 2.f;
    BOBUIest::newRow("should PASS 3") << 1e-5f << -1e-5f;
    // ... but rounding makes that a bit unrelaible when scaled close to the bounds.
    BOBUIest::newRow("should FAIL 4") << 1e-5f + 1e-10f << 1e-5f - 1e-10f;
    BOBUIest::newRow("should PASS 4") << 1e38f + 1e33f << 1e38f + 2e33f;
    BOBUIest::newRow("should FAIL 5") << 1e38f - 1e33f << 1e38f - 3e33f;

    nonFinite_data(zero, one);
}

void tst_float::float16Comparisons() const
{
    QFETCH(qfloat16, operandLeft);
    QFETCH(qfloat16, operandRight);

    QCOMPARE(operandLeft, operandRight);
}

void tst_float::float16Comparisons_data() const
{
    BOBUIest::addColumn<qfloat16>("operandLeft");
    BOBUIest::addColumn<qfloat16>("operandRight");
    const qfloat16 zero(0), one(1);
    const qfloat16 tiny(9.756e-03f);

    BOBUIest::newRow("should FAIL 1") << one << qfloat16(3);
    BOBUIest::newRow("should PASS: 0 == -0") << zero << negate(zero);
    BOBUIest::newRow("should PASS 1") << zero << zero;

    // QCOMPARE for uses qFuzzyCompare(), which ignores differences of one part
    // in 102.5 and considers any two qFuzzyIsNull() values, i.e. values smaller
    // than 1/102.5, equal
    BOBUIest::newRow("should FAIL 2") << qfloat16(.01f) << qfloat16(.03f);
    BOBUIest::newRow("should PASS 2") << qfloat16(1001) << qfloat16(1002);
    BOBUIest::newRow("should FAIL 3") << qfloat16(98) << qfloat16(99);
    BOBUIest::newRow("should PASS 3") << tiny << -tiny;
    // ... which gets a bit unreliable near to the type's bounds
    BOBUIest::newRow("should FAIL 4") << qfloat16(10e-3f) << qfloat16(9.7e-3f);
    BOBUIest::newRow("should PASS 4") << qfloat16(6e4) + qfloat16(700) << qfloat16(6e4) + qfloat16(1200);
    BOBUIest::newRow("should FAIL 5") << qfloat16(6e4) - qfloat16(600) << qfloat16(6e4) - qfloat16(1200);

    nonFinite_data(zero, one);
}

void tst_float::compareFloatTests() const
{
    QFETCH(float, t1);

    // Create two more values
    // t2 differs from t1 by 1 ppm (part per million)
    // t3 differs from t1 by 200%
    // We should consider that t1 == t2 and t1 != t3 (provided at least one is > 1e-5)
    const float t2 = t1 + (t1 / 1e6);
    const float t3 = 3 * t1;

    QCOMPARE(t1, t2);

    /* Should FAIL. */
    QCOMPARE(t1, t3);
}

void tst_float::compareFloatTests_data() const
{
    BOBUIest::addColumn<float>("t1");
    BOBUIest::newRow("1e0") << 1e0f;
    BOBUIest::newRow("1e-5") << 1e-5f;
    BOBUIest::newRow("1e+7") << 1e+7f;
}

void tst_float::compareQListDouble() const
{
    QList<double> double1; double1 << 1.5 << 2 << 3;
    QList<double> double2; double2 << 1 << 2 << 4;
    QCOMPARE(double1, double2);
}

BOBUIEST_MAIN(tst_float)

#include "tst_float.moc"
