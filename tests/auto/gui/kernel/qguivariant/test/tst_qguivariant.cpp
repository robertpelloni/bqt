// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <qvariant.h>

#include <qkeysequence.h>
#include <qbitmap.h>
#include <qcursor.h>
#include <qimage.h>
#include <qicon.h>
#include <qmatrix4x4.h>
#include <qpen.h>
#include <qpolygon.h>
#include <qpalette.h>
#include <bobuiransform.h>
#include <qvector2d.h>
#include <qvector3d.h>
#include <qvector4d.h>
#include <qquaternion.h>
#include <bobuiextdocument.h>
#include <bobuiextformat.h>
#include <qfont.h>

#include "tst_qvariant_common.h"

class tst_QGuiVariant : public QObject
{
    Q_OBJECT

private slots:
    void constructor_invalid_data();
    void constructor_invalid();

    void canConvert_data();
    void canConvert();

    void toInt_data();
    void toInt();

    void toFont_data();
    void toFont();

    void toKeySequence_data();
    void toKeySequence();

    void toString_data();
    void toString();

    void toColor_data();
    void toColor();

    void toPixmap_data();
    void toPixmap();

    void toImage_data();
    void toImage();

    void toBrush_data();
    void toBrush();

    void transform();

    void matrix4x4();
    void vector2D();
    void vector3D();
    void vector4D();
    void quaternion();

    void writeToReadFromDataStream_data();
    void writeToReadFromDataStream();
    void writeToReadFromOldDataStream();

    void colorInteger();
    void invalidQColor();
    void validQColor();

    void debugStream_data();
    void debugStream();

    void implicitConstruction();

    void guiVariantAtExit();

    void bobui4QPolygonFDataStream();
};

void tst_QGuiVariant::constructor_invalid_data()
{
    BOBUIest::addColumn<uint>("typeId");

    BOBUIest::newRow("LastGuiType + 1") << uint(QMetaType::LastGuiType + 1);
    QVERIFY(!QMetaType::isRegistered(QMetaType::LastGuiType + 1));
    BOBUIest::newRow("LastWidgetsType + 1") << uint(QMetaType::LastWidgetsType + 1);
    QVERIFY(!QMetaType::isRegistered(QMetaType::LastWidgetsType + 1));
}

void tst_QGuiVariant::constructor_invalid()
{

    QFETCH(uint, typeId);
    {
        BOBUIest::ignoreMessage(BobUIWarningMsg, QRegularExpression("^Trying to construct an instance of an invalid type, type id:"));
        QVariant variant{QMetaType(typeId)};
        QVERIFY(!variant.isValid());
        QCOMPARE(variant.userType(), int(QMetaType::UnknownType));
    }
    {
        BOBUIest::ignoreMessage(BobUIWarningMsg, QRegularExpression("^Trying to construct an instance of an invalid type, type id:"));
        QVariant variant(QMetaType(typeId), nullptr);
        QVERIFY(!variant.isValid());
        QCOMPARE(variant.userType(), int(QMetaType::UnknownType));
    }
}

void tst_QGuiVariant::canConvert_data()
{
    TST_QVARIANT_CANCONVERT_DATATABLE_HEADERS

#ifdef Y
#undef Y
#endif
#ifdef N
#undef N
#endif
#define Y true
#define N false

    QVariant var;

    //            bita bitm bool brsh byta col  curs date dt   dbl  font img  int  inv  kseq list ll   map  pal  pen  pix  pnt  rect reg  size sp   str  strl time uint ull


    var = QVariant::fromValue(QBitmap());
    BOBUIest::newRow("Bitmap")
        << var << N << Y << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QBrush());
    BOBUIest::newRow("Brush")
        << var << N << N << N << Y << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QColor());
    BOBUIest::newRow("Color")
        << var << N << N << N << Y << Y << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
#ifndef BOBUI_NO_CURSOR
    var = QVariant::fromValue(QCursor());
    BOBUIest::newRow("Cursor")
        << var << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
#endif
    var = QVariant::fromValue(QFont());
    BOBUIest::newRow("Font")
        << var << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
    var = QVariant::fromValue(QIcon());
    BOBUIest::newRow("Icon")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QImage());
    BOBUIest::newRow("Image")
        << var << N << Y << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QKeySequence());
    BOBUIest::newRow("KeySequence")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << Y << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
    var = QVariant::fromValue(QPalette());
    BOBUIest::newRow("Palette")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QPen());
    BOBUIest::newRow("Pen")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QPixmap());
    BOBUIest::newRow("Pixmap")
        << var << N << Y << N << Y << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QPolygon());
    BOBUIest::newRow("PointArray")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = QVariant::fromValue(QRegion());
    BOBUIest::newRow("Region")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N;

