// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <QAtomicInt>
#include <QCoreApplication>
#include <QElapsedTimer>

#include <limits.h>

class tst_QAtomicInt : public QObject
{
    Q_OBJECT

private slots:
    void warningFree();
    void alignment();

    // QAtomicInt members
    void constructor_data();
    void constructor();
    void copy_constructor_data();
    void copy_constructor();
    void assignment_operator_data();
    void assignment_operator();

    void isReferenceCountingNative();
    void isReferenceCountingWaitFree();
    void ref_data();
    void ref();
    void deref_data();
    void deref();

    void isTestAndSetNative();
    void isTestAndSetWaitFree();
    void testAndSet_data();
    void testAndSet();

    void isFetchAndStoreNative();
    void isFetchAndStoreWaitFree();
    void fetchAndStore_data();
    void fetchAndStore();

    void isFetchAndAddNative();
    void isFetchAndAddWaitFree();
    void fetchAndAdd_data();
    void fetchAndAdd();

    void operators();

    // stress tests
    void testAndSet_loop();
    void fetchAndAdd_loop();
    void fetchAndAdd_threadedLoop();

private:
    static void warningFreeHelper();
};

template <int I>
static inline void assemblyMarker(void *ptr = nullptr)
{
    puts((char *)ptr + I);
}

template <typename  T, typename Atomic>
static void warningFreeHelperTemplate()
{
    T expectedValue = 0;
    T newValue = 0;
    T valueToAdd = 0;

    // the marker calls are here only to provide a divider for
    // those reading the assembly output
    assemblyMarker<0>();
    Atomic i = Q_BASIC_ATOMIC_INITIALIZER(0);
    printf("%d\n", int(i.loadAcquire()));
    assemblyMarker<1>(&i);

    // the loads sometimes generate no assembly output
    i.loadRelaxed();
    assemblyMarker<11>(&i);
    i.loadAcquire();
    assemblyMarker<12>(&i);

    i.storeRelaxed(newValue);
    assemblyMarker<21>(&i);
    i.storeRelease(newValue);
    assemblyMarker<22>(&i);

    i.ref();
    assemblyMarker<31>(&i);
    i.deref();
    assemblyMarker<32>(&i);

    i.testAndSetRelaxed(expectedValue, newValue);
    assemblyMarker<41>(&i);
    i.testAndSetAcquire(expectedValue, newValue);
    assemblyMarker<42>(&i);
    i.testAndSetRelease(expectedValue, newValue);
    assemblyMarker<43>(&i);
    i.testAndSetOrdered(expectedValue, newValue);
    assemblyMarker<44>(&i);

    i.fetchAndStoreRelaxed(newValue);
    assemblyMarker<51>(&i);
    i.fetchAndStoreAcquire(newValue);
    assemblyMarker<52>(&i);
    i.fetchAndStoreRelease(newValue);
    assemblyMarker<53>(&i);
    i.fetchAndStoreOrdered(newValue);
    assemblyMarker<54>(&i);

    i.fetchAndAddRelaxed(valueToAdd);
    assemblyMarker<61>(&i);
    i.fetchAndAddAcquire(valueToAdd);
    assemblyMarker<62>(&i);
    i.fetchAndAddRelease(valueToAdd);
    assemblyMarker<63>(&i);
    i.fetchAndAddOrdered(valueToAdd);
    assemblyMarker<64>(&i);
}

template <bool> inline void booleanHelper()
{ }

template <typename Atomic>
static void constexprFunctionsHelperTemplate()
{
    // this is a compile-time test only
    booleanHelper<Atomic::isReferenceCountingWaitFree()>();
    booleanHelper<Atomic::isTestAndSetWaitFree()>();
    booleanHelper<Atomic::isFetchAndStoreWaitFree()>();
    booleanHelper<Atomic::isFetchAndAddWaitFree()>();
}

