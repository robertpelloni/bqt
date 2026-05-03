// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/BobUICore>
#include <BOBUIest>
#include <BOBUIestEventLoop>

#include "common.h"
#include <limits>

#include <BobUIDBus/private/qdbusutil_p.h>
#include <BobUIDBus/private/qdbusconnection_p.h>
#include <BobUIDBus/private/qdbus_symbols_p.h>

#ifndef DBUS_TYPE_UNIX_FD
#  define DBUS_TYPE_UNIX_FD int('h')
#  define DBUS_TYPE_UNIX_FD_AS_STRING "h"
#endif

static const char serviceName[] = "org.bobuiproject.autotests.qpong";
static const char objectPath[] = "/org/bobuiproject/qpong";
static const char *interfaceName = serviceName;

class tst_QDBusMarshall: public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void sendBasic_data();
    void sendBasic();

    void sendVariant_data();
    void sendVariant();

    void sendArrays_data();
    void sendArrays();

    void sendArrayOfArrays_data();
    void sendArrayOfArrays();

    void sendMaps_data();
    void sendMaps();

    void sendStructs_data();
    void sendStructs();

    void sendComplex_data();
    void sendComplex();

    void sendArgument_data();
    void sendArgument();

    void sendSignalErrors();
    void sendCallErrors_data();
    void sendCallErrors();

    void receiveUnknownType_data();
    void receiveUnknownType();

    void demarshallPrimitives_data();
    void demarshallPrimitives();

    void demarshallStrings_data();
    void demarshallStrings();

    void demarshallInvalidStringList_data();
    void demarshallInvalidStringList();

    void demarshallInvalidByteArray_data();
    void demarshallInvalidByteArray();

private:
    int fileDescriptorForTest();

    QProcess proc;
    BOBUIemporaryFile tempFile;
    bool fileDescriptorPassing;
};

class QDBusMessageSpy: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE int theSlot(const QDBusMessage &msg)
    {
        list << msg;
        return 42;
    }
public:
    QList<QDBusMessage> list;
};

struct UnregisteredType { };
Q_DECLARE_METATYPE(UnregisteredType)

void tst_QDBusMarshall::initTestCase()
{
    commonInit();
    QDBusConnection con = QDBusConnection::sessionBus();
    fileDescriptorPassing = con.connectionCapabilities() & QDBusConnection::UnixFileDescriptorPassing;

#ifdef Q_OS_WIN
#  define EXE ".exe"
#else
#  define EXE ""
#endif
    proc.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    proc.start(QFINDTESTDATA("qpong/qpong" EXE));
    QVERIFY2(proc.waitForStarted(), qPrintable(proc.errorString()));
    QVERIFY(proc.waitForReadyRead());
}

void tst_QDBusMarshall::cleanupTestCase()
{
    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName, objectPath, interfaceName, "quit");
    QDBusConnection::sessionBus().call(msg);
    proc.waitForFinished(200);
    proc.close();
}

int tst_QDBusMarshall::fileDescriptorForTest()
{
    if (!tempFile.isOpen()) {
        tempFile.setFileTemplate(QDir::tempPath() + "/qdbusmarshalltestXXXXXX.tmp");
        if (!tempFile.open()) {
            qWarning("%s: Cannot create temporary file: %s", Q_FUNC_INFO,
                     qPrintable(tempFile.errorString()));
            return 0;
        }
    }
    return tempFile.handle();
}

void addBasicTypesColumns()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<QString>("stringResult");
}

void basicNumericTypes_data()
{
    BOBUIest::newRow("bool") << QVariant(false) << "b" << "false";
    BOBUIest::newRow("bool2") << QVariant(true) << "b" << "true";
    BOBUIest::newRow("byte") << QVariant::fromValue(uchar(1)) << "y" << "1";
    BOBUIest::newRow("int16") << QVariant::fromValue(short(2)) << "n" << "2";
    BOBUIest::newRow("uint16") << QVariant::fromValue(ushort(3)) << "q" << "3";
    BOBUIest::newRow("int") << QVariant(1) << "i" << "1";
    BOBUIest::newRow("uint") << QVariant(2U) << "u" << "2";
    BOBUIest::newRow("int64") << QVariant(Q_INT64_C(3)) << "x" << "3";
    BOBUIest::newRow("uint64") << QVariant(Q_UINT64_C(4)) << "t" << "4";
    BOBUIest::newRow("double") << QVariant(42.5) << "d" << "42.5";
}

void basicStringTypes_data()
{
    BOBUIest::newRow("string") << QVariant("ping") << "s" << "\"ping\"";
    BOBUIest::newRow("objectpath") << QVariant::fromValue(QDBusObjectPath("/org/kde")) << "o" << "[ObjectPath: /org/kde]";
    BOBUIest::newRow("emptysignature") << QVariant::fromValue(QDBusSignature(QString())) << "g" << "[Signature: ]";
    BOBUIest::newRow("signature") << QVariant::fromValue(QDBusSignature("g")) << "g" << "[Signature: g]";
    BOBUIest::newRow("multisignature") << QVariant::fromValue(QDBusSignature("bit")) << "g" << "[Signature: bit]";
    BOBUIest::newRow("emptystring") << QVariant("") << "s" << "\"\"";
    BOBUIest::newRow("nullstring") << QVariant(QString()) << "s" << "\"\"";
}

void tst_QDBusMarshall::sendBasic_data()
{
    addBasicTypesColumns();

    // basic types:
    basicNumericTypes_data();
    basicStringTypes_data();

    if (fileDescriptorPassing)
        BOBUIest::newRow("file-descriptor") << QVariant::fromValue(QDBusUnixFileDescriptor(fileDescriptorForTest())) << "h" << "[Unix FD: valid]";
}

void tst_QDBusMarshall::sendVariant_data()
{
    sendBasic_data();

    BOBUIest::newRow("variant") << QVariant::fromValue(QDBusVariant(1)) << "v" << "[Variant(int): 1]";

    QDBusVariant nested(1);
    BOBUIest::newRow("variant-variant") << QVariant::fromValue(QDBusVariant(QVariant::fromValue(nested))) << "v"
                                     << "[Variant(QDBusVariant): [Variant(int): 1]]";
}

void tst_QDBusMarshall::sendArrays_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<QString>("stringResult");

    // arrays
    QStringList strings;
    BOBUIest::newRow("emptystringlist") << QVariant(strings) << "as" << "{}";
    strings << "hello" << "world";
    BOBUIest::newRow("stringlist") << QVariant(strings) << "as" << "{\"hello\", \"world\"}";

    strings.clear();
    strings << "" << "" << "";
    BOBUIest::newRow("list-of-emptystrings") << QVariant(strings) << "as" << "{\"\", \"\", \"\"}";

    strings.clear();
    strings << QString() << QString() << QString() << QString();
    BOBUIest::newRow("list-of-nullstrings") << QVariant(strings) << "as" << "{\"\", \"\", \"\", \"\"}";

    QByteArray bytearray;
    BOBUIest::newRow("nullbytearray") << QVariant(bytearray) << "ay" << "{}";
    bytearray = "";             // empty, not null
    BOBUIest::newRow("emptybytearray") << QVariant(bytearray) << "ay" << "{}";
    bytearray = "foo";
    BOBUIest::newRow("bytearray") << QVariant(bytearray) << "ay" << "{102, 111, 111}";

    QList<bool> bools;
    BOBUIest::newRow("emptyboollist") << QVariant::fromValue(bools) << "ab" << "[Argument: ab {}]";
    bools << false << true << false;
    BOBUIest::newRow("boollist") << QVariant::fromValue(bools) << "ab" << "[Argument: ab {false, true, false}]";

    QList<short> shorts;
    BOBUIest::newRow("emptyshortlist") << QVariant::fromValue(shorts) << "an" << "[Argument: an {}]";
    shorts << 42 << -43 << 44 << 45 << -32768 << 32767;
    BOBUIest::newRow("shortlist") << QVariant::fromValue(shorts) << "an"
                               << "[Argument: an {42, -43, 44, 45, -32768, 32767}]";

    QList<ushort> ushorts;
    BOBUIest::newRow("emptyushortlist") << QVariant::fromValue(ushorts) << "aq" << "[Argument: aq {}]";
    ushorts << 12u << 13u << 14u << 15 << 65535;
    BOBUIest::newRow("ushortlist") << QVariant::fromValue(ushorts) << "aq" << "[Argument: aq {12, 13, 14, 15, 65535}]";

    QList<int> ints;
    BOBUIest::newRow("emptyintlist") << QVariant::fromValue(ints) << "ai" << "[Argument: ai {}]";
    ints << 42 << -43 << 44 << 45 << 2147483647 << -2147483647-1;
    BOBUIest::newRow("intlist") << QVariant::fromValue(ints) << "ai" << "[Argument: ai {42, -43, 44, 45, 2147483647, -2147483648}]";

    QList<uint> uints;
    BOBUIest::newRow("emptyuintlist") << QVariant::fromValue(uints) << "au" << "[Argument: au {}]";
    uints << uint(12) << uint(13) << uint(14) << 4294967295U;
    BOBUIest::newRow("uintlist") << QVariant::fromValue(uints) << "au" << "[Argument: au {12, 13, 14, 4294967295}]";

    QList<qlonglong> llints;
    BOBUIest::newRow("emptyllintlist") << QVariant::fromValue(llints) << "ax" << "[Argument: ax {}]";
    llints << Q_INT64_C(99) << Q_INT64_C(-100)
           << Q_INT64_C(-9223372036854775807)-1 << Q_INT64_C(9223372036854775807);
    BOBUIest::newRow("llintlist") << QVariant::fromValue(llints) << "ax"
                               << "[Argument: ax {99, -100, -9223372036854775808, 9223372036854775807}]";

    QList<qulonglong> ullints;
    BOBUIest::newRow("emptyullintlist") << QVariant::fromValue(ullints) << "at" << "[Argument: at {}]";
    ullints << Q_UINT64_C(66) << Q_UINT64_C(67)
            << Q_UINT64_C(18446744073709551615);
    BOBUIest::newRow("ullintlist") << QVariant::fromValue(ullints) << "at" << "[Argument: at {66, 67, 18446744073709551615}]";

    QList<double> doubles;
    BOBUIest::newRow("emptydoublelist") << QVariant::fromValue(doubles) << "ad" << "[Argument: ad {}]";
    doubles << 1.2 << 2.2 << 4.4
            << -std::numeric_limits<double>::infinity()
            << std::numeric_limits<double>::infinity()
            << std::numeric_limits<double>::quiet_NaN();
    BOBUIest::newRow("doublelist") << QVariant::fromValue(doubles) << "ad" << "[Argument: ad {1.2, 2.2, 4.4, -inf, inf, nan}]";

    QList<QDBusObjectPath> objectPaths;
    BOBUIest::newRow("emptyobjectpathlist") << QVariant::fromValue(objectPaths) << "ao" << "[Argument: ao {}]";
    objectPaths << QDBusObjectPath("/") << QDBusObjectPath("/foo");
    BOBUIest::newRow("objectpathlist") << QVariant::fromValue(objectPaths) << "ao" << "[Argument: ao {[ObjectPath: /], [ObjectPath: /foo]}]";

    if (fileDescriptorPassing) {
        QList<QDBusUnixFileDescriptor> fileDescriptors;
        BOBUIest::newRow("emptyfiledescriptorlist") << QVariant::fromValue(fileDescriptors) << "ah" << "[Argument: ah {}]";
        fileDescriptors << QDBusUnixFileDescriptor(fileDescriptorForTest()) << QDBusUnixFileDescriptor(1);
        BOBUIest::newRow("filedescriptorlist") << QVariant::fromValue(fileDescriptors) << "ah" << "[Argument: ah {[Unix FD: valid], [Unix FD: valid]}]";
    }

    QVariantList variants;
    BOBUIest::newRow("emptyvariantlist") << QVariant(variants) << "av" << "[Argument: av {}]";
    variants << QString("Hello") << QByteArray("World") << 42 << -43.0 << 44U << Q_INT64_C(-45)
             << Q_UINT64_C(46) << true << QVariant::fromValue(short(-47))
             << QVariant::fromValue(QDBusSignature("av"))
             << QVariant::fromValue(QDBusVariant(QVariant::fromValue(QDBusObjectPath("/"))));
    BOBUIest::newRow("variantlist") << QVariant(variants) << "av"
        << "[Argument: av {[Variant(QString): \"Hello\"], [Variant(QByteArray): {87, 111, 114, 108, 100}], [Variant(int): 42], [Variant(double): -43], [Variant(uint): 44], [Variant(qlonglong): -45], [Variant(qulonglong): 46], [Variant(bool): true], [Variant(short): -47], [Variant: [Signature: av]], [Variant: [Variant: [ObjectPath: /]]]}]";
}

