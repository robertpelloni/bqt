// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qpainterpath.h>
#include <qpolygon.h>
#include <bobuiransform.h>
#include <qdebug.h>

class tst_BOBUIransform : public QObject
{
    Q_OBJECT

private slots:
    void mapRect_data();
    void mapToPolygon_data();
    void mapRect();
    void assignments();
    void mapToPolygon();
    void qhash();
    void translate();
    void scale();
    void types();
    void types2_data();
    void types2();
    void scalarOps();
    void transform();
    void mapEmptyPath();
    void boundingRect();
    void controlPointRect();
    void inverted_data();
    void inverted();
    void projectivePathMapping();
    void mapInt();
    void mapPathWithPoint();
    void mapRectToPolygon(); // BOBUIBUG-127723
    void quadToQuad(); // BOBUIBUG-21329

private:
    void mapping_data();
};

Q_DECLARE_METATYPE(BOBUIransform)
Q_DECLARE_METATYPE(BOBUIransform::TransformationType)

void tst_BOBUIransform::mapRect_data()
{
    mapping_data();

    // rotations that are not multiples of 90 degrees. mapRect returns the bounding rect here.
    qreal deg = -45;
    BOBUIest::newRow( "rot 45 a" )
        << BOBUIransform().rotate(deg)
        << QRect( 0, 0, 10, 10 )
        << QPolygon( QRect( 0, -7, 14, 14 ) );
    BOBUIest::newRow( "rot 45 b" )
        << BOBUIransform().rotate(deg)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 21, -14, 50, 49 ) );
    BOBUIest::newRow( "rot 45 c" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 10, 10 )
        << QPolygon( QRect( 0, -71, 141, 142 ) );
    BOBUIest::newRow( "rot 45 d" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 212, -141, 495, 495 ) );

    deg = 45;
    BOBUIest::newRow( "rot -45 a" )
        << BOBUIransform().rotate(deg)
        << QRect( 0, 0, 10, 10 )
        << QPolygon( QRect( -7, 0, 14, 14 ) );
    BOBUIest::newRow( "rot -45 b" )
        << BOBUIransform().rotate(deg)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -35, 21, 49, 50 ) );
    BOBUIest::newRow( "rot -45 c" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 10, 10 )
        << QPolygon( QRect( -71, 0, 142, 141 ) );
    BOBUIest::newRow( "rot -45 d" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -354, 212, 495, 495 ) );
}

void tst_BOBUIransform::mapToPolygon_data()
{
    mapping_data();
}

