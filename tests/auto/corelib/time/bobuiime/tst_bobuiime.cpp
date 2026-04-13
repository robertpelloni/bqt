// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <private/qglobal_p.h>
#include <private/qcomparisontesthelper_p.h>
#include <BOBUIest>
#include "qdatetime.h"
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(6, 0, 0)
#  include <locale.h>
#endif

using namespace BobUI::StringLiterals;

class tst_BOBUIime : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void msecsTo_data();
    void msecsTo();
    void secsTo_data();
    void secsTo();
    void setHMS_data();
    void setHMS();
    void hour_data();
    void hour();
    void isValid();
    void isNull();
    void addMSecs_data();
    void addMSecs();
    void addSecs_data();
    void addSecs();
    void orderingCompiles();
    void operator_eq_eq_data();
    void operator_eq_eq();
    void ordering_data();
    void ordering();
#if BOBUI_CONFIG(datestring)
# if BOBUI_CONFIG(datetimeparser)
    void fromStringFormat_data();
    void fromStringFormat();
# endif
    void fromStringDateFormat_data();
    void fromStringDateFormat();
    void toStringDateFormat_data();
    void toStringDateFormat();
    void toStringFormat_data();
    void toStringFormat();
#endif
    void msecsSinceStartOfDay_data();
    void msecsSinceStartOfDay();

private:
    BOBUIime invalidTime() { return BOBUIime(-1, -1, -1); }
};

Q_DECLARE_METATYPE(BobUI::DateFormat)

void tst_BOBUIime::addSecs_data()
{
    BOBUIest::addColumn<BOBUIime>("t1");
    BOBUIest::addColumn<int>("i");
    BOBUIest::addColumn<BOBUIime>("exp");

    BOBUIest::newRow("Data0") << BOBUIime(0,0,0) << 200 << BOBUIime(0,3,20);
    BOBUIest::newRow("Data1") << BOBUIime(0,0,0) << 20 << BOBUIime(0,0,20);
    BOBUIest::newRow("overflow")
        << BOBUIime(0,0,0) << (INT_MAX / 1000 + 1)
        << BOBUIime::fromMSecsSinceStartOfDay(((INT_MAX / 1000 + 1) % 86400) * 1000);
}

void tst_BOBUIime::addSecs()
{
    QFETCH( BOBUIime, t1 );
    QFETCH( int, i );
    BOBUIime t2;
    t2 = t1.addSecs( i );
    QFETCH( BOBUIime, exp );
    QCOMPARE( t2, exp );
}

