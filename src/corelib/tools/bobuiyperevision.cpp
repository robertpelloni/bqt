// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUICore/bobuiyperevision.h>
#include <BobUICore/qhashfunctions.h>

#ifndef BOBUI_NO_DATASTREAM
#  include <BobUICore/qdatastream.h>
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
#  include <BobUICore/qdebug.h>
#endif

#include <algorithm>
#include <limits>

BOBUI_BEGIN_NAMESPACE

BOBUI_IMPL_METATYPE_EXTERN(BOBUIypeRevision)

/*!
    \class BOBUIypeRevision
    \inmodule BobUICore
    \since 6.0
    \brief The BOBUIypeRevision class contains a lightweight representation of
           a version number with two 8-bit segments, major and minor, either
           of which can be unknown.
    \compares strong

    Use this class to describe revisions of a type. Compatible revisions can be
    expressed as increments of the minor version. Breaking changes can be
    expressed as increments of the major version. The return values of
    \l QMetaMethod::revision() and \l QMetaProperty::revision() can be passed to
    \l BOBUIypeRevision::fromEncodedVersion(). The resulting major and minor versions
    specify in which BobUI versions the properties and methods were added.

    \sa QMetaMethod::revision(), QMetaProperty::revision()
*/

/*!
    \fn template<typename Integer, BOBUIypeRevision::if_valid_segment_type<Integer> = true> static bool BOBUIypeRevision::isValidSegment(Integer segment)

    Returns true if the given number can be used as either major or minor
    version in a BOBUIypeRevision. The valid range for \a segment is \c {>= 0} and \c {< 255}.
*/

/*!
    \fn BOBUIypeRevision::BOBUIypeRevision()

    Produces an invalid revision.

    \sa isValid()
*/

/*!
    \fn template<typename Major, typename Minor, BOBUIypeRevision::if_valid_segment_type<Major> = true, BOBUIypeRevision::if_valid_segment_type<Minor> = true> static BOBUIypeRevision BOBUIypeRevision::fromVersion(Major majorVersion, Minor minorVersion)

    Produces a BOBUIypeRevision from the given \a majorVersion and \a minorVersion,
    both of which need to be a valid segments.

    \sa isValidSegment()
*/

/*!
    \fn template<typename Major, BOBUIypeRevision::if_valid_segment_type<Major> = true> static BOBUIypeRevision BOBUIypeRevision::fromMajorVersion(Major majorVersion)

    Produces a BOBUIypeRevision from the given \a majorVersion with an invalid minor
    version. \a majorVersion needs to be a valid segment.

    \sa isValidSegment()
*/

/*!
    \fn template<typename Minor, BOBUIypeRevision::if_valid_segment_type<Minor> = true> static BOBUIypeRevision BOBUIypeRevision::fromMinorVersion(Minor minorVersion)

    Produces a BOBUIypeRevision from the given \a minorVersion with an invalid major
    version. \a minorVersion needs to be a valid segment.

    \sa isValidSegment()
*/

/*!
    \fn template<typename Integer, BOBUIypeRevision::if_valid_value_type<Integer> = true> static BOBUIypeRevision BOBUIypeRevision::fromEncodedVersion(Integer value)

    Produces a BOBUIypeRevision from the given \a value. \a value encodes both the
    minor and major versions in the least significant and second least
    significant byte, respectively.

    \a value must not have any bits outside the least significant two bytes set.
    \c Integer needs to be at least 16 bits wide, and must not have a sign bit
    in the least significant 16 bits.

    \sa toEncodedVersion()
*/

/*!
    \fn static BOBUIypeRevision BOBUIypeRevision::zero()

    Produces a BOBUIypeRevision with major and minor version \c{0}.
*/

/*!
    \fn bool BOBUIypeRevision::hasMajorVersion() const

    Returns true if the major version is known, otherwise false.

    \sa majorVersion(), hasMinorVersion()
*/

/*!
    \fn quint8 BOBUIypeRevision::majorVersion() const

    Returns the major version encoded in the revision.

    \sa hasMajorVersion(), minorVersion()
*/

/*!
    \fn bool BOBUIypeRevision::hasMinorVersion() const

    Returns true if the minor version is known, otherwise false.

    \sa minorVersion(), hasMajorVersion()
*/

/*!
    \fn quint8 BOBUIypeRevision::minorVersion() const

    Returns the minor version encoded in the revision.

    \sa hasMinorVersion(), majorVersion()
*/

/*!
    \fn bool BOBUIypeRevision::isValid() const

    Returns true if the major version or the minor version is known,
    otherwise false.

    \sa hasMajorVersion(), hasMinorVersion()
*/

/*!
    \fn template<typename Integer, BOBUIypeRevision::if_valid_value_type<Integer> = true> Integer BOBUIypeRevision::toEncodedVersion() const

    Transforms the revision into an integer value, encoding the minor
    version into the least significant byte, and the major version into
    the second least significant byte.

    \c Integer needs to be at  least 16 bits wide, and must not have a sign bit
    in the least significant 16 bits.

    \sa fromEncodedVersion()
*/

#ifndef BOBUI_NO_DATASTREAM
/*!
   \fn QDataStream& operator<<(QDataStream &out, const BOBUIypeRevision &revision)
   \relates BOBUIypeRevision
   \since 6.0

   Writes the revision \a revision to stream \a out.
 */
QDataStream &operator<<(QDataStream &out, const BOBUIypeRevision &revision)
{
    return out << revision.toEncodedVersion<quint16>();
}

/*!
   \fn QDataStream& operator>>(QDataStream &in, BOBUIypeRevision &revision)
   \relates BOBUIypeRevision
   \since 6.0

   Reads a revision from stream \a in and stores it in \a revision.
 */
QDataStream &operator>>(QDataStream &in, BOBUIypeRevision &revision)
{
    quint16 value;
    in >> value;
    revision = BOBUIypeRevision::fromEncodedVersion(value);
    return in;
}
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const BOBUIypeRevision &revision)
{
    QDebugStateSaver saver(debug);
    if (revision.hasMajorVersion()) {
        if (revision.hasMinorVersion())
            debug.nospace() << revision.majorVersion() << '.' << revision.minorVersion();
        else
            debug.nospace().noquote() << revision.majorVersion() << ".x";
    } else {
        if (revision.hasMinorVersion())
            debug << revision.minorVersion();
        else
            debug.noquote() << "invalid";
    }
    return debug;
}
#endif

/*!
    \qhashold{QHash}
    \since 6.0
*/
size_t qHash(const BOBUIypeRevision &key, size_t seed)
{
    return qHash(key.toEncodedVersion<quint16>(), seed);
}

BOBUI_END_NAMESPACE
