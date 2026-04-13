// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#define _CRT_SECURE_NO_WARNINGS 1

#include <BobUICore/QString>
#include <BOBUIest>
#include <BobUICore/private/qipaddress_p.h>

#ifdef __GLIBC__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class tst_QIpAddress : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void parseIp4_data();
    void parseIp4();
    void invalidParseIp4_data();
    void invalidParseIp4();
    void ip4ToString_data();
    void ip4ToString();

    void parseIp6_data();
    void parseIp6();
    void invalidParseIp6_data();
    void invalidParseIp6();
    void ip6ToString_data();
    void ip6ToString();
};

struct Ip6
{
    QIPAddressUtils::IPv6Address u8;
    Ip6() { *this = Ip6(0,0,0,0, 0,0,0,0); }
    Ip6(quint16 p1, quint16 p2, quint16 p3, quint16 p4,
         quint16 p5, quint16 p6, quint16 p7, quint16 p8)
    {
        u8[0] = p1 >> 8;
        u8[2] = p2 >> 8;
        u8[4] = p3 >> 8;
        u8[6] = p4 >> 8;
        u8[8] = p5 >> 8;
        u8[10] = p6 >> 8;
        u8[12] = p7 >> 8;
        u8[14] = p8 >> 8;

        u8[1] = p1 & 0xff;
        u8[3] = p2 & 0xff;
        u8[5] = p3 & 0xff;
        u8[7] = p4 & 0xff;
        u8[9] = p5 & 0xff;
        u8[11] = p6 & 0xff;
        u8[13] = p7 & 0xff;
        u8[15] = p8 & 0xff;
    }

    bool operator==(const Ip6 &other) const
    { return memcmp(u8, other.u8, sizeof u8) == 0; }
};
Q_DECLARE_METATYPE(Ip6)

BOBUI_BEGIN_NAMESPACE
namespace BOBUIest {
    template<>
    char *toString(const Ip6 &ip6)
    {
        char buf[sizeof "1111:2222:3333:4444:5555:6666:7777:8888" + 2];
        snprintf(buf, sizeof(buf), "%x:%x:%x:%x:%x:%x:%x:%x",
                ip6.u8[0] << 8 | ip6.u8[1],
                ip6.u8[2] << 8 | ip6.u8[3],
                ip6.u8[4] << 8 | ip6.u8[5],
                ip6.u8[6] << 8 | ip6.u8[7],
                ip6.u8[8] << 8 | ip6.u8[9],
                ip6.u8[10] << 8 | ip6.u8[11],
                ip6.u8[12] << 8 | ip6.u8[13],
                ip6.u8[14] << 8 | ip6.u8[15]);
        return qstrdup(buf);
    }
}
BOBUI_END_NAMESPACE

