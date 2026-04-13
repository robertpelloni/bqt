// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <BOBUIransform>
#include <QPainterPath>

class tst_BOBUIransform : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIransform();
    virtual ~tst_BOBUIransform();

public slots:
    void init();
    void cleanup();

private slots:
    void construct();
    void translate_data();
    void translate();
    void scale_data();
    void scale();
    void shear_data();
    void shear();
    void rotate_data();
    void rotate();
    void rotateXYZ_data();
    void rotateXYZ();
    void operatorAssign_data();
    void operatorAssign();
    void operatorEqual_data();
    void operatorEqual();
    void operatorNotEqual_data();
    void operatorNotEqual();
    void operatorMultiply_data();
    void operatorMultiply();
    void operatorPlusEqualScalar_data();
    void operatorPlusEqualScalar();
    void operatorMinusEqualScalar_data();
    void operatorMinusEqualScalar();
    void operatorMultiplyEqual_data();
    void operatorMultiplyEqual();
    void operatorMultiplyEqualScalar_data();
    void operatorMultiplyEqualScalar();
    void operatorDivideEqualScalar_data();
    void operatorDivideEqualScalar();
    void mapQPoint_data();
    void mapQPoint();
    void mapQPointF_data();
    void mapQPointF();
    void mapRect_data();
    void mapRect();
    void mapRectF_data();
    void mapRectF();
    void mapQPolygon_data();
    void mapQPolygon();
    void mapQPolygonF_data();
    void mapQPolygonF();
    void mapQRegion_data();
    void mapQRegion();
    void mapToPolygon_data();
    void mapToPolygon();
    void mapQPainterPath_data();
    void mapQPainterPath();
    void isIdentity_data();
    void isIdentity();
    void isAffine_data();
    void isAffine();
    void isInvertible_data();
    void isInvertible();
    void isRotating_data();
    void isRotating();
    void isScaling_data();
    void isScaling();
    void isTranslating_data();
    void isTranslating();
    void type_data();
    void type();
    void determinant_data();
    void determinant();
    void adjoint_data();
    void adjoint();
    void transposed_data();
    void transposed();
    void inverted_data();
    void inverted();

private:
    QMap<const char *, BOBUIransform> generateTransforms() const;
};

tst_BOBUIransform::tst_BOBUIransform()
{
}

tst_BOBUIransform::~tst_BOBUIransform()
{
}

void tst_BOBUIransform::init()
{
}

void tst_BOBUIransform::cleanup()
{
}

QMap<const char *, BOBUIransform> tst_BOBUIransform::generateTransforms() const
{
    QMap<const char *, BOBUIransform> x;
    x["0: identity"] = BOBUIransform();
    x["1: translate"] = BOBUIransform().translate(10, 10);
    x["2: translate"] = BOBUIransform().translate(-10, -10);
    x["3: rotate45"] = BOBUIransform().rotate(45);
    x["4: rotate90"] = BOBUIransform().rotate(90);
    x["5: rotate180"] = BOBUIransform().rotate(180);
    x["6: shear2,2"] = BOBUIransform().shear(2, 2);
    x["7: shear-2,-2"] = BOBUIransform().shear(-2, -2);
    x["8: scaleUp2,2"] = BOBUIransform().scale(2, 2);
    x["9: scaleUp2,3"] = BOBUIransform().scale(2, 3);
    x["10: scaleDown0.5,0.5"] = BOBUIransform().scale(0.5, 0.5);
    x["11: scaleDown0.5,0.25"] = BOBUIransform().scale(0.5, 0.25);
    x["12: rotateX"] = BOBUIransform().rotate(45, BobUI::XAxis);
    x["13: rotateY"] = BOBUIransform().rotate(45, BobUI::YAxis);
    x["14: rotateXY"] = BOBUIransform().rotate(45, BobUI::XAxis).rotate(45, BobUI::YAxis);
    x["15: rotateYZ"] = BOBUIransform().rotate(45, BobUI::YAxis).rotate(45, BobUI::ZAxis);
    x["16: full"] = BOBUIransform().translate(10, 10).rotate(45).shear(2, 2).scale(2, 2).rotate(45, BobUI::YAxis).rotate(45, BobUI::XAxis).rotate(45, BobUI::ZAxis);
    return x;
}

void tst_BOBUIransform::construct()
{
    QBENCHMARK {
        BOBUIransform x;
    }
}

