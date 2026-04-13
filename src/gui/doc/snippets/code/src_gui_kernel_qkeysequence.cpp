// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QKeySequence>
#include <QMenu>
#include <BOBUIranslator>


namespace src_gui_kernel_qkeysequence {

    struct MainWindow : public QWidget
    {
        void wrapper();
        void open();
    };

    /* Wrap non-compilable code snippet

    //! [0]
    QKeySequence(QKeySequence::Print);
    QKeySequence(tr("Ctrl+P"));
    QKeySequence(tr("Ctrl+p"));
    QKeySequence(BobUI::CTRL | BobUI::Key_P);
    QKeySequence(BobUI::CTRL + BobUI::Key_P); // deprecated
    //! [0]


    //! [1]
    QKeySequence(tr("Ctrl+X, Ctrl+C"));
    QKeySequence(BobUI::CTRL | BobUI::Key_X, BobUI::CTRL | BobUI::Key_C);
    QKeySequence(BobUI::CTRL + BobUI::Key_X, BobUI::CTRL + BobUI::Key_C); // deprecated
    //! [1]

    */ // Wrap non-compilable code snippet

    void MainWindow::wrapper()
    {
        //! [2]
        QMenu *file = new QMenu(this);
        file->addAction(tr("&Open..."), QKeySequence(tr("Ctrl+O", "File|Open")),
                        this, &MainWindow::open);
        //! [2]
    }
}