#undef N
#undef Y
}

void tst_QGuiVariant::canConvert()
{
    TST_QVARIANT_CANCONVERT_FETCH_DATA

    TST_QVARIANT_CANCONVERT_COMPARE_DATA
}

void tst_QGuiVariant::toInt_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<int>("result");
    BOBUIest::addColumn<bool>("valueOK");

    BOBUIest::newRow( "keysequence" ) << QVariant::fromValue( QKeySequence( BobUI::Key_A ) ) << 65 << true;
}

void tst_QGuiVariant::toInt()
{
    QFETCH( QVariant, value );
    QFETCH( int, result );
    QFETCH( bool, valueOK );
    QVERIFY(value.isValid() == value.canConvert(QMetaType(QMetaType::Int)));
    bool ok;
    int i = value.toInt( &ok );
    QCOMPARE( i, result );
    QVERIFY( ok == valueOK );
}

void tst_QGuiVariant::toColor_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QColor>("result");

    QColor c("red");
    BOBUIest::newRow( "string" ) << QVariant( QString( "red" ) ) << c;
    BOBUIest::newRow( "solid brush" ) << QVariant( QBrush(c) ) << c;
    BOBUIest::newRow("qbytearray") << QVariant(QByteArray("red")) << c;
    BOBUIest::newRow("same color") << QVariant(c) << c;
    BOBUIest::newRow("qstring(#ff0000)") << QVariant(QString::fromUtf8("#ff0000")) << c;
    BOBUIest::newRow("qbytearray(#ff0000)") << QVariant(QByteArray("#ff0000")) << c;

    c = QColor::fromString("#88112233");
    BOBUIest::newRow("qstring(#88112233)") << QVariant(QString::fromUtf8("#88112233")) << c;
    BOBUIest::newRow("qbytearray(#88112233)") << QVariant(QByteArray("#88112233")) << c;
}

void tst_QGuiVariant::toColor()
{
    QFETCH( QVariant, value );
    QFETCH( QColor, result );
    QVERIFY( value.isValid() );
    QVERIFY(value.canConvert(QMetaType(QMetaType::QColor)));
    QColor d = qvariant_cast<QColor>(value);
    QCOMPARE( d, result );
    QVERIFY(value.convert(QMetaType(QMetaType::QColor)));
    QCOMPARE(d, QColor(value.toString()));
}

void tst_QGuiVariant::toPixmap_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QPixmap>("result");

    QPixmap pm(30, 30);
    pm.fill(BobUI::red);
    BOBUIest::newRow( "image" ) << QVariant( pm ) << pm;

    QBitmap bm(30, 30);
    bm.fill(BobUI::color1);
    BOBUIest::newRow( "bitmap" ) << QVariant( bm ) << QPixmap(bm);
}

void tst_QGuiVariant::toPixmap()
{
    QFETCH( QVariant, value );
    QFETCH( QPixmap, result );
    QVERIFY( value.isValid() );
    QVERIFY(value.canConvert(QMetaType(QMetaType::QPixmap)));
    QPixmap d = qvariant_cast<QPixmap>(value);
    QCOMPARE( d, result );
}

void tst_QGuiVariant::toImage_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QImage>("result");

    QImage im(30, 30, QImage::Format_ARGB32);
    im.fill(0x7fff0000);
    BOBUIest::newRow( "image" ) << QVariant( im ) << im;
}

void tst_QGuiVariant::toImage()
{
    QFETCH( QVariant, value );
    QFETCH( QImage, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert(QMetaType(QMetaType::QImage)));
    QImage d = qvariant_cast<QImage>(value);
    QCOMPARE( d, result );
}

void tst_QGuiVariant::toBrush_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QBrush>("result");

    QColor c(BobUI::red);
    BOBUIest::newRow( "color" ) << QVariant( c ) << QBrush(c);
    QPixmap pm(30, 30);
    pm.fill(c);
    BOBUIest::newRow( "pixmap" ) << QVariant( pm ) << QBrush(pm);
}