void tst_QAtomicInt::warningFreeHelper()
{
    qFatal("This code is bogus, and shouldn't be run. We're looking for compiler warnings only.");
    warningFreeHelperTemplate<int, QBasicAtomicInt>();

    // 32-bit are always supported:
    warningFreeHelperTemplate<int, QBasicAtomicInteger<int> >();
    warningFreeHelperTemplate<unsigned int, QBasicAtomicInteger<unsigned int> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<int> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<unsigned int> >();
    warningFreeHelperTemplate<qint16, QBasicAtomicInteger<char32_t> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<char32_t> >();

    // pointer-sized integers are always supported:
    warningFreeHelperTemplate<int, QBasicAtomicInteger<qptrdiff> >();
    warningFreeHelperTemplate<unsigned int, QBasicAtomicInteger<quintptr> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<qptrdiff> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<quintptr> >();

    // long is always supported because it's either 32-bit or pointer-sized:
    warningFreeHelperTemplate<int, QBasicAtomicInteger<long int> >();
    warningFreeHelperTemplate<unsigned int, QBasicAtomicInteger<unsigned long int> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<long int> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<unsigned long int> >();

    warningFreeHelperTemplate<qint16, QBasicAtomicInteger<qint16> >();
    warningFreeHelperTemplate<quint16, QBasicAtomicInteger<quint16> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<qint16> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<quint16> >();
    warningFreeHelperTemplate<qint16, QBasicAtomicInteger<char16_t> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<char16_t> >();

    warningFreeHelperTemplate<char, QBasicAtomicInteger<char> >();
    warningFreeHelperTemplate<signed char, QBasicAtomicInteger<signed char> >();
    warningFreeHelperTemplate<unsigned char, QBasicAtomicInteger<unsigned char> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<char> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<signed char> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<unsigned char> >();

#ifdef Q_ATOMIC_INT64_IS_SUPPORTED
#if !defined(__i386__) || (defined(Q_CC_GNU) && defined(__OPTIMIZE__))
    warningFreeHelperTemplate<qlonglong, QBasicAtomicInteger<qlonglong> >();
    warningFreeHelperTemplate<qulonglong, QBasicAtomicInteger<qulonglong> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<qlonglong> >();
    constexprFunctionsHelperTemplate<QBasicAtomicInteger<qulonglong> >();
#endif
#endif
}

void tst_QAtomicInt::warningFree()
{
    // This is a compile time check for warnings.
    // No need for actual work here.

    void (*foo)() = &warningFreeHelper;
    (void)foo;
}

template <typename T> struct TypeInStruct { T type; };

void tst_QAtomicInt::alignment()
{
    static_assert(alignof(QBasicAtomicInt) == alignof(TypeInStruct<int>));
    static_assert(alignof(QBasicAtomicInt) == alignof(TypeInStruct<int>));

    QCOMPARE(alignof(QBasicAtomicInteger<int>), alignof(TypeInStruct<int>));
    QCOMPARE(alignof(QBasicAtomicInteger<short>), alignof(TypeInStruct<short>));
    QCOMPARE(alignof(QBasicAtomicInteger<char>), alignof(TypeInStruct<char>));

#if !defined(Q_PROCESSOR_X86_32) && defined(Q_ATOMIC_INT64_IS_SUPPORTED)
    // The alignment is different on x86_32
    QCOMPARE(alignof(QBasicAtomicInteger<qlonglong>), alignof(TypeInStruct<qlonglong>));
#endif
}

void tst_QAtomicInt::constructor_data()
{
    BOBUIest::addColumn<int>("value");

    BOBUIest::newRow("0") << 31337;
    BOBUIest::newRow("1") << 0;
    BOBUIest::newRow("2") << 1;
    BOBUIest::newRow("3") << -1;
    BOBUIest::newRow("4") << 2;
    BOBUIest::newRow("5") << -2;
    BOBUIest::newRow("6") << 3;
    BOBUIest::newRow("7") << -3;
    BOBUIest::newRow("8") << INT_MAX;
    BOBUIest::newRow("9") << INT_MIN+1;
}

void tst_QAtomicInt::constructor()
{
    QFETCH(int, value);
    QAtomicInt atomic1(value);
    QCOMPARE(atomic1.loadRelaxed(), value);
    QAtomicInt atomic2 = value;
    QCOMPARE(atomic2.loadRelaxed(), value);
}

void tst_QAtomicInt::copy_constructor_data()
{ constructor_data(); }

void tst_QAtomicInt::copy_constructor()
{
    QFETCH(int, value);
    QAtomicInt atomic1(value);
    QCOMPARE(atomic1.loadRelaxed(), value);

    QAtomicInt atomic2(atomic1);
    QCOMPARE(atomic2.loadRelaxed(), value);
    QAtomicInt atomic3 = atomic1;
    QCOMPARE(atomic3.loadRelaxed(), value);
    QAtomicInt atomic4(atomic2);
    QCOMPARE(atomic4.loadRelaxed(), value);
    QAtomicInt atomic5 = atomic2;
    QCOMPARE(atomic5.loadRelaxed(), value);
}

