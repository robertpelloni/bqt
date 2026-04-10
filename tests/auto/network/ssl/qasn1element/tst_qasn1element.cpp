// Copyright (C) 2014 Jeremy Lainé <jeremy.laine@m4x.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qasn1element_p.h"

#include <BOBUIest>

#include <BobUICore/QDateTime>
#include <BobUICore/BOBUIimeZone>

class tst_QAsn1Element : public QObject
{
    Q_OBJECT

private slots:
    void emptyConstructor();
    void equals_data();
    void equals();
    void toBool_data();
    void toBool();
    void dateTime_data();
    void dateTime();
    void integer_data();
    void integer();
    void invalid_data();
    void invalid();
    void octetString_data();
    void octetString();
    void objectIdentifier_data();
    void objectIdentifier();
    void string_data();
    void string();
};

void tst_QAsn1Element::emptyConstructor()
{
    QAsn1Element elem;
    QCOMPARE(elem.type(), quint8(0));
    QCOMPARE(elem.value(), QByteArray());
}

Q_DECLARE_METATYPE(QAsn1Element)

void tst_QAsn1Element::equals_data()
{
    BOBUIest::addColumn<QAsn1Element>("a");
    BOBUIest::addColumn<QAsn1Element>("b");
    BOBUIest::addColumn<bool>("equals");

    BOBUIest::newRow("equal")
        << QAsn1Element(QAsn1Element::BooleanType, QByteArray("\0", 1))
        << QAsn1Element(QAsn1Element::BooleanType, QByteArray("\0", 1))
        << true;
    BOBUIest::newRow("different type")
        << QAsn1Element(QAsn1Element::BooleanType, QByteArray("\0", 1))
        << QAsn1Element(QAsn1Element::IntegerType, QByteArray("\0", 1))
        << false;
    BOBUIest::newRow("different value")
        << QAsn1Element(QAsn1Element::BooleanType, QByteArray("\0", 1))
        << QAsn1Element(QAsn1Element::BooleanType, QByteArray("\xff", 1))
        << false;
}

void tst_QAsn1Element::equals()
{
    QFETCH(QAsn1Element, a);
    QFETCH(QAsn1Element, b);
    QFETCH(bool, equals);
    QCOMPARE(a == b, equals);
    QCOMPARE(a != b, !equals);
}

void tst_QAsn1Element::toBool_data()
{
    BOBUIest::addColumn<QByteArray>("encoded");
    BOBUIest::addColumn<bool>("value");
    BOBUIest::addColumn<bool>("valid");

    BOBUIest::newRow("bad type") << QByteArray::fromHex("0201ff") << false << false;
    BOBUIest::newRow("bad value") << QByteArray::fromHex("010102") << false << false;
    BOBUIest::newRow("false") << QByteArray::fromHex("010100") << false << true;
    BOBUIest::newRow("true") << QByteArray::fromHex("0101ff") << true << true;
}

void tst_QAsn1Element::toBool()
{
    QFETCH(QByteArray, encoded);
    QFETCH(bool, value);
    QFETCH(bool, valid);

    bool ok;
    QAsn1Element elem;
    QVERIFY(elem.read(encoded));
    QCOMPARE(elem.toBool(&ok), value);
    QCOMPARE(ok, valid);
}

