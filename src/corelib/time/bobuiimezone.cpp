// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2013 John Layt <jlayt@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiimezone.h"
#if BOBUI_CONFIG(timezone)
#  include "bobuiimezoneprivate_p.h"
#endif

#include <BobUICore/qdatastream.h>
#include <BobUICore/qdatetime.h>

#include <qdebug.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

static_assert(!std::is_constructible_v<BOBUIimeZone, BobUI::TimeSpec>);
using namespace BobUI::StringLiterals;

#if BOBUI_CONFIG(timezone)
// Create default time zone using appropriate backend
static BOBUIimeZonePrivate *newBackendTimeZone()
{
#if BOBUI_CONFIG(timezone_tzdb)
    return new QChronoTimeZonePrivate();
#elif defined(Q_OS_DARWIN)
    return new QMacTimeZonePrivate();
#elif defined(Q_OS_ANDROID)
    return new QAndroidTimeZonePrivate();
#elif defined(Q_OS_UNIX) && !defined(Q_OS_VXWORKS)
    return new BOBUIzTimeZonePrivate();
#elif BOBUI_CONFIG(icu)
    return new QIcuTimeZonePrivate();
#elif defined(Q_OS_WIN)
    return new QWinTimeZonePrivate();
#else
    return new QUtcTimeZonePrivate();
#endif // Backend selection
}

// Create named time zone using appropriate backend
static BOBUIimeZonePrivate *newBackendTimeZone(const QByteArray &ianaId)
{
    Q_ASSERT(!ianaId.isEmpty());
#if BOBUI_CONFIG(timezone_tzdb)
    return new QChronoTimeZonePrivate(ianaId);
#elif defined(Q_OS_DARWIN)
    return new QMacTimeZonePrivate(ianaId);
#elif defined(Q_OS_ANDROID)
    return new QAndroidTimeZonePrivate(ianaId);
#elif defined(Q_OS_UNIX) && !defined(Q_OS_VXWORKS)
    return new BOBUIzTimeZonePrivate(ianaId);
#elif BOBUI_CONFIG(icu)
    return new QIcuTimeZonePrivate(ianaId);
#elif defined(Q_OS_WIN)
    return new QWinTimeZonePrivate(ianaId);
#else
    return new QUtcTimeZonePrivate(ianaId);
#endif // Backend selection
}

class BOBUIimeZoneSingleton
{
public:
    BOBUIimeZoneSingleton() : backend(newBackendTimeZone()) {}

    // The global_tz is the tz to use in static methods such as
    // availableTimeZoneIds() and isTimeZoneIdAvailable() and to create named
    // IANA time zones. This is usually the host system, but may be different if
    // the host resources are insufficient. A simple UTC backend is used if no
    // alternative is available.
    QExplicitlySharedDataPointer<BOBUIimeZonePrivate> backend;
    // TODO BOBUIBUG-56899: refresh should update this backend.
};

Q_GLOBAL_STATIC(BOBUIimeZoneSingleton, global_tz);
#endif // feature timezone

/*!
    \class BOBUIimeZone
    \inmodule BobUICore
    \since 5.2
    \threadsafe

    \brief BOBUIimeZone identifies how a time representation relates to UTC.

    \compares equality

    When dates and times are combined, the meaning of the result depends on how
    time is being represented. There are various international standards for
    representing time; one of these, UTC, corresponds to the traditional
    standard of solar mean time at Greenwich (a.k.a. GMT). All other time
    systems supported by BobUI are ultimately specified in relation to UTC. An
    instance of this class provides a stateless calculator for conversions
    between UTC and other time representations.

    Some time representations are simply defined at a fixed offset to UTC.
    Others are defined by governments for use within their jurisdictions. The
    latter are properly known as time zones, but BOBUIimeZone (since BobUI 6.5) is
    unifies their representation with that of general time systems. One time
    zone generally supported on most operating systems is designated local time;
    this is presumed to correspond to the time zone within which the user is
    living.

    For time zones other than local time, UTC and those at fixed offsets from
    UTC, BobUI can only provide support when the operating system provides some way
    to access that information. When BobUI is built, the \c timezone feature
    controls whether such information is available. When it is not, some
    constructors and methods of BOBUIimeZone are excluded from its API; these are
    documented as depending on feature \c timezone. Note that, even when BobUI is
    built with this feature enabled, it may be unavailable to users whose
    systems are misconfigured, or where some standard packages (for example, the
    \c tzdata package on Linux) are not installed. This feature is enabled by
    default when time zone information is available.

    This class is primarily designed for use in QDateTime; most applications
    will not need to access this class directly and should instead use an
    instance of it when constructing a QDateTime.

    \note For consistency with QDateTime, BOBUIimeZone does not account for leap
    seconds.

    \section1 Remarks

    BOBUIimeZone, like QDateTime, measures offsets from UTC in seconds. This
    contrasts with their measurement of time generally, which they do in
    milliseconds. Real-world time zones generally have UTC offsets that are
    whole-number multiples of five minutes (300 seconds), at least since well
    before 1970. A positive offset from UTC gives a time representation puts
    noon on any given day before UTC noon on that day; a negative offset puts
    noon after UTC noon on the same day.

    \section2 Lightweight Time Representations

    BOBUIimeZone can represent UTC, local time and fixed offsets from UTC even when
    feature \c timezone is disabled. The form in which it does so is also
    available when the feature is enabled; it is a more lightweight form and
    processing using it will typically be more efficient, unless methods only
    available when feature \c timezone is enabled are being exercised. See \l
    Initialization and \l BOBUIimeZone::fromSecondsAheadOfUtc(int) for how to
    construct these representations.

    This documentation distinguishes between "time zone", used to describe a
    time representation described by system-supplied or standard information,
    and time representations more generally, which include these lightweight
    forms. The methods available only when feature \c timezone is enabled are
    apt to be cheaper for time zones than for lightweight time representations,
    for which these methods may construct a suitable transient time zone object
    to which to forward the query.

    \section2 IANA Time Zone IDs

    BOBUIimeZone uses the IANA time zone IDs as defined in the IANA Time Zone
    Database (http://www.iana.org/time-zones). This is to ensure a standard ID
    across all supported platforms.  Most platforms support the IANA IDs
    and the IANA Database natively, but for Windows a mapping is required to
    the native IDs.  See below for more details.

    The IANA IDs can and do change on a regular basis, and can vary depending
    on how recently the host system data was updated.  As such you cannot rely
    on any given ID existing on any host system.  You must use
    availableTimeZoneIds() to determine what IANA IDs are available.

    The IANA IDs and database are also know as the Olson IDs and database,
    named after the original compiler of the database.

    \section2 UTC Offset Time Zones

    A default UTC time zone backend is provided which is always available when
    feature \c timezone is enabled. This provides a set of generic Offset From
    UTC time zones in the range UTC-16:00 to UTC+16:00. These time zones can be
    created using either the standard ISO format names, such as "UTC+00:00", as
    listed by availableTimeZoneIds(), or using a name of similar form in
    combination with the number of offset seconds.

    \section2 Windows Time Zones

    Windows native time zone support is severely limited compared to the
    standard IANA TZ Database.  Windows time zones cover larger geographic
    areas and are thus less accurate in their conversions.  They also do not
    support as much historical data and so may only be accurate for the
    current year.  In particular, when MS's zone data claims that DST was
    observed prior to 1900 (this is historically known to be untrue), the
    claim is ignored and the standard time (allegedly) in force in 1900 is
    taken to have always been in effect.

    BOBUIimeZone uses a conversion table derived from the Unicode CLDR data to map
    between IANA IDs and Windows IDs.  Depending on your version of Windows
    and BobUI, this table may not be able to provide a valid conversion, in which
    "UTC" will be returned.

    BOBUIimeZone provides a public API to use this conversion table.  The Windows ID
    used is the Windows Registry Key for the time zone which is also the MS
    Exchange EWS ID as well, but is different to the Time Zone Name (TZID) and
    COD code used by MS Exchange in versions before 2007.

    \note When BobUI is built with the ICU library, it is used in preference to the
    Windows system APIs, bypassing all problems with those APIs using different
    names.

    \section2 System Time Zone

    The method systemTimeZoneId() returns the current system IANA time zone
    ID which on Unix-like systems will always be correct.  On Windows this ID is
    translated from the Windows system ID using an internal translation
    table and the user's selected country.  As a consequence there is a small
    chance any Windows install may have IDs not known by BobUI, in which case
    "UTC" will be returned.

    Creating a new BOBUIimeZone instance using the system time zone ID will only
    produce a fixed named copy of the time zone, it will not change if the
    system time zone changes.  BOBUIimeZone::systemTimeZone() will return an
    instance representing the zone named by this system ID.  Note that
    constructing a QDateTime using this system zone may behave differently than
    constructing a QDateTime that uses BobUI::LocalTime as its BobUI::TimeSpec, as the
    latter directly uses system APIs for accessing local time information, which
    may behave differently (and, in particular, might adapt if the user adjusts
    the system zone setting).

    \section2 Time Zone Offsets

    The difference between UTC and the local time in a time zone is expressed
    as an offset in seconds from UTC, i.e. the number of seconds to add to UTC
    to obtain the local time.  The total offset is comprised of two component
    parts, the standard time offset and the daylight-saving time offset.  The
    standard time offset is the number of seconds to add to UTC to obtain
    standard time in the time zone.  The daylight-saving time offset is the
    number of seconds to add to the standard time offset to obtain
    daylight-saving time (abbreviated DST and sometimes called "daylight time"
    or "summer time") in the time zone. The usual case for DST (using
    standard time in winter, DST in summer) has a positive daylight-saving
    time offset. However, some zones have negative DST offsets, used in
    winter, with summer using standard time.

    Note that the standard and DST offsets for a time zone may change over time
    as countries have changed DST laws or even their standard time offset.

    \section2 License

    This class includes data obtained from the CLDR data files under the terms
    of the Unicode Data Files and Software License. See
    \l{unicode-cldr}{Unicode Common Locale Data Repository (CLDR)} for details.

    \sa QDateTime, QCalendar
*/

