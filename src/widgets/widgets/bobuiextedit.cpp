// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuiextedit_p.h"
#if BOBUI_CONFIG(lineedit)
#include "qlineedit.h"
#endif
#if BOBUI_CONFIG(textbrowser)
#include "bobuiextbrowser.h"
#endif

#include <qfont.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>
#if BOBUI_CONFIG(draganddrop)
#include <qdrag.h>
#endif
#include <qclipboard.h>
#if BOBUI_CONFIG(menu)
#include <qmenu.h>
#endif
#include <qstyle.h>
#if BOBUI_CONFIG(accessibility)
#include <qaccessible.h>
#endif
#include "private/bobuiextdocumentlayout_p.h"
#include "bobuiextdocument.h"
#include "private/bobuiextdocument_p.h"
#include "bobuiextlist.h"
#include "private/qwidgettextcontrol_p.h"

#include <bobuiextformat.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <private/qapplication_p.h>
#include <limits.h>
#include <bobuiextobject.h>
#include <bobuiexttable.h>
#include <qvariant.h>

BOBUI_BEGIN_NAMESPACE

static inline bool shouldEnableInputMethod(BOBUIextEdit *textedit)
{
#if defined (Q_OS_ANDROID)
    return !textedit->isReadOnly() || (textedit->textInteractionFlags() & BobUI::TextSelectableByMouse);
#else
    return !textedit->isReadOnly();
#endif
}

class BOBUIextEditControl : public QWidgetTextControl
{
public:
    inline BOBUIextEditControl(QObject *parent) : QWidgetTextControl(parent) {}

    virtual QMimeData *createMimeDataFromSelection() const override {
        BOBUIextEdit *ed = qobject_cast<BOBUIextEdit *>(parent());
        if (!ed)
            return QWidgetTextControl::createMimeDataFromSelection();
        return ed->createMimeDataFromSelection();
    }
    virtual bool canInsertFromMimeData(const QMimeData *source) const override {
        BOBUIextEdit *ed = qobject_cast<BOBUIextEdit *>(parent());
        if (!ed)
            return QWidgetTextControl::canInsertFromMimeData(source);
        return ed->canInsertFromMimeData(source);
    }
    virtual void insertFromMimeData(const QMimeData *source) override {
        BOBUIextEdit *ed = qobject_cast<BOBUIextEdit *>(parent());
        if (!ed)
            QWidgetTextControl::insertFromMimeData(source);
        else
            ed->insertFromMimeData(source);
    }
    QVariant loadResource(int type, const QUrl &name) override {
        auto *ed = qobject_cast<BOBUIextEdit *>(parent());
        if (!ed)
            return QWidgetTextControl::loadResource(type, name);

        QUrl resolvedName = ed->d_func()->resolveUrl(name);
        return ed->loadResource(type, resolvedName);
    }
};

BOBUIextEditPrivate::BOBUIextEditPrivate()
    : control(nullptr),
      autoFormatting(BOBUIextEdit::AutoNone), tabChangesFocus(false),
      lineWrap(BOBUIextEdit::WidgetWidth), lineWrapColumnOrWidth(0),
      wordWrap(BOBUIextOption::WrapAtWordBoundaryOrAnywhere), clickCausedFocus(0)
{
    ignoreAutomaticScrollbarAdjustment = false;
    preferRichText = false;
    showCursorOnInitialShow = true;
    inDrag = false;
}

BOBUIextEditPrivate::~BOBUIextEditPrivate()
{
    for (const QMetaObject::Connection &connection : connections)
        QObject::disconnect(connection);
}

void BOBUIextEditPrivate::createAutoBulletList()
{
    BOBUIextCursor cursor = control->textCursor();
    cursor.beginEditBlock();

    BOBUIextBlockFormat blockFmt = cursor.blockFormat();

    BOBUIextListFormat listFmt;
    listFmt.setStyle(BOBUIextListFormat::ListDisc);
    listFmt.setIndent(blockFmt.indent() + 1);

    blockFmt.setIndent(0);
    cursor.setBlockFormat(blockFmt);

    cursor.createList(listFmt);

    cursor.endEditBlock();
    control->setTextCursor(cursor);
}

void BOBUIextEditPrivate::init(const QString &html)
{
    Q_Q(BOBUIextEdit);
    control = new BOBUIextEditControl(q);
    control->setPalette(q->palette());

    connections = {
        QObjectPrivate::connect(control, &BOBUIextEditControl::documentSizeChanged,
                                this, &BOBUIextEditPrivate::adjustScrollbars),
        QObjectPrivate::connect(control, &BOBUIextEditControl::updateRequest,
                                this, &BOBUIextEditPrivate::repaintContents),
        QObjectPrivate::connect(control, &BOBUIextEditControl::visibilityRequest,
                                this, &BOBUIextEditPrivate::ensureVisible),
        QObjectPrivate::connect(control, &BOBUIextEditControl::blockMarkerHovered,
                                this, &BOBUIextEditPrivate::hoveredBlockWithMarkerChanged),
        QObjectPrivate::connect(control, &BOBUIextEditControl::cursorPositionChanged,
                                this, &BOBUIextEditPrivate::cursorPositionChanged),
        QObject::connect(control, &BOBUIextEditControl::microFocusChanged,
                         q, [q]() { q->updateMicroFocus(); }),
        QObject::connect(control, &BOBUIextEditControl::currentCharFormatChanged,
                         q, &BOBUIextEdit::currentCharFormatChanged),
        QObject::connect(control, &BOBUIextEditControl::textChanged,
                         q, &BOBUIextEdit::textChanged),
        QObject::connect(control, &BOBUIextEditControl::undoAvailable,
                         q, &BOBUIextEdit::undoAvailable),
        QObject::connect(control, &BOBUIextEditControl::redoAvailable,
                         q, &BOBUIextEdit::redoAvailable),
        QObject::connect(control, &BOBUIextEditControl::copyAvailable,
                         q, &BOBUIextEdit::copyAvailable),
        QObject::connect(control, &BOBUIextEditControl::selectionChanged,
                         q, &BOBUIextEdit::selectionChanged),
        QObject::connect(control, &BOBUIextEditControl::textChanged,
                         q, [q]() { q->updateMicroFocus(); }),
    };

    BOBUIextDocument *doc = control->document();
    // set a null page size initially to avoid any relayouting until the textedit
    // is shown. relayoutDocument() will take care of setting the page size to the
    // viewport dimensions later.
    doc->setPageSize(QSize(0, 0));
    doc->documentLayout()->setPaintDevice(viewport);
    doc->setDefaultFont(q->font());
    doc->setUndoRedoEnabled(false); // flush undo buffer.
    doc->setUndoRedoEnabled(true);

    if (!html.isEmpty())
        control->setHtml(html);

    const auto singleStep = defaultSingleStep();
    hbar->setSingleStep(singleStep);
    vbar->setSingleStep(singleStep);

    viewport->setBackgroundRole(QPalette::Base);
    q->setMouseTracking(true);
    q->setAcceptDrops(true);
    q->setFocusPolicy(BobUI::StrongFocus);
    q->setAttribute(BobUI::WA_KeyCompression);
    q->setAttribute(BobUI::WA_InputMethodEnabled);
    q->setInputMethodHints(BobUI::ImhMultiLine);
#ifndef BOBUI_NO_CURSOR
    viewport->setCursor(BobUI::IBeamCursor);
#endif
}

void BOBUIextEditPrivate::repaintContents(const QRectF &contentsRect)
{
    if (!contentsRect.isValid()) {
        viewport->update();
        return;
    }
    const int xOffset = horizontalOffset();
    const int yOffset = verticalOffset();
    const QRectF visibleRect(xOffset, yOffset, viewport->width(), viewport->height());

    QRect r = contentsRect.intersected(visibleRect).toAlignedRect();
    if (r.isEmpty())
        return;

    r.translate(-xOffset, -yOffset);
    viewport->update(r);
}

void BOBUIextEditPrivate::cursorPositionChanged()
{
    Q_Q(BOBUIextEdit);
    emit q->cursorPositionChanged();
#if BOBUI_CONFIG(accessibility)
    QAccessibleTextCursorEvent event(q, q->textCursor().position());
    QAccessible::updateAccessibility(&event);
#endif
}

void BOBUIextEditPrivate::hoveredBlockWithMarkerChanged(const BOBUIextBlock &block)
{
#if BOBUI_CONFIG(cursor)
    Q_Q(BOBUIextEdit);
    BobUI::CursorShape cursor = cursorToRestoreAfterHover;
    if (block.isValid() && !q->isReadOnly()) {
        BOBUIextBlockFormat::MarkerType marker = block.blockFormat().marker();
        if (marker != BOBUIextBlockFormat::MarkerType::NoMarker) {
            if (viewport->cursor().shape() != BobUI::PointingHandCursor)
                cursorToRestoreAfterHover = viewport->cursor().shape();
            cursor = BobUI::PointingHandCursor;
        }
    }
    viewport->setCursor(cursor);
#endif
}

void BOBUIextEditPrivate::pageUpDown(BOBUIextCursor::MoveOperation op, BOBUIextCursor::MoveMode moveMode)
{
    BOBUIextCursor cursor = control->textCursor();
    bool moved = false;
    qreal lastY = control->cursorRect(cursor).top();
    qreal distance = 0;
    // move using movePosition to keep the cursor's x
    do {
        qreal y = control->cursorRect(cursor).top();
        distance += qAbs(y - lastY);
        lastY = y;
        moved = cursor.movePosition(op, moveMode);
    } while (moved && distance < viewport->height());

    if (moved) {
        if (op == BOBUIextCursor::Up) {
            cursor.movePosition(BOBUIextCursor::Down, moveMode);
            vbar->triggerAction(QAbstractSlider::SliderPageStepSub);
        } else {
            cursor.movePosition(BOBUIextCursor::Up, moveMode);
            vbar->triggerAction(QAbstractSlider::SliderPageStepAdd);
        }
    }
    control->setTextCursor(cursor, moveMode == BOBUIextCursor::KeepAnchor);
}

#if BOBUI_CONFIG(scrollbar)
static QSize documentSize(QWidgetTextControl *control)
{
    BOBUIextDocument *doc = control->document();
    QAbstractTextDocumentLayout *layout = doc->documentLayout();

    QSize docSize;

    if (BOBUIextDocumentLayout *tlayout = qobject_cast<BOBUIextDocumentLayout *>(layout)) {
        docSize = tlayout->dynamicDocumentSize().toSize();
        int percentageDone = tlayout->layoutStatus();
        // extrapolate height
        if (percentageDone > 0)
            docSize.setHeight(docSize.height() * 100 / percentageDone);
    } else {
        docSize = layout->documentSize().toSize();
    }

    return docSize;
}

