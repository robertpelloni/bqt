// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiextdocument.h"
#include <bobuiextformat.h>
#include "bobuiextcursor_p.h"
#include "bobuiextdocument_p.h"
#include "bobuiextdocumentlayout_p.h"
#include "bobuiextdocumentfragment.h"
#include "bobuiextdocumentfragment_p.h"
#include "bobuiexttable.h"
#include "bobuiextlist.h"
#include <qdebug.h>
#include <qloggingcategory.h>
#if BOBUI_CONFIG(regularexpression)
#include <qregularexpression.h>
#endif
#include <qvarlengtharray.h>
#include <bobuihread.h>
#include <qcoreapplication.h>
#include <qmetaobject.h>

#include "bobuiexthtmlparser_p.h"
#include "qpainter.h"
#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include "qfont_p.h"
#include "private/qdataurl_p.h"

#include "bobuiextdocument_p.h"
#include <private/qabstracttextdocumentlayout_p.h>
#include "qpagedpaintdevice.h"
#include "private/qpagedpaintdevice_p.h"
#if BOBUI_CONFIG(textmarkdownreader)
#include <private/bobuiextmarkdownimporter_p.h>
#endif
#if BOBUI_CONFIG(textmarkdownwriter)
#include <private/bobuiextmarkdownwriter_p.h>
#endif

#include <limits.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

Q_CORE_EXPORT Q_DECL_CONST_FUNCTION unsigned int bobui_int_sqrt(unsigned int n);

namespace {
    BOBUIextDocument::ResourceProvider bobui_defaultResourceProvider;
}

QAbstractUndoItem::~QAbstractUndoItem()
    = default;

/*!
    \fn bool BobUI::mightBeRichText(QAnyStringView text)

    Returns \c true if the string \a text is likely to be rich text;
    otherwise returns \c false.

    This function uses a fast and therefore simple heuristic. It
    mainly checks whether there is something that looks like a tag
    before the first line break. Although the result may be correct
    for common cases, there is no guarantee.

    This function is defined in the \c <BOBUIextDocument> header file.

    \note In BobUI versions prior to 6.7, this function took QString only.
 */
template <typename T>
static bool mightBeRichTextImpl(T text)
{
    if (text.isEmpty())
        return false;
    qsizetype start = 0;

    while (start < text.size() && QChar(text.at(start)).isSpace())
        ++start;

    // skip a leading <?xml ... ?> as for example with xhtml
    if (text.mid(start, 5).compare("<?xml"_L1) == 0) {
        while (start < text.size()) {
            if (text.at(start) == u'?'
                && start + 2 < text.size()
                && text.at(start + 1) == u'>') {
                start += 2;
                break;
            }
            ++start;
        }

        while (start < text.size() && QChar(text.at(start)).isSpace())
            ++start;
    }

    if (text.mid(start, 5).compare("<!doc"_L1, BobUI::CaseInsensitive) == 0)
        return true;
    qsizetype open = start;
    while (open < text.size() && text.at(open) != u'<'
            && text.at(open) != u'\n') {
        if (text.at(open) == u'&' && text.mid(open + 1, 3) == "lt;"_L1)
            return true; // support desperate attempt of user to see <...>
        ++open;
    }
    if (open < text.size() && text.at(open) == u'<') {
        const qsizetype close = text.indexOf(u'>', open);
        if (close > -1) {
            QVarLengthArray<char16_t> tag;
            for (qsizetype i = open + 1; i < close; ++i) {
                const auto current = QChar(text[i]);
                if (current.isDigit() || current.isLetter())
                    tag.append(current.toLower().unicode());
                else if (!tag.isEmpty() && current.isSpace())
                    break;
                else if (!tag.isEmpty() && current == u'/' && i + 1 == close)
                    break;
                else if (!current.isSpace() && (!tag.isEmpty() || current != u'!'))
                    return false; // that's not a tag
            }
#ifndef BOBUI_NO_TEXTHTMLPARSER
            return BOBUIextHtmlParser::lookupElement(tag) != -1;
#else
            return false;
#endif // BOBUI_NO_TEXTHTMLPARSER
        }
    }
    return false;
}

static bool mightBeRichTextImpl(QUtf8StringView text)
{
    return mightBeRichTextImpl(QLatin1StringView(QByteArrayView(text)));
}

bool BobUI::mightBeRichText(QAnyStringView text)
{
    return text.visit([](auto text) { return mightBeRichTextImpl(text); });
}

/*!
    Converts the plain text string \a plain to an HTML-formatted
    paragraph while preserving most of its look.

    \a mode defines how whitespace is handled.

    This function is defined in the \c <BOBUIextDocument> header file.

    \sa QString::toHtmlEscaped(), mightBeRichText()
*/
QString BobUI::convertFromPlainText(const QString &plain, BobUI::WhiteSpaceMode mode)
{
    qsizetype col = 0;
    QString rich;
    rich += "<p>"_L1;
    for (qsizetype i = 0; i < plain.size(); ++i) {
        if (plain[i] == u'\n'){
            qsizetype c = 1;
            while (i+1 < plain.size() && plain[i+1] == u'\n') {
                i++;
                c++;
            }
            if (c == 1)
                rich += "<br>\n"_L1;
            else {
                rich += "</p>\n"_L1;
                while (--c > 1)
                    rich += "<br>\n"_L1;
                rich += "<p>"_L1;
            }
            col = 0;
        } else {
            if (mode == BobUI::WhiteSpacePre && plain[i] == u'\t'){
                rich += QChar::Nbsp;
                ++col;
                while (col % 8) {
                    rich += QChar::Nbsp;
                    ++col;
                }
            }
            else if (mode == BobUI::WhiteSpacePre && plain[i].isSpace())
                rich += QChar::Nbsp;
            else if (plain[i] == u'<')
                rich += "&lt;"_L1;
            else if (plain[i] == u'>')
                rich += "&gt;"_L1;
            else if (plain[i] == u'&')
                rich += "&amp;"_L1;
            else
                rich += plain[i];
            ++col;
        }
    }
    if (col != 0)
        rich += "</p>"_L1;
    return rich;
}

/*!
    \class BOBUIextDocument
    \reentrant
    \inmodule BobUIGui

    \brief The BOBUIextDocument class holds formatted text.

    \ingroup richtext-processing


    BOBUIextDocument is a container for structured rich text documents, providing
    support for styled text and various types of document elements, such as
    lists, tables, frames, and images.
    They can be created for use in a BOBUIextEdit, or used independently.

    Each document element is described by an associated format object. Each
    format object is treated as a unique object by BOBUIextDocuments, and can be
    passed to objectForFormat() to obtain the document element that it is
    applied to.

    A BOBUIextDocument can be edited programmatically using a BOBUIextCursor, and
    its contents can be examined by traversing the document structure. The
    entire document structure is stored as a hierarchy of document elements
    beneath the root frame, found with the rootFrame() function. Alternatively,
    if you just want to iterate over the textual contents of the document you
    can use begin(), end(), and findBlock() to retrieve text blocks that you
    can examine and iterate over.

    The layout of a document is determined by the documentLayout();
    you can create your own QAbstractTextDocumentLayout subclass and
    set it using setDocumentLayout() if you want to use your own
    layout logic. The document's title and other meta-information can be
    obtained by calling the metaInformation() function. For documents that
    are exposed to users through the BOBUIextEdit class, the document title
    is also available via the BOBUIextEdit::documentTitle() function.

    The toPlainText() and toHtml() convenience functions allow you to retrieve the
    contents of the document as plain text and HTML.
    The document's text can be searched using the find() functions.

    Undo/redo of operations performed on the document can be controlled using
    the setUndoRedoEnabled() function. The undo/redo system can be controlled
    by an editor widget through the undo() and redo() slots; the document also
    provides contentsChanged(), undoAvailable(), and redoAvailable() signals
    that inform connected editor widgets about the state of the undo/redo
    system. The following are the undo/redo operations of a BOBUIextDocument:

    \list
        \li Insertion or removal of characters. A sequence of insertions or removals
           within the same text block are regarded as a single undo/redo operation.
        \li Insertion or removal of text blocks. Sequences of insertion or removals
           in a single operation (e.g., by selecting and then deleting text) are
           regarded as a single undo/redo operation.
        \li Text character format changes.
        \li Text block format changes.
        \li Text block group format changes.
    \endlist

    \sa BOBUIextCursor, BOBUIextEdit, {Rich Text Processing}
*/

/*!
    \property BOBUIextDocument::defaultFont
    \brief the default font used to display the document's text
*/

/*!
    \property BOBUIextDocument::defaultTextOption
    \brief the default text option will be set on all \l{BOBUIextLayout}s in the document.

    When \l{BOBUIextBlock}s are created, the defaultTextOption is set on their
    BOBUIextLayout. This allows setting global properties for the document such as the
    default word wrap mode.
 */

/*!
    Constructs an empty BOBUIextDocument with the given \a parent.
*/
BOBUIextDocument::BOBUIextDocument(QObject *parent)
    : QObject(*new BOBUIextDocumentPrivate, parent)
{
    Q_D(BOBUIextDocument);
    d->init();
}

/*!
    Constructs a BOBUIextDocument containing the plain (unformatted) \a text
    specified, and with the given \a parent.
*/
BOBUIextDocument::BOBUIextDocument(const QString &text, QObject *parent)
    : QObject(*new BOBUIextDocumentPrivate, parent)
{
    Q_D(BOBUIextDocument);
    d->init();
    BOBUIextCursor(this).insertText(text);
}

/*!
    \internal
*/
BOBUIextDocument::BOBUIextDocument(BOBUIextDocumentPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(BOBUIextDocument);
    d->init();
}

/*!
    Destroys the document.
*/
BOBUIextDocument::~BOBUIextDocument()
{
}


/*!
  Creates a new BOBUIextDocument that is a copy of this text document. \a
  parent is the parent of the returned text document.
*/
BOBUIextDocument *BOBUIextDocument::clone(QObject *parent) const
{
    Q_D(const BOBUIextDocument);
    BOBUIextDocument *doc = new BOBUIextDocument(parent);
    if (isEmpty()) {
        const BOBUIextCursor thisCursor(const_cast<BOBUIextDocument *>(this));

        const auto blockFormat = thisCursor.blockFormat();
        if (blockFormat.isValid() && !blockFormat.isEmpty())
            BOBUIextCursor(doc).setBlockFormat(blockFormat);

        const auto blockCharFormat = thisCursor.blockCharFormat();
        if (blockCharFormat.isValid() && !blockCharFormat.isEmpty())
            BOBUIextCursor(doc).setBlockCharFormat(blockCharFormat);
    } else {
        BOBUIextCursor(doc).insertFragment(BOBUIextDocumentFragment(this));
    }
    doc->rootFrame()->setFrameFormat(rootFrame()->frameFormat());
    BOBUIextDocumentPrivate *priv = doc->d_func();
    priv->title = d->title;
    priv->url = d->url;
    priv->cssMedia = d->cssMedia;
    priv->pageSize = d->pageSize;
    priv->indentWidth = d->indentWidth;
    priv->defaultTextOption = d->defaultTextOption;
    priv->setDefaultFont(d->defaultFont());
    priv->resources = d->resources;
    priv->cachedResources.clear();
    priv->resourceProvider = d->resourceProvider;
#ifndef BOBUI_NO_CSSPARSER
    priv->defaultStyleSheet = d->defaultStyleSheet;
    priv->parsedDefaultStyleSheet = d->parsedDefaultStyleSheet;
#endif
    return doc;
}

/*!
    Returns \c true if the document is empty; otherwise returns \c false.
*/
bool BOBUIextDocument::isEmpty() const
{
    Q_D(const BOBUIextDocument);
    /* because if we're empty we still have one single paragraph as
     * one single fragment */
    return d->length() <= 1;
}

/*!
  Clears the document.
*/
void BOBUIextDocument::clear()
{
    Q_D(BOBUIextDocument);
    d->clear();
    d->resources.clear();
}

/*!
    \since 4.2

    Undoes the last editing operation on the document if undo is
    available. The provided \a cursor is positioned at the end of the
    location where the edition operation was undone.

    See the \l {Overview of BobUI's Undo Framework}{BobUI Undo Framework}
    documentation for details.

    \sa undoAvailable(), isUndoRedoEnabled()
*/
void BOBUIextDocument::undo(BOBUIextCursor *cursor)
{
    Q_D(BOBUIextDocument);
    const int pos = d->undoRedo(true);
    if (cursor && pos >= 0) {
        *cursor = BOBUIextCursor(this);
        cursor->setPosition(pos);
    }
}

/*!
    \since 4.2
    Redoes the last editing operation on the document if \l{BOBUIextDocument::isRedoAvailable()}{redo is available}.

    The provided \a cursor is positioned at the end of the location where
    the edition operation was redone.
*/
void BOBUIextDocument::redo(BOBUIextCursor *cursor)
{
    Q_D(BOBUIextDocument);
    const int pos = d->undoRedo(false);
    if (cursor && pos >= 0) {
        *cursor = BOBUIextCursor(this);
        cursor->setPosition(pos);
    }
}

/*! \enum BOBUIextDocument::Stacks

  \value UndoStack              The undo stack.
  \value RedoStack              The redo stack.
  \value UndoAndRedoStacks      Both the undo and redo stacks.
*/

/*!
    \since 4.7
    Clears the stacks specified by \a stacksToClear.

    This method clears any commands on the undo stack, the redo stack,
    or both (the default). If commands are cleared, the appropriate
    signals are emitted, BOBUIextDocument::undoAvailable() or
    BOBUIextDocument::redoAvailable().

    \sa BOBUIextDocument::undoAvailable(), BOBUIextDocument::redoAvailable()
*/
void BOBUIextDocument::clearUndoRedoStacks(Stacks stacksToClear)
{
    Q_D(BOBUIextDocument);
    d->clearUndoRedoStacks(stacksToClear, true);
}

/*!
    \overload

*/
void BOBUIextDocument::undo()
{
    Q_D(BOBUIextDocument);
    d->undoRedo(true);
}

/*!
    \overload
    Redoes the last editing operation on the document if \l{BOBUIextDocument::isRedoAvailable()}{redo is available}.
*/
void BOBUIextDocument::redo()
{
    Q_D(BOBUIextDocument);
    d->undoRedo(false);
}

/*!
    \internal

    Appends a custom undo \a item to the undo stack.
*/
void BOBUIextDocument::appendUndoItem(QAbstractUndoItem *item)
{
    Q_D(BOBUIextDocument);
    d->appendUndoItem(item);
}

/*!
    \property BOBUIextDocument::undoRedoEnabled
    \brief whether undo/redo are enabled for this document

    This defaults to true. If disabled, the undo stack is cleared and
    no items will be added to it.
*/
void BOBUIextDocument::setUndoRedoEnabled(bool enable)
{
    Q_D(BOBUIextDocument);
    d->enableUndoRedo(enable);
}

bool BOBUIextDocument::isUndoRedoEnabled() const
{
    Q_D(const BOBUIextDocument);
    return d->isUndoRedoEnabled();
}

