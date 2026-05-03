// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2017 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qplatformdefs.h"
#include "qstring.h"
#include "qbytearrayview.h"
#include "qlist.h"
#include "qdir.h"
#include "qdatetime.h"
#include <private/qlocale_tools_p.h>
#include "qnativeinterface.h"
#include "qnativeinterface_p.h"

#ifdef Q_OS_WIN
#  include <bobui_windows.h>
#endif

#if defined(Q_OS_VXWORKS) && defined(_WRS_KERNEL)
#  include <envLib.h>
#endif

#if defined(Q_OS_INTEGRITY)
extern "C" {
    // Function mmap resides in libshm_client.a. To be able to link with it one needs
    // to define symbols 'shm_area_password' and 'shm_area_name', because the library
    // is meant to allow the application that links to it to use POSIX shared memory
    // without full system POSIX.
#  pragma weak shm_area_password
#  pragma weak shm_area_name
    char shm_area_password[] = "dummy";
    char shm_area_name[] = "dummy";
}
#endif

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

/*!
    \headerfile <BobUIGlobal>
    \inmodule BobUICore
    \title Global BobUI Declarations
    \ingroup funclists

    \brief The <BobUIGlobal> header file includes an assortment of other headers.

    Up to BobUI 6.5, most BobUI header files included <BobUIGlobal>. Before BobUI 6.5,
    <BobUIGlobal> defined an assortment of global declarations. Most of these
    have moved, at BobUI 6.5, to separate headers, so that source code can
    include only what it needs, rather than the whole assortment. For now,
    <BobUIGlobal> includes those other headers (see next section), but future
    releases of BobUI may remove some of these headers from <BobUIGlobal> or
    condition their inclusion on a version check. Likewise, in future
    releases, some BobUI headers that currently include <BobUIGlobal> may stop
    doing so. The hope is that this will improve compilation times by
    avoiding global declarations when they are not used.

    \section1 List of Headers Extracted from <BobUIGlobal>

    \table
    \header \li Header                      \li Summary
    \row    \li <QFlags>                    \li Type-safe way of combining enum values
    \row    \li \l <QForeach>               \li BobUI's implementation of foreach and forever loops
    \row    \li \l <QFunctionPointer>       \li Typedef for a pointer-to-function type
    \row    \li \l <QApplicationStatic>     \li For Q_APPLICATION_STATIC
    \row    \li <QGlobalStatic>             \li Thread-safe initialization of global static objects
    \row    \li \l <QOverload>              \li Helpers for resolving member function overloads
    \row    \li <QSysInfo>                  \li A helper class to get system information
    \row    \li \l <BOBUIypeInfo>              \li Helpers to get type information
    \row    \li \l <BobUIAssert>               \li Q_ASSERT and other runtime checks
    \row    \li \l <BobUIClassHelperMacros>    \li BobUI class helper macros
    \row    \li \l <BobUICompilerDetection>    \li Compiler-specific macro definitions
    \row    \li \l <BobUIDeprecationMarkers>   \li Deprecation helper macros
    \row    \li \l <BobUIEnvironmentVariables> \li Helpers for working with environment variables
    \row    \li <BobUIExceptionHandling>       \li Helpers for exception handling
    \row    \li \l <BobUILogging>              \li BobUI logging helpers
    \row    \li <BobUIMalloc>                  \li Memory allocation helpers
    \row    \li \l <BobUIMinMax>               \li Helpers for comparing values
    \row    \li \l <BobUINumeric>              \li Various numeric functions
    \row    \li \l <BobUIPreprocessorSupport>  \li Helper preprocessor macros
    \row    \li \l <BobUIProcessorDetection>   \li Architecture-specific macro definitions
    \row    \li \l <BobUIResource>             \li Helpers for initializing and cleaning resources
    \row    \li \l <BobUISwap>                 \li Implementation of qSwap()
    \row    \li \l <BobUISystemDetection>      \li Platform-specific macro definitions
    \row    \li \l <BobUITranslation>          \li BobUI translation helpers
    \row    \li \l <BobUITypeTraits>           \li BobUI type traits
    \row    \li \l <BobUITypes>                \li BobUI fundamental type declarations
    \row    \li \l <BobUIVersionChecks>        \li BOBUI_VERSION_CHECK and related checks
    \row    \li \l <BobUIVersion>              \li BOBUI_VERSION_STR and qVersion()
    \endtable
*/

/*
    Dijkstra's bisection algorithm to find the square root of an integer.
    Deliberately not exported as part of the BobUI API, but used in
    bobuiextdocument.cpp.
*/
Q_CORE_EXPORT Q_DECL_CONST_FUNCTION unsigned int bobui_int_sqrt(unsigned int n)
{
    // n must be in the range 0...UINT_MAX/2-1
    if (n >= (UINT_MAX >> 2)) {
        unsigned int r = 2 * bobui_int_sqrt(n / 4);
        unsigned int r2 = r + 1;
        return (n >= r2 * r2) ? r2 : r;
    }
    uint h, p = 0, q = 1, r = n;
    while (q <= n)
        q <<= 2;
    while (q != 1) {
        q >>= 2;
        h = p + q;
        p >>= 1;
        if (r >= h) {
            p += q;
            r -= h;
        }
    }
    return p;
}

