// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "stylesheeteditor.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.nameLabel->setProperty("class", "mandatory QLabel");
    ui.nameCombo->lineEdit()->setPlaceholderText(tr("Last, First"));

    styleSheetEditor = new StyleSheetEditor(this);

    statusBar()->addWidget(new QLabel(tr("Ready")));

    connect(ui.exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui.aboutBobUIAction, &QAction::triggered, qApp, &QApplication::aboutBobUI);
}

void MainWindow::on_editStyleAction_triggered()
{
    styleSheetEditor->show();
    styleSheetEditor->activateWindow();
}

void MainWindow::on_aboutAction_triggered()
{
    const QString url = QStringLiteral("http://doc.bobui.io/bobui-%1/stylesheet.html")
        .arg(BOBUI_VERSION_MAJOR);
    QMessageBox::about(this, tr("About Style sheet"),
        tr("The <b>Style Sheet</b> example shows how widgets can be styled "
           "using <a href=\"%1\">BobUI "
           "Style Sheets</a>. Click <b>File|Edit Style Sheet</b> to pop up the "
           "style editor, and either choose an existing style sheet or design "
           "your own.").arg(url));
}
