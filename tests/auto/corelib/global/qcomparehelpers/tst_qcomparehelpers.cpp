// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "tst_qcomparehelpers.h"
#include "wrappertypes.h"

#include <BobUICore/qscopeguard.h>

#if defined(__STDCPP_FLOAT16_T__) && __has_include(<stdfloat>)
#include <stdfloat>
#endif

/*
    NOTE: Do not add any other test cases to this cpp file!
    minGW already complains about a too large tst_qcomparehelpers.cpp.obj
    object file.

    Create a new cpp file and add new tests there.
*/

template<typename LeftType, typename RightType, typename OrderingType>
void tst_QCompareHelpers::compareImpl()
{
    QFETCH(LeftType, lhs);
    QFETCH(RightType, rhs);
    QFETCH(OrderingType, expectedOrdering);

    BOBUI_TEST_ALL_COMPARISON_OPS(lhs, rhs, expectedOrdering);
#ifdef __cpp_lib_three_way_comparison
    // Also check std types.
    BOBUI_TEST_ALL_COMPARISON_OPS(lhs, rhs, BobUIOrderingPrivate::to_std(expectedOrdering));
#endif // __cpp_lib_three_way_comparison
}

template<typename LeftType, typename RightType>
void tst_QCompareHelpers::compareIntData()
{
    BOBUIest::addColumn<LeftType>("lhs");
    BOBUIest::addColumn<RightType>("rhs");
    BOBUIest::addColumn<BobUI::strong_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, BobUI::strong_ordering ordering) {
        BOBUIest::addRow("%d vs %d", lhs, rhs) << LeftType(lhs) << RightType(rhs) << ordering;
    };

    createRow(0, 0, BobUI::strong_ordering::equivalent);
    createRow(-1, 0, BobUI::strong_ordering::less);
    createRow(1, 0, BobUI::strong_ordering::greater);
    constexpr int max = std::numeric_limits<int>::max();
    constexpr int min = std::numeric_limits<int>::min();
    createRow(max, max, BobUI::strong_ordering::equivalent);
    createRow(min, min, BobUI::strong_ordering::equivalent);
    createRow(max, min, BobUI::strong_ordering::greater);
    createRow(min, max, BobUI::strong_ordering::less);
}

template<typename LeftType, typename RightType>
void tst_QCompareHelpers::compareFloatData()
{
    BOBUIest::addColumn<LeftType>("lhs");
    BOBUIest::addColumn<RightType>("rhs");
    BOBUIest::addColumn<BobUI::partial_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, BobUI::partial_ordering ordering) {
        BOBUIest::addRow("%f vs %f", lhs, rhs) << LeftType(lhs) << RightType(rhs) << ordering;
    };

    createRow(0.0, 0.0, BobUI::partial_ordering::equivalent);
    createRow(-0.000001, 0.0, BobUI::partial_ordering::less);
    createRow(0.000001, 0.0, BobUI::partial_ordering::greater);

    const double nan = qQNaN();
    createRow(nan, 0.0, BobUI::partial_ordering::unordered);
    createRow(0.0, nan, BobUI::partial_ordering::unordered);
    createRow(nan, nan, BobUI::partial_ordering::unordered);

    const double inf = qInf();
    createRow(inf, 0.0, BobUI::partial_ordering::greater);
    createRow(0.0, inf, BobUI::partial_ordering::less);
    createRow(-inf, 0.0, BobUI::partial_ordering::less);
    createRow(0.0, -inf, BobUI::partial_ordering::greater);
    createRow(inf, inf, BobUI::partial_ordering::equivalent);
    createRow(-inf, -inf, BobUI::partial_ordering::equivalent);
    createRow(-inf, inf, BobUI::partial_ordering::less);
    createRow(inf, -inf, BobUI::partial_ordering::greater);

    createRow(nan, inf, BobUI::partial_ordering::unordered);
    createRow(inf, nan, BobUI::partial_ordering::unordered);
    createRow(nan, -inf, BobUI::partial_ordering::unordered);
    createRow(-inf, nan, BobUI::partial_ordering::unordered);
}

