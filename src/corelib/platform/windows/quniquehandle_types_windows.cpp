// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "quniquehandle_types_windows_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIUniqueHandleTraits {

bool HDCTraits::close(Type handle, HWND hwnd) noexcept
{
    return ::ReleaseDC(hwnd, handle);
}

} // namespace BobUIUniqueHandleTraits

BOBUI_END_NAMESPACE