void tst_QDBusMarshall::sendArrayOfArrays_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<QString>("stringResult");

    // arrays:
    QList<QStringList> strings;
    BOBUIest::newRow("empty-list-of-stringlist") << QVariant::fromValue(strings) << "aas"
            << "[Argument: aas {}]";
    strings << QStringList();
    BOBUIest::newRow("list-of-emptystringlist") << QVariant::fromValue(strings) << "aas"
            << "[Argument: aas {{}}]";
    strings << (QStringList() << "hello" << "world")
            << (QStringList() << "hi" << "there")
            << (QStringList() << QString());
    BOBUIest::newRow("stringlist") << QVariant::fromValue(strings) << "aas"
            << "[Argument: aas {{}, {\"hello\", \"world\"}, {\"hi\", \"there\"}, {\"\"}}]";

    QList<QByteArray> bytearray;
    BOBUIest::newRow("empty-list-of-bytearray") << QVariant::fromValue(bytearray) << "aay"
            << "[Argument: aay {}]";
    bytearray << QByteArray();
    BOBUIest::newRow("list-of-emptybytearray") << QVariant::fromValue(bytearray) << "aay"
            << "[Argument: aay {{}}]";
    bytearray << "foo" << "bar" << "baz" << "" << QByteArray();
    BOBUIest::newRow("bytearray") << QVariant::fromValue(bytearray) << "aay"
            << "[Argument: aay {{}, {102, 111, 111}, {98, 97, 114}, {98, 97, 122}, {}, {}}]";

    QList<QList<bool> > bools;
    BOBUIest::newRow("empty-list-of-boollist") << QVariant::fromValue(bools) << "aab"
            << "[Argument: aab {}]";
    bools << QList<bool>();
    BOBUIest::newRow("list-of-emptyboollist") << QVariant::fromValue(bools) << "aab"
            << "[Argument: aab {[Argument: ab {}]}]";
    bools << (QList<bool>() << false << true) << (QList<bool>() << false) << (QList<bool>());
    BOBUIest::newRow("boollist") << QVariant::fromValue(bools) << "aab"
            << "[Argument: aab {[Argument: ab {}], [Argument: ab {false, true}], [Argument: ab {false}], [Argument: ab {}]}]";
    QList<QList<short> > shorts;
    BOBUIest::newRow("empty-list-of-shortlist") << QVariant::fromValue(shorts) << "aan"
            << "[Argument: aan {}]";
    shorts << QList<short>();
    BOBUIest::newRow("list-of-emptyshortlist") << QVariant::fromValue(shorts) << "aan"
            << "[Argument: aan {[Argument: an {}]}]";
    shorts << (QList<short>() << 42 << -43 << 44 << 45)
           << (QList<short>() << -32768 << 32767)
           << (QList<short>());
    BOBUIest::newRow("shortlist") << QVariant::fromValue(shorts) << "aan"
            << "[Argument: aan {[Argument: an {}], [Argument: an {42, -43, 44, 45}], [Argument: an {-32768, 32767}], [Argument: an {}]}]";

    QList<QList<ushort> > ushorts;
    BOBUIest::newRow("empty-list-of-ushortlist") << QVariant::fromValue(ushorts) << "aaq"
            << "[Argument: aaq {}]";
    ushorts << QList<ushort>();
    BOBUIest::newRow("list-of-emptyushortlist") << QVariant::fromValue(ushorts) << "aaq"
            << "[Argument: aaq {[Argument: aq {}]}]";
    ushorts << (QList<ushort>() << 12u << 13u << 14u << 15)
            << (QList<ushort>() << 65535)
            << (QList<ushort>());
    BOBUIest::newRow("ushortlist") << QVariant::fromValue(ushorts) << "aaq"
            << "[Argument: aaq {[Argument: aq {}], [Argument: aq {12, 13, 14, 15}], [Argument: aq {65535}], [Argument: aq {}]}]";

    QList<QList<int> > ints;
    BOBUIest::newRow("empty-list-of-intlist") << QVariant::fromValue(ints) << "aai"
            << "[Argument: aai {}]";
    ints << QList<int>();
    BOBUIest::newRow("list-of-emptyintlist") << QVariant::fromValue(ints) << "aai"
            << "[Argument: aai {[Argument: ai {}]}]";
    ints << (QList<int>() << 42 << -43 << 44 << 45)
         << (QList<int>() << 2147483647 << -2147483647-1)
         << (QList<int>());
    BOBUIest::newRow("intlist") << QVariant::fromValue(ints) << "aai"
            << "[Argument: aai {[Argument: ai {}], [Argument: ai {42, -43, 44, 45}], [Argument: ai {2147483647, -2147483648}], [Argument: ai {}]}]";

    QList<QList<uint> > uints;
    BOBUIest::newRow("empty-list-of-uintlist") << QVariant::fromValue(uints) << "aau"
            << "[Argument: aau {}]";
    uints << QList<uint>();
    BOBUIest::newRow("list-of-emptyuintlist") << QVariant::fromValue(uints) << "aau"
            << "[Argument: aau {[Argument: au {}]}]";
    uints << (QList<uint>() << uint(12) << uint(13) << uint(14))
          << (QList<uint>() << 4294967295U)
          << (QList<uint>());
    BOBUIest::newRow("uintlist") << QVariant::fromValue(uints) << "aau"
            << "[Argument: aau {[Argument: au {}], [Argument: au {12, 13, 14}], [Argument: au {4294967295}], [Argument: au {}]}]";

    QList<QList<qlonglong> > llints;
    BOBUIest::newRow("empty-list-of-llintlist") << QVariant::fromValue(llints) << "aax"
            << "[Argument: aax {}]";
    llints << QList<qlonglong>();
    BOBUIest::newRow("list-of-emptyllintlist") << QVariant::fromValue(llints) << "aax"
            << "[Argument: aax {[Argument: ax {}]}]";
    llints << (QList<qlonglong>() << Q_INT64_C(99) << Q_INT64_C(-100))
           << (QList<qlonglong>() << Q_INT64_C(-9223372036854775807)-1 << Q_INT64_C(9223372036854775807))
           << (QList<qlonglong>());
    BOBUIest::newRow("llintlist") << QVariant::fromValue(llints) << "aax"
            << "[Argument: aax {[Argument: ax {}], [Argument: ax {99, -100}], [Argument: ax {-9223372036854775808, 9223372036854775807}], [Argument: ax {}]}]";

    QList<QList<qulonglong> > ullints;
    BOBUIest::newRow("empty-list-of-ullintlist") << QVariant::fromValue(ullints) << "aat"
            << "[Argument: aat {}]";
    ullints << QList<qulonglong>();
    BOBUIest::newRow("list-of-emptyullintlist") << QVariant::fromValue(ullints) << "aat"
            << "[Argument: aat {[Argument: at {}]}]";
    ullints << (QList<qulonglong>() << Q_UINT64_C(66) << Q_UINT64_C(67))
            << (QList<qulonglong>() << Q_UINT64_C(18446744073709551615))
            << (QList<qulonglong>());
    BOBUIest::newRow("ullintlist") << QVariant::fromValue(ullints) << "aat"
            << "[Argument: aat {[Argument: at {}], [Argument: at {66, 67}], [Argument: at {18446744073709551615}], [Argument: at {}]}]";

    QList<QList<double> > doubles;
    BOBUIest::newRow("empty-list-ofdoublelist") << QVariant::fromValue(doubles) << "aad"
            << "[Argument: aad {}]";
    doubles << QList<double>();
    BOBUIest::newRow("list-of-emptydoublelist") << QVariant::fromValue(doubles) << "aad"
            << "[Argument: aad {[Argument: ad {}]}]";
    doubles << (QList<double>() << 1.2 << 2.2 << 4.4)
            << (QList<double>() << -std::numeric_limits<double>::infinity()
                << std::numeric_limits<double>::infinity()
                << std::numeric_limits<double>::quiet_NaN())
            << (QList<double>());
    BOBUIest::newRow("doublelist") << QVariant::fromValue(doubles) << "aad"
            << "[Argument: aad {[Argument: ad {}], [Argument: ad {1.2, 2.2, 4.4}], [Argument: ad {-inf, inf, nan}], [Argument: ad {}]}]";

    QList<QVariantList> variants;
    BOBUIest::newRow("emptyvariantlist") << QVariant::fromValue(variants) << "aav"
            << "[Argument: aav {}]";
    variants << QVariantList();
    BOBUIest::newRow("variantlist-empty-variantlist-element") << QVariant::fromValue(variants) << "aav"
            << "[Argument: aav {[Argument: av {}]}]";
    variants << (QVariantList() << QString("Hello") << QByteArray("World"))
             << (QVariantList() << 42 << -43.0 << 44U << Q_INT64_C(-45))
             << (QVariantList() << Q_UINT64_C(46) << true << QVariant::fromValue(short(-47)));
    BOBUIest::newRow("variantlist") << QVariant::fromValue(variants) << "aav"
            << "[Argument: aav {[Argument: av {}], [Argument: av {[Variant(QString): \"Hello\"], [Variant(QByteArray): {87, 111, 114, 108, 100}]}], [Argument: av {[Variant(int): 42], [Variant(double): -43], [Variant(uint): 44], [Variant(qlonglong): -45]}], [Argument: av {[Variant(qulonglong): 46], [Variant(bool): true], [Variant(short): -47]}]}]";
}

