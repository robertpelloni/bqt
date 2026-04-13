// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOFFSCREENSURFACE_PLATFORM_H
#define QOFFSCREENSURFACE_PLATFORM_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the native interface APIs. Usage of
// this API may make your code source and binary incompatible
// with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qoffscreensurface.h>
#include <BobUICore/qnativeinterface.h>

#if defined(Q_OS_ANDROID)
struct ANativeWindow;
#endif

BOBUI_BEGIN_NAMESPACE

namespace QNativeInterface {

#if defined(Q_OS_ANDROID) || defined(Q_QDOC)
struct Q_GUI_EXPORT QAndroidOffscreenSurface
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QAndroidOffscreenSurface, 1, QOffscreenSurface)
    static QOffscreenSurface *fromNative(ANativeWindow *nativeSurface);
    virtual ANativeWindow *nativeSurface() const = 0;
};
#endif

} // QNativeInterface

BOBUI_END_NAMESPACE

#endif // QOFFSCREENSURFACE_PLATFORM_H
