// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIWidgets>

static QGraphicsProxyWidget *createItem(const QSizeF &minimum = QSizeF(100.0, 100.0),
                                        const QSizeF &preferred = QSize(150.0, 100.0),
                                        const QSizeF &maximum = QSizeF(200.0, 100.0),
                                        const QString &name = "0")
{
    QGraphicsProxyWidget *w = new QGraphicsProxyWidget;
    w->setWidget(new QPushButton(name));
    w->setData(0, name);
    w->setMinimumSize(minimum);
    w->setPreferredSize(preferred);
    w->setMaximumSize(maximum);

    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    return w;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 800, 480);

    QSizeF minSize(30, 100);
    QSizeF prefSize(210, 100);
    QSizeF maxSize(300, 100);

    QGraphicsProxyWidget *a = createItem(minSize, prefSize, maxSize, "A");
    QGraphicsProxyWidget *b = createItem(minSize, prefSize, maxSize, "B");
    QGraphicsProxyWidget *c = createItem(minSize, prefSize, maxSize, "C");
    QGraphicsProxyWidget *d = createItem(minSize, prefSize, maxSize, "D");
    QGraphicsProxyWidget *e = createItem(minSize, prefSize, maxSize, "E");
    QGraphicsProxyWidget *f = createItem(QSizeF(30, 50), QSizeF(150, 50), maxSize, "F (overflow)");
    QGraphicsProxyWidget *g = createItem(QSizeF(30, 50), QSizeF(30, 100), maxSize, "G (overflow)");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setSpacing(0);

    QGraphicsWidget *w = new QGraphicsWidget(nullptr, BobUI::Window);
    w->setPos(20, 20);
    w->setLayout(l);

    // vertical
    l->addAnchor(a, BobUI::AnchorTop, l, BobUI::AnchorTop);
    l->addAnchor(b, BobUI::AnchorTop, l, BobUI::AnchorTop);

    l->addAnchor(c, BobUI::AnchorTop, a, BobUI::AnchorBottom);
    l->addAnchor(c, BobUI::AnchorTop, b, BobUI::AnchorBottom);
    l->addAnchor(c, BobUI::AnchorBottom, d, BobUI::AnchorTop);
    l->addAnchor(c, BobUI::AnchorBottom, e, BobUI::AnchorTop);

    l->addAnchor(d, BobUI::AnchorBottom, l, BobUI::AnchorBottom);
    l->addAnchor(e, BobUI::AnchorBottom, l, BobUI::AnchorBottom);

    l->addAnchor(c, BobUI::AnchorTop, f, BobUI::AnchorTop);
    l->addAnchor(c, BobUI::AnchorVerticalCenter, f, BobUI::AnchorBottom);
    l->addAnchor(f, BobUI::AnchorBottom, g, BobUI::AnchorTop);
    l->addAnchor(c, BobUI::AnchorBottom, g, BobUI::AnchorBottom);

    // horizontal
    l->addAnchor(l, BobUI::AnchorLeft, a, BobUI::AnchorLeft);
    l->addAnchor(l, BobUI::AnchorLeft, d, BobUI::AnchorLeft);
    l->addAnchor(a, BobUI::AnchorRight, b, BobUI::AnchorLeft);

    l->addAnchor(a, BobUI::AnchorRight, c, BobUI::AnchorLeft);
    l->addAnchor(c, BobUI::AnchorRight, e, BobUI::AnchorLeft);

    l->addAnchor(b, BobUI::AnchorRight, l, BobUI::AnchorRight);
    l->addAnchor(e, BobUI::AnchorRight, l, BobUI::AnchorRight);
    l->addAnchor(d, BobUI::AnchorRight, e, BobUI::AnchorLeft);

    l->addAnchor(l, BobUI::AnchorLeft, f, BobUI::AnchorLeft);
    l->addAnchor(l, BobUI::AnchorLeft, g, BobUI::AnchorLeft);
    l->addAnchor(f, BobUI::AnchorRight, g, BobUI::AnchorRight);


    scene.addItem(w);
    scene.setBackgroundBrush(BobUI::darkGreen);
    QGraphicsView view(&scene);

    view.show();

    return app.exec();
}