void tst_QDBusMarshall::sendMaps_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<QString>("stringResult");

    QMap<int, QString> ismap;
    BOBUIest::newRow("empty-is-map") << QVariant::fromValue(ismap) << "a{is}"
            << "[Argument: a{is} {}]";
    ismap[1] = "a";
    ismap[2000] = "b";
    ismap[-47] = "c";
    BOBUIest::newRow("is-map") << QVariant::fromValue(ismap) << "a{is}"
            << "[Argument: a{is} {-47 = \"c\", 1 = \"a\", 2000 = \"b\"}]";

    QMap<QString, QString> ssmap;
    BOBUIest::newRow("empty-ss-map") << QVariant::fromValue(ssmap) << "a{ss}"
            << "[Argument: a{ss} {}]";
    ssmap["a"] = "a";
    ssmap["c"] = "b";
    ssmap["b"] = "c";
    BOBUIest::newRow("ss-map") << QVariant::fromValue(ssmap) << "a{ss}"
            << "[Argument: a{ss} {\"a\" = \"a\", \"b\" = \"c\", \"c\" = \"b\"}]";

    QVariantMap svmap;
    BOBUIest::newRow("empty-sv-map") << QVariant::fromValue(svmap) << "a{sv}"
            << "[Argument: a{sv} {}]";
    svmap["a"] = 1;
    svmap["c"] = "b";
    svmap["b"] = QByteArray("c");
    svmap["d"] = 42U;
    svmap["e"] = QVariant::fromValue(short(-47));
    svmap["f"] = QVariant::fromValue(QDBusVariant(0));
    BOBUIest::newRow("sv-map1") << QVariant::fromValue(svmap) << "a{sv}"
            << "[Argument: a{sv} {\"a\" = [Variant(int): 1], \"b\" = [Variant(QByteArray): {99}], \"c\" = [Variant(QString): \"b\"], \"d\" = [Variant(uint): 42], \"e\" = [Variant(short): -47], \"f\" = [Variant: [Variant(int): 0]]}]";

    QMap<QDBusObjectPath, QString> osmap;
    BOBUIest::newRow("empty-os-map") << QVariant::fromValue(osmap) << "a{os}"
            << "[Argument: a{os} {}]";
    osmap[QDBusObjectPath("/")] = "root";
    osmap[QDBusObjectPath("/foo")] = "foo";
    osmap[QDBusObjectPath("/bar/baz")] = "bar and baz";
    BOBUIest::newRow("os-map") << QVariant::fromValue(osmap) << "a{os}"
            << "[Argument: a{os} {[ObjectPath: /] = \"root\", [ObjectPath: /bar/baz] = \"bar and baz\", [ObjectPath: /foo] = \"foo\"}]";

    QMap<QDBusSignature, QString> gsmap;
    BOBUIest::newRow("empty-gs-map") << QVariant::fromValue(gsmap) << "a{gs}"
            << "[Argument: a{gs} {}]";
    gsmap[QDBusSignature("i")] = "int32";
    gsmap[QDBusSignature("s")] = "string";
    gsmap[QDBusSignature("a{gs}")] = "array of dict_entry of (signature, string)";
    BOBUIest::newRow("gs-map") << QVariant::fromValue(gsmap) << "a{gs}"
            << "[Argument: a{gs} {[Signature: a{gs}] = \"array of dict_entry of (signature, string)\", [Signature: i] = \"int32\", [Signature: s] = \"string\"}]";

    QMap<QString, std::pair<int, int>> siimap;
    BOBUIest::newRow("empty-sii-map") << QVariant::fromValue(siimap) << "a{s(ii)}"
                                   << "[Argument: a{s(ii)} {}]";
    siimap["0,0"] = { 0, 0 };
    siimap["1,-1"] = { 1, -1 };
    BOBUIest::newRow("sii-map") << QVariant::fromValue(siimap) << "a{s(ii)}"
                             << "[Argument: a{s(ii)} {"
                                    "\"0,0\" = [Argument: (ii) 0, 0], "
                                    "\"1,-1\" = [Argument: (ii) 1, -1]"
                                "}]";

    if (fileDescriptorPassing) {
        svmap["zzfiledescriptor"] = QVariant::fromValue(QDBusUnixFileDescriptor(fileDescriptorForTest()));
        BOBUIest::newRow("sv-map1-fd") << QVariant::fromValue(svmap) << "a{sv}"
                                    << "[Argument: a{sv} {\"a\" = [Variant(int): 1], \"b\" = [Variant(QByteArray): {99}], \"c\" = [Variant(QString): \"b\"], \"d\" = [Variant(uint): 42], \"e\" = [Variant(short): -47], \"f\" = [Variant: [Variant(int): 0]], \"zzfiledescriptor\" = [Variant(QDBusUnixFileDescriptor): [Unix FD: valid]]}]";
    }

    svmap.clear();
    svmap["ismap"] = QVariant::fromValue(ismap);
    svmap["ssmap"] = QVariant::fromValue(ssmap);
    svmap["osmap"] = QVariant::fromValue(osmap);
    svmap["gsmap"] = QVariant::fromValue(gsmap);
    BOBUIest::newRow("sv-map2") << QVariant::fromValue(svmap) << "a{sv}"
            << "[Argument: a{sv} {\"gsmap\" = [Variant: [Argument: a{gs} {[Signature: a{gs}] = \"array of dict_entry of (signature, string)\", [Signature: i] = \"int32\", [Signature: s] = \"string\"}]], \"ismap\" = [Variant: [Argument: a{is} {-47 = \"c\", 1 = \"a\", 2000 = \"b\"}]], \"osmap\" = [Variant: [Argument: a{os} {[ObjectPath: /] = \"root\", [ObjectPath: /bar/baz] = \"bar and baz\", [ObjectPath: /foo] = \"foo\"}]], \"ssmap\" = [Variant: [Argument: a{ss} {\"a\" = \"a\", \"b\" = \"c\", \"c\" = \"b\"}]]}]";
}