/*!
    \property BOBUIextDocument::maximumBlockCount
    \since 4.2
    \brief Specifies the limit for blocks in the document.

    Specifies the maximum number of blocks the document may have. If there are
    more blocks in the document that specified with this property blocks are removed
    from the beginning of the document.

    A negative or zero value specifies that the document may contain an unlimited
    amount of blocks.

    The default value is 0.

    Note that setting this property will apply the limit immediately to the document
    contents.

    Setting this property also disables the undo redo history.

    This property is undefined in documents with tables or frames.
*/
int BOBUIextDocument::maximumBlockCount() const
{
    Q_D(const BOBUIextDocument);
    return d->maximumBlockCount;
}

void BOBUIextDocument::setMaximumBlockCount(int maximum)
{
    Q_D(BOBUIextDocument);
    d->maximumBlockCount = maximum;
    d->ensureMaximumBlockCount();
    setUndoRedoEnabled(false);
}

/*!
    \since 4.3

    The default text option is used on all BOBUIextLayout objects in the document.
    This allows setting global properties for the document such as the default
    word wrap mode.
*/
BOBUIextOption BOBUIextDocument::defaultTextOption() const
{
    Q_D(const BOBUIextDocument);
    return d->defaultTextOption;
}

/*!
    \since 4.3

    Sets the default text option to \a option.
*/
void BOBUIextDocument::setDefaultTextOption(const BOBUIextOption &option)
{
    Q_D(BOBUIextDocument);
    d->defaultTextOption = option;
    if (d->lout)
        d->lout->documentChanged(0, 0, d->length());
}

/*!
    \property BOBUIextDocument::baseUrl
    \since 5.3
    \brief the base URL used to resolve relative resource URLs within the document.

    Resource URLs are resolved to be within the same directory as the target of the base
    URL meaning any portion of the path after the last '/' will be ignored.

    \table
    \header \li Base URL \li Relative URL \li Resolved URL
    \row \li file:///path/to/content \li images/logo.png \li file:///path/to/images/logo.png
    \row \li file:///path/to/content/ \li images/logo.png \li file:///path/to/content/images/logo.png
    \row \li file:///path/to/content/index.html \li images/logo.png \li file:///path/to/content/images/logo.png
    \row \li file:///path/to/content/images/ \li ../images/logo.png \li file:///path/to/content/images/logo.png
    \endtable
*/
QUrl BOBUIextDocument::baseUrl() const
{
    Q_D(const BOBUIextDocument);
    return d->baseUrl;
}

void BOBUIextDocument::setBaseUrl(const QUrl &url)
{
    Q_D(BOBUIextDocument);
    if (d->baseUrl != url) {
        d->baseUrl = url;
        if (d->lout)
            d->lout->documentChanged(0, 0, d->length());
        emit baseUrlChanged(url);
    }
}

/*!
    \since 4.8

    The default cursor movement style is used by all BOBUIextCursor objects
    created from the document. The default is BobUI::LogicalMoveStyle.
*/
BobUI::CursorMoveStyle BOBUIextDocument::defaultCursorMoveStyle() const
{
    Q_D(const BOBUIextDocument);
    return d->defaultCursorMoveStyle;
}

/*!
    \since 4.8

    Sets the default cursor movement style to the given \a style.
*/
void BOBUIextDocument::setDefaultCursorMoveStyle(BobUI::CursorMoveStyle style)
{
    Q_D(BOBUIextDocument);
    d->defaultCursorMoveStyle = style;
}

/*!
    \fn void BOBUIextDocument::markContentsDirty(int position, int length)

    Marks the contents specified by the given \a position and \a length
    as "dirty", informing the document that it needs to be laid out
    again.
*/
void BOBUIextDocument::markContentsDirty(int from, int length)
{
    Q_D(BOBUIextDocument);
    d->documentChange(from, length);
    if (!d->inContentsChange) {
        if (d->lout) {
            d->lout->documentChanged(d->docChangeFrom, d->docChangeOldLength, d->docChangeLength);
            d->docChangeFrom = -1;
        }
    }
}

/*!
    \property BOBUIextDocument::useDesignMetrics
    \since 4.1
    \brief whether the document uses design metrics of fonts to improve the accuracy of text layout

    If this property is set to true, the layout will use design metrics.
    Otherwise, the metrics of the paint device as set on
    QAbstractTextDocumentLayout::setPaintDevice() will be used.

    Using design metrics makes a layout have a width that is no longer dependent on hinting
    and pixel-rounding. This means that WYSIWYG text layout becomes possible because the width
    scales much more linearly based on paintdevice metrics than it would otherwise.

    By default, this property is \c false.
*/

void BOBUIextDocument::setUseDesignMetrics(bool b)
{
    Q_D(BOBUIextDocument);
    if (b == d->defaultTextOption.useDesignMetrics())
        return;
    d->defaultTextOption.setUseDesignMetrics(b);
    if (d->lout)
        d->lout->documentChanged(0, 0, d->length());
}

bool BOBUIextDocument::useDesignMetrics() const
{
    Q_D(const BOBUIextDocument);
    return d->defaultTextOption.useDesignMetrics();
}

/*!
    \property BOBUIextDocument::layoutEnabled
    \since 6.4
    \brief whether BOBUIextDocument should recalculate the layout after every change

    If this property is set to true, any change to the document triggers a layout,
    which makes everything work as expected but takes time.

    Temporarily disabling the layout can save time when making multiple changes
    (not just text content, but also default font, default text option....)
    so that the document is only laid out once at the end. This can be useful when
    the text width or page size isn't yet known, for instance.

    By default, this property is \c true.

    \sa setTextWidth
*/

void BOBUIextDocument::setLayoutEnabled(bool b)
{
    Q_D(BOBUIextDocument);
    if (d->layoutEnabled == b)
        return;
    d->layoutEnabled = b;
    if (b && d->lout)
        d->lout->documentChanged(0, 0, d->length());
}

bool BOBUIextDocument::isLayoutEnabled() const
{
    Q_D(const BOBUIextDocument);
    return d->layoutEnabled;
}

/*!
    \since 4.2

    Draws the content of the document with painter \a p, clipped to \a rect.
    If \a rect is a null rectangle (default) then the document is painted unclipped.
*/
void BOBUIextDocument::drawContents(QPainter *p, const QRectF &rect)
{
    p->save();
    QAbstractTextDocumentLayout::PaintContext ctx;
    if (rect.isValid()) {
        p->setClipRect(rect);
        ctx.clip = rect;
    }
    documentLayout()->draw(p, ctx);
    p->restore();
}

/*!
    \property BOBUIextDocument::textWidth
    \since 4.2

    The text width specifies the preferred width for text in the document. If
    the text (or content in general) is wider than the specified with it is broken
    into multiple lines and grows vertically. If the text cannot be broken into multiple
    lines to fit into the specified text width it will be larger and the size() and the
    idealWidth() property will reflect that.

    If the text width is set to -1 then the text will not be broken into multiple lines
    unless it is enforced through an explicit line break or a new paragraph.

    The default value is -1.

    Setting the text width will also set the page height to -1, causing the document to
    grow or shrink vertically in a continuous way. If you want the document layout to break
    the text into multiple pages then you have to set the pageSize property instead.

    \sa size(), idealWidth(), pageSize()
*/
void BOBUIextDocument::setTextWidth(qreal width)
{
    Q_D(BOBUIextDocument);
    QSizeF sz = d->pageSize;

    qCDebug(lcLayout) << "page size" << sz << "-> width" << width;
    sz.setWidth(width);
    sz.setHeight(-1);
    setPageSize(sz);
}

qreal BOBUIextDocument::textWidth() const
{
    Q_D(const BOBUIextDocument);
    return d->pageSize.width();
}

/*!
    \since 4.2

    Returns the ideal width of the text document. The ideal width is the actually used width
    of the document without optional alignments taken into account. It is always <= size().width().

    \sa adjustSize(), textWidth
*/
qreal BOBUIextDocument::idealWidth() const
{
    if (BOBUIextDocumentLayout *lout = qobject_cast<BOBUIextDocumentLayout *>(documentLayout()))
        return lout->idealWidth();
    return textWidth();
}

/*!
    \property BOBUIextDocument::documentMargin
    \since 4.5

     The margin around the document. The default is 4.
*/
qreal BOBUIextDocument::documentMargin() const
{
    Q_D(const BOBUIextDocument);
    return d->documentMargin;
}

void BOBUIextDocument::setDocumentMargin(qreal margin)
{
    Q_D(BOBUIextDocument);
    if (d->documentMargin != margin) {
        d->documentMargin = margin;

        BOBUIextFrame* root = rootFrame();
        BOBUIextFrameFormat format = root->frameFormat();
        format.setMargin(margin);
        root->setFrameFormat(format);

        if (d->lout)
            d->lout->documentChanged(0, 0, d->length());
    }
}


/*!
    \property BOBUIextDocument::indentWidth
    \since 4.4

    Returns the width used for text list and text block indenting.

    The indent properties of BOBUIextListFormat and BOBUIextBlockFormat specify
    multiples of this value. The default indent width is 40.
*/
qreal BOBUIextDocument::indentWidth() const
{
    Q_D(const BOBUIextDocument);
    return d->indentWidth;
}


/*!
    \since 4.4

    Sets the \a width used for text list and text block indenting.

    The indent properties of BOBUIextListFormat and BOBUIextBlockFormat specify
    multiples of this value. The default indent width is 40 .

    \sa indentWidth()
*/
void BOBUIextDocument::setIndentWidth(qreal width)
{
    Q_D(BOBUIextDocument);
    if (d->indentWidth != width) {
        d->indentWidth = width;
        if (d->lout)
            d->lout->documentChanged(0, 0, d->length());
    }
}




/*!
    \since 4.2

    Adjusts the document to a reasonable size.

    \sa idealWidth(), textWidth, size
*/
void BOBUIextDocument::adjustSize()
{
    // Pull this private function in from qglobal.cpp
    QFont f = defaultFont();
    QFontMetrics fm(f);
    int mw =  fm.horizontalAdvance(u'x') * 80;
    int w = mw;
    setTextWidth(w);
    QSizeF size = documentLayout()->documentSize();
    if (size.width() != 0) {
        w = bobui_int_sqrt((uint)(5 * size.height() * size.width() / 3));
        setTextWidth(qMin(w, mw));

        size = documentLayout()->documentSize();
        if (w*3 < 5*size.height()) {
            w = bobui_int_sqrt((uint)(2 * size.height() * size.width()));
            setTextWidth(qMin(w, mw));
        }
    }
    setTextWidth(idealWidth());
}

/*!
    \property BOBUIextDocument::size
    \since 4.2

    \brief the actual size of the document.
    This is equivalent to documentLayout()->documentSize();

    The size of the document can be changed either by setting
    a text width or setting an entire page size.

    Note that the width is always >= pageSize().width().

    By default, for a newly-created, empty document, this property contains
    a configuration-dependent size.

    \sa setTextWidth(), setPageSize(), idealWidth()
*/
QSizeF BOBUIextDocument::size() const
{
    return documentLayout()->documentSize();
}

/*!
    \property BOBUIextDocument::blockCount
    \since 4.2

    \brief the number of text blocks in the document.

    The value of this property is undefined in documents with tables or frames.

    By default, if defined, this property contains a value of 1.
    \sa lineCount(), characterCount()
*/
int BOBUIextDocument::blockCount() const
{
    Q_D(const BOBUIextDocument);
    return d->blockMap().numNodes();
}


/*!
  \since 4.5

  Returns the number of lines of this document (if the layout supports
  this). Otherwise, this is identical to the number of blocks.

  \sa blockCount(), characterCount()
 */
int BOBUIextDocument::lineCount() const
{
    Q_D(const BOBUIextDocument);
    return d->blockMap().length(2);
}

/*!
  \since 4.5

  Returns the number of characters of this document.

  \note As a BOBUIextDocument always contains at least one
  QChar::ParagraphSeparator, this method will return at least 1.

  \sa blockCount(), characterAt()
 */
int BOBUIextDocument::characterCount() const
{
    Q_D(const BOBUIextDocument);
    return d->length();
}

/*!
  \since 4.5

  Returns the character at position \a pos, or a null character if the
  position is out of range.

  \sa characterCount()
 */
QChar BOBUIextDocument::characterAt(int pos) const
{
    Q_D(const BOBUIextDocument);
    if (pos < 0 || pos >= d->length())
        return QChar();
    BOBUIextDocumentPrivate::FragmentIterator fragIt = d->find(pos);
    const BOBUIextFragmentData * const frag = fragIt.value();
    const int offsetInFragment = qMax(0, pos - fragIt.position());
    return d->text.at(frag->stringPosition + offsetInFragment);
}


/*!
    \property BOBUIextDocument::defaultStyleSheet
    \since 4.2

    The default style sheet is applied to all newly HTML formatted text that is
    inserted into the document, for example using setHtml() or BOBUIextCursor::insertHtml().

    The style sheet needs to be compliant to CSS 2.1 syntax.

    \b{Note:} Changing the default style sheet does not have any effect to the existing content
    of the document.

    \sa {Supported HTML Subset}
*/

#ifndef BOBUI_NO_CSSPARSER
void BOBUIextDocument::setDefaultStyleSheet(const QString &sheet)
{
    Q_D(BOBUIextDocument);
    d->defaultStyleSheet = sheet;
    QCss::Parser parser(sheet);
    d->parsedDefaultStyleSheet = QCss::StyleSheet();
    d->parsedDefaultStyleSheet.origin = QCss::StyleSheetOrigin_UserAgent;
    parser.parse(&d->parsedDefaultStyleSheet);
}

QString BOBUIextDocument::defaultStyleSheet() const
{
    Q_D(const BOBUIextDocument);
    return d->defaultStyleSheet;
}
#endif // BOBUI_NO_CSSPARSER

/*!
    \fn void BOBUIextDocument::contentsChanged()

    This signal is emitted whenever the document's content changes; for
    example, when text is inserted or deleted, or when formatting is applied.

    \sa contentsChange()
*/

/*!
    \fn void BOBUIextDocument::contentsChange(int position, int charsRemoved, int charsAdded)

    This signal is emitted whenever the document's content changes; for
    example, when text is inserted or deleted, or when formatting is applied.

    Information is provided about the \a position of the character in the
    document where the change occurred, the number of characters removed
    (\a charsRemoved), and the number of characters added (\a charsAdded).

    The signal is emitted before the document's layout manager is notified
    about the change. This hook allows you to implement syntax highlighting
    for the document.

    \sa QAbstractTextDocumentLayout::documentChanged(), contentsChanged()
*/


/*!
    \fn void BOBUIextDocument::undoAvailable(bool available);

    This signal is emitted whenever undo operations become available
    (\a available is true) or unavailable (\a available is false).

    See the \l {Overview of BobUI's Undo Framework}{BobUI Undo Framework}
    documentation for details.

    \sa undo(), isUndoRedoEnabled()
*/

/*!
    \fn void BOBUIextDocument::redoAvailable(bool available);

    This signal is emitted whenever redo operations become available
    (\a available is true) or unavailable (\a available is false).
*/

/*!
    \fn void BOBUIextDocument::cursorPositionChanged(const BOBUIextCursor &cursor);

    This signal is emitted whenever the position of a cursor changed
    due to an editing operation. The cursor that changed is passed in
    \a cursor.  If the document is used with the BOBUIextEdit class and you need a signal when the
    cursor is moved with the arrow keys you can use the \l{BOBUIextEdit::}{cursorPositionChanged()}
    signal in BOBUIextEdit.
*/

