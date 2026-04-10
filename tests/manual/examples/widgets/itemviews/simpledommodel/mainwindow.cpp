// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "dommodel.h"

#include <QDomDocument>
#include <BOBUIreeView>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      model(new DomModel(QDomDocument(), this)),
      view(new BOBUIreeView(this))
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open..."), QKeySequence::Open, this, &MainWindow::openFile);
    fileMenu->addAction(tr("E&xit"), QKeySequence::Quit, this, &QWidget::close);

    view->setModel(model);

    setCentralWidget(view);
    setWindowTitle(tr("Simple DOM Model"));
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
        xmlPath, tr("XML files (*.xml);;HTML files (*.html);;"
                    "SVG files (*.svg);;User Interface files (*.ui)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDomDocument document;
            if (document.setContent(&file)) {
                DomModel *newModel = new DomModel(document, this);
                view->setModel(newModel);
                delete model;
                model = newModel;
                xmlPath = filePath;
            }
            file.close();
        }
    }
}
