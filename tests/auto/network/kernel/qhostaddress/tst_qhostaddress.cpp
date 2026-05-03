// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qhostaddress.h>
#include <private/qhostaddress_p.h>
#include <qcoreapplication.h>
#include <BOBUIest>
#include <qplatformdefs.h>
#include <qdebug.h>
#include <qhash.h>
#include <qbytearray.h>
#include <qdatastream.h>
#ifdef Q_OS_WIN
#  include <bobui_windows.h>
#endif

#if defined(Q_OS_ANDROID) || defined(Q_OS_WASM) || defined(Q_OS_VXWORKS)
#  include <netinet/in.h>
#endif

Q_DECLARE_METATYPE(AddressClassification)
Q_DECLARE_METATYPE(QHostAddress::SpecialAddress)

class tst_QHostAddress : public QObject
{
    Q_OBJECT

public:
    tst_QHostAddress();

private slots:
    void constructor();
    void constructor_QString_data();
    void constructor_QString();
    void setAddress_QString_data();
    void setAddress_QString();
    void specialAddresses_data();
    void specialAddresses();
    void compare_data();
    void compare();
    void isEqual_data();
    void isEqual();
    void assignment();
    void scopeId();
    void hashKey();
    void streaming_data();
    void streaming();
    void parseSubnet_data();
    void parseSubnet();
    void isInSubnet_data();
    void isInSubnet();
    void classification_data();
    void classification();
    void convertv4v6_data();
    void convertv4v6();
};

Q_DECLARE_METATYPE(QHostAddress)

tst_QHostAddress::tst_QHostAddress()
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
}

static void verifyClear(const QHostAddress &addr)
{
    QVERIFY(addr.isNull());
    QVERIFY(!addr.isLoopback());
    QVERIFY(!addr.isGlobal());
    QVERIFY(!addr.isLinkLocal());
    QVERIFY(!addr.isSiteLocal());
    QVERIFY(!addr.isUniqueLocalUnicast());
    QVERIFY(!addr.isMulticast());
    QVERIFY(!addr.isBroadcast());
    QVERIFY(!addr.isPrivateUse());
    QCOMPARE(addr, QHostAddress());
    QCOMPARE(addr, QHostAddress::Null);
    QCOMPARE(addr.protocol(), QHostAddress::UnknownNetworkLayerProtocol);

    bool ok = true;
    QCOMPARE(addr.toIPv4Address(&ok), 0);
    QVERIFY(!ok);

    QCOMPARE(QByteArrayView::fromArray(addr.toIPv6Address().c),
             QByteArrayView::fromArray(QIPv6Address{}.c));
    QCOMPARE(addr.scopeId(), QString());

    QCOMPARE(addr.toString(), QString());

    size_t seed = QHashSeed::globalSeed();
    QCOMPARE(qHash(addr, seed), qHash(QHostAddress(), seed));
    seed = 0;
    QCOMPARE(qHash(addr, seed), qHash(QHostAddress(), seed));
}

void tst_QHostAddress::constructor()
{
    QHostAddress addr;
    verifyClear(addr);
}

void tst_QHostAddress::constructor_QString_data()
{
    setAddress_QString_data();
}

void tst_QHostAddress::constructor_QString()
{
    QFETCH(QString, address);
    QFETCH(bool, ok);
    QFETCH(int, protocol);

    QHostAddress hostAddr(address);

    if (address == "0.0.0.0" || address == "::") {
        QVERIFY(ok);
    } else {
        QVERIFY(hostAddr.isNull() != ok);
    }

    if (ok)
        BOBUIEST(hostAddr.toString(), "resAddr");

    if ( protocol == 4 ) {
        QVERIFY( hostAddr.protocol() == QHostAddress::IPv4Protocol || hostAddr.protocol() == QHostAddress::UnknownNetworkLayerProtocol );
        QVERIFY( hostAddr.protocol() != QHostAddress::IPv6Protocol );
    } else if ( protocol == 6 ) {
        QVERIFY( hostAddr.protocol() != QHostAddress::IPv4Protocol && hostAddr.protocol() != QHostAddress::UnknownNetworkLayerProtocol );
        QVERIFY( hostAddr.protocol() == QHostAddress::IPv6Protocol );
    } else {
        QVERIFY( hostAddr.isNull() );
        QVERIFY( hostAddr.protocol() == QHostAddress::UnknownNetworkLayerProtocol );
    }

    hostAddr.clear();
    verifyClear(hostAddr);
}

