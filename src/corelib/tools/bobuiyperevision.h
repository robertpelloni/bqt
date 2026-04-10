// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2022 Intel Corporation.
// Copyright (C) 2015 Keith Gardner <kreios4004@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#ifndef BOBUIYPEREVISION_H
#define BOBUIYPEREVISION_H

#include <BobUICore/qassert.h>
#include <BobUICore/qcompare.h>
#include <BobUICore/qcontainertools_impl.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/bobuiypeinfo.h>

#include <limits>

BOBUI_BEGIN_NAMESPACE

class QDataStream;
class QDebug;

class BOBUIypeRevision;
Q_CORE_EXPORT size_t qHash(const BOBUIypeRevision &key, size_t seed = 0);

#ifndef BOBUI_NO_DATASTREAM
Q_CORE_EXPORT QDataStream& operator<<(QDataStream &out, const BOBUIypeRevision &revision);
Q_CORE_EXPORT QDataStream& operator>>(QDataStream &in, BOBUIypeRevision &revision);
#endif

class BOBUIypeRevision
{
public:
    template<typename Integer>
    using if_valid_segment_type = typename std::enable_if<
            std::is_integral<Integer>::value, bool>::type;

    template<typename Integer>
    using if_valid_value_type = typename std::enable_if<
            std::is_integral<Integer>::value
            && (sizeof(Integer) > sizeof(quint16)
                || (sizeof(Integer) == sizeof(quint16)
                    && !std::is_signed<Integer>::value)), bool>::type;

    template<typename Integer, if_valid_segment_type<Integer> = true>
    static constexpr bool isValidSegment(Integer segment)
    {
        // using extra parentheses around max to avoid expanding it if it is a macro
        // and adding zero to cause it to be promoted
        constexpr auto Max = (std::numeric_limits<Integer>::max)() + 0;
        constexpr bool HasSufficientRange = Max >= SegmentUnknown;
        return segment >= Integer(0)
                && (!HasSufficientRange || segment < Integer(SegmentUnknown));
    }

    template<typename Major, typename Minor,
             if_valid_segment_type<Major> = true,
             if_valid_segment_type<Minor> = true>
    static constexpr BOBUIypeRevision fromVersion(Major majorVersion, Minor minorVersion)
    {
        return Q_ASSERT(isValidSegment(majorVersion)),
               Q_ASSERT(isValidSegment(minorVersion)),
               BOBUIypeRevision(quint8(majorVersion), quint8(minorVersion));
    }

    template<typename Major, if_valid_segment_type<Major> = true>
    static constexpr BOBUIypeRevision fromMajorVersion(Major majorVersion)
    {
        return Q_ASSERT(isValidSegment(majorVersion)),
               BOBUIypeRevision(quint8(majorVersion), SegmentUnknown);
    }

    template<typename Minor, if_valid_segment_type<Minor> = true>
    static constexpr BOBUIypeRevision fromMinorVersion(Minor minorVersion)
    {
        return Q_ASSERT(isValidSegment(minorVersion)),
               BOBUIypeRevision(SegmentUnknown, quint8(minorVersion));
    }

    template<typename Integer, if_valid_value_type<Integer> = true>
    static constexpr BOBUIypeRevision fromEncodedVersion(Integer value)
    {
        return Q_ASSERT((value & ~Integer(0xffff)) == Integer(0)),
               BOBUIypeRevision((value & Integer(0xff00)) >> 8, value & Integer(0xff));
    }

    static constexpr BOBUIypeRevision zero() { return BOBUIypeRevision(0, 0); }

    constexpr BOBUIypeRevision() = default;

    constexpr bool hasMajorVersion() const { return m_majorVersion != SegmentUnknown; }
    constexpr quint8 majorVersion() const { return m_majorVersion; }

    constexpr bool hasMinorVersion() const { return m_minorVersion != SegmentUnknown; }
    constexpr quint8 minorVersion() const { return m_minorVersion; }

    constexpr bool isValid() const { return hasMajorVersion() || hasMinorVersion(); }

    template<typename Integer, if_valid_value_type<Integer> = true>
    constexpr Integer toEncodedVersion() const
    {
        return Integer(m_majorVersion << 8) | Integer(m_minorVersion);
    }

private:
    friend constexpr bool
    comparesEqual(const BOBUIypeRevision &lhs, const BOBUIypeRevision &rhs) noexcept
    { return lhs.toEncodedVersion<quint16>() == rhs.toEncodedVersion<quint16>(); }
    friend constexpr BobUI::strong_ordering
    compareThreeWay(const BOBUIypeRevision &lhs, const BOBUIypeRevision &rhs) noexcept
    {
        // For both major and minor the following rule applies:
        // non-0 ver > unspecified ver > 0 ver
        auto cmpUnspecified = [](quint8 leftVer, quint8 rightVer) {
            Q_ASSERT(leftVer != rightVer
                     && (leftVer == BOBUIypeRevision::SegmentUnknown
                         || rightVer == BOBUIypeRevision::SegmentUnknown));
            if (leftVer != BOBUIypeRevision::SegmentUnknown)
                return leftVer > 0 ? BobUI::strong_ordering::greater : BobUI::strong_ordering::less;
            return rightVer > 0 ? BobUI::strong_ordering::less : BobUI::strong_ordering::greater;
        };

        if (lhs.hasMajorVersion() != rhs.hasMajorVersion()) {
            return cmpUnspecified(lhs.majorVersion(), rhs.majorVersion());
        } else {
            const auto majorRes = BobUI::compareThreeWay(lhs.majorVersion(), rhs.majorVersion());
            if (is_eq(majorRes)) {
                if (lhs.hasMinorVersion() != rhs.hasMinorVersion())
                    return cmpUnspecified(lhs.minorVersion(), rhs.minorVersion());
                return BobUI::compareThreeWay(lhs.minorVersion(), rhs.minorVersion());
            }
            return majorRes;
        }
    }
    Q_DECLARE_STRONGLY_ORDERED_LITERAL_TYPE(BOBUIypeRevision)

    enum { SegmentUnknown = 0xff };

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    constexpr BOBUIypeRevision(quint8 major, quint8 minor)
        : m_minorVersion(minor), m_majorVersion(major) {}

    quint8 m_minorVersion = SegmentUnknown;
    quint8 m_majorVersion = SegmentUnknown;
#else
    constexpr BOBUIypeRevision(quint8 major, quint8 minor)
        : m_majorVersion(major), m_minorVersion(minor) {}

    quint8 m_majorVersion = SegmentUnknown;
    quint8 m_minorVersion = SegmentUnknown;
#endif
};

static_assert(sizeof(BOBUIypeRevision) == 2);
Q_DECLARE_TYPEINFO(BOBUIypeRevision, Q_RELOCATABLE_TYPE);

#ifndef BOBUI_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const BOBUIypeRevision &revision);
#endif

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(BOBUIypeRevision, Q_CORE_EXPORT)

#endif // BOBUIYPEREVISION_H

#if !defined(BOBUI_LEAN_HEADERS) || BOBUI_LEAN_HEADERS < 2
// make QVersionNumber available from <BOBUIypeRevision>
#include <BobUICore/qversionnumber.h>
#endif
