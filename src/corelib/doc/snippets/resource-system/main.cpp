// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QQmlApplicationEngine>
#include <QAction>
#include <QIcon>
#include <QApplication>
#include <QWidget>

class DummyWidget : public QWidget {
    Q_OBJECT

    QAction *cutAct;

    DummyWidget() {
        //! [QAction]
        cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
        //! [QAction]
    }
};

int main()
{
//! [url]
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/myapp/main.qml"));
//! [url]
}