void tst_QDBusMarshall::sendStructs_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<QString>("stringResult");

    BOBUIest::newRow("point") << QVariant(QPoint(1, 2)) << "(ii)" << "[Argument: (ii) 1, 2]";
    BOBUIest::newRow("pointf") << QVariant(QPointF(1.5, -1.5)) << "(dd)" << "[Argument: (dd) 1.5, -1.5]";

    BOBUIest::newRow("size") << QVariant(QSize(1, 2)) << "(ii)" << "[Argument: (ii) 1, 2]";
    BOBUIest::newRow("sizef") << QVariant(QSizeF(1.5, 1.5)) << "(dd)" << "[Argument: (dd) 1.5, 1.5]";

    BOBUIest::newRow("rect") << QVariant(QRect(1, 2, 3, 4)) << "(iiii)" << "[Argument: (iiii) 1, 2, 3, 4]";
    BOBUIest::newRow("rectf") << QVariant(QRectF(0.5, 0.5, 1.5, 1.5)) << "(dddd)" << "[Argument: (dddd) 0.5, 0.5, 1.5, 1.5]";

    BOBUIest::newRow("line") << QVariant(QLine(1, 2, 3, 4)) << "((ii)(ii))"
                          << "[Argument: ((ii)(ii)) [Argument: (ii) 1, 2], [Argument: (ii) 3, 4]]";
    BOBUIest::newRow("linef") << QVariant(QLineF(0.5, 0.5, 1.5, 1.5)) << "((dd)(dd))"
                           << "[Argument: ((dd)(dd)) [Argument: (dd) 0.5, 0.5], [Argument: (dd) 1.5, 1.5]]";

    QDate date(2006, 6, 18);
    BOBUIime time(12, 25, 00);     // the date I wrote this test on :-)
    BOBUIest::newRow("date") << QVariant(date) << "(iii)" << "[Argument: (iii) 2006, 6, 18]";
    BOBUIest::newRow("time") << QVariant(time) << "(iiii)" << "[Argument: (iiii) 12, 25, 0, 0]";
    BOBUIest::newRow("datetime") << QVariant(QDateTime(date, time)) << "((iii)(iiii)i)"
        << "[Argument: ((iii)(iiii)i) [Argument: (iii) 2006, 6, 18], [Argument: (iiii) 12, 25, 0, 0], 0]";

    MyStruct ms = { 1, "Hello, World" };
    BOBUIest::newRow("int-string") << QVariant::fromValue(ms) << "(is)" << "[Argument: (is) 1, \"Hello, World\"]";

    MyVariantMapStruct mvms = { "Hello, World", QVariantMap() };
    BOBUIest::newRow("string-variantmap") << QVariant::fromValue(mvms) << "(sa{sv})" << "[Argument: (sa{sv}) \"Hello, World\", [Argument: a{sv} {}]]";

    // use only basic types, otherwise comparison will fail
    mvms.map["int"] = 42;
    mvms.map["uint"] = 42u;
    mvms.map["short"] = QVariant::fromValue<short>(-47);
    mvms.map["bytearray"] = QByteArray("Hello, world");
    BOBUIest::newRow("string-variantmap2") << QVariant::fromValue(mvms) << "(sa{sv})" << "[Argument: (sa{sv}) \"Hello, World\", [Argument: a{sv} {\"bytearray\" = [Variant(QByteArray): {72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100}], \"int\" = [Variant(int): 42], \"short\" = [Variant(short): -47], \"uint\" = [Variant(uint): 42]}]]";

    QList<MyVariantMapStruct> list;
    BOBUIest::newRow("empty-list-of-string-variantmap") << QVariant::fromValue(list) << "a(sa{sv})" << "[Argument: a(sa{sv}) {}]";
    list << mvms;
    BOBUIest::newRow("list-of-string-variantmap") << QVariant::fromValue(list) << "a(sa{sv})" << "[Argument: a(sa{sv}) {[Argument: (sa{sv}) \"Hello, World\", [Argument: a{sv} {\"bytearray\" = [Variant(QByteArray): {72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100}], \"int\" = [Variant(int): 42], \"short\" = [Variant(short): -47], \"uint\" = [Variant(uint): 42]}]]}]";

    BOBUIest::newRow("std::tuple<int>")
            << QVariant::fromValue(std::tuple<int>{ 1 }) << "(i)" << "[Argument: (i) 1]";
    BOBUIest::newRow("std::tuple<QString>") << QVariant::fromValue(std::tuple<QString>{ "foo" })
                                         << "(s)" << "[Argument: (s) \"foo\"]";
    BOBUIest::newRow("std::tuple<QVariantMap>")
            << QVariant::fromValue(std::tuple<QVariantMap>{ { { "foo", 1 } } }) << "(a{sv})"
            << "[Argument: (a{sv}) [Argument: a{sv} {\"foo\" = [Variant(int): 1]}]]";
    BOBUIest::newRow("std::tuple<QPoint>") << QVariant::fromValue(std::tuple<QPoint>{ { 1, 2 } })
                                        << "((ii))" << "[Argument: ((ii)) [Argument: (ii) 1, 2]]";
    BOBUIest::newRow("std::tuple<std::tuple<int>>")
            << QVariant::fromValue(std::tuple<std::tuple<int>>{ 1 }) << "((i))"
            << "[Argument: ((i)) [Argument: (i) 1]]";
    BOBUIest::newRow("std::tuple<QList<int>>")
            << QVariant::fromValue(std::tuple<QList<int>>{ { 1, 2, 3 } }) << "(ai)"
            << "[Argument: (ai) [Argument: ai {1, 2, 3}]]";
    BOBUIest::newRow("std::tuple<int, QString, QVariantMap>")
            << QVariant::fromValue(
                       std::tuple<int, QString, QVariantMap>{ 1, "foo", { { "bar", 2 } } })
            << "(isa{sv})"
            << "[Argument: (isa{sv}) 1, \"foo\", [Argument: a{sv} {\"bar\" = [Variant(int): 2]}]]";

    if (fileDescriptorPassing) {
        MyFileDescriptorStruct fds;
        fds.fd = QDBusUnixFileDescriptor(fileDescriptorForTest());
        BOBUIest::newRow("fdstruct") << QVariant::fromValue(fds) << "(h)" << "[Argument: (h) [Unix FD: valid]]";

        QList<MyFileDescriptorStruct> fdlist;
        BOBUIest::newRow("empty-list-of-fdstruct") << QVariant::fromValue(fdlist) << "a(h)" << "[Argument: a(h) {}]";

        fdlist << fds;
        BOBUIest::newRow("list-of-fdstruct") << QVariant::fromValue(fdlist) << "a(h)" << "[Argument: a(h) {[Argument: (h) [Unix FD: valid]]}]";
    }
}

void tst_QDBusMarshall::sendComplex_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<QString>("stringResult");

    QList<QDateTime> dtlist;
    BOBUIest::newRow("empty-datetimelist") << QVariant::fromValue(dtlist) << "a((iii)(iiii)i)"
            << "[Argument: a((iii)(iiii)i) {}]";
    dtlist << QDateTime();
    BOBUIest::newRow("list-of-emptydatetime") << QVariant::fromValue(dtlist) << "a((iii)(iiii)i)"
            << "[Argument: a((iii)(iiii)i) {[Argument: ((iii)(iiii)i) [Argument: (iii) 0, 0, 0], [Argument: (iiii) -1, -1, -1, -1], 0]}]";
    dtlist << QDateTime(QDate(1977, 9, 13), BOBUIime(0, 0, 0))
           << QDateTime(QDate(2006, 6, 18), BOBUIime(13, 14, 0));
    BOBUIest::newRow("datetimelist") << QVariant::fromValue(dtlist) << "a((iii)(iiii)i)"
            << "[Argument: a((iii)(iiii)i) {[Argument: ((iii)(iiii)i) [Argument: (iii) 0, 0, 0], [Argument: (iiii) -1, -1, -1, -1], 0], [Argument: ((iii)(iiii)i) [Argument: (iii) 1977, 9, 13], [Argument: (iiii) 0, 0, 0, 0], 0], [Argument: ((iii)(iiii)i) [Argument: (iii) 2006, 6, 18], [Argument: (iiii) 13, 14, 0, 0], 0]}]";

    QMap<qlonglong, QDateTime> lldtmap;
    BOBUIest::newRow("empty-lldtmap") << QVariant::fromValue(lldtmap) << "a{x((iii)(iiii)i)}"
            << "[Argument: a{x((iii)(iiii)i)} {}]";
    lldtmap[0] = QDateTime();
    lldtmap[1] = QDateTime(QDate(1970, 1, 1), BOBUIime(0, 0, 1), BOBUIimeZone::UTC);
    lldtmap[1150629776] = QDateTime(QDate(2006, 6, 18), BOBUIime(11, 22, 56), BOBUIimeZone::UTC);
    BOBUIest::newRow("lldtmap") << QVariant::fromValue(lldtmap) << "a{x((iii)(iiii)i)}"
            << "[Argument: a{x((iii)(iiii)i)} {0 = [Argument: ((iii)(iiii)i) [Argument: (iii) 0, 0, 0], [Argument: (iiii) -1, -1, -1, -1], 0], 1 = [Argument: ((iii)(iiii)i) [Argument: (iii) 1970, 1, 1], [Argument: (iiii) 0, 0, 1, 0], 1], 1150629776 = [Argument: ((iii)(iiii)i) [Argument: (iii) 2006, 6, 18], [Argument: (iiii) 11, 22, 56, 0], 1]}]";


    QMap<int, QString> ismap;
    ismap[1] = "a";
    ismap[2000] = "b";
    ismap[-47] = "c";

    QMap<QString, QString> ssmap;
    ssmap["a"] = "a";
    ssmap["c"] = "b";
    ssmap["b"] = "c";

    QMap<QDBusSignature, QString> gsmap;
    gsmap[QDBusSignature("i")] = "int32";
    gsmap[QDBusSignature("s")] = "string";
    gsmap[QDBusSignature("a{gs}")] = "array of dict_entry of (signature, string)";

    QVariantMap svmap;
    svmap["a"] = 1;
    svmap["c"] = "b";
    svmap["b"] = QByteArray("c");
    svmap["d"] = 42U;
    svmap["e"] = QVariant::fromValue(short(-47));
    svmap["f"] = QVariant::fromValue(QDBusVariant(0));
    svmap["date"] = QDate(1977, 1, 1);
    svmap["time"] = BOBUIime(8, 58, 0);
    svmap["datetime"] = QDateTime(QDate(13, 9, 2008), BOBUIime(8, 59, 31));
    svmap["pointf"] = QPointF(0.5, -0.5);
    svmap["ismap"] = QVariant::fromValue(ismap);
    svmap["ssmap"] = QVariant::fromValue(ssmap);
    svmap["gsmap"] = QVariant::fromValue(gsmap);
    svmap["dtlist"] = QVariant::fromValue(dtlist);
    svmap["lldtmap"] = QVariant::fromValue(lldtmap);
    BOBUIest::newRow("sv-map") << QVariant::fromValue(svmap) << "a{sv}"
            << "[Argument: a{sv} {\"a\" = [Variant(int): 1], \"b\" = [Variant(QByteArray): {99}], \"c\" = [Variant(QString): \"b\"], \"d\" = [Variant(uint): 42], \"date\" = [Variant: [Argument: (iii) 1977, 1, 1]], \"datetime\" = [Variant: [Argument: ((iii)(iiii)i) [Argument: (iii) 0, 0, 0], [Argument: (iiii) 8, 59, 31, 0], 0]], \"dtlist\" = [Variant: [Argument: a((iii)(iiii)i) {[Argument: ((iii)(iiii)i) [Argument: (iii) 0, 0, 0], [Argument: (iiii) -1, -1, -1, -1], 0], [Argument: ((iii)(iiii)i) [Argument: (iii) 1977, 9, 13], [Argument: (iiii) 0, 0, 0, 0], 0], [Argument: ((iii)(iiii)i) [Argument: (iii) 2006, 6, 18], [Argument: (iiii) 13, 14, 0, 0], 0]}]], \"e\" = [Variant(short): -47], \"f\" = [Variant: [Variant(int): 0]], \"gsmap\" = [Variant: [Argument: a{gs} {[Signature: a{gs}] = \"array of dict_entry of (signature, string)\", [Signature: i] = \"int32\", [Signature: s] = \"string\"}]], \"ismap\" = [Variant: [Argument: a{is} {-47 = \"c\", 1 = \"a\", 2000 = \"b\"}]], \"lldtmap\" = [Variant: [Argument: a{x((iii)(iiii)i)} {0 = [Argument: ((iii)(iiii)i) [Argument: (iii) 0, 0, 0], [Argument: (iiii) -1, -1, -1, -1], 0], 1 = [Argument: ((iii)(iiii)i) [Argument: (iii) 1970, 1, 1], [Argument: (iiii) 0, 0, 1, 0], 1], 1150629776 = [Argument: ((iii)(iiii)i) [Argument: (iii) 2006, 6, 18], [Argument: (iiii) 11, 22, 56, 0], 1]}]], \"pointf\" = [Variant: [Argument: (dd) 0.5, -0.5]], \"ssmap\" = [Variant: [Argument: a{ss} {\"a\" = \"a\", \"b\" = \"c\", \"c\" = \"b\"}]], \"time\" = [Variant: [Argument: (iiii) 8, 58, 0, 0]]}]";
}

