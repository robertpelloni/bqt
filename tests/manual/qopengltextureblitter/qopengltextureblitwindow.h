// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QOPENGLTEXTUREBLITWINDOW_H
#define QOPENGLTEXTUREBLITWINDOW_H

#include <BobUIGui/QWindow>
#include <BobUIGui/QOpenGLContext>
#include <BobUIGui/QOpenGLTextureBlitter>

class QOpenGLTextureBlitWindow : public QWindow
{
    Q_OBJECT
public:
    QOpenGLTextureBlitWindow();

    void render();
protected:
    void exposeEvent(QExposeEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    qreal dWidth() const { return width() * devicePixelRatio(); }
    qreal dHeight() const { return height() * devicePixelRatio(); }

    QScopedPointer<QOpenGLContext> m_context;
    QOpenGLTextureBlitter m_blitter;
    QImage m_image;
    QImage m_image_mirrord;
};

#endif
