// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QWindow>
#include <QImage>

class Window : public QWindow
{
public:
    Window(QWindow *parent = nullptr);
    Window(QScreen *screen);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *);

    void exposeEvent(QExposeEvent *);
    void resizeEvent(QResizeEvent *);

    void timerEvent(BOBUIimerEvent *);

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
    QPoint scrollOffset;
};
