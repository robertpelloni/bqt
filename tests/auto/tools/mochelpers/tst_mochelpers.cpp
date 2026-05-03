// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

// Testing bobuimochelpers.h is probably pointless... if there's a problem with it
// then you most likely can't compile this test in the first place.
#include <BobUICore/bobuimochelpers.h>
#include "bobuimocconstants.h"

#include <BOBUIest>

#include <BobUICore/qobject.h>

#include <initializer_list>
#include <q20algorithm.h>

BOBUI_BEGIN_NAMESPACE
namespace BobUIMocHelpers {
} BOBUI_END_NAMESPACE

class tst_MocHelpers : public QObject
{
    Q_OBJECT
private slots:
    void stringData();

    // parts of the uint array
    void classinfoData();
    void classinfoDataGroup();
    void enumUintData();
    void enumUintGroup();
    void propertyUintData();
    void propertyUintGroup();
    void methodUintData();
    void methodUintGroup();
    void constructorUintData();
    void constructorUintGroup();

    void emptyUintArray();
    void uintArrayNoMethods();
    void uintArray();
};

template <int Count, size_t StringSize>
void verifyStringData(const BobUIMocHelpers::StringData<Count, StringSize> &data,
                      std::initializer_list<const char *> strings)
{
    QCOMPARE(std::size(strings), size_t(Count) / 2);
    ptrdiff_t i = 0;
    for (const char *str : strings) {
        uint offset = data.offsetsAndSizes[i++] - sizeof(data.offsetsAndSizes);
        uint len = data.offsetsAndSizes[i++];
        QByteArrayView result(data.stringdata0 + offset, len);

        QCOMPARE(len, strlen(str));
        QCOMPARE(result, str);
    }
}

void tst_MocHelpers::stringData()
{
#define CHECK(...)  \
    verifyStringData(BobUIMocHelpers::stringData(__VA_ARGS__), { __VA_ARGS__ })

    BOBUIest::setThrowOnFail(true);
    CHECK("Hello");
    CHECK("Hello", "World");
    CHECK("Hello", "", "World");
#undef CHECK
}

void tst_MocHelpers::classinfoData()
{
    {
        auto result = BobUIMocHelpers::ClassInfos({{1, 2}});
        QCOMPARE(result.headerSize(), 2U);
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 2U);
    }
    {
        auto result = BobUIMocHelpers::ClassInfos({{1, 2}, {3, 4}});
        QCOMPARE(result.headerSize(), 4U);
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 2U);
        QCOMPARE(result.header[2], 3U);
        QCOMPARE(result.header[3], 4U);
    }
}

constexpr BobUIMocHelpers::StringRefStorage dummyStringData {"tst_BobUIMocHelpers"};

template <size_t N> static void checkClassInfos(const uint (&data)[N])
{
    QCOMPARE(data[2], 2U);
    QCOMPARE_GE(data[3], 14U);

    const uint *classinfos = data + data[3];
    QCOMPARE(classinfos[0], 1U);
    QCOMPARE(classinfos[1], 2U);
    QCOMPARE(classinfos[2], 3U);
    QCOMPARE(classinfos[3], 4U);
}

void tst_MocHelpers::classinfoDataGroup()
{
    constexpr auto data = BobUIMocHelpers::metaObjectData<void, void>(0, dummyStringData,
            BobUIMocHelpers::UintData{}, BobUIMocHelpers::UintData{},
            BobUIMocHelpers::UintData{}, -1, BobUIMocHelpers::UintData{},
            BobUIMocHelpers::ClassInfos({{1, 2}, {3, 4}}));
    checkClassInfos(data.staticData.data);
}

template <typename MetaTypeHolder> constexpr auto getMetaTypes(MetaTypeHolder)
{
    BobUIMocHelpers::MetaObjectContents<1, 1, 1, MetaTypeHolder::count()> r = {};
    uint metatypeoffset = 0;
    MetaTypeHolder::template copyTo<void>(r, metatypeoffset);

    std::array<QMetaType, MetaTypeHolder::count()> result;
    for (uint i = 0; i < result.size(); ++i)
        result[i] = QMetaType(r.relocatingData.metaTypes[i]);

    return result;
}

