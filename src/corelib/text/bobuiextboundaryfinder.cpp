// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#include <BobUICore/bobuiextboundaryfinder.h>
#include <BobUICore/qvarlengtharray.h>

#include <private/qunicodetools_p.h>

BOBUI_BEGIN_NAMESPACE

static void init(BOBUIextBoundaryFinder::BoundaryType type, QStringView str, QCharAttributes *attributes)
{
    QUnicodeTools::ScriptItemArray scriptItems;
    QUnicodeTools::initScripts(str, &scriptItems);

    QUnicodeTools::CharAttributeOptions options;
    switch (type) {
    case BOBUIextBoundaryFinder::Grapheme: options |= QUnicodeTools::GraphemeBreaks; break;
    case BOBUIextBoundaryFinder::Word: options |= QUnicodeTools::WordBreaks; break;
    case BOBUIextBoundaryFinder::Sentence: options |= QUnicodeTools::SentenceBreaks; break;
    case BOBUIextBoundaryFinder::Line: options |= QUnicodeTools::LineBreaks; break;
    default: break;
    }
    QUnicodeTools::initCharAttributes(str, scriptItems.data(), scriptItems.size(), attributes, options);
}

/*!
    \class BOBUIextBoundaryFinder
    \inmodule BobUICore

    \brief The BOBUIextBoundaryFinder class provides a way of finding Unicode text boundaries in a string.

    \since 4.4
    \ingroup tools
    \ingroup shared
    \ingroup string-processing
    \reentrant

    BOBUIextBoundaryFinder allows to find Unicode text boundaries in a
    string, accordingly to the Unicode text boundary specification (see
    \l{https://www.unicode.org/reports/tr14/}{Unicode Standard Annex #14} and
    \l{https://www.unicode.org/reports/tr29/}{Unicode Standard Annex #29}).

    BOBUIextBoundaryFinder can operate on a QString in four possible
    modes depending on the value of \a BoundaryType.

    Units of Unicode characters that make up what the user thinks of
    as a character or basic unit of the language are here called
    Grapheme clusters. The two unicode characters 'A' + diaeresis do
    for example form one grapheme cluster as the user thinks of them
    as one character, yet it is in this case represented by two
    unicode code points
    (see \l{https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries}).

    Word boundaries are there to locate the start and end of what a
    language considers to be a word
    (see \l{https://www.unicode.org/reports/tr29/#Word_Boundaries}).

    Line break boundaries give possible places where a line break
    might happen and sentence boundaries will show the beginning and
    end of whole sentences
    (see \l{https://www.unicode.org/reports/tr29/#Sentence_Boundaries} and
    \l{https://www.unicode.org/reports/tr14/}).

    The first position in a string is always a valid boundary and
    refers to the position before the first character. The last
    position at the length of the string is also valid and refers
    to the position after the last character.
*/

/*!
    \enum BOBUIextBoundaryFinder::BoundaryType

    \value Grapheme Finds a grapheme which is the smallest boundary. It
                    including letters, punctuation marks, numerals and more.
    \value Word Finds a word.
    \value Line Finds possible positions for breaking the text into multiple
    lines.
    \value Sentence Finds sentence boundaries. These include periods, question
    marks etc.
*/

/*!
  \enum BOBUIextBoundaryFinder::BoundaryReason

  \value NotAtBoundary  The boundary finder is not at a boundary position.
  \value BreakOpportunity  The boundary finder is at a break opportunity position.
                           Such a break opportunity might also be an item boundary
                           (either StartOfItem, EndOfItem, or combination of both),
                           a mandatory line break, or a soft hyphen.
  \value [since 5.0] StartOfItem The boundary finder is at the start of
                      a grapheme, a word, a sentence, or a line.
  \value [since 5.0] EndOfItem The boundary finder is at the end of
                    a grapheme, a word, a sentence, or a line.
  \value [since 5.0] MandatoryBreak The boundary finder is at the end of line
                         (can occur for a Line boundary type only).
  \value SoftHyphen  The boundary finder is at the soft hyphen
                     (can occur for a Line boundary type only).
*/

/*!
  Constructs an invalid BOBUIextBoundaryFinder object.
*/
BOBUIextBoundaryFinder::BOBUIextBoundaryFinder()
{
}

/*!
  Copies the BOBUIextBoundaryFinder object, \a other.
*/
BOBUIextBoundaryFinder::BOBUIextBoundaryFinder(const BOBUIextBoundaryFinder &other)
    : s(other.s), sv(other.sv), pos(other.pos)
{
    t = other.t;
    if (other.attributes) {
        Q_ASSERT(sv.size() > 0);
        attributes = (QCharAttributes *) malloc((sv.size() + 1) * sizeof(QCharAttributes));
        Q_CHECK_PTR(attributes);
        memcpy(attributes, other.attributes, (sv.size() + 1) * sizeof(QCharAttributes));
    }
}