void tst_QGuiVariant::toBrush()
{
    QFETCH( QVariant, value );
    QFETCH( QBrush, result );
    QVERIFY( value.isValid() );
    QVERIFY(value.canConvert(QMetaType(QMetaType::QBrush)));
    QBrush d = qvariant_cast<QBrush>(value);
    QCOMPARE( d, result );
}

void tst_QGuiVariant::toFont_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QFont>("result");

    QFont f("times",12,-1,false);
    BOBUIest::newRow( "string" ) << QVariant( QString( "times,12,-1,5,50,0,0,0,0,0" ) ) << f;
}

void tst_QGuiVariant::toFont()
{
    QFETCH( QVariant, value );
    QFETCH( QFont, result );
    QVERIFY( value.isValid() );
    QVERIFY(value.canConvert(QMetaType(QMetaType::QFont)));
    QFont d = qvariant_cast<QFont>(value);
    QCOMPARE( d, result );
}

void tst_QGuiVariant::toKeySequence_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QKeySequence>("result");


    BOBUIest::newRow( "int" ) << QVariant( 67108929 ) << QKeySequence( BobUI::CTRL | BobUI::Key_A );


    BOBUIest::newRow( "qstring" )
        << QVariant( QString( "Ctrl+A" ) )
        << QKeySequence( BobUI::CTRL | BobUI::Key_A );
}

void tst_QGuiVariant::toKeySequence()
{
    QFETCH( QVariant, value );
    QFETCH( QKeySequence, result );
    QVERIFY( value.isValid() );
    QVERIFY(value.canConvert(QMetaType(QMetaType::QKeySequence)));
    QKeySequence d = qvariant_cast<QKeySequence>(value);
    QCOMPARE( d, result );
}

void tst_QGuiVariant::toString_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("result");

    BOBUIest::newRow( "qkeysequence" ) << QVariant::fromValue( QKeySequence( BobUI::CTRL | BobUI::Key_A ) )
#ifndef Q_OS_MAC
        << QString( "Ctrl+A" );
#else
        << QString(QChar(0x2318)) + QLatin1Char('A');
#endif

    QFont font( "times", 12 );
    BOBUIest::newRow("qfont") << QVariant::fromValue(font) << QString("times,12,-1,5,400,0,0,0,0,0,0,0,0,0,0,1,,0,0");
    BOBUIest::newRow( "qcolor" ) << QVariant::fromValue( QColor( 10, 10, 10 ) ) << QString( "#0a0a0a" );
}

void tst_QGuiVariant::toString()
{
    QFETCH( QVariant, value );
    QFETCH( QString, result );
    QVERIFY( value.isValid() );
    QVERIFY(value.canConvert(QMetaType(QMetaType::QString)));
    QString str = value.toString();
    QCOMPARE( str, result );
}

void tst_QGuiVariant::matrix4x4()
{
    QVariant variant;
    QMatrix4x4 matrix = qvariant_cast<QMatrix4x4>(variant);
    QVERIFY(matrix.isIdentity());
    QMatrix4x4 m;
    m.scale(2.0f);
    variant.setValue(m);
    QCOMPARE(m, qvariant_cast<QMatrix4x4>(variant));

    void *mmatrix = QMetaType(QMetaType::QMatrix4x4).create();
    QVERIFY(mmatrix);
    QMetaType(QMetaType::QMatrix4x4).destroy(mmatrix);
}

void tst_QGuiVariant::transform()
{
    QVariant variant;
    BOBUIransform matrix = qvariant_cast<BOBUIransform>(variant);
    QVERIFY(matrix.isIdentity());
    variant.setValue(BOBUIransform().rotate(90));
    QCOMPARE(BOBUIransform().rotate(90), qvariant_cast<BOBUIransform>(variant));

    void *mmatrix = QMetaType(QMetaType::BOBUIransform).create();
    QVERIFY(mmatrix);
    QMetaType(QMetaType::BOBUIransform).destroy(mmatrix);
}


void tst_QGuiVariant::vector2D()
{
    QVariant variant;
    QVector2D vector = qvariant_cast<QVector2D>(variant);
    QVERIFY(vector.isNull());
    variant.setValue(QVector2D(0.1f, 0.2f));
    QCOMPARE(QVector2D(0.1f, 0.2f), qvariant_cast<QVector2D>(variant));

    void *pvector = QMetaType(QMetaType::QVector2D).create();
    QVERIFY(pvector);
    QMetaType(QMetaType::QVector2D).destroy(pvector);
}

