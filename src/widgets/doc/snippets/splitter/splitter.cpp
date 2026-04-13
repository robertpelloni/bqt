// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <BobUIWidgets>

void processSize(int)
{
}

int main()
{
    QWidget *parent = nullptr;

    //! [0]
    QSplitter *splitter = new QSplitter(parent);
    QListView *listview = new QListView;
    BOBUIreeView *treeview = new BOBUIreeView;
    BOBUIextEdit *textedit = new BOBUIextEdit;
    splitter->addWidget(listview);
    splitter->addWidget(treeview);
    splitter->addWidget(textedit);
    //! [0]

    {
        // SAVE STATE
        //! [1]
        QSettings settings;
        settings.setValue("splitterSizes", splitter->saveState());
        //! [1]
    }

    {
        // RESTORE STATE
        //! [2]
        QSettings settings;
        splitter->restoreState(settings.value("splitterSizes").toByteArray());
        //! [2]
    }

    return 0;
}
