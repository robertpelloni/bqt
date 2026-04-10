// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOPENGLWIDGET_H
#define QOPENGLWIDGET_H

#include <BobUIOpenGLWidgets/bobuiopenglwidgetsglobal.h>

#include <BobUIWidgets/QWidget>
#include <BobUIGui/QSurfaceFormat>
#include <BobUIGui/qopengl.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLWidgetPrivate;

class Q_OPENGLWIDGETS_EXPORT QOpenGLWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QOpenGLWidget)

public:
    enum UpdateBehavior {
        NoPartialUpdate,
        PartialUpdate
    };
    Q_ENUM(UpdateBehavior)

    enum TargetBuffer : uint8_t {
        LeftBuffer = 0,   // Default
        RightBuffer       // Only used when QSurfaceFormat::StereoBuffers is enabled
    };
    Q_ENUM(TargetBuffer)

    explicit QOpenGLWidget(QWidget* parent = nullptr, BobUI::WindowFlags f = BobUI::WindowFlags());
    ~QOpenGLWidget();

    void setUpdateBehavior(UpdateBehavior updateBehavior);
    UpdateBehavior updateBehavior() const;

    void setFormat(const QSurfaceFormat &format);
    QSurfaceFormat format() const;

    GLenum textureFormat() const;
    void setTextureFormat(GLenum texFormat);

    bool isValid() const;

    void makeCurrent();
    void makeCurrent(TargetBuffer targetBuffer);
    void doneCurrent();

    QOpenGLContext *context() const;
    GLuint defaultFramebufferObject() const;
    GLuint defaultFramebufferObject(TargetBuffer targetBuffer) const;

    QImage grabFramebuffer();
    QImage grabFramebuffer(TargetBuffer targetBuffer);

    TargetBuffer currentTargetBuffer() const;

Q_SIGNALS:
    void aboutToCompose();
    void frameSwapped();
    void aboutToResize();
    void resized();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    bool event(QEvent *e) override;

    int metric(QPaintDevice::PaintDeviceMetric metric) const override;
    QPaintDevice *redirected(QPoint *p) const override;
    QPaintEngine *paintEngine() const override;

private:
    Q_DISABLE_COPY(QOpenGLWidget)
};

BOBUI_END_NAMESPACE

#endif // QOPENGLWIDGET_H