void tst_QHostAddress::setAddress_QString_data()
{
    BOBUIest::addColumn<QString>("address");
    BOBUIest::addColumn<bool>("ok");
    BOBUIest::addColumn<QString>("resAddr");
    BOBUIest::addColumn<int>("protocol"); // 4: IPv4, 6: IPv6, other: undefined

    //next we fill it with data
    BOBUIest::newRow("ip4_00")  << QString("127.0.0.1") << true << QString("127.0.0.1") << 4;
    BOBUIest::newRow("ip4_01")  << QString("255.3.2.1") << true << QString("255.3.2.1") << 4;
    BOBUIest::newRow("ip4_03")  << QString(" 255.3.2.1") << true << QString("255.3.2.1") << 4;
    BOBUIest::newRow("ip4_04")  << QString("255.3.2.1\r ") << true << QString("255.3.2.1") << 4;
    BOBUIest::newRow("ip4_05")  << QString("0.0.0.0") << true << QString("0.0.0.0") << 4;
    BOBUIest::newRow("ip4_06")  << QString("123.0.0") << true << QString("123.0.0.0") << 4;

    // for the format of IPv6 addresses see also RFC 5952
    // rule 4.1: Leading zeros MUST be suppressed
    // rule 4.2.1: Shorten as Much as Possible
    // rule 4.2.2: The symbol "::" MUST NOT be used to shorten just one 16-bit 0 field.
    // rule 4.2.3: the longest run of consecutive 16-bit 0 fields MUST be shortened
    //             When the length of the consecutive 16-bit 0 fields, the first sequence
    //             of zero bits MUST be shortened
    // rule 4.3: The characters "a", "b", "c", "d", "e", and "f" in an IPv6 address
    //           MUST be represented in lowercase
    BOBUIest::newRow("ip6_00")  << QString("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210") << true << QString("fedc:ba98:7654:3210:fedc:ba98:7654:3210") << 6; // 4.3
    BOBUIest::newRow("ip6_01")  << QString("1080:0000:0000:0000:0008:0800:200C:417A") << true << QString("1080::8:800:200c:417a") << 6; // 4.1, 4.2.1
    BOBUIest::newRow("ip6_02")  << QString("1080:0:0:0:8:800:200C:417A") << true << QString("1080::8:800:200c:417a") << 6;
    BOBUIest::newRow("ip6_03")  << QString("1080::8:800:200C:417A") << true << QString("1080::8:800:200c:417a") << 6;
    BOBUIest::newRow("ip6_04")  << QString("FF01::43") << true << QString("ff01::43") << 6;
    BOBUIest::newRow("ip6_05")  << QString("::1") << true << QString("::1") << 6;
    BOBUIest::newRow("ip6_06")  << QString("1::") << true << QString("1::") << 6;
    BOBUIest::newRow("ip6_07")  << QString("::") << true << QString("::") << 6;
    BOBUIest::newRow("ip6_08")  << QString("0:0:0:0:0:0:13.1.68.3") << true << QString("::13.1.68.3") << 6;
    BOBUIest::newRow("ip6_09")  << QString("::13.1.68.3") << true <<  QString("::13.1.68.3") << 6;
    BOBUIest::newRow("ip6_10")  << QString("0:0:0:0:0:FFFF:129.144.52.38") << true << QString("::ffff:129.144.52.38") << 6;
    BOBUIest::newRow("ip6_11")  << QString("::FFFF:129.144.52.38") << true << QString("::ffff:129.144.52.38") << 6;
    BOBUIest::newRow("ip6_12")  << QString("1::FFFF:129.144.52.38") << true << QString("1::ffff:8190:3426") << 6;
    BOBUIest::newRow("ip6_13")  << QString("A:B::D:E") << true << QString("a:b::d:e") << 6;
    BOBUIest::newRow("ip6_14")  << QString("1080:0:1:0:8:800:200C:417A") << true << QString("1080:0:1:0:8:800:200c:417a") << 6; // 4.2.2
    BOBUIest::newRow("ip6_15")  << QString("1080:0:1:0:8:800:200C:0") << true << QString("1080:0:1:0:8:800:200c:0") << 6;
    BOBUIest::newRow("ip6_16")  << QString("1080:0:1:0:8:800:0:0") << true << QString("1080:0:1:0:8:800::") << 6;
    BOBUIest::newRow("ip6_17a") << QString("1080:0:0:8:800:0:0:0") << true << QString("1080:0:0:8:800::") << 6; // 4.2.3a
    BOBUIest::newRow("ip6_17b") << QString("1080:0:0:0:8:0:0:0") << true << QString("1080::8:0:0:0") << 6; // 4.2.3b
    BOBUIest::newRow("ip6_18")  << QString("0:1:1:1:8:800:0:0") << true << QString("0:1:1:1:8:800::") << 6;
    BOBUIest::newRow("ip6_19")  << QString("0:1:1:1:8:800:0:1") << true << QString("0:1:1:1:8:800:0:1") << 6;

    BOBUIest::newRow("error_00")  << QString("foobarcom") << false << QString() << 0;
    BOBUIest::newRow("error_01")  << QString("foo.bar.com") << false << QString() << 0;
    BOBUIest::newRow("error_02")  << QString("") << false << QString() << 0;
    BOBUIest::newRow("error_03")  << QString() << false << QString() << 0;
    BOBUIest::newRow("error_04")  << QString(" \t\r") << false << QString() << 0;

    BOBUIest::newRow("error_ip4_00")  << QString("256.9.9.9") << false << QString() << 0;
    BOBUIest::newRow("error_ip4_01")  << QString("-1.9.9.9") << false << QString() << 0;
    //BOBUIest::newRow("error_ip4_02")  << QString("123.0.0") << false << QString() << 0; // no longer invalid in BobUI5
    BOBUIest::newRow("error_ip4_02")  << QString("123.0.0.") << false << QString() << 0;
    BOBUIest::newRow("error_ip4_03")  << QString("123.0.0.0.0") << false << QString() << 0;
    BOBUIest::newRow("error_ip4_04")  << QString("255.2 3.2.1") << false << QString() << 0;

    BOBUIest::newRow("error_ip6_00")  << QString(":") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_01")  << QString(":::") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_02")  << QString("::AAAA:") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_03")  << QString(":AAAA::") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_04")  << QString("FFFF:::129.144.52.38") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_05")  << QString("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210:1234") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_06")  << QString("129.144.52.38::") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_07")  << QString("::129.144.52.38:129.144.52.38") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_08")  << QString(":::129.144.52.38") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_09")  << QString("1FEDC:BA98:7654:3210:FEDC:BA98:7654:3210") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_10")  << QString("::FFFFFFFF") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_11")  << QString("::EFGH") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_12")  << QString("ABCD:ABCD:ABCD") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_13")  << QString("::ABCD:ABCD::") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_14")  << QString("1::2::3") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_15")  << QString("1:2:::") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_16")  << QString(":::1:2") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_17")  << QString("1:::2") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_18")  << QString("FEDC::7654:3210:FEDC:BA98::3210") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_19")  << QString("ABCD:ABCD:ABCD:1.2.3.4") << false << QString() << 0;
    BOBUIest::newRow("error_ip6_20")  << QString("ABCD::ABCD::ABCD:1.2.3.4") << false << QString() << 0;

}

void tst_QHostAddress::setAddress_QString()
{
    QFETCH(QString, address);
    QFETCH(bool, ok);
    QFETCH(int, protocol);

    QHostAddress hostAddr;
    QCOMPARE(hostAddr.setAddress(address), ok);

    if (ok)
        BOBUIEST(hostAddr.toString(), "resAddr");

    if ( protocol == 4 ) {
        QVERIFY( hostAddr.protocol() == QHostAddress::IPv4Protocol || hostAddr.protocol() == QHostAddress::UnknownNetworkLayerProtocol );
        QVERIFY( hostAddr.protocol() != QHostAddress::IPv6Protocol );
    } else if ( protocol == 6 ) {
        QVERIFY( hostAddr.protocol() != QHostAddress::IPv4Protocol && hostAddr.protocol() != QHostAddress::UnknownNetworkLayerProtocol );
        QVERIFY( hostAddr.protocol() == QHostAddress::IPv6Protocol );
    } else {
        QVERIFY( hostAddr.isNull() );
        QVERIFY( hostAddr.protocol() == QHostAddress::UnknownNetworkLayerProtocol );
    }

    hostAddr.clear();
    verifyClear(hostAddr);
}

