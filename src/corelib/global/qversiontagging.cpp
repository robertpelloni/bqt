// Copyright (C) 2022 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qglobal.h"
#include "qversiontagging.h"

extern "C" {
#define SYM BOBUI_MANGLE_NAMESPACE(bobui_version_tag)
#define SSYM BOBUI_STRINGIFY(SYM)

// With compilers that have the "alias" attribute, the macro creates a global
// variable "bobui_version_tag_M_m" (M: major, m: minor) as an alias to either
// bobui_version_tag or _bobui_version_tag. Everywhere else, we simply create a new
// global variable bobui_version_tag_M_m without aliasing to a single variable.
//
// Additionally, on systems using ELF binaries (Linux, FreeBSD, etc.), we
// create ELF versions by way of the .symver assembly directive[1].
//
// Unfortunately, Clang on Darwin systems says it supports the "alias"
// attribute, but fails when used. That's a Clang bug (as of XCode 12).
//
// [1] https://sourceware.org/binutils/docs/as/Symver.html

#if defined(Q_CC_GNU) && defined(Q_OF_ELF)
#  define make_versioned_symbol2(sym, m, n, separator)     \
    Q_CORE_EXPORT extern __attribute__((alias("_" SSYM))) const char sym ## _ ## m ## _ ## n; \
    asm(".symver " BOBUI_STRINGIFY(sym) "_" BOBUI_STRINGIFY(m) "_" BOBUI_STRINGIFY(n) ", " \
        BOBUI_STRINGIFY(sym) separator "BobUI_" BOBUI_STRINGIFY(m) "." BOBUI_STRINGIFY(n))

extern const char BOBUI_MANGLE_NAMESPACE(_bobui_version_tag) = 0;
#elif __has_attribute(alias) && !defined(Q_OS_DARWIN)
#  define make_versioned_symbol2(sym, m, n, separator)     \
    Q_CORE_EXPORT extern __attribute__((alias(SSYM))) const char sym ## _ ## m ## _ ## n
extern const char SYM = 0;
#else
#  define make_versioned_symbol2(sym, m, n, separator)     \
    Q_CORE_EXPORT extern const char sym ## _ ## m ## _ ## n = 0;
#endif
#define make_versioned_symbol(sym, m, n, separator)    make_versioned_symbol2(sym, m, n, separator)

#if BOBUI_VERSION_MINOR > 0
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 0, "@");
#endif
#if BOBUI_VERSION_MINOR > 1
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 1, "@");
#endif
#if BOBUI_VERSION_MINOR > 2
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 2, "@");
#endif
#if BOBUI_VERSION_MINOR > 3
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 3, "@");
#endif
#if BOBUI_VERSION_MINOR > 4
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 4, "@");
#endif
#if BOBUI_VERSION_MINOR > 5
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 5, "@");
#endif
#if BOBUI_VERSION_MINOR > 6
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 6, "@");
#endif
#if BOBUI_VERSION_MINOR > 7
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 7, "@");
#endif
#if BOBUI_VERSION_MINOR > 8
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 8, "@");
#endif
#if BOBUI_VERSION_MINOR > 9
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 9, "@");
#endif
#if BOBUI_VERSION_MINOR > 10
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 10, "@");
#endif
#if BOBUI_VERSION_MINOR > 11
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 11, "@");
#endif
#if BOBUI_VERSION_MINOR > 12
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 12, "@");
#endif
#if BOBUI_VERSION_MINOR > 13
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 13, "@");
#endif
#if BOBUI_VERSION_MINOR > 14
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 14, "@");
#endif
#if BOBUI_VERSION_MINOR > 15
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, 15, "@");
#endif
#if BOBUI_VERSION_MINOR > 16
#  error "Please update this file with more BobUI versions."
#endif

// the default version:
make_versioned_symbol(SYM, BOBUI_VERSION_MAJOR, BOBUI_VERSION_MINOR, "@@");
}

BOBUI_BEGIN_NAMESPACE

static_assert(std::is_trivially_destructible_v<BobUIPrivate::QVersionTag>);

BOBUI_END_NAMESPACE