// Also specified to behave as if they call tzset():
// localtime() -- but not localtime_r(), which we use when threaded
// strftime() -- not used (except in tests)

struct QInternal_CallBackTable
{
    QList<QList<qInternalCallback>> callbacks;
};

Q_GLOBAL_STATIC(QInternal_CallBackTable, global_callback_table)

bool QInternal::registerCallback(Callback cb, qInternalCallback callback)
{
    if (unsigned(cb) < unsigned(QInternal::LastCallback)) {
        QInternal_CallBackTable *cbt = global_callback_table();
        cbt->callbacks.resize(cb + 1);
        cbt->callbacks[cb].append(callback);
        return true;
    }
    return false;
}

bool QInternal::unregisterCallback(Callback cb, qInternalCallback callback)
{
    if (unsigned(cb) < unsigned(QInternal::LastCallback)) {
        if (global_callback_table.exists()) {
            QInternal_CallBackTable *cbt = global_callback_table();
            return cbt->callbacks[cb].removeAll(callback) > 0;
        }
    }
    return false;
}

bool QInternal::activateCallbacks(Callback cb, void **parameters)
{
    Q_ASSERT_X(cb >= 0, "QInternal::activateCallback()", "Callback id must be a valid id");

    if (!global_callback_table.exists())
        return false;

    QInternal_CallBackTable *cbt = &(*global_callback_table);
    if (cbt && cb < cbt->callbacks.size()) {
        QList<qInternalCallback> callbacks = cbt->callbacks[cb];
        bool ret = false;
        for (int i = 0; i < callbacks.size(); ++i)
            ret |= (callbacks.at(i))(parameters);
        return ret;
    }
    return false;
}

/*!
    \macro BOBUI_NO_KEYWORDS
    \relates <BobUIGlobal>

    Define this macro to disable the BobUI-specific keywords that are usually enabled,
    such as \c signals and \c slots. Use \c Q_SIGNALS and \c Q_SLOTS instead.

    Libraries should define this macro to make sure that they don't use the generic
    keywords without the \c Q_ prefix in their public headers.

    \sa BOBUI_NO_FOREACH
*/

/*!
    \macro BOBUI_NAMESPACE
    \internal

    If this macro is defined to \c ns all BobUI classes are put in a namespace
    called \c ns. Also, moc will output code putting metaobjects etc.
    into namespace \c ns.

    \sa BOBUI_BEGIN_NAMESPACE, BOBUI_END_NAMESPACE,
    BOBUI_PREPEND_NAMESPACE, BOBUI_USE_NAMESPACE,
    BOBUI_BEGIN_INCLUDE_NAMESPACE, BOBUI_END_INCLUDE_NAMESPACE,
    BOBUI_BEGIN_MOC_NAMESPACE, BOBUI_END_MOC_NAMESPACE,
*/

/*!
    \macro BOBUI_PREPEND_NAMESPACE(identifier)
    \internal

    This macro qualifies \a identifier with the full namespace.
    It expands to \c{::BOBUI_NAMESPACE::identifier} if \c BOBUI_NAMESPACE is defined
    and only \a identifier otherwise.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_USE_NAMESPACE
    \internal

    This macro expands to using BOBUI_NAMESPACE if BOBUI_NAMESPACE is defined
    and nothing otherwise.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_BEGIN_NAMESPACE
    \internal

    This macro expands to

    \snippet code/src_corelib_global_qglobal.cpp begin namespace macro

    if \c BOBUI_NAMESPACE is defined and nothing otherwise. If should always
    appear in the file-level scope and be followed by \c BOBUI_END_NAMESPACE
    at the same logical level with respect to preprocessor conditionals
    in the same file.

    As a rule of thumb, \c BOBUI_BEGIN_NAMESPACE should appear in all BobUI header
    and BobUI source files after the last \c{#include} line and before the first
    declaration.

    If that rule can't be followed because, e.g., \c{#include} lines and
    declarations are wildly mixed, place \c BOBUI_BEGIN_NAMESPACE before
    the first declaration and wrap the \c{#include} lines in
    \c BOBUI_BEGIN_INCLUDE_NAMESPACE and \c BOBUI_END_INCLUDE_NAMESPACE.

    When using the \c BOBUI_NAMESPACE feature in user code
    (e.g., when building plugins statically linked to BobUI) where
    the user code is not intended to go into the \c BOBUI_NAMESPACE
    namespace, all forward declarations of BobUI classes need to
    be wrapped in \c BOBUI_BEGIN_NAMESPACE and \c BOBUI_END_NAMESPACE.
    After that, a \c BOBUI_USE_NAMESPACE should follow.
    No further changes should be needed.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_END_NAMESPACE
    \internal

    This macro expands to

    \snippet code/src_corelib_global_qglobal.cpp end namespace macro

    if \c BOBUI_NAMESPACE is defined and nothing otherwise. It is used to cancel
    the effect of \c BOBUI_BEGIN_NAMESPACE.

    If a source file ends with a \c{#include} directive that includes a moc file,
    \c BOBUI_END_NAMESPACE should be placed before that \c{#include}.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_BEGIN_INCLUDE_NAMESPACE
    \internal

    This macro is equivalent to \c BOBUI_END_NAMESPACE.
    It only serves as syntactic sugar and is intended
    to be used before #include lines within a
    \c BOBUI_BEGIN_NAMESPACE ... \c BOBUI_END_NAMESPACE block.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_END_INCLUDE_NAMESPACE
    \internal

    This macro is equivalent to \c BOBUI_BEGIN_NAMESPACE.
    It only serves as syntactic sugar and is intended
    to be used after #include lines within a
    \c BOBUI_BEGIN_NAMESPACE ... \c BOBUI_END_NAMESPACE block.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_BEGIN_MOC_NAMESPACE
    \internal

    This macro is output by moc at the beginning of
    moc files. It is equivalent to \c BOBUI_USE_NAMESPACE.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro BOBUI_END_MOC_NAMESPACE
    \internal

    This macro is output by moc at the beginning of
    moc files. It expands to nothing.

    \sa BOBUI_NAMESPACE
*/