template <typename E, int N> void enumUintData_check(const E (&values)[N])
{
    using namespace BobUIMocHelpers;

    // make an array of dummy offsets
    typename EnumData<E>::EnumEntry namesAndOffsets[N];
    for (int i = 0; i < N; ++i) {
        namesAndOffsets[i].nameIndex = 2 * (i + 7);
        namesAndOffsets[i].value = values[i];
    }

    auto result = EnumData<E>(0, 0, 0).add(namesAndOffsets);
    for (uint i = 0; i < N; ++i) {
        QCOMPARE(result.payload[2 * i + 0], uint(namesAndOffsets[i].nameIndex));
        QCOMPARE(result.payload[2 * i + 1], uint(values[i]));
    }

    if constexpr (sizeof(E) > sizeof(uint)) {
        using U = std::underlying_type_t<E>;
        for (uint i = 0; i < N; ++i)
            QCOMPARE(result.payload[2 * N + i], uint(U(values[i]) >> 32));
    }
}

enum E1 { AnEnumValue };
enum class E2 { V0 = INT_MAX, V1 = INT_MIN };
enum class E3 : qint64 { V = 0x1111'2222'3333'4444, V2 = -V };
void tst_MocHelpers::enumUintData()
{
    using namespace BobUIMocHelpers;
    using namespace BobUIMocConstants;
    {
        auto result = BobUIMocHelpers::EnumData<E1>(1, 1, EnumFlags{});
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 1U);
        QCOMPARE(result.header[2], EnumFlags{});
        QCOMPARE(result.header[3], 0U);
        QCOMPARE(result.payloadSize(), 0U);
    }
    {
        auto result = BobUIMocHelpers::EnumData<QFlags<E1>>(1, 2, EnumIsFlag);
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 2U);
        QCOMPARE(result.header[2], EnumIsFlag);
        QCOMPARE(result.header[3], 0U);
        QCOMPARE(result.payloadSize(), 0U);
    }
    {
        auto result = BobUIMocHelpers::EnumData<E1>(1, 1, EnumFlags{}).add({ { 1, E1::AnEnumValue } });
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 1U);
        QCOMPARE(result.header[2], EnumFlags{});
        QCOMPARE(result.header[3], 1U);
        QCOMPARE(result.payload[0], 1U);
        QCOMPARE(result.payload[1], uint(E1::AnEnumValue));
    }
    {
        auto result = BobUIMocHelpers::EnumData<QFlags<E1>>(1, 2, EnumFlags{}).add({ { 1, E1::AnEnumValue } });
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 2U);
        QCOMPARE(result.header[2], uint(EnumIsFlag));
        QCOMPARE(result.header[3], 1U);
        QCOMPARE(result.payload[0], 1U);
        QCOMPARE(result.payload[1], uint(E1::AnEnumValue));
    }
    {
        constexpr auto result = BobUIMocHelpers::EnumData<E3>(1, 1, EnumIsScoped)
                .add({ { 2, E3::V }, {3, E3::V2 }, });
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 1U);
        QCOMPARE(result.header[2], EnumIsScoped | EnumIs64Bit);
        QCOMPARE(result.header[3], 2U);
        QCOMPARE(result.payload[0], 2U);
        QCOMPARE(result.payload[1], uint(E3::V));
        QCOMPARE(result.payload[2], 3U);
        QCOMPARE(result.payload[3], uint(E3::V2));
        QCOMPARE(result.payload[4], uint(quint64(E3::V) >> 32));
        QCOMPARE(result.payload[5], uint(quint64(E3::V2) >> 32));
    }

    BOBUIest::setThrowOnFail(true);
    {
        enum E { E0, E1 = -1, E2 = 123, E3 = INT_MIN };
        enumUintData_check({E0, E1, E2, E3});
    }
    {
        enum E : quint64 { E0, E1 = quint64(INT_MIN), E2 = 0x1'0000'0000, E3 = quint64(LLONG_MIN) };
        enumUintData_check({E0, E1, E2, E3});
    }
}

template <typename Data> void testUintData(const Data &data)
{
    uint count = 0;
    size_t headerSize = 0;
    size_t payloadSize = 0;
    data.forEach([&](const auto &block) {
        ++count;
        headerSize += block.headerSize();
        payloadSize += block.payloadSize();
    });

    QCOMPARE(data.count(), count);
    QCOMPARE(data.headerSize(), headerSize);
    QCOMPARE(data.payloadSize(), payloadSize);
}