void tst_QIpAddress::parseIp4_data()
{
    BOBUIest::addColumn<QString>("data");
    BOBUIest::addColumn<QIPAddressUtils::IPv4Address>("ip");

    // valid strings
    BOBUIest::newRow("0.0.0.0") << "0.0.0.0" << 0u;
    BOBUIest::newRow("10.0.0.1") << "10.0.0.1" << 0x0a000001u;
    BOBUIest::newRow("127.0.0.1") << "127.0.0.1" << 0x7f000001u;
    BOBUIest::newRow("172.16.0.1") << "172.16.0.1" << 0xac100001u;
    BOBUIest::newRow("172.16.16.1") << "172.16.16.1" << 0xac101001u;
    BOBUIest::newRow("172.16.16.16") << "172.16.16.16" << 0xac101010u;
    BOBUIest::newRow("192.168.0.1") << "192.168.0.1" << 0xc0a80001u;
    BOBUIest::newRow("192.168.16.1") << "192.168.16.1" << 0xc0a81001u;
    BOBUIest::newRow("192.168.16.16") << "192.168.16.16" << 0xc0a81010u;
    BOBUIest::newRow("192.168.192.1") << "192.168.192.1" << 0xc0a8c001u;
    BOBUIest::newRow("192.168.192.16") << "192.168.192.16" << 0xc0a8c010u;
    BOBUIest::newRow("192.168.192.255") << "192.168.192.255" << 0xc0a8c0ffu;
    BOBUIest::newRow("224.0.0.1") << "224.0.0.1" << 0xe0000001u;
    BOBUIest::newRow("239.255.255.255") << "239.255.255.255" << 0xefffffffu;
    BOBUIest::newRow("255.255.255.255") << "255.255.255.255" << uint(-1);

    // still valid but unusual
    BOBUIest::newRow("000.000.000.000") << "000.000.000.000" << 0u;
    BOBUIest::newRow("000001.000002.000000003.000000000004") << "000001.000002.000000003.000000000004" << 0x01020304u;

    // octals:
    BOBUIest::newRow("012.0250.0377.0377") << "012.0250.0377.0377" << 0x0aa8ffffu;
    BOBUIest::newRow("0000000000012.00000000000250.000000000000377.0000000000000000000000000000000000000377")
            << "0000000000012.00000000000250.000000000000377.0000000000000000000000000000000000000377" << 0x0aa8ffffu;

    // hex:
    BOBUIest::newRow("0xa.0xa.0x7f.0xff") << "0xa.0xa.0x7f.0xff" << 0x0a0a7fffu;

    // dots missing, less than 255:
    BOBUIest::newRow("1.2.3") << "1.2.3" << 0x01020003u;
    BOBUIest::newRow("1.2") << "1.2" << 0x01000002u;
    BOBUIest::newRow("1") << "1" << 1u;

    // dots missing, more than 255, no overwrite
    BOBUIest::newRow("1.2.257") << "1.2.257" << 0x01020101u;
    BOBUIest::newRow("1.0x010101") << "1.0x010101" << 0x01010101u;
    BOBUIest::newRow("2130706433") << "2130706433" << 0x7f000001u;
}

void tst_QIpAddress::parseIp4()
{
    QFETCH(QString, data);
    QFETCH(QIPAddressUtils::IPv4Address, ip);

#ifdef __GLIBC__
    {
        in_addr inet_result;
        int inet_ok = inet_aton(data.toLatin1(), &inet_result);
        QVERIFY(inet_ok);
        QCOMPARE(ntohl(inet_result.s_addr), ip);
    }
#endif

    QIPAddressUtils::IPv4Address result;
    bool ok = QIPAddressUtils::parseIp4(result, data.constBegin(), data.constEnd());
    QVERIFY(ok);
    QCOMPARE(result, ip);
}

void tst_QIpAddress::invalidParseIp4_data()
{
    BOBUIest::addColumn<QString>("data");

    // too many dots
    BOBUIest::newRow(".") << ".";
    BOBUIest::newRow("..") << "..";
    BOBUIest::newRow("...") << "...";
    BOBUIest::newRow("....") << "....";
    BOBUIest::newRow(".1.2.3") << ".1.2.3";
    BOBUIest::newRow("1.") << "1.";
    BOBUIest::newRow("1.2.") << "1.2.";
    BOBUIest::newRow("1.2.3.") << "1.2.3.";
    BOBUIest::newRow("1.2.3.4.") << "1.2.3.4.";
    BOBUIest::newRow("1.2.3..4") << "1.2.3..4";

    // octet more than 255
    BOBUIest::newRow("2.2.2.257") << "2.2.2.257";
    BOBUIest::newRow("2.2.257.2") << "2.2.257.2";
    BOBUIest::newRow("2.257.2.2") << "2.257.2.2";
    BOBUIest::newRow("257.2.2.2") << "257.2.2.2";

    // number more than field available
    BOBUIest::newRow("2.2.0x01010101") << "2.2.0x01010101";
    BOBUIest::newRow("2.0x01010101") << "2.0x01010101";
    BOBUIest::newRow("4294967296") << "4294967296";

    // bad octals
    BOBUIest::newRow("09") << "09";

    // bad hex
    BOBUIest::newRow("0x1g") << "0x1g";

    // negative numbers
    BOBUIest::newRow("-1") << "-1";
    BOBUIest::newRow("-1.1") << "-1.1";
    BOBUIest::newRow("1.-1") << "1.-1";
    BOBUIest::newRow("1.1.1.-1") << "1.1.1.-1";
    BOBUIest::newRow("300-05") << "300-05";
    BOBUIest::newRow("127.-1") << "127.-1";
    BOBUIest::newRow("-127-10") << "-127-10";
    BOBUIest::newRow("198.-16") << "198-16";
    BOBUIest::newRow("-127.-0.") << "-127.-0.";

    // letters
    BOBUIest::newRow("abc") << "abc";
    BOBUIest::newRow("localhost") << "localhost";
    BOBUIest::newRow("1.2.3a.4") << "1.2.3a.4";
    BOBUIest::newRow("a.2.3.4") << "a.2.3.4";
    BOBUIest::newRow("1.2.3.4a") << "1.2.3.4a";
}