/*!
    \fn void BOBUIextDocument::blockCountChanged(int newBlockCount);
    \since 4.3

    This signal is emitted when the total number of text blocks in the
    document changes. The value passed in \a newBlockCount is the new
    total.
*/

/*!
    \fn void BOBUIextDocument::documentLayoutChanged();
    \since 4.4

    This signal is emitted when a new document layout is set.

    \sa setDocumentLayout()

*/


/*!
    Returns \c true if undo is available; otherwise returns \c false.

    \sa isRedoAvailable(), availableUndoSteps()
*/
bool BOBUIextDocument::isUndoAvailable() const
{
    Q_D(const BOBUIextDocument);
    return d->isUndoAvailable();
}

/*!
    Returns \c true if redo is available; otherwise returns \c false.

    \sa isUndoAvailable(), availableRedoSteps()
*/
bool BOBUIextDocument::isRedoAvailable() const
{
    Q_D(const BOBUIextDocument);
    return d->isRedoAvailable();
}

/*! \since 4.6

    Returns the number of available undo steps.

    \sa isUndoAvailable()
*/
int BOBUIextDocument::availableUndoSteps() const
{
    Q_D(const BOBUIextDocument);
    return d->availableUndoSteps();
}

/*! \since 4.6

    Returns the number of available redo steps.

    \sa isRedoAvailable()
*/
int BOBUIextDocument::availableRedoSteps() const
{
    Q_D(const BOBUIextDocument);
    return d->availableRedoSteps();
}

/*! \since 4.4

    Returns the document's revision (if undo is enabled).

    The revision is guaranteed to increase when a document that is not
    modified is edited.

    \sa BOBUIextBlock::revision(), isModified()
 */
int BOBUIextDocument::revision() const
{
    Q_D(const BOBUIextDocument);
    return d->revision;
}



/*!
    Sets the document to use the given \a layout. The previous layout
    is deleted.

    \sa documentLayoutChanged()
*/
void BOBUIextDocument::setDocumentLayout(QAbstractTextDocumentLayout *layout)
{
    Q_D(BOBUIextDocument);
    d->setLayout(layout);
}

/*!
    Returns the document layout for this document.
*/
QAbstractTextDocumentLayout *BOBUIextDocument::documentLayout() const
{
    Q_D(const BOBUIextDocument);
    if (!d->lout) {
        BOBUIextDocument *that = const_cast<BOBUIextDocument *>(this);
        that->d_func()->setLayout(new BOBUIextDocumentLayout(that));
    }
    return d->lout;
}


/*!
    Returns meta information about the document of the type specified by
    \a info.

    \sa setMetaInformation()
*/
QString BOBUIextDocument::metaInformation(MetaInformation info) const
{
    Q_D(const BOBUIextDocument);
    switch (info) {
    case DocumentTitle:
        return d->title;
    case DocumentUrl:
        return d->url;
    case CssMedia:
        return d->cssMedia;
    case FrontMatter:
        return d->frontMatter;
    }
    return QString();
}

/*!
    Sets the document's meta information of the type specified by \a info
    to the given \a string.

    \sa metaInformation()
*/
void BOBUIextDocument::setMetaInformation(MetaInformation info, const QString &string)
{
    Q_D(BOBUIextDocument);
    switch (info) {
    case DocumentTitle:
        d->title = string;
        break;
    case DocumentUrl:
        d->url = string;
        break;
    case CssMedia:
        d->cssMedia = string;
        break;
    case FrontMatter:
        d->frontMatter = string;
        break;
    }
}

/*!
    Returns the raw text contained in the document without any
    formatting information. If you want formatting information
    use a BOBUIextCursor instead.

    \since 5.9
    \sa toPlainText()
*/
QString BOBUIextDocument::toRawText() const
{
    Q_D(const BOBUIextDocument);
    return d->plainText();
}

/*!
    Returns the plain text contained in the document. If you want
    formatting information use a BOBUIextCursor instead.

    This function returns the same as toRawText(), but will replace
    some unicode characters with ASCII alternatives.
    In particular, no-break space (U+00A0) is replaced by a regular
    space (U+0020), and both paragraph (U+2029) and line (U+2028)
    separators are replaced by line feed (U+000A).
    If you need the precise contents of the document, use toRawText()
    instead.

    \note Embedded objects, such as images, are represented by a
    Unicode value U+FFFC (OBJECT REPLACEMENT CHARACTER).

    \sa toHtml()
*/
QString BOBUIextDocument::toPlainText() const
{
    Q_D(const BOBUIextDocument);
    QString txt = d->plainText();

    constexpr char16_t delims[] = { 0xfdd0, 0xfdd1,
                                    QChar::ParagraphSeparator, QChar::LineSeparator, QChar::Nbsp };

    const size_t pos = std::u16string_view(txt).find_first_of(
                              std::u16string_view(delims, std::size(delims)));
    if (pos == std::u16string_view::npos)
        return txt;

    QChar *uc = txt.data();
    QChar *const e = uc + txt.size();

    for (uc += pos; uc != e; ++uc) {
        switch (uc->unicode()) {
        case 0xfdd0: // BOBUIextBeginningOfFrame
        case 0xfdd1: // BOBUIextEndOfFrame
        case QChar::ParagraphSeparator:
        case QChar::LineSeparator:
            *uc = u'\n';
            break;
        case QChar::Nbsp:
            *uc = u' ';
            break;
        default:
            ;
        }
    }
    return txt;
}

/*!
    Replaces the entire contents of the document with the given plain
    \a text. The undo/redo history is reset when this function is called.

    \sa setHtml()
*/
void BOBUIextDocument::setPlainText(const QString &text)
{
    Q_D(BOBUIextDocument);
    bool previousState = d->isUndoRedoEnabled();
    d->enableUndoRedo(false);
    d->beginEditBlock();
    d->clear();
    BOBUIextCursor(this).insertText(text);
    d->endEditBlock();
    d->enableUndoRedo(previousState);
}

/*!
    Replaces the entire contents of the document with the given
    HTML-formatted text in the \a html string. The undo/redo history
    is reset when this function is called.

    The HTML formatting is respected as much as possible; for example,
    "<b>bold</b> text" will produce text where the first word has a font
    weight that gives it a bold appearance: "\b{bold} text".

    To select a css media rule other than the default "screen" rule,
    use setMetaInformation() with 'CssMedia' as "info" parameter.

    \note It is the responsibility of the caller to make sure that the
    text is correctly decoded when a QString containing HTML is created
    and passed to setHtml().

    \sa setPlainText(), {Supported HTML Subset}, setMetaInformation()
*/

#ifndef BOBUI_NO_TEXTHTMLPARSER

void BOBUIextDocument::setHtml(const QString &html)
{
    Q_D(BOBUIextDocument);
    bool previousState = d->isUndoRedoEnabled();
    d->enableUndoRedo(false);
    d->beginEditBlock();
    d->clear();
    // ctor calls parse() to build up BOBUIextHtmlParser::nodes list
    // then import() populates the BOBUIextDocument from those
    BOBUIextHtmlImporter(this, html, BOBUIextHtmlImporter::ImportToDocument).import();
    d->endEditBlock();
    d->enableUndoRedo(previousState);
}

#endif // BOBUI_NO_TEXTHTMLPARSER

/*!
    \enum BOBUIextDocument::FindFlag

    This enum describes the options available to BOBUIextDocument's find function. The options
    can be OR-ed together from the following list:

    \value FindBackward Search backwards instead of forwards.
    \value FindCaseSensitively By default find works case insensitive. Specifying this option
    changes the behaviour to a case sensitive find operation.
    \value FindWholeWords Makes find match only complete words.
*/

/*!
    \enum BOBUIextDocument::MetaInformation

    This enum describes the different types of meta information that can be
    added to a document.

    \value DocumentTitle    The title of the document.
    \value DocumentUrl      The url of the document. The loadResource() function uses
                            this url as the base when loading relative resources.
    \value CssMedia         This value is used to select the corresponding '@media'
                            rule, if any, from a specified CSS stylesheet when setHtml()
                            is called. This enum value has been introduced in BobUI 6.3.
    \value FrontMatter      This value is used to select header material, if any was
                            extracted during parsing of the source file (currently
                            only from Markdown format). This enum value has been
                            introduced in BobUI 6.8.

    \sa metaInformation(), setMetaInformation(), setHtml()
*/

static bool findInBlock(const BOBUIextBlock &block, const QString &expression, int offset,
                        BOBUIextDocument::FindFlags options, BOBUIextCursor *cursor)
{
    QString text = block.text();
    text.replace(QChar::Nbsp, u' ');
    BobUI::CaseSensitivity sensitivity = options & BOBUIextDocument::FindCaseSensitively ? BobUI::CaseSensitive : BobUI::CaseInsensitive;
    int idx = -1;

    while (offset >= 0 && offset <= text.size()) {
        idx = (options & BOBUIextDocument::FindBackward) ?
               text.lastIndexOf(expression, offset, sensitivity) : text.indexOf(expression, offset, sensitivity);
        if (idx == -1)
            return false;

        if (options & BOBUIextDocument::FindWholeWords) {
            const int start = idx;
            const int end = start + expression.size();
            if ((start != 0 && text.at(start - 1).isLetterOrNumber())
                || (end != text.size() && text.at(end).isLetterOrNumber())) {
                //if this is not a whole word, continue the search in the string
                offset = (options & BOBUIextDocument::FindBackward) ? idx-1 : end+1;
                idx = -1;
                continue;
            }
        }
        //we have a hit, return the cursor for that.
        *cursor = BOBUIextCursorPrivate::fromPosition(const_cast<BOBUIextDocumentPrivate *>(BOBUIextDocumentPrivate::get(block)),
                                                   block.position() + idx);
        cursor->setPosition(cursor->position() + expression.size(), BOBUIextCursor::KeepAnchor);
        return true;
    }
    return false;
}

/*!
    \fn BOBUIextCursor BOBUIextDocument::find(const QString &subString, int position, FindFlags options) const

    \overload

    Finds the next occurrence of the string, \a subString, in the document.
    The search starts at the given \a position, and proceeds forwards
    through the document unless specified otherwise in the search options.
    The \a options control the type of search performed.

    Returns a cursor with the match selected if \a subString
    was found; otherwise returns a null cursor.

    If the \a position is 0 (the default) the search begins from the beginning
    of the document; otherwise it begins at the specified position.
*/
BOBUIextCursor BOBUIextDocument::find(const QString &subString, int from, FindFlags options) const
{
    Q_D(const BOBUIextDocument);

    if (subString.isEmpty())
        return BOBUIextCursor();

    int pos = from;
    //the cursor is positioned between characters, so for a backward search
    //do not include the character given in the position.
    if (options & FindBackward) {
        --pos ;
        if (pos < 0)
            return BOBUIextCursor();
    }

    BOBUIextCursor cursor;
    BOBUIextBlock block = d->blocksFind(pos);
    int blockOffset = pos - block.position();

    if (!(options & FindBackward)) {
        while (block.isValid()) {
            if (findInBlock(block, subString, blockOffset, options, &cursor))
                return cursor;
            block = block.next();
            blockOffset = 0;
        }
    } else {
        if (blockOffset == block.length() - 1)
            --blockOffset;  // make sure to skip end-of-paragraph character
        while (block.isValid()) {
            if (findInBlock(block, subString, blockOffset, options, &cursor))
                return cursor;
            block = block.previous();
            blockOffset = block.length() - 2;
        }
    }

    return BOBUIextCursor();
}

/*!
    Finds the next occurrence of the string, \a subString, in the document.
    The search starts at the position of the given \a cursor, and proceeds
    forwards through the document unless specified otherwise in the search
    options. The \a options control the type of search performed.

    Returns a cursor with the match selected if \a subString was found; otherwise
    returns a null cursor.

    If the given \a cursor has a selection, the search begins after the
    selection; otherwise it begins at the cursor's position.

    By default the search is case insensitive, and can match text anywhere in the
    document.
*/
BOBUIextCursor BOBUIextDocument::find(const QString &subString, const BOBUIextCursor &cursor, FindFlags options) const
{
    int pos = 0;
    if (!cursor.isNull()) {
        if (options & BOBUIextDocument::FindBackward)
            pos = cursor.selectionStart();
        else
            pos = cursor.selectionEnd();
    }

    return find(subString, pos, options);
}

#if BOBUI_CONFIG(regularexpression)
static bool findInBlock(const BOBUIextBlock &block, const QRegularExpression &expr, int offset,
                        BOBUIextDocument::FindFlags options, BOBUIextCursor *cursor)
{
    QString text = block.text();
    text.replace(QChar::Nbsp, u' ');
    QRegularExpressionMatch match;
    int idx = -1;

    while (offset >= 0 && offset <= text.size()) {
        idx = (options & BOBUIextDocument::FindBackward) ?
               text.lastIndexOf(expr, offset, &match) : text.indexOf(expr, offset, &match);
        if (idx == -1)
            return false;

        if (options & BOBUIextDocument::FindWholeWords) {
            const int start = idx;
            const int end = start + match.capturedLength();
            if ((start != 0 && text.at(start - 1).isLetterOrNumber())
                || (end != text.size() && text.at(end).isLetterOrNumber())) {
                //if this is not a whole word, continue the search in the string
                offset = (options & BOBUIextDocument::FindBackward) ? idx-1 : end+1;
                idx = -1;
                continue;
            }
        }
        //we have a hit, return the cursor for that.
        *cursor = BOBUIextCursorPrivate::fromPosition(const_cast<BOBUIextDocumentPrivate *>(BOBUIextDocumentPrivate::get(block)),
                                                   block.position() + idx);
        cursor->setPosition(cursor->position() + match.capturedLength(), BOBUIextCursor::KeepAnchor);
        return true;
    }
    return false;
}

/*!
    \since 5.5

    Finds the next occurrence that matches the given regular expression,
    \a expr, within the same paragraph in the document.

    The search starts at the given \a from position, and proceeds forwards
    through the document unless specified otherwise in the search options.
    The \a options control the type of search performed.

    Returns a cursor with the match selected if a match was found; otherwise
    returns a null cursor.

    If the \a from position is 0 (the default) the search begins from the beginning
    of the document; otherwise it begins at the specified position.

    \warning For historical reasons, the case sensitivity option set on
    \a expr is ignored. Instead, the \a options are used to determine
    if the search is case sensitive or not.
*/
BOBUIextCursor BOBUIextDocument::find(const QRegularExpression &expr, int from, FindFlags options) const
{
    Q_D(const BOBUIextDocument);

    if (!expr.isValid())
        return BOBUIextCursor();

    int pos = from;
    //the cursor is positioned between characters, so for a backward search
    //do not include the character given in the position.
    if (options & FindBackward) {
        --pos ;
        if (pos < 0)
            return BOBUIextCursor();
    }

    BOBUIextCursor cursor;
    BOBUIextBlock block = d->blocksFind(pos);
    int blockOffset = pos - block.position();

    QRegularExpression expression(expr);
    if (!(options & BOBUIextDocument::FindCaseSensitively))
        expression.setPatternOptions(expr.patternOptions() | QRegularExpression::CaseInsensitiveOption);
    else
        expression.setPatternOptions(expr.patternOptions() & ~QRegularExpression::CaseInsensitiveOption);

    if (!(options & FindBackward)) {
        while (block.isValid()) {
            if (findInBlock(block, expression, blockOffset, options, &cursor))
                return cursor;
            block = block.next();
            blockOffset = 0;
        }
    } else {
        while (block.isValid()) {
            if (findInBlock(block, expression, blockOffset, options, &cursor))
                return cursor;
            block = block.previous();
            blockOffset = block.length() - 1;
        }
    }

    return BOBUIextCursor();
}