void tst_QGuiVariant::vector3D()
{
    QVariant variant;
    QVector3D vector = qvariant_cast<QVector3D>(variant);
    QVERIFY(vector.isNull());
    variant.setValue(QVector3D(0.1f, 0.2f, 0.3f));
    QCOMPARE(QVector3D(0.1f, 0.2f, 0.3f), qvariant_cast<QVector3D>(variant));

    void *pvector = QMetaType(QMetaType::QVector3D).create();
    QVERIFY(pvector);
    QMetaType(QMetaType::QVector3D).destroy(pvector);
}

void tst_QGuiVariant::vector4D()
{
    QVariant variant;
    QVector4D vector = qvariant_cast<QVector4D>(variant);
    QVERIFY(vector.isNull());
    variant.setValue(QVector4D(0.1f, 0.2f, 0.3f, 0.4f));
    QCOMPARE(QVector4D(0.1f, 0.2f, 0.3f, 0.4f), qvariant_cast<QVector4D>(variant));

    void *pvector = QMetaType(QMetaType::QVector4D).create();
    QVERIFY(pvector);
    QMetaType(QMetaType::QVector4D).destroy(pvector);
}

void tst_QGuiVariant::quaternion()
{
    QVariant variant;
    QQuaternion quaternion = qvariant_cast<QQuaternion>(variant);
    QVERIFY(quaternion.isIdentity());
    variant.setValue(QQuaternion(0.1f, 0.2f, 0.3f, 0.4f));
    QCOMPARE(QQuaternion(0.1f, 0.2f, 0.3f, 0.4f), qvariant_cast<QQuaternion>(variant));

    void *pquaternion = QMetaType(QMetaType::QQuaternion).create();
    QVERIFY(pquaternion);
    QMetaType(QMetaType::QQuaternion).destroy(pquaternion);
}

void tst_QGuiVariant::writeToReadFromDataStream_data()
{
    BOBUIest::addColumn<QVariant>("writeVariant");
    BOBUIest::addColumn<bool>("isNull");

    BOBUIest::newRow( "bitmap_invalid" ) << QVariant::fromValue( QBitmap() ) << false;
    QBitmap bitmap( 10, 10 );
    bitmap.fill( BobUI::red );
    BOBUIest::newRow( "bitmap_valid" ) << QVariant::fromValue( bitmap ) << false;
    BOBUIest::newRow( "brush_valid" ) << QVariant::fromValue( QBrush( BobUI::red ) ) << false;
    BOBUIest::newRow( "color_valid" ) << QVariant::fromValue( QColor( BobUI::red ) ) << false;
#ifndef BOBUI_NO_CURSOR
    BOBUIest::newRow( "cursor_valid" ) << QVariant::fromValue( QCursor( BobUI::PointingHandCursor ) ) << false;
#endif
    BOBUIest::newRow( "font_valid" ) << QVariant::fromValue( QFont( "times", 12 ) ) << false;
    BOBUIest::newRow( "pixmap_invalid" ) << QVariant::fromValue( QPixmap() ) << false;
    QPixmap pixmap( 10, 10 );
    pixmap.fill( BobUI::red );
    BOBUIest::newRow( "pixmap_valid" ) << QVariant::fromValue( pixmap ) << false;
    BOBUIest::newRow( "image_invalid" ) << QVariant::fromValue( QImage() ) << false;
    BOBUIest::newRow( "keysequence_valid" ) << QVariant::fromValue( QKeySequence( BobUI::CTRL | BobUI::Key_A ) ) << false;
    BOBUIest::newRow( "palette_valid" ) << QVariant::fromValue(QPalette(QColor("turquoise"))) << false;
    BOBUIest::newRow( "pen_valid" ) << QVariant::fromValue( QPen( BobUI::red ) ) << false;
    BOBUIest::newRow( "pointarray_invalid" ) << QVariant::fromValue( QPolygon() ) << false;
    BOBUIest::newRow( "pointarray_valid" ) << QVariant::fromValue( QPolygon( QRect( 10, 10, 20, 20 ) ) ) << false;
    BOBUIest::newRow( "region_invalid" ) << QVariant::fromValue( QRegion() ) << false;
    BOBUIest::newRow( "region_valid" ) << QVariant::fromValue( QRegion( 10, 10, 20, 20 ) ) << false;
    BOBUIest::newRow("polygonf_invalid") << QVariant::fromValue(QPolygonF()) << false;
    BOBUIest::newRow("polygonf_valid") << QVariant::fromValue(QPolygonF(QRectF(10, 10, 20, 20))) << false;
}