static void
checkEnums(const uint *data, const BobUIPrivate::QMetaTypeInterface *const *metaTypes)
{
    using namespace BobUIMocConstants;
    QCOMPARE(data[8], 4U);
    QCOMPARE_NE(data[9], 0U);

    const uint *header = data + data[9];
    metaTypes += data[6];          // property count

    // E1:
    QCOMPARE(header[0 + 0], 1U);
    QCOMPARE(header[0 + 1], 1U);
    QCOMPARE(header[0 + 2], 0U);
    QCOMPARE(header[0 + 3], 1U);
    QCOMPARE_GE(header[0 + 4], 14U);
    const uint *payload = data + header[0 + 4];
    QCOMPARE(payload[0], 3U);
    QCOMPARE(payload[1], uint(E1::AnEnumValue));
    QCOMPARE(QMetaType(metaTypes[0]), QMetaType::fromType<E1>());

    // E3:
    QCOMPARE(header[5 + 0], 4U);
    QCOMPARE(header[5 + 1], 5U);
    QCOMPARE(header[5 + 2], EnumIsFlag | EnumIsScoped | EnumIs64Bit);
    QCOMPARE(header[5 + 3], 2U);
    QCOMPARE_GE(header[5 + 4], 14U);
    payload = data + header[5 + 4];
    QCOMPARE(payload[0], 6U);
    QCOMPARE(payload[1], uint(E3::V));
    QCOMPARE(payload[2], 8U);
    QCOMPARE(payload[3], uint(E3::V2));
    QCOMPARE(payload[4], uint(quint64(E3::V) >> 32));
    QCOMPARE(payload[5], uint(quint64(E3::V2) >> 32));
    QCOMPARE(QMetaType(metaTypes[1]), QMetaType::fromType<E3>());

    // E2:
    QCOMPARE(header[10 + 0], 7U);
    QCOMPARE(header[10 + 1], 6U);
    QCOMPARE(header[10 + 2], EnumIsFlag | EnumIsScoped);
    QCOMPARE(header[10 + 3], 2U);
    QCOMPARE_GE(header[10 + 4], 14U);
    payload = data + header[10 + 4];
    QCOMPARE(payload[0], 7U);
    QCOMPARE(payload[1], uint(E2::V0));
    QCOMPARE(payload[2], 10U);
    QCOMPARE(payload[3], uint(E2::V1));
    QCOMPARE(QMetaType(metaTypes[2]), QMetaType::fromType<E2>());

    // QFlags<E1>
    QCOMPARE(header[15 + 0], 11U);
    QCOMPARE(header[15 + 1], 1U);
    QCOMPARE(header[15 + 2], EnumIsFlag);
    QCOMPARE(header[15 + 3], 1U);
    QCOMPARE_GE(header[15 + 4], 14U);
    payload = data + header[15 + 4];
    QCOMPARE(payload[0], 3U);
    QCOMPARE(payload[1], uint(E1::AnEnumValue));
    QCOMPARE(QMetaType(metaTypes[3]), QMetaType::fromType<QFlags<E1>>());
}

void tst_MocHelpers::enumUintGroup()
{
    using namespace BobUIMocConstants;
    BOBUIest::setThrowOnFail(true);
    constexpr BobUIMocHelpers::UintData enums = {
        BobUIMocHelpers::EnumData<E1>(1, 1, 0x00).add({ { 3, E1::AnEnumValue } }),
        BobUIMocHelpers::EnumData<E3>(4, 5, EnumIsFlag | EnumIsScoped)
            .add({ { 6, E3::V }, { 8, E3::V2 }, }),
        BobUIMocHelpers::EnumData<E2>(7, 6, EnumIsFlag | EnumIsScoped)
            .add({ { 7, E2::V0 }, { 10, E2::V1 }, }),
        BobUIMocHelpers::EnumData<QFlags<E1>>(11, 1, EnumIsFlag).add({ { 3, E1::AnEnumValue } }),
    };
    testUintData(enums);

    constexpr auto data = BobUIMocHelpers::metaObjectData<void, void>(0, dummyStringData,
            BobUIMocHelpers::UintData{}, BobUIMocHelpers::UintData{}, enums);
    checkEnums(data.staticData.data, data.relocatingData.metaTypes);
}