void tst_QDBusMarshall::sendArgument_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<QString>("sig");
    BOBUIest::addColumn<int>("classification");
    BOBUIest::addColumn<QVariant>("unwrappedValue");

    QDBusArgument();
    QDBusArgument arg;

    arg = QDBusArgument();
    arg << true;
    BOBUIest::newRow("bool") << QVariant::fromValue(arg) << "b" << int(QDBusArgument::BasicType)
                          << QVariant::fromValue(true);

    arg = QDBusArgument();
    arg << false;
    BOBUIest::newRow("bool2") << QVariant::fromValue(arg) << "b" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(false);

    arg = QDBusArgument();
    arg << uchar(1);
    BOBUIest::newRow("byte") << QVariant::fromValue(arg) << "y" << int(QDBusArgument::BasicType)
                          << QVariant::fromValue(uchar(1));

    arg = QDBusArgument();
    arg << short(2);
    BOBUIest::newRow("int16") << QVariant::fromValue(arg) << "n" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(short(2));

    arg = QDBusArgument();
    arg << ushort(3);
    BOBUIest::newRow("uint16") << QVariant::fromValue(arg) << "q" << int(QDBusArgument::BasicType)
                            << QVariant::fromValue(ushort(3));

    arg = QDBusArgument();
    arg << 1;
    BOBUIest::newRow("int32") << QVariant::fromValue(arg) << "i" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(1);

    arg = QDBusArgument();
    arg << 2U;
    BOBUIest::newRow("uint32") << QVariant::fromValue(arg) << "u" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(2U);

    arg = QDBusArgument();
    arg << Q_INT64_C(3);
    BOBUIest::newRow("int64") << QVariant::fromValue(arg) << "x" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(Q_INT64_C(3));

    arg = QDBusArgument();
    arg << Q_UINT64_C(4);
    BOBUIest::newRow("uint64") << QVariant::fromValue(arg) << "t" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(Q_UINT64_C(4));

    arg = QDBusArgument();
    arg << 42.5;
    BOBUIest::newRow("double") << QVariant::fromValue(arg) << "d" << int(QDBusArgument::BasicType)
                            << QVariant::fromValue(42.5);

    arg = QDBusArgument();
    arg << QLatin1String("ping");
    BOBUIest::newRow("string") << QVariant::fromValue(arg) << "s" << int(QDBusArgument::BasicType)
                           << QVariant::fromValue(QString("ping"));

    arg = QDBusArgument();
    arg << QDBusObjectPath("/org/kde");
    BOBUIest::newRow("objectpath") << QVariant::fromValue(arg) << "o" << int(QDBusArgument::BasicType)
                                << QVariant::fromValue(QDBusObjectPath("/org/kde"));

    arg = QDBusArgument();
    arg << QDBusSignature("g");
    BOBUIest::newRow("signature") << QVariant::fromValue(arg) << "g" << int(QDBusArgument::BasicType)
                               << QVariant::fromValue(QDBusSignature("g"));

    arg = QDBusArgument();
    arg << QLatin1String("");
    BOBUIest::newRow("emptystring") << QVariant::fromValue(arg) << "s" << int(QDBusArgument::BasicType)
                                 << QVariant::fromValue(QString(""));

    arg = QDBusArgument();
    arg << QString();
    BOBUIest::newRow("nullstring") << QVariant::fromValue(arg) << "s" << int(QDBusArgument::BasicType)
                                << QVariant::fromValue(QString());

    if (fileDescriptorPassing) {
        arg = QDBusArgument();
        arg << QDBusUnixFileDescriptor(fileDescriptorForTest());
        BOBUIest::newRow("filedescriptor") << QVariant::fromValue(arg) << "h" << int(QDBusArgument::BasicType)
                                        << QVariant::fromValue(QDBusUnixFileDescriptor(fileDescriptorForTest()));
    }

    arg = QDBusArgument();
    arg << QDBusVariant(1);
    BOBUIest::newRow("variant") << QVariant::fromValue(arg) << "v" << int(QDBusArgument::VariantType)
                             << QVariant::fromValue(QDBusVariant(1));

    arg = QDBusArgument();
    arg << QDBusVariant(QVariant::fromValue(QDBusVariant(1)));
    BOBUIest::newRow("variant-variant") << QVariant::fromValue(arg) << "v" << int(QDBusArgument::VariantType)
                                     << QVariant::fromValue(QVariant::fromValue(QDBusVariant(1)));

    arg = QDBusArgument();
    arg.beginArray(QMetaType::Int);
    arg << 1 << 2 << 3 << -4;
    arg.endArray();
    BOBUIest::newRow("array-of-int") << QVariant::fromValue(arg) << "ai" << int(QDBusArgument::ArrayType)
                                  << QVariant::fromValue(arg);

    arg = QDBusArgument();
    arg.beginMap(QMetaType::Int, QMetaType::UInt);
    arg.beginMapEntry();
    arg << 1 << 2U;
    arg.endMapEntry();
    arg.beginMapEntry();
    arg << 3 << 4U;
    arg.endMapEntry();
    arg.endMap();
    BOBUIest::newRow("map") << QVariant::fromValue(arg) << "a{iu}" << int(QDBusArgument::MapType)
                         << QVariant::fromValue(arg);

    arg = QDBusArgument();
    arg.beginStructure();
    arg << 1 << 2U << short(-3) << ushort(4) << 5.0 << false;
    arg.endStructure();
    BOBUIest::newRow("structure") << QVariant::fromValue(arg) << "(iunqdb)" << int(QDBusArgument::StructureType)
                               << QVariant::fromValue(arg);
}

void tst_QDBusMarshall::sendBasic()
{
    QFETCH(QVariant, value);
    QFETCH(QString, stringResult);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName,
                                                      objectPath, interfaceName, "ping");
    msg << value;

    QDBusMessage reply = con.call(msg);
    QVERIFY2(reply.type() == QDBusMessage::ReplyMessage,
             qPrintable(reply.errorName() + ": " + reply.errorMessage()));
    //qDebug() << reply;

    QCOMPARE(reply.arguments().size(), msg.arguments().size());
    BOBUIEST(reply.signature(), "sig");
    for (int i = 0; i < reply.arguments().size(); ++i) {
        QVERIFY(compare(reply.arguments().at(i), msg.arguments().at(i)));
        //printf("\n! %s\n* %s\n", qPrintable(qDBusArgumentToString(reply.arguments().at(i))), qPrintable(stringResult));
        QCOMPARE(QDBusUtil::argumentToString(reply.arguments().at(i)), stringResult);
    }
}

void tst_QDBusMarshall::sendVariant()
{
    QFETCH(QVariant, value);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName,
                                                      objectPath, interfaceName, "ping");
    msg << QVariant::fromValue(QDBusVariant(value));

    QDBusMessage reply = con.call(msg);
 //   qDebug() << reply;

    QCOMPARE(reply.arguments().size(), msg.arguments().size());
    QCOMPARE(reply.signature(), QString("v"));
    for (int i = 0; i < reply.arguments().size(); ++i)
        QVERIFY(compare(reply.arguments().at(i), msg.arguments().at(i)));
}

void tst_QDBusMarshall::sendArrays()
{
    sendBasic();
}

void tst_QDBusMarshall::sendArrayOfArrays()
{
    sendBasic();
}

void tst_QDBusMarshall::sendMaps()
{
    sendBasic();
}

void tst_QDBusMarshall::sendStructs()
{
    sendBasic();
}

void tst_QDBusMarshall::sendComplex()
{
    sendBasic();
}

