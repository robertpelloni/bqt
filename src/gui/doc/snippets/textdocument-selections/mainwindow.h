// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <BOBUIextDocumentFragment>

BOBUI_BEGIN_NAMESPACE

class QAction;
class BOBUIextDocument;
class BOBUIextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void cutSelection();
    void copySelection();
    void openFile();
    void pasteSelection();
    void selectWord();
    void selectLine();
    void selectBlock();
    void selectFrame();
    void updateMenus();

private:
    QAction *cutAction = nullptr;
    QAction *copyAction = nullptr;
    QAction *pasteAction = nullptr;
    QString currentFile;
    BOBUIextEdit *editor = nullptr;
    BOBUIextDocument *document = nullptr;
    BOBUIextDocumentFragment selection;
};

BOBUI_END_NAMESPACE

#endif