void tst_MocHelpers::propertyUintData()
{
    using namespace BobUIMocHelpers;
    {
        auto result = PropertyData<int>(3, QMetaType::Int, 0x3, 13, 0x101);
        QCOMPARE(result.payloadSize(), 0U);
        QCOMPARE(result.header[0], 3U);
        QCOMPARE(result.header[1], uint(QMetaType::Int));
        QCOMPARE(result.header[2], 0x3U);
        QCOMPARE(result.header[3], 13U);
        QCOMPARE(result.header[4], 0x0101U);
    }
    {
        // check that QMetaType doesn't override if it's an alias
        using Dummy = QString;
        auto result = PropertyData<Dummy>(3, 0x80000000 | 4, 0x03);
        QCOMPARE(result.header[1], 0x80000000U | 4);
    }
    {
        // Or derived from
        struct Dummy : QString {};
        auto result = PropertyData<Dummy>(3, 0x80000000 | 4, 0x03);
        QCOMPARE(result.header[1], 0x80000000U | 4);
    }
}

static void
checkProperties(const uint *data, const BobUIPrivate::QMetaTypeInterface *const *metaTypes)
{
    QCOMPARE(data[6], 3U);
    QCOMPARE_NE(data[7], 0U);

    const uint *header = data + data[7];

    QCOMPARE(header[0 + 0], 3U);
    QCOMPARE(header[0 + 1], uint(QMetaType::Int));
    QCOMPARE(header[0 + 2], 0x3U);
    QCOMPARE(header[0 + 3], 13U);
    QCOMPARE(header[0 + 4], 0x0101U);
    QCOMPARE(QMetaType(metaTypes[0]), QMetaType::fromType<int>());

    QCOMPARE(header[5 + 0], 4U);
    QCOMPARE(header[5 + 1], 0x80000000U | 5);
    QCOMPARE(header[5 + 2], 0x3U);
    QCOMPARE(header[5 + 3], uint(-1));
    QCOMPARE(header[5 + 4], 0U);
    QCOMPARE(QMetaType(metaTypes[1]), QMetaType::fromType<QString>());

    QCOMPARE(header[10 + 0], 6U);
    QCOMPARE(header[10 + 1], 0x80000000U | 7);
    QCOMPARE(header[10 + 2], 0x3U);
    QCOMPARE(header[10 + 3], uint(-1));
    QCOMPARE(header[10 + 4], 0U);
    QCOMPARE(QMetaType(metaTypes[2]), QMetaType::fromType<tst_MocHelpers *>());
}

void tst_MocHelpers::propertyUintGroup()
{
    BOBUIest::setThrowOnFail(true);
    constexpr BobUIMocHelpers::UintData properties = {
        BobUIMocHelpers::PropertyData<int>(3, QMetaType::Int, 0x3, 13, 0x101),
        BobUIMocHelpers::PropertyData<QString>(4, 0x80000000 | 5, 0x03),
        BobUIMocHelpers::PropertyData<tst_MocHelpers *>(6, 0x80000000 | 7, 0x03)
    };
    testUintData(properties);

    constexpr auto data = BobUIMocHelpers::metaObjectData<void, void>(0, dummyStringData,
            BobUIMocHelpers::UintData{}, properties, BobUIMocHelpers::UintData{});
    checkProperties(data.staticData.data, data.relocatingData.metaTypes);
}

void tst_MocHelpers::methodUintData()
{
    using namespace BobUIMocHelpers;
    using namespace BobUIMocConstants;
    {
        auto result = SignalData<void()>(1, 2, AccessPublic, QMetaType::Void, {});
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 0U);
        QCOMPARE(result.header[3], 2U);
        QCOMPARE(result.header[4], AccessPublic | MethodSignal);
        QCOMPARE(result.header[5], 0U);
        QCOMPARE(result.payload[0], uint(QMetaType::Void));

        QCOMPARE(result.metaTypes().count(), 1);
        std::array mt = getMetaTypes(result.metaTypes());
        QCOMPARE(mt[0], QMetaType::fromType<void>());
    }
    {
        auto result = SlotData<void (const QString &) const>(1, 2, AccessPublic,
                    QMetaType::Void, { { { QMetaType::QString, 1000 } } });
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 1U);
        QCOMPARE(result.header[3], 2U);
        QCOMPARE(result.header[4], AccessPublic | MethodSlot | MethodIsConst);
        QCOMPARE(result.header[5], 0U);
        QCOMPARE(result.payload[0], uint(QMetaType::Void));
        QCOMPARE(result.payload[1], uint(QMetaType::QString));
        QCOMPARE(result.payload[2], 1000U);

        QCOMPARE(result.metaTypes().count(), 2);
        std::array mt = getMetaTypes(result.metaTypes());
        QCOMPARE(mt[0], QMetaType::fromType<void>());
        QCOMPARE(mt[1], QMetaType::fromType<QString>());
    }
    {
        auto result = RevisionedSlotData<void (const QString &)>(1, 2, AccessPublic, 0xff01,
                    QMetaType::Void, { { { QMetaType::QString, 1000 } } });
        QCOMPARE(result.header[0], 1U);
        QCOMPARE(result.header[1], 1U);
        QCOMPARE(result.header[3], 2U);
        QCOMPARE(result.header[4], AccessPublic | MethodSlot | MethodRevisioned);
        QCOMPARE(result.header[5], 0U);
        QCOMPARE(result.payload[0], 0xff01U);
        QCOMPARE(result.payload[1], uint(QMetaType::Void));
        QCOMPARE(result.payload[2], uint(QMetaType::QString));
        QCOMPARE(result.payload[3], 1000U);

        QCOMPARE(result.metaTypes().count(), 2);
        std::array mt = getMetaTypes(result.metaTypes());
        QCOMPARE(mt[0], QMetaType::fromType<void>());
        QCOMPARE(mt[1], QMetaType::fromType<QString>());
    }
}

