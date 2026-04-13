// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2020 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEST_H
#define BOBUIEST_H

#if 0
#pragma bobui_class(BOBUIest)
#endif

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiestcase.h>
#include <BobUITest/bobuiestdata.h>
#include <BobUITest/bobuiesttostring.h>
#include <BobUITest/qbenchmark.h>

#if defined(TESTCASE_LOWDPI)
#include <BobUICore/qcoreapplication.h>
#endif

#include <cstdio>
#include <initializer_list>
#include <memory>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest
{

template<>
inline bool qCompare(QString const &t1, QLatin1StringView const &t2, const char *actual,
                     const char *expected, const char *file, int line)
{
    return qCompare(t1, QString(t2), actual, expected, file, line);
}
template<>
inline bool qCompare(QLatin1StringView const &t1, QString const &t2, const char *actual,
                     const char *expected, const char *file, int line)
{
    return qCompare(QString(t1), t2, actual, expected, file, line);
}

// Compare sequences of equal size
template <typename ActualIterator, typename ExpectedIterator>
bool _q_compareSequence(ActualIterator actualIt, ActualIterator actualEnd,
                        ExpectedIterator expectedBegin, ExpectedIterator expectedEnd,
                        const char *actual, const char *expected,
                        const char *file, int line)
{
    char msg[1024];
    msg[0] = '\0';

    const qsizetype actualSize = actualEnd - actualIt;
    const qsizetype expectedSize = expectedEnd - expectedBegin;
    bool isOk = actualSize == expectedSize;

    if (!isOk) {
        std::snprintf(msg, sizeof(msg), "Compared lists have different sizes.\n"
                      "   Actual   (%s) size: %lld\n"
                      "   Expected (%s) size: %lld",
                      actual, qlonglong(actualSize),
                      expected, qlonglong(expectedSize));
    }

    for (auto expectedIt = expectedBegin; isOk && expectedIt < expectedEnd; ++actualIt, ++expectedIt) {
        if (!(*actualIt == *expectedIt)) {
            const qsizetype i = qsizetype(expectedIt - expectedBegin);
            char *val1 = toString(*actualIt);
            char *val2 = toString(*expectedIt);

            std::snprintf(msg, sizeof(msg), "Compared lists differ at index %lld.\n"
                          "   Actual   (%s): %s\n"
                          "   Expected (%s): %s",
                          qlonglong(i), actual, val1 ? val1 : "<null>",
                          expected, val2 ? val2 : "<null>");
            isOk = false;

            delete [] val1;
            delete [] val2;
        }
    }
    return compare_helper(isOk, msg, actual, expected, file, line);
}

namespace Internal {

#if defined(TESTCASE_LOWDPI)
void disableHighDpi()
{
    qputenv("BOBUI_ENABLE_HIGHDPI_SCALING", "0");
}
Q_CONSTRUCTOR_FUNCTION(disableHighDpi);
#endif

} // namespace Internal

template <typename T>
inline bool qCompare(QList<T> const &t1, QList<T> const &t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return _q_compareSequence(t1.cbegin(), t1.cend(), t2.cbegin(), t2.cend(),
                                     actual, expected, file, line);
}

template <typename T, int N>
bool qCompare(QList<T> const &t1, std::initializer_list<T> t2,
              const char *actual, const char *expected,
              const char *file, int line)
{
    return _q_compareSequence(t1.cbegin(), t1.cend(), t2.cbegin(), t2.cend(),
                                     actual, expected, file, line);
}

// Compare QList against array
template <typename T, int N>
bool qCompare(QList<T> const &t1, const T (& t2)[N],
              const char *actual, const char *expected,
              const char *file, int line)
{
    return _q_compareSequence(t1.cbegin(), t1.cend(), t2, t2 + N,
                                     actual, expected, file, line);
}

template <typename T>
inline bool qCompare(QFlags<T> const &t1, T const &t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    using Int = typename QFlags<T>::Int;
    return qCompare(Int(t1), Int(t2), actual, expected, file, line);
}

template <typename T>
inline bool qCompare(QFlags<T> const &t1, int const &t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    using Int = typename QFlags<T>::Int;
    return qCompare(Int(t1), Int(t2), actual, expected, file, line);
}

template<>
inline bool qCompare(qint64 const &t1, qint32 const &t2, const char *actual,
                    const char *expected, const char *file, int line)
{
    return qCompare(t1, static_cast<qint64>(t2), actual, expected, file, line);
}

template<>
inline bool qCompare(qint64 const &t1, quint32 const &t2, const char *actual,
                    const char *expected, const char *file, int line)
{
    return qCompare(t1, static_cast<qint64>(t2), actual, expected, file, line);
}

template<>
inline bool qCompare(quint64 const &t1, quint32 const &t2, const char *actual,
                    const char *expected, const char *file, int line)
{
    return qCompare(t1, static_cast<quint64>(t2), actual, expected, file, line);
}

template<>
inline bool qCompare(qint32 const &t1, qint64 const &t2, const char *actual,
                    const char *expected, const char *file, int line)
{
    return qCompare(static_cast<qint64>(t1), t2, actual, expected, file, line);
}

template<>
inline bool qCompare(quint32 const &t1, qint64 const &t2, const char *actual,
                    const char *expected, const char *file, int line)
{
    return qCompare(static_cast<qint64>(t1), t2, actual, expected, file, line);
}

template<>
inline bool qCompare(quint32 const &t1, quint64 const &t2, const char *actual,
                    const char *expected, const char *file, int line)
{
    return qCompare(static_cast<quint64>(t1), t2, actual, expected, file, line);
}
namespace Internal {

template <typename T>
using InitMainTest = decltype(&T::initMain);

template <typename T>
constexpr inline bool hasInitMain = qxp::is_detected_v<InitMainTest, T>;

template<typename T>
void callInitMain()
{
    if constexpr (hasInitMain<T>)
        T::initMain();
}

} // namespace Internal

} // namespace BOBUIest
BOBUI_END_NAMESPACE

