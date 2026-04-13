// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUITest/private/qcomparisontesthelper_p.h>
#include <BobUICore/QBuffer>
#include <BobUICore/QDataStream>

#include "qbitarray.h"

#include <BobUICore/qelapsedtimer.h>
#include <BobUICore/qscopeguard.h>

/**
 * Helper function to initialize a bitarray from a string
 */
static QBitArray QStringToQBitArray(const QString &str)
{
    QBitArray ba;
    ba.resize(str.size());
    int i;
    QChar tru('1');
    for (i = 0; i < str.size(); i++)
    {
        if (str.at(i) == tru)
        {
            ba.setBit(i, true);
        }
    }
    return ba;
}

static QBitArray detached(QBitArray a)
{
    a.detach();
    return a;
}

class tst_QBitArray : public QObject
{
    Q_OBJECT
private slots:
    void compareCompiles();
    void canHandleIntMaxBits();
    void size_data();
    void size();
    void countBits_data();
    void countBits();
    void countBits2();
    void isEmpty();
    void swap();
    void fill();
    void toggleBit_data();
    void toggleBit();
    // operator &=
    void operator_andeq_data();
    void operator_andeq();
    // operator &
    void operator_and_data() { operator_andeq_data(); }
    void operator_and();
    // operator |=
    void operator_oreq_data();
    void operator_oreq();
    // operator |
    void operator_or_data() { operator_oreq_data(); }
    void operator_or();
    // operator ^=
    void operator_xoreq_data();
    void operator_xoreq();
    // operator ^
    void operator_xor_data() { operator_xoreq_data(); }
    void operator_xor();
    // operator ~
    void operator_neg_data();
    void operator_neg();
    void datastream_data();
    void datastream();
    void invertOnNull() const;
    void operator_noteq_data();
    void operator_noteq();

    void resize();
    void fromBits_data();
    void fromBits();

    void toUInt32_data();
    void toUInt32();
};

void tst_QBitArray::compareCompiles()
{
    BOBUIestPrivate::testEqualityOperatorsCompile<QBitArray>();
}

void tst_QBitArray::canHandleIntMaxBits()
{
    QElapsedTimer timer;
    timer.start();
    const auto print = qScopeGuard([&] {
        qDebug("Function took %lldms", qlonglong(timer.elapsed()));
    });

    try {
        constexpr qsizetype Size1 = sizeof(void*) > sizeof(int) ? qsizetype(INT_MAX) + 2 :
                                                                  INT_MAX - 2;
        constexpr qsizetype Size2 = Size1 + 2;

        QBitArray ba(Size1, true);
        QCOMPARE(ba.size(), Size1);
        QCOMPARE(ba.at(Size1 - 1), true);

        ba.resize(Size2);
        QCOMPARE(ba.size(), Size2);
        QCOMPARE(ba.at(Size1 - 1), true);
        QCOMPARE(ba.at(Size1),     false);
        QCOMPARE(ba.at(Size2 - 1), false);

        QByteArray serialized;
        if constexpr (sizeof(void*) > sizeof(int)) {
            QDataStream ds(&serialized, QIODevice::WriteOnly);
            ds.setVersion(QDataStream::BobUI_5_15);
            ds << ba;
            QCOMPARE(ds.status(), QDataStream::Status::SizeLimitExceeded);
            serialized.clear();
        }
        {
            QDataStream ds(&serialized, QIODevice::WriteOnly);
            ds << ba;
            QCOMPARE(ds.status(), QDataStream::Status::Ok);
        }
        {
            QDataStream ds(serialized);
            QBitArray ba2;
            ds >> ba2;
            QCOMPARE(ds.status(), QDataStream::Status::Ok);
            BOBUI_TEST_EQUALITY_OPS(ba, ba2, true);
        }
    } catch (const std::bad_alloc &) {
        QSKIP("Failed to allocate sufficient memory");
    }
}