/*!
    \variable BOBUIimeZone::MinUtcOffsetSecs
    \brief Timezone offsets from UTC are expected to be no lower than this.

    The lowest UTC offset of any early 21st century timezone is -12 hours (Baker
    Island, USA), or 12 hours west of Greenwich.

    Historically, until 1844, The Philippines (then controlled by Spain) used
    the same date as Spain's American holdings, so had offsets close to 16 hours
    west of Greenwich. As The Philippines was using local solar mean time, it is
    possible some outlying territory of it may have been operating at more than
    16 hours west of Greenwich, but no early 21st century timezone traces its
    history back to such an extreme.

    \sa MaxUtcOffsetSecs
*/
/*!
    \variable BOBUIimeZone::MaxUtcOffsetSecs
    \brief Timezone offsets from UTC are expected to be no higher than this.

    The highest UTC offset of any early 21st century timezone is +14 hours
    (Christmas Island, Kiribati, Kiritimati), or 14 hours east of Greenwich.

    Historically, before 1867, when Russia sold Alaska to America, Alaska used
    the same date as Russia, so had offsets over 15 hours east of Greenwich. As
    Alaska was using local solar mean time, its offsets varied, but all were
    less than 16 hours east of Greenwich.

    \sa MinUtcOffsetSecs
*/

#if BOBUI_CONFIG(timezone)
/*!
    \enum BOBUIimeZone::TimeType

    A timezone's name may vary seasonally to indicate whether it is using its
    standard offset from UTC or applying a daylight-saving adjustment to that
    offset. In such cases, it typically also has an overall name that applies to
    it regardless of season. When requesting the display name of a zone, this
    type identifies which of those names to use. In time zones that do not apply
    DST, all three values may return the same result.

    \value StandardTime
           The standard-time name of the zone.
           For example, "Pacific Standard Time".
    \value DaylightTime
           The name of the zone when Daylight-Saving is in effect.
           For example, "Pacific Daylight Time".
    \value GenericTime
           The name by which the zone is described independent of whether it is
           applying any daylight-saving adjustment.
           For example, "Pacific Time".

    This type is only available when feature \c timezone is enabled.
*/

/*!
    \enum BOBUIimeZone::NameType

    The type of time zone name.

    \value DefaultName
           The default form of the time zone name, one of LongName, ShortName or
           OffsetName
    \value LongName
           The long form of the time zone name, e.g. "Central European Time"
    \value ShortName
           The short form of the time zone name, usually an abbreviation,
           e.g. "CET", in locales that have one for the zone, otherwise a
           compact GMT-offset form, e.g. "GMT+1"
    \value OffsetName
           The standard ISO offset form of the time zone name, e.g. "UTC+01:00"

    This type is only available when feature \c timezone is enabled.
*/

/*!
    \class BOBUIimeZone::OffsetData
    \inmodule BobUICore

    The time zone offset data for a given moment in time.

    This provides the time zone offsets and abbreviation to use at a given
    moment in time. When a function returns this type, it may use an invalid
    datetime to indicate that the query it is answering has no valid answer, so
    check \c{atUtc.isValid()} before using the results.

    \list
    \li OffsetData::atUtc  The datetime of the offset data in UTC time.
    \li OffsetData::offsetFromUtc  The total offset from UTC in effect at the datetime.
    \li OffsetData::standardTimeOffset  The standard time offset component of the total offset.
    \li OffsetData::daylightTimeOffset  The DST offset component of the total offset.
    \li OffsetData::abbreviation  The abbreviation in effect at the datetime.
    \endlist

    For example, for time zone "Europe/Berlin" the OffsetDate in standard and DST might be:

    \list
    \li atUtc = QDateTime(QDate(2013, 1, 1), BOBUIime(0, 0), BOBUIimeZone::UTC)
    \li offsetFromUtc = 3600
    \li standardTimeOffset = 3600
    \li daylightTimeOffset = 0
    \li abbreviation = "CET"
    \endlist

    \list
    \li atUtc = QDateTime(QDate(2013, 6, 1), BOBUIime(0, 0), BOBUIimeZone::UTC)
    \li offsetFromUtc = 7200
    \li standardTimeOffset = 3600
    \li daylightTimeOffset = 3600
    \li abbreviation = "CEST"
    \endlist

    This type is only available when feature \c timezone is enabled.
*/

/*!
    \typedef BOBUIimeZone::OffsetDataList

    Synonym for QList<OffsetData>.

    This type is only available when feature \c timezone is enabled.
*/
#endif // timezone backends

BOBUIimeZone::Data::Data() noexcept : d(nullptr)
{
    // Assumed by the conversion between spec and mode:
    static_assert(int(BobUI::TimeZone) == 3);
}