void tst_BOBUIime::addMSecs_data()
{
    BOBUIest::addColumn<BOBUIime>("t1");
    BOBUIest::addColumn<int>("i");
    BOBUIest::addColumn<BOBUIime>("exp");

    // start with testing positive values
    BOBUIest::newRow( "Data1_0") << BOBUIime(0,0,0,0) << 2000 << BOBUIime(0,0,2,0);
    BOBUIest::newRow( "Data1_1") << BOBUIime(0,0,0,0) << 200 << BOBUIime(0,0,0,200);
    BOBUIest::newRow( "Data1_2") << BOBUIime(0,0,0,0) << 20 << BOBUIime(0,0,0,20);
    BOBUIest::newRow( "Data1_3") << BOBUIime(0,0,0,1) << 1 << BOBUIime(0,0,0,2);
    BOBUIest::newRow( "Data1_4") << BOBUIime(0,0,0,0) << 0 << BOBUIime(0,0,0,0);

    BOBUIest::newRow( "Data2_0") << BOBUIime(0,0,0,98) << 0 << BOBUIime(0,0,0,98);
    BOBUIest::newRow( "Data2_1") << BOBUIime(0,0,0,98) << 1 << BOBUIime(0,0,0,99);
    BOBUIest::newRow( "Data2_2") << BOBUIime(0,0,0,98) << 2 << BOBUIime(0,0,0,100);
    BOBUIest::newRow( "Data2_3") << BOBUIime(0,0,0,98) << 3 << BOBUIime(0,0,0,101);

    BOBUIest::newRow( "Data3_0") << BOBUIime(0,0,0,998) << 0 << BOBUIime(0,0,0,998);
    BOBUIest::newRow( "Data3_1") << BOBUIime(0,0,0,998) << 1 << BOBUIime(0,0,0,999);
    BOBUIest::newRow( "Data3_2") << BOBUIime(0,0,0,998) << 2 << BOBUIime(0,0,1,0);
    BOBUIest::newRow( "Data3_3") << BOBUIime(0,0,0,998) << 3 << BOBUIime(0,0,1,1);

    BOBUIest::newRow( "Data4_0") << BOBUIime(0,0,1,995) << 4 << BOBUIime(0,0,1,999);
    BOBUIest::newRow( "Data4_1") << BOBUIime(0,0,1,995) << 5 << BOBUIime(0,0,2,0);
    BOBUIest::newRow( "Data4_2") << BOBUIime(0,0,1,995) << 6 << BOBUIime(0,0,2,1);
    BOBUIest::newRow( "Data4_3") << BOBUIime(0,0,1,995) << 100 << BOBUIime(0,0,2,95);
    BOBUIest::newRow( "Data4_4") << BOBUIime(0,0,1,995) << 105 << BOBUIime(0,0,2,100);

    BOBUIest::newRow( "Data5_0") << BOBUIime(0,0,59,995) << 4 << BOBUIime(0,0,59,999);
    BOBUIest::newRow( "Data5_1") << BOBUIime(0,0,59,995) << 5 << BOBUIime(0,1,0,0);
    BOBUIest::newRow( "Data5_2") << BOBUIime(0,0,59,995) << 6 << BOBUIime(0,1,0,1);
    BOBUIest::newRow( "Data5_3") << BOBUIime(0,0,59,995) << 1006 << BOBUIime(0,1,1,1);

    BOBUIest::newRow( "Data6_0") << BOBUIime(0,59,59,995) << 4 << BOBUIime(0,59,59,999);
    BOBUIest::newRow( "Data6_1") << BOBUIime(0,59,59,995) << 5 << BOBUIime(1,0,0,0);
    BOBUIest::newRow( "Data6_2") << BOBUIime(0,59,59,995) << 6 << BOBUIime(1,0,0,1);
    BOBUIest::newRow( "Data6_3") << BOBUIime(0,59,59,995) << 106 << BOBUIime(1,0,0,101);
    BOBUIest::newRow( "Data6_4") << BOBUIime(0,59,59,995) << 1004 << BOBUIime(1,0,0,999);
    BOBUIest::newRow( "Data6_5") << BOBUIime(0,59,59,995) << 1005 << BOBUIime(1,0,1,0);
    BOBUIest::newRow( "Data6_6") << BOBUIime(0,59,59,995) << 61006 << BOBUIime(1,1,1,1);

    BOBUIest::newRow( "Data7_0") << BOBUIime(23,59,59,995) << 0 << BOBUIime(23,59,59,995);
    BOBUIest::newRow( "Data7_1") << BOBUIime(23,59,59,995) << 4 << BOBUIime(23,59,59,999);
    BOBUIest::newRow( "Data7_2") << BOBUIime(23,59,59,995) << 5 << BOBUIime(0,0,0,0);
    BOBUIest::newRow( "Data7_3") << BOBUIime(23,59,59,995) << 6 << BOBUIime(0,0,0,1);
    BOBUIest::newRow( "Data7_4") << BOBUIime(23,59,59,995) << 7 << BOBUIime(0,0,0,2);

    // must test negative values too...
    BOBUIest::newRow( "Data11_0") << BOBUIime(0,0,2,0) << -2000 << BOBUIime(0,0,0,0);
    BOBUIest::newRow( "Data11_1") << BOBUIime(0,0,0,200) << -200 << BOBUIime(0,0,0,0);
    BOBUIest::newRow( "Data11_2") << BOBUIime(0,0,0,20) << -20 << BOBUIime(0,0,0,0);
    BOBUIest::newRow( "Data11_3") << BOBUIime(0,0,0,2) << -1 << BOBUIime(0,0,0,1);
    BOBUIest::newRow( "Data11_4") << BOBUIime(0,0,0,0) << -0 << BOBUIime(0,0,0,0);

    BOBUIest::newRow( "Data12_0") << BOBUIime(0,0,0,98) << -0 << BOBUIime(0,0,0,98);
    BOBUIest::newRow( "Data12_1") << BOBUIime(0,0,0,99) << -1 << BOBUIime(0,0,0,98);
    BOBUIest::newRow( "Data12_2") << BOBUIime(0,0,0,100) << -2 << BOBUIime(0,0,0,98);
    BOBUIest::newRow( "Data12_3") << BOBUIime(0,0,0,101) << -3 << BOBUIime(0,0,0,98);

    BOBUIest::newRow( "Data13_0") << BOBUIime(0,0,0,998) << -0 << BOBUIime(0,0,0,998);
    BOBUIest::newRow( "Data13_1") << BOBUIime(0,0,0,999) << -1 << BOBUIime(0,0,0,998);
    BOBUIest::newRow( "Data13_2") << BOBUIime(0,0,1,0) << -2 << BOBUIime(0,0,0,998);
    BOBUIest::newRow( "Data13_3") << BOBUIime(0,0,1,1) << -3 << BOBUIime(0,0,0,998);

    BOBUIest::newRow( "Data14_0") << BOBUIime(0,0,1,999) << -4 << BOBUIime(0,0,1,995);
    BOBUIest::newRow( "Data14_1") << BOBUIime(0,0,2,0) << -5 << BOBUIime(0,0,1,995);
    BOBUIest::newRow( "Data14_2") << BOBUIime(0,0,2,1) << -6 << BOBUIime(0,0,1,995);
    BOBUIest::newRow( "Data14_3") << BOBUIime(0,0,2,95) << -100 << BOBUIime(0,0,1,995);
    BOBUIest::newRow( "Data14_4") << BOBUIime(0,0,2,100) << -105 << BOBUIime(0,0,1,995);

    BOBUIest::newRow( "Data15_0") << BOBUIime(0,0,59,999) << -4 << BOBUIime(0,0,59,995);
    BOBUIest::newRow( "Data15_1") << BOBUIime(0,1,0,0) << -5 << BOBUIime(0,0,59,995);
    BOBUIest::newRow( "Data15_2") << BOBUIime(0,1,0,1) << -6 << BOBUIime(0,0,59,995);
    BOBUIest::newRow( "Data15_3") << BOBUIime(0,1,1,1) << -1006 << BOBUIime(0,0,59,995);

    BOBUIest::newRow( "Data16_0") << BOBUIime(0,59,59,999) << -4 << BOBUIime(0,59,59,995);
    BOBUIest::newRow( "Data16_1") << BOBUIime(1,0,0,0) << -5 << BOBUIime(0,59,59,995);
    BOBUIest::newRow( "Data16_2") << BOBUIime(1,0,0,1) << -6 << BOBUIime(0,59,59,995);
    BOBUIest::newRow( "Data16_3") << BOBUIime(1,0,0,101) << -106 << BOBUIime(0,59,59,995);
    BOBUIest::newRow( "Data16_4") << BOBUIime(1,0,0,999) << -1004 << BOBUIime(0,59,59,995);
    BOBUIest::newRow( "Data16_5") << BOBUIime(1,0,1,0) << -1005 << BOBUIime(0,59,59,995);
    BOBUIest::newRow( "Data16_6") << BOBUIime(1,1,1,1) << -61006 << BOBUIime(0,59,59,995);

    BOBUIest::newRow( "Data17_0") << BOBUIime(23,59,59,995) << -0 << BOBUIime(23,59,59,995);
    BOBUIest::newRow( "Data17_1") << BOBUIime(23,59,59,999) << -4 << BOBUIime(23,59,59,995);
    BOBUIest::newRow( "Data17_2") << BOBUIime(0,0,0,0) << -5 << BOBUIime(23,59,59,995);
    BOBUIest::newRow( "Data17_3") << BOBUIime(0,0,0,1) << -6 << BOBUIime(23,59,59,995);
    BOBUIest::newRow( "Data17_4") << BOBUIime(0,0,0,2) << -7 << BOBUIime(23,59,59,995);

    BOBUIest::newRow( "Data18_0" ) << invalidTime() << 1 << invalidTime();
}