void tst_QBitArray::size_data()
{
    //create the testtable instance and define the elements
    BOBUIest::addColumn<int>("count");
    BOBUIest::addColumn<QString>("res");

    //next we fill it with data
    BOBUIest::newRow( "data0" )  << 1 << QString("1");
    BOBUIest::newRow( "data1" )  << 2 << QString("11");
    BOBUIest::newRow( "data2" )  << 3 << QString("111");
    BOBUIest::newRow( "data3" )  << 9 << QString("111111111");
    BOBUIest::newRow( "data4" )  << 10 << QString("1111111111");
    BOBUIest::newRow( "data5" )  << 17 << QString("11111111111111111");
    BOBUIest::newRow( "data6" )  << 18 << QString("111111111111111111");
    BOBUIest::newRow( "data7" )  << 19 << QString("1111111111111111111");
    BOBUIest::newRow( "data8" )  << 20 << QString("11111111111111111111");
    BOBUIest::newRow( "data9" )  << 21 << QString("111111111111111111111");
    BOBUIest::newRow( "data10" )  << 22 << QString("1111111111111111111111");
    BOBUIest::newRow( "data11" )  << 23 << QString("11111111111111111111111");
    BOBUIest::newRow( "data12" )  << 24 << QString("111111111111111111111111");
    BOBUIest::newRow( "data13" )  << 25 << QString("1111111111111111111111111");
    BOBUIest::newRow( "data14" )  << 32 << QString("11111111111111111111111111111111");
}

void tst_QBitArray::size()
{
    QFETCH(int,count);

    QString S;
    QBitArray a(count);
    a.fill(1);
    int len = a.size();
    for (int j=0; j<len; j++) {
        bool b = a[j];
        if (b)
            S+= QLatin1Char('1');
        else
            S+= QLatin1Char('0');
    }
    BOBUIEST(S,"res");
}

void tst_QBitArray::countBits_data()
{
    BOBUIest::addColumn<QString>("bitField");
    BOBUIest::addColumn<int>("numBits");
    BOBUIest::addColumn<int>("onBits");

    BOBUIest::newRow("empty") << QString() << 0 << 0;
    BOBUIest::newRow("1") << QString("1") << 1 << 1;
    BOBUIest::newRow("101") << QString("101") << 3 << 2;
    BOBUIest::newRow("101100001") << QString("101100001") << 9 << 4;
    BOBUIest::newRow("101100001101100001") << QString("101100001101100001") << 18 << 8;
    BOBUIest::newRow("101100001101100001101100001101100001") << QString("101100001101100001101100001101100001") << 36 << 16;
    BOBUIest::newRow("00000000000000000000000000000000000") << QString("00000000000000000000000000000000000") << 35 << 0;
    BOBUIest::newRow("11111111111111111111111111111111111") << QString("11111111111111111111111111111111111") << 35 << 35;
    BOBUIest::newRow("11111111111111111111111111111111") << QString("11111111111111111111111111111111") << 32 << 32;
    BOBUIest::newRow("11111111111111111111111111111111111111111111111111111111")
        << QString("11111111111111111111111111111111111111111111111111111111") << 56 << 56;
    BOBUIest::newRow("00000000000000000000000000000000") << QString("00000000000000000000000000000000") << 32 << 0;
    BOBUIest::newRow("00000000000000000000000000000000000000000000000000000000")
        << QString("00000000000000000000000000000000000000000000000000000000") << 56 << 0;
}

void tst_QBitArray::countBits()
{
    QFETCH(QString, bitField);
    QFETCH(int, numBits);
    QFETCH(int, onBits);

    QBitArray bits(bitField.size());
    for (int i = 0; i < bitField.size(); ++i) {
        if (bitField.at(i) == QLatin1Char('1'))
            bits.setBit(i);
    }

    QCOMPARE(bits.size(), numBits);
    // NOLINTNEXTLINE(bobui-port-to-std-compatible-api): We want to test count() and size()
    QCOMPARE(bits.count(), numBits);
    QCOMPARE(bits.count(true), onBits);
    QCOMPARE(bits.count(false), numBits - onBits);
}

void tst_QBitArray::countBits2()
{
    QBitArray bitArray;
    for (int i = 0; i < 4017; ++i) {
        bitArray.resize(i);
        bitArray.fill(true);
        QCOMPARE(bitArray.count(true), i);
        QCOMPARE(bitArray.count(false), 0);
        bitArray.fill(false);
        QCOMPARE(bitArray.count(true), 0);
        QCOMPARE(bitArray.count(false), i);
    }
}