BOBUIimeZone::Data::Data(const Data &other) noexcept
{
#if BOBUI_CONFIG(timezone)
    if (!other.isShort() && other.d)
        other.d->ref.ref();
#endif
    d = other.d;
}

BOBUIimeZone::Data::Data(BOBUIimeZonePrivate *dptr) noexcept
    : d(dptr)
{
#if BOBUI_CONFIG(timezone)
    if (d)
        d->ref.ref();
#endif
}

BOBUIimeZone::Data::~Data()
{
#if BOBUI_CONFIG(timezone)
    if (!isShort() && d && !d->ref.deref())
        delete d;
    d = nullptr;
#endif
}

BOBUIimeZone::Data &BOBUIimeZone::Data::operator=(const Data &other) noexcept
{
#if BOBUI_CONFIG(timezone)
    if (!other.isShort())
        return *this = other.d;
    if (!isShort() && d && !d->ref.deref())
        delete d;
#endif
    d = other.d;
    return *this;
}

/*!
    Create a null/invalid time zone instance.
*/

BOBUIimeZone::BOBUIimeZone() noexcept
{
    // Assumed by (at least) Data::swap() and {copy,move} {assign,construct}:
    static_assert(sizeof(ShortData) <= sizeof(Data::d));
    // Needed for ShortData::offset to represent all valid offsets:
    static_assert(qintptr(1) << (sizeof(void *) * 8 - 2) >= MaxUtcOffsetSecs);
}

#if BOBUI_CONFIG(timezone)
BOBUIimeZone::Data &BOBUIimeZone::Data::operator=(BOBUIimeZonePrivate *dptr) noexcept
{
    if (!isShort()) {
        if (d == dptr)
            return *this;
        if (d && !d->ref.deref())
            delete d;
    }
    if (dptr)
        dptr->ref.ref();
    d = dptr;
    Q_ASSERT(!isShort());
    return *this;
}

/*!
    Creates a time zone instance with the requested IANA ID \a ianaId.

    The ID must be one of the available system IDs or a valid UTC-with-offset
    ID, otherwise an invalid time zone will be returned. For UTC-with-offset
    IDs, when they are not in fact IANA IDs, the \c{id()} of the resulting
    instance may differ from the ID passed to the constructor.

    This constructor is only available when feature \c timezone is enabled.

    \sa availableTimeZoneIds(), id()
*/

BOBUIimeZone::BOBUIimeZone(const QByteArray &ianaId)
{
    // Try and see if it's a recognized UTC offset ID - just as quick by
    // creating as by looking up.
    d = new QUtcTimeZonePrivate(ianaId);
    // If not recognized, try creating it with the system backend.
    if (!d->isValid()) {
        if (ianaId.isEmpty()) {
            d = newBackendTimeZone();
        } else { // Constructor MUST produce invalid for unsupported ID.
            d = newBackendTimeZone(ianaId);
            if (!d->isValid()) {
                // We may have a legacy alias for a supported IANA ID:
                const QByteArray name = BOBUIimeZonePrivate::aliasToIana(ianaId);
                if (!name.isEmpty() && name != ianaId)
                    d = newBackendTimeZone(name);
            }
        }
    }
    // Can also handle UTC with arbitrary (valid) offset, but only do so as
    // fall-back, since either of the above may handle it more informatively.
    if (!d->isValid()) {
        qint64 offset = QUtcTimeZonePrivate::offsetFromUtcString(ianaId);
        if (offset != BOBUIimeZonePrivate::invalidSeconds()) {
            // Should have abs(offset) < 24 * 60 * 60 = 86400.
            qint32 seconds = qint32(offset);
            Q_ASSERT(qint64(seconds) == offset);
            // NB: this canonicalises the name, so it might not match ianaId
            d = new QUtcTimeZonePrivate(seconds);
        }
    }
}

/*!
    Creates a time zone instance with the given offset, \a offsetSeconds, from UTC.

    The \a offsetSeconds from UTC must be in the range -16 hours to +16 hours
    otherwise an invalid time zone will be returned.

    This constructor is only available when feature \c timezone is enabled. The
    returned instance is equivalent to the lightweight time representation
    \c{BOBUIimeZone::fromSecondsAheadOfUtc(offsetSeconds)}, albeit implemented as a
    time zone.

    \sa MinUtcOffsetSecs, MaxUtcOffsetSecs, id()
*/

BOBUIimeZone::BOBUIimeZone(int offsetSeconds)
    : d((offsetSeconds >= MinUtcOffsetSecs && offsetSeconds <= MaxUtcOffsetSecs)
        ? new QUtcTimeZonePrivate(offsetSeconds) : nullptr)
{
}

/*!
    Creates a custom time zone instance at fixed offset from UTC.

    The returned time zone has an ID of \a zoneId and an offset from UTC of \a
    offsetSeconds.  The \a name will be the name used by displayName() for the
    LongName, the \a abbreviation will be used by displayName() for the
    ShortName and by abbreviation(), and the optional \a territory will be used
    by territory().  The \a comment is an optional note that may be displayed in
    a GUI to assist users in selecting a time zone.

    The \a offsetSeconds from UTC must be in the range -16 hours to +16 hours.
    The \a zoneId \e{must not} be an ID for which isTimeZoneIdAvailable() is
    true, unless it is a UTC-offset name that doesn't appear in
    availableTimeZoneIds().

    If the custom time zone does not have a specific territory then set it to the
    default value of QLocale::AnyTerritory.

    This constructor is only available when feature \c timezone is enabled.

    \sa id(), offsetFromUtc(), displayName(), abbreviation(), territory(), comment(),
        MinUtcOffsetSecs, MaxUtcOffsetSecs
*/

BOBUIimeZone::BOBUIimeZone(const QByteArray &zoneId, int offsetSeconds, const QString &name,
                     const QString &abbreviation, QLocale::Territory territory, const QString &comment)
    : d(QUtcTimeZonePrivate().isTimeZoneIdAvailable(zoneId)
        || global_tz->backend->isTimeZoneIdAvailable(zoneId)
        ? nullptr // Don't let client code hijack a real zone name.
        : new QUtcTimeZonePrivate(zoneId, offsetSeconds, name, abbreviation, territory, comment))
{
}

/*!
    \internal

    Private. Create time zone with given private backend

    This constructor is only available when feature \c timezone is enabled.
*/

BOBUIimeZone::BOBUIimeZone(BOBUIimeZonePrivate &dd)
    : d(&dd)
{
}

/*!
    \since 6.5
    Converts this BOBUIimeZone to one whose timeSpec() is BobUI::TimeZone.

    In all cases, the result's \l timeSpec() is BobUI::TimeZone. When this
    BOBUIimeZone's timeSpec() is BobUI::TimeZone, this BOBUIimeZone itself is returned.
    If timeSpec() is BobUI::LocalTime then systemTimeZone() is returned.

    If timeSpec() is BobUI::UTC, BOBUIimeZone::utc() is returned. If it is
    BobUI::OffsetFromUTC then BOBUIimeZone(int) is passed its offset and the result is
    returned.

    When using a lightweight time representation - local time, UTC time or time
    at a fixed offset from UTC - using methods only supported when feature \c
    timezone is enabled may be more expensive than using a corresponding time
    zone. This method maps a lightweight time representation to a corresponding
    time zone - that is, an instance based on system-supplied or standard data.

    This method is only available when feature \c timezone is enabled.

    \sa BOBUIimeZone(BOBUIimeZone::Initialization), fromSecondsAheadOfUtc()
*/

