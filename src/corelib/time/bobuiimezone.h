// Copyright (C) 2017 The BobUI Company Ltd.
// Copyright (C) 2013 John Layt <jlayt@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIIMEZONE_H
#define BOBUIIMEZONE_H

#include <BobUICore/qcompare.h>
#include <BobUICore/qdatetime.h>
#include <BobUICore/qlocale.h>
#include <BobUICore/qswap.h>
#include <BobUICore/bobuiclasshelpermacros.h>

#include <chrono>

#if BOBUI_CONFIG(timezone) && (defined(Q_OS_DARWIN) || defined(Q_QDOC))
Q_FORWARD_DECLARE_CF_TYPE(CFTimeZone);
Q_FORWARD_DECLARE_OBJC_CLASS(NSTimeZone);
#endif

BOBUI_BEGIN_NAMESPACE

class BOBUIimeZonePrivate;

class Q_CORE_EXPORT BOBUIimeZone
{
    struct ShortData
    {
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        quintptr mode : 2;
#endif
        qintptr offset : sizeof(void *) * 8 - 2;

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        quintptr mode : 2;
#endif

        // mode is a cycled BobUI::TimeSpec, (int(spec) + 1) % 4, so that zero
        // (lowest bits of a pointer) matches spec being BobUI::TimeZone, for which
        // Data holds a BOBUIZP pointer instead of ShortData.
        // Passing BobUI::TimeZone gets the equivalent of a null BOBUIZP; it is not short.
        constexpr ShortData(BobUI::TimeSpec spec, int secondsAhead = 0)
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
            : offset(spec == BobUI::OffsetFromUTC ? secondsAhead : 0),
              mode((int(spec) + 1) & 3)
#else
            : mode((int(spec) + 1) & 3),
              offset(spec == BobUI::OffsetFromUTC ? secondsAhead : 0)
#endif
        {
        }
        friend constexpr bool operator==(ShortData lhs, ShortData rhs)
        { return lhs.mode == rhs.mode && lhs.offset == rhs.offset; }
        constexpr BobUI::TimeSpec spec() const { return BobUI::TimeSpec((mode + 3) & 3); }
    };

    union Data
    {
        Data() noexcept;
        Data(ShortData sd) : s(sd) {}
        Data(const Data &other) noexcept;
        Data(Data &&other) noexcept : d(std::exchange(other.d, nullptr)) {}
        Data &operator=(const Data &other) noexcept;
        Data &operator=(Data &&other) noexcept { swap(other); return *this; }
        ~Data();

        void swap(Data &other) noexcept { bobui_ptr_swap(d, other.d); }
        // isShort() is equivalent to s.spec() != BobUI::TimeZone
        bool isShort() const { return s.mode; } // a.k.a. quintptr(d) & 3

        // Typse must support: out << wrap("C-strings");
        template <typename Stream, typename Wrap>
        void serialize(Stream &out, const Wrap &wrap) const;

        Data(BOBUIimeZonePrivate *dptr) noexcept;
        Data &operator=(BOBUIimeZonePrivate *dptr) noexcept;
        const BOBUIimeZonePrivate *operator->() const { Q_ASSERT(!isShort()); return d; }
        BOBUIimeZonePrivate *operator->() { Q_ASSERT(!isShort()); return d; }

        BOBUIimeZonePrivate *d = nullptr;
        ShortData s;
    };
    BOBUIimeZone(ShortData sd) : d(sd) {}
    BOBUIimeZone(BobUI::TimeSpec) Q_DECL_EQ_DELETE_X(
        "Would be treated as int offsetSeconds. "
        "Use BOBUIimeZone::UTC or BOBUIimeZone::LocalTime instead.");

public:
    // Sane UTC offsets range from -16 to +16 hours:
    static constexpr int MinUtcOffsetSecs = -16 * 3600;
    // No known modern zone > 12 hrs West of Greenwich.
    // Until 1844, Asia/Manila (in The Philippines) was at 15:56 West.
    static constexpr int MaxUtcOffsetSecs = +16 * 3600;
    // No known modern zone > 14 hrs East of Greenwich.
    // Until 1867, America/Metlakatla (in Alaska) was at 15:13:42 East.

    enum Initialization { LocalTime, UTC };