void tst_QHostAddress::specialAddresses_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QHostAddress::SpecialAddress>("address");
    BOBUIest::addColumn<bool>("result");

    BOBUIest::newRow("localhost_1") << QString("127.0.0.1") << QHostAddress::LocalHost << true;
    BOBUIest::newRow("localhost_2") << QString("127.0.0.2") << QHostAddress::LocalHost << false;
    BOBUIest::newRow("localhost_3") << QString("127.0.0.2") << QHostAddress::LocalHostIPv6 << false;

    BOBUIest::newRow("localhost_ipv6_4") << QString("::1") << QHostAddress::LocalHostIPv6 << true;
    BOBUIest::newRow("localhost_ipv6_5") << QString("::2") << QHostAddress::LocalHostIPv6 << false;
    BOBUIest::newRow("localhost_ipv6_6") << QString("::1") << QHostAddress::LocalHost << false;

    BOBUIest::newRow("null_1") << QString("") << QHostAddress::Null << true;
    BOBUIest::newRow("null_2") << QString("bjarne") << QHostAddress::Null << true;

    BOBUIest::newRow("compare_from_null") << QString("") << QHostAddress::Broadcast << false;

    BOBUIest::newRow("broadcast_1") << QString("255.255.255.255") << QHostAddress::Any << false;
    BOBUIest::newRow("broadcast_2") << QString("255.255.255.255") << QHostAddress::Broadcast << true;

    BOBUIest::newRow("any_ipv6") << QString("::") << QHostAddress::AnyIPv6 << true;
    BOBUIest::newRow("any_ipv4") << QString("0.0.0.0") << QHostAddress::AnyIPv4 << true;

    BOBUIest::newRow("dual_not_ipv6") << QString("::") << QHostAddress::Any << false;
    BOBUIest::newRow("dual_not_ipv4") << QString("0.0.0.0") << QHostAddress::Any << false;
}


void tst_QHostAddress::specialAddresses()
{
    QFETCH(QString, text);
    QFETCH(QHostAddress::SpecialAddress, address);
    QFETCH(bool, result);
    QCOMPARE(QHostAddress(text) == address, result);
    size_t seed = QHashSeed::globalSeed();

    //check special address equal to itself (BOBUIBUG-22898), note two overloads of operator==
    QVERIFY(QHostAddress(address) == QHostAddress(address));
    QVERIFY(QHostAddress(address) == address);
    QVERIFY(address == QHostAddress(address));
    QVERIFY(!(QHostAddress(address) != QHostAddress(address)));
    QVERIFY(!(QHostAddress(address) != address));
    QVERIFY(!(address != QHostAddress(address)));

    {
        QHostAddress ha;
        ha.setAddress(address);
        QVERIFY(ha == address);
        QCOMPARE(qHash(ha, seed), qHash(QHostAddress(address), seed));
        QCOMPARE(qHash(ha, 0), qHash(QHostAddress(address), 0));
    }

    QHostAddress setter;
    setter.setAddress(text);
    if (result) {
        QCOMPARE(setter, address);
        QCOMPARE(qHash(setter, seed), qHash(QHostAddress(address), seed));
        QCOMPARE(qHash(setter, 0), qHash(QHostAddress(address), 0));
    } else {
        QCOMPARE_NE(setter, address);
    }
}


void tst_QHostAddress::compare_data()
{
    BOBUIest::addColumn<QHostAddress>("first");
    BOBUIest::addColumn<QHostAddress>("second");
    BOBUIest::addColumn<bool>("result");

    BOBUIest::newRow("1") << QHostAddress() << QHostAddress() << true;
    BOBUIest::newRow("2") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::Any) << true;
    BOBUIest::newRow("3") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv6) << true;
    BOBUIest::newRow("4") << QHostAddress(QHostAddress::Broadcast) << QHostAddress(QHostAddress::Broadcast) << true;
    BOBUIest::newRow("5") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::Broadcast) << false;
    BOBUIest::newRow("6") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHostIPv6) << false;
    BOBUIest::newRow("7") << QHostAddress() << QHostAddress(QHostAddress::LocalHostIPv6) << false;
    BOBUIest::newRow("any4-any6") << QHostAddress(QHostAddress::AnyIPv4) << QHostAddress(QHostAddress::AnyIPv6) << false;

    Q_IPV6ADDR localhostv4mapped = { { 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 255, 255,  127, 0, 0, 1 } };
    BOBUIest::newRow("v4-v4mapped") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("::ffff:127.0.0.1") << false;
    BOBUIest::newRow("v4-v4mapped-2") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(localhostv4mapped) << false;
}

void tst_QHostAddress::compare()
{
    QFETCH(QHostAddress, first);
    QFETCH(QHostAddress, second);
    QFETCH(bool, result);

    QCOMPARE(first == second, result);
    QCOMPARE(second == first, result);
    if (result == true) {
        size_t seed = QHashSeed::globalSeed();
        QCOMPARE(qHash(first, seed), qHash(second, seed));
        QCOMPARE(qHash(first, 0), qHash(second, 0));
    }
}

