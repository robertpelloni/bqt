// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOMPARISONTESTHELPER_P_H
#define QCOMPARISONTESTHELPER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/q20type_traits.h>
#include <BobUICore/qxptype_traits.h>
#include <BobUITest/bobuiest.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIestPrivate {

#ifdef __cpp_lib_three_way_comparison
template <typename LT, typename RT>
using HasThreeWayComparisonOp = decltype(std::declval<LT>() <=> std::declval<RT>());

template <typename LT, typename RT>
constexpr bool implementsThreeWayComparisonOp_v = qxp::is_detected_v<HasThreeWayComparisonOp,
                                                                     LT, RT>;
#endif

Q_TESTLIB_EXPORT QByteArray formatTypeWithCRefImpl(QMetaType type, bool isConst,
                                                   bool isRef, bool isRvalueRef);

template <typename T>
QByteArray formatTypeWithCRef()
{
    return formatTypeWithCRefImpl(QMetaType::fromType<q20::remove_cvref_t<T>>(),
                                  std::is_const_v<std::remove_reference_t<T>>,
                                  std::is_reference_v<T>,
                                  std::is_rvalue_reference_v<T>);
}

#define FOR_EACH_CREF(Func, Left, Right, Op, Result) \
    Func(Left &, Right &, Op, Result) \
    Func(Left &, Right const &, Op, Result) \
    Func(Left &, Right &&, Op, Result) \
    Func(Left &, Right const &&, Op, Result) \
    Func(Left const &, Right &, Op, Result) \
    Func(Left const &, Right const &, Op, Result) \
    Func(Left const &, Right &&, Op, Result) \
    Func(Left const &, Right const &&, Op, Result) \
    Func(Left &&, Right &, Op, Result) \
    Func(Left &&, Right const &, Op, Result) \
    Func(Left &&, Right &&, Op, Result) \
    Func(Left &&, Right const &&, Op, Result) \
    Func(Left const &&, Right &, Op, Result) \
    Func(Left const &&, Right const &, Op, Result) \
    Func(Left const &&, Right &&, Op, Result) \
    Func(Left const &&, Right const &&, Op, Result) \
    /* END */

#define CHECK_SINGLE_OPERATOR(Left, Right, Op, Result) \
    do { \
        static_assert(std::is_convertible_v<decltype( \
                        std::declval<Left>() Op std::declval<Right>()), Result>); \
        if constexpr (!std::is_same_v<Left, Right>) { \
            static_assert(std::is_convertible_v<decltype( \
                            std::declval<Right>() Op std::declval<Left>()), Result>); \
        } \
    } while (false); \
    /* END */

/*!
    \internal

    This function checks that the types \c LeftType and \c RightType properly
    define {in}equality operators (== and !=). The checks are performed for
    all combinations of cvref-qualified lvalues and rvalues.
*/
template <typename LeftType, typename RightType = LeftType>
void testEqualityOperatorsCompile()
{
    FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, ==, bool)
    FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, !=, bool)
}

/*!
    \internal

    This function checks that the types \c LeftType and \c RightType properly
    define all comparison operators (==, !=, <, >, <=, >=). The checks are
    performed for all combinations of cvref-qualified lvalues and rvalues.

    If compiled in C++20 mode, also checks \c {operator<=>()} if that is
    implemented.
*/
template <typename LeftType, typename RightType = LeftType>
void testAllComparisonOperatorsCompile()
{
    testEqualityOperatorsCompile<LeftType, RightType>();
    if (BOBUIest::currentTestFailed())
        return;
    FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, >, bool)
    FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, <, bool)
    FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, >=, bool)
    FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, <=, bool)
#ifdef __cpp_lib_three_way_comparison
    if constexpr (implementsThreeWayComparisonOp_v<LeftType, RightType>) {
        FOR_EACH_CREF(CHECK_SINGLE_OPERATOR, LeftType, RightType, <=>, std::partial_ordering)
    }
#endif
}

#undef CHECK_SINGLE_OPERATOR
#undef FOR_EACH_CREF

#define CHECK_RUNTIME_CREF(Func, Left, Right, Op, Expected) \
    do { \
        Func(Left, Right, Op, Expected); \
        Func(std::as_const(Left), Right, Op, Expected); \
        Func(Left, std::as_const(Right), Op, Expected); \
        Func(std::as_const(Left), std::as_const(Right), Op, Expected); \
    } while (false) \
    /* END */

#define CHECK_RUNTIME_LR(Left, Right, Op, Expected) \
    do { \
        QCOMPARE_EQ(Left Op Right, Expected); \
        QCOMPARE_EQ(std::move(Left) Op Right, Expected); \
        QCOMPARE_EQ(Left Op std::move(Right), Expected); \
        QCOMPARE_EQ(std::move(Left) Op std::move(Right), Expected); \
    } while (false) \
    /* END */

#ifdef __cpp_lib_three_way_comparison

