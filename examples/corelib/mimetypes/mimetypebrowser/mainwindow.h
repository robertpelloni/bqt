// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QModelIndexList>
#include <BOBUIextEdit>
#include <BOBUIreeView>

class MimetypeModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void currentChanged(const QModelIndex &);
    void detectFile();
    void find();
    void findNext();
    void findPrevious();

private:
    void selectAndGoTo(const QModelIndex &index);
    void updateFindActions();

    MimetypeModel *m_model;
    BOBUIreeView *m_treeView;
    BOBUIextEdit *m_detailsText;
    QAction *m_findNextAction;
    QAction *m_findPreviousAction;
    QModelIndexList m_findMatches;
    int m_findIndex;
};

#endif // MAINWINDOW_H