template<typename LeftType, typename RightType>
void tst_QCompareHelpers::compareStringData()
{
    BOBUIest::addColumn<LeftType>("lhs");
    BOBUIest::addColumn<RightType>("rhs");
    BOBUIest::addColumn<BobUI::weak_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, BobUI::weak_ordering ordering) {
        BOBUIest::addRow("'%s' vs '%s'", lhs, rhs) << LeftType(lhs) << RightType(rhs) << ordering;
    };

    createRow("", "", BobUI::weak_ordering::equivalent);
    createRow("Ab", "abc", BobUI::weak_ordering::less);
    createRow("aBc", "AB", BobUI::weak_ordering::greater);
    createRow("ab", "AB", BobUI::weak_ordering::equivalent);
    createRow("ABC", "abc", BobUI::weak_ordering::equivalent);
}

void tst_QCompareHelpers::comparisonCompiles()
{
    BOBUIestPrivate::testAllComparisonOperatorsCompile<IntWrapper>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<IntWrapper, int>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<DoubleWrapper>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<DoubleWrapper, double>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<DoubleWrapper, IntWrapper>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<StringWrapper<QString>>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<StringWrapper<QString>, QAnyStringView>();
}

void tst_QCompareHelpers::compare_IntWrapper_data()
{
    compareIntData<IntWrapper, IntWrapper>();
}

void tst_QCompareHelpers::compare_IntWrapper()
{
    compareImpl<IntWrapper, IntWrapper, BobUI::strong_ordering>();
}

void tst_QCompareHelpers::compare_IntWrapper_int_data()
{
    compareIntData<IntWrapper, int>();
}

void tst_QCompareHelpers::compare_IntWrapper_int()
{
    compareImpl<IntWrapper, int, BobUI::strong_ordering>();
}

void tst_QCompareHelpers::compare_DoubleWrapper_data()
{
    compareFloatData<DoubleWrapper, DoubleWrapper>();
}

void tst_QCompareHelpers::compare_DoubleWrapper()
{
    compareImpl<DoubleWrapper, DoubleWrapper, BobUI::partial_ordering>();
}

void tst_QCompareHelpers::compare_DoubleWrapper_double_data()
{
    compareFloatData<DoubleWrapper, double>();
}

void tst_QCompareHelpers::compare_DoubleWrapper_double()
{
    compareImpl<DoubleWrapper, double, BobUI::partial_ordering>();
}

void tst_QCompareHelpers::compare_IntWrapper_DoubleWrapper_data()
{
    BOBUIest::addColumn<IntWrapper>("lhs");
    BOBUIest::addColumn<DoubleWrapper>("rhs");
    BOBUIest::addColumn<BobUI::partial_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, BobUI::partial_ordering ordering) {
        BOBUIest::addRow("%d vs %f", lhs, rhs) << IntWrapper(lhs) << DoubleWrapper(rhs) << ordering;
    };

    createRow(0, 0.0, BobUI::partial_ordering::equivalent);
    createRow(-1, 0.0, BobUI::partial_ordering::less);
    createRow(1, 0.0, BobUI::partial_ordering::greater);
    createRow(0, -0.000001, BobUI::partial_ordering::greater);
    createRow(0, 0.000001, BobUI::partial_ordering::less);

    constexpr int max = std::numeric_limits<int>::max();
    constexpr int min = std::numeric_limits<int>::min();
    const double nan = qQNaN();
    createRow(0, nan, BobUI::partial_ordering::unordered);
    createRow(max, nan, BobUI::partial_ordering::unordered);
    createRow(min, nan, BobUI::partial_ordering::unordered);

    const double inf = qInf();
    createRow(0, inf, BobUI::partial_ordering::less);
    createRow(0, -inf, BobUI::partial_ordering::greater);
    createRow(max, inf, BobUI::partial_ordering::less);
    createRow(max, -inf, BobUI::partial_ordering::greater);
    createRow(min, inf, BobUI::partial_ordering::less);
    createRow(min, -inf, BobUI::partial_ordering::greater);
}

void tst_QCompareHelpers::compare_IntWrapper_DoubleWrapper()
{
    compareImpl<IntWrapper, DoubleWrapper, BobUI::partial_ordering>();
}

void tst_QCompareHelpers::compare_StringWrapper_data()
{
    compareStringData<StringWrapper<QString>, StringWrapper<QString>>();
}

void tst_QCompareHelpers::compare_StringWrapper()
{
    compareImpl<StringWrapper<QString>, StringWrapper<QString>, BobUI::weak_ordering>();
}

void tst_QCompareHelpers::compare_StringWrapper_AnyStringView_data()
{
    compareStringData<StringWrapper<QString>, QAnyStringView>();
}

void tst_QCompareHelpers::compare_StringWrapper_AnyStringView()
{
    compareImpl<StringWrapper<QString>, QAnyStringView, BobUI::weak_ordering>();
}