void BOBUIextEditPrivate::adjustScrollbars()
{
    if (ignoreAutomaticScrollbarAdjustment)
        return;
    ignoreAutomaticScrollbarAdjustment = true; // avoid recursion, #106108

    QSize viewportSize = viewport->size();
    QSize docSize = documentSize(control);

    // due to the recursion guard we have to repeat this step a few times,
    // as adding/removing a scroll bar will cause the document or viewport
    // size to change
    // ideally we should loop until the viewport size and doc size stabilize,
    // but in corner cases they might fluctuate, so we need to limit the
    // number of iterations
    for (int i = 0; i < 4; ++i) {
        hbar->setRange(0, docSize.width() - viewportSize.width());
        hbar->setPageStep(viewportSize.width());

        vbar->setRange(0, docSize.height() - viewportSize.height());
        vbar->setPageStep(viewportSize.height());

        // if we are in left-to-right mode widening the document due to
        // lazy layouting does not require a repaint. If in right-to-left
        // the scroll bar has the value zero and it visually has the maximum
        // value (it is visually at the right), then widening the document
        // keeps it at value zero but visually adjusts it to the new maximum
        // on the right, hence we need an update.
        if (q_func()->isRightToLeft())
            viewport->update();

        _q_showOrHideScrollBars();

        const QSize oldViewportSize = viewportSize;
        const QSize oldDocSize = docSize;

        // make sure the document is layouted if the viewport width changes
        viewportSize = viewport->size();
        if (viewportSize.width() != oldViewportSize.width())
            relayoutDocument();

        docSize = documentSize(control);
        if (viewportSize == oldViewportSize && docSize == oldDocSize)
            break;
    }
    ignoreAutomaticScrollbarAdjustment = false;
}
#endif

// rect is in content coordinates
void BOBUIextEditPrivate::ensureVisible(const QRectF &_rect)
{
    const QRect rect = _rect.toRect();
    if ((vbar->isVisible() && vbar->maximum() < rect.bottom())
        || (hbar->isVisible() && hbar->maximum() < rect.right()))
        adjustScrollbars();
    const int visibleWidth = viewport->width();
    const int visibleHeight = viewport->height();
    const bool rtl = q_func()->isRightToLeft();

    if (rect.x() < horizontalOffset()) {
        if (rtl)
            hbar->setValue(hbar->maximum() - rect.x());
        else
            hbar->setValue(rect.x());
    } else if (rect.x() + rect.width() > horizontalOffset() + visibleWidth) {
        if (rtl)
            hbar->setValue(hbar->maximum() - (rect.x() + rect.width() - visibleWidth));
        else
            hbar->setValue(rect.x() + rect.width() - visibleWidth);
    }

    if (rect.y() < verticalOffset())
        vbar->setValue(rect.y());
    else if (rect.y() + rect.height() > verticalOffset() + visibleHeight)
        vbar->setValue(rect.y() + rect.height() - visibleHeight);
}

/*!
    \class BOBUIextEdit
    \brief The BOBUIextEdit class provides a widget that is used to edit and display
    both plain and rich text.

    \ingroup richtext-processing
    \inmodule BobUIWidgets

    \section1 Introduction and Concepts

    BOBUIextEdit is an advanced WYSIWYG viewer/editor supporting rich
    text formatting using HTML-style tags, or Markdown format. It is optimized
    to handle large documents and to respond quickly to user input.

    BOBUIextEdit works on paragraphs and characters. A paragraph is a
    formatted string which is word-wrapped to fit into the width of
    the widget. By default when reading plain text, one newline
    signifies a paragraph. A document consists of zero or more
    paragraphs. The words in the paragraph are aligned in accordance
    with the paragraph's alignment. Paragraphs are separated by hard
    line breaks. Each character within a paragraph has its own
    attributes, for example, font and color.

    BOBUIextEdit can display images, lists and tables. If the text is
    too large to view within the text edit's viewport, scroll bars will
    appear. The text edit can load both plain text and rich text files.
    Rich text can be described using a subset of HTML 4 markup; refer to the
    \l {Supported HTML Subset} page for more information.

    If you just need to display a small piece of rich text use QLabel.

    The rich text support in BobUI is designed to provide a fast, portable and
    efficient way to add reasonable online help facilities to
    applications, and to provide a basis for rich text editors. If
    you find the HTML support insufficient for your needs you may consider
    the use of BobUI WebKit, which provides a full-featured web browser
    widget.

    The shape of the mouse cursor on a BOBUIextEdit is BobUI::IBeamCursor by default.
    It can be changed through the viewport()'s cursor property.

    \section1 Using BOBUIextEdit as a Display Widget

    BOBUIextEdit can display a large HTML subset, including tables and
    images.

    The text can be set or replaced using \l setHtml() which deletes any
    existing text and replaces it with the text passed in the
    setHtml() call. If you call setHtml() with legacy HTML, and then
    call toHtml(), the text that is returned may have different markup,
    but will render the same. The entire text can be deleted with clear().

    Text can also be set or replaced using \l setMarkdown(), and the same
    caveats apply: if you then call \l toMarkdown(), the text that is returned
    may be different, but the meaning is preserved as much as possible.
    Markdown with some embedded HTML can be parsed, with the same limitations
    that \l setHtml() has; but \l toMarkdown() only writes "pure" Markdown,
    without any embedded HTML.

    Text itself can be inserted using the BOBUIextCursor class or using the
    convenience functions insertHtml(), insertPlainText(), append() or
    paste(). BOBUIextCursor is also able to insert complex objects like tables
    or lists into the document, and it deals with creating selections
    and applying changes to selected text.

    By default the text edit wraps words at whitespace to fit within
    the text edit widget. The setLineWrapMode() function is used to
    specify the kind of line wrap you want, or \l NoWrap if you don't
    want any wrapping. Call setLineWrapMode() to set a fixed pixel width
    \l FixedPixelWidth, or character column (e.g. 80 column) \l
    FixedColumnWidth with the pixels or columns specified with
    setLineWrapColumnOrWidth(). If you use word wrap to the widget's width
    \l WidgetWidth, you can specify whether to break on whitespace or
    anywhere with setWordWrapMode().

    The find() function can be used to find and select a given string
    within the text.

    If you want to limit the total number of paragraphs in a BOBUIextEdit,
    as for example it is often useful in a log viewer, then you can use
    BOBUIextDocument's maximumBlockCount property for that.

    \section2 Read-only Key Bindings

    When BOBUIextEdit is used read-only the key bindings are limited to
    navigation, and text may only be selected with the mouse:
    \table
    \header \li Keypresses \li Action
    \row \li Up        \li Moves one line up.
    \row \li Down        \li Moves one line down.
    \row \li Left        \li Moves one character to the left.
    \row \li Right        \li Moves one character to the right.
    \row \li PageUp        \li Moves one (viewport) page up.
    \row \li PageDown        \li Moves one (viewport) page down.
    \row \li Home        \li Moves to the beginning of the text.
    \row \li End                \li Moves to the end of the text.
    \row \li Alt+Wheel
         \li Scrolls the page horizontally (the Wheel is the mouse wheel).
    \row \li Ctrl+Wheel        \li Zooms the text.
    \row \li Ctrl+A            \li Selects all text.
    \endtable

    The text edit may be able to provide some meta-information. For
    example, the documentTitle() function will return the text from
    within HTML \c{<title>} tags.

    \note Zooming into HTML documents only works if the font-size is not set to a fixed size.

    \section1 Using BOBUIextEdit as an Editor

    All the information about using BOBUIextEdit as a display widget also
    applies here.

    The current char format's attributes are set with setFontItalic(),
    setFontWeight(), setFontUnderline(), setFontFamily(),
    setFontPointSize(), setTextColor() and setCurrentFont(). The current
    paragraph's alignment is set with setAlignment().

    Selection of text is handled by the BOBUIextCursor class, which provides
    functionality for creating selections, retrieving the text contents or
    deleting selections. You can retrieve the object that corresponds with
    the user-visible cursor using the textCursor() method. If you want to set
    a selection in BOBUIextEdit just create one on a BOBUIextCursor object and
    then make that cursor the visible cursor using setTextCursor(). The selection
    can be copied to the clipboard with copy(), or cut to the clipboard with
    cut(). The entire text can be selected using selectAll().

    When the cursor is moved and the underlying formatting attributes change,
    the currentCharFormatChanged() signal is emitted to reflect the new attributes
    at the new cursor position.

    The textChanged() signal is emitted whenever the text changes (as a result
    of setText() or through the editor itself).

    BOBUIextEdit holds a BOBUIextDocument object which can be retrieved using the
    document() method. You can also set your own document object using setDocument().

    BOBUIextDocument provides an \l {BOBUIextDocument::isModified()}{isModified()}
    function which will return true if the text has been modified since it was
    either loaded or since the last call to setModified with false as argument.
    In addition it provides methods for undo and redo.

    \section2 Drag and Drop

    BOBUIextEdit also supports custom drag and drop behavior. By default,
    BOBUIextEdit will insert plain text, HTML and rich text when the user drops
    data of these MIME types onto a document. Reimplement
    canInsertFromMimeData() and insertFromMimeData() to add support for
    additional MIME types.

    For example, to allow the user to drag and drop an image onto a BOBUIextEdit,
    you could the implement these functions in the following way:

    \snippet textdocument-imagedrop/textedit.cpp 0

    We add support for image MIME types by returning true. For all other
    MIME types, we use the default implementation.

    \snippet textdocument-imagedrop/textedit.cpp 1

    We unpack the image from the QVariant held by the MIME source and insert
    it into the document as a resource.

    \section2 Editing Key Bindings

    The list of key bindings which are implemented for editing:
    \table
    \header \li Keypresses \li Action
    \row \li Backspace \li Deletes the character to the left of the cursor.
    \row \li Delete \li Deletes the character to the right of the cursor.
    \row \li Ctrl+C \li Copy the selected text to the clipboard.
    \row \li Ctrl+Insert \li Copy the selected text to the clipboard.
    \row \li Ctrl+K \li Deletes to the end of the line.
    \row \li Ctrl+V \li Pastes the clipboard text into text edit.
    \row \li Shift+Insert \li Pastes the clipboard text into text edit.
    \row \li Ctrl+X \li Deletes the selected text and copies it to the clipboard.
    \row \li Shift+Delete \li Deletes the selected text and copies it to the clipboard.
    \row \li Ctrl+Z \li Undoes the last operation.
    \row \li Ctrl+Y \li Redoes the last operation.
    \row \li Left \li Moves the cursor one character to the left.
    \row \li Ctrl+Left \li Moves the cursor one word to the left.
    \row \li Right \li Moves the cursor one character to the right.
    \row \li Ctrl+Right \li Moves the cursor one word to the right.
    \row \li Up \li Moves the cursor one line up.
    \row \li Down \li Moves the cursor one line down.
    \row \li PageUp \li Moves the cursor one page up.
    \row \li PageDown \li Moves the cursor one page down.
    \row \li Home \li Moves the cursor to the beginning of the line.
    \row \li Ctrl+Home \li Moves the cursor to the beginning of the text.
    \row \li End \li Moves the cursor to the end of the line.
    \row \li Ctrl+End \li Moves the cursor to the end of the text.
    \row \li Alt+Wheel \li Scrolls the page horizontally (the Wheel is the mouse wheel).
    \endtable

    To select (mark) text hold down the Shift key whilst pressing one
    of the movement keystrokes, for example, \e{Shift+Right}
    will select the character to the right, and \e{Shift+Ctrl+Right} will select the word to the right, etc.

    \sa BOBUIextDocument, BOBUIextCursor,
        {Syntax Highlighter Example}, {Rich Text Processing}
*/