void tst_QIpAddress::invalidParseIp4()
{
    QFETCH(QString, data);

#ifdef __GLIBC__
    {
        in_addr inet_result;
        int inet_ok = inet_aton(data.toLatin1(), &inet_result);
# ifdef Q_OS_DARWIN
        QEXPECT_FAIL("4294967296", "Mac's library does parse this one", Continue);
# endif
        QVERIFY(!inet_ok);
    }
#endif

    QIPAddressUtils::IPv4Address result;
    bool ok = QIPAddressUtils::parseIp4(result, data.constBegin(), data.constEnd());
    QVERIFY(!ok);
}

void tst_QIpAddress::ip4ToString_data()
{
    BOBUIest::addColumn<QIPAddressUtils::IPv4Address>("ip");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("0.0.0.0") << 0u << "0.0.0.0";
    BOBUIest::newRow("1.2.3.4") << 0x01020304u << "1.2.3.4";
    BOBUIest::newRow("127.0.0.1") << 0x7f000001u << "127.0.0.1";
    BOBUIest::newRow("111.222.33.44") << 0x6fde212cu << "111.222.33.44";
    BOBUIest::newRow("255.255.255.255") << 0xffffffffu << "255.255.255.255";
}

void tst_QIpAddress::ip4ToString()
{
    QFETCH(QIPAddressUtils::IPv4Address, ip);
    QFETCH(QString, expected);

#ifdef __GLIBC__
    in_addr inet_ip;
    inet_ip.s_addr = htonl(ip);
    QCOMPARE(QString(inet_ntoa(inet_ip)), expected);
#endif

    QString result;
    QIPAddressUtils::toString(result, ip);
    QCOMPARE(result, expected);
}

