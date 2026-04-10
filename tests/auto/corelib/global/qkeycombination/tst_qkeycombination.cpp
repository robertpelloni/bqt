// Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QObject>
#include <BOBUIest>
#include <BobUITest/private/qcomparisontesthelper_p.h>

class tst_QKeyCombination : public QObject
{
    Q_OBJECT
private slots:
    void compareCompiles();
    void construction();
    void operator_eq();
    void operator_or();
};

constexpr int bitwiseOr()
{
    return 0;
}

template <typename ...T>
constexpr auto bitwiseOr(T ... args)
{
    return (... | ((int)args));
}

void tst_QKeyCombination::compareCompiles()
{
    BOBUIestPrivate::testEqualityOperatorsCompile<QKeyCombination>();
}

void tst_QKeyCombination::construction()
{
    {
        QKeyCombination combination;
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers{});
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::Key_unknown));
    }

    {
        QKeyCombination combination(BobUI::CTRL); // explicit
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::ControlModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::ControlModifier, BobUI::Key_unknown));
    }

    {
        QKeyCombination combination(BobUI::SHIFT); // explicit
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::ShiftModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::ShiftModifier, BobUI::Key_unknown));
    }

    {
        QKeyCombination combination(BobUI::AltModifier); // explicit
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::AltModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::AltModifier, BobUI::Key_unknown));
    }

    {
        QKeyCombination combination(BobUI::AltModifier | BobUI::ControlModifier); // explicit
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::AltModifier | BobUI::ControlModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::AltModifier, BobUI::ControlModifier, BobUI::Key_unknown));
    }

    {
        QKeyCombination combination = BobUI::Key_A; // implicit
        QCOMPARE(combination.key(), BobUI::Key_A);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers{});
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::Key_A));
    }

    {
        QKeyCombination combination = BobUI::Key_F1; // implicit
        QCOMPARE(combination.key(), BobUI::Key_F1);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers{});
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::Key_F1));
    }

    {
        QKeyCombination combination(BobUI::SHIFT, BobUI::Key_F1);
        QCOMPARE(combination.key(), BobUI::Key_F1);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::ShiftModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::SHIFT, BobUI::Key_F1));
    }

    {
        QKeyCombination combination(BobUI::SHIFT | BobUI::CTRL, BobUI::Key_F1);
        QCOMPARE(combination.key(), BobUI::Key_F1);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::ShiftModifier | BobUI::ControlModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::SHIFT, BobUI::CTRL, BobUI::Key_F1));
    }

    {
        QKeyCombination combination(BobUI::AltModifier, BobUI::Key_F1);
        QCOMPARE(combination.key(), BobUI::Key_F1);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::AltModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::AltModifier, BobUI::Key_F1));
    }

    // corner cases
    {
        QKeyCombination combination = BobUI::Key_Alt;
        QCOMPARE(combination.key(), BobUI::Key_Alt);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers{});
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::Key_Alt));
    }

    {
        QKeyCombination combination(BobUI::ALT, BobUI::Key_Alt);
        QCOMPARE(combination.key(), BobUI::Key_Alt);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::AltModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::ALT, BobUI::Key_Alt));
    }

    {
        QKeyCombination combination(BobUI::Key_unknown);
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers{});
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::Key_unknown));
    }

    {
        QKeyCombination combination(BobUI::CTRL | BobUI::SHIFT, BobUI::Key_unknown);
        QCOMPARE(combination.key(), BobUI::Key_unknown);
        QCOMPARE(combination.keyboardModifiers(), BobUI::KeyboardModifiers(BobUI::ControlModifier | BobUI::ShiftModifier));
        QCOMPARE(combination.toCombined(), bitwiseOr(BobUI::CTRL, BobUI::SHIFT, BobUI::Key_unknown));
    }
}