void tst_QDBusMarshall::sendArgument()
{
    QFETCH(QVariant, value);
    QFETCH(QString, sig);
    QFETCH(QVariant, unwrappedValue);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName, objectPath,
                                                      interfaceName, "ping");
    msg << value;

    QDBusMessage reply = con.call(msg);

//    QCOMPARE(reply.arguments().count(), msg.arguments().count());
    QCOMPARE(reply.signature(), sig);
//    for (int i = 0; i < reply.arguments().count(); ++i)
//        QVERIFY(compare(reply.arguments().at(i), msg.arguments().at(i)));

    // do it again inside a STRUCT now
    QDBusArgument sendArg;
    sendArg.beginStructure();
    sendArg.appendVariant(value);
    sendArg.endStructure();
    msg.setArguments(QVariantList() << QVariant::fromValue(sendArg));
    reply = con.call(msg);

    QCOMPARE(reply.signature(), QString("(%1)").arg(sig));
    QCOMPARE(reply.arguments().at(0).userType(), qMetaTypeId<QDBusArgument>());

    const QDBusArgument arg = qvariant_cast<QDBusArgument>(reply.arguments().at(0));
    QCOMPARE(int(arg.currentType()), int(QDBusArgument::StructureType));

    arg.beginStructure();
    QVERIFY(!arg.atEnd());
    QCOMPARE(arg.currentSignature(), sig);
    BOBUIEST(int(arg.currentType()), "classification");

    QVariant extracted = arg.asVariant();
    QVERIFY(arg.atEnd());

    arg.endStructure();
    QVERIFY(arg.atEnd());
    QCOMPARE(arg.currentType(), QDBusArgument::UnknownType);


    QEXPECT_FAIL("variant-variant", "Needs more QVariant unwrapping", Continue);
    if (extracted.metaType() == QMetaType::fromType<QDBusUnixFileDescriptor>()) {
        auto fd1 = extracted.value<QDBusUnixFileDescriptor>();
        auto fd2 = unwrappedValue.value<QDBusUnixFileDescriptor>();
        QVERIFY(compare(fd1, fd2));
    } else if (extracted.metaType() != QMetaType::fromType<QDBusArgument>()) {
        QCOMPARE(extracted, unwrappedValue);
    } else {
        QEXPECT_FAIL("", "Comparison logic needs to be fixed", Continue);
        QVERIFY(compare(extracted, value));
    }
}

void tst_QDBusMarshall::sendSignalErrors()
{
    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());
    QDBusMessage msg = QDBusMessage::createSignal("/foo", "local.interfaceName",
                                                  "signalName");
    msg << QVariant::fromValue(QDBusObjectPath());

    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusConnection: error: could not send signal to service \"\" path \"/foo\" interface \"local.interfaceName\" member \"signalName\": Marshalling failed: Invalid object path passed in arguments");
    QVERIFY(!con.send(msg));

    msg.setArguments(QVariantList());
    QDBusObjectPath path;

    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusObjectPath: invalid path \"abc\"");
    path.setPath("abc");
    msg << QVariant::fromValue(path);

    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusConnection: error: could not send signal to service \"\" path \"/foo\" interface \"local.interfaceName\" member \"signalName\": Marshalling failed: Invalid object path passed in arguments");
    QVERIFY(!con.send(msg));

    QDBusSignature sig(QChar(0));
    msg.setArguments(QVariantList() << QVariant::fromValue(sig));
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusConnection: error: could not send signal to service \"\" path \"/foo\" interface \"local.interfaceName\" member \"signalName\": Marshalling failed: Invalid signature passed in arguments");
    QVERIFY(!con.send(msg));

    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusSignature: invalid signature \"a\"");
    sig.setSignature("a");
    msg.setArguments(QVariantList());
    msg << QVariant::fromValue(sig);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusConnection: error: could not send signal to service \"\" path \"/foo\" interface \"local.interfaceName\" member \"signalName\": Marshalling failed: Invalid signature passed in arguments");
    QVERIFY(!con.send(msg));
}

void tst_QDBusMarshall::sendCallErrors_data()
{
    BOBUIest::addColumn<QString>("service");
    BOBUIest::addColumn<QString>("path");
    BOBUIest::addColumn<QString>("interface");
    BOBUIest::addColumn<QString>("method");
    BOBUIest::addColumn<QVariantList>("arguments");
    BOBUIest::addColumn<QString>("errorName");
    BOBUIest::addColumn<QString>("errorMsg");
    BOBUIest::addColumn<QString>("ignoreMsg");

    BOBUIest::newRow("empty-service") << "" << objectPath << interfaceName << "ping" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidService"
            << "Service name cannot be empty" << "";

    BOBUIest::newRow("invalid-service-single-label") << "service" << objectPath << interfaceName << "ping" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidService"
            << "Invalid service name: service" << "";

    BOBUIest::newRow("invalid-service") << "this isn't valid" << objectPath << interfaceName << "ping" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidService"
            << "Invalid service name: this isn't valid" << "";

    BOBUIest::newRow("empty-path") << serviceName << "" << interfaceName << "ping" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidObjectPath"
            << "Object path cannot be empty" << "";
    BOBUIest::newRow("invalid-path") << serviceName << "//" << interfaceName << "ping" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidObjectPath"
            << "Invalid object path: //" << "";

    // empty interfaces are valid
    BOBUIest::newRow("invalid-interface") << serviceName << objectPath << "this isn't valid" << "ping" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidInterface"
            << "Invalid interface class: this isn't valid" << "";

    BOBUIest::newRow("empty-method") << serviceName << objectPath << interfaceName << "" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidMember"
            << "method name cannot be empty" << "";
    BOBUIest::newRow("invalid-method") << serviceName << objectPath << interfaceName << "this isn't valid" << QVariantList()
            << "org.bobuiproject.BobUIDBus.Error.InvalidMember"
            << "Invalid method name: this isn't valid" << "";

    BOBUIest::newRow("invalid-variant") << serviceName << objectPath << interfaceName << "ping"
            << (QVariantList() << QVariant())
            << "org.freedesktop.DBus.Error.Failed"
            << "Marshalling failed: Invalid QVariant passed in arguments"
            << "QDBusMarshaller: cannot add an invalid QVariant";
    BOBUIest::newRow("invalid-qdbusvariant") << serviceName << objectPath << interfaceName << "ping"
            << (QVariantList() << QVariant::fromValue(QDBusVariant()))
            << "org.freedesktop.DBus.Error.Failed"
            << "Marshalling failed: Invalid QVariant passed in arguments"
            << "QDBusMarshaller: cannot add a null QDBusVariant";

    BOBUIest::newRow("builtin-unregistered") << serviceName << objectPath << interfaceName << "ping"
            << (QVariantList() << QLocale::c())
            << "org.freedesktop.DBus.Error.Failed"
            << "Marshalling failed: Unregistered type QLocale passed in arguments"
            << "QDBusMarshaller: type 'QLocale' (18) is not registered with D-Bus. Use qDBusRegisterMetaType to register it";

    // this type is known to the meta type system, but not registered with D-Bus
    qRegisterMetaType<UnregisteredType>();
    BOBUIest::newRow("extra-unregistered") << serviceName << objectPath << interfaceName << "ping"
            << (QVariantList() << QVariant::fromValue(UnregisteredType()))
            << "org.freedesktop.DBus.Error.Failed"
            << "Marshalling failed: Unregistered type UnregisteredType passed in arguments"
            << QString("QDBusMarshaller: type 'UnregisteredType' (%1) is not registered with D-Bus. Use qDBusRegisterMetaType to register it")
            .arg(qMetaTypeId<UnregisteredType>());

    BOBUIest::newRow("invalid-object-path-arg") << serviceName << objectPath << interfaceName << "ping"
            << (QVariantList() << QVariant::fromValue(QDBusObjectPath()))
            << "org.freedesktop.DBus.Error.Failed"
            << "Marshalling failed: Invalid object path passed in arguments"
            << "";

    BOBUIest::newRow("invalid-signature-arg") << serviceName << objectPath << interfaceName << "ping"
            << (QVariantList() << QVariant::fromValue(QDBusSignature(QChar(0))))
            << "org.freedesktop.DBus.Error.Failed"
            << "Marshalling failed: Invalid signature passed in arguments"
            << "";

    // invalid file descriptor
    if (fileDescriptorPassing) {
        BOBUIest::newRow("invalid-file-descriptor") << serviceName << objectPath << interfaceName << "ping"
                << (QVariantList() << QVariant::fromValue(QDBusUnixFileDescriptor(-1)))
                << "org.freedesktop.DBus.Error.Failed"
                << "Marshalling failed: Invalid file descriptor passed in arguments"
                << "";
    }
}

void tst_QDBusMarshall::sendCallErrors()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QFETCH(QString, service);
    QFETCH(QString, path);
    QFETCH(QString, interface);
    QFETCH(QString, method);
    QFETCH(QVariantList, arguments);
    QFETCH(QString, errorMsg);

    QFETCH(QString, ignoreMsg);
    if (!ignoreMsg.isEmpty())
        BOBUIest::ignoreMessage(BobUIWarningMsg, ignoreMsg.toLatin1());
    if (!ignoreMsg.isNull())
        BOBUIest::ignoreMessage(BobUIWarningMsg,
                             QString("QDBusConnection: error: could not send message to service \"%1\" path \"%2\" interface \"%3\" member \"%4\": %5")
                             .arg(service, path, interface, method, errorMsg)
                             .toLatin1());

    QDBusMessage msg = QDBusMessage::createMethodCall(service, path, interface, method);
    msg.setArguments(arguments);

    QDBusMessage reply = con.call(msg, QDBus::Block);
    QCOMPARE(reply.type(), QDBusMessage::ErrorMessage);
    BOBUIEST(reply.errorName(), "errorName");
    QCOMPARE(reply.errorMessage(), errorMsg);
}