#define SINGLE_DATA_IMPLEMENTATION(func)        \
void tst_BOBUIransform::func##_data() \
{ \
    BOBUIest::addColumn<BOBUIransform>("transform"); \
    QMap<const char *, BOBUIransform> x = generateTransforms(); \
    for (auto it = x.begin(), end = x.end(); it != end; ++it) { \
        BOBUIest::newRow(it.key()) << it.value(); \
    } \
}

#define DOUBLE_DATA_IMPLEMENTATION(func) \
void tst_BOBUIransform::func##_data() \
{ \
    BOBUIest::addColumn<BOBUIransform>("x1"); \
    BOBUIest::addColumn<BOBUIransform>("x2"); \
    QMap<const char *, BOBUIransform> x = generateTransforms(); \
    for (auto it = x.cbegin(), end = x.cend(); it != end; ++it) { \
        const char *key1 = it.key(); \
        BOBUIransform x1 = it.value(); \
        for (auto it2 = x.cbegin(), end = x.cend(); it2 != end; ++it2) { \
            BOBUIest::newRow(QString("%1 + %2").arg(key1).arg(it2.key()).toLatin1().constData()) \
                << x1 << it2.value(); \
        } \
    } \
}

SINGLE_DATA_IMPLEMENTATION(translate)

void tst_BOBUIransform::translate()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x.translate(10, 10);
    }
}

SINGLE_DATA_IMPLEMENTATION(scale)

void tst_BOBUIransform::scale()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x.scale(2, 2);
    }
}

SINGLE_DATA_IMPLEMENTATION(shear)

void tst_BOBUIransform::shear()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x.shear(2, 2);
    }
}

SINGLE_DATA_IMPLEMENTATION(rotate)

void tst_BOBUIransform::rotate()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x.rotate(45);
    }
}

SINGLE_DATA_IMPLEMENTATION(rotateXYZ)

void tst_BOBUIransform::rotateXYZ()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x.rotate(45, BobUI::XAxis);
        x.rotate(45, BobUI::YAxis);
        x.rotate(45, BobUI::ZAxis);
    }
}

DOUBLE_DATA_IMPLEMENTATION(operatorAssign)

void tst_BOBUIransform::operatorAssign()
{
    QFETCH(BOBUIransform, x1);
    QFETCH(BOBUIransform, x2);
    BOBUIransform x = x1;
    QBENCHMARK {
        x = x2;
    }
}

DOUBLE_DATA_IMPLEMENTATION(operatorEqual)

void tst_BOBUIransform::operatorEqual()
{
    QFETCH(BOBUIransform, x1);
    QFETCH(BOBUIransform, x2);
    BOBUIransform x = x1;
    QBENCHMARK {
        [[maybe_unused]] auto r = x == x2;
    }
}

DOUBLE_DATA_IMPLEMENTATION(operatorNotEqual)

void tst_BOBUIransform::operatorNotEqual()
{
    QFETCH(BOBUIransform, x1);
    QFETCH(BOBUIransform, x2);
    BOBUIransform x = x1;
    QBENCHMARK {
        [[maybe_unused]] auto r = x != x2;
    }
}

DOUBLE_DATA_IMPLEMENTATION(operatorMultiply)

void tst_BOBUIransform::operatorMultiply()
{
    QFETCH(BOBUIransform, x1);
    QFETCH(BOBUIransform, x2);
    BOBUIransform x = x1;
    QBENCHMARK {
        [[maybe_unused]] auto r = x * x2;
    }
}

SINGLE_DATA_IMPLEMENTATION(operatorPlusEqualScalar)

void tst_BOBUIransform::operatorPlusEqualScalar()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x += 3.14;
    }
}

SINGLE_DATA_IMPLEMENTATION(operatorMinusEqualScalar)

void tst_BOBUIransform::operatorMinusEqualScalar()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x -= 3.14;
    }
}

DOUBLE_DATA_IMPLEMENTATION(operatorMultiplyEqual)

void tst_BOBUIransform::operatorMultiplyEqual()
{
    QFETCH(BOBUIransform, x1);
    QFETCH(BOBUIransform, x2);
    BOBUIransform x = x1;
    QBENCHMARK {
        x *= x2;
    }
}

SINGLE_DATA_IMPLEMENTATION(operatorMultiplyEqualScalar)

void tst_BOBUIransform::operatorMultiplyEqualScalar()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x *= 3;
    }
}

SINGLE_DATA_IMPLEMENTATION(operatorDivideEqualScalar)

void tst_BOBUIransform::operatorDivideEqualScalar()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        x /= 3;
    }
}

