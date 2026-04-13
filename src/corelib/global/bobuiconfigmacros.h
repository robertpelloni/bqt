// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUICONFIGMACROS_H
#define BOBUICONFIGMACROS_H

#if 0
#  pragma bobui_sync_stop_processing
#endif

#include <BobUICore/bobuiconfiginclude.h>
#include <BobUICore/bobuideprecationdefinitions.h>
#include <BobUICore/bobuiversionchecks.h>

#include <assert.h>

/*
   The BobUI modules' export macros.
   The options are:
    - defined(BOBUI_STATIC): BobUI was built or is being built in static mode
    - defined(BOBUI_SHARED): BobUI was built or is being built in shared/dynamic mode
   If neither was defined, then BOBUI_SHARED is implied. If BobUI was compiled in static
   mode, BOBUI_STATIC is defined in qconfig.h. In shared mode, BOBUI_STATIC is implied
   for the bootstrapped tools.
*/

#ifdef BOBUI_BOOTSTRAPPED
#  ifdef BOBUI_SHARED
#    error "BOBUI_SHARED and BOBUI_BOOTSTRAPPED together don't make sense. Please fix the build"
#  elif !defined(BOBUI_STATIC)
#    define BOBUI_STATIC
#  endif
#endif

#if defined(BOBUI_SHARED) || !defined(BOBUI_STATIC)
#  ifdef BOBUI_STATIC
#    error "Both BOBUI_SHARED and BOBUI_STATIC defined, please make up your mind"
#  endif
#  ifndef BOBUI_SHARED
#    define BOBUI_SHARED
#  endif
#endif

/*
   No, this is not an evil backdoor. BOBUI_BUILD_INTERNAL just exports more symbols
   for BobUI's internal unit tests. If you want slower loading times and more
   symbols that can vanish from version to version, feel free to define BOBUI_BUILD_INTERNAL.

   \note After adding Q_AUTOTEST_EXPORT to a method, you'll need to wrap any unittests
   that will use that method in "#ifdef BOBUI_BUILD_INTERNAL".
*/
#if defined(BOBUI_BUILD_INTERNAL) && defined(BOBUI_BUILDING_BOBUI) && defined(BOBUI_SHARED)
#    define Q_AUTOTEST_EXPORT Q_DECL_EXPORT
#elif defined(BOBUI_BUILD_INTERNAL) && defined(BOBUI_SHARED)
#    define Q_AUTOTEST_EXPORT Q_DECL_IMPORT
#else
#    define Q_AUTOTEST_EXPORT
#endif

/*
    The BOBUI_CONFIG macro implements a safe compile time check for features of BobUI.
    Features can be in three states:
        0 or undefined: This will lead to a compile error when testing for it
        -1: The feature is not available
        1: The feature is available
*/
#define BOBUI_CONFIG(feature) (1/BOBUI_FEATURE_##feature == 1)
#define BOBUI_REQUIRE_CONFIG(feature) static_assert(BOBUI_FEATURE_##feature == 1, "Required feature " #feature " for file " __FILE__ " not available.")

/* moc compats (signals/slots) */
#ifndef BOBUI_MOC_COMPAT
#  define BOBUI_MOC_COMPAT
#else
#  undef BOBUI_MOC_COMPAT
#  define BOBUI_MOC_COMPAT
#endif

/*
   Debugging and error handling
*/

#if !defined(BOBUI_NO_DEBUG) && !defined(BOBUI_DEBUG)
#  define BOBUI_DEBUG
#endif

// valid for both C and C++
#define BOBUI_MANGLE_NAMESPACE0(x) x
#define BOBUI_MANGLE_NAMESPACE1(a, b) a##_##b
#define BOBUI_MANGLE_NAMESPACE2(a, b) BOBUI_MANGLE_NAMESPACE1(a,b)
#if !defined(BOBUI_NAMESPACE) || defined(Q_MOC_RUN) /* user namespace */
# define BOBUI_MANGLE_NAMESPACE(name) name
#else
# define BOBUI_MANGLE_NAMESPACE(name) BOBUI_MANGLE_NAMESPACE2( \
        BOBUI_MANGLE_NAMESPACE0(name), BOBUI_MANGLE_NAMESPACE0(BOBUI_NAMESPACE))