// Hide the macro under an ifdef, because it otherwise triggers a warning
// in Clang C++17 build.
#define CHECK_RUNTIME_3WAY(Left, Right, Op, Expected) \
    do { \
        QCOMPARE_EQ((Left <=> Right) Op 0, Expected); \
        QCOMPARE_EQ((std::move(Left) <=> Right) Op 0, Expected); \
        QCOMPARE_EQ((Left <=> std::move(Right)) Op 0, Expected); \
        QCOMPARE_EQ((std::move(Left) <=> std::move(Right)) Op 0, Expected); \
    } while (false) \
    /* END */

#endif // __cpp_lib_three_way_comparison

/*!
    \internal
    Basic testing of equality operators.

    The helper function tests {in}equality operators (== and !=) for the \a lhs
    operand of type \c {LeftType} and the \a rhs operand of type \c {RightType},
    plus the reverse order of the operands.

    The \a expectedEqual parameter is an expected result for \c {operator==()}.

    \note Any test calling this method will need to check the test state after
    doing so, if there is any later code in the test.

    \code
    BOBUIime early(12, 34, 56, 00);
    BOBUIime later(12, 34, 56, 01);
    BOBUIestPrivate::testEqualityOperators(early, later, false);
    if (BOBUIest:currentTestFailed())
        return;
    \endcode
*/
template <typename LeftType, typename RightType>
void testEqualityOperators(LeftType lhs, RightType rhs, bool expectedEqual,
                           const char *lhsExpr, const char *rhsExpr, const char *expected,
                           const char *file, int line)
{
    if (expectedEqual) {
        if (!BOBUIest::qCompareOp<BOBUIest::ComparisonOperation::Equal>(lhs, rhs, lhsExpr, rhsExpr, file, line))
            BOBUIEST_FAIL_ACTION;
    } else {
        if (!BOBUIest::qCompareOp<BOBUIest::ComparisonOperation::NotEqual>(lhs, rhs, lhsExpr, rhsExpr, file, line))
            BOBUIEST_FAIL_ACTION;
    }

    auto report = qScopeGuard([=] {
        qDebug("testEqualityOperators(%s,%s,%s) failed in %s on line %d", lhsExpr, rhsExpr,
               expected, file, line);
    });
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, ==, expectedEqual);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, !=, !expectedEqual);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, ==, expectedEqual);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, !=, !expectedEqual);
    report.dismiss();
}

/*!
    \internal
    Basic testing of equality and relation operators.

    The helper function tests all six relation and equality operators
    (==, !=, <, >, <=, >=) for the \a lhs operand of type \c {LeftType} and
    the \a rhs operand of type \c {RightType} and all six for the reverse
    order of the operands.

    If compiled in C++20 mode, also checks \c {operator<=>()} if that is
    implemented.

    When compiled in C++17 mode, the \c OrderingType must be one of
    BobUI::partial_ordering, BobUI::strong_ordering, or BobUI::weak_ordering.
    In C++20 mode, also the \c {std::*_ordering} types can be used.

    The \a expectedOrdering parameter provides the expected
    relation between \a lhs and \a rhs.

    \note Any test calling this method will need to check the test state after
    doing so, if there is any later code in the test.

    \code
    QDateTime now = QDateTime::currentDateTime();
    QDateTime later = now.addMSec(1);
    BOBUIestPrivate::testComparisonOperators(now, later, BobUI::weak_ordering::less);
    if (BOBUIest:currentTestFailed())
        return;
    \endcode
*/
template <typename LeftType, typename RightType, typename OrderingType>
void testAllComparisonOperators(LeftType lhs, RightType rhs, OrderingType expectedOrdering)
{
    constexpr bool isQOrderingType = std::is_same_v<OrderingType, BobUI::partial_ordering>
                                        || std::is_same_v<OrderingType, BobUI::weak_ordering>
                                        || std::is_same_v<OrderingType, BobUI::strong_ordering>;
#ifdef __cpp_lib_three_way_comparison
    constexpr bool isStdOrderingType = std::is_same_v<OrderingType, std::partial_ordering>
                                        || std::is_same_v<OrderingType, std::weak_ordering>
                                        || std::is_same_v<OrderingType, std::strong_ordering>;
#else
    constexpr bool isStdOrderingType = false;
#endif

    static_assert(isQOrderingType || isStdOrderingType,
                  "Please provide, as the expectedOrdering parameter, a value "
                  "of one of the BobUI::{partial,weak,strong}_ordering or "
                  "std::{partial,weak,strong}_ordering types.");

    // We have all sorts of operator==() between Q*Ordering and std::*_ordering
    // types, so we can just compare to BobUI::partial_ordering.
    const bool expectedEqual = expectedOrdering == BobUI::partial_ordering::equivalent;
    const bool expectedLess = expectedOrdering == BobUI::partial_ordering::less;
    const bool expectedUnordered = expectedOrdering == BobUI::partial_ordering::unordered;

    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, ==,
                       !expectedUnordered && expectedEqual);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, !=,
                       expectedUnordered || !expectedEqual);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, <,
                       !expectedUnordered && expectedLess);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, >,
                       !expectedUnordered && !expectedLess && !expectedEqual);
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, <=,
                       !expectedUnordered && (expectedEqual || expectedLess));
    CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, lhs, rhs, >=,
                       !expectedUnordered && !expectedLess);
