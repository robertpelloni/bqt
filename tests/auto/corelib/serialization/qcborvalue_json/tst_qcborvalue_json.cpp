// Copyright (C) 2018 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qcborvalue.h>
#include <BOBUIest>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

Q_DECLARE_METATYPE(QCborValue)

class tst_QCborValue_Json : public QObject
{
    Q_OBJECT

private slots:
    void toVariant_data();
    void toVariant();
    void toJson_data() { toVariant_data(); }
    void toJson();
    void taggedByteArrayToJson_data();
    void taggedByteArrayToJson();

    void fromVariant_data();
    void fromVariant();
    void fromJson_data();
    void fromJson();

    void nonStringKeysInMaps_data();
    void nonStringKeysInMaps();
};

void tst_QCborValue_Json::toVariant_data()
{
    BOBUIest::addColumn<QCborValue>("v");
    BOBUIest::addColumn<QVariant>("variant");
    BOBUIest::addColumn<QJsonValue>("json");
    QDateTime dt = QDateTime::currentDateTimeUtc();
    QUuid uuid = QUuid::createUuid();

    QMetaEnum me = QMetaEnum::fromType<QCborValue::Type>();
    auto add = [me](const QCborValue &v, const QVariant &exp, const QJsonValue &json) {
        auto addRow = [=]() -> BOBUIestData & {
            const char *typeString = me.valueToKey(v.type());
            if (v.type() == QCborValue::Integer)
                return BOBUIest::addRow("Integer:%lld", exp.toLongLong());
            if (v.type() == QCborValue::Double)
                return BOBUIest::addRow("Double:%g", exp.toDouble());
            if (v.type() == QCborValue::ByteArray || v.type() == QCborValue::String)
                return BOBUIest::addRow("%s:%zd", typeString, size_t(exp.toString().size()));
            if (v.type() >= 0x10000)
                return BOBUIest::newRow(exp.typeName());
            return BOBUIest::newRow(typeString);
        };
        addRow() << v << exp << json;
    };

    // good JSON matching:
    add(QCborValue(), QVariant(), QJsonValue::Null);
    add(nullptr, QVariant::fromValue(nullptr), QJsonValue::Null);
    add(false, false, false);
    add(true, true, true);
    add(0, 0, 0);
    add(1, 1, 1);
    add(-1, -1, -1);
    add(0., 0., 0.);
    add(2., 2., 2.);
    add(1.25, 1.25, 1.25);
    add(-1.25, -1.25, -1.25);
    add("Hello", "Hello", "Hello");
    add(std::numeric_limits<qint64>::max(), std::numeric_limits<qint64>::max(), std::numeric_limits<qint64>::max());
    add(std::numeric_limits<qint64>::min(), std::numeric_limits<qint64>::min(), std::numeric_limits<qint64>::min());

    // converts to string in JSON:
    add(QByteArray("Hello"), QByteArray("Hello"), "SGVsbG8");
    add(QCborValue(dt), dt, dt.toString(BobUI::ISODateWithMs));
    add(QCborValue(QUrl("http://example.com/{q}")), QUrl("http://example.com/{q}"),
        "http://example.com/%7Bq%7D");      // note the encoded form in JSON
    add(QCborValue(QRegularExpression(".")), QRegularExpression("."), ".");
    add(QCborValue(uuid), uuid, uuid.toString(QUuid::WithoutBraces));

    // not valid in JSON
    BOBUIest::newRow("simpletype") << QCborValue(QCborSimpleType(255))
                                << QVariant::fromValue(QCborSimpleType(255))
                                << QJsonValue("simple(255)");
    BOBUIest::newRow("Double:inf") << QCborValue(qInf())
                                << QVariant(qInf())
                                << QJsonValue();
    BOBUIest::newRow("Double:-inf") << QCborValue(-qInf())
                                 << QVariant(-qInf())
                                 << QJsonValue();
    BOBUIest::newRow("Double:nan") << QCborValue(qQNaN())
                                << QVariant(qQNaN())
                                << QJsonValue();

    // empty arrays and maps
    add(QCborArray(), QVariantList(), QJsonArray());
    add(QCborMap(), QVariantMap(), QJsonObject());
}

void tst_QCborValue_Json::toVariant()
{
    QFETCH(QCborValue, v);
    QFETCH(QVariant, variant);

    if (qIsNaN(variant.toDouble())) {
        // because NaN != NaN, QVariant(NaN) != QVariant(NaN), so we
        // only need to compare the classification
        QVERIFY(qIsNaN(v.toVariant().toDouble()));

        // the rest of this function depends on the variant comparison
        return;
    }

    QCOMPARE(v.toVariant(), variant);
    if (variant.isValid()) {
        QVariant variant2 = QVariant::fromValue(v);
        QVERIFY(variant2.canConvert(variant.metaType()));
        QVERIFY(variant2.convert(variant.metaType()));
        QCOMPARE(variant2, variant);
    }

    // tags get ignored:
    QCOMPARE(QCborValue(QCborKnownTags::Signature, v).toVariant(), variant);

    // make arrays with this item
    QCOMPARE(QCborArray({v}).toVariantList(), QVariantList({variant}));
    QCOMPARE(QCborArray({v, v}).toVariantList(), QVariantList({variant, variant}));

    // and maps
    QCOMPARE(QCborMap({{"foo", v}}).toVariantMap(), QVariantMap({{"foo", variant}}));
    QCOMPARE(QCborMap({{"foo", v}}).toVariantHash(), QVariantHash({{"foo", variant}}));

    // finally, mixed
    QCOMPARE(QCborArray{QCborMap({{"foo", v}})}.toVariantList(),
             QVariantList{QVariantMap({{"foo", variant}})});
}