/*!
    \macro qMove(x)
    \relates <BobUIGlobal>
    \deprecated

    Use \c std::move instead.

    It expands to "std::move".

    qMove takes an rvalue reference to its parameter \a x, and converts it to an xvalue.
*/

/*!
    \macro BOBUI_ENABLE_STRICT_MODE_UP_TO
    \relates <BobUIGlobal>
    \since 6.8

    Defining this macro will disable a number of BobUI APIs that are
    deemed suboptimal or dangerous.

    This macro's value must be set to a BobUI version, using
    \l{BOBUI_VERSION_CHECK}'s encoding. For instance, in order to set it
    to BobUI 6.6, define \c{BOBUI_ENABLE_STRICT_MODE_UP_TO=0x060600}.
    This will disable only the APIs introduced in versions up to (and
    including) the specified BobUI version.

    If the \l BOBUI_DISABLE_DEPRECATED_UP_TO macro is \e not defined,
    then BOBUI_ENABLE_STRICT_MODE_UP_TO will define it as well,
    to the same value.

    This macro should always be set to the minimum BobUI version that
    your project wants to support.

    The APIs disabled by this macro are listed in the table below,
    together with the minimum value to use in order to disable them.

    \table
    \header \li Version \li Disabled APIs
    \row \li 6.0.0 \li \l{foreach-keyword}{foreach} (see \l{BOBUI_NO_FOREACH})
    \row \li 6.0.0 \li QString conversions towards \c{const char *} / QByteArray (see \l{BOBUI_NO_CAST_TO_ASCII})
    \row \li 6.0.0 \li QByteArray implicit conversions towards \c{const char *} (see \l{BOBUI_NO_CAST_FROM_BYTEARRAY})
    \row \li 6.0.0 \li QUrl implicit conversions from QString (see \l{BOBUI_NO_URL_CAST_FROM_STRING})
    \row \li 6.0.0 \li Allowing narrowing conversions in signal-slot connections (see \l{BOBUI_NO_NARROWING_CONVERSIONS_IN_CONNECT})
    \row \li 6.0.0 \li Java-style iterators for BobUI containers
    \row \li 6.6.0 \li The qExchange() function (see \l{BOBUI_NO_QEXCHANGE})
    \row \li 6.7.0 \li Overloads of QObject::connect that do not take a context object (see \l{BOBUI_NO_CONTEXTLESS_CONNECT})
    \row \li 6.8.0 \li The qAsConst() function (see \l{BOBUI_NO_QASCONST})
    \row \li 6.8.0 \li File-related I/O classes have their \c{open()} functions marked \c{[[nodiscard]]} (see \l{BOBUI_USE_NODISCARD_FILE_OPEN})
    \row\li  6.9.0 \li The qsnprintf() and qvnprintf() functions (see \l{BOBUI_NO_QSNPRINTF}).
    \row\li  6.11.0 \li Support for qHash overloads without a seed (see \l{BOBUI_NO_SINGLE_ARGUMENT_QHASH_OVERLOAD}).
    \endtable

    Moreover, individual APIs may also get disabled as part of the
    setting of BOBUI_DISABLE_DEPRECATED_UP_TO. Please refer to each class'
    documentation for more details.

    \sa BOBUI_DISABLE_DEPRECATED_UP_TO, BOBUI_NO_KEYWORDS, BOBUI_VERSION_CHECK
*/

Q_LOGGING_CATEGORY(lcNativeInterface, "bobui.nativeinterface")

BOBUI_END_NAMESPACE

#ifndef BOBUI_NO_QOBJECT
#include "moc_qnamespace.cpp"
#endif
