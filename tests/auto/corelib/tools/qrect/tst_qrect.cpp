// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <qrect.h>
#include <qmargins.h>
#include <limits.h>
#include <qdebug.h>

#include <private/qcomparisontesthelper_p.h>

#include <array>

class tst_QRect : public QObject
{
    Q_OBJECT
public:
    enum QRectCases {
        InvalidQRect, SmallestQRect, MiddleQRect, LargestQRect, SmallestCoordQRect,
        LargestCoordQRect, RandomQRect, NegativeSizeQRect, NegativePointQRect, NullQRect, EmptyQRect,
        MiddleCoordQRect = MiddleQRect
    };

    enum IntCases {
                MinimumInt, MiddleNegativeInt, ZeroInt, MiddlePositiveInt, MaximumInt, RandomInt
    };

    enum QPointCases {
        NullQPoint, SmallestCoordQPoint, MiddleNegCoordQPoint, MiddlePosCoordQPoint, LargestCoordQPoint, NegativeQPoint,
        NegXQPoint, NegYQPoint, RandomQPoint
    };

    static QRect getQRectCase( QRectCases c );
    static int getIntCase( IntCases i );
    static QPoint getQPointCase( QPointCases p );

private slots:
    void comparisonCompiles();
    void comparison_data();
    void comparison();
    void fuzzyComparison_data();
    void fuzzyComparison();
    void isNull_data();
    void isNull();
    void fuzzyIsNull();
    void newIsEmpty_data();
    void newIsEmpty();
    void newIsValid_data();
    void newIsValid();
    void normalized_data();
    void normalized();
    void left_data();
    void left();
    void top_data();
    void top();
    void right_data();
    void right();
    void bottom_data();
    void bottom();
    void x_data();
    void x();
    void y_data();
    void y();
    void setWidthHeight_data();
    void setWidthHeight();
    void setLeft_data();
    void setLeft();
    void setTop_data();
    void setTop();
    void setRight_data();
    void setRight();
    void setBottom_data();
    void setBottom();
    void newSetTopLeft_data();
    void newSetTopLeft();
    void newSetBottomRight_data();
    void newSetBottomRight();
    void newSetTopRight_data();
    void newSetTopRight();
    void newSetBottomLeft_data();
    void newSetBottomLeft();
    void topLeft_data();
    void topLeft();
    void bottomRight_data();
    void bottomRight();
    void topRight_data();
    void topRight();
    void bottomLeft_data();
    void bottomLeft();
    void center_data();
    void center();
    void getRect_data();
    void getRect();
    void getCoords_data();
    void getCoords();
    void newMoveLeft_data();
    void newMoveLeft();
    void newMoveTop_data();
    void newMoveTop();
    void newMoveRight_data();
    void newMoveRight();
    void newMoveBottom_data();
    void newMoveBottom();
    void newMoveTopLeft_data();
    void newMoveTopLeft();
    void newMoveBottomRight_data();
    void newMoveBottomRight();
    void margins();
    void marginsf();

    void toRectF_data();
    void toRectF();

    void translate_data();
    void translate();

    void transposed_data();
    void transposed();

    void moveTop();
    void moveBottom();
    void moveLeft();
    void moveRight();
    void moveTopLeft();
    void moveTopRight();
    void moveBottomLeft();
    void moveBottomRight();
    void setTopLeft();
    void setTopRight();
    void setBottomLeft();
    void setBottomRight();
    void operator_amp();
    void operator_amp_eq();
    void isEmpty();
    void isValid();

    void testAdjust_data();
    void testAdjust();

    void intersectedRect_data();
    void intersectedRect();
    void intersectedRectF_data();
    void intersectedRectF();
    void unitedRect_data();
    void unitedRect();
    void unitedRectF_data();
    void unitedRectF();
    void intersectsRect_data();
    void intersectsRect();
    void intersectsRectF_data();
    void intersectsRectF();
    void containsRect_data();
    void containsRect();
    void containsRectNormalized();
    void containsRectF_data();
    void containsRectF();
    void containsPoint_data();
    void containsPoint();
    void containsPointNormalized();
    void containsPointF_data();
    void containsPointF();
    void smallRects() const;
    void toRect();
    void span();

    void debug();
};

// Used to work around some floating point precision problems.
#define LARGE 1000000000
static bool isLarge(int x) { return x > LARGE || x < -LARGE; }

static constexpr qreal qreal_min = std::numeric_limits<qreal>::min();

QRect tst_QRect::getQRectCase( QRectCases c )
{
    // Should return the best variety of possible QRects, if a
    // case is missing, please add it.

    switch ( c ) {
    case InvalidQRect:
        return QRect();
    case SmallestQRect:
        return QRect( 1, 1, 1, 1 );
    case MiddleQRect:
        return QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ) );
    case LargestQRect:
        return QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
    case SmallestCoordQRect:
        return QRect( QPoint( INT_MIN, INT_MIN ), QSize( 1, 1 ) );
    case LargestCoordQRect:
        return QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
    case RandomQRect:
        return QRect( 100, 200, 11, 16 );
    case NegativeSizeQRect:
        return QRect( 1, 1, -10, -10 );
    case NegativePointQRect:
        return QRect( -10, -10, 5, 5 );
    case NullQRect:
        return QRect( 5, 5, 0, 0 );
    case EmptyQRect:
        return QRect( QPoint( 2, 2 ), QPoint( 1, 1 ) );
    default:
        return QRect();
    }
}

int tst_QRect::getIntCase( IntCases i )
{
    // Should return the best variety of possible ints, if a
    // case is missing, please add it.

    switch ( i ) {
    case MinimumInt:
        return INT_MIN;
    case MiddleNegativeInt:
        return INT_MIN / 2;
    case ZeroInt:
        return 0;
    case MiddlePositiveInt:
        return INT_MAX / 2;
    case MaximumInt:
        return INT_MAX;
    case RandomInt:
        return 4953;
    default:
        return 0;
    }
}

QPoint tst_QRect::getQPointCase( QPointCases p )
{
    // Should return the best variety of possible QPoints, if a
    // case is missing, please add it.
    switch ( p ) {
    case NullQPoint:
        return QPoint();
    case SmallestCoordQPoint:
        return QPoint(INT_MIN,INT_MIN);
    case MiddleNegCoordQPoint:
        return QPoint(INT_MIN/2,INT_MIN/2);
    case MiddlePosCoordQPoint:
        return QPoint(INT_MAX/2,INT_MAX/2);
    case LargestCoordQPoint:
        return QPoint(INT_MAX,INT_MAX);
    case NegXQPoint:
        return QPoint(-12,7);
    case NegYQPoint:
        return QPoint(12,-7);
    case RandomQPoint:
        return QPoint(12,7);
    default:
        return QPoint();
    }
}

void tst_QRect::comparisonCompiles()
{
    BOBUIestPrivate::testEqualityOperatorsCompile<QRect>();
    BOBUIestPrivate::testEqualityOperatorsCompile<QRectF>();
    BOBUIestPrivate::testEqualityOperatorsCompile<QRectF, QRect>();
}

void tst_QRect::comparison_data()
{
    BOBUIest::addColumn<QRectF>("lhsF");
    BOBUIest::addColumn<QRectF>("rhsF");
    BOBUIest::addColumn<bool>("result");
    BOBUIest::addColumn<bool>("floatResult");
    BOBUIest::addColumn<bool>("mixedResult");

    BOBUIest::newRow("Invalid_vs_Invalid") << getQRectCase(InvalidQRect).toRectF()
                                        << getQRectCase(InvalidQRect).toRectF()
                                        << true << true << true;

    BOBUIest::newRow("Null_vs_Null") << getQRectCase(NullQRect).toRectF()
                                  << getQRectCase(NullQRect).toRectF()
                                  << true << true << true;

    BOBUIest::newRow("Empty_vs_Empty") << getQRectCase(EmptyQRect).toRectF()
                                    << getQRectCase(EmptyQRect).toRectF()
                                    << true << true << true;

    BOBUIest::newRow("NegativeSize_vs_NegativeSize") << getQRectCase(NegativeSizeQRect).toRectF()
                                                  << getQRectCase(NegativeSizeQRect).toRectF()
                                                  << true << true << true;

    BOBUIest::newRow("Invalid_vs_Null") << getQRectCase(InvalidQRect).toRectF()
                                     << getQRectCase(NullQRect).toRectF()
                                     << false << false << false;

    BOBUIest::newRow("NearlySimilar") << QRectF(QPointF(1.1, 9.9), QPointF(9.9, 1.1))
                                   << QRectF(QPointF(1., 10.), QPointF(10., 1.))
                                   << true << false << true;

    BOBUIest::newRow("WithQREAL_MIN") << QRectF(QPointF(0., -10.), QPointF(-1., 0.))
                                   << QRectF(QPointF(-qreal_min, -10.), QPointF(-1., qreal_min))
                                   << true << true << true;
}

void tst_QRect::comparison()
{
    QFETCH(const QRectF, lhsF);
    QFETCH(const QRectF, rhsF);
    QFETCH(const bool, result);
    QFETCH(const bool, floatResult);
    QFETCH(const bool, mixedResult);

    const QRect lhs = lhsF.toRect();
    const QRect rhs = rhsF.toRect();

    BOBUI_TEST_EQUALITY_OPS(lhs, rhs, result);
    BOBUI_TEST_EQUALITY_OPS(lhsF, rhsF, floatResult);
    BOBUI_TEST_EQUALITY_OPS(lhs, rhsF, mixedResult);
}

void tst_QRect::fuzzyComparison_data()
{
    comparison_data();
}

void tst_QRect::fuzzyComparison()
{
    QFETCH(const QRectF, lhsF);
    QFETCH(const QRectF, rhsF);
    QFETCH(const bool, floatResult);

    QCOMPARE_EQ(qFuzzyCompare(lhsF, rhsF), floatResult);
}

void tst_QRect::isNull_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<bool>("isNull");
    BOBUIest::addColumn<bool>("isNullF");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << true << true;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << false << false;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << false << false;
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << false << false;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << false << false;
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << true << false; // Due to overflow
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << false << false;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << false << false;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << false << false;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << true << true;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << true << true;
}

void tst_QRect::isNull()
{
    QFETCH( QRect, r );
    QFETCH( bool, isNull );
    QFETCH( bool, isNullF );

    QRectF rf(r);

    QCOMPARE( r.isNull(), isNull );
    QCOMPARE( rf.isNull(), isNullF );
}

void tst_QRect::fuzzyIsNull()
{
    QRectF rf(QPointF(-qreal_min, qreal_min), QPointF(qreal_min, -qreal_min));

    QVERIFY(!rf.isNull()); // QRectF::isNull() does strict comparison
    QVERIFY(qFuzzyIsNull(rf));
}

void tst_QRect::newIsEmpty_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<bool>("isEmpty");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << true;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << false;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << false;
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << false;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << false;
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << false;
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << false;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << true;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << false;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << true;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << true;
}

void tst_QRect::newIsEmpty()
{
    // A new test is written so the old one isn't removed
    QFETCH( QRect, r );
    QFETCH( bool, isEmpty );

    QRectF rf(r);

    QVERIFY( r.isEmpty() == isEmpty );

    if (isLarge(r.x()) || isLarge(r.y()) || isLarge(r.width()) || isLarge(r.height()))
        return;
    QVERIFY( rf.isEmpty() == isEmpty );
}

void tst_QRect::newIsValid_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<bool>("isValid");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << false;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << true;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << true;
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << true;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << true;
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << true;
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << true;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << false;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << true;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << false;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << false;
}

void tst_QRect::newIsValid()
{
    // A new test is written so the old one isn't removed
    QFETCH( QRect, r );
    QFETCH( bool, isValid );

    QRectF rf(r);

    QVERIFY( r.isValid() == isValid );

    if (isLarge(r.x()) || isLarge(r.y()) || isLarge(r.width()) || isLarge(r.height()))
        return;

    QVERIFY( rf.isValid() == isValid );
}

void tst_QRect::normalized_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QRect>("nr");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << getQRectCase( InvalidQRect );
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << QRect( 1, 1, 1, 1 );
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect )
                                << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ) );
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect )
                                 << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect )
                                       << QRect( QPoint( INT_MIN, INT_MIN ), QSize( 1, 1 ) );
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect )
                                      << getQRectCase( LargestCoordQRect ); // overflow
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << QRect( 100, 200, 11, 16 );
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << QRect(-9, -9, 10, 10);
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << QRect( -10, -10, 5, 5 );
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << getQRectCase( NullQRect );
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << getQRectCase( EmptyQRect );
    BOBUIest::newRow( "ZeroWidth" ) << QRect(100, 200, 100, 0) << QRect(100, 200, 100, 0);
    BOBUIest::newRow( "ZeroHeight" ) << QRect(100, 200, 0, 100) << QRect(100, 200, 0, 100);
    // Since "NegativeSizeQRect passes, I expect both of these to pass too.
    // This passes, since height() returns -1 before normalization
    BOBUIest::newRow( "NegativeWidth") << QRect(QPoint(200,100), QSize(-1,100)) << QRect(QPoint(199,100), QSize(1,100));
    BOBUIest::newRow( "NegativeHeight") << QRect(QPoint(100,200), QSize(100,-1)) << QRect(QPoint(100,199), QSize(100,1));
    BOBUIest::newRow( "NegativeWidth2") << QRect(QPoint(200,100), QPoint(198,199)) << QRect(QPoint(199,100), QPoint(199,199));
    // This, on the other hand height() returns 0 before normalization.
    BOBUIest::newRow( "ZeroHeight1" ) << QRect(QPoint(100,200), QPoint(199,199)) << QRect(QPoint(100,200), QPoint(199,199));
    BOBUIest::newRow( "ZeroHeight2" ) << QRect(QPoint(263,113), QPoint(136,112)) << QRect(QPoint(137,113), QPoint(262,112));
}

void tst_QRect::normalized()
{
    QFETCH(QRect, r);
    QFETCH(QRect, nr);

    QCOMPARE(r.normalized(), nr);
}

void tst_QRect::left_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("left");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << 0;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MIN) / 2;
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << 0;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 100;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << 1;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -10;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << 5;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 2;
}

void tst_QRect::left()
{
    QFETCH( QRect, r );
    QFETCH( int, left );

    QRectF rf(r);

    QCOMPARE( r.left(), left );
    QCOMPARE(QRectF(rf).left(), qreal(left));
}

void tst_QRect::top_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("top");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << 0;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MIN) / 2;
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << 0;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 200;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << 1;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -10;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << 5;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 2;
}

void tst_QRect::top()
{
    QFETCH( QRect, r );
    QFETCH( int, top );

    QCOMPARE( r.top(), top );
    QCOMPARE(QRectF(r).top(), qreal(top));
}

void tst_QRect::right_data()
{
    // We don't test the NullQRect case as the return value is undefined.

    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("right");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << -1;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MAX / 2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << int(INT_MAX) - 1;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MAX);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 110;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << -10;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -6;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 1;
}

void tst_QRect::right()
{
    QFETCH( QRect, r );
    QFETCH( int, right );

    QCOMPARE( r.right(), right );
    QCOMPARE(QRectF(r).right(), qreal(right) + 1);
}

void tst_QRect::bottom_data()
{
    // We don't test the NullQRect case as the return value is undefined.

    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("bottom");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << -1;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MAX / 2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << int(INT_MAX - 1);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MAX);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 215;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << -10;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -6;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 1;
}


void tst_QRect::bottom()
{
    QFETCH( QRect, r );
    QFETCH( int, bottom );

    QCOMPARE( r.bottom(), bottom );
    QCOMPARE(QRectF(r).bottom(), qreal(bottom) + 1);
}

void tst_QRect::x_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("x");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << 0;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MIN / 2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << 0;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 100;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << 1;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -10;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << 5;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 2;

}

void tst_QRect::x()
{
    QFETCH( QRect, r );
    QFETCH( int, x );

    QCOMPARE( r.x(), x );
    QCOMPARE(QRectF(r).x(), qreal(x));
}

void tst_QRect::y_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("y");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << 0;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MIN / 2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << 0;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MIN);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 200;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << 1;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -10;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << 5;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 2;
}

void tst_QRect::y()
{
    QFETCH( QRect, r );
    QFETCH( int, y );

    QCOMPARE( r.y(), y );
    QCOMPARE(QRectF(r).y(), qreal(y));
}

void tst_QRect::setWidthHeight_data()
{
    BOBUIest::addColumn<int>("w");
    BOBUIest::addColumn<int>("h");

    BOBUIest::newRow("10x20") << 10 << 20;
    BOBUIest::newRow("-1x-1") << -1 << -1;
    BOBUIest::newRow("0x0") << 0 << 0;
    BOBUIest::newRow("-10x-100") << -10 << -100;
}

void tst_QRect::setWidthHeight()
{
    QFETCH(int, w);
    QFETCH(int, h);

    QRect r;
    r.setWidth(w);
    r.setHeight(h);

    QCOMPARE(r.width(), w);
    QCOMPARE(r.height(), h);

    QRectF rf;
    rf.setWidth(w);
    rf.setHeight(h);
    QCOMPARE(rf.width(), qreal(w));
    QCOMPARE(rf.height(), qreal(h));
}


void tst_QRect::setLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("left");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow( "InvalidQRect_MinimumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MaximumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953,0), QPoint(-1,-1) );
    }

    {
        BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN,1), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2,1), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,1), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2,1), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX,1), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953,1), QPoint(1,1) );
    }

    {
        BOBUIest::newRow( "MiddleQRect_MinimumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_MaximumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_RandomInt" ) << getQRectCase( MiddleQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953, INT_MIN / 2 ), QPoint( INT_MAX / 2, INT_MAX / 2 ));
    }

    {
        BOBUIest::newRow( "LargestQRect_MinimumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MaximumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_RandomInt" ) << getQRectCase( LargestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
    }

    {
        BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
    }

    {
        BOBUIest::newRow( "LargestCoordQRect_MinimumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddleNegativeInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_ZeroInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddlePositiveInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MaximumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_RandomInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
    }

    {
        BOBUIest::newRow( "RandomQRect_MinimumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, 200 ), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 200 ), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 200 ), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 200 ), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_MaximumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 200 ), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 200 ), QPoint( 110, 215 ) );
    }

    {
        BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, 1 ), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 1 ), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 1 ), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 1 ), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 1 ), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 1 ), QPoint( -10, -10 ) );
    }

    {
        BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, -10 ), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, -10 ), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, -10 ), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, -10 ), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, -10 ), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, -10 ), QPoint( -6, -6 ) );
    }

    {
        BOBUIest::newRow( "NullQRect_MinimumInt" ) << getQRectCase( NullQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, 5 ), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 5 ), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 5 ), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 5), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_MaximumInt" ) << getQRectCase( NullQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 5 ), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 5 ), QPoint( 4, 4 ) );
    }

    {
        BOBUIest::newRow( "EmptyQRect_MinimumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, 2 ), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 2 ), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 2 ), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 2 ), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MaximumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 2 ), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 2 ), QPoint( 1, 1 ) );
    }
}

void tst_QRect::setLeft()
{
    QFETCH( QRect, r );
    QFETCH( int, left );
    QFETCH( QRect, nr );

    QRectF rf(r);
    QRectF nrf(nr);

    r.setLeft( left );

    QCOMPARE( r, nr );

    rf.setLeft(left);
}