void tst_QHostAddress::isEqual_data()
{
    BOBUIest::addColumn<QHostAddress>("first");
    BOBUIest::addColumn<QHostAddress>("second");
    BOBUIest::addColumn<int>("flags");
    BOBUIest::addColumn<bool>("result");

    // QHostAddress::StrictConversion is already tested in compare()
    BOBUIest::newRow("localhost4to6-local") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertLocalHost << true;
    BOBUIest::newRow("localhost4to6-compat") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertV4CompatToIPv4 << false;
    BOBUIest::newRow("localhost4to6-mapped") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertV4MappedToIPv4 << false;
    BOBUIest::newRow("localhost4to6-unspec") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertUnspecifiedAddress << false;
    BOBUIest::newRow("0.0.0.1-::1-local") << QHostAddress("0.0.0.1") << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("v4-v4compat-local") << QHostAddress("192.168.1.1") << QHostAddress("::192.168.1.1") << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("v4-v4mapped-local") << QHostAddress("192.168.1.1") << QHostAddress("::ffff:192.168.1.1") << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("0.0.0.1-::1-unspec") << QHostAddress("0.0.0.1") << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertUnspecifiedAddress << false;
    BOBUIest::newRow("v4-v4compat-unspec") << QHostAddress("192.168.1.1") << QHostAddress("::192.168.1.1") << (int)QHostAddress::ConvertUnspecifiedAddress << false;
    BOBUIest::newRow("v4-v4mapped-unspec") << QHostAddress("192.168.1.1") << QHostAddress("::ffff:192.168.1.1") << (int)QHostAddress::ConvertUnspecifiedAddress << false;
    BOBUIest::newRow("0.0.0.1-::1-compat") << QHostAddress("0.0.0.1") << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertV4CompatToIPv4 << false;
    BOBUIest::newRow("v4-v4compat-compat") << QHostAddress("192.168.1.1") << QHostAddress("::192.168.1.1") << (int)QHostAddress::ConvertV4CompatToIPv4 << true;
    BOBUIest::newRow("v4-v4mapped-compat") << QHostAddress("192.168.1.1") << QHostAddress("::ffff:192.168.1.1") << (int)QHostAddress::ConvertV4CompatToIPv4 << false;
    BOBUIest::newRow("0.0.0.1-::1-mapped") << QHostAddress("0.0.0.1") << QHostAddress(QHostAddress::LocalHostIPv6) << (int)QHostAddress::ConvertV4MappedToIPv4  << false;
    BOBUIest::newRow("v4-v4compat-mapped") << QHostAddress("192.168.1.1") << QHostAddress("::192.168.1.1") << (int)QHostAddress::ConvertV4MappedToIPv4  << false;
    BOBUIest::newRow("v4-v4mapped-mapped") << QHostAddress("192.168.1.1") << QHostAddress("::FFFF:192.168.1.1") << (int)QHostAddress::ConvertV4MappedToIPv4 << true;
    BOBUIest::newRow("undef-any-local") << QHostAddress() << QHostAddress(QHostAddress::Any) << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("undef-any-unspec") << QHostAddress() << QHostAddress(QHostAddress::Any) << (int)QHostAddress::ConvertUnspecifiedAddress << false;
    BOBUIest::newRow("anyv6-anyv4-compat") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::ConvertV4CompatToIPv4 << true;
    BOBUIest::newRow("anyv6-anyv4-mapped") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::ConvertV4MappedToIPv4 << false;
    BOBUIest::newRow("anyv6-anyv4-unspec") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::ConvertUnspecifiedAddress << true;
    BOBUIest::newRow("any-anyv4-unspec") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::ConvertUnspecifiedAddress << true;
    BOBUIest::newRow("any-anyv6-unspec") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::AnyIPv6) << (int)QHostAddress::ConvertUnspecifiedAddress << true;
    BOBUIest::newRow("anyv6-anyv4-local") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("any-anyv4-local") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("any-anyv6-local") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::AnyIPv6) << (int)QHostAddress::ConvertLocalHost << false;
    BOBUIest::newRow("localhostv6-any-tolerant") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::Any) << (int)QHostAddress::TolerantConversion << false;
    BOBUIest::newRow("localhostv4-any-tolerant") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::Any) << (int)QHostAddress::TolerantConversion << false;
    BOBUIest::newRow("localhostv6-anyv6-tolerant") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::AnyIPv6) << (int)QHostAddress::TolerantConversion << false;
    BOBUIest::newRow("localhostv4-anyv6-tolerant") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::AnyIPv6) << (int)QHostAddress::TolerantConversion << false;
    BOBUIest::newRow("localhostv6-anyv4-tolerant") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::TolerantConversion << false;
    BOBUIest::newRow("localhostv4-anyv4-tolerant") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::AnyIPv4) << (int)QHostAddress::TolerantConversion << false;
}

void tst_QHostAddress::isEqual()
{
    QFETCH(QHostAddress, first);
    QFETCH(QHostAddress, second);
    QFETCH(int, flags);
    QFETCH(bool, result);

    QCOMPARE(first.isEqual(second, QHostAddress::ConversionModeFlag(flags)), result);
    QCOMPARE(second.isEqual(first, QHostAddress::ConversionModeFlag(flags)), result);
}

void tst_QHostAddress::assignment()
{
    QHostAddress address;
    QHostAddress addr("4.2.2.1");
    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(addr.toIPv4Address());
    address.setAddress((sockaddr *)&sockAddr);
    QCOMPARE(address, addr);
}

void tst_QHostAddress::scopeId()
{
    QHostAddress address("fe80::2e0:4cff:fefb:662a%eth0");
    QCOMPARE(address.scopeId(), QString("eth0"));
    QCOMPARE(address.toString().toLower(), QString("fe80::2e0:4cff:fefb:662a%eth0"));

    QHostAddress address2("fe80::2e0:4cff:fefb:662a");
    QCOMPARE(address2.scopeId(), QString());
    address2.setScopeId(QString("en0"));
    QCOMPARE(address2.toString().toLower(), QString("fe80::2e0:4cff:fefb:662a%en0"));

    address2 = address;
    QCOMPARE(address2.scopeId(), QString("eth0"));
    QCOMPARE(address2.toString().toLower(), QString("fe80::2e0:4cff:fefb:662a%eth0"));

    address.clear();
    verifyClear(address);
    address2.clear();
    verifyClear(address2);
}

void tst_QHostAddress::hashKey()
{
    QHash<QHostAddress, QString> hostHash;
    hostHash.insert(QHostAddress(), "ole");
}

void tst_QHostAddress::streaming_data()
{
    BOBUIest::addColumn<QHostAddress>("address");
    BOBUIest::newRow("1") << QHostAddress();
    BOBUIest::newRow("2") << QHostAddress(0xDEADBEEF);
    BOBUIest::newRow("3") << QHostAddress("127.128.129.130");
    BOBUIest::newRow("4") << QHostAddress("1080:0000:0000:0000:0008:0800:200C:417A");
    BOBUIest::newRow("5") << QHostAddress("fe80::2e0:4cff:fefb:662a%eth0");
    BOBUIest::newRow("6") << QHostAddress(QHostAddress::Null);
    BOBUIest::newRow("7") << QHostAddress(QHostAddress::LocalHost);
    BOBUIest::newRow("8") << QHostAddress(QHostAddress::LocalHostIPv6);
    BOBUIest::newRow("9") << QHostAddress(QHostAddress::Broadcast);
    BOBUIest::newRow("10") << QHostAddress(QHostAddress::Any);
    BOBUIest::newRow("11") << QHostAddress(QHostAddress::AnyIPv4);
    BOBUIest::newRow("12") << QHostAddress(QHostAddress::AnyIPv6);
    BOBUIest::newRow("13") << QHostAddress("foo.bar.com");
}