/*!
    \property BOBUIextEdit::plainText
    \since 4.3

    \brief the text editor's contents as plain text.

    Previous contents are removed and undo/redo history is reset
    when the property is set. currentCharFormat() is also reset, unless
    textCursor() is already at the beginning of the document.

    If the text edit has another content type, it will not be replaced
    by plain text if you call toPlainText(). The only exception to this
    is the non-break space, \e{nbsp;}, that will be converted into
    standard space.

    By default, for an editor with no contents, this property contains
    an empty string.

    \sa html
*/

/*!
    \property BOBUIextEdit::undoRedoEnabled
    \brief whether undo and redo are enabled.

    Users are only able to undo or redo actions if this property is
    true, and if there is an action that can be undone (or redone).
*/

/*!
    \enum BOBUIextEdit::LineWrapMode

    \value NoWrap
    \value WidgetWidth
    \value FixedPixelWidth
    \value FixedColumnWidth
*/

/*!
    \enum BOBUIextEdit::AutoFormattingFlag

    \value AutoNone Don't do any automatic formatting.
    \value AutoBulletList Automatically create bullet lists (e.g. when
    the user enters an asterisk ('*') in the left most column, or
    presses Enter in an existing list item.
    \value AutoAll Apply all automatic formatting. Currently only
    automatic bullet lists are supported.
*/


/*!
    Constructs an empty BOBUIextEdit with parent \a
    parent.
*/
BOBUIextEdit::BOBUIextEdit(QWidget *parent)
    : QAbstractScrollArea(*new BOBUIextEditPrivate, parent)
{
    Q_D(BOBUIextEdit);
    d->init();
}

/*!
    \internal
*/
BOBUIextEdit::BOBUIextEdit(BOBUIextEditPrivate &dd, QWidget *parent)
    : QAbstractScrollArea(dd, parent)
{
    Q_D(BOBUIextEdit);
    d->init();
}

/*!
    Constructs a BOBUIextEdit with parent \a parent. The text edit will display
    the text \a text. The text is interpreted as html.
*/
BOBUIextEdit::BOBUIextEdit(const QString &text, QWidget *parent)
    : QAbstractScrollArea(*new BOBUIextEditPrivate, parent)
{
    Q_D(BOBUIextEdit);
    d->init(text);
}



/*!
    Destructor.
*/
BOBUIextEdit::~BOBUIextEdit()
{
}

/*!
    Returns the point size of the font of the current format.

    \sa setFontFamily(), setCurrentFont(), setFontPointSize()
*/
qreal BOBUIextEdit::fontPointSize() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().charFormat().fontPointSize();
}

/*!
    Returns the font family of the current format.

    \sa setFontFamily(), setCurrentFont(), setFontPointSize()
*/
QString BOBUIextEdit::fontFamily() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().charFormat().fontFamilies().toStringList().value(0, QString());
}

/*!
    Returns the font weight of the current format.

    \sa setFontWeight(), setCurrentFont(), setFontPointSize(), QFont::Weight
*/
int BOBUIextEdit::fontWeight() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().charFormat().fontWeight();
}

/*!
    Returns \c true if the font of the current format is underlined; otherwise returns
    false.

    \sa setFontUnderline()
*/
bool BOBUIextEdit::fontUnderline() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().charFormat().fontUnderline();
}

/*!
    Returns \c true if the font of the current format is italic; otherwise returns
    false.

    \sa setFontItalic()
*/
bool BOBUIextEdit::fontItalic() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().charFormat().fontItalic();
}

/*!
    Returns the text color of the current format.

    \sa setTextColor()
*/
QColor BOBUIextEdit::textColor() const
{
    Q_D(const BOBUIextEdit);

    const auto fg = d->control->textCursor().charFormat().foreground();
    if (fg.style() == BobUI::NoBrush) {
        const auto context = d->control->getPaintContext(const_cast<BOBUIextEdit *>(this));
        return context.palette.color(QPalette::Text);
    }

    return fg.color();
}

/*!
    \since 4.4

    Returns the text background color of the current format.

    \sa setTextBackgroundColor()
*/
QColor BOBUIextEdit::textBackgroundColor() const
{
    Q_D(const BOBUIextEdit);
    const QBrush &brush = d->control->textCursor().charFormat().background();
    return brush.style() == BobUI::NoBrush ? BobUI::transparent : brush.color();
}

/*!
    Returns the font of the current format.

    \sa setCurrentFont(), setFontFamily(), setFontPointSize()
*/
QFont BOBUIextEdit::currentFont() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().charFormat().font();
}

/*!
    Sets the alignment of the current paragraph to \a a. Valid
    alignments are BobUI::AlignLeft, BobUI::AlignRight,
    BobUI::AlignJustify and BobUI::AlignCenter (which centers
    horizontally).
*/
void BOBUIextEdit::setAlignment(BobUI::Alignment a)
{
    Q_D(BOBUIextEdit);
    BOBUIextBlockFormat fmt;
    fmt.setAlignment(a);
    BOBUIextCursor cursor = d->control->textCursor();
    cursor.mergeBlockFormat(fmt);
    d->control->setTextCursor(cursor);
    d->relayoutDocument();
}

/*!
    Returns the alignment of the current paragraph.

    \sa setAlignment()
*/
BobUI::Alignment BOBUIextEdit::alignment() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor().blockFormat().alignment();
}

/*!
    \property BOBUIextEdit::document
    \brief the underlying document of the text editor.

    \note The editor \e{does not take ownership of the document} unless it
    is the document's parent object. The parent object of the provided document
    remains the owner of the object. If the previously assigned document is a
    child of the editor then it will be deleted.
*/
void BOBUIextEdit::setDocument(BOBUIextDocument *document)
{
    Q_D(BOBUIextEdit);
    d->control->setDocument(document);
    d->updateDefaultTextOption();
    d->relayoutDocument();
}

BOBUIextDocument *BOBUIextEdit::document() const
{
    Q_D(const BOBUIextEdit);
    return d->control->document();
}

/*!
    \since 5.2

    \property BOBUIextEdit::placeholderText
    \brief the editor placeholder text

    Setting this property makes the editor display a grayed-out
    placeholder text as long as the document() is empty.

    By default, this property contains an empty string.

    \sa document()
*/
QString BOBUIextEdit::placeholderText() const
{
    Q_D(const BOBUIextEdit);
    return d->placeholderText;
}

void BOBUIextEdit::setPlaceholderText(const QString &placeholderText)
{
    Q_D(BOBUIextEdit);
    if (d->placeholderText != placeholderText) {
        d->placeholderText = placeholderText;
        if (d->control->document()->isEmpty())
            d->viewport->update();
    }
}

/*!
    Sets the visible \a cursor.
*/
void BOBUIextEdit::setTextCursor(const BOBUIextCursor &cursor)
{
    doSetTextCursor(cursor);
}

/*!
    \internal

     This provides a hook for subclasses to intercept cursor changes.
*/

void BOBUIextEdit::doSetTextCursor(const BOBUIextCursor &cursor)
{
    Q_D(BOBUIextEdit);
    d->control->setTextCursor(cursor);
}

/*!
    Returns a copy of the BOBUIextCursor that represents the currently visible cursor.
    Note that changes on the returned cursor do not affect BOBUIextEdit's cursor; use
    setTextCursor() to update the visible cursor.
 */
BOBUIextCursor BOBUIextEdit::textCursor() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textCursor();
}

/*!
    Sets the font family of the current format to \a fontFamily.

    \sa fontFamily(), setCurrentFont()
*/
void BOBUIextEdit::setFontFamily(const QString &fontFamily)
{
    BOBUIextCharFormat fmt;
    fmt.setFontFamilies({fontFamily});
    mergeCurrentCharFormat(fmt);
}

/*!
    Sets the point size of the current format to \a s.

    Note that if \a s is zero or negative, the behavior of this
    function is not defined.

    \sa fontPointSize(), setCurrentFont(), setFontFamily()
*/
void BOBUIextEdit::setFontPointSize(qreal s)
{
    BOBUIextCharFormat fmt;
    fmt.setFontPointSize(s);
    mergeCurrentCharFormat(fmt);
}

/*!
    \fn void BOBUIextEdit::setFontWeight(int weight)

    Sets the font weight of the current format to the given \a weight,
    where the value used is in the range defined by the QFont::Weight
    enum.

    \sa fontWeight(), setCurrentFont(), setFontFamily()
*/
void BOBUIextEdit::setFontWeight(int w)
{
    BOBUIextCharFormat fmt;
    fmt.setFontWeight(w);
    mergeCurrentCharFormat(fmt);
}

/*!
    If \a underline is true, sets the current format to underline;
    otherwise sets the current format to non-underline.

    \sa fontUnderline()
*/
void BOBUIextEdit::setFontUnderline(bool underline)
{
    BOBUIextCharFormat fmt;
    fmt.setFontUnderline(underline);
    mergeCurrentCharFormat(fmt);
}

/*!
    If \a italic is true, sets the current format to italic;
    otherwise sets the current format to non-italic.

    \sa fontItalic()
*/
void BOBUIextEdit::setFontItalic(bool italic)
{
    BOBUIextCharFormat fmt;
    fmt.setFontItalic(italic);
    mergeCurrentCharFormat(fmt);
}

/*!
    Sets the text color of the current format to \a c.

    \sa textColor()
*/
void BOBUIextEdit::setTextColor(const QColor &c)
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(QBrush(c));
    mergeCurrentCharFormat(fmt);
}

/*!
    \since 4.4

    Sets the text background color of the current format to \a c.

    \sa textBackgroundColor()
*/
void BOBUIextEdit::setTextBackgroundColor(const QColor &c)
{
    BOBUIextCharFormat fmt;
    fmt.setBackground(QBrush(c));
    mergeCurrentCharFormat(fmt);
}

/*!
    Sets the font of the current format to \a f.

    \sa currentFont(), setFontPointSize(), setFontFamily()
*/
void BOBUIextEdit::setCurrentFont(const QFont &f)
{
    BOBUIextCharFormat fmt;
    fmt.setFont(f);
    mergeCurrentCharFormat(fmt);
}

