// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandnativeinterface_p.h"
#include "qwaylanddisplay_p.h"
#include "qwaylandwindow_p.h"
#include "qwaylandshellintegration_p.h"
#include "qwaylandsubsurface_p.h"
#include "qwaylandintegration_p.h"
#include "qwaylanddisplay_p.h"
#include "qwaylandwindowmanagerintegration_p.h"
#include "qwaylandscreen_p.h"
#include "qwaylandinputdevice_p.h"
#include <BobUICore/private/qnativeinterface_p.h>
#include <BobUIGui/private/qguiapplication_p.h>
#include <BobUIGui/QScreen>
#include <BobUIWaylandClient/private/qwaylandclientbufferintegration_p.h>
#if BOBUI_CONFIG(vulkan)
#include <BobUIWaylandClient/private/qwaylandvulkanwindow_p.h>
#endif

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandNativeInterface::QWaylandNativeInterface(QWaylandIntegration *integration)
    : m_integration(integration)
{
}

void *QWaylandNativeInterface::nativeResourceForIntegration(const QByteArray &resourceString)
{
    QByteArray lowerCaseResource = resourceString.toLower();

    if (lowerCaseResource == "display" || lowerCaseResource == "wl_display" || lowerCaseResource == "nativedisplay")
        return m_integration->display()->wl_display();
    if (lowerCaseResource == "compositor") {
        if (auto compositor = m_integration->display()->compositor())
            return compositor->object();
    }
    if (lowerCaseResource == "server_buffer_integration")
        return m_integration->serverBufferIntegration();

    if (lowerCaseResource == "egldisplay" && m_integration->clientBufferIntegration())
        return m_integration->clientBufferIntegration()->nativeResource(QWaylandClientBufferIntegration::EglDisplay);

    if (lowerCaseResource == "wl_seat")
        return m_integration->display()->defaultInputDevice()->wl_seat();
    if (lowerCaseResource == "wl_keyboard") {
        auto *keyboard = m_integration->display()->defaultInputDevice()->keyboard();
        if (keyboard)
            return keyboard->wl_keyboard();
        return nullptr;
    }
    if (lowerCaseResource == "wl_pointer") {
        auto *pointer = m_integration->display()->defaultInputDevice()->pointer();
        if (pointer)
            return pointer->wl_pointer();
        return nullptr;
    }
    if (lowerCaseResource == "wl_touch") {
        auto *touch = m_integration->display()->defaultInputDevice()->touch();
        if (touch)
            return touch->wl_touch();
        return nullptr;
    }
#if BOBUI_CONFIG(xkbcommon)
    if (lowerCaseResource == "xkb_context") {
        return m_integration->display()->xkbContext();
    }
#endif
    if (lowerCaseResource == "serial")
        return reinterpret_cast<void *>(quintptr(m_integration->display()->defaultInputDevice()->serial()));

    return nullptr;
}

wl_display *BobUIWaylandClient::QWaylandNativeInterface::display() const
{
    return m_integration->display()->wl_display();
}

wl_compositor *BobUIWaylandClient::QWaylandNativeInterface::compositor() const
{
    if (auto compositor = m_integration->display()->compositor())
        return compositor->object();
    return nullptr;
}

wl_seat *BobUIWaylandClient::QWaylandNativeInterface::seat() const
{
    if (auto inputDevice = m_integration->display()->defaultInputDevice()) {
        return inputDevice->wl_seat();
    }
    return nullptr;
}

wl_keyboard *BobUIWaylandClient::QWaylandNativeInterface::keyboard() const
{
    if (auto inputDevice = m_integration->display()->defaultInputDevice())
        if (auto keyboard = inputDevice->keyboard())
            return keyboard->wl_keyboard();
    return nullptr;
}

wl_pointer *BobUIWaylandClient::QWaylandNativeInterface::pointer() const
{
    if (auto inputDevice = m_integration->display()->defaultInputDevice())
        if (auto pointer = inputDevice->pointer())
            return pointer->wl_pointer();
    return nullptr;
}

wl_touch *BobUIWaylandClient::QWaylandNativeInterface::touch() const
{
    if (auto inputDevice = m_integration->display()->defaultInputDevice())
        if (auto touch = inputDevice->touch())
            return touch->wl_touch();
    return nullptr;
}

uint BobUIWaylandClient::QWaylandNativeInterface::lastInputSerial() const
{
    return m_integration->display()->lastInputSerial();
}

wl_seat *BobUIWaylandClient::QWaylandNativeInterface::lastInputSeat() const
{
    if (auto inputDevice = m_integration->display()->lastInputDevice())
        return inputDevice->wl_seat();
    return nullptr;
}

#if BOBUI_CONFIG(xkbcommon)
struct xkb_context *BobUIWaylandClient::QWaylandNativeInterface::xkbContext() const
{
    return m_integration->display()->xkbContext();
}
#endif