static void
checkMethods(const uint *data, const BobUIPrivate::QMetaTypeInterface *const *metaTypes)
{
    using namespace BobUIMocConstants;
    QCOMPARE(data[4], 3U);
    QCOMPARE_NE(data[5], 0U);

    const uint *header = data + data[5];
    uint initialMetaTypeOffset = data[6] + data[8] + 1; // propcount + enumcount + object

    // signals: void signal()
    QCOMPARE(header[0], 1U);
    QCOMPARE(header[1], 0U);
    QCOMPARE_NE(header[2], 0U);
    QCOMPARE(header[3], 2U);
    QCOMPARE(header[4], AccessPublic | MethodSignal | MethodRevisioned);
    QCOMPARE(header[5], initialMetaTypeOffset);
    const uint *payload = data + header[2];
    QCOMPARE(payload[-1], 0x0509U);
    QCOMPARE(payload[0], uint(QMetaType::Void));
    QCOMPARE(QMetaType(metaTypes[header[5]]), QMetaType::fromType<void>());

    // signals: void signal(E1, Dummy) [Dummy = QString]
    header += 6;
    QCOMPARE(header[0], 3U);
    QCOMPARE(header[1], 2U);
    QCOMPARE_NE(header[2], 0U);
    QCOMPARE(header[3], 2U);
    QCOMPARE(header[4], AccessPublic | MethodSignal);
    QCOMPARE(header[5], initialMetaTypeOffset + 1);
    payload = data + header[2];
    QCOMPARE(payload[0], uint(QMetaType::Void));
    QCOMPARE(payload[1], 0x80000000U | 4);  // not a builtin type
    QCOMPARE(payload[2], 0x80000000U | 5);
    QCOMPARE(payload[3], 6U);
    QCOMPARE(payload[4], 7U);
    QCOMPARE(QMetaType(metaTypes[header[5] + 0]), QMetaType::fromType<void>());
    QCOMPARE(QMetaType(metaTypes[header[5] + 1]), QMetaType::fromType<E1>());
    QCOMPARE(QMetaType(metaTypes[header[5] + 2]), QMetaType::fromType<QString>());

    // public slots: bool slot(QString &) const
    header += 6;
    QCOMPARE(header[0], 8U);
    QCOMPARE(header[1], 1U);
    QCOMPARE_NE(header[2], 0U);
    QCOMPARE(header[3], 2U);
    QCOMPARE(header[4], AccessPublic | MethodSlot | MethodIsConst);
    QCOMPARE(header[5], initialMetaTypeOffset + 4);
    payload = data + header[2];
    QCOMPARE(payload[0], uint(QMetaType::Bool));
    QCOMPARE(payload[1], 0x80000000U | 10); // not a builtin type
    QCOMPARE(payload[2], 11U);
    QCOMPARE(QMetaType(metaTypes[header[5] + 0]), QMetaType::fromType<bool>());
    QCOMPARE(metaTypes[header[5] + 1], nullptr);    // is a reference
}