/*!
    \since 4.2

    Undoes the last operation.

    If there is no operation to undo, i.e. there is no undo step in
    the undo/redo history, nothing happens.

    \sa redo()
*/
void BOBUIextEdit::undo()
{
    Q_D(BOBUIextEdit);
    d->control->undo();
}

void BOBUIextEdit::redo()
{
    Q_D(BOBUIextEdit);
    d->control->redo();
}

/*!
    \fn void BOBUIextEdit::redo()
    \since 4.2

    Redoes the last operation.

    If there is no operation to redo, i.e. there is no redo step in
    the undo/redo history, nothing happens.

    \sa undo()
*/

#ifndef BOBUI_NO_CLIPBOARD
/*!
    Copies the selected text to the clipboard and deletes it from
    the text edit.

    If there is no selected text nothing happens.

    \sa copy(), paste()
*/

void BOBUIextEdit::cut()
{
    Q_D(BOBUIextEdit);
    d->control->cut();
}

/*!
    Copies any selected text to the clipboard.

    \sa copyAvailable()
*/

void BOBUIextEdit::copy()
{
    Q_D(BOBUIextEdit);
    d->control->copy();
}

/*!
    Pastes the text from the clipboard into the text edit at the
    current cursor position.

    If there is no text in the clipboard nothing happens.

    To change the behavior of this function, i.e. to modify what
    BOBUIextEdit can paste and how it is being pasted, reimplement the
    virtual canInsertFromMimeData() and insertFromMimeData()
    functions.

    \sa cut(), copy()
*/

void BOBUIextEdit::paste()
{
    Q_D(BOBUIextEdit);
    d->control->paste();
}
#endif

/*!
    Deletes all the text in the text edit.

    Notes:
    \list
    \li The undo/redo history is also cleared.
    \li currentCharFormat() is reset, unless textCursor()
    is already at the beginning of the document.
    \endlist

    \sa cut(), setPlainText(), setHtml()
*/
void BOBUIextEdit::clear()
{
    Q_D(BOBUIextEdit);
    // clears and sets empty content
    d->control->clear();
}


/*!
    Selects all text.

    \sa copy(), cut(), textCursor()
 */
void BOBUIextEdit::selectAll()
{
    Q_D(BOBUIextEdit);
    d->control->selectAll();
}

/*! \internal
*/
bool BOBUIextEdit::event(QEvent *e)
{
    Q_D(BOBUIextEdit);
    switch (e->type()) {
    case QEvent::ShortcutOverride:
    case QEvent::ToolTip:
        d->sendControlEvent(e);
        break;
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
        d->control->setPalette(palette());
        break;
#ifndef BOBUI_NO_CONTEXTMENU
    case QEvent::ContextMenu:
        if (static_cast<QContextMenuEvent *>(e)->reason() == QContextMenuEvent::Keyboard) {
            ensureCursorVisible();
            const QPoint cursorPos = cursorRect().center();
            QContextMenuEvent ce(QContextMenuEvent::Keyboard, cursorPos, d->viewport->mapToGlobal(cursorPos));
            ce.setAccepted(e->isAccepted());
            const bool result = QAbstractScrollArea::event(&ce);
            e->setAccepted(ce.isAccepted());
            return result;
        }
        break;
#endif // BOBUI_NO_CONTEXTMENU
#ifdef BOBUI_KEYPAD_NAVIGATION
    case QEvent::EnterEditFocus:
    case QEvent::LeaveEditFocus:
        if (QApplicationPrivate::keypadNavigationEnabled())
            d->sendControlEvent(e);
        break;
#endif
    default:
        break;
    }
    return QAbstractScrollArea::event(e);
}

/*! \internal
*/

void BOBUIextEdit::timerEvent(BOBUIimerEvent *e)
{
    Q_D(BOBUIextEdit);
    if (e->timerId() == d->autoScrollTimer.timerId()) {
        QRect visible = d->viewport->rect();
        QPoint pos;
        if (d->inDrag) {
            pos = d->autoScrollDragPos;
            visible.adjust(qMin(visible.width()/3,20), qMin(visible.height()/3,20),
                           -qMin(visible.width()/3,20), -qMin(visible.height()/3,20));
        } else {
            const QPoint globalPos = QCursor::pos();
            pos = d->viewport->mapFromGlobal(globalPos);
            QMouseEvent ev(QEvent::MouseMove, pos, mapTo(topLevelWidget(), pos), globalPos, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier);
            mouseMoveEvent(&ev);
        }
        int deltaY = qMax(pos.y() - visible.top(), visible.bottom() - pos.y()) - visible.height();
        int deltaX = qMax(pos.x() - visible.left(), visible.right() - pos.x()) - visible.width();
        int delta = qMax(deltaX, deltaY);
        if (delta >= 0) {
            if (delta < 7)
                delta = 7;
            int timeout = 4900 / (delta * delta);
            d->autoScrollTimer.start(timeout, this);

            if (deltaY > 0)
                d->vbar->triggerAction(pos.y() < visible.center().y() ?
                                       QAbstractSlider::SliderSingleStepSub
                                       : QAbstractSlider::SliderSingleStepAdd);
            if (deltaX > 0)
                d->hbar->triggerAction(pos.x() < visible.center().x() ?
                                       QAbstractSlider::SliderSingleStepSub
                                       : QAbstractSlider::SliderSingleStepAdd);
        }
    }
#ifdef BOBUI_KEYPAD_NAVIGATION
    else if (e->timerId() == d->deleteAllTimer.timerId()) {
        d->deleteAllTimer.stop();
        clear();
    }
#endif
}

/*!
    Changes the text of the text edit to the string \a text.
    Any previous text is removed.

    Notes:
    \list
    \li \a text is interpreted as plain text.
    \li The undo/redo history is also cleared.
    \li currentCharFormat() is reset, unless textCursor()
    is already at the beginning of the document.
    \endlist

    \sa toPlainText()
*/

void BOBUIextEdit::setPlainText(const QString &text)
{
    Q_D(BOBUIextEdit);
    d->control->setPlainText(text);
    d->preferRichText = false;
}

/*!
    QString BOBUIextEdit::toPlainText() const

    Returns the text of the text edit as plain text.

    \sa BOBUIextEdit::setPlainText()
 */
QString BOBUIextEdit::toPlainText() const
{
    Q_D(const BOBUIextEdit);
    return d->control->toPlainText();
}

/*!
    \property BOBUIextEdit::html

    This property provides an HTML interface to the text of the text edit.

    toHtml() returns the text of the text edit as html.

    setHtml() changes the text of the text edit.  Any previous text is
    removed and the undo/redo history is cleared. The input text is
    interpreted as rich text in html format. currentCharFormat() is also
    reset, unless textCursor() is already at the beginning of the document.

    \note It is the responsibility of the caller to make sure that the
    text is correctly decoded when a QString containing HTML is created
    and passed to setHtml().

    By default, for a newly-created, empty document, this property contains
    text to describe an HTML 4.0 document with no body text.

    \sa {Supported HTML Subset}, plainText
*/

#ifndef BOBUI_NO_TEXTHTMLPARSER
void BOBUIextEdit::setHtml(const QString &text)
{
    Q_D(BOBUIextEdit);
    d->control->setHtml(text);
    d->preferRichText = true;
}

QString BOBUIextEdit::toHtml() const
{
    Q_D(const BOBUIextEdit);
    return d->control->toHtml();
}
#endif

#if BOBUI_CONFIG(textmarkdownreader) && BOBUI_CONFIG(textmarkdownwriter)
/*!
    \property BOBUIextEdit::markdown

    This property provides a Markdown interface to the text of the text edit.

    \c toMarkdown() returns the text of the text edit as "pure" Markdown,
    without any embedded HTML formatting. Some features that BOBUIextDocument
    supports (such as the use of specific colors and named fonts) cannot be
    expressed in "pure" Markdown, and they will be omitted.

    \c setMarkdown() changes the text of the text edit.  Any previous text is
    removed and the undo/redo history is cleared. The input text is
    interpreted as rich text in Markdown format.

    Parsing of HTML included in the \a markdown string is handled in the same
    way as in \l setHtml; however, Markdown formatting inside HTML blocks is
    not supported.

    Some features of the parser can be enabled or disabled via the \a features
    argument:

    \value MarkdownNoHTML
           Any HTML tags in the Markdown text will be discarded
    \value MarkdownDialectCommonMark
           The parser supports only the features standardized by CommonMark
    \value MarkdownDialectGitHub
           The parser supports the GitHub dialect

    The default is \c MarkdownDialectGitHub.

    \sa plainText, html, BOBUIextDocument::toMarkdown(), BOBUIextDocument::setMarkdown()
    \since 5.14
*/
#endif

#if BOBUI_CONFIG(textmarkdownreader)
void BOBUIextEdit::setMarkdown(const QString &markdown)
{
    Q_D(const BOBUIextEdit);
    d->control->setMarkdown(markdown);
}
#endif

#if BOBUI_CONFIG(textmarkdownwriter)
QString BOBUIextEdit::toMarkdown(BOBUIextDocument::MarkdownFeatures features) const
{
    Q_D(const BOBUIextEdit);
    return d->control->toMarkdown(features);
}
#endif