void tst_BOBUIime::addMSecs()
{
    QFETCH( BOBUIime, t1 );
    QFETCH( int, i );
    BOBUIime t2;
    t2 = t1.addMSecs( i );
    QFETCH( BOBUIime, exp );
    QCOMPARE( t2, exp );
}

void tst_BOBUIime::isNull()
{
    BOBUIime t1;
    QVERIFY( t1.isNull() );
    BOBUIime t2(0,0,0);
    QVERIFY( !t2.isNull() );
    BOBUIime t3(0,0,1);
    QVERIFY( !t3.isNull() );
    BOBUIime t4(0,0,0,1);
    QVERIFY( !t4.isNull() );
    BOBUIime t5(23,59,59);
    QVERIFY( !t5.isNull() );
}

void tst_BOBUIime::isValid()
{
    BOBUIime t1;
    QVERIFY( !t1.isValid() );
    BOBUIime t2(24,0,0,0);
    QVERIFY( !t2.isValid() );
    BOBUIime t3(23,60,0,0);
    QVERIFY( !t3.isValid() );
    BOBUIime t4(23,0,-1,0);
    QVERIFY( !t4.isValid() );
    BOBUIime t5(23,0,60,0);
    QVERIFY( !t5.isValid() );
    BOBUIime t6(23,0,0,1000);
    QVERIFY( !t6.isValid() );
}

void tst_BOBUIime::hour_data()
{
    BOBUIest::addColumn<int>("hour");
    BOBUIest::addColumn<int>("minute");
    BOBUIest::addColumn<int>("sec");
    BOBUIest::addColumn<int>("msec");

    BOBUIest::newRow(  "data0" ) << 0 << 0 << 0 << 0;
    BOBUIest::newRow(  "data1" ) << 0 << 0 << 0 << 1;
    BOBUIest::newRow(  "data2" ) << 1 << 2 << 3 << 4;
    BOBUIest::newRow(  "data3" ) << 2 << 12 << 13 << 65;
    BOBUIest::newRow(  "data4" ) << 23 << 59 << 59 << 999;
    BOBUIest::newRow(  "data5" ) << -1 << -1 << -1 << -1;
}

void tst_BOBUIime::hour()
{
    QFETCH( int, hour );
    QFETCH( int, minute );
    QFETCH( int, sec );
    QFETCH( int, msec );

    BOBUIime t1( hour, minute, sec, msec );
    QCOMPARE( t1.hour(), hour );
    QCOMPARE( t1.minute(), minute );
    QCOMPARE( t1.second(), sec );
    QCOMPARE( t1.msec(), msec );
}

void tst_BOBUIime::setHMS_data()
{
    BOBUIest::addColumn<int>("hour");
    BOBUIest::addColumn<int>("minute");
    BOBUIest::addColumn<int>("sec");

    BOBUIest::newRow(  "data0" ) << 0 << 0 << 0;
    BOBUIest::newRow(  "data1" ) << 1 << 2 << 3;
    BOBUIest::newRow(  "data2" ) << 0 << 59 << 0;
    BOBUIest::newRow(  "data3" ) << 0 << 59 << 59;
    BOBUIest::newRow(  "data4" ) << 23 << 0 << 0;
    BOBUIest::newRow(  "data5" ) << 23 << 59 << 0;
    BOBUIest::newRow(  "data6" ) << 23 << 59 << 59;
    BOBUIest::newRow(  "data7" ) << -1 << -1 << -1;
}