void tst_QAtomicInt::assignment_operator_data()
{
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<int>("newval");

    BOBUIest::newRow("value0") <<  0 <<  1;
    BOBUIest::newRow("value1") <<  1 <<  0;
    BOBUIest::newRow("value2") <<  0 << -1;
    BOBUIest::newRow("value3") << -1 <<  0;
    BOBUIest::newRow("value4") << -1 <<  1;
    BOBUIest::newRow("value5") <<  1 << -1;
}

void tst_QAtomicInt::assignment_operator()
{
    QFETCH(int, value);
    QFETCH(int, newval);

    {
        QAtomicInt atomic1 = value;
        atomic1 = newval;
        QCOMPARE(atomic1.loadRelaxed(), newval);
        atomic1 = value;
        QCOMPARE(atomic1.loadRelaxed(), value);

        QAtomicInt atomic2 = newval;
        atomic1 = atomic2;
        QCOMPARE(atomic1.loadRelaxed(), atomic2.loadRelaxed());
    }
}

void tst_QAtomicInt::isReferenceCountingNative()
{
#if defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isReferenceCountingNative());

#  if (defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicInt::isReferenceCountingNative() || !QAtomicInt::isReferenceCountingNative());

#  if (defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isReferenceCountingNative());

#  if (defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicInt::isReferenceCountingWaitFree()
{
#if defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isReferenceCountingWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicInt::isReferenceCountingNative());
#  if defined(Q_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isReferenceCountingWaitFree());
#endif
}

void tst_QAtomicInt::ref_data()
{
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<int>("result");
    BOBUIest::addColumn<int>("expected");

    BOBUIest::newRow("data0") <<  0 << 1 << 1;
    BOBUIest::newRow("data1") << -1 << 0 << 0;
    BOBUIest::newRow("data2") <<  1 << 1 << 2;
}

void tst_QAtomicInt::ref()
{
    QFETCH(int, value);
    QAtomicInt x = value;
    BOBUIEST(x.ref() ? 1 : 0, "result");
    BOBUIEST(x.loadRelaxed(), "expected");
}

void tst_QAtomicInt::deref_data()
{
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<int>("result");
    BOBUIest::addColumn<int>("expected");

    BOBUIest::newRow("data0") <<  0 << 1 << -1;
    BOBUIest::newRow("data1") <<  1 << 0 <<  0;
    BOBUIest::newRow("data2") <<  2 << 1 <<  1;
}

void tst_QAtomicInt::deref()
{
    QFETCH(int, value);
    QAtomicInt x = value;
    BOBUIEST(x.deref() ? 1 : 0, "result");
    BOBUIEST(x.loadRelaxed(), "expected");
}

void tst_QAtomicInt::isTestAndSetNative()
{
#if defined(Q_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isTestAndSetNative());

#  if (defined(Q_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicInt::isTestAndSetNative() || !QAtomicInt::isTestAndSetNative());

#  if (defined(Q_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isTestAndSetNative());

#  if (defined(Q_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicInt::isTestAndSetWaitFree()
{
#if defined(Q_ATOMIC_INT_TEST_AND_SET_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isTestAndSetWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicInt::isTestAndSetNative());
#  if defined(Q_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isTestAndSetWaitFree());
#endif
}

void tst_QAtomicInt::testAndSet_data()
{
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<int>("expected");
    BOBUIest::addColumn<int>("newval");
    BOBUIest::addColumn<bool>("result");

    // these should succeed
    BOBUIest::newRow("success0") <<         0 <<         0 <<         0 << true;
    BOBUIest::newRow("success1") <<         0 <<         0 <<         1 << true;
    BOBUIest::newRow("success2") <<         0 <<         0 <<        -1 << true;
    BOBUIest::newRow("success3") <<         1 <<         1 <<         0 << true;
    BOBUIest::newRow("success4") <<         1 <<         1 <<         1 << true;
    BOBUIest::newRow("success5") <<         1 <<         1 <<        -1 << true;
    BOBUIest::newRow("success6") <<        -1 <<        -1 <<         0 << true;
    BOBUIest::newRow("success7") <<        -1 <<        -1 <<         1 << true;
    BOBUIest::newRow("success8") <<        -1 <<        -1 <<        -1 << true;
    BOBUIest::newRow("success9") << INT_MIN+1 << INT_MIN+1 << INT_MIN+1 << true;
    BOBUIest::newRow("successA") << INT_MIN+1 << INT_MIN+1 <<         1 << true;
    BOBUIest::newRow("successB") << INT_MIN+1 << INT_MIN+1 <<        -1 << true;
    BOBUIest::newRow("successC") << INT_MAX   << INT_MAX   << INT_MAX   << true;
    BOBUIest::newRow("successD") << INT_MAX   << INT_MAX   <<         1 << true;
    BOBUIest::newRow("successE") << INT_MAX   << INT_MAX   <<        -1 << true;

    // these should fail
    BOBUIest::newRow("failure0") <<       0   <<       1   <<        ~0 << false;
    BOBUIest::newRow("failure1") <<       0   <<      -1   <<        ~0 << false;
    BOBUIest::newRow("failure2") <<       1   <<       0   <<        ~0 << false;
    BOBUIest::newRow("failure3") <<      -1   <<       0   <<        ~0 << false;
    BOBUIest::newRow("failure4") <<       1   <<      -1   <<        ~0 << false;
    BOBUIest::newRow("failure5") <<      -1   <<       1   <<        ~0 << false;
    BOBUIest::newRow("failure6") << INT_MIN+1 << INT_MAX   <<        ~0 << false;
    BOBUIest::newRow("failure7") << INT_MAX   << INT_MIN+1 <<        ~0 << false;
}

void tst_QAtomicInt::testAndSet()
{
    QFETCH(int, value);
    QFETCH(int, expected);
    QFETCH(int, newval);

    {
        QAtomicInt atomic = value;
        BOBUIEST(atomic.testAndSetRelaxed(expected, newval), "result");
    }

    {
        QAtomicInt atomic = value;
        BOBUIEST(atomic.testAndSetAcquire(expected, newval), "result");
    }

    {
        QAtomicInt atomic = value;
        BOBUIEST(atomic.testAndSetRelease(expected, newval), "result");
    }

    {
        QAtomicInt atomic = value;
        BOBUIEST(atomic.testAndSetOrdered(expected, newval), "result");
    }

    QFETCH(bool, result);
    // the new implementation has the version that loads the current value

    {
        QAtomicInt atomic = value;
        int currentval = 0xdeadbeef;
        QCOMPARE(atomic.testAndSetRelaxed(expected, newval, currentval), result);
        if (!result)
            QCOMPARE(currentval, value);
    }

    {
        QAtomicInt atomic = value;
        int currentval = 0xdeadbeef;
        QCOMPARE(atomic.testAndSetAcquire(expected, newval, currentval), result);
        if (!result)
            QCOMPARE(currentval, value);
    }

    {
        QAtomicInt atomic = value;
        int currentval = 0xdeadbeef;
        QCOMPARE(atomic.testAndSetRelease(expected, newval, currentval), result);
        if (!result)
            QCOMPARE(currentval, value);
    }

    {
        QAtomicInt atomic = value;
        int currentval = 0xdeadbeef;
        QCOMPARE(atomic.testAndSetOrdered(expected, newval, currentval), result);
        if (!result)
            QCOMPARE(currentval, value);
    }
}

void tst_QAtomicInt::isFetchAndStoreNative()
{
#if defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isFetchAndStoreNative());

#  if (defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicInt::isFetchAndStoreNative() || !QAtomicInt::isFetchAndStoreNative());

#  if (defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isFetchAndStoreNative());

#  if (defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicInt::isFetchAndStoreWaitFree()
{
#if defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isFetchAndStoreWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicInt::isFetchAndStoreNative());
#  if defined(Q_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isFetchAndStoreWaitFree());
#endif
}

void tst_QAtomicInt::fetchAndStore_data()
{
    BOBUIest::addColumn<int>("value");
    BOBUIest::addColumn<int>("newval");

    BOBUIest::newRow("data0") << 0 << 1;
    BOBUIest::newRow("data1") << 1 << 2;
    BOBUIest::newRow("data2") << 3 << 8;
}

void tst_QAtomicInt::fetchAndStore()
{
    QFETCH(int, value);
    QFETCH(int, newval);

    {
        QAtomicInt atomic = value;
        QCOMPARE(atomic.fetchAndStoreRelaxed(newval), value);
        QCOMPARE(atomic.loadRelaxed(), newval);
    }

    {
        QAtomicInt atomic = value;
        QCOMPARE(atomic.fetchAndStoreAcquire(newval), value);
        QCOMPARE(atomic.loadRelaxed(), newval);
    }

    {
        QAtomicInt atomic = value;
        QCOMPARE(atomic.fetchAndStoreRelease(newval), value);
        QCOMPARE(atomic.loadRelaxed(), newval);
    }

    {
        QAtomicInt atomic = value;
        QCOMPARE(atomic.fetchAndStoreOrdered(newval), value);
        QCOMPARE(atomic.loadRelaxed(), newval);
    }
}

void tst_QAtomicInt::isFetchAndAddNative()
{
#if defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isFetchAndAddNative());

#  if (defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)     \
       || defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)
    // could be either, just want to make sure the function is implemented
    QVERIFY(QAtomicInt::isFetchAndAddNative() || !QAtomicInt::isFetchAndAddNative());

#  if (defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE)
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isFetchAndAddNative());

#  if (defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE) \
       || defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE))
