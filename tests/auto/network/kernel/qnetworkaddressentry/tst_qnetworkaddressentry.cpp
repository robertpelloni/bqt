// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qcoreapplication.h>
#include <qnetworkinterface.h>

Q_DECLARE_METATYPE(QHostAddress)

class tst_QNetworkAddressEntry: public QObject
{
    Q_OBJECT
private slots:
    void getSetCheck();
    void prefixAndNetmask_data();
    void prefixAndNetmask();
};

void tst_QNetworkAddressEntry::getSetCheck()
{
    QNetworkAddressEntry entry;

    QVERIFY(entry.ip().isNull());
    QVERIFY(entry.netmask().isNull());
    QVERIFY(entry.broadcast().isNull());
    QCOMPARE(entry.prefixLength(), -1);

    entry.setIp(QHostAddress::LocalHost);
    QCOMPARE(entry.ip(), QHostAddress(QHostAddress::LocalHost));
    entry.setIp(QHostAddress());
    QVERIFY(entry.ip().isNull());

    entry.setBroadcast(QHostAddress::LocalHost);
    QCOMPARE(entry.broadcast(), QHostAddress(QHostAddress::LocalHost));
    entry.setBroadcast(QHostAddress());
    QVERIFY(entry.broadcast().isNull());

    // netmask and prefix length tested in the next test
    entry.setIp(QHostAddress::LocalHost);
    entry.setBroadcast(QHostAddress::LocalHost);

    QNetworkAddressEntry entry2;
    QVERIFY(entry != entry2);
    QVERIFY(!(entry == entry2));

    entry = entry2;
    QCOMPARE(entry, entry2);
    QCOMPARE(entry, entry);
    QVERIFY(!(entry != entry2));
}

void tst_QNetworkAddressEntry::prefixAndNetmask_data()
{
    BOBUIest::addColumn<QHostAddress>("ip");
    BOBUIest::addColumn<QHostAddress>("netmask");
    BOBUIest::addColumn<int>("prefix");

    // IPv4 set:
    QHostAddress ipv4(QHostAddress::LocalHost);
    BOBUIest::newRow("v4/0") << ipv4 << QHostAddress(QHostAddress::AnyIPv4) << 0;
    BOBUIest::newRow("v4/32") << ipv4 << QHostAddress("255.255.255.255") << 32;
    BOBUIest::newRow("v4/24") << ipv4 << QHostAddress("255.255.255.0") << 24;
    BOBUIest::newRow("v4/23") << ipv4 << QHostAddress("255.255.254.0") << 23;
    BOBUIest::newRow("v4/20") << ipv4 << QHostAddress("255.255.240.0") << 20;
    BOBUIest::newRow("v4/invalid1") << ipv4 << QHostAddress(QHostAddress::LocalHost) << -1;
    BOBUIest::newRow("v4/invalid2") << ipv4 << QHostAddress(QHostAddress::AnyIPv6) << -1;
    BOBUIest::newRow("v4/invalid3") << ipv4 << QHostAddress("255.255.253.0") << -1;
    BOBUIest::newRow("v4/invalid4") << ipv4 << QHostAddress() << -2;
    BOBUIest::newRow("v4/invalid5") << ipv4 << QHostAddress() << 33;

    // IPv6 set:
    QHostAddress ipv6(QHostAddress::LocalHostIPv6);
    BOBUIest::newRow("v6/0") << ipv6 << QHostAddress(QHostAddress::AnyIPv6) << 0;
    BOBUIest::newRow("v6/128") << ipv6 << QHostAddress("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff") << 128;
    BOBUIest::newRow("v6/64") << ipv6 << QHostAddress("ffff:ffff:ffff:ffff::") << 64;
    BOBUIest::newRow("v6/63") << ipv6 << QHostAddress("ffff:ffff:ffff:fffe::") << 63;
    BOBUIest::newRow("v6/60") << ipv6 << QHostAddress("ffff:ffff:ffff:fff0::") << 60;
    BOBUIest::newRow("v6/48") << ipv6 << QHostAddress("ffff:ffff:ffff::") << 48;
    BOBUIest::newRow("v6/3") << ipv6 << QHostAddress("e000::") << 3;
    BOBUIest::newRow("v6/invalid1") << ipv6 << QHostAddress(QHostAddress::LocalHostIPv6) << -1;
    BOBUIest::newRow("v6/invalid2") << ipv6 << QHostAddress(QHostAddress::Any) << -1;
    BOBUIest::newRow("v6/invalid3") << ipv6 << QHostAddress("fffd::") << -1;
    BOBUIest::newRow("v6/invalid4") << ipv6 << QHostAddress() << -2;
    BOBUIest::newRow("v6/invalid5") << ipv6 << QHostAddress() << 129;
}

void tst_QNetworkAddressEntry::prefixAndNetmask()
{
    QFETCH(QHostAddress, ip);
    QFETCH(QHostAddress, netmask);
    QFETCH(int, prefix);

    QNetworkAddressEntry entry;

    // first, without setting the IP, all must be invalid:
    entry.setNetmask(netmask);
    QVERIFY(entry.netmask().isNull());
    entry.setPrefixLength(prefix);
    QCOMPARE(entry.prefixLength(), -1);

    // set the IP:
    entry.setIp(ip);

    // set the netmask:
    if (!netmask.isNull()) {
        entry.setNetmask(netmask);

        // was it a valid one?
        if (prefix != -1) {
            QVERIFY(!entry.netmask().isNull());
            QCOMPARE(entry.netmask(), netmask);
            QCOMPARE(entry.prefixLength(), prefix);
        } else {
            // not valid
            QVERIFY(entry.netmask().isNull());
            QCOMPARE(entry.prefixLength(), -1);
        }
    }
    entry.setNetmask(QHostAddress());
    QVERIFY(entry.netmask().isNull());
    QCOMPARE(entry.prefixLength(), -1);

    // set the prefix
    if (prefix != -1) {
        entry.setPrefixLength(prefix);

        // was it a valid one?
        if (!netmask.isNull()) {
            QVERIFY(!entry.netmask().isNull());
            QCOMPARE(entry.netmask(), netmask);
            QCOMPARE(entry.prefixLength(), prefix);
        } else {
            // not valid
            QVERIFY(entry.netmask().isNull());
            QCOMPARE(entry.prefixLength(), -1);
        }
    }
    entry.setPrefixLength(-1);
    QVERIFY(entry.netmask().isNull());
    QCOMPARE(entry.prefixLength(), -1);
}

BOBUIEST_MAIN(tst_QNetworkAddressEntry)
#include "tst_qnetworkaddressentry.moc"