void tst_QHostAddress::streaming()
{
    QFETCH(QHostAddress, address);
    QByteArray ba;
    QDataStream ds1(&ba, QIODevice::WriteOnly);
    ds1 << address;
    QCOMPARE(ds1.status(), QDataStream::Ok);
    QDataStream ds2(&ba, QIODevice::ReadOnly);
    QHostAddress address2;
    ds2 >> address2;
    QCOMPARE(ds2.status(), QDataStream::Ok);
    QCOMPARE(address, address2);
}

void tst_QHostAddress::parseSubnet_data()
{
    BOBUIest::addColumn<QString>("subnet");
    BOBUIest::addColumn<QHostAddress>("prefix");
    BOBUIest::addColumn<int>("prefixLength");

    // invalid/error values
    BOBUIest::newRow("empty") << QString() << QHostAddress() << -1;
    BOBUIest::newRow("invalid_01") << "foobar" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_02") << "   " << QHostAddress() << -1;
    BOBUIest::newRow("invalid_03") << "1.2.3.a" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_04") << "1.2.3.4.5" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_05") << "1.2.3.4:80" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_06") << "1.2.3.4/33" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_07") << "1.2.3.4/-1" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_08") << "1.2.3.4/256.0.0.0" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_09") << "1.2.3.4/255.253.0.0" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_10") << "1.2.3.4/255.0.0.255" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_11") << "1.2.3.4." << QHostAddress() << -1;
    BOBUIest::newRow("invalid_20") << "ffff::/-1" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_21") << "ffff::/129" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_22") << "ffff::/255.255.0.0" << QHostAddress() << -1;
    BOBUIest::newRow("invalid_23") << "ffff::/ff00::" << QHostAddress() << -1;

    // correct IPv4 with netmask
    BOBUIest::newRow("netmask_0") << "0.0.0.0/0.0.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 0;
    BOBUIest::newRow("netmask_1") << "0.0.0.0/255.128.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 9;
    BOBUIest::newRow("netmask_2") << "0.0.0.0/255.192.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 10;
    BOBUIest::newRow("netmask_3") << "0.0.0.0/255.224.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 11;
    BOBUIest::newRow("netmask_4") << "0.0.0.0/255.240.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 12;
    BOBUIest::newRow("netmask_5") << "0.0.0.0/255.248.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 13;
    BOBUIest::newRow("netmask_6") << "0.0.0.0/255.252.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 14;
    BOBUIest::newRow("netmask_7") << "0.0.0.0/255.254.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 15;
    BOBUIest::newRow("netmask_8") << "0.0.0.0/255.255.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 16;
    BOBUIest::newRow("netmask_16") << "0.0.0.0/255.255.0.0" << QHostAddress(QHostAddress::AnyIPv4) << 16;
    BOBUIest::newRow("netmask_24") << "0.0.0.0/255.255.255.0" << QHostAddress(QHostAddress::AnyIPv4) << 24;
    BOBUIest::newRow("netmask_31") << "0.0.0.0/255.255.255.254" << QHostAddress(QHostAddress::AnyIPv4) << 31;
    BOBUIest::newRow("netmask_32") << "0.0.0.0/255.255.255.255" << QHostAddress(QHostAddress::AnyIPv4) << 32;

    // correct IPv4 with prefix
    BOBUIest::newRow("prefix_0") << "0.0.0.0/0" << QHostAddress(QHostAddress::AnyIPv4) << 0;
    BOBUIest::newRow("prefix_1") << "0.0.0.0/1" << QHostAddress(QHostAddress::AnyIPv4) << 1;
    BOBUIest::newRow("prefix_9") << "0.0.0.0/9" << QHostAddress(QHostAddress::AnyIPv4) << 9;
    BOBUIest::newRow("prefix_31") << "0.0.0.0/31" << QHostAddress(QHostAddress::AnyIPv4) << 31;
    BOBUIest::newRow("prefix_32") << "0.0.0.0/32" << QHostAddress(QHostAddress::AnyIPv4) << 32;

    // correct IPv4 without prefix or netmask
    BOBUIest::newRow("classA") << "10" << QHostAddress("10.0.0.0") << 8;
    BOBUIest::newRow("classA+dot") << "10." << QHostAddress("10.0.0.0") << 8;
    BOBUIest::newRow("classB") << "172.16" << QHostAddress("172.16.0.0") << 16;
    BOBUIest::newRow("classB+dot") << "172.16." << QHostAddress("172.16.0.0") << 16;
    BOBUIest::newRow("classC") << "192.168.0" << QHostAddress("192.168.0.0") << 24;
    BOBUIest::newRow("classC+dot") << "192.168.0" << QHostAddress("192.168.0.0") << 24;
    BOBUIest::newRow("full-ipv4") << "192.168.0.1" << QHostAddress("192.168.0.1") << 32;

    // correct IPv6 with prefix
    BOBUIest::newRow("ipv6_01") << "::/0" << QHostAddress(QHostAddress::AnyIPv6) << 0;
    BOBUIest::newRow("ipv6_03") << "::/3" << QHostAddress(QHostAddress::AnyIPv6) << 3;
    BOBUIest::newRow("ipv6_16") << "::/16" << QHostAddress(QHostAddress::AnyIPv6) << 16;
    BOBUIest::newRow("ipv6_48") << "::/48" << QHostAddress(QHostAddress::AnyIPv6) << 48;
    BOBUIest::newRow("ipv6_127") << "::/127" << QHostAddress(QHostAddress::AnyIPv6) << 127;
    BOBUIest::newRow("ipv6_128") << "::/128" << QHostAddress(QHostAddress::AnyIPv6) << 128;

    // tail bit clearing:
    BOBUIest::newRow("clear_01") << "255.255.255.255/31" << QHostAddress("255.255.255.254") << 31;
    BOBUIest::newRow("clear_08") << "255.255.255.255/24" << QHostAddress("255.255.255.0") << 24;
    BOBUIest::newRow("clear_09") << "255.255.255.255/23" << QHostAddress("255.255.254.0") << 23;
    BOBUIest::newRow("clear_10") << "255.255.255.255/22" << QHostAddress("255.255.252.0") << 22;
    BOBUIest::newRow("clear_11") << "255.255.255.255/21" << QHostAddress("255.255.248.0") << 21;
    BOBUIest::newRow("clear_12") << "255.255.255.255/20" << QHostAddress("255.255.240.0") << 20;
    BOBUIest::newRow("clear_13") << "255.255.255.255/19" << QHostAddress("255.255.224.0") << 19;
    BOBUIest::newRow("clear_14") << "255.255.255.255/18" << QHostAddress("255.255.192.0") << 18;
    BOBUIest::newRow("clear_15") << "255.255.255.255/17" << QHostAddress("255.255.128.0") << 17;
    BOBUIest::newRow("clear_16") << "255.255.255.255/16" << QHostAddress("255.255.0.0") << 16;
    BOBUIest::newRow("clear_24") << "255.255.255.255/8" << QHostAddress("255.0.0.0") << 8;
    BOBUIest::newRow("clear_31") << "255.255.255.255/1" << QHostAddress("128.0.0.0") << 1;
    BOBUIest::newRow("clear_32") << "255.255.255.255/0" << QHostAddress("0.0.0.0") << 0;

    // same for IPv6:
    BOBUIest::newRow("ipv6_clear_01") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/127"
                                   << QHostAddress("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fffe")
                                   << 127;
    BOBUIest::newRow("ipv6_clear_07") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/121"
                                   << QHostAddress("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff80")
                                   << 121;
    BOBUIest::newRow("ipv6_clear_08") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/120"
                                   << QHostAddress("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff00")
                                   << 120;
    BOBUIest::newRow("ipv6_clear_16") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/112"
                                   << QHostAddress("ffff:ffff:ffff:ffff:ffff:ffff:ffff:0")
                                   << 112;
    BOBUIest::newRow("ipv6_clear_80") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/48"
                                   << QHostAddress("ffff:ffff:ffff::")
                                   << 48;
    BOBUIest::newRow("ipv6_clear_81") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/47"
                                   << QHostAddress("ffff:ffff:fffe::")
                                   << 47;
    BOBUIest::newRow("ipv6_clear_82") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/46"
                                   << QHostAddress("ffff:ffff:fffc::")
                                   << 46;
    BOBUIest::newRow("ipv6_clear_83") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/45"
                                   << QHostAddress("ffff:ffff:fff8::")
                                   << 45;
    BOBUIest::newRow("ipv6_clear_84") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/44"
                                   << QHostAddress("ffff:ffff:fff0::")
                                   << 44;
    BOBUIest::newRow("ipv6_clear_85") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/43"
                                   << QHostAddress("ffff:ffff:ffe0::")
                                   << 43;
    BOBUIest::newRow("ipv6_clear_86") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/42"
                                   << QHostAddress("ffff:ffff:ffc0::")
                                   << 42;
    BOBUIest::newRow("ipv6_clear_87") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/41"
                                   << QHostAddress("ffff:ffff:ff80::")
                                   << 41;
    BOBUIest::newRow("ipv6_clear_88") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/40"
                                   << QHostAddress("ffff:ffff:ff00::")
                                   << 40;
    BOBUIest::newRow("ipv6_clear_125") << "3fff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/3"
                                    << QHostAddress("2000::")
                                    << 3;
    BOBUIest::newRow("ipv6_clear_127") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/1"
                                    << QHostAddress("8000::")
                                    << 1;
    BOBUIest::newRow("ipv6_clear_128") << "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/0"
                                    << QHostAddress(QHostAddress::AnyIPv6)
                                    << 0;
}