#    error "Define only one of Q_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "Q_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

void tst_QAtomicInt::isFetchAndAddWaitFree()
{
#if defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_WAIT_FREE)
    // the runtime test should say the same thing
    QVERIFY(QAtomicInt::isFetchAndAddWaitFree());

    // enforce some invariants
    QVERIFY(QAtomicInt::isFetchAndAddNative());
#  if defined(Q_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
    // the runtime test should say the same thing
    QVERIFY(!QAtomicInt::isFetchAndAddWaitFree());
#endif
}

void tst_QAtomicInt::fetchAndAdd_data()
{
    BOBUIest::addColumn<int>("value1");
    BOBUIest::addColumn<int>("value2");

    BOBUIest::newRow("0+1") << 0 << 1;
    BOBUIest::newRow("1+0") << 1 << 0;
    BOBUIest::newRow("1+2") << 1 << 2;
    BOBUIest::newRow("2+1") << 2 << 1;
    BOBUIest::newRow("10+21") << 10 << 21;
    BOBUIest::newRow("31+40") << 31 << 40;
    BOBUIest::newRow("51+62") << 51 << 62;
    BOBUIest::newRow("72+81") << 72 << 81;
    BOBUIest::newRow("810+721") << 810 << 721;
    BOBUIest::newRow("631+540") << 631 << 540;
    BOBUIest::newRow("451+362") << 451 << 362;
    BOBUIest::newRow("272+181") << 272 << 181;
    BOBUIest::newRow("1810+8721") << 1810 << 8721;
    BOBUIest::newRow("3631+6540") << 3631 << 6540;
    BOBUIest::newRow("5451+4362") << 5451 << 4362;
    BOBUIest::newRow("7272+2181") << 7272 << 2181;

    BOBUIest::newRow("0+-1") << 0 << -1;
    BOBUIest::newRow("1+-2") << 1 << -2;
    BOBUIest::newRow("2+-1") << 2 << -1;
    BOBUIest::newRow("10+-21") << 10 << -21;
    BOBUIest::newRow("31+-40") << 31 << -40;
    BOBUIest::newRow("51+-62") << 51 << -62;
    BOBUIest::newRow("72+-81") << 72 << -81;
    BOBUIest::newRow("810+-721") << 810 << -721;
    BOBUIest::newRow("631+-540") << 631 << -540;
    BOBUIest::newRow("451+-362") << 451 << -362;
    BOBUIest::newRow("272+-181") << 272 << -181;
    BOBUIest::newRow("1810+-8721") << 1810 << -8721;
    BOBUIest::newRow("3631+-6540") << 3631 << -6540;
    BOBUIest::newRow("5451+-4362") << 5451 << -4362;
    BOBUIest::newRow("7272+-2181") << 7272 << -2181;

    BOBUIest::newRow("-1+0") << -1 << 0;
    BOBUIest::newRow("-1+2") << -1 << 2;
    BOBUIest::newRow("-2+1") << -2 << 1;
    BOBUIest::newRow("-10+21") << -10 << 21;
    BOBUIest::newRow("-31+40") << -31 << 40;
    BOBUIest::newRow("-51+62") << -51 << 62;
    BOBUIest::newRow("-72+81") << -72 << 81;
    BOBUIest::newRow("-810+721") << -810 << 721;
    BOBUIest::newRow("-631+540") << -631 << 540;
    BOBUIest::newRow("-451+362") << -451 << 362;
    BOBUIest::newRow("-272+181") << -272 << 181;
    BOBUIest::newRow("-1810+8721") << -1810 << 8721;
    BOBUIest::newRow("-3631+6540") << -3631 << 6540;
    BOBUIest::newRow("-5451+4362") << -5451 << 4362;
    BOBUIest::newRow("-7272+2181") << -7272 << 2181;
}

