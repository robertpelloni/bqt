// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QSize>
#ifdef QVARIANT_H
# error "This test requires qsize.h to not include qvariant.h"
#endif

// don't assume <type_traits>
template <typename T, typename U>
constexpr inline bool my_is_same_v = false;
template <typename T>
constexpr inline bool my_is_same_v<T, T> = true;

#define CHECK(cvref) \
    static_assert(my_is_same_v<decltype(get<0>(std::declval<QSize cvref >())), int cvref >); \
    static_assert(my_is_same_v<decltype(get<1>(std::declval<QSize cvref >())), int cvref >)

CHECK(&);
CHECK(const &);
CHECK(&&);
CHECK(const &&);

#undef CHECK

#include <BOBUIest>
#include <BobUITest/private/qcomparisontesthelper_p.h>
#include <qsize.h>

#include <array>

Q_DECLARE_METATYPE(QMargins)

class tst_QSize : public QObject
{
    Q_OBJECT
private slots:
    void compareCompiles();
    void compare_data();
    void compare();

    void getSetCheck();
    void scale();

    void expandedTo();
    void expandedTo_data();

    void boundedTo_data();
    void boundedTo();

    void grownOrShrunkBy_data();
    void grownOrShrunkBy();

    void toSizeF_data();
    void toSizeF();

    void transpose_data();
    void transpose();

    void structuredBinding();
};

void tst_QSize::compareCompiles()
{
    BOBUIestPrivate::testEqualityOperatorsCompile<QSize>();
}

void tst_QSize::compare_data()
{
    BOBUIest::addColumn<QSize>("lhs");
    BOBUIest::addColumn<QSize>("rhs");
    BOBUIest::addColumn<bool>("result");

    auto row = [](QSize lhs, QSize rhs, bool res) {
        BOBUIest::addRow("(%d, %d) vs (%d, %d)", lhs.width(), lhs.height(), rhs.width(), rhs.height())
                << lhs << rhs << res;
    };

    row(QSize(0, 0), QSize(0, 0), true);
    row(QSize(1, 0), QSize(0, 1), false);
    row(QSize(-1, -1), QSize(-1, -1), true);
    row(QSize(-1, -1), QSize(1, 1), false);
    row(QSize(INT_MIN, INT_MAX), QSize(INT_MAX, INT_MIN), false);
}

void tst_QSize::compare()
{
    QFETCH(QSize, lhs);
    QFETCH(QSize, rhs);
    QFETCH(bool, result);

    BOBUI_TEST_EQUALITY_OPS(lhs, rhs, result);
}

// Testing get/set functions
void tst_QSize::getSetCheck()
{
    QSize obj1;
    // int QSize::width()
    // void QSize::setWidth(int)
    obj1.setWidth(0);
    QCOMPARE(0, obj1.width());
    obj1.setWidth(INT_MIN);
    QCOMPARE(INT_MIN, obj1.width());
    obj1.setWidth(INT_MAX);
    QCOMPARE(INT_MAX, obj1.width());

    // int QSize::height()
    // void QSize::setHeight(int)
    obj1.setHeight(0);
    QCOMPARE(0, obj1.height());
    obj1.setHeight(INT_MIN);
    QCOMPARE(INT_MIN, obj1.height());
    obj1.setHeight(INT_MAX);
    QCOMPARE(INT_MAX, obj1.height());

    QSizeF obj2;
    // qreal QSizeF::width()
    // void QSizeF::setWidth(qreal)
    obj2.setWidth(0.0);
    QCOMPARE(0.0, obj2.width());
    obj2.setWidth(1.1);
    QCOMPARE(1.1, obj2.width());

    // qreal QSizeF::height()
    // void QSizeF::setHeight(qreal)
    obj2.setHeight(0.0);
    QCOMPARE(0.0, obj2.height());
    obj2.setHeight(1.1);
    QCOMPARE(1.1, obj2.height());
}

