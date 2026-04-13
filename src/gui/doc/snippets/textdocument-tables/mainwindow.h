// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class BOBUIextEdit;
class BOBUIextFrame;
class BOBUIextBlock;
class BOBUIextTable;

BOBUI_BEGIN_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void saveFile();
    void showTable();

private:
    bool writeXml(const QString &fileName);
    void processFrame(BOBUIextFrame *);
    void processBlock(BOBUIextBlock);
    void processTable(BOBUIextTable *table);

    BOBUIextEdit *editor = nullptr;
};

BOBUI_END_NAMESPACE

#endif
