// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIDEPRECATIONMARKERS_H
#define BOBUIDEPRECATIONMARKERS_H

#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuiclasshelpermacros.h>
#include <BobUICore/bobuideprecationdefinitions.h>
#include <BobUICore/bobuiversionchecks.h>
#include <BobUICore/qcompilerdetection.h> // for Q_DECL_DEPRECATED

#if 0
#pragma bobui_class(BobUIDeprecationMarkers)
#pragma bobui_sync_stop_processing
#endif

BOBUI_BEGIN_NAMESPACE

#if defined(BOBUI_NO_DEPRECATED)
/* undef, so as to cause compile-errors when they're used outside #if BOBUI_DEPRECATED_SINCE blocks */
#  undef BOBUI_DEPRECATED
#  undef BOBUI_DEPRECATED_X
#  undef BOBUI_DEPRECATED_VARIABLE
#  undef BOBUI_DEPRECATED_CONSTRUCTOR
#elif !defined(BOBUI_NO_DEPRECATED_WARNINGS)
#  undef BOBUI_DEPRECATED
#  define BOBUI_DEPRECATED Q_DECL_DEPRECATED
#  undef BOBUI_DEPRECATED_X
#  define BOBUI_DEPRECATED_X(text) Q_DECL_DEPRECATED_X(text)
#  undef BOBUI_DEPRECATED_VARIABLE
#  define BOBUI_DEPRECATED_VARIABLE Q_DECL_VARIABLE_DEPRECATED
#  undef BOBUI_DEPRECATED_CONSTRUCTOR
#  define BOBUI_DEPRECATED_CONSTRUCTOR Q_DECL_CONSTRUCTOR_DEPRECATED explicit
#else
#  undef BOBUI_DEPRECATED
#  define BOBUI_DEPRECATED
#  undef BOBUI_DEPRECATED_X
#  define BOBUI_DEPRECATED_X(text)
#  undef BOBUI_DEPRECATED_VARIABLE
#  define BOBUI_DEPRECATED_VARIABLE
#  undef BOBUI_DEPRECATED_CONSTRUCTOR
#  define BOBUI_DEPRECATED_CONSTRUCTOR
#  undef Q_DECL_ENUMERATOR_DEPRECATED
#  define Q_DECL_ENUMERATOR_DEPRECATED
#  undef Q_DECL_ENUMERATOR_DEPRECATED_X
#  define Q_DECL_ENUMERATOR_DEPRECATED_X(ignored)
#endif

/*
    BOBUI_DEPRECATED_SINCE(major, minor) evaluates as true if the BobUI version is greater than
    the deprecation point specified.

    Use it to specify from which version of BobUI a function or class has been deprecated

    Example:
        #if BOBUI_DEPRECATED_SINCE(5,1)
            BOBUI_DEPRECATED void deprecatedFunction(); //function deprecated since BobUI 5.1
        #endif

*/
#ifdef BOBUI_DEPRECATED
#define BOBUI_DEPRECATED_SINCE(major, minor) (BOBUI_VERSION_CHECK(major, minor, 0) > BOBUI_DISABLE_DEPRECATED_UP_TO)
#else
#define BOBUI_DEPRECATED_SINCE(major, minor) 0
#endif

/*
    BOBUI_REMOVAL_BOBUI{VER}_DEPRECATED_SINCE(major, minor)

    The macro should be used if the API is deprecated and should be removed
    in the {VER} major release.

    The \a major and \a minor parameters specify the deprecation version.

    For now, we provide the macros to remove the deprecated APIs in BobUI 7
    and in BobUI 8.

    Example:

    \code
    #if BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 9)
        BOBUI_DEPRECATED_VERSION_X_6_9("The reason for the deprecation")
        void deprecatedFunc();
    #endif
    \endcode

    The \c {deprecatedFunc()} function is deprecated since BobUI 6.9, and will be
    completely removed in BobUI 7.0.
*/
#define BOBUI_DEPRECATED_TO_BE_REMOVED_HELPER(dep_major, dep_minor, rem_major) \
    (BOBUI_DEPRECATED_SINCE(dep_major, dep_minor) && (BOBUI_VERSION < BOBUI_VERSION_CHECK(rem_major, 0, 0)))

