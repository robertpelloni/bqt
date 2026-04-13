// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "car.h"
#include "car_adaptor.h"
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QGraphicsView>
#include <BobUIWidgets/QGraphicsScene>
#include <BobUIDBus/QDBusConnection>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene scene;
    scene.setSceneRect(-500, -500, 1000, 1000);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    auto car = new Car();
    scene.addItem(car);

    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setBackgroundBrush(BobUI::darkGray);
    view.setWindowTitle(BOBUI_TRANSLATE_NOOP(QGraphicsView, "BobUI DBus Controlled Car"));
    view.resize(view.sizeHint());
    view.show();

    new CarInterfaceAdaptor(car);
    auto connection = QDBusConnection::sessionBus();
    connection.registerObject("/Car", car);
    connection.registerService("org.example.CarExample");

    return app.exec();
}
