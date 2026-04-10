// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIGui/private/bobuiguiglobal_p.h>

#if BOBUI_CONFIG(opengl)
#  include <BobUIGui/private/qopenglcontext_p.h>
#endif
#include <BobUIGui/private/qguiapplication_p.h>

#include <qpa/qplatformopenglcontext.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformscreen_p.h>
#include <qpa/qplatformwindow_p.h>

#include <BobUIGui/private/qkeymapper_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace QNativeInterface::Private;

#ifndef BOBUI_NO_OPENGL

#if BOBUI_CONFIG(xcb_glx_plugin)

/*!
    \class QNativeInterface::QGLXContext
    \since 6.0
    \brief Native interface to a GLX context.

    Accessed through QOpenGLContext::nativeInterface().

    \inmodule BobUIGui
    \inheaderfile QOpenGLContext
    \ingroup native-interfaces
    \ingroup native-interfaces-qopenglcontext
*/

/*!
    \fn QOpenGLContext *QNativeInterface::QGLXContext::fromNative(GLXContext configBasedContext, QOpenGLContext *shareContext = nullptr)

    \brief Adopts a GLXContext \a configBasedContext created from an FBConfig.

    The context must be created from a framebuffer configuration, using the \c glXCreateNewContext function.

    Ownership of the created QOpenGLContext \a shareContext is transferred to the caller.
*/

/*!
    \fn QOpenGLContext *QNativeInterface::QGLXContext::fromNative(GLXContext visualBasedContext, void *visualInfo, QOpenGLContext *shareContext = nullptr)

    \brief Adopts a GLXContext created from an X visual.

    The context must be created from a visual, using the \c glXCreateContext function.
    The same visual must be passed as a pointer to an \c XVisualInfo struct, in the \a visualInfo argument.

    Ownership of the created QOpenGLContext is transferred to the caller.
*/

/*!
    \fn GLXContext QNativeInterface::QGLXContext::nativeContext() const

    \return the underlying GLXContext.
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QGLXContext);
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QGLXIntegration);

QOpenGLContext *QNativeInterface::QGLXContext::fromNative(GLXContext configBasedContext, QOpenGLContext *shareContext)
{
    return QGuiApplicationPrivate::platformIntegration()->call<
        &QGLXIntegration::createOpenGLContext>(configBasedContext, nullptr, shareContext);
}

QOpenGLContext *QNativeInterface::QGLXContext::fromNative(GLXContext visualBasedContext, void *visualInfo, QOpenGLContext *shareContext)
{
    return QGuiApplicationPrivate::platformIntegration()->call<
        &QGLXIntegration::createOpenGLContext>(visualBasedContext, visualInfo, shareContext);
}
#endif // BOBUI_CONFIG(xcb_glx_plugin)

#if BOBUI_CONFIG(egl)

/*!
    \class QNativeInterface::QEGLContext
    \since 6.0
    \brief Native interface to an EGL context.

    Accessed through QOpenGLContext::nativeInterface().

    \inmodule BobUIGui
    \inheaderfile QOpenGLContext
    \ingroup native-interfaces
    \ingroup native-interfaces-qopenglcontext
*/

/*!
    \fn QOpenGLContext *QNativeInterface::QEGLContext::fromNative(EGLContext context, EGLDisplay display, QOpenGLContext *shareContext = nullptr)

    \brief Adopts an EGLContext \a context.

    The same \c EGLDisplay passed to \c eglCreateContext must be passed as the \a display argument.

    Ownership of the created QOpenGLContext \a shareContext is transferred
    to the caller.
*/

/*!
    \fn EGLContext QNativeInterface::QEGLContext::nativeContext() const

    \return the underlying EGLContext.
*/

/*!
    \fn EGLConfig QNativeInterface::QEGLContext::config() const
    \since 6.3
    \return the EGLConfig associated with the underlying EGLContext.
*/

/*!
    \fn EGLDisplay QNativeInterface::QEGLContext::display() const
    \since 6.3
    \return the EGLDisplay associated with the underlying EGLContext.
*/


