// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include "qpen.h"
#include "qbrush.h"

#include <qdebug.h>

class tst_QPen : public QObject
{
    Q_OBJECT

public:
    tst_QPen();

private slots:
    void getSetCheck();
    void swap();
    void move();
    void move_assign();
    void operator_eq_eq();
    void operator_eq_eq_data();

    void stream();
    void stream_data();

    void constructor();
    void constructor_data();
};

// Testing get/set functions
void tst_QPen::getSetCheck()
{
    QPen obj1;
    // qreal QPen::miterLimit()
    // void QPen::setMiterLimit(qreal)
    obj1.setMiterLimit(0.0);
    QCOMPARE(0.0, obj1.miterLimit());
    obj1.setMiterLimit(qreal(1.1));
    QCOMPARE(qreal(1.1), obj1.miterLimit());

    // qreal QPen::widthF()
    // void QPen::setWidthF(qreal)
    obj1.setWidthF(0.0);
    QCOMPARE(0.0, obj1.widthF());
    obj1.setWidthF(qreal(1.1));
    QCOMPARE(qreal(1.1), obj1.widthF());

    // int QPen::width()
    // void QPen::setWidth(int)
    for (int i = 0; i < 100; ++i) {
        obj1.setWidth(i);
        QCOMPARE(i, obj1.width());
    }
}

void tst_QPen::swap()
{
    QPen p1(BobUI::black), p2(BobUI::white);
    p1.swap(p2);
    QCOMPARE(p1.color(), QColor(BobUI::white));
    QCOMPARE(p2.color(), QColor(BobUI::black));
}

void tst_QPen::move()
{
    QPen p1(BobUI::black);

    // check that moving does the right thing:
    QPen p2 = std::move(p1); // could be move or copy construction, so don't check p1's state
    QCOMPARE(p2.color(), QColor(BobUI::black));

    // this, executed ehre, would crash:
    // QVERIFY(p1.style() != BobUI::NoPen);

    // check that moved-from QPen p1 can still be safely copied:
    const QPen p3 = p1;

    // check that moved-from QPen p1 can still be safely assigned to:
    const QPen p4(BobUI::yellow);
    p1 = p4;
    QCOMPARE(p1.color(), QColor(BobUI::yellow));

    // check that moved-from QPens p2, p3 can still be safely destroyed:
    QPen p5 = std::move(p2);

    // intentionally no more statements beyond this point
}

void tst_QPen::move_assign()
{
    QPen p1(BobUI::black), p2(BobUI::white);

    // check that moving does the right thing:
    p2 = std::move(p1); // could be move or copy assignment, so don't check p1's state
    QCOMPARE(p2.color(), QColor(BobUI::black));

    // check that move-assigned-from QPen p1 can still be used, albeit
    // with undocumented state (it's p2's original state):
    QVERIFY(p1.style() != BobUI::NoPen);

    // check that moved-from QPen p1 can still be safely copied:
    const QPen p3 = p1;

    // check that moved-from QPen p1 can still be safely assigned to:
    const QPen p4(BobUI::yellow);
    p1 = p4;
    QCOMPARE(p1.color(), QColor(BobUI::yellow));

    // check that moved-from QPens p2, p3 can still be safely destroyed:
    QPen p5;
    p5 = std::move(p2);

    // intentionally no more statements beyond this point
}

tst_QPen::tst_QPen()

{
}

void tst_QPen::operator_eq_eq_data()
{
    BOBUIest::addColumn<QPen>("pen1");
    BOBUIest::addColumn<QPen>("pen2");
    BOBUIest::addColumn<bool>("isEqual");

    BOBUIest::newRow("differentColor") << QPen(BobUI::red)
                                    << QPen(BobUI::blue)
                                    << false;
    BOBUIest::newRow("differentWidth") << QPen(BobUI::red, 2)
                                    << QPen(BobUI::red, 3)
                                    << false;
    BOBUIest::newRow("differentPenStyle") << QPen(BobUI::red, 2, BobUI::DashLine)
                                       << QPen(BobUI::red, 2, BobUI::DotLine)
                                       << false;
    BOBUIest::newRow("differentCapStyle") << QPen(BobUI::red, 2, BobUI::DashLine, BobUI::RoundCap, BobUI::BevelJoin)
                                       << QPen(BobUI::red, 2, BobUI::DashLine, BobUI::SquareCap, BobUI::BevelJoin)
                                       << false;
    BOBUIest::newRow("differentJoinStyle") << QPen(BobUI::red, 2, BobUI::DashLine, BobUI::RoundCap, BobUI::BevelJoin)
                                        << QPen(BobUI::red, 2, BobUI::DashLine, BobUI::RoundCap, BobUI::MiterJoin)
                                        << false;
    BOBUIest::newRow("same") << QPen(BobUI::red, 2, BobUI::DashLine, BobUI::RoundCap, BobUI::BevelJoin)
                          << QPen(BobUI::red, 2, BobUI::DashLine, BobUI::RoundCap, BobUI::BevelJoin)
                          << true;

}

