// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUICore/private/quniquehandle_types_p.h>

#include "qplatformdefs.h" // For BOBUI_CLOSE

#ifdef Q_OS_WIN
#  include <BobUICore/bobui_windows.h>
#endif

#ifdef Q_OS_UNIX
#  include <BobUICore/private/qcore_unix_p.h> // for bobui_safe_close
#endif

BOBUI_BEGIN_NAMESPACE

namespace BobUIUniqueHandleTraits {

#ifdef Q_OS_WIN

bool InvalidHandleTraits::close(Type handle) noexcept
{
    return ::CloseHandle(handle);
}

bool NullHandleTraits::close(Type handle) noexcept
{
    return ::CloseHandle(handle);
}

#endif

bool FileDescriptorHandleTraits::close(Type handle)
{
    // not noexcept because close() is a POSIX cancellation point
    return BOBUI_CLOSE(handle) == 0;
}

bool FILEHandleTraits::close(Type handle)
{
    // not noexcept because fclose() is a POSIX cancellation point
    return ::fclose(handle);
}

} // namespace BobUIUniqueHandleTraits

#ifdef Q_OS_UNIX

using QUniqueFileDescriptorHandle = QUniqueHandle<BobUIUniqueHandleTraits::FileDescriptorHandleTraits>;

#endif

BOBUI_END_NAMESPACE