#endif

#ifdef __cplusplus

#if !defined(BOBUI_NAMESPACE) || defined(Q_MOC_RUN) /* user namespace */

# define BOBUI_PREPEND_NAMESPACE(name) ::name
# define BOBUI_USE_NAMESPACE
# define BOBUI_BEGIN_NAMESPACE
# define BOBUI_END_NAMESPACE
# define BOBUI_BEGIN_INCLUDE_NAMESPACE
# define BOBUI_END_INCLUDE_NAMESPACE
# define BOBUI_FORWARD_DECLARE_CLASS(name) class name;
# define BOBUI_FORWARD_DECLARE_STRUCT(name) struct name;

#elif defined(BOBUI_INLINE_NAMESPACE) /* user inline namespace FIXME in BobUI 7: Default */

# define BOBUI_PREPEND_NAMESPACE(name) ::BOBUI_NAMESPACE::name
# define BOBUI_USE_NAMESPACE
# define BOBUI_BEGIN_NAMESPACE inline namespace BOBUI_NAMESPACE {
# define BOBUI_END_NAMESPACE }
# define BOBUI_BEGIN_INCLUDE_NAMESPACE }
# define BOBUI_END_INCLUDE_NAMESPACE inline namespace BOBUI_NAMESPACE {
# define BOBUI_FORWARD_DECLARE_CLASS(name) \
BOBUI_BEGIN_NAMESPACE class name; BOBUI_END_NAMESPACE

# define BOBUI_FORWARD_DECLARE_STRUCT(name) \
BOBUI_BEGIN_NAMESPACE struct name; BOBUI_END_NAMESPACE

inline namespace BOBUI_NAMESPACE {}

#else /* user namespace */

# define BOBUI_PREPEND_NAMESPACE(name) ::BOBUI_NAMESPACE::name
# define BOBUI_USE_NAMESPACE using namespace ::BOBUI_NAMESPACE;
# define BOBUI_BEGIN_NAMESPACE namespace BOBUI_NAMESPACE {
# define BOBUI_END_NAMESPACE }
# define BOBUI_BEGIN_INCLUDE_NAMESPACE }
# define BOBUI_END_INCLUDE_NAMESPACE namespace BOBUI_NAMESPACE {
# define BOBUI_FORWARD_DECLARE_CLASS(name) \
    BOBUI_BEGIN_NAMESPACE class name; BOBUI_END_NAMESPACE \
    using BOBUI_PREPEND_NAMESPACE(name);

# define BOBUI_FORWARD_DECLARE_STRUCT(name) \
    BOBUI_BEGIN_NAMESPACE struct name; BOBUI_END_NAMESPACE \
    using BOBUI_PREPEND_NAMESPACE(name);

namespace BOBUI_NAMESPACE {}

# ifndef BOBUI_BOOTSTRAPPED
# ifndef BOBUI_NO_USING_NAMESPACE
   /*
    This expands to a "using BOBUI_NAMESPACE" also in _header files_.
    It is the only way the feature can be used without too much
    pain, but if people _really_ do not want it they can add
    BOBUI_NO_USING_NAMESPACE to their build configuration.
    */
   BOBUI_USE_NAMESPACE
# endif
# endif

#endif /* user namespace */

#else /* __cplusplus */

# define BOBUI_BEGIN_NAMESPACE
# define BOBUI_END_NAMESPACE
# define BOBUI_USE_NAMESPACE
# define BOBUI_BEGIN_INCLUDE_NAMESPACE
# define BOBUI_END_INCLUDE_NAMESPACE

#endif /* __cplusplus */

