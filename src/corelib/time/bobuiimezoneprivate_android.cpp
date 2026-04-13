// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2014 Drew Parsons <dparsons@emerall.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiimezone.h"
#include "bobuiimezoneprivate_p.h"

#include <BobUICore/QJniEnvironment>
#include <BobUICore/QSet>
#include <BobUICore/qjnitypes.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_JNI_CLASS(TimeZone, "java/util/TimeZone");
Q_DECLARE_JNI_CLASS(Locale, "java/util/Locale");
Q_DECLARE_JNI_CLASS(Date, "java/util/Date");

/*
    Private

    Android implementation

    Note that a QJniObject manages a global reference, so it serves as an
    owning smart-pointer, ensuring an object doesn't get garbage-collected
    before we're done with it.
*/

// Create the system default time zone
QAndroidTimeZonePrivate::QAndroidTimeZonePrivate()
    : BOBUIimeZonePrivate()
{
    // Keep in sync with systemTimeZoneId():
    androidTimeZone = QJniObject::callStaticMethod<BobUIJniTypes::TimeZone>(
                          BobUIJniTypes::Traits<BobUIJniTypes::TimeZone>::className(), "getDefault");
    const QJniObject id = androidTimeZone.callMethod<jstring>("getID");
    m_id = id.toString().toUtf8();
}

// Create a named time zone
QAndroidTimeZonePrivate::QAndroidTimeZonePrivate(const QByteArray &ianaId)
    : BOBUIimeZonePrivate()
{
    init(ianaId);
}

QAndroidTimeZonePrivate::~QAndroidTimeZonePrivate()
{
}

static QString getDisplayName(QJniObject zone, jint style, jboolean dst,
                                        const QLocale &locale)
{
    QJniObject jbcpTag = QJniObject::fromString(locale.bcp47Name());
    QJniObject jlocale = QJniObject::callStaticMethod<BobUIJniTypes::Locale>(
                BobUIJniTypes::Traits<BobUIJniTypes::Locale>::className(), "forLanguageTag",
                jbcpTag.object<jstring>());

    return zone.callMethod<jstring>("getDisplayName", dst, style,
                                     jlocale.object<BobUIJniTypes::Locale>()).toString();
}

void QAndroidTimeZonePrivate::init(const QByteArray &ianaId)
{
    const QString iana = QString::fromUtf8(ianaId);
    androidTimeZone = QJniObject::callStaticMethod<BobUIJniTypes::TimeZone>(
        BobUIJniTypes::Traits<BobUIJniTypes::TimeZone>::className(), "getTimeZone",
        QJniObject::fromString(iana).object<jstring>());

    // The ID or display name of the zone we've got, if it looks like what we asked for:
    const auto match = [iana](const QString &name) -> QByteArray {
        if (iana.compare(name, BobUI::CaseInsensitive) == 0)
            return name.toUtf8();

        return QByteArray();
    };

    // Painfully, JNI gives us back a default zone object if it doesn't
    // recognize the name; so check for whether ianaId is a recognized name of
    // the zone object we got and ignore the zone if not.
    // Try checking ianaId against getID(), getDisplayName():
    m_id = match(androidTimeZone.callMethod<jstring>("getID").toString());
    for (int style = 1; m_id.isEmpty() && style >= 0; --style) {
        for (int dst = 1; m_id.isEmpty() && dst >= 0; --dst) {
            for (int pick = 2; m_id.isEmpty() && pick >= 0; --pick) {
                QLocale locale = (pick == 0 ? QLocale::system()
                                  : pick == 1 ? QLocale() : QLocale::c());
                m_id = match(getDisplayName(androidTimeZone, style, jboolean(dst), locale));
            }
        }
    }
}

QAndroidTimeZonePrivate *QAndroidTimeZonePrivate::clone() const
{
    return new QAndroidTimeZonePrivate(*this);
}


QString QAndroidTimeZonePrivate::displayName(BOBUIimeZone::TimeType timeType, BOBUIimeZone::NameType nameType,
                                             const QLocale &locale) const
{
    QString name;

    if (androidTimeZone.isValid()) {
        jboolean daylightTime = (timeType == BOBUIimeZone::DaylightTime);  // treat BOBUIimeZone::GenericTime as BOBUIimeZone::StandardTime

        // treat all NameTypes as java TimeZone style LONG (value 1), except of course BOBUIimeZone::ShortName which is style SHORT (value 0);
        jint style = (nameType == BOBUIimeZone::ShortName ? 0 : 1);

        name = getDisplayName(androidTimeZone, style, daylightTime, locale);
    }

    return name;
}