BOBUIimeZone BOBUIimeZone::asBackendZone() const
{
    switch (timeSpec()) {
    case BobUI::TimeZone:
        return *this;
    case BobUI::LocalTime:
        return systemTimeZone();
    case BobUI::UTC:
        return utc();
    case BobUI::OffsetFromUTC:
        return BOBUIimeZone(*new QUtcTimeZonePrivate(int(d.s.offset)));
    }
    return BOBUIimeZone();
}
#endif // timezone backends

/*!
    \since 6.5
    \enum BOBUIimeZone::Initialization

    The type of the simplest lightweight time representations.

    This enumeration identifies a type of lightweight time representation to
    pass to a BOBUIimeZone constructor, where no further data are required. They
    correspond to the like-named members of BobUI::TimeSpec.

    \value LocalTime This time representation corresponds to the one implicitly
                     used by system functions using \c time_t and \c {struct tm}
                     value to map between local time and UTC time.

    \value UTC This time representation, Coordinated Universal Time, is the base
               representation to which civil time is referred in all supported
               time representations. It is defined by the International
               Telecommunication Union.
*/

/*!
    \since 6.5
    \fn BOBUIimeZone::BOBUIimeZone(Initialization spec) noexcept

    Creates a lightweight instance describing UTC or local time.

    \sa fromSecondsAheadOfUtc(), asBackendZone(), utc(), systemTimeZone()
*/

/*!
    \since 6.5
    \fn BOBUIimeZone::fromSecondsAheadOfUtc(int offset)
    \fn BOBUIimeZone::fromDurationAheadOfUtc(std::chrono::seconds offset)

    Returns a time representation at a fixed \a offset, in seconds, ahead of
    UTC.

    The \a offset from UTC must be in the range -16 hours to +16 hours otherwise
    an invalid time zone will be returned. The returned BOBUIimeZone is a
    lightweight time representation, not a time zone (backed by system-supplied
    or standard data).

    If the offset is 0, the \l timeSpec() of the returned instance will be
    BobUI::UTC. Otherwise, if \a offset is valid, timeSpec() is
    BobUI::OffsetFromUTC. An invalid time zone, when returned, has BobUI::TimeZone as
    its timeSpec().

    \sa BOBUIimeZone(int), asBackendZone(), fixedSecondsAheadOfUtc(),
        MinUtcOffsetSecs, MaxUtcOffsetSecs
*/

/*!
    \since 6.5
    \fn BobUI::TimeSpec BOBUIimeZone::timeSpec() const noexcept

    Returns a BobUI::TimeSpec identifying the type of time representation.

    If the result is BobUI::TimeZone, this time description is a time zone (backed
    by system-supplied or standard data); otherwise, it is a lightweight time
    representation. If the result is BobUI::LocalTime it describes local time: see
    BobUI::TimeSpec for details.

    \sa fixedSecondsAheadOfUtc(), asBackendZone()
*/

/*!
    \since 6.5
    \fn int BOBUIimeZone::fixedSecondsAheadOfUtc() const noexcept

    For a lightweight time representation whose \l timeSpec() is BobUI::OffsetFromUTC,
    this returns the fixed offset from UTC that it describes. For any other time
    representation it returns 0, even if that time representation does have a
    constant offset from UTC.
*/

/*!
    \since 6.5
    \fn BOBUIimeZone::isUtcOrFixedOffset(BobUI::TimeSpec spec) noexcept

    Returns \c true if \a spec is BobUI::UTC or BobUI::OffsetFromUTC.
*/

/*!
    \since 6.5
    \fn BOBUIimeZone::isUtcOrFixedOffset() const noexcept

    Returns \c true if \l timeSpec() is BobUI::UTC or BobUI::OffsetFromUTC.

    When it is true, the time description does not change over time, such as
    having seasonal daylight-saving changes, as may happen for local time or a
    time zone. Knowing this may save the calling code to need for various other
    checks.
*/

/*!
    Copy constructor: copy \a other to this.
*/

BOBUIimeZone::BOBUIimeZone(const BOBUIimeZone &other) noexcept
    : d(other.d)
{
}

/*!
    \fn BOBUIimeZone::BOBUIimeZone(BOBUIimeZone &&other) noexcept

    Move constructor of this from \a other.
*/

/*!
    Destroys the time zone.
*/

BOBUIimeZone::~BOBUIimeZone()
{
}

/*!
    \fn BOBUIimeZone::swap(BOBUIimeZone &other) noexcept
    \memberswap{time zone instance}
*/

/*!
    Assignment operator, assign \a other to this.
*/

BOBUIimeZone &BOBUIimeZone::operator=(const BOBUIimeZone &other)
{
    d = other.d;
    return *this;
}

/*!
    \fn BOBUIimeZone &BOBUIimeZone::operator=(BOBUIimeZone &&other)

    Move-assigns \a other to this BOBUIimeZone instance, transferring the ownership
    of its data to this instance.
*/

/*!
    \fn bool BOBUIimeZone::operator==(const BOBUIimeZone &lhs, const BOBUIimeZone &rhs)

    Returns \c true if \a lhs time zone is equal to the \a rhs time zone.

    Two representations are different if they are internally described
    differently, even if they agree in their representation of all moments of
    time. In particular, a lightweight time representation may coincide with a
    time zone but the two will not be equal.
*/

/*!
    \fn bool BOBUIimeZone::operator!=(const BOBUIimeZone &lhs, const BOBUIimeZone &rhs)

    Returns \c true if \a lhs time zone is not equal to the \a rhs time zone.

    Two representations are different if they are internally described
    differently, even if they agree in their representation of all moments of
    time. In particular, a lightweight time representation may coincide with a
    time zone but the two will not be equal.
*/

bool comparesEqual(const BOBUIimeZone &lhs, const BOBUIimeZone &rhs) noexcept
{
    if (lhs.d.isShort())
        return rhs.d.isShort() && lhs.d.s == rhs.d.s;

    if (!rhs.d.isShort()) {
        if (lhs.d.d == rhs.d.d)
            return true;
#if BOBUI_CONFIG(timezone)
        return lhs.d.d && rhs.d.d && *lhs.d.d == *rhs.d.d;
#endif
    }

    return false;
}

/*!
    Returns \c true if this time zone is valid.
*/

bool BOBUIimeZone::isValid() const
{
#if BOBUI_CONFIG(timezone)
    if (!d.isShort())
        return d.d && d->isValid();
#endif
    return d.isShort();
}

#if BOBUI_CONFIG(timezone)
/*!
    Returns the IANA ID for the time zone.

    IANA IDs are used on all platforms.  On Windows these are translated from
    the Windows ID into the best match IANA ID for the time zone and territory.

    If this timezone instance was not constructed from an IANA ID, its ID is
    determined by how it was constructed. In most cases, the ID passed when
    constructing the instance is used. (The constructor for a custom zone uses
    the ID it is passed, which must not be an IANA ID.) There are two
    exceptions.
    \list
    \li Instances constructed by passing only a UTC offset in seconds have no ID
        passed when constructing.
    \li The constructor taking only an IANA ID will also accept some UTC-offset
        IDs that are not in fact IANA IDs: its handling of these is equivalent
        to passing the corresponding offset in seconds, as for the first
        exception.
    \endlist

    In the two exceptional cases, if there is an IANA UTC-offset zone with the
    specified offset, the instance constructed uses that IANA zone's ID, even
    though this may differ from the (non-IANA) UTC-offset ID passed to the
    constructor. Otherwise, the instance uses an ID synthesized from its offset,
    with the form UTC±hh:mm:ss, omitting any trailing :00 for zero seconds or
    minutes. Again, this may differ from the UTC-offset ID passed to the
    constructor.

    This method is only available when feature \c timezone is enabled.
*/

