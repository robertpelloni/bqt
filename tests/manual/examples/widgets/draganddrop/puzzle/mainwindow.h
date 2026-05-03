// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

class PiecesList;
class PuzzleWidget;
BOBUI_BEGIN_NAMESPACE
class QListWidgetItem;
BOBUI_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void loadImage(const QString &path);

public slots:
    void openImage();
    void setupPuzzle();

private slots:
    void setCompleted();

private:
    void setupMenus();
    void setupWidgets();

    QPixmap puzzleImage;
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;
};

#endif // MAINWINDOW_H