void tst_QPen::operator_eq_eq()
{
    QFETCH(QPen, pen1);
    QFETCH(QPen, pen2);
    QFETCH(bool, isEqual);
    QCOMPARE(pen1 == pen2, isEqual);

    // exercise operator== overloads
    QCOMPARE(QPen(pen1.style()), pen1.style());
    QCOMPARE(QPen(pen1.color()), pen1.color());
}


void tst_QPen::constructor_data()
{
    BOBUIest::addColumn<QPen>("pen");
    BOBUIest::addColumn<QBrush>("brush");
    BOBUIest::addColumn<double>("width");
    BOBUIest::addColumn<int>("style");
    BOBUIest::addColumn<int>("capStyle");
    BOBUIest::addColumn<int>("joinStyle");

    BOBUIest::newRow("solid_black") << QPen() << QBrush(BobUI::black) << 1. << (int)BobUI::SolidLine
                              << (int) BobUI::SquareCap << (int)BobUI::BevelJoin;
    BOBUIest::newRow("solid_red") << QPen(BobUI::red) << QBrush(BobUI::red) << 1. << (int)BobUI::SolidLine
                            << (int)BobUI::SquareCap << (int)BobUI::BevelJoin;
    BOBUIest::newRow("full") << QPen(QBrush(QLinearGradient(0, 0, 100, 100)), 10,
                               BobUI::SolidLine, BobUI::RoundCap, BobUI::MiterJoin)
                       << QBrush(QLinearGradient(0, 0, 100, 100)) << 10. << (int)BobUI::SolidLine
                       << (int)BobUI::RoundCap << (int)BobUI::MiterJoin;

}


void tst_QPen::constructor()
{
    QFETCH(QPen, pen);
    QFETCH(QBrush, brush);
    QFETCH(double, width);
    QFETCH(int, style);
    QFETCH(int, capStyle);
    QFETCH(int, joinStyle);

    QCOMPARE(pen.style(), BobUI::PenStyle(style));
    QCOMPARE(pen.capStyle(), BobUI::PenCapStyle(capStyle));
    QCOMPARE(pen.joinStyle(), BobUI::PenJoinStyle(joinStyle));
    QCOMPARE(pen.widthF(), width);
    QCOMPARE(pen.brush(), brush);
}


void tst_QPen::stream_data()
{
    BOBUIest::addColumn<QPen>("pen");

    BOBUIest::newRow("solid_black") << QPen();
    BOBUIest::newRow("solid_red") << QPen(BobUI::red);
    BOBUIest::newRow("full") << QPen(QBrush(QLinearGradient(0, 0, 100, 100)), 10, BobUI::SolidLine, BobUI::RoundCap, BobUI::MiterJoin);
}


void tst_QPen::stream()
{
    QFETCH(QPen, pen);

    QByteArray bytes;

    {
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream << pen;
    }

    QPen cmp;
    {
        QDataStream stream(&bytes, QIODevice::ReadOnly);
        stream >> cmp;
    }

    QCOMPARE(pen.widthF(), cmp.widthF());
    QCOMPARE(pen.style(), cmp.style());
    QCOMPARE(pen.capStyle(), cmp.capStyle());
    QCOMPARE(pen.joinStyle(), cmp.joinStyle());
    QCOMPARE(pen.brush(), cmp.brush());

    QCOMPARE(pen, cmp);
}

BOBUIEST_APPLESS_MAIN(tst_QPen)
#include "tst_qpen.moc"