void tst_QBitArray::isEmpty()
{
    QBitArray a1;
    QVERIFY(a1.isEmpty());
    QVERIFY(a1.isNull());
    QVERIFY(a1.size() == 0);

    QBitArray a2(0, true);
    QVERIFY(a2.isEmpty());
    QVERIFY(!a2.isNull());
    QVERIFY(a2.size() == 0);

    QBitArray a3(1, true);
    QVERIFY(!a3.isEmpty());
    QVERIFY(!a3.isNull());
    QVERIFY(a3.size() == 1);

    a1.resize(0);
    QVERIFY(a1.isEmpty());
    QVERIFY(!a1.isNull());
    QVERIFY(a1.size() == 0);

    a2.resize(0);
    QVERIFY(a2.isEmpty());
    QVERIFY(!a2.isNull());
    QVERIFY(a2.size() == 0);

    a1.resize(1);
    QVERIFY(!a1.isEmpty());
    QVERIFY(!a1.isNull());
    QVERIFY(a1.size() == 1);

    a1.resize(2);
    QVERIFY(!a1.isEmpty());
    QVERIFY(!a1.isNull());
    QVERIFY(a1.size() == 2);

    BOBUI_TEST_EQUALITY_OPS(a1, a2, false);
    BOBUI_TEST_EQUALITY_OPS(a2, a3, false);
    BOBUI_TEST_EQUALITY_OPS(QBitArray(), QBitArray(), true);
    a3 = a2;
    BOBUI_TEST_EQUALITY_OPS(a2, a3, true);
}

void tst_QBitArray::swap()
{
    QBitArray b1 = QStringToQBitArray("1"), b2 = QStringToQBitArray("10");
    b1.swap(b2);
    BOBUI_TEST_EQUALITY_OPS(b1,QStringToQBitArray("10"), true);
    BOBUI_TEST_EQUALITY_OPS(b2,QStringToQBitArray("1"), true);
}

void tst_QBitArray::fill()
{
    int N = 64;
    int M = 17;
    QBitArray a(N, false);
    int i, j;

    for (i = 0; i < N-M; ++i) {
        a.fill(true, i, i + M);
        for (j = 0; j < N; ++j) {
            if (j >= i && j < i + M) {
                QVERIFY(a.at(j));
            } else {
                QVERIFY(!a.at(j));
            }
        }
        a.fill(false, i, i + M);
    }
    for (i = 0; i < N; ++i)
        a.fill(i % 2 == 0, i, i + 1);
    for (i = 0; i < N; ++i) {
        QVERIFY(a.at(i) == (i % 2 == 0));
    }
}

void tst_QBitArray::toggleBit_data()
{
    BOBUIest::addColumn<int>("index");
    BOBUIest::addColumn<QBitArray>("input");
    BOBUIest::addColumn<QBitArray>("res");
    // 8 bits, toggle first bit
    BOBUIest::newRow( "data0" )  << 0 << QStringToQBitArray(QString("11111111")) << QStringToQBitArray(QString("01111111"));
    // 8 bits
    BOBUIest::newRow( "data1" )  << 1 << QStringToQBitArray(QString("11111111")) << QStringToQBitArray(QString("10111111"));
    // 11 bits, toggle last bit
    BOBUIest::newRow( "data2" )  << 10 << QStringToQBitArray(QString("11111111111")) << QStringToQBitArray(QString("11111111110"));

}

void tst_QBitArray::toggleBit()
{
    QFETCH(int,index);
    QFETCH(QBitArray, input);
    QFETCH(QBitArray, res);

    input.toggleBit(index);

    BOBUI_TEST_EQUALITY_OPS(input, res, true);
}

void tst_QBitArray::operator_andeq_data()
{
    BOBUIest::addColumn<QBitArray>("input1");
    BOBUIest::addColumn<QBitArray>("input2");
    BOBUIest::addColumn<QBitArray>("res");

    BOBUIest::newRow( "data0" )   << QStringToQBitArray(QString("11111111"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("00101100"));


    BOBUIest::newRow( "data1" )   << QStringToQBitArray(QString("11011011"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("00001000"));

    BOBUIest::newRow( "data2" )   << QStringToQBitArray(QString("11011011111"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("00001000000"));

    BOBUIest::newRow( "data3" )   << QStringToQBitArray(QString("11011011"))
                            << QStringToQBitArray(QString("00101100111"))
                            << QStringToQBitArray(QString("00001000000"));

    BOBUIest::newRow( "data4" )   << QStringToQBitArray(QString())
                            << QStringToQBitArray(QString("00101100111"))
                            << QStringToQBitArray(QString("00000000000"));

    BOBUIest::newRow( "data5" ) << QStringToQBitArray(QString("00101100111"))
                             << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString("00000000000"));

    BOBUIest::newRow( "data6" ) << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString());
}

#define COMPARE_BITARRAY_EQ(actual, expected)       do {        \
        BOBUI_TEST_EQUALITY_OPS(actual, expected, true);           \
        QCOMPARE(actual.count(), expected.count());             \
        QCOMPARE(actual.count(true), expected.count(true));     \
        QCOMPARE(actual.count(false), expected.count(false));   \
    } while (false)