void tst_BOBUIransform::mapping_data()
{
    //create the testtable instance and define the elements
    BOBUIest::addColumn<BOBUIransform>("matrix");
    BOBUIest::addColumn<QRect>("src");
    BOBUIest::addColumn<QPolygon>("res");

    //next we fill it with data

    // identity
    BOBUIest::newRow( "identity" )
        << BOBUIransform()
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 10, 20, 30, 40 ) );
    // scaling
    BOBUIest::newRow( "scale 0" )
        << BOBUIransform().scale(2, 2)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 20, 40, 60, 80 ) );
    BOBUIest::newRow( "scale 1" )
        << BOBUIransform().scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 100, 200, 300, 400 ) );
    // mirroring
    BOBUIest::newRow( "mirror 0" )
        << BOBUIransform().scale(-1, 1)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -40, 20, 30, 40 ) );
    BOBUIest::newRow( "mirror 1" )
        << BOBUIransform().scale(1, -1)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 10, -60, 30, 40 ) );
    BOBUIest::newRow( "mirror 2" )
        << BOBUIransform().scale(-1, -1)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -40, -60, 30, 40 ) );
    BOBUIest::newRow( "mirror 3" )
        << BOBUIransform().scale(-2, -2)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -80, -120, 60, 80 ) );
    BOBUIest::newRow( "mirror 4" )
        << BOBUIransform().scale(-10, -10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -400, -600, 300, 400 ) );
    BOBUIest::newRow( "mirror 5" )
        << BOBUIransform().scale(-1, 1)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -30, 0, 30, 40 ) );
    BOBUIest::newRow( "mirror 6" )
        << BOBUIransform().scale(1, -1)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( 0, -40, 30, 40 ) );
    BOBUIest::newRow( "mirror 7" )
        << BOBUIransform().scale(-1, -1)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -30, -40, 30, 40 ) );
    BOBUIest::newRow( "mirror 8" )
        << BOBUIransform().scale(-2, -2)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -60, -80, 60, 80 ) );
    BOBUIest::newRow( "mirror 9" )
        << BOBUIransform().scale(-10, -10) << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -300, -400, 300, 400 ) );

    // rotations
    float deg = 0.;
    BOBUIest::newRow( "rot 0 a" )
        << BOBUIransform().rotate(deg)
        << QRect( 0, 0, 30, 40 )
        << QPolygon ( QRect( 0, 0, 30, 40 ) );
    deg = 0.00001f;
    BOBUIest::newRow( "rot 0 b" )
        << BOBUIransform().rotate(deg)
        << QRect( 0, 0, 30, 40 )
        << QPolygon ( QRect( 0, 0, 30, 40 ) );
    deg = 0.;
    BOBUIest::newRow( "rot 0 c" )
        << BOBUIransform().rotate(deg)
        << QRect( 10, 20, 30, 40 )
        << QPolygon ( QRect( 10, 20, 30, 40 ) );
    deg = 0.00001f;
    BOBUIest::newRow( "rot 0 d" )
        << BOBUIransform().rotate(deg)
        << QRect( 10, 20, 30, 40 )
        << QPolygon ( QRect( 10, 20, 30, 40 ) );

    // rotations
    deg = -90.f;
    BOBUIest::newRow( "rotscale 90 a" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( 0, -300, 400, 300 ) );
    deg = -90.00001f;
    BOBUIest::newRow( "rotscale 90 b" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( 0, -300, 400, 300 ) );
    deg = -90.f;
    BOBUIest::newRow( "rotscale 90 c" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 200, -400, 400, 300 ) );
    deg = -90.00001f;
    BOBUIest::newRow( "rotscale 90 d" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( 200, -400, 400, 300 ) );

    deg = 180.f;
    BOBUIest::newRow( "rotscale 180 a" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -300, -400, 300, 400 ) );
    deg = 180.000001f;
    BOBUIest::newRow( "rotscale 180 b" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -300, -400, 300, 400 ) );
    deg = 180.f;
    BOBUIest::newRow( "rotscale 180 c" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -400, -600, 300, 400 ) );
    deg = 180.000001f;
    BOBUIest::newRow( "rotscale 180 d" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -400, -600, 300, 400 ) );

    deg = -270.f;
    BOBUIest::newRow( "rotscale 270 a" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -400, 0, 400, 300 ) );
    deg = -270.0000001f;
    BOBUIest::newRow( "rotscale 270 b" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 0, 0, 30, 40 )
        << QPolygon( QRect( -400, 0, 400, 300 ) );
    deg = -270.f;
    BOBUIest::newRow( "rotscale 270 c" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -600, 100, 400, 300 ) );
    deg = -270.000001f;
    BOBUIest::newRow( "rotscale 270 d" )
        << BOBUIransform().rotate(deg).scale(10, 10)
        << QRect( 10, 20, 30, 40 )
        << QPolygon( QRect( -600, 100, 400, 300 ) );
}

void tst_BOBUIransform::mapRect()
{
    QFETCH( BOBUIransform, matrix );
    QFETCH( QRect, src );
    QFETCH( QPolygon, res );
    QRect mapped = matrix.mapRect(src);
    QCOMPARE( mapped, res.boundingRect().adjusted(0, 0, -1, -1) );

    QRectF r = matrix.mapRect(QRectF(src));
    QRect ir(r.topLeft().toPoint(), r.bottomRight().toPoint() - QPoint(1, 1));
    QCOMPARE( mapped, ir );
}

void tst_BOBUIransform::assignments()
{
    BOBUIransform m;
    m.scale(2, 3);
    m.rotate(45);
    m.shear(4, 5);

    BOBUIransform c1(m);

    QCOMPARE(m.m11(), c1.m11());
    QCOMPARE(m.m12(), c1.m12());
    QCOMPARE(m.m21(), c1.m21());
    QCOMPARE(m.m22(), c1.m22());
    QCOMPARE(m.dx(), c1.dx());
    QCOMPARE(m.dy(), c1.dy());

    BOBUIransform c2 = m;
    QCOMPARE(m.m11(), c2.m11());
    QCOMPARE(m.m12(), c2.m12());
    QCOMPARE(m.m21(), c2.m21());
    QCOMPARE(m.m22(), c2.m22());
    QCOMPARE(m.dx(),  c2.dx());
    QCOMPARE(m.dy(),  c2.dy());
}