/*! \reimp
*/
void BOBUIextEdit::keyPressEvent(QKeyEvent *e)
{
    Q_D(BOBUIextEdit);

#ifdef BOBUI_KEYPAD_NAVIGATION
    switch (e->key()) {
        case BobUI::Key_Select:
            if (QApplicationPrivate::keypadNavigationEnabled()) {
                // code assumes linksaccessible + editable isn't meaningful
                if (d->control->textInteractionFlags() & BobUI::TextEditable) {
                    setEditFocus(!hasEditFocus());
                } else {
                    if (!hasEditFocus())
                        setEditFocus(true);
                    else {
                        BOBUIextCursor cursor = d->control->textCursor();
                        BOBUIextCharFormat charFmt = cursor.charFormat();
                        if (!(d->control->textInteractionFlags() & BobUI::LinksAccessibleByKeyboard)
                            || !cursor.hasSelection() || charFmt.anchorHref().isEmpty()) {
                            e->accept();
                            return;
                        }
                    }
                }
            }
            break;
        case BobUI::Key_Back:
        case BobUI::Key_No:
            if (!QApplicationPrivate::keypadNavigationEnabled() || !hasEditFocus()) {
                e->ignore();
                return;
            }
            break;
        default:
            if (QApplicationPrivate::keypadNavigationEnabled()) {
                if (!hasEditFocus() && !(e->modifiers() & BobUI::ControlModifier)) {
                    if (e->text()[0].isPrint())
                        setEditFocus(true);
                    else {
                        e->ignore();
                        return;
                    }
                }
            }
            break;
    }
#endif
#ifndef BOBUI_NO_SHORTCUT

    BobUI::TextInteractionFlags tif = d->control->textInteractionFlags();

    if (tif & BobUI::TextSelectableByKeyboard){
        if (e == QKeySequence::SelectPreviousPage) {
            e->accept();
            d->pageUpDown(BOBUIextCursor::Up, BOBUIextCursor::KeepAnchor);
            return;
        } else if (e ==QKeySequence::SelectNextPage) {
            e->accept();
            d->pageUpDown(BOBUIextCursor::Down, BOBUIextCursor::KeepAnchor);
            return;
        }
    }
    if (tif & (BobUI::TextSelectableByKeyboard | BobUI::TextEditable)) {
        if (e == QKeySequence::MoveToPreviousPage) {
            e->accept();
            d->pageUpDown(BOBUIextCursor::Up, BOBUIextCursor::MoveAnchor);
            return;
        } else if (e == QKeySequence::MoveToNextPage) {
            e->accept();
            d->pageUpDown(BOBUIextCursor::Down, BOBUIextCursor::MoveAnchor);
            return;
        }
    }

    if (!(tif & BobUI::TextEditable)) {
        switch (e->key()) {
            case BobUI::Key_Space:
                e->accept();
                if (e->modifiers() & BobUI::ShiftModifier)
                    d->vbar->triggerAction(QAbstractSlider::SliderPageStepSub);
                else
                    d->vbar->triggerAction(QAbstractSlider::SliderPageStepAdd);
                break;
            default:
                d->sendControlEvent(e);
                if (!e->isAccepted() && e->modifiers() == BobUI::NoModifier) {
                    if (e->key() == BobUI::Key_Home) {
                        d->vbar->triggerAction(QAbstractSlider::SliderToMinimum);
                        e->accept();
                    } else if (e->key() == BobUI::Key_End) {
                        d->vbar->triggerAction(QAbstractSlider::SliderToMaximum);
                        e->accept();
                    }
                }
                if (!e->isAccepted()) {
                    QAbstractScrollArea::keyPressEvent(e);
                }
        }
        return;
    }
#endif // BOBUI_NO_SHORTCUT

    {
        BOBUIextCursor cursor = d->control->textCursor();
        const QString text = e->text();
        if (cursor.atBlockStart()
            && (d->autoFormatting & AutoBulletList)
            && (text.size() == 1)
            && (text.at(0) == u'-' || text.at(0) == u'*')
            && (!cursor.currentList())) {

            d->createAutoBulletList();
            e->accept();
            return;
        }
    }

    d->sendControlEvent(e);
#ifdef BOBUI_KEYPAD_NAVIGATION
    if (!e->isAccepted()) {
        switch (e->key()) {
            case BobUI::Key_Up:
            case BobUI::Key_Down:
                if (QApplicationPrivate::keypadNavigationEnabled()) {
                    // Cursor position didn't change, so we want to leave
                    // these keys to change focus.
                    e->ignore();
                    return;
                }
                break;
            case BobUI::Key_Back:
                if (!e->isAutoRepeat()) {
                    if (QApplicationPrivate::keypadNavigationEnabled()) {
                        if (document()->isEmpty() || !(d->control->textInteractionFlags() & BobUI::TextEditable)) {
                            setEditFocus(false);
                            e->accept();
                        } else if (!d->deleteAllTimer.isActive()) {
                            e->accept();
                            d->deleteAllTimer.start(750, this);
                        }
                    } else {
                        e->ignore();
                        return;
                    }
                }
                break;
            default: break;
        }
    }
#endif
}

/*! \reimp
*/
void BOBUIextEdit::keyReleaseEvent(QKeyEvent *e)
{
    Q_D(BOBUIextEdit);
    if (!isReadOnly())
        d->handleSoftwareInputPanel();
#ifdef BOBUI_KEYPAD_NAVIGATION
    if (QApplicationPrivate::keypadNavigationEnabled()) {
        if (!e->isAutoRepeat() && e->key() == BobUI::Key_Back
            && d->deleteAllTimer.isActive()) {
            d->deleteAllTimer.stop();
            BOBUIextCursor cursor = d->control->textCursor();
            BOBUIextBlockFormat blockFmt = cursor.blockFormat();

            BOBUIextList *list = cursor.currentList();
            if (list && cursor.atBlockStart()) {
                list->remove(cursor.block());
            } else if (cursor.atBlockStart() && blockFmt.indent() > 0) {
                blockFmt.setIndent(blockFmt.indent() - 1);
                cursor.setBlockFormat(blockFmt);
            } else {
                cursor.deletePreviousChar();
            }
            setTextCursor(cursor);
            e->accept();
            return;
        }
    }
#endif
    e->ignore();
}

/*!
    Loads the resource specified by the given \a type and \a name.

    This function is an extension of BOBUIextDocument::loadResource().

    \sa BOBUIextDocument::loadResource()
*/
QVariant BOBUIextEdit::loadResource(int type, const QUrl &name)
{
    Q_UNUSED(type);
    Q_UNUSED(name);
    return QVariant();
}

/*! \reimp
*/
void BOBUIextEdit::resizeEvent(QResizeEvent *e)
{
    Q_D(BOBUIextEdit);

    if (d->lineWrap == NoWrap) {
        BOBUIextDocument *doc = d->control->document();
        QVariant alignmentProperty = doc->documentLayout()->property("contentHasAlignment");

        if (!doc->pageSize().isNull()
            && alignmentProperty.userType() == QMetaType::Bool
            && !alignmentProperty.toBool()) {

            d->adjustScrollbars();
            return;
        }
    }

    if (d->lineWrap != FixedPixelWidth
        && e->oldSize().width() != e->size().width())
        d->relayoutDocument();
    else
        d->adjustScrollbars();
}

void BOBUIextEditPrivate::relayoutDocument()
{
    BOBUIextDocument *doc = control->document();
    QAbstractTextDocumentLayout *layout = doc->documentLayout();

    if (BOBUIextDocumentLayout *tlayout = qobject_cast<BOBUIextDocumentLayout *>(layout)) {
        if (lineWrap == BOBUIextEdit::FixedColumnWidth)
            tlayout->setFixedColumnWidth(lineWrapColumnOrWidth);
        else
            tlayout->setFixedColumnWidth(-1);
    }

    BOBUIextDocumentLayout *tlayout = qobject_cast<BOBUIextDocumentLayout *>(layout);
    QSize lastUsedSize;
    if (tlayout)
        lastUsedSize = tlayout->dynamicDocumentSize().toSize();
    else
        lastUsedSize = layout->documentSize().toSize();

    // ignore calls to adjustScrollbars caused by an emission of the
    // usedSizeChanged() signal in the layout, as we're calling it
    // later on our own anyway (or deliberately not) .
    const bool oldIgnoreScrollbarAdjustment = ignoreAutomaticScrollbarAdjustment;
    ignoreAutomaticScrollbarAdjustment = true;

    int width = viewport->width();
    if (lineWrap == BOBUIextEdit::FixedPixelWidth)
        width = lineWrapColumnOrWidth;
    else if (lineWrap == BOBUIextEdit::NoWrap) {
        QVariant alignmentProperty = doc->documentLayout()->property("contentHasAlignment");
        if (alignmentProperty.userType() == QMetaType::Bool && !alignmentProperty.toBool()) {

            width = 0;
        }
    }

    doc->setPageSize(QSize(width, -1));
    if (tlayout)
        tlayout->ensureLayouted(verticalOffset() + viewport->height());

    ignoreAutomaticScrollbarAdjustment = oldIgnoreScrollbarAdjustment;

    QSize usedSize;
    if (tlayout)
        usedSize = tlayout->dynamicDocumentSize().toSize();
    else
        usedSize = layout->documentSize().toSize();

    // this is an obscure situation in the layout that can happen:
    // if a character at the end of a line is the tallest one and therefore
    // influencing the total height of the line and the line right below it
    // is always taller though, then it can happen that if due to line breaking
    // that tall character wraps into the lower line the document not only shrinks
    // horizontally (causing the character to wrap in the first place) but also
    // vertically, because the original line is now smaller and the one below kept
    // its size. So a layout with less width _can_ take up less vertical space, too.
    // If the wider case causes a vertical scroll bar to appear and the narrower one
    // (narrower because the vertical scroll bar takes up horizontal space)) to disappear
    // again then we have an endless loop, as adjustScrollbars sets new ranges on the
    // scroll bars, the QAbstractScrollArea will find out about it and try to show/hide
    // the scroll bars again. That's why we try to detect this case here and break out.
    //
    // (if you change this please also check the layoutingLoop() testcase in
    // BOBUIextEdit's autotests)
    if (lastUsedSize.isValid()
        && !vbar->isHidden()
        && viewport->width() < lastUsedSize.width()
        && usedSize.height() < lastUsedSize.height()
        && usedSize.height() <= viewport->height())
        return;

    adjustScrollbars();
}

void BOBUIextEditPrivate::paint(QPainter *p, QPaintEvent *e)
{
    const int xOffset = horizontalOffset();
    const int yOffset = verticalOffset();

    QRect r = e->rect();
    p->translate(-xOffset, -yOffset);
    r.translate(xOffset, yOffset);

    BOBUIextDocument *doc = control->document();
    BOBUIextDocumentLayout *layout = qobject_cast<BOBUIextDocumentLayout *>(doc->documentLayout());

    // the layout might need to expand the root frame to
    // the viewport if NoWrap is set
    if (layout)
        layout->setViewport(viewport->rect());

    control->drawContents(p, r, q_func());

    if (layout)
        layout->setViewport(QRect());

    if (!placeholderText.isEmpty() && doc->isEmpty() && !control->isPreediting()) {
        const QColor col = control->palette().placeholderText().color();
        p->setPen(col);
        const int margin = int(doc->documentMargin());
        QRectF boundingRect = layout ? layout->frameBoundingRect(doc->rootFrame()) : viewport->rect();
        p->drawText(boundingRect.adjusted(margin, margin, -margin, -margin),
                    BobUI::AlignTop | BobUI::TextWordWrap,
                    placeholderText);
    }
}

/*! \fn void BOBUIextEdit::paintEvent(QPaintEvent *event)

This event handler can be reimplemented in a subclass to receive paint events passed in \a event.
It is usually unnecessary to reimplement this function in a subclass of BOBUIextEdit.

\note If you create a QPainter, it must operate on the \l{QAbstractScrollArea::}{viewport()}.

\warning The underlying text document must not be modified from within a reimplementation
of this function.
*/
void BOBUIextEdit::paintEvent(QPaintEvent *e)
{
    Q_D(BOBUIextEdit);
    QPainter p(d->viewport);
    d->paint(&p, e);
}

