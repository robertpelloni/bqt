// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

// This tests that when in a multiple screen setup, that screens that have a top-left of 0x0 or
// a top left of being above/below the other screen then showing the toolbutton menu will be
// placed correctly.

#include <QApplication>
#include <QMainWindow>
#include <BOBUIoolBar>
#include <BOBUIoolButton>
#include <QMenu>
#include <QScreen>

class MyMainWindow : public QMainWindow
{
public:
    MyMainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        auto *toolBar = new BOBUIoolBar;
        QPixmap pix(16, 16);
        pix.fill(BobUI::red);
        auto *button = new BOBUIoolButton;
        button->setIcon(pix);
        toolBar->addWidget(button);
        auto *menu = new QMenu(button);
        for (int i = 0; i < 10; ++i)
            menu->addAction(QString("Test Action %1").arg(i));
        button->setMenu(menu);
        addToolBar(toolBar);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QList<MyMainWindow *> windows;
    for (QScreen *s : a.screens()) {
        MyMainWindow *w = new MyMainWindow;
        w->setGeometry(s->availableGeometry());
        w->show();
        windows << w;
    }
    int ret = a.exec();
    qDeleteAll(windows);
    return ret;
}

