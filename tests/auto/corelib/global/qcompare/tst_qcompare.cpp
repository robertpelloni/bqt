// Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/BobUICompare>
#include <BobUITest/BOBUIest>

#ifdef __cpp_lib_three_way_comparison
#include <compare>
#endif // __cpp_lib_three_way_comparison

class tst_QCompare: public QObject
{
    Q_OBJECT

    enum TestEnum : int {
        Smaller,
        Bigger
    };

#ifdef __cpp_lib_three_way_comparison
    template<typename LeftType, typename RightType, typename OrderingType>
    void compare3WayHelper(LeftType lhs, RightType rhs, OrderingType order);
#endif

private slots:
    void legacyPartialOrdering();
    void legacyConversions();
    void stdBobUIBinaryCompatibility();
    void partialOrdering();
    void weakOrdering();
    void strongOrdering();
    void threeWayCompareWithLiteralZero();
    void conversions();
    void is_eq_overloads();
    void compareThreeWay();
    void unorderedNeqLiteralZero();
    void compare3WayMacro();
};

void tst_QCompare::legacyPartialOrdering()
{
    static_assert(QPartialOrdering::Unordered ==  QPartialOrdering::unordered);
    static_assert(QPartialOrdering::Less ==       QPartialOrdering::less);
    static_assert(QPartialOrdering::Equivalent == QPartialOrdering::equivalent);
    static_assert(QPartialOrdering::Greater ==    QPartialOrdering::greater);

    static_assert(QPartialOrdering::Unordered == QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Unordered != QPartialOrdering::Less);
    static_assert(QPartialOrdering::Unordered != QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Unordered != QPartialOrdering::Greater);

    static_assert(QPartialOrdering::Less != QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Less == QPartialOrdering::Less);
    static_assert(QPartialOrdering::Less != QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Less != QPartialOrdering::Greater);

    static_assert(QPartialOrdering::Equivalent != QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Equivalent != QPartialOrdering::Less);
    static_assert(QPartialOrdering::Equivalent == QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Equivalent != QPartialOrdering::Greater);

    static_assert(QPartialOrdering::Greater != QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Greater != QPartialOrdering::Less);
    static_assert(QPartialOrdering::Greater != QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Greater == QPartialOrdering::Greater);

    static_assert(!is_eq  (QPartialOrdering::Unordered));
    static_assert( is_neq (QPartialOrdering::Unordered));
    static_assert(!is_lt  (QPartialOrdering::Unordered));
    static_assert(!is_lteq(QPartialOrdering::Unordered));
    static_assert(!is_gt  (QPartialOrdering::Unordered));
    static_assert(!is_gteq(QPartialOrdering::Unordered));

    static_assert(!(QPartialOrdering::Unordered == 0));
    static_assert( (QPartialOrdering::Unordered != 0));
    static_assert(!(QPartialOrdering::Unordered <  0));
    static_assert(!(QPartialOrdering::Unordered <= 0));
    static_assert(!(QPartialOrdering::Unordered >  0));
    static_assert(!(QPartialOrdering::Unordered >= 0));

    static_assert(!(0 == QPartialOrdering::Unordered));
    static_assert( (0 != QPartialOrdering::Unordered));
    static_assert(!(0 <  QPartialOrdering::Unordered));
    static_assert(!(0 <= QPartialOrdering::Unordered));
    static_assert(!(0 >  QPartialOrdering::Unordered));
    static_assert(!(0 >= QPartialOrdering::Unordered));


    static_assert(!is_eq  (QPartialOrdering::Less));
    static_assert( is_neq (QPartialOrdering::Less));
    static_assert( is_lt  (QPartialOrdering::Less));
    static_assert( is_lteq(QPartialOrdering::Less));
    static_assert(!is_gt  (QPartialOrdering::Less));
    static_assert(!is_gteq(QPartialOrdering::Less));

    static_assert(!(QPartialOrdering::Less == 0));
    static_assert( (QPartialOrdering::Less != 0));
    static_assert( (QPartialOrdering::Less <  0));
    static_assert( (QPartialOrdering::Less <= 0));
    static_assert(!(QPartialOrdering::Less >  0));
    static_assert(!(QPartialOrdering::Less >= 0));

    static_assert(!(0 == QPartialOrdering::Less));
    static_assert( (0 != QPartialOrdering::Less));
    static_assert(!(0 <  QPartialOrdering::Less));
    static_assert(!(0 <= QPartialOrdering::Less));
    static_assert( (0 >  QPartialOrdering::Less));
    static_assert( (0 >= QPartialOrdering::Less));


    static_assert( is_eq  (QPartialOrdering::Equivalent));
    static_assert(!is_neq (QPartialOrdering::Equivalent));
    static_assert(!is_lt  (QPartialOrdering::Equivalent));
    static_assert( is_lteq(QPartialOrdering::Equivalent));
    static_assert(!is_gt  (QPartialOrdering::Equivalent));
    static_assert( is_gteq(QPartialOrdering::Equivalent));

    static_assert( (QPartialOrdering::Equivalent == 0));
    static_assert(!(QPartialOrdering::Equivalent != 0));
    static_assert(!(QPartialOrdering::Equivalent <  0));
    static_assert( (QPartialOrdering::Equivalent <= 0));
    static_assert(!(QPartialOrdering::Equivalent >  0));
    static_assert( (QPartialOrdering::Equivalent >= 0));

    static_assert( (0 == QPartialOrdering::Equivalent));
    static_assert(!(0 != QPartialOrdering::Equivalent));
    static_assert(!(0 <  QPartialOrdering::Equivalent));
    static_assert( (0 <= QPartialOrdering::Equivalent));
    static_assert(!(0 >  QPartialOrdering::Equivalent));
    static_assert( (0 >= QPartialOrdering::Equivalent));


    static_assert(!is_eq  (QPartialOrdering::Greater));
    static_assert( is_neq (QPartialOrdering::Greater));
    static_assert(!is_lt  (QPartialOrdering::Greater));
    static_assert(!is_lteq(QPartialOrdering::Greater));
    static_assert( is_gt  (QPartialOrdering::Greater));
    static_assert( is_gteq(QPartialOrdering::Greater));

    static_assert(!(QPartialOrdering::Greater == 0));
    static_assert( (QPartialOrdering::Greater != 0));
    static_assert(!(QPartialOrdering::Greater <  0));
    static_assert(!(QPartialOrdering::Greater <= 0));
    static_assert( (QPartialOrdering::Greater >  0));
    static_assert( (QPartialOrdering::Greater >= 0));

    static_assert(!(0 == QPartialOrdering::Greater));
    static_assert( (0 != QPartialOrdering::Greater));
    static_assert( (0 <  QPartialOrdering::Greater));
    static_assert( (0 <= QPartialOrdering::Greater));
    static_assert(!(0 >  QPartialOrdering::Greater));
    static_assert(!(0 >= QPartialOrdering::Greater));
}