/*!
    \since 5.5

    Finds the next occurrence that matches the given regular expression,
    \a expr, within the same paragraph in the document.

    The search starts at the position of the given \a cursor, and proceeds
    forwards through the document unless specified otherwise in the search
    options. The \a options control the type of search performed.

    Returns a cursor with the match selected if a match was found; otherwise
    returns a null cursor.

    If the given \a cursor has a selection, the search begins after the
    selection; otherwise it begins at the cursor's position.

    By default the search is case insensitive, and can match text anywhere in the
    document.
*/
BOBUIextCursor BOBUIextDocument::find(const QRegularExpression &expr, const BOBUIextCursor &cursor, FindFlags options) const
{
    int pos = 0;
    if (!cursor.isNull()) {
        if (options & BOBUIextDocument::FindBackward)
            pos = cursor.selectionStart();
        else
            pos = cursor.selectionEnd();
    }
    return find(expr, pos, options);
}
#endif // BOBUI_CONFIG(regularexpression)

/*!
    \fn BOBUIextObject *BOBUIextDocument::createObject(const BOBUIextFormat &format)

    Creates and returns a new document object (a BOBUIextObject), based
    on the given \a format.

    BOBUIextObjects will always get created through this method, so you
    must reimplement it if you use custom text objects inside your document.
*/
BOBUIextObject *BOBUIextDocument::createObject(const BOBUIextFormat &f)
{
    BOBUIextObject *obj = nullptr;
    if (f.isListFormat())
        obj = new BOBUIextList(this);
    else if (f.isTableFormat())
        obj = new BOBUIextTable(this);
    else if (f.isFrameFormat())
        obj = new BOBUIextFrame(this);

    return obj;
}

/*!
    \internal

    Returns the frame that contains the text cursor position \a pos.
*/
BOBUIextFrame *BOBUIextDocument::frameAt(int pos) const
{
    Q_D(const BOBUIextDocument);
    return d->frameAt(pos);
}

/*!
    Returns the document's root frame.
*/
BOBUIextFrame *BOBUIextDocument::rootFrame() const
{
    Q_D(const BOBUIextDocument);
    return d->rootFrame();
}

/*!
    Returns the text object associated with the given \a objectIndex.
*/
BOBUIextObject *BOBUIextDocument::object(int objectIndex) const
{
    Q_D(const BOBUIextDocument);
    return d->objectForIndex(objectIndex);
}

/*!
    Returns the text object associated with the format \a f.
*/
BOBUIextObject *BOBUIextDocument::objectForFormat(const BOBUIextFormat &f) const
{
    Q_D(const BOBUIextDocument);
    return d->objectForFormat(f);
}


/*!
    Returns the text block that contains the \a{pos}-th character.
*/
BOBUIextBlock BOBUIextDocument::findBlock(int pos) const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), d->blockMap().findNode(pos));
}

/*!
    \since 4.4
    Returns the text block with the specified \a blockNumber.

    \sa BOBUIextBlock::blockNumber()
*/
BOBUIextBlock BOBUIextDocument::findBlockByNumber(int blockNumber) const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), d->blockMap().findNode(blockNumber, 1));
}

/*!
    \since 4.5
    Returns the text block that contains the specified \a lineNumber.

    \sa BOBUIextBlock::firstLineNumber()
*/
BOBUIextBlock BOBUIextDocument::findBlockByLineNumber(int lineNumber) const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), d->blockMap().findNode(lineNumber, 2));
}

/*!
    Returns the document's first text block.

    \sa firstBlock()
*/
BOBUIextBlock BOBUIextDocument::begin() const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), d->blockMap().begin().n);
}

/*!
    This function returns a block to test for the end of the document
    while iterating over it.

    \snippet textdocument-end/textdocumentendsnippet.cpp 0

    The block returned is invalid and represents the block after the
    last block in the document. You can use lastBlock() to retrieve the
    last valid block of the document.

    \sa lastBlock()
*/
BOBUIextBlock BOBUIextDocument::end() const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), 0);
}

/*!
    \since 4.4
    Returns the document's first text block.
*/
BOBUIextBlock BOBUIextDocument::firstBlock() const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), d->blockMap().begin().n);
}

/*!
    \since 4.4
    Returns the document's last (valid) text block.
*/
BOBUIextBlock BOBUIextDocument::lastBlock() const
{
    Q_D(const BOBUIextDocument);
    return BOBUIextBlock(const_cast<BOBUIextDocumentPrivate *>(d), d->blockMap().last().n);
}

/*!
    \property BOBUIextDocument::pageSize
    \brief the page size that should be used for laying out the document

    The units are determined by the underlying paint device. The size is
    measured in logical pixels when painting to the screen, and in points
    (1/72 inch) when painting to a printer.

    By default, for a newly-created, empty document, this property contains
    an undefined size.

    \sa modificationChanged()
*/

void BOBUIextDocument::setPageSize(const QSizeF &size)
{
    Q_D(BOBUIextDocument);
    d->pageSize = size;
    if (d->lout)
        d->lout->documentChanged(0, 0, d->length());
}

QSizeF BOBUIextDocument::pageSize() const
{
    Q_D(const BOBUIextDocument);
    return d->pageSize;
}

/*!
  returns the number of pages in this document.
*/
int BOBUIextDocument::pageCount() const
{
    return documentLayout()->pageCount();
}

/*!
    Sets the default \a font to use in the document layout.
*/
void BOBUIextDocument::setDefaultFont(const QFont &font)
{
    Q_D(BOBUIextDocument);
    d->setDefaultFont(font);
    if (d->lout)
        d->lout->documentChanged(0, 0, d->length());
}

/*!
    Returns the default font to be used in the document layout.
*/
QFont BOBUIextDocument::defaultFont() const
{
    Q_D(const BOBUIextDocument);
    return d->defaultFont();
}

/*!
    \fn void BOBUIextDocument::setSuperScriptBaseline(qreal baseline)
    \since 6.0

    Sets the default superscript's base line as a % of font height to use in the document
    layout to \a baseline. The default value is 50% (1/2 of height).

    \sa superScriptBaseline(), setSubScriptBaseline(), subScriptBaseline(), setBaselineOffset(), baselineOffset()
*/
void BOBUIextDocument::setSuperScriptBaseline(qreal baseline)
{
    Q_D(BOBUIextDocument);
    d->formats.setSuperScriptBaseline(baseline);
}

/*!
    \fn qreal BOBUIextDocument::superScriptBaseline() const
    \since 6.0

    Returns the superscript's base line as a % of font height used in the document layout.

    \sa setSuperScriptBaseline(), setSubScriptBaseline(), subScriptBaseline(), setBaselineOffset(), baselineOffset()
*/
qreal BOBUIextDocument::superScriptBaseline() const
{
    Q_D(const BOBUIextDocument);
    return d->formats.defaultTextFormat().superScriptBaseline();
}

/*!
    \fn void BOBUIextDocument::setSubScriptBaseline(qreal baseline)
    \since 6.0

    Sets the default subscript's base line as a % of font height to use in the document layout
    to \a baseline. The default value is 16.67% (1/6 of height).

    \sa subScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline(), setBaselineOffset(), baselineOffset()
*/
void BOBUIextDocument::setSubScriptBaseline(qreal baseline)
{
    Q_D(BOBUIextDocument);
    d->formats.setSubScriptBaseline(baseline);
}

/*!
    \fn qreal BOBUIextDocument::subScriptBaseline() const
    \since 6.0

    Returns the superscript's base line as a % of font height used in the document layout.

    \sa setSubScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline(), setBaselineOffset(), baselineOffset()
*/
qreal BOBUIextDocument::subScriptBaseline() const
{
    Q_D(const BOBUIextDocument);
    return d->formats.defaultTextFormat().subScriptBaseline();
}

/*!
    \fn void BOBUIextDocument::setBaselineOffset(qreal baseline)
    \since 6.0

    Sets the base line as a% of font height to use in the document layout to \a baseline.
    The default value is 0.
    A positive value moves up the text, by the corresponding %; a negative value moves it down.

    \sa baselineOffset(), setSubScriptBaseline(), subScriptBaseline(), setSuperScriptBaseline(), superScriptBaseline()
*/
void BOBUIextDocument::setBaselineOffset(qreal baseline)
{
    Q_D(BOBUIextDocument);
    d->formats.setBaselineOffset(baseline);
}

/*!
    \fn qreal BOBUIextDocument::baselineOffset() const
    \since 6.0

    Returns the baseline offset in % used in the document layout.

    \sa setBaselineOffset(), setSubScriptBaseline(), subScriptBaseline(), setSuperScriptBaseline(),
   superScriptBaseline()
*/
qreal BOBUIextDocument::baselineOffset() const
{
    Q_D(const BOBUIextDocument);
    return d->formats.defaultTextFormat().baselineOffset();
}

/*!
    \fn void BOBUIextDocument::modificationChanged(bool changed)

    This signal is emitted whenever the content of the document
    changes in a way that affects the modification state. If \a
    changed is true, the document has been modified; otherwise it is
    false.

    For example, calling setModified(false) on a document and then
    inserting text causes the signal to get emitted. If you undo that
    operation, causing the document to return to its original
    unmodified state, the signal will get emitted again.
*/

/*!
    \property BOBUIextDocument::modified
    \brief whether the document has been modified by the user

    By default, this property is \c false.

    \sa modificationChanged()
*/

bool BOBUIextDocument::isModified() const
{
    Q_D(const BOBUIextDocument);
    return d->isModified();
}

void BOBUIextDocument::setModified(bool m)
{
    Q_D(BOBUIextDocument);
    d->setModified(m);
}

#ifndef BOBUI_NO_PRINTER
static void printPage(int index, QPainter *painter, const BOBUIextDocument *doc, const QRectF &body, const QPointF &pageNumberPos)
{
    painter->save();
    painter->translate(body.left(), body.top() - (index - 1) * body.height());
    QRectF view(0, (index - 1) * body.height(), body.width(), body.height());

    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    QAbstractTextDocumentLayout::PaintContext ctx;

    painter->setClipRect(view);
    ctx.clip = view;

    // don't use the system palette text as default text color, on HP/UX
    // for example that's white, and white text on white paper doesn't
    // look that nice
    ctx.palette.setColor(QPalette::Text, BobUI::black);

    layout->draw(painter, ctx);

    if (!pageNumberPos.isNull()) {
        painter->setClipping(false);
        painter->setFont(QFont(doc->defaultFont()));
        const QString pageString = QString::number(index);

        painter->drawText(qRound(pageNumberPos.x() - painter->fontMetrics().horizontalAdvance(pageString)),
                          qRound(pageNumberPos.y() + view.top()),
                          pageString);
    }

    painter->restore();
}

/*!
    Prints the document to the given \a printer. The QPagedPaintDevice must be
    set up before being used with this function.

    This is only a convenience method to print the whole document to the printer.

    If the document is already paginated through a specified height in the pageSize()
    property it is printed as-is.

    If the document is not paginated, like for example a document used in a BOBUIextEdit,
    then a temporary copy of the document is created and the copy is broken into
    multiple pages according to the size of the paint device's paperRect(). By default
    a 2 cm margin is set around the document contents. In addition the current page
    number is printed at the bottom of each page.

    \sa BOBUIextEdit::print()
*/

void BOBUIextDocument::print(QPagedPaintDevice *printer) const
{
    Q_D(const BOBUIextDocument);

    if (!printer)
        return;

    bool documentPaginated = d->pageSize.isValid() && !d->pageSize.isNull()
                             && d->pageSize.height() != qreal(INT_MAX);

    // ### set page size to paginated size?
    QMarginsF m = printer->pageLayout().margins(QPageLayout::Millimeter);
    if (!documentPaginated && m.left() == 0. && m.right() == 0. && m.top() == 0. && m.bottom() == 0.) {
        m.setLeft(2);
        m.setRight(2);
        m.setTop(2);
        m.setBottom(2);
        printer->setPageMargins(m, QPageLayout::Millimeter);
    }
    // ### use the margins correctly

    QPainter p(printer);

    // Check that there is a valid device to print to.
    if (!p.isActive())
        return;

    const BOBUIextDocument *doc = this;
    QScopedPointer<BOBUIextDocument> clonedDoc;
    (void)doc->documentLayout(); // make sure that there is a layout

    QRectF body = QRectF(QPointF(0, 0), d->pageSize);
    QPointF pageNumberPos;

    qreal sourceDpiX = bobui_defaultDpiX();
    qreal sourceDpiY = bobui_defaultDpiY();
    const qreal dpiScaleX = qreal(printer->logicalDpiX()) / sourceDpiX;
    const qreal dpiScaleY = qreal(printer->logicalDpiY()) / sourceDpiY;

    if (documentPaginated) {

        QPaintDevice *dev = doc->documentLayout()->paintDevice();
        if (dev) {
            sourceDpiX = dev->logicalDpiX();
            sourceDpiY = dev->logicalDpiY();
        }

        // scale to dpi
        p.scale(dpiScaleX, dpiScaleY);

        QSizeF scaledPageSize = d->pageSize;
        scaledPageSize.rwidth() *= dpiScaleX;
        scaledPageSize.rheight() *= dpiScaleY;

        const QSizeF printerPageSize(printer->width(), printer->height());

        // scale to page
        p.scale(printerPageSize.width() / scaledPageSize.width(),
                printerPageSize.height() / scaledPageSize.height());
    } else {
        doc = clone(const_cast<BOBUIextDocument *>(this));
        clonedDoc.reset(const_cast<BOBUIextDocument *>(doc));

        for (BOBUIextBlock srcBlock = firstBlock(), dstBlock = clonedDoc->firstBlock();
             srcBlock.isValid() && dstBlock.isValid();
             srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
            dstBlock.layout()->setFormats(srcBlock.layout()->formats());
        }

        QAbstractTextDocumentLayout *layout = doc->documentLayout();
        layout->setPaintDevice(p.device());

        // copy the custom object handlers
        layout->d_func()->handlers = documentLayout()->d_func()->handlers;

        // 2 cm margins, scaled to device in BOBUIextDocumentLayoutPrivate::layoutFrame
        const int horizontalMargin = int((2/2.54)*sourceDpiX);
        const int verticalMargin = int((2/2.54)*sourceDpiY);
        BOBUIextFrameFormat fmt = doc->rootFrame()->frameFormat();
        fmt.setLeftMargin(horizontalMargin);
        fmt.setRightMargin(horizontalMargin);
        fmt.setTopMargin(verticalMargin);
        fmt.setBottomMargin(verticalMargin);
        doc->rootFrame()->setFrameFormat(fmt);

        // pageNumberPos must be in device coordinates, so scale to device here
        const int dpiy = p.device()->logicalDpiY();
        body = QRectF(0, 0, printer->width(), printer->height());
        pageNumberPos = QPointF(body.width() - horizontalMargin * dpiScaleX,
                                body.height() - verticalMargin * dpiScaleY
                                + QFontMetrics(doc->defaultFont(), p.device()).ascent()
                                + 5 * dpiy / 72.0);
        clonedDoc->setPageSize(body.size());
    }

    const QPageRanges pageRanges = printer->pageRanges();
    int fromPage = pageRanges.firstPage();
    int toPage = pageRanges.lastPage();

    if (fromPage == 0 && toPage == 0) {
        fromPage = 1;
        toPage = doc->pageCount();
    }
    // paranoia check
    fromPage = qMax(1, fromPage);
    toPage = qMin(doc->pageCount(), toPage);

    if (toPage < fromPage) {
        // if the user entered a page range outside the actual number
        // of printable pages, just return
        return;
    }

//    bool ascending = true;
//    if (printer->pageOrder() == QPrinter::LastPageFirst) {
//        int tmp = fromPage;
//        fromPage = toPage;
//        toPage = tmp;
//        ascending = false;
//    }

    int page = fromPage;
    while (true) {
        if (pageRanges.isEmpty() || pageRanges.contains(page))
            printPage(page, &p, doc, body, pageNumberPos);

        if (page == toPage)
            break;
        ++page;
        if (!printer->newPage())
            return;
    }
}
#endif

