// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPoint>

class QComboBox;
class QLabel;
class QLineEdit;
class QMouseEvent;
class BOBUIextEdit;
class DragWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void setDragResult(const QString &actionText);
    void setMimeTypes(const QStringList &types);

private:
    QComboBox *mimeTypeCombo;
    DragWidget *dragWidget;
};

#endif
