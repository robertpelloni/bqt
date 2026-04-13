// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiextoption.h"
#include "qguiapplication.h"
#include "qlist.h"

BOBUI_BEGIN_NAMESPACE

BOBUI_IMPL_METATYPE_EXTERN_TAGGED(BOBUIextOption::Tab, BOBUIextOption_Tab)

struct BOBUIextOptionPrivate
{
    QList<BOBUIextOption::Tab> tabStops;
};

/*!
    Constructs a text option with default properties for text.
    The text alignment property is set to BobUI::AlignLeft. The
    word wrap property is set to BOBUIextOption::WordWrap. The
    using of design metrics flag is set to false.
*/
BOBUIextOption::BOBUIextOption()
    : BOBUIextOption(BobUI::AlignLeft)
{
    direction = BobUI::LayoutDirectionAuto;
}

/*!
    Constructs a text option with the given \a alignment for text.
    The word wrap property is set to BOBUIextOption::WordWrap. The using
    of design metrics flag is set to false.
*/
BOBUIextOption::BOBUIextOption(BobUI::Alignment alignment)
    : align(alignment),
      wordWrap(BOBUIextOption::WordWrap),
      design(false),
      unused(0),
      f(0),
      tab(-1),
      d(nullptr)
{
    direction = QGuiApplication::layoutDirection();
}

/*!
    Destroys the text option.
*/
BOBUIextOption::~BOBUIextOption()
{
    delete d;
}

/*!
    \fn BOBUIextOption::BOBUIextOption(const BOBUIextOption &other)

    Construct a copy of the \a other text option.
*/
BOBUIextOption::BOBUIextOption(const BOBUIextOption &o)
    : align(o.align),
      wordWrap(o.wordWrap),
      design(o.design),
      direction(o.direction),
      unused(o.unused),
      f(o.f),
      tab(o.tab),
      d(nullptr)
{
    if (o.d)
        d = new BOBUIextOptionPrivate(*o.d);
}

/*!
    \fn BOBUIextOption &BOBUIextOption::operator=(const BOBUIextOption &other)

    Returns \c true if the text option is the same as the \a other text option;
    otherwise returns \c false.
*/
BOBUIextOption &BOBUIextOption::operator=(const BOBUIextOption &o)
{
    if (this == &o)
        return *this;

    BOBUIextOptionPrivate* dNew = nullptr;
    if (o.d)
        dNew = new BOBUIextOptionPrivate(*o.d);
    delete d;
    d = dNew;

    align = o.align;
    wordWrap = o.wordWrap;
    design = o.design;
    direction = o.direction;
    unused = o.unused;
    f = o.f;
    tab = o.tab;
    return *this;
}

/*!
    Sets the tab positions for the text layout to those specified by
    \a tabStops.

    \sa tabArray(), setTabStopDistance(), setTabs()
*/
void BOBUIextOption::setTabArray(const QList<qreal> &tabStops)
{
    if (!d)
        d = new BOBUIextOptionPrivate;
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab;
    tabs.reserve(tabStops.size());
    for (qreal pos : tabStops) {
        tab.position = pos;
        tabs.append(tab);
    }
    d->tabStops = tabs;
}

/*!
    \since 4.4
    Sets the tab positions for the text layout to those specified by
    \a tabStops.

    \sa tabStopDistance()
*/
void BOBUIextOption::setTabs(const QList<BOBUIextOption::Tab> &tabStops)
{
    if (!d)
        d = new BOBUIextOptionPrivate;
    d->tabStops = tabStops;
}

/*!
    Returns a list of tab positions defined for the text layout.

    \sa setTabArray(), tabStopDistance()
*/
QList<qreal> BOBUIextOption::tabArray() const
{
    QList<qreal> answer;
    if (!d)
        return answer;

    answer.reserve(d->tabStops.size());
    QList<BOBUIextOption::Tab>::ConstIterator iter = d->tabStops.constBegin();
    while(iter != d->tabStops.constEnd()) {
        answer.append( (*iter).position);
        ++iter;
    }
    return answer;
}


QList<BOBUIextOption::Tab> BOBUIextOption::tabs() const
{
    if (!d)
        return QList<BOBUIextOption::Tab>();
    return d->tabStops;
}

/*!
    \class BOBUIextOption
    \reentrant

    \brief The BOBUIextOption class provides a description of general rich text
    properties.
    \inmodule BobUIGui

    \ingroup richtext-processing

    BOBUIextOption is used to encapsulate common rich text properties in a single
    object. It contains information about text alignment, layout direction,
    word wrapping, and other standard properties associated with text rendering
    and layout.

    \sa BOBUIextEdit, BOBUIextDocument, BOBUIextCursor
*/

/*!
    \enum BOBUIextOption::WrapMode

    This enum describes how text is wrapped in a document.

    \value NoWrap       Text is not wrapped at all.
    \value WordWrap     Text is wrapped at word boundaries.
    \value ManualWrap   Same as BOBUIextOption::NoWrap
    \value WrapAnywhere Text can be wrapped at any point on a line, even if
                        it occurs in the middle of a word.
    \value WrapAtWordBoundaryOrAnywhere If possible, wrapping occurs at a word
                        boundary; otherwise it will occur at the appropriate
                        point on the line, even in the middle of a word.
*/

/*!
  \fn void BOBUIextOption::setUseDesignMetrics(bool enable)

    If \a enable is true then the layout will use design metrics;
    otherwise it will use the metrics of the paint device (which is
    the default behavior).

    \sa useDesignMetrics()
*/

