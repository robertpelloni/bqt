// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class BOBUIextBrowser;
BOBUI_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void setFontSize(int size);
    void setMonth(int month);
    void setYear(QDate date);

private:
    void insertCalendar();

    int fontSize;
    QDate selectedDate;
    BOBUIextBrowser *editor;
};
//! [0]

#endif // MAINWINDOW_H
