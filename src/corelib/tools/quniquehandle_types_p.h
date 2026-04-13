// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUNIQUEHANDLE_TYPES_P_H
#define QUNIQUEHANDLE_TYPES_P_H

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

#include <BobUICore/qnamespace.h>
#include <BobUICore/private/quniquehandle_p.h>

#include <cstdio>

BOBUI_BEGIN_NAMESPACE

namespace BobUIUniqueHandleTraits {

#ifdef Q_OS_WIN

struct InvalidHandleTraits
{
    using Type = BobUI::HANDLE;
    static Type invalidValue() noexcept
    {
        return BobUI::HANDLE(-1); // AKA INVALID_HANDLE_VALUE
    }
    Q_CORE_EXPORT static bool close(Type handle) noexcept;
};

struct NullHandleTraits
{
    using Type = BobUI::HANDLE;
    static Type invalidValue() noexcept { return nullptr; }
    Q_CORE_EXPORT static bool close(Type handle) noexcept;
};

#endif

struct FileDescriptorHandleTraits
{
    using Type = int;
    static constexpr Type invalidValue() noexcept { return -1; }
    Q_CORE_EXPORT static bool close(Type handle);
};

struct FILEHandleTraits
{
    using Type = FILE *;
    static constexpr Type invalidValue() noexcept { return nullptr; }
    Q_CORE_EXPORT static bool close(Type handle);
};

} // namespace BobUIUniqueHandleTraits

#ifdef Q_OS_WIN

using QUniqueWin32Handle = QUniqueHandle<BobUIUniqueHandleTraits::InvalidHandleTraits>;
using QUniqueWin32NullHandle = QUniqueHandle<BobUIUniqueHandleTraits::NullHandleTraits>;

#endif

#ifdef Q_OS_UNIX

using QUniqueFileDescriptorHandle = QUniqueHandle<BobUIUniqueHandleTraits::FileDescriptorHandleTraits>;

#endif

using QUniqueFILEHandle = QUniqueHandle<BobUIUniqueHandleTraits::FILEHandleTraits>;

BOBUI_END_NAMESPACE

#endif