#define DECLARE_TYPE(Name, Type, Attrs, RetType, Constexpr, Suffix) \
class Dummy ## Name \
{ \
public: \
    Constexpr Dummy ## Name () {} \
\
private: \
    friend Attrs Constexpr bool \
    comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept; \
    friend Attrs Constexpr RetType \
    compareThreeWay(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept; \
    friend Attrs Constexpr bool \
    comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept; \
    friend Attrs Constexpr RetType \
    compareThreeWay(const Dummy ## Name &lhs, int rhs) noexcept; \
    Q_DECLARE_ ## Type ##_ORDERED ## Suffix (Dummy ## Name) \
    Q_DECLARE_ ## Type ##_ORDERED ## Suffix (Dummy ## Name, int) \
}; \
\
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \
Attrs Constexpr RetType \
compareThreeWay(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return RetType::equivalent; } \
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \
Attrs Constexpr RetType compareThreeWay(const Dummy ## Name &lhs, int rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return RetType::equivalent; }

DECLARE_TYPE(PartialConstAttr, PARTIALLY, Q_DECL_PURE_FUNCTION, BobUI::partial_ordering, constexpr,
             _LITERAL_TYPE)
DECLARE_TYPE(PartialConst, PARTIALLY, /* no attrs */, BobUI::partial_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(PartialAttr, PARTIALLY, Q_DECL_CONST_FUNCTION, BobUI::partial_ordering, , )
DECLARE_TYPE(Partial, PARTIALLY, /* no attrs */, BobUI::partial_ordering, , )

DECLARE_TYPE(WeakConstAttr, WEAKLY, Q_DECL_PURE_FUNCTION, BobUI::weak_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(WeakConst, WEAKLY, /* no attrs */, BobUI::weak_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(WeakAttr, WEAKLY, Q_DECL_CONST_FUNCTION, BobUI::weak_ordering, , )
DECLARE_TYPE(Weak, WEAKLY, /* no attrs */, BobUI::weak_ordering, , )

DECLARE_TYPE(StrongConstAttr, STRONGLY, Q_DECL_PURE_FUNCTION, BobUI::strong_ordering, constexpr,
             _LITERAL_TYPE)
DECLARE_TYPE(StrongConst, STRONGLY, /* no attrs */, BobUI::strong_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(StrongAttr, STRONGLY, Q_DECL_CONST_FUNCTION, BobUI::strong_ordering, , )
DECLARE_TYPE(Strong, STRONGLY, /* no attrs */, BobUI::strong_ordering, , )

#define DECLARE_EQUALITY_COMPARABLE(Name, Attrs, Constexpr, Suffix) \
class Dummy ## Name \
{ \
public: \
    Constexpr Dummy ## Name (int) {} \
\
private: \
    friend Attrs Constexpr bool \
    comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept; \
    friend Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept; \
    Q_DECLARE_EQUALITY_COMPARABLE ## Suffix (Dummy ## Name) \
    Q_DECLARE_EQUALITY_COMPARABLE ## Suffix (Dummy ## Name, int) \
}; \
\
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \

DECLARE_EQUALITY_COMPARABLE(ConstAttr, Q_DECL_PURE_FUNCTION, constexpr, _LITERAL_TYPE)
DECLARE_EQUALITY_COMPARABLE(Const, /* no attrs */, constexpr, _LITERAL_TYPE)
DECLARE_EQUALITY_COMPARABLE(Attr, Q_DECL_CONST_FUNCTION, , )
DECLARE_EQUALITY_COMPARABLE(None, /* no attrs */, , )

void tst_QCompareHelpers::generatedClasses()
{
#define COMPARE(ClassName) \
    do { \
        BOBUIestPrivate::testAllComparisonOperatorsCompile<ClassName>(); \
        BOBUIestPrivate::testAllComparisonOperatorsCompile<ClassName, int>(); \
    } while (0)

    COMPARE(DummyPartialConstAttr);
    COMPARE(DummyPartialConst);
    COMPARE(DummyPartialAttr);
    COMPARE(DummyPartial);

    COMPARE(DummyWeakConstAttr);
    COMPARE(DummyWeakConst);
    COMPARE(DummyWeakAttr);
    COMPARE(DummyWeak);

    COMPARE(DummyStrongConstAttr);
    COMPARE(DummyStrongConst);
    COMPARE(DummyStrongAttr);
    COMPARE(DummyStrong);
#undef COMPARE

    BOBUIestPrivate::testEqualityOperatorsCompile<DummyConstAttr>();
    BOBUIestPrivate::testEqualityOperatorsCompile<DummyConstAttr, int>();

    BOBUIestPrivate::testEqualityOperatorsCompile<DummyConst>();
    BOBUIestPrivate::testEqualityOperatorsCompile<DummyConst, int>();

    BOBUIestPrivate::testEqualityOperatorsCompile<DummyAttr>();
    BOBUIestPrivate::testEqualityOperatorsCompile<DummyAttr, int>();

    BOBUIestPrivate::testEqualityOperatorsCompile<DummyNone>();
    BOBUIestPrivate::testEqualityOperatorsCompile<DummyNone, int>();
}

template <typename LeftType, typename RightType,
          BobUI::if_integral<LeftType> = true,
          BobUI::if_integral<RightType> = true>
void testOrderForTypes()
{
    LeftType l0{0};
    LeftType l1{1};
    RightType r0{0};
    RightType r1{1};
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, r1), BobUI::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(l1, r0), BobUI::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(l1, r1), BobUI::strong_ordering::equivalent);
    // also swap types
    QCOMPARE_EQ(BobUI::compareThreeWay(r1, l0), BobUI::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(r0, l1), BobUI::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(r1, l1), BobUI::strong_ordering::equivalent);

#ifdef __cpp_lib_three_way_comparison
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, r1), std::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(l1, r0), std::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(l1, r1), std::strong_ordering::equivalent);

    QCOMPARE_EQ(BobUI::compareThreeWay(r1, l0), std::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(r0, l1), std::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(r1, l1), std::strong_ordering::equivalent);
#endif // __cpp_lib_three_way_comparison

    if constexpr (std::is_signed_v<LeftType>) {
        LeftType lm1{-1};
        QCOMPARE_EQ(BobUI::compareThreeWay(lm1, r1), BobUI::strong_ordering::less);
        QCOMPARE_EQ(BobUI::compareThreeWay(r1, lm1), BobUI::strong_ordering::greater);
#ifdef __cpp_lib_three_way_comparison
        QCOMPARE_EQ(BobUI::compareThreeWay(lm1, r1), std::strong_ordering::less);
        QCOMPARE_EQ(BobUI::compareThreeWay(r1, lm1), std::strong_ordering::greater);
#endif // __cpp_lib_three_way_comparison
    }
    if constexpr (std::is_signed_v<RightType>) {
        RightType rm1{-1};
        QCOMPARE_EQ(BobUI::compareThreeWay(rm1, l1), BobUI::strong_ordering::less);
        QCOMPARE_EQ(BobUI::compareThreeWay(l1, rm1), BobUI::strong_ordering::greater);
#ifdef __cpp_lib_three_way_comparison
        QCOMPARE_EQ(BobUI::compareThreeWay(rm1, l1), std::strong_ordering::less);
        QCOMPARE_EQ(BobUI::compareThreeWay(l1, rm1), std::strong_ordering::greater);
#endif // __cpp_lib_three_way_comparison
    }
}

template <typename LeftType, typename RightType,
          BobUI::if_floating_point<LeftType> = true,
          BobUI::if_floating_point<RightType> = true>
void testOrderForTypes()
{
    constexpr auto lNeg = LeftType(-1);
    constexpr auto lPos = LeftType( 1);

    constexpr auto rNeg = RightType(-1);
    constexpr auto rPos = RightType( 1);

    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rPos), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(lPos, rNeg), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNeg, lPos), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(rPos, lNeg), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rNeg), BobUI::partial_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNeg, lNeg), BobUI::partial_ordering::equivalent);

    LeftType lNaN{std::numeric_limits<LeftType>::quiet_NaN()};
    LeftType lInf{std::numeric_limits<LeftType>::infinity()};

    RightType rNaN{std::numeric_limits<RightType>::quiet_NaN()};
    RightType rInf{std::numeric_limits<RightType>::infinity()};

    QCOMPARE_EQ(BobUI::compareThreeWay(lNaN, rPos), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNeg, lNaN), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rNaN), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, lPos), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, lNaN), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNaN, rNaN), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNaN, rInf), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, -lInf), BobUI::partial_ordering::unordered);

    QCOMPARE_EQ(BobUI::compareThreeWay(lInf, rPos), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(rPos, lInf), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(rInf, lNeg), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rInf), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(lInf, -rInf), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(-lInf, rInf), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(-rInf, lInf), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(rInf, -lInf), BobUI::partial_ordering::greater);