void tst_QBitArray::operator_andeq()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(QBitArray, res);

    QBitArray result = input1;
    result &= input2;
    COMPARE_BITARRAY_EQ(result, res);
    result = input1;
    result &= std::move(input2);
    COMPARE_BITARRAY_EQ(result, res);
    result = input1;
    result &= detached(input2);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is commutative
    result = input2;
    result &= input1;
    COMPARE_BITARRAY_EQ(result, res);
    result = input2;
    result &= std::move(input1);
    COMPARE_BITARRAY_EQ(result, res);
    result = input2;
    result &= detached(input1);
    COMPARE_BITARRAY_EQ(result, res);

BOBUI_WARNING_PUSH
// Android clang emits warning: explicitly assigning value of variable of type 'QBitArray' to itself
BOBUI_WARNING_DISABLE_CLANG("-Wself-assign-overloaded")
    // operation is idempotent
    result &= result;
    COMPARE_BITARRAY_EQ(result, res);
    result &= std::move(result);
    COMPARE_BITARRAY_EQ(result, res);
    result &= detached(result);
    COMPARE_BITARRAY_EQ(result, res);
BOBUI_WARNING_POP
}

void tst_QBitArray::operator_and()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(QBitArray, res);

    QBitArray result = input1 & input2;
    COMPARE_BITARRAY_EQ(result, res);
    result = input1 & QBitArray(input2);
    COMPARE_BITARRAY_EQ(result, res);
    result = input1 & detached(input2);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is commutative
    result = input2 & input1;
    COMPARE_BITARRAY_EQ(result, res);
    result = input2 & QBitArray(input1);
    COMPARE_BITARRAY_EQ(result, res);
    result = input2 & detached(input1);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is idempotent
    result = result & result;
    COMPARE_BITARRAY_EQ(result, res);
    result = result & QBitArray(result);
    COMPARE_BITARRAY_EQ(result, res);
    result = result & detached(result);
    COMPARE_BITARRAY_EQ(result, res);
}

void tst_QBitArray::operator_oreq_data()
{
    BOBUIest::addColumn<QBitArray>("input1");
    BOBUIest::addColumn<QBitArray>("input2");
    BOBUIest::addColumn<QBitArray>("res");

    BOBUIest::newRow( "data0" )   << QStringToQBitArray(QString("11111111"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("11111111"));


    BOBUIest::newRow( "data1" )   << QStringToQBitArray(QString("11011011"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("11111111"));

    BOBUIest::newRow( "data2" )   << QStringToQBitArray(QString("01000010"))
                            << QStringToQBitArray(QString("10100001"))
                            << QStringToQBitArray(QString("11100011"));

    BOBUIest::newRow( "data3" )   << QStringToQBitArray(QString("11011011"))
                            << QStringToQBitArray(QString("00101100000"))
                            << QStringToQBitArray(QString("11111111000"));

    BOBUIest::newRow( "data4" )   << QStringToQBitArray(QString("11011011111"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("11111111111"));

    BOBUIest::newRow( "data5" )   << QStringToQBitArray(QString())
                            << QStringToQBitArray(QString("00101100111"))
                            << QStringToQBitArray(QString("00101100111"));

    BOBUIest::newRow( "data6" ) << QStringToQBitArray(QString("00101100111"))
                             << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString("00101100111"));

    BOBUIest::newRow( "data7" ) << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString());
}

void tst_QBitArray::operator_oreq()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(QBitArray, res);

    QBitArray result = input1;
    result |= input2;
    COMPARE_BITARRAY_EQ(result, res);
    result = input1;
    result |= QBitArray(input2);
    COMPARE_BITARRAY_EQ(result, res);
    result = input1;
    result |= detached(input2);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is commutative
    result = input2;
    result |= input1;
    COMPARE_BITARRAY_EQ(result, res);
    result = input2;
    result |= QBitArray(input1);
    COMPARE_BITARRAY_EQ(result, res);
    result = input2;
    result |= detached(input1);
    COMPARE_BITARRAY_EQ(result, res);

BOBUI_WARNING_PUSH
// Android clang emits warning: explicitly assigning value of variable of type 'QBitArray' to itself
BOBUI_WARNING_DISABLE_CLANG("-Wself-assign-overloaded")
    // operation is idempotent
    result |= result;
    COMPARE_BITARRAY_EQ(result, res);
    result |= QBitArray(result);
    COMPARE_BITARRAY_EQ(result, res);
    result |= detached(result);
    COMPARE_BITARRAY_EQ(result, res);
BOBUI_WARNING_POP
}