void BOBUIextEditPrivate::updateDefaultTextOption()
{
    BOBUIextDocument *doc = control->document();

    BOBUIextOption opt = doc->defaultTextOption();
    BOBUIextOption::WrapMode oldWrapMode = opt.wrapMode();

    if (lineWrap == BOBUIextEdit::NoWrap)
        opt.setWrapMode(BOBUIextOption::NoWrap);
    else
        opt.setWrapMode(wordWrap);

    if (opt.wrapMode() != oldWrapMode)
        doc->setDefaultTextOption(opt);
}

/*! \reimp
*/
void BOBUIextEdit::mousePressEvent(QMouseEvent *e)
{
    Q_D(BOBUIextEdit);
#ifdef BOBUI_KEYPAD_NAVIGATION
    if (QApplicationPrivate::keypadNavigationEnabled() && !hasEditFocus())
        setEditFocus(true);
#endif
    d->sendControlEvent(e);
}

/*! \reimp
*/
void BOBUIextEdit::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(BOBUIextEdit);
    d->inDrag = false; // paranoia
    const QPoint pos = e->position().toPoint();
    d->sendControlEvent(e);
    if (!(e->buttons() & BobUI::LeftButton))
        return;
    if (e->source() == BobUI::MouseEventNotSynthesized) {
        const QRect visible = d->viewport->rect();
        if (visible.contains(pos))
            d->autoScrollTimer.stop();
        else if (!d->autoScrollTimer.isActive())
            d->autoScrollTimer.start(100, this);
    }
}

/*! \reimp
*/
void BOBUIextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(BOBUIextEdit);
    d->sendControlEvent(e);
    if (e->source() == BobUI::MouseEventNotSynthesized && d->autoScrollTimer.isActive()) {
        d->autoScrollTimer.stop();
        ensureCursorVisible();
    }
    if (!isReadOnly() && rect().contains(e->position().toPoint()))
        d->handleSoftwareInputPanel(e->button(), d->clickCausedFocus);
    d->clickCausedFocus = 0;
}

/*! \reimp
*/
void BOBUIextEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_D(BOBUIextEdit);
    d->sendControlEvent(e);
}

/*! \reimp
*/
bool BOBUIextEdit::focusNextPrevChild(bool next)
{
    Q_D(const BOBUIextEdit);
    if (!d->tabChangesFocus && d->control->textInteractionFlags() & BobUI::TextEditable)
        return false;
    return QAbstractScrollArea::focusNextPrevChild(next);
}

#ifndef BOBUI_NO_CONTEXTMENU
/*!
  \fn void BOBUIextEdit::contextMenuEvent(QContextMenuEvent *event)

  Shows the standard context menu created with createStandardContextMenu().

  If you do not want the text edit to have a context menu, you can set
  its \l contextMenuPolicy to BobUI::NoContextMenu. If you want to
  customize the context menu, reimplement this function. If you want
  to extend the standard context menu, reimplement this function, call
  createStandardContextMenu() and extend the menu returned.

  Information about the event is passed in the \a event object.

  \snippet code/src_gui_widgets_bobuiextedit.cpp 0
*/
void BOBUIextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    Q_D(BOBUIextEdit);
    d->sendControlEvent(e);
}
#endif // BOBUI_NO_CONTEXTMENU

#if BOBUI_CONFIG(draganddrop)
/*! \reimp
*/
void BOBUIextEdit::dragEnterEvent(QDragEnterEvent *e)
{
    Q_D(BOBUIextEdit);
    d->inDrag = true;
    d->sendControlEvent(e);
}

/*! \reimp
*/
void BOBUIextEdit::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_D(BOBUIextEdit);
    d->inDrag = false;
    d->autoScrollTimer.stop();
    d->sendControlEvent(e);
}

/*! \reimp
*/
void BOBUIextEdit::dragMoveEvent(QDragMoveEvent *e)
{
    Q_D(BOBUIextEdit);
    d->autoScrollDragPos = e->position().toPoint();
    if (!d->autoScrollTimer.isActive())
        d->autoScrollTimer.start(100, this);
    d->sendControlEvent(e);
}

/*! \reimp
*/
void BOBUIextEdit::dropEvent(QDropEvent *e)
{
    Q_D(BOBUIextEdit);
    d->inDrag = false;
    d->autoScrollTimer.stop();
    d->sendControlEvent(e);
}

#endif // BOBUI_CONFIG(draganddrop)

/*! \reimp
 */
void BOBUIextEdit::inputMethodEvent(QInputMethodEvent *e)
{
    Q_D(BOBUIextEdit);
#ifdef BOBUI_KEYPAD_NAVIGATION
    if (d->control->textInteractionFlags() & BobUI::TextEditable
        && QApplicationPrivate::keypadNavigationEnabled()
        && !hasEditFocus())
        setEditFocus(true);
#endif
    d->sendControlEvent(e);
    const bool emptyEvent = e->preeditString().isEmpty() && e->commitString().isEmpty()
                         && e->attributes().isEmpty();
    if (emptyEvent)
        return;
    ensureCursorVisible();
}

/*!\reimp
*/
void BOBUIextEdit::scrollContentsBy(int dx, int dy)
{
    Q_D(BOBUIextEdit);
    if (isRightToLeft())
        dx = -dx;
    d->viewport->scroll(dx, dy);
    QGuiApplication::inputMethod()->update(BobUI::ImCursorRectangle | BobUI::ImAnchorRectangle);
}

/*!\reimp
*/
QVariant BOBUIextEdit::inputMethodQuery(BobUI::InputMethodQuery property) const
{
    return inputMethodQuery(property, QVariant());
}

/*!\internal
 */
QVariant BOBUIextEdit::inputMethodQuery(BobUI::InputMethodQuery query, QVariant argument) const
{
    Q_D(const BOBUIextEdit);
    switch (query) {
    case BobUI::ImEnabled:
        return isEnabled() && !isReadOnly();
    case BobUI::ImHints:
    case BobUI::ImInputItemClipRectangle:
        return QWidget::inputMethodQuery(query);
    case BobUI::ImReadOnly:
        return isReadOnly();
    default:
        break;
    }

    const QPointF offset(-d->horizontalOffset(), -d->verticalOffset());
    switch (argument.userType()) {
    case QMetaType::QRectF:
        argument = argument.toRectF().translated(-offset);
        break;
    case QMetaType::QPointF:
        argument = argument.toPointF() - offset;
        break;
    case QMetaType::QRect:
        argument = argument.toRect().translated(-offset.toPoint());
        break;
    case QMetaType::QPoint:
        argument = argument.toPoint() - offset;
        break;
    default:
        break;
    }

    const QVariant v = d->control->inputMethodQuery(query, argument);
    switch (v.userType()) {
    case QMetaType::QRectF:
        return v.toRectF().translated(offset);
    case QMetaType::QPointF:
        return v.toPointF() + offset;
    case QMetaType::QRect:
        return v.toRect().translated(offset.toPoint());
    case QMetaType::QPoint:
        return v.toPoint() + offset.toPoint();
    default:
        break;
    }
    return v;
}

/*! \reimp
*/
void BOBUIextEdit::focusInEvent(QFocusEvent *e)
{
    Q_D(BOBUIextEdit);
    if (e->reason() == BobUI::MouseFocusReason) {
        d->clickCausedFocus = 1;
    }
    QAbstractScrollArea::focusInEvent(e);
    d->sendControlEvent(e);
}

/*! \reimp
*/
void BOBUIextEdit::focusOutEvent(QFocusEvent *e)
{
    Q_D(BOBUIextEdit);
    QAbstractScrollArea::focusOutEvent(e);
    d->sendControlEvent(e);
}

/*! \reimp
*/
void BOBUIextEdit::showEvent(QShowEvent *)
{
    Q_D(BOBUIextEdit);
    if (!d->anchorToScrollToWhenVisible.isEmpty()) {
        scrollToAnchor(d->anchorToScrollToWhenVisible);
        d->anchorToScrollToWhenVisible.clear();
        d->showCursorOnInitialShow = false;
    } else if (d->showCursorOnInitialShow) {
        d->showCursorOnInitialShow = false;
        ensureCursorVisible();
    }
}

/*! \reimp
*/
void BOBUIextEdit::changeEvent(QEvent *e)
{
    Q_D(BOBUIextEdit);
    QAbstractScrollArea::changeEvent(e);
    if (e->type() == QEvent::ApplicationFontChange
        || e->type() == QEvent::FontChange) {
        d->control->document()->setDefaultFont(font());
    }  else if (e->type() == QEvent::ActivationChange) {
        if (!isActiveWindow())
            d->autoScrollTimer.stop();
    } else if (e->type() == QEvent::EnabledChange) {
        e->setAccepted(isEnabled());
        d->control->setPalette(palette());
        d->sendControlEvent(e);
    } else if (e->type() == QEvent::PaletteChange) {
        d->control->setPalette(palette());
    } else if (e->type() == QEvent::LayoutDirectionChange) {
        d->sendControlEvent(e);
    }
}

/*! \reimp
*/
#if BOBUI_CONFIG(wheelevent)
void BOBUIextEdit::wheelEvent(QWheelEvent *e)
{
    Q_D(BOBUIextEdit);
    if (!(d->control->textInteractionFlags() & BobUI::TextEditable)) {
        if (e->modifiers() & BobUI::ControlModifier) {
            float delta = e->angleDelta().y() / 120.f;
            zoomInF(delta);
            return;
        }
    }
    QAbstractScrollArea::wheelEvent(e);
    updateMicroFocus();
}
#endif

#ifndef BOBUI_NO_CONTEXTMENU
/*!  This function creates the standard context menu which is shown
  when the user clicks on the text edit with the right mouse
  button. It is called from the default contextMenuEvent() handler.
  The popup menu's ownership is transferred to the caller.

  We recommend that you use the createStandardContextMenu(QPoint) version instead
  which will enable the actions that are sensitive to where the user clicked.
*/

QMenu *BOBUIextEdit::createStandardContextMenu()
{
    Q_D(BOBUIextEdit);
    return d->control->createStandardContextMenu(QPointF(), this);
}

/*!
  \since 4.4
  This function creates the standard context menu which is shown
  when the user clicks on the text edit with the right mouse
  button. It is called from the default contextMenuEvent() handler
  and it takes the \a position in document coordinates where the mouse click was.
  This can enable actions that are sensitive to the position where the user clicked.
  The popup menu's ownership is transferred to the caller.
*/

QMenu *BOBUIextEdit::createStandardContextMenu(const QPoint &position)
{
    Q_D(BOBUIextEdit);
    return d->control->createStandardContextMenu(position, this);
}
#endif // BOBUI_NO_CONTEXTMENU

/*!
  returns a BOBUIextCursor at position \a pos (in viewport coordinates).
*/
BOBUIextCursor BOBUIextEdit::cursorForPosition(const QPoint &pos) const
{
    Q_D(const BOBUIextEdit);
    return d->control->cursorForPosition(d->mapToContents(pos));
}

