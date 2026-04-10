// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUIWidgets/qgraphicsanchorlayout.h>
#include <BobUIWidgets/qgraphicslinearlayout.h>
#include <BobUIWidgets/qgraphicswidget.h>
#include <BobUIWidgets/qgraphicsview.h>

class tst_QGraphicsAnchorLayout : public QObject
{
    Q_OBJECT
public:
    tst_QGraphicsAnchorLayout() {}
    ~tst_QGraphicsAnchorLayout() {}

private slots:
    void hard_complex_data();
    void hard_complex();
    void linearVsAnchorSizeHints_data();
    void linearVsAnchorSizeHints();
    void linearVsAnchorSetGeometry_data();
    void linearVsAnchorSetGeometry();
    void linearVsAnchorNested_data();
    void linearVsAnchorNested();
};


class RectWidget : public QGraphicsWidget
{
public:
    RectWidget(QGraphicsItem *parent = nullptr) : QGraphicsWidget(parent){}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->drawRoundedRect(rect(), 25, 25, BobUI::RelativeSize);
        painter->drawLine(rect().topLeft(), rect().bottomRight());
        painter->drawLine(rect().bottomLeft(), rect().topRight());
    }
};

static QGraphicsWidget *createItem(const QSizeF &minimum = QSizeF(100.0, 100.0),
                                   const QSizeF &preferred = QSize(150.0, 100.0),
                                   const QSizeF &maximum = QSizeF(200.0, 100.0),
                                   const QString &name = QString())
{
    QGraphicsWidget *w = new RectWidget;
    w->setMinimumSize(minimum);
    w->setPreferredSize(preferred);
    w->setMaximumSize(maximum);
    w->setData(0, name);
    return w;
}

static void setAnchor(QGraphicsAnchorLayout *l,
                      QGraphicsLayoutItem *firstItem,
                      BobUI::AnchorPoint firstEdge,
                      QGraphicsLayoutItem *secondItem,
                      BobUI::AnchorPoint secondEdge,
                      qreal spacing)
{
    QGraphicsAnchor *anchor = l->addAnchor(firstItem, firstEdge, secondItem, secondEdge);
    anchor->setSpacing(spacing);
}

void tst_QGraphicsAnchorLayout::hard_complex_data()
{
    BOBUIest::addColumn<int>("whichSizeHint");
    BOBUIest::newRow("minimumSizeHint")
        << int(BobUI::MinimumSize);
    BOBUIest::newRow("preferredSizeHint")
        << int(BobUI::PreferredSize);
    BOBUIest::newRow("maximumSizeHint")
        << int(BobUI::MaximumSize);
    // Add it as a reference to see how much overhead the body of effectiveSizeHint takes.
    BOBUIest::newRow("noSizeHint")
        << -1;
}