void tst_BOBUIransform::mapToPolygon()
{
    QFETCH( BOBUIransform, matrix );
    QFETCH( QRect, src );
    QFETCH( QPolygon, res );

    QPolygon poly = matrix.mapToPolygon(src);

    // don't care about starting point
    bool equal = false;
    for (int i = 0; i < poly.size(); ++i) {
        QPolygon rot;
        for (int j = i; j < poly.size(); ++j)
            rot << poly[j];
        for (int j = 0; j < i; ++j)
            rot << poly[j];
        if (rot == res)
            equal = true;
    }

    QVERIFY(equal);
}

void tst_BOBUIransform::qhash()
{
    BOBUIransform t1;
    t1.shear(3.0, 2.0);
    t1.rotate(44);

    BOBUIransform t2 = t1;

    // not really much to test here, so just the bare minimum:
    QCOMPARE(qHash(t1), qHash(t2));
}


void tst_BOBUIransform::translate()
{
    BOBUIransform m( 1, 2, 3, 4, 5, 6 );
    BOBUIransform res2( m );
    BOBUIransform res( 1, 2, 3, 4, 75, 106 );
    m.translate( 10,  20 );
    QVERIFY( m == res );
    m.translate( -10,  -20 );
    QVERIFY( m == res2 );
    QVERIFY( BOBUIransform::fromTranslate( 0, 0 ).type() == BOBUIransform::TxNone );
    QVERIFY( BOBUIransform::fromTranslate( 10, 0 ).type() == BOBUIransform::TxTranslate );
    QVERIFY( BOBUIransform::fromTranslate( -1, 5 ) == BOBUIransform().translate( -1, 5 ));
    QVERIFY( BOBUIransform::fromTranslate( 0, 0 ) == BOBUIransform());
}

void tst_BOBUIransform::scale()
{
    BOBUIransform m( 1, 2, 3, 4, 5, 6 );
    BOBUIransform res2( m );
    BOBUIransform res( 10, 20, 60, 80, 5, 6 );
    m.scale( 10,  20 );
    QVERIFY( m == res );
    m.scale( 1./10.,  1./20. );
    QVERIFY( m == res2 );
    QVERIFY( BOBUIransform::fromScale( 1, 1 ).type() == BOBUIransform::TxNone );
    QVERIFY( BOBUIransform::fromScale( 2, 4 ).type() == BOBUIransform::TxScale );
    QVERIFY( BOBUIransform::fromScale( 2, 4 ) == BOBUIransform().scale( 2, 4 ));
    QVERIFY( BOBUIransform::fromScale( 1, 1 ) == BOBUIransform());
}