void tst_QRect::setTop_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("top");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow( "InvalidQRect_MinimumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(0,INT_MIN), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,INT_MIN/2), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,INT_MAX/2), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MaximumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(0,INT_MAX), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,4953), QPoint(-1,-1) );
    }

    {
        BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(1,INT_MIN), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,INT_MIN/2), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,0), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,INT_MAX/2), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,INT_MAX), QPoint(1,1) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,4953), QPoint(1,1) );
    }

    {
        BOBUIest::newRow( "MiddleQRect_MinimumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN/2,INT_MIN), QPoint( INT_MAX / 2, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2,INT_MIN/2), QPoint( INT_MAX / 2, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(INT_MIN/2,0), QPoint( INT_MAX / 2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MIN/2,INT_MAX/2), QPoint( INT_MAX / 2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_MaximumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MIN/2,INT_MAX), QPoint( INT_MAX / 2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_RandomInt" ) << getQRectCase( MiddleQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(INT_MIN/2,4953), QPoint( INT_MAX / 2, INT_MAX / 2 ));
    }

    {
        BOBUIest::newRow( "LargestQRect_MinimumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(0,INT_MIN), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,INT_MIN/2), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0, 0), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,INT_MAX/2), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MaximumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(0,INT_MAX), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_RandomInt" ) << getQRectCase( LargestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,4953), QPoint( INT_MAX - 1, INT_MAX - 1 ) );
    }

    {
        BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN,INT_MIN), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN,INT_MIN/2), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(INT_MIN,0), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MIN,INT_MAX/2), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MIN,INT_MAX), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(INT_MIN,4953), QPoint( INT_MIN, INT_MIN ) );
    }

    {
        BOBUIest::newRow( "LargestCoordQRect_MinimumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN,INT_MIN), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddleNegativeInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN,INT_MIN/2), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_ZeroInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(INT_MIN,0), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddlePositiveInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MIN,INT_MAX/2), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MaximumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MIN,INT_MAX), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_RandomInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(INT_MIN,4953), QPoint( INT_MAX, INT_MAX ) );
    }

    {
        BOBUIest::newRow( "RandomQRect_MinimumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(100,INT_MIN), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(100,INT_MIN/2), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(100,0), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(100,INT_MAX/2), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_MaximumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(100,INT_MAX), QPoint( 110, 215 ) );
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(100,4953), QPoint( 110, 215 ) );
    }

    {
        BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(1,INT_MIN), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,INT_MIN/2), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,0), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,INT_MAX/2), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,INT_MAX), QPoint( -10, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,4953), QPoint( -10, -10 ) );
    }

    {
        BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(-10,INT_MIN), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(-10,INT_MIN/2), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(-10,0), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(-10,INT_MAX/2), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(-10,INT_MAX), QPoint( -6, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(-10,4953), QPoint( -6, -6 ) );
    }

    {
        BOBUIest::newRow( "NullQRect_MinimumInt" ) << getQRectCase( NullQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(5,INT_MIN), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(5,INT_MIN/2), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(5,0), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(5,INT_MAX/2), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_MaximumInt" ) << getQRectCase( NullQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(5,INT_MAX), QPoint( 4, 4 ) );
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(5,4953), QPoint( 4, 4 ) );
    }

    {
        BOBUIest::newRow( "EmptyQRect_MinimumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(2,INT_MIN), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(2,INT_MIN/2), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(2,0), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(2,INT_MAX/2), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MaximumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(2,INT_MAX), QPoint( 1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(2,4953), QPoint( 1, 1 ) );
    }
}

void tst_QRect::setTop()
{
    QFETCH( QRect, r );
    QFETCH( int, top );
    QFETCH( QRect, nr );

    r.setTop( top );

    QCOMPARE( r, nr );
}

void tst_QRect::setRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("right");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow( "InvalidQRect_MinimumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(0,0), QPoint(INT_MIN,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,0), QPoint(INT_MIN/2,-1) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0), QPoint(0,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,0), QPoint(INT_MAX/2,-1) );
        BOBUIest::newRow( "InvalidQRect_MaximumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(0,0), QPoint(INT_MAX,-1) );
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,0), QPoint(4953,-1) );
    }

    {
        BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(1,1), QPoint(INT_MIN,1) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,1), QPoint(INT_MIN/2,1) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,1), QPoint(0,1) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,1), QPoint(INT_MAX/2,1) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,1), QPoint(INT_MAX,1) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,1), QPoint(4953,1) );
    }

    {
        BOBUIest::newRow( "MiddleQRect_MinimumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MIN, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MIN/2, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(0, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX/2, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_MaximumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX, INT_MAX / 2 ));
        BOBUIest::newRow( "MiddleQRect_RandomInt" ) << getQRectCase( MiddleQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(4953, INT_MAX / 2 ));
    }

    {
        BOBUIest::newRow( "LargestQRect_MinimumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 0, 0 ), QPoint( INT_MIN, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 0, 0 ), QPoint( INT_MIN/2, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 0 ), QPoint( 0, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 0, 0 ), QPoint( INT_MAX/2, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MaximumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 0, 0 ), QPoint( INT_MAX, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_RandomInt" ) << getQRectCase( LargestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 0, 0 ), QPoint( 4953, INT_MAX - 1 ) );
    }

    {
        BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN/2, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( 0, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX/2, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( 4953, INT_MIN ) );
    }

    {
        BOBUIest::newRow( "LargestCoordQRect_MinimumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddleNegativeInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN/2, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_ZeroInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( 0, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddlePositiveInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX/2, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_MaximumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_RandomInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( 4953, INT_MAX ) );
    }

    {
        BOBUIest::newRow( "RandomQRect_MinimumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 100, 200 ), QPoint( INT_MIN, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 100, 200 ), QPoint( INT_MIN/2, 215 ) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 100, 200 ), QPoint( 0, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 100, 200 ), QPoint( INT_MAX/2, 215 ) );
        BOBUIest::newRow( "RandomQRect_MaximumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 100, 200 ), QPoint( INT_MAX, 215 ) );
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 100, 200 ), QPoint( 4953, 215 ) );
    }

    {
        BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 1, 1 ), QPoint( INT_MIN, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 1, 1 ), QPoint( INT_MIN/2, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 1, 1 ), QPoint( 0, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 1, 1 ), QPoint( INT_MAX/2, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 1, 1 ), QPoint( INT_MAX, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 1, 1 ), QPoint( 4953, -10 ) );
    }

    {
        BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( -10, -10 ), QPoint( INT_MIN, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( -10, -10 ), QPoint( INT_MIN/2, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( -10, -10 ), QPoint( 0, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( -10, -10 ), QPoint( INT_MAX/2, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( -10, -10 ), QPoint( INT_MAX, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( -10, -10 ), QPoint( 4953, -6 ) );
    }

    {
        BOBUIest::newRow( "NullQRect_MinimumInt" ) << getQRectCase( NullQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 5, 5 ), QPoint( INT_MIN, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 5, 5 ), QPoint( INT_MIN/2, 4 ) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 5, 5 ), QPoint( 0, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 5, 5 ), QPoint( INT_MAX/2, 4 ) );
        BOBUIest::newRow( "NullQRect_MaximumInt" ) << getQRectCase( NullQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 5, 5 ), QPoint( INT_MAX, 4 ) );
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 5, 5 ), QPoint( 4953, 4 ) );
    }

    {
        BOBUIest::newRow( "EmptyQRect_MinimumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 2, 2 ), QPoint( INT_MIN, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 2, 2 ), QPoint( INT_MIN/2, 1 ) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 2, 2 ), QPoint( 0, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 2, 2 ), QPoint( INT_MAX/2, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MaximumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 2, 2 ), QPoint( INT_MAX, 1 ) );
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 2, 2 ), QPoint( 4953, 1 ) );
    }
}

void tst_QRect::setRight()
{
    QFETCH( QRect, r );
    QFETCH( int, right );
    QFETCH( QRect, nr );

    r.setRight( right );

    QCOMPARE( r, nr );
}

void tst_QRect::setBottom_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("bottom");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow( "InvalidQRect_MinimumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(0,0), QPoint(-1,INT_MIN) );
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,0), QPoint(-1,INT_MIN/2) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0), QPoint(-1,0) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,0), QPoint(-1,INT_MAX/2) );
        BOBUIest::newRow( "InvalidQRect_MaximumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(0,0), QPoint(-1,INT_MAX) );
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,0), QPoint(-1,4953) );
    }

    {
        BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(1,1), QPoint(1,INT_MIN) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,1), QPoint(1,INT_MIN/2) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,1), QPoint(1,0) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,1), QPoint(1,INT_MAX/2) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,1), QPoint(1,INT_MAX) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,1), QPoint(1,4953) );
    }

    {
        BOBUIest::newRow( "MiddleQRect_MinimumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX / 2, INT_MIN ) );
        BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX / 2, INT_MIN / 2 ) );
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX / 2, 0) );
        BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX / 2, INT_MAX / 2 ) );
        BOBUIest::newRow( "MiddleQRect_MaximumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX / 2, INT_MAX) );
        BOBUIest::newRow( "MiddleQRect_RandomInt" ) << getQRectCase( MiddleQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( INT_MIN / 2, INT_MIN / 2 ), QPoint(INT_MAX / 2, 4953) );
    }

    {
        BOBUIest::newRow( "LargestQRect_MinimumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, INT_MIN) );
        BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, INT_MIN/2) );
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, 0 ) );
        BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, INT_MAX/2 ) );
        BOBUIest::newRow( "LargestQRect_MaximumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, INT_MAX ) );
        BOBUIest::newRow( "LargestQRect_RandomInt" ) << getQRectCase( LargestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 0, 0 ), QPoint( INT_MAX - 1, 4953) );
    }

    {
        BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MIN ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MIN/2 ) );
        BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, 0 ) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MAX/2 ) );
        BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, INT_MAX ) );
        BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MIN, 4953 ) );
    }

    {
        BOBUIest::newRow( "LargestCoordQRect_MinimumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MIN ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddleNegativeInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MIN/2 ) );
        BOBUIest::newRow( "LargestCoordQRect_ZeroInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, 0 ) );
        BOBUIest::newRow( "LargestCoordQRect_MiddlePositiveInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MAX/2 ) );
        BOBUIest::newRow( "LargestCoordQRect_MaximumInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, INT_MAX ) );
        BOBUIest::newRow( "LargestCoordQRect_RandomInt" ) << getQRectCase( LargestCoordQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( INT_MIN, INT_MIN ), QPoint( INT_MAX, 4953 ) );
    }

    {
        BOBUIest::newRow( "RandomQRect_MinimumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 100, 200 ), QPoint( 110, INT_MIN ) );
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 100, 200 ), QPoint( 110, INT_MIN/2 ) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 100, 200 ), QPoint( 110, 0) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 100, 200 ), QPoint( 110, INT_MAX/2) );
        BOBUIest::newRow( "RandomQRect_MaximumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 100, 200 ), QPoint( 110, INT_MAX ) );
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 100, 200 ), QPoint( 110,4953 ) );
    }

    {
        BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 1, 1 ), QPoint( -10, INT_MIN ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 1, 1 ), QPoint( -10, INT_MIN/2 ) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 1, 1 ), QPoint( -10, 0 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 1, 1 ), QPoint( -10, INT_MAX/2 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 1, 1 ), QPoint( -10, INT_MAX ) );
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 1, 1 ), QPoint( -10, 4953 ) );
    }

    {
        BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( -10, -10 ), QPoint( -6, INT_MIN ) );
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( -10, -10 ), QPoint( -6, INT_MIN/2 ) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( -10, -10 ), QPoint( -6, 0 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( -10, -10 ), QPoint( -6, INT_MAX/2 ) );
        BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( -10, -10 ), QPoint( -6, INT_MAX ) );
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( -10, -10 ), QPoint( -6, 4953 ) );
    }

    {
        BOBUIest::newRow( "NullQRect_MinimumInt" ) << getQRectCase( NullQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 5, 5 ), QPoint( 4, INT_MIN ) );
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 5, 5 ), QPoint( 4, INT_MIN/2 ) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 5, 5 ), QPoint( 4, 0 ) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 5, 5 ), QPoint( 4, INT_MAX/2 ) );
        BOBUIest::newRow( "NullQRect_MaximumInt" ) << getQRectCase( NullQRect ) << getIntCase( MaximumInt )
                                             << QRect( QPoint( 5, 5 ), QPoint( 4, INT_MAX ) );
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 5, 5 ), QPoint( 4, 4953 ) );
    }

    {
        BOBUIest::newRow( "EmptyQRect_MinimumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( 2, 2 ), QPoint( 1, INT_MIN ) );
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( 2, 2 ), QPoint( 1, INT_MIN/2 ) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 2, 2 ), QPoint( 1, 0 ) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( 2, 2 ), QPoint( 1, INT_MAX/2 ) );
        BOBUIest::newRow( "EmptyQRect_MaximumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( 2, 2 ), QPoint( 1, INT_MAX ) );
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 2, 2 ), QPoint( 1, 4953 ) );
    }
}

void tst_QRect::setBottom()
{
    QFETCH( QRect, r );
    QFETCH( int, bottom );
    QFETCH( QRect, nr );

    r.setBottom( bottom );

    QCOMPARE( r, nr );
}