#ifdef __cpp_lib_three_way_comparison
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rPos), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(lPos, rNeg), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNeg, lPos), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(rPos, lNeg), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rNeg), std::partial_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNeg, lNeg), std::partial_ordering::equivalent);

    QCOMPARE_EQ(BobUI::compareThreeWay(lNaN, rPos), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNeg, lNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, lPos), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, lNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNaN, rNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNaN, rInf), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, -lInf), std::partial_ordering::unordered);

    QCOMPARE_EQ(BobUI::compareThreeWay(lInf, rPos), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(rPos, lInf), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(rInf, lNeg), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(lNeg, rInf), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(lInf, -rInf), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(-lInf, rInf), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(-rInf, lInf), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(rInf, -lInf), std::partial_ordering::greater);
#endif // __cpp_lib_three_way_comparison
}

template <typename IntType, typename FloatType,
          BobUI::if_integral<IntType> = true,
          BobUI::if_floating_point<FloatType> = true>
void testOrderForTypes()
{
    IntType l0{0};
    IntType l1{1};

    constexpr FloatType r0{0};
    constexpr FloatType r1{1};
    FloatType rNaN{std::numeric_limits<FloatType>::quiet_NaN()};

    QCOMPARE_EQ(BobUI::compareThreeWay(l0, r1), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(l1, r0), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(r1, l0), BobUI::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(r0, l1), BobUI::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, r0), BobUI::partial_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(r0, l0), BobUI::partial_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, rNaN), BobUI::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, l1), BobUI::partial_ordering::unordered);