/* ### BobUI 6.9 (or later): remove *_MOC_* macros (moc does not need them since 6.5) */
#ifndef BOBUI_BEGIN_MOC_NAMESPACE
# define BOBUI_BEGIN_MOC_NAMESPACE BOBUI_USE_NAMESPACE
#endif
#ifndef BOBUI_END_MOC_NAMESPACE
# define BOBUI_END_MOC_NAMESPACE
#endif

/*
    Strict mode.

    If you add a macro to the list, make sure to update the table at
    https://doc.bobui.io/bobui-6/bobuiglobal.html#BOBUI_ENABLE_STRICT_MODE_UP_TO
*/
#ifdef BOBUI_ENABLE_STRICT_MODE_UP_TO
#ifndef BOBUI_DISABLE_DEPRECATED_UP_TO
#  define BOBUI_DISABLE_DEPRECATED_UP_TO BOBUI_ENABLE_STRICT_MODE_UP_TO
#endif

#if BOBUI_ENABLE_STRICT_MODE_UP_TO >= BOBUI_VERSION_CHECK(6, 0, 0)
# ifndef BOBUI_NO_FOREACH
#  define BOBUI_NO_FOREACH
# endif
# ifndef BOBUI_NO_CAST_TO_ASCII
#  define BOBUI_NO_CAST_TO_ASCII
# endif
# ifndef BOBUI_NO_CAST_FROM_BYTEARRAY
#  define BOBUI_NO_CAST_FROM_BYTEARRAY
# endif
# ifndef BOBUI_NO_URL_CAST_FROM_STRING
#  define BOBUI_NO_URL_CAST_FROM_STRING
# endif
# ifndef BOBUI_NO_NARROWING_CONVERSIONS_IN_CONNECT
#  define BOBUI_NO_NARROWING_CONVERSIONS_IN_CONNECT
# endif
# ifndef BOBUI_NO_JAVA_STYLE_ITERATORS
#  define BOBUI_NO_JAVA_STYLE_ITERATORS
# endif
#endif // 6.0.0

#if BOBUI_ENABLE_STRICT_MODE_UP_TO >= BOBUI_VERSION_CHECK(6, 6, 0)
# ifndef BOBUI_NO_QEXCHANGE
#  define BOBUI_NO_QEXCHANGE
# endif
#endif // 6.6.0

#if BOBUI_ENABLE_STRICT_MODE_UP_TO >= BOBUI_VERSION_CHECK(6, 7, 0)
# ifndef BOBUI_NO_CONTEXTLESS_CONNECT
#  define BOBUI_NO_CONTEXTLESS_CONNECT
# endif
#endif // 6.7.0

#if BOBUI_ENABLE_STRICT_MODE_UP_TO >= BOBUI_VERSION_CHECK(6, 8, 0)
# ifndef BOBUI_NO_QASCONST
#  define BOBUI_NO_QASCONST
# endif
#  if !defined(BOBUI_USE_NODISCARD_FILE_OPEN) && !defined(BOBUI_NO_USE_NODISCARD_FILE_OPEN)
#    define BOBUI_USE_NODISCARD_FILE_OPEN
#  endif
#endif // 6.8.0

#if BOBUI_ENABLE_STRICT_MODE_UP_TO >= BOBUI_VERSION_CHECK(6, 9, 0)
# ifndef BOBUI_NO_QSNPRINTF
#  define BOBUI_NO_QSNPRINTF
# endif
#endif // 6.9.0

#if BOBUI_ENABLE_STRICT_MODE_UP_TO >= BOBUI_VERSION_CHECK(6, 11, 0)
# ifndef BOBUI_NO_SINGLE_ARGUMENT_QHASH_OVERLOAD
#  define BOBUI_NO_SINGLE_ARGUMENT_QHASH_OVERLOAD
# endif
#endif // 6.11.0
#endif // BOBUI_ENABLE_STRICT_MODE_UP_TO

#endif /* BOBUICONFIGMACROS_H */