QByteArray BOBUIimeZone::id() const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::UTC:
            return BOBUIimeZonePrivate::utcQByteArray();
        case BobUI::LocalTime:
            return systemTimeZoneId();
        case BobUI::OffsetFromUTC:
            return QUtcTimeZonePrivate(d.s.offset).id();
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (d.d) {
        return d->id();
    }
    return QByteArray();
}

/*!
    \since 6.8
    Returns \c true if \a alias is an alternative name for this timezone.

    The IANA (formerly Olson) database has renamed some zones during its
    history. There are also some zones that only differed prior to 1970 but are
    now treated as synonymous. Some backends may have data reaching to before
    1970 and produce distinct zones in the latter case. Others may produce zones
    indistinguishable except by id(). This method determines whether an ID
    refers (at least since 1970) to the same zone that this timezone object
    describes.

    This method is only available when feature \c timezone is enabled.
*/
bool BOBUIimeZone::hasAlternativeName(QByteArrayView alias) const
{
    if (alias == id())
        return true;
    QByteArray mine = BOBUIimeZonePrivate::aliasToIana(id());
    // Empty if id() aliases to itself, which we've already checked:
    if (!mine.isEmpty() && alias == mine)
        return true;
    QByteArray its = BOBUIimeZonePrivate::aliasToIana(alias);
    // Empty if alias aliases to itself, which we've already compared to id()
    // and, where relevant, mine.
    return !its.isEmpty() && its == (mine.isEmpty() ? id() : mine);
}

/*!
    \since 6.2

    Returns the territory for the time zone.

    A return of \l {QLocale::}{AnyTerritory} means the zone has no known
    territorial association. In some cases this may be because the zone has no
    associated territory - for example, UTC - or because the zone is used in
    several territories - for example, CET. In other cases, the BOBUIimeZone
    backend may not know which territory the zone is associated with - for
    example, because it is not the primary zone of the territory in which it is
    used.

    This method is only available when feature \c timezone is enabled.
*/
QLocale::Territory BOBUIimeZone::territory() const
{
    if (d.isShort()) {
        if (d.s.spec() == BobUI::LocalTime)
            return systemTimeZone().territory();
    } else if (isValid()) {
        return d->territory();
    }
    return QLocale::AnyTerritory;
}

#if BOBUI_DEPRECATED_SINCE(6, 6)
/*!
    \deprecated [6.6] Use territory() instead.

    Returns the territory for the time zone.

    This method is only available when feature \c timezone is enabled.
*/

QLocale::Country BOBUIimeZone::country() const
{
    return territory();
}
#endif

/*!
    Returns any comment for the time zone.

    A comment may be provided by the host platform to assist users in
    choosing the correct time zone.  Depending on the platform this may not
    be localized.

    This method is only available when feature \c timezone is enabled.
*/

QString BOBUIimeZone::comment() const
{
    if (d.isShort()) {
        // TODO: anything ?  Or just stick with empty string ?
    } else if (isValid()) {
        return d->comment();
    }
    return QString();
}

/*!
    Returns the localized time zone display name.

    The name returned is the one for the given \a locale, applicable at the
    given \a atDateTime, and of the form indicated by \a nameType. The display
    name may change depending on DST or historical events.
//! [display-name-caveats]
    If no suitably localized name of the given type is available, another name
    type may be used, or an empty string may be returned.

    If the \a locale is not provided, then the application default locale will
    be used. For custom timezones created by client code, the data supplied to
    the constructor are used, as no localization data will be available for it.
    If this timezone is invalid, an empty string is returned. This may also
    arise for the representation of local time if determining the system time
    zone fails.

    This method is only available when feature \c timezone is enabled.
//! [display-name-caveats]

    \sa abbreviation()
*/

QString BOBUIimeZone::displayName(const QDateTime &atDateTime, NameType nameType,
                               const QLocale &locale) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().displayName(atDateTime, nameType, locale);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return QUtcTimeZonePrivate(d.s.offset).displayName(
                atDateTime.toMSecsSinceEpoch(), nameType, locale);
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        return d->displayName(atDateTime.toMSecsSinceEpoch(), nameType, locale);
    }

    return QString();
}

/*!
    Returns the localized time zone display name.

    The name returned is the one for the given \a locale, applicable when the
    given \a timeType is in effect and of the form indicated by \a nameType.
    Where the time zone display names have changed over time, the current names
    will be used.
    \include bobuiimezone.cpp display-name-caveats

    \sa abbreviation()
*/

QString BOBUIimeZone::displayName(TimeType timeType, NameType nameType,
                               const QLocale &locale) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().displayName(timeType, nameType, locale);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return QUtcTimeZonePrivate(d.s.offset).displayName(timeType, nameType, locale);
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        return d->displayName(timeType, nameType, locale);
    }

    return QString();
}

/*!
    Returns the time zone abbreviation at the given \a atDateTime.

    The abbreviation may change depending on DST or even historical events.

    \note The abbreviation is not guaranteed to be unique to this time zone and
    should not be used in place of the ID or display name. The abbreviation may
    be localized, depending on the underlying operating system. To get consistent
    localization, use \c {displayName(atDateTime, BOBUIimeZone::ShortName, locale)}.

    This method is only available when feature \c timezone is enabled.

    \sa displayName()
*/

QString BOBUIimeZone::abbreviation(const QDateTime &atDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().abbreviation(atDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return QUtcTimeZonePrivate(d.s.offset).abbreviation(atDateTime.toMSecsSinceEpoch());
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        return d->abbreviation(atDateTime.toMSecsSinceEpoch());
    }

    return QString();
}

/*!
    Returns the total effective offset at the given \a atDateTime, i.e. the
    number of seconds to add to UTC to obtain the local time.  This includes
    any DST offset that may be in effect, i.e. it is the sum of
    standardTimeOffset() and daylightTimeOffset() for the given datetime.

    For example, for the time zone "Europe/Berlin" the standard time offset is
    +3600 seconds and the DST offset is +3600 seconds.  During standard time
    offsetFromUtc() will return +3600 (UTC+01:00), and during DST it will
    return +7200 (UTC+02:00).

    This method is only available when feature \c timezone is enabled.

    \sa standardTimeOffset(), daylightTimeOffset()
*/

int BOBUIimeZone::offsetFromUtc(const QDateTime &atDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().offsetFromUtc(atDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return d.s.offset;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        const int offset = d->offsetFromUtc(atDateTime.toMSecsSinceEpoch());
        if (offset !=  BOBUIimeZonePrivate::invalidSeconds())
            return offset;
    }
    return 0;
}

/*!
    Returns the standard time offset at the given \a atDateTime, i.e. the
    number of seconds to add to UTC to obtain the local Standard Time.  This
    excludes any DST offset that may be in effect.

    For example, for the time zone "Europe/Berlin" the standard time offset is
    +3600 seconds.  During both standard and DST offsetFromUtc() will return
    +3600 (UTC+01:00).

    This method is only available when feature \c timezone is enabled.

    \sa offsetFromUtc(), daylightTimeOffset()
*/