void tst_QCborValue_Json::toJson()
{
    QFETCH(QCborValue, v);
    QFETCH(QJsonValue, json);

    QCOMPARE(v.toJsonValue(), json);
    QCOMPARE(QVariant::fromValue(v).toJsonValue(), json);

    // most tags get ignored:
    QCOMPARE(QCborValue(QCborKnownTags::Signature, v).toJsonValue(), json);

    // make arrays with this item
    QCOMPARE(QCborArray({v}).toJsonArray(), QJsonArray({json}));
    QCOMPARE(QCborArray({v, v}).toJsonArray(), QJsonArray({json, json}));

    // and maps
    QCOMPARE(QCborMap({{"foo", v}}).toJsonObject(), QJsonObject({{"foo", json}}));

    // finally, mixed
    QCOMPARE(QCborArray{QCborMap({{"foo", v}})}.toJsonArray(),
             QJsonArray{QJsonObject({{"foo", json}})});
}

void tst_QCborValue_Json::taggedByteArrayToJson_data()
{
    BOBUIest::addColumn<QCborValue>("v");
    BOBUIest::addColumn<QJsonValue>("json");

    QByteArray data("\xff\x01");
    BOBUIest::newRow("base64url") << QCborValue(QCborKnownTags::ExpectedBase64url, data) << QJsonValue("_wE");
    BOBUIest::newRow("base64") << QCborValue(QCborKnownTags::ExpectedBase64, data) << QJsonValue("/wE=");
    BOBUIest::newRow("hex") << QCborValue(QCborKnownTags::ExpectedBase16, data) << QJsonValue("ff01");
}

void tst_QCborValue_Json::taggedByteArrayToJson()
{
    QFETCH(QCborValue, v);
    QFETCH(QJsonValue, json);

    QCOMPARE(v.toJsonValue(), json);
    QCOMPARE(QCborArray({v}).toJsonArray(), QJsonArray({json}));
}