void tst_BOBUIransform::types()
{
    BOBUIransform m1;
    QCOMPARE(m1.type(), BOBUIransform::TxNone);

    m1.translate(1.0f, 0.0f);
    QCOMPARE(m1.type(), BOBUIransform::TxTranslate);
    QCOMPARE(m1.inverted().type(), BOBUIransform::TxTranslate);

    m1.scale(1.0f, 2.0f);
    QCOMPARE(m1.type(), BOBUIransform::TxScale);
    QCOMPARE(m1.inverted().type(), BOBUIransform::TxScale);

    m1.rotate(45.0f);
    // Rotation after non-uniform scaling -> shearing. Uniform scale + rotate tested below.
    QCOMPARE(m1.type(), BOBUIransform::TxShear);
    QCOMPARE(m1.inverted().type(), BOBUIransform::TxShear);

    m1.shear(0.5f, 0.25f);
    QCOMPARE(m1.type(), BOBUIransform::TxShear);
    QCOMPARE(m1.inverted().type(), BOBUIransform::TxShear);

    m1.rotate(45.0f, BobUI::XAxis);
    QCOMPARE(m1.type(), BOBUIransform::TxProject);
    m1.shear(0.5f, 0.25f);
    QCOMPARE(m1.type(), BOBUIransform::TxProject);
    m1.rotate(45.0f);
    QCOMPARE(m1.type(), BOBUIransform::TxProject);
    m1.scale(1.0f, 2.0f);
    QCOMPARE(m1.type(), BOBUIransform::TxProject);
    m1.translate(1.0f, 0.0f);
    QCOMPARE(m1.type(), BOBUIransform::TxProject);

    BOBUIransform m2(1.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f,
                  -1.0f, -1.0f, 1.0f);

    QCOMPARE(m2.type(), BOBUIransform::TxTranslate);
    QCOMPARE((m1 * m2).type(), BOBUIransform::TxProject);

    m1 *= BOBUIransform();
    QCOMPARE(m1.type(), BOBUIransform::TxProject);

    m1 *= BOBUIransform(1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     1.0f, 0.0f, 1.0f);
    QCOMPARE(m1.type(), BOBUIransform::TxProject);

    m2.reset();
    QCOMPARE(m2.type(), BOBUIransform::TxNone);

    m2.setMatrix(1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxNone);

    m2 *= BOBUIransform();
    QCOMPARE(m2.type(), BOBUIransform::TxNone);

    m2.setMatrix(2.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxScale);
    m2 *= BOBUIransform();
    QCOMPARE(m2.type(), BOBUIransform::TxScale);

    m2.setMatrix(0.0f, 1.0f, 0.0f,
                 1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxRotate);
    m2 *= BOBUIransform();
    QCOMPARE(m2.type(), BOBUIransform::TxRotate);

    m2.setMatrix(1.0f, 0.0f, 0.5f,
                 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxProject);
    m2 *= BOBUIransform();
    QCOMPARE(m2.type(), BOBUIransform::TxProject);

    m2.setMatrix(1.0f, 1.0f, 0.0f,
                 1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxShear);

    m2 *= m2.inverted();
    QCOMPARE(m2.type(), BOBUIransform::TxNone);

    m2.translate(5.0f, 5.0f);
    m2.rotate(45.0f);
    m2.rotate(-45.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxTranslate);

    m2.scale(2.0f, 3.0f);
    m2.shear(1.0f, 0.0f);
    m2.shear(-1.0f, 0.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxScale);

    m2 *= BOBUIransform(1.0f, 1.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxShear);

    m2 *= BOBUIransform(1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     1.0f, 0.0f, 1.0f);
    QCOMPARE(m2.type(), BOBUIransform::TxShear);

    BOBUIransform m3(1.8f, 0.0f, 0.0f,
                  0.0f, 1.8f, 0.0f,
                  0.0f, 0.0f, 1.0f);

    QCOMPARE(m3.type(), BOBUIransform::TxScale);
    m3.translate(5.0f, 5.0f);
    QCOMPARE(m3.type(), BOBUIransform::TxScale);
    QCOMPARE(m3.inverted().type(), BOBUIransform::TxScale);

    m3.setMatrix(1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 2.0f);
    QCOMPARE(m3.type(), BOBUIransform::TxProject);

    m3.setMatrix(0.0f, 2.0f, 0.0f,
                 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 2.0f);
    QCOMPARE(m3.type(), BOBUIransform::TxProject);

    BOBUIransform m4;
    m4.scale(5, 5);
    m4.translate(4, 2);
    m4.rotate(45);

    QCOMPARE(m4.type(), BOBUIransform::TxRotate);

    BOBUIransform m5;
    m5.scale(5, 5);
    m5 = m5.adjoint() / m5.determinant();
    QCOMPARE(m5.type(), BOBUIransform::TxScale);
}

void tst_BOBUIransform::types2_data()
{
    BOBUIest::addColumn<BOBUIransform>("t1");
    BOBUIest::addColumn<BOBUIransform::TransformationType>("type");

    BOBUIest::newRow( "identity" ) << BOBUIransform() << BOBUIransform::TxNone;
    BOBUIest::newRow( "translate" ) << BOBUIransform().translate(10, -0.1) << BOBUIransform::TxTranslate;
    BOBUIest::newRow( "scale" ) << BOBUIransform().scale(10, -0.1) << BOBUIransform::TxScale;
    BOBUIest::newRow( "rotate" ) << BOBUIransform().rotate(10) << BOBUIransform::TxRotate;
    BOBUIest::newRow( "shear" ) << BOBUIransform().shear(10, -0.1) << BOBUIransform::TxShear;
    BOBUIest::newRow( "project" ) << BOBUIransform().rotate(10, BobUI::XAxis) << BOBUIransform::TxProject;
    BOBUIest::newRow( "combined" ) << BOBUIransform().translate(10, -0.1).scale(10, -0.1).rotate(10, BobUI::YAxis) << BOBUIransform::TxProject;
}