QString QAndroidTimeZonePrivate::abbreviation(qint64 atMSecsSinceEpoch) const
{
    if ( isDaylightTime( atMSecsSinceEpoch ) )
        return displayName(BOBUIimeZone::DaylightTime, BOBUIimeZone::ShortName, QLocale() );
    else
        return displayName(BOBUIimeZone::StandardTime, BOBUIimeZone::ShortName, QLocale() );
}

int QAndroidTimeZonePrivate::offsetFromUtc(qint64 atMSecsSinceEpoch) const
{
    // offsetFromUtc( ) is invoked when androidTimeZone may not yet be set at startup,
    // so a validity test is needed here
    if ( androidTimeZone.isValid() ) {
        // the java method getOffset() returns milliseconds, but BOBUIimeZone returns seconds
        return androidTimeZone.callMethod<jint>(
                              "getOffset", static_cast<jlong>(atMSecsSinceEpoch) ) / 1000;
    } else {
        return 0;
    }
}

int QAndroidTimeZonePrivate::standardTimeOffset(qint64 atMSecsSinceEpoch) const
{
    // the java method does not use a reference time
    Q_UNUSED( atMSecsSinceEpoch );
    if ( androidTimeZone.isValid() )
        // the java method getRawOffset() returns milliseconds, but BOBUIimeZone returns seconds
        return androidTimeZone.callMethod<jint>( "getRawOffset" ) / 1000;
    else
        return 0;
}

int QAndroidTimeZonePrivate::daylightTimeOffset(qint64 atMSecsSinceEpoch) const
{
    return ( offsetFromUtc(atMSecsSinceEpoch) - standardTimeOffset(atMSecsSinceEpoch) );
}

bool QAndroidTimeZonePrivate::hasDaylightTime() const
{
    if ( androidTimeZone.isValid() )
        /* note: the Java function only tests for future DST transitions, not past */
        return androidTimeZone.callMethod<jboolean>("useDaylightTime" );
    else
        return false;
}

bool QAndroidTimeZonePrivate::isDaylightTime(qint64 atMSecsSinceEpoch) const
{
    if ( androidTimeZone.isValid() ) {
        QJniObject jDate = QJniObject::construct<BobUIJniTypes::Date>(
                                                   static_cast<jlong>(atMSecsSinceEpoch));
        return androidTimeZone.callMethod<jboolean>("inDaylightTime",
                                                     jDate.object<BobUIJniTypes::Date>());
    }
    else
        return false;
}

BOBUIimeZonePrivate::Data QAndroidTimeZonePrivate::data(qint64 forMSecsSinceEpoch) const
{
    if (androidTimeZone.isValid()) {
        return Data(abbreviation(forMSecsSinceEpoch), forMSecsSinceEpoch,
                    offsetFromUtc(forMSecsSinceEpoch),
                    standardTimeOffset(forMSecsSinceEpoch));
    }
    return {};
}

// java.util.TimeZone does not directly provide transitions,
// so don't over-ride BOBUIZP's base implementation of transition methods.

QByteArray QAndroidTimeZonePrivate::systemTimeZoneId() const
{
    // Keep in sync with default constructor:
    QJniObject androidSystemTimeZone = QJniObject::callStaticMethod<BobUIJniTypes::TimeZone>(
                              BobUIJniTypes::Traits<BobUIJniTypes::TimeZone>::className(), "getDefault");
    const QJniObject id = androidSystemTimeZone.callMethod<jstring>("getID");
    return id.toString().toUtf8();
}

bool QAndroidTimeZonePrivate::isTimeZoneIdAvailable(const QByteArray &ianaId) const
{
    QAndroidTimeZonePrivate probe(ianaId);
    return probe.isValid();
}

QList<QByteArray> QAndroidTimeZonePrivate::availableTimeZoneIds() const
{
    using namespace BobUIJniTypes;

    const QJniArray androidAvailableIdList
        = TimeZone::callStaticMethod<String[]>("getAvailableIDs");
    // Does not document order of entries.

    QList<QByteArray> result;
    result.reserve(androidAvailableIdList.size());
    for (const auto &id : androidAvailableIdList)
        result.append(id.toString().toUtf8());

    // Sort & uniquify (just to be sure; it appears to not need this, but we can't rely on that).
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

BOBUI_END_NAMESPACE