void tst_QCborValue_Json::fromVariant_data()
{
    toVariant_data();

    auto addIntegral = [](auto number) {
        QCborValue cv = qint64(number);
        QJsonValue jv = qint64(number);
        QVariant vv = QVariant::fromValue(number);
        BOBUIest::addRow("%s:%lld", vv.typeName(), qlonglong(number)) << cv << vv << jv;
    };

    // exercise different QVariant numeric types
    addIntegral(short(-1));
    addIntegral(ushort(1));
    // int already tested
    addIntegral(65536U);
    addIntegral(-0x7fff'ffffL);
    addIntegral(0xffff'ffffUL);
    addIntegral(-0x1'0000'0000LL);
    addIntegral(0x1000'0000'0000ULL);

    BOBUIest::addRow("float:1.875") << QCborValue(1.875) << QVariant::fromValue(1.875f) << QJsonValue(1.875);
    BOBUIest::addRow("qfloat16:-0.5") << QCborValue(-0.5) << QVariant::fromValue(qfloat16(-0.5f)) << QJsonValue(-0.5);
}

void tst_QCborValue_Json::fromVariant()
{
    QFETCH(QCborValue, v);
    QFETCH(QVariant, variant);

    QCOMPARE(QCborValue::fromVariant(variant), v);
    QCOMPARE(variant.value<QCborValue>(), v);

    // try arrays
    QCOMPARE(QCborArray::fromVariantList({variant}), QCborArray{v});
    QCOMPARE(QCborArray::fromVariantList({variant, variant}), QCborArray({v, v}));

    if (variant.metaType() == QMetaType(QMetaType::QString)) {
        QString s = variant.toString();
        QCOMPARE(QCborArray::fromStringList({s}), QCborArray{v});
        QCOMPARE(QCborArray::fromStringList({s, s}), QCborArray({v, v}));
    }

    // maps...
    QVariantMap map{{"foo", variant}};
    QCOMPARE(QCborMap::fromVariantMap(map), QCborMap({{"foo", v}}));
    QCOMPARE(QCborMap::fromVariantHash({{"foo", variant}}), QCborMap({{"foo", v}}));

    // nested
    QVariantMap outer{{"bar", QVariantList{0, map, true}}};
    QCOMPARE(QCborMap::fromVariantMap(outer),
             QCborMap({{"bar", QCborArray{0, QCborMap{{"foo", v}}, true}}}));
}

void tst_QCborValue_Json::fromJson_data()
{
    BOBUIest::addColumn<QCborValue>("v");
    BOBUIest::addColumn<QJsonValue>("json");

    BOBUIest::newRow("null") << QCborValue(QCborValue::Null) << QJsonValue(QJsonValue::Null);
    BOBUIest::newRow("false") << QCborValue(false) << QJsonValue(false);
    BOBUIest::newRow("true") << QCborValue(true) << QJsonValue(true);
    BOBUIest::newRow("0") << QCborValue(0) << QJsonValue(0.);
    BOBUIest::newRow("1") << QCborValue(1) << QJsonValue(1);
    BOBUIest::newRow("1.5") << QCborValue(1.5) << QJsonValue(1.5);
    BOBUIest::newRow("Integer:max") << QCborValue(std::numeric_limits<qint64>::max())
                                 << QJsonValue(std::numeric_limits<qint64>::max());
    BOBUIest::newRow("Integer:min") << QCborValue(std::numeric_limits<qint64>::min())
                                 << QJsonValue(std::numeric_limits<qint64>::min());
    BOBUIest::newRow("string") << QCborValue("Hello") << QJsonValue("Hello");
    BOBUIest::newRow("array") << QCborValue(QCborValue::Array) << QJsonValue(QJsonValue::Array);
    BOBUIest::newRow("map") << QCborValue(QCborValue::Map) << QJsonValue(QJsonValue::Object);
}

void tst_QCborValue_Json::fromJson()
{
    QFETCH(QCborValue, v);
    QFETCH(QJsonValue, json);

    QCOMPARE(QCborValue::fromJsonValue(json), v);
    QCOMPARE(QVariant(json).value<QCborValue>(), v);
    QCOMPARE(QCborArray::fromJsonArray({json}), QCborArray({v}));
    QCOMPARE(QCborArray::fromJsonArray({json, json}), QCborArray({v, v}));
    QCOMPARE(QCborMap::fromJsonObject({{"foo", json}}), QCborMap({{"foo", v}}));

    // confirm we can roundtrip back to JSON
    QCOMPARE(QCborValue::fromJsonValue(json).toJsonValue(), json);
}

void tst_QCborValue_Json::nonStringKeysInMaps_data()
{
    BOBUIest::addColumn<QCborValue>("key");
    BOBUIest::addColumn<QString>("converted");

    auto add = [](const char *str, const QCborValue &v) {
        BOBUIest::newRow(str) << v << str;
    };
    add("0", 0);
    add("-1", -1);
    add("false", false);
    add("true", true);
    add("null", nullptr);
    add("undefined", {});   // should this be ""?
    add("simple(255)", QCborSimpleType(255));
    add("2.5", 2.5);

    QByteArray data("\xff\x01");
    BOBUIest::newRow("bytearray") << QCborValue(data) << "_wE";
    BOBUIest::newRow("base64url") << QCborValue(QCborKnownTags::ExpectedBase64url, data) << "_wE";
    BOBUIest::newRow("base64") << QCborValue(QCborKnownTags::ExpectedBase64, data) << "/wE=";
    BOBUIest::newRow("hex") << QCborValue(QCborKnownTags::ExpectedBase16, data) << "ff01";

    BOBUIest::newRow("emptyarray") << QCborValue(QCborValue::Array) << "[]";
    BOBUIest::newRow("emptymap") << QCborValue(QCborValue::Map) << "{}";
    BOBUIest::newRow("array") << QCborValue(QCborArray{1, true, 2.5, "Hello"})
                           << "[1, true, 2.5, \"Hello\"]";
    BOBUIest::newRow("map") << QCborValue(QCborMap{{"Hello", 0}, {0, "Hello"}})
                         << "{\"Hello\": 0, 0: \"Hello\"}";

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QUrl url("https://example.com");
    QUuid uuid = QUuid::createUuid();
    BOBUIest::newRow("QDateTime") << QCborValue(dt) << dt.toString(BobUI::ISODateWithMs);
    BOBUIest::newRow("QUrl") << QCborValue(url) << url.toString(QUrl::FullyEncoded);
    BOBUIest::newRow("QRegularExpression") << QCborValue(QRegularExpression(".*")) << ".*";
    BOBUIest::newRow("QUuid") << QCborValue(uuid) << uuid.toString(QUuid::WithoutBraces);
}

void tst_QCborValue_Json::nonStringKeysInMaps()
{
    QFETCH(QCborValue, key);
    QFETCH(QString, converted);

    QCborMap m;
    m.insert(key, 0);

    {
        QVariantMap vm = m.toVariantMap();
        auto it = vm.begin();
        QVERIFY(it != vm.end());
        QCOMPARE(it.key(), converted);
        QCOMPARE(it.value(), 0);
        QCOMPARE(++it, vm.end());
    }

    {
        QJsonObject o = m.toJsonObject();
        auto it = o.begin();
        QVERIFY(it != o.end());
        QCOMPARE(it.keyView(), converted);
        QCOMPARE(it.key(), converted);
        QCOMPARE(it.value(), 0);
        QCOMPARE(++it, o.end());
    }
}

BOBUIEST_MAIN(tst_QCborValue_Json)

#include "tst_qcborvalue_json.moc"