// For APIs that should be removed in BobUI 7
#define BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(major, minor) \
    BOBUI_DEPRECATED_TO_BE_REMOVED_HELPER(major, minor, 7)

// For APIs that should be removed in BobUI 8
#define BOBUI_REMOVAL_BOBUI8_DEPRECATED_SINCE(major, minor) \
    BOBUI_DEPRECATED_TO_BE_REMOVED_HELPER(major, minor, 8)

/*
  BOBUI_DEPRECATED_VERSION(major, minor) and BOBUI_DEPRECATED_VERSION_X(major, minor, text)
  outputs a deprecation warning if BOBUI_WARN_DEPRECATED_UP_TO is equal to or greater
  than the version specified as major, minor. This makes it possible to deprecate a
  function without annoying a user who needs to stay compatible with a specified minimum
  version and therefore can't use the new function.
*/
#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(5, 12, 0)
# define BOBUI_DEPRECATED_VERSION_X_5_12(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_5_12         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_5_12(text)
# define BOBUI_DEPRECATED_VERSION_5_12
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(5, 13, 0)
# define BOBUI_DEPRECATED_VERSION_X_5_13(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_5_13         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_5_13(text)
# define BOBUI_DEPRECATED_VERSION_5_13
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(5, 14, 0)
# define BOBUI_DEPRECATED_VERSION_X_5_14(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_5_14         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_5_14(text)
# define BOBUI_DEPRECATED_VERSION_5_14
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(5, 15, 0)
# define BOBUI_DEPRECATED_VERSION_X_5_15(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_5_15         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_5_15(text)
# define BOBUI_DEPRECATED_VERSION_5_15
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 0, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_0(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_0         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_0(text)
# define BOBUI_DEPRECATED_VERSION_6_0
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 1, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_1(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_1         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_1(text)
# define BOBUI_DEPRECATED_VERSION_6_1
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 2, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_2(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_2         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_2(text)
# define BOBUI_DEPRECATED_VERSION_6_2
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 3, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_3(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_3         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_3(text)
# define BOBUI_DEPRECATED_VERSION_6_3
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 4, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_4(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_4         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_4(text)
# define BOBUI_DEPRECATED_VERSION_6_4
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 5, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_5(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_5         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_5(text)
# define BOBUI_DEPRECATED_VERSION_6_5
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 6, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_6(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_6         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_6(text)
# define BOBUI_DEPRECATED_VERSION_6_6
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 7, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_7(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_7         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_7(text)
# define BOBUI_DEPRECATED_VERSION_6_7
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 8, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_8(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_8         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_8(text)
# define BOBUI_DEPRECATED_VERSION_6_8
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 9, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_9(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_9         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_9(text)
# define BOBUI_DEPRECATED_VERSION_6_9
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 10, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_10(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_10         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_10(text)
# define BOBUI_DEPRECATED_VERSION_6_10
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 11, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_11(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_11         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_11(text)
# define BOBUI_DEPRECATED_VERSION_6_11
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 12, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_12(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_12         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_12(text)
# define BOBUI_DEPRECATED_VERSION_6_12
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 13, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_13(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_13         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_13(text)
# define BOBUI_DEPRECATED_VERSION_6_13
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 14, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_14(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_14         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_14(text)
# define BOBUI_DEPRECATED_VERSION_6_14
#endif

#if BOBUI_WARN_DEPRECATED_UP_TO >= BOBUI_VERSION_CHECK(6, 15, 0)
# define BOBUI_DEPRECATED_VERSION_X_6_15(text) BOBUI_DEPRECATED_X(text)
# define BOBUI_DEPRECATED_VERSION_6_15         BOBUI_DEPRECATED
#else
# define BOBUI_DEPRECATED_VERSION_X_6_15(text)
# define BOBUI_DEPRECATED_VERSION_6_15
#endif

#define BOBUI_DEPRECATED_VERSION_X_5(minor, text)      BOBUI_DEPRECATED_VERSION_X_5_##minor(text)
#define BOBUI_DEPRECATED_VERSION_X(major, minor, text) BOBUI_DEPRECATED_VERSION_X_##major##_##minor(text)

#define BOBUI_DEPRECATED_VERSION_5(minor)      BOBUI_DEPRECATED_VERSION_5_##minor
#define BOBUI_DEPRECATED_VERSION(major, minor) BOBUI_DEPRECATED_VERSION_##major##_##minor

/*
    BOBUI_IF_DEPRECATED_SINCE(major, minor, whenTrue, whenFalse) expands to
    \a whenTrue if the specified (\a major, \a minor) version is less than or
    equal to the deprecation version defined by BOBUI_DISABLE_DEPRECATED_UP_TO,
    and to \a whenFalse otherwise.

    Currently used for BOBUI_INLINE_SINCE(maj, min), but can also be helpful for
    other macros of that kind.

    The implementation uses BOBUI_DEPRECATED_SINCE(maj, min) to define a bunch of
    helper BOBUI_IF_DEPRECATED_SINCE_X_Y macros, which expand to \a whenTrue or
    \a whenFalse depending on the value of BOBUI_DEPRECATED_SINCE.

    If you need to use BOBUI_IF_DEPRECATED_SINCE() for a (major, minor) version,
    that is not yet covered by the list below, you need to copy the definition
    and change the major and minor versions accordingly. For example, for
    version (X, Y), you will need to add

    \code
    #if BOBUI_DEPRECATED_SINCE(X, Y)
    # define BOBUI_IF_DEPRECATED_SINCE_X_Y(whenTrue, whenFalse) whenFalse
    #else
    # define BOBUI_IF_DEPRECATED_SINCE_X_Y(whenTrue, whenFalse) whenTrue
    #endif
    \endcode
*/

#define BOBUI_IF_DEPRECATED_SINCE(major, minor, whenTrue, whenFalse) \
    BOBUI_IF_DEPRECATED_SINCE_ ## major ## _ ## minor(whenTrue, whenFalse)

#if BOBUI_DEPRECATED_SINCE(6, 0)
# define BOBUI_IF_DEPRECATED_SINCE_6_0(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_0(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 1)
# define BOBUI_IF_DEPRECATED_SINCE_6_1(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_1(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 2)
# define BOBUI_IF_DEPRECATED_SINCE_6_2(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_2(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 3)
# define BOBUI_IF_DEPRECATED_SINCE_6_3(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_3(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 4)
# define BOBUI_IF_DEPRECATED_SINCE_6_4(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_4(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 5)
# define BOBUI_IF_DEPRECATED_SINCE_6_5(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_5(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 6)
# define BOBUI_IF_DEPRECATED_SINCE_6_6(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_6(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 7)
# define BOBUI_IF_DEPRECATED_SINCE_6_7(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_7(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 8)
# define BOBUI_IF_DEPRECATED_SINCE_6_8(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_8(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 9)
# define BOBUI_IF_DEPRECATED_SINCE_6_9(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_9(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 10)
# define BOBUI_IF_DEPRECATED_SINCE_6_10(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_10(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 11)
# define BOBUI_IF_DEPRECATED_SINCE_6_11(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_11(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 12)
# define BOBUI_IF_DEPRECATED_SINCE_6_12(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_12(whenTrue, whenFalse) whenTrue
#endif

#if BOBUI_DEPRECATED_SINCE(6, 13)
# define BOBUI_IF_DEPRECATED_SINCE_6_13(whenTrue, whenFalse) whenFalse
#else
# define BOBUI_IF_DEPRECATED_SINCE_6_13(whenTrue, whenFalse) whenTrue
#endif

#ifdef __cplusplus
// A tag to help mark stuff deprecated (cf. QStringViewLiteral)
namespace BobUIPrivate {
inline BOBUI_DEFINE_TAG(Deprecated);
}
#endif

#ifdef BOBUI_ASCII_CAST_WARNINGS
#  define BOBUI_ASCII_CAST_WARN \
    Q_DECL_DEPRECATED_X("Use fromUtf8, QStringLiteral, or QLatin1StringView")
#else
#  define BOBUI_ASCII_CAST_WARN
#endif

BOBUI_END_NAMESPACE

#endif // BOBUIDEPRECATIONMARKERS_H