void tst_BOBUIime::setHMS()
{
    QFETCH( int, hour );
    QFETCH( int, minute );
    QFETCH( int, sec );

    BOBUIime t(3,4,5);
    t.setHMS( hour, minute, sec );
    QCOMPARE( t.hour(), hour );
    QCOMPARE( t.minute(), minute );
    QCOMPARE( t.second(), sec );
}

void tst_BOBUIime::secsTo_data()
{
    BOBUIest::addColumn<BOBUIime>("t1");
    BOBUIest::addColumn<BOBUIime>("t2");
    BOBUIest::addColumn<int>("delta");

    BOBUIest::newRow(  "data0" ) << BOBUIime(0,0,0) << BOBUIime(0,0,59) << 59;
    BOBUIest::newRow(  "data1" ) << BOBUIime(0,0,0) << BOBUIime(0,1,0) << 60;
    BOBUIest::newRow(  "data2" ) << BOBUIime(0,0,0) << BOBUIime(0,10,0) << 600;
    BOBUIest::newRow(  "data3" ) << BOBUIime(0,0,0) << BOBUIime(23,59,59) << 86399;
    BOBUIest::newRow(  "data4" ) << BOBUIime(-1, -1, -1) << BOBUIime(0, 0, 0) << 0;
    BOBUIest::newRow(  "data5" ) << BOBUIime(0, 0, 0) << BOBUIime(-1, -1, -1) << 0;
    BOBUIest::newRow(  "data6" ) << BOBUIime(-1, -1, -1) << BOBUIime(-1, -1, -1) << 0;
    BOBUIest::newRow("disregard msec (1s)") << BOBUIime(12, 30, 1, 500) << BOBUIime(12, 30, 2, 400) << 1;
    BOBUIest::newRow("disregard msec (0s)") << BOBUIime(12, 30, 1, 500) << BOBUIime(12, 30, 1, 900) << 0;
    BOBUIest::newRow("disregard msec (-1s)") << BOBUIime(12, 30, 2, 400) << BOBUIime(12, 30, 1, 500) << -1;
    BOBUIest::newRow("disregard msec (-0s)") << BOBUIime(12, 30, 1, 900) << BOBUIime(12, 30, 1, 500) << 0;
}

void tst_BOBUIime::secsTo()
{
    QFETCH( BOBUIime, t1 );
    QFETCH( BOBUIime, t2 );
    QFETCH( int, delta );

    QCOMPARE( t1.secsTo( t2 ), delta );
}

void tst_BOBUIime::msecsTo_data()
{
    BOBUIest::addColumn<BOBUIime>("t1");
    BOBUIest::addColumn<BOBUIime>("t2");
    BOBUIest::addColumn<int>("delta");

    BOBUIest::newRow(  "data0" ) << BOBUIime(0,0,0,0) << BOBUIime(0,0,0,0) << 0;
    BOBUIest::newRow(  "data1" ) << BOBUIime(0,0,0,0) << BOBUIime(0,0,1,0) << 1000;
    BOBUIest::newRow(  "data2" ) << BOBUIime(0,0,0,0) << BOBUIime(0,0,10,0) << 10000;
    BOBUIest::newRow(  "data3" ) << BOBUIime(0,0,0,0) << BOBUIime(23,59,59,0) << 86399000;
    BOBUIest::newRow(  "data4" ) << BOBUIime(-1, -1, -1, -1) << BOBUIime(0, 0, 0, 0) << 0;
    BOBUIest::newRow(  "data5" ) << BOBUIime(0, 0, 0, 0) << BOBUIime(-1, -1, -1, -1) << 0;
    BOBUIest::newRow(  "data6" ) << BOBUIime(-1, -1, -1, -1) << BOBUIime(-1, -1, -1, -1) << 0;
}

void tst_BOBUIime::msecsTo()
{
    QFETCH( BOBUIime, t1 );
    QFETCH( BOBUIime, t2 );
    QFETCH( int, delta );

    QCOMPARE( t1.msecsTo( t2 ), delta );
}

void tst_BOBUIime::orderingCompiles()
{
    BOBUIestPrivate::testAllComparisonOperatorsCompile<BOBUIime>();
}

void tst_BOBUIime::operator_eq_eq_data()
{
    BOBUIest::addColumn<BOBUIime>("t1");
    BOBUIest::addColumn<BOBUIime>("t2");
    BOBUIest::addColumn<bool>("expectEqual");

    BOBUIime time1(0, 0, 0, 0);
    BOBUIime time2 = time1.addMSecs(1);
    BOBUIime time3 = time1.addMSecs(-1);
    BOBUIime time4(23, 59, 59, 999);

    BOBUIest::newRow("data0") << time1 << time2 << false;
    BOBUIest::newRow("data1") << time2 << time3 << false;
    BOBUIest::newRow("data2") << time4 << time1 << false;
    BOBUIest::newRow("data3") << time1 << time1 << true;
    BOBUIest::newRow("data4") << BOBUIime(12,34,56,20) << BOBUIime(12,34,56,20) << true;
    BOBUIest::newRow("data5") << BOBUIime(01,34,56,20) << BOBUIime(13,34,56,20) << false;
}