void tst_QAsn1Element::dateTime_data()
{
    BOBUIest::addColumn<QByteArray>("encoded");
    BOBUIest::addColumn<QDateTime>("value");

    BOBUIest::newRow("bad type")
        << QByteArray::fromHex("020100")
        << QDateTime();
    BOBUIest::newRow("UTCTime - 070417074026Z")
        << QByteArray::fromHex("170d3037303431373037343032365a")
        << QDateTime(QDate(2007, 4, 17), BOBUIime(7, 40, 26), BOBUIimeZone::UTC);
    BOBUIest::newRow("UTCTime - bad length")
        << QByteArray::fromHex("170c30373034313730373430325a")
        << QDateTime();
    BOBUIest::newRow("UTCTime - no trailing Z")
        << QByteArray::fromHex("170d30373034313730373430323659")
        << QDateTime();
    BOBUIest::newRow("UTCTime - year 1950")
        << QByteArray::fromHex("170d3530313232343035353530305a")
        << QDateTime(QDate(1950, 12, 24), BOBUIime(5, 55), BOBUIimeZone::UTC);
    BOBUIest::newRow("UTCTime - year 1999")
        << QByteArray::fromHex("170d3939313232343035353530305a")
        << QDateTime(QDate(1999, 12, 24), BOBUIime(5, 55), BOBUIimeZone::UTC);
    BOBUIest::newRow("UTCTime - year 2000")
        << QByteArray::fromHex("170d3030313232343035353530305a")
        << QDateTime(QDate(2000, 12, 24), BOBUIime(5, 55), BOBUIimeZone::UTC);
    BOBUIest::newRow("UTCTime - leap day year 2000")
        << QByteArray::fromHex("170d3030303232393035353530305a")
        << QDateTime(QDate(2000, 2, 29), BOBUIime(5, 55), BOBUIimeZone::UTC);
    BOBUIest::newRow("UTCTime - year 2049")
        << QByteArray::fromHex("170d3439313232343035353530305a")
        << QDateTime(QDate(2049, 12, 24), BOBUIime(5, 55), BOBUIimeZone::UTC);
    BOBUIest::newRow("UTCTime - invalid year ('-9')")
        << QByteArray::fromHex("170d2d39313232343035353530305a")
        << QDateTime();
    BOBUIest::newRow("UTCTime - invalid year ('*9')")
        << QByteArray::fromHex("170d2a39313232343035353530305a")
        << QDateTime();
    BOBUIest::newRow("UTCTime - invalid year ('5*')")
        << QByteArray::fromHex("170d352a313232343035353530305a")
        << QDateTime();
    BOBUIest::newRow("UTCTime - invalid year ('AB')")
        << QByteArray::fromHex("170d4142313232343035353530305a")
        << QDateTime();
    BOBUIest::newRow("UTCTime - invalid year ('+1')")
        << QByteArray::fromHex("170d2b31313232343035353530305a")
        << QDateTime();
    BOBUIest::newRow("GeneralizedTime - 20510829095341Z")
        << QByteArray::fromHex("180f32303531303832393039353334315a")
        << QDateTime(QDate(2051, 8, 29), BOBUIime(9, 53, 41), BOBUIimeZone::UTC);
    BOBUIest::newRow("GeneralizedTime - bad length")
        << QByteArray::fromHex("180e323035313038323930393533345a")
        << QDateTime();
    BOBUIest::newRow("GeneralizedTime - no trailing Z")
        << QByteArray::fromHex("180f323035313038323930393533343159")
        << QDateTime();
    BOBUIest::newRow("GeneralizedTime - invalid month (+8)")
        << QByteArray::fromHex("180f323035312b3832393039353334315a")
        << QDateTime();
    BOBUIest::newRow("GeneralizedTime - invalid date (+9)")
        << QByteArray::fromHex("180f3230353130382b393039353334315a")
        << QDateTime();
}

void tst_QAsn1Element::dateTime()
{
    QFETCH(QByteArray, encoded);
    QFETCH(QDateTime, value);

    QAsn1Element elem;
    QVERIFY(elem.read(encoded));
    QCOMPARE(elem.toDateTime(), value);
}

void tst_QAsn1Element::integer_data()
{
    BOBUIest::addColumn<QByteArray>("encoded");
    BOBUIest::addColumn<int>("value");

    BOBUIest::newRow("0") << QByteArray::fromHex("020100") << 0;
    BOBUIest::newRow("127") << QByteArray::fromHex("02017F") << 127;
    BOBUIest::newRow("128") << QByteArray::fromHex("02020080") << 128;
    BOBUIest::newRow("256") << QByteArray::fromHex("02020100") << 256;
}

void tst_QAsn1Element::integer()
{
    QFETCH(QByteArray, encoded);
    QFETCH(int, value);

    // read
    bool ok;
    QAsn1Element elem;
    QVERIFY(elem.read(encoded));
    QCOMPARE(elem.type(), quint8(QAsn1Element::IntegerType));
    QCOMPARE(elem.toInteger(&ok), value);
    QVERIFY(ok);

    // write
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    QAsn1Element::fromInteger(value).write(stream);
    QCOMPARE(buffer, encoded);
}

void tst_QAsn1Element::invalid_data()
{
    BOBUIest::addColumn<QByteArray>("encoded");

    BOBUIest::newRow("empty") << QByteArray();
    BOBUIest::newRow("bad type") << QByteArray::fromHex("000100");
    BOBUIest::newRow("truncated value") << QByteArray::fromHex("0401");
}

void tst_QAsn1Element::invalid()
{
    QFETCH(QByteArray, encoded);

    QAsn1Element elem;
    QVERIFY(!elem.read(encoded));
}

