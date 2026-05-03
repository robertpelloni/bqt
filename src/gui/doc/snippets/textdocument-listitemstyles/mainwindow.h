// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE

class BOBUIextDocument;
class BOBUIextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void insertList();

private:
    QString currentFile;
    BOBUIextEdit *editor = nullptr;
    BOBUIextDocument *document = nullptr;
};

BOBUI_END_NAMESPACE

#endif