void tst_QBitArray::operator_or()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(QBitArray, res);

    QBitArray result = input1 | input2;
    COMPARE_BITARRAY_EQ(result, res);
    result = input1 | QBitArray(input2);
    COMPARE_BITARRAY_EQ(result, res);
    result = input1 | detached(input2);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is commutative
    result = input2 | input1;
    COMPARE_BITARRAY_EQ(result, res);
    result = input2 | QBitArray(input1);
    COMPARE_BITARRAY_EQ(result, res);
    result = input2 | detached(input1);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is idempotent
    result = result | result;
    COMPARE_BITARRAY_EQ(result, res);
    result = result | QBitArray(result);
    COMPARE_BITARRAY_EQ(result, res);
    result = result | detached(result);
    COMPARE_BITARRAY_EQ(result, res);
}

void tst_QBitArray::operator_xoreq_data()
{
    BOBUIest::addColumn<QBitArray>("input1");
    BOBUIest::addColumn<QBitArray>("input2");
    BOBUIest::addColumn<QBitArray>("res");
    BOBUIest::newRow( "data0" )   << QStringToQBitArray(QString("11111111"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("11010011"));

    BOBUIest::newRow( "data1" )   << QStringToQBitArray(QString("11011011"))
                            << QStringToQBitArray(QString("00101100"))
                            << QStringToQBitArray(QString("11110111"));

    BOBUIest::newRow( "data2" )   << QStringToQBitArray(QString("01000010"))
                            << QStringToQBitArray(QString("10100001"))
                            << QStringToQBitArray(QString("11100011"));

    BOBUIest::newRow( "data3" )   << QStringToQBitArray(QString("01000010"))
                            << QStringToQBitArray(QString("10100001101"))
                            << QStringToQBitArray(QString("11100011101"));

    BOBUIest::newRow( "data4" )   << QStringToQBitArray(QString("01000010111"))
                            << QStringToQBitArray(QString("101000011"))
                            << QStringToQBitArray(QString("11100011011"));

    BOBUIest::newRow( "data5" )   << QStringToQBitArray(QString())
                            << QStringToQBitArray(QString("00101100111"))
                            << QStringToQBitArray(QString("00101100111"));

    BOBUIest::newRow( "data6" ) << QStringToQBitArray(QString("00101100111"))
                             << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString("00101100111"));

    BOBUIest::newRow( "data7" ) << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString())
                             << QStringToQBitArray(QString());
}

void tst_QBitArray::operator_xoreq()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(QBitArray, res);

    QBitArray result = input1;
    result ^= input2;
    COMPARE_BITARRAY_EQ(result, res);
    result = input1;
    result ^= QBitArray(input2);
    COMPARE_BITARRAY_EQ(result, res);
    result = input1;
    result ^= detached(input2);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is commutative
    result = input2;
    result ^= input1;
    COMPARE_BITARRAY_EQ(result, res);
    result = input2;
    result ^= QBitArray(input1);
    COMPARE_BITARRAY_EQ(result, res);
    result = input2;
    result ^= detached(input1);
    COMPARE_BITARRAY_EQ(result, res);

    // XORing with oneself is nilpotent
    result = input1;
    result ^= input1;
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input1.size()), true);
    result = input1;
    result ^= QBitArray(result);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input1.size()), true);
    result = input1;
    result ^= detached(result);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input1.size()), true);

    result = input2;
    result ^= input2;
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input2.size()), true);
    result = input2;
    result ^= QBitArray(input2);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input2.size()), true);
    result = input2;
    result ^= detached(input2);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input2.size()), true);

    result = res;
    result ^= res;
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(res.size()), true);
    result = res;
    result ^= QBitArray(res);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(res.size()), true);
    result = res;
    result ^= detached(res);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(res.size()), true);
}