void tst_BOBUIransform::types2()
{
#define CHECKTXTYPE(func) { BOBUIransform t2(func); \
                            BOBUIransform t3(t2.m11(), t2.m12(), t2.m13(), t2.m21(), t2.m22(), t2.m23(), t2.m31(), t2.m32(), t2.m33()); \
                            QVERIFY2(t3.type() == t2.type(), #func); \
                          }

    QFETCH( BOBUIransform, t1 );
    QFETCH( BOBUIransform::TransformationType, type );

    Q_ASSERT(t1.type() == type);

    CHECKTXTYPE(t1.adjoint());
    CHECKTXTYPE(t1.inverted());
    CHECKTXTYPE(t1.transposed());

#undef CHECKTXTYPE
}

void tst_BOBUIransform::scalarOps()
{
    BOBUIransform t;
    QCOMPARE(t.m11(), 1.);
    QCOMPARE(t.m33(), 1.);
    QCOMPARE(t.m21(), 0.);

    t = BOBUIransform() + 3;
    QCOMPARE(t.m11(), 4.);
    QCOMPARE(t.m33(), 4.);
    QCOMPARE(t.m21(), 3.);

    t = t - 3;
    QCOMPARE(t.m11(), 1.);
    QCOMPARE(t.m33(), 1.);
    QCOMPARE(t.m21(), 0.);
    QCOMPARE(t.isIdentity(), true);

    t += 3;
    t = t * 2;
    QCOMPARE(t.m11(), 8.);
    QCOMPARE(t.m33(), 8.);
    QCOMPARE(t.m21(), 6.);
}

void tst_BOBUIransform::transform()
{
    BOBUIransform t;
    t.rotate(30, BobUI::YAxis);
    t.translate(15, 10);
    t.scale(2, 2);
    t.rotate(30);
    t.shear(0.5, 0.5);

    BOBUIransform a, b, c, d, e;
    a.rotate(30, BobUI::YAxis);
    b.translate(15, 10);
    c.scale(2, 2);
    d.rotate(30);
    e.shear(0.5, 0.5);

    QVERIFY(qFuzzyCompare(t, e * d * c * b * a));
}

void tst_BOBUIransform::mapEmptyPath()
{
    QPainterPath path;
    path.moveTo(10, 10);
    path.lineTo(10, 10);
    QCOMPARE(BOBUIransform().map(path), path);
}

void tst_BOBUIransform::boundingRect()
{
    QPainterPath path;
    path.moveTo(10, 10);
    path.lineTo(10, 10);
    QCOMPARE(path.boundingRect(), QRectF(10, 10, 0, 0));
}

void tst_BOBUIransform::controlPointRect()
{
    QPainterPath path;
    path.moveTo(10, 10);
    path.lineTo(10, 10);
    QCOMPARE(path.controlPointRect(), QRectF(10, 10, 0, 0));
}