void tst_QCompare::legacyConversions()
{
#define CHECK_CONVERTS(Lhs, Rhs) static_assert(std::is_convertible_v<Lhs, Rhs>)
#define CHECK_ALL(NS) do { \
        CHECK_CONVERTS(QPartialOrdering, NS ::partial_ordering); \
        static_assert(QPartialOrdering::Less == NS ::partial_ordering::less); \
        static_assert(QPartialOrdering::Greater == NS ::partial_ordering::greater); \
        static_assert(QPartialOrdering::Equivalent == NS ::partial_ordering::equivalent); \
        static_assert(QPartialOrdering::Unordered == NS ::partial_ordering::unordered); \
        \
        CHECK_CONVERTS(NS ::partial_ordering, QPartialOrdering); \
        CHECK_CONVERTS(NS ::weak_ordering,    QPartialOrdering); \
        CHECK_CONVERTS(NS ::strong_ordering,  QPartialOrdering); \
    } while (false)

    CHECK_ALL(BobUI);
#ifdef __cpp_lib_three_way_comparison
    CHECK_ALL(std);
#endif // __cpp_lib_three_way_comparison

#undef CHECK_ALL
#undef CHECK_CONVERTS
}

void tst_QCompare::stdBobUIBinaryCompatibility()
{
#ifndef __cpp_lib_three_way_comparison
    QSKIP("This test requires C++20 three-way-comparison support enabled in the stdlib.");
#else
    QCOMPARE_EQ(sizeof(std::partial_ordering), 1U);
    QCOMPARE_EQ(sizeof( BobUI::partial_ordering), 1U);
    QCOMPARE_EQ(sizeof(std::   weak_ordering), 1U);
    QCOMPARE_EQ(sizeof( BobUI::   weak_ordering), 1U);
    QCOMPARE_EQ(sizeof(std:: strong_ordering), 1U);
    QCOMPARE_EQ(sizeof( BobUI:: strong_ordering), 1U);

    auto valueOf = [](auto obj) {
        typename QIntegerForSizeof<decltype(obj)>::Unsigned value;
        memcpy(&value, &obj, sizeof(obj));
        return value;
    };
#define CHECK(type, flag) \
        QCOMPARE_EQ(valueOf( BobUI:: type ## _ordering :: flag), \
                    valueOf(std:: type ## _ordering :: flag)) \
        /* end */
#  if !defined(Q_STL_LIBSTDCPP) || BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0)
    CHECK(partial, unordered);
#  endif
    CHECK(partial, less);
    CHECK(partial, greater);
    CHECK(partial, equivalent);

    CHECK(weak, less);
    CHECK(weak, greater);
    CHECK(weak, equivalent);

    CHECK(strong, less);
    CHECK(strong, greater);
    CHECK(strong, equivalent);
    CHECK(strong, equal);
#undef CHECK
#endif //__cpp_lib_three_way_comparison
}

void tst_QCompare::partialOrdering()
{
    static_assert(BobUI::partial_ordering::unordered == BobUI::partial_ordering::unordered);
    static_assert(BobUI::partial_ordering::unordered != BobUI::partial_ordering::less);
    static_assert(BobUI::partial_ordering::unordered != BobUI::partial_ordering::equivalent);
    static_assert(BobUI::partial_ordering::unordered != BobUI::partial_ordering::greater);

    static_assert(BobUI::partial_ordering::less != BobUI::partial_ordering::unordered);
    static_assert(BobUI::partial_ordering::less == BobUI::partial_ordering::less);
    static_assert(BobUI::partial_ordering::less != BobUI::partial_ordering::equivalent);
    static_assert(BobUI::partial_ordering::less != BobUI::partial_ordering::greater);

    static_assert(BobUI::partial_ordering::equivalent != BobUI::partial_ordering::unordered);
    static_assert(BobUI::partial_ordering::equivalent != BobUI::partial_ordering::less);
    static_assert(BobUI::partial_ordering::equivalent == BobUI::partial_ordering::equivalent);
    static_assert(BobUI::partial_ordering::equivalent != BobUI::partial_ordering::greater);

    static_assert(BobUI::partial_ordering::greater != BobUI::partial_ordering::unordered);
    static_assert(BobUI::partial_ordering::greater != BobUI::partial_ordering::less);
    static_assert(BobUI::partial_ordering::greater != BobUI::partial_ordering::equivalent);
    static_assert(BobUI::partial_ordering::greater == BobUI::partial_ordering::greater);

    static_assert(!is_eq  (BobUI::partial_ordering::unordered));
    static_assert( is_neq (BobUI::partial_ordering::unordered));
    static_assert(!is_lt  (BobUI::partial_ordering::unordered));
    static_assert(!is_lteq(BobUI::partial_ordering::unordered));
    static_assert(!is_gt  (BobUI::partial_ordering::unordered));
    static_assert(!is_gteq(BobUI::partial_ordering::unordered));

    static_assert(!(BobUI::partial_ordering::unordered == 0));
    static_assert( (BobUI::partial_ordering::unordered != 0));
    static_assert(!(BobUI::partial_ordering::unordered <  0));
    static_assert(!(BobUI::partial_ordering::unordered <= 0));
    static_assert(!(BobUI::partial_ordering::unordered >  0));
    static_assert(!(BobUI::partial_ordering::unordered >= 0));

    static_assert(!(0 == BobUI::partial_ordering::unordered));
    static_assert( (0 != BobUI::partial_ordering::unordered));
    static_assert(!(0 <  BobUI::partial_ordering::unordered));
    static_assert(!(0 <= BobUI::partial_ordering::unordered));
    static_assert(!(0 >  BobUI::partial_ordering::unordered));
    static_assert(!(0 >= BobUI::partial_ordering::unordered));


    static_assert(!is_eq  (BobUI::partial_ordering::less));
    static_assert( is_neq (BobUI::partial_ordering::less));
    static_assert( is_lt  (BobUI::partial_ordering::less));
    static_assert( is_lteq(BobUI::partial_ordering::less));
    static_assert(!is_gt  (BobUI::partial_ordering::less));
    static_assert(!is_gteq(BobUI::partial_ordering::less));

    static_assert(!(BobUI::partial_ordering::less == 0));
    static_assert( (BobUI::partial_ordering::less != 0));
    static_assert( (BobUI::partial_ordering::less <  0));
    static_assert( (BobUI::partial_ordering::less <= 0));
    static_assert(!(BobUI::partial_ordering::less >  0));
    static_assert(!(BobUI::partial_ordering::less >= 0));

    static_assert(!(0 == BobUI::partial_ordering::less));
    static_assert( (0 != BobUI::partial_ordering::less));
    static_assert(!(0 <  BobUI::partial_ordering::less));
    static_assert(!(0 <= BobUI::partial_ordering::less));
    static_assert( (0 >  BobUI::partial_ordering::less));
    static_assert( (0 >= BobUI::partial_ordering::less));


    static_assert( is_eq  (BobUI::partial_ordering::equivalent));
    static_assert(!is_neq (BobUI::partial_ordering::equivalent));
    static_assert(!is_lt  (BobUI::partial_ordering::equivalent));
    static_assert( is_lteq(BobUI::partial_ordering::equivalent));
    static_assert(!is_gt  (BobUI::partial_ordering::equivalent));
    static_assert( is_gteq(BobUI::partial_ordering::equivalent));

    static_assert( (BobUI::partial_ordering::equivalent == 0));
    static_assert(!(BobUI::partial_ordering::equivalent != 0));
    static_assert(!(BobUI::partial_ordering::equivalent <  0));
    static_assert( (BobUI::partial_ordering::equivalent <= 0));
    static_assert(!(BobUI::partial_ordering::equivalent >  0));
    static_assert( (BobUI::partial_ordering::equivalent >= 0));

    static_assert( (0 == BobUI::partial_ordering::equivalent));
    static_assert(!(0 != BobUI::partial_ordering::equivalent));
    static_assert(!(0 <  BobUI::partial_ordering::equivalent));
    static_assert( (0 <= BobUI::partial_ordering::equivalent));
    static_assert(!(0 >  BobUI::partial_ordering::equivalent));
    static_assert( (0 >= BobUI::partial_ordering::equivalent));


    static_assert(!is_eq  (BobUI::partial_ordering::greater));
    static_assert( is_neq (BobUI::partial_ordering::greater));
    static_assert(!is_lt  (BobUI::partial_ordering::greater));
    static_assert(!is_lteq(BobUI::partial_ordering::greater));
    static_assert( is_gt  (BobUI::partial_ordering::greater));
    static_assert( is_gteq(BobUI::partial_ordering::greater));

    static_assert(!(BobUI::partial_ordering::greater == 0));
    static_assert( (BobUI::partial_ordering::greater != 0));
    static_assert(!(BobUI::partial_ordering::greater <  0));
    static_assert(!(BobUI::partial_ordering::greater <= 0));
    static_assert( (BobUI::partial_ordering::greater >  0));
    static_assert( (BobUI::partial_ordering::greater >= 0));

    static_assert(!(0 == BobUI::partial_ordering::greater));
    static_assert( (0 != BobUI::partial_ordering::greater));
    static_assert( (0 <  BobUI::partial_ordering::greater));
    static_assert( (0 <= BobUI::partial_ordering::greater));
    static_assert(!(0 >  BobUI::partial_ordering::greater));
    static_assert(!(0 >= BobUI::partial_ordering::greater));
}

void tst_QCompare::weakOrdering()
{
    static_assert(BobUI::weak_ordering::less == BobUI::weak_ordering::less);
    static_assert(BobUI::weak_ordering::less != BobUI::weak_ordering::equivalent);
    static_assert(BobUI::weak_ordering::less != BobUI::weak_ordering::greater);

    static_assert(BobUI::weak_ordering::equivalent != BobUI::weak_ordering::less);
    static_assert(BobUI::weak_ordering::equivalent == BobUI::weak_ordering::equivalent);
    static_assert(BobUI::weak_ordering::equivalent != BobUI::weak_ordering::greater);

    static_assert(BobUI::weak_ordering::greater != BobUI::weak_ordering::less);
    static_assert(BobUI::weak_ordering::greater != BobUI::weak_ordering::equivalent);
    static_assert(BobUI::weak_ordering::greater == BobUI::weak_ordering::greater);

    static_assert(!is_eq  (BobUI::weak_ordering::less));
    static_assert( is_neq (BobUI::weak_ordering::less));
    static_assert( is_lt  (BobUI::weak_ordering::less));
    static_assert( is_lteq(BobUI::weak_ordering::less));
    static_assert(!is_gt  (BobUI::weak_ordering::less));
    static_assert(!is_gteq(BobUI::weak_ordering::less));

    static_assert(!(BobUI::weak_ordering::less == 0));
    static_assert( (BobUI::weak_ordering::less != 0));
    static_assert( (BobUI::weak_ordering::less <  0));
    static_assert( (BobUI::weak_ordering::less <= 0));
    static_assert(!(BobUI::weak_ordering::less >  0));
    static_assert(!(BobUI::weak_ordering::less >= 0));

    static_assert(!(0 == BobUI::weak_ordering::less));
    static_assert( (0 != BobUI::weak_ordering::less));
    static_assert(!(0 <  BobUI::weak_ordering::less));
    static_assert(!(0 <= BobUI::weak_ordering::less));
    static_assert( (0 >  BobUI::weak_ordering::less));
    static_assert( (0 >= BobUI::weak_ordering::less));


    static_assert( is_eq  (BobUI::weak_ordering::equivalent));
    static_assert(!is_neq (BobUI::weak_ordering::equivalent));
    static_assert(!is_lt  (BobUI::weak_ordering::equivalent));
    static_assert( is_lteq(BobUI::weak_ordering::equivalent));
    static_assert(!is_gt  (BobUI::weak_ordering::equivalent));
    static_assert( is_gteq(BobUI::weak_ordering::equivalent));

    static_assert( (BobUI::weak_ordering::equivalent == 0));
    static_assert(!(BobUI::weak_ordering::equivalent != 0));
    static_assert(!(BobUI::weak_ordering::equivalent <  0));
    static_assert( (BobUI::weak_ordering::equivalent <= 0));
    static_assert(!(BobUI::weak_ordering::equivalent >  0));
    static_assert( (BobUI::weak_ordering::equivalent >= 0));

    static_assert( (0 == BobUI::weak_ordering::equivalent));
    static_assert(!(0 != BobUI::weak_ordering::equivalent));
    static_assert(!(0 <  BobUI::weak_ordering::equivalent));
    static_assert( (0 <= BobUI::weak_ordering::equivalent));
    static_assert(!(0 >  BobUI::weak_ordering::equivalent));
    static_assert( (0 >= BobUI::weak_ordering::equivalent));


    static_assert(!is_eq  (BobUI::weak_ordering::greater));
    static_assert( is_neq (BobUI::weak_ordering::greater));
    static_assert(!is_lt  (BobUI::weak_ordering::greater));
    static_assert(!is_lteq(BobUI::weak_ordering::greater));
    static_assert( is_gt  (BobUI::weak_ordering::greater));
    static_assert( is_gteq(BobUI::weak_ordering::greater));

    static_assert(!(BobUI::weak_ordering::greater == 0));
    static_assert( (BobUI::weak_ordering::greater != 0));
    static_assert(!(BobUI::weak_ordering::greater <  0));
    static_assert(!(BobUI::weak_ordering::greater <= 0));
    static_assert( (BobUI::weak_ordering::greater >  0));
    static_assert( (BobUI::weak_ordering::greater >= 0));

    static_assert(!(0 == BobUI::weak_ordering::greater));
    static_assert( (0 != BobUI::weak_ordering::greater));
    static_assert( (0 <  BobUI::weak_ordering::greater));
    static_assert( (0 <= BobUI::weak_ordering::greater));
    static_assert(!(0 >  BobUI::weak_ordering::greater));
    static_assert(!(0 >= BobUI::weak_ordering::greater));
}

void tst_QCompare::strongOrdering()
{
    static_assert(BobUI::strong_ordering::less == BobUI::strong_ordering::less);
    static_assert(BobUI::strong_ordering::less != BobUI::strong_ordering::equal);
    static_assert(BobUI::strong_ordering::less != BobUI::strong_ordering::equivalent);
    static_assert(BobUI::strong_ordering::less != BobUI::strong_ordering::greater);

    static_assert(BobUI::strong_ordering::equal != BobUI::strong_ordering::less);
    static_assert(BobUI::strong_ordering::equal == BobUI::strong_ordering::equal);
    static_assert(BobUI::strong_ordering::equal == BobUI::strong_ordering::equivalent);
    static_assert(BobUI::strong_ordering::equal != BobUI::strong_ordering::greater);

    static_assert(BobUI::strong_ordering::equivalent != BobUI::strong_ordering::less);
    static_assert(BobUI::strong_ordering::equivalent == BobUI::strong_ordering::equal);
    static_assert(BobUI::strong_ordering::equivalent == BobUI::strong_ordering::equivalent);
    static_assert(BobUI::strong_ordering::equivalent != BobUI::strong_ordering::greater);

    static_assert(BobUI::strong_ordering::greater != BobUI::strong_ordering::less);
    static_assert(BobUI::strong_ordering::greater != BobUI::strong_ordering::equal);
    static_assert(BobUI::strong_ordering::greater != BobUI::strong_ordering::equivalent);
    static_assert(BobUI::strong_ordering::greater == BobUI::strong_ordering::greater);

    static_assert(!is_eq  (BobUI::strong_ordering::less));
    static_assert( is_neq (BobUI::strong_ordering::less));
    static_assert( is_lt  (BobUI::strong_ordering::less));
    static_assert( is_lteq(BobUI::strong_ordering::less));
    static_assert(!is_gt  (BobUI::strong_ordering::less));
    static_assert(!is_gteq(BobUI::strong_ordering::less));

    static_assert(!(BobUI::strong_ordering::less == 0));
    static_assert( (BobUI::strong_ordering::less != 0));
    static_assert( (BobUI::strong_ordering::less <  0));
    static_assert( (BobUI::strong_ordering::less <= 0));
    static_assert(!(BobUI::strong_ordering::less >  0));
    static_assert(!(BobUI::strong_ordering::less >= 0));

    static_assert(!(0 == BobUI::strong_ordering::less));
    static_assert( (0 != BobUI::strong_ordering::less));
    static_assert(!(0 <  BobUI::strong_ordering::less));
    static_assert(!(0 <= BobUI::strong_ordering::less));
    static_assert( (0 >  BobUI::strong_ordering::less));
    static_assert( (0 >= BobUI::strong_ordering::less));


    static_assert( is_eq  (BobUI::strong_ordering::equal));
    static_assert(!is_neq (BobUI::strong_ordering::equal));
    static_assert(!is_lt  (BobUI::strong_ordering::equal));
    static_assert( is_lteq(BobUI::strong_ordering::equal));
    static_assert(!is_gt  (BobUI::strong_ordering::equal));
    static_assert( is_gteq(BobUI::strong_ordering::equal));

    static_assert( (BobUI::strong_ordering::equal == 0));
    static_assert(!(BobUI::strong_ordering::equal != 0));
    static_assert(!(BobUI::strong_ordering::equal <  0));
    static_assert( (BobUI::strong_ordering::equal <= 0));
    static_assert(!(BobUI::strong_ordering::equal >  0));
    static_assert( (BobUI::strong_ordering::equal >= 0));

    static_assert( (0 == BobUI::strong_ordering::equal));
    static_assert(!(0 != BobUI::strong_ordering::equal));
    static_assert(!(0 <  BobUI::strong_ordering::equal));
    static_assert( (0 <= BobUI::strong_ordering::equal));
    static_assert(!(0 >  BobUI::strong_ordering::equal));
    static_assert( (0 >= BobUI::strong_ordering::equal));


    static_assert( is_eq  (BobUI::strong_ordering::equivalent));
    static_assert(!is_neq (BobUI::strong_ordering::equivalent));
    static_assert(!is_lt  (BobUI::strong_ordering::equivalent));
    static_assert( is_lteq(BobUI::strong_ordering::equivalent));
    static_assert(!is_gt  (BobUI::strong_ordering::equivalent));
    static_assert( is_gteq(BobUI::strong_ordering::equivalent));

    static_assert( (BobUI::strong_ordering::equivalent == 0));
    static_assert(!(BobUI::strong_ordering::equivalent != 0));
    static_assert(!(BobUI::strong_ordering::equivalent <  0));
    static_assert( (BobUI::strong_ordering::equivalent <= 0));
    static_assert(!(BobUI::strong_ordering::equivalent >  0));
    static_assert( (BobUI::strong_ordering::equivalent >= 0));

    static_assert( (0 == BobUI::strong_ordering::equivalent));
    static_assert(!(0 != BobUI::strong_ordering::equivalent));
    static_assert(!(0 <  BobUI::strong_ordering::equivalent));
    static_assert( (0 <= BobUI::strong_ordering::equivalent));
    static_assert(!(0 >  BobUI::strong_ordering::equivalent));
    static_assert( (0 >= BobUI::strong_ordering::equivalent));


    static_assert(!is_eq  (BobUI::strong_ordering::greater));
    static_assert( is_neq (BobUI::strong_ordering::greater));
    static_assert(!is_lt  (BobUI::strong_ordering::greater));
    static_assert(!is_lteq(BobUI::strong_ordering::greater));
    static_assert( is_gt  (BobUI::strong_ordering::greater));
    static_assert( is_gteq(BobUI::strong_ordering::greater));

    static_assert(!(BobUI::strong_ordering::greater == 0));
    static_assert( (BobUI::strong_ordering::greater != 0));
    static_assert(!(BobUI::strong_ordering::greater <  0));
    static_assert(!(BobUI::strong_ordering::greater <= 0));
    static_assert( (BobUI::strong_ordering::greater >  0));
    static_assert( (BobUI::strong_ordering::greater >= 0));

    static_assert(!(0 == BobUI::strong_ordering::greater));
    static_assert( (0 != BobUI::strong_ordering::greater));
    static_assert( (0 <  BobUI::strong_ordering::greater));
    static_assert( (0 <= BobUI::strong_ordering::greater));
    static_assert(!(0 >  BobUI::strong_ordering::greater));
    static_assert(!(0 >= BobUI::strong_ordering::greater));
}

void tst_QCompare::threeWayCompareWithLiteralZero()
{
#ifndef __cpp_lib_three_way_comparison
    QSKIP("This test requires C++20 <=> support enabled in the compiler and the stdlib.");
#else
    // the result of <=> is _always_ a std::_ordering type:
#define CHECK(O) do { \
        using StdO = typename BobUIOrderingPrivate::StdOrdering<O>::type; \
        static_assert(std::is_same_v<decltype(0 <=> std::declval<O&>()), StdO>); \
        static_assert(std::is_same_v<decltype(std::declval<O&>() <=> 0), StdO>); \
    } while (false)

    CHECK(BobUI::partial_ordering);
    CHECK(BobUI::weak_ordering);
    CHECK(BobUI::strong_ordering);
    CHECK(QPartialOrdering);
    // API symmetry check:
    CHECK(std::partial_ordering);
    CHECK(std::weak_ordering);
    CHECK(std::strong_ordering);

#undef CHECK

#define CHECK(O, what, reversed) do { \
        using StdO = typename BobUIOrderingPrivate::StdOrdering<O>::type; \
        static_assert((O :: what <=> 0) == StdO:: what); \
        static_assert((0 <=> O :: what) == StdO:: reversed); \
    } while (false)

    CHECK(BobUI::partial_ordering, unordered,  unordered);
    CHECK(BobUI::partial_ordering, equivalent, equivalent);
    CHECK(BobUI::partial_ordering, less,       greater);
    CHECK(BobUI::partial_ordering, greater,    less);

    CHECK(BobUI::weak_ordering, equivalent, equivalent);
    CHECK(BobUI::weak_ordering, less,       greater);
    CHECK(BobUI::weak_ordering, greater,    less);

    CHECK(BobUI::strong_ordering, equal,     equal);
    CHECK(BobUI::strong_ordering, less,      greater);
    CHECK(BobUI::strong_ordering, greater,   less);

    CHECK(QPartialOrdering, unordered,  unordered);
    CHECK(QPartialOrdering, equivalent, equivalent);
    CHECK(QPartialOrdering, less,       greater);
    CHECK(QPartialOrdering, greater,    less);

    // API symmetry check:

    CHECK(std::partial_ordering, unordered,  unordered);
    CHECK(std::partial_ordering, equivalent, equivalent);
    CHECK(std::partial_ordering, less,       greater);
    CHECK(std::partial_ordering, greater,    less);

    CHECK(std::weak_ordering, equivalent, equivalent);
    CHECK(std::weak_ordering, less,       greater);
    CHECK(std::weak_ordering, greater,    less);

    CHECK(std::strong_ordering, equal,     equal);
    CHECK(std::strong_ordering, less,      greater);
    CHECK(std::strong_ordering, greater,   less);

#undef CHECK
#endif // __cpp_lib_three_way_comparisons

}

void tst_QCompare::conversions()
{
    // BobUI::weak_ordering -> BobUI::partial_ordering
    {
        constexpr BobUI::partial_ordering less = BobUI::weak_ordering::less;
        static_assert(less == BobUI::partial_ordering::less);
        constexpr BobUI::partial_ordering equivalent = BobUI::weak_ordering::equivalent;
        static_assert(equivalent == BobUI::partial_ordering::equivalent);
        constexpr BobUI::partial_ordering greater = BobUI::weak_ordering::greater;
        static_assert(greater == BobUI::partial_ordering::greater);
    }
    // BobUI::strong_ordering -> BobUI::partial_ordering
    {
        constexpr BobUI::partial_ordering less = BobUI::strong_ordering::less;
        static_assert(less == BobUI::partial_ordering::less);
        constexpr BobUI::partial_ordering equal = BobUI::strong_ordering::equal;
        static_assert(equal == BobUI::partial_ordering::equivalent);
        constexpr BobUI::partial_ordering equivalent = BobUI::strong_ordering::equivalent;
        static_assert(equivalent == BobUI::partial_ordering::equivalent);
        constexpr BobUI::partial_ordering greater = BobUI::strong_ordering::greater;
        static_assert(greater == BobUI::partial_ordering::greater);
    }
    // BobUI::strong_ordering -> BobUI::weak_ordering
    {
        constexpr BobUI::weak_ordering less = BobUI::strong_ordering::less;
        static_assert(less == BobUI::weak_ordering::less);
        constexpr BobUI::weak_ordering equal = BobUI::strong_ordering::equal;
        static_assert(equal == BobUI::weak_ordering::equivalent);
        constexpr BobUI::weak_ordering equivalent = BobUI::strong_ordering::equivalent;
        static_assert(equivalent == BobUI::weak_ordering::equivalent);
        constexpr BobUI::weak_ordering greater = BobUI::strong_ordering::greater;
        static_assert(greater == BobUI::weak_ordering::greater);
    }
    // Mixed types
    {
        static_assert(BobUI::partial_ordering::less == BobUI::strong_ordering::less);
        static_assert(BobUI::partial_ordering::equivalent != BobUI::strong_ordering::less);
        static_assert(BobUI::partial_ordering::equivalent == BobUI::strong_ordering::equal);
        static_assert(BobUI::partial_ordering::greater == BobUI::strong_ordering::greater);

        static_assert(BobUI::partial_ordering::less == BobUI::weak_ordering::less);
        static_assert(BobUI::partial_ordering::equivalent == BobUI::weak_ordering::equivalent);
        static_assert(BobUI::partial_ordering::greater == BobUI::weak_ordering::greater);

        static_assert(BobUI::weak_ordering::less == BobUI::strong_ordering::less);
        static_assert(BobUI::weak_ordering::equivalent != BobUI::strong_ordering::greater);
        static_assert(BobUI::weak_ordering::equivalent == BobUI::strong_ordering::equal);
        static_assert(BobUI::weak_ordering::greater == BobUI::strong_ordering::greater);

        static_assert(BobUI::weak_ordering::less == BobUI::partial_ordering::less);
        static_assert(BobUI::weak_ordering::equivalent == BobUI::partial_ordering::equivalent);
        static_assert(BobUI::weak_ordering::greater == BobUI::partial_ordering::greater);

        static_assert(BobUI::strong_ordering::less == BobUI::partial_ordering::less);
        static_assert(BobUI::strong_ordering::equivalent == BobUI::partial_ordering::equivalent);
        static_assert(BobUI::strong_ordering::equal == BobUI::partial_ordering::equivalent);
        static_assert(BobUI::strong_ordering::greater == BobUI::partial_ordering::greater);

        static_assert(BobUI::strong_ordering::less == BobUI::weak_ordering::less);
        static_assert(BobUI::strong_ordering::equivalent == BobUI::weak_ordering::equivalent);
        static_assert(BobUI::strong_ordering::equal == BobUI::weak_ordering::equivalent);
        static_assert(BobUI::strong_ordering::greater == BobUI::weak_ordering::greater);
    }
#ifdef __cpp_lib_three_way_comparison
    // BobUI::partial_ordering <-> std::partial_ordering
    {
        static_assert(BobUI::partial_ordering::less == std::partial_ordering::less);
        static_assert(BobUI::partial_ordering::less != std::partial_ordering::greater);
        static_assert(std::partial_ordering::unordered != BobUI::partial_ordering::equivalent);
        static_assert(std::partial_ordering::unordered == BobUI::partial_ordering::unordered);

        static_assert((BobUI::partial_ordering(std::partial_ordering::less) ==
                       std::partial_ordering::less));
        static_assert((BobUI::partial_ordering(std::partial_ordering::equivalent) ==
                       std::partial_ordering::equivalent));
        static_assert((BobUI::partial_ordering(std::partial_ordering::greater) ==
                       std::partial_ordering::greater));
        static_assert((BobUI::partial_ordering(std::partial_ordering::unordered) ==
                       std::partial_ordering::unordered));
    }
    // BobUI::weak_ordering <-> std::weak_ordering
    {
        static_assert(BobUI::weak_ordering::less == std::weak_ordering::less);
        static_assert(BobUI::weak_ordering::less != std::weak_ordering::equivalent);
        static_assert(std::weak_ordering::greater != BobUI::weak_ordering::less);
        static_assert(std::weak_ordering::equivalent == BobUI::weak_ordering::equivalent);

        static_assert((BobUI::weak_ordering(std::weak_ordering::less) ==
                       std::weak_ordering::less));
        static_assert((BobUI::weak_ordering(std::weak_ordering::equivalent) ==
                       std::weak_ordering::equivalent));
        static_assert((BobUI::weak_ordering(std::weak_ordering::greater) ==
                       std::weak_ordering::greater));
    }
    // BobUI::strong_ordering <-> std::strong_ordering
    {
        static_assert(BobUI::strong_ordering::less == std::strong_ordering::less);
        static_assert(BobUI::strong_ordering::less != std::strong_ordering::equivalent);
        static_assert(std::strong_ordering::greater != BobUI::strong_ordering::less);
        static_assert(std::strong_ordering::equivalent == BobUI::strong_ordering::equivalent);

        static_assert((BobUI::strong_ordering(std::strong_ordering::less) ==
                       std::strong_ordering::less));
        static_assert((BobUI::strong_ordering(std::strong_ordering::equivalent) ==
                       std::strong_ordering::equivalent));
        static_assert((BobUI::strong_ordering(std::strong_ordering::greater) ==
                       std::strong_ordering::greater));
    }
    // Mixed BobUI::*_ordering <> std::*_ordering types
    {
        static_assert(BobUI::strong_ordering::less == std::partial_ordering::less);
        static_assert(BobUI::strong_ordering::less != std::partial_ordering::greater);
        static_assert(BobUI::strong_ordering::equal == std::weak_ordering::equivalent);
        static_assert(BobUI::strong_ordering::equivalent != std::weak_ordering::less);

        static_assert(BobUI::weak_ordering::less != std::partial_ordering::greater);
        static_assert(BobUI::weak_ordering::less == std::partial_ordering::less);
        static_assert(BobUI::weak_ordering::equivalent == std::strong_ordering::equivalent);
        static_assert(BobUI::weak_ordering::equivalent != std::strong_ordering::less);

        static_assert(BobUI::partial_ordering::less != std::weak_ordering::greater);
        static_assert(BobUI::partial_ordering::less == std::weak_ordering::less);
        static_assert(BobUI::partial_ordering::equivalent == std::strong_ordering::equivalent);
        static_assert(BobUI::partial_ordering::equivalent != std::strong_ordering::less);
    }
#endif

}

void tst_QCompare::is_eq_overloads()
{
#ifndef __cpp_lib_three_way_comparison
    QSKIP("This test requires C++20 three-way-comparison support enabled in the stdlib.");
#else
    constexpr auto u = std::partial_ordering::unordered;
    constexpr auto l = std::weak_ordering::less;
    constexpr auto g = std::strong_ordering::greater;
    constexpr auto e = std::weak_ordering::equivalent;
    constexpr auto s = std::strong_ordering::equal;

    // This is a compile-time check that unqualified name lookup of
    // std::is_eq-like functions isn't ambiguous, so we can recommend it to our
    // users for minimizing porting on the way to C++20.

    // The goal is to check each std::ordering and each is_eq function at least
    // once, not to test all combinations (we're not the stdlib test suite here).

    QVERIFY(is_eq(s));
    QVERIFY(is_neq(u));
    QVERIFY(is_lt(l));
    QVERIFY(is_gt(g));
    QVERIFY(is_lteq(e));
    QVERIFY(is_gteq(s));
#endif // __cpp_lib_three_way_comparison
}

class StringWrapper
{
public:
    explicit StringWrapper() {}
    explicit StringWrapper(const QString &val) : m_val(val) {}
    QString value() const { return m_val; }

private:
    static BobUI::weak_ordering compareHelper(const QString &lhs, const QString &rhs) noexcept
    {
        const int res = QString::compare(lhs, rhs, BobUI::CaseInsensitive);
        if (res < 0)
            return BobUI::weak_ordering::less;
        else if (res > 0)
            return BobUI::weak_ordering::greater;
        else
            return BobUI::weak_ordering::equivalent;
    }

    friend bool comparesEqual(const StringWrapper &lhs, const StringWrapper &rhs) noexcept
    { return QString::compare(lhs.m_val, rhs.m_val, BobUI::CaseInsensitive) == 0; }
    friend BobUI::weak_ordering
    compareThreeWay(const StringWrapper &lhs, const StringWrapper &rhs) noexcept
    { return compareHelper(lhs.m_val, rhs.m_val); }
    Q_DECLARE_WEAKLY_ORDERED(StringWrapper)

    // these helper functions are intentionally non-noexcept
    friend bool comparesEqual(const StringWrapper &lhs, int rhs)
    { return comparesEqual(lhs, StringWrapper(QString::number(rhs))); }
    friend BobUI::weak_ordering compareThreeWay(const StringWrapper &lhs, int rhs)
    { return compareHelper(lhs.m_val, QString::number(rhs)); }
    Q_DECLARE_WEAKLY_ORDERED_NON_NOEXCEPT(StringWrapper, int)

    QString m_val;
};

void tst_QCompare::compareThreeWay()
{
    // test noexcept

    // for custom types
    static_assert(noexcept(qCompareThreeWay(std::declval<StringWrapper>(),
                                            std::declval<StringWrapper>())));
    static_assert(!noexcept(qCompareThreeWay(std::declval<StringWrapper>(),
                                             std::declval<int>())));
    static_assert(!noexcept(qCompareThreeWay(std::declval<int>(),
                                             std::declval<StringWrapper>())));
    // for built-in types
    static_assert(noexcept(qCompareThreeWay(std::declval<int>(), std::declval<int>())));
    static_assert(noexcept(qCompareThreeWay(std::declval<float>(), std::declval<int>())));
    static_assert(noexcept(qCompareThreeWay(std::declval<double>(), std::declval<float>())));
    static_assert(noexcept(qCompareThreeWay(std::declval<int>(), std::declval<int>())));
    // enums
    static_assert(noexcept(qCompareThreeWay(std::declval<TestEnum>(), std::declval<TestEnum>())));

    // pointers
    using StringWrapperPtr = BobUI::totally_ordered_wrapper<StringWrapper *>;
    static_assert(noexcept(qCompareThreeWay(std::declval<StringWrapperPtr>(),
                                            std::declval<StringWrapperPtr>())));
    static_assert(noexcept(qCompareThreeWay(std::declval<StringWrapperPtr>(), nullptr)));

    // Test some actual comparison results

    // for custom types
    QCOMPARE_EQ(qCompareThreeWay(StringWrapper("ABC"), StringWrapper("abc")),
                BobUI::weak_ordering::equivalent);
    QVERIFY(StringWrapper("ABC") == StringWrapper("abc"));
    QCOMPARE_EQ(qCompareThreeWay(StringWrapper("ABC"), StringWrapper("qwe")),
                BobUI::weak_ordering::less);
    QVERIFY(StringWrapper("ABC") != StringWrapper("qwe"));
    QCOMPARE_EQ(qCompareThreeWay(StringWrapper("qwe"), StringWrapper("ABC")),
                BobUI::weak_ordering::greater);
    QVERIFY(StringWrapper("qwe") != StringWrapper("ABC"));
    QCOMPARE_EQ(qCompareThreeWay(StringWrapper("10"), 10), BobUI::weak_ordering::equivalent);
    QVERIFY(StringWrapper("10") == 10);
    QCOMPARE_EQ(qCompareThreeWay(StringWrapper("10"), 12), BobUI::weak_ordering::less);
    QVERIFY(StringWrapper("10") != 12);
    QCOMPARE_EQ(qCompareThreeWay(StringWrapper("12"), 10), BobUI::weak_ordering::greater);
    QVERIFY(StringWrapper("12") != 10);

    // reversed compareThreeWay()
    auto result = qCompareThreeWay(10, StringWrapper("12"));
    QCOMPARE_EQ(result, BobUI::weak_ordering::less);
    static_assert(std::is_same_v<decltype(result), BobUI::weak_ordering>);
    QVERIFY(10 != StringWrapper("12"));
    result = qCompareThreeWay(12, StringWrapper("10"));
    QCOMPARE_EQ(result, BobUI::weak_ordering::greater);
    static_assert(std::is_same_v<decltype(result), BobUI::weak_ordering>);
    QVERIFY(12 != StringWrapper("10"));
    result = qCompareThreeWay(10, StringWrapper("10"));
    QCOMPARE_EQ(result, BobUI::weak_ordering::equivalent);
    static_assert(std::is_same_v<decltype(result), BobUI::weak_ordering>);
    QVERIFY(10 == StringWrapper("10"));

    // built-in types
    QCOMPARE_EQ(qCompareThreeWay(1, 1.0), BobUI::partial_ordering::equivalent);
    QCOMPARE_EQ(qCompareThreeWay(1, 2), BobUI::strong_ordering::less);
    QCOMPARE_EQ(qCompareThreeWay(2.0f, 1.0), BobUI::partial_ordering::greater);

    // enums
    QCOMPARE_EQ(qCompareThreeWay(Smaller, Bigger), BobUI::strong_ordering::less);

    // pointers
    std::array<int, 2> arr{1, 0};
#if BOBUI_DEPRECATED_SINCE(6, 8)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
    QCOMPARE_EQ(qCompareThreeWay(&arr[1], &arr[0]), BobUI::strong_ordering::greater);
    QCOMPARE_EQ(qCompareThreeWay(arr.data(), &arr[0]), BobUI::strong_ordering::equivalent);
BOBUI_WARNING_POP
#endif // BOBUI_DEPRECATED_SINCE(6, 8)
    const auto a0 = BobUI::totally_ordered_wrapper(&arr[0]);
    const auto a1 = BobUI::totally_ordered_wrapper(&arr[1]);
    QCOMPARE_EQ(qCompareThreeWay(a1, a0), BobUI::strong_ordering::greater);
    QCOMPARE_EQ(qCompareThreeWay(arr.data(), a0), BobUI::strong_ordering::equivalent);
}

void tst_QCompare::unorderedNeqLiteralZero()
{
    // This test is checking BOBUIBUG-127759
    constexpr auto bobuiUnordered = BobUI::partial_ordering::unordered;
    constexpr auto bobuiLegacyUnordered = QPartialOrdering::Unordered;
#ifdef __cpp_lib_three_way_comparison
    constexpr auto stdUnordered = std::partial_ordering::unordered;

    QVERIFY(stdUnordered != 0);
    QVERIFY(0 != stdUnordered);
    QVERIFY(is_neq(stdUnordered));

    QCOMPARE_EQ(bobuiUnordered != 0, stdUnordered != 0);
    QCOMPARE_EQ(0 != bobuiUnordered, 0 != stdUnordered);
    QCOMPARE_EQ(is_neq(bobuiUnordered), is_neq(stdUnordered));

    QCOMPARE_EQ(bobuiLegacyUnordered != 0, stdUnordered != 0);
    QCOMPARE_EQ(0 != bobuiLegacyUnordered, 0 != stdUnordered);
    QCOMPARE_EQ(is_neq(bobuiLegacyUnordered), is_neq(stdUnordered));
#endif
    QVERIFY(bobuiUnordered != 0);
    QVERIFY(0 != bobuiUnordered);
    QVERIFY(is_neq(bobuiUnordered));

    QVERIFY(bobuiLegacyUnordered != 0);
    QVERIFY(0 != bobuiLegacyUnordered);
    QVERIFY(is_neq(bobuiLegacyUnordered));
}

#ifdef __cpp_lib_three_way_comparison
template<typename LeftType, typename RightType, typename OrderingType>
void tst_QCompare::compare3WayHelper(LeftType lhs, RightType rhs, OrderingType order)
{
    // check BobUI ordering type.
    QCOMPARE_3WAY(lhs, rhs, BobUIOrderingPrivate::to_BobUI(order));
    // Also check std ordering type.
    QCOMPARE_3WAY(lhs, rhs, BobUIOrderingPrivate::to_std(order));
}
#endif

void tst_QCompare::compare3WayMacro()
{
#ifdef __cpp_lib_three_way_comparison
    constexpr std::array comparison_array = {1, 0};
    // for custom types
    compare3WayHelper(StringWrapper("ABC"), StringWrapper("abc"),
                      BobUI::weak_ordering::equivalent);
    compare3WayHelper(StringWrapper("ABC"), StringWrapper("qwe"),
                      BobUI::weak_ordering::less);
    compare3WayHelper(StringWrapper("qwe"), StringWrapper("ABC"),
                      BobUI::weak_ordering::greater);

    compare3WayHelper(StringWrapper("10"), 10, BobUI::weak_ordering::equivalent);
    compare3WayHelper(StringWrapper("10"), 12, BobUI::weak_ordering::less);
    compare3WayHelper(StringWrapper("12"), 10, BobUI::weak_ordering::greater);

    // reversed compareThreeWay()
    compare3WayHelper(10, StringWrapper("12"), BobUI::weak_ordering::less);
    compare3WayHelper(12, StringWrapper("10"), BobUI::weak_ordering::greater);
    compare3WayHelper(10, StringWrapper("10"), BobUI::weak_ordering::equivalent);

    // built-in types
    compare3WayHelper(1, 1.0, BobUI::partial_ordering::equivalent);
    compare3WayHelper(1, 2, BobUI::strong_ordering::less);
    compare3WayHelper(2.0f, 1.0, BobUI::partial_ordering::greater);

    // enums
    compare3WayHelper(Smaller, Bigger, BobUI::strong_ordering::less);

    // pointers
    compare3WayHelper(&comparison_array[1], &comparison_array[0], BobUI::strong_ordering::greater);
    compare3WayHelper(comparison_array.data(),
                      &comparison_array[0], BobUI::strong_ordering::equivalent);

    const QString lhs = QStringLiteral("abc");

    // We don't want to put those into test data since we want to specifically test comparison
    // against inline string literals.
    compare3WayHelper(lhs, u"", BobUI::strong_ordering::greater);
    compare3WayHelper(lhs, u"abc", BobUI::strong_ordering::equal);
    compare3WayHelper(lhs, u"abcd", BobUI::strong_ordering::less);

    compare3WayHelper(lhs, "", BobUI::strong_ordering::greater);
    compare3WayHelper(lhs, "abc", BobUI::strong_ordering::equal);
    compare3WayHelper(lhs, "abcd", BobUI::strong_ordering::less);
#else
    QSKIP("This test requires C++20 comparison support enabled in the standard library");
#endif
}

BOBUIEST_MAIN(tst_QCompare)
#include "tst_qcompare.moc"
