// Copyright (C) 2014 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDPLATFORMOPENGLWINDOW_H
#define QANDROIDPLATFORMOPENGLWINDOW_H

#include "qandroidplatformwindow.h"

#include <QWaitCondition>
#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjniobject.h>

#include <EGL/egl.h>

BOBUI_BEGIN_NAMESPACE

class QAndroidPlatformOpenGLWindow : public QAndroidPlatformWindow
{
public:
    explicit QAndroidPlatformOpenGLWindow(QWindow *window, EGLDisplay display);
    ~QAndroidPlatformOpenGLWindow();

    void setGeometry(const QRect &rect) override;
    EGLSurface eglSurface(EGLConfig config);
    bool ensureEglSurfaceCreated(EGLConfig config);
    QSurfaceFormat format() const override;

    void applicationStateChanged(BobUI::ApplicationState) override;

protected:
    void clearSurface() override;

private:
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    EGLSurface m_eglSurface = EGL_NO_SURFACE;
    EGLNativeWindowType m_nativeWindow = nullptr;

    QSurfaceFormat m_format;
};

BOBUI_END_NAMESPACE
#endif // QANDROIDPLATFORMOPENGLWINDOW_H
