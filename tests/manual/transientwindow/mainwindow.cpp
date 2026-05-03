// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_showButton("Toggle visible", this), m_window(0)
{
    connect(&m_showButton, SIGNAL(clicked()), this, SLOT(toggleVisible()));
    setWindowTitle(QString::fromLatin1("Main Window"));
    m_showButton.setVisible(true);
    setMinimumSize(300, 200);
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleVisible()
{
    if (!m_window) {
        m_window = new QWindow();
        m_window->setTransientParent(windowHandle());
        m_window->setMinimumSize(QSize(200, 100));
        m_window->setTitle("Transient Window");
        m_window->setFlags(BobUI::Dialog);
    }
    m_window->setVisible(!m_window->isVisible());
}