/*!
    \fn void QNativeInterface::QEGLContext::invalidateContext()
    \since 6.5
    \brief Marks the context as invalid

    If this context is used by the BobUI Quick scenegraph, this will trigger the
    SceneGraph to destroy this context and create a new one.

    Similarly to QPlatformWindow::invalidateSurface(),
    this function can only be expected to have an effect on certain platforms,
    such as eglfs.

    \sa QOpenGLContext::isValid(), QPlatformWindow::invalidateSurface()
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QEGLContext);
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QEGLIntegration);

QOpenGLContext *QNativeInterface::QEGLContext::fromNative(EGLContext context, EGLDisplay display, QOpenGLContext *shareContext)
{
    return QGuiApplicationPrivate::platformIntegration()->call<
        &QEGLIntegration::createOpenGLContext>(context, display, shareContext);
}
#endif // BOBUI_CONFIG(egl)

#endif // BOBUI_NO_OPENGL

#if BOBUI_CONFIG(xcb)

/*!
    \class QNativeInterface::Private::QXcbScreen
    \since 6.0
    \internal
    \brief Native interface to QPlatformScreen.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QXcbScreen);

/*!
    \class QNativeInterface::Private::QXcbWindow
    \since 6.0
    \internal
    \brief Native interface to QPlatformWindow.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QXcbWindow);

/*!
    \class QNativeInterface::QX11Application
    \since 6.2
    \brief Native interface to an X11 application.

    Accessed through QGuiApplication::nativeInterface().

    \inmodule BobUIGui
    \inheaderfile QGuiApplication
    \ingroup native-interfaces
    \ingroup native-interfaces-qguiapplication
*/

/*!
    \fn Display *QNativeInterface::QX11Application::display() const

    \return the X display of the application, for use with Xlib.

    \sa connection()
*/

/*!
    \fn xcb_connection_t *QNativeInterface::QX11Application::connection() const

    \return the X connection of the application, for use with XCB.

    \sa display()
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QX11Application);

#endif // BOBUI_CONFIG(xcb)

#if BOBUI_CONFIG(vsp2)
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QVsp2Screen);
#endif

#ifdef Q_OS_WEBOS
BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QWebOSScreen);
#endif

#if BOBUI_CONFIG(evdev)

/*!
    \class QNativeInterface::Private::QEvdevKeyMapper
    \since 6.0
    \internal
    \brief Native interface to QKeyMapper.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QEvdevKeyMapper);

#endif // BOBUI_CONFIG(evdev)

#if BOBUI_CONFIG(vxworksevdev)

/*!
    \class QNativeInterface::Private::QVxKeyMapper
    \since 6.8
    \internal
    \brief Native interface to QKeyMapper.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QVxKeyMapper);

#endif // BOBUI_CONFIG(vxworksevdev)

#if BOBUI_CONFIG(wayland)

/*!
    \class QNativeInterface::QWaylandApplication
    \inheaderfile QGuiApplication
    \since 6.5
    \brief Native interface to a Wayland application.

    Accessed through QGuiApplication::nativeInterface().
    \inmodule BobUIGui
    \ingroup native-interfaces
    \ingroup native-interfaces-qguiapplication
*/
/*!
    \fn wl_display *QNativeInterface::QWaylandApplication::display() const
    \return the wl_display that the application is using.
*/
/*!
    \fn wl_compositor *QNativeInterface::QWaylandApplication::compositor() const
    \return the wl_compositor that the application is using.
*/
/*!
    \fn wl_keyboard *QNativeInterface::QWaylandApplication::keyboard() const
    \return the wl_keyboard belonging to seat() if available.
*/
/*!
    \fn wl_pointer *QNativeInterface::QWaylandApplication::pointer() const
    \return the wl_pointer belonging to seat() if available.
*/
/*!
    \fn wl_touch *QNativeInterface::QWaylandApplication::touch() const
    \return the wl_touch belonging to seat() if available.
*/
/*!
    \fn uint *QNativeInterface::QWaylandApplication::lastInputSerial() const
    \return the serial of the last input event on any seat.
*/
/*!
    \fn wl_seat *QNativeInterface::QWaylandApplication::lastInputSeat() const
    \return the seat on which the last input event happened.
*/
/*!
    \fn wl_seat *QNativeInterface::QWaylandApplication::seat() const
    \return the seat associated with the default input device.
*/

/*!
    \fn struct xkb_context *QNativeInterface::QWaylandApplication::xkbContext() const
    \return the XKB context associated with the application.
*/

BOBUI_DEFINE_NATIVE_INTERFACE(QWaylandApplication);

/*!
    \class QNativeInterface::QWaylandScreen
    \since 6.7
    \brief Native interface to a screen on Wayland.

    Accessed through QScreen::nativeInterface().
    \inmodule BobUIGui
    \ingroup native-interfaces
    \ingroup native-interfaces-qscreen
*/
/*!
    \fn wl_output *QNativeInterface::QWaylandScreen::output() const
    \return the underlying wl_output of this QScreen.
*/
BOBUI_DEFINE_NATIVE_INTERFACE(QWaylandScreen);

/*!
    \class QNativeInterface::QWaylandWindow
    \since 6.5
    \internal
    \brief Native interface to a Wayland window.
    \inmodule BobUIGui
    \ingroup native-interfaces
*/

BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(QWaylandWindow);

#endif // BOBUI_CONFIG(wayland)

BOBUI_END_NAMESPACE