void tst_BOBUIransform::inverted_data()
{
    BOBUIest::addColumn<BOBUIransform>("matrix");

    BOBUIest::newRow("identity")
        << BOBUIransform();

    BOBUIest::newRow("TxTranslate")
        << BOBUIransform().translate(200, 10);

    BOBUIest::newRow("TxScale")
        << BOBUIransform().scale(5, 2);

    BOBUIest::newRow("TxTranslate TxScale")
        << BOBUIransform().translate(100, -10).scale(40, 2);

    BOBUIest::newRow("TxScale TxTranslate")
        << BOBUIransform().scale(40, 2).translate(100, -10);

    BOBUIest::newRow("TxRotate")
        << BOBUIransform().rotate(40, BobUI::ZAxis);

    BOBUIest::newRow("TxRotate TxScale")
        << BOBUIransform().rotate(60, BobUI::ZAxis).scale(2, 0.25);

    BOBUIest::newRow("TxScale TxRotate")
        << BOBUIransform().scale(2, 0.25).rotate(30, BobUI::ZAxis);

    BOBUIest::newRow("TxRotate TxScale TxTranslate")
        << BOBUIransform().rotate(60, BobUI::ZAxis).scale(2, 0.25).translate(200, -3000);

    BOBUIest::newRow("TxRotate TxTranslate TxScale")
        << BOBUIransform().rotate(60, BobUI::ZAxis).translate(200, -3000).scale(19, 77);

    BOBUIest::newRow("TxShear")
        << BOBUIransform().shear(10, 10);

    BOBUIest::newRow("TxShear TxRotate")
        << BOBUIransform().shear(10, 10).rotate(45, BobUI::ZAxis);

    BOBUIest::newRow("TxShear TxRotate TxScale")
        << BOBUIransform().shear(10, 10).rotate(45, BobUI::ZAxis).scale(19, 81);

    BOBUIest::newRow("TxTranslate TxShear TxRotate TxScale")
        << BOBUIransform().translate(150, -1).shear(10, 10).rotate(45, BobUI::ZAxis).scale(19, 81);

    const qreal s = 500000;

    BOBUIransform big;
    big.scale(s, s);

    BOBUIest::newRow("big") << big;

    BOBUIransform smallTransform;
    smallTransform.scale(1/s, 1/s);

    BOBUIest::newRow("small") << smallTransform;
}

void tst_BOBUIransform::inverted()
{
    if (sizeof(qreal) != sizeof(double))
        QSKIP("precision error if qreal is not double");

    QFETCH(BOBUIransform, matrix);

    const BOBUIransform inverted = matrix.inverted();

    QCOMPARE(matrix.isIdentity(), inverted.isIdentity());
    QCOMPARE(matrix.type(), inverted.type());

    QVERIFY((matrix * inverted).isIdentity());
    QVERIFY((inverted * matrix).isIdentity());
}

void tst_BOBUIransform::projectivePathMapping()
{
    QPainterPath path;
    path.addRect(-50, -50, 100, 100);

    const QRectF view(0, 0, 1024, 1024);

    QVERIFY(view.intersects(path.boundingRect()));

    for (int i = 0; i < 85; i += 5) {
        BOBUIransform transform;
        transform.translate(512, 512);
        transform.rotate(i, BobUI::YAxis);

        const QPainterPath mapped = transform.map(path);

        QVERIFY(view.intersects(mapped.boundingRect()));
        QVERIFY(transform.inverted().mapRect(view).intersects(path.boundingRect()));
    }
}

void tst_BOBUIransform::mapInt()
{
    int x = 0;
    int y = 0;

    BOBUIransform::fromTranslate(10, 10).map(x, y, &x, &y);

    QCOMPARE(x, 10);
    QCOMPARE(y, 10);
}

void tst_BOBUIransform::mapPathWithPoint()
{
    QPainterPath p(QPointF(10, 10));
    p = BOBUIransform::fromTranslate(10, 10).map(p);
    QCOMPARE(p.currentPosition(), QPointF(20, 20));
}

void tst_BOBUIransform::mapRectToPolygon()
{
    QRectF r(7, 7, 36, 36);
    BOBUIransform tx(2, 0, 0, 2, 0, 0);
    QPolygonF polygon1 = tx.mapToPolygon(r.toRect()).toPolygonF();
    QPolygonF polygon2 = tx.map(QPolygonF(r));
    if (polygon1.size() > 4)
        polygon1.removeLast();
    if (polygon2.size() > 4)
        polygon2.removeLast();
    QCOMPARE(polygon1, polygon2);
}

void tst_BOBUIransform::quadToQuad() // BOBUIBUG-21329
{
    BOBUIransform result;
    QVERIFY(BOBUIransform::quadToQuad(QRectF(0, 0, 1, 1), QRectF(0, 0, 1, 1), result));
    QPolygonF trapezoid({{0, 0}, {10, 0}, {11, 11}, {0, 10}});
    QVERIFY(BOBUIransform::quadToQuad(trapezoid, trapezoid.boundingRect(), result));
}

BOBUIEST_APPLESS_MAIN(tst_BOBUIransform)


#include "tst_bobuiransform.moc"
