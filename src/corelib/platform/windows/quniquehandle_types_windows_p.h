// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QUNIQUEHANDLE_TYPES_WINDOWS_P_H
#define QUNIQUEHANDLE_TYPES_WINDOWS_P_H

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
#include <BobUICore/bobui_windows.h>
#include <BobUICore/private/quniquehandle_p.h>

#if defined(Q_OS_WIN) || defined(Q_QDOC)

BOBUI_BEGIN_NAMESPACE

namespace BobUIUniqueHandleTraits {

struct HDCTraits
{
    using Type = HDC;
    static Type invalidValue() noexcept { return nullptr; }
    Q_CORE_EXPORT static bool close(Type handle, HWND hwnd) noexcept;
};

struct HDCDeleter
{
    using Type = HDCTraits::Type;

    constexpr HDCDeleter() noexcept = default;
    explicit constexpr HDCDeleter(HWND hwnd) noexcept
        : hwnd(hwnd)
    {}

    void operator()(Type handle) const noexcept
    {
        if (handle != HDCTraits::invalidValue()) {
            const bool success = HDCTraits::close(handle, hwnd);
            Q_ASSERT(success);
        }
    }

    HWND hwnd{ nullptr };
};

} // namespace BobUIUniqueHandleTraits
using QUniqueHDCHandle = QUniqueHandle<
    BobUIUniqueHandleTraits::HDCTraits,
    BobUIUniqueHandleTraits::HDCDeleter
>;

BOBUI_END_NAMESPACE

#endif // Q_OS_WIN

#endif // QUNIQUEHANDLE_TYPES_WINDOWS_P_H