// If DBUS_TYPE_UNIX_FD is not defined, it means the current system's D-Bus library is too old for this test
void tst_QDBusMarshall::receiveUnknownType_data()
{
    BOBUIest::addColumn<int>("receivedTypeId");
    BOBUIest::newRow("in-call") << qMetaTypeId<void*>();
    BOBUIest::newRow("type-variant") << qMetaTypeId<QDBusVariant>();
    BOBUIest::newRow("type-array") << qMetaTypeId<QDBusArgument>();
    BOBUIest::newRow("type-struct") << qMetaTypeId<QDBusArgument>();
    BOBUIest::newRow("type-naked") << qMetaTypeId<void *>();
}

struct DisconnectRawDBus {
    static void cleanup(DBusConnection *connection)
    {
        if (!connection)
            return;
        q_dbus_connection_close(connection);
        q_dbus_connection_unref(connection);
    }
};
struct UnrefDBusMessage
{
    static void cleanup(DBusMessage *type)
    {
        if (!type) return;
        q_dbus_message_unref(type);
    }
};
struct UnrefDBusPendingCall
{
    static void cleanup(DBusPendingCall *type)
    {
        if (!type) return;
        q_dbus_pending_call_unref(type);
    }
};

// use these scoped types to avoid memory leaks if QVERIFY or QCOMPARE fails
typedef QScopedPointer<DBusConnection, DisconnectRawDBus> ScopedDBusConnection;
typedef QScopedPointer<DBusMessage, UnrefDBusMessage> ScopedDBusMessage;
typedef QScopedPointer<DBusPendingCall, UnrefDBusPendingCall> ScopedDBusPendingCall;

template <typename T, typename T2 = T> struct SetResetValue
{
    const T2 oldValue;
    T &value;
public:
    SetResetValue(T &v, T2 newValue) : oldValue(v), value(v)
    {
        value = newValue;
    }
    ~SetResetValue()
    {
        value = oldValue;
    }
};

// mostly the same as qdbusintegrator.cpp:connectionCapabilities
static bool canSendUnixFd(DBusConnection *connection)
{
    typedef dbus_bool_t (*can_send_type_t)(DBusConnection *, int);
    static can_send_type_t can_send_type = 0;

#if defined(BOBUI_LINKED_LIBDBUS)
# if DBUS_VERSION-0 >= 0x010400
    can_send_type = dbus_connection_can_send_type;
# endif
#elif BOBUI_CONFIG(library)
    // run-time check if the next functions are available
    can_send_type = (can_send_type_t)qdbus_resolve_conditionally("dbus_connection_can_send_type");
#endif

    return can_send_type && can_send_type(connection, DBUS_TYPE_UNIX_FD);
}

void tst_QDBusMarshall::receiveUnknownType()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    // this needs to be implemented in raw
    // open a new connection to the bus daemon
    DBusError error;
    q_dbus_error_init(&error);
    ScopedDBusConnection rawcon(q_dbus_bus_get_private(DBUS_BUS_SESSION, &error));
    QVERIFY2(rawcon.data(), error.name);

    // check if this bus supports passing file descriptors

    if (!canSendUnixFd(rawcon.data()))
        QSKIP("Your session bus does not allow sending Unix file descriptors");

    // make sure this QDBusConnection won't handle Unix file descriptors
    QAtomicInt &capabRef = QDBusConnectionPrivate::d(con)->capabilities;
    SetResetValue<QAtomicInt, int> resetter(capabRef,
                                            capabRef & ~QDBusConnection::UnixFileDescriptorPassing);

    if (qstrcmp(BOBUIest::currentDataTag(), "in-call") == 0) {
        // create a call back to us containing a file descriptor
        QDBusMessageSpy spy;
        con.registerObject("/spyObject", &spy, QDBusConnection::ExportAllSlots);
        ScopedDBusMessage msg(q_dbus_message_new_method_call(con.baseService().toLatin1(), "/spyObject", NULL, "theSlot"));

        int fd = fileno(stdout);
        DBusMessageIter iter;
        q_dbus_message_iter_init_append(msg.data(), &iter);
        q_dbus_message_iter_append_basic(&iter, DBUS_TYPE_UNIX_FD, &fd);

        // try to send to us
        DBusPendingCall *pending_ptr;
        q_dbus_connection_send_with_reply(rawcon.data(), msg.data(), &pending_ptr, 1000);
        ScopedDBusPendingCall pending(pending_ptr);

        // check that it got sent
        while (q_dbus_connection_dispatch(rawcon.data()) == DBUS_DISPATCH_DATA_REMAINS)
            ;

        // now spin our event loop. We don't catch this call, so let's get the reply
        QEventLoop loop;
        BOBUIimer::singleShot(200, &loop, SLOT(quit()));
        BOBUIest::ignoreMessage(BobUIWarningMsg, "QDBusConnection: couldn't handle call to theSlot, no slot matched");
        loop.exec();

        // now try to receive the reply
        q_dbus_pending_call_block(pending.data());

        // check that the spy received what it was supposed to receive
        QCOMPARE(spy.list.size(), 1);
        QCOMPARE(spy.list.at(0).arguments().size(), 1);
        QFETCH(int, receivedTypeId);
        QCOMPARE(spy.list.at(0).arguments().at(0).userType(), receivedTypeId);

        msg.reset(q_dbus_pending_call_steal_reply(pending.data()));
        QVERIFY(msg);
        QCOMPARE(q_dbus_message_get_type(msg.data()), DBUS_MESSAGE_TYPE_METHOD_RETURN);
        QCOMPARE(q_dbus_message_get_signature(msg.data()), DBUS_TYPE_INT32_AS_STRING);

        int retval;
        QVERIFY(q_dbus_message_iter_init(msg.data(), &iter));
        q_dbus_message_iter_get_basic(&iter, &retval);
        QCOMPARE(retval, 42);
    } else {
        // create a signal that we'll emit
        static const char signalName[] = "signalName";
        static const char interfaceName[] = "local.interface.name";
        ScopedDBusMessage msg(q_dbus_message_new_signal("/", interfaceName, signalName));
        con.connect(q_dbus_bus_get_unique_name(rawcon.data()), QString(), interfaceName, signalName, &BOBUIestEventLoop::instance(), SLOT(exitLoop()));

        QDBusMessageSpy spy;
        con.connect(q_dbus_bus_get_unique_name(rawcon.data()), QString(), interfaceName, signalName, &spy, SLOT(theSlot(QDBusMessage)));

        DBusMessageIter iter;
        q_dbus_message_iter_init_append(msg.data(), &iter);
        int fd = fileno(stdout);

        if (qstrcmp(BOBUIest::currentDataTag(), "type-naked") == 0) {
            // send naked
            q_dbus_message_iter_append_basic(&iter, DBUS_TYPE_UNIX_FD, &fd);
        } else {
            DBusMessageIter subiter;
            if (qstrcmp(BOBUIest::currentDataTag(), "type-variant") == 0)
                q_dbus_message_iter_open_container(&iter, DBUS_TYPE_VARIANT, DBUS_TYPE_UNIX_FD_AS_STRING, &subiter);
            else if (qstrcmp(BOBUIest::currentDataTag(), "type-array") == 0)
                q_dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_UNIX_FD_AS_STRING, &subiter);
            else if (qstrcmp(BOBUIest::currentDataTag(), "type-struct") == 0)
                q_dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, 0, &subiter);
            q_dbus_message_iter_append_basic(&subiter, DBUS_TYPE_UNIX_FD, &fd);
            q_dbus_message_iter_close_container(&iter, &subiter);
        }

        // send it
        q_dbus_connection_send(rawcon.data(), msg.data(), 0);

        // check that it got sent
        while (q_dbus_connection_dispatch(rawcon.data()) == DBUS_DISPATCH_DATA_REMAINS)
            ;

        // now let's see what happens
        BOBUIestEventLoop::instance().enterLoop(1);
        QVERIFY(!BOBUIestEventLoop::instance().timeout());
        QCOMPARE(spy.list.size(), 1);
        QCOMPARE(spy.list.at(0).arguments().size(), 1);
        QFETCH(int, receivedTypeId);
        //qDebug() << spy.list.at(0).arguments().at(0).typeName();
        QCOMPARE(spy.list.at(0).arguments().at(0).userType(), receivedTypeId);
    }
}

void tst_QDBusMarshall::demarshallPrimitives_data()
{
    addBasicTypesColumns();

    // Primitive types, excluding strings and FD
    basicNumericTypes_data();
}

template<class T>
QVariant demarshallPrimitiveAs(const QDBusArgument& dbusArg)
{
    T val;
    dbusArg >> val;
    return QVariant::fromValue(val);
}

QVariant demarshallPrimitiveAs(int typeIndex, const QDBusArgument& dbusArg)
{
    switch (typeIndex) {
    case 0:
        return demarshallPrimitiveAs<uchar>(dbusArg);
    case 1:
        return demarshallPrimitiveAs<bool>(dbusArg);
    case 2:
        return demarshallPrimitiveAs<short>(dbusArg);
    case 3:
        return demarshallPrimitiveAs<ushort>(dbusArg);
    case 4:
        return demarshallPrimitiveAs<int>(dbusArg);
    case 5:
        return demarshallPrimitiveAs<uint>(dbusArg);
    case 6:
        return demarshallPrimitiveAs<qlonglong>(dbusArg);
    case 7:
        return demarshallPrimitiveAs<qulonglong>(dbusArg);
    case 8:
        return demarshallPrimitiveAs<double>(dbusArg);
    default:
        return QVariant();
    }
}