void tst_QHostAddress::parseSubnet()
{
    QFETCH(QString, subnet);
    QFETCH(QHostAddress, prefix);
    QFETCH(int, prefixLength);

    std::pair<QHostAddress, int> result = QHostAddress::parseSubnet(subnet);
    QCOMPARE(result.first, prefix);
    QCOMPARE(result.second, prefixLength);
}

void tst_QHostAddress::isInSubnet_data()
{
    BOBUIest::addColumn<QHostAddress>("address");
    BOBUIest::addColumn<QHostAddress>("prefix");
    BOBUIest::addColumn<int>("prefixLength");
    BOBUIest::addColumn<bool>("result");

    // invalid QHostAddresses are never in any subnets
    BOBUIest::newRow("invalid_01") << QHostAddress() << QHostAddress() << 32 << false;
    BOBUIest::newRow("invalid_02") << QHostAddress() << QHostAddress(QHostAddress::AnyIPv4) << 32 << false;
    BOBUIest::newRow("invalid_03") << QHostAddress() << QHostAddress(QHostAddress::AnyIPv4) << 8 << false;
    BOBUIest::newRow("invalid_04") << QHostAddress() << QHostAddress(QHostAddress::AnyIPv4) << 0 << false;
    BOBUIest::newRow("invalid_05") << QHostAddress() << QHostAddress("255.255.255.0") << 24 << false;
    BOBUIest::newRow("invalid_06") << QHostAddress() << QHostAddress(QHostAddress::AnyIPv6) << 0 << false;
    BOBUIest::newRow("invalid_07") << QHostAddress() << QHostAddress(QHostAddress::AnyIPv6) << 32 << false;
    BOBUIest::newRow("invalid_08") << QHostAddress() << QHostAddress(QHostAddress::AnyIPv6) << 128<< false;

    // and no host address can be in a subnet whose prefix is invalid
    BOBUIest::newRow("invalid_20") << QHostAddress(QHostAddress::AnyIPv4) << QHostAddress() << 16 << false;
    BOBUIest::newRow("invalid_21") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress() << 16 << false;
    BOBUIest::newRow("invalid_22") << QHostAddress(QHostAddress::LocalHost) << QHostAddress() << 16 << false;
    BOBUIest::newRow("invalid_23") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress() << 16 << false;

    // negative netmasks don't make sense:
    BOBUIest::newRow("invalid_30") << QHostAddress(QHostAddress::AnyIPv4) << QHostAddress(QHostAddress::Any) << -1 << false;
    BOBUIest::newRow("invalid_31") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv6) << -1 << false;

    // we don't support IPv4 belonging in an IPv6 netmask and vice-versa
    BOBUIest::newRow("v4-in-v6") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::AnyIPv6) << 0 << false;
    BOBUIest::newRow("v6-in-v4") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::Any) << 0 << false;
    BOBUIest::newRow("v4-in-v6mapped") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("ffff:ffff:ffff:ffff:ffff:ffff:255.0.0.0") << 113 << false;
    BOBUIest::newRow("v4-in-v6mapped2") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("::ffff:255.0.0.0") << 113 << false;

    // IPv4 correct ones
    BOBUIest::newRow("netmask_0") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::AnyIPv4) << 0 << true;
    BOBUIest::newRow("netmask_0bis") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("255.255.0.0") << 0 << true;
    BOBUIest::newRow("netmask_0ter") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("1.2.3.4") << 0 << true;
    BOBUIest::newRow("netmask_1") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::AnyIPv4) << 1 << true;
    BOBUIest::newRow("~netmask_1") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("128.0.0.0") << 1 << false;
    BOBUIest::newRow("netmask_1bis") << QHostAddress("224.0.0.1") << QHostAddress("128.0.0.0") << 1 << true;
    BOBUIest::newRow("~netmask_1bis") << QHostAddress("224.0.0.1") << QHostAddress("0.0.0.0") << 1 << false;
    BOBUIest::newRow("netmask_8") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("127.0.0.0") << 8 << true;
    BOBUIest::newRow("~netmask_8") << QHostAddress(QHostAddress::LocalHost) << QHostAddress("126.0.0.0") << 8 << false;
    BOBUIest::newRow("netmask_15") << QHostAddress("10.0.1.255") << QHostAddress("10.0.0.0") << 15 << true;
    BOBUIest::newRow("netmask_16") << QHostAddress("172.16.0.1") << QHostAddress("172.16.0.0") << 16 << true;

    // the address is always in the subnet containing its address, regardless of length:
    BOBUIest::newRow("same_01") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHost) << 1 << true;
    BOBUIest::newRow("same_07") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHost) << 7 << true;
    BOBUIest::newRow("same_8") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHost) << 8 << true;
    BOBUIest::newRow("same_24") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHost) << 23 << true;
    BOBUIest::newRow("same_31") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHost) << 31 << true;
    BOBUIest::newRow("same_32") << QHostAddress(QHostAddress::LocalHost) << QHostAddress(QHostAddress::LocalHost) << 32 << true;

    // IPv6 correct ones:
    BOBUIest::newRow("ipv6_netmask_0") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::AnyIPv6) << 0 << true;
    BOBUIest::newRow("ipv6_netmask_0bis") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::LocalHostIPv6) << 0 << true;
    BOBUIest::newRow("ipv6_netmask_0ter") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress("ffff::") << 0 << true;
    BOBUIest::newRow("ipv6_netmask_1") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress(QHostAddress::AnyIPv6) << 1 << true;
    BOBUIest::newRow("ipv6_netmask_1bis") << QHostAddress("fec0::1") << QHostAddress("8000::") << 1 << true;
    BOBUIest::newRow("~ipv6_netmask_1") << QHostAddress(QHostAddress::LocalHostIPv6) << QHostAddress("8000::") << 1 << false;
    BOBUIest::newRow("~ipv6_netmask_1bis") << QHostAddress("fec0::1") << QHostAddress("::") << 1 << false;
    BOBUIest::newRow("ipv6_netmask_47") << QHostAddress("2:3:5::1") << QHostAddress("2:3:4::") << 47 << true;
    BOBUIest::newRow("ipv6_netmask_48") << QHostAddress("2:3:4::1") << QHostAddress("2:3:4::") << 48 << true;
    BOBUIest::newRow("~ipv6_netmask_48") << QHostAddress("2:3:5::1") << QHostAddress("2:3:4::") << 48 << false;
    BOBUIest::newRow("ipv6_netmask_127") << QHostAddress("2:3:4:5::1") << QHostAddress("2:3:4:5::") << 127 << true;
    BOBUIest::newRow("ipv6_netmask_128") << QHostAddress("2:3:4:5::1") << QHostAddress("2:3:4:5::1") << 128 << true;
    BOBUIest::newRow("~ipv6_netmask_128") << QHostAddress("2:3:4:5::1") << QHostAddress("2:3:4:5::0") << 128 << false;
}

