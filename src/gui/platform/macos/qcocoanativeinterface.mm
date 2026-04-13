// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/bobuigui-config.h>
#ifndef BOBUI_NO_OPENGL
#  include <BobUIGui/private/qopenglcontext_p.h>
#endif

#include <BobUIGui/private/qguiapplication_p.h>
#include <qpa/qplatformopenglcontext.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformwindow_p.h>
#include <qpa/qplatformmenu_p.h>

#include <AppKit/AppKit.h>

BOBUI_BEGIN_NAMESPACE

using namespace QNativeInterface::Private;

/*!
    \class QNativeInterface::Private::QCocoaWindow
    \since 6.0
    \internal
    \brief Native interface for QPlatformWindow on \macos.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QCocoaWindow);


/*!
    \class QNativeInterface::Private::QCocoaMenu
    \since 6.0
    \internal
    \brief Native interface for QPlatformMenu on \macos.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QCocoaMenu);
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QCocoaMenuBar);

/*!
    \class QNativeInterface::QCocoaScreen
    \since 6.11
    \brief Native interface to a screen on \macos.

    Accessed through QScreen::nativeInterface().
    \inmodule BobUIGui
    \ingroup native-interfaces
    \ingroup native-interfaces-qscreen
*/
/*!
    \fn NSScreen *QNativeInterface::QCocoaScreen::nativeScreen() const
    \return the underlying NSScreen of this QScreen.
*/
BOBUI_DEFINE_NATIVE_INTERFACE(QCocoaScreen);

#ifndef BOBUI_NO_OPENGL

/*!
    \class QNativeInterface::QCocoaGLContext
    \since 6.0
    \brief Native interface to an NSOpenGLContext on \macos.

    Accessed through QOpenGLContext::nativeInterface().

    \inmodule BobUIGui
    \inheaderfile QOpenGLContext
    \ingroup native-interfaces
    \ingroup native-interfaces-qopenglcontext
*/

/*!
    \fn QOpenGLContext *QNativeInterface::QCocoaGLContext::fromNative(NSOpenGLContext *context, QOpenGLContext *shareContext = nullptr)

    \brief Adopts an NSOpenGLContext.

    The adopted NSOpenGLContext \a context is retained. Ownership of the
    created QOpenGLContext \a shareContext is transferred to the caller.
*/

/*!
    \fn NSOpenGLContext *QNativeInterface::QCocoaGLContext::nativeContext() const

    \return the underlying NSOpenGLContext.
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QCocoaGLContext);
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QCocoaGLIntegration);

QOpenGLContext *QNativeInterface::QCocoaGLContext::fromNative(NSOpenGLContext *nativeContext, QOpenGLContext *shareContext)
{
    return QGuiApplicationPrivate::platformIntegration()->call<
        &QCocoaGLIntegration::createOpenGLContext>(nativeContext, shareContext);
}

#endif // BOBUI_NO_OPENGL

BOBUI_END_NAMESPACE