/*!
  returns a rectangle (in viewport coordinates) that includes the
  \a cursor.
 */
QRect BOBUIextEdit::cursorRect(const BOBUIextCursor &cursor) const
{
    Q_D(const BOBUIextEdit);
    if (cursor.isNull())
        return QRect();

    QRect r = d->control->cursorRect(cursor).toRect();
    r.translate(-d->horizontalOffset(),-d->verticalOffset());
    return r;
}

/*!
  returns a rectangle (in viewport coordinates) that includes the
  cursor of the text edit.
 */
QRect BOBUIextEdit::cursorRect() const
{
    Q_D(const BOBUIextEdit);
    QRect r = d->control->cursorRect().toRect();
    r.translate(-d->horizontalOffset(),-d->verticalOffset());
    return r;
}


/*!
    Returns the reference of the anchor at position \a pos, or an
    empty string if no anchor exists at that point.
*/
QString BOBUIextEdit::anchorAt(const QPoint& pos) const
{
    Q_D(const BOBUIextEdit);
    return d->control->anchorAt(d->mapToContents(pos));
}

/*!
   \property BOBUIextEdit::overwriteMode
   \since 4.1
   \brief whether text entered by the user will overwrite existing text

   As with many text editors, the text editor widget can be configured
   to insert or overwrite existing text with new text entered by the user.

   If this property is \c true, existing text is overwritten, character-for-character
   by new text; otherwise, text is inserted at the cursor position, displacing
   existing text.

   By default, this property is \c false (new text does not overwrite existing text).
*/

bool BOBUIextEdit::overwriteMode() const
{
    Q_D(const BOBUIextEdit);
    return d->control->overwriteMode();
}

void BOBUIextEdit::setOverwriteMode(bool overwrite)
{
    Q_D(BOBUIextEdit);
    d->control->setOverwriteMode(overwrite);
}

/*!
    \property BOBUIextEdit::tabStopDistance
    \brief the tab stop distance in pixels
    \since 5.10

    By default, this property contains a value of 80 pixels.

    Do not set a value less than the \l {QFontMetrics::}{horizontalAdvance()}
    of the QChar::VisualTabCharacter character, otherwise the tab-character
    will be drawn incompletely.

    \sa BOBUIextOption::ShowTabsAndSpaces, BOBUIextDocument::defaultTextOption
*/

qreal BOBUIextEdit::tabStopDistance() const
{
    Q_D(const BOBUIextEdit);
    return d->control->document()->defaultTextOption().tabStopDistance();
}

void BOBUIextEdit::setTabStopDistance(qreal distance)
{
    Q_D(BOBUIextEdit);
    BOBUIextOption opt = d->control->document()->defaultTextOption();
    if (opt.tabStopDistance() == distance || distance < 0)
        return;
    opt.setTabStopDistance(distance);
    d->control->document()->setDefaultTextOption(opt);
}

/*!
    \since 4.2
    \property BOBUIextEdit::cursorWidth

    This property specifies the width of the cursor in pixels. The default value is 1.
*/
int BOBUIextEdit::cursorWidth() const
{
    Q_D(const BOBUIextEdit);
    return d->control->cursorWidth();
}

void BOBUIextEdit::setCursorWidth(int width)
{
    Q_D(BOBUIextEdit);
    d->control->setCursorWidth(width);
}

/*!
    \property BOBUIextEdit::acceptRichText
    \brief whether the text edit accepts rich text insertions by the user
    \since 4.1

    When this property is set to false text edit will accept only
    plain text input from the user. For example through clipboard or drag and drop.

    This property's default is true.
*/

bool BOBUIextEdit::acceptRichText() const
{
    Q_D(const BOBUIextEdit);
    return d->control->acceptRichText();
}

void BOBUIextEdit::setAcceptRichText(bool accept)
{
    Q_D(BOBUIextEdit);
    d->control->setAcceptRichText(accept);
}

/*!
    \class BOBUIextEdit::ExtraSelection
    \since 4.2
    \inmodule BobUIWidgets

    \brief The BOBUIextEdit::ExtraSelection structure provides a way of specifying a
           character format for a given selection in a document.
*/

/*!
    \variable BOBUIextEdit::ExtraSelection::cursor
    A cursor that contains a selection in a BOBUIextDocument
*/

/*!
    \variable BOBUIextEdit::ExtraSelection::format
    A format that is used to specify a foreground or background brush/color
    for the selection.
*/

/*!
    \since 4.2
    This function allows temporarily marking certain regions in the document
    with a given color, specified as \a selections. This can be useful for
    example in a programming editor to mark a whole line of text with a given
    background color to indicate the existence of a breakpoint.

    \sa BOBUIextEdit::ExtraSelection, extraSelections()
*/
void BOBUIextEdit::setExtraSelections(const QList<ExtraSelection> &selections)
{
    Q_D(BOBUIextEdit);
    d->control->setExtraSelections(selections);
}

/*!
    \since 4.2
    Returns previously set extra selections.

    \sa setExtraSelections()
*/
QList<BOBUIextEdit::ExtraSelection> BOBUIextEdit::extraSelections() const
{
    Q_D(const BOBUIextEdit);
    return d->control->extraSelections();
}

/*!
    This function returns a new MIME data object to represent the contents
    of the text edit's current selection. It is called when the selection needs
    to be encapsulated into a new QMimeData object; for example, when a drag
    and drop operation is started, or when data is copied to the clipboard.

    If you reimplement this function, note that the ownership of the returned
    QMimeData object is passed to the caller. The selection can be retrieved
    by using the textCursor() function.
*/
QMimeData *BOBUIextEdit::createMimeDataFromSelection() const
{
    Q_D(const BOBUIextEdit);
    return d->control->QWidgetTextControl::createMimeDataFromSelection();
}

/*!
    This function returns \c true if the contents of the MIME data object, specified
    by \a source, can be decoded and inserted into the document. It is called
    for example when during a drag operation the mouse enters this widget and it
    is necessary to determine whether it is possible to accept the drag and drop
    operation.

    Reimplement this function to enable drag and drop support for additional MIME types.
 */
bool BOBUIextEdit::canInsertFromMimeData(const QMimeData *source) const
{
    Q_D(const BOBUIextEdit);
    return d->control->QWidgetTextControl::canInsertFromMimeData(source);
}

/*!
    This function inserts the contents of the MIME data object, specified
    by \a source, into the text edit at the current cursor position. It is
    called whenever text is inserted as the result of a clipboard paste
    operation, or when the text edit accepts data from a drag and drop
    operation.

    Reimplement this function to enable drag and drop support for additional MIME types.
 */
void BOBUIextEdit::insertFromMimeData(const QMimeData *source)
{
    Q_D(BOBUIextEdit);
    d->control->QWidgetTextControl::insertFromMimeData(source);
}

/*!
    \property BOBUIextEdit::readOnly
    \brief whether the text edit is read-only

    In a read-only text edit the user can only navigate through the
    text and select text; modifying the text is not possible.

    This property's default is false.
*/

bool BOBUIextEdit::isReadOnly() const
{
    Q_D(const BOBUIextEdit);
    return !d->control || !(d->control->textInteractionFlags() & BobUI::TextEditable);
}

void BOBUIextEdit::setReadOnly(bool ro)
{
    Q_D(BOBUIextEdit);
    BobUI::TextInteractionFlags flags = BobUI::NoTextInteraction;
    if (ro) {
        flags = BobUI::TextSelectableByMouse;
#if BOBUI_CONFIG(textbrowser)
        if (qobject_cast<BOBUIextBrowser *>(this))
            flags |= BobUI::TextBrowserInteraction;
#endif
    } else {
        flags = BobUI::TextEditorInteraction;
    }
    d->control->setTextInteractionFlags(flags);
    setAttribute(BobUI::WA_InputMethodEnabled, shouldEnableInputMethod(this));
    QEvent event(QEvent::ReadOnlyChange);
    QCoreApplication::sendEvent(this, &event);
}

/*!
    \property BOBUIextEdit::textInteractionFlags
    \since 4.2

    Specifies how the widget should interact with user input.

    The default value depends on whether the BOBUIextEdit is read-only
    or editable, and whether it is a BOBUIextBrowser or not.
*/

void BOBUIextEdit::setTextInteractionFlags(BobUI::TextInteractionFlags flags)
{
    Q_D(BOBUIextEdit);
    d->control->setTextInteractionFlags(flags);
}

BobUI::TextInteractionFlags BOBUIextEdit::textInteractionFlags() const
{
    Q_D(const BOBUIextEdit);
    return d->control->textInteractionFlags();
}

/*!
    Merges the properties specified in \a modifier into the current character
    format by calling BOBUIextCursor::mergeCharFormat on the editor's cursor.
    If the editor has a selection then the properties of \a modifier are
    directly applied to the selection.

    \sa BOBUIextCursor::mergeCharFormat()
 */
void BOBUIextEdit::mergeCurrentCharFormat(const BOBUIextCharFormat &modifier)
{
    Q_D(BOBUIextEdit);
    d->control->mergeCurrentCharFormat(modifier);
}

/*!
    Sets the char format that is be used when inserting new text to \a
    format by calling BOBUIextCursor::setCharFormat() on the editor's
    cursor.  If the editor has a selection then the char format is
    directly applied to the selection.
 */
void BOBUIextEdit::setCurrentCharFormat(const BOBUIextCharFormat &format)
{
    Q_D(BOBUIextEdit);
    d->control->setCurrentCharFormat(format);
}

/*!
    Returns the char format that is used when inserting new text.
 */
BOBUIextCharFormat BOBUIextEdit::currentCharFormat() const
{
    Q_D(const BOBUIextEdit);
    return d->control->currentCharFormat();
}

/*!
    \property BOBUIextEdit::autoFormatting
    \brief the enabled set of auto formatting features

    The value can be any combination of the values in the
    AutoFormattingFlag enum.  The default is AutoNone. Choose
    AutoAll to enable all automatic formatting.

    Currently, the only automatic formatting feature provided is
    AutoBulletList; future versions of BobUI may offer more.
*/

BOBUIextEdit::AutoFormatting BOBUIextEdit::autoFormatting() const
{
    Q_D(const BOBUIextEdit);
    return d->autoFormatting;
}

void BOBUIextEdit::setAutoFormatting(AutoFormatting features)
{
    Q_D(BOBUIextEdit);
    d->autoFormatting = features;
}

/*!
    Convenience slot that inserts \a text at the current
    cursor position.

    It is equivalent to

    \snippet code/src_gui_widgets_bobuiextedit.cpp 1
 */
void BOBUIextEdit::insertPlainText(const QString &text)
{
    Q_D(BOBUIextEdit);
    d->control->insertPlainText(text);
}

