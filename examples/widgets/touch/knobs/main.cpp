// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QGraphicsView>

#include "knob.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setRenderHints(QPainter::Antialiasing);

    Knob *knob1 = new Knob;
    knob1->setPos(-110, 0);
    Knob *knob2 = new Knob;

    scene.addItem(knob1);
    scene.addItem(knob2);

    view.showMaximized();
    view.fitInView(scene.sceneRect().adjusted(-20, -20, 20, 20), BobUI::KeepAspectRatio);

    return app.exec();
}