void tst_QIpAddress::parseIp6_data()
{
    qRegisterMetaType<Ip6>();
    BOBUIest::addColumn<QString>("address");
    BOBUIest::addColumn<Ip6>("expected");

    // 7 colons, no ::
    BOBUIest::newRow("0:0:0:0:0:0:0:0") << "0:0:0:0:0:0:0:0" << Ip6(0,0,0,0,0,0,0,0);
    BOBUIest::newRow("0:0:0:0:0:0:0:1") << "0:0:0:0:0:0:0:1" << Ip6(0,0,0,0,0,0,0,1);
    BOBUIest::newRow("0:0:0:0:0:0:1:1") << "0:0:0:0:0:0:1:1" << Ip6(0,0,0,0,0,0,1,1);
    BOBUIest::newRow("0:0:0:0:0:0:0:103") << "0:0:0:0:0:0:0:103" << Ip6(0,0,0,0,0,0,0,0x103);
    BOBUIest::newRow("1:2:3:4:5:6:7:8") << "1:2:3:4:5:6:7:8" << Ip6(1,2,3,4,5,6,7,8);
    BOBUIest::newRow("ffee:ddcc:bbaa:9988:7766:5544:3322:1100")
            << "ffee:ddcc:bbaa:9988:7766:5544:3322:1100"
            << Ip6(0xffee, 0xddcc, 0xbbaa, 0x9988, 0x7766, 0x5544, 0x3322, 0x1100);

    // double-colon
    BOBUIest::newRow("::1:2:3:4:5:6:7") << "::1:2:3:4:5:6:7" << Ip6(0,1,2,3,4,5,6,7);
    BOBUIest::newRow("1:2:3:4:5:6:7::") << "1:2:3:4:5:6:7::" << Ip6(1,2,3,4,5,6,7,0);

    BOBUIest::newRow("1::2:3:4:5:6:7") << "1::2:3:4:5:6:7" << Ip6(1,0,2,3,4,5,6,7);
    BOBUIest::newRow("1:2::3:4:5:6:7") << "1:2::3:4:5:6:7" << Ip6(1,2,0,3,4,5,6,7);
    BOBUIest::newRow("1:2:3::4:5:6:7") << "1:2:3::4:5:6:7" << Ip6(1,2,3,0,4,5,6,7);
    BOBUIest::newRow("1:2:3:4::5:6:7") << "1:2:3:4::5:6:7" << Ip6(1,2,3,4,0,5,6,7);
    BOBUIest::newRow("1:2:3:4:5::6:7") << "1:2:3:4:5::6:7" << Ip6(1,2,3,4,5,0,6,7);
    BOBUIest::newRow("1:2:3:4:5:6::7") << "1:2:3:4:5:6::7" << Ip6(1,2,3,4,5,6,0,7);

    BOBUIest::newRow("::1:2:3:4:5:6") << "::1:2:3:4:5:6" << Ip6(0,0,1,2,3,4,5,6);
    BOBUIest::newRow("1:2:3:4:5:6::") << "1:2:3:4:5:6::" << Ip6(1,2,3,4,5,6,0,0);

    BOBUIest::newRow("1::2:3:4:5:6") << "1::2:3:4:5:6" << Ip6(1,0,0,2,3,4,5,6);
    BOBUIest::newRow("1:2::3:4:5:6") << "1:2::3:4:5:6" << Ip6(1,2,0,0,3,4,5,6);
    BOBUIest::newRow("1:2:3::4:5:6") << "1:2:3::4:5:6" << Ip6(1,2,3,0,0,4,5,6);
    BOBUIest::newRow("1:2:3:4::5:6") << "1:2:3:4::5:6" << Ip6(1,2,3,4,0,0,5,6);
    BOBUIest::newRow("1:2:3:4:5::6") << "1:2:3:4:5::6" << Ip6(1,2,3,4,5,0,0,6);

    BOBUIest::newRow("::1:2:3:4:5") << "::1:2:3:4:5" << Ip6(0,0,0,1,2,3,4,5);
    BOBUIest::newRow("1:2:3:4:5::") << "1:2:3:4:5::" << Ip6(1,2,3,4,5,0,0,0);

    BOBUIest::newRow("1::2:3:4:5") << "1::2:3:4:5" << Ip6(1,0,0,0,2,3,4,5);
    BOBUIest::newRow("1:2::3:4:5") << "1:2::3:4:5" << Ip6(1,2,0,0,0,3,4,5);
    BOBUIest::newRow("1:2:3::4:5") << "1:2:3::4:5" << Ip6(1,2,3,0,0,0,4,5);
    BOBUIest::newRow("1:2:3:4::5") << "1:2:3:4::5" << Ip6(1,2,3,4,0,0,0,5);

    BOBUIest::newRow("::1:2:3:4") << "::1:2:3:4" << Ip6(0,0,0,0,1,2,3,4);
    BOBUIest::newRow("1:2:3:4::") << "1:2:3:4::" << Ip6(1,2,3,4,0,0,0,0);

    BOBUIest::newRow("1::2:3:4") << "1::2:3:4" << Ip6(1,0,0,0,0,2,3,4);
    BOBUIest::newRow("1:2::3:4") << "1:2::3:4" << Ip6(1,2,0,0,0,0,3,4);
    BOBUIest::newRow("1:2:3::4") << "1:2:3::4" << Ip6(1,2,3,0,0,0,0,4);

    BOBUIest::newRow("::1:2:3") << "::1:2:3" << Ip6(0,0,0,0,0,1,2,3);
    BOBUIest::newRow("1:2:3::") << "1:2:3::" << Ip6(1,2,3,0,0,0,0,0);

    BOBUIest::newRow("1::2:3") << "1::2:3" << Ip6(1,0,0,0,0,0,2,3);
    BOBUIest::newRow("1:2::3") << "1:2::3" << Ip6(1,2,0,0,0,0,0,3);

    BOBUIest::newRow("::1:2") << "::1:2" << Ip6(0,0,0,0,0,0,1,2);
    BOBUIest::newRow("1:2::") << "1:2::" << Ip6(1,2,0,0,0,0,0,0);

    BOBUIest::newRow("1::2") << "1::2" << Ip6(1,0,0,0,0,0,0,2);

    BOBUIest::newRow("::1") << "::1" << Ip6(0,0,0,0,0,0,0,1);
    BOBUIest::newRow("1::") << "1::" << Ip6(1,0,0,0,0,0,0,0);

    BOBUIest::newRow("::") << "::" << Ip6(0,0,0,0,0,0,0,0);

    // embedded IPv4
    BOBUIest::newRow("1:2:3:4:5:6:10.0.16.1") << "1:2:3:4:5:6:10.0.16.1" << Ip6(1,2,3,4,5,6,0xa00,0x1001);
    BOBUIest::newRow("1::10.0.16.1") << "1::10.0.16.1" << Ip6(1,0,0,0,0,0,0xa00,0x1001);
    BOBUIest::newRow("::10.0.16.1") << "::10.0.16.1" << Ip6(0,0,0,0,0,0,0xa00,0x1001);
    BOBUIest::newRow("::0.0.0.0") << "::0.0.0.0" << Ip6(0,0,0,0,0,0,0,0);
}