/*!
    \enum BOBUIextDocument::ResourceType

    This enum describes the types of resources that can be loaded by
    BOBUIextDocument's loadResource() function or by BOBUIextBrowser::setSource().

    \value UnknownResource No resource is loaded, or the resource type is not known.
    \value HtmlResource  The resource contains HTML.
    \value ImageResource The resource contains image data.
                         Currently supported data types are QMetaType::QPixmap and
                         QMetaType::QImage. If the corresponding variant is of type
                         QMetaType::QByteArray then BobUI attempts to load the image using
                         QImage::loadFromData. QMetaType::QIcon is currently not supported.
                         The icon needs to be converted to one of the supported types first,
                         for example using QIcon::pixmap.
    \value StyleSheetResource The resource contains CSS.
    \value MarkdownResource The resource contains Markdown.
    \value UserResource  The first available value for user defined
                         resource types.

    \sa loadResource(), BOBUIextBrowser::sourceType()
*/

/*!
    Returns data of the specified \a type from the resource with the
    given \a name.

    This function is called by the rich text engine to request data that isn't
    directly stored by BOBUIextDocument, but still associated with it. For example,
    images are referenced indirectly by the name attribute of a BOBUIextImageFormat
    object.

    Resources are cached internally in the document. If a resource can
    not be found in the cache, loadResource is called to try to load
    the resource. loadResource should then use addResource to add the
    resource to the cache.

    If loadResource does not load the resource, then the resourceProvider and
    lastly the defaultResourceProvider will be called, if set. Note that the
    result from the provider will not be added automatically to the cache.

    \sa BOBUIextDocument::ResourceType, resourceProvider()
*/
QVariant BOBUIextDocument::resource(int type, const QUrl &name) const
{
    Q_D(const BOBUIextDocument);
    const QUrl url = d->baseUrl.resolved(name);
    QVariant r = d->resources.value(url);
    if (!r.isValid()) {
        r = d->cachedResources.value(url);
        if (!r.isValid()) {
            r = const_cast<BOBUIextDocument *>(this)->loadResource(type, url);
            if (!r.isValid()) {
                if (d->resourceProvider)
                    r = d->resourceProvider(url);
                else if (auto defaultProvider = defaultResourceProvider())
                    r = defaultProvider(url);
            }
        }
    }
    return r;
}

/*!
    Adds the resource \a resource to the resource cache, using \a
    type and \a name as identifiers. \a type should be a value from
    BOBUIextDocument::ResourceType.

    For example, you can add an image as a resource in order to reference it
    from within the document:

    \snippet textdocument-resources/main.cpp Adding a resource

    The image can be inserted into the document using the BOBUIextCursor API:

    \snippet textdocument-resources/main.cpp Inserting an image with a cursor

    Alternatively, you can insert images using the HTML \c img tag:

    \snippet textdocument-resources/main.cpp Inserting an image using HTML
*/
void BOBUIextDocument::addResource(int type, const QUrl &name, const QVariant &resource)
{
    Q_UNUSED(type);
    Q_D(BOBUIextDocument);
    d->resources.insert(name, resource);
}

/*!
    \since 6.1

    Returns the resource provider for this text document.

    \sa setResourceProvider(), defaultResourceProvider(), loadResource()
*/
BOBUIextDocument::ResourceProvider BOBUIextDocument::resourceProvider() const
{
    Q_D(const BOBUIextDocument);
    return d->resourceProvider;
}

/*!
   \since 6.1
   \typealias BOBUIextDocument::ResourceProvider

   Type alias for std::function\<QVariant(const QUrl&)\>.
*/

/*!
    \since 6.1

    Sets the provider of resources for the text document to \a provider.

    \sa resourceProvider(), loadResource()
*/
void BOBUIextDocument::setResourceProvider(const ResourceProvider &provider)
{
    Q_D(BOBUIextDocument);
    d->resourceProvider = provider;
}

/*!
    \since 6.1

    Sets the default resource provider to \a provider.

    The default provider will be used by all BOBUIextDocuments that don't have an
    explicit provider set.

    \sa setResourceProvider(), loadResource()
*/
void BOBUIextDocument::setDefaultResourceProvider(const ResourceProvider &provider)
{
    bobui_defaultResourceProvider = provider;
}

/*!
    \since 6.1

    Returns the default resource provider.

    \sa resourceProvider(), loadResource()
*/
BOBUIextDocument::ResourceProvider BOBUIextDocument::defaultResourceProvider()
{
    return bobui_defaultResourceProvider;
}

/*!
    Loads data of the specified \a type from the resource with the
    given \a name.

    This function is called by the rich text engine to request data that isn't
    directly stored by BOBUIextDocument, but still associated with it. For example,
    images are referenced indirectly by the name attribute of a BOBUIextImageFormat
    object.

    When called by BobUI, \a type is one of the values of
    BOBUIextDocument::ResourceType.

    If the BOBUIextDocument is a child object of a QObject that has an invokable
    loadResource method such as BOBUIextEdit, BOBUIextBrowser
    or a BOBUIextDocument itself then the default implementation tries
    to retrieve the data from the parent.

    \sa BOBUIextDocument::ResourceProvider
*/
QVariant BOBUIextDocument::loadResource(int type, const QUrl &name)
{
    Q_D(BOBUIextDocument);
    QVariant r;

    QObject *p = parent();
    if (p) {
        const QMetaObject *me = p->metaObject();
        int index = me->indexOfMethod("loadResource(int,QUrl)");
        if (index >= 0) {
            QMetaMethod loader = me->method(index);
            // don't invoke() via a queued connection: this function needs to return a value
            loader.invoke(p, BobUI::DirectConnection, Q_RETURN_ARG(QVariant, r), Q_ARG(int, type), Q_ARG(QUrl, name));
        }
    }

    // handle data: URLs
    if (r.isNull() && name.scheme().compare("data"_L1, BobUI::CaseInsensitive) == 0) {
        QString mimetype;
        QByteArray payload;
        if (qDecodeDataUrl(name, mimetype, payload))
            r = payload;
    }

    // if resource was not loaded try to load it here
    if (!qobject_cast<BOBUIextDocument *>(p) && r.isNull()) {
        QUrl resourceUrl = name;

        if (name.isRelative()) {
            const QUrl currentURL{d->url};
            // For the second case QUrl can merge "#someanchor" with "foo.html"
            // correctly to "foo.html#someanchor"
            if (!(currentURL.isRelative()
                  || (currentURL.scheme() == "file"_L1
                      && !QFileInfo(currentURL.toLocalFile()).isAbsolute()))
                || (name.hasFragment() && name.path().isEmpty())) {
                resourceUrl =  currentURL.resolved(name);
            } else {
                // this is our last resort when current url and new url are both relative
                // we try to resolve against the current working directory in the local
                // file system.
                QFileInfo fi(currentURL.toLocalFile());
                if (fi.exists()) {
                    resourceUrl =
                        QUrl::fromLocalFile(fi.absolutePath() + QDir::separator()).resolved(name);
                } else if (currentURL.isEmpty()) {
                    resourceUrl.setScheme("file"_L1);
                }
            }
        }

        QString s = resourceUrl.toLocalFile();
        QFile f(s);
        if (!s.isEmpty() && f.open(QFile::ReadOnly)) {
            r = f.readAll();
            f.close();
        }
    }

    if (!r.isNull()) {
        if (type == ImageResource && r.userType() == QMetaType::QByteArray) {
            if (!BOBUIhread::isMainThread()) {
                // must use images in non-GUI threads
                QImage image;
                image.loadFromData(r.toByteArray());
                if (!image.isNull())
                    r = image;
            } else {
                QPixmap pm;
                pm.loadFromData(r.toByteArray());
                if (!pm.isNull())
                    r = pm;
            }
        }
        d->cachedResources.insert(name, r);
    }
    return r;
}

static BOBUIextFormat formatDifference(const BOBUIextFormat &from, const BOBUIextFormat &to)
{
    BOBUIextFormat diff = to;

    const QMap<int, QVariant> props = to.properties();
    for (QMap<int, QVariant>::ConstIterator it = props.begin(), end = props.end();
         it != end; ++it)
        if (it.value() == from.property(it.key()))
            diff.clearProperty(it.key());

    return diff;
}

static QString colorValue(QColor color)
{
    QString result;

    if (color.alpha() == 255) {
        result = color.name();
    } else if (color.alpha()) {
        QString alphaValue = QString::number(color.alphaF(), 'f', 6);
        while (alphaValue.size() > 1 && alphaValue.at(alphaValue.size() - 1) == u'0')
            alphaValue.chop(1);
        if (alphaValue.at(alphaValue.size() - 1) == u'.')
            alphaValue.chop(1);
        result = QString::fromLatin1("rgba(%1,%2,%3,%4)").arg(color.red())
                                                         .arg(color.green())
                                                         .arg(color.blue())
                                                         .arg(alphaValue);
    } else {
        result = "transparent"_L1;
    }

    return result;
}

/*!
    \class BOBUIextHtmlExporter
    \inmodule BobUIGui
    \internal
*/
BOBUIextHtmlExporter::BOBUIextHtmlExporter(const BOBUIextDocument *_doc)
    : doc(_doc), fragmentMarkers(false)
{
    const QFont defaultFont = doc->defaultFont();
    defaultCharFormat.setFont(defaultFont);
}

static QStringList resolvedFontFamilies(const BOBUIextCharFormat &format)
{
    return format.fontFamilies().toStringList();
}

/*!
    Returns the document in HTML format. The conversion may not be
    perfect, especially for complex documents, due to the limitations
    of HTML.
*/
QString BOBUIextHtmlExporter::toHtml(ExportMode mode)
{
    html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" "
           "\"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
           "<html><head><meta name=\"qrichtext\" content=\"1\" />"_L1;
    html.reserve(BOBUIextDocumentPrivate::get(doc)->length());

    fragmentMarkers = (mode == ExportFragment);

    html += "<meta charset=\"utf-8\" />"_L1;

    QString title  = doc->metaInformation(BOBUIextDocument::DocumentTitle);
    if (!title.isEmpty()) {
        html += "<title>"_L1;
        html += title;
        html += "</title>"_L1;
    }
    html += "<style type=\"text/css\">\n"_L1;
    html += "p, li { white-space: pre-wrap; }\n"_L1;
    html += "hr { height: 1px; border-width: 0; }\n"_L1;
    html += "li.unchecked::marker { content: \"\\2610\"; }\n"_L1;
    html += "li.checked::marker { content: \"\\2612\"; }\n"_L1;
    html += "</style>"_L1;
    html += "</head><body"_L1;

    if (mode == ExportEntireDocument) {
        html += " style=\""_L1;

        emitFontFamily(resolvedFontFamilies(defaultCharFormat));

        if (defaultCharFormat.hasProperty(BOBUIextFormat::FontPointSize)) {
            html += " font-size:"_L1;
            html += QString::number(defaultCharFormat.fontPointSize());
            html += "pt;"_L1;
        } else if (defaultCharFormat.hasProperty(BOBUIextFormat::FontPixelSize)) {
            html += " font-size:"_L1;
            html += QString::number(defaultCharFormat.intProperty(BOBUIextFormat::FontPixelSize));
            html += "px;"_L1;
        }

        html += " font-weight:"_L1;
        html += QString::number(defaultCharFormat.fontWeight());
        html += u';';

        html += " font-style:"_L1;
        html += (defaultCharFormat.fontItalic() ? "italic"_L1 : "normal"_L1);
        html += u';';

        const bool percentSpacing = (defaultCharFormat.fontLetterSpacingType() == QFont::PercentageSpacing);
        if (defaultCharFormat.hasProperty(BOBUIextFormat::FontLetterSpacing) &&
            (!percentSpacing || defaultCharFormat.fontLetterSpacing() != 0.0)) {
            html += " letter-spacing:"_L1;
            qreal value = defaultCharFormat.fontLetterSpacing();
            if (percentSpacing) // Map to em (100% == 0em)
                value = (value / 100) - 1;
            html += QString::number(value);
            html += percentSpacing ? "em;"_L1 : "px;"_L1;
        }

        if (defaultCharFormat.hasProperty(BOBUIextFormat::FontWordSpacing) &&
            defaultCharFormat.fontWordSpacing() != 0.0) {
            html += " word-spacing:"_L1;
            html += QString::number(defaultCharFormat.fontWordSpacing());
            html += "px;"_L1;
        }

        QString decorationTag(" text-decoration:"_L1);
        bool atLeastOneDecorationSet = false;
        if (defaultCharFormat.hasProperty(BOBUIextFormat::FontUnderline) || defaultCharFormat.hasProperty(BOBUIextFormat::TextUnderlineStyle)) {
            if (defaultCharFormat.fontUnderline()) {
                decorationTag += " underline"_L1;
                atLeastOneDecorationSet = true;
            }
        }
        if (defaultCharFormat.hasProperty(BOBUIextFormat::FontOverline)) {
            if (defaultCharFormat.fontOverline()) {
                decorationTag += " overline"_L1;
                atLeastOneDecorationSet = true;
            }
        }
        if (defaultCharFormat.hasProperty(BOBUIextFormat::FontStrikeOut)) {
            if (defaultCharFormat.fontStrikeOut()) {
                decorationTag += " line-through"_L1;
                atLeastOneDecorationSet = true;
            }
        }
        if (atLeastOneDecorationSet)
            html += decorationTag + u';';

        html += u'\"';

        const BOBUIextFrameFormat fmt = doc->rootFrame()->frameFormat();
        emitBackgroundAttribute(fmt);

    } else {
        defaultCharFormat = BOBUIextCharFormat();
    }
    html += u'>';

    BOBUIextFrameFormat rootFmt = doc->rootFrame()->frameFormat();
    rootFmt.clearProperty(BOBUIextFormat::BackgroundBrush);

    BOBUIextFrameFormat defaultFmt;
    defaultFmt.setMargin(doc->documentMargin());

    if (rootFmt == defaultFmt)
        emitFrame(doc->rootFrame()->begin());
    else
        emitTextFrame(doc->rootFrame());

    html += "</body></html>"_L1;
    return html;
}