#ifdef __cpp_lib_three_way_comparison
    if constexpr (implementsThreeWayComparisonOp_v<LeftType, RightType>) {
        if constexpr (std::is_convertible_v<OrderingType, std::strong_ordering>)
            static_assert(std::is_same_v<decltype(lhs <=> rhs), std::strong_ordering>);
        else if constexpr (std::is_convertible_v<OrderingType, std::weak_ordering>)
            static_assert(std::is_same_v<decltype(lhs <=> rhs), std::weak_ordering>);
        else
            static_assert(std::is_same_v<decltype(lhs <=> rhs), std::partial_ordering>);

        CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, lhs, rhs, ==,
                           !expectedUnordered && expectedEqual);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, lhs, rhs, !=,
                           expectedUnordered || !expectedEqual);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, lhs, rhs, <,
                           !expectedUnordered && expectedLess);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, lhs, rhs, >,
                           !expectedUnordered && !expectedLess && !expectedEqual);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, lhs, rhs, <=,
                           !expectedUnordered && (expectedEqual || expectedLess));
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, lhs, rhs, >=,
                           !expectedUnordered && !expectedLess);
    }
#endif

        CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, ==,
                           !expectedUnordered && expectedEqual);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, !=,
                           expectedUnordered || !expectedEqual);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, <,
                           !expectedUnordered && !expectedLess && !expectedEqual);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, >,
                           !expectedUnordered && expectedLess);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, <=,
                           !expectedUnordered && !expectedLess);
        CHECK_RUNTIME_CREF(CHECK_RUNTIME_LR, rhs, lhs, >=,
                           !expectedUnordered && (expectedEqual || expectedLess));
#ifdef __cpp_lib_three_way_comparison
        if constexpr (implementsThreeWayComparisonOp_v<LeftType, RightType>) {
            if constexpr (std::is_convertible_v<OrderingType, std::strong_ordering>)
                static_assert(std::is_same_v<decltype(rhs <=> lhs), std::strong_ordering>);
            else if constexpr (std::is_convertible_v<OrderingType, std::weak_ordering>)
                static_assert(std::is_same_v<decltype(rhs <=> lhs), std::weak_ordering>);
            else
                static_assert(std::is_same_v<decltype(rhs <=> lhs), std::partial_ordering>);

            CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, rhs, lhs, ==,
                               !expectedUnordered && expectedEqual);
            CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, rhs, lhs, !=,
                               expectedUnordered || !expectedEqual);
            CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, rhs, lhs, <,
                               !expectedUnordered && !expectedLess && !expectedEqual);
            CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, rhs, lhs, >,
                               !expectedUnordered && expectedLess);
            CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, rhs, lhs, <=,
                               !expectedUnordered && !expectedLess);
            CHECK_RUNTIME_CREF(CHECK_RUNTIME_3WAY, rhs, lhs, >=,
                               !expectedUnordered && (expectedEqual || expectedLess));
        }
#endif
}

#ifdef __cpp_lib_three_way_comparison
#undef CHECK_RUNTIME_3WAY
#endif
#undef CHECK_RUNTIME_LR
#undef CHECK_RUNTIME_CREF

} // namespace BOBUIestPrivate

/*!
    \internal

    A helper macro that calls BOBUIestPrivate::testEqualityOperators(), checks the
    test's state after the function is executed, and generates a meaningful
    debug message with the original file and line numbers if the test has
    failed.
*/
#define BOBUI_TEST_EQUALITY_OPS(Left, Right, Expected) \
    do { \
        BOBUIestPrivate::testEqualityOperators(Left, Right, Expected, #Left, #Right, #Expected, \
                                            __FILE__, __LINE__); \
        if (BOBUIest::currentTestFailed()) \
            return; \
    } while (false)

/*!
    \internal

    A helper macro that calls BOBUIestPrivate::testAllComparisonOperators(), checks
    the test's state after the function is executed, and generates a meaningful
    debug message with the original file and line numbers if the test has
    failed.
*/
#define BOBUI_TEST_ALL_COMPARISON_OPS(Left, Right, Expected) \
    do { \
        auto report = qScopeGuard([] { \
            qDebug("testAllComparisonOperators(" #Left ", " #Right ", " #Expected ") " \
                   "failed in " __FILE__ " on line %d", __LINE__); \
        }); \
        BOBUIestPrivate::testAllComparisonOperators(Left, Right, Expected); \
        if (BOBUIest::currentTestFailed()) \
            return; \
        report.dismiss(); \
    } while (false)

BOBUI_END_NAMESPACE

#endif // QCOMPARISONTESTHELPER_P_H