void tst_MocHelpers::methodUintGroup()
{
    BOBUIest::setThrowOnFail(true);
    using Dummy = QString;
    constexpr BobUIMocHelpers::UintData methods = {
        BobUIMocHelpers::RevisionedSignalData<void()>(1, 2, BobUIMocConstants::AccessPublic, 0x509,
            QMetaType::Void, {{ }}
        ),
        BobUIMocHelpers::SignalData<void (E1, Dummy)>(3, 2, BobUIMocConstants::AccessPublic,
            QMetaType::Void, {{ { 0x80000000 | 4, 6 }, { 0x80000000 | 5, 7 }} }
        ),
        BobUIMocHelpers::SlotData<bool (QString &) const>(8, 2, BobUIMocConstants::AccessPublic,
            QMetaType::Bool, {{ { 0x80000000 | 10,  11 } }}
        )
    };
    testUintData(methods);

    constexpr auto data =
            BobUIMocHelpers::metaObjectData<tst_MocHelpers, tst_MocHelpers>(0, dummyStringData,
                    methods, BobUIMocHelpers::UintData{}, BobUIMocHelpers::UintData{});
    checkMethods(data.staticData.data, data.relocatingData.metaTypes);
}

void tst_MocHelpers::constructorUintData()
{
    using namespace BobUIMocHelpers;
    using namespace BobUIMocConstants;
    {
        auto result = ConstructorData<BobUIMocHelpers::NoType()>(2, AccessPublic, {});
        QCOMPARE(result.header[0], 0U);
        QCOMPARE(result.header[1], 0U);
        QCOMPARE(result.header[3], 2U);
        QCOMPARE(result.header[4], AccessPublic | MethodConstructor);
        QCOMPARE(result.header[5], 0U);
        QCOMPARE(result.payload[0], uint(QMetaType::UnknownType));

        QCOMPARE(result.metaTypes().count(), 0);
    }
    {
        auto result = ConstructorData<BobUIMocHelpers::NoType(QObject *)>(1, AccessPublic,
                                                                       {{ { QMetaType::QObjectStar, 2 } }});
        QCOMPARE(result.header[0], 0U);
        QCOMPARE(result.header[1], 1U);
        QCOMPARE(result.header[3], 1U);
        QCOMPARE(result.header[4], AccessPublic | MethodConstructor);
        QCOMPARE(result.header[5], 0U);
        QCOMPARE(result.payload[0], uint(QMetaType::UnknownType));
        QCOMPARE(result.payload[1], uint(QMetaType::QObjectStar));
        QCOMPARE(result.payload[2], 2U);

        QCOMPARE(result.metaTypes().count(), 1);
        std::array mt = getMetaTypes(result.metaTypes());
        QCOMPARE(mt[0], QMetaType::fromType<QObject *>());
    }
}

static void
checkConstructors(const uint *data, const BobUIPrivate::QMetaTypeInterface *const *metaTypes)
{
    using namespace BobUIMocConstants;
    QCOMPARE(data[10], 3U);
    QCOMPARE_NE(data[11], 0U);

    const uint *header = data + data[11];

    // Constructor(QObject *)
    QCOMPARE(header[0], 0U);
    QCOMPARE(header[1], 1U);
    QCOMPARE_NE(header[2], 0U);
    QCOMPARE(header[3], 1U);
    QCOMPARE(header[4], AccessPublic | MethodConstructor);
    QCOMPARE_GT(header[5], 0U);
    const uint *payload = data + header[2];
    QCOMPARE(payload[0], uint(QMetaType::UnknownType));
    QCOMPARE(payload[1], uint(QMetaType::QObjectStar));
    QCOMPARE(QMetaType(metaTypes[header[5]]), QMetaType::fromType<QObject *>());

    // Constructor() [cloned from the previous with a default argument]
    header += 6;
    QCOMPARE(header[0], 0U);
    QCOMPARE(header[1], 0U);
    QCOMPARE_NE(header[2], 0U);
    QCOMPARE(header[3], 1U);
    QCOMPARE(header[4], AccessPublic | MethodConstructor | MethodCloned);
    QCOMPARE_GT(header[5], 0U);
    payload = data + header[2];
    QCOMPARE(payload[0], uint(QMetaType::UnknownType));
    // no metatype stored for this constructor

    // Constructor(const QString &)
    header += 6;
    QCOMPARE(header[0], 0U);
    QCOMPARE(header[1], 1U);
    QCOMPARE_NE(header[2], 0U);
    QCOMPARE(header[3], 1U);
    QCOMPARE(header[4], AccessPublic | MethodConstructor);
    QCOMPARE_GT(header[5], 0U);
    payload = data + header[2];
    QCOMPARE(payload[0], uint(QMetaType::UnknownType));
    QCOMPARE(payload[1], uint(QMetaType::QString));
    QCOMPARE(QMetaType(metaTypes[header[5]]), QMetaType::fromType<QString>());
}