SINGLE_DATA_IMPLEMENTATION(mapQPoint)

void tst_BOBUIransform::mapQPoint()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        [[maybe_unused]] auto r = x.map(QPoint(3, 3));
    }
}

SINGLE_DATA_IMPLEMENTATION(mapQPointF)

void tst_BOBUIransform::mapQPointF()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        [[maybe_unused]] auto r = x.map(QPointF(3, 3));
    }
}

SINGLE_DATA_IMPLEMENTATION(mapRect)

void tst_BOBUIransform::mapRect()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        [[maybe_unused]] auto r = x.mapRect(QRect(0, 0, 100, 100));
    }
}

SINGLE_DATA_IMPLEMENTATION(mapRectF)

void tst_BOBUIransform::mapRectF()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        [[maybe_unused]] auto r = x.mapRect(QRectF(0, 0, 100, 100));
    }
}

SINGLE_DATA_IMPLEMENTATION(mapQPolygon)

void tst_BOBUIransform::mapQPolygon()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QPolygon poly = QPolygon(QRect(0, 0, 100, 100));
    QBENCHMARK {
        [[maybe_unused]] auto r = x.map(poly);
    }
}

SINGLE_DATA_IMPLEMENTATION(mapQPolygonF)

void tst_BOBUIransform::mapQPolygonF()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QPolygonF poly = QPolygonF(QRectF(0, 0, 100, 100));
    QBENCHMARK {
        [[maybe_unused]] auto r = x.map(poly);
    }
}

SINGLE_DATA_IMPLEMENTATION(mapQRegion)

void tst_BOBUIransform::mapQRegion()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QRegion region;
    for (int i = 0; i < 10; ++i)
        region += QRect(i * 10, i * 10, 100, 100);
    QBENCHMARK {
        [[maybe_unused]] auto r = x.map(region);
    }
}

SINGLE_DATA_IMPLEMENTATION(mapToPolygon)

void tst_BOBUIransform::mapToPolygon()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QBENCHMARK {
        [[maybe_unused]] auto r = x.mapToPolygon(QRect(0, 0, 100, 100));
    }
}


SINGLE_DATA_IMPLEMENTATION(mapQPainterPath)

void tst_BOBUIransform::mapQPainterPath()
{
    QFETCH(BOBUIransform, transform);
    BOBUIransform x = transform;
    QPainterPath path;
    for (int i = 0; i < 10; ++i)
        path.addEllipse(i * 10, i * 10, 100, 100);
    QBENCHMARK {
        [[maybe_unused]] auto r = x.map(path);
    }
}

SINGLE_DATA_IMPLEMENTATION(isIdentity)

void tst_BOBUIransform::isIdentity()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.isIdentity();
    }
}

SINGLE_DATA_IMPLEMENTATION(isAffine)

void tst_BOBUIransform::isAffine()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.isAffine();
    }
}

SINGLE_DATA_IMPLEMENTATION(isInvertible)

void tst_BOBUIransform::isInvertible()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.isInvertible();
    }
}

SINGLE_DATA_IMPLEMENTATION(isRotating)

void tst_BOBUIransform::isRotating()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.isRotating();
    }
}

SINGLE_DATA_IMPLEMENTATION(isScaling)

void tst_BOBUIransform::isScaling()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.isScaling();
    }
}

SINGLE_DATA_IMPLEMENTATION(isTranslating)

void tst_BOBUIransform::isTranslating()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.isTranslating();
    }
}

SINGLE_DATA_IMPLEMENTATION(type)

void tst_BOBUIransform::type()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.type();
    }
}

SINGLE_DATA_IMPLEMENTATION(determinant)

void tst_BOBUIransform::determinant()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        [[maybe_unused]] auto r = transform.determinant();
    }
}

SINGLE_DATA_IMPLEMENTATION(adjoint)

void tst_BOBUIransform::adjoint()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        Q_UNUSED(transform.adjoint())
    }
}

SINGLE_DATA_IMPLEMENTATION(transposed)

void tst_BOBUIransform::transposed()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        Q_UNUSED(transform.transposed())
    }
}

SINGLE_DATA_IMPLEMENTATION(inverted)

void tst_BOBUIransform::inverted()
{
    QFETCH(BOBUIransform, transform);
    QBENCHMARK {
        Q_UNUSED(transform.inverted())
    }
}

BOBUIEST_MAIN(tst_BOBUIransform)
#include "tst_bobuiransform.moc"