void BOBUIextHtmlExporter::emitAttribute(const char *attribute, const QString &value)
{
    html += u' ';
    html += QLatin1StringView(attribute);
    html += "=\""_L1;
    html += value.toHtmlEscaped();
    html += u'"';
}

bool BOBUIextHtmlExporter::emitCharFormatStyle(const BOBUIextCharFormat &format)
{
    bool attributesEmitted = false;

    {
        const QStringList families = resolvedFontFamilies(format);
        if (!families.isEmpty() && families != resolvedFontFamilies(defaultCharFormat)) {
            emitFontFamily(families);
            attributesEmitted = true;
        }
    }

    if (format.hasProperty(BOBUIextFormat::FontPointSize)
        && format.fontPointSize() != defaultCharFormat.fontPointSize()) {
        html += " font-size:"_L1;
        html += QString::number(format.fontPointSize());
        html += "pt;"_L1;
        attributesEmitted = true;
    } else if (format.hasProperty(BOBUIextFormat::FontSizeAdjustment)) {
        static const char sizeNameData[] =
            "small" "\0"
            "medium" "\0"
            "xx-large" ;
        static const quint8 sizeNameOffsets[] = {
            0,                                         // "small"
            sizeof("small"),                           // "medium"
            sizeof("small") + sizeof("medium") + 3,    // "large"    )
            sizeof("small") + sizeof("medium") + 1,    // "x-large"  )> compressed into "xx-large"
            sizeof("small") + sizeof("medium"),        // "xx-large" )
        };
        const char *name = nullptr;
        const int idx = format.intProperty(BOBUIextFormat::FontSizeAdjustment) + 1;
        if (idx >= 0 && idx <= 4) {
            name = sizeNameData + sizeNameOffsets[idx];
        }
        if (name) {
            html += " font-size:"_L1;
            html += QLatin1StringView(name);
            html += u';';
            attributesEmitted = true;
        }
    } else if (format.hasProperty(BOBUIextFormat::FontPixelSize)
               && format.property(BOBUIextFormat::FontPixelSize)
                       != defaultCharFormat.property(BOBUIextFormat::FontPixelSize)) {
        html += " font-size:"_L1;
        html += QString::number(format.intProperty(BOBUIextFormat::FontPixelSize));
        html += "px;"_L1;
        attributesEmitted = true;
    }

    if (format.hasProperty(BOBUIextFormat::FontWeight)
        && format.fontWeight() != defaultCharFormat.fontWeight()) {
        html += " font-weight:"_L1;
        html += QString::number(format.fontWeight());
        html += u';';
        attributesEmitted = true;
    }

    if (format.hasProperty(BOBUIextFormat::FontItalic)
        && format.fontItalic() != defaultCharFormat.fontItalic()) {
        html += " font-style:"_L1;
        html += (format.fontItalic() ? "italic"_L1 : "normal"_L1);
        html += u';';
        attributesEmitted = true;
    }

    const auto decorationTag = " text-decoration:"_L1;
    html += decorationTag;
    bool hasDecoration = false;
    bool atLeastOneDecorationSet = false;

    if ((format.hasProperty(BOBUIextFormat::FontUnderline) || format.hasProperty(BOBUIextFormat::TextUnderlineStyle))
        && format.fontUnderline() != defaultCharFormat.fontUnderline()) {
        hasDecoration = true;
        if (format.fontUnderline()) {
            html += " underline"_L1;
            atLeastOneDecorationSet = true;
        }
    }

    if (format.hasProperty(BOBUIextFormat::FontOverline)
        && format.fontOverline() != defaultCharFormat.fontOverline()) {
        hasDecoration = true;
        if (format.fontOverline()) {
            html += " overline"_L1;
            atLeastOneDecorationSet = true;
        }
    }

    if (format.hasProperty(BOBUIextFormat::FontStrikeOut)
        && format.fontStrikeOut() != defaultCharFormat.fontStrikeOut()) {
        hasDecoration = true;
        if (format.fontStrikeOut()) {
            html += " line-through"_L1;
            atLeastOneDecorationSet = true;
        }
    }

    if (hasDecoration) {
        if (!atLeastOneDecorationSet)
            html += "none"_L1;
        html += u';';
        if (format.hasProperty(BOBUIextFormat::TextUnderlineColor)) {
            html += " text-decoration-color:"_L1;
            html += colorValue(format.underlineColor());
            html += u';';
        }
        attributesEmitted = true;
    } else {
        html.chop(decorationTag.size());
    }

    if (format.foreground() != defaultCharFormat.foreground()
        && format.foreground().style() != BobUI::NoBrush) {
        QBrush brush = format.foreground();
        if (brush.style() == BobUI::TexturePattern) {
            const bool isPixmap = qHasPixmapTexture(brush);
            const qint64 cacheKey = isPixmap ? brush.texture().cacheKey() : brush.textureImage().cacheKey();

            html += " -bobui-fg-texture-cachekey:"_L1;
            html += QString::number(cacheKey);
            html += ";"_L1;
        } else if (brush.style() == BobUI::LinearGradientPattern
                   || brush.style() == BobUI::RadialGradientPattern
                   || brush.style() == BobUI::ConicalGradientPattern) {
            const QGradient *gradient = brush.gradient();
            if (gradient->type() == QGradient::LinearGradient) {
                const QLinearGradient *linearGradient = static_cast<const QLinearGradient *>(brush.gradient());

                html += " -bobui-foreground: qlineargradient("_L1;
                html += "x1:"_L1 + QString::number(linearGradient->start().x()) + u',';
                html += "y1:"_L1 + QString::number(linearGradient->start().y()) + u',';
                html += "x2:"_L1 + QString::number(linearGradient->finalStop().x()) + u',';
                html += "y2:"_L1 + QString::number(linearGradient->finalStop().y()) + u',';
            } else if (gradient->type() == QGradient::RadialGradient) {
                const QRadialGradient *radialGradient = static_cast<const QRadialGradient *>(brush.gradient());

                html += " -bobui-foreground: qradialgradient("_L1;
                html += "cx:"_L1 + QString::number(radialGradient->center().x()) + u',';
                html += "cy:"_L1 + QString::number(radialGradient->center().y()) + u',';
                html += "fx:"_L1 + QString::number(radialGradient->focalPoint().x()) + u',';
                html += "fy:"_L1 + QString::number(radialGradient->focalPoint().y()) + u',';
                html += "radius:"_L1 + QString::number(radialGradient->radius()) + u',';
            } else {
                const QConicalGradient *conicalGradient = static_cast<const QConicalGradient *>(brush.gradient());

                html += " -bobui-foreground: qconicalgradient("_L1;
                html += "cx:"_L1 + QString::number(conicalGradient->center().x()) + u',';
                html += "cy:"_L1 + QString::number(conicalGradient->center().y()) + u',';
                html += "angle:"_L1 + QString::number(conicalGradient->angle()) + u',';
            }

            const QStringList coordinateModes = { "logical"_L1, "stretchtodevice"_L1, "objectbounding"_L1, "object"_L1 };
            html += "coordinatemode:"_L1;
            html += coordinateModes.at(int(gradient->coordinateMode()));
            html += u',';

            const QStringList spreads = { "pad"_L1, "reflect"_L1, "repeat"_L1 };
            html += "spread:"_L1;
            html += spreads.at(int(gradient->spread()));

            for (const QGradientStop &stop : gradient->stops()) {
                html += ",stop:"_L1;
                html += QString::number(stop.first);
                html += u' ';
                html += colorValue(stop.second);
            }

            html += ");"_L1;
        } else {
            html += " color:"_L1;
            html += colorValue(brush.color());
            html += u';';
        }
        attributesEmitted = true;
    }

    if (format.background() != defaultCharFormat.background()
        && format.background().style() == BobUI::SolidPattern) {
        html += " background-color:"_L1;
        html += colorValue(format.background().color());
        html += u';';
        attributesEmitted = true;
    }

    if (format.verticalAlignment() != defaultCharFormat.verticalAlignment()
        && format.verticalAlignment() != BOBUIextCharFormat::AlignNormal)
    {
        html += " vertical-align:"_L1;

        BOBUIextCharFormat::VerticalAlignment valign = format.verticalAlignment();
        if (valign == BOBUIextCharFormat::AlignSubScript)
            html += "sub"_L1;
        else if (valign == BOBUIextCharFormat::AlignSuperScript)
            html += "super"_L1;
        else if (valign == BOBUIextCharFormat::AlignMiddle)
            html += "middle"_L1;
        else if (valign == BOBUIextCharFormat::AlignTop)
            html += "top"_L1;
        else if (valign == BOBUIextCharFormat::AlignBottom)
            html += "bottom"_L1;

        html += u';';
        attributesEmitted = true;
    }

    if (format.fontCapitalization() != QFont::MixedCase) {
        const QFont::Capitalization caps = format.fontCapitalization();
        if (caps == QFont::AllUppercase)
            html += " text-transform:uppercase;"_L1;
        else if (caps == QFont::AllLowercase)
            html += " text-transform:lowercase;"_L1;
        else if (caps == QFont::SmallCaps)
            html += " font-variant:small-caps;"_L1;
        attributesEmitted = true;
    }

    if (format.fontWordSpacing() != 0.0) {
        html += " word-spacing:"_L1;
        html += QString::number(format.fontWordSpacing());
        html += "px;"_L1;
        attributesEmitted = true;
    }

    if (format.hasProperty(BOBUIextFormat::TextOutline)) {
        QPen outlinePen = format.textOutline();
        html += " -bobui-stroke-color:"_L1;
        html += colorValue(outlinePen.color());
        html += u';';

        html += " -bobui-stroke-width:"_L1;
        html += QString::number(outlinePen.widthF());
        html += "px;"_L1;

        html += " -bobui-stroke-linecap:"_L1;
        if (outlinePen.capStyle() == BobUI::SquareCap)
            html += "squarecap;"_L1;
        else if (outlinePen.capStyle() == BobUI::FlatCap)
            html += "flatcap;"_L1;
        else if (outlinePen.capStyle() == BobUI::RoundCap)
            html += "roundcap;"_L1;

        html += " -bobui-stroke-linejoin:"_L1;
        if (outlinePen.joinStyle() == BobUI::MiterJoin)
            html += "miterjoin;"_L1;
        else if (outlinePen.joinStyle() == BobUI::SvgMiterJoin)
            html += "svgmiterjoin;"_L1;
        else if (outlinePen.joinStyle() == BobUI::BevelJoin)
            html += "beveljoin;"_L1;
        else if (outlinePen.joinStyle() == BobUI::RoundJoin)
            html += "roundjoin;"_L1;

        if (outlinePen.joinStyle() == BobUI::MiterJoin ||
            outlinePen.joinStyle() == BobUI::SvgMiterJoin) {
            html += " -bobui-stroke-miterlimit:"_L1;
            html += QString::number(outlinePen.miterLimit());
            html += u';';
        }

        if (outlinePen.style() == BobUI::CustomDashLine && !outlinePen.dashPattern().empty()) {
            html += " -bobui-stroke-dasharray:"_L1;
            QString dashArrayString;
            QList<qreal> dashes = outlinePen.dashPattern();

            for (int i = 0; i < dashes.length() - 1; i++) {
                qreal dash = dashes[i];
                dashArrayString += QString::number(dash) + u',';
            }

            dashArrayString += QString::number(dashes.last());
            html += dashArrayString;
            html += u';';

            html += " -bobui-stroke-dashoffset:"_L1;
            html += QString::number(outlinePen.dashOffset());
            html += u';';
        }

        attributesEmitted = true;
    }

    return attributesEmitted;
}

void BOBUIextHtmlExporter::emitTextLength(const char *attribute, const BOBUIextLength &length)
{
    if (length.type() == BOBUIextLength::VariableLength) // default
        return;

    html += u' ';
    html += QLatin1StringView(attribute);
    html += "=\""_L1;
    html += QString::number(length.rawValue());

    if (length.type() == BOBUIextLength::PercentageLength)
        html += "%\""_L1;
    else
        html += u'\"';
}

void BOBUIextHtmlExporter::emitAlignment(BobUI::Alignment align)
{
    if (align & BobUI::AlignLeft)
        return;
    else if (align & BobUI::AlignRight)
        html += " align=\"right\""_L1;
    else if (align & BobUI::AlignHCenter)
        html += " align=\"center\""_L1;
    else if (align & BobUI::AlignJustify)
        html += " align=\"justify\""_L1;
}

void BOBUIextHtmlExporter::emitFloatStyle(BOBUIextFrameFormat::Position pos, StyleMode mode)
{
    if (pos == BOBUIextFrameFormat::InFlow)
        return;

    if (mode == EmitStyleTag)
        html += " style=\"float:"_L1;
    else
        html += " float:"_L1;

    if (pos == BOBUIextFrameFormat::FloatLeft)
        html += " left;"_L1;
    else if (pos == BOBUIextFrameFormat::FloatRight)
        html += " right;"_L1;
    else
        Q_ASSERT_X(0, "BOBUIextHtmlExporter::emitFloatStyle()", "pos should be a valid enum type");

    if (mode == EmitStyleTag)
        html += u'\"';
}

static QLatin1StringView richtextBorderStyleToHtmlBorderStyle(BOBUIextFrameFormat::BorderStyle style)
{
    switch (style) {
    case BOBUIextFrameFormat::BorderStyle_None:
        return "none"_L1;
    case BOBUIextFrameFormat::BorderStyle_Dotted:
        return "dotted"_L1;
    case BOBUIextFrameFormat::BorderStyle_Dashed:
        return "dashed"_L1;
    case BOBUIextFrameFormat::BorderStyle_Solid:
        return "solid"_L1;
    case BOBUIextFrameFormat::BorderStyle_Double:
        return "double"_L1;
    case BOBUIextFrameFormat::BorderStyle_DotDash:
        return "dot-dash"_L1;
    case BOBUIextFrameFormat::BorderStyle_DotDotDash:
        return "dot-dot-dash"_L1;
    case BOBUIextFrameFormat::BorderStyle_Groove:
        return "groove"_L1;
    case BOBUIextFrameFormat::BorderStyle_Ridge:
        return "ridge"_L1;
    case BOBUIextFrameFormat::BorderStyle_Inset:
        return "inset"_L1;
    case BOBUIextFrameFormat::BorderStyle_Outset:
        return "outset"_L1;
    default:
        Q_UNREACHABLE();
    };
    return ""_L1;
}

void BOBUIextHtmlExporter::emitBorderStyle(BOBUIextFrameFormat::BorderStyle style)
{
    Q_ASSERT(style <= BOBUIextFrameFormat::BorderStyle_Outset);

    html += " border-style:"_L1;
    html += richtextBorderStyleToHtmlBorderStyle(style);
    html += u';';
}

void BOBUIextHtmlExporter::emitPageBreakPolicy(BOBUIextFormat::PageBreakFlags policy)
{
    if (policy & BOBUIextFormat::PageBreak_AlwaysBefore)
        html += " page-break-before:always;"_L1;

    if (policy & BOBUIextFormat::PageBreak_AlwaysAfter)
        html += " page-break-after:always;"_L1;
}

void BOBUIextHtmlExporter::emitFontFamily(const QStringList &families)
{
    html += " font-family:"_L1;

    bool first = true;
    for (const QString &family : families) {
        auto quote = "\'"_L1;
        if (family.contains(u'\''))
            quote = "&quot;"_L1;

        if (!first)
            html += ","_L1;
        else
            first = false;
        html += quote;
        html += family.toHtmlEscaped();
        html += quote;
    }
    html += u';';
}