void tst_QAtomicInt::fetchAndAdd()
{
    QFETCH(int, value1);
    QFETCH(int, value2);
    int result;

    {
        QAtomicInt atomic = value1;
        result = atomic.fetchAndAddRelaxed(value2);
        QCOMPARE(result, value1);
        QCOMPARE(atomic.loadRelaxed(), value1 + value2);
    }

    {
        QAtomicInt atomic = value1;
        result = atomic.fetchAndAddAcquire(value2);
        QCOMPARE(result, value1);
        QCOMPARE(atomic.loadRelaxed(), value1 + value2);
    }

    {
        QAtomicInt atomic = value1;
        result = atomic.fetchAndAddRelease(value2);
        QCOMPARE(result, value1);
        QCOMPARE(atomic.loadRelaxed(), value1 + value2);
    }

    {
        QAtomicInt atomic = value1;
        result = atomic.fetchAndAddOrdered(value2);
        QCOMPARE(result, value1);
        QCOMPARE(atomic.loadRelaxed(), value1 + value2);
    }
}

void tst_QAtomicInt::operators()
{
    {
        // Test that QBasicAtomicInt also has operator= and cast operators
        // We've been using them for QAtomicInt elsewhere
        QBasicAtomicInt atomic = Q_BASIC_ATOMIC_INITIALIZER(0);
        atomic = 1;
        QCOMPARE(int(atomic), 1);
    }

    QAtomicInt atomic = 0;
    int x = ++atomic;
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 1);

    x = atomic++;
    QCOMPARE(int(atomic), x + 1);
    QCOMPARE(int(atomic), 2);

    x = atomic--;
    QCOMPARE(int(atomic), x - 1);
    QCOMPARE(int(atomic), 1);

    x = --atomic;
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 0);

    x = (atomic += 1);
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 1);

    x = (atomic -= 1);
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 0);

    x = (atomic |= 0xf);
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 0xf);

    x = (atomic &= 0x17);
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 7);

    x = (atomic ^= 0x14);
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 0x13);

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_CLANG("-Wself-assign-overloaded")
    x = (atomic ^= atomic);
    QCOMPARE(int(atomic), x);
    QCOMPARE(int(atomic), 0);
