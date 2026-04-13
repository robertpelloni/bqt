// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QWindow>
#include <QImage>

class Window : public QWindow
{
public:
    Window(QWindow *parent = nullptr);
    Window(QScreen *screen);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    void keyPressEvent(QKeyEvent *) override;

    void exposeEvent(QExposeEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    void timerEvent(BOBUIimerEvent *) override;

private:
    void render();
    void scheduleRender();
    void initialize();

    QString m_text;
    QImage m_image;
    QPoint m_lastPos;
    int m_backgroundColorIndex;
    QBackingStore *m_backingStore;
    int m_renderTimer;
};

#endif // WINDOW_H