void tst_QGraphicsAnchorLayout::hard_complex()
{
    QFETCH(int, whichSizeHint);

    // Test for "hard" complex case, taken from wiki
    // https://cwiki.nokia.com/S60BOBUIUI/AnchorLayoutComplexCases
    QSizeF min(0, 10);
    QSizeF pref(50, 10);
    QSizeF max(100, 10);

    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");
    QGraphicsWidget *c = createItem(min, pref, max, "c");
    QGraphicsWidget *d = createItem(min, pref, max, "d");
    QGraphicsWidget *e = createItem(min, pref, max, "e");
    QGraphicsWidget *f = createItem(min, pref, max, "f");
    QGraphicsWidget *g = createItem(min, pref, max, "g");

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setContentsMargins(0, 0, 0, 0);

    //<!-- Trunk -->
    setAnchor(l, l, BobUI::AnchorLeft, a, BobUI::AnchorLeft, 10);
    setAnchor(l, a, BobUI::AnchorRight, b, BobUI::AnchorLeft, 10);
    setAnchor(l, b, BobUI::AnchorRight, c, BobUI::AnchorLeft, 10);
    setAnchor(l, c, BobUI::AnchorRight, d, BobUI::AnchorLeft, 10);
    setAnchor(l, d, BobUI::AnchorRight, l, BobUI::AnchorRight, 10);

    //<!-- Above trunk -->
    setAnchor(l, b, BobUI::AnchorLeft, e, BobUI::AnchorLeft, 10);
    setAnchor(l, e, BobUI::AnchorRight, d, BobUI::AnchorLeft, 10);

    //<!-- Below trunk -->
    setAnchor(l, a, BobUI::AnchorHorizontalCenter, g, BobUI::AnchorLeft, 10);
    setAnchor(l, g, BobUI::AnchorRight, f, BobUI::AnchorHorizontalCenter, 10);
    setAnchor(l, c, BobUI::AnchorLeft, f, BobUI::AnchorLeft, 10);
    setAnchor(l, f, BobUI::AnchorRight, d, BobUI::AnchorRight, 10);

    //<!-- vertical is simpler -->
    setAnchor(l, l, BobUI::AnchorTop, e, BobUI::AnchorTop, 0);
    setAnchor(l, e, BobUI::AnchorBottom, a, BobUI::AnchorTop, 0);
    setAnchor(l, e, BobUI::AnchorBottom, b, BobUI::AnchorTop, 0);
    setAnchor(l, e, BobUI::AnchorBottom, c, BobUI::AnchorTop, 0);
    setAnchor(l, e, BobUI::AnchorBottom, d, BobUI::AnchorTop, 0);
    setAnchor(l, a, BobUI::AnchorBottom, f, BobUI::AnchorTop, 0);
    setAnchor(l, a, BobUI::AnchorBottom, b, BobUI::AnchorBottom, 0);
    setAnchor(l, a, BobUI::AnchorBottom, c, BobUI::AnchorBottom, 0);
    setAnchor(l, a, BobUI::AnchorBottom, d, BobUI::AnchorBottom, 0);
    setAnchor(l, f, BobUI::AnchorBottom, g, BobUI::AnchorTop, 0);
    setAnchor(l, g, BobUI::AnchorBottom, l, BobUI::AnchorBottom, 0);

    // It won't query the size hint if it already has a size set.
    // If only one of the sizes is unset it will query sizeHint only of for that hint type.
    l->setMinimumSize(60,40);
    l->setPreferredSize(220,40);
    l->setMaximumSize(240,40);

    switch (whichSizeHint) {
    case BobUI::MinimumSize:
        l->setMinimumSize(-1, -1);
        break;
    case BobUI::PreferredSize:
        l->setPreferredSize(-1, -1);
        break;
    case BobUI::MaximumSize:
        l->setMaximumSize(-1, -1);
        break;
    default:
        break;
    }

    QSizeF sizeHint;
    // warm up instruction cache
    l->invalidate();
    sizeHint = l->effectiveSizeHint((BobUI::SizeHint)whichSizeHint);
    // ...then measure...
    QBENCHMARK {
        l->invalidate();
        sizeHint = l->effectiveSizeHint((BobUI::SizeHint)whichSizeHint);
    }
}

static QGraphicsLayout* createLayouts(int whichLayout)
{
    QSizeF min(0, 10);
    QSizeF pref(50, 10);
    QSizeF max(100, 10);

    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");
    QGraphicsWidget *c = createItem(min, pref, max, "c");
    QGraphicsWidget *d = createItem(min, pref, max, "d");

    QGraphicsLayout *l;
    if (whichLayout == 0) {
        l = new QGraphicsLinearLayout;
        QGraphicsLinearLayout *linear = static_cast<QGraphicsLinearLayout *>(l);
        linear->setContentsMargins(0, 0, 0, 0);

        linear->addItem(a);
        linear->addItem(b);
        linear->addItem(c);
        linear->addItem(d);
    } else {
        l = new QGraphicsAnchorLayout;
        QGraphicsAnchorLayout *anchor = static_cast<QGraphicsAnchorLayout *>(l);
        anchor->setContentsMargins(0, 0, 0, 0);

        // Horizontal
        setAnchor(anchor, anchor, BobUI::AnchorLeft, a, BobUI::AnchorLeft, 0);
        setAnchor(anchor, a, BobUI::AnchorRight, b, BobUI::AnchorLeft, 0);
        setAnchor(anchor, b, BobUI::AnchorRight, c, BobUI::AnchorLeft, 0);
        setAnchor(anchor, c, BobUI::AnchorRight, d, BobUI::AnchorLeft, 0);
        setAnchor(anchor, d, BobUI::AnchorRight, anchor, BobUI::AnchorRight, 0);

        // Vertical
        anchor->addAnchors(anchor, a, BobUI::Vertical);
        anchor->addAnchors(anchor, b, BobUI::Vertical);
        anchor->addAnchors(anchor, c, BobUI::Vertical);
        anchor->addAnchors(anchor, d, BobUI::Vertical);
    }

    return l;
}