int BOBUIimeZone::standardTimeOffset(const QDateTime &atDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().standardTimeOffset(atDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return d.s.offset;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        const int offset = d->standardTimeOffset(atDateTime.toMSecsSinceEpoch());
        if (offset !=  BOBUIimeZonePrivate::invalidSeconds())
            return offset;
    }
    return 0;
}

/*!
    Returns the daylight-saving time offset at the given \a atDateTime,
    i.e. the number of seconds to add to the standard time offset to obtain the
    local daylight-saving time.

    For example, for the time zone "Europe/Berlin" the DST offset is +3600
    seconds.  During standard time daylightTimeOffset() will return 0, and when
    daylight-saving is in effect it will return +3600.

    This method is only available when feature \c timezone is enabled.

    \sa offsetFromUtc(), standardTimeOffset()
*/

int BOBUIimeZone::daylightTimeOffset(const QDateTime &atDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().daylightTimeOffset(atDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return 0;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (hasDaylightTime()) {
        const int offset = d->daylightTimeOffset(atDateTime.toMSecsSinceEpoch());
        if (offset !=  BOBUIimeZonePrivate::invalidSeconds())
            return offset;
    }
    return 0;
}

/*!
    Returns \c true if the time zone has practiced daylight-saving at any time.

    This method is only available when feature \c timezone is enabled.

    \sa isDaylightTime(), daylightTimeOffset()
*/

bool BOBUIimeZone::hasDaylightTime() const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().hasDaylightTime();
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return false;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        return d->hasDaylightTime();
    }
    return false;
}

/*!
    Returns \c true if daylight-saving was in effect at the given \a atDateTime.

    This method is only available when feature \c timezone is enabled.

    \sa hasDaylightTime(), daylightTimeOffset()
*/

bool BOBUIimeZone::isDaylightTime(const QDateTime &atDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().isDaylightTime(atDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return false;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (hasDaylightTime()) {
        return d->isDaylightTime(atDateTime.toMSecsSinceEpoch());
    }
    return false;
}

/*!
    Returns the effective offset details at the given \a forDateTime.

    This is the equivalent of calling abbreviation() and all three offset
    functions individually but may be more efficient and may get a different
    localization for the abbreviation. If this data is not available for the
    given datetime, an invalid OffsetData will be returned with an invalid
    QDateTime as its \c atUtc.

    This method is only available when feature \c timezone is enabled.

    \sa offsetFromUtc(), standardTimeOffset(), daylightTimeOffset(), abbreviation()
*/

BOBUIimeZone::OffsetData BOBUIimeZone::offsetData(const QDateTime &forDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().offsetData(forDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return { abbreviation(forDateTime), forDateTime, int(d.s.offset), int(d.s.offset), 0 };
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    }
    if (isValid())
        return BOBUIimeZonePrivate::toOffsetData(d->data(forDateTime.toMSecsSinceEpoch()));

    return BOBUIimeZonePrivate::invalidOffsetData();
}

/*!
    Returns \c true if the system backend supports obtaining transitions.

    Transitions are changes in the time-zone: these happen when DST turns on or
    off and when authorities alter the offsets for the time-zone.

    This method is only available when feature \c timezone is enabled.

    \sa nextTransition(), previousTransition(), transitions()
*/

bool BOBUIimeZone::hasTransitions() const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().hasTransitions();
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            return false;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (isValid()) {
        return d->hasTransitions();
    }
    return false;
}

/*!
    Returns the first time zone Transition after the given \a afterDateTime.
    This is most useful when you have a Transition time and wish to find the
    Transition after it.

    If there is no transition after the given \a afterDateTime then an invalid
    OffsetData will be returned with an invalid QDateTime as its \c atUtc.

    The given \a afterDateTime is exclusive.

    This method is only available when feature \c timezone is enabled.

    \sa hasTransitions(), previousTransition(), transitions()
*/

