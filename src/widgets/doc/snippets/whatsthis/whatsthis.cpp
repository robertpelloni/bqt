// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QMainWindow>
#include <QAction>

class MainWindow : public QMainWindow
{
public:
    QAction *newAct;
    MainWindow()
    {
        //! [0]
        newAct = new QAction(tr("&New"), this);
        newAct->setShortcut(tr("Ctrl+N"));
        newAct->setStatusTip(tr("Create a new file"));
        newAct->setWhatsThis(tr("Click this option to create a new file."));
        //! [0]
    }
};

int main()
{
    return 0;
}
