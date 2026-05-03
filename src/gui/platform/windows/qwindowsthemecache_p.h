// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSTHEME_CACHE_P_H
#define QWINDOWSTHEME_CACHE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "BobUIGui/private/bobuiguiglobal_p.h"

#include <BobUICore/bobui_windows.h>
#include <uxtheme.h>

BOBUI_BEGIN_NAMESPACE

namespace QWindowsThemeCache
{
    Q_GUI_EXPORT QString themeName(int theme);
    Q_GUI_EXPORT HTHEME createTheme(int theme, HWND hwnd);
    Q_GUI_EXPORT void clearThemeCache(HWND hwnd);
    Q_GUI_EXPORT void clearAllThemeCaches();
}

BOBUI_END_NAMESPACE

#endif // QWINDOWSTHEME_CACHE_P_H
