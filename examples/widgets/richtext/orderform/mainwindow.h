// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QPair>

BOBUI_BEGIN_NAMESPACE
class QAction;
class BOBUIabWidget;
BOBUI_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void createSample();

public slots:
    void openDialog();
    void printFile();

private:
    void createLetter(const QString &name, const QString &address,
                      QList<QPair<QString,int> > orderItems,
                      bool sendOffers);

    QAction *printAction;
    BOBUIabWidget *letters;
};
//! [0]

#endif // MAINWINDOW_H
