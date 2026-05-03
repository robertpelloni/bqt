// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class BOBUIreeView; // forward declarations
class QStandardItemModel;
class QItemSelection;
BOBUI_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void selectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection);

private:
    BOBUIreeView *treeView;
    QStandardItemModel *standardModel;
};

#endif // MAINWINDOW_H
