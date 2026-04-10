// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef CHIPTESTER_H
#define CHIPTESTER_H

#include <BobUIWidgets/QGraphicsView>
#include <BobUICore/QEventLoop>

BOBUI_FORWARD_DECLARE_CLASS(QGraphicsScene)
BOBUI_FORWARD_DECLARE_CLASS(QGraphicsView)
BOBUI_FORWARD_DECLARE_CLASS(QLabel)
BOBUI_FORWARD_DECLARE_CLASS(QSlider)
BOBUI_FORWARD_DECLARE_CLASS(QSplitter)

class ChipTester : public QGraphicsView
{
    Q_OBJECT
public:
    enum Operation {
        Rotate360,
        ZoomInOut,
        Translate
    };
    ChipTester(QWidget *parent = nullptr);

    void setAntialias(bool enabled);
    void runBenchmark();
    void setOperation(Operation operation);

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(BOBUIimerEvent *event) override;

private:
    void populateScene();

    QGraphicsView *view;
    QGraphicsScene *scene;
    int npaints;
    int timerId;
    QEventLoop eventLoop;
    Operation operation;
};

#endif