void tst_QGuiVariant::invalidQColor()
{
    QVariant va("An invalid QColor::name() value.");
    QVERIFY(va.canConvert(QMetaType(QMetaType::QColor)));

    QVERIFY(!va.convert(QMetaType(QMetaType::QColor)));

    QVERIFY(!qvariant_cast<QColor>(va).isValid());
}

void tst_QGuiVariant::validQColor()
{
    QColor col(BobUI::red);
    QVariant va(col.name());
    QVERIFY(va.canConvert(QMetaType(QMetaType::QColor)));

    QVERIFY(va.convert(QMetaType(QMetaType::QColor)));

    QVERIFY(col.isValid());

    QVERIFY(va.convert(QMetaType(QMetaType::QString)));

    QCOMPARE(qvariant_cast<QString>(va), col.name());
}

void tst_QGuiVariant::colorInteger()
{
    QVariant v = QColor(BobUI::red);
    QCOMPARE(v.metaType(), QMetaType(QMetaType::QColor));
    QCOMPARE(v.value<QColor>(), QColor(BobUI::red));

    v.setValue(1000);
    QCOMPARE(v.metaType(), QMetaType(QMetaType::Int));
    QCOMPARE(v.toInt(), 1000);

    v.setValue(QColor(BobUI::yellow));
    QCOMPARE(v.metaType(), QMetaType(QMetaType::QColor));
    QCOMPARE(v.value<QColor>(), QColor(BobUI::yellow));
}

void tst_QGuiVariant::writeToReadFromDataStream()
{
    QFETCH( QVariant, writeVariant );
    QFETCH( bool, isNull );
    QByteArray data;

    QDataStream writeStream( &data, QIODevice::WriteOnly );
    writeStream << writeVariant;

    QVariant readVariant;
    QDataStream readStream( &data, QIODevice::ReadOnly );
    readStream >> readVariant;
    QVERIFY( readVariant.isNull() == isNull );
    // Best way to confirm the readVariant contains the same data?
    // Since only a few won't match since the serial numbers are different
    // I won't bother adding another bool in the data test.
    const int writeType = writeVariant.userType();
    if ( writeType != QMetaType::UnknownType && writeType != QMetaType::QBitmap && writeType != QMetaType::QPixmap
        && writeType != QMetaType::QImage) {
        switch (writeType) {
        default:
            QCOMPARE( readVariant, writeVariant );
            break;

        // compare types know by QMetaType but not QVariant (QVariant::operator==() knows nothing about them)
        case QMetaType::Long:
            QCOMPARE(qvariant_cast<long>(readVariant), qvariant_cast<long>(writeVariant));
            break;
        case QMetaType::ULong:
            QCOMPARE(qvariant_cast<ulong>(readVariant), qvariant_cast<ulong>(writeVariant));
            break;
        case QMetaType::Short:
            QCOMPARE(qvariant_cast<short>(readVariant), qvariant_cast<short>(writeVariant));
            break;
        case QMetaType::UShort:
            QCOMPARE(qvariant_cast<ushort>(readVariant), qvariant_cast<ushort>(writeVariant));
            break;
        case QMetaType::Char:
            QCOMPARE(qvariant_cast<char>(readVariant), qvariant_cast<char>(writeVariant));
            break;
        case QMetaType::UChar:
            QCOMPARE(qvariant_cast<uchar>(readVariant), qvariant_cast<uchar>(writeVariant));
            break;
        case QMetaType::Float:
            {
                // the uninitialized float can be NaN (observed on Windows Mobile 5 ARMv4i)
                float readFloat = qvariant_cast<float>(readVariant);
                float writtenFloat = qvariant_cast<float>(writeVariant);
                QCOMPARE(qIsNaN(readFloat), qIsNaN(writtenFloat));
                if (!qIsNaN(readFloat))
                    QCOMPARE(readFloat, writtenFloat);
            }
            break;
        }
    }
}