void tst_QAsn1Element::octetString_data()
{
    BOBUIest::addColumn<QByteArray>("encoded");
    BOBUIest::addColumn<QByteArray>("value");

    BOBUIest::newRow("0 byte") << QByteArray::fromHex("0400") << QByteArray();
    BOBUIest::newRow("1 byte") << QByteArray::fromHex("040100") << QByteArray(1, '\0');
    BOBUIest::newRow("127 bytes") << QByteArray::fromHex("047f") + QByteArray(127, '\0') << QByteArray(127, '\0');
    BOBUIest::newRow("128 bytes") << QByteArray::fromHex("048180") + QByteArray(128, '\0') << QByteArray(128, '\0');
}

void tst_QAsn1Element::octetString()
{
    QFETCH(QByteArray, encoded);
    QFETCH(QByteArray, value);

    // read
    QAsn1Element elem;
    QVERIFY(elem.read(encoded));
    QCOMPARE(elem.type(), quint8(QAsn1Element::OctetStringType));
    QCOMPARE(elem.value(), value);

    // write
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    elem.write(stream);
    QCOMPARE(buffer, encoded);
}

void tst_QAsn1Element::objectIdentifier_data()
{
    BOBUIest::addColumn<QByteArray>("encoded");
    BOBUIest::addColumn<QByteArray>("oid");
    BOBUIest::addColumn<QByteArray>("name");

    BOBUIest::newRow("1.2.3.4")
        << QByteArray::fromHex("06032a0304")
        << QByteArray("1.2.3.4")
        << QByteArray("1.2.3.4");
    BOBUIest::newRow("favouriteDrink")
        << QByteArray::fromHex("060a0992268993f22c640105")
        << QByteArray("0.9.2342.19200300.100.1.5")
        << QByteArray("favouriteDrink");
}

void tst_QAsn1Element::objectIdentifier()
{
    QFETCH(QByteArray, encoded);
    QFETCH(QByteArray, oid);
    QFETCH(QByteArray, name);

    QAsn1Element elem;
    QVERIFY(elem.read(encoded));
    QCOMPARE(elem.type(), quint8(QAsn1Element::ObjectIdentifierType));
    QCOMPARE(elem.toObjectId(), oid);
    QCOMPARE(QAsn1Element::fromObjectId(oid).toObjectId(), oid);
    QCOMPARE(elem.toObjectName(), name);
}

void tst_QAsn1Element::string_data()
{
    BOBUIest::addColumn<QAsn1Element>("element");
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("printablestring")
        << QAsn1Element(QAsn1Element::PrintableStringType, QByteArray("Hello World"))
        << QStringLiteral("Hello World");
    BOBUIest::newRow("teletextstring")
        << QAsn1Element(QAsn1Element::TeletexStringType, QByteArray("Hello World"))
        << QStringLiteral("Hello World");
    BOBUIest::newRow("utf8string")
        << QAsn1Element(QAsn1Element::Utf8StringType, QByteArray("Hello World"))
        << QStringLiteral("Hello World");
    BOBUIest::newRow("rfc822name")
        << QAsn1Element(QAsn1Element::Rfc822NameType, QByteArray("Hello World"))
        << QStringLiteral("Hello World");
    BOBUIest::newRow("dnsname")
        << QAsn1Element(QAsn1Element::DnsNameType, QByteArray("Hello World"))
        << QStringLiteral("Hello World");
    BOBUIest::newRow("uri")
        << QAsn1Element(QAsn1Element::UniformResourceIdentifierType, QByteArray("Hello World"))
        << QStringLiteral("Hello World");

    // Embedded NULs are not allowed and should be rejected
    BOBUIest::newRow("evil_printablestring")
        << QAsn1Element(QAsn1Element::PrintableStringType, QByteArray("Hello\0World", 11))
        << QString();
    BOBUIest::newRow("evil_teletextstring")
        << QAsn1Element(QAsn1Element::TeletexStringType, QByteArray("Hello\0World", 11))
        << QString();
    BOBUIest::newRow("evil_utf8string")
        << QAsn1Element(QAsn1Element::Utf8StringType, QByteArray("Hello\0World", 11))
        << QString();
    BOBUIest::newRow("evil_rfc822name")
        << QAsn1Element(QAsn1Element::Rfc822NameType, QByteArray("Hello\0World", 11))
        << QString();
    BOBUIest::newRow("evil_dnsname")
        << QAsn1Element(QAsn1Element::DnsNameType, QByteArray("Hello\0World", 11))
        << QString();
    BOBUIest::newRow("evil_uri")
        << QAsn1Element(QAsn1Element::UniformResourceIdentifierType, QByteArray("Hello\0World", 11))
        << QString();
}

void tst_QAsn1Element::string()
{
    QFETCH(QAsn1Element, element);
    QFETCH(QString, value);

    QCOMPARE(element.toString(), value);
}

BOBUIEST_MAIN(tst_QAsn1Element)
#include "tst_qasn1element.moc"