void tst_QHostAddress::isInSubnet()
{
    QFETCH(QHostAddress, address);
    QFETCH(QHostAddress, prefix);
    QFETCH(int, prefixLength);

    BOBUIEST(address.isInSubnet(prefix, prefixLength), "result");
}

void tst_QHostAddress::classification_data()
{
    BOBUIest::addColumn<QHostAddress>("address");
    BOBUIest::addColumn<AddressClassification>("result");

    BOBUIest::newRow("default") << QHostAddress() << UnknownAddress;
    BOBUIest::newRow("invalid") << QHostAddress("&&&") << UnknownAddress;

    BOBUIest::newRow("Any") << QHostAddress(QHostAddress::Any) << LocalNetAddress;
    BOBUIest::newRow("Null") << QHostAddress(QHostAddress::Null) << UnknownAddress;

    // IPv6 address space
    auto addV6 = [](const char *str, AddressClassification cl) {
        BOBUIest::newRow(str) << QHostAddress(str) << cl;
    };
    BOBUIest::newRow("AnyIPv6") << QHostAddress(QHostAddress::AnyIPv6) << LocalNetAddress;
    BOBUIest::newRow("ipv6_loop") << QHostAddress(QHostAddress::LocalHostIPv6) << LoopbackAddress;
    addV6("::", LocalNetAddress);
    addV6("::1", LoopbackAddress);
    addV6("::2", GlobalAddress);
    addV6("2000::", GlobalAddress);
    addV6("3fff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", GlobalAddress);
    addV6("fbff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", GlobalAddress);
    addV6("fc00::", UniqueLocalAddress);
    addV6("fdff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", UniqueLocalAddress);
    addV6("fe00::", UnknownAddress);
    addV6("fe7f:ffff:ffff:ffff:ffff:ffff:ffff:ffff", UnknownAddress);
    addV6("fe80::", LinkLocalAddress);
    addV6("febf:ffff:ffff:ffff:ffff:ffff:ffff:ffff", LinkLocalAddress);
    addV6("fec0::", SiteLocalAddress);
    addV6("feff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", SiteLocalAddress);
    addV6("ff00::", MulticastAddress);
    addV6("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", MulticastAddress);

    // IPv4 address space
    auto addV4 = [](const char *str, AddressClassification cl) {
        BOBUIest::newRow(str) << QHostAddress(str) << cl;
        QByteArray v6 = "::ffff:";
        v6 += str;
        BOBUIest::newRow(v6.constData()) << QHostAddress(QString::fromLatin1(v6)) << cl;
    };
    BOBUIest::newRow("AnyIPv4") << QHostAddress(QHostAddress::AnyIPv4) << LocalNetAddress;
    BOBUIest::newRow("ipv4_loop") << QHostAddress(QHostAddress::LocalHost) << LoopbackAddress;
    BOBUIest::newRow("Broadcast") << QHostAddress(QHostAddress::Broadcast) << BroadcastAddress;
    addV4("0.0.0.0", LocalNetAddress);
    addV4("0.0.0.1", LocalNetAddress);
    addV4("0.255.255.255", LocalNetAddress);
    addV4("1.0.0.0", GlobalAddress);
    addV4("1.2.3.4", GlobalAddress);
    addV4("10.0.0.4", PrivateNetworkAddress);
    addV4("127.0.0.1", LoopbackAddress);
    addV4("127.0.0.2", LoopbackAddress);
    addV4("127.255.255.255", LoopbackAddress);
    addV4("192.168.3.4", PrivateNetworkAddress);
    addV4("223.255.255.255", GlobalAddress);
    addV4("224.0.0.0", MulticastAddress);
    addV4("239.255.255.255", MulticastAddress);
    addV4("240.0.0.0", UnknownAddress);
    addV4("255.255.255.254", UnknownAddress);
    addV4("255.255.255.255", BroadcastAddress);
}

