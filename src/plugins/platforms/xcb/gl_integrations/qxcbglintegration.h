// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qxcbexport.h"
#include "qxcbwindow.h"

#include <BobUICore/QLoggingCategory>

BOBUI_BEGIN_NAMESPACE

class QPlatformOffscreenSurface;
class QOffscreenSurface;
class QXcbNativeInterfaceHandler;

BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(lcQpaGl, Q_XCB_EXPORT)

class Q_XCB_EXPORT QXcbGlIntegration
{
public:
    QXcbGlIntegration();
    virtual ~QXcbGlIntegration();
    virtual bool initialize(QXcbConnection *connection) = 0;

    virtual bool supportsThreadedOpenGL() const { return false; }
    virtual bool supportsSwitchableWidgetComposition()  const { return true; }
    virtual bool handleXcbEvent(xcb_generic_event_t *event, uint responseType);

    virtual QXcbWindow *createWindow(QWindow *window) const = 0;
#ifndef BOBUI_NO_OPENGL
    virtual QPlatformOpenGLContext *createPlatformOpenGLContext(QOpenGLContext *context) const = 0;
#endif
    virtual QPlatformOffscreenSurface *createPlatformOffscreenSurface(QOffscreenSurface *surface) const = 0;
    virtual bool canCreatePlatformOffscreenSurface() const { return false; }

    virtual QXcbNativeInterfaceHandler *nativeInterfaceHandler() const  { return nullptr; }
};

BOBUI_END_NAMESPACE