void tst_QDBusMarshall::demarshallPrimitives()
{
    QFETCH(QVariant, value);
    QFETCH(QString, sig);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    // Demarshall each test data value to all primitive types to test
    // demarshalling to the wrong type does not cause a crash
    for (int typeIndex = 0; true; ++typeIndex) {
        QDBusMessage msg = QDBusMessage::createMethodCall(serviceName, objectPath,
                                                          interfaceName, "ping");
        QDBusArgument sendArg;
        sendArg.beginStructure();
        sendArg.appendVariant(value);
        sendArg.endStructure();
        msg.setArguments(QVariantList() << QVariant::fromValue(sendArg));
        QDBusMessage reply = con.call(msg);

        const QDBusArgument receiveArg = qvariant_cast<QDBusArgument>(reply.arguments().at(0));
        receiveArg.beginStructure();
        QCOMPARE(receiveArg.currentSignature(), sig);

        const QVariant receiveValue = demarshallPrimitiveAs(typeIndex, receiveArg);
        if (receiveValue.metaType() == value.metaType()) {
            // Value type is the same, compare the values
            QCOMPARE(receiveValue, value);
            QVERIFY(receiveArg.atEnd());
        }

        receiveArg.endStructure();
        QVERIFY(receiveArg.atEnd());

        if (!receiveValue.isValid())
            break;
    }
}

void tst_QDBusMarshall::demarshallStrings_data()
{
    BOBUIest::addColumn<QVariant>("value");
    BOBUIest::addColumn<char>("targetSig");
    BOBUIest::addColumn<QVariant>("expectedValue");

    // All primitive types demarshall to null string types
    using ValSigPair = std::pair<QVariant, char>;
    const QList<ValSigPair> nullStringTypes = {
        ValSigPair(QVariant::fromValue(QString()), 's'),
        ValSigPair(QVariant::fromValue(QDBusObjectPath()), 'o'),
        ValSigPair(QVariant::fromValue(QDBusSignature()), 'g')
    };
    for (const auto &[v, charSymbol] : nullStringTypes) {
        const char *name = v.typeName();
        BOBUIest::addRow("bool(false)-%s", name) << QVariant(false) << charSymbol << v;
        BOBUIest::addRow("bool(true)-%s", name) << QVariant(true) << charSymbol << v;
        BOBUIest::addRow("byte-%s", name) << QVariant::fromValue(uchar(1)) << charSymbol << v;
        BOBUIest::addRow("int16-%s", name) << QVariant::fromValue(short(2)) << charSymbol << v;
        BOBUIest::addRow("uint16-%s", name) << QVariant::fromValue(ushort(3)) << charSymbol << v;
        BOBUIest::addRow("int-%s", name) << QVariant(1) << charSymbol << v;
        BOBUIest::addRow("uint-%s", name) << QVariant(2U) << charSymbol << v;
        BOBUIest::addRow("int64-%s", name) << QVariant(Q_INT64_C(3)) << charSymbol << v;
        BOBUIest::addRow("uint64-%s", name) << QVariant(Q_UINT64_C(4)) << charSymbol << v;
        BOBUIest::addRow("double-%s", name) << QVariant(42.5) << charSymbol << v;
    }

    // String types should demarshall to each other. This is a regression test
    // to check released functionality is maintained even after checks have
    // been added to string demarshalling
    BOBUIest::newRow("empty string->invalid objectpath") << QVariant("")
                                                      << 'o' << QVariant::fromValue(QDBusObjectPath());
    BOBUIest::newRow("null string->invalid objectpath") << QVariant(QString())
                                                     << 'o' << QVariant::fromValue(QDBusObjectPath());
    BOBUIest::newRow("string->invalid objectpath") << QVariant("invalid objectpath")
                                                << 'o' << QVariant::fromValue(QDBusObjectPath());
    BOBUIest::newRow("string->valid objectpath") << QVariant("/org/kde")
                                              << 'o' << QVariant::fromValue(QDBusObjectPath("/org/kde"));

    BOBUIest::newRow("empty string->invalid signature") << QVariant("")
                                                     << 'g' << QVariant::fromValue(QDBusSignature());
    BOBUIest::newRow("null string->invalid signature") << QVariant(QString())
                                                    << 'g' << QVariant::fromValue(QDBusSignature());
    BOBUIest::newRow("string->invalid signature") << QVariant("_invalid signature")
                                               << 'g' << QVariant::fromValue(QDBusSignature());
    BOBUIest::newRow("string->valid signature") << QVariant("s")
                                             << 'g' << QVariant::fromValue(QDBusSignature("s"));

    BOBUIest::newRow("objectpath->string") << QVariant::fromValue(QDBusObjectPath("/org/kde"))
                                        << 's' << QVariant::fromValue(QString("/org/kde"));
    BOBUIest::newRow("objectpath->invalid signature") << QVariant::fromValue(QDBusObjectPath("/org/kde"))
                                                   << 'g' << QVariant::fromValue(QDBusSignature());

    BOBUIest::newRow("signature->string") << QVariant::fromValue(QDBusSignature("s"))
                                       << 's' << QVariant::fromValue(QString("s"));
    BOBUIest::newRow("signature->invalid objectpath") << QVariant::fromValue(QDBusSignature("s"))
                                                   << 'o' << QVariant::fromValue(QDBusObjectPath());
}

QVariant demarshallAsString(const QDBusArgument& dbusArg, char targetSig)
{
    switch (targetSig) {
        case 's': {
            QString s;
            dbusArg >> s;
            return s;
        }
        case 'o': {
            QDBusObjectPath op;
            dbusArg >> op;
            return QVariant::fromValue(op);
        }
        case 'g' : {
            QDBusSignature sig;
            dbusArg >> sig;
            return QVariant::fromValue(sig);
        }
        default: {
            return QVariant();
        }
    }
}

void tst_QDBusMarshall::demarshallStrings()
{
    QFETCH(QVariant, value);
    QFETCH(char, targetSig);
    QFETCH(QVariant, expectedValue);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName, objectPath,
                                                      interfaceName, "ping");
    QDBusArgument sendArg;
    sendArg.beginStructure();
    sendArg.appendVariant(value);
    sendArg.endStructure();
    msg.setArguments(QVariantList() << QVariant::fromValue(sendArg));
    QDBusMessage reply = con.call(msg);

    const QDBusArgument receiveArg = qvariant_cast<QDBusArgument>(reply.arguments().at(0));
    receiveArg.beginStructure();

    QVariant receiveValue = demarshallAsString(receiveArg, targetSig);
    QVERIFY2(receiveValue.isValid(), "Invalid targetSig in demarshallStrings_data()");
    QVERIFY(compare(receiveValue, expectedValue));

    receiveArg.endStructure();
    QVERIFY(receiveArg.atEnd());
}

void tst_QDBusMarshall::demarshallInvalidStringList_data()
{
    addBasicTypesColumns();

    // None of the basic types should demarshall to a string list
    basicNumericTypes_data();
    basicStringTypes_data();

    // Arrays of non-string type should not demarshall to a string list
    QList<bool> bools;
    BOBUIest::newRow("emptyboollist") << QVariant::fromValue(bools);
    bools << false << true << false;
    BOBUIest::newRow("boollist") << QVariant::fromValue(bools);

    // Structures should not demarshall to a QByteArray
    BOBUIest::newRow("struct of strings")
            << QVariant::fromValue(QVariantList() << QString("foo") << QString("bar"));
    BOBUIest::newRow("struct of mixed types")
            << QVariant::fromValue(QVariantList() << QString("foo") << int(42) << double(3.14));
}

void tst_QDBusMarshall::demarshallInvalidStringList()
{
    QFETCH(QVariant, value);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName, objectPath,
                                                      interfaceName, "ping");
    QDBusArgument sendArg;
    sendArg.beginStructure();
    sendArg.appendVariant(value);
    sendArg.endStructure();
    msg.setArguments(QVariantList() << QVariant::fromValue(sendArg));
    QDBusMessage reply = con.call(msg);

    const QDBusArgument receiveArg = qvariant_cast<QDBusArgument>(reply.arguments().at(0));
    receiveArg.beginStructure();

    QStringList receiveValue;
    receiveArg >> receiveValue;
    QCOMPARE(receiveValue, QStringList());

    receiveArg.endStructure();
    QVERIFY(receiveArg.atEnd());
}

void tst_QDBusMarshall::demarshallInvalidByteArray_data()
{
    addBasicTypesColumns();

    // None of the basic types should demarshall to a QByteArray
    basicNumericTypes_data();
    basicStringTypes_data();

    // Arrays of other types than byte should not demarshall to a QByteArray
    QList<bool> bools;
    BOBUIest::newRow("empty array of bool") << QVariant::fromValue(bools);
    bools << true << false << true;
    BOBUIest::newRow("non-empty array of bool") << QVariant::fromValue(bools);

    // Structures should not demarshall to a QByteArray
    BOBUIest::newRow("struct of bytes")
            << QVariant::fromValue(QVariantList() << uchar(1) << uchar(2));

    BOBUIest::newRow("struct of mixed types")
            << QVariant::fromValue(QVariantList() << int(42) << QString("foo") << double(3.14));
}

void tst_QDBusMarshall::demarshallInvalidByteArray()
{
    QFETCH(QVariant, value);

    QDBusConnection con = QDBusConnection::sessionBus();

    QVERIFY(con.isConnected());

    QDBusMessage msg = QDBusMessage::createMethodCall(serviceName, objectPath,
                                                      interfaceName, "ping");
    QDBusArgument sendArg;
    sendArg.beginStructure();
    sendArg.appendVariant(value);
    sendArg.endStructure();
    msg.setArguments(QVariantList() << QVariant::fromValue(sendArg));
    QDBusMessage reply = con.call(msg);

    const QDBusArgument receiveArg = qvariant_cast<QDBusArgument>(reply.arguments().at(0));
    receiveArg.beginStructure();

    QByteArray receiveValue;
    receiveArg >> receiveValue;
    QCOMPARE(receiveValue, QByteArray());

    receiveArg.endStructure();
    QVERIFY(receiveArg.atEnd());
}

BOBUIEST_MAIN(tst_QDBusMarshall)
#include "tst_qdbusmarshall.moc"