void tst_MocHelpers::constructorUintGroup()
{
    using BobUIMocHelpers::NoType;
    BOBUIest::setThrowOnFail(true);
    constexpr BobUIMocHelpers::UintData constructors = {
        BobUIMocHelpers::ConstructorData<NoType(QObject *)>(1, BobUIMocConstants::AccessPublic,
            {{ { QMetaType::QObjectStar, 2 } }}
        ),
        BobUIMocHelpers::ConstructorData<NoType()>(1, BobUIMocConstants::AccessPublic | BobUIMocConstants::MethodCloned,
            {{ }}
        ),
        BobUIMocHelpers::ConstructorData<NoType(const QString &)>(1, BobUIMocConstants::AccessPublic,
            {{ { QMetaType::QString,  3 }, }}
        )
    };
    testUintData(constructors);

    constexpr auto data = BobUIMocHelpers::metaObjectData<void, void>(0, dummyStringData,
            BobUIMocHelpers::UintData{}, BobUIMocHelpers::UintData{},
            BobUIMocHelpers::UintData{}, -1, constructors);
    checkConstructors(data.staticData.data, data.relocatingData.metaTypes);
}

struct Gadget {};

template <size_t N> static void checkUintArrayGeneric(const uint (&data)[N], uint flags = 0)
{
    using namespace BobUIMocConstants;
    QCOMPARE(data[0], uint(OutputRevision));
    QCOMPARE(data[1], 0U);
    QCOMPARE(data[12], flags);
    QCOMPARE(data[N-1], 0U);

    // check the offsets are valid
    QCOMPARE_LT(data[2], N);     // classinfos
    QCOMPARE_LT(data[4], N);     // methods
    QCOMPARE_LT(data[6], N);     // properties
    QCOMPARE_LT(data[8], N);     // enums
    QCOMPARE_LT(data[10], N);    // constructors
}

void tst_MocHelpers::emptyUintArray()
{
    using namespace BobUIMocConstants;
    constexpr auto mo = BobUIMocHelpers::metaObjectData<Gadget, void>(MetaObjectFlag{}, dummyStringData,
            BobUIMocHelpers::UintData{}, BobUIMocHelpers::UintData{}, BobUIMocHelpers::UintData{});
    BOBUIest::setThrowOnFail(true);
    checkUintArrayGeneric(mo.staticData.data, MetaObjectFlag{});
    BOBUIest::setThrowOnFail(false);

    const uint *data = mo.staticData.data;
    auto &metaTypes = mo.relocatingData.metaTypes;

    // check it says nothing was added
    QCOMPARE(data[2], 0U);      // classinfos
    QCOMPARE(data[4], 0U);      // methods
    QCOMPARE(data[6], 0U);      // properties
    QCOMPARE(data[8], 0U);      // enums
    QCOMPARE(data[10], 0U);     // constructors
    QCOMPARE(data[13], 0U);     // signals

    QCOMPARE(std::size(metaTypes), 1U);
    QMetaType self(metaTypes[data[6] + data[8]]);
    QCOMPARE(self, QMetaType::fromType<Gadget>());
}

void tst_MocHelpers::uintArrayNoMethods()
{
    using namespace BobUIMocConstants;
    constexpr auto mo = BobUIMocHelpers::metaObjectData<Gadget, Gadget>(PropertyAccessInStaticMetaCall,
            dummyStringData,
            BobUIMocHelpers::UintData{},
            BobUIMocHelpers::UintData{
                BobUIMocHelpers::PropertyData<int>(3, QMetaType::Int, 0x3, 13, 0x101),
                BobUIMocHelpers::PropertyData<QString>(4, 0x80000000 | 5, 0x03),
                BobUIMocHelpers::PropertyData<tst_MocHelpers *>(6, 0x80000000 | 7, 0x03)
            }, BobUIMocHelpers::UintData{
                BobUIMocHelpers::EnumData<E1>(1, 1, 0x00).add({ { 3, E1::AnEnumValue } }),
                BobUIMocHelpers::EnumData<E3>(4, 5, EnumIsFlag | EnumIsScoped)
                    .add({ { 6, E3::V }, { 8, E3::V2 }, }),
                BobUIMocHelpers::EnumData<E2>(7, 6, EnumIsFlag | EnumIsScoped)
                    .add({ { 7, E2::V0 }, { 10, E2::V1 }, }),
                BobUIMocHelpers::EnumData<QFlags<E1>>(11, 1, EnumIsFlag).add({ { 3, E1::AnEnumValue } }),
            }, -1, BobUIMocHelpers::UintData{}, BobUIMocHelpers::ClassInfos({{1, 2}, {3, 4}}));

    auto &data = mo.staticData.data;
    auto &metaTypes = mo.relocatingData.metaTypes;

    BOBUIest::setThrowOnFail(true);
    checkUintArrayGeneric(data, PropertyAccessInStaticMetaCall);
    checkClassInfos(data);
    checkProperties(data, metaTypes);
    checkEnums(data, metaTypes);
    BOBUIest::setThrowOnFail(false);
    QCOMPARE(data[4], 0U);      // methods
    QCOMPARE(data[10], 0U);     // constructors
    QCOMPARE(data[13], 0U);     // signals
    QMetaType self(metaTypes[data[6] + data[8]]);
    QCOMPARE(self, QMetaType::fromType<Gadget>());
}