void tst_QIpAddress::parseIp6()
{
    QFETCH(QString, address);
    QFETCH(Ip6, expected);

#if defined(__GLIBC__) && defined(AF_INET6)
    Ip6 inet_result;
    bool inet_ok = inet_pton(AF_INET6, address.toLatin1(), &inet_result.u8);
    QVERIFY(inet_ok);
    QCOMPARE(inet_result, expected);
#endif

    Ip6 result;
    bool ok = QIPAddressUtils::parseIp6(result.u8, address.constBegin(), address.constEnd()) == 0;
    QVERIFY(ok);
    QCOMPARE(result, expected);
}

void tst_QIpAddress::invalidParseIp6_data()
{
    BOBUIest::addColumn<QString>("address");

    // too many colons
    BOBUIest::newRow("0:0:0:0::0:0:0:0") << "0:0:0:0::0:0:0:0";
    BOBUIest::newRow("0:::") << "0:::"; BOBUIest::newRow(":::0") << ":::0";
    BOBUIest::newRow("16:::::::::::::::::::::::") << "16:::::::::::::::::::::::";

    // non-hex
    BOBUIest::newRow("a:b:c:d:e:f:g:h") << "a:b:c:d:e:f:g:h";

    // too big number
    BOBUIest::newRow("0:0:0:0:0:0:0:10103") << "0:0:0:0:0:0:0:10103";

    // too many zeroes
    BOBUIest::newRow("0:0:0:0:0:0:0:00103") << "0:0:0:0:0:0:0:00103";

    // too short
    BOBUIest::newRow("0:0:0:0:0:0:0:") << "0:0:0:0:0:0:0:";
    BOBUIest::newRow("0:0:0:0:0:0:0") << "0:0:0:0:0:0:0";
    BOBUIest::newRow("0:0:0:0:0:0:") << "0:0:0:0:0:0:";
    BOBUIest::newRow("0:0:0:0:0:0") << "0:0:0:0:0:0";
    BOBUIest::newRow("0:0:0:0:0:") << "0:0:0:0:0:";
    BOBUIest::newRow("0:0:0:0:0") << "0:0:0:0:0";
    BOBUIest::newRow("0:0:0:0:") << "0:0:0:0:";
    BOBUIest::newRow("0:0:0:0") << "0:0:0:0";
    BOBUIest::newRow("0:0:0:") << "0:0:0:";
    BOBUIest::newRow("0:0:0") << "0:0:0";
    BOBUIest::newRow("0:0:") << "0:0:";
    BOBUIest::newRow("0:0") << "0:0";
    BOBUIest::newRow("0:") << "0:";
    BOBUIest::newRow("0") << "0";
    BOBUIest::newRow(":0") << ":0";
    BOBUIest::newRow(":0:0") << ":0:0";
    BOBUIest::newRow(":0:0:0") << ":0:0:0";
    BOBUIest::newRow(":0:0:0:0") << ":0:0:0:0";
    BOBUIest::newRow(":0:0:0:0:0") << ":0:0:0:0:0";
    BOBUIest::newRow(":0:0:0:0:0:0") << ":0:0:0:0:0:0";
    BOBUIest::newRow(":0:0:0:0:0:0:0") << ":0:0:0:0:0:0:0";

    // IPv4
    BOBUIest::newRow("1.2.3.4") << "1.2.3.4";

    // embedded IPv4 in the wrong position
    BOBUIest::newRow("1.2.3.4::") << "1.2.3.4::";
    BOBUIest::newRow("f:1.2.3.4::") << "f:1.2.3.4::";
    BOBUIest::newRow("f:e:d:c:b:1.2.3.4:0") << "f:e:d:c:b:1.2.3.4:0";

    // bad embedded IPv4
    BOBUIest::newRow("::1.2.3") << "::1.2.3";
    BOBUIest::newRow("::1.2.257") << "::1.2.257";
    BOBUIest::newRow("::1.2") << "::1.2";
    BOBUIest::newRow("::0250.0x10101") << "::0250.0x10101";
    BOBUIest::newRow("::1.2.3.0250") << "::1.2.3.0250";
    BOBUIest::newRow("::1.2.3.0xff") << "::1.2.3.0xff";
    BOBUIest::newRow("::1.2.3.07") << "::1.2.3.07";
    BOBUIest::newRow("::1.2.3.010") << "::1.2.3.010";

    // separated by something else
    BOBUIest::newRow("1.2.3.4.5.6.7.8") << "1.2.3.4.5.6.7.8";
    BOBUIest::newRow("1,2,3,4,5,6,7,8") << "1,2,3,4,5,6,7,8";
    BOBUIest::newRow("1..2") << "1..2";
    BOBUIest::newRow("1:.2") << "1:.2";
    BOBUIest::newRow("1.:2") << "1.:2";
}