void tst_QSize::scale()
{
    QSize t1( 10, 12 );
    t1.scale( 60, 60, BobUI::IgnoreAspectRatio );
    QCOMPARE( t1, QSize(60, 60) );

    QSize t2( 10, 12 );
    t2.scale( 60, 60, BobUI::KeepAspectRatio );
    QCOMPARE( t2, QSize(50, 60) );

    QSize t3( 10, 12 );
    t3.scale( 60, 60, BobUI::KeepAspectRatioByExpanding );
    QCOMPARE( t3, QSize(60, 72) );

    QSize t4( 12, 10 );
    t4.scale( 60, 60, BobUI::KeepAspectRatio );
    QCOMPARE( t4, QSize(60, 50) );

    QSize t5( 12, 10 );
    t5.scale( 60, 60, BobUI::KeepAspectRatioByExpanding );
    QCOMPARE( t5, QSize(72, 60) );

    // test potential int overflow
    QSize t6(88473, 88473);
    t6.scale(141817, 141817, BobUI::KeepAspectRatio);
    QCOMPARE(t6, QSize(141817, 141817));

    QSize t7(800, 600);
    t7.scale(400, INT_MAX, BobUI::KeepAspectRatio);
    QCOMPARE(t7, QSize(400, 300));

    QSize t8(800, 600);
    t8.scale(INT_MAX, 150, BobUI::KeepAspectRatio);
    QCOMPARE(t8, QSize(200, 150));

    QSize t9(600, 800);
    t9.scale(300, INT_MAX, BobUI::KeepAspectRatio);
    QCOMPARE(t9, QSize(300, 400));

    QSize t10(600, 800);
    t10.scale(INT_MAX, 200, BobUI::KeepAspectRatio);
    QCOMPARE(t10, QSize(150, 200));

    QSize t11(0, 0);
    t11.scale(240, 200, BobUI::IgnoreAspectRatio);
    QCOMPARE(t11, QSize(240, 200));

    QSize t12(0, 0);
    t12.scale(240, 200, BobUI::KeepAspectRatio);
    QCOMPARE(t12, QSize(240, 200));

    QSize t13(0, 0);
    t13.scale(240, 200, BobUI::KeepAspectRatioByExpanding);
    QCOMPARE(t13, QSize(240, 200));
}


void tst_QSize::expandedTo_data()
{
    BOBUIest::addColumn<QSize>("input1");
    BOBUIest::addColumn<QSize>("input2");
    BOBUIest::addColumn<QSize>("expected");

    BOBUIest::newRow("data0") << QSize(10,12) << QSize(6,4) << QSize(10,12);
    BOBUIest::newRow("data1") << QSize(0,0)   << QSize(6,4) << QSize(6,4);
    // This should pick the highest of w,h components independently of each other,
    // thus the results don't have to be equal to neither input1 nor input2.
    BOBUIest::newRow("data3") << QSize(6,4)   << QSize(4,6) << QSize(6,6);
}

void tst_QSize::expandedTo()
{
    QFETCH( QSize, input1);
    QFETCH( QSize, input2);
    QFETCH( QSize, expected);

    QCOMPARE( input1.expandedTo(input2), expected);
}

void tst_QSize::boundedTo_data()
{
    BOBUIest::addColumn<QSize>("input1");
    BOBUIest::addColumn<QSize>("input2");
    BOBUIest::addColumn<QSize>("expected");

    BOBUIest::newRow("data0") << QSize(10,12) << QSize(6,4) << QSize(6,4);
    BOBUIest::newRow("data1") << QSize(0,0) << QSize(6,4) << QSize(0,0);
    // This should pick the lowest of w,h components independently of each other,
    // thus the results don't have to be equal to neither input1 nor input2.
    BOBUIest::newRow("data3") << QSize(6,4) << QSize(4,6) << QSize(4,4);
}