#ifdef __cpp_lib_three_way_comparison
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, r1), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(l1, r0), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(r1, l0), std::partial_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(r0, l1), std::partial_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, r0), std::partial_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(r0, l0), std::partial_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(l0, rNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(BobUI::compareThreeWay(rNaN, l1), std::partial_ordering::unordered);
#endif // __cpp_lib_three_way_comparison
}

enum class TestEnum : quint8 {
    Smaller,
    Bigger
};

void tst_QCompareHelpers::builtinOrder()
{
#define TEST_BUILTIN(Left, Right) \
    do { \
        auto printOnFailure = qScopeGuard([] { \
                qDebug("Failed BobUI::compareThreeWay() test for builtin types %s and %s", \
                       #Left, #Right); \
            }); \
        testOrderForTypes<Left, Right>(); \
        printOnFailure.dismiss(); \
    } while (false);

    // some combinations
    TEST_BUILTIN(char, char)
#if CHAR_MIN < 0
    TEST_BUILTIN(char, short)
    TEST_BUILTIN(qint8, char)
#else
    TEST_BUILTIN(char, ushort)
    TEST_BUILTIN(quint8, char)
#endif
    TEST_BUILTIN(qint8, qint8)
    TEST_BUILTIN(qint8, int)
    TEST_BUILTIN(ulong, quint8)
    TEST_BUILTIN(ushort, uchar)
    TEST_BUILTIN(int, int)
    TEST_BUILTIN(uint, ulong)
    TEST_BUILTIN(long, int)
    TEST_BUILTIN(uint, quint64)
    TEST_BUILTIN(qint64, short)
    TEST_BUILTIN(wchar_t, wchar_t)
    TEST_BUILTIN(uint, char16_t)
    TEST_BUILTIN(char32_t, char32_t)
    TEST_BUILTIN(char32_t, ushort)
#ifdef __cpp_char8_t
    TEST_BUILTIN(char8_t, char8_t)
    TEST_BUILTIN(char8_t, ushort)
    TEST_BUILTIN(char8_t, uint)
    TEST_BUILTIN(char8_t, quint64)
#endif // __cpp_char8_t
#ifdef BOBUI_SUPPORTS_INT128
    TEST_BUILTIN(qint128, qint128)
    TEST_BUILTIN(quint128, quint128)
    TEST_BUILTIN(qint128, int)
    TEST_BUILTIN(ushort, quint128)
#endif
    TEST_BUILTIN(float, double)
    TEST_BUILTIN(double, float)
    TEST_BUILTIN(quint64, float)
    TEST_BUILTIN(qint64, double)
#ifdef __STDCPP_FLOAT16_T__
    TEST_BUILTIN(std::float16_t, std::float16_t)
    TEST_BUILTIN(std::float16_t, double)
    TEST_BUILTIN(qint64, std::float16_t)
    TEST_BUILTIN(uint, std::float16_t)
#endif
    TEST_BUILTIN(long double, long double)
    TEST_BUILTIN(float, long double)
    TEST_BUILTIN(double, long double)
    TEST_BUILTIN(quint64, long double)
    TEST_BUILTIN(ushort, long double)

#if QFLOAT16_IS_NATIVE
    {
        // Cannot use TEST_BUILTIN here, because std::numeric_limits are not defined
        // for BobUIPrivate::NativeFloat16Type.
        constexpr auto smaller = BobUIPrivate::NativeFloat16Type(1);
        constexpr auto bigger  = BobUIPrivate::NativeFloat16Type(2);
        // native vs native
        QCOMPARE_EQ(BobUI::compareThreeWay(smaller, smaller), BobUI::partial_ordering::equivalent);
        QCOMPARE_EQ(BobUI::compareThreeWay(smaller, bigger), BobUI::partial_ordering::less);
        QCOMPARE_EQ(BobUI::compareThreeWay(bigger, smaller), BobUI::partial_ordering::greater);
        // native vs float
        QCOMPARE_EQ(BobUI::compareThreeWay(smaller, 1.0f), BobUI::partial_ordering::equivalent);
        QCOMPARE_EQ(BobUI::compareThreeWay(1.0f, bigger), BobUI::partial_ordering::less);
        QCOMPARE_EQ(BobUI::compareThreeWay(bigger, 1.0f), BobUI::partial_ordering::greater);
        const auto floatNaN = std::numeric_limits<float>::quiet_NaN();
        QCOMPARE_EQ(BobUI::compareThreeWay(bigger, floatNaN), BobUI::partial_ordering::unordered);
    }
#endif

    QCOMPARE_EQ(BobUI::compareThreeWay(TestEnum::Smaller, TestEnum::Bigger),
                BobUI::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(TestEnum::Bigger, TestEnum::Smaller),
                BobUI::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(TestEnum::Smaller, TestEnum::Smaller),
                BobUI::strong_ordering::equivalent);

    std::array<int, 2> arr{1, 0};
#if BOBUI_DEPRECATED_SINCE(6, 8)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
    QCOMPARE_EQ(BobUI::compareThreeWay(&arr[0], &arr[1]), BobUI::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(arr.data(), &arr[0]), BobUI::strong_ordering::equivalent);
BOBUI_WARNING_POP
#endif // BOBUI_DEPRECATED_SINCE(6, 8)

    class Base {};
    class Derived : public Base {};

    auto b = std::make_unique<Base>();
    auto d = std::make_unique<Derived>();
#if BOBUI_DEPRECATED_SINCE(6, 8)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
    QCOMPARE_NE(BobUI::compareThreeWay(b.get(), d.get()), BobUI::strong_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(b.get(), nullptr), BobUI::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(nullptr, d.get()), BobUI::strong_ordering::less);
BOBUI_WARNING_POP
#endif // BOBUI_DEPRECATED_SINCE(6, 8)

    // Check BobUI::totally_ordered_wrapper
    auto a0 = BobUI::totally_ordered_wrapper(&arr[0]);
    auto a1 = BobUI::totally_ordered_wrapper(&arr[1]);
    QCOMPARE_EQ(BobUI::compareThreeWay(a0, a1), BobUI::strong_ordering::less);
    QCOMPARE_EQ(BobUI::compareThreeWay(arr.data(), a0), BobUI::strong_ordering::equivalent);

    auto bWrapper = BobUI::totally_ordered_wrapper(b.get());
    auto dWrapper = BobUI::totally_ordered_wrapper(d.get());
    QCOMPARE_NE(BobUI::compareThreeWay(bWrapper, dWrapper), BobUI::strong_ordering::equivalent);
    QCOMPARE_NE(BobUI::compareThreeWay(bWrapper, d.get()), BobUI::strong_ordering::equivalent);
    QCOMPARE_NE(BobUI::compareThreeWay(b.get(), dWrapper), BobUI::strong_ordering::equivalent);
    QCOMPARE_EQ(BobUI::compareThreeWay(bWrapper, nullptr), BobUI::strong_ordering::greater);
    QCOMPARE_EQ(BobUI::compareThreeWay(nullptr, dWrapper), BobUI::strong_ordering::less);
    dWrapper.reset(nullptr);
    QCOMPARE_EQ(BobUI::compareThreeWay(nullptr, dWrapper), BobUI::strong_ordering::equivalent);

#undef TEST_BUILTIN
}

BOBUIEST_MAIN(tst_QCompareHelpers)
#include "moc_tst_qcomparehelpers.cpp"