void tst_BOBUIime::operator_eq_eq()
{
    QFETCH(BOBUIime, t1);
    QFETCH(BOBUIime, t2);
    QFETCH(bool, expectEqual);

    BOBUI_TEST_EQUALITY_OPS(t1, t2, expectEqual);

    if (expectEqual)
        QVERIFY(qHash(t1) == qHash(t2));
}

void tst_BOBUIime::ordering_data()
{
    BOBUIest::addColumn<BOBUIime>("left");
    BOBUIest::addColumn<BOBUIime>("right");
    BOBUIest::addColumn<BobUI::strong_ordering>("expectedOrdering");

    auto generateRow = [](BOBUIime t1, BOBUIime t2, BobUI::strong_ordering ordering) {
        const QByteArray t1Str = t1.toString(u"hh:mm:ss.zz").toLatin1();
        const QByteArray t2Str = t2.toString(u"hh:mm:ss.zz").toLatin1();
        BOBUIest::addRow("%s_vs_%s", t1Str.constData(), t2Str.constData()) << t1 << t2 << ordering;
    };

    generateRow(BOBUIime(0, 0), BOBUIime(0, 0), BobUI::strong_ordering::equivalent);
    generateRow(BOBUIime(12, 34, 56, 20), BOBUIime(12, 34, 56, 30), BobUI::strong_ordering::less);
    generateRow(BOBUIime(13, 34, 46, 20), BOBUIime(13, 34, 56, 20), BobUI::strong_ordering::less);
    generateRow(BOBUIime(13, 24, 56, 20), BOBUIime(13, 34, 56, 20), BobUI::strong_ordering::less);
    generateRow(BOBUIime(12, 34, 56, 20), BOBUIime(13, 34, 56, 20), BobUI::strong_ordering::less);
    generateRow(BOBUIime(14, 34, 56, 20), BOBUIime(13, 34, 56, 20), BobUI::strong_ordering::greater);
    generateRow(BOBUIime(13, 44, 56, 20), BOBUIime(13, 34, 56, 20), BobUI::strong_ordering::greater);
    generateRow(BOBUIime(13, 34, 56, 20), BOBUIime(13, 34, 46, 20), BobUI::strong_ordering::greater);
    generateRow(BOBUIime(13, 34, 56, 30), BOBUIime(13, 34, 56, 20), BobUI::strong_ordering::greater);
}

void tst_BOBUIime::ordering()
{
    QFETCH(BOBUIime, left);
    QFETCH(BOBUIime, right);
    QFETCH(BobUI::strong_ordering, expectedOrdering);

    BOBUI_TEST_ALL_COMPARISON_OPS(left, right, expectedOrdering);
}

#if BOBUI_CONFIG(datestring)
# if BOBUI_CONFIG(datetimeparser)
void tst_BOBUIime::fromStringFormat_data()
{
    BOBUIest::addColumn<QString>("string");
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<BOBUIime>("expected");

    BOBUIest::newRow("data0") << u"1010"_s << u"mmm"_s << BOBUIime(0, 10, 0);
    BOBUIest::newRow("data1") << u"00"_s << u"hm"_s << invalidTime();
    BOBUIest::newRow("data2") << u"10am"_s << u"hap"_s << BOBUIime(10, 0, 0);
    BOBUIest::newRow("data3") << u"10pm"_s << u"hap"_s << BOBUIime(22, 0, 0);
    BOBUIest::newRow("data4") << u"10pmam"_s << u"hapap"_s << invalidTime();
    BOBUIest::newRow("data5") << u"1070"_s << u"hhm"_s << invalidTime();
    BOBUIest::newRow("data6") << u"1011"_s << u"hh"_s << invalidTime();
    BOBUIest::newRow("data7") << u"25"_s << u"hh"_s << invalidTime();
    BOBUIest::newRow("data8") << u"22pm"_s << u"Hap"_s << BOBUIime(22, 0, 0);
    BOBUIest::newRow("data9") << u"2221"_s << u"hhhh"_s << invalidTime();
    // Parsing of am/pm indicators is case-insensitive
    BOBUIest::newRow("pm-upper") << u"02:23PM"_s << u"hh:mmAp"_s << BOBUIime(14, 23);
    BOBUIest::newRow("pm-lower") << u"02:23pm"_s << u"hh:mmaP"_s << BOBUIime(14, 23);
    BOBUIest::newRow("pm-as-upper") << u"02:23Pm"_s << u"hh:mmAP"_s << BOBUIime(14, 23);
    BOBUIest::newRow("pm-as-lower") << u"02:23pM"_s << u"hh:mmap"_s << BOBUIime(14, 23);
    // Millisecond parsing must interpolate 0s only at the end and notice them at the start.
    BOBUIest::newRow("short-msecs-lt100")
        << u"10:12:34:045"_s << u"hh:m:ss:z"_s << BOBUIime(10, 12, 34, 45);
    BOBUIest::newRow("short-msecs-gt100")
        << u"10:12:34:45"_s << u"hh:m:ss:z"_s << BOBUIime(10, 12, 34, 450);
    BOBUIest::newRow("late")
        << u"23:59:59.999"_s << u"hh:mm:ss.z"_s << BOBUIime(23, 59, 59, 999);

    // Test unicode handling.
    BOBUIest::newRow("emoji in format string 1")
        << u"12👍31:25.05"_s << u"hh👍mm:ss.z"_s << BOBUIime(12, 31, 25, 50);
    BOBUIest::newRow("emoji in format string 2")
        << u"💖12👍31🌈25😺05🚀"_s << u"💖hh👍mm🌈ss😺z🚀"_s << BOBUIime(12, 31, 25, 50);
}