void tst_QGraphicsAnchorLayout::linearVsAnchorSizeHints_data()
{
    BOBUIest::addColumn<int>("whichLayout");
    BOBUIest::addColumn<int>("whichSizeHint");

    BOBUIest::newRow("QGraphicsLinearLayout::minimum")
        << 0 << int(BobUI::MinimumSize);
    BOBUIest::newRow("QGraphicsLinearLayout::preferred")
        << 0 << int(BobUI::PreferredSize);
    BOBUIest::newRow("QGraphicsLinearLayout::maximum")
        << 0 << int(BobUI::MaximumSize);
    BOBUIest::newRow("QGraphicsLinearLayout::noSizeHint")
        << 0 << -1;

    BOBUIest::newRow("QGraphicsAnchorLayout::minimum")
        << 1 << int(BobUI::MinimumSize);
    BOBUIest::newRow("QGraphicsAnchorLayout::preferred")
        << 1 << int(BobUI::PreferredSize);
    BOBUIest::newRow("QGraphicsAnchorLayout::maximum")
        << 1 << int(BobUI::MaximumSize);
    BOBUIest::newRow("QGraphicsAnchorLayout::noSizeHint")
        << 1 << -1;
}

void tst_QGraphicsAnchorLayout::linearVsAnchorSizeHints()
{
    QFETCH(int, whichSizeHint);
    QFETCH(int, whichLayout);

    QGraphicsLayout *l = createLayouts(whichLayout);

    QSizeF sizeHint;
    // warm up instruction cache
    l->invalidate();
    sizeHint = l->effectiveSizeHint((BobUI::SizeHint)whichSizeHint);
    // ...then measure...

    QBENCHMARK {
        l->invalidate();
        sizeHint = l->effectiveSizeHint((BobUI::SizeHint)whichSizeHint);
    }
}

void tst_QGraphicsAnchorLayout::linearVsAnchorSetGeometry_data()
{
    BOBUIest::addColumn<int>("whichLayout");

    BOBUIest::newRow("QGraphicsLinearLayout")
        << 0;
    BOBUIest::newRow("QGraphicsAnchorLayout")
        << 1;
}

void tst_QGraphicsAnchorLayout::linearVsAnchorSetGeometry()
{
    QFETCH(int, whichLayout);

    QGraphicsLayout *l = createLayouts(whichLayout);

    QRectF sizeHint;
    qreal maxWidth;
    qreal increment;
    // warm up instruction cache
    l->invalidate();
    sizeHint.setSize(l->effectiveSizeHint(BobUI::MinimumSize));
    maxWidth = l->effectiveSizeHint(BobUI::MaximumSize).width();
    increment = (maxWidth - sizeHint.width()) / 100;
    l->setGeometry(sizeHint);
    // ...then measure...

    QBENCHMARK {
        l->invalidate();
        for (qreal width = sizeHint.width(); width <= maxWidth; width += increment) {
            sizeHint.setWidth(width);
            l->setGeometry(sizeHint);
        }
    }
}

void tst_QGraphicsAnchorLayout::linearVsAnchorNested_data()
{
    BOBUIest::addColumn<int>("whichLayout");
    BOBUIest::newRow("LinearLayout")
        << 0;
    BOBUIest::newRow("AnchorLayout setup with null-anchors knot")
        << 1;
    BOBUIest::newRow("AnchorLayout setup easy to simplificate")
        << 2;
}

