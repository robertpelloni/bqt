// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef BOBUIFORMAT_IMPL_H
#define BOBUIFORMAT_IMPL_H

#if 0
#pragma bobui_no_master_include
#pragma bobui_sync_skip_header_check
#endif

#include <BobUICore/qsystemdetection.h>
#include <BobUICore/bobuiconfigmacros.h>

// Users can disable std::format support in their
// projects by using this definition.
#if !defined(BOBUI_NO_STD_FORMAT_SUPPORT) && defined(__cpp_lib_format) && __cpp_lib_format >= 202106L

#include <format>

// If this macro is defined, std::format support is actually available.
// Use it to provide the implementation!
// Note that any out-of-line helper function should not depend on this
// definition, as it should be unconditionally available even in C++17 builds
// to keep BC.
#define BOBUI_SUPPORTS_STD_FORMAT  1

#endif // __cpp_lib_format && !BOBUI_NO_STD_FORMAT_SUPPORT

#endif // BOBUIFORMAT_IMPL_H