void tst_QBitArray::operator_xor()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(QBitArray, res);

    QBitArray result = input1 ^ input2;
    COMPARE_BITARRAY_EQ(result, res);
    result = input1 ^ QBitArray(input2);
    COMPARE_BITARRAY_EQ(result, res);
    result = input1 ^ detached(input2);
    COMPARE_BITARRAY_EQ(result, res);

    // operation is commutative
    result = input2 ^ input1;
    COMPARE_BITARRAY_EQ(result, res);
    result = input2 ^ QBitArray(input1);
    COMPARE_BITARRAY_EQ(result, res);
    result = input2 ^ detached(input1);
    COMPARE_BITARRAY_EQ(result, res);

    // XORing with oneself is nilpotent
    result = input1 ^ input1;
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input1.size()), true);
    result = input1 ^ QBitArray(input1);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input1.size()), true);
    result = input1 ^ detached(input1);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input1.size()), true);

    result = input2 ^ input2;
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input2.size()), true);
    result = input2 ^ QBitArray(input2);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input2.size()), true);
    result = input2 ^ detached(input2);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(input2.size()), true);

    result = res ^ res;
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(res.size()), true);
    result = res ^ QBitArray(res);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(res.size()), true);
    result = res ^ detached(res);
    BOBUI_TEST_EQUALITY_OPS(result, QBitArray(res.size()), true);
}

void tst_QBitArray::operator_neg_data()
{
    BOBUIest::addColumn<QBitArray>("input");
    BOBUIest::addColumn<QBitArray>("res");

    BOBUIest::newRow( "data0" )   << QStringToQBitArray(QString("11111111"))
                               << QStringToQBitArray(QString("00000000"));

    BOBUIest::newRow( "data1" )   << QStringToQBitArray(QString("11011011"))
                               << QStringToQBitArray(QString("00100100"));

    BOBUIest::newRow( "data2" )   << QStringToQBitArray(QString("00000000"))
                               << QStringToQBitArray(QString("11111111"));

    BOBUIest::newRow( "data3" )   << QStringToQBitArray(QString())
                               << QStringToQBitArray(QString());

    BOBUIest::newRow( "data4" )   << QStringToQBitArray("1")
                               << QStringToQBitArray("0");

    BOBUIest::newRow( "data5" )   << QStringToQBitArray("0")
                               << QStringToQBitArray("1");

    BOBUIest::newRow( "data6" )   << QStringToQBitArray("01")
                               << QStringToQBitArray("10");

    BOBUIest::newRow( "data7" )   << QStringToQBitArray("1110101")
                               << QStringToQBitArray("0001010");

    BOBUIest::newRow( "data8" )   << QStringToQBitArray("01110101")
                               << QStringToQBitArray("10001010");

    BOBUIest::newRow( "data9" )   << QStringToQBitArray("011101010")
                               << QStringToQBitArray("100010101");

    BOBUIest::newRow( "data10" )   << QStringToQBitArray("0111010101111010")
                                << QStringToQBitArray("1000101010000101");
}

void tst_QBitArray::operator_neg()
{
    QFETCH(QBitArray, input);
    QFETCH(QBitArray, res);

    input = ~input;

    COMPARE_BITARRAY_EQ(input, res);
    BOBUI_TEST_EQUALITY_OPS(~~input, res, true);     // performs two in-place negations
}

void tst_QBitArray::datastream_data()
{
    BOBUIest::addColumn<QString>("bitField");
    BOBUIest::addColumn<int>("numBits");
    BOBUIest::addColumn<int>("onBits");

    BOBUIest::newRow("empty") << QString() << 0 << 0;
    BOBUIest::newRow("1") << QString("1") << 1 << 1;
    BOBUIest::newRow("101") << QString("101") << 3 << 2;
    BOBUIest::newRow("101100001") << QString("101100001") << 9 << 4;
    BOBUIest::newRow("101100001101100001") << QString("101100001101100001") << 18 << 8;
    BOBUIest::newRow("101100001101100001101100001101100001") << QString("101100001101100001101100001101100001") << 36 << 16;
    BOBUIest::newRow("00000000000000000000000000000000000") << QString("00000000000000000000000000000000000") << 35 << 0;
    BOBUIest::newRow("11111111111111111111111111111111111") << QString("11111111111111111111111111111111111") << 35 << 35;
    BOBUIest::newRow("11111111111111111111111111111111") << QString("11111111111111111111111111111111") << 32 << 32;
    BOBUIest::newRow("11111111111111111111111111111111111111111111111111111111")
        << QString("11111111111111111111111111111111111111111111111111111111") << 56 << 56;
    BOBUIest::newRow("00000000000000000000000000000000") << QString("00000000000000000000000000000000") << 32 << 0;
    BOBUIest::newRow("00000000000000000000000000000000000000000000000000000000")
        << QString("00000000000000000000000000000000000000000000000000000000") << 56 << 0;
}