void tst_QRect::newSetTopLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("topLeft");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow("InvalidQRect_NullQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_SmallestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_MiddleNegCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_MiddlePosCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_LargestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_NegXQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_NegYQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(-1,-1));
        BOBUIest::newRow("InvalidQRect_RandomQPoint") << getQRectCase(InvalidQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(-1,-1));
    }

    {
        BOBUIest::newRow("SmallestQRect_NullQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_SmallestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_LargestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_NegXQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_NegYQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(1,1));
        BOBUIest::newRow("SmallestQRect_RandomQPoint") << getQRectCase(SmallestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(1,1));
    }

    {
        BOBUIest::newRow("MiddleQRect_NullQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_SmallestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_MiddleNegCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_MiddlePosCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_LargestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_NegXQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_NegYQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_RandomQPoint") << getQRectCase(MiddleQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(INT_MAX/2,INT_MAX/2));
    }

    {
        BOBUIest::newRow("LargestQRect_NullQPoint") << getQRectCase(LargestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_SmallestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_MiddleNegCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_MiddlePosCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_LargestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_NegXQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_NegYQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_RandomQPoint") << getQRectCase(LargestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(INT_MAX-1,INT_MAX-1));
    }

    {
        BOBUIest::newRow("SmallestCoordQRect_NullQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_SmallestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_LargestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_NegXQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_NegYQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_RandomQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(INT_MIN,INT_MIN));
    }

    {
        BOBUIest::newRow("LargestCoordQRect_NullQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_SmallestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_LargestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_NegXQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_NegYQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_RandomQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(INT_MAX,INT_MAX));
    }

    {
        BOBUIest::newRow("RandomQRect_NullQPoint") << getQRectCase(RandomQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_SmallestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_MiddleNegCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_MiddlePosCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_LargestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_NegXQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_NegYQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(110,215));
        BOBUIest::newRow("RandomQRect_RandomQPoint") << getQRectCase(RandomQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(110,215));
    }

    {
        BOBUIest::newRow("NegativeSizeQRect_NullQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_SmallestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_MiddleNegCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_MiddlePosCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_LargestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_NegXQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_NegYQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(-10,-10));
        BOBUIest::newRow("NegativeSizeQRect_RandomQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(-10,-10));
    }

    {
        BOBUIest::newRow("NegativePointQRect_NullQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_SmallestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_MiddleNegCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_MiddlePosCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_LargestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_NegXQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_NegYQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(-6,-6));
        BOBUIest::newRow("NegativePointQRect_RandomQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(-6,-6));
    }

    {
        BOBUIest::newRow("NullQRect_NullQPoint") << getQRectCase(NullQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(4,4));
        BOBUIest::newRow("NullQRect_SmallestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(4,4));
        BOBUIest::newRow("NullQRect_MiddleNegCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(4,4));
        BOBUIest::newRow("NullQRect_MiddlePosCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(4,4));
        BOBUIest::newRow("NullQRect_LargestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(4,4));
        BOBUIest::newRow("NullQRect_NegXQPoint") << getQRectCase(NullQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(4,4));
        BOBUIest::newRow("NullQRect_NegYQPoint") << getQRectCase(NullQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(4,4));
        BOBUIest::newRow("NullQRect_RandomQPoint") << getQRectCase(NullQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(4,4));
    }

    {
        BOBUIest::newRow("EmptyQRect_NullQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_SmallestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_MiddleNegCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_MiddlePosCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_LargestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_NegXQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_NegYQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(1,1));
        BOBUIest::newRow("EmptyQRect_RandomQPoint") << getQRectCase(EmptyQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(1,1));
    }
}

void tst_QRect::newSetTopLeft()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, topLeft );
    QFETCH( QRect, nr );

    r.setTopLeft( topLeft );
    QCOMPARE( r, nr );
}

void tst_QRect::newSetBottomRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("bottomRight");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow("InvalidQRect_NullQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(0,0));
        BOBUIest::newRow("InvalidQRect_SmallestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("InvalidQRect_MiddleNegCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("InvalidQRect_MiddlePosCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("InvalidQRect_LargestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("InvalidQRect_NegXQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(0,0), QPoint(-12,7));
        BOBUIest::newRow("InvalidQRect_NegYQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(0,0), QPoint(12,-7));
        BOBUIest::newRow("InvalidQRect_RandomQPoint") << getQRectCase(InvalidQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(0,0), QPoint(12,7));
    }

    {
        BOBUIest::newRow("SmallestQRect_NullQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(1,1), QPoint(0,0));
        BOBUIest::newRow("SmallestQRect_SmallestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("SmallestQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("SmallestQRect_LargestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("SmallestQRect_NegXQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(1,1), QPoint(-12,7));
        BOBUIest::newRow("SmallestQRect_NegYQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(1,1), QPoint(12,-7));
        BOBUIest::newRow("SmallestQRect_RandomQPoint") << getQRectCase(SmallestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(1,1), QPoint(12,7));
    }

    {
        BOBUIest::newRow("MiddleQRect_NullQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(QPoint(INT_MIN/2,INT_MIN/2)), QPoint(0,0));
        BOBUIest::newRow("MiddleQRect_SmallestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("MiddleQRect_MiddleNegCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("MiddleQRect_MiddlePosCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_LargestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("MiddleQRect_NegXQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(-12,7));
        BOBUIest::newRow("MiddleQRect_NegYQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(12,-7));
        BOBUIest::newRow("MiddleQRect_RandomQPoint") << getQRectCase(MiddleQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(12,7));
    }

    {
        BOBUIest::newRow("LargestQRect_NullQPoint") << getQRectCase(LargestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(0,0));
        BOBUIest::newRow("LargestQRect_SmallestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("LargestQRect_MiddleNegCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("LargestQRect_MiddlePosCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("LargestQRect_LargestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestQRect_NegXQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(0,0), QPoint(-12,7));
        BOBUIest::newRow("LargestQRect_NegYQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(0,0), QPoint(12,-7));
        BOBUIest::newRow("LargestQRect_RandomQPoint") << getQRectCase(LargestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(0,0), QPoint(12,7));
    }

    {
        BOBUIest::newRow("SmallestCoordQRect_NullQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(0,0));
        BOBUIest::newRow("SmallestCoordQRect_SmallestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("SmallestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("SmallestCoordQRect_LargestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("SmallestCoordQRect_NegXQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(-12,7));
        BOBUIest::newRow("SmallestCoordQRect_NegYQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(12,-7));
        BOBUIest::newRow("SmallestCoordQRect_RandomQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(12,7));
    }

    {
        BOBUIest::newRow("LargestCoordQRect_NullQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(0,0));
        BOBUIest::newRow("LargestCoordQRect_SmallestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("LargestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("LargestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("LargestCoordQRect_LargestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_NegXQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(-12,7));
        BOBUIest::newRow("LargestCoordQRect_NegYQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(12,-7));
        BOBUIest::newRow("LargestCoordQRect_RandomQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(12,7));
    }

    {
        BOBUIest::newRow("RandomQRect_NullQPoint") << getQRectCase(RandomQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(100,200), QPoint(0,0));
        BOBUIest::newRow("RandomQRect_SmallestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(100,200), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("RandomQRect_MiddleNegCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(100,200), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("RandomQRect_MiddlePosCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(100,200), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("RandomQRect_LargestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(100,200), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("RandomQRect_NegXQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(100,200), QPoint(-12,7));
        BOBUIest::newRow("RandomQRect_NegYQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(100,200), QPoint(12,-7));
        BOBUIest::newRow("RandomQRect_RandomQPoint") << getQRectCase(RandomQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(100,200), QPoint(12,7));
    }

    {
        BOBUIest::newRow("NegativeSizeQRect_NullQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(1,1), QPoint(0,0));
        BOBUIest::newRow("NegativeSizeQRect_SmallestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("NegativeSizeQRect_MiddleNegCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("NegativeSizeQRect_MiddlePosCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("NegativeSizeQRect_LargestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(1,1), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("NegativeSizeQRect_NegXQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(1,1), QPoint(-12,7));
        BOBUIest::newRow("NegativeSizeQRect_NegYQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(1,1), QPoint(12,-7));
        BOBUIest::newRow("NegativeSizeQRect_RandomQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(1,1), QPoint(12,7));
    }

    {
        BOBUIest::newRow("NegativePointQRect_NullQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(-10,-10), QPoint(0,0));
        BOBUIest::newRow("NegativePointQRect_SmallestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(-10,-10), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("NegativePointQRect_MiddleNegCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(-10,-10), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("NegativePointQRect_MiddlePosCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(-10,-10), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("NegativePointQRect_LargestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(-10,-10), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("NegativePointQRect_NegXQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-10,-10), QPoint(-12,7));
        BOBUIest::newRow("NegativePointQRect_NegYQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(-10,-10), QPoint(12,-7));
        BOBUIest::newRow("NegativePointQRect_RandomQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(-10,-10), QPoint(12,7));
    }

    {
        BOBUIest::newRow("NullQRect_NullQPoint") << getQRectCase(NullQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(5,5), QPoint(0,0));
        BOBUIest::newRow("NullQRect_SmallestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(5,5), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("NullQRect_MiddleNegCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(5,5), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("NullQRect_MiddlePosCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(5,5), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("NullQRect_LargestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(5,5), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("NullQRect_NegXQPoint") << getQRectCase(NullQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(5,5), QPoint(-12,7));
        BOBUIest::newRow("NullQRect_NegYQPoint") << getQRectCase(NullQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(5,5), QPoint(12,-7));
        BOBUIest::newRow("NullQRect_RandomQPoint") << getQRectCase(NullQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(5,5), QPoint(12,7));
    }

    {
        BOBUIest::newRow("EmptyQRect_NullQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(2,2), QPoint(0,0));
        BOBUIest::newRow("EmptyQRect_SmallestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(2,2), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("EmptyQRect_MiddleNegCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(2,2), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("EmptyQRect_MiddlePosCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(2,2), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("EmptyQRect_LargestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(2,2), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("EmptyQRect_NegXQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(2,2), QPoint(-12,7));
        BOBUIest::newRow("EmptyQRect_NegYQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(2,2), QPoint(12,-7));
        BOBUIest::newRow("EmptyQRect_RandomQPoint") << getQRectCase(EmptyQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(2,2), QPoint(12,7));
    }
}

void tst_QRect::newSetBottomRight()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, bottomRight );
    QFETCH( QRect, nr );

    r.setBottomRight( bottomRight );

    QCOMPARE( r, nr );
}

void tst_QRect::newSetTopRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("topRight");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow("InvalidQRect_NullQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(0,-1));
        BOBUIest::newRow("InvalidQRect_SmallestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(0,INT_MIN), QPoint(INT_MIN,-1));
        BOBUIest::newRow("InvalidQRect_MiddleNegCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(0,INT_MIN/2), QPoint(INT_MIN/2,-1));
        BOBUIest::newRow("InvalidQRect_MiddlePosCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(0,INT_MAX/2), QPoint(INT_MAX/2,-1));
        BOBUIest::newRow("InvalidQRect_LargestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(0,INT_MAX), QPoint(INT_MAX,-1));
        BOBUIest::newRow("InvalidQRect_NegXQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(0,7), QPoint(-12,-1));
        BOBUIest::newRow("InvalidQRect_NegYQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(0,-7), QPoint(12,-1));
        BOBUIest::newRow("InvalidQRect_RandomQPoint") << getQRectCase(InvalidQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(0,7), QPoint(12,-1));
    }

    {
        BOBUIest::newRow("SmallestQRect_NullQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(1,0), QPoint(0,1));
        BOBUIest::newRow("SmallestQRect_SmallestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(1,INT_MIN), QPoint(INT_MIN,1));
        BOBUIest::newRow("SmallestQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(1,INT_MIN/2), QPoint(INT_MIN/2,1));
        BOBUIest::newRow("SmallestQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(1,INT_MAX/2), QPoint(INT_MAX/2,1));
        BOBUIest::newRow("SmallestQRect_LargestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(1,INT_MAX), QPoint(INT_MAX,1));
        BOBUIest::newRow("SmallestQRect_NegXQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(1,7), QPoint(-12,1));
        BOBUIest::newRow("SmallestQRect_NegYQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(1,-7), QPoint(12,1));
        BOBUIest::newRow("SmallestQRect_RandomQPoint") << getQRectCase(SmallestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(1,7), QPoint(12,1));
    }

    {
        BOBUIest::newRow("MiddleQRect_NullQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(INT_MIN/2,0),QPoint(0,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_SmallestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN),QPoint(INT_MIN,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_MiddleNegCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2),QPoint(INT_MIN/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_MiddlePosCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MAX/2),QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_LargestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MAX),QPoint(INT_MAX,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_NegXQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(INT_MIN/2,7),QPoint(-12,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_NegYQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(INT_MIN/2,-7),QPoint(12,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_RandomQPoint") << getQRectCase(MiddleQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN/2,7),QPoint(12,INT_MAX/2));
    }

    {
        BOBUIest::newRow("LargestQRect_NullQPoint") << getQRectCase(LargestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0),QPoint(0,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_SmallestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(0,INT_MIN),QPoint(INT_MIN,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_MiddleNegCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(0,INT_MIN/2),QPoint(INT_MIN/2,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_MiddlePosCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(0,INT_MAX/2),QPoint(INT_MAX/2,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_LargestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(0,INT_MAX),QPoint(INT_MAX,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_NegXQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(0,7),QPoint(-12,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_NegYQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(0,-7),QPoint(12,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_RandomQPoint") << getQRectCase(LargestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(0,7),QPoint(12,INT_MAX-1));
    }

    {
        BOBUIest::newRow("SmallestCoordQRect_NullQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(INT_MIN,0),QPoint(0,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_SmallestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN),QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN/2),QPoint(INT_MIN/2,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MAX/2),QPoint(INT_MAX/2,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_LargestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MAX),QPoint(INT_MAX,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_NegXQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(INT_MIN,7),QPoint(-12,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_NegYQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(INT_MIN,-7),QPoint(12,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_RandomQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN,7),QPoint(12,INT_MIN));
    }

    {
        BOBUIest::newRow("LargestCoordQRect_NullQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(INT_MIN,0),QPoint(0,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_SmallestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN),QPoint(INT_MIN,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN/2),QPoint(INT_MIN/2,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MAX/2),QPoint(INT_MAX/2,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_LargestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MAX),QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_NegXQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(INT_MIN,7),QPoint(-12,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_NegYQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(INT_MIN,-7),QPoint(12,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_RandomQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN,7),QPoint(12,INT_MAX));
    }

    {
        BOBUIest::newRow("RandomQRect_NullQPoint") << getQRectCase(RandomQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(100,0),QPoint(0,215));
        BOBUIest::newRow("RandomQRect_SmallestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(100,INT_MIN),QPoint(INT_MIN,215));
        BOBUIest::newRow("RandomQRect_MiddleNegCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(100,INT_MIN/2),QPoint(INT_MIN/2,215));
        BOBUIest::newRow("RandomQRect_MiddlePosCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(100,INT_MAX/2),QPoint(INT_MAX/2,215));
        BOBUIest::newRow("RandomQRect_LargestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(100,INT_MAX),QPoint(INT_MAX,215));
        BOBUIest::newRow("RandomQRect_NegXQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(100,7),QPoint(-12,215));
        BOBUIest::newRow("RandomQRect_NegYQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(100,-7),QPoint(12,215));
        BOBUIest::newRow("RandomQRect_RandomQPoint") << getQRectCase(RandomQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(100,7),QPoint(12,215));
    }

    {
        BOBUIest::newRow("NegativeSizeQRect_NullQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(1, 0),QPoint(0,-10));
        BOBUIest::newRow("NegativeSizeQRect_SmallestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(1, INT_MIN),QPoint(INT_MIN,-10));
        BOBUIest::newRow("NegativeSizeQRect_MiddleNegCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(1, INT_MIN/2),QPoint(INT_MIN/2,-10));
        BOBUIest::newRow("NegativeSizeQRect_MiddlePosCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(1, INT_MAX/2),QPoint(INT_MAX/2,-10));
        BOBUIest::newRow("NegativeSizeQRect_LargestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(1, INT_MAX),QPoint(INT_MAX,-10));
        BOBUIest::newRow("NegativeSizeQRect_NegXQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(1, 7),QPoint(-12,-10));
        BOBUIest::newRow("NegativeSizeQRect_NegYQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(1, -7),QPoint(12,-10));
        BOBUIest::newRow("NegativeSizeQRect_RandomQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(1, 7),QPoint(12,-10));
    }

    {
        BOBUIest::newRow("NegativePointQRect_NullQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(-10,0),QPoint(0,-6));
        BOBUIest::newRow("NegativePointQRect_SmallestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(-10,INT_MIN),QPoint(INT_MIN,-6));
        BOBUIest::newRow("NegativePointQRect_MiddleNegCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(-10,INT_MIN/2),QPoint(INT_MIN/2,-6));
        BOBUIest::newRow("NegativePointQRect_MiddlePosCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(-10,INT_MAX/2),QPoint(INT_MAX/2,-6));
        BOBUIest::newRow("NegativePointQRect_LargestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(-10,INT_MAX),QPoint(INT_MAX,-6));
        BOBUIest::newRow("NegativePointQRect_NegXQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-10,7),QPoint(-12,-6));
        BOBUIest::newRow("NegativePointQRect_NegYQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(-10,-7),QPoint(12,-6));
        BOBUIest::newRow("NegativePointQRect_RandomQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(-10,7),QPoint(12,-6));
    }

    {
        BOBUIest::newRow("NullQRect_NullQPoint") << getQRectCase(NullQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(5,0),QPoint(0,4));
        BOBUIest::newRow("NullQRect_SmallestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(5,INT_MIN),QPoint(INT_MIN,4));
        BOBUIest::newRow("NullQRect_MiddleNegCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(5,INT_MIN/2),QPoint(INT_MIN/2,4));
        BOBUIest::newRow("NullQRect_MiddlePosCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(5,INT_MAX/2),QPoint(INT_MAX/2,4));
        BOBUIest::newRow("NullQRect_LargestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(5,INT_MAX),QPoint(INT_MAX,4));
        BOBUIest::newRow("NullQRect_NegXQPoint") << getQRectCase(NullQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(5,7),QPoint(-12,4));
        BOBUIest::newRow("NullQRect_NegYQPoint") << getQRectCase(NullQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(5,-7),QPoint(12,4));
        BOBUIest::newRow("NullQRect_RandomQPoint") << getQRectCase(NullQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(5,7),QPoint(12,4));
    }

    {
        BOBUIest::newRow("EmptyQRect_NullQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(2,0),QPoint(0,1));
        BOBUIest::newRow("EmptyQRect_SmallestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(2,INT_MIN),QPoint(INT_MIN,1));
        BOBUIest::newRow("EmptyQRect_MiddleNegCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(2,INT_MIN/2),QPoint(INT_MIN/2,1));
        BOBUIest::newRow("EmptyQRect_MiddlePosCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(2,INT_MAX/2),QPoint(INT_MAX/2,1));
        BOBUIest::newRow("EmptyQRect_LargestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(2,INT_MAX),QPoint(INT_MAX,1));
        BOBUIest::newRow("EmptyQRect_NegXQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(2,7),QPoint(-12,1));
        BOBUIest::newRow("EmptyQRect_NegYQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(2,-7),QPoint(12,1));
        BOBUIest::newRow("EmptyQRect_RandomQPoint") << getQRectCase(EmptyQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(2,7),QPoint(12,1));
    }
}

void tst_QRect::newSetTopRight()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, topRight );
    QFETCH( QRect, nr );

    r.setTopRight( topRight );

    QCOMPARE( r, nr );
}

void tst_QRect::newSetBottomLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("bottomLeft");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow("InvalidQRect_NullQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0),QPoint(-1,0));
        BOBUIest::newRow("InvalidQRect_SmallestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,0),QPoint(-1,INT_MIN));
        BOBUIest::newRow("InvalidQRect_MiddleNegCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,0),QPoint(-1,INT_MIN/2));
        BOBUIest::newRow("InvalidQRect_MiddlePosCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,0),QPoint(-1,INT_MAX/2));
        BOBUIest::newRow("InvalidQRect_LargestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,0),QPoint(-1,INT_MAX));
        BOBUIest::newRow("InvalidQRect_NegXQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,0),QPoint(-1,7));
        BOBUIest::newRow("InvalidQRect_NegYQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,0),QPoint(-1,-7));
        BOBUIest::newRow("InvalidQRect_RandomQPoint") << getQRectCase(InvalidQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,0),QPoint(-1,7));
    }

    {
        BOBUIest::newRow("SmallestQRect_NullQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,1),QPoint(1,0));
        BOBUIest::newRow("SmallestQRect_SmallestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,1),QPoint(1,INT_MIN));
        BOBUIest::newRow("SmallestQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,1),QPoint(1,INT_MIN/2));
        BOBUIest::newRow("SmallestQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,1), QPoint(1,INT_MAX/2));
        BOBUIest::newRow("SmallestQRect_LargestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,1), QPoint(1,INT_MAX));
        BOBUIest::newRow("SmallestQRect_NegXQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,1),QPoint(1,7));
        BOBUIest::newRow("SmallestQRect_NegYQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,1),QPoint(1,-7));
        BOBUIest::newRow("SmallestQRect_RandomQPoint") << getQRectCase(SmallestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,1),QPoint(1,7));
    }

    {
        BOBUIest::newRow("MiddleQRect_NullQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,INT_MIN/2),QPoint(INT_MAX/2,0));
        BOBUIest::newRow("MiddleQRect_SmallestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN/2),QPoint(INT_MAX/2,INT_MIN));
        BOBUIest::newRow("MiddleQRect_MiddleNegCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2),QPoint(INT_MAX/2,INT_MIN/2));
        BOBUIest::newRow("MiddleQRect_MiddlePosCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MIN/2),QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_LargestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MIN/2),QPoint(INT_MAX/2,INT_MAX));
        BOBUIest::newRow("MiddleQRect_NegXQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,INT_MIN/2),QPoint(INT_MAX/2,7));
        BOBUIest::newRow("MiddleQRect_NegYQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,INT_MIN/2),QPoint(INT_MAX/2,-7));
        BOBUIest::newRow("MiddleQRect_RandomQPoint") << getQRectCase(MiddleQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,INT_MIN/2),QPoint(INT_MAX/2,7));
    }

    {
        BOBUIest::newRow("LargestQRect_NullQPoint") << getQRectCase(LargestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0),QPoint(INT_MAX-1,0));
        BOBUIest::newRow("LargestQRect_SmallestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,0),QPoint(INT_MAX-1,INT_MIN));
        BOBUIest::newRow("LargestQRect_MiddleNegCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,0),QPoint(INT_MAX-1,INT_MIN/2));
        BOBUIest::newRow("LargestQRect_MiddlePosCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,0),QPoint(INT_MAX-1,INT_MAX/2));
        BOBUIest::newRow("LargestQRect_LargestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,0),QPoint(INT_MAX-1,INT_MAX));
        BOBUIest::newRow("LargestQRect_NegXQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,0),QPoint(INT_MAX-1,7));
        BOBUIest::newRow("LargestQRect_NegYQPoint") << getQRectCase(LargestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,0),QPoint(INT_MAX-1,-7));
        BOBUIest::newRow("LargestQRect_RandomQPoint") << getQRectCase(LargestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,0),QPoint(INT_MAX-1,7));
    }

    {
        BOBUIest::newRow("SmallestCoordQRect_NullQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,INT_MIN),QPoint(INT_MIN,0));
        BOBUIest::newRow("SmallestCoordQRect_SmallestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN),QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN),QPoint(INT_MIN,INT_MIN/2));
        BOBUIest::newRow("SmallestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MIN),QPoint(INT_MIN,INT_MAX/2));
        BOBUIest::newRow("SmallestCoordQRect_LargestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MIN),QPoint(INT_MIN,INT_MAX));
        BOBUIest::newRow("SmallestCoordQRect_NegXQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,INT_MIN),QPoint(INT_MIN,7));
        BOBUIest::newRow("SmallestCoordQRect_NegYQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,INT_MIN),QPoint(INT_MIN,-7));
        BOBUIest::newRow("SmallestCoordQRect_RandomQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,INT_MIN),QPoint(INT_MIN,7));
    }

    {
        BOBUIest::newRow("LargestCoordQRect_NullQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,INT_MIN),QPoint(INT_MAX,0));
        BOBUIest::newRow("LargestCoordQRect_SmallestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN),QPoint(INT_MAX,INT_MIN));
        BOBUIest::newRow("LargestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN),QPoint(INT_MAX,INT_MIN/2));
        BOBUIest::newRow("LargestCoordQRect_MiddlePosCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MIN),QPoint(INT_MAX,INT_MAX/2));
        BOBUIest::newRow("LargestCoordQRect_LargestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MIN),QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("LargestCoordQRect_NegXQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,INT_MIN),QPoint(INT_MAX,7));
        BOBUIest::newRow("LargestCoordQRect_NegYQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,INT_MIN),QPoint(INT_MAX,-7));
        BOBUIest::newRow("LargestCoordQRect_RandomQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,INT_MIN),QPoint(INT_MAX,7));
    }

    {
        BOBUIest::newRow("RandomQRect_NullQPoint") << getQRectCase(RandomQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,200),QPoint(110,0));
        BOBUIest::newRow("RandomQRect_SmallestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,200),QPoint(110,INT_MIN));
        BOBUIest::newRow("RandomQRect_MiddleNegCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,200),QPoint(110,INT_MIN/2));
        BOBUIest::newRow("RandomQRect_MiddlePosCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,200),QPoint(110,INT_MAX/2));
        BOBUIest::newRow("RandomQRect_LargestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,200),QPoint(110,INT_MAX));
        BOBUIest::newRow("RandomQRect_NegXQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,200),QPoint(110,7));
        BOBUIest::newRow("RandomQRect_NegYQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,200),QPoint(110,-7));
        BOBUIest::newRow("RandomQRect_RandomQPoint") << getQRectCase(RandomQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,200),QPoint(110,7));
    }

    {
        BOBUIest::newRow("NegativeSizeQRect_NullQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0, 1),QPoint(-10,0));
        BOBUIest::newRow("NegativeSizeQRect_SmallestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN, 1),QPoint(-10,INT_MIN));
        BOBUIest::newRow("NegativeSizeQRect_MiddleNegCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2, 1),QPoint(-10,INT_MIN/2));
        BOBUIest::newRow("NegativeSizeQRect_MiddlePosCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2, 1),QPoint(-10,INT_MAX/2));
        BOBUIest::newRow("NegativeSizeQRect_LargestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX, 1),QPoint(-10,INT_MAX));
        BOBUIest::newRow("NegativeSizeQRect_NegXQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12, 1),QPoint(-10,7));
        BOBUIest::newRow("NegativeSizeQRect_NegYQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12, 1),QPoint(-10,-7));
        BOBUIest::newRow("NegativeSizeQRect_RandomQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12, 1),QPoint(-10,7));
    }

    {
        BOBUIest::newRow("NegativePointQRect_NullQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,-10),QPoint(-6,0));
        BOBUIest::newRow("NegativePointQRect_SmallestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,-10),QPoint(-6,INT_MIN));
        BOBUIest::newRow("NegativePointQRect_MiddleNegCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,-10),QPoint(-6,INT_MIN/2));
        BOBUIest::newRow("NegativePointQRect_MiddlePosCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,-10),QPoint(-6,INT_MAX/2));
        BOBUIest::newRow("NegativePointQRect_LargestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,-10),QPoint(-6,INT_MAX));
        BOBUIest::newRow("NegativePointQRect_NegXQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,-10),QPoint(-6,7));
        BOBUIest::newRow("NegativePointQRect_NegYQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-10),QPoint(-6,-7));
        BOBUIest::newRow("NegativePointQRect_RandomQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,-10),QPoint(-6,7));
    }

    {
        BOBUIest::newRow("NullQRect_NullQPoint") << getQRectCase(NullQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,5),QPoint(4,0));
        BOBUIest::newRow("NullQRect_SmallestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,5),QPoint(4,INT_MIN));
        BOBUIest::newRow("NullQRect_MiddleNegCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,5),QPoint(4,INT_MIN/2));
        BOBUIest::newRow("NullQRect_MiddlePosCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,5),QPoint(4,INT_MAX/2));
        BOBUIest::newRow("NullQRect_LargestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,5),QPoint(4,INT_MAX));
        BOBUIest::newRow("NullQRect_NegXQPoint") << getQRectCase(NullQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,5),QPoint(4,7));
        BOBUIest::newRow("NullQRect_NegYQPoint") << getQRectCase(NullQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,5),QPoint(4,-7));
        BOBUIest::newRow("NullQRect_RandomQPoint") << getQRectCase(NullQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,5),QPoint(4,7));
    }

    {
        BOBUIest::newRow("EmptyQRect_NullQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,2),QPoint(1,0));
        BOBUIest::newRow("EmptyQRect_SmallestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,2),QPoint(1,INT_MIN));
        BOBUIest::newRow("EmptyQRect_MiddleNegCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,2),QPoint(1,INT_MIN/2));
        BOBUIest::newRow("EmptyQRect_MiddlePosCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,2),QPoint(1,INT_MAX/2));
        BOBUIest::newRow("EmptyQRect_LargestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,2),QPoint(1,INT_MAX));
        BOBUIest::newRow("EmptyQRect_NegXQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,2),QPoint(1,7));
        BOBUIest::newRow("EmptyQRect_NegYQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,2),QPoint(1,-7));
        BOBUIest::newRow("EmptyQRect_RandomQPoint") << getQRectCase(EmptyQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,2),QPoint(1,7));
    }
}

void tst_QRect::newSetBottomLeft()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, bottomLeft );
    QFETCH( QRect, nr );

    r.setBottomLeft( bottomLeft );

    QCOMPARE( r, nr );
}

void tst_QRect::topLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("topLeft");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << QPoint(0,0);
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << QPoint(1,1);
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << QPoint(INT_MIN/2,INT_MIN/2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << QPoint(0,0);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << QPoint(INT_MIN,INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << QPoint(INT_MIN,INT_MIN);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << QPoint(100,200);
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << QPoint(1,1);
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << QPoint(-10,-10);
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << QPoint(5,5);
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << QPoint(2,2);
}
void tst_QRect::topLeft()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, topLeft );

    QCOMPARE( r.topLeft(), topLeft );
}
void tst_QRect::bottomRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("bottomRight");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << QPoint(-1,-1);
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << QPoint(1,1);
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << QPoint(INT_MAX/2,INT_MAX/2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << QPoint(INT_MAX-1,INT_MAX-1);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << QPoint(INT_MIN,INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << QPoint(INT_MAX,INT_MAX);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << QPoint(110,215);
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << QPoint(-10,-10);
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << QPoint(-6,-6);
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << QPoint(4,4);
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << QPoint(1,1);
}
void tst_QRect::bottomRight()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, bottomRight );

    QCOMPARE( r.bottomRight(), bottomRight );
}
void tst_QRect::topRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("topRight");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << QPoint(-1,0);
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << QPoint(1,1);
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << QPoint(INT_MAX/2,INT_MIN/2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << QPoint(INT_MAX-1,0);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << QPoint(INT_MIN,INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << QPoint(INT_MAX,INT_MIN);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << QPoint(110,200);
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << QPoint(-10,1);
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << QPoint(-6,-10);
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << QPoint(4,5);
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << QPoint(1,2);
}
void tst_QRect::topRight()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, topRight );

    QCOMPARE( r.topRight(), topRight );
}
void tst_QRect::bottomLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("bottomLeft");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << QPoint(0,-1);
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << QPoint(1,1);
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << QPoint(INT_MIN/2,INT_MAX/2);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << QPoint(0,INT_MAX-1);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << QPoint(INT_MIN,INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << QPoint(INT_MIN,INT_MAX);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << QPoint(100,215);
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << QPoint(1,-10);
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << QPoint(-10,-6);
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << QPoint(5,4);
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << QPoint(2,1);
}
void tst_QRect::bottomLeft()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, bottomLeft );

    QCOMPARE( r.bottomLeft(), bottomLeft );
}
void tst_QRect::center_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("center");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << QPoint(0,0);
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << QPoint(1,1);
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << QPoint(0,0);
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << QPoint(INT_MAX/2,INT_MAX/2);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << QPoint(INT_MIN, INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << QPoint(0,0);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << QPoint(105,207);
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << QPoint(-4,-4);
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << QPoint(-8,-8);
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << QPoint(4,4);
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << QPoint(1,1);
}
void tst_QRect::center()
{
    QFETCH( QRect, r );
    QFETCH( QPoint, center );

    QCOMPARE( r.center(), center );
}

void tst_QRect::getRect_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("x");
    BOBUIest::addColumn<int>("y");
    BOBUIest::addColumn<int>("w");
    BOBUIest::addColumn<int>("h");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << 0 << 0 << 0 << 0;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1 << 1 << 1 << 1;
    // BOBUIest::newRow( "MiddleQRect" ) -- Not tested as this would cause an overflow
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << 0 << 0 << int(INT_MAX) << int(INT_MAX);
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN) << int(INT_MIN) << 1 << 1;
    // BOBUIest::newRow( "LargestCoordQRect" ) -- Not tested as this would cause an overflow
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 100 << 200 << 11 << 16;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << 1 << 1 << -10 << -10;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -10 << -10 << 5 << 5;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << 5 << 5 << 0 << 0;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 2 << 2 << 0 << 0;
}

void tst_QRect::getRect()
{
    QFETCH( QRect, r );
    QFETCH( int, x );
    QFETCH( int, y );
    QFETCH( int, w );
    QFETCH( int, h );

    int x2, y2, w2, h2;
    r.getRect( &x2, &y2, &w2, &h2 );

    QVERIFY( x == x2 );
    QVERIFY( y == y2 );
    QVERIFY( w == w2 );
    QVERIFY( h == h2 );
}

void tst_QRect::getCoords_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("xp1");
    BOBUIest::addColumn<int>("yp1");
    BOBUIest::addColumn<int>("xp2");
    BOBUIest::addColumn<int>("yp2");

    BOBUIest::newRow( "InvalidQRect" ) << getQRectCase( InvalidQRect ) << 0 << 0 << -1 << -1;
    BOBUIest::newRow( "SmallestQRect" ) << getQRectCase( SmallestQRect ) << 1 << 1 << 1 << 1;
    BOBUIest::newRow( "MiddleQRect" ) << getQRectCase( MiddleQRect ) << int(INT_MIN)/2 << int(INT_MIN)/2 << int(INT_MAX)/2 << int(INT_MAX)/2;
    BOBUIest::newRow( "LargestQRect" ) << getQRectCase( LargestQRect ) << 0 << 0 << int(INT_MAX)-1 << int(INT_MAX)-1;
    BOBUIest::newRow( "SmallestCoordQRect" ) << getQRectCase( SmallestCoordQRect ) << int(INT_MIN) << int(INT_MIN) << int(INT_MIN) << int(INT_MIN);
    BOBUIest::newRow( "LargestCoordQRect" ) << getQRectCase( LargestCoordQRect ) << int(INT_MIN) << int(INT_MIN) << int(INT_MAX) << int(INT_MAX);
    BOBUIest::newRow( "RandomQRect" ) << getQRectCase( RandomQRect ) << 100 << 200 << 110 << 215;
    BOBUIest::newRow( "NegativeSizeQRect" ) << getQRectCase( NegativeSizeQRect ) << 1 << 1 << -10 << -10;
    BOBUIest::newRow( "NegativePointQRect" ) << getQRectCase( NegativePointQRect ) << -10 << -10 << -6 << -6;
    BOBUIest::newRow( "NullQRect" ) << getQRectCase( NullQRect ) << 5 << 5 << 4 << 4;
    BOBUIest::newRow( "EmptyQRect" ) << getQRectCase( EmptyQRect ) << 2 << 2 << 1 << 1;
}

void tst_QRect::getCoords()
{
    QFETCH( QRect, r );
    QFETCH( int, xp1 );
    QFETCH( int, yp1 );
    QFETCH( int, xp2 );
    QFETCH( int, yp2 );

    int xp12, yp12, xp22, yp22;
    r.getCoords( &xp12, &yp12, &xp22, &yp22 );

    QVERIFY( xp1 == xp12 );
    QVERIFY( yp1 == yp12 );
    QVERIFY( xp2 == xp22 );
    QVERIFY( yp2 == yp22 );
}

void tst_QRect::newMoveLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("left");
    BOBUIest::addColumn<QRect>("nr");

    {
        // BOBUIest::newRow( "InvalidQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2,0), QPoint(INT_MIN/2-1,-1) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2,0), QPoint(INT_MAX/2-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MaximumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX,0), QPoint(INT_MAX-1,-1) );
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953,0), QPoint(4952,-1) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
        //                                          << QRect( QPoint(INT_MIN,1), QPoint(INT_MIN,1) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2,1), QPoint(INT_MIN/2,1) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,1), QPoint(0,1) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2,1), QPoint(INT_MAX/2,1) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX,1), QPoint(INT_MAX,1) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953,1), QPoint(4953,1) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_MinimumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MinimumInt )
        //                                         << QRect( QPoint(INT_MIN, INT_MIN / 2 ), QPoint( (INT_MAX/2)+(INT_MIN-INT_MIN/2), INT_MAX / 2 ) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddleNegativeInt )
        //                                                << QRect( QPoint(INT_MIN/2, INT_MIN / 2 ), QPoint((INT_MAX/2)+(INT_MIN/2-INT_MIN/2), INT_MAX / 2 ) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
        //                                      << QRect( QPoint(0, INT_MIN / 2 ), QPoint((INT_MAX/2)+(0-INT_MIN/2),INT_MAX/2));
        // BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
        BOBUIest::newRow( "LargestQRect_MinimumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN, 0), QPoint((INT_MAX-1)+INT_MIN, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2, 0), QPoint((INT_MAX-1)+(INT_MIN/2), INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0, 0), QPoint(INT_MAX-1,INT_MAX - 1 ) );
        // BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, INT_MIN ), QPoint(INT_MIN/2, INT_MIN ) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( ZeroInt )
        //                                      << QRect( QPoint( 0, INT_MIN ), QPoint(0, INT_MIN ) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddlePositiveInt )
        //                                                << QRect( QPoint( INT_MAX/2, INT_MIN ), QPoint(INT_MAX/2, INT_MIN ) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MaximumInt )
        //                                         << QRect( QPoint( INT_MAX, INT_MIN ), QPoint(INT_MAX, INT_MIN ) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( RandomInt )
        //                                        << QRect( QPoint( 4953, INT_MIN ), QPoint(4953, INT_MIN ) );
    }

    {
        // BOBUIest::newRow( "LargestCoordQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_MiddleNegativeInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_ZeroInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "RandomQRect_MinimumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MinimumInt )
        //                                         << QRect( QPoint( INT_MIN, 200 ), QPoint(10+INT_MIN, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 200 ), QPoint(10+INT_MIN/2, 215 ) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 200 ), QPoint(10, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 200 ), QPoint(10+INT_MAX/2, 215 ) );
        // BOBUIest::newRow( "RandomQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 200 ), QPoint(4963, 215 ) );
    }

    {
        // BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 1 ), QPoint(INT_MIN/2-11, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 1 ), QPoint(-11, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 1 ), QPoint(INT_MAX/2-11, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 1 ), QPoint(INT_MAX-11, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 1 ), QPoint(4942, -10 ) );
    }

    {
        BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint( INT_MIN, -10 ), QPoint(4+INT_MIN, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, -10 ), QPoint(4+INT_MIN/2, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, -10 ), QPoint(4, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, -10 ), QPoint(4+INT_MAX/2, -6 ) );
        // BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, -10 ), QPoint(4957, -6 ) );
    }

    {
        // BOBUIest::newRow( "NullQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 5 ), QPoint(INT_MIN/2-1, 4 ) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 5 ), QPoint(-1, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 5), QPoint(INT_MAX/2-1, 4 ) );
        BOBUIest::newRow( "NullQRect_MaximumInt" ) << getQRectCase( NullQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 5 ), QPoint(INT_MAX-1, 4 ) );
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 5 ), QPoint(4952, 4 ) );
    }

    {
        // BOBUIest::newRow( "EmptyQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint( INT_MIN/2, 2 ), QPoint(INT_MIN/2-1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint( 0, 2 ), QPoint(-1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint( INT_MAX/2, 2 ), QPoint(INT_MAX/2-1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MaximumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint( INT_MAX, 2 ), QPoint(INT_MAX-1, 1 ) );
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4953, 2 ), QPoint(4952, 1 ) );
    }
}

void tst_QRect::newMoveLeft()
{
    QFETCH( QRect, r );
    QFETCH( int, left );
    QFETCH( QRect, nr );

    r.moveLeft( left );

    QCOMPARE( r, nr );
}

void tst_QRect::newMoveTop_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("top");
    BOBUIest::addColumn<QRect>("nr");

    {
        // BOBUIest::newRow( "InvalidQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,INT_MIN/2), QPoint(-1,INT_MIN/2-1) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0), QPoint(-1,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,INT_MAX/2), QPoint(-1,INT_MAX/2-1) );
        BOBUIest::newRow( "InvalidQRect_MaximumInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(0,INT_MAX), QPoint(-1,INT_MAX-1) );
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,4953), QPoint(-1,4952) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
        //                                          << QRect( QPoint(1,INT_MIN), QPoint(1,INT_MIN) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,INT_MIN/2), QPoint(1,INT_MIN/2) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,0), QPoint(1,0) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,INT_MAX/2), QPoint(1,INT_MAX/2) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,INT_MAX), QPoint(1,INT_MAX) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,4953), QPoint(1,4953) );
    }

    {
        BOBUIest::newRow( "MiddleQRect_MinimumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(INT_MIN / 2,INT_MIN ), QPoint(INT_MAX / 2,(INT_MAX/2)+(INT_MIN-INT_MIN/2)) );
        BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN / 2,INT_MIN / 2), QPoint(INT_MAX / 2,(INT_MAX/2)+(INT_MIN/2-INT_MIN/2)) );
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(INT_MIN / 2,0 ), QPoint(INT_MAX/2,(INT_MAX/2)+(0-INT_MIN/2)));
        // BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
        BOBUIest::newRow( "LargestQRect_MinimumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(0,INT_MIN), QPoint(INT_MAX - 1,(INT_MAX-1)+INT_MIN) );
        BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,INT_MIN/2), QPoint(INT_MAX - 1,(INT_MAX-1)+(INT_MIN/2)) );
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0, 0), QPoint(INT_MAX-1,INT_MAX - 1) );
        // BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN,INT_MIN/2), QPoint(INT_MIN,INT_MIN/2) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( ZeroInt )
        //                                      << QRect( QPoint(INT_MIN,0), QPoint(INT_MIN,0) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddlePositiveInt )
        //                                                << QRect( QPoint(INT_MIN,INT_MAX/2), QPoint(INT_MIN,INT_MAX/2) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MaximumInt )
        //                                         << QRect( QPoint(INT_MIN,INT_MAX), QPoint(INT_MIN,INT_MAX) );
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( RandomInt )
        //                                        << QRect( QPoint(INT_MIN,4953), QPoint(INT_MIN,4953) );
    }

    {
        // BOBUIest::newRow( "LargestCoordQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_MiddleNegativeInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_ZeroInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestCoordQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "RandomQRect_MinimumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MinimumInt )
        //                                         << QRect( QPoint(100,INT_MIN), QPoint(110,15+INT_MIN) );
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(100,INT_MIN/2), QPoint(110,15+INT_MIN/2) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(100, 0), QPoint(110,15) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(100,INT_MAX/2), QPoint(110,15+INT_MAX/2) );
        // BOBUIest::newRow( "RandomQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(100,4953), QPoint(110,4968) );
    }

    {
        // BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,INT_MIN/2), QPoint(-10,INT_MIN/2-11) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,0), QPoint(-10,-11) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,INT_MAX/2), QPoint(-10,INT_MAX/2-11) );
        BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,INT_MAX), QPoint(-10,INT_MAX-11) );
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,4953), QPoint(-10,4942) );
    }

    {
        BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MinimumInt )
                                                << QRect( QPoint(-10,INT_MIN), QPoint(-6,4+INT_MIN) );
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(-10,INT_MIN/2), QPoint(-6,4+INT_MIN/2) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(-10,0), QPoint(-6,4) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(-10,INT_MAX/2), QPoint(-6,4+INT_MAX/2) );
        // BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(-10,4953), QPoint(-6,4957) );
    }

    {
        // BOBUIest::newRow( "NullQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(5,INT_MIN/2), QPoint(4,INT_MIN/2-1) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(5,0), QPoint(4,-1) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(5,INT_MAX/2), QPoint(4,INT_MAX/2-1) );
        BOBUIest::newRow( "NullQRect_MaximumInt" ) << getQRectCase( NullQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(5,INT_MAX), QPoint(4,INT_MAX-1) );
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(5,4953), QPoint(4,4952) );
    }

    {
        // BOBUIest::newRow( "EmptyQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(2,INT_MIN/2), QPoint(1,INT_MIN/2-1) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(2,0), QPoint(1,-1) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(2,INT_MAX/2), QPoint(1,INT_MAX/2-1) );
        BOBUIest::newRow( "EmptyQRect_MaximumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(2,INT_MAX), QPoint(1,INT_MAX-1) );
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(2,4953), QPoint(1,4952) );
    }
}

void tst_QRect::newMoveTop()
{
    QFETCH( QRect, r );
    QFETCH( int, top );
    QFETCH( QRect, nr );

    r.moveTop( top );

    QCOMPARE( r, nr );
}

void tst_QRect::newMoveRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("right");
    BOBUIest::addColumn<QRect>("nr");

    {
        // BOBUIest::newRow( "InvalidQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2+1,0), QPoint(INT_MIN/2,-1) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,0), QPoint(0,-1) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2+1,0), QPoint(INT_MAX/2,-1) );
        // BOBUIest::newRow( "InvalidQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4954,0), QPoint(4953,-1) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
        //                                          << QRect( QPoint(INT_MIN,1), QPoint(INT_MIN,1) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2,1), QPoint(INT_MIN/2,1) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,1), QPoint(0,1) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2,1), QPoint(INT_MAX/2,1) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(INT_MAX,1), QPoint(INT_MAX,1) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953,1), QPoint(4953,1) );
    }

    {
        // BOBUIest::newRow( "MiddleQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(INT_MIN/2+(0-INT_MAX/2),INT_MIN/2), QPoint(0,INT_MAX/2));
        BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddlePositiveInt )
                                                      << QRect(QPoint(INT_MIN/2+(INT_MAX/2-INT_MAX/2),INT_MIN/2),QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow( "MiddleQRect_MaximumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MaximumInt )
                                                      << QRect(QPoint(INT_MIN/2+(INT_MAX-INT_MAX/2),INT_MIN/2),QPoint(INT_MAX,INT_MAX/2));
        BOBUIest::newRow( "MiddleQRect_RandomInt" ) << getQRectCase( MiddleQRect ) << getIntCase( RandomInt )
                                              << QRect(QPoint(INT_MIN/2+(4953-INT_MAX/2),INT_MIN/2),QPoint(4953,INT_MAX/2));
    }

    {
        // BOBUIest::newRow( "LargestQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0-(INT_MAX-1), 0), QPoint(0, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2-(INT_MAX-1), 0), QPoint(INT_MAX/2, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_MaximumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MaximumInt )
                                                       << QRect( QPoint(INT_MAX-(INT_MAX-1), 0), QPoint(INT_MAX, INT_MAX - 1 ) );
        BOBUIest::newRow( "LargestQRect_RandomInt" ) << getQRectCase( LargestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4953-(INT_MAX-1), 0), QPoint(4953, INT_MAX - 1 ) );
    }

    {
        BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MinimumInt )
                                                       << QRect(QPoint(INT_MIN+(INT_MIN-INT_MIN),INT_MIN), QPoint(INT_MIN,INT_MIN) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect(QPoint(INT_MIN+(INT_MIN/2-INT_MIN),INT_MIN), QPoint(INT_MIN/2, INT_MIN ) );
        // BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
         // LargestQRect cases -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow( "RandomQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(100+(INT_MIN/2-110), 200 ), QPoint(INT_MIN/2, 215 ) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(-10, 200 ), QPoint(0, 215 ) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(100+(INT_MAX/2-110), 200 ), QPoint(INT_MAX/2, 215 ) );
        BOBUIest::newRow( "RandomQRect_MaximumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MaximumInt )
                                                       << QRect( QPoint(100+(INT_MAX-110), 200 ), QPoint(INT_MAX, 215 ) );
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4943, 200 ), QPoint(4953, 215 ) );
    }

    {
        BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MinimumInt )
                                                       << QRect( QPoint(1+(INT_MIN-(-10)), 1 ), QPoint(INT_MIN, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1+(INT_MIN/2-(-10)), 1 ), QPoint(INT_MIN/2, -10 ) );
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(11, 1 ), QPoint(0, -10 ) );
        // BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) -- Not tested as this would cause an overflow
        // BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4964, 1 ), QPoint(4953, -10 ) );
    }

    {
        // BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint((-10)+(INT_MIN/2-(-6)), -10 ), QPoint(INT_MIN/2, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(-4, -10 ), QPoint(0, -6 ) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint((-10)+(INT_MAX/2-(-6)), -10 ), QPoint(INT_MAX/2, -6 ) );
        // BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4949, -10 ), QPoint(4953, -6 ) );
    }

    {
        // BOBUIest::newRow( "NullQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(5+(INT_MIN/2-4), 5 ), QPoint(INT_MIN/2, 4 ) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1, 5 ), QPoint(0, 4 ) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(5+(INT_MAX/2-4), 5 ), QPoint(INT_MAX/2, 4 ) );
        // BOBUIest::newRow( "NullQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(4954, 5 ), QPoint(4953, 4 ) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "EmptyQRect_MinimumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MinimumInt )
        //                                                << QRect( QPoint(INT_MIN+1,2 ), QPoint(INT_MIN, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(INT_MIN/2+1, 2 ), QPoint(INT_MIN/2, 1 ) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,2 ), QPoint(0, 1 ) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(INT_MAX/2+1,2 ), QPoint(INT_MAX/2, 1 ) );
        // BOBUIest::newRow( "EmptyQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint( 4954, 2 ), QPoint(4953, 1 ) );
    }
}

void tst_QRect::newMoveRight()
{
    QFETCH( QRect, r );
    QFETCH( int, right );
    QFETCH( QRect, nr );

    r.moveRight( right );

    QCOMPARE( r, nr );
}

void tst_QRect::newMoveBottom_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<int>("bottom");
    BOBUIest::addColumn<QRect>("nr");

    {
        // BOBUIest::newRow( "InvalidQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "InvalidQRect_MiddleNegativeInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(0,INT_MIN/2+1), QPoint(-1,INT_MIN/2) );
        BOBUIest::newRow( "InvalidQRect_ZeroInt" ) << getQRectCase( InvalidQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,1), QPoint(-1,0) );
        BOBUIest::newRow( "InvalidQRect_MiddlePositiveInt" ) << getQRectCase( InvalidQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,INT_MAX/2+1), QPoint(-1,INT_MAX/2) );
        // BOBUIest::newRow( "InvalidQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "InvalidQRect_RandomInt" ) << getQRectCase( InvalidQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,4954), QPoint(-1,4953) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestQRect_MinimumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MinimumInt )
        //                                          << QRect( QPoint(1,INT_MIN), QPoint(1,INT_MIN) );
        BOBUIest::newRow( "SmallestQRect_MiddleNegativeInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,INT_MIN/2), QPoint(1,INT_MIN/2) );
        BOBUIest::newRow( "SmallestQRect_ZeroInt" ) << getQRectCase( SmallestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,0), QPoint(1,0) );
        BOBUIest::newRow( "SmallestQRect_MiddlePositiveInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(1,INT_MAX/2), QPoint(1,INT_MAX/2) );
        BOBUIest::newRow( "SmallestQRect_MaximumInt" ) << getQRectCase( SmallestQRect ) << getIntCase( MaximumInt )
                                                << QRect( QPoint(1,INT_MAX), QPoint(1,INT_MAX) );
        BOBUIest::newRow( "SmallestQRect_RandomInt" ) << getQRectCase( SmallestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,4953), QPoint(1,4953) );
    }

    {
        // BOBUIest::newRow( "MiddleQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "MiddleQRect_MiddleNegativeInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "MiddleQRect_ZeroInt" ) << getQRectCase( MiddleQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(INT_MIN/2,INT_MIN/2+(0-INT_MAX/2)), QPoint(INT_MAX/2,0));
        BOBUIest::newRow( "MiddleQRect_MiddlePositiveInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MiddlePositiveInt )
                                                      << QRect(QPoint(INT_MIN/2,INT_MIN/2+(INT_MAX/2-INT_MAX/2)),QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow( "MiddleQRect_MaximumInt" ) << getQRectCase( MiddleQRect ) << getIntCase( MaximumInt )
                                                      << QRect(QPoint(INT_MIN/2,INT_MIN/2+(INT_MAX-INT_MAX/2)),QPoint(INT_MAX/2,INT_MAX));
        BOBUIest::newRow( "MiddleQRect_RandomInt" ) << getQRectCase( MiddleQRect ) << getIntCase( RandomInt )
                                              << QRect(QPoint(INT_MIN/2,INT_MIN/2+(4953-INT_MAX/2)),QPoint(INT_MAX/2,4953));
    }

    {
        // BOBUIest::newRow( "LargestQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "LargestQRect_MiddleNegativeInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "LargestQRect_ZeroInt" ) << getQRectCase( LargestQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(0,0-(INT_MAX-1)), QPoint(INT_MAX - 1,0 ) );
        BOBUIest::newRow( "LargestQRect_MiddlePositiveInt" ) << getQRectCase( LargestQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(0,INT_MAX/2-(INT_MAX-1)), QPoint(INT_MAX - 1,INT_MAX/2 ) );
        BOBUIest::newRow( "LargestQRect_MaximumInt" ) << getQRectCase( LargestQRect ) << getIntCase( MaximumInt )
                                                       << QRect( QPoint(0,INT_MAX-(INT_MAX-1)), QPoint(INT_MAX - 1,INT_MAX) );
        BOBUIest::newRow( "LargestQRect_RandomInt" ) << getQRectCase( LargestQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(0,4953-(INT_MAX-1)), QPoint(INT_MAX - 1,4953) );
    }

    {
        BOBUIest::newRow( "SmallestCoordQRect_MinimumInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MinimumInt )
                                                       << QRect(QPoint(INT_MIN,INT_MIN+(INT_MIN-INT_MIN)), QPoint(INT_MIN,INT_MIN) );
        BOBUIest::newRow( "SmallestCoordQRect_MiddleNegativeInt" ) << getQRectCase( SmallestCoordQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect(QPoint(INT_MIN,INT_MIN+(INT_MIN/2-INT_MIN)), QPoint(INT_MIN,INT_MIN/2) );
        // BOBUIest::newRow( "SmallestCoordQRect_ZeroInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MiddlePositiveInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_MaximumInt" ) -- Not tested as it would cause an overflow
        // BOBUIest::newRow( "SmallestCoordQRect_RandomInt" ) -- Not tested as it would cause an overflow
    }

    {
         // LargestQRect cases -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow( "RandomQRect_MinimumInt" ) -- Not tested as it would cause an overflow
        BOBUIest::newRow( "RandomQRect_MiddleNegativeInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(100,200+(INT_MIN/2-215)), QPoint(110,INT_MIN/2) );
        BOBUIest::newRow( "RandomQRect_ZeroInt" ) << getQRectCase( RandomQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(100,-15), QPoint(110,0) );
        BOBUIest::newRow( "RandomQRect_MiddlePositiveInt" ) << getQRectCase( RandomQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(100,200+(INT_MAX/2-215)), QPoint(110,INT_MAX/2) );
        BOBUIest::newRow( "RandomQRect_MaximumInt" ) << getQRectCase( RandomQRect ) << getIntCase( MaximumInt )
                                                       << QRect( QPoint(100,200+(INT_MAX-215)), QPoint(110,INT_MAX) );
        BOBUIest::newRow( "RandomQRect_RandomInt" ) << getQRectCase( RandomQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(100,4938), QPoint(110,4953) );
    }

    {
        BOBUIest::newRow( "NegativeSizeQRect_MinimumInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MinimumInt )
                                                       << QRect( QPoint(1,1+(INT_MIN-(-10))), QPoint(-10,INT_MIN));
        BOBUIest::newRow( "NegativeSizeQRect_MiddleNegativeInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(1,1+(INT_MIN/2-(-10))), QPoint(-10,INT_MIN/2));
        BOBUIest::newRow( "NegativeSizeQRect_ZeroInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(1,11), QPoint(-10,0) );
        // BOBUIest::newRow( "NegativeSizeQRect_MiddlePositiveInt" ) -- Not tested as this would cause an overflow
        // BOBUIest::newRow( "NegativeSizeQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativeSizeQRect_RandomInt" ) << getQRectCase( NegativeSizeQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(1,4964), QPoint(-10,4953) );
    }

    {
        // BOBUIest::newRow( "NegativePointQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativePointQRect_MiddleNegativeInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(-10,(-10)+(INT_MIN/2-(-6))), QPoint(-6,INT_MIN/2) );
        BOBUIest::newRow( "NegativePointQRect_ZeroInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(-10,-4), QPoint(-6,0) );
        BOBUIest::newRow( "NegativePointQRect_MiddlePositiveInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(-10,(-10)+(INT_MAX/2-(-6))), QPoint(-6,INT_MAX/2) );
        // BOBUIest::newRow( "NegativePointQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NegativePointQRect_RandomInt" ) << getQRectCase( NegativePointQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(-10,4949), QPoint(-6,4953) );
    }

    {
        // BOBUIest::newRow( "NullQRect_MinimumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NullQRect_MiddleNegativeInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(5,5+(INT_MIN/2-4)), QPoint(4,INT_MIN/2) );
        BOBUIest::newRow( "NullQRect_ZeroInt" ) << getQRectCase( NullQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(5,1), QPoint(4,0) );
        BOBUIest::newRow( "NullQRect_MiddlePositiveInt" ) << getQRectCase( NullQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(5,5+(INT_MAX/2-4)), QPoint(4,INT_MAX/2) );
        // BOBUIest::newRow( "NullQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "NullQRect_RandomInt" ) << getQRectCase( NullQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(5,4954), QPoint(4,4953) );
    }

    {
        // Not tested as it would cause an overflow
        // BOBUIest::newRow( "EmptyQRect_MinimumInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MinimumInt )
        //                                                << QRect( QPoint(2,INT_MIN+1), QPoint(1,INT_MIN) );
        BOBUIest::newRow( "EmptyQRect_MiddleNegativeInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddleNegativeInt )
                                                       << QRect( QPoint(2,INT_MIN/2+1), QPoint(1,INT_MIN/2) );
        BOBUIest::newRow( "EmptyQRect_ZeroInt" ) << getQRectCase( EmptyQRect ) << getIntCase( ZeroInt )
                                             << QRect( QPoint(2,1), QPoint(1,0) );
        BOBUIest::newRow( "EmptyQRect_MiddlePositiveInt" ) << getQRectCase( EmptyQRect ) << getIntCase( MiddlePositiveInt )
                                                       << QRect( QPoint(2,INT_MAX/2+1), QPoint(1,INT_MAX/2) );
        // BOBUIest::newRow( "EmptyQRect_MaximumInt" ) -- Not tested as this would cause an overflow
        BOBUIest::newRow( "EmptyQRect_RandomInt" ) << getQRectCase( EmptyQRect ) << getIntCase( RandomInt )
                                               << QRect( QPoint(2,4954), QPoint(1,4953) );
    }
}

void tst_QRect::newMoveBottom()
{
    QFETCH( QRect, r );
    QFETCH( int, bottom );
    QFETCH( QRect, nr );

    r.moveBottom( bottom );

    QCOMPARE( r, nr );
}

void tst_QRect::newMoveTopLeft_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("topLeft");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow("InvalidQRect_NullQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-1,-1));
        // BOBUIest::newRow("InvalidQRect_SmallestCoordQPoint") -- Not tested as this would cause an overflow
        BOBUIest::newRow("InvalidQRect_MiddleNegCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(-1+(INT_MIN/2-0),-1+(INT_MIN/2-0)));
        BOBUIest::newRow("InvalidQRect_MiddlePosCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(-1+(INT_MAX/2-0),-1+(INT_MAX/2-0)));
        BOBUIest::newRow("InvalidQRect_LargestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(-1+(INT_MAX-0),-1+(INT_MAX-0)));
        BOBUIest::newRow("InvalidQRect_NegXQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-13,6));
        BOBUIest::newRow("InvalidQRect_NegYQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(11,-8));
        BOBUIest::newRow("InvalidQRect_RandomQPoint") << getQRectCase(InvalidQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(11,6));
    }

    {
        BOBUIest::newRow("SmallestQRect_NullQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(0,0));
        // Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestQRect_SmallestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(SmallestCoordQPoint)
        //     << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("SmallestQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("SmallestQRect_LargestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("SmallestQRect_NegXQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-12,7));
        BOBUIest::newRow("SmallestQRect_NegYQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(12,-7));
        BOBUIest::newRow("SmallestQRect_RandomQPoint") << getQRectCase(SmallestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(12,7));
    }

    {
        BOBUIest::newRow("MiddleQRect_NullQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX/2+(0-INT_MIN/2),INT_MAX/2+(0-INT_MIN/2)));
        BOBUIest::newRow("MiddleQRect_SmallestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX/2+(INT_MIN-INT_MIN/2),INT_MAX/2+(INT_MIN-INT_MIN/2)));
        BOBUIest::newRow("MiddleQRect_MiddleNegCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MAX/2,INT_MAX/2));
        // BOBUIest::newRow("MiddleQRect_MiddlePosCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("MiddleQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("MiddleQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("MiddleQRect_NegYQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("MiddleQRect_RandomQPoint") -- Not tested as it would cause an overflow
    }

    {
        BOBUIest::newRow("LargestQRect_NullQPoint") << getQRectCase(LargestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("LargestQRect_SmallestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX-1+INT_MIN,INT_MAX-1+INT_MIN));
        // BOBUIest::newRow("LargestQRect_MiddleNegCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_MiddlePosCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_NegYQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_RandomQPoint") -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow("SmallestCoordQRect_NullQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("SmallestCoordQRect_SmallestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(INT_MIN+(INT_MIN/2-INT_MIN),INT_MIN+(INT_MIN/2-INT_MIN)));
        // BOBUIest::newRow("SmallestCoordQRect_MiddlePosCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_NegYQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_RandomQPoint") -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow("LargestCoordQRect_NullQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("LargestCoordQRect_SmallestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MAX,INT_MAX));
        // BOBUIest::newRow("LargestCoordQRect_MiddleNegCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_MiddlePosCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_NegYQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_RandomQPoint") -- Not tested as it would cause an overflow
    }

    {
        BOBUIest::newRow("RandomQRect_NullQPoint") << getQRectCase(RandomQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(10,15));
        // BOBUIest::newRow("RandomQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("RandomQRect_MiddleNegCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(110+(INT_MIN/2-100),215+(INT_MIN/2-200)));
        BOBUIest::newRow("RandomQRect_MiddlePosCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(110+(INT_MAX/2-100),215+(INT_MAX/2-200)));
        // BOBUIest::newRow("RandomQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("RandomQRect_NegXQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-2,22));
        BOBUIest::newRow("RandomQRect_NegYQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(22,8));
        BOBUIest::newRow("RandomQRect_RandomQPoint") << getQRectCase(RandomQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(22,22));
    }

    {
        BOBUIest::newRow("NegativeSizeQRect_NullQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-11,-11));
        // BOBUIest::newRow("NegativeSizeQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NegativeSizeQRect_MiddleNegCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint(-10+((INT_MIN/2)-1),-10+((INT_MIN/2)-1)));
        BOBUIest::newRow("NegativeSizeQRect_MiddlePosCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint(-10+((INT_MAX/2)-1),-10+((INT_MAX/2)-1)));
        // BOBUIest::newRow("NegativeSizeQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NegativeSizeQRect_NegXQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-23,-4));
        BOBUIest::newRow("NegativeSizeQRect_NegYQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(1,-18));
        BOBUIest::newRow("NegativeSizeQRect_RandomQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(1,-4));
    }

    {
        BOBUIest::newRow("NegativePointQRect_NullQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(4,4));
        BOBUIest::newRow("NegativePointQRect_SmallestCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN,INT_MIN), QPoint(INT_MIN+4,INT_MIN+4));
        BOBUIest::newRow("NegativePointQRect_MiddleNegCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint((INT_MIN/2)+4,(INT_MIN/2)+4));
        BOBUIest::newRow("NegativePointQRect_MiddlePosCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint((INT_MAX/2)+4,(INT_MAX/2)+4));
        // BOBUIest::newRow("NegativePointQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NegativePointQRect_NegXQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-8,11));
        BOBUIest::newRow("NegativePointQRect_NegYQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(16,-3));
        BOBUIest::newRow("NegativePointQRect_RandomQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(16,11));
    }

    {
        BOBUIest::newRow("NullQRect_NullQPoint") << getQRectCase(NullQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-1,-1));
        // BOBUIest::newRow("NullQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NullQRect_MiddleNegCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint((INT_MIN/2)-1,(INT_MIN/2)-1));
        BOBUIest::newRow("NullQRect_MiddlePosCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint((INT_MAX/2)-1,(INT_MAX/2)-1));
        BOBUIest::newRow("NullQRect_LargestCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("NullQRect_NegXQPoint") << getQRectCase(NullQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-13,6));
        BOBUIest::newRow("NullQRect_NegYQPoint") << getQRectCase(NullQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(11,-8));
        BOBUIest::newRow("NullQRect_RandomQPoint") << getQRectCase(NullQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(11,6));
    }

    {
        BOBUIest::newRow("EmptyQRect_NullQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0,0), QPoint(-1,-1));
        // BOBUIest::newRow("EmptyQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("EmptyQRect_MiddleNegCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN/2,INT_MIN/2), QPoint((INT_MIN/2)-1,(INT_MIN/2)-1));
        BOBUIest::newRow("EmptyQRect_MiddlePosCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MAX/2,INT_MAX/2), QPoint((INT_MAX/2)-1,(INT_MAX/2)-1));
        BOBUIest::newRow("EmptyQRect_LargestCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MAX,INT_MAX), QPoint(INT_MAX-1,INT_MAX-1));
        BOBUIest::newRow("EmptyQRect_NegXQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(-12,7), QPoint(-13,6));
        BOBUIest::newRow("EmptyQRect_NegYQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(12,-7), QPoint(11,-8));
        BOBUIest::newRow("EmptyQRect_RandomQPoint") << getQRectCase(EmptyQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(12,7), QPoint(11,6));
    }
}

void tst_QRect::newMoveTopLeft()
{
    QFETCH(QRect,r);
    QFETCH(QPoint,topLeft);
    QFETCH(QRect,nr);

    r.moveTopLeft(topLeft);

    QCOMPARE(r,nr);
}

void tst_QRect::newMoveBottomRight_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("bottomRight");
    BOBUIest::addColumn<QRect>("nr");

    {
        BOBUIest::newRow("InvalidQRect_NullQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0+(0-(-1)),0+(0-(-1))), QPoint(0,0));
        BOBUIest::newRow("InvalidQRect_SmallestCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(0+(INT_MIN-(-1)),0+(INT_MIN-(-1))), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("InvalidQRect_MiddleNegCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(0+((INT_MIN/2)-(-1)),0+((INT_MIN/2)-(-1))), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("InvalidQRect_MiddlePosCoordQPoint") << getQRectCase(InvalidQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(0+((INT_MAX/2)-(-1)),0+((INT_MAX/2)-(-1))), QPoint(INT_MAX/2,INT_MAX/2));
        // BOBUIest::newRow("InvalidQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("InvalidQRect_NegXQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(0+(-12-(-1)),0+(7-(-1))), QPoint(-12,7));
        BOBUIest::newRow("InvalidQRect_NegYQPoint") << getQRectCase(InvalidQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(0+(12-(-1)),0+(-7-(-1))), QPoint(12,-7));
        BOBUIest::newRow("InvalidQRect_RandomQPoint") << getQRectCase(InvalidQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(0+(12-(-1)),0+(7-(-1))), QPoint(12,7));
    }

    {
        BOBUIest::newRow("SmallestQRect_NullQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(1+(0-1),1+(0-1)), QPoint(0,0));
        // BOBUIest::newRow("SmallestQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("SmallestQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(1+((INT_MIN/2)-1),1+((INT_MIN/2)-1)), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("SmallestQRect_MiddlePosCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(1+((INT_MAX/2)-1),1+((INT_MAX/2)-1)), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("SmallestQRect_LargestCoordQPoint") << getQRectCase(SmallestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(1+(INT_MAX-1),1+(INT_MAX-1)), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("SmallestQRect_NegXQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(1+(-12-1),1+(7-1)), QPoint(-12,7));
        BOBUIest::newRow("SmallestQRect_NegYQPoint") << getQRectCase(SmallestQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(1+(12-1),1+(-7-1)), QPoint(12,-7));
        BOBUIest::newRow("SmallestQRect_RandomQPoint") << getQRectCase(SmallestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(1+(12-1),1+(7-1)), QPoint(12,7));
    }

    {
        BOBUIest::newRow("MiddleQRect_NullQPoint") << getQRectCase(MiddleQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(INT_MIN/2+(0-(INT_MAX/2)),INT_MIN/2+(0-(INT_MAX/2))), QPoint(0,0));
        // BOBUIest::newRow("MiddleQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("MiddleQRect_MiddleNegCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("MiddleQRect_MiddlePosCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(INT_MIN/2+((INT_MAX/2)-(INT_MAX/2)),INT_MIN/2+((INT_MAX/2)-(INT_MAX/2))), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("MiddleQRect_LargestCoordQPoint") << getQRectCase(MiddleQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN/2+(INT_MAX-(INT_MAX/2)),INT_MIN/2+(INT_MAX-(INT_MAX/2))), QPoint(INT_MAX,INT_MAX));
        // BOBUIest::newRow("MiddleQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("MiddleQRect_NegYQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("MiddleQRect_RandomQPoint") << getQRectCase(MiddleQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(INT_MIN/2+(12-(INT_MAX/2)),INT_MIN/2+(7-(INT_MAX/2))), QPoint(12,7));
    }

    {
        BOBUIest::newRow("LargestQRect_NullQPoint") << getQRectCase(LargestQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(0+(0-(INT_MAX-1)),0+(0-(INT_MAX-1))), QPoint(0,0));
        // BOBUIest::newRow("LargestQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_MiddleNegCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("LargestQRect_MiddlePosCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(0+((INT_MAX/2)-(INT_MAX-1)),0+((INT_MAX/2)-(INT_MAX-1))), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("LargestQRect_LargestCoordQPoint") << getQRectCase(LargestQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(0+(INT_MAX-(INT_MAX-1)),0+(INT_MAX-(INT_MAX-1))), QPoint(INT_MAX,INT_MAX));
        // BOBUIest::newRow("LargestQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestQRect_NegYQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("LargestQRect_RandomQPoint") << getQRectCase(LargestQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(0+(12-(INT_MAX-1)),0+(7-(INT_MAX-1))), QPoint(12,7));
    }

    {
        // BOBUIest::newRow("SmallestCoordQRect_NullQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("SmallestCoordQRect_SmallestCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(INT_MIN+(INT_MIN-INT_MIN),INT_MIN+(INT_MIN-INT_MIN)), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("SmallestCoordQRect_MiddleNegCoordQPoint") << getQRectCase(SmallestCoordQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(INT_MIN+((INT_MIN/2)-INT_MIN),INT_MIN+((INT_MIN/2)-INT_MIN)), QPoint(INT_MIN/2,INT_MIN/2));
        // BOBUIest::newRow("SmallestCoordQRect_MiddlePosCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_NegYQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("SmallestCoordQRect_RandomQPoint") -- Not tested as it would cause an overflow
    }

    {
        // BOBUIest::newRow("LargestCoordQRect_NullQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_MiddleNegCoordQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_MiddlePosCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("LargestCoordQRect_LargestCoordQPoint") << getQRectCase(LargestCoordQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(INT_MIN+(INT_MAX-INT_MAX),INT_MIN+(INT_MAX-INT_MAX)), QPoint(INT_MAX,INT_MAX));
        // BOBUIest::newRow("LargestCoordQRect_NegXQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_NegYQPoint") -- Not tested as it would cause an overflow
        // BOBUIest::newRow("LargestCoordQRect_RandomQPoint") -- Not tested as it would cause an overflow
    }

    {
        BOBUIest::newRow("RandomQRect_NullQPoint") << getQRectCase(RandomQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(100+(0-110),200+(0-215)), QPoint(0,0));
        // BOBUIest::newRow("RandomQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("RandomQRect_MiddleNegCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(100+((INT_MIN/2)-110),200+((INT_MIN/2)-215)), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("RandomQRect_MiddlePosCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(100+((INT_MAX/2)-110),200+((INT_MAX/2)-215)), QPoint(INT_MAX/2,INT_MAX/2));
        BOBUIest::newRow("RandomQRect_LargestCoordQPoint") << getQRectCase(RandomQRect) << getQPointCase(LargestCoordQPoint)
            << QRect(QPoint(100+(INT_MAX-110),200+(INT_MAX-215)), QPoint(INT_MAX,INT_MAX));
        BOBUIest::newRow("RandomQRect_NegXQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(100+(-12-110),200+(7-215)), QPoint(-12,7));
        BOBUIest::newRow("RandomQRect_NegYQPoint") << getQRectCase(RandomQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(100+(12-110),200+(-7-215)), QPoint(12,-7));
        BOBUIest::newRow("RandomQRect_RandomQPoint") << getQRectCase(RandomQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(100+(12-110),200+(7-215)), QPoint(12,7));
    }

    {
        BOBUIest::newRow("NegativeSizeQRect_NullQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(1+(0-(-10)),1+(0-(-10))), QPoint(0,0));
        BOBUIest::newRow("NegativeSizeQRect_SmallestCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(SmallestCoordQPoint)
            << QRect(QPoint(1+(INT_MIN-(-10)),1+(INT_MIN-(-10))), QPoint(INT_MIN,INT_MIN));
        BOBUIest::newRow("NegativeSizeQRect_MiddleNegCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(1+((INT_MIN/2)-(-10)),1+((INT_MIN/2)-(-10))), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("NegativeSizeQRect_MiddlePosCoordQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(1+((INT_MAX/2)-(-10)),1+((INT_MAX/2)-(-10))), QPoint(INT_MAX/2,INT_MAX/2));
        // BOBUIest::newRow("NegativeSizeQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NegativeSizeQRect_NegXQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(1+(-12-(-10)),1+(7-(-10))), QPoint(-12,7));
        BOBUIest::newRow("NegativeSizeQRect_NegYQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(1+(12-(-10)),1+(-7-(-10))), QPoint(12,-7));
        BOBUIest::newRow("NegativeSizeQRect_RandomQPoint") << getQRectCase(NegativeSizeQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(1+(12-(-10)),1+(7-(-10))), QPoint(12,7));
    }

    {
        BOBUIest::newRow("NegativePointQRect_NullQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint((-10)+(0-(-6)),(-10)+(0-(-6))), QPoint(0,0));
        //  BOBUIest::newRow("NegativePointQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NegativePointQRect_MiddleNegCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint((-10)+((INT_MIN/2)-(-6)),(-10)+((INT_MIN/2)-(-6))), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("NegativePointQRect_MiddlePosCoordQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint((-10)+((INT_MAX/2)-(-6)),(-10)+((INT_MAX/2)-(-6))), QPoint(INT_MAX/2,INT_MAX/2));
        // BOBUIest::newRow("NegativePointQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NegativePointQRect_NegXQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint((-10)+(-12-(-6)),(-10)+(7-(-6))), QPoint(-12,7));
        BOBUIest::newRow("NegativePointQRect_NegYQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint((-10)+(12-(-6)),(-10)+(-7-(-6))), QPoint(12,-7));
        BOBUIest::newRow("NegativePointQRect_RandomQPoint") << getQRectCase(NegativePointQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint((-10)+(12-(-6)),(-10)+(7-(-6))), QPoint(12,7));
    }

    {
        BOBUIest::newRow("NullQRect_NullQPoint") << getQRectCase(NullQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(5+(0-4),5+(0-4)), QPoint(0,0));
        // BOBUIest::newRow("NullQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NullQRect_MiddleNegCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(5+((INT_MIN/2)-4),5+((INT_MIN/2)-4)), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("NullQRect_MiddlePosCoordQPoint") << getQRectCase(NullQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(5+((INT_MAX/2)-4),5+((INT_MAX/2)-4)), QPoint(INT_MAX/2,INT_MAX/2));
        // BOBUIest::newRow("NullQRect_LargestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("NullQRect_NegXQPoint") << getQRectCase(NullQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(5+(-12-4),5+(7-4)), QPoint(-12,7));
        BOBUIest::newRow("NullQRect_NegYQPoint") << getQRectCase(NullQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(5+(12-4),5+(-7-4)), QPoint(12,-7));
        BOBUIest::newRow("NullQRect_RandomQPoint") << getQRectCase(NullQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(5+(12-4),5+(7-4)), QPoint(12,7));
    }

    {
        BOBUIest::newRow("EmptyQRect_NullQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NullQPoint)
            << QRect(QPoint(2+(0-1),2+(0-1)), QPoint(0,0));
        // BOBUIest::newRow("EmptyQRect_SmallestCoordQPoint") -- Not tested as it would cause an overflow
        BOBUIest::newRow("EmptyQRect_MiddleNegCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddleNegCoordQPoint)
            << QRect(QPoint(2+((INT_MIN/2)-1),2+((INT_MIN/2)-1)), QPoint(INT_MIN/2,INT_MIN/2));
        BOBUIest::newRow("EmptyQRect_MiddlePosCoordQPoint") << getQRectCase(EmptyQRect) << getQPointCase(MiddlePosCoordQPoint)
            << QRect(QPoint(2+((INT_MAX/2)-1),2+((INT_MAX/2)-1)), QPoint(INT_MAX/2,INT_MAX/2));
        // BOBUIest::newRow("EmptyQRect_LargestCoordQPoint") << getQRectCase(EmptyQRect) -- Not tested as it would cause an overflow
        BOBUIest::newRow("EmptyQRect_NegXQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegXQPoint)
            << QRect(QPoint(2+(-12-1),2+(7-1)), QPoint(-12,7));
        BOBUIest::newRow("EmptyQRect_NegYQPoint") << getQRectCase(EmptyQRect) << getQPointCase(NegYQPoint)
            << QRect(QPoint(2+(12-1),2+(-7-1)), QPoint(12,-7));
        BOBUIest::newRow("EmptyQRect_RandomQPoint") << getQRectCase(EmptyQRect) << getQPointCase(RandomQPoint)
            << QRect(QPoint(2+(12-1),2+(7-1)), QPoint(12,7));
    }
}

void tst_QRect::newMoveBottomRight()
{
    QFETCH(QRect,r);
    QFETCH(QPoint,bottomRight);
    QFETCH(QRect,nr);

    r.moveBottomRight(bottomRight);

    QCOMPARE(r,nr);
}

void tst_QRect::margins()
{
    const QRect rectangle = QRect(QPoint(10, 10), QSize(50 ,50));
    const QMargins margins = QMargins(2, 3, 4, 5);

    const QRect added = rectangle + margins;
    QCOMPARE(added, QRect(QPoint(8, 7), QSize(56, 58)));
    QCOMPARE(added, margins + rectangle);
    QCOMPARE(added, rectangle.marginsAdded(margins));

    const QRect subtracted = rectangle - margins;
    QCOMPARE(subtracted, QRect(QPoint(12, 13), QSize(44, 42)));
    QCOMPARE(subtracted, rectangle.marginsRemoved(margins));

    QRect a = rectangle;
    a += margins;
    QCOMPARE(added, a);

    a = rectangle;
    a -= margins;
    QCOMPARE(a, QRect(QPoint(12, 13), QSize(44, 42)));
    QCOMPARE(a, rectangle.marginsRemoved(margins));
}

void tst_QRect::marginsf()
{
    const QRectF rectangle = QRectF(QPointF(10.5, 10.5), QSizeF(50.5 ,150.5));
    const QMarginsF margins = QMarginsF(2.5, 3.5, 4.5, 5.5);

    const QRectF added = rectangle + margins;
    QCOMPARE(added, QRectF(QPointF(8.0, 7.0), QSizeF(57.5, 159.5)));
    QCOMPARE(added, margins + rectangle);
    QCOMPARE(added, rectangle.marginsAdded(margins));

    const QRectF subtracted = rectangle - margins;
    QCOMPARE(subtracted, QRectF(QPointF(13.0, 14.0), QSizeF(43.5, 141.5)));
    QCOMPARE(subtracted, rectangle.marginsRemoved(margins));

    QRectF a = rectangle;
    a += margins;
    QCOMPARE(added, a);

    a = rectangle;
    a -= margins;
    QCOMPARE(a, QRectF(QPoint(13.0, 14.0), QSizeF(43.5, 141.5)));
    QCOMPARE(a, rectangle.marginsRemoved(margins));
}

void tst_QRect::toRectF_data()
{
    BOBUIest::addColumn<QRect>("input");
    BOBUIest::addColumn<QRectF>("result");

    auto row = [](int x1, int y1, int w, int h) {
        // QRectF -> QRect conversion tries to maintain size(), not bottomRight(),
        // so compare in (topLeft(), size()) space
        BOBUIest::addRow("((%d, %d) (%dx%d))", x1, y1, w, h)
                << QRect({x1, y1}, QSize{w, h}) << QRectF(QPointF(x1, y1), QSizeF(w, h));
    };
    constexpr std::array samples = {-1, 0, 1};
    for (int x1 : samples) {
        for (int y1 : samples) {
            for (int w : samples) {
                for (int h : samples) {
                    row(x1, y1, w, h);
                }
            }
        }
    }
}

void tst_QRect::toRectF()
{
    QFETCH(const QRect, input);
    QFETCH(const QRectF, result);

    QCOMPARE(result.toRect(), input); // consistency check
    QCOMPARE(input.toRectF(), result);
}


void tst_QRect::translate_data()
{
    BOBUIest::addColumn<QRect>("r");
    BOBUIest::addColumn<QPoint>("delta");
    BOBUIest::addColumn<QRect>("result");

    BOBUIest::newRow("Case 1") << QRect(10,20,5,15) << QPoint(3,7) << QRect(13,27,5,15);
    BOBUIest::newRow("Case 2") << QRect(0,0,-1,-1) << QPoint(3,7) << QRect(3,7,-1,-1);
    BOBUIest::newRow("Case 3") << QRect(0,0,-1,-1) << QPoint(0,0) << QRect(0,0,-1,-1);
    BOBUIest::newRow("Case 4") << QRect(10,20,5,15) << QPoint(3,0) << QRect(13,20, 5,15);
    BOBUIest::newRow("Case 5") << QRect(10,20,5,15) << QPoint(0,7) << QRect(10,27,5,15);
    BOBUIest::newRow("Case 6") << QRect(10,20,5,15) << QPoint(-3,7) << QRect(7,27,5,15);
    BOBUIest::newRow("Case 7") << QRect(10,20,5,15) << QPoint(3,-7) << QRect(13,13,5,15);
}

void tst_QRect::translate()
{
    QFETCH(QRect, r);
    QFETCH(QPoint, delta);
    QFETCH(QRect, result);

    QRect oldr = r;
    QRect r2 = r;
    r2.translate(delta);
    QCOMPARE(result,r2);

    r2 = r;
    r2.translate(delta.x(), delta.y());
    QCOMPARE(result,r2);

    r2 = r.translated(delta);
    QCOMPARE(result,r2);
    QCOMPARE(oldr,r); //r should not change

    r2 = r.translated(delta.x(), delta.y());
    QCOMPARE(result,r2);
    QCOMPARE(oldr,r);

}

void tst_QRect::transposed_data()
{
    BOBUIest::addColumn<QRect>("r");

    BOBUIest::newRow("InvalidQRect") << getQRectCase(InvalidQRect);
    BOBUIest::newRow("SmallestQRect") << getQRectCase(SmallestQRect);
    // Not tested as it would cause an overflow
    // BOBUIest::newRow("MiddleQRect") << getQRectCase(MiddleQRect);
    BOBUIest::newRow("LargestQRect") << getQRectCase(LargestQRect);
    BOBUIest::newRow("SmallestCoordQRect") << getQRectCase(SmallestCoordQRect);
    // Not tested as it would cause an overflow
    // BOBUIest::newRow("LargestCoordQRect") << getQRectCase(LargestCoordQRect);
    BOBUIest::newRow("RandomQRect") << getQRectCase(RandomQRect);
    BOBUIest::newRow("NegativeSizeQRect") << getQRectCase(NegativeSizeQRect);
    BOBUIest::newRow("NegativePointQRect") << getQRectCase(NegativePointQRect);
    BOBUIest::newRow("NullQRect") << getQRectCase(NullQRect);
    BOBUIest::newRow("EmptyQRect") << getQRectCase(EmptyQRect);
}

void tst_QRect::transposed()
{
    QFETCH(QRect, r);

    const QRect rt = r.transposed();
    QCOMPARE(rt.height(), r.width());
    QCOMPARE(rt.width(), r.height());
    QCOMPARE(rt.topLeft(), r.topLeft());

    const QRectF rf = r;

    const QRectF rtf = rf.transposed();
    QCOMPARE(rtf.height(), rf.width());
    QCOMPARE(rtf.width(), rf.height());
    QCOMPARE(rtf.topLeft(), rf.topLeft());

    QCOMPARE(rtf, QRectF(rt));
}

void tst_QRect::moveTop()
{
    {
        QRect r( 10, 10, 100, 100 );
        r.moveTop( 3 );
        QCOMPARE( r, QRect(10, 3, 100, 100) );

        r = QRect( 10, 3, 100, 100 );
        r.moveTop( -22 );
        QCOMPARE( r, QRect(10, -22, 100, 100) );
    }
    {
        QRectF r( 10, 10, 100, 100 );
        r.moveTop( 3 );
        QCOMPARE( r, QRectF(10, 3, 100, 100) );

        r = QRectF( 10, 3, 100, 100 );
        r.moveTop( -22 );
        QCOMPARE( r, QRectF(10, -22, 100, 100) );
    }
}

void tst_QRect::moveBottom()
{
    {
        QRect r( 10, -22, 100, 100 );
        r.moveBottom( 104 );
        QCOMPARE( r, QRect(10, 5, 100, 100) );
    }
    {
        QRectF r( 10, -22, 100, 100 );
        r.moveBottom( 104 );
        QCOMPARE( r, QRectF(10, 4, 100, 100) );
    }
}

void tst_QRect::moveLeft()
{
    {
        QRect r( 10, 5, 100, 100 );
        r.moveLeft( 11 );
        QCOMPARE( r, QRect(11, 5, 100, 100) );
    }
    {
        QRectF r( 10, 5, 100, 100 );
        r.moveLeft( 11 );
        QCOMPARE( r, QRectF(11, 5, 100, 100) );
    }
}

void tst_QRect::moveRight()
{
    {
        QRect r( 11, 5, 100, 100 );
        r.moveRight( 106 );
        QCOMPARE( r, QRect(7, 5, 100, 100) );
    }
    {
        QRectF r( 11, 5, 100, 100 );
        r.moveRight( 106 );
        QCOMPARE( r, QRectF(6, 5, 100, 100) );
    }
}

void tst_QRect::moveTopLeft()
{
    {
        QRect r( 7, 5, 100, 100 );
        r.moveTopLeft( QPoint(1, 2) );
        QCOMPARE( r, QRect(1, 2, 100, 100) );
    }
    {
        QRectF r( 7, 5, 100, 100 );
        r.moveTopLeft( QPoint(1, 2) );
        QCOMPARE( r, QRectF(1, 2, 100, 100) );
    }
}

void tst_QRect::moveTopRight()
{
    {
        QRect r( 1, 2, 100, 100 );
        r.moveTopRight( QPoint(103, 5) );
        QCOMPARE( r, QRect(4, 5, 100, 100) );
    }
    {
        QRectF r( 1, 2, 100, 100 );
        r.moveTopRight( QPoint(103, 5) );
        QCOMPARE( r, QRectF(3, 5, 100, 100) );
    }
}

void tst_QRect::moveBottomLeft()
{
    {
        QRect r( 4, 5, 100, 100 );
        r.moveBottomLeft( QPoint(3, 105) );
        QCOMPARE( r, QRect(3, 6, 100, 100) );
    }
    {
        QRectF r( 4, 5, 100, 100 );
        r.moveBottomLeft( QPoint(3, 105) );
        QCOMPARE( r, QRectF(3, 5, 100, 100) );
    }
}

void tst_QRect::moveBottomRight()
{
    {
        QRect r( 3, 6, 100, 100 );
        r.moveBottomRight( QPoint(103, 105) );
        QCOMPARE( r, QRect(4, 6, 100, 100) );
    }
    {
        QRectF r( 3, 6, 100, 100 );
        r.moveBottomRight( QPoint(103, 105) );
        QCOMPARE( r, QRectF(3, 5, 100, 100) );
    }
}

void tst_QRect::setTopLeft()
{
    {
        QRect r( 20, 10, 200, 100 );
        r.setTopLeft( QPoint(5, 7) );
        QCOMPARE( r, QRect(5, 7, 215, 103) );
    }
    {
        QRectF r( 20, 10, 200, 100 );
        r.setTopLeft( QPoint(5, 7) );
        QCOMPARE( r, QRectF(5, 7, 215, 103) );
    }
}

void tst_QRect::setTopRight()
{
    {
        QRect r( 20, 10, 200, 100 );
        r.setTopRight( QPoint(225, 7) );
        QCOMPARE( r, QRect(20, 7, 206, 103) );
    }
    {
        QRectF r( 20, 10, 200, 100 );
        r.setTopRight( QPoint(225, 7) );
        QCOMPARE( r, QRectF(20, 7, 205, 103) );
    }
}

void tst_QRect::setBottomLeft()
{
    {
        QRect r( 20, 10, 200, 100 );
        r.setBottomLeft( QPoint(5, 117) );
        QCOMPARE( r, QRect(5, 10, 215, 108) );
    }
    {
        QRectF r( 20, 10, 200, 100 );
        r.setBottomLeft( QPoint(5, 117) );
        QCOMPARE( r, QRectF(5, 10, 215, 107) );
    }
}

void tst_QRect::setBottomRight()
{
    {
    QRect r( 20, 10, 200, 100 );
    r.setBottomRight( QPoint(225, 117) );
    QCOMPARE( r, QRect(20, 10, 206, 108) );
    }
    {
    QRectF r( 20, 10, 200, 100 );
    r.setBottomRight( QPoint(225, 117) );
    QCOMPARE( r, QRectF(20, 10, 205, 107) );
    }
}

void tst_QRect::operator_amp()
{
    QRect r( QPoint(20, 10), QPoint(200, 100) );
    QRect r2( QPoint(50, 50), QPoint(300, 300) );
    QRect r3 = r & r2;
    QCOMPARE( r3, QRect( QPoint(50, 50), QPoint(200, 100) ) );
    QVERIFY( !r3.isEmpty() );
    QVERIFY( r3.isValid() );

    QRect r4( QPoint(300, 300), QPoint(400, 400) );
    QRect r5 = r & r4;
    QVERIFY( r5.isEmpty() );
    QVERIFY( !r5.isValid() );
}

void tst_QRect::operator_amp_eq()
{
    QRect r( QPoint(20, 10), QPoint(200, 100) );
    QRect r2( QPoint(50, 50), QPoint(300, 300) );
    r &= r2;
    QCOMPARE( r, QRect( QPoint(50, 50), QPoint(200, 100) ) );
    QVERIFY( !r.isEmpty() );
    QVERIFY( r.isValid() );

    QRect r3( QPoint(300, 300), QPoint(400, 400) );
    r &= r3;
    QVERIFY( r.isEmpty() );
    QVERIFY( !r.isValid() );
}

void tst_QRect::isValid()
{
    QRect r;
    QVERIFY( !r.isValid() );
    QRect r2( 0, 0, 0, 0 );
    QVERIFY( !r2.isValid() );
    QRect r3( 100, 200, 300, 400 );
    QVERIFY( r3.isValid() );
}

void tst_QRect::isEmpty()
{
    QRect r;
    QVERIFY( r.isEmpty() );
    QRect r2( 0, 0, 0, 0 );
    QVERIFY( r2.isEmpty() );
    QRect r3( 100, 200, 300, 400 );
    QVERIFY( !r3.isEmpty() );
    QRect r4( QPoint(300, 100), QPoint(200, 200) );
    QVERIFY( r4.isEmpty() );
    QRect r5( QPoint(200, 200), QPoint(200, 100) );
    QVERIFY( r5.isEmpty() );
    QRect r6( QPoint(300, 200), QPoint(200, 100) );
    QVERIFY( r6.isEmpty() );
}


void tst_QRect::testAdjust_data()
{
    BOBUIest::addColumn<QRect>("original");
    BOBUIest::addColumn<int>("x1_adjust");
    BOBUIest::addColumn<int>("y1_adjust");
    BOBUIest::addColumn<int>("x2_adjust");
    BOBUIest::addColumn<int>("y2_adjust");
    BOBUIest::addColumn<QRect>("expected");

    BOBUIest::newRow("test 01") << QRect(13, 12, 11, 10) << 4 << 3 << 2 << 1 << QRect(17, 15,  9,  8);
    BOBUIest::newRow("test 02") << QRect(13, 12, 11, 10) << 4 << 3 << 2 << 0 << QRect(17, 15,  9,  7);
    BOBUIest::newRow("test 03") << QRect(13, 12, 11, 10) << 4 << 3 << 0 << 1 << QRect(17, 15,  7,  8);
    BOBUIest::newRow("test 04") << QRect(13, 12, 11, 10) << 4 << 3 << 0 << 0 << QRect(17, 15,  7,  7);
    BOBUIest::newRow("test 05") << QRect(13, 12, 11, 10) << 4 << 0 << 2 << 1 << QRect(17, 12,  9, 11);
    BOBUIest::newRow("test 06") << QRect(13, 12, 11, 10) << 4 << 0 << 2 << 0 << QRect(17, 12,  9, 10);
    BOBUIest::newRow("test 07") << QRect(13, 12, 11, 10) << 4 << 0 << 0 << 1 << QRect(17, 12,  7, 11);
    BOBUIest::newRow("test 08") << QRect(13, 12, 11, 10) << 4 << 0 << 0 << 0 << QRect(17, 12,  7, 10);
    BOBUIest::newRow("test 09") << QRect(13, 12, 11, 10) << 0 << 3 << 2 << 1 << QRect(13, 15, 13,  8);
    BOBUIest::newRow("test 10") << QRect(13, 12, 11, 10) << 0 << 3 << 2 << 0 << QRect(13, 15, 13,  7);
    BOBUIest::newRow("test 11") << QRect(13, 12, 11, 10) << 0 << 3 << 0 << 1 << QRect(13, 15, 11,  8);
    BOBUIest::newRow("test 12") << QRect(13, 12, 11, 10) << 0 << 3 << 0 << 0 << QRect(13, 15, 11,  7);
    BOBUIest::newRow("test 13") << QRect(13, 12, 11, 10) << 0 << 0 << 2 << 1 << QRect(13, 12, 13, 11);
    BOBUIest::newRow("test 14") << QRect(13, 12, 11, 10) << 0 << 0 << 2 << 0 << QRect(13, 12, 13, 10);
    BOBUIest::newRow("test 15") << QRect(13, 12, 11, 10) << 0 << 0 << 0 << 1 << QRect(13, 12, 11, 11);
    BOBUIest::newRow("test 16") << QRect(13, 12, 11, 10) << 0 << 0 << 0 << 0 << QRect(13, 12, 11, 10);

    BOBUIest::newRow("test 17") << QRect(13, 12, 11, 10) << -4 << -3 << -2 << -1 << QRect( 9,  9, 13, 12);
    BOBUIest::newRow("test 18") << QRect(13, 12, 11, 10) << -4 << -3 << -2 <<  0 << QRect( 9,  9, 13, 13);
    BOBUIest::newRow("test 19") << QRect(13, 12, 11, 10) << -4 << -3 <<  0 << -1 << QRect( 9,  9, 15, 12);
    BOBUIest::newRow("test 20") << QRect(13, 12, 11, 10) << -4 << -3 <<  0 <<  0 << QRect( 9,  9, 15, 13);
    BOBUIest::newRow("test 21") << QRect(13, 12, 11, 10) << -4 <<  0 << -2 << -1 << QRect( 9, 12, 13,  9);
    BOBUIest::newRow("test 22") << QRect(13, 12, 11, 10) << -4 <<  0 << -2 <<  0 << QRect( 9, 12, 13, 10);
    BOBUIest::newRow("test 23") << QRect(13, 12, 11, 10) << -4 <<  0 <<  0 << -1 << QRect( 9, 12, 15,  9);
    BOBUIest::newRow("test 24") << QRect(13, 12, 11, 10) << -4 <<  0 <<  0 <<  0 << QRect( 9, 12, 15, 10);
    BOBUIest::newRow("test 25") << QRect(13, 12, 11, 10) <<  0 << -3 << -2 << -1 << QRect(13,  9,  9, 12);
    BOBUIest::newRow("test 26") << QRect(13, 12, 11, 10) <<  0 << -3 << -2 <<  0 << QRect(13,  9,  9, 13);
    BOBUIest::newRow("test 27") << QRect(13, 12, 11, 10) <<  0 << -3 <<  0 << -1 << QRect(13,  9, 11, 12);
    BOBUIest::newRow("test 28") << QRect(13, 12, 11, 10) <<  0 << -3 <<  0 <<  0 << QRect(13,  9, 11, 13);
    BOBUIest::newRow("test 29") << QRect(13, 12, 11, 10) <<  0 <<  0 << -2 << -1 << QRect(13, 12,  9,  9);
    BOBUIest::newRow("test 30") << QRect(13, 12, 11, 10) <<  0 <<  0 << -2 <<  0 << QRect(13, 12,  9, 10);
    BOBUIest::newRow("test 31") << QRect(13, 12, 11, 10) <<  0 <<  0 <<  0 << -1 << QRect(13, 12, 11,  9);
    BOBUIest::newRow("test 32") << QRect(13, 12, 11, 10) <<  0 <<  0 <<  0 <<  0 << QRect(13, 12, 11, 10);

}

void tst_QRect::testAdjust()
{
    QFETCH(QRect, original);
    QFETCH(int, x1_adjust);
    QFETCH(int, y1_adjust);
    QFETCH(int, x2_adjust);
    QFETCH(int, y2_adjust);
    QFETCH(QRect, expected);

    {
        QRect r1 = original;
        r1.adjust(x1_adjust, y1_adjust, x2_adjust, y2_adjust);
        QCOMPARE(r1.x(), expected.x());
        QCOMPARE(r1.y(), expected.y());
        QCOMPARE(r1.width(), expected.width());
        QCOMPARE(r1.height(), expected.height());

        QRect r2 = original.adjusted(x1_adjust, y1_adjust, x2_adjust, y2_adjust);
        QCOMPARE(r2.x(), expected.x());
        QCOMPARE(r2.y(), expected.y());
        QCOMPARE(r2.width(), expected.width());
        QCOMPARE(r2.height(), expected.height());
    }
    {
        QRectF expectedF(expected);

        QRectF r1 = original;
        r1.adjust(x1_adjust, y1_adjust, x2_adjust, y2_adjust);
        QCOMPARE(r1.x(), expectedF.x());
        QCOMPARE(r1.y(), expectedF.y());
        QCOMPARE(r1.width(), expectedF.width());
        QCOMPARE(r1.height(), expectedF.height());

        QRectF r2 = original.adjusted(x1_adjust, y1_adjust, x2_adjust, y2_adjust);
        QCOMPARE(r2.x(), expectedF.x());
        QCOMPARE(r2.y(), expectedF.y());
        QCOMPARE(r2.width(), expectedF.width());
        QCOMPARE(r2.height(), expectedF.height());
    }
}

void tst_QRect::intersectedRect_data()
{
    BOBUIest::addColumn<QRect>("rect1");
    BOBUIest::addColumn<QRect>("rect2");
    BOBUIest::addColumn<QRect>("intersect");

    BOBUIest::newRow("test 01") << QRect(0, 0, 10, 10) << QRect( 2,  2,  6,  6) << QRect(2, 2,  6,  6);
    BOBUIest::newRow("test 02") << QRect(0, 0, 10, 10) << QRect( 0,  0, 10, 10) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 03") << QRect(0, 0, 10, 10) << QRect( 2,  2, 10, 10) << QRect(2, 2,  8,  8);
    BOBUIest::newRow("test 04") << QRect(0, 0, 10, 10) << QRect(20, 20, 10, 10) << QRect();

    BOBUIest::newRow("test 05") << QRect(10, 10, -10, -10) << QRect( 2,  2,  6,  6) << QRect(2, 2,  6,  6);
    BOBUIest::newRow("test 06") << QRect(10, 10, -10, -10) << QRect( 0,  0, 10, 10) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 07") << QRect(10, 10, -10, -10) << QRect( 2,  2, 10, 10) << QRect(2, 2,  8,  8);
    BOBUIest::newRow("test 08") << QRect(10, 10, -10, -10) << QRect(20, 20, 10, 10) << QRect();

    BOBUIest::newRow("test 09") << QRect(0, 0, 10, 10) << QRect( 6,  6,  -4,  -4) << QRect(2, 2,  4,  4);
    BOBUIest::newRow("test 10") << QRect(0, 0, 10, 10) << QRect(10, 10, -10, -10) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 11") << QRect(0, 0, 10, 10) << QRect(12, 12, -10, -10) << QRect(2, 2,  8,  8);
    BOBUIest::newRow("test 12") << QRect(0, 0, 10, 10) << QRect(30, 30, -10, -10) << QRect();

    BOBUIest::newRow("test 13") << QRect(0, 0, 10, 10) << QRect() << QRect();
    BOBUIest::newRow("test 14") << QRect() << QRect(0, 0, 10, 10) << QRect();
    BOBUIest::newRow("test 15") << QRect() << QRect() << QRect();
    BOBUIest::newRow("test 16") << QRect(2, 0, 1, 652) << QRect(2, 0, 1, 652) << QRect(2, 0, 1, 652);
}

void tst_QRect::intersectedRect()
{
    QFETCH(QRect, rect1);
    QFETCH(QRect, rect2);
    QFETCH(QRect, intersect);

    if (intersect.isValid())
        QCOMPARE(rect1.intersected(rect2), intersect);
    else
        QVERIFY(rect1.intersected(rect2).isEmpty());

    QRect wayOutside(rect1.right() + 100, rect1.bottom() + 100, 10, 10);
    QRect empty = rect1 & wayOutside;
    QVERIFY(empty.intersected(rect2).isEmpty());
}

void tst_QRect::intersectedRectF_data()
{
    BOBUIest::addColumn<QRectF>("rect1");
    BOBUIest::addColumn<QRectF>("rect2");
    BOBUIest::addColumn<QRectF>("intersect");

    BOBUIest::newRow("test 01") << QRectF(0, 0, 10, 10) << QRectF( 2,  2,  6,  6) << QRectF(2, 2,  6,  6);
    BOBUIest::newRow("test 02") << QRectF(0, 0, 10, 10) << QRectF( 0,  0, 10, 10) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 03") << QRectF(0, 0, 10, 10) << QRectF( 2,  2, 10, 10) << QRectF(2, 2,  8,  8);
    BOBUIest::newRow("test 04") << QRectF(0, 0, 10, 10) << QRectF(20, 20, 10, 10) << QRectF();

    BOBUIest::newRow("test 05") << QRectF(10, 10, -10, -10) << QRectF( 2,  2,  6,  6) << QRectF(2, 2,  6,  6);
    BOBUIest::newRow("test 06") << QRectF(10, 10, -10, -10) << QRectF( 0,  0, 10, 10) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 07") << QRectF(10, 10, -10, -10) << QRectF( 2,  2, 10, 10) << QRectF(2, 2,  8,  8);
    BOBUIest::newRow("test 08") << QRectF(10, 10, -10, -10) << QRectF(20, 20, 10, 10) << QRectF();

    BOBUIest::newRow("test 09") << QRectF(0, 0, 10, 10) << QRectF( 8,  8,  -6,  -6) << QRectF(2, 2,  6,  6);
    BOBUIest::newRow("test 10") << QRectF(0, 0, 10, 10) << QRectF(10, 10, -10, -10) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 11") << QRectF(0, 0, 10, 10) << QRectF(12, 12, -10, -10) << QRectF(2, 2,  8,  8);
    BOBUIest::newRow("test 12") << QRectF(0, 0, 10, 10) << QRectF(30, 30, -10, -10) << QRectF();

    BOBUIest::newRow("test 13") << QRectF(-1, 1, 10, 10) << QRectF() << QRectF();
    BOBUIest::newRow("test 14") << QRectF() << QRectF(0, 0, 10, 10) << QRectF();
    BOBUIest::newRow("test 15") << QRectF() << QRectF() << QRectF();
}

void tst_QRect::intersectedRectF()
{
    QFETCH(QRectF, rect1);
    QFETCH(QRectF, rect2);
    QFETCH(QRectF, intersect);

    if (intersect.isValid())
        QCOMPARE(rect1.intersected(rect2), intersect);
    else
        QVERIFY(rect1.intersected(rect2).isEmpty());

    QRectF wayOutside(rect1.right() + 100.0, rect1.bottom() + 100.0, 10.0, 10.0);
    QRectF empty = rect1 & wayOutside;
    QVERIFY(empty.intersected(rect2).isEmpty());
}

void tst_QRect::unitedRect_data()
{
    BOBUIest::addColumn<QRect>("rect1");
    BOBUIest::addColumn<QRect>("rect2");
    BOBUIest::addColumn<QRect>("unite");

    BOBUIest::newRow("test 01") << QRect(0, 0, 10, 10) << QRect( 2,  2,  6,  6) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 02") << QRect(0, 0, 10, 10) << QRect( 0,  0, 10, 10) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 03") << QRect(0, 0, 10, 10) << QRect( 2,  2, 10, 10) << QRect(0, 0, 12, 12);
    BOBUIest::newRow("test 04") << QRect(0, 0, 10, 10) << QRect(20, 20, 10, 10) << QRect(0, 0, 30, 30);

    BOBUIest::newRow("test 05") << QRect(10, 10, -10, -10) << QRect( 2,  2,  6,  6) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 06") << QRect(10, 10, -10, -10) << QRect( 0,  0, 10, 10) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 07") << QRect(10, 10, -10, -10) << QRect( 2,  2, 10, 10) << QRect(0, 0, 12, 12);
    BOBUIest::newRow("test 08") << QRect(10, 10, -10, -10) << QRect(20, 20, 10, 10) << QRect(0, 0, 30, 30);

    BOBUIest::newRow("test 09") << QRect(0, 0, 10, 10) << QRect( 7,  7, -4, -4) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 10") << QRect(0, 0, 10, 10) << QRect( 9,  9, -8, -8) << QRect(0, 0, 10, 10);
    BOBUIest::newRow("test 11") << QRect(0, 0, 10, 10) << QRect(12, 12, -8, -8) << QRect(0, 0, 12, 12);
    BOBUIest::newRow("test 12") << QRect(0, 0, 10, 10) << QRect(30, 30, -8, -8) << QRect(0, 0, 30, 30);

    BOBUIest::newRow("test 13") << QRect() << QRect(10, 10, 10, 10) << QRect(10, 10, 10, 10);
    BOBUIest::newRow("test 14") << QRect(10, 10, 10, 10) << QRect() << QRect(10, 10, 10, 10);
    BOBUIest::newRow("test 15") << QRect() << QRect() << QRect();
    BOBUIest::newRow("test 16") << QRect(0, 0, 100, 0) << QRect(0, 0, 0, 100) << QRect(0, 0, 100, 100);
}

void tst_QRect::unitedRect()
{
    QFETCH(QRect, rect1);
    QFETCH(QRect, rect2);
    QFETCH(QRect, unite);

    QCOMPARE(rect1.united(rect2), unite);
}

void tst_QRect::unitedRectF_data()
{
    BOBUIest::addColumn<QRectF>("rect1");
    BOBUIest::addColumn<QRectF>("rect2");
    BOBUIest::addColumn<QRectF>("unite");

    BOBUIest::newRow("test 01") << QRectF(0, 0, 10, 10) << QRectF( 2,  2,  6,  6) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 02") << QRectF(0, 0, 10, 10) << QRectF( 0,  0, 10, 10) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 03") << QRectF(0, 0, 10, 10) << QRectF( 2,  2, 10, 10) << QRectF(0, 0, 12, 12);
    BOBUIest::newRow("test 04") << QRectF(0, 0, 10, 10) << QRectF(20, 20, 10, 10) << QRectF(0, 0, 30, 30);

    BOBUIest::newRow("test 05") << QRectF(10, 10, -10, -10) << QRectF( 2,  2,  6,  6) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 06") << QRectF(10, 10, -10, -10) << QRectF( 0,  0, 10, 10) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 07") << QRectF(10, 10, -10, -10) << QRectF( 2,  2, 10, 10) << QRectF(0, 0, 12, 12);
    BOBUIest::newRow("test 08") << QRectF(10, 10, -10, -10) << QRectF(20, 20, 10, 10) << QRectF(0, 0, 30, 30);

    BOBUIest::newRow("test 09") << QRectF(0, 0, 10, 10) << QRectF( 8,  8,  -6,  -6) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 10") << QRectF(0, 0, 10, 10) << QRectF(10, 10, -10, -10) << QRectF(0, 0, 10, 10);
    BOBUIest::newRow("test 11") << QRectF(0, 0, 10, 10) << QRectF(12, 12, -10, -10) << QRectF(0, 0, 12, 12);
    BOBUIest::newRow("test 12") << QRectF(0, 0, 10, 10) << QRectF(30, 30, -10, -10) << QRectF(0, 0, 30, 30);

    BOBUIest::newRow("test 13") << QRectF() << QRectF(10, 10, 10, 10) << QRectF(10, 10, 10, 10);
    BOBUIest::newRow("test 14") << QRectF(10, 10, 10, 10) << QRectF() << QRectF(10, 10, 10, 10);
    BOBUIest::newRow("test 15") << QRectF() << QRectF() << QRectF();
    BOBUIest::newRow("test 16") << QRectF(0, 0, 100, 0) << QRectF(0, 0, 0, 100) << QRectF(0, 0, 100, 100);
}

void tst_QRect::unitedRectF()
{
    QFETCH(QRectF, rect1);
    QFETCH(QRectF, rect2);
    QFETCH(QRectF, unite);

    QCOMPARE(rect1.united(rect2), unite);
}

void tst_QRect::intersectsRect_data()
{
    BOBUIest::addColumn<QRect>("rect1");
    BOBUIest::addColumn<QRect>("rect2");
    BOBUIest::addColumn<bool>("intersects");

    BOBUIest::newRow("test 01") << QRect(0, 0, 10, 10) << QRect( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 02") << QRect(0, 0, 10, 10) << QRect( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 03") << QRect(0, 0, 10, 10) << QRect( 2,  2, 10, 10) << true;
    BOBUIest::newRow("test 04") << QRect(0, 0, 10, 10) << QRect(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 05") << QRect(9, 9, -8, -8) << QRect( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 06") << QRect(9, 9, -8, -8) << QRect( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 07") << QRect(9, 9, -8, -8) << QRect( 2,  2, 10, 10) << true;
    BOBUIest::newRow("test 08") << QRect(9, 9, -8, -8) << QRect(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 09") << QRect(0, 0, 10, 10) << QRect( 7,  7,  -4,  -4) << true;
    BOBUIest::newRow("test 10") << QRect(0, 0, 10, 10) << QRect( 9,  9, -8, -8) << true;
    BOBUIest::newRow("test 11") << QRect(0, 0, 10, 10) << QRect(11, 11, -8, -8) << true;
    BOBUIest::newRow("test 12") << QRect(0, 0, 10, 10) << QRect(29, 29, -8, -8) << false;

    BOBUIest::newRow("test 13") << QRect() << QRect(10, 10, 10, 10) << false;
    BOBUIest::newRow("test 14") << QRect(10, 10, 10, 10) << QRect() << false;
    BOBUIest::newRow("test 15") << QRect() << QRect() << false;
    BOBUIest::newRow("test 16") << QRect(10, 10, 10, 10) << QRect(19, 15, 1, 5) << true;

    BOBUIest::newRow("test 17") << QRect(10, 10, 10, 10) << QRect(15, 19, 5, 1) << true;
    BOBUIest::newRow("test 18") << QRect(2, 0, 1, 652) << QRect(2, 0, 1, 652) << true;
}

void tst_QRect::intersectsRect()
{
    QFETCH(QRect, rect1);
    QFETCH(QRect, rect2);
    QFETCH(bool, intersects);

    QVERIFY(rect1.intersects(rect2) == intersects);
}

void tst_QRect::intersectsRectF_data()
{
    BOBUIest::addColumn<QRectF>("rect1");
    BOBUIest::addColumn<QRectF>("rect2");
    BOBUIest::addColumn<bool>("intersects");

    BOBUIest::newRow("test 01") << QRectF(0, 0, 10, 10) << QRectF( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 02") << QRectF(0, 0, 10, 10) << QRectF( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 03") << QRectF(0, 0, 10, 10) << QRectF( 2,  2, 10, 10) << true;
    BOBUIest::newRow("test 04") << QRectF(0, 0, 10, 10) << QRectF(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 05") << QRectF(10, 10, -10, -10) << QRectF( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 06") << QRectF(10, 10, -10, -10) << QRectF( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 07") << QRectF(10, 10, -10, -10) << QRectF( 2,  2, 10, 10) << true;
    BOBUIest::newRow("test 08") << QRectF(10, 10, -10, -10) << QRectF(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 09") << QRectF(0, 0, 10, 10) << QRectF( 8,  8,  -6,  -6) << true;
    BOBUIest::newRow("test 10") << QRectF(0, 0, 10, 10) << QRectF(10, 10, -10, -10) << true;
    BOBUIest::newRow("test 11") << QRectF(0, 0, 10, 10) << QRectF(12, 12, -10, -10) << true;
    BOBUIest::newRow("test 12") << QRectF(0, 0, 10, 10) << QRectF(30, 30, -10, -10) << false;

    BOBUIest::newRow("test 13") << QRectF() << QRectF(10, 10, 10, 10) << false;
    BOBUIest::newRow("test 14") << QRectF(10, 10, 10, 10) << QRectF() << false;
    BOBUIest::newRow("test 15") << QRectF() << QRectF() << false;

    BOBUIest::newRow("test 16") << QRectF(0, 0, 10, 10) << QRectF(10, 10, 10, 10) << false;
    BOBUIest::newRow("test 17") << QRectF(0, 0, 10, 10) << QRectF(0, 10, 10, 10) << false;
    BOBUIest::newRow("test 18") << QRectF(0, 0, 10, 10) << QRectF(10, 0, 10, 10) << false;
}

void tst_QRect::intersectsRectF()
{
    QFETCH(QRectF, rect1);
    QFETCH(QRectF, rect2);
    QFETCH(bool, intersects);

    QVERIFY(rect1.intersects(rect2) == intersects);
}

void tst_QRect::containsRect_data()
{
    BOBUIest::addColumn<QRect>("rect1");
    BOBUIest::addColumn<QRect>("rect2");
    BOBUIest::addColumn<bool>("contains");

    BOBUIest::newRow("test 01") << QRect(0, 0, 10, 10) << QRect( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 02") << QRect(0, 0, 10, 10) << QRect( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 03") << QRect(0, 0, 10, 10) << QRect( 2,  2, 10, 10) << false;
    BOBUIest::newRow("test 04") << QRect(0, 0, 10, 10) << QRect(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 05") << QRect(9, 9, -9, -9) << QRect( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 06") << QRect(9, 9, -9, -9) << QRect( 0,  0,  9,  9) << true;
    BOBUIest::newRow("test 07") << QRect(9, 9, -9, -9) << QRect( 2,  2,  9,  9) << false;
    BOBUIest::newRow("test 08") << QRect(9, 9, -9, -9) << QRect(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 09") << QRect(0, 0, 10, 10) << QRect( 7,  7,  -4,  -4) << true;
    BOBUIest::newRow("test 10") << QRect(0, 0, 10, 10) << QRect( 9,  9, -8, -8) << true;
    BOBUIest::newRow("test 11") << QRect(0, 0, 10, 10) << QRect(11, 11, -8, -8) << false;
    BOBUIest::newRow("test 12") << QRect(0, 0, 10, 10) << QRect(29, 29, -8, -8) << false;

    BOBUIest::newRow("test 13") << QRect(-1, 1, 10, 10) << QRect() << false;
    BOBUIest::newRow("test 14") << QRect() << QRect(0, 0, 10, 10) << false;
    BOBUIest::newRow("test 15") << QRect() << QRect() << false;
}

void tst_QRect::containsRect()
{
    QFETCH(QRect, rect1);
    QFETCH(QRect, rect2);
    QFETCH(bool, contains);

    QVERIFY(rect1.contains(rect2) == contains);
}

void tst_QRect::containsRectNormalized()
{
    QRect rect(QPoint(10, 10), QPoint(0,0));
    QRect normalized = rect.normalized();
    for (int i = -2 ; i < 12; ++i) {
        for (int j = -2 ; j < 12; ++j) {
            for (int k = -2 ; k <= 2; ++k)
                QCOMPARE(rect.contains(QRect(i,j,k,k)), normalized.contains(QRect(i,j,k,k)));
        }
    }
}

void tst_QRect::containsRectF_data()
{
    BOBUIest::addColumn<QRectF>("rect1");
    BOBUIest::addColumn<QRectF>("rect2");
    BOBUIest::addColumn<bool>("contains");

    BOBUIest::newRow("test 01") << QRectF(0, 0, 10, 10) << QRectF( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 02") << QRectF(0, 0, 10, 10) << QRectF( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 03") << QRectF(0, 0, 10, 10) << QRectF( 2,  2, 10, 10) << false;
    BOBUIest::newRow("test 04") << QRectF(0, 0, 10, 10) << QRectF(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 05") << QRectF(10, 10, -10, -10) << QRectF( 2,  2,  6,  6) << true;
    BOBUIest::newRow("test 06") << QRectF(10, 10, -10, -10) << QRectF( 0,  0, 10, 10) << true;
    BOBUIest::newRow("test 07") << QRectF(10, 10, -10, -10) << QRectF( 2,  2, 10, 10) << false;
    BOBUIest::newRow("test 08") << QRectF(10, 10, -10, -10) << QRectF(20, 20, 10, 10) << false;

    BOBUIest::newRow("test 09") << QRectF(0, 0, 10, 10) << QRectF( 8,  8,  -6,  -6) << true;
    BOBUIest::newRow("test 10") << QRectF(0, 0, 10, 10) << QRectF(10, 10, -10, -10) << true;
    BOBUIest::newRow("test 11") << QRectF(0, 0, 10, 10) << QRectF(12, 12, -10, -10) << false;
    BOBUIest::newRow("test 12") << QRectF(0, 0, 10, 10) << QRectF(30, 30, -10, -10) << false;

    BOBUIest::newRow("test 13") << QRectF(-1, 1, 10, 10) << QRectF() << false;
    BOBUIest::newRow("test 14") << QRectF() << QRectF(0, 0, 10, 10) << false;
    BOBUIest::newRow("test 15") << QRectF() << QRectF() << false;
}

void tst_QRect::containsRectF()
{
    QFETCH(QRectF, rect1);
    QFETCH(QRectF, rect2);
    QFETCH(bool, contains);

    QVERIFY(rect1.contains(rect2) == contains);
}

void tst_QRect::containsPoint_data()
{
    BOBUIest::addColumn<QRect>("rect");
    BOBUIest::addColumn<QPoint>("point");
    BOBUIest::addColumn<bool>("contains");
    BOBUIest::addColumn<bool>("containsProper");

    BOBUIest::newRow("test 01") << QRect(0, 0, 10, 10) << QPoint( 0,  0) << true  << false;
    BOBUIest::newRow("test 02") << QRect(0, 0, 10, 10) << QPoint( 0, 10) << false << false;
    BOBUIest::newRow("test 03") << QRect(0, 0, 10, 10) << QPoint(10,  0) << false << false;
    BOBUIest::newRow("test 04") << QRect(0, 0, 10, 10) << QPoint(10, 10) << false << false;
    BOBUIest::newRow("test 05") << QRect(0, 0, 10, 10) << QPoint( 0,  9) << true  << false;
    BOBUIest::newRow("test 06") << QRect(0, 0, 10, 10) << QPoint( 9,  0) << true  << false;
    BOBUIest::newRow("test 07") << QRect(0, 0, 10, 10) << QPoint( 9,  9) << true  << false;
    BOBUIest::newRow("test 08") << QRect(0, 0, 10, 10) << QPoint( 1,  0) << true  << false;
    BOBUIest::newRow("test 09") << QRect(0, 0, 10, 10) << QPoint( 9,  1) << true  << false;
    BOBUIest::newRow("test 10") << QRect(0, 0, 10, 10) << QPoint( 1,  1) << true  << true;
    BOBUIest::newRow("test 11") << QRect(0, 0, 10, 10) << QPoint( 1,  8) << true  << true;
    BOBUIest::newRow("test 12") << QRect(0, 0, 10, 10) << QPoint( 8,  8) << true  << true;

    BOBUIest::newRow("test 13") << QRect(9, 9, -9, -9) << QPoint( 0,  0) << true  << false;
    BOBUIest::newRow("test 14") << QRect(9, 9, -9, -9) << QPoint( 0,  9) << false << false;
    BOBUIest::newRow("test 15") << QRect(9, 9, -9, -9) << QPoint( 9,  0) << false << false;
    BOBUIest::newRow("test 16") << QRect(9, 9, -9, -9) << QPoint( 9,  9) << false << false;
    BOBUIest::newRow("test 17") << QRect(9, 9, -9, -9) << QPoint( 0,  8) << true  << false;
    BOBUIest::newRow("test 18") << QRect(9, 9, -9, -9) << QPoint( 8,  0) << true  << false;
    BOBUIest::newRow("test 19") << QRect(9, 9, -9, -9) << QPoint( 8,  8) << true  << false;
    BOBUIest::newRow("test 20") << QRect(9, 9, -9, -9) << QPoint( 1,  0) << true  << false;
    BOBUIest::newRow("test 21") << QRect(9, 9, -9, -9) << QPoint( 8,  1) << true  << false;
    BOBUIest::newRow("test 22") << QRect(9, 9, -9, -9) << QPoint( 1,  1) << true  << true;
    BOBUIest::newRow("test 23") << QRect(9, 9, -9, -9) << QPoint( 1,  7) << true  << true;
    BOBUIest::newRow("test 24") << QRect(9, 9, -9, -9) << QPoint( 7,  7) << true  << true;

    BOBUIest::newRow("test 25") << QRect(-1, 1, 10, 10) << QPoint() << false << false;
    BOBUIest::newRow("test 26") << QRect() << QPoint(1, 1) << false << false;
    BOBUIest::newRow("test 27") << QRect() << QPoint() << false << false;
}

void tst_QRect::containsPoint()
{
    QFETCH(QRect, rect);
    QFETCH(QPoint, point);
    QFETCH(bool, contains);
    QFETCH(bool, containsProper);

    QVERIFY(rect.contains(point) == contains);
    QVERIFY(rect.contains(point, true) == containsProper);
}

void tst_QRect::containsPointNormalized()
{
    QRect rect(QPoint(10, 10), QPoint(0,0));
    QRect normalized = rect.normalized();
    for (int i = 0 ; i < 10; ++i) {
        for (int j = 0 ; j < 10; ++j)
            QCOMPARE(rect.contains(QPoint(i,j)), normalized.contains(QPoint(i,j)));
    }
}

void tst_QRect::containsPointF_data()
{
    BOBUIest::addColumn<QRectF>("rect");
    BOBUIest::addColumn<QPointF>("point");
    BOBUIest::addColumn<bool>("contains");

    BOBUIest::newRow("test 01") << QRectF(0, 0, 10, 10) << QPointF( 0,  0) << true;
    BOBUIest::newRow("test 02") << QRectF(0, 0, 10, 10) << QPointF( 0, 10) << true;
    BOBUIest::newRow("test 03") << QRectF(0, 0, 10, 10) << QPointF(10,  0) << true;
    BOBUIest::newRow("test 04") << QRectF(0, 0, 10, 10) << QPointF(10, 10) << true;
    BOBUIest::newRow("test 05") << QRectF(0, 0, 10, 10) << QPointF( 0,  9) << true;
    BOBUIest::newRow("test 06") << QRectF(0, 0, 10, 10) << QPointF( 9,  0) << true;
    BOBUIest::newRow("test 07") << QRectF(0, 0, 10, 10) << QPointF( 9,  9) << true;
    BOBUIest::newRow("test 08") << QRectF(0, 0, 10, 10) << QPointF( 1,  0) << true;
    BOBUIest::newRow("test 09") << QRectF(0, 0, 10, 10) << QPointF( 9,  1) << true;
    BOBUIest::newRow("test 10") << QRectF(0, 0, 10, 10) << QPointF( 1,  1) << true;
    BOBUIest::newRow("test 11") << QRectF(0, 0, 10, 10) << QPointF( 1,  8) << true;
    BOBUIest::newRow("test 12") << QRectF(0, 0, 10, 10) << QPointF( 8,  8) << true;

    BOBUIest::newRow("test 13") << QRectF(10, 10, -10, -10) << QPointF( 0,  0) << true;
    BOBUIest::newRow("test 14") << QRectF(10, 10, -10, -10) << QPointF( 0, 10) << true;
    BOBUIest::newRow("test 15") << QRectF(10, 10, -10, -10) << QPointF(10,  0) << true;
    BOBUIest::newRow("test 16") << QRectF(10, 10, -10, -10) << QPointF(10, 10) << true;
    BOBUIest::newRow("test 17") << QRectF(10, 10, -10, -10) << QPointF( 0,  9) << true;
    BOBUIest::newRow("test 18") << QRectF(10, 10, -10, -10) << QPointF( 9,  0) << true;
    BOBUIest::newRow("test 19") << QRectF(10, 10, -10, -10) << QPointF( 9,  9) << true;
    BOBUIest::newRow("test 20") << QRectF(10, 10, -10, -10) << QPointF( 1,  0) << true;
    BOBUIest::newRow("test 21") << QRectF(10, 10, -10, -10) << QPointF( 9,  1) << true;
    BOBUIest::newRow("test 22") << QRectF(10, 10, -10, -10) << QPointF( 1,  1) << true;
    BOBUIest::newRow("test 23") << QRectF(10, 10, -10, -10) << QPointF( 1,  8) << true;
    BOBUIest::newRow("test 24") << QRectF(10, 10, -10, -10) << QPointF( 8,  8) << true;

    BOBUIest::newRow("test 25") << QRectF(-1, 1, 10, 10) << QPointF() << false;
    BOBUIest::newRow("test 26") << QRectF() << QPointF(1, 1) << false;
    BOBUIest::newRow("test 27") << QRectF() << QPointF() << false;
}

void tst_QRect::containsPointF()
{
    QFETCH(QRectF, rect);
    QFETCH(QPointF, point);
    QFETCH(bool, contains);

    QVERIFY(rect.contains(point) == contains);
}

void tst_QRect::smallRects() const
{
    const QRectF r1( 0, 0, 1e-12, 1e-12 );
    const QRectF r2( 0, 0, 1e-14, 1e-14 );

    // r2 is 10000 times bigger than r1
    QVERIFY(!(r1 == r2));
    QVERIFY(r1 != r2);
}

void tst_QRect::toRect()
{
    for (qreal x = 1.0; x < 2.0; x += 0.25) {
        for (qreal y = 1.0; y < 2.0; y += 0.25) {
            for (qreal w = 1.0; w < 2.0; w += 0.25) {
                for (qreal h = 1.0; h < 2.0; h += 0.25) {
                    const QRectF rectf(x, y, w, h);
                    const QRectF rect = rectf.toRect();
                    QVERIFY(qAbs(rect.x() - rectf.x()) <= 0.75);
                    QVERIFY(qAbs(rect.y() - rectf.y()) <= 0.75);
                    QVERIFY(qAbs(rect.width() - rectf.width()) <= 0.75);
                    QVERIFY(qAbs(rect.height() - rectf.height()) <= 0.75);
                    QVERIFY(qAbs(rect.right() - rectf.right()) <= 0.75);
                    QVERIFY(qAbs(rect.bottom() - rectf.bottom()) <= 0.75);

                    const QRectF arect = rectf.toAlignedRect();
                    QVERIFY(qAbs(arect.x() - rectf.x()) < 1.0);
                    QVERIFY(qAbs(arect.y() - rectf.y()) < 1.0);
                    QVERIFY(qAbs(arect.width() - rectf.width()) < 2.0);
                    QVERIFY(qAbs(arect.height() - rectf.height()) < 2.0);
                    QVERIFY(qAbs(arect.right() - rectf.right()) < 1.0);
                    QVERIFY(qAbs(arect.bottom() - rectf.bottom()) < 1.0);

                    QVERIFY(arect.contains(rectf));
                    QVERIFY(arect.contains(rect));
                }
            }
        }
    }
}

void tst_QRect::span()
{
    QCOMPARE(QRect::span(QPoint( 0,  1), QPoint(9, 10)), QRect(QPoint(0, 1), QPoint( 9, 10)));

    QCOMPARE(QRect::span(QPoint(10,  9), QPoint(1,  0)), QRect(QPoint(1, 0), QPoint(10,  9)));

    QCOMPARE(QRect::span(QPoint(10,  1), QPoint(0,  9)), QRect(QPoint(0, 1), QPoint(10,  9)));

    QCOMPARE(QRect::span(QPoint( 1, 10), QPoint(9,  0)), QRect(QPoint(1, 0), QPoint( 9, 10)));
}

void tst_QRect::debug()
{
    QString str;
    QDebug debug(&str);

    debug.nospace();

    str.clear();
    debug << QRect();
    QCOMPARE(str, "QRect(0,0 0x0)");

    str.clear();
    debug << QRect(10, 20, 30, 40);
    QCOMPARE(str, "QRect(10,20 30x40)");

    str.clear();
    debug << QRect(-10, -20, 30, 40);
    QCOMPARE(str, "QRect(-10,-20 30x40)");

    str.clear();
    debug << QRect(-10, -20, -30, -40);
    QCOMPARE(str, "QRect(-10,-20 -30x-40)");

    str.clear();
    debug << QRect(QPoint(INT_MIN, INT_MIN), QPoint(INT_MAX, INT_MAX));
    QCOMPARE(str, "QRect(-2147483648,-2147483648 4294967296x4294967296 (oversized))");
}

namespace ConstexprTests {
constexpr QRect r = QRect(1, 2, 3, 4).translated(10, 20);
static_assert(r.width() == 3);
static_assert(r.height() == 4);
static_assert(r.left() == 11);
static_assert(r.top() == 22);
static_assert(r.right() == 13);
static_assert(r.bottom() == 25);

constexpr QRectF rf = QRectF(1.0, 2.0, 3.0, 4.0).translated(10.5, 20.5);
static_assert(rf.width() == 3.0);
static_assert(rf.height() == 4.0);
static_assert(rf.left() == 11.5);
static_assert(rf.top() == 22.5);
static_assert(rf.right() == 14.5);
static_assert(rf.bottom() == 26.5);
} // namespace ConstexprTests

BOBUIEST_MAIN(tst_QRect)
#include "tst_qrect.moc"
