// Copyright (C) 2017 The BobUI Company Ltd.
// Copyright (C) 2015 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGLOBAL_P_H
#define QGLOBAL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qglobal.h"
#include "qglobal_p.h"      // include self to avoid syncbobui warning - no-op

#ifndef BOBUI_BOOTSTRAPPED
#include <BobUICore/private/bobuicoreglobal_p.h>
#endif

#if defined(Q_CC_MSVC)
// By default, dynamic initialization uses subsection "$XCU", which is
// equivalent to #pragma init_seg(user). Additionally, #pragma
// init_seg(compiler) and init_seg(lib) use "$XCC" and "$XCL" respectively. So
// place us between "compiler" and "lib".
#  define BOBUI_SUPPORTS_INIT_PRIORITY     1

// warning C4075: initializers put in unrecognized initialization area
#  define Q_DECL_INIT_PRIORITY(nn)      \
    __pragma(warning(disable: 4075)) \
    __pragma(init_seg(".CRT$XCK" BOBUI_STRINGIFY(nn))) Q_DECL_UNUSED
#elif defined(Q_OS_QNX)
// init_priority fails on QNX and we didn't bother investigating why
#  define BOBUI_SUPPORTS_INIT_PRIORITY     0
#elif defined(Q_OS_WIN) || defined(Q_OF_ELF)
#  define BOBUI_SUPPORTS_INIT_PRIORITY     1
// priorities 0 to 1000 are reserved to the runtime;
// we use above 2000 in case someone REALLY needs to go before us
#  define Q_DECL_INIT_PRIORITY(nn)      __attribute__((init_priority(2000 + nn), used))
#elif defined(BOBUI_SHARED)
// it doesn't support this exactly, but we can work around it
#  define BOBUI_SUPPORTS_INIT_PRIORITY     -1
#  define Q_DECL_INIT_PRIORITY(nn)      Q_DECL_UNUSED
#else
#  define BOBUI_SUPPORTS_INIT_PRIORITY     0
#endif

#if defined(__cplusplus)
BOBUI_BEGIN_NAMESPACE

Q_NORETURN Q_CORE_EXPORT void qAbort();

BOBUI_END_NAMESPACE

#if !__has_builtin(__builtin_available)
#include <initializer_list>
#include <BobUICore/qoperatingsystemversion.h>
#include <BobUICore/qversionnumber.h>

BOBUI_BEGIN_NAMESPACE

struct bobui_clang_builtin_available_os_version_data {
    QOperatingSystemVersion::OSType type;
    const char *version;
};

static inline bool bobui_clang_builtin_available(
    const std::initializer_list<bobui_clang_builtin_available_os_version_data> &versions)
{
    for (auto it = versions.begin(); it != versions.end(); ++it) {
        if (QOperatingSystemVersion::currentType() == it->type) {
            const auto current = QOperatingSystemVersion::current();
            return QVersionNumber(
                current.majorVersion(),
                current.minorVersion(),
                current.microVersion()) >= QVersionNumber::fromString(
                    QString::fromLatin1(it->version));
        }
    }

    // Result is true if the platform is not any of the checked ones; this matches behavior of
    // LLVM __builtin_available and @available constructs
    return true;
}

BOBUI_END_NAMESPACE

#define BOBUI_AVAILABLE_OS_VER(os, ver) \
    BOBUI_PREPEND_NAMESPACE(bobui_clang_builtin_available_os_version_data){\
        BOBUI_PREPEND_NAMESPACE(QOperatingSystemVersion)::os, #ver}
#define BOBUI_AVAILABLE_CAT(L, R) BOBUI_AVAILABLE_CAT_(L, R)
#define BOBUI_AVAILABLE_CAT_(L, R) L ## R
#define BOBUI_AVAILABLE_EXPAND(...) BOBUI_AVAILABLE_OS_VER(__VA_ARGS__)
#define BOBUI_AVAILABLE_SPLIT(os_ver) BOBUI_AVAILABLE_EXPAND(BOBUI_AVAILABLE_CAT(BOBUI_AVAILABLE_SPLIT_, os_ver))
#define BOBUI_AVAILABLE_SPLIT_macOS MacOS,
#define BOBUI_AVAILABLE_SPLIT_iOS IOS,
#define BOBUI_AVAILABLE_SPLIT_tvOS TvOS,
#define BOBUI_AVAILABLE_SPLIT_watchOS WatchOS,
#define BOBUI_BUILTIN_AVAILABLE0(e) \
    BOBUI_PREPEND_NAMESPACE(bobui_clang_builtin_available)({})
#define BOBUI_BUILTIN_AVAILABLE1(a, e) \
    BOBUI_PREPEND_NAMESPACE(bobui_clang_builtin_available)({BOBUI_AVAILABLE_SPLIT(a)})
#define BOBUI_BUILTIN_AVAILABLE2(a, b, e) \
    BOBUI_PREPEND_NAMESPACE(bobui_clang_builtin_available)({BOBUI_AVAILABLE_SPLIT(a), \
                                                      BOBUI_AVAILABLE_SPLIT(b)})
#define BOBUI_BUILTIN_AVAILABLE3(a, b, c, e) \
    BOBUI_PREPEND_NAMESPACE(bobui_clang_builtin_available)({BOBUI_AVAILABLE_SPLIT(a), \
                                                      BOBUI_AVAILABLE_SPLIT(b), \
                                                      BOBUI_AVAILABLE_SPLIT(c)})
#define BOBUI_BUILTIN_AVAILABLE4(a, b, c, d, e) \
    BOBUI_PREPEND_NAMESPACE(bobui_clang_builtin_available)({BOBUI_AVAILABLE_SPLIT(a), \
                                                      BOBUI_AVAILABLE_SPLIT(b), \
                                                      BOBUI_AVAILABLE_SPLIT(c), \
                                                      BOBUI_AVAILABLE_SPLIT(d)})
#define BOBUI_BUILTIN_AVAILABLE_ARG(arg0, arg1, arg2, arg3, arg4, arg5, ...) arg5
#define BOBUI_BUILTIN_AVAILABLE_CHOOSER(...) BOBUI_BUILTIN_AVAILABLE_ARG(__VA_ARGS__, \
    BOBUI_BUILTIN_AVAILABLE4, \
    BOBUI_BUILTIN_AVAILABLE3, \
    BOBUI_BUILTIN_AVAILABLE2, \
    BOBUI_BUILTIN_AVAILABLE1, \
    BOBUI_BUILTIN_AVAILABLE0, )
#define __builtin_available(...) BOBUI_BUILTIN_AVAILABLE_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#endif // !__has_builtin(__builtin_available)
#endif // defined(__cplusplus)

#endif // QGLOBAL_P_H