void tst_QBitArray::datastream()
{
    QFETCH(QString, bitField);
    QFETCH(int, numBits);
    QFETCH(int, onBits);

    QBuffer buffer;
    QVERIFY(buffer.open(QBuffer::ReadWrite));
    QDataStream stream(&buffer);

    QBitArray bits(bitField.size());
    for (int i = 0; i < bitField.size(); ++i) {
        if (bitField.at(i) == QLatin1Char('1'))
            bits.setBit(i);
    }

    QCOMPARE(bits.size(), numBits);
    QCOMPARE(bits.count(true), onBits);
    QCOMPARE(bits.count(false), numBits - onBits);

    stream << bits << bits << bits;
    buffer.close();

    QCOMPARE(stream.status(), QDataStream::Ok);

    QVERIFY(buffer.open(QBuffer::ReadWrite));
    QDataStream stream2(&buffer);

    QBitArray array1, array2, array3;
    stream2 >> array1 >> array2 >> array3;

    QCOMPARE(array1.size(), numBits);
    QCOMPARE(array1.count(true), onBits);
    QCOMPARE(array1.count(false), numBits - onBits);

    COMPARE_BITARRAY_EQ(array1, bits);
    COMPARE_BITARRAY_EQ(array2, bits);
    COMPARE_BITARRAY_EQ(array3, bits);
}

void tst_QBitArray::invertOnNull() const
{
    QBitArray a;
    BOBUI_TEST_EQUALITY_OPS(a = ~a, QBitArray(), true);
}

void tst_QBitArray::operator_noteq_data()
{
    BOBUIest::addColumn<QBitArray>("input1");
    BOBUIest::addColumn<QBitArray>("input2");
    BOBUIest::addColumn<bool>("res");

    BOBUIest::newRow("data0") << QStringToQBitArray(QString("11111111"))
                           << QStringToQBitArray(QString("00101100"))
                           << true;

    BOBUIest::newRow("data1") << QStringToQBitArray(QString("11011011"))
                           << QStringToQBitArray(QString("11011011"))
                           << false;

    BOBUIest::newRow("data2") << QStringToQBitArray(QString())
                           << QStringToQBitArray(QString("00101100111"))
                           << true;

    BOBUIest::newRow("data3") << QStringToQBitArray(QString())
                           << QStringToQBitArray(QString())
                           << false;

    BOBUIest::newRow("data4") << QStringToQBitArray(QString("00101100"))
                           << QStringToQBitArray(QString("11111111"))
                           << true;

    BOBUIest::newRow("data5") << QStringToQBitArray(QString("00101100111"))
                           << QStringToQBitArray(QString())
                           << true;
}

void tst_QBitArray::operator_noteq()
{
    QFETCH(QBitArray, input1);
    QFETCH(QBitArray, input2);
    QFETCH(bool, res);

    bool b = input1 != input2;
    BOBUI_TEST_EQUALITY_OPS(b, res, true);
}

void tst_QBitArray::resize()
{
    // -- check that a resize handles the bits correctly
    QBitArray a = QStringToQBitArray(QString("11"));
    a.resize(10);
    QVERIFY(a.size() == 10);
    BOBUI_TEST_EQUALITY_OPS( a, QStringToQBitArray(QString("1100000000")), true);

    a.setBit(9);
    a.resize(9);
    // now the bit in a should have been gone:
    BOBUI_TEST_EQUALITY_OPS( a, QStringToQBitArray(QString("110000000")), true);

    // grow the array back and check the new bit
    a.resize(10);
    BOBUI_TEST_EQUALITY_OPS( a, QStringToQBitArray(QString("1100000000")), true);

    // other test with and
    a.resize(9);
    QBitArray b = QStringToQBitArray(QString("1111111111"));
    b &= a;
    BOBUI_TEST_EQUALITY_OPS( b, QStringToQBitArray(QString("1100000000")), true);

}