void BOBUIextHtmlExporter::emitMargins(const QString &top, const QString &bottom, const QString &left, const QString &right)
{
    html += " margin-top:"_L1;
    html += top;
    html += "px;"_L1;

    html += " margin-bottom:"_L1;
    html += bottom;
    html += "px;"_L1;

    html += " margin-left:"_L1;
    html += left;
    html += "px;"_L1;

    html += " margin-right:"_L1;
    html += right;
    html += "px;"_L1;
}

void BOBUIextHtmlExporter::emitFragment(const BOBUIextFragment &fragment)
{
    const BOBUIextCharFormat format = fragment.charFormat();

    bool closeAnchor = false;

    if (format.isAnchor()) {
        const auto names = format.anchorNames();
        if (!names.isEmpty()) {
            html += "<a name=\""_L1;
            html += names.constFirst().toHtmlEscaped();
            html += "\"></a>"_L1;
        }
        const QString href = format.anchorHref();
        if (!href.isEmpty()) {
            html += "<a href=\""_L1;
            html += href.toHtmlEscaped();
            html += "\">"_L1;
            closeAnchor = true;
        }
    }

    QString txt = fragment.text();
    const bool isObject = txt.contains(QChar::ObjectReplacementCharacter);
    const bool isImage = isObject && format.isImageFormat();

    const auto styleTag = "<span style=\""_L1;
    html += styleTag;

    bool attributesEmitted = false;
    if (!isImage)
        attributesEmitted = emitCharFormatStyle(format);
    if (attributesEmitted)
        html += "\">"_L1;
    else
        html.chop(styleTag.size());

    if (isObject) {
        for (int i = 0; isImage && i < txt.size(); ++i) {
            BOBUIextImageFormat imgFmt = format.toImageFormat();

            html += "<img"_L1;

            QString maxWidthCss;

            if (imgFmt.hasProperty(BOBUIextFormat::ImageMaxWidth)) {
                auto length = imgFmt.lengthProperty(BOBUIextFormat::ImageMaxWidth);
                maxWidthCss += "max-width:"_L1;
                if (length.type() == BOBUIextLength::PercentageLength)
                    maxWidthCss += QString::number(length.rawValue()) + "%;"_L1;
                else if (length.type() == BOBUIextLength::FixedLength)
                    maxWidthCss += QString::number(length.rawValue()) + "px;"_L1;
            }

            if (imgFmt.hasProperty(BOBUIextFormat::ImageName))
                emitAttribute("src", imgFmt.name());

            if (imgFmt.hasProperty(BOBUIextFormat::ImageAltText))
                emitAttribute("alt", imgFmt.stringProperty(BOBUIextFormat::ImageAltText));

            if (imgFmt.hasProperty(BOBUIextFormat::ImageTitle))
                emitAttribute("title", imgFmt.stringProperty(BOBUIextFormat::ImageTitle));

            if (imgFmt.hasProperty(BOBUIextFormat::ImageWidth))
                emitAttribute("width", QString::number(imgFmt.width()));

            if (imgFmt.hasProperty(BOBUIextFormat::ImageHeight))
                emitAttribute("height", QString::number(imgFmt.height()));

            if (imgFmt.verticalAlignment() == BOBUIextCharFormat::AlignMiddle)
                html += " style=\"vertical-align: middle;"_L1 + maxWidthCss + u'\"';
            else if (imgFmt.verticalAlignment() == BOBUIextCharFormat::AlignTop)
                html += " style=\"vertical-align: top;"_L1 + maxWidthCss + u'\"';
            else if (!maxWidthCss.isEmpty())
                html += " style=\""_L1 + maxWidthCss + u'\"';

            if (BOBUIextFrame *imageFrame = qobject_cast<BOBUIextFrame *>(doc->objectForFormat(imgFmt)))
                emitFloatStyle(imageFrame->frameFormat().position());

            html += " />"_L1;
        }
    } else {
        Q_ASSERT(!txt.contains(QChar::ObjectReplacementCharacter));

        txt = txt.toHtmlEscaped();

        // split for [\n{LineSeparator}]
        // space in BR on purpose for compatibility with old-fashioned browsers
        txt.replace(u'\n', "<br />"_L1);
        txt.replace(QChar::LineSeparator, "<br />"_L1);
        html += txt;
    }

    if (attributesEmitted)
        html += "</span>"_L1;

    if (closeAnchor)
        html += "</a>"_L1;
}

static bool isOrderedList(int style)
{
    return style == BOBUIextListFormat::ListDecimal || style == BOBUIextListFormat::ListLowerAlpha
           || style == BOBUIextListFormat::ListUpperAlpha
           || style == BOBUIextListFormat::ListUpperRoman
           || style == BOBUIextListFormat::ListLowerRoman
           ;
}

void BOBUIextHtmlExporter::emitBlockAttributes(const BOBUIextBlock &block)
{
    BOBUIextBlockFormat format = block.blockFormat();
    emitAlignment(format.alignment());

    // assume default to not bloat the html too much
    // html += " dir='ltr'"_L1;
    if (block.textDirection() == BobUI::RightToLeft)
        html += " dir='rtl'"_L1;

    const auto style = " style=\""_L1;
    html += style;

    const bool emptyBlock = block.begin().atEnd();
    if (emptyBlock) {
        html += "-bobui-paragraph-type:empty;"_L1;
    }

    emitMargins(QString::number(format.topMargin()),
                QString::number(format.bottomMargin()),
                QString::number(format.leftMargin()),
                QString::number(format.rightMargin()));

    html += " -bobui-block-indent:"_L1;
    html += QString::number(format.indent());
    html += u';';

    html += " text-indent:"_L1;
    html += QString::number(format.textIndent());
    html += "px;"_L1;

    if (block.userState() != -1) {
        html += " -bobui-user-state:"_L1;
        html += QString::number(block.userState());
        html += u';';
    }

    if (format.lineHeightType() != BOBUIextBlockFormat::SingleHeight) {
        html += " line-height:"_L1
             + QString::number(format.lineHeight());
        switch (format.lineHeightType()) {
            case BOBUIextBlockFormat::ProportionalHeight:
                html += "%;"_L1;
                break;
            case BOBUIextBlockFormat::FixedHeight:
                html += "; -bobui-line-height-type: fixed;"_L1;
                break;
            case BOBUIextBlockFormat::MinimumHeight:
                html += "px;"_L1;
                break;
            case BOBUIextBlockFormat::LineDistanceHeight:
                html += "; -bobui-line-height-type: line-distance;"_L1;
                break;
            default:
                html += ";"_L1;
                break; // Should never reach here
        }
    }

    emitPageBreakPolicy(format.pageBreakPolicy());

    BOBUIextCharFormat diff;
    if (emptyBlock) { // only print character properties when we don't expect them to be repeated by actual text in the parag
        const BOBUIextCharFormat blockCharFmt = block.charFormat();
        diff = formatDifference(defaultCharFormat, blockCharFmt).toCharFormat();
    }

    diff.clearProperty(BOBUIextFormat::BackgroundBrush);
    if (format.hasProperty(BOBUIextFormat::BackgroundBrush)) {
        QBrush bg = format.background();
        if (bg.style() != BobUI::NoBrush)
            diff.setProperty(BOBUIextFormat::BackgroundBrush, format.property(BOBUIextFormat::BackgroundBrush));
    }

    if (!diff.properties().isEmpty())
        emitCharFormatStyle(diff);

    html += u'"';

}

void BOBUIextHtmlExporter::emitBlock(const BOBUIextBlock &block)
{
    if (block.begin().atEnd()) {
        // ### HACK, remove once BOBUIextFrame::Iterator is fixed
        int p = block.position();
        if (p > 0)
            --p;

        BOBUIextDocumentPrivate::FragmentIterator frag = BOBUIextDocumentPrivate::get(doc)->find(p);
        QChar ch = BOBUIextDocumentPrivate::get(doc)->buffer().at(frag->stringPosition);
        if (ch == BOBUIextBeginningOfFrame
            || ch == BOBUIextEndOfFrame)
            return;
    }

    html += u'\n';

    // save and later restore, in case we 'change' the default format by
    // emitting block char format information
    BOBUIextCharFormat oldDefaultCharFormat = defaultCharFormat;

    BOBUIextList *list = block.textList();
    if (list) {
        if (list->itemNumber(block) == 0) { // first item? emit <ul> or appropriate
            const BOBUIextListFormat format = list->format();
            const int style = format.style();
            bool ordered = false;
            switch (style) {
                case BOBUIextListFormat::ListDisc: html += "<ul"_L1; break;
                case BOBUIextListFormat::ListCircle: html += "<ul type=\"circle\""_L1; break;
                case BOBUIextListFormat::ListSquare: html += "<ul type=\"square\""_L1; break;
                case BOBUIextListFormat::ListDecimal: html += "<ol"_L1; ordered = true; break;
                case BOBUIextListFormat::ListLowerAlpha: html += "<ol type=\"a\""_L1; ordered = true; break;
                case BOBUIextListFormat::ListUpperAlpha: html += "<ol type=\"A\""_L1; ordered = true; break;
                case BOBUIextListFormat::ListLowerRoman: html += "<ol type=\"i\""_L1; ordered = true; break;
                case BOBUIextListFormat::ListUpperRoman: html += "<ol type=\"I\""_L1; ordered = true; break;
                default: html += "<ul"_L1; // ### should not happen
            }

            if (ordered && format.start() != 1) {
                html += " start=\""_L1;
                html += QString::number(format.start());
                html += u'"';
            }

            QString styleString;
            styleString += "margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px;"_L1;

            if (format.hasProperty(BOBUIextFormat::ListIndent)) {
                styleString += " -bobui-list-indent: "_L1;
                styleString += QString::number(format.indent());
                styleString += u';';
            }

            if (format.hasProperty(BOBUIextFormat::ListNumberPrefix)) {
                QString numberPrefix = format.numberPrefix();
                numberPrefix.replace(u'"', "\\22"_L1);
                numberPrefix.replace(u'\'', "\\27"_L1); // FIXME: There's a problem in the CSS parser the prevents this from being correctly restored
                styleString += " -bobui-list-number-prefix: "_L1;
                styleString += u'\'';
                styleString += numberPrefix;
                styleString += u'\'';
                styleString += u';';
            }

            if (format.hasProperty(BOBUIextFormat::ListNumberSuffix)) {
                if (format.numberSuffix() != "."_L1) { // this is our default
                    QString numberSuffix = format.numberSuffix();
                    numberSuffix.replace(u'"', "\\22"_L1);
                    numberSuffix.replace(u'\'', "\\27"_L1); // see above
                    styleString += " -bobui-list-number-suffix: "_L1;
                    styleString += u'\'';
                    styleString += numberSuffix;
                    styleString += u'\'';
                    styleString += u';';
                }
            }

            html += " style=\""_L1;
            html += styleString;
            html += "\">\n"_L1;
        }

        html += "<li"_L1;

        const BOBUIextCharFormat blockFmt = formatDifference(defaultCharFormat, block.charFormat()).toCharFormat();
        if (!blockFmt.properties().isEmpty()) {
            html += " style=\""_L1;
            emitCharFormatStyle(blockFmt);
            html += u'\"';

            defaultCharFormat.merge(block.charFormat());
        }
        if (block.blockFormat().hasProperty(BOBUIextFormat::BlockMarker)) {
            switch (block.blockFormat().marker()) {
            case BOBUIextBlockFormat::MarkerType::Checked:
                html += " class=\"checked\""_L1;
                break;
            case BOBUIextBlockFormat::MarkerType::Unchecked:
                html += " class=\"unchecked\""_L1;
                break;
            case BOBUIextBlockFormat::MarkerType::NoMarker:
                break;
            }
        }
    }

    const BOBUIextBlockFormat blockFormat = block.blockFormat();
    if (blockFormat.hasProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth)) {
        html += "<hr"_L1;

        BOBUIextLength width = blockFormat.lengthProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth);
        if (width.type() != BOBUIextLength::VariableLength)
            emitTextLength("width", width);
        html += u' ';

        if (blockFormat.hasProperty(BOBUIextFormat::BackgroundBrush)) {
            html += "style=\""_L1;
            html += "background-color:"_L1;
            html += colorValue(qvariant_cast<QBrush>(blockFormat.property(BOBUIextFormat::BackgroundBrush)).color());
            html += u';';
            html += u'\"';
        }

        html += "/>"_L1;
        return;
    }

    const bool pre = blockFormat.nonBreakableLines();
    if (pre) {
        if (list)
            html += u'>';
        html += "<pre"_L1;
    } else if (!list) {
        int headingLevel = blockFormat.headingLevel();
        if (headingLevel > 0 && headingLevel <= 6)
            html += "<h"_L1 + QString::number(headingLevel);
        else
            html += "<p"_L1;
    }

    emitBlockAttributes(block);

    html += u'>';
    if (block.begin().atEnd())
        html += "<br />"_L1;

    BOBUIextBlock::Iterator it = block.begin();
    if (fragmentMarkers && !it.atEnd() && block == doc->begin())
        html += "<!--StartFragment-->"_L1;

    for (; !it.atEnd(); ++it)
        emitFragment(it.fragment());

    if (fragmentMarkers && block.position() + block.length() == BOBUIextDocumentPrivate::get(doc)->length())
        html += "<!--EndFragment-->"_L1;

    QString closeTags;

    if (pre)
        html += "</pre>"_L1;
    else if (list)
        closeTags += "</li>"_L1;
    else {
        int headingLevel = blockFormat.headingLevel();
        if (headingLevel > 0 && headingLevel <= 6)
            html += QString::asprintf("</h%d>", headingLevel);
        else
            html += "</p>"_L1;
    }

    if (list) {
        if (list->itemNumber(block) == list->count() - 1) { // last item? close list
            if (isOrderedList(list->format().style()))
                closeTags += "</ol>"_L1;
            else
                closeTags += "</ul>"_L1;
        }
        const BOBUIextBlock nextBlock = block.next();
        // If the next block is the beginning of a new deeper nested list, then we don't
        // want to close the current list item just yet. This should be closed when this
        // item is fully finished
        if (nextBlock.isValid() && nextBlock.textList() &&
            nextBlock.textList()->itemNumber(nextBlock) == 0 &&
            nextBlock.textList()->format().indent() > list->format().indent()) {
            QString lastTag;
            if (!closingTags.isEmpty() && list->itemNumber(block) == list->count() - 1)
                lastTag = closingTags.takeLast();
            lastTag.prepend(closeTags);
            closingTags << lastTag;
        } else if (list->itemNumber(block) == list->count() - 1) {
            // If we are at the end of the list now then we can add in the closing tags for that
            // current block
            html += closeTags;
            if (!closingTags.isEmpty())
                html += closingTags.takeLast();
        } else {
            html += closeTags;
        }
    }

    defaultCharFormat = oldDefaultCharFormat;
}

extern bool qHasPixmapTexture(const QBrush& brush);

