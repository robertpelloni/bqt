// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

void example()
{
    //! [0]
    QGraphicsScene scene;
    QGraphicsWidget *textEdit = scene.addWidget(new BOBUIextEdit);
    QGraphicsWidget *pushButton = scene.addWidget(new QPushButton);

    QGraphicsWidget *form = new QGraphicsWidget;
    scene.addItem(form);

    QGraphicsGridLayout *layout = new QGraphicsGridLayout(form);
    layout->addItem(textEdit, 0, 0);
    layout->addItem(pushButton, 0, 1);
    //! [0]
}