void tst_QBitArray::fromBits_data()
{
    BOBUIest::addColumn<QByteArray>("data");
    BOBUIest::addColumn<int>("size");
    BOBUIest::addColumn<QBitArray>("expected");

    BOBUIest::newRow("empty") << QByteArray() << 0 << QBitArray();

    auto add = [](const QByteArray &tag, const char *data) {
        BOBUIest::newRow(tag) << QByteArray(data, (tag.size() + 7) / 8) << tag.size()
                           << QStringToQBitArray(tag);
    };

    // "0" to "0000000000000000"
    for (int i = 1; i < 16; ++i) {
        char zero[2] = { 0, 0 };
        QByteArray pattern(i, '0');
        add(pattern, zero);
    }

    // "1" to "1111111111111111"
    for (int i = 1; i < 16; ++i) {
        char one[2] = { '\xff', '\xff' };
        QByteArray pattern(i, '1');
        add(pattern, one);
    }

    // trailing 0 and 1
    char zero = 1;
    char one = 0;
    QByteArray pzero = "1";
    QByteArray pone = "0";
    for (int i = 2; i < 8; ++i) {
        zero <<= 1;
        pzero.prepend('0');
        add(pzero, &zero);

        one = (one << 1) | 1;
        pone.prepend('1');
        add(pone, &one);
    }
}

void tst_QBitArray::fromBits()
{
    QFETCH(QByteArray, data);
    QFETCH(int, size);
    QFETCH(QBitArray, expected);

    QBitArray fromBits = QBitArray::fromBits(data, size);
    BOBUI_TEST_EQUALITY_OPS(fromBits, expected, true);

    BOBUI_TEST_EQUALITY_OPS(QBitArray::fromBits(fromBits.bits(), fromBits.size()), expected, true);
}

void tst_QBitArray::toUInt32_data()
{
    BOBUIest::addColumn<QBitArray>("data");
    BOBUIest::addColumn<int>("endianness");
    BOBUIest::addColumn<bool>("check");
    BOBUIest::addColumn<quint32>("result");

    BOBUIest::newRow("ctor")           << QBitArray()
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(0);

    BOBUIest::newRow("empty")          << QBitArray(0)
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(0);

    BOBUIest::newRow("LittleEndian4")  << QStringToQBitArray(QString("0111"))
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(14);

    BOBUIest::newRow("BigEndian4")     << QStringToQBitArray(QString("0111"))
                                    << static_cast<int>(QSysInfo::Endian::BigEndian)
                                    << true
                                    << quint32(7);

    BOBUIest::newRow("LittleEndian8")  << QStringToQBitArray(QString("01111111"))
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(254);

    BOBUIest::newRow("BigEndian8")     << QStringToQBitArray(QString("01111111"))
                                    << static_cast<int>(QSysInfo::Endian::BigEndian)
                                    << true
                                    << quint32(127);

    BOBUIest::newRow("LittleEndian16") << QStringToQBitArray(QString("0111111111111111"))
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(65534);

    BOBUIest::newRow("BigEndian16")    << QStringToQBitArray(QString("0111111111111111"))
                                    << static_cast<int>(QSysInfo::Endian::BigEndian)
                                    << true
                                    << quint32(32767);

    BOBUIest::newRow("LittleEndian31") << QBitArray(31, true)
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(2147483647);

    BOBUIest::newRow("BigEndian31")    << QBitArray(31, true)
                                    << static_cast<int>(QSysInfo::Endian::BigEndian)
                                    << true
                                    << quint32(2147483647);

    BOBUIest::newRow("LittleEndian32") << QBitArray(32, true)
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << true
                                    << quint32(4294967295);

    BOBUIest::newRow("BigEndian32")    << QBitArray(32, true)
                                    << static_cast<int>(QSysInfo::Endian::BigEndian)
                                    << true
                                    << quint32(4294967295);

    BOBUIest::newRow("LittleEndian33") << QBitArray(33, true)
                                    << static_cast<int>(QSysInfo::Endian::LittleEndian)
                                    << false
                                    << quint32(0);

    BOBUIest::newRow("BigEndian33")    << QBitArray(33, true)
                                    << static_cast<int>(QSysInfo::Endian::BigEndian)
                                    << false
                                    << quint32(0);
}

void tst_QBitArray::toUInt32()
{
    QFETCH(QBitArray, data);
    QFETCH(int, endianness);
    QFETCH(bool, check);
    QFETCH(quint32, result);
    bool ok = false;

    QCOMPARE(data.toUInt32(static_cast<QSysInfo::Endian>(endianness), &ok), result);
    QCOMPARE(ok, check);
}

BOBUIEST_APPLESS_MAIN(tst_QBitArray)
#include "tst_qbitarray.moc"
