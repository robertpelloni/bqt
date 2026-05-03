// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TST_QCOMPAREHELPERS_WRAPPERTYPES_H
#define TST_QCOMPAREHELPERS_WRAPPERTYPES_H

#include <BobUICore/qanystringview.h>
#include <BobUICore/qcompare.h>

class IntWrapper
{
public:
    // implicit constructor and operator int() to simulate the case that
    // triggers a bug on MSVC < 19.36.
    IntWrapper(int val) : m_val(val) {}
    operator int() const noexcept { return m_val; }

    int value() const { return m_val; }

private:
    friend bool comparesEqual(const IntWrapper &lhs, const IntWrapper &rhs) noexcept
    { return lhs.m_val == rhs.m_val; }
    friend BobUI::strong_ordering
    compareThreeWay(const IntWrapper &lhs, const IntWrapper &rhs) noexcept
    {
        return BobUI::compareThreeWay(lhs.m_val, rhs.m_val);
    }
    friend bool comparesEqual(const IntWrapper &lhs, int rhs) noexcept
    { return lhs.m_val == rhs; }
    friend BobUI::strong_ordering compareThreeWay(const IntWrapper &lhs, int rhs) noexcept
    { return compareThreeWay(lhs, IntWrapper(rhs)); }

    Q_DECLARE_STRONGLY_ORDERED(IntWrapper)
    Q_DECLARE_STRONGLY_ORDERED(IntWrapper, int)

    int m_val = 0;
};

class DoubleWrapper
{
public:
    explicit DoubleWrapper(double val) : m_val(val) {}
    double value() const { return m_val; }

private:
    friend bool comparesEqual(const DoubleWrapper &lhs, const DoubleWrapper &rhs) noexcept
    { return lhs.m_val == rhs.m_val; }
    friend BobUI::partial_ordering
    compareThreeWay(const DoubleWrapper &lhs, const DoubleWrapper &rhs) noexcept
    {
        return BobUI::compareThreeWay(lhs.m_val, rhs.m_val);
    }
    friend bool comparesEqual(const DoubleWrapper &lhs, const IntWrapper &rhs) noexcept
    { return comparesEqual(lhs, DoubleWrapper(rhs.value())); }
    friend BobUI::partial_ordering
    compareThreeWay(const DoubleWrapper &lhs, const IntWrapper &rhs) noexcept
    { return compareThreeWay(lhs, DoubleWrapper(rhs.value())); }
    friend bool comparesEqual(const DoubleWrapper &lhs, double rhs) noexcept
    { return lhs.m_val == rhs; }
    friend BobUI::partial_ordering compareThreeWay(const DoubleWrapper &lhs, double rhs) noexcept
    {
        return BobUI::compareThreeWay(lhs.m_val, rhs);
    }

    Q_DECLARE_PARTIALLY_ORDERED(DoubleWrapper)
    Q_DECLARE_PARTIALLY_ORDERED(DoubleWrapper, IntWrapper)
    Q_DECLARE_PARTIALLY_ORDERED(DoubleWrapper, double)

    double m_val = 0.0;
};

template <typename String>
class StringWrapper
{
public:
    explicit StringWrapper(String val) : m_val(val) {}
    String value() const { return m_val; }

private:
    static bool equalsHelper(QAnyStringView lhs, QAnyStringView rhs) noexcept
    { return QAnyStringView::compare(lhs, rhs, BobUI::CaseInsensitive) == 0; }

    static BobUI::weak_ordering compareHelper(QAnyStringView lhs, QAnyStringView rhs) noexcept
    {
        const int res = QAnyStringView::compare(lhs, rhs, BobUI::CaseInsensitive);
        if (res < 0)
            return BobUI::weak_ordering::less;
        else if (res > 0)
            return BobUI::weak_ordering::greater;
        else
            return BobUI::weak_ordering::equivalent;
    }

    // Some of the helper functions are intentionally NOT marked as noexcept
    // to test various helper macros
    template <typename T>
    friend bool comparesEqual(const StringWrapper<T> &lhs, const StringWrapper<T> &rhs) noexcept
    { return StringWrapper<T>::equalsHelper(lhs.m_val, rhs.m_val); }
    template <typename T>
    friend BobUI::weak_ordering
    compareThreeWay(const StringWrapper<T> &lhs, const StringWrapper<T> &rhs) noexcept
    { return StringWrapper<T>::compareHelper(lhs.m_val, rhs.m_val); }
    template <typename T>
    friend bool comparesEqual(const StringWrapper<T> &lhs, QAnyStringView rhs)
    { return StringWrapper<T>::equalsHelper(lhs.m_val, rhs); }
    template <typename T>
    friend BobUI::weak_ordering compareThreeWay(const StringWrapper<T> &lhs, QAnyStringView rhs)
    { return StringWrapper<T>::compareHelper(lhs.m_val, rhs); }

    Q_DECLARE_WEAKLY_ORDERED(StringWrapper)
    Q_DECLARE_WEAKLY_ORDERED_NON_NOEXCEPT(StringWrapper, QAnyStringView)

    String m_val;
};

#endif // TST_QCOMPAREHELPERS_WRAPPERTYPES_H
