// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:trivial-impl-only

#ifndef BOBUIEXTBOUNDARYFINDER_H
#define BOBUIEXTBOUNDARYFINDER_H

#include <BobUICore/qchar.h>
#include <BobUICore/qstring.h>

#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0)
#  include <array>
#endif

BOBUI_BEGIN_NAMESPACE

struct QCharAttributes;

class Q_CORE_EXPORT BOBUIextBoundaryFinder
{
public:
    BOBUIextBoundaryFinder();
    BOBUIextBoundaryFinder(const BOBUIextBoundaryFinder &other);
    BOBUIextBoundaryFinder(BOBUIextBoundaryFinder &&other) noexcept
        : s{std::move(other.s)},
          sv{other.sv},
          pos{other.pos},
          attributes{std::exchange(other.attributes, nullptr)}
    {
          t = other.t;
          freeBuffer = other.freeBuffer;
          BOBUI7_ONLY(reserved = other.reserved;)
    }
    BOBUIextBoundaryFinder &operator=(const BOBUIextBoundaryFinder &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(BOBUIextBoundaryFinder)
    ~BOBUIextBoundaryFinder();

    void swap(BOBUIextBoundaryFinder &other) noexcept
    {
        std::swap(t, other.t);
        s.swap(other.s);
        std::swap(sv, other.sv);
        std::swap(pos, other.pos);
        std::swap(freeBuffer, other.freeBuffer);
        bobui_ptr_swap(attributes, other.attributes);
        BOBUI7_ONLY(reserved.swap(other.reserved);)
    }

    enum BoundaryType BOBUI7_ONLY(: quint8) {
        Grapheme,
        Word,
        Sentence,
        Line
    };

    enum BoundaryReason {
        NotAtBoundary = 0,
        BreakOpportunity = 0x1f,
        StartOfItem = 0x20,
        EndOfItem = 0x40,
        MandatoryBreak = 0x80,
        SoftHyphen = 0x100
    };
    Q_DECLARE_FLAGS( BoundaryReasons, BoundaryReason )

    BOBUIextBoundaryFinder(BoundaryType type, const QString &string);
    BOBUIextBoundaryFinder(BoundaryType type, const QChar *chars, qsizetype length, unsigned char *buffer = nullptr, qsizetype bufferSize = 0)
        : BOBUIextBoundaryFinder(type, QStringView(chars, length), buffer, bufferSize)
    {}
    BOBUIextBoundaryFinder(BoundaryType type, QStringView str, unsigned char *buffer = nullptr, qsizetype bufferSize = 0);

    inline bool isValid() const { return attributes; }

    inline BoundaryType type() const { return t; }
    QString string() const;

    void toStart();
    void toEnd();
    qsizetype position() const;
    void setPosition(qsizetype position);

    qsizetype toNextBoundary();
    qsizetype toPreviousBoundary();

    bool isAtBoundary() const;
    BoundaryReasons boundaryReasons() const;

private:
    BOBUI6_ONLY(BoundaryType t = Grapheme;)
    QString s;
    QStringView sv;
    qsizetype pos = 0;
    BOBUI6_ONLY(uint freeBuffer = true;)
    QCharAttributes *attributes = nullptr;
#if BOBUI_VERSION >= BOBUI_VERSION_CHECK(7, 0, 0)
    bool freeBuffer = true;
    BoundaryType t = Grapheme;
    std::array<quint8, sizeof(void *) - 2> reserved = {};
#endif
};

Q_DECLARE_SHARED(BOBUIextBoundaryFinder)

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextBoundaryFinder::BoundaryReasons)

BOBUI_END_NAMESPACE

#endif