QString BOBUIextHtmlExporter::findUrlForImage(const BOBUIextDocument *doc, qint64 cacheKey, bool isPixmap)
{
    QString url;
    if (!doc)
        return url;

    if (BOBUIextDocument *parent = qobject_cast<BOBUIextDocument *>(doc->parent()))
        return findUrlForImage(parent, cacheKey, isPixmap);

    const BOBUIextDocumentPrivate *priv = BOBUIextDocumentPrivate::get(doc);
    Q_ASSERT(priv != nullptr);

    QMap<QUrl, QVariant>::const_iterator it = priv->cachedResources.constBegin();
    for (; it != priv->cachedResources.constEnd(); ++it) {

        const QVariant &v = it.value();
        if (v.userType() == QMetaType::QImage && !isPixmap) {
            if (qvariant_cast<QImage>(v).cacheKey() == cacheKey)
                break;
        }

        if (v.userType() == QMetaType::QPixmap && isPixmap) {
            if (qvariant_cast<QPixmap>(v).cacheKey() == cacheKey)
                break;
        }
    }

    if (it != priv->cachedResources.constEnd())
        url = it.key().toString();

    return url;
}

void BOBUIextDocumentPrivate::mergeCachedResources(const BOBUIextDocumentPrivate *priv)
{
    if (!priv)
        return;

    cachedResources.insert(priv->cachedResources);
}

void BOBUIextHtmlExporter::emitBackgroundAttribute(const BOBUIextFormat &format)
{
    if (format.hasProperty(BOBUIextFormat::BackgroundImageUrl)) {
        QString url = format.property(BOBUIextFormat::BackgroundImageUrl).toString();
        emitAttribute("background", url);
    } else {
        const QBrush &brush = format.background();
        if (brush.style() == BobUI::SolidPattern) {
            emitAttribute("bgcolor", colorValue(brush.color()));
        } else if (brush.style() == BobUI::TexturePattern) {
            const bool isPixmap = qHasPixmapTexture(brush);
            const qint64 cacheKey = isPixmap ? brush.texture().cacheKey() : brush.textureImage().cacheKey();

            const QString url = findUrlForImage(doc, cacheKey, isPixmap);

            if (!url.isEmpty())
                emitAttribute("background", url);
        }
    }
}

void BOBUIextHtmlExporter::emitTable(const BOBUIextTable *table)
{
    BOBUIextTableFormat format = table->format();

    html += "\n<table"_L1;

    if (format.hasProperty(BOBUIextFormat::FrameBorder))
        emitAttribute("border", QString::number(format.border()));

    emitFrameStyle(format, TableFrame);

    emitAlignment(format.alignment());
    emitTextLength("width", format.width());

    if (format.hasProperty(BOBUIextFormat::TableCellSpacing))
        emitAttribute("cellspacing", QString::number(format.cellSpacing()));
    if (format.hasProperty(BOBUIextFormat::TableCellPadding))
        emitAttribute("cellpadding", QString::number(format.cellPadding()));

    emitBackgroundAttribute(format);

    html += u'>';

    const int rows = table->rows();
    const int columns = table->columns();

    QList<BOBUIextLength> columnWidths = format.columnWidthConstraints();
    if (columnWidths.isEmpty()) {
        columnWidths.resize(columns);
        columnWidths.fill(BOBUIextLength());
    }
    Q_ASSERT(columnWidths.size() == columns);

    QVarLengthArray<bool> widthEmittedForColumn(columns);
    for (int i = 0; i < columns; ++i)
        widthEmittedForColumn[i] = false;

    const int headerRowCount = qMin(format.headerRowCount(), rows);
    if (headerRowCount > 0)
        html += "<thead>"_L1;

    for (int row = 0; row < rows; ++row) {
        html += "\n<tr>"_L1;

        for (int col = 0; col < columns; ++col) {
            const BOBUIextTableCell cell = table->cellAt(row, col);

            // for col/rowspans
            if (cell.row() != row)
                continue;

            if (cell.column() != col)
                continue;

            html += "\n<td"_L1;

            if (!widthEmittedForColumn[col] && cell.columnSpan() == 1) {
                emitTextLength("width", columnWidths.at(col));
                widthEmittedForColumn[col] = true;
            }

            if (cell.columnSpan() > 1)
                emitAttribute("colspan", QString::number(cell.columnSpan()));

            if (cell.rowSpan() > 1)
                emitAttribute("rowspan", QString::number(cell.rowSpan()));

            const BOBUIextTableCellFormat cellFormat = cell.format().toTableCellFormat();
            emitBackgroundAttribute(cellFormat);

            BOBUIextCharFormat oldDefaultCharFormat = defaultCharFormat;

            BOBUIextCharFormat::VerticalAlignment valign = cellFormat.verticalAlignment();

            QString styleString;
            if (valign >= BOBUIextCharFormat::AlignMiddle && valign <= BOBUIextCharFormat::AlignBottom) {
                styleString += " vertical-align:"_L1;
                switch (valign) {
                case BOBUIextCharFormat::AlignMiddle:
                    styleString += "middle"_L1;
                    break;
                case BOBUIextCharFormat::AlignTop:
                    styleString += "top"_L1;
                    break;
                case BOBUIextCharFormat::AlignBottom:
                    styleString += "bottom"_L1;
                    break;
                default:
                    break;
                }
                styleString += u';';

                BOBUIextCharFormat temp;
                temp.setVerticalAlignment(valign);
                defaultCharFormat.merge(temp);
            }

            if (cellFormat.hasProperty(BOBUIextFormat::TableCellLeftPadding))
                styleString += " padding-left:"_L1 + QString::number(cellFormat.leftPadding()) + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellRightPadding))
                styleString += " padding-right:"_L1 + QString::number(cellFormat.rightPadding()) + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellTopPadding))
                styleString += " padding-top:"_L1 + QString::number(cellFormat.topPadding()) + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellBottomPadding))
                styleString += " padding-bottom:"_L1 + QString::number(cellFormat.bottomPadding()) + u';';

            if (cellFormat.hasProperty(BOBUIextFormat::TableCellTopBorder))
                styleString += " border-top:"_L1 + QString::number(cellFormat.topBorder()) + "px;"_L1;
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellRightBorder))
                styleString += " border-right:"_L1 + QString::number(cellFormat.rightBorder()) + "px;"_L1;
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellBottomBorder))
                styleString += " border-bottom:"_L1 + QString::number(cellFormat.bottomBorder()) + "px;"_L1;
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellLeftBorder))
                styleString += " border-left:"_L1 + QString::number(cellFormat.leftBorder()) + "px;"_L1;

            if (cellFormat.hasProperty(BOBUIextFormat::TableCellTopBorderBrush))
                styleString += " border-top-color:"_L1 + cellFormat.topBorderBrush().color().name() + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellRightBorderBrush))
                styleString += " border-right-color:"_L1 + cellFormat.rightBorderBrush().color().name() + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellBottomBorderBrush))
                styleString += " border-bottom-color:"_L1 + cellFormat.bottomBorderBrush().color().name() + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellLeftBorderBrush))
                styleString += " border-left-color:"_L1 + cellFormat.leftBorderBrush().color().name() + u';';

            if (cellFormat.hasProperty(BOBUIextFormat::TableCellTopBorderStyle))
                styleString += " border-top-style:"_L1 + richtextBorderStyleToHtmlBorderStyle(cellFormat.topBorderStyle()) + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellRightBorderStyle))
                styleString += " border-right-style:"_L1 + richtextBorderStyleToHtmlBorderStyle(cellFormat.rightBorderStyle()) + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellBottomBorderStyle))
                styleString += " border-bottom-style:"_L1 + richtextBorderStyleToHtmlBorderStyle(cellFormat.bottomBorderStyle()) + u';';
            if (cellFormat.hasProperty(BOBUIextFormat::TableCellLeftBorderStyle))
                styleString += " border-left-style:"_L1 + richtextBorderStyleToHtmlBorderStyle(cellFormat.leftBorderStyle()) + u';';

            if (!styleString.isEmpty())
                html += " style=\""_L1 + styleString + u'\"';

            html += u'>';

            emitFrame(cell.begin());

            html += "</td>"_L1;

            defaultCharFormat = oldDefaultCharFormat;
        }

        html += "</tr>"_L1;
        if (headerRowCount > 0 && row == headerRowCount - 1)
            html += "</thead>"_L1;
    }

    html += "</table>"_L1;
}

void BOBUIextHtmlExporter::emitFrame(const BOBUIextFrame::Iterator &frameIt)
{
    if (!frameIt.atEnd()) {
        BOBUIextFrame::Iterator next = frameIt;
        ++next;
        if (next.atEnd()
            && frameIt.currentFrame() == nullptr
            && frameIt.parentFrame() != doc->rootFrame()
            && frameIt.currentBlock().begin().atEnd())
            return;
    }

    for (BOBUIextFrame::Iterator it = frameIt;
         !it.atEnd(); ++it) {
        if (BOBUIextFrame *f = it.currentFrame()) {
            if (BOBUIextTable *table = qobject_cast<BOBUIextTable *>(f)) {
                emitTable(table);
            } else {
                emitTextFrame(f);
            }
        } else if (it.currentBlock().isValid()) {
            emitBlock(it.currentBlock());
        }
    }
}

void BOBUIextHtmlExporter::emitTextFrame(const BOBUIextFrame *f)
{
    FrameType frameType = f->parentFrame() ? TextFrame : RootFrame;

    html += "\n<table"_L1;
    BOBUIextFrameFormat format = f->frameFormat();

    if (format.hasProperty(BOBUIextFormat::FrameBorder))
        emitAttribute("border", QString::number(format.border()));

    emitFrameStyle(format, frameType);

    emitTextLength("width", format.width());
    emitTextLength("height", format.height());

    // root frame's bcolor goes in the <body> tag
    if (frameType != RootFrame)
        emitBackgroundAttribute(format);

    html += u'>';
    html += "\n<tr>\n<td style=\"border: none;\">"_L1;
    emitFrame(f->begin());
    html += "</td></tr></table>"_L1;
}

void BOBUIextHtmlExporter::emitFrameStyle(const BOBUIextFrameFormat &format, FrameType frameType)
{
    const auto styleAttribute = " style=\""_L1;
    html += styleAttribute;
    const qsizetype originalHtmlLength = html.size();

    if (frameType == TextFrame)
        html += "-bobui-table-type: frame;"_L1;
    else if (frameType == RootFrame)
        html += "-bobui-table-type: root;"_L1;

    const BOBUIextFrameFormat defaultFormat;

    emitFloatStyle(format.position(), OmitStyleTag);
    emitPageBreakPolicy(format.pageBreakPolicy());

    if (format.borderBrush() != defaultFormat.borderBrush()) {
        html += " border-color:"_L1;
        html += colorValue(format.borderBrush().color());
        html += u';';
    }

    if (format.borderStyle() != defaultFormat.borderStyle())
        emitBorderStyle(format.borderStyle());

    if (format.hasProperty(BOBUIextFormat::FrameMargin)
        || format.hasProperty(BOBUIextFormat::FrameLeftMargin)
        || format.hasProperty(BOBUIextFormat::FrameRightMargin)
        || format.hasProperty(BOBUIextFormat::FrameTopMargin)
        || format.hasProperty(BOBUIextFormat::FrameBottomMargin))
        emitMargins(QString::number(format.topMargin()),
                    QString::number(format.bottomMargin()),
                    QString::number(format.leftMargin()),
                    QString::number(format.rightMargin()));

    if (format.property(BOBUIextFormat::TableBorderCollapse).toBool())
        html += " border-collapse:collapse;"_L1;

    if (html.size() == originalHtmlLength) // nothing emitted?
        html.chop(styleAttribute.size());
    else
        html += u'\"';
}

/*!
    Returns a string containing an HTML representation of the document.

    The content of the document specifies its encoding to be UTF-8.
    If you later on convert the returned html string into a byte array for
    transmission over a network or when saving to disk you should use
    QString::toUtf8() to convert the string to a QByteArray.

    \sa {Supported HTML Subset}
*/
#ifndef BOBUI_NO_TEXTHTMLPARSER
QString BOBUIextDocument::toHtml() const
{
    return BOBUIextHtmlExporter(this).toHtml();
}
#endif // BOBUI_NO_TEXTHTMLPARSER

/*!
    \since 5.14
    Returns a string containing a Markdown representation of the document with
    the given \a features, or an empty string if writing fails for any reason.

    \sa setMarkdown
*/
#if BOBUI_CONFIG(textmarkdownwriter)
QString BOBUIextDocument::toMarkdown(BOBUIextDocument::MarkdownFeatures features) const
{
    QString ret;
    BOBUIextStream s(&ret);
    BOBUIextMarkdownWriter w(s, features);
    if (w.writeAll(this))
        return ret;
    return QString();
}
#endif

/*!
    \since 5.14
    \enum BOBUIextDocument::MarkdownFeature

    This enum selects the supported feature set when reading or writing Markdown.

    \value MarkdownNoHTML
        Any HTML tags in the Markdown text will be discarded
    \value MarkdownDialectCommonMark
        Only the features standardized by \l {https://spec.commonmark.org/0.31.2/}{CommonMark}
    \value MarkdownDialectGitHub
        Most features from the
        \l {https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax}
        {GitHub dialect}

    Specifically, the supported subset of the GitHub dialect includes
    everything from CommonMark, plus:

    \list
    \li recognizing URLs, www and email addresses and turning them into links
    \li strikethrough
    \li underline (distinct from italics; in CommonMark it's the same)
    \li tables
    \li task lists
    \li \l {BOBUIextDocument::metaInformation()}{front matter}
    \endlist

    "Front matter" is often metadata in YAML format. BobUI does not currently
    include a parser for that; but you may choose a third-party parser, call
    BOBUIextDocument::metaInformation() to get the whole block, and invoke your
    own parser after BobUI has parsed the Markdown file.

    \note The Markdown output from toMarkdown() currently may include GitHub
    features even if you attempt to disable them by specifying another enum
    value. This may be fixed in a future version of BobUI.

    \sa toMarkdown(), setMarkdown()
*/

/*!
    \since 5.14
    Replaces the entire contents of the document with the given
    Markdown-formatted text in the \a markdown string, with the given
    \a features supported.  By default, all supported GitHub-style
    Markdown features are included; pass \c MarkdownDialectCommonMark
    for a more basic parse.

    The Markdown formatting is respected as much as possible; for example,
    "*bold* text" will produce text where the first word has a font weight that
    gives it an emphasized appearance.

    Parsing of HTML included in the \a markdown string is handled in the same
    way as in \l setHtml; however, Markdown formatting inside HTML blocks is
    not supported.

    Some features of the parser can be enabled or disabled via the \a features
    argument. The default is \c MarkdownDialectGitHub.

    The undo/redo history is reset when this function is called.
*/
#if BOBUI_CONFIG(textmarkdownreader)
void BOBUIextDocument::setMarkdown(const QString &markdown, BOBUIextDocument::MarkdownFeatures features)
{
    BOBUIextMarkdownImporter(this, features).import(markdown);
}
#endif

/*!
    Returns a list of text formats for all the formats used in the document.
*/
QList<BOBUIextFormat> BOBUIextDocument::allFormats() const
{
    Q_D(const BOBUIextDocument);
    return d->formatCollection()->formats;
}

/*!
    \since 4.4
    \fn BOBUIextDocument::undoCommandAdded()

    This signal is emitted  every time a new level of undo is added to the BOBUIextDocument.
*/

BOBUI_END_NAMESPACE

#include "moc_bobuiextdocument.cpp"
