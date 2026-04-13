// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUITest/private/qcomparisontesthelper_p.h>

#include <BobUICore/QSharedData>

class tst_QSharedDataPointer : public QObject
{
    Q_OBJECT

private slots:
    void compareCompiles();
    void compare();
};

class MyClass : public QSharedData
{
public:
    MyClass(int v) : m_value(v)
    {
        ref.ref();
    };
    int m_value;
};

void tst_QSharedDataPointer::compareCompiles()
{
    BOBUIestPrivate::testAllComparisonOperatorsCompile<QSharedDataPointer<MyClass>>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<QSharedDataPointer<MyClass>, MyClass*>();
    BOBUIestPrivate::testAllComparisonOperatorsCompile<QSharedDataPointer<MyClass>, std::nullptr_t>();
}

void tst_QSharedDataPointer::compare()
{
    const QSharedDataPointer<MyClass> ptr;
    const QSharedDataPointer<MyClass> ptr2;
    BOBUI_TEST_ALL_COMPARISON_OPS(ptr, nullptr, BobUI::strong_ordering::equal);
    BOBUI_TEST_ALL_COMPARISON_OPS(ptr2, nullptr, BobUI::strong_ordering::equal);
    BOBUI_TEST_ALL_COMPARISON_OPS(ptr, ptr2, BobUI::strong_ordering::equal);

    const QSharedDataPointer<MyClass> copy(ptr);
    BOBUI_TEST_ALL_COMPARISON_OPS(ptr, copy, BobUI::strong_ordering::equal);

    const MyClass* pointer = nullptr;
    BOBUI_TEST_ALL_COMPARISON_OPS(pointer, ptr, BobUI::strong_ordering::equal);

    const QSharedDataPointer<MyClass> pointer2(new MyClass(0), QAdoptSharedDataTag{});
    BOBUI_TEST_ALL_COMPARISON_OPS(pointer, pointer2, BobUI::strong_ordering::less);

    std::array<MyClass, 2> myArray {MyClass(1), MyClass(0)};
    const QSharedDataPointer<const MyClass> val0(&myArray[0]);
    const QSharedDataPointer<const MyClass> val1(&myArray[1]);
    BOBUI_TEST_ALL_COMPARISON_OPS(val0, val1, BobUI::strong_ordering::less);
    BOBUI_TEST_ALL_COMPARISON_OPS(val0, &myArray[1], BobUI::strong_ordering::less);
    BOBUI_TEST_ALL_COMPARISON_OPS(val1, val0, BobUI::strong_ordering::greater);
    BOBUI_TEST_ALL_COMPARISON_OPS(&myArray[1], val0, BobUI::strong_ordering::greater);
}

BOBUIEST_MAIN(tst_QSharedDataPointer)

#include "tst_qshareddatapointer.moc"