void tst_QKeyCombination::operator_eq()
{
    // default
    {
        QKeyCombination a, b;
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    // key only
    {
        QKeyCombination a;
        QKeyCombination b(BobUI::Key_X);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::Key_Y);
        QKeyCombination b;
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::Key_Y);
        QKeyCombination b(BobUI::Key_X);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::Key_F1);
        QKeyCombination b(BobUI::Key_F1);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    // modifier only
    {
        QKeyCombination a;
        QKeyCombination b(BobUI::CTRL);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::CTRL);
        QKeyCombination b;
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::CTRL);
        QKeyCombination b(BobUI::SHIFT);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::CTRL);
        QKeyCombination b(BobUI::CTRL);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    {
        QKeyCombination a(BobUI::CTRL);
        QKeyCombination b(BobUI::ControlModifier);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    {
        QKeyCombination a(BobUI::ControlModifier);
        QKeyCombination b(BobUI::CTRL);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    {
        QKeyCombination a(BobUI::ControlModifier);
        QKeyCombination b(BobUI::ControlModifier);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    // key and modifier
    {
        QKeyCombination a(BobUI::Key_A);
        QKeyCombination b(BobUI::SHIFT, BobUI::Key_A);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::CTRL, BobUI::Key_A);
        QKeyCombination b(BobUI::SHIFT, BobUI::Key_A);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::SHIFT, BobUI::Key_A);
        QKeyCombination b(BobUI::SHIFT, BobUI::Key_A);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    {
        QKeyCombination a(BobUI::SHIFT, BobUI::Key_A);
        QKeyCombination b(BobUI::SHIFT, BobUI::Key_Escape);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::SHIFT, BobUI::Key_A);
        QKeyCombination b(BobUI::ShiftModifier, BobUI::Key_A);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    {
        QKeyCombination a(BobUI::SHIFT | BobUI::CTRL, BobUI::Key_A);
        QKeyCombination b(BobUI::ControlModifier | BobUI::ShiftModifier, BobUI::Key_A);
        BOBUI_TEST_EQUALITY_OPS(a, b, true);
    }

    // corner cases
    {
        QKeyCombination a(BobUI::CTRL);
        QKeyCombination b(BobUI::Key_Control);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }

    {
        QKeyCombination a(BobUI::ALT);
        QKeyCombination b(BobUI::Key_Alt);
        BOBUI_TEST_EQUALITY_OPS(a, b, false);
    }
}

void tst_QKeyCombination::operator_or()
{
    // note tha operator+ between enumerators of the same enumeration
    // yields int, so one can't do
    //    BobUI::SHIFT + BobUI::CTRL + BobUI::Key_A
    // but one can do
    //    BobUI::SHIFT | BobUI::CTRL | BobUI::Key_A

    QCOMPARE(BobUI::SHIFT | BobUI::Key_A, QKeyCombination(BobUI::SHIFT, BobUI::Key_A));
    QCOMPARE(BobUI::AltModifier | BobUI::Key_F1, QKeyCombination(BobUI::AltModifier, BobUI::Key_F1));
    QCOMPARE(BobUI::SHIFT | BobUI::ALT | BobUI::Key_F1, QKeyCombination(BobUI::SHIFT | BobUI::ALT, BobUI::Key_F1));
    QCOMPARE(BobUI::ControlModifier | BobUI::Key_Escape, QKeyCombination(BobUI::ControlModifier, BobUI::Key_Escape));
}

namespace BOBUIBUG_126820 {
struct S {};
S operator|(S, S);
S operator+(S, S);

[[maybe_unused]] constexpr QKeyCombination kc1 = BobUI::SHIFT | BobUI::Key_A;
#if BOBUI_DEPRECATED_SINCE(6, 0)
[[maybe_unused]] constexpr QKeyCombination kc2 = BobUI::SHIFT + BobUI::Key_A;
#endif
}

BOBUIEST_APPLESS_MAIN(tst_QKeyCombination)

#include "tst_qkeycombination.moc"