void tst_BOBUIime::fromStringFormat()
{
    QFETCH(QString, string);
    QFETCH(QString, format);
    QFETCH(BOBUIime, expected);

    BOBUIime dt = BOBUIime::fromString(string, format);
    QCOMPARE(dt, expected);
}
# endif // datetimeparser

void tst_BOBUIime::fromStringDateFormat_data()
{
    BOBUIest::addColumn<QString>("string");
    BOBUIest::addColumn<BobUI::DateFormat>("format");
    BOBUIest::addColumn<BOBUIime>("expected");

    BOBUIest::newRow("TextDate - zero") << u"00:00:00"_s << BobUI::TextDate << BOBUIime(0, 0);
    BOBUIest::newRow("TextDate - ordinary")
        << u"10:12:34"_s << BobUI::TextDate << BOBUIime(10, 12, 34);
    BOBUIest::newRow("TextDate - milli-max")
        << u"19:03:54.998601"_s << BobUI::TextDate << BOBUIime(19, 3, 54, 999);
    BOBUIest::newRow("TextDate - milli-wrap")
        << u"19:03:54.999601"_s << BobUI::TextDate << BOBUIime(19, 3, 55);
    BOBUIest::newRow("TextDate - no-secs")
        << u"10:12"_s << BobUI::TextDate << BOBUIime(10, 12);
    BOBUIest::newRow("TextDate - midnight-nowrap")
        << u"23:59:59.9999"_s << BobUI::TextDate << BOBUIime(23, 59, 59, 999);
    BOBUIest::newRow("TextDate - invalid, minutes") << u"23:XX:00"_s << BobUI::TextDate << invalidTime();
    BOBUIest::newRow("TextDate - invalid, minute fraction") << u"23:00.123456"_s << BobUI::TextDate << invalidTime();
    BOBUIest::newRow("TextDate - invalid, seconds") << u"23:00:XX"_s << BobUI::TextDate << invalidTime();
    BOBUIest::newRow("TextDate - invalid, milliseconds") << u"23:01:01:XXXX"_s << BobUI::TextDate
        << invalidTime();
    BOBUIest::newRow("TextDate - midnight 24") << u"24:00:00"_s << BobUI::TextDate << BOBUIime();

    BOBUIest::newRow("IsoDate - valid, start of day, omit seconds") << u"00:00"_s << BobUI::ISODate << BOBUIime(0, 0, 0);
    BOBUIest::newRow("IsoDate - valid, omit seconds") << u"22:21"_s << BobUI::ISODate << BOBUIime(22, 21, 0);
    BOBUIest::newRow("IsoDate - minute fraction") // 60 * 0.816666 = 48.99996 should round up:
        << u"22:21.816666"_s << BobUI::ISODate << BOBUIime(22, 21, 49);
    BOBUIest::newRow("IsoDate - valid, omit seconds (2)") << u"23:59"_s << BobUI::ISODate << BOBUIime(23, 59, 0);
    BOBUIest::newRow("IsoDate - valid, end of day") << u"23:59:59"_s << BobUI::ISODate << BOBUIime(23, 59, 59);

    BOBUIest::newRow("IsoDate - invalid, empty string") << u""_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, too many hours") << u"25:00"_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, too many minutes") << u"10:70"_s << BobUI::ISODate << invalidTime();
    // This is a valid time if it happens on June 30 or December 31 (leap seconds).
    BOBUIest::newRow("IsoDate - invalid, too many seconds") << u"23:59:60"_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, minutes") << u"23:XX:00"_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, not enough minutes") << u"23:0"_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, minute fraction") << u"23:00,XX"_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, seconds") << u"23:00:XX"_s << BobUI::ISODate << invalidTime();
    BOBUIest::newRow("IsoDate - invalid, milliseconds") << u"23:01:01:XXXX"_s << BobUI::ISODate
        << invalidTime();
    BOBUIest::newRow("IsoDate - zero") << u"00:00:00"_s << BobUI::ISODate << BOBUIime(0, 0);
    BOBUIest::newRow("IsoDate - ordinary") << u"10:12:34"_s << BobUI::ISODate << BOBUIime(10, 12, 34);
    BOBUIest::newRow("IsoDate - milli-max")
        << u"19:03:54.998601"_s << BobUI::ISODate << BOBUIime(19, 3, 54, 999);
    BOBUIest::newRow("IsoDate - milli-wrap")
        << u"19:03:54.999601"_s << BobUI::ISODate << BOBUIime(19, 3, 55);
    BOBUIest::newRow("IsoDate - midnight-nowrap")
        << u"23:59:59.9999"_s << BobUI::ISODate << BOBUIime(23, 59, 59, 999);
    BOBUIest::newRow("IsoDate - midnight 24")
        << u"24:00:00"_s << BobUI::ISODate << BOBUIime(0, 0);
    BOBUIest::newRow("IsoDate - minute fraction midnight")
        << u"24:00,0"_s << BobUI::ISODate << BOBUIime(0, 0);

    // Test BobUI::RFC2822Date format (RFC 2822).
    BOBUIest::newRow("RFC 2822") << u"13 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    BOBUIest::newRow("RFC 2822 after space")
        << u" 13 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    BOBUIest::newRow("RFC 2822 with day") << u"Thu, 01 Jan 1970 00:12:34 +0000"_s
        << BobUI::RFC2822Date << BOBUIime(0, 12, 34);
    BOBUIest::newRow("RFC 2822 with day after space")
        << u" Thu, 01 Jan 1970 00:12:34 +0000"_s
        << BobUI::RFC2822Date << BOBUIime(0, 12, 34);
    // No timezone
    BOBUIest::newRow("RFC 2822 no timezone") << u"01 Jan 1970 00:12:34"_s
        << BobUI::RFC2822Date << BOBUIime(0, 12, 34);
    // No time specified
    BOBUIest::newRow("RFC 2822 date only") << u"01 Nov 2002"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 2822 with day date only") << u"Fri, 01 Nov 2002"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 2822 malformed time")
        << u"01 Nov 2002 0:"_s << BobUI::RFC2822Date << BOBUIime();
    // Test invalid month, day, year are ignored:
    BOBUIest::newRow("RFC 2822 invalid month name") << u"13 Fev 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    BOBUIest::newRow("RFC 2822 invalid day") << u"36 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    BOBUIest::newRow("RFC 2822 invalid day name") << u"Mud, 23 Feb 1987 13:24:51 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    BOBUIest::newRow("RFC 2822 invalid year") << u"13 Feb 0000 13:24:51 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    // Test invalid characters:
    BOBUIest::newRow("RFC 2822 invalid character at end")
        << u"01 Jan 2012 08:00:00 +0100!"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 2822 invalid character at front")
        << u"!01 Jan 2012 08:00:00 +0100"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 2822 invalid character both ends")
        << u"!01 Jan 2012 08:00:00 +0100!"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 2822 invalid character at front, 2 at back")
        << u"!01 Jan 2012 08:00:00 +0100.."_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 2822 invalid character 2 at front")
        << u"!!01 Jan 2012 08:00:00 +0100"_s
        << BobUI::RFC2822Date << invalidTime();
    // The common date text used by the "invalid character" tests, just to be
    // sure *it's* not what's invalid:
    BOBUIest::newRow("RFC 2822 (not invalid)")
        << u"01 Jan 2012 08:00:00 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(8, 0, 0);

    // Test BobUI::RFC2822Date format (RFC 850 and 1036, permissive).
    BOBUIest::newRow("RFC 850 and 1036") << u"Fri Feb 13 13:24:51 1987 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    BOBUIest::newRow("RFC 850 and 1036 after space")
        << u" Fri Feb 13 13:24:51 1987 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(13, 24, 51);
    // No timezone
    BOBUIest::newRow("RFC 850 and 1036 no timezone") << u"Thu Jan 01 00:12:34 1970"_s
        << BobUI::RFC2822Date << BOBUIime(0, 12, 34);
    // No time specified
    BOBUIest::newRow("RFC 850 and 1036 date only") << u"Fri Nov 01 2002"_s
        << BobUI::RFC2822Date << invalidTime();
    // Test invalid characters.
    BOBUIest::newRow("RFC 850 and 1036 invalid character at end")
        << u"Sun Jan 01 08:00:00 2012 +0100!"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character at front")
        << u"!Sun Jan 01 08:00:00 2012 +0100"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character both ends")
        << u"!Sun Jan 01 08:00:00 2012 +0100!"_s
        << BobUI::RFC2822Date << invalidTime();
    BOBUIest::newRow("RFC 850 and 1036 invalid character at front, 2 at back")
        << u"!Sun Jan 01 08:00:00 2012 +0100.."_s
        << BobUI::RFC2822Date << invalidTime();
    // The common date text used by the "invalid character" tests, just to be
    // sure *it's* not what's invalid:
    BOBUIest::newRow("RFC 850 and 1036 no invalid character")
        << u"Sun Jan 01 08:00:00 2012 +0100"_s
        << BobUI::RFC2822Date << BOBUIime(8, 0, 0);

    BOBUIest::newRow("RFC empty") << u""_s << BobUI::RFC2822Date << invalidTime();
}