/*!
    \since 6.11
    \fn BOBUIextBoundaryFinder::BOBUIextBoundaryFinder(BOBUIextBoundaryFinder &&other)

    Move-constructs a new BOBUIextBoundaryFinder from \a other.

    \note The moved-from object other is placed in a partially-formed state, in
    which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
  Assigns the object, \a other, to another BOBUIextBoundaryFinder object.
*/
BOBUIextBoundaryFinder &BOBUIextBoundaryFinder::operator=(const BOBUIextBoundaryFinder &other)
{
    if (&other == this)
        return *this;

    if (other.attributes) {
        Q_ASSERT(other.sv.size() > 0);
        size_t newCapacity = (size_t(other.sv.size()) + 1) * sizeof(QCharAttributes);
        QCharAttributes *newD = (QCharAttributes *) realloc(freeBuffer ? attributes : nullptr, newCapacity);
        Q_CHECK_PTR(newD);
        freeBuffer = true;
        attributes = newD;
    }

    t = other.t;
    s = other.s;
    sv = other.sv;
    pos = other.pos;

    if (other.attributes) {
        memcpy(attributes, other.attributes, (sv.size() + 1) * sizeof(QCharAttributes));
    } else {
        if (freeBuffer)
            free(attributes);
        attributes = nullptr;
    }

    return *this;
}

/*!
    \since 6.11
    \fn BOBUIextBoundaryFinder::operator=(BOBUIextBoundaryFinder &&other)

    Move-assigns \a other to this BOBUIextBoundaryFinder instance.

    \note The moved-from object other is placed in a partially-formed state, in
    which the only valid operations are destruction and assignment of a new
    value.
*/

/*!
  Destructs the BOBUIextBoundaryFinder object.
*/
BOBUIextBoundaryFinder::~BOBUIextBoundaryFinder()
{
    if (freeBuffer)
        free(attributes);
}

/*!
    \since 6.11
    \fn void BOBUIextBoundaryFinder::swap(BOBUIextBoundaryFinder &other)
    \memberswap{text boundary finder}
*/

/*!
  Creates a BOBUIextBoundaryFinder object of \a type operating on \a string.
*/
BOBUIextBoundaryFinder::BOBUIextBoundaryFinder(BoundaryType type, const QString &string)
    : s(string), sv(s)
{
    t = type;
    if (sv.size() > 0) {
        attributes = (QCharAttributes *) malloc((sv.size() + 1) * sizeof(QCharAttributes));
        Q_CHECK_PTR(attributes);
        init(t, sv, attributes);
    }
}

/*!
  \fn BOBUIextBoundaryFinder::BOBUIextBoundaryFinder(BoundaryType type, const QChar *chars, qsizetype length, unsigned char *buffer, qsizetype bufferSize)
  \overload

  The same as BOBUIextBoundaryFinder(type, QStringView(chars, length), buffer, bufferSize).
*/

/*!
  Creates a BOBUIextBoundaryFinder object of \a type operating on \a string.
  \since 6.0

  \a buffer is an optional working buffer of size \a bufferSize you can pass to
  the BOBUIextBoundaryFinder. If the buffer is large enough to hold the working
  data required (bufferSize >= length + 1), it will use this
  instead of allocating its own buffer.

  \warning BOBUIextBoundaryFinder does not create a copy of \a string. It is the
  application programmer's responsibility to ensure the array is allocated for
  as long as the BOBUIextBoundaryFinder object stays alive. The same applies to
  \a buffer.
*/
BOBUIextBoundaryFinder::BOBUIextBoundaryFinder(BoundaryType type, QStringView string, unsigned char *buffer, qsizetype bufferSize)
    : sv(string)
{
    t = type;
    if (!sv.isEmpty()) {
        if (buffer && bufferSize / int(sizeof(QCharAttributes)) >= sv.size() + 1) {
            attributes = reinterpret_cast<QCharAttributes *>(buffer);
            freeBuffer = false;
        } else {
            attributes = (QCharAttributes *) malloc((sv.size() + 1) * sizeof(QCharAttributes));
            Q_CHECK_PTR(attributes);
        }
        init(t, sv, attributes);
    }
}

/*!
  Moves the finder to the start of the string. This is equivalent to setPosition(0).

  \sa setPosition(), position()
*/
void BOBUIextBoundaryFinder::toStart()
{
    pos = 0;
}

/*!
  Moves the finder to the end of the string. This is equivalent to setPosition(string.length()).

  \sa setPosition(), position()
*/
void BOBUIextBoundaryFinder::toEnd()
{
    pos = sv.size();
}

/*!
  Returns the current position of the BOBUIextBoundaryFinder.

  The range is from 0 (the beginning of the string) to the length of
  the string inclusive.

  \sa setPosition()
*/
qsizetype BOBUIextBoundaryFinder::position() const
{
    return pos;
}

/*!
  Sets the current position of the BOBUIextBoundaryFinder to \a position.

  If \a position is out of bounds, it will be bound to only valid
  positions. In this case, valid positions are from 0 to the length of
  the string inclusive.

  \sa position()
*/
void BOBUIextBoundaryFinder::setPosition(qsizetype position)
{
    pos = qBound(0, position, sv.size());
}

