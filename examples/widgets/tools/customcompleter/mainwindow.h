// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class QAbstractItemModel;
class QCompleter;
BOBUI_END_NAMESPACE
class TextEdit;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void about();

private:
    void createMenu();
    QAbstractItemModel *modelFromFile(const QString& fileName);

    QCompleter *completer = nullptr;
    TextEdit *completingTextEdit;
};
//! [0]

#endif // MAINWINDOW_H