void tst_BOBUIime::fromStringDateFormat()
{
    QFETCH(QString, string);
    QFETCH(BobUI::DateFormat, format);
    QFETCH(BOBUIime, expected);

    BOBUIime dt = BOBUIime::fromString(string, format);
    QCOMPARE(dt, expected);
}

void tst_BOBUIime::toStringDateFormat_data()
{
    BOBUIest::addColumn<BOBUIime>("time");
    BOBUIest::addColumn<BobUI::DateFormat>("format");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("00:00:00.000") << BOBUIime(0, 0, 0, 0) << BobUI::TextDate << u"00:00:00"_s;
    BOBUIest::newRow("ISO 00:00:00.000") << BOBUIime(0, 0, 0, 0) << BobUI::ISODate << u"00:00:00"_s;
    BOBUIest::newRow("Text 10:12:34.000") << BOBUIime(10, 12, 34, 0) << BobUI::TextDate << u"10:12:34"_s;
    BOBUIest::newRow("ISO 10:12:34.000") << BOBUIime(10, 12, 34, 0) << BobUI::ISODate << u"10:12:34"_s;
    BOBUIest::newRow("Text 10:12:34.001") << BOBUIime(10, 12, 34, 001) << BobUI::TextDate << u"10:12:34"_s;
    BOBUIest::newRow("ISO 10:12:34.001") << BOBUIime(10, 12, 34, 001) << BobUI::ISODate << u"10:12:34"_s;
    BOBUIest::newRow("Text 10:12:34.999") << BOBUIime(10, 12, 34, 999) << BobUI::TextDate << u"10:12:34"_s;
    BOBUIest::newRow("ISO 10:12:34.999") << BOBUIime(10, 12, 34, 999) << BobUI::ISODate << u"10:12:34"_s;
    BOBUIest::newRow("RFC2822Date") << BOBUIime(10, 12, 34, 999) << BobUI::RFC2822Date << u"10:12:34"_s;
    BOBUIest::newRow("ISOWithMs 10:12:34.000") << BOBUIime(10, 12, 34, 0) << BobUI::ISODateWithMs << u"10:12:34.000"_s;
    BOBUIest::newRow("ISOWithMs 10:12:34.020") << BOBUIime(10, 12, 34, 20) << BobUI::ISODateWithMs << u"10:12:34.020"_s;
    BOBUIest::newRow("ISOWithMs 10:12:34.999") << BOBUIime(10, 12, 34, 999) << BobUI::ISODateWithMs << u"10:12:34.999"_s;
}