#ifdef BOBUI_TESTCASE_BUILDDIR
#  define BOBUIEST_SET_MAIN_SOURCE_PATH  BOBUIest::setMainSourcePath(__FILE__, BOBUI_TESTCASE_BUILDDIR);
#else
#  define BOBUIEST_SET_MAIN_SOURCE_PATH  BOBUIest::setMainSourcePath(__FILE__);
#endif

// Hooks for coverage-testing of BOBUIestLib itself:
#if BOBUI_CONFIG(testlib_selfcover) && defined(__COVERAGESCANNER__)
struct BobUICoverageScanner
{
    BobUICoverageScanner(const char *name)
    {
        __coveragescanner_clear();
        __coveragescanner_testname(name);
    }
    ~BobUICoverageScanner()
    {
        __coveragescanner_save();
        __coveragescanner_testname("");
    }
};
#define TESTLIB_SELFCOVERAGE_START(name) BobUICoverageScanner _bobuiCoverageScanner(name);
#else
#define TESTLIB_SELFCOVERAGE_START(name)
#endif

#if !defined(BOBUIEST_BATCH_TESTS)
// Internal (but used by some testlib selftests to hack argc and argv).
// Tests should normally implement initMain() if they have set-up to do before
// instantiating the test class.
#define BOBUIEST_MAIN_WRAPPER(TestObject, ...) \
int main(int argc, char *argv[]) \
{ \
    TESTLIB_SELFCOVERAGE_START(#TestObject) \
    BOBUI_PREPEND_NAMESPACE(BOBUIest::Internal::callInitMain)<TestObject>(); \
    __VA_ARGS__ \
    TestObject tc; \
    BOBUIEST_SET_MAIN_SOURCE_PATH \
    return BOBUIest::qExec(&tc, argc, argv); \
}
#else
// BATCHED_TEST_NAME is defined for each test in a batch in cmake. Some odd
// targets, like snippets, don't define it though. Play safe by providing a
// default value.
#if !defined(BATCHED_TEST_NAME)
#define BATCHED_TEST_NAME "other"
#endif
#define BOBUIEST_MAIN_WRAPPER(TestObject, ...) \
\
void qRegister##TestObject() \
{ \
    auto runTest = [](int argc, char** argv) -> int { \
        TESTLIB_SELFCOVERAGE_START(TestObject) \
        BOBUI_PREPEND_NAMESPACE(BOBUIest::Internal::callInitMain)<TestObject>(); \
        __VA_ARGS__ \
        TestObject tc; \
        BOBUIEST_SET_MAIN_SOURCE_PATH \
        return BOBUIest::qExec(&tc, argc, argv); \
    }; \
    BOBUIest::qRegisterTestCase(QStringLiteral(BATCHED_TEST_NAME), runTest); \
} \
\
Q_CONSTRUCTOR_FUNCTION(qRegister##TestObject)
#endif

// For when you don't even want a QApplication:
#define BOBUIEST_APPLESS_MAIN(TestObject) BOBUIEST_MAIN_WRAPPER(TestObject)

#include <BobUITest/bobuiestsystem.h>

#if defined(BOBUI_NETWORK_LIB)
#  include <BobUITest/bobuiest_network.h>
#endif

// Internal
#define BOBUIEST_QAPP_SETUP(klaz) \
    klaz app(argc, argv); \
    app.setAttribute(BobUI::AA_Use96Dpi, true);

#if defined(BOBUI_WIDGETS_LIB)
#  include <BobUITest/bobuiest_widgets.h>
#  ifdef BOBUI_KEYPAD_NAVIGATION
#    define BOBUIEST_DISABLE_KEYPAD_NAVIGATION QApplication::setNavigationMode(BobUI::NavigationModeNone);
#  else
#    define BOBUIEST_DISABLE_KEYPAD_NAVIGATION
#  endif
// Internal
#  define BOBUIEST_MAIN_SETUP() BOBUIEST_QAPP_SETUP(QApplication) BOBUIEST_DISABLE_KEYPAD_NAVIGATION
#elif defined(BOBUI_GUI_LIB)
#  include <BobUITest/bobuiest_gui.h>
// Internal
#  define BOBUIEST_MAIN_SETUP() BOBUIEST_QAPP_SETUP(QGuiApplication)
#else
// Internal
#  define BOBUIEST_MAIN_SETUP() BOBUIEST_QAPP_SETUP(QCoreApplication)
#endif // BOBUI_GUI_LIB

// For most tests:
#define BOBUIEST_MAIN(TestObject) BOBUIEST_MAIN_WRAPPER(TestObject, BOBUIEST_MAIN_SETUP())

// For command-line tests
#define BOBUIEST_GUILESS_MAIN(TestObject) \
    BOBUIEST_MAIN_WRAPPER(TestObject, BOBUIEST_QAPP_SETUP(QCoreApplication))

#endif
