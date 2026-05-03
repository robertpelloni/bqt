// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbglintegration.h"

BOBUI_BEGIN_NAMESPACE

class QXcbNativeInterfaceHandler;

class QXcbGlxIntegration : public QXcbGlIntegration,
                           public QNativeInterface::Private::QGLXIntegration
{
public:
    QXcbGlxIntegration();
    ~QXcbGlxIntegration();

    bool initialize(QXcbConnection *connection) override;
    bool handleXcbEvent(xcb_generic_event_t *event, uint responseType) override;

    QXcbWindow *createWindow(QWindow *window) const override;
    QPlatformOpenGLContext *createPlatformOpenGLContext(QOpenGLContext *context) const override;
    QPlatformOffscreenSurface *createPlatformOffscreenSurface(QOffscreenSurface *surface) const override;
    bool canCreatePlatformOffscreenSurface() const override;
    QOpenGLContext *createOpenGLContext(GLXContext context, void *visualInfo, QOpenGLContext *shareContext) const override;

    bool supportsThreadedOpenGL() const override;
    bool supportsSwitchableWidgetComposition() const override;

private:
    QXcbConnection *m_connection;
    uint32_t m_glx_first_event;

    QScopedPointer<QXcbNativeInterfaceHandler> m_native_interface_handler;
};

BOBUI_END_NAMESPACE
