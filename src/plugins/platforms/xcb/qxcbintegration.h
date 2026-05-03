// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformscreen.h>
#include <qpa/qplatformopenglcontext.h>

#include "qxcbexport.h"

#include <xcb/xcb.h>

BOBUI_BEGIN_NAMESPACE

class QXcbConnection;
class QAbstractEventDispatcher;
class QXcbNativeInterface;
class QXcbUnixServices;

class Q_XCB_EXPORT QXcbIntegration : public QPlatformIntegration
#ifndef BOBUI_NO_OPENGL
# if BOBUI_CONFIG(xcb_glx_plugin)
    , public QNativeInterface::Private::QGLXIntegration
# endif
# if BOBUI_CONFIG(egl)
    , public QNativeInterface::Private::QEGLIntegration
# endif
#endif
{
public:
    QXcbIntegration(const QStringList &parameters, int &argc, char **argv);
    ~QXcbIntegration();

    QPlatformPixmap *createPlatformPixmap(QPlatformPixmap::PixelType type) const override;
    QPlatformWindow *createPlatformWindow(QWindow *window) const override;
    QPlatformWindow *createForeignWindow(QWindow *window, WId nativeHandle) const override;
#ifndef BOBUI_NO_OPENGL
    QPlatformOpenGLContext *createPlatformOpenGLContext(QOpenGLContext *context) const override;
# if BOBUI_CONFIG(xcb_glx_plugin)
    QOpenGLContext *createOpenGLContext(GLXContext context, void *visualInfo, QOpenGLContext *shareContext) const override;
# endif
# if BOBUI_CONFIG(egl)
    QOpenGLContext *createOpenGLContext(EGLContext context, EGLDisplay display, QOpenGLContext *shareContext) const override;
# endif
#endif
    QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const override;

    QPlatformOffscreenSurface *createPlatformOffscreenSurface(QOffscreenSurface *surface) const override;

    bool hasCapability(Capability cap) const override;
    QAbstractEventDispatcher *createEventDispatcher() const override;
    void initialize() override;

    void moveToScreen(QWindow *window, int screen);

    QPlatformFontDatabase *fontDatabase() const override;

    QPlatformNativeInterface *nativeInterface()const override;

#ifndef BOBUI_NO_CLIPBOARD
    QPlatformClipboard *clipboard() const override;
#endif
#if BOBUI_CONFIG(draganddrop)
    QPlatformDrag *drag() const override;
#endif

    QPlatformInputContext *inputContext() const override;

#if BOBUI_CONFIG(accessibility)
    QPlatformAccessibility *accessibility() const override;
#endif

    QPlatformServices *services() const override;

    QPlatformKeyMapper *keyMapper() const override;

    QStringList themeNames() const override;
    QPlatformTheme *createPlatformTheme(const QString &name) const override;
    QVariant styleHint(StyleHint hint) const override;

    bool hasConnection() const { return m_connection; }
    QXcbConnection *connection() const { return m_connection; }

    QByteArray wmClass() const;

#if BOBUI_CONFIG(xcb_sm)
    QPlatformSessionManager *createPlatformSessionManager(const QString &id, const QString &key) const override;
#endif

    void sync() override;

    void beep() const override;

#if BOBUI_CONFIG(vulkan)
    QPlatformVulkanInstance *createPlatformVulkanInstance(QVulkanInstance *instance) const override;
#endif

    static QXcbIntegration *instance() { return m_instance; }

    void setApplicationBadge(qint64 number) override;

private:
    QXcbConnection *m_connection = nullptr;

    QScopedPointer<QPlatformFontDatabase> m_fontDatabase;
    QScopedPointer<QXcbNativeInterface> m_nativeInterface;

    QScopedPointer<QPlatformInputContext> m_inputContext;

#if BOBUI_CONFIG(accessibility)
    mutable QScopedPointer<QPlatformAccessibility> m_accessibility;
#endif

    QScopedPointer<QXcbUnixServices> m_services;

    mutable QByteArray m_wmClass;
    const char *m_instanceName;
    bool m_canGrab;
    xcb_visualid_t m_defaultVisualId;

    static QXcbIntegration *m_instance;
};

BOBUI_END_NAMESPACE
