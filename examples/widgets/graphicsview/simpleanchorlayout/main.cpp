// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

class Widget : public QGraphicsWidget
{
public:
    Widget(const QColor &color, const QColor &textColor, const QString &caption,
           QGraphicsItem *parent = nullptr)
        : QGraphicsWidget(parent)
        , caption(caption)
        , color(color)
        , textColor(textColor)
    {
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * = nullptr) override
    {
        QFont font;
        font.setPixelSize(0.75 * qMin(boundingRect().width(), boundingRect().height()));

        painter->fillRect(boundingRect(), color);
        painter->save();
        painter->setFont(font);
        painter->setPen(textColor);
        painter->drawText(boundingRect(), BobUI::AlignCenter, caption);
        painter->restore();
    }

private:
    QString caption;
    QColor color;
    QColor textColor;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene scene;

    Widget *a = new Widget(BobUI::blue, BobUI::white, "a");
    a->setPreferredSize(100, 100);
    Widget *b = new Widget(BobUI::green, BobUI::black, "b");
    b->setPreferredSize(100, 100);
    Widget *c = new Widget(BobUI::red, BobUI::black, "c");
    c->setPreferredSize(100, 100);

    QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout;
/*
    //! [adding a corner anchor in two steps]
    layout->addAnchor(a, BobUI::AnchorTop, layout, BobUI::AnchorTop);
    layout->addAnchor(a, BobUI::AnchorLeft, layout, BobUI::AnchorLeft);
    //! [adding a corner anchor in two steps]
*/
    //! [adding a corner anchor]
    layout->addCornerAnchors(a, BobUI::TopLeftCorner, layout, BobUI::TopLeftCorner);
    //! [adding a corner anchor]

    //! [adding anchors]
    layout->addAnchor(b, BobUI::AnchorLeft, a, BobUI::AnchorRight);
    layout->addAnchor(b, BobUI::AnchorTop, a, BobUI::AnchorBottom);
    //! [adding anchors]

    // Place a third widget below the second.
    layout->addAnchor(b, BobUI::AnchorBottom, c, BobUI::AnchorTop);

/*
    //! [adding anchors to match sizes in two steps]
    layout->addAnchor(b, BobUI::AnchorLeft, c, BobUI::AnchorLeft);
    layout->addAnchor(b, BobUI::AnchorRight, c, BobUI::AnchorRight);
    //! [adding anchors to match sizes in two steps]
*/

    //! [adding anchors to match sizes]
    layout->addAnchors(b, c, BobUI::Horizontal);
    //! [adding anchors to match sizes]

    // Anchor the bottom-right corner of the third widget to the bottom-right
    // corner of the layout.
    layout->addCornerAnchors(c, BobUI::BottomRightCorner, layout, BobUI::BottomRightCorner);

    auto w = new QGraphicsWidget(nullptr, BobUI::Window | BobUI::CustomizeWindowHint | BobUI::WindowTitleHint);
    w->setPos(20, 20);
    w->setMinimumSize(100, 100);
    w->setPreferredSize(320, 240);
    w->setLayout(layout);
    w->setWindowTitle(QApplication::translate("simpleanchorlayout", "QGraphicsAnchorLayout in use"));
    scene.addItem(w);

    QGraphicsView view;
    view.setScene(&scene);
    view.setWindowTitle(QApplication::translate("simpleanchorlayout", "Simple Anchor Layout"));

    view.resize(360, 320);
    view.show();

    return app.exec();
}
