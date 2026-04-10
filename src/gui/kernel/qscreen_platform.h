// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSCREEN_PLATFORM_H
#define QSCREEN_PLATFORM_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the native interface APIs. Usage of
// this API may make your code source and binary incompatible
// with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qnativeinterface.h>
#include <BobUIGui/qguiapplication.h>

#if defined(Q_OS_WIN32)
#include <BobUIGui/qwindowdefs_win.h>
#endif

#if BOBUI_CONFIG(wayland)
struct wl_output;
#endif

#if defined(Q_OS_MACOS)
Q_FORWARD_DECLARE_OBJC_CLASS(NSScreen);
#endif

BOBUI_BEGIN_NAMESPACE

namespace QNativeInterface {

#if defined(Q_OS_WIN32) || defined(Q_QDOC)
struct Q_GUI_EXPORT QWindowsScreen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QWindowsScreen, 1, QScreen)
    virtual HMONITOR handle() const = 0;
};
#endif

#if BOBUI_CONFIG(wayland) || defined(Q_QDOC)
struct Q_GUI_EXPORT QWaylandScreen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QWaylandScreen, 1, QScreen)
    virtual wl_output *output() const = 0;
};
#endif

#if defined(Q_OS_ANDROID) || defined(Q_QDOC)
struct Q_GUI_EXPORT QAndroidScreen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QAndroidScreen, 1, QScreen)
    virtual int displayId() const = 0;
};
#endif

#if defined(Q_OS_MACOS) || defined(Q_QDOC)
struct Q_GUI_EXPORT QCocoaScreen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QCocoaScreen, 1, QScreen)
    virtual NSScreen *nativeScreen() const = 0;
};
#endif

} // namespace QNativeInterface

BOBUI_END_NAMESPACE

#endif
