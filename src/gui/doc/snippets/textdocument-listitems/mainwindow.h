// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QList>
#include <QMainWindow>
#include <QString>

BOBUI_BEGIN_NAMESPACE

class QAction;
class BOBUIextDocument;
class BOBUIextEdit;
class BOBUIextList;
class BOBUIreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void insertList();
    void highlightListItems();
    void showList();

private:
    QString currentFile;
    BOBUIextEdit *editor = nullptr;
    BOBUIextDocument *document = nullptr;
    QList<BOBUIextList*> listStructures;
    QList<BOBUIreeWidgetItem*> previousItems;
    QList<BOBUIreeWidgetItem*> parentItems;
};

BOBUI_END_NAMESPACE

#endif