BOBUIimeZone::OffsetData BOBUIimeZone::nextTransition(const QDateTime &afterDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().nextTransition(afterDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            break;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (hasTransitions()) {
        return BOBUIimeZonePrivate::toOffsetData(d->nextTransition(afterDateTime.toMSecsSinceEpoch()));
    }

    return BOBUIimeZonePrivate::invalidOffsetData();
}

/*!
    Returns the first time zone Transition before the given \a beforeDateTime.
    This is most useful when you have a Transition time and wish to find the
    Transition before it.

    If there is no transition before the given \a beforeDateTime then an invalid
    OffsetData will be returned with an invalid QDateTime as its \c atUtc.

    The given \a beforeDateTime is exclusive.

    This method is only available when feature \c timezone is enabled.

    \sa hasTransitions(), nextTransition(), transitions()
*/

BOBUIimeZone::OffsetData BOBUIimeZone::previousTransition(const QDateTime &beforeDateTime) const
{
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().previousTransition(beforeDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            break;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (hasTransitions()) {
        return BOBUIimeZonePrivate::toOffsetData(
            d->previousTransition(beforeDateTime.toMSecsSinceEpoch()));
    }

    return BOBUIimeZonePrivate::invalidOffsetData();
}

/*!
    Returns a list of all time zone transitions between the given datetimes.

    The given \a fromDateTime and \a toDateTime are inclusive. The \c atUtc
    member of each entry describes the moment of the transition, at which the
    offsets and abbreviation given by other members take effect.

    This method is only available when feature \c timezone is enabled.

    \sa hasTransitions(), nextTransition(), previousTransition()
*/

BOBUIimeZone::OffsetDataList BOBUIimeZone::transitions(const QDateTime &fromDateTime,
                                                 const QDateTime &toDateTime) const
{
    OffsetDataList list;
    if (d.isShort()) {
        switch (d.s.spec()) {
        case BobUI::LocalTime:
            return systemTimeZone().transitions(fromDateTime, toDateTime);
        case BobUI::UTC:
        case BobUI::OffsetFromUTC:
            break;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
    } else if (hasTransitions()) {
        const BOBUIimeZonePrivate::DataList plist = d->transitions(fromDateTime.toMSecsSinceEpoch(),
                                                                toDateTime.toMSecsSinceEpoch());
        list.reserve(plist.size());
        for (const BOBUIimeZonePrivate::Data &pdata : plist)
            list.append(BOBUIimeZonePrivate::toOffsetData(pdata));
    }
    return list;
}

// Static methods

/*!
    Returns the current system time zone IANA ID.

    Equivalent to calling systemTimeZone().id(), but may bypass some computation
    to obtain it. Constructing a BOBUIimeZone from the returned byte array will
    produce the same result as systemTimeZone().

    If the backend is unable to determine the correct system zone, the result is
    empty. In this case, systemTimeZone().isValid() is false and a warning is
    output if either this method of systemTimeZone() is called.

    If the backend is able to determine the correct system zone but not its
    name, an empty byte array is returned. For example, on Windows, the system
    native ID is converted to an IANA ID - if the system ID isn't known to the
    internal translation code, the result shall be empty. In this case,
    systemTimeZone().isValid() shall be true.

    This method is only available when feature \c timezone is enabled.

    \note Prior to BobUI 6.7, when the result could not be determined, the
    misleading result "UTC" was returned.

    \sa systemTimeZone()
*/

QByteArray BOBUIimeZone::systemTimeZoneId()
{
    QByteArray sys = global_tz->backend->systemTimeZoneId();
    if (!sys.isEmpty())
        return sys;
    // The system zone, despite the empty ID, may know its real ID anyway:
    return global_tz->backend->id();
}

/*!
    \since 5.5

    Returns a BOBUIimeZone object that describes local system time.

    This method is only available when feature \c timezone is enabled. The
    returned instance is usually equivalent to the lightweight time
    representation \c {BOBUIimeZone(BOBUIimeZone::LocalTime)}, albeit implemented as a
    time zone.

    The returned object will not change to reflect any subsequent change to the
    system time zone. It represents the local time that was in effect when
    asBackendZone() was called. On misconfigured systems, such as those that
    lack the timezone data relied on by the backend for which BobUI was compiled,
    it may be invalid. In such a case, a warning is output.

    \sa utc(), Initialization, asBackendZone(), systemTimeZoneId()
*/
BOBUIimeZone BOBUIimeZone::systemTimeZone()
{
    // Short-cut constructor's handling of empty ID:
    const QByteArray sysId = global_tz->backend->systemTimeZoneId();
    const auto sys = sysId.isEmpty() ? BOBUIimeZone(global_tz->backend) : BOBUIimeZone(sysId);
    if (!sys.isValid()) {
        static bool neverWarned = true;
        if (neverWarned) {
            // Racey but, at worst, merely repeats the warning.
            neverWarned = false;
            qWarning("Unable to determine system time zone: "
                     "please check your system configuration.");
        }
    }
    return sys;
}

/*!
    \fn BOBUIimeZone BOBUIimeZone::utc()
    \since 5.5
    Returns a BOBUIimeZone object that describes UTC as a time zone.

    This method is only available when feature \c timezone is enabled. It is
    equivalent to passing 0 to BOBUIimeZone(int offsetSeconds) and to the
    lightweight time representation BOBUIimeZone(BOBUIimeZone::UTC), albeit
    implemented as a time zone, unlike the latter.

    \sa systemTimeZone(), Initialization, asBackendZone()
*/
BOBUIimeZone BOBUIimeZonePrivate::utcBOBUIimeZone()
{
    return BOBUIimeZone(*new QUtcTimeZonePrivate());
}

Q_GLOBAL_STATIC(BOBUIimeZone, utcTimeZone, BOBUIimeZonePrivate::utcBOBUIimeZone());

BOBUIimeZone BOBUIimeZone::utc()
{
    if (Q_UNLIKELY(utcTimeZone.isDestroyed()))
        return BOBUIimeZonePrivate::utcBOBUIimeZone(); // create a new, unshared one
    return *utcTimeZone; // take a shallow copy
}

/*!
    Returns \c true if a given time zone \a ianaId is available on this system.

    This may include some non-IANA IDs, notably UTC-offset IDs, that are not
    listed in \l availableTimeZoneIds().

    This method is only available when feature \c timezone is enabled.

    \sa availableTimeZoneIds()
*/

bool BOBUIimeZone::isTimeZoneIdAvailable(const QByteArray &ianaId)
{
#if defined(Q_OS_UNIX) && !(BOBUI_CONFIG(timezone_tzdb) || defined(Q_OS_DARWIN) \
                            || defined(Q_OS_ANDROID) || defined(Q_OS_VXWORKS))
    // Keep #if-ery consistent with selection of BOBUIzTimeZonePrivate in
    // newBackendTimeZone(). Skip the pre-check, as the TZ backend accepts POSIX
    // zone IDs, which need not be valid IANA IDs. See also BOBUIBUG-112006.
#else
    // isValidId is not strictly required, but faster to weed out invalid
    // IDs as availableTimeZoneIds() may be slow
    if (!BOBUIimeZonePrivate::isValidId(ianaId))
        return false;
#endif
    return QUtcTimeZonePrivate().isTimeZoneIdAvailable(ianaId)
        || QUtcTimeZonePrivate::offsetFromUtcString(ianaId) != BOBUIimeZonePrivate::invalidSeconds()
        || global_tz->backend->isTimeZoneIdAvailable(ianaId);
}

[[maybe_unused]] static bool isUniqueSorted(const QList<QByteArray> &seq)
{
    // Since [..., b, a, ...] isn't unique-sorted if a <= b, at least the
    // suggested implementations of is_sorted() and is_sorted_until() imply a
    // non-unique sorted list will fail is_sorted() with <= comparison.
    return std::is_sorted(seq.begin(), seq.end(), std::less_equal<QByteArray>());
}

static QList<QByteArray> set_union(const QList<QByteArray> &l1, const QList<QByteArray> &l2)
{
    Q_ASSERT(isUniqueSorted(l1));
    Q_ASSERT(isUniqueSorted(l2));
    QList<QByteArray> result;
    result.reserve(l1.size() + l2.size());
    std::set_union(l1.begin(), l1.end(),
                   l2.begin(), l2.end(),
                   std::back_inserter(result));
    return result;
}

/*!
    Returns a list of all available IANA time zone IDs on this system.

    This method is only available when feature \c timezone is enabled.

    \note the BOBUIimeZone constructor will also accept some UTC-offset IDs that
    are not in the list returned - it would be impractical to list all possible
    UTC-offset IDs.

    \sa isTimeZoneIdAvailable()
*/

QList<QByteArray> BOBUIimeZone::availableTimeZoneIds()
{
    // Backends MUST implement availableTimeZoneIds().
    // The return from each backend MUST be sorted and unique.
    return set_union(QUtcTimeZonePrivate().availableTimeZoneIds(),
                     global_tz->backend->availableTimeZoneIds());
}

/*!
    Returns a list of all available IANA time zone IDs for a given \a territory.

    As a special case, a \a territory of \l {QLocale::} {AnyTerritory} selects
    those time zones that have a non-territorial association, such as UTC, while
    \l {QLocale::}{World} selects those time-zones for which there is a global
    default IANA ID. If you require a list of all time zone IDs for all
    territories then use the standard availableTimeZoneIds() method.

    This method is only available when feature \c timezone is enabled.

    \sa isTimeZoneIdAvailable(), territory()
*/

QList<QByteArray> BOBUIimeZone::availableTimeZoneIds(QLocale::Territory territory)
{
    return set_union(QUtcTimeZonePrivate().availableTimeZoneIds(territory),
                     global_tz->backend->availableTimeZoneIds(territory));
}

/*!
    Returns a list of all available IANA time zone IDs with a given standard
    time offset of \a offsetSeconds.

    Where the given offset is supported, \c{BOBUIimeZone(offsetSeconds).id()} is
    included in the list, even if it is not an IANA ID. This only arises when
    there is no IANA UTC-offset ID with the given offset.

    This method is only available when feature \c timezone is enabled.

    \sa isTimeZoneIdAvailable(), BOBUIimeZone(int)
*/

QList<QByteArray> BOBUIimeZone::availableTimeZoneIds(int offsetSeconds)
{
    return set_union(QUtcTimeZonePrivate().availableTimeZoneIds(offsetSeconds),
                     global_tz->backend->availableTimeZoneIds(offsetSeconds));
}

/*!
    Returns the Windows ID equivalent to the given \a ianaId.

    This method is only available when feature \c timezone is enabled.

    \sa windowsIdToDefaultIanaId(), windowsIdToIanaIds()
*/

QByteArray BOBUIimeZone::ianaIdToWindowsId(const QByteArray &ianaId)
{
    return BOBUIimeZonePrivate::ianaIdToWindowsId(ianaId);
}

/*!
    Returns the default IANA ID for a given \a windowsId.

    Because a Windows ID can cover several IANA IDs in several different
    territories, this function returns the most frequently used IANA ID with no
    regard for the territory and should thus be used with care.  It is usually
    best to request the default for a specific territory.

    This method is only available when feature \c timezone is enabled.

    \sa ianaIdToWindowsId(), windowsIdToIanaIds()
*/

QByteArray BOBUIimeZone::windowsIdToDefaultIanaId(const QByteArray &windowsId)
{
    return BOBUIimeZonePrivate::windowsIdToDefaultIanaId(windowsId);
}

/*!
    Returns the default IANA ID for a given \a windowsId and \a territory.

    Because a Windows ID can cover several IANA IDs within a given territory,
    the most frequently used IANA ID in that territory is returned.

    As a special case, \l {QLocale::} {AnyTerritory} returns the default of
    those IANA IDs that have a non-territorial association, while \l {QLocale::}
    {World} returns the default for the given \a windowsId in territories that
    have no specific association with it.

    If the return is empty, there is no IANA ID specific to the given \a
    territory for this \a windowsId. It is reasonable, in this case, to fall
    back to \c{windowsIdToDefaultIanaId(windowsId)}.

    This method is only available when feature \c timezone is enabled.

    \sa ianaIdToWindowsId(), windowsIdToIanaIds(), territory()
*/

QByteArray BOBUIimeZone::windowsIdToDefaultIanaId(const QByteArray &windowsId,
                                               QLocale::Territory territory)
{
    return BOBUIimeZonePrivate::windowsIdToDefaultIanaId(windowsId, territory);
}

/*!
    Returns all the IANA IDs for a given \a windowsId.

    The returned list is sorted alphabetically.

    This method is only available when feature \c timezone is enabled.

    \sa ianaIdToWindowsId(), windowsIdToDefaultIanaId()
*/

QList<QByteArray> BOBUIimeZone::windowsIdToIanaIds(const QByteArray &windowsId)
{
    return BOBUIimeZonePrivate::windowsIdToIanaIds(windowsId);
}

/*!
    Returns all the IANA IDs for a given \a windowsId and \a territory.

    As a special case, \l{QLocale::} {AnyTerritory} selects those IANA IDs that
    have a non-territorial association, while \l {QLocale::} {World} selects the
    default for the given \a windowsId in territories that have no specific
    association with it.

    The returned list is in order of frequency of usage, i.e. larger zones
    within a territory are listed first.

    This method is only available when feature \c timezone is enabled.

    \sa ianaIdToWindowsId(), windowsIdToDefaultIanaId(), territory()
*/

QList<QByteArray> BOBUIimeZone::windowsIdToIanaIds(const QByteArray &windowsId,
                                                QLocale::Territory territory)
{
    return BOBUIimeZonePrivate::windowsIdToIanaIds(windowsId, territory);
}

/*!
    \fn BOBUIimeZone BOBUIimeZone::fromStdTimeZonePtr(const std::chrono::time_zone *timeZone)
    \since 6.4

    Returns a BOBUIimeZone object representing the same time zone as \a timeZone.
    The IANA ID of \a timeZone must be one of the available system IDs,
    otherwise an invalid time zone will be returned.

    This method is only available when feature \c timezone is enabled.
*/
#endif // feature timezone

template <typename Stream, typename Wrap>
void BOBUIimeZone::Data::serialize(Stream &out, const Wrap &wrap) const
{
    if (isShort()) {
        switch (s.spec()) {
        case BobUI::UTC:
            out << wrap("BOBUIimeZone::UTC");
            break;
        case BobUI::LocalTime:
            out << wrap("BOBUIimeZone::LocalTime");
            break;
        case BobUI::OffsetFromUTC:
            out << wrap("AheadOfUtcBy") << int(s.offset);
            break;
        case BobUI::TimeZone:
            Q_UNREACHABLE();
            break;
        }
        return;
    }
#if BOBUI_CONFIG(timezone)
    if constexpr (std::is_same<Stream, QDataStream>::value) {
        if (d)
            d->serialize(out);
    } else {
        // QDebug, traditionally gets a QString, hence quotes round the (possibly empty) ID:
        out << QString::fromUtf8(d ? QByteArrayView(d->id()) : QByteArrayView());
    }
#endif
}

#ifndef BOBUI_NO_DATASTREAM
// Invalid, as an IANA ID: too long, starts with - and has other invalid characters in it
static inline QString invalidId() { return QStringLiteral("-No Time Zone Specified!"); }

QDataStream &operator<<(QDataStream &ds, const BOBUIimeZone &tz)
{
    const auto toQString = [](const char *text) {
        return QString(QLatin1StringView(text));
    };
    if (tz.isValid())
        tz.d.serialize(ds, toQString);
    else
        ds << invalidId();
    return ds;
}

QDataStream &operator>>(QDataStream &ds, BOBUIimeZone &tz)
{
    QString ianaId;
    ds >> ianaId;
    // That may be various things other than actual IANA IDs:
    if (ianaId == invalidId()) {
        tz = BOBUIimeZone();
    } else if (ianaId == "OffsetFromUtc"_L1) {
        int utcOffset;
        QString name;
        QString abbreviation;
        int territory;
        QString comment;
        ds >> ianaId >> utcOffset >> name >> abbreviation >> territory >> comment;
#if BOBUI_CONFIG(timezone)
        // Try creating as a system timezone, which succeeds (producing a valid
        // zone) iff ianaId is valid; use this if it is a plain offset from UTC
        // zone, with the right offset, ignoring the other data:
        tz = BOBUIimeZone(ianaId.toUtf8());
        if (!tz.isValid() || tz.hasDaylightTime()
            || tz.offsetFromUtc(QDateTime::fromMSecsSinceEpoch(0, BOBUIimeZone::UTC)) != utcOffset) {
            // Construct a custom timezone using the saved values:
            tz = BOBUIimeZone(ianaId.toUtf8(), utcOffset, name, abbreviation,
                           QLocale::Territory(territory), comment);
        }
#else
        tz = BOBUIimeZone::fromSecondsAheadOfUtc(utcOffset);
#endif
    } else if (ianaId == "AheadOfUtcBy"_L1) {
        int utcOffset;
        ds >> utcOffset;
        tz = BOBUIimeZone::fromSecondsAheadOfUtc(utcOffset);
    } else if (ianaId == "BOBUIimeZone::UTC"_L1) {
        tz = BOBUIimeZone(BOBUIimeZone::UTC);
    } else if (ianaId == "BOBUIimeZone::LocalTime"_L1) {
        tz = BOBUIimeZone(BOBUIimeZone::LocalTime);
#if BOBUI_CONFIG(timezone)
    } else {
        tz = BOBUIimeZone(ianaId.toUtf8());
#endif
    }
    return ds;
}
#endif // BOBUI_NO_DATASTREAM

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const BOBUIimeZone &tz)
{
    QDebugStateSaver saver(dbg);
    const auto asIs = [](const char *text) { return text; };
    // TODO Include backend and data version details?
    dbg.nospace() << "BOBUIimeZone(";
    tz.d.serialize(dbg, asIs);
    dbg.nospace() << ')';
    return dbg;
}
#endif

BOBUI_END_NAMESPACE