void tst_MocHelpers::uintArray()
{
    using Dummy = QString;
    using BobUIMocHelpers::NoType;
    using namespace BobUIMocConstants;
    constexpr auto mo = BobUIMocHelpers::metaObjectData<tst_MocHelpers, tst_MocHelpers>(PropertyAccessInStaticMetaCall,
            dummyStringData,
            BobUIMocHelpers::UintData{
                BobUIMocHelpers::RevisionedSignalData<void()>(1, 2, BobUIMocConstants::AccessPublic, 0x509,
                    QMetaType::Void, {{ }}
                ),
                BobUIMocHelpers::SignalData<void (E1, Dummy)>(3, 2, BobUIMocConstants::AccessPublic,
                    QMetaType::Void, {{ { 0x80000000 | 4, 6 }, { 0x80000000 | 5, 7 }} }
                ),
                BobUIMocHelpers::SlotData<bool (QString &) const>(8, 2, BobUIMocConstants::AccessPublic,
                    QMetaType::Bool, {{ { 0x80000000 | 10,  11 } }}
                )
            },
            BobUIMocHelpers::UintData{
                BobUIMocHelpers::PropertyData<int>(3, QMetaType::Int, 0x3, 13, 0x101),
                BobUIMocHelpers::PropertyData<QString>(4, 0x80000000 | 5, 0x03),
                BobUIMocHelpers::PropertyData<tst_MocHelpers *>(6, 0x80000000 | 7, 0x03)
            }, BobUIMocHelpers::UintData{
                BobUIMocHelpers::EnumData<E1>(1, 1, 0x00).add({ { 3, E1::AnEnumValue } }),
                BobUIMocHelpers::EnumData<E3>(4, 5, EnumIsFlag | EnumIsScoped)
                    .add({ { 6, E3::V }, { 8, E3::V2 }, }),
                BobUIMocHelpers::EnumData<E2>(7, 6, EnumIsFlag | EnumIsScoped)
                    .add({ { 7, E2::V0 }, { 10, E2::V1 }, }),
                BobUIMocHelpers::EnumData<QFlags<E1>>(11, 1, EnumIsFlag).add({ { 3, E1::AnEnumValue } }),
            },
            -1,
            BobUIMocHelpers::UintData{
                BobUIMocHelpers::ConstructorData<NoType(QObject *)>(1, BobUIMocConstants::AccessPublic,
                    {{ { QMetaType::QObjectStar, 2 } }}
                ),
                BobUIMocHelpers::ConstructorData<NoType()>(1, BobUIMocConstants::AccessPublic | BobUIMocConstants::MethodCloned,
                    {{ }}
                ),
                BobUIMocHelpers::ConstructorData<NoType(const QString &)>(1, BobUIMocConstants::AccessPublic,
                    {{ { QMetaType::QString,  3 }, }}
                )
            }, BobUIMocHelpers::ClassInfos({{1, 2}, {3, 4}}));

    auto &data = mo.staticData.data;
    auto &metaTypes = mo.relocatingData.metaTypes;

    BOBUIest::setThrowOnFail(true);
    checkUintArrayGeneric(data, PropertyAccessInStaticMetaCall);
    checkClassInfos(data);
    checkProperties(data, metaTypes);
    checkEnums(data, metaTypes);
    checkMethods(data, metaTypes);
    checkConstructors(data, metaTypes);
    QMetaType self(metaTypes[data[6] + data[8]]);
    QCOMPARE(self, QMetaType::fromType<tst_MocHelpers>());
}

BOBUIEST_MAIN(tst_MocHelpers)
#include "tst_mochelpers.moc"