BOBUI_WARNING_POP
}

void tst_QAtomicInt::testAndSet_loop()
{
    QElapsedTimer stopWatch;
    stopWatch.start();

    int iterations = 10000000;

    QAtomicInt val=0;
    for (int i = 0; i < iterations; ++i) {
        int v = val.loadRelaxed();
        QVERIFY(val.testAndSetRelaxed(v, v+1));
        if ((i % 1000) == 999) {
            if (stopWatch.elapsed() > 60 * 1000) {
                // This test shouldn't run for more than two minutes.
                qDebug("Interrupted test after %d iterations (%.2f iterations/sec)",
                       i, (i * 1000.0) / double(stopWatch.elapsed()));
                break;
            }
        }
    }
}

void tst_QAtomicInt::fetchAndAdd_loop()
{
    int iterations = 10000000;
#if defined (Q_OS_HPUX)
    iterations = 1000000;
#endif

    QAtomicInt val=0;
    for (int i = 0; i < iterations; ++i) {
        const int prev = val.fetchAndAddRelaxed(1);
        QCOMPARE(prev, val.loadRelaxed() -1);
    }
}

class FetchAndAddThread : public BOBUIhread
{
public:
    void run() override
    {

        for (int i = 0; i < iterations; ++i)
            val->fetchAndAddAcquire(1);

        for (int i = 0; i < iterations; ++i)
            val->fetchAndAddAcquire(-1);

    }
QAtomicInt *val;
int iterations;
};


void tst_QAtomicInt::fetchAndAdd_threadedLoop()
{
    QAtomicInt val;
    FetchAndAddThread t1;
    t1.val = &val;
    t1.iterations = 1000000;

    FetchAndAddThread t2;
    t2.val = &val;
    t2.iterations = 2000000;

    t1.start();
    t2.start();
    t1.wait();
    t2.wait();

    QCOMPARE(val.loadRelaxed(), 0);
}

BOBUIEST_MAIN(tst_QAtomicInt)
#include "tst_qatomicint.moc"
