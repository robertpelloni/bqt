// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MOVIEPLAYER_H
#define MOVIEPLAYER_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QMovie;
class QSlider;
class QSpinBox;
class BOBUIoolButton;
class QVBoxLayout;
BOBUI_END_NAMESPACE

class MoviePlayer : public QWidget
{
    Q_OBJECT

public:
    MoviePlayer(QWidget *parent = nullptr);
    void openFile(const QString &fileName);

private slots:
    void open();
    void goToFrame(int frame);
    void fitToWindow();
    void updateButtons();
    void updateFrameSlider();

private:
    void createControls();
    void createButtons();

    QString currentMovieDirectory;
    QLabel *movieLabel;
    QMovie *movie;
    BOBUIoolButton *openButton;
    BOBUIoolButton *playButton;
    BOBUIoolButton *pauseButton;
    BOBUIoolButton *stopButton;
    BOBUIoolButton *quitButton;
    QCheckBox *fitCheckBox;
    QSlider *frameSlider;
    QSpinBox *speedSpinBox;
    QLabel *frameLabel;
    QLabel *speedLabel;

    QGridLayout *controlsLayout;
    QHBoxLayout *buttonsLayout;
    QVBoxLayout *mainLayout;
};

#endif