void tst_QHostAddress::classification()
{
    QFETCH(QHostAddress, address);
    QFETCH(AddressClassification, result);

    bool isLoopback = (result == LoopbackAddress);
    bool isGlobal = (result & GlobalAddress);    // GlobalAddress is a bit
    bool isLinkLocal = (result == LinkLocalAddress);
    bool isSiteLocal = (result == SiteLocalAddress);
    bool isUniqueLocalAddress = (result == UniqueLocalAddress);
    bool isMulticast = (result == MulticastAddress);
    bool isBroadcast = (result == BroadcastAddress);
    bool isPrivateUse = (result == PrivateNetworkAddress || result == UniqueLocalAddress);

    QCOMPARE(address.isLoopback(), isLoopback);
    QCOMPARE(address.isGlobal(), isGlobal);
    QCOMPARE(address.isLinkLocal(), isLinkLocal);
    QCOMPARE(address.isSiteLocal(), isSiteLocal);
    QCOMPARE(address.isUniqueLocalUnicast(), isUniqueLocalAddress);
    QCOMPARE(address.isMulticast(), isMulticast);
    QCOMPARE(address.isBroadcast(), isBroadcast);
    QCOMPARE(address.isPrivateUse(), isPrivateUse);
}

void tst_QHostAddress::convertv4v6_data()
{
    BOBUIest::addColumn<QHostAddress>("source");
    BOBUIest::addColumn<int>("protocol");
    BOBUIest::addColumn<QHostAddress>("result");

    BOBUIest::newRow("any-to-v4") << QHostAddress(QHostAddress::Any) << 4 << QHostAddress(QHostAddress::AnyIPv4);
    BOBUIest::newRow("any-to-v6") << QHostAddress(QHostAddress::Any) << 6 << QHostAddress(QHostAddress::AnyIPv6);
    BOBUIest::newRow("anyv4-to-v6") << QHostAddress(QHostAddress::AnyIPv4) << 6 << QHostAddress(QHostAddress::AnyIPv6);
    BOBUIest::newRow("anyv6-to-v4") << QHostAddress(QHostAddress::AnyIPv6) << 4 << QHostAddress(QHostAddress::AnyIPv4);

    BOBUIest::newRow("v4mapped-to-v4") << QHostAddress("::ffff:192.0.2.1") << 4 << QHostAddress("192.0.2.1");
    BOBUIest::newRow("v4-to-v4mapped") << QHostAddress("192.0.2.1") << 6 << QHostAddress("::ffff:192.0.2.1");

    // we won't convert 127.0.0.1 to ::1 or vice-versa:
    // you can connect to a v4 server socket with ::ffff:127.0.0.1, but not with ::1
    BOBUIest::newRow("localhost-to-v4mapped") << QHostAddress(QHostAddress::LocalHost) << 6 << QHostAddress("::ffff:127.0.0.1");
    BOBUIest::newRow("v4mapped-to-localhost") << QHostAddress("::ffff:127.0.0.1") << 4 << QHostAddress(QHostAddress::LocalHost);

    // in turn, that means localhost6 doesn't convert to v4
    BOBUIest::newRow("localhost6-to-v4") << QHostAddress(QHostAddress::LocalHostIPv6) << 4 << QHostAddress();

    // some other v6 addresses that won't convert to v4
    BOBUIest::newRow("v4compat-to-v4") << QHostAddress("::192.0.2.1") << 4 << QHostAddress();
    BOBUIest::newRow("localhostv4compat-to-v4") << QHostAddress("::127.0.0.1") << 4 << QHostAddress();
    BOBUIest::newRow("v6global-to-v4") << QHostAddress("2001:db8::1") << 4 << QHostAddress();
    BOBUIest::newRow("v6multicast-to-v4") << QHostAddress("ff02::1") << 4 << QHostAddress();
}

void tst_QHostAddress::convertv4v6()
{
    QFETCH(QHostAddress, source);
    QFETCH(int, protocol);
    QFETCH(QHostAddress, result);

    if (protocol == 4) {
        bool ok;
        quint32 v4 = source.toIPv4Address(&ok);
        QCOMPARE(ok, result.protocol() == QHostAddress::IPv4Protocol);
        if (ok)
            QCOMPARE(QHostAddress(v4), result);
    } else if (protocol == 6) {
        QCOMPARE(QHostAddress(source.toIPv6Address()), result);
    }
}

BOBUIEST_MAIN(tst_QHostAddress)
#include "tst_qhostaddress.moc"
