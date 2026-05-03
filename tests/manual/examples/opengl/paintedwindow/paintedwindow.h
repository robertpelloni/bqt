// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PAINTEDWINDOW_H
#define PAINTEDWINDOW_H

#include <QWindow>

#include <BobUIGui/qopengl.h>
#include <BobUIOpenGL/qopenglshaderprogram.h>
#include <BobUIOpenGL/qopenglframebufferobject.h>

#include <QPropertyAnimation>

#include <QColor>
#include <QImage>
#include <BOBUIime>

BOBUI_BEGIN_NAMESPACE
class QOpenGLContext;
BOBUI_END_NAMESPACE

class PaintedWindow : public QWindow
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    PaintedWindow();

    qreal rotation() const { return m_rotation; }

signals:
    void rotationChanged(qreal rotation);

private slots:
    void paint();
    void setRotation(qreal r);
    void orientationChanged(BobUI::ScreenOrientation newOrientation);
    void rotationDone();

private:
    void exposeEvent(QExposeEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

    void paint(QPainter *painter, const QRect &rect);

    QOpenGLContext *m_context;
    qreal m_rotation;

    QImage m_prevImage;
    QImage m_nextImage;
    qreal m_deltaRotation;

    BobUI::ScreenOrientation m_targetOrientation;
    BobUI::ScreenOrientation m_nextTargetOrientation;

    QPropertyAnimation *m_animation;
    BOBUIimer *m_paintTimer;
};

#endif // PAINTEDWINDOW_H