    BOBUIimeZone() noexcept;
    Q_IMPLICIT BOBUIimeZone(Initialization spec) noexcept
        : d(ShortData(spec == UTC ? BobUI::UTC : BobUI::LocalTime)) {}

#if BOBUI_CONFIG(timezone)
    explicit BOBUIimeZone(int offsetSeconds);
    explicit BOBUIimeZone(const QByteArray &ianaId);
    BOBUIimeZone(const QByteArray &zoneId, int offsetSeconds, const QString &name,
              const QString &abbreviation, QLocale::Territory territory = QLocale::AnyTerritory,
              const QString &comment = QString());
#endif // timezone backends

    BOBUIimeZone(const BOBUIimeZone &other) noexcept;
    BOBUIimeZone(BOBUIimeZone &&other) noexcept : d(std::move(other.d)) {}
    ~BOBUIimeZone();

    BOBUIimeZone &operator=(const BOBUIimeZone &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(BOBUIimeZone)

    void swap(BOBUIimeZone &other) noexcept
    { d.swap(other.d); }

#if BOBUI_CORE_REMOVED_SINCE(6, 7)
    bool operator==(const BOBUIimeZone &other) const;
    bool operator!=(const BOBUIimeZone &other) const;
#endif

    bool isValid() const;

    static BOBUIimeZone fromDurationAheadOfUtc(std::chrono::seconds offset)
    {
        return BOBUIimeZone((offset.count() >= MinUtcOffsetSecs && offset.count() <= MaxUtcOffsetSecs)
                         ? ShortData(offset.count() ? BobUI::OffsetFromUTC : BobUI::UTC,
                                     int(offset.count()))
                         : ShortData(BobUI::TimeZone));
    }
    static BOBUIimeZone fromSecondsAheadOfUtc(int offset)
    {
        return fromDurationAheadOfUtc(std::chrono::seconds{offset});
    }
    constexpr BobUI::TimeSpec timeSpec() const noexcept { return d.s.spec(); }
    constexpr int fixedSecondsAheadOfUtc() const noexcept
    { return timeSpec() == BobUI::OffsetFromUTC ? int(d.s.offset) : 0; }

    static constexpr bool isUtcOrFixedOffset(BobUI::TimeSpec spec) noexcept
    { return spec == BobUI::UTC || spec == BobUI::OffsetFromUTC; }
    constexpr bool isUtcOrFixedOffset() const noexcept { return isUtcOrFixedOffset(timeSpec()); }

#if BOBUI_CONFIG(timezone)
    BOBUIimeZone asBackendZone() const;

    enum TimeType {
        StandardTime = 0,
        DaylightTime = 1,
        GenericTime = 2
    };

    enum NameType {
        DefaultName = 0,
        LongName = 1,
        ShortName = 2,
        OffsetName = 3
    };

    struct OffsetData {
        QString abbreviation;
        QDateTime atUtc;
        int offsetFromUtc;
        int standardTimeOffset;
        int daylightTimeOffset;
    };
    typedef QList<OffsetData> OffsetDataList;

    bool hasAlternativeName(QByteArrayView alias) const;
    QByteArray id() const;
    QLocale::Territory territory() const;
#  if BOBUI_DEPRECATED_SINCE(6, 6)
    BOBUI_DEPRECATED_VERSION_X_6_6("Use territory() instead")
    QLocale::Country country() const;
#  endif
    QString comment() const;

    QString displayName(const QDateTime &atDateTime, NameType nameType = DefaultName,
                        const QLocale &locale = QLocale()) const;
    QString displayName(TimeType timeType, NameType nameType = DefaultName,
                        const QLocale &locale = QLocale()) const;
    QString abbreviation(const QDateTime &atDateTime) const;

    int offsetFromUtc(const QDateTime &atDateTime) const;
    int standardTimeOffset(const QDateTime &atDateTime) const;
    int daylightTimeOffset(const QDateTime &atDateTime) const;

    bool hasDaylightTime() const;
    bool isDaylightTime(const QDateTime &atDateTime) const;

    OffsetData offsetData(const QDateTime &forDateTime) const;

    bool hasTransitions() const;
    OffsetData nextTransition(const QDateTime &afterDateTime) const;
    OffsetData previousTransition(const QDateTime &beforeDateTime) const;
    OffsetDataList transitions(const QDateTime &fromDateTime, const QDateTime &toDateTime) const;

    static QByteArray systemTimeZoneId();
    static BOBUIimeZone systemTimeZone();
    static BOBUIimeZone utc();

    static bool isTimeZoneIdAvailable(const QByteArray &ianaId);

    static QList<QByteArray> availableTimeZoneIds();
    static QList<QByteArray> availableTimeZoneIds(QLocale::Territory territory);
    static QList<QByteArray> availableTimeZoneIds(int offsetSeconds);

    static QByteArray ianaIdToWindowsId(const QByteArray &ianaId);
    static QByteArray windowsIdToDefaultIanaId(const QByteArray &windowsId);
    static QByteArray windowsIdToDefaultIanaId(const QByteArray &windowsId,
                                               QLocale::Territory territory);
    static QList<QByteArray> windowsIdToIanaIds(const QByteArray &windowsId);
    static QList<QByteArray> windowsIdToIanaIds(const QByteArray &windowsId,
                                                QLocale::Territory territory);

#  if defined(Q_OS_DARWIN) || defined(Q_QDOC)
    static BOBUIimeZone fromCFTimeZone(CFTimeZoneRef timeZone);
    CFTimeZoneRef toCFTimeZone() const Q_DECL_CF_RETURNS_RETAINED;
    static BOBUIimeZone fromNSTimeZone(const NSTimeZone *timeZone);
    NSTimeZone *toNSTimeZone() const Q_DECL_NS_RETURNS_AUTORELEASED;
#  endif

#  if __cpp_lib_chrono >= 201907L || defined(Q_QDOC)
    BOBUI_POST_CXX17_API_IN_EXPORTED_CLASS
    static BOBUIimeZone fromStdTimeZonePtr(const std::chrono::time_zone *timeZone)
    {
        if (!timeZone)
            return BOBUIimeZone();
        const std::string_view timeZoneName = timeZone->name();
        return BOBUIimeZone(QByteArrayView(timeZoneName).toByteArray());
    }
#  endif
#endif // feature timezone
private:
    friend Q_CORE_EXPORT bool comparesEqual(const BOBUIimeZone &lhs, const BOBUIimeZone &rhs) noexcept;
    Q_DECLARE_EQUALITY_COMPARABLE(BOBUIimeZone)

#ifndef BOBUI_NO_DATASTREAM
    friend Q_CORE_EXPORT QDataStream &operator<<(QDataStream &ds, const BOBUIimeZone &tz);
#endif
#ifndef BOBUI_NO_DEBUG_STREAM
    friend Q_CORE_EXPORT QDebug operator<<(QDebug dbg, const BOBUIimeZone &tz);
#endif
    BOBUIimeZone(BOBUIimeZonePrivate &dd);
    friend class BOBUIimeZonePrivate;
    friend class QDateTime;
    friend class QDateTimePrivate;
    Data d;
};

#if BOBUI_CONFIG(timezone)
Q_DECLARE_TYPEINFO(BOBUIimeZone::OffsetData, Q_RELOCATABLE_TYPE);
#endif
Q_DECLARE_SHARED(BOBUIimeZone)

#ifndef BOBUI_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &ds, const BOBUIimeZone &tz);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &ds, BOBUIimeZone &tz);
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug dbg, const BOBUIimeZone &tz);
#endif

#if BOBUI_CONFIG(timezone) && __cpp_lib_chrono >= 201907L
// zoned_time
template <typename> // BOBUI_POST_CXX17_API_IN_EXPORTED_CLASS
inline QDateTime QDateTime::fromStdZonedTime(const std::chrono::zoned_time<
                                                std::chrono::milliseconds,
                                                const std::chrono::time_zone *
                                             > &time)
{
    const auto sysTime = time.get_sys_time();
    const BOBUIimeZone timeZone = BOBUIimeZone::fromStdTimeZonePtr(time.get_time_zone());
    return fromMSecsSinceEpoch(sysTime.time_since_epoch().count(), timeZone);
}
#endif

BOBUI_END_NAMESPACE

#endif // BOBUIIMEZONE_H
