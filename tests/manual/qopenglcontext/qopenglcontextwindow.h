// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QOPENGLCONTEXTWINDOW_H
#define QOPENGLCONTEXTWINDOW_H

#include <BobUIGui/QWindow>
#include <BobUIGui/QOpenGLContext>
#include <BobUIOpenGL/QOpenGLTextureBlitter>
#include <BobUIGui/QImage>
#include <BobUICore/QVariant>

class QOpenGLContextWindow : public QWindow
{
    Q_OBJECT

public:
    QOpenGLContextWindow();
    ~QOpenGLContextWindow();

    void render();

protected:
    void exposeEvent(QExposeEvent *event);

private:
    qreal dWidth() const { return width() * devicePixelRatio(); }
    qreal dHeight() const { return height() * devicePixelRatio(); }
    void createForeignContext();

    QOpenGLContext *m_context;
    QImage m_image;
    QVariant m_nativeHandle;
    uint m_textureId;
    QOpenGLTextureBlitter *m_blitter;
};

#endif