void tst_QIpAddress::invalidParseIp6()
{
    QFETCH(QString, address);

#if defined(__GLIBC__) && defined(AF_INET6)
    Ip6 inet_result;
    bool inet_ok = inet_pton(AF_INET6, address.toLatin1(), &inet_result.u8);
    if (__GLIBC_MINOR__ < 26)
        QEXPECT_FAIL("0:0:0:0:0:0:0:00103", "Bug fixed in glibc 2.26", Continue);
    QVERIFY(!inet_ok);
#endif

    Ip6 result;
    bool ok = QIPAddressUtils::parseIp6(result.u8, address.constBegin(), address.constEnd()) == 0;
    QVERIFY(!ok);
}

void tst_QIpAddress::ip6ToString_data()
{
    qRegisterMetaType<Ip6>();
    BOBUIest::addColumn<Ip6>("ip");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("1:2:3:4:5:6:7:8") << Ip6(1,2,3,4,5,6,7,8) << "1:2:3:4:5:6:7:8";
    BOBUIest::newRow("1:2:3:4:5:6:7:88") << Ip6(1,2,3,4,5,6,7,0x88) << "1:2:3:4:5:6:7:88";
    BOBUIest::newRow("1:2:3:4:5:6:7:888") << Ip6(1,2,3,4,5,6,7,0x888) << "1:2:3:4:5:6:7:888";
    BOBUIest::newRow("1:2:3:4:5:6:7:8888") << Ip6(1,2,3,4,5,6,7,0x8888) << "1:2:3:4:5:6:7:8888";
    BOBUIest::newRow("1:2:3:4:5:6:7:8880") << Ip6(1,2,3,4,5,6,7,0x8880) << "1:2:3:4:5:6:7:8880";
    BOBUIest::newRow("1:2:3:4:5:6:7:8808") << Ip6(1,2,3,4,5,6,7,0x8808) << "1:2:3:4:5:6:7:8808";
    BOBUIest::newRow("1:2:3:4:5:6:7:8088") << Ip6(1,2,3,4,5,6,7,0x8088) << "1:2:3:4:5:6:7:8088";

    BOBUIest::newRow("1:2:3:4:5:6:7:0") << Ip6(1,2,3,4,5,6,7,0) << "1:2:3:4:5:6:7:0";
    BOBUIest::newRow("0:1:2:3:4:5:6:7") << Ip6(0,1,2,3,4,5,6,7) << "0:1:2:3:4:5:6:7";

    BOBUIest::newRow("1:2:3:4:5:6::") << Ip6(1,2,3,4,5,6,0,0) << "1:2:3:4:5:6::";
    BOBUIest::newRow("::1:2:3:4:5:6") << Ip6(0,0,1,2,3,4,5,6) << "::1:2:3:4:5:6";
    BOBUIest::newRow("1:0:0:2::3") << Ip6(1,0,0,2,0,0,0,3) << "1:0:0:2::3";
    BOBUIest::newRow("1:::2:0:0:3") << Ip6(1,0,0,0,2,0,0,3) << "1::2:0:0:3";
    BOBUIest::newRow("1::2:0:0:0") << Ip6(1,0,0,0,2,0,0,0) << "1::2:0:0:0";
    BOBUIest::newRow("0:0:0:1::") << Ip6(0,0,0,1,0,0,0,0) << "0:0:0:1::";
    BOBUIest::newRow("::1:0:0:0") << Ip6(0,0,0,0,1,0,0,0) << "::1:0:0:0";
    BOBUIest::newRow("ff02::1") << Ip6(0xff02,0,0,0,0,0,0,1) << "ff02::1";
    BOBUIest::newRow("1::1") << Ip6(1,0,0,0,0,0,0,1) << "1::1";
    BOBUIest::newRow("::1") << Ip6(0,0,0,0,0,0,0,1) << "::1";
    BOBUIest::newRow("1::") << Ip6(1,0,0,0,0,0,0,0) << "1::";
    BOBUIest::newRow("::") << Ip6(0,0,0,0,0,0,0,0) << "::";

    BOBUIest::newRow("::1.2.3.4") << Ip6(0,0,0,0,0,0,0x102,0x304) << "::1.2.3.4";
    BOBUIest::newRow("::ffff:1.2.3.4") << Ip6(0,0,0,0,0,0xffff,0x102,0x304) << "::ffff:1.2.3.4";
}

void tst_QIpAddress::ip6ToString()
{
    QFETCH(Ip6, ip);
    QFETCH(QString, expected);

#if defined(__GLIBC__) && defined(AF_INET6)
    {
        char buf[INET6_ADDRSTRLEN];
        bool ok = inet_ntop(AF_INET6, ip.u8, buf, sizeof buf) != 0;
        QVERIFY(ok);
        QCOMPARE(QString(buf), expected);
    }
#endif

    QString result;
    QIPAddressUtils::toString(result, ip.u8);
    QCOMPARE(result, expected);
}

BOBUIEST_APPLESS_MAIN(tst_QIpAddress)

#include "tst_qipaddress.moc"
