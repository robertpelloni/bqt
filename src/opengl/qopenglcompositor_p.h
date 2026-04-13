// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOPENGLCOMPOSITOR_H
#define QOPENGLCOMPOSITOR_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIOpenGL/bobuiopenglglobal.h>

#include <BobUICore/BOBUIimer>
#include <BobUIOpenGL/QOpenGLTextureBlitter>
#include <BobUIGui/QMatrix4x4>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLContext;
class QOpenGLFramebufferObject;
class QWindow;
class QPlatformTextureList;

class QOpenGLCompositorBackingStore;
class QOpenGLCompositorWindow
{
public:
    virtual ~QOpenGLCompositorWindow() { }
    virtual QWindow *sourceWindow() const = 0;
    virtual const QPlatformTextureList *textures() const = 0;
    virtual void beginCompositing() { }
    virtual void endCompositing() { }
    virtual void setBackingStore(QOpenGLCompositorBackingStore *backingStore) = 0;
    virtual QOpenGLCompositorBackingStore *backingStore() const = 0;
};

class Q_OPENGL_EXPORT QOpenGLCompositor : public QObject
{
    Q_OBJECT

public:
    enum GrabOrientation {
        Flipped,
        NotFlipped,
    };

    static QOpenGLCompositor *instance();
    static void destroy();

    void setTargetWindow(QWindow *window, const QRect &nativeTargetGeometry);
    void setTargetContext(QOpenGLContext *context);
    void setRotation(int degrees);
    QOpenGLContext *context() const { return m_context; }
    QWindow *targetWindow() const { return m_targetWindow; }
    QRect nativeTargetGeometry() const { return m_nativeTargetGeometry; }

    void update();
    QImage grab();

    bool grabToFrameBufferObject(QOpenGLFramebufferObject *fbo, GrabOrientation orientation = Flipped);

    QList<QOpenGLCompositorWindow *> windows() const { return m_windows; }
    void addWindow(QOpenGLCompositorWindow *window);
    void removeWindow(QOpenGLCompositorWindow *window);
    void moveToTop(QOpenGLCompositorWindow *window);
    void changeWindowIndex(QOpenGLCompositorWindow *window, int newIdx);

signals:
    void topWindowChanged(QOpenGLCompositorWindow *window);

private slots:
    void handleRenderAllRequest();

private:
    QOpenGLCompositor();
    ~QOpenGLCompositor();

    void renderAll(QOpenGLFramebufferObject *fbo,
                   QOpenGLTextureBlitter::Origin origin = QOpenGLTextureBlitter::OriginTopLeft);
    void render(QOpenGLCompositorWindow *window,
                QOpenGLTextureBlitter::Origin origin = QOpenGLTextureBlitter::OriginTopLeft);
    void ensureCorrectZOrder();

    QOpenGLContext *m_context;
    QWindow *m_targetWindow;
    QRect m_nativeTargetGeometry;
    int m_rotation;
    QMatrix4x4 m_rotationMatrix;
    BOBUIimer m_updateTimer;
    QOpenGLTextureBlitter m_blitter;
    QList<QOpenGLCompositorWindow *> m_windows;
};

BOBUI_END_NAMESPACE

#endif // QOPENGLCOMPOSITOR_H
