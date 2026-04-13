// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOPENGLWINDOW_H
#define QOPENGLWINDOW_H

#include <BobUIOpenGL/bobuiopenglglobal.h>

#ifndef BOBUI_NO_OPENGL

#include <BobUIGui/QPaintDeviceWindow>
#include <BobUIGui/QOpenGLContext>
#include <BobUIGui/QImage>

BOBUI_BEGIN_NAMESPACE

class QOpenGLWindowPrivate;

class Q_OPENGL_EXPORT QOpenGLWindow : public QPaintDeviceWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QOpenGLWindow)

public:
    enum UpdateBehavior {
        NoPartialUpdate,
        PartialUpdateBlit,
        PartialUpdateBlend
    };

    explicit QOpenGLWindow(UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = nullptr);
    explicit QOpenGLWindow(QOpenGLContext *shareContext, UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = nullptr);
    ~QOpenGLWindow();

    UpdateBehavior updateBehavior() const;
    bool isValid() const;

    void makeCurrent();
    void doneCurrent();

    QOpenGLContext *context() const;
    QOpenGLContext *shareContext() const;

    GLuint defaultFramebufferObject() const;

    QImage grabFramebuffer();

Q_SIGNALS:
    void frameSwapped();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void paintUnderGL();
    virtual void paintOverGL();

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    int metric(PaintDeviceMetric metric) const override;
    QPaintDevice *redirected(QPoint *) const override;

private:
    Q_DISABLE_COPY(QOpenGLWindow)
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_OPENGL

#endif
