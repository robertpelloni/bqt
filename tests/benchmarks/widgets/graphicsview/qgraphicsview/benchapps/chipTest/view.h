// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>

BOBUI_FORWARD_DECLARE_CLASS(QGraphicsView)
BOBUI_FORWARD_DECLARE_CLASS(QLabel)
BOBUI_FORWARD_DECLARE_CLASS(QSlider)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIoolButton)

class View : public QFrame
{
    Q_OBJECT
public:
    View(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();

    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();

    void timerEvent(BOBUIimerEvent *);

private:
    QGraphicsView *graphicsView;
    QLabel *label;
    BOBUIoolButton *openGlButton;
    BOBUIoolButton *antialiasButton;
    BOBUIoolButton *printButton;
    BOBUIoolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
};

#endif
