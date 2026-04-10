// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDBRCMEGLWINDOW_H
#define QWAYLANDBRCMEGLWINDOW_H

#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include "qwaylandbrcmeglintegration.h"

#include <QMutex>

#include <EGL/egl.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandBrcmBuffer;

class QWaylandBrcmEglWindow : public QWaylandWindow
{
    Q_OBJECT
public:
    QWaylandBrcmEglWindow(QWindow *window, QWaylandDisplay *display);
    ~QWaylandBrcmEglWindow();
    WindowType windowType() const override;
    void setGeometry(const QRect &rect) override;

    QSurfaceFormat format() const override;

    bool makeCurrent(EGLContext context);
    void swapBuffers();

private:
    void createEglSurfaces();
    void destroyEglSurfaces();

    QWaylandBrcmEglIntegration *m_eglIntegration = nullptr;
    struct wl_egl_window *m_waylandEglWindow = nullptr;

    const QWaylandWindow *m_parentWindow = nullptr;

    EGLint m_globalImages[3*5];
    EGLSurface m_eglSurfaces[3];

    QWaylandBrcmBuffer *m_buffers[3];
    QSurfaceFormat m_format;

    struct wl_event_queue *m_eventQueue = nullptr;

    int m_current = 0;
    int m_count = 0;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDBRCMEGLWINDOW_H
