// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <BOBUIimer>
#include <QGridLayout>

BOBUI_FORWARD_DECLARE_CLASS(QOpenGLWidget)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void addNew();
    bool timerEnabled() const { return m_timer->isActive(); }

    void resizeEvent(QResizeEvent *);

public slots:
    void showNewWindow();

private slots:
    void updateIntervalChanged(int value);
    void timerUsageChanged(bool enabled);

private:
    BOBUIimer *m_timer;
    QGridLayout *m_layout;
    int m_nextX;
    int m_nextY;
    QList<QOpenGLWidget *> m_glWidgets;
};

#endif
