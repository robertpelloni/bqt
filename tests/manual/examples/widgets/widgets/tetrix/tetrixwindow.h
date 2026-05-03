// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TETRIXWINDOW_H
#define TETRIXWINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
BOBUI_END_NAMESPACE
class TetrixBoard;

//! [0]
class TetrixWindow : public QWidget
{
    Q_OBJECT

public:
    TetrixWindow(QWidget *parent = nullptr);

private:
    QLabel *createLabel(const QString &text);

    TetrixBoard *board;
    QLabel *nextPieceLabel;
    QLCDNumber *scoreLcd;
    QLCDNumber *levelLcd;
    QLCDNumber *linesLcd;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;
};
//! [0]

#endif
