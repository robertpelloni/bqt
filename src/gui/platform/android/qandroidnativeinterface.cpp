// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qpa/qplatformoffscreensurface.h>
#include <qpa/qplatformintegration.h>

#include <BobUIGui/qoffscreensurface_platform.h>
#include <BobUIGui/private/qguiapplication_p.h>

#include <BobUIGui/qpa/qplatformscreen_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace QNativeInterface::Private;

/*!
    \class QNativeInterface::QAndroidOffscreenSurface
    \since 6.0
    \brief Native interface to a offscreen surface on Android.

    Accessed through QOffscreenSurface::nativeInterface().

    \inmodule BobUIGui
    \inheaderfile QOffscreenSurface
    \ingroup native-interfaces
    \ingroup native-interfaces-qoffscreensurface
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QAndroidOffscreenSurface);
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QAndroidOffScreenIntegration);

QOffscreenSurface  *QNativeInterface::QAndroidOffscreenSurface::fromNative(ANativeWindow *nativeSurface)
{
    return QGuiApplicationPrivate::platformIntegration()->call<
            &QAndroidOffScreenIntegration::createOffscreenSurface>(nativeSurface);
}

/*!
    \class QNativeInterface::QAndroidScreen
    \since 6.7
    \brief Native interface to a screen.

    Accessed through QScreen::nativeInterface().
    \inmodule BobUIGui
    \ingroup native-interfaces
    \ingroup native-interfaces-qscreen
*/
/*!
    \fn int QNativeInterface::QAndroidScreen::displayId() const;
    \return the id of the underlying Android display.
*/
BOBUI_DEFINE_NATIVE_INTERFACE(QAndroidScreen);

BOBUI_END_NAMESPACE