void tst_QGraphicsAnchorLayout::linearVsAnchorNested()
{
    QFETCH(int, whichLayout);

    QSizeF min(10, 10);
    QSizeF pref(80, 80);
    QSizeF max(150, 150);

    QGraphicsWidget *a = createItem(min, pref, max, "a");
    QGraphicsWidget *b = createItem(min, pref, max, "b");
    QGraphicsWidget *c = createItem(min, pref, max, "c");
    QGraphicsWidget *d = createItem(min, pref, max, "d");

    QGraphicsLayout *layout;

    if (whichLayout == 0) {
        QGraphicsLinearLayout *linear1 = new QGraphicsLinearLayout;
        QGraphicsLinearLayout *linear2 = new QGraphicsLinearLayout(BobUI::Vertical);
        QGraphicsLinearLayout *linear3 = new QGraphicsLinearLayout;

        linear1->addItem(a);
        linear1->addItem(linear2);
        linear2->addItem(b);
        linear2->addItem(linear3);
        linear3->addItem(c);
        linear3->addItem(d);

        layout = linear1;
    } else if (whichLayout == 1) {
        QGraphicsAnchorLayout *anchor = new QGraphicsAnchorLayout;

        // A
        anchor->addCornerAnchors(a, BobUI::TopLeftCorner, anchor, BobUI::TopLeftCorner);
        anchor->addCornerAnchors(a, BobUI::TopRightCorner, b, BobUI::TopLeftCorner);
        anchor->addCornerAnchors(a, BobUI::BottomLeftCorner, anchor, BobUI::BottomLeftCorner);
        anchor->addCornerAnchors(a, BobUI::BottomRightCorner, c, BobUI::BottomLeftCorner);

        // B
        anchor->addCornerAnchors(b, BobUI::TopRightCorner, anchor, BobUI::TopRightCorner);
        anchor->addCornerAnchors(b, BobUI::BottomLeftCorner, c, BobUI::TopLeftCorner);
        anchor->addCornerAnchors(b, BobUI::BottomRightCorner, d, BobUI::TopRightCorner);

        // C
        anchor->addCornerAnchors(c, BobUI::TopRightCorner, d, BobUI::TopLeftCorner);
        anchor->addCornerAnchors(c, BobUI::BottomRightCorner, d, BobUI::BottomLeftCorner);

        // D
        anchor->addCornerAnchors(d, BobUI::BottomRightCorner, anchor, BobUI::BottomRightCorner);

        layout = anchor;
    } else {
        QGraphicsAnchorLayout *anchor = new QGraphicsAnchorLayout;

        // A
        anchor->addAnchor(a, BobUI::AnchorLeft, anchor, BobUI::AnchorLeft);
        anchor->addAnchors(a, anchor, BobUI::Vertical);
        anchor->addAnchor(a, BobUI::AnchorRight, b, BobUI::AnchorLeft);
        anchor->addAnchor(a, BobUI::AnchorRight, c, BobUI::AnchorLeft);

        // B
        anchor->addAnchor(b, BobUI::AnchorTop, anchor, BobUI::AnchorTop);
        anchor->addAnchor(b, BobUI::AnchorRight, anchor, BobUI::AnchorRight);
        anchor->addAnchor(b, BobUI::AnchorBottom, c, BobUI::AnchorTop);
        anchor->addAnchor(b, BobUI::AnchorBottom, d, BobUI::AnchorTop);

        // C
        anchor->addAnchor(c, BobUI::AnchorRight, d, BobUI::AnchorLeft);
        anchor->addAnchor(c, BobUI::AnchorBottom, anchor, BobUI::AnchorBottom);

        // D
        anchor->addAnchor(d, BobUI::AnchorRight, anchor, BobUI::AnchorRight);
        anchor->addAnchor(d, BobUI::AnchorBottom, anchor, BobUI::AnchorBottom);

        layout = anchor;
    }

    QSizeF sizeHint;
    // warm up instruction cache
    layout->invalidate();
    sizeHint = layout->effectiveSizeHint(BobUI::PreferredSize);

    // ...then measure...
    QBENCHMARK {
        // To ensure that all sizeHints caches are invalidated in
        // the LinearLayout setup, we must call updateGeometry on the
        // children. If we didn't, only the top level layout would be
        // re-calculated.
        static_cast<QGraphicsLayoutItem *>(a)->updateGeometry();
        static_cast<QGraphicsLayoutItem *>(b)->updateGeometry();
        static_cast<QGraphicsLayoutItem *>(c)->updateGeometry();
        static_cast<QGraphicsLayoutItem *>(d)->updateGeometry();
        layout->invalidate();
        sizeHint = layout->effectiveSizeHint(BobUI::PreferredSize);
    }
}

BOBUIEST_MAIN(tst_QGraphicsAnchorLayout)

#include "tst_qgraphicsanchorlayout.moc"