/*!
  \fn bool BOBUIextOption::useDesignMetrics() const

    Returns \c true if the layout uses design rather than device metrics;
    otherwise returns \c false.

    \sa setUseDesignMetrics()
*/

/*!
  \fn BobUI::Alignment BOBUIextOption::alignment() const

  Returns the text alignment defined by the option.

  \sa setAlignment()
*/

/*!
  \fn void BOBUIextOption::setAlignment(BobUI::Alignment alignment);

  Sets the option's text alignment to the specified \a alignment.

  \sa alignment()
*/

/*!
  \fn BobUI::LayoutDirection BOBUIextOption::textDirection() const

  Returns the direction of the text layout defined by the option.

  \sa setTextDirection()
*/

/*!
  \fn void BOBUIextOption::setTextDirection(BobUI::LayoutDirection direction)

  Sets the direction of the text layout defined by the option to the
  given \a direction.

  \sa textDirection()
*/

/*!
  \fn WrapMode BOBUIextOption::wrapMode() const

  Returns the text wrap mode defined by the option.

  \sa setWrapMode()
*/

/*!
  \fn void BOBUIextOption::setWrapMode(WrapMode mode)

  Sets the option's text wrap mode to the given \a mode.
*/

/*!
  \enum BOBUIextOption::Flag

  \value IncludeTrailingSpaces When this option is set, BOBUIextLine::naturalTextWidth() and naturalTextRect() will
                               return a value that includes the width of trailing spaces in the text; otherwise
                               this width is excluded.
  \value ShowTabsAndSpaces Visualize spaces with little dots, and tabs with little arrows. Non-breaking spaces are
            shown differently to breaking spaces.
  \value ShowLineAndParagraphSeparators Visualize line and paragraph separators with appropriate symbol characters.
  \value [since 5.7] ShowDocumentTerminator Visualize the end of the document with a section sign.
  \value [since 6.9] ShowDefaultIgnorables Render normally non-visual characters if supported by font.
  \value AddSpaceForLineAndParagraphSeparators While determining the line-break positions take into account the
            space added for drawing a separator character.
  \value SuppressColors Suppress all color changes in the character formats (except the main selection).
  \value [since 6.9] DisableEmojiParsing By default, BobUI will detect emoji sequences in input strings
    and prioritize using color fonts to display them. This extra step can be disabled by setting the
    DisableEmojiParsing flag if it is known in advance that it will not be needed.
*/

/*!
  \fn Flags BOBUIextOption::flags() const

  Returns the flags associated with the option.

  \sa setFlags()
*/

/*!
  \fn void BOBUIextOption::setFlags(Flags flags)

  Sets the flags associated with the option to the given \a flags.

  \sa flags()
*/

/*!
  \fn qreal BOBUIextOption::tabStopDistance() const
  \since 5.10

  Returns the distance in device units between tab stops.

  \sa setTabStopDistance(), tabArray(), setTabs(), tabs()
*/

/*!
  \fn void BOBUIextOption::setTabStopDistance(qreal tabStopDistance)
  \since 5.10

  Sets the default distance in device units between tab stops to the value specified
  by \a tabStopDistance.

  \sa tabStopDistance(), setTabArray(), setTabs(), tabs()
*/

/*!
    \enum BOBUIextOption::TabType
    \since 4.4

    This enum holds the different types of tabulator

    \value LeftTab      A left-tab
    \value RightTab     A right-tab
    \value CenterTab    A centered-tab
    \value DelimiterTab A tab stopping at a certain delimiter-character
*/

/*!
    \class BOBUIextOption::Tab
    \since 4.4
    \inmodule BobUIGui
    Each tab definition is represented by this struct.
*/

/*!
    \variable BOBUIextOption::Tab::position
    Distance from the start of the paragraph.
    The position of a tab is from the start of the paragraph which implies that when
    the alignment of the paragraph is set to centered, the tab is interpreted to be
    moved the same distance as the left edge of the paragraph does.
    In case the paragraph is set to have a layoutDirection() RightToLeft the position
    is interpreted to be from the right side of the paragraph with higher numbers moving
    the tab to the left.
*/

/*!
    \variable BOBUIextOption::Tab::type
    Determine which type is used.
    In a paragraph that has layoutDirection() RightToLeft the type LeftTab will
    be interpreted to be a RightTab and vice versa.
*/

/*!
    \variable BOBUIextOption::Tab::delimiter
    If type is DelimitorTab; tab until this char is found in the text.
*/

/*!
    \fn BOBUIextOption::Tab::Tab()
    Creates a default left tab with position 80.
*/

/*!
    \fn BOBUIextOption::Tab::Tab(qreal pos, TabType tabType, QChar delim = QChar())

    Creates a tab with the given position, tab type, and delimiter
    (\a pos, \a tabType, \a delim).

    \note \a delim is only used when \a tabType is DelimiterTab.

    \since 4.7
*/

/*!
    \fn bool BOBUIextOption::Tab::operator==(const BOBUIextOption::Tab &other) const

    Returns \c true if tab \a other is equal to this tab;
    otherwise returns \c false.
*/

/*!
    \fn bool BOBUIextOption::Tab::operator!=(const BOBUIextOption::Tab &other) const

    Returns \c true if tab \a other is not equal to this tab;
    otherwise returns \c false.
*/

/*!
  \since 4.4
  \fn QList<BOBUIextOption::Tab> BOBUIextOption::tabs() const
  Returns a list of tab positions defined for the text layout.

  \sa tabStopDistance(), setTabs(), setTabStopDistance()
*/


BOBUI_END_NAMESPACE