void tst_BOBUIime::toStringDateFormat()
{
    QFETCH(BOBUIime, time);
    QFETCH(BobUI::DateFormat, format);
    QFETCH(QString, expected);

    QCOMPARE(time.toString(format), expected);
}

void tst_BOBUIime::toStringFormat_data()
{
    BOBUIest::addColumn<BOBUIime>("t");
    BOBUIest::addColumn<QString>("format");
    BOBUIest::addColumn<QString>("str");

    BOBUIest::newRow( "midnight" ) << BOBUIime(0,0,0,0) << u"h:m:s:z"_s << u"0:0:0:0"_s;
    BOBUIest::newRow( "full" ) << BOBUIime(10,12,34,53) << u"hh:mm:ss:zzz"_s << u"10:12:34:053"_s;
    BOBUIest::newRow( "short-msecs-lt100" ) << BOBUIime(10,12,34,45) << u"hh:m:ss:z"_s << u"10:12:34:045"_s;
    BOBUIest::newRow( "short-msecs-gt100" ) << BOBUIime(10,12,34,450) << u"hh:m:ss:z"_s << u"10:12:34:45"_s;
    BOBUIest::newRow( "am-pm" ) << BOBUIime(10,12,34,45) << u"hh:ss ap"_s << u"10:34 am"_s;
    BOBUIest::newRow( "AM-PM" ) << BOBUIime(22,12,34,45) << u"hh:zzz AP"_s << u"10:045 PM"_s;
    BOBUIest::newRow( "invalid" ) << BOBUIime(230,230,230,230) << u"hh:mm:ss"_s << QString();
    BOBUIest::newRow( "empty format" ) << BOBUIime(4,5,6,6) << u""_s << u""_s;
}

void tst_BOBUIime::toStringFormat()
{
    QFETCH( BOBUIime, t );
    QFETCH( QString, format );
    QFETCH( QString, str );

    QCOMPARE( t.toString( format ), str );
}
#endif // datestring

void tst_BOBUIime::msecsSinceStartOfDay_data()
{
    BOBUIest::addColumn<int>("msecs");
    BOBUIest::addColumn<bool>("isValid");
    BOBUIest::addColumn<int>("hour");
    BOBUIest::addColumn<int>("minute");
    BOBUIest::addColumn<int>("second");
    BOBUIest::addColumn<int>("msec");

    BOBUIest::newRow("00:00:00.000") << 0 << true
                                  << 0 << 0 << 0 << 0;
    BOBUIest::newRow("01:00:00.001") << ((1 * 3600 * 1000) + 1) << true
                                  << 1 << 0 << 0 << 1;
    BOBUIest::newRow("03:04:05.678") << ((3 * 3600 + 4 * 60 + 5) * 1000 + 678) << true
                                  << 3 << 4 << 5 << 678;
    BOBUIest::newRow("23:59:59.999") << ((23 * 3600 + 59 * 60 + 59) * 1000 + 999) << true
                                  << 23 << 59 << 59 << 999;
    BOBUIest::newRow("24:00:00.000") << ((24 * 3600) * 1000) << false
                                  << -1 << -1 << -1 << -1;
    BOBUIest::newRow("-1 invalid")   << -1 << false
                                  << -1 << -1 << -1 << -1;
}

void tst_BOBUIime::msecsSinceStartOfDay()
{
    QFETCH(int, msecs);
    QFETCH(bool, isValid);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);
    QFETCH(int, msec);

    BOBUIime time = BOBUIime::fromMSecsSinceStartOfDay(msecs);
    QCOMPARE(time.isValid(), isValid);
    QCOMPARE(time.msecsSinceStartOfDay(), msecs < 0 ? 0 : msecs);
    QCOMPARE(time.hour(), hour);
    QCOMPARE(time.minute(), minute);
    QCOMPARE(time.second(), second);
    QCOMPARE(time.msec(), msec);
}

BOBUIEST_APPLESS_MAIN(tst_BOBUIime)
#include "tst_bobuiime.moc"