/*!
    Convenience slot that inserts \a text which is assumed to be of
    html formatting at the current cursor position.

    It is equivalent to:

    \snippet code/src_gui_widgets_bobuiextedit.cpp 2

    \note When using this function with a style sheet, the style sheet will
    only apply to the current block in the document. In order to apply a style
    sheet throughout a document, use BOBUIextDocument::setDefaultStyleSheet()
    instead.
 */
#ifndef BOBUI_NO_TEXTHTMLPARSER
void BOBUIextEdit::insertHtml(const QString &text)
{
    Q_D(BOBUIextEdit);
    d->control->insertHtml(text);
}
#endif // BOBUI_NO_TEXTHTMLPARSER

/*!
    Scrolls the text edit so that the anchor with the given \a name is
    visible; does nothing if the \a name is empty, or is already
    visible, or isn't found.
*/
void BOBUIextEdit::scrollToAnchor(const QString &name)
{
    Q_D(BOBUIextEdit);
    if (name.isEmpty())
        return;

    if (!isVisible()) {
        d->anchorToScrollToWhenVisible = name;
        return;
    }

    QPointF p = d->control->anchorPosition(name);
    const int newPosition = qRound(p.y());
    if ( d->vbar->maximum() < newPosition )
        d->adjustScrollbars();
    d->vbar->setValue(newPosition);
}

/*!
    Zooms in on the text by making the base font size \a range
    points larger and recalculating all font sizes to be the new size.
    This does not change the size of any images.

    \sa zoomOut()
*/
void BOBUIextEdit::zoomIn(int range)
{
    zoomInF(range);
}

/*!
    Zooms out on the text by making the base font size \a range points
    smaller and recalculating all font sizes to be the new size. This
    does not change the size of any images.

    \sa zoomIn()
*/
void BOBUIextEdit::zoomOut(int range)
{
    zoomInF(-range);
}

/*!
    \internal
*/
void BOBUIextEdit::zoomInF(float range)
{
    if (range == 0.f)
        return;
    QFont f = font();
    const float newSize = f.pointSizeF() + range;
    if (newSize <= 0)
        return;
    f.setPointSizeF(newSize);
    setFont(f);
}

/*!
    \since 4.2
    Moves the cursor by performing the given \a operation.

    If \a mode is BOBUIextCursor::KeepAnchor, the cursor selects the text it moves over.
    This is the same effect that the user achieves when they hold down the Shift key
    and move the cursor with the cursor keys.

    \sa BOBUIextCursor::movePosition()
*/
void BOBUIextEdit::moveCursor(BOBUIextCursor::MoveOperation operation, BOBUIextCursor::MoveMode mode)
{
    Q_D(BOBUIextEdit);
    d->control->moveCursor(operation, mode);
}

/*!
    \since 4.2
    Returns whether text can be pasted from the clipboard into the textedit.
*/
bool BOBUIextEdit::canPaste() const
{
    Q_D(const BOBUIextEdit);
    return d->control->canPaste();
}

/*!
    \since 4.3
    Convenience function to print the text edit's document to the given \a printer. This
    is equivalent to calling the print method on the document directly except that this
    function also supports QPrinter::Selection as print range.

    \sa BOBUIextDocument::print()
*/
#ifndef BOBUI_NO_PRINTER
void BOBUIextEdit::print(QPagedPaintDevice *printer) const
{
    Q_D(const BOBUIextEdit);
    d->control->print(printer);
}
#endif

/*! \property BOBUIextEdit::tabChangesFocus
  \brief whether \uicontrol Tab changes focus or is accepted as input

  In some occasions text edits should not allow the user to input
  tabulators or change indentation using the \uicontrol Tab key, as this breaks
  the focus chain. The default is false.

*/

bool BOBUIextEdit::tabChangesFocus() const
{
    Q_D(const BOBUIextEdit);
    return d->tabChangesFocus;
}

void BOBUIextEdit::setTabChangesFocus(bool b)
{
    Q_D(BOBUIextEdit);
    d->tabChangesFocus = b;
}

/*!
    \property BOBUIextEdit::documentTitle
    \brief the title of the document parsed from the text.

    By default, for a newly-created, empty document, this property contains
    an empty string.
*/

/*!
    \property BOBUIextEdit::lineWrapMode
    \brief the line wrap mode

    The default mode is WidgetWidth which causes words to be
    wrapped at the right edge of the text edit. Wrapping occurs at
    whitespace, keeping whole words intact. If you want wrapping to
    occur within words use setWordWrapMode(). If you set a wrap mode of
    FixedPixelWidth or FixedColumnWidth you should also call
    setLineWrapColumnOrWidth() with the width you want.

    \sa lineWrapColumnOrWidth
*/

BOBUIextEdit::LineWrapMode BOBUIextEdit::lineWrapMode() const
{
    Q_D(const BOBUIextEdit);
    return d->lineWrap;
}

void BOBUIextEdit::setLineWrapMode(LineWrapMode wrap)
{
    Q_D(BOBUIextEdit);
    if (d->lineWrap == wrap)
        return;
    d->lineWrap = wrap;
    d->updateDefaultTextOption();
    d->relayoutDocument();
}

/*!
    \property BOBUIextEdit::lineWrapColumnOrWidth
    \brief the position (in pixels or columns depending on the wrap mode) where text will be wrapped

    If the wrap mode is FixedPixelWidth, the value is the number of
    pixels from the left edge of the text edit at which text should be
    wrapped. If the wrap mode is FixedColumnWidth, the value is the
    column number (in character columns) from the left edge of the
    text edit at which text should be wrapped.

    By default, this property contains a value of 0.

    \sa lineWrapMode
*/

int BOBUIextEdit::lineWrapColumnOrWidth() const
{
    Q_D(const BOBUIextEdit);
    return d->lineWrapColumnOrWidth;
}

void BOBUIextEdit::setLineWrapColumnOrWidth(int w)
{
    Q_D(BOBUIextEdit);
    d->lineWrapColumnOrWidth = w;
    d->relayoutDocument();
}

/*!
    \property BOBUIextEdit::wordWrapMode
    \brief the mode BOBUIextEdit will use when wrapping text by words

    By default, this property is set to BOBUIextOption::WrapAtWordBoundaryOrAnywhere.

    \sa BOBUIextOption::WrapMode
*/

BOBUIextOption::WrapMode BOBUIextEdit::wordWrapMode() const
{
    Q_D(const BOBUIextEdit);
    return d->wordWrap;
}

void BOBUIextEdit::setWordWrapMode(BOBUIextOption::WrapMode mode)
{
    Q_D(BOBUIextEdit);
    if (mode == d->wordWrap)
        return;
    d->wordWrap = mode;
    d->updateDefaultTextOption();
}

/*!
    Finds the next occurrence of the string, \a exp, using the given
    \a options. Returns \c true if \a exp was found and changes the
    cursor to select the match; otherwise returns \c false.
*/
bool BOBUIextEdit::find(const QString &exp, BOBUIextDocument::FindFlags options)
{
    Q_D(BOBUIextEdit);
    return d->control->find(exp, options);
}

/*!
    \fn bool BOBUIextEdit::find(const QRegularExpression &exp, BOBUIextDocument::FindFlags options)

    \since 5.13
    \overload

    Finds the next occurrence, matching the regular expression, \a exp, using the given
    \a options.

    Returns \c true if a match was found and changes the cursor to select the match;
    otherwise returns \c false.

    \warning For historical reasons, the case sensitivity option set on
    \a exp is ignored. Instead, the \a options are used to determine
    if the search is case sensitive or not.
*/
#if BOBUI_CONFIG(regularexpression)
bool BOBUIextEdit::find(const QRegularExpression &exp, BOBUIextDocument::FindFlags options)
{
    Q_D(BOBUIextEdit);
    return d->control->find(exp, options);
}
#endif

/*!
    \fn void BOBUIextEdit::copyAvailable(bool yes)

    This signal is emitted when text is selected or de-selected in the
    text edit.

    When text is selected this signal will be emitted with \a yes set
    to true. If no text has been selected or if the selected text is
    de-selected this signal is emitted with \a yes set to false.

    If \a yes is true then copy() can be used to copy the selection to
    the clipboard. If \a yes is false then copy() does nothing.

    \sa selectionChanged()
*/

/*!
    \fn void BOBUIextEdit::currentCharFormatChanged(const BOBUIextCharFormat &f)

    This signal is emitted if the current character format has changed, for
    example caused by a change of the cursor position.

    The new format is \a f.

    \sa setCurrentCharFormat()
*/

/*!
    \fn void BOBUIextEdit::selectionChanged()

    This signal is emitted whenever the selection changes.

    \sa copyAvailable()
*/

/*!
    \fn void BOBUIextEdit::cursorPositionChanged()

    This signal is emitted whenever the position of the
    cursor changed.
*/

/*!
    \since 4.2

    Sets the text edit's \a text. The text can be plain text or HTML
    and the text edit will try to guess the right format.

    Use setHtml() or setPlainText() directly to avoid text edit's guessing.

    \sa toPlainText(), toHtml()
*/
void BOBUIextEdit::setText(const QString &text)
{
    BobUI::TextFormat format = BobUI::mightBeRichText(text) ? BobUI::RichText : BobUI::PlainText;
#ifndef BOBUI_NO_TEXTHTMLPARSER
    if (format == BobUI::RichText)
        setHtml(text);
    else
#else
    Q_UNUSED(format);
#endif
        setPlainText(text);
}


/*!
    Appends a new paragraph with \a text to the end of the text edit.

    \note The new paragraph appended will have the same character format and
    block format as the current paragraph, determined by the position of the cursor.

    \sa currentCharFormat(), BOBUIextCursor::blockFormat()
*/

void BOBUIextEdit::append(const QString &text)
{
    Q_D(BOBUIextEdit);
    const bool atBottom = isReadOnly() ?  d->verticalOffset() >= d->vbar->maximum() :
            d->control->textCursor().atEnd();
    d->control->append(text);
    if (atBottom)
        d->vbar->setValue(d->vbar->maximum());
}

/*!
    Ensures that the cursor is visible by scrolling the text edit if
    necessary.
*/
void BOBUIextEdit::ensureCursorVisible()
{
    Q_D(BOBUIextEdit);
    d->control->ensureCursorVisible();
}

/*!
    \fn void BOBUIextEdit::textChanged()

    This signal is emitted whenever the document's content changes; for
    example, when text is inserted or deleted, or when formatting is applied.
*/

/*!
    \fn void BOBUIextEdit::undoAvailable(bool available)

    This signal is emitted whenever undo operations become available
    (\a available is true) or unavailable (\a available is false).
*/

/*!
    \fn void BOBUIextEdit::redoAvailable(bool available)

    This signal is emitted whenever redo operations become available
    (\a available is true) or unavailable (\a available is false).
*/

BOBUI_END_NAMESPACE

#include "moc_bobuiextedit.cpp"
