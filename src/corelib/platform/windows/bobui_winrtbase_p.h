// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUI_WINRTBASE_P_H
#define BOBUI_WINRTBASE_P_H

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

#include <BobUICore/qglobal.h>

#if BOBUI_CONFIG(cpp_winrt)
#  include <guiddef.h> // required by clang-cl: https://github.com/microsoft/cppwinrt/issues/1179
#  include <winrt/base.h>
#  include <BobUICore/private/qfactorycacheregistration_p.h>
// Workaround for Windows SDK bug.
// See https://github.com/microsoft/Windows.UI.Composition-Win32-Samples/issues/47
namespace winrt::impl
{
    template <typename Async>
    auto wait_for(Async const& async, Windows::Foundation::TimeSpan const& timeout);
}
// See https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/faq#how-do-i-resolve-ambiguities-with-getcurrenttime-and-or-try-
// for more workarounds.
#endif // BOBUI_CONFIG(cpp/winrt)

#endif // BOBUI_WINRTBASE_P_H