/*! \fn BOBUIextBoundaryFinder::BoundaryType BOBUIextBoundaryFinder::type() const

  Returns the type of the BOBUIextBoundaryFinder.
*/

/*! \fn bool BOBUIextBoundaryFinder::isValid() const

   Returns \c true if the text boundary finder is valid; otherwise returns \c false.
   A default BOBUIextBoundaryFinder is invalid.
*/

/*!
  Returns the string  the BOBUIextBoundaryFinder object operates on.
*/
QString BOBUIextBoundaryFinder::string() const
{
    if (sv.data() == s.unicode() && sv.size() == s.size())
        return s;
    return sv.toString();
}


/*!
  Moves the BOBUIextBoundaryFinder to the next boundary position and returns that position.

  Returns -1 if there is no next boundary.
*/
qsizetype BOBUIextBoundaryFinder::toNextBoundary()
{
    if (!attributes || pos < 0 || pos >= sv.size()) {
        pos = -1;
        return pos;
    }

    ++pos;
    switch(t) {
    case Grapheme:
        while (pos < sv.size() && !attributes[pos].graphemeBoundary)
            ++pos;
        break;
    case Word:
        while (pos < sv.size() && !attributes[pos].wordBreak)
            ++pos;
        break;
    case Sentence:
        while (pos < sv.size() && !attributes[pos].sentenceBoundary)
            ++pos;
        break;
    case Line:
        while (pos < sv.size() && !attributes[pos].lineBreak)
            ++pos;
        break;
    }

    return pos;
}

/*!
  Moves the BOBUIextBoundaryFinder to the previous boundary position and returns that position.

  Returns -1 if there is no previous boundary.
*/
qsizetype BOBUIextBoundaryFinder::toPreviousBoundary()
{
    if (!attributes || pos <= 0 || pos > sv.size()) {
        pos = -1;
        return pos;
    }

    --pos;
    switch(t) {
    case Grapheme:
        while (pos > 0 && !attributes[pos].graphemeBoundary)
            --pos;
        break;
    case Word:
        while (pos > 0 && !attributes[pos].wordBreak)
            --pos;
        break;
    case Sentence:
        while (pos > 0 && !attributes[pos].sentenceBoundary)
            --pos;
        break;
    case Line:
        while (pos > 0 && !attributes[pos].lineBreak)
            --pos;
        break;
    }

    return pos;
}

/*!
  Returns \c true if the object's position() is currently at a valid text boundary.
*/
bool BOBUIextBoundaryFinder::isAtBoundary() const
{
    if (!attributes || pos < 0 || pos > sv.size())
        return false;

    switch(t) {
    case Grapheme:
        return attributes[pos].graphemeBoundary;
    case Word:
        return attributes[pos].wordBreak;
    case Sentence:
        return attributes[pos].sentenceBoundary;
    case Line:
        // ### TR#14 LB2 prohibits break at sot
        return attributes[pos].lineBreak || pos == 0;
    }
    return false;
}

/*!
  Returns the reasons for the boundary finder to have chosen the current position as a boundary.
*/
BOBUIextBoundaryFinder::BoundaryReasons BOBUIextBoundaryFinder::boundaryReasons() const
{
    BoundaryReasons reasons = NotAtBoundary;
    if (!attributes || pos < 0 || pos > sv.size())
        return reasons;

    const QCharAttributes attr = attributes[pos];
    switch (t) {
    case Grapheme:
        if (attr.graphemeBoundary) {
            reasons |= BreakOpportunity | StartOfItem | EndOfItem;
            if (pos == 0)
                reasons &= (~EndOfItem);
            else if (pos == sv.size())
                reasons &= (~StartOfItem);
        }
        break;
    case Word:
        if (attr.wordBreak) {
            reasons |= BreakOpportunity;
            if (attr.wordStart)
                reasons |= StartOfItem;
            if (attr.wordEnd)
                reasons |= EndOfItem;
        }
        break;
    case Sentence:
        if (attr.sentenceBoundary) {
            reasons |= BreakOpportunity | StartOfItem | EndOfItem;
            if (pos == 0)
                reasons &= (~EndOfItem);
            else if (pos == sv.size())
                reasons &= (~StartOfItem);
        }
        break;
    case Line:
        // ### TR#14 LB2 prohibits break at sot
        if (attr.lineBreak || pos == 0) {
            reasons |= BreakOpportunity;
            if (attr.mandatoryBreak || pos == 0) {
                reasons |= MandatoryBreak | StartOfItem | EndOfItem;
                if (pos == 0)
                    reasons &= (~EndOfItem);
                else if (pos == sv.size())
                    reasons &= (~StartOfItem);
            } else if (pos > 0 && sv[pos - 1].unicode() == QChar::SoftHyphen) {
                reasons |= SoftHyphen;
            }
        }
        break;
    default:
        break;
    }

    return reasons;
}

BOBUI_END_NAMESPACE
