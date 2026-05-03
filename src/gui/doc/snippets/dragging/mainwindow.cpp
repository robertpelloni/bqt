// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <BobUIWidgets>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QFrame *centralFrame = new QFrame(this);

    QLabel *nameLabel = new QLabel(tr("Comment:"), centralFrame);
    commentEdit = new BOBUIextEdit(centralFrame);
    QLabel *dragLabel = new QLabel(tr("<p>Drag the icon to a filer "
                                      "window or the desktop background:</p>"),
                                      centralFrame);
    iconLabel = new QLabel(centralFrame);
    iconPixmap.load(":/images/file.png");
    iconLabel->setPixmap(iconPixmap);

    QGridLayout *grid = new QGridLayout(centralFrame);
    grid->addWidget(nameLabel, 0, 0);
    grid->addWidget(commentEdit, 1, 0, 1, 2);
    grid->addWidget(dragLabel, 2, 0);
    grid->addWidget(iconLabel, 2, 1);

    statusBar();
    setCentralWidget(centralFrame);
    setWindowTitle(tr("Dragging"));
}

//! [0]
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == BobUI::LeftButton
        && iconLabel->geometry().contains(event->pos())) {

//! [1]
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(commentEdit->toPlainText());
        drag->setMimeData(mimeData);
//! [1]
        drag->setPixmap(iconPixmap);

        BobUI::DropAction dropAction = drag->exec();
//! [0]

        QString actionText;
        switch (dropAction) {
            case BobUI::CopyAction:
                actionText = tr("The text was copied.");
                break;
            case BobUI::MoveAction:
                actionText = tr("The text was moved.");
                break;
            case BobUI::LinkAction:
                actionText = tr("The text was linked.");
                break;
            case BobUI::IgnoreAction:
                actionText = tr("The drag was ignored.");
                break;
            default:
                actionText = tr("Unknown action.");
                break;
        }
        statusBar()->showMessage(actionText);
//! [2]
    }
}
//! [2]
