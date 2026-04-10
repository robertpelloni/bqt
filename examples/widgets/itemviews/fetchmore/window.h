// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QModelIndex;
class QListView;
class QPlainTextEdit;
BOBUI_END_NAMESPACE

class FileListModel;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

public slots:
    void updateLog(const QString &path, int start, int number, int total);
    void activated(const QModelIndex &);

private:
    QPlainTextEdit *logViewer;
    FileListModel *model;
    QListView *view;
};

#endif // WINDOW_H