void tst_QSize::boundedTo()
{
    QFETCH( QSize, input1);
    QFETCH( QSize, input2);
    QFETCH( QSize, expected);

    QCOMPARE( input1.boundedTo(input2), expected);
}

void tst_QSize::grownOrShrunkBy_data()
{
    BOBUIest::addColumn<QSize>("input");
    BOBUIest::addColumn<QMargins>("margins");
    BOBUIest::addColumn<QSize>("grown");
    BOBUIest::addColumn<QSize>("shrunk");

    auto row = [](QSize i, QMargins m, QSize g, QSize s) {
        BOBUIest::addRow("{%d,%d}/{%d,%d,%d,%d}", i.width(), i.height(),
                      m.left(), m.top(), m.right(), m.bottom())
                << i << m << g << s;
    };

    const QSize zero = {0, 0};
    const QSize some = {100, 200};
    const QMargins zeroMargins = {};
    const QMargins negative = {-1, -2, -3, -4};
    const QMargins positive = { 1,  2,  3,  4};

    row(zero, zeroMargins, zero, zero);
    row(zero, negative, {-4, -6}, { 4,  6});
    row(zero, positive, { 4,  6}, {-4, -6});
    row(some, zeroMargins, some, some);
    row(some, negative, { 96, 194}, {104, 206});
    row(some, positive, {104, 206}, { 96, 194});
}

void tst_QSize::grownOrShrunkBy()
{
    QFETCH(const QSize, input);
    QFETCH(const QMargins, margins);
    QFETCH(const QSize, grown);
    QFETCH(const QSize, shrunk);

    QCOMPARE(input.grownBy(margins), grown);
    QCOMPARE(input.shrunkBy(margins), shrunk);
    QCOMPARE(grown.shrunkBy(margins), input);
    QCOMPARE(shrunk.grownBy(margins), input);
}

void tst_QSize::toSizeF_data()
{
    BOBUIest::addColumn<QSize>("input");
    BOBUIest::addColumn<QSizeF>("result");

    auto row = [](int w, int h) {
        BOBUIest::addRow("(%d, %d)", w, h) << QSize(w, h) << QSizeF(w, h);
    };
    constexpr std::array samples = {-1, 0, 1};
    for (int w : samples) {
        for (int h : samples) {
            row(w, h);
        }
    }
}

void tst_QSize::toSizeF()
{
    QFETCH(const QSize, input);
    QFETCH(const QSizeF, result);

    QCOMPARE(input.toSizeF(), result);
}

void tst_QSize::transpose_data()
{
    BOBUIest::addColumn<QSize>("input1");
    BOBUIest::addColumn<QSize>("expected");

    BOBUIest::newRow("data0") << QSize(10,12) << QSize(12,10);
    BOBUIest::newRow("data1") << QSize(0,0) << QSize(0,0);
    BOBUIest::newRow("data3") << QSize(6,4) << QSize(4,6);
}

void tst_QSize::transpose()
{
    QFETCH( QSize, input1);
    QFETCH( QSize, expected);

    // transpose() works only inplace and does not return anything, so we must do the operation itself before the compare.
    input1.transpose();
    QCOMPARE(input1 , expected);
}

void tst_QSize::structuredBinding()
{
    {
        QSize size(10, 20);
        auto [width, height] = size;
        QCOMPARE(width, 10);
        QCOMPARE(height, 20);
    }
    {
        QSize size(30, 40);
        auto &[width, height] = size;
        QCOMPARE(width, 30);
        QCOMPARE(height, 40);

        width = 100;
        height = 200;
        QCOMPARE(size.width(), 100);
        QCOMPARE(size.height(), 200);
    }
}

namespace ConstexprTests {
constexpr QSize s = (QSize(10, 20) + QSize(30, 40)) * 2;
static_assert(s.width() == 80);
static_assert(s.height() == 120);
} // namespace ConstexprTests

BOBUIEST_APPLESS_MAIN(tst_QSize)
#include "tst_qsize.moc"