void tst_QGuiVariant::writeToReadFromOldDataStream()
{
    QPolygonF polyF(QRectF(10, 10, 50, 50));
    QVariant testVariant(polyF);
    {
        // Read into a variant and compare
        QFile file(":/data/qpolygonf.bin");
        QVERIFY(file.open(QIODevice::ReadOnly));
        QDataStream dataFileStream(&file);
        dataFileStream.setVersion(QDataStream::BobUI_4_9);
        QVariant readVariant;
        dataFileStream >> readVariant;
        QCOMPARE(readVariant.userType(), int(QMetaType::QPolygonF));
        QCOMPARE(testVariant, readVariant);
        file.close();
    }
    {
        QByteArray variantData;
        {
            QDataStream varDataStream(&variantData, QIODevice::WriteOnly);
            varDataStream << testVariant;
        }
        // Read into a bytearray and compare
        QFile file(":/data/qpolygonf.bin");
        QVERIFY(file.open(QIODevice::ReadOnly));
        QDataStream dataFileStream(&file);
        dataFileStream.setVersion(QDataStream::BobUI_4_9);
        int dummy;
        dataFileStream >> dummy;
        QByteArray polyData49;
        dataFileStream >> polyData49;
        file.close();
        QByteArray polyData50;
        QDataStream readVarData(variantData);
        readVarData >> dummy;
        readVarData >> polyData50;
        QCOMPARE(polyData49, polyData50);
    }
}

void tst_QGuiVariant::debugStream_data()
{
    BOBUIest::addColumn<QVariant>("variant");
    BOBUIest::addColumn<int>("typeId");
    for (int id = QMetaType::FirstGuiType; id <= QMetaType::LastGuiType; ++id) {
        const char *tagName = QMetaType(id).name();
        if (!tagName)
            continue;
        BOBUIest::newRow(tagName) << QVariant(QMetaType(id)) << id;
    }
}

void tst_QGuiVariant::debugStream()
{
    QFETCH(QVariant, variant);
    QFETCH(int, typeId);

    MessageHandler msgHandler(typeId);
    qDebug() << variant;
    QVERIFY(msgHandler.testPassed());
}

void tst_QGuiVariant::implicitConstruction()
{
    // This is a compile-time test
    QVariant v;

#define FOR_EACH_GUI_CLASS_BASE(F) \
    F(Font) \
    F(Pixmap) \
    F(Brush) \
    F(Color) \
    F(Palette) \
    F(Icon) \
    F(Image) \
    F(Polygon) \
    F(Region) \
    F(Bitmap) \
    F(KeySequence) \
    F(Pen) \
    F(TextLength) \
    F(TextFormat) \
    F(Transform) \
    F(Matrix4x4) \
    F(Vector2D) \
    F(Vector3D) \
    F(Vector4D) \
    F(Quaternion) \
    F(PolygonF)

#ifndef BOBUI_NO_CURSOR
#  define FOR_EACH_GUI_CLASS(F) \
    FOR_EACH_GUI_CLASS_BASE(F) \
    F(Cursor)
#else // !BOBUI_NO_CURSOR
#  define FOR_EACH_GUI_CLASS(F) \
    FOR_EACH_GUI_CLASS_BASE(F)
#endif // BOBUI_NO_CURSOR

#define CONSTRUCT(TYPE) \
    { \
        Q##TYPE t; \
        v = t; \
        QVERIFY(true); \
    }

    FOR_EACH_GUI_CLASS(CONSTRUCT)

#undef CONSTRUCT
#undef FOR_EACH_GUI_CLASS
}

void tst_QGuiVariant::guiVariantAtExit()
{
    // crash test, it should not crash at QGuiApplication exit
#ifndef BOBUI_NO_CURSOR
    static QVariant cursor = QCursor();
#endif
    static QVariant point = QPoint();
    static QVariant icon = QIcon();
    static QVariant image = QImage();
    static QVariant palette = QPalette();
#ifndef BOBUI_NO_CURSOR
    Q_UNUSED(cursor);
#endif
    Q_UNUSED(point);
    Q_UNUSED(icon);
    Q_UNUSED(image);
    Q_UNUSED(palette);
    QVERIFY(true);
}

void tst_QGuiVariant::bobui4QPolygonFDataStream()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::BobUI_4_8);
    QPolygonF polygon;
    polygon.append(QPointF(2, 3));
    stream << QVariant::fromValue(polygon);
    const QByteArray bobui4Data = QByteArray::fromHex("0000007f000000000a51506f6c79676f6e46000000000140000000000000004008000000000000");
    QCOMPARE(data, bobui4Data);

    QDataStream input(&data, QIODevice::ReadOnly);
    input.setVersion(QDataStream::BobUI_4_8);
    QVariant result;
    input >> result;
    QCOMPARE(result.value<QPolygonF>(), polygon);
}

BOBUIEST_MAIN(tst_QGuiVariant)
#include "tst_qguivariant.moc"