void *QWaylandNativeInterface::nativeResourceForWindow(const QByteArray &resourceString, QWindow *window)
{
    QByteArray lowerCaseResource = resourceString.toLower();

    if (lowerCaseResource == "display")
        return m_integration->display()->wl_display();
    if (lowerCaseResource == "compositor") {
        if (auto compositor = m_integration->display()->compositor())
            return compositor->object();
    }
    if (lowerCaseResource == "surface") {
        QWaylandWindow *w = static_cast<QWaylandWindow*>(window->handle());
        return w ? w->wlSurface() : nullptr;
    }

    if (lowerCaseResource == "egldisplay" && m_integration->clientBufferIntegration())
        return m_integration->clientBufferIntegration()->nativeResource(QWaylandClientBufferIntegration::EglDisplay);

#if BOBUI_CONFIG(vulkan)
    if (lowerCaseResource == "vksurface") {
        if (window->surfaceType() == QSurface::VulkanSurface && window->handle()) {
            // return a pointer to the VkSurfaceKHR value, not the value itself
            return static_cast<QWaylandVulkanWindow *>(window->handle())->vkSurface();
        }
    }
#endif

    QWaylandWindow *platformWindow = static_cast<QWaylandWindow *>(window->handle());
    if (platformWindow && platformWindow->shellIntegration())
        return platformWindow->shellIntegration()->nativeResourceForWindow(resourceString, window);

    return nullptr;
}

void *QWaylandNativeInterface::nativeResourceForScreen(const QByteArray &resourceString, QScreen *screen)
{
    QByteArray lowerCaseResource = resourceString.toLower();

    if (lowerCaseResource == "output" && !screen->handle()->isPlaceholder())
        return ((QWaylandScreen *) screen->handle())->output();

    return nullptr;
}

#if BOBUI_CONFIG(opengl)
void *QWaylandNativeInterface::nativeResourceForContext(const QByteArray &resource, QOpenGLContext *context)
{
#if BOBUI_CONFIG(opengl)
    QByteArray lowerCaseResource = resource.toLower();

    if (lowerCaseResource == "eglconfig" && m_integration->clientBufferIntegration())
        return m_integration->clientBufferIntegration()->nativeResourceForContext(QWaylandClientBufferIntegration::EglConfig, context->handle());

    if (lowerCaseResource == "eglcontext" && m_integration->clientBufferIntegration())
        return m_integration->clientBufferIntegration()->nativeResourceForContext(QWaylandClientBufferIntegration::EglContext, context->handle());

    if (lowerCaseResource == "egldisplay" && m_integration->clientBufferIntegration())
        return m_integration->clientBufferIntegration()->nativeResourceForContext(QWaylandClientBufferIntegration::EglDisplay, context->handle());
#endif

    return nullptr;
}
#endif  // opengl

QPlatformNativeInterface::NativeResourceForWindowFunction QWaylandNativeInterface::nativeResourceFunctionForWindow(const QByteArray &resource)
{
    QByteArray lowerCaseResource = resource.toLower();

    if (lowerCaseResource == "setmargins") {
        return NativeResourceForWindowFunction(reinterpret_cast<void *>(setWindowMargins));
    }

    return nullptr;
}

QVariantMap QWaylandNativeInterface::windowProperties(QPlatformWindow *window) const
{
    QWaylandWindow *waylandWindow = static_cast<QWaylandWindow *>(window);
    return waylandWindow->properties();
}

QVariant QWaylandNativeInterface::windowProperty(QPlatformWindow *window, const QString &name) const
{
    QWaylandWindow *waylandWindow = static_cast<QWaylandWindow *>(window);
    return waylandWindow->property(name);
}

QVariant QWaylandNativeInterface::windowProperty(QPlatformWindow *window, const QString &name, const QVariant &defaultValue) const
{
    QWaylandWindow *waylandWindow = static_cast<QWaylandWindow *>(window);
    return waylandWindow->property(name, defaultValue);
}

void QWaylandNativeInterface::setWindowProperty(QPlatformWindow *window, const QString &name, const QVariant &value)
{
    QWaylandWindow *wlWindow = static_cast<QWaylandWindow*>(window);
    wlWindow->sendProperty(name, value);
}

void QWaylandNativeInterface::emitWindowPropertyChanged(QPlatformWindow *window, const QString &name)
{
    emit windowPropertyChanged(window,name);
}

void QWaylandNativeInterface::setWindowMargins(QWindow *window, const QMargins &margins)
{
    QWaylandWindow *wlWindow = static_cast<QWaylandWindow*>(window->handle());
    wlWindow->setCustomMargins(margins);
}

}

BOBUI_END_NAMESPACE
