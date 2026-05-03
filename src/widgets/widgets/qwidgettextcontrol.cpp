// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qwidgettextcontrol_p.h"
#include "qwidgettextcontrol_p_p.h"

#ifndef BOBUI_NO_TEXTCONTROL

#include <qfont.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>
#if BOBUI_CONFIG(draganddrop)
#include <qdrag.h>
#endif
#include <qclipboard.h>
#include <qstyle.h>
#include "private/qapplication_p.h"
#include "private/bobuiextdocumentlayout_p.h"
#include "private/qabstracttextdocumentlayout_p.h"
#if BOBUI_CONFIG(menu)
#include "private/qmenu_p.h"
#endif
#include "bobuiextdocument.h"
#include "private/bobuiextdocument_p.h"
#include "private/bobuiextdocumentfragment_p.h"
#include "bobuiextlist.h"
#include "private/qwidgettextcontrol_p.h"
#if BOBUI_CONFIG(style_stylesheet)
#  include "private/qstylesheetstyle_p.h"
#endif
#if BOBUI_CONFIG(graphicsview)
#include "qgraphicssceneevent.h"
#endif
#include "qpagedpaintdevice.h"
#include "private/qpagedpaintdevice_p.h"
#include "bobuiextdocumentwriter.h"
#include "qstylehints.h"
#include "private/bobuiextcursor_p.h"

#include <bobuiextformat.h>
#include <qdatetime.h>
#include <qbuffer.h>
#include <qapplication.h>
#include <limits.h>
#include <bobuiexttable.h>
#include <qvariant.h>
#include <qurl.h>
#include <qdesktopservices.h>
#include <qinputmethod.h>
#if BOBUI_CONFIG(tooltip)
#include <bobuiooltip.h>
#endif
#include <qstyleoption.h>
#if BOBUI_CONFIG(lineedit)
#include <BobUIWidgets/qlineedit.h>
#endif
#include <BobUIGui/qaccessible.h>
#include <BobUICore/qmetaobject.h>
#ifdef Q_OS_WASM
#include <BobUICore/private/qstdweb_p.h>
#endif

#include <private/qoffsetstringarray_p.h>

#if BOBUI_CONFIG(shortcut)
#include "private/qapplication_p.h"
#include "private/qshortcutmap_p.h"
#include <qkeysequence.h>
#define ACCEL_KEY(k) (!QCoreApplication::testAttribute(BobUI::AA_DontShowShortcutsInContextMenus) \
                      && !QGuiApplicationPrivate::instance()->shortcutMap.hasShortcutForKeySequence(k) ? \
                      u'\t' + QKeySequence(k).toString(QKeySequence::NativeText) : QString())

#else
#define ACCEL_KEY(k) QString()
#endif

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

// could go into BOBUIextCursor...
static BOBUIextLine currentTextLine(const BOBUIextCursor &cursor)
{
    const BOBUIextBlock block = cursor.block();
    if (!block.isValid())
        return BOBUIextLine();

    const BOBUIextLayout *layout = block.layout();
    if (!layout)
        return BOBUIextLine();

    const int relativePos = cursor.position() - block.position();
    return layout->lineForTextPosition(relativePos);
}

QWidgetTextControlPrivate::QWidgetTextControlPrivate()
    : doc(nullptr), cursorOn(false), cursorVisible(false), cursorIsFocusIndicator(false),
#ifndef Q_OS_ANDROID
      interactionFlags(BobUI::TextEditorInteraction),
#else
      interactionFlags(BobUI::TextEditable | BobUI::TextSelectableByKeyboard),
#endif
      dragEnabled(true),
#if BOBUI_CONFIG(draganddrop)
      mousePressed(false), mightStartDrag(false),
#endif
      lastSelectionPosition(0), lastSelectionAnchor(0),
      ignoreAutomaticScrollbarAdjustement(false),
      overwriteMode(false),
      acceptRichText(true),
      preeditCursor(0), hideCursor(false),
      hasFocus(false),
#ifdef BOBUI_KEYPAD_NAVIGATION
      hasEditFocus(false),
#endif
      isEnabled(true),
      hadSelectionOnMousePress(false),
      ignoreUnusedNavigationEvents(false),
      openExternalLinks(false),
      wordSelectionEnabled(false)
{}

bool QWidgetTextControlPrivate::cursorMoveKeyEvent(QKeyEvent *e)
{
#ifdef BOBUI_NO_SHORTCUT
    Q_UNUSED(e);
#endif

    Q_Q(QWidgetTextControl);
    if (cursor.isNull())
        return false;

    const BOBUIextCursor oldSelection = cursor;
    const int oldCursorPos = cursor.position();

    BOBUIextCursor::MoveMode mode = BOBUIextCursor::MoveAnchor;
    BOBUIextCursor::MoveOperation op = BOBUIextCursor::NoMove;

    if (false) {
    }
#ifndef BOBUI_NO_SHORTCUT
    if (e == QKeySequence::MoveToNextChar) {
            op = BOBUIextCursor::Right;
    }
    else if (e == QKeySequence::MoveToPreviousChar) {
            op = BOBUIextCursor::Left;
    }
    else if (e == QKeySequence::SelectNextChar) {
           op = BOBUIextCursor::Right;
           mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousChar) {
            op = BOBUIextCursor::Left;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectNextWord) {
            op = BOBUIextCursor::WordRight;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousWord) {
            op = BOBUIextCursor::WordLeft;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfLine) {
            op = BOBUIextCursor::StartOfLine;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfLine) {
            op = BOBUIextCursor::EndOfLine;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfBlock) {
            op = BOBUIextCursor::StartOfBlock;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfBlock) {
            op = BOBUIextCursor::EndOfBlock;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectStartOfDocument) {
            op = BOBUIextCursor::Start;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectEndOfDocument) {
            op = BOBUIextCursor::End;
            mode = BOBUIextCursor::KeepAnchor;
    }
    else if (e == QKeySequence::SelectPreviousLine) {
            op = BOBUIextCursor::Up;
            mode = BOBUIextCursor::KeepAnchor;
            {
                BOBUIextBlock block = cursor.block();
                BOBUIextLine line = currentTextLine(cursor);
                if (!block.previous().isValid()
                    && line.isValid()
                    && line.lineNumber() == 0)
                    op = BOBUIextCursor::Start;
            }
    }
    else if (e == QKeySequence::SelectNextLine) {
            op = BOBUIextCursor::Down;
            mode = BOBUIextCursor::KeepAnchor;
            {
                BOBUIextBlock block = cursor.block();
                BOBUIextLine line = currentTextLine(cursor);
                if (!block.next().isValid()
                    && line.isValid()
                    && line.lineNumber() == block.layout()->lineCount() - 1)
                    op = BOBUIextCursor::End;
            }
    }
    else if (e == QKeySequence::MoveToNextWord) {
            op = BOBUIextCursor::WordRight;
    }
    else if (e == QKeySequence::MoveToPreviousWord) {
            op = BOBUIextCursor::WordLeft;
    }
    else if (e == QKeySequence::MoveToEndOfBlock) {
            op = BOBUIextCursor::EndOfBlock;
    }
    else if (e == QKeySequence::MoveToStartOfBlock) {
            op = BOBUIextCursor::StartOfBlock;
    }
    else if (e == QKeySequence::MoveToNextLine) {
            op = BOBUIextCursor::Down;
    }
    else if (e == QKeySequence::MoveToPreviousLine) {
            op = BOBUIextCursor::Up;
    }
    else if (e == QKeySequence::MoveToStartOfLine) {
            op = BOBUIextCursor::StartOfLine;
    }
    else if (e == QKeySequence::MoveToEndOfLine) {
            op = BOBUIextCursor::EndOfLine;
    }
    else if (e == QKeySequence::MoveToStartOfDocument) {
            op = BOBUIextCursor::Start;
    }
    else if (e == QKeySequence::MoveToEndOfDocument) {
            op = BOBUIextCursor::End;
    }
#endif // BOBUI_NO_SHORTCUT
    else {
        return false;
    }

// Except for pageup and pagedown, OS X has very different behavior, we don't do it all, but
// here's the breakdown:
// Shift still works as an anchor, but only one of the other keys can be down Ctrl (Command),
// Alt (Option), or Meta (Control).
// Command/Control + Left/Right -- Move to left or right of the line
//                 + Up/Down -- Move to top bottom of the file. (Control doesn't move the cursor)
// Option + Left/Right -- Move one word Left/right.
//        + Up/Down  -- Begin/End of Paragraph.
// Home/End Top/Bottom of file. (usually don't move the cursor, but will select)

    bool visualNavigation = cursor.visualNavigation();
    cursor.setVisualNavigation(true);
    const bool moved = cursor.movePosition(op, mode);
    cursor.setVisualNavigation(visualNavigation);
    q->ensureCursorVisible();

    bool ignoreNavigationEvents = ignoreUnusedNavigationEvents;
    bool isNavigationEvent = e->key() == BobUI::Key_Up || e->key() == BobUI::Key_Down;

#ifdef BOBUI_KEYPAD_NAVIGATION
    ignoreNavigationEvents = ignoreNavigationEvents || QApplicationPrivate::keypadNavigationEnabled();
    isNavigationEvent = isNavigationEvent ||
                        (QApplication::navigationMode() == BobUI::NavigationModeKeypadDirectional
                         && (e->key() == BobUI::Key_Left || e->key() == BobUI::Key_Right));
#else
    isNavigationEvent = isNavigationEvent || e->key() == BobUI::Key_Left || e->key() == BobUI::Key_Right;
#endif

    if (moved) {
        if (cursor.position() != oldCursorPos)
            emit q->cursorPositionChanged();
        emit q->microFocusChanged();
    } else if (ignoreNavigationEvents && isNavigationEvent && oldSelection.anchor() == cursor.anchor()) {
        return false;
    }

    selectionChanged(/*forceEmitSelectionChanged =*/(mode == BOBUIextCursor::KeepAnchor));

    repaintOldAndNewSelection(oldSelection);

    return true;
}

void QWidgetTextControlPrivate::updateCurrentCharFormat()
{
    Q_Q(QWidgetTextControl);

    BOBUIextCharFormat fmt = cursor.charFormat();
    if (fmt == lastCharFormat)
        return;
    lastCharFormat = fmt;

    emit q->currentCharFormatChanged(fmt);
    emit q->microFocusChanged();
}

void QWidgetTextControlPrivate::indent()
{
    BOBUIextBlockFormat blockFmt = cursor.blockFormat();

    BOBUIextList *list = cursor.currentList();
    if (!list) {
        BOBUIextBlockFormat modifier;
        modifier.setIndent(blockFmt.indent() + 1);
        cursor.mergeBlockFormat(modifier);
    } else {
        BOBUIextListFormat format = list->format();
        format.setIndent(format.indent() + 1);

        if (list->itemNumber(cursor.block()) == 1)
            list->setFormat(format);
        else
            cursor.createList(format);
    }
}

void QWidgetTextControlPrivate::outdent()
{
    BOBUIextBlockFormat blockFmt = cursor.blockFormat();

    BOBUIextList *list = cursor.currentList();

    if (!list) {
        BOBUIextBlockFormat modifier;
        modifier.setIndent(blockFmt.indent() - 1);
        cursor.mergeBlockFormat(modifier);
    } else {
        BOBUIextListFormat listFmt = list->format();
        listFmt.setIndent(listFmt.indent() - 1);
        list->setFormat(listFmt);
    }
}

void QWidgetTextControlPrivate::gotoNextTableCell()
{
    BOBUIextTable *table = cursor.currentTable();
    BOBUIextTableCell cell = table->cellAt(cursor);

    int newColumn = cell.column() + cell.columnSpan();
    int newRow = cell.row();

    if (newColumn >= table->columns()) {
        newColumn = 0;
        ++newRow;
        if (newRow >= table->rows())
            table->insertRows(table->rows(), 1);
    }

    cell = table->cellAt(newRow, newColumn);
    cursor = cell.firstCursorPosition();
}

void QWidgetTextControlPrivate::gotoPreviousTableCell()
{
    BOBUIextTable *table = cursor.currentTable();
    BOBUIextTableCell cell = table->cellAt(cursor);

    int newColumn = cell.column() - 1;
    int newRow = cell.row();

    if (newColumn < 0) {
        newColumn = table->columns() - 1;
        --newRow;
        if (newRow < 0)
            return;
    }

    cell = table->cellAt(newRow, newColumn);
    cursor = cell.firstCursorPosition();
}

void QWidgetTextControlPrivate::createAutoBulletList()
{
    cursor.beginEditBlock();

    BOBUIextBlockFormat blockFmt = cursor.blockFormat();

    BOBUIextListFormat listFmt;
    listFmt.setStyle(BOBUIextListFormat::ListDisc);
    listFmt.setIndent(blockFmt.indent() + 1);

    blockFmt.setIndent(0);
    cursor.setBlockFormat(blockFmt);

    cursor.createList(listFmt);

    cursor.endEditBlock();
}

void QWidgetTextControlPrivate::init(BobUI::TextFormat format, const QString &text, BOBUIextDocument *document)
{
    Q_Q(QWidgetTextControl);
    setContent(format, text, document);

    doc->setUndoRedoEnabled(interactionFlags & BobUI::TextEditable);
    q->setCursorWidth(-1);
}

void QWidgetTextControlPrivate::setContent(BobUI::TextFormat format, const QString &text, BOBUIextDocument *document)
{
    Q_Q(QWidgetTextControl);

    // for use when called from setPlainText. we may want to re-use the currently
    // set char format then.
    const BOBUIextCharFormat charFormatForInsertion = cursor.charFormat();

    bool clearDocument = true;
    if (!doc) {
        if (document) {
            doc = document;
        } else {
            palette = QApplication::palette("QWidgetTextControl");
            doc = new BOBUIextDocument(q);
        }
        clearDocument = false;
        _q_documentLayoutChanged();
        cursor = BOBUIextCursor(doc);

// ####        doc->documentLayout()->setPaintDevice(viewport);

        QObjectPrivate::connect(doc, &BOBUIextDocument::contentsChanged, this,
                                &QWidgetTextControlPrivate::_q_updateCurrentCharFormatAndSelection);
        QObjectPrivate::connect(doc, &BOBUIextDocument::cursorPositionChanged, this,
                                &QWidgetTextControlPrivate::_q_emitCursorPosChanged);
        QObjectPrivate::connect(doc, &BOBUIextDocument::documentLayoutChanged, this,
                                &QWidgetTextControlPrivate::_q_documentLayoutChanged);

        // convenience signal forwards
        QObject::connect(doc, &BOBUIextDocument::undoAvailable, q, &QWidgetTextControl::undoAvailable);
        QObject::connect(doc, &BOBUIextDocument::redoAvailable, q, &QWidgetTextControl::redoAvailable);
        QObject::connect(doc, &BOBUIextDocument::modificationChanged, q,
                         &QWidgetTextControl::modificationChanged);
        QObject::connect(doc, &BOBUIextDocument::blockCountChanged, q,
                         &QWidgetTextControl::blockCountChanged);
    }

    bool previousUndoRedoState = doc->isUndoRedoEnabled();
    if (!document)
        doc->setUndoRedoEnabled(false);

    //Saving the index save some time.
    static int contentsChangedIndex = QMetaMethod::fromSignal(&BOBUIextDocument::contentsChanged).methodIndex();
    static int textChangedIndex = QMetaMethod::fromSignal(&QWidgetTextControl::textChanged).methodIndex();
    // avoid multiple textChanged() signals being emitted
    QMetaObject::disconnect(doc, contentsChangedIndex, q, textChangedIndex);

    if (!text.isEmpty()) {
        // clear 'our' cursor for insertion to prevent
        // the emission of the cursorPositionChanged() signal.
        // instead we emit it only once at the end instead of
        // at the end of the document after loading and when
        // positioning the cursor again to the start of the
        // document.
        cursor = BOBUIextCursor();
        if (format == BobUI::PlainText) {
            BOBUIextCursor formatCursor(doc);
            // put the setPlainText and the setCharFormat into one edit block,
            // so that the syntax highlight triggers only /once/ for the entire
            // document, not twice.
            formatCursor.beginEditBlock();
            doc->setPlainText(text);
            doc->setUndoRedoEnabled(false);
            formatCursor.select(BOBUIextCursor::Document);
            formatCursor.setCharFormat(charFormatForInsertion);
            formatCursor.endEditBlock();
#if BOBUI_CONFIG(textmarkdownreader)
        } else if (format == BobUI::MarkdownText) {
            doc->setMarkdown(text);
            doc->setUndoRedoEnabled(false);
#endif
        } else {
#ifndef BOBUI_NO_TEXTHTMLPARSER
            doc->setHtml(text);
#else
            doc->setPlainText(text);
#endif
            doc->setUndoRedoEnabled(false);
        }
        cursor = BOBUIextCursor(doc);
    } else if (clearDocument) {
        doc->clear();
    }
    cursor.setCharFormat(charFormatForInsertion);

    QMetaObject::connect(doc, contentsChangedIndex, q, textChangedIndex);
    emit q->textChanged();
    if (!document)
        doc->setUndoRedoEnabled(previousUndoRedoState);
    _q_updateCurrentCharFormatAndSelection();
    if (!document)
        doc->setModified(false);

    q->ensureCursorVisible();
    emit q->cursorPositionChanged();

    QObjectPrivate::connect(doc, &BOBUIextDocument::contentsChange, this,
                            &QWidgetTextControlPrivate::_q_contentsChanged, BobUI::UniqueConnection);
}

void QWidgetTextControlPrivate::startDrag()
{

#ifdef Q_OS_WASM
    // QDrag::exec() will crash without asyncify; disable drag instead.
    if (!qstdweb::haveAsyncify())
        return;
#endif

#if BOBUI_CONFIG(draganddrop)
    Q_Q(QWidgetTextControl);
    mousePressed = false;
    if (!contextWidget)
        return;
    QMimeData *data = q->createMimeDataFromSelection();

    QDrag *drag = new QDrag(contextWidget);
    drag->setMimeData(data);

    BobUI::DropActions actions = BobUI::CopyAction;
    BobUI::DropAction action;
    if (interactionFlags & BobUI::TextEditable) {
        actions |= BobUI::MoveAction;
        action = drag->exec(actions, BobUI::MoveAction);
    } else {
        action = drag->exec(actions, BobUI::CopyAction);
    }

    if (action == BobUI::MoveAction && drag->target() != contextWidget)
        cursor.removeSelectedText();
#endif
}

void QWidgetTextControlPrivate::setCursorPosition(const QPointF &pos)
{
    Q_Q(QWidgetTextControl);
    const int cursorPos = q->hitTest(pos, BobUI::FuzzyHit);
    if (cursorPos == -1)
        return;
    cursor.setPosition(cursorPos);
}

void QWidgetTextControlPrivate::setCursorPosition(int pos, BOBUIextCursor::MoveMode mode)
{
    cursor.setPosition(pos, mode);

    if (mode != BOBUIextCursor::KeepAnchor) {
        selectedWordOnDoubleClick = BOBUIextCursor();
        selectedBlockOnTrippleClick = BOBUIextCursor();
    }
}

void QWidgetTextControlPrivate::repaintCursor()
{
    Q_Q(QWidgetTextControl);
    emit q->updateRequest(cursorRectPlusUnicodeDirectionMarkers(cursor));
}

void QWidgetTextControlPrivate::repaintOldAndNewSelection(const BOBUIextCursor &oldSelection)
{
    Q_Q(QWidgetTextControl);
    if (cursor.hasSelection()
        && oldSelection.hasSelection()
        && cursor.currentFrame() == oldSelection.currentFrame()
        && !cursor.hasComplexSelection()
        && !oldSelection.hasComplexSelection()
        && cursor.anchor() == oldSelection.anchor()
        ) {
        BOBUIextCursor differenceSelection(doc);
        differenceSelection.setPosition(oldSelection.position());
        differenceSelection.setPosition(cursor.position(), BOBUIextCursor::KeepAnchor);
        emit q->updateRequest(q->selectionRect(differenceSelection));
    } else {
        if (!oldSelection.isNull())
            emit q->updateRequest(q->selectionRect(oldSelection) | cursorRectPlusUnicodeDirectionMarkers(oldSelection));
        emit q->updateRequest(q->selectionRect() | cursorRectPlusUnicodeDirectionMarkers(cursor));
    }
}

void QWidgetTextControlPrivate::selectionChanged(bool forceEmitSelectionChanged /*=false*/)
{
    Q_Q(QWidgetTextControl);
    if (forceEmitSelectionChanged) {
        emit q->selectionChanged();
#if BOBUI_CONFIG(accessibility)
        if (q->parent() && q->parent()->isWidgetType()) {
            QAccessibleTextSelectionEvent ev(q->parent(), cursor.anchor(), cursor.position());
            QAccessible::updateAccessibility(&ev);
        }
#endif
    }

    if (cursor.position() == lastSelectionPosition
        && cursor.anchor() == lastSelectionAnchor)
        return;

    bool selectionStateChange = (cursor.hasSelection()
                                 != (lastSelectionPosition != lastSelectionAnchor));
    if (selectionStateChange)
        emit q->copyAvailable(cursor.hasSelection());

    if (!forceEmitSelectionChanged
        && (selectionStateChange
            || (cursor.hasSelection()
                && (cursor.position() != lastSelectionPosition
                    || cursor.anchor() != lastSelectionAnchor)))) {
        emit q->selectionChanged();
#if BOBUI_CONFIG(accessibility)
        if (q->parent() && q->parent()->isWidgetType()) {
            QAccessibleTextSelectionEvent ev(q->parent(), cursor.anchor(), cursor.position());
            QAccessible::updateAccessibility(&ev);
        }
#endif
    }
    emit q->microFocusChanged();
    lastSelectionPosition = cursor.position();
    lastSelectionAnchor = cursor.anchor();
}

void QWidgetTextControlPrivate::_q_updateCurrentCharFormatAndSelection()
{
    updateCurrentCharFormat();
    selectionChanged();
}

#ifndef BOBUI_NO_CLIPBOARD
void QWidgetTextControlPrivate::setClipboardSelection()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (!cursor.hasSelection() || !clipboard->supportsSelection())
        return;
    Q_Q(QWidgetTextControl);
    QMimeData *data = q->createMimeDataFromSelection();
    clipboard->setMimeData(data, QClipboard::Selection);
}
#endif

void QWidgetTextControlPrivate::_q_emitCursorPosChanged(const BOBUIextCursor &someCursor)
{
    Q_Q(QWidgetTextControl);
    if (someCursor.isCopyOf(cursor)) {
        emit q->cursorPositionChanged();
        emit q->microFocusChanged();
    }
}

void QWidgetTextControlPrivate::_q_contentsChanged(int from, int charsRemoved, int charsAdded)
{
#if BOBUI_CONFIG(accessibility)
    Q_Q(QWidgetTextControl);

    if (QAccessible::isActive() && q->parent() && q->parent()->isWidgetType()) {
        BOBUIextCursor tmp(doc);
        tmp.setPosition(from);
        // when setting a new text document the length is off
        // BOBUIBUG-32583 - characterCount is off by 1 requires the -1
        tmp.setPosition(qMin(doc->characterCount() - 1, from + charsAdded), BOBUIextCursor::KeepAnchor);
        QString newText = tmp.selectedText();

        // always report the right number of removed chars, but in lack of the real string use spaces
        QString oldText = QString(charsRemoved, u' ');

        QAccessibleEvent *ev = nullptr;
        if (charsRemoved == 0) {
            ev = new QAccessibleTextInsertEvent(q->parent(), from, newText);
        } else if (charsAdded == 0) {
            ev = new QAccessibleTextRemoveEvent(q->parent(), from, oldText);
        } else {
            ev = new QAccessibleTextUpdateEvent(q->parent(), from, oldText, newText);
        }
        QAccessible::updateAccessibility(ev);
        delete ev;
    }
#else
    Q_UNUSED(from);
    Q_UNUSED(charsRemoved);
    Q_UNUSED(charsAdded);
#endif
}

void QWidgetTextControlPrivate::_q_documentLayoutChanged()
{
    Q_Q(QWidgetTextControl);
    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    QObject::connect(layout, &QAbstractTextDocumentLayout::update, q,
                     &QWidgetTextControl::updateRequest);
    QObjectPrivate::connect(layout, &QAbstractTextDocumentLayout::updateBlock, this,
                            &QWidgetTextControlPrivate::_q_updateBlock);
    QObject::connect(layout, &QAbstractTextDocumentLayout::documentSizeChanged, q,
                     &QWidgetTextControl::documentSizeChanged);
}

void QWidgetTextControlPrivate::setCursorVisible(bool visible)
{
    if (cursorVisible == visible)
        return;

    cursorVisible = visible;
    updateCursorBlinking();

    if (cursorVisible)
        connect(QGuiApplication::styleHints(), &QStyleHints::cursorFlashTimeChanged, this, &QWidgetTextControlPrivate::updateCursorBlinking);
    else
        disconnect(QGuiApplication::styleHints(), &QStyleHints::cursorFlashTimeChanged, this, &QWidgetTextControlPrivate::updateCursorBlinking);
}

void QWidgetTextControlPrivate::updateCursorBlinking()
{
    cursorBlinkTimer.stop();
    if (cursorVisible) {
        int flashTime = QGuiApplication::styleHints()->cursorFlashTime();
        if (flashTime >= 2)
            cursorBlinkTimer.start(flashTime / 2, q_func());
    }

    cursorOn = cursorVisible;
    repaintCursor();
}

void QWidgetTextControlPrivate::extendWordwiseSelection(int suggestedNewPosition, qreal mouseXPosition)
{
    Q_Q(QWidgetTextControl);

    // if inside the initial selected word keep that
    if (suggestedNewPosition >= selectedWordOnDoubleClick.selectionStart()
        && suggestedNewPosition <= selectedWordOnDoubleClick.selectionEnd()) {
        q->setTextCursor(selectedWordOnDoubleClick);
        return;
    }

    BOBUIextCursor curs = selectedWordOnDoubleClick;
    curs.setPosition(suggestedNewPosition, BOBUIextCursor::KeepAnchor);

    if (!curs.movePosition(BOBUIextCursor::StartOfWord))
        return;
    const int wordStartPos = curs.position();

    const int blockPos = curs.block().position();
    const QPointF blockCoordinates = q->blockBoundingRect(curs.block()).topLeft();

    BOBUIextLine line = currentTextLine(curs);
    if (!line.isValid())
        return;

    const qreal wordStartX = line.cursorToX(curs.position() - blockPos) + blockCoordinates.x();

    if (!curs.movePosition(BOBUIextCursor::EndOfWord))
        return;
    const int wordEndPos = curs.position();

    const BOBUIextLine otherLine = currentTextLine(curs);
    if (otherLine.textStart() != line.textStart()
        || wordEndPos == wordStartPos)
        return;

    const qreal wordEndX = line.cursorToX(curs.position() - blockPos) + blockCoordinates.x();

    if (!wordSelectionEnabled && (mouseXPosition < wordStartX || mouseXPosition > wordEndX))
        return;

    if (wordSelectionEnabled) {
        if (suggestedNewPosition < selectedWordOnDoubleClick.position()) {
            cursor.setPosition(selectedWordOnDoubleClick.selectionEnd());
            setCursorPosition(wordStartPos, BOBUIextCursor::KeepAnchor);
        } else {
            cursor.setPosition(selectedWordOnDoubleClick.selectionStart());
            setCursorPosition(wordEndPos, BOBUIextCursor::KeepAnchor);
        }
    } else {
        // keep the already selected word even when moving to the left
        // (#39164)
        if (suggestedNewPosition < selectedWordOnDoubleClick.position())
            cursor.setPosition(selectedWordOnDoubleClick.selectionEnd());
        else
            cursor.setPosition(selectedWordOnDoubleClick.selectionStart());

        const qreal differenceToStart = mouseXPosition - wordStartX;
        const qreal differenceToEnd = wordEndX - mouseXPosition;

        if (differenceToStart < differenceToEnd)
            setCursorPosition(wordStartPos, BOBUIextCursor::KeepAnchor);
        else
            setCursorPosition(wordEndPos, BOBUIextCursor::KeepAnchor);
    }

    if (interactionFlags & BobUI::TextSelectableByMouse) {
#ifndef BOBUI_NO_CLIPBOARD
        setClipboardSelection();
#endif
        selectionChanged(true);
    }
}

void QWidgetTextControlPrivate::extendBlockwiseSelection(int suggestedNewPosition)
{
    Q_Q(QWidgetTextControl);

    // if inside the initial selected line keep that
    if (suggestedNewPosition >= selectedBlockOnTrippleClick.selectionStart()
        && suggestedNewPosition <= selectedBlockOnTrippleClick.selectionEnd()) {
        q->setTextCursor(selectedBlockOnTrippleClick);
        return;
    }

    if (suggestedNewPosition < selectedBlockOnTrippleClick.position()) {
        cursor.setPosition(selectedBlockOnTrippleClick.selectionEnd());
        cursor.setPosition(suggestedNewPosition, BOBUIextCursor::KeepAnchor);
        cursor.movePosition(BOBUIextCursor::StartOfBlock, BOBUIextCursor::KeepAnchor);
    } else {
        cursor.setPosition(selectedBlockOnTrippleClick.selectionStart());
        cursor.setPosition(suggestedNewPosition, BOBUIextCursor::KeepAnchor);
        cursor.movePosition(BOBUIextCursor::EndOfBlock, BOBUIextCursor::KeepAnchor);
        cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
    }

    if (interactionFlags & BobUI::TextSelectableByMouse) {
#ifndef BOBUI_NO_CLIPBOARD
        setClipboardSelection();
#endif
        selectionChanged(true);
    }
}

void QWidgetTextControlPrivate::_q_deleteSelected()
{
    if (!(interactionFlags & BobUI::TextEditable) || !cursor.hasSelection())
        return;
    cursor.removeSelectedText();
}

void QWidgetTextControl::undo()
{
    Q_D(QWidgetTextControl);
    d->repaintSelection();
    const int oldCursorPos = d->cursor.position();
    d->doc->undo(&d->cursor);
    if (d->cursor.position() != oldCursorPos)
        emit cursorPositionChanged();
    emit microFocusChanged();
    ensureCursorVisible();
}

void QWidgetTextControl::redo()
{
    Q_D(QWidgetTextControl);
    d->repaintSelection();
    const int oldCursorPos = d->cursor.position();
    d->doc->redo(&d->cursor);
        if (d->cursor.position() != oldCursorPos)
        emit cursorPositionChanged();
    emit microFocusChanged();
    ensureCursorVisible();
}

QWidgetTextControl::QWidgetTextControl(QObject *parent)
    : QInputControl(QInputControl::TextEdit, *new QWidgetTextControlPrivate, parent)
{
    Q_D(QWidgetTextControl);
    d->init();
}

QWidgetTextControl::QWidgetTextControl(const QString &text, QObject *parent)
    : QInputControl(QInputControl::TextEdit, *new QWidgetTextControlPrivate, parent)
{
    Q_D(QWidgetTextControl);
    d->init(BobUI::RichText, text);
}

QWidgetTextControl::QWidgetTextControl(BOBUIextDocument *doc, QObject *parent)
    : QInputControl(QInputControl::TextEdit, *new QWidgetTextControlPrivate, parent)
{
    Q_D(QWidgetTextControl);
    d->init(BobUI::RichText, QString(), doc);
}

QWidgetTextControl::~QWidgetTextControl()
{
}

void QWidgetTextControl::setDocument(BOBUIextDocument *document)
{
    Q_D(QWidgetTextControl);
    if (d->doc == document)
        return;

    d->doc->disconnect(this);
    d->doc->documentLayout()->disconnect(this);
    d->doc->documentLayout()->setPaintDevice(nullptr);

    if (d->doc->parent() == this)
        delete d->doc;

    d->doc = nullptr;
    d->setContent(BobUI::RichText, QString(), document);
}

BOBUIextDocument *QWidgetTextControl::document() const
{
    Q_D(const QWidgetTextControl);
    return d->doc;
}

void QWidgetTextControl::setTextCursor(const BOBUIextCursor &cursor, bool selectionClipboard)
{
    Q_D(QWidgetTextControl);
    d->cursorIsFocusIndicator = false;
    const bool posChanged = cursor.position() != d->cursor.position();
    const BOBUIextCursor oldSelection = d->cursor;
    d->cursor = cursor;
    d->cursorOn = d->hasFocus
            && (d->interactionFlags & (BobUI::TextSelectableByKeyboard | BobUI::TextEditable));
    d->_q_updateCurrentCharFormatAndSelection();
    ensureCursorVisible();
    d->repaintOldAndNewSelection(oldSelection);
    if (posChanged)
        emit cursorPositionChanged();

#ifndef BOBUI_NO_CLIPBOARD
    if (selectionClipboard)
        d->setClipboardSelection();
#else
    Q_UNUSED(selectionClipboard);
#endif
}

BOBUIextCursor QWidgetTextControl::textCursor() const
{
    Q_D(const QWidgetTextControl);
    return d->cursor;
}

#ifndef BOBUI_NO_CLIPBOARD

void QWidgetTextControl::cut()
{
    Q_D(QWidgetTextControl);
    if (!(d->interactionFlags & BobUI::TextEditable) || !d->cursor.hasSelection())
        return;
    copy();
    d->cursor.removeSelectedText();
}

void QWidgetTextControl::copy()
{
    Q_D(QWidgetTextControl);
    if (!d->cursor.hasSelection())
        return;
    QMimeData *data = createMimeDataFromSelection();
    QGuiApplication::clipboard()->setMimeData(data);
}

void QWidgetTextControl::paste(QClipboard::Mode mode)
{
    const QMimeData *md = QGuiApplication::clipboard()->mimeData(mode);
    if (md)
        insertFromMimeData(md);
}
#endif

void QWidgetTextControl::clear()
{
    Q_D(QWidgetTextControl);
    // clears and sets empty content
    d->extraSelections.clear();
    d->setContent();
}


void QWidgetTextControl::selectAll()
{
    Q_D(QWidgetTextControl);
    const int selectionLength = qAbs(d->cursor.position() - d->cursor.anchor());
    const int oldCursorPos = d->cursor.position();
    d->cursor.select(BOBUIextCursor::Document);
    d->selectionChanged(selectionLength != qAbs(d->cursor.position() - d->cursor.anchor()));
    d->cursorIsFocusIndicator = false;
    if (d->cursor.position() != oldCursorPos)
        emit cursorPositionChanged();
    emit updateRequest();
}

void QWidgetTextControl::processEvent(QEvent *e, const QPointF &coordinateOffset, QWidget *contextWidget)
{
    BOBUIransform t;
    t.translate(coordinateOffset.x(), coordinateOffset.y());
    processEvent(e, t, contextWidget);
}

void QWidgetTextControl::processEvent(QEvent *e, const BOBUIransform &transform, QWidget *contextWidget)
{
    Q_D(QWidgetTextControl);
    if (d->interactionFlags == BobUI::NoTextInteraction) {
        e->ignore();
        return;
    }

    d->contextWidget = contextWidget;

    if (!d->contextWidget) {
        switch (e->type()) {
#if BOBUI_CONFIG(graphicsview)
            case QEvent::GraphicsSceneMouseMove:
            case QEvent::GraphicsSceneMousePress:
            case QEvent::GraphicsSceneMouseRelease:
            case QEvent::GraphicsSceneMouseDoubleClick:
            case QEvent::GraphicsSceneContextMenu:
            case QEvent::GraphicsSceneHoverEnter:
            case QEvent::GraphicsSceneHoverMove:
            case QEvent::GraphicsSceneHoverLeave:
            case QEvent::GraphicsSceneHelp:
            case QEvent::GraphicsSceneDragEnter:
            case QEvent::GraphicsSceneDragMove:
            case QEvent::GraphicsSceneDragLeave:
            case QEvent::GraphicsSceneDrop: {
                QGraphicsSceneEvent *ev = static_cast<QGraphicsSceneEvent *>(e);
                d->contextWidget = ev->widget();
                break;
            }
#endif // BOBUI_CONFIG(graphicsview)
            default: break;
        };
    }

    switch (e->type()) {
        case QEvent::KeyPress:
            d->keyPressEvent(static_cast<QKeyEvent *>(e));
            break;
        case QEvent::MouseButtonPress: {
            QMouseEvent *ev = static_cast<QMouseEvent *>(e);
            d->mousePressEvent(ev, ev->button(), transform.map(ev->position().toPoint()), ev->modifiers(),
                               ev->buttons(), ev->globalPosition().toPoint());
            break; }
        case QEvent::MouseMove: {
            QMouseEvent *ev = static_cast<QMouseEvent *>(e);
            d->mouseMoveEvent(ev, ev->button(), transform.map(ev->position().toPoint()), ev->modifiers(),
                              ev->buttons(), ev->globalPosition().toPoint());
            break; }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *ev = static_cast<QMouseEvent *>(e);
            d->mouseReleaseEvent(ev, ev->button(), transform.map(ev->position().toPoint()), ev->modifiers(),
                                 ev->buttons(), ev->globalPosition().toPoint());
            break; }
        case QEvent::MouseButtonDblClick: {
            QMouseEvent *ev = static_cast<QMouseEvent *>(e);
            d->mouseDoubleClickEvent(ev, ev->button(), transform.map(ev->position().toPoint()), ev->modifiers(),
                                     ev->buttons(), ev->globalPosition().toPoint());
            break; }
        case QEvent::InputMethod:
            d->inputMethodEvent(static_cast<QInputMethodEvent *>(e));
            break;
#ifndef BOBUI_NO_CONTEXTMENU
    case QEvent::ContextMenu: {
            QContextMenuEvent *ev = static_cast<QContextMenuEvent *>(e);
            d->contextMenuEvent(ev->globalPos(), transform.map(ev->pos()), contextWidget);
            break; }
#endif // BOBUI_NO_CONTEXTMENU
        case QEvent::FocusIn:
        case QEvent::FocusOut:
            d->focusEvent(static_cast<QFocusEvent *>(e));
            break;

        case QEvent::EnabledChange:
            d->isEnabled = e->isAccepted();
            break;

#if BOBUI_CONFIG(tooltip)
        case QEvent::ToolTip: {
            QHelpEvent *ev = static_cast<QHelpEvent *>(e);
            d->showToolTip(ev->globalPos(), transform.map(ev->pos()), contextWidget);
            break;
        }
#endif // BOBUI_CONFIG(tooltip)

#if BOBUI_CONFIG(draganddrop)
        case QEvent::DragEnter: {
            QDragEnterEvent *ev = static_cast<QDragEnterEvent *>(e);
            if (d->dragEnterEvent(e, ev->mimeData()))
                ev->acceptProposedAction();
            break;
        }
        case QEvent::DragLeave:
            d->dragLeaveEvent();
            break;
        case QEvent::DragMove: {
            QDragMoveEvent *ev = static_cast<QDragMoveEvent *>(e);
            if (d->dragMoveEvent(e, ev->mimeData(), transform.map(ev->position().toPoint())))
                ev->acceptProposedAction();
            break;
        }
        case QEvent::Drop: {
            QDropEvent *ev = static_cast<QDropEvent *>(e);
            if (d->dropEvent(ev->mimeData(), transform.map(ev->position().toPoint()), ev->dropAction(), ev->source()))
                ev->acceptProposedAction();
            break;
        }
#endif

#if BOBUI_CONFIG(graphicsview)
        case QEvent::GraphicsSceneMousePress: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
            d->mousePressEvent(ev, ev->button(), transform.map(ev->pos()), ev->modifiers(), ev->buttons(),
                               ev->screenPos());
            break; }
        case QEvent::GraphicsSceneMouseMove: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
            d->mouseMoveEvent(ev, ev->button(), transform.map(ev->pos()), ev->modifiers(), ev->buttons(),
                              ev->screenPos());
            break; }
        case QEvent::GraphicsSceneMouseRelease: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
            d->mouseReleaseEvent(ev, ev->button(), transform.map(ev->pos()), ev->modifiers(), ev->buttons(),
                                 ev->screenPos());
            break; }
        case QEvent::GraphicsSceneMouseDoubleClick: {
            QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent *>(e);
            d->mouseDoubleClickEvent(ev, ev->button(), transform.map(ev->pos()), ev->modifiers(), ev->buttons(),
                                     ev->screenPos());
            break; }
        case QEvent::GraphicsSceneContextMenu: {
            QGraphicsSceneContextMenuEvent *ev = static_cast<QGraphicsSceneContextMenuEvent *>(e);
            d->contextMenuEvent(ev->screenPos(), transform.map(ev->pos()), contextWidget);
            break; }

        case QEvent::GraphicsSceneHoverMove: {
            QGraphicsSceneHoverEvent *ev = static_cast<QGraphicsSceneHoverEvent *>(e);
            d->mouseMoveEvent(ev, BobUI::NoButton, transform.map(ev->pos()), ev->modifiers(),BobUI::NoButton,
                              ev->screenPos());
            break; }

        case QEvent::GraphicsSceneDragEnter: {
            QGraphicsSceneDragDropEvent *ev = static_cast<QGraphicsSceneDragDropEvent *>(e);
            if (d->dragEnterEvent(e, ev->mimeData()))
                ev->acceptProposedAction();
            break; }
        case QEvent::GraphicsSceneDragLeave:
            d->dragLeaveEvent();
            break;
        case QEvent::GraphicsSceneDragMove: {
            QGraphicsSceneDragDropEvent *ev = static_cast<QGraphicsSceneDragDropEvent *>(e);
            if (d->dragMoveEvent(e, ev->mimeData(), transform.map(ev->pos())))
                ev->acceptProposedAction();
            break; }
        case QEvent::GraphicsSceneDrop: {
            QGraphicsSceneDragDropEvent *ev = static_cast<QGraphicsSceneDragDropEvent *>(e);
            if (d->dropEvent(ev->mimeData(), transform.map(ev->pos()), ev->dropAction(), ev->source()))
                ev->accept();
            break; }
#endif // BOBUI_CONFIG(graphicsview)
#ifdef BOBUI_KEYPAD_NAVIGATION
        case QEvent::EnterEditFocus:
        case QEvent::LeaveEditFocus:
            if (QApplicationPrivate::keypadNavigationEnabled())
                d->editFocusEvent(e);
            break;
#endif
        case QEvent::ShortcutOverride:
            if (d->interactionFlags & BobUI::TextEditable) {
                QKeyEvent* ke = static_cast<QKeyEvent *>(e);
                if (isCommonTextEditShortcut(ke))
                    ke->accept();
            }
            break;
        default:
            break;
    }
}

bool QWidgetTextControl::event(QEvent *e)
{
    return QObject::event(e);
}

void QWidgetTextControl::timerEvent(BOBUIimerEvent *e)
{
    Q_D(QWidgetTextControl);
    if (e->timerId() == d->cursorBlinkTimer.timerId()) {
        d->cursorOn = !d->cursorOn;

        if (d->cursor.hasSelection())
            d->cursorOn &= (QApplication::style()->styleHint(QStyle::SH_BlinkCursorWhenTextSelected)
                            != 0);

        d->repaintCursor();
    } else if (e->timerId() == d->trippleClickTimer.timerId()) {
        d->trippleClickTimer.stop();
    }
}

void QWidgetTextControl::setPlainText(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->setContent(BobUI::PlainText, text);
}

#if BOBUI_CONFIG(textmarkdownreader)
void QWidgetTextControl::setMarkdown(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->setContent(BobUI::MarkdownText, text);
}
#endif

void QWidgetTextControl::setHtml(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->setContent(BobUI::RichText, text);
}

void QWidgetTextControlPrivate::keyPressEvent(QKeyEvent *e)
{
    Q_Q(QWidgetTextControl);
#ifndef BOBUI_NO_SHORTCUT
    if (e == QKeySequence::SelectAll) {
            e->accept();
            q->selectAll();
#ifndef BOBUI_NO_CLIPBOARD
            setClipboardSelection();
#endif
            return;
    }
#ifndef BOBUI_NO_CLIPBOARD
    else if (e == QKeySequence::Copy) {
            e->accept();
            q->copy();
            return;
    }
#endif
#endif // BOBUI_NO_SHORTCUT

    if (interactionFlags & BobUI::TextSelectableByKeyboard
        && cursorMoveKeyEvent(e))
        goto accept;

    if (interactionFlags & BobUI::LinksAccessibleByKeyboard) {
        if ((e->key() == BobUI::Key_Return
             || e->key() == BobUI::Key_Enter
#ifdef BOBUI_KEYPAD_NAVIGATION
             || e->key() == BobUI::Key_Select
#endif
             )
            && cursor.hasSelection()) {

            e->accept();
            activateLinkUnderCursor();
            return;
        }
    }

    if (!(interactionFlags & BobUI::TextEditable)) {
        e->ignore();
        return;
    }

    if (e->key() == BobUI::Key_Direction_L || e->key() == BobUI::Key_Direction_R) {
        BOBUIextBlockFormat fmt;
        fmt.setLayoutDirection((e->key() == BobUI::Key_Direction_L) ? BobUI::LeftToRight : BobUI::RightToLeft);
        cursor.mergeBlockFormat(fmt);
        goto accept;
    }

    // schedule a repaint of the region of the cursor, as when we move it we
    // want to make sure the old cursor disappears (not noticeable when moving
    // only a few pixels but noticeable when jumping between cells in tables for
    // example)
    repaintSelection();

    if (e->key() == BobUI::Key_Backspace && !(e->modifiers() & ~(BobUI::ShiftModifier | BobUI::GroupSwitchModifier))) {
        BOBUIextBlockFormat blockFmt = cursor.blockFormat();
        BOBUIextList *list = cursor.currentList();
        if (list && cursor.atBlockStart() && !cursor.hasSelection()) {
            list->remove(cursor.block());
        } else if (cursor.atBlockStart() && blockFmt.indent() > 0) {
            blockFmt.setIndent(blockFmt.indent() - 1);
            cursor.setBlockFormat(blockFmt);
        } else {
            BOBUIextCursor localCursor = cursor;
            localCursor.deletePreviousChar();
            if (cursor.d)
                cursor.d->setX();
        }
        goto accept;
    }
#ifndef BOBUI_NO_SHORTCUT
      else if (e == QKeySequence::InsertParagraphSeparator) {
        insertParagraphSeparator();
        e->accept();
        goto accept;
    } else if (e == QKeySequence::InsertLineSeparator) {
        cursor.insertText(QString(QChar::LineSeparator));
        e->accept();
        goto accept;
    }
#endif
    if (false) {
    }
#ifndef BOBUI_NO_SHORTCUT
    else if (e == QKeySequence::Undo) {
            q->undo();
    }
    else if (e == QKeySequence::Redo) {
           q->redo();
    }
#ifndef BOBUI_NO_CLIPBOARD
    else if (e == QKeySequence::Cut) {
           q->cut();
    }
    else if (e == QKeySequence::Paste) {
        QClipboard::Mode mode = QClipboard::Clipboard;
        if (QGuiApplication::clipboard()->supportsSelection()) {
            if (e->modifiers() == (BobUI::CTRL | BobUI::SHIFT) && e->key() == BobUI::Key_Insert)
                mode = QClipboard::Selection;
        }
        q->paste(mode);
    }
#endif
    else if (e == QKeySequence::Delete) {
        BOBUIextCursor localCursor = cursor;
        localCursor.deleteChar();
        if (cursor.d)
            cursor.d->setX();
    } else if (e == QKeySequence::Backspace) {
        BOBUIextCursor localCursor = cursor;
        localCursor.deletePreviousChar();
        if (cursor.d)
            cursor.d->setX();
    }else if (e == QKeySequence::DeleteEndOfWord) {
        if (!cursor.hasSelection())
            cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
    else if (e == QKeySequence::DeleteStartOfWord) {
        if (!cursor.hasSelection())
            cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
    else if (e == QKeySequence::DeleteEndOfLine) {
        BOBUIextBlock block = cursor.block();
        if (cursor.position() == block.position() + block.length() - 2)
            cursor.movePosition(BOBUIextCursor::Right, BOBUIextCursor::KeepAnchor);
        else
            cursor.movePosition(BOBUIextCursor::EndOfBlock, BOBUIextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
#endif // BOBUI_NO_SHORTCUT
    else {
        goto process;
    }
    goto accept;

process:
    {
        if (q->isAcceptableInput(e)) {
            if (overwriteMode
                // no need to call deleteChar() if we have a selection, insertText
                // does it already
                && !cursor.hasSelection()
                && !cursor.atBlockEnd())
                cursor.deleteChar();

            cursor.insertText(e->text());
            selectionChanged();
        } else {
            e->ignore();
            return;
        }
    }

 accept:

#ifndef BOBUI_NO_CLIPBOARD
    setClipboardSelection();
#endif

    e->accept();
    cursorOn = true;

    q->ensureCursorVisible();

    updateCurrentCharFormat();
}

QVariant QWidgetTextControl::loadResource(int type, const QUrl &name)
{
    Q_UNUSED(type);
    Q_UNUSED(name);
    return QVariant();
}

void QWidgetTextControlPrivate::_q_updateBlock(const BOBUIextBlock &block)
{
    Q_Q(QWidgetTextControl);
    QRectF br = q->blockBoundingRect(block);
    br.setRight(qreal(INT_MAX)); // the block might have shrunk
    emit q->updateRequest(br);
}

QRectF QWidgetTextControlPrivate::rectForPosition(int position) const
{
    Q_Q(const QWidgetTextControl);
    const BOBUIextBlock block = doc->findBlock(position);
    if (!block.isValid())
        return QRectF();
    const QAbstractTextDocumentLayout *docLayout = doc->documentLayout();
    const BOBUIextLayout *layout = block.layout();
    const QPointF layoutPos = q->blockBoundingRect(block).topLeft();
    int relativePos = position - block.position();
    if (preeditCursor != 0) {
        int preeditPos = layout->preeditAreaPosition();
        if (relativePos == preeditPos)
            relativePos += preeditCursor;
        else if (relativePos > preeditPos)
            relativePos += layout->preeditAreaText().size();
    }
    BOBUIextLine line = layout->lineForTextPosition(relativePos);

    int cursorWidth;
    {
        bool ok = false;
        cursorWidth = docLayout->property("cursorWidth").toInt(&ok);
        if (!ok)
            cursorWidth = 1;
    }

    QRectF r;

    if (line.isValid()) {
        qreal x = line.cursorToX(relativePos);
        qreal w = 0;
        if (overwriteMode) {
            if (relativePos < line.textLength() - line.textStart())
                w = line.cursorToX(relativePos + 1) - x;
            else
                w = QFontMetrics(block.layout()->font()).horizontalAdvance(u' '); // in sync with BOBUIextLine::draw()
        }
        r = QRectF(layoutPos.x() + x, layoutPos.y() + line.y(),
                   cursorWidth + w, line.height());
    } else {
        r = QRectF(layoutPos.x(), layoutPos.y(), cursorWidth, 10); // #### correct height
    }

    return r;
}

namespace {
struct BOBUIextFrameComparator {
    bool operator()(BOBUIextFrame *frame, int position) { return frame->firstPosition() < position; }
    bool operator()(int position, BOBUIextFrame *frame) { return position < frame->firstPosition(); }
};
}

static QRectF boundingRectOfFloatsInSelection(const BOBUIextCursor &cursor)
{
    QRectF r;
    BOBUIextFrame *frame = cursor.currentFrame();
    const QList<BOBUIextFrame *> children = frame->childFrames();

    const QList<BOBUIextFrame *>::ConstIterator firstFrame = std::lower_bound(children.constBegin(), children.constEnd(),
                                                                           cursor.selectionStart(), BOBUIextFrameComparator());
    const QList<BOBUIextFrame *>::ConstIterator lastFrame = std::upper_bound(children.constBegin(), children.constEnd(),
                                                                          cursor.selectionEnd(), BOBUIextFrameComparator());
    for (QList<BOBUIextFrame *>::ConstIterator it = firstFrame; it != lastFrame; ++it) {
        if ((*it)->frameFormat().position() != BOBUIextFrameFormat::InFlow)
            r |= frame->document()->documentLayout()->frameBoundingRect(*it);
    }
    return r;
}

QRectF QWidgetTextControl::selectionRect(const BOBUIextCursor &cursor) const
{
    Q_D(const QWidgetTextControl);

    QRectF r = d->rectForPosition(cursor.selectionStart());

    if (cursor.hasComplexSelection() && cursor.currentTable()) {
        BOBUIextTable *table = cursor.currentTable();

        r = d->doc->documentLayout()->frameBoundingRect(table);
        /*
        int firstRow, numRows, firstColumn, numColumns;
        cursor.selectedTableCells(&firstRow, &numRows, &firstColumn, &numColumns);

        const BOBUIextTableCell firstCell = table->cellAt(firstRow, firstColumn);
        const BOBUIextTableCell lastCell = table->cellAt(firstRow + numRows - 1, firstColumn + numColumns - 1);

        const QAbstractTextDocumentLayout * const layout = doc->documentLayout();

        QRectF tableSelRect = layout->blockBoundingRect(firstCell.firstCursorPosition().block());

        for (int col = firstColumn; col < firstColumn + numColumns; ++col) {
            const BOBUIextTableCell cell = table->cellAt(firstRow, col);
            const qreal y = layout->blockBoundingRect(cell.firstCursorPosition().block()).top();

            tableSelRect.setTop(qMin(tableSelRect.top(), y));
        }

        for (int row = firstRow; row < firstRow + numRows; ++row) {
            const BOBUIextTableCell cell = table->cellAt(row, firstColumn);
            const qreal x = layout->blockBoundingRect(cell.firstCursorPosition().block()).left();

            tableSelRect.setLeft(qMin(tableSelRect.left(), x));
        }

        for (int col = firstColumn; col < firstColumn + numColumns; ++col) {
            const BOBUIextTableCell cell = table->cellAt(firstRow + numRows - 1, col);
            const qreal y = layout->blockBoundingRect(cell.lastCursorPosition().block()).bottom();

            tableSelRect.setBottom(qMax(tableSelRect.bottom(), y));
        }

        for (int row = firstRow; row < firstRow + numRows; ++row) {
            const BOBUIextTableCell cell = table->cellAt(row, firstColumn + numColumns - 1);
            const qreal x = layout->blockBoundingRect(cell.lastCursorPosition().block()).right();

            tableSelRect.setRight(qMax(tableSelRect.right(), x));
        }

        r = tableSelRect.toRect();
        */
    } else if (cursor.hasSelection()) {
        const int position = cursor.selectionStart();
        const int anchor = cursor.selectionEnd();
        const BOBUIextBlock posBlock = d->doc->findBlock(position);
        const BOBUIextBlock anchorBlock = d->doc->findBlock(anchor);
        if (posBlock == anchorBlock && posBlock.isValid() && posBlock.layout()->lineCount()) {
            const BOBUIextLine posLine = posBlock.layout()->lineForTextPosition(position - posBlock.position());
            const BOBUIextLine anchorLine = anchorBlock.layout()->lineForTextPosition(anchor - anchorBlock.position());

            const int firstLine = qMin(posLine.lineNumber(), anchorLine.lineNumber());
            const int lastLine = qMax(posLine.lineNumber(), anchorLine.lineNumber());
            const BOBUIextLayout *layout = posBlock.layout();
            r = QRectF();
            for (int i = firstLine; i <= lastLine; ++i) {
                r |= layout->lineAt(i).rect();
                r |= layout->lineAt(i).naturalTextRect(); // might be bigger in the case of wrap not enabled
            }
            r.translate(blockBoundingRect(posBlock).topLeft());
        } else {
            QRectF anchorRect = d->rectForPosition(cursor.selectionEnd());
            r |= anchorRect;
            r |= boundingRectOfFloatsInSelection(cursor);
            QRectF frameRect(d->doc->documentLayout()->frameBoundingRect(cursor.currentFrame()));
            r.setLeft(frameRect.left());
            r.setRight(frameRect.right());
        }
        if (r.isValid())
            r.adjust(-1, -1, 1, 1);
    }

    return r;
}

QRectF QWidgetTextControl::selectionRect() const
{
    Q_D(const QWidgetTextControl);
    return selectionRect(d->cursor);
}

void QWidgetTextControlPrivate::mousePressEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos, BobUI::KeyboardModifiers modifiers,
                                          BobUI::MouseButtons buttons, const QPoint &globalPos)
{
    Q_Q(QWidgetTextControl);

    mousePressPos = pos.toPoint();

#if BOBUI_CONFIG(draganddrop)
    mightStartDrag = false;
#endif

    if (sendMouseEventToInputContext(
            e, QEvent::MouseButtonPress, button, pos, modifiers, buttons, globalPos)) {
        return;
    }

    if (interactionFlags & BobUI::LinksAccessibleByMouse) {
        anchorOnMousePress = q->anchorAt(pos);

        if (cursorIsFocusIndicator) {
            cursorIsFocusIndicator = false;
            repaintSelection();
            cursor.clearSelection();
        }
    }
    if (!(button & BobUI::LeftButton) ||
        !((interactionFlags & BobUI::TextSelectableByMouse) || (interactionFlags & BobUI::TextEditable))) {
            e->ignore();
            return;
    }
    bool wasValid = blockWithMarkerUnderMouse.isValid();
    blockWithMarkerUnderMouse = q->blockWithMarkerAt(pos);
    if (wasValid != blockWithMarkerUnderMouse.isValid())
        emit q->blockMarkerHovered(blockWithMarkerUnderMouse);


    cursorIsFocusIndicator = false;
    const BOBUIextCursor oldSelection = cursor;
    const int oldCursorPos = cursor.position();

    mousePressed = (interactionFlags & BobUI::TextSelectableByMouse);

    commitPreedit();

    if (trippleClickTimer.isActive()
        && ((pos - trippleClickPoint).toPoint().manhattanLength() < QApplication::startDragDistance())) {

        cursor.movePosition(BOBUIextCursor::StartOfBlock);
        cursor.movePosition(BOBUIextCursor::EndOfBlock, BOBUIextCursor::KeepAnchor);
        cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
        selectedBlockOnTrippleClick = cursor;

        anchorOnMousePress = QString();
        blockWithMarkerUnderMouse = BOBUIextBlock();
        emit q->blockMarkerHovered(blockWithMarkerUnderMouse);

        trippleClickTimer.stop();
    } else {
        int cursorPos = q->hitTest(pos, BobUI::FuzzyHit);
        if (cursorPos == -1) {
            e->ignore();
            return;
        }

        if (modifiers == BobUI::ShiftModifier && (interactionFlags & BobUI::TextSelectableByMouse)) {
            if (wordSelectionEnabled && !selectedWordOnDoubleClick.hasSelection()) {
                selectedWordOnDoubleClick = cursor;
                selectedWordOnDoubleClick.select(BOBUIextCursor::WordUnderCursor);
            }

            if (selectedBlockOnTrippleClick.hasSelection())
                extendBlockwiseSelection(cursorPos);
            else if (selectedWordOnDoubleClick.hasSelection())
                extendWordwiseSelection(cursorPos, pos.x());
            else if (!wordSelectionEnabled)
                setCursorPosition(cursorPos, BOBUIextCursor::KeepAnchor);
        } else {

            if (dragEnabled
                && cursor.hasSelection()
                && !cursorIsFocusIndicator
                && cursorPos >= cursor.selectionStart()
                && cursorPos <= cursor.selectionEnd()
                && q->hitTest(pos, BobUI::ExactHit) != -1) {
#if BOBUI_CONFIG(draganddrop)
                mightStartDrag = true;
#endif
                return;
            }

            setCursorPosition(cursorPos);
        }
    }

    if (interactionFlags & BobUI::TextEditable) {
        q->ensureCursorVisible();
        if (cursor.position() != oldCursorPos)
            emit q->cursorPositionChanged();
        _q_updateCurrentCharFormatAndSelection();
    } else {
        if (cursor.position() != oldCursorPos) {
            emit q->cursorPositionChanged();
            emit q->microFocusChanged();
        }
        selectionChanged();
    }
    repaintOldAndNewSelection(oldSelection);
    hadSelectionOnMousePress = cursor.hasSelection();
}

void QWidgetTextControlPrivate::mouseMoveEvent(QEvent *e, BobUI::MouseButton button, const QPointF &mousePos, BobUI::KeyboardModifiers modifiers,
                                         BobUI::MouseButtons buttons, const QPoint &globalPos)
{
    Q_Q(QWidgetTextControl);

    if (interactionFlags & BobUI::LinksAccessibleByMouse) {
        QString anchor = q->anchorAt(mousePos);
        if (anchor != highlightedAnchor) {
            highlightedAnchor = anchor;
            emit q->linkHovered(anchor);
        }
    }

    if (buttons & BobUI::LeftButton) {
        const bool editable = interactionFlags & BobUI::TextEditable;

        if (!(mousePressed
              || editable
              || mightStartDrag
              || selectedWordOnDoubleClick.hasSelection()
              || selectedBlockOnTrippleClick.hasSelection()))
            return;

        const BOBUIextCursor oldSelection = cursor;
        const int oldCursorPos = cursor.position();

        if (mightStartDrag) {
            if ((mousePos.toPoint() - mousePressPos).manhattanLength() > QApplication::startDragDistance())
                startDrag();
            return;
        }

        const qreal mouseX = qreal(mousePos.x());

        int newCursorPos = q->hitTest(mousePos, BobUI::FuzzyHit);

        if (isPreediting()) {
            // note: oldCursorPos not including preedit
            int selectionStartPos = q->hitTest(mousePressPos, BobUI::FuzzyHit);

            if (newCursorPos != selectionStartPos) {
                commitPreedit();
                // commit invalidates positions
                newCursorPos = q->hitTest(mousePos, BobUI::FuzzyHit);
                selectionStartPos = q->hitTest(mousePressPos, BobUI::FuzzyHit);
                setCursorPosition(selectionStartPos);
            }
        }

        if (newCursorPos == -1)
            return;

        if (mousePressed && wordSelectionEnabled && !selectedWordOnDoubleClick.hasSelection()) {
            selectedWordOnDoubleClick = cursor;
            selectedWordOnDoubleClick.select(BOBUIextCursor::WordUnderCursor);
        }

        if (selectedBlockOnTrippleClick.hasSelection())
            extendBlockwiseSelection(newCursorPos);
        else if (selectedWordOnDoubleClick.hasSelection())
            extendWordwiseSelection(newCursorPos, mouseX);
        else if (mousePressed && !isPreediting())
            setCursorPosition(newCursorPos, BOBUIextCursor::KeepAnchor);

        if (interactionFlags & BobUI::TextEditable) {
            // don't call ensureVisible for the visible cursor to avoid jumping
            // scrollbars. the autoscrolling ensures smooth scrolling if necessary.
            //q->ensureCursorVisible();
            if (cursor.position() != oldCursorPos)
                emit q->cursorPositionChanged();
            _q_updateCurrentCharFormatAndSelection();
#ifndef BOBUI_NO_IM
            if (contextWidget)
                QGuiApplication::inputMethod()->update(BobUI::ImQueryInput);
#endif //BOBUI_NO_IM
        } else {
            //emit q->visibilityRequest(QRectF(mousePos, QSizeF(1, 1)));
            if (cursor.position() != oldCursorPos) {
                emit q->cursorPositionChanged();
                emit q->microFocusChanged();
            }
        }
        selectionChanged(true);
        repaintOldAndNewSelection(oldSelection);
    } else {
        bool wasValid = blockWithMarkerUnderMouse.isValid();
        blockWithMarkerUnderMouse = q->blockWithMarkerAt(mousePos);
        if (wasValid != blockWithMarkerUnderMouse.isValid())
            emit q->blockMarkerHovered(blockWithMarkerUnderMouse);
    }

    sendMouseEventToInputContext(e, QEvent::MouseMove, button, mousePos, modifiers, buttons, globalPos);
}

void QWidgetTextControlPrivate::mouseReleaseEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos, BobUI::KeyboardModifiers modifiers,
                                            BobUI::MouseButtons buttons, const QPoint &globalPos)
{
    Q_Q(QWidgetTextControl);

    const BOBUIextCursor oldSelection = cursor;
    if (sendMouseEventToInputContext(
            e, QEvent::MouseButtonRelease, button, pos, modifiers, buttons, globalPos)) {
        repaintOldAndNewSelection(oldSelection);
        return;
    }

    const int oldCursorPos = cursor.position();

#if BOBUI_CONFIG(draganddrop)
    if (mightStartDrag && (button & BobUI::LeftButton)) {
        mousePressed = false;
        setCursorPosition(pos);
        cursor.clearSelection();
        selectionChanged();
    }
#endif
    if (mousePressed) {
        mousePressed = false;
#ifndef BOBUI_NO_CLIPBOARD
        setClipboardSelection();
        selectionChanged(true);
    } else if (button == BobUI::MiddleButton
               && (interactionFlags & BobUI::TextEditable)
               && QGuiApplication::clipboard()->supportsSelection()) {
        setCursorPosition(pos);
        const QMimeData *md = QGuiApplication::clipboard()->mimeData(QClipboard::Selection);
        if (md)
            q->insertFromMimeData(md);
#endif
    }

    repaintOldAndNewSelection(oldSelection);

    if (cursor.position() != oldCursorPos) {
        emit q->cursorPositionChanged();
        emit q->microFocusChanged();
    }

    // toggle any checkbox that the user clicks
    if ((interactionFlags & BobUI::TextEditable) && (button & BobUI::LeftButton) &&
            (blockWithMarkerUnderMouse.isValid()) && !cursor.hasSelection()) {
        BOBUIextBlock markerBlock = q->blockWithMarkerAt(pos);
        if (markerBlock == blockWithMarkerUnderMouse) {
            auto fmt = blockWithMarkerUnderMouse.blockFormat();
            switch (fmt.marker()) {
            case BOBUIextBlockFormat::MarkerType::Unchecked :
                fmt.setMarker(BOBUIextBlockFormat::MarkerType::Checked);
                break;
            case BOBUIextBlockFormat::MarkerType::Checked:
                fmt.setMarker(BOBUIextBlockFormat::MarkerType::Unchecked);
                break;
            default:
                break;
            }
            cursor.setBlockFormat(fmt);
        }
    }

    if (interactionFlags & BobUI::LinksAccessibleByMouse) {

        // Ignore event unless left button has been pressed
        if (!(button & BobUI::LeftButton)) {
            e->ignore();
            return;
        }

        const QString anchor = q->anchorAt(pos);

        // Ignore event without selection anchor
        if (anchor.isEmpty()) {
            e->ignore();
            return;
        }

        if (!cursor.hasSelection()
            || (anchor == anchorOnMousePress && hadSelectionOnMousePress)) {

            const int anchorPos = q->hitTest(pos, BobUI::ExactHit);

            // Ignore event without valid anchor position
            if (anchorPos < 0) {
                e->ignore();
                return;
            }

            cursor.setPosition(anchorPos);
            QString anchor = anchorOnMousePress;
            anchorOnMousePress = QString();
            activateLinkUnderCursor(anchor);
        }
    }
}

void QWidgetTextControlPrivate::mouseDoubleClickEvent(QEvent *e, BobUI::MouseButton button, const QPointF &pos,
                                                      BobUI::KeyboardModifiers modifiers, BobUI::MouseButtons buttons,
                                                      const QPoint &globalPos)
{
    Q_Q(QWidgetTextControl);

    if (button == BobUI::LeftButton
        && (interactionFlags & BobUI::TextSelectableByMouse)) {

#if BOBUI_CONFIG(draganddrop)
        mightStartDrag = false;
#endif
        commitPreedit();

        const BOBUIextCursor oldSelection = cursor;
        setCursorPosition(pos);
        BOBUIextLine line = currentTextLine(cursor);
        bool doEmit = false;
        if (line.isValid() && line.textLength()) {
            cursor.select(BOBUIextCursor::WordUnderCursor);
            doEmit = true;
        }
        repaintOldAndNewSelection(oldSelection);

        cursorIsFocusIndicator = false;
        selectedWordOnDoubleClick = cursor;

        trippleClickPoint = pos;
        trippleClickTimer.start(QApplication::doubleClickInterval(), q);
        if (doEmit) {
            selectionChanged();
#ifndef BOBUI_NO_CLIPBOARD
            setClipboardSelection();
#endif
            emit q->cursorPositionChanged();
        }
    } else if (!sendMouseEventToInputContext(e, QEvent::MouseButtonDblClick, button, pos,
                                             modifiers, buttons, globalPos)) {
        e->ignore();
    }
}

bool QWidgetTextControlPrivate::sendMouseEventToInputContext(
        QEvent *e, QEvent::Type eventType, BobUI::MouseButton button, const QPointF &pos,
        BobUI::KeyboardModifiers modifiers, BobUI::MouseButtons buttons, const QPoint &globalPos)
{
    Q_UNUSED(eventType);
    Q_UNUSED(button);
    Q_UNUSED(pos);
    Q_UNUSED(modifiers);
    Q_UNUSED(buttons);
    Q_UNUSED(globalPos);
#if !defined(BOBUI_NO_IM)
    Q_Q(QWidgetTextControl);

    if (isPreediting()) {
        BOBUIextLayout *layout = cursor.block().layout();
        int cursorPos = q->hitTest(pos, BobUI::FuzzyHit) - cursor.position();

        if (cursorPos < 0 || cursorPos > layout->preeditAreaText().size())
            cursorPos = -1;

        if (cursorPos >= 0) {
            if (eventType == QEvent::MouseButtonRelease)
                QGuiApplication::inputMethod()->invokeAction(QInputMethod::Click, cursorPos);

            e->setAccepted(true);
            return true;
        }
    }
#else
    Q_UNUSED(e);
#endif
    return false;
}

void QWidgetTextControlPrivate::contextMenuEvent(const QPoint &screenPos, const QPointF &docPos, QWidget *contextWidget)
{
#ifdef BOBUI_NO_CONTEXTMENU
    Q_UNUSED(screenPos);
    Q_UNUSED(docPos);
    Q_UNUSED(contextWidget);
#else
    Q_Q(QWidgetTextControl);
    QMenu *menu = q->createStandardContextMenu(docPos, contextWidget);
    if (!menu)
        return;
    menu->setAttribute(BobUI::WA_DeleteOnClose);

    if (auto *widget = qobject_cast<QWidget *>(parent)) {
        if (auto *window = widget->window()->windowHandle())
            QMenuPrivate::get(menu)->topData()->initialScreen = window->screen();
    }

    menu->popup(screenPos);
#endif
}

bool QWidgetTextControlPrivate::dragEnterEvent(QEvent *e, const QMimeData *mimeData)
{
    Q_Q(QWidgetTextControl);
    if (!(interactionFlags & BobUI::TextEditable) || !q->canInsertFromMimeData(mimeData)) {
        e->ignore();
        return false;
    }

    dndFeedbackCursor = BOBUIextCursor();

    return true; // accept proposed action
}

void QWidgetTextControlPrivate::dragLeaveEvent()
{
    Q_Q(QWidgetTextControl);

    const QRectF crect = q->cursorRect(dndFeedbackCursor);
    dndFeedbackCursor = BOBUIextCursor();

    if (crect.isValid())
        emit q->updateRequest(crect);
}

bool QWidgetTextControlPrivate::dragMoveEvent(QEvent *e, const QMimeData *mimeData, const QPointF &pos)
{
    Q_Q(QWidgetTextControl);
    if (!(interactionFlags & BobUI::TextEditable) || !q->canInsertFromMimeData(mimeData)) {
        e->ignore();
        return false;
    }

    const int cursorPos = q->hitTest(pos, BobUI::FuzzyHit);
    if (cursorPos != -1) {
        QRectF crect = q->cursorRect(dndFeedbackCursor);
        if (crect.isValid())
            emit q->updateRequest(crect);

        dndFeedbackCursor = cursor;
        dndFeedbackCursor.setPosition(cursorPos);

        crect = q->cursorRect(dndFeedbackCursor);
        emit q->updateRequest(crect);
    }

    return true; // accept proposed action
}

bool QWidgetTextControlPrivate::dropEvent(const QMimeData *mimeData, const QPointF &pos, BobUI::DropAction dropAction, QObject *source)
{
    Q_Q(QWidgetTextControl);
    dndFeedbackCursor = BOBUIextCursor();

    if (!(interactionFlags & BobUI::TextEditable) || !q->canInsertFromMimeData(mimeData))
        return false;

    repaintSelection();

    BOBUIextCursor insertionCursor = q->cursorForPosition(pos);
    insertionCursor.beginEditBlock();

    if (dropAction == BobUI::MoveAction && source == contextWidget)
        cursor.removeSelectedText();

    cursor = insertionCursor;
    q->insertFromMimeData(mimeData);
    insertionCursor.endEditBlock();
    q->ensureCursorVisible();
    return true; // accept proposed action
}

void QWidgetTextControlPrivate::inputMethodEvent(QInputMethodEvent *e)
{
    Q_Q(QWidgetTextControl);
    if (!(interactionFlags & (BobUI::TextEditable | BobUI::TextSelectableByMouse)) || cursor.isNull()) {
        e->ignore();
        return;
    }
    bool isGettingInput = !e->commitString().isEmpty()
            || e->preeditString() != cursor.block().layout()->preeditAreaText()
            || e->replacementLength() > 0;

    if (!isGettingInput && e->attributes().isEmpty()) {
        e->ignore();
        return;
    }

    int oldCursorPos = cursor.position();

    cursor.beginEditBlock();
    if (isGettingInput) {
        cursor.removeSelectedText();
    }

    BOBUIextBlock block;

    // insert commit string
    if (!e->commitString().isEmpty() || e->replacementLength()) {
        auto *mimeData = QInputControl::mimeDataForInputEvent(e);
        if (mimeData && q->canInsertFromMimeData(mimeData)) {
            q->insertFromMimeData(mimeData);
        } else {
            if (e->commitString().endsWith(QChar::LineFeed))
                block = cursor.block(); // Remember the block where the preedit text is
            BOBUIextCursor c = cursor;
            c.setPosition(c.position() + e->replacementStart());
            c.setPosition(c.position() + e->replacementLength(), BOBUIextCursor::KeepAnchor);
            c.insertText(e->commitString());
        }
    }

    for (int i = 0; i < e->attributes().size(); ++i) {
        const QInputMethodEvent::Attribute &a = e->attributes().at(i);
        if (a.type == QInputMethodEvent::Selection) {
            BOBUIextCursor oldCursor = cursor;
            int blockStart = a.start + cursor.block().position();
            cursor.setPosition(blockStart, BOBUIextCursor::MoveAnchor);
            cursor.setPosition(blockStart + a.length, BOBUIextCursor::KeepAnchor);
            q->ensureCursorVisible();
            repaintOldAndNewSelection(oldCursor);
        }
    }

    if (!block.isValid())
        block = cursor.block();
    BOBUIextLayout *layout = block.layout();
    if (isGettingInput)
        layout->setPreeditArea(cursor.position() - block.position(), e->preeditString());
    QList<BOBUIextLayout::FormatRange> overrides;
    overrides.reserve(e->attributes().size());
    const int oldPreeditCursor = preeditCursor;
    preeditCursor = e->preeditString().size();
    hideCursor = false;
    for (int i = 0; i < e->attributes().size(); ++i) {
        const QInputMethodEvent::Attribute &a = e->attributes().at(i);
        if (a.type == QInputMethodEvent::Cursor) {
            preeditCursor = a.start;
            hideCursor = !a.length;
        } else if (a.type == QInputMethodEvent::TextFormat) {
            BOBUIextCharFormat f = cursor.charFormat();
            f.merge(qvariant_cast<BOBUIextFormat>(a.value).toCharFormat());
            if (f.isValid()) {
                BOBUIextLayout::FormatRange o;
                o.start = a.start + cursor.position() - block.position();
                o.length = a.length;
                o.format = f;

                // Make sure list is sorted by start index
                QList<BOBUIextLayout::FormatRange>::iterator it = overrides.end();
                while (it != overrides.begin()) {
                    QList<BOBUIextLayout::FormatRange>::iterator previous = it - 1;
                    if (o.start >= previous->start) {
                        overrides.insert(it, o);
                        break;
                    }
                    it = previous;
                }

                if (it == overrides.begin())
                    overrides.prepend(o);
            }
        }
    }

    if (cursor.charFormat().isValid()) {
        int start = cursor.position() - block.position();
        int end = start + e->preeditString().size();

        QList<BOBUIextLayout::FormatRange>::iterator it = overrides.begin();
        while (it != overrides.end()) {
            BOBUIextLayout::FormatRange range = *it;
            int rangeStart = range.start;
            if (rangeStart > start) {
                BOBUIextLayout::FormatRange o;
                o.start = start;
                o.length = rangeStart - start;
                o.format = cursor.charFormat();
                it = overrides.insert(it, o) + 1;
            }

            ++it;
            start = range.start + range.length;
        }

        if (start < end) {
            BOBUIextLayout::FormatRange o;
            o.start = start;
            o.length = end - start;
            o.format = cursor.charFormat();
            overrides.append(o);
        }
    }
    layout->setFormats(overrides);

    cursor.endEditBlock();

    if (cursor.d)
        cursor.d->setX();
    if (oldCursorPos != cursor.position())
        emit q->cursorPositionChanged();
    if (oldPreeditCursor != preeditCursor)
        emit q->microFocusChanged();
}

QVariant QWidgetTextControl::inputMethodQuery(BobUI::InputMethodQuery property, QVariant argument) const
{
    Q_D(const QWidgetTextControl);
    BOBUIextBlock block = d->cursor.block();
    switch(property) {
    case BobUI::ImCursorRectangle:
        return cursorRect();
    case BobUI::ImAnchorRectangle:
        return d->rectForPosition(d->cursor.anchor());
    case BobUI::ImFont:
        return QVariant(d->cursor.charFormat().font());
    case BobUI::ImCursorPosition: {
        const QPointF pt = argument.toPointF();
        if (!pt.isNull())
            return QVariant(cursorForPosition(pt).position() - block.position());
        return QVariant(d->cursor.position() - block.position()); }
    case BobUI::ImSurroundingText:
        return QVariant(block.text());
    case BobUI::ImCurrentSelection: {
        QMimeData *mimeData = createMimeDataFromSelection();
        mimeData->deleteLater();
        return QInputControl::selectionWrapper(mimeData);
    }
    case BobUI::ImMaximumTextLength:
        return QVariant(); // No limit.
    case BobUI::ImAnchorPosition:
        return QVariant(d->cursor.anchor() - block.position());
    case BobUI::ImAbsolutePosition: {
        const QPointF pt = argument.toPointF();
        if (!pt.isNull())
            return QVariant(cursorForPosition(pt).position());
        return QVariant(d->cursor.position()); }
    case BobUI::ImTextAfterCursor:
    {
        int maxLength = argument.isValid() ? argument.toInt() : 1024;
        BOBUIextCursor tmpCursor = d->cursor;
        int localPos = d->cursor.position() - block.position();
        QString result = block.text().mid(localPos);
        while (result.size() < maxLength) {
            int currentBlock = tmpCursor.blockNumber();
            tmpCursor.movePosition(BOBUIextCursor::NextBlock);
            if (tmpCursor.blockNumber() == currentBlock)
                break;
            result += u'\n' + tmpCursor.block().text();
        }
        return QVariant(result);
    }
    case BobUI::ImTextBeforeCursor:
    {
        int maxLength = argument.isValid() ? argument.toInt() : 1024;
        BOBUIextCursor tmpCursor = d->cursor;
        int localPos = d->cursor.position() - block.position();
        int numBlocks = 0;
        int resultLen = localPos;
        while (resultLen < maxLength) {
            int currentBlock = tmpCursor.blockNumber();
            tmpCursor.movePosition(BOBUIextCursor::PreviousBlock);
            if (tmpCursor.blockNumber() == currentBlock)
                break;
            numBlocks++;
            resultLen += tmpCursor.block().length();
        }
        QString result;
        while (numBlocks) {
            result += tmpCursor.block().text() + u'\n';
            tmpCursor.movePosition(BOBUIextCursor::NextBlock);
            --numBlocks;
        }
        result += QStringView{block.text()}.mid(0, localPos);
        return QVariant(result);
    }
    default:
        return QVariant();
    }
}

void QWidgetTextControl::setFocus(bool focus, BobUI::FocusReason reason)
{
    QFocusEvent ev(focus ? QEvent::FocusIn : QEvent::FocusOut,
                   reason);
    processEvent(&ev);
}

void QWidgetTextControlPrivate::focusEvent(QFocusEvent *e)
{
    Q_Q(QWidgetTextControl);
    emit q->updateRequest(q->selectionRect());
    if (e->gotFocus()) {
#ifdef BOBUI_KEYPAD_NAVIGATION
        if (!QApplicationPrivate::keypadNavigationEnabled() || (hasEditFocus && (e->reason() == BobUI::PopupFocusReason))) {
#endif
        cursorOn = (interactionFlags & (BobUI::TextSelectableByKeyboard | BobUI::TextEditable));
        if (interactionFlags & BobUI::TextEditable) {
            setCursorVisible(true);
        }
#ifdef BOBUI_KEYPAD_NAVIGATION
        }
#endif
    } else {
        setCursorVisible(false);
        cursorOn = false;

        if (cursorIsFocusIndicator
            && e->reason() != BobUI::ActiveWindowFocusReason
            && e->reason() != BobUI::PopupFocusReason
            && cursor.hasSelection()) {
            cursor.clearSelection();
        }
    }
    hasFocus = e->gotFocus();
}

QString QWidgetTextControlPrivate::anchorForCursor(const BOBUIextCursor &anchorCursor) const
{
    if (anchorCursor.hasSelection()) {
        BOBUIextCursor cursor = anchorCursor;
        if (cursor.selectionStart() != cursor.position())
            cursor.setPosition(cursor.selectionStart());
        cursor.movePosition(BOBUIextCursor::NextCharacter);
        BOBUIextCharFormat fmt = cursor.charFormat();
        if (fmt.isAnchor() && fmt.hasProperty(BOBUIextFormat::AnchorHref))
            return fmt.stringProperty(BOBUIextFormat::AnchorHref);
    }
    return QString();
}

#ifdef BOBUI_KEYPAD_NAVIGATION
void QWidgetTextControlPrivate::editFocusEvent(QEvent *e)
{
    Q_Q(QWidgetTextControl);

    if (QApplicationPrivate::keypadNavigationEnabled()) {
        if (e->type() == QEvent::EnterEditFocus && interactionFlags & BobUI::TextEditable) {
            const BOBUIextCursor oldSelection = cursor;
            const int oldCursorPos = cursor.position();
            const bool moved = cursor.movePosition(BOBUIextCursor::End, BOBUIextCursor::MoveAnchor);
            q->ensureCursorVisible();
            if (moved) {
                if (cursor.position() != oldCursorPos)
                    emit q->cursorPositionChanged();
                emit q->microFocusChanged();
            }
            selectionChanged();
            repaintOldAndNewSelection(oldSelection);

            setBlinkingCursorEnabled(true);
        } else
            setBlinkingCursorEnabled(false);
    }

    hasEditFocus = e->type() == QEvent::EnterEditFocus;
}
#endif

#ifndef BOBUI_NO_CONTEXTMENU
void setActionIcon(QAction *action, const QString &name)
{
    const QIcon icon = QIcon::fromTheme(name);
    if (!icon.isNull())
        action->setIcon(icon);
}

QMenu *QWidgetTextControl::createStandardContextMenu(const QPointF &pos, QWidget *parent)
{
    Q_D(QWidgetTextControl);

    const bool showTextSelectionActions = d->interactionFlags & (BobUI::TextEditable | BobUI::TextSelectableByKeyboard | BobUI::TextSelectableByMouse);

    d->linkToCopy = QString();
    if (!pos.isNull())
        d->linkToCopy = anchorAt(pos);

    if (d->linkToCopy.isEmpty() && !showTextSelectionActions)
        return nullptr;

    QMenu *menu = new QMenu(parent);
    QAction *a;

    if (d->interactionFlags & BobUI::TextEditable) {
        a = menu->addAction(tr("&Undo") + ACCEL_KEY(QKeySequence::Undo), this, SLOT(undo()));
        a->setEnabled(d->doc->isUndoAvailable());
        a->setObjectName(QStringLiteral("edit-undo"));
        setActionIcon(a, QStringLiteral("edit-undo"));
        a = menu->addAction(tr("&Redo") + ACCEL_KEY(QKeySequence::Redo), this, SLOT(redo()));
        a->setEnabled(d->doc->isRedoAvailable());
        a->setObjectName(QStringLiteral("edit-redo"));
        setActionIcon(a, QStringLiteral("edit-redo"));
        menu->addSeparator();

#ifndef BOBUI_NO_CLIPBOARD
        a = menu->addAction(tr("Cu&t") + ACCEL_KEY(QKeySequence::Cut), this, SLOT(cut()));
        a->setEnabled(d->cursor.hasSelection());
        a->setObjectName(QStringLiteral("edit-cut"));
        setActionIcon(a, QStringLiteral("edit-cut"));
#endif
    }

#ifndef BOBUI_NO_CLIPBOARD
    if (showTextSelectionActions) {
        a = menu->addAction(tr("&Copy") + ACCEL_KEY(QKeySequence::Copy), this, SLOT(copy()));
        a->setEnabled(d->cursor.hasSelection());
        a->setObjectName(QStringLiteral("edit-copy"));
        setActionIcon(a, QStringLiteral("edit-copy"));
    }

    if ((d->interactionFlags & BobUI::LinksAccessibleByKeyboard)
            || (d->interactionFlags & BobUI::LinksAccessibleByMouse)) {

        a = menu->addAction(tr("Copy &Link Location"), this, SLOT(_q_copyLink()));
        a->setEnabled(!d->linkToCopy.isEmpty());
        a->setObjectName(QStringLiteral("link-copy"));
    }
#endif // BOBUI_NO_CLIPBOARD

    if (d->interactionFlags & BobUI::TextEditable) {
#ifndef BOBUI_NO_CLIPBOARD
        a = menu->addAction(tr("&Paste") + ACCEL_KEY(QKeySequence::Paste), this, SLOT(paste()));
        a->setEnabled(canPaste());
        a->setObjectName(QStringLiteral("edit-paste"));
        setActionIcon(a, QStringLiteral("edit-paste"));
#endif
        a = menu->addAction(tr("Delete"), this, SLOT(_q_deleteSelected()));
        a->setEnabled(d->cursor.hasSelection());
        a->setObjectName(QStringLiteral("edit-delete"));
        setActionIcon(a, QStringLiteral("edit-delete"));
    }


    if (showTextSelectionActions) {
        menu->addSeparator();
        a = menu->addAction(tr("Select All") + ACCEL_KEY(QKeySequence::SelectAll), this, SLOT(selectAll()));
        a->setEnabled(!d->doc->isEmpty());
        a->setObjectName(QStringLiteral("select-all"));
        setActionIcon(a, QStringLiteral("edit-select-all"));
    }

    if ((d->interactionFlags & BobUI::TextEditable) && QGuiApplication::styleHints()->useRtlExtensions()) {
        menu->addSeparator();
        QUnicodeControlCharacterMenu *ctrlCharacterMenu = new QUnicodeControlCharacterMenu(this, menu);
        menu->addMenu(ctrlCharacterMenu);
    }

    return menu;
}
#endif // BOBUI_NO_CONTEXTMENU

BOBUIextCursor QWidgetTextControl::cursorForPosition(const QPointF &pos) const
{
    Q_D(const QWidgetTextControl);
    int cursorPos = hitTest(pos, BobUI::FuzzyHit);
    if (cursorPos == -1)
        cursorPos = 0;
    BOBUIextCursor c(d->doc);
    c.setPosition(cursorPos);
    return c;
}

QRectF QWidgetTextControl::cursorRect(const BOBUIextCursor &cursor) const
{
    Q_D(const QWidgetTextControl);
    if (cursor.isNull())
        return QRectF();

    return d->rectForPosition(cursor.position());
}

QRectF QWidgetTextControl::cursorRect() const
{
    Q_D(const QWidgetTextControl);
    return cursorRect(d->cursor);
}

QRectF QWidgetTextControlPrivate::cursorRectPlusUnicodeDirectionMarkers(const BOBUIextCursor &cursor) const
{
    if (cursor.isNull())
        return QRectF();

    return rectForPosition(cursor.position()).adjusted(-4, 0, 4, 0);
}

QString QWidgetTextControl::anchorAt(const QPointF &pos) const
{
    Q_D(const QWidgetTextControl);
    return d->doc->documentLayout()->anchorAt(pos);
}

QString QWidgetTextControl::anchorAtCursor() const
{
    Q_D(const QWidgetTextControl);

    return d->anchorForCursor(d->cursor);
}

BOBUIextBlock QWidgetTextControl::blockWithMarkerAt(const QPointF &pos) const
{
    Q_D(const QWidgetTextControl);
    return d->doc->documentLayout()->blockWithMarkerAt(pos);
}

bool QWidgetTextControl::overwriteMode() const
{
    Q_D(const QWidgetTextControl);
    return d->overwriteMode;
}

void QWidgetTextControl::setOverwriteMode(bool overwrite)
{
    Q_D(QWidgetTextControl);
    d->overwriteMode = overwrite;
}

int QWidgetTextControl::cursorWidth() const
{
    Q_D(const QWidgetTextControl);
    return d->doc->documentLayout()->property("cursorWidth").toInt();
}

void QWidgetTextControl::setCursorWidth(int width)
{
    Q_D(QWidgetTextControl);
    if (width == -1)
        width = QApplication::style()->pixelMetric(QStyle::PM_TextCursorWidth, nullptr, qobject_cast<QWidget *>(parent()));
    d->doc->documentLayout()->setProperty("cursorWidth", width);
    d->repaintCursor();
}

bool QWidgetTextControl::acceptRichText() const
{
    Q_D(const QWidgetTextControl);
    return d->acceptRichText;
}

void QWidgetTextControl::setAcceptRichText(bool accept)
{
    Q_D(QWidgetTextControl);
    d->acceptRichText = accept;
}

#if BOBUI_CONFIG(textedit)

void QWidgetTextControl::setExtraSelections(const QList<BOBUIextEdit::ExtraSelection> &selections)
{
    Q_D(QWidgetTextControl);

    QMultiHash<int, int> hash;
    for (int i = 0; i < d->extraSelections.size(); ++i) {
        const QAbstractTextDocumentLayout::Selection &esel = d->extraSelections.at(i);
        hash.insert(esel.cursor.anchor(), i);
    }

    for (int i = 0; i < selections.size(); ++i) {
        const BOBUIextEdit::ExtraSelection &sel = selections.at(i);
        const auto it = hash.constFind(sel.cursor.anchor());
        if (it != hash.cend()) {
            const QAbstractTextDocumentLayout::Selection &esel = d->extraSelections.at(it.value());
            if (esel.cursor.position() == sel.cursor.position()
                && esel.format == sel.format) {
                hash.erase(it);
                continue;
            }
        }
        QRectF r = selectionRect(sel.cursor);
        if (sel.format.boolProperty(BOBUIextFormat::FullWidthSelection)) {
            r.setLeft(0);
            r.setWidth(qreal(INT_MAX));
        }
        emit updateRequest(r);
    }

    for (auto it = hash.cbegin(); it != hash.cend(); ++it) {
        const QAbstractTextDocumentLayout::Selection &esel = d->extraSelections.at(it.value());
        QRectF r = selectionRect(esel.cursor);
        if (esel.format.boolProperty(BOBUIextFormat::FullWidthSelection)) {
            r.setLeft(0);
            r.setWidth(qreal(INT_MAX));
        }
        emit updateRequest(r);
    }

    d->extraSelections.resize(selections.size());
    for (int i = 0; i < selections.size(); ++i) {
        d->extraSelections[i].cursor = selections.at(i).cursor;
        d->extraSelections[i].format = selections.at(i).format;
    }
}

QList<BOBUIextEdit::ExtraSelection> QWidgetTextControl::extraSelections() const
{
    Q_D(const QWidgetTextControl);
    QList<BOBUIextEdit::ExtraSelection> selections;
    const int numExtraSelections = d->extraSelections.size();
    selections.reserve(numExtraSelections);
    for (int i = 0; i < numExtraSelections; ++i) {
        BOBUIextEdit::ExtraSelection sel;
        const QAbstractTextDocumentLayout::Selection &sel2 = d->extraSelections.at(i);
        sel.cursor = sel2.cursor;
        sel.format = sel2.format;
        selections.append(sel);
    }
    return selections;
}

#endif // BOBUI_CONFIG(textedit)

void QWidgetTextControl::setTextWidth(qreal width)
{
    Q_D(QWidgetTextControl);
    d->doc->setTextWidth(width);
}

qreal QWidgetTextControl::textWidth() const
{
    Q_D(const QWidgetTextControl);
    return d->doc->textWidth();
}

QSizeF QWidgetTextControl::size() const
{
    Q_D(const QWidgetTextControl);
    return d->doc->size();
}

void QWidgetTextControl::setOpenExternalLinks(bool open)
{
    Q_D(QWidgetTextControl);
    d->openExternalLinks = open;
}

bool QWidgetTextControl::openExternalLinks() const
{
    Q_D(const QWidgetTextControl);
    return d->openExternalLinks;
}

bool QWidgetTextControl::ignoreUnusedNavigationEvents() const
{
    Q_D(const QWidgetTextControl);
    return d->ignoreUnusedNavigationEvents;
}

void QWidgetTextControl::setIgnoreUnusedNavigationEvents(bool ignore)
{
    Q_D(QWidgetTextControl);
    d->ignoreUnusedNavigationEvents = ignore;
}

void QWidgetTextControl::moveCursor(BOBUIextCursor::MoveOperation op, BOBUIextCursor::MoveMode mode)
{
    Q_D(QWidgetTextControl);
    const BOBUIextCursor oldSelection = d->cursor;
    const bool moved = d->cursor.movePosition(op, mode);
    d->_q_updateCurrentCharFormatAndSelection();
    ensureCursorVisible();
    d->repaintOldAndNewSelection(oldSelection);
    if (moved)
        emit cursorPositionChanged();
}

bool QWidgetTextControl::canPaste() const
{
#ifndef BOBUI_NO_CLIPBOARD
    Q_D(const QWidgetTextControl);
    if (d->interactionFlags & BobUI::TextEditable) {
        const QMimeData *md = QGuiApplication::clipboard()->mimeData();
        return md && canInsertFromMimeData(md);
    }
#endif
    return false;
}

void QWidgetTextControl::setCursorIsFocusIndicator(bool b)
{
    Q_D(QWidgetTextControl);
    d->cursorIsFocusIndicator = b;
    d->repaintCursor();
}

bool QWidgetTextControl::cursorIsFocusIndicator() const
{
    Q_D(const QWidgetTextControl);
    return d->cursorIsFocusIndicator;
}


void QWidgetTextControl::setDragEnabled(bool enabled)
{
    Q_D(QWidgetTextControl);
    d->dragEnabled = enabled;
}

bool QWidgetTextControl::isDragEnabled() const
{
    Q_D(const QWidgetTextControl);
    return d->dragEnabled;
}

void QWidgetTextControl::setWordSelectionEnabled(bool enabled)
{
    Q_D(QWidgetTextControl);
    d->wordSelectionEnabled = enabled;
}

bool QWidgetTextControl::isWordSelectionEnabled() const
{
    Q_D(const QWidgetTextControl);
    return d->wordSelectionEnabled;
}

bool QWidgetTextControl::isPreediting()
{
    return d_func()->isPreediting();
}

#ifndef BOBUI_NO_PRINTER
void QWidgetTextControl::print(QPagedPaintDevice *printer) const
{
    Q_D(const QWidgetTextControl);
    if (!printer)
        return;
    BOBUIextDocument *tempDoc = nullptr;
    const BOBUIextDocument *doc = d->doc;
    if (QPagedPaintDevicePrivate::get(printer)->printSelectionOnly) {
        if (!d->cursor.hasSelection())
            return;
        tempDoc = new BOBUIextDocument(const_cast<BOBUIextDocument *>(doc));
        tempDoc->setResourceProvider(doc->resourceProvider());
        tempDoc->setMetaInformation(BOBUIextDocument::DocumentTitle, doc->metaInformation(BOBUIextDocument::DocumentTitle));
        tempDoc->setPageSize(doc->pageSize());
        tempDoc->setDefaultFont(doc->defaultFont());
        tempDoc->setUseDesignMetrics(doc->useDesignMetrics());
        BOBUIextCursor(tempDoc).insertFragment(d->cursor.selection());
        doc = tempDoc;

        // copy the custom object handlers
        doc->documentLayout()->d_func()->handlers = d->doc->documentLayout()->d_func()->handlers;
    }
    doc->print(printer);
    delete tempDoc;
}
#endif

QMimeData *QWidgetTextControl::createMimeDataFromSelection() const
{
    Q_D(const QWidgetTextControl);
    const BOBUIextDocumentFragment fragment(d->cursor);
    return new BOBUIextEditMimeData(fragment);
}

bool QWidgetTextControl::canInsertFromMimeData(const QMimeData *source) const
{
    Q_D(const QWidgetTextControl);
    if (d->acceptRichText)
        return (source->hasText() && !source->text().isEmpty())
            || source->hasHtml()
            || source->hasFormat("application/x-qrichtext"_L1)
            || source->hasFormat("application/x-bobui-richtext"_L1);
    else
        return source->hasText() && !source->text().isEmpty();
}

void QWidgetTextControl::insertFromMimeData(const QMimeData *source)
{
    Q_D(QWidgetTextControl);
    if (!(d->interactionFlags & BobUI::TextEditable) || !source)
        return;

    bool hasData = false;
    BOBUIextDocumentFragment fragment;
#if BOBUI_CONFIG(textmarkdownreader)
    const auto formats = source->formats();
    if (formats.size() && formats.first() == "text/markdown"_L1) {
        auto s = QString::fromUtf8(source->data("text/markdown"_L1));
        fragment = BOBUIextDocumentFragment::fromMarkdown(s);
        hasData = true;
    } else
#endif
#ifndef BOBUI_NO_TEXTHTMLPARSER
    if (source->hasFormat("application/x-qrichtext"_L1) && d->acceptRichText) {
        // x-qrichtext is always UTF-8 (taken from BobUI3 since we don't use it anymore).
        const QString richtext = "<meta name=\"qrichtext\" content=\"1\" />"_L1
                + QString::fromUtf8(source->data("application/x-qrichtext"_L1));
        fragment = BOBUIextDocumentFragment::fromHtml(richtext, d->doc);
        hasData = true;
    } else if (source->hasHtml() && d->acceptRichText) {
        fragment = BOBUIextDocumentFragment::fromHtml(source->html(), d->doc);
        hasData = true;
    }
#endif // BOBUI_NO_TEXTHTMLPARSER
    if (!hasData) {
        const QString text = source->text();
        if (!text.isNull()) {
            fragment = BOBUIextDocumentFragment::fromPlainText(text);
            hasData = true;
        }
    }

    if (hasData)
        d->cursor.insertFragment(fragment);
    ensureCursorVisible();
}

bool QWidgetTextControl::findNextPrevAnchor(const BOBUIextCursor &startCursor, bool next, BOBUIextCursor &newAnchor)
{
    Q_D(QWidgetTextControl);

    int anchorStart = -1;
    QString anchorHref;
    int anchorEnd = -1;

    if (next) {
        const int startPos = startCursor.selectionEnd();

        BOBUIextBlock block = d->doc->findBlock(startPos);
        BOBUIextBlock::Iterator it = block.begin();

        while (!it.atEnd() && it.fragment().position() < startPos)
            ++it;

        while (block.isValid()) {
            anchorStart = -1;

            // find next anchor
            for (; !it.atEnd(); ++it) {
                const BOBUIextFragment fragment = it.fragment();
                const BOBUIextCharFormat fmt = fragment.charFormat();

                if (fmt.isAnchor() && fmt.hasProperty(BOBUIextFormat::AnchorHref)) {
                    anchorStart = fragment.position();
                    anchorHref = fmt.anchorHref();
                    break;
                }
            }

            if (anchorStart != -1) {
                anchorEnd = -1;

                // find next non-anchor fragment
                for (; !it.atEnd(); ++it) {
                    const BOBUIextFragment fragment = it.fragment();
                    const BOBUIextCharFormat fmt = fragment.charFormat();

                    if (!fmt.isAnchor() || fmt.anchorHref() != anchorHref) {
                        anchorEnd = fragment.position();
                        break;
                    }
                }

                if (anchorEnd == -1)
                    anchorEnd = block.position() + block.length() - 1;

                // make found selection
                break;
            }

            block = block.next();
            it = block.begin();
        }
    } else {
        int startPos = startCursor.selectionStart();
        if (startPos > 0)
            --startPos;

        BOBUIextBlock block = d->doc->findBlock(startPos);
        BOBUIextBlock::Iterator blockStart = block.begin();
        BOBUIextBlock::Iterator it = block.end();

        if (startPos == block.position()) {
            it = block.begin();
        } else {
            do {
                if (it == blockStart) {
                    it = BOBUIextBlock::Iterator();
                    block = BOBUIextBlock();
                } else {
                    --it;
                }
            } while (!it.atEnd() && it.fragment().position() + it.fragment().length() - 1 > startPos);
        }

        while (block.isValid()) {
            anchorStart = -1;

            if (!it.atEnd()) {
                do {
                    const BOBUIextFragment fragment = it.fragment();
                    const BOBUIextCharFormat fmt = fragment.charFormat();

                    if (fmt.isAnchor() && fmt.hasProperty(BOBUIextFormat::AnchorHref)) {
                        anchorStart = fragment.position() + fragment.length();
                        anchorHref = fmt.anchorHref();
                        break;
                    }

                    if (it == blockStart)
                        it = BOBUIextBlock::Iterator();
                    else
                        --it;
                } while (!it.atEnd());
            }

            if (anchorStart != -1 && !it.atEnd()) {
                anchorEnd = -1;

                do {
                    const BOBUIextFragment fragment = it.fragment();
                    const BOBUIextCharFormat fmt = fragment.charFormat();

                    if (!fmt.isAnchor() || fmt.anchorHref() != anchorHref) {
                        anchorEnd = fragment.position() + fragment.length();
                        break;
                    }

                    if (it == blockStart)
                        it = BOBUIextBlock::Iterator();
                    else
                        --it;
                } while (!it.atEnd());

                if (anchorEnd == -1)
                    anchorEnd = qMax(0, block.position());

                break;
            }

            block = block.previous();
            it = block.end();
            if (it != block.begin())
                --it;
            blockStart = block.begin();
        }

    }

    if (anchorStart != -1 && anchorEnd != -1) {
        newAnchor = d->cursor;
        newAnchor.setPosition(anchorStart);
        newAnchor.setPosition(anchorEnd, BOBUIextCursor::KeepAnchor);
        return true;
    }

    return false;
}

void QWidgetTextControlPrivate::activateLinkUnderCursor(QString href)
{
    BOBUIextCursor oldCursor = cursor;

    if (href.isEmpty()) {
        BOBUIextCursor tmp = cursor;
        if (tmp.selectionStart() != tmp.position())
            tmp.setPosition(tmp.selectionStart());
        tmp.movePosition(BOBUIextCursor::NextCharacter);
        href = tmp.charFormat().anchorHref();
    }
    if (href.isEmpty())
        return;

    if (!cursor.hasSelection()) {
        BOBUIextBlock block = cursor.block();
        const int cursorPos = cursor.position();

        BOBUIextBlock::Iterator it = block.begin();
        BOBUIextBlock::Iterator linkFragment;

        for (; !it.atEnd(); ++it) {
            BOBUIextFragment fragment = it.fragment();
            const int fragmentPos = fragment.position();
            if (fragmentPos <= cursorPos &&
                fragmentPos + fragment.length() > cursorPos) {
                linkFragment = it;
                break;
            }
        }

        if (!linkFragment.atEnd()) {
            it = linkFragment;
            cursor.setPosition(it.fragment().position());
            if (it != block.begin()) {
                do {
                    --it;
                    BOBUIextFragment fragment = it.fragment();
                    if (fragment.charFormat().anchorHref() != href)
                        break;
                    cursor.setPosition(fragment.position());
                } while (it != block.begin());
            }

            for (it = linkFragment; !it.atEnd(); ++it) {
                BOBUIextFragment fragment = it.fragment();
                if (fragment.charFormat().anchorHref() != href)
                    break;
                cursor.setPosition(fragment.position() + fragment.length(), BOBUIextCursor::KeepAnchor);
            }
        }
    }

    if (hasFocus) {
        cursorIsFocusIndicator = true;
    } else {
        cursorIsFocusIndicator = false;
        cursor.clearSelection();
    }
    repaintOldAndNewSelection(oldCursor);

#ifndef BOBUI_NO_DESKTOPSERVICES
    if (openExternalLinks)
        QDesktopServices::openUrl(QUrl{href});
    else
#endif
        emit q_func()->linkActivated(href);
}

#if BOBUI_CONFIG(tooltip)
void QWidgetTextControlPrivate::showToolTip(const QPoint &globalPos, const QPointF &pos, QWidget *contextWidget)
{
    const QString toolTip = q_func()->cursorForPosition(pos).charFormat().toolTip();
    if (toolTip.isEmpty())
        return;
    BOBUIoolTip::showText(globalPos, toolTip, contextWidget);
}
#endif // BOBUI_CONFIG(tooltip)

bool QWidgetTextControlPrivate::isPreediting() const
{
    BOBUIextLayout *layout = cursor.block().layout();
    if (layout && !layout->preeditAreaText().isEmpty())
        return true;

    return false;
}

void QWidgetTextControlPrivate::commitPreedit()
{
    if (!isPreediting())
        return;

    QGuiApplication::inputMethod()->commit();

    if (!isPreediting())
        return;

    cursor.beginEditBlock();
    preeditCursor = 0;
    BOBUIextBlock block = cursor.block();
    BOBUIextLayout *layout = block.layout();
    layout->setPreeditArea(-1, QString());
    layout->clearFormats();
    cursor.endEditBlock();
}

bool QWidgetTextControl::setFocusToNextOrPreviousAnchor(bool next)
{
    Q_D(QWidgetTextControl);

    if (!(d->interactionFlags & BobUI::LinksAccessibleByKeyboard))
        return false;

    QRectF crect = selectionRect();
    emit updateRequest(crect);

    // If we don't have a current anchor, we start from the start/end
    if (!d->cursor.hasSelection()) {
        d->cursor = BOBUIextCursor(d->doc);
        if (next)
            d->cursor.movePosition(BOBUIextCursor::Start);
        else
            d->cursor.movePosition(BOBUIextCursor::End);
    }

    BOBUIextCursor newAnchor;
    if (findNextPrevAnchor(d->cursor, next, newAnchor)) {
        d->cursor = newAnchor;
        d->cursorIsFocusIndicator = true;
    } else {
        d->cursor.clearSelection();
    }

    if (d->cursor.hasSelection()) {
        crect = selectionRect();
        emit updateRequest(crect);
        emit visibilityRequest(crect);
        return true;
    } else {
        return false;
    }
}

bool QWidgetTextControl::setFocusToAnchor(const BOBUIextCursor &newCursor)
{
    Q_D(QWidgetTextControl);

    if (!(d->interactionFlags & BobUI::LinksAccessibleByKeyboard))
        return false;

    // Verify that this is an anchor.
    const QString anchorHref = d->anchorForCursor(newCursor);
    if (anchorHref.isEmpty())
        return false;

    // and process it
    QRectF crect = selectionRect();
    emit updateRequest(crect);

    d->cursor.setPosition(newCursor.selectionStart());
    d->cursor.setPosition(newCursor.selectionEnd(), BOBUIextCursor::KeepAnchor);
    d->cursorIsFocusIndicator = true;

    crect = selectionRect();
    emit updateRequest(crect);
    emit visibilityRequest(crect);
    return true;
}

void QWidgetTextControl::setTextInteractionFlags(BobUI::TextInteractionFlags flags)
{
    Q_D(QWidgetTextControl);
    if (flags == d->interactionFlags)
        return;
    d->interactionFlags = flags;

    if (d->hasFocus)
        d->setCursorVisible(flags & BobUI::TextEditable);
}

BobUI::TextInteractionFlags QWidgetTextControl::textInteractionFlags() const
{
    Q_D(const QWidgetTextControl);
    return d->interactionFlags;
}

void QWidgetTextControl::mergeCurrentCharFormat(const BOBUIextCharFormat &modifier)
{
    Q_D(QWidgetTextControl);
    d->cursor.mergeCharFormat(modifier);
    d->updateCurrentCharFormat();
}

void QWidgetTextControl::setCurrentCharFormat(const BOBUIextCharFormat &format)
{
    Q_D(QWidgetTextControl);
    d->cursor.setCharFormat(format);
    d->updateCurrentCharFormat();
}

BOBUIextCharFormat QWidgetTextControl::currentCharFormat() const
{
    Q_D(const QWidgetTextControl);
    return d->cursor.charFormat();
}

void QWidgetTextControl::insertPlainText(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->cursor.insertText(text);
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void QWidgetTextControl::insertHtml(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->cursor.insertHtml(text);
}
#endif // BOBUI_NO_TEXTHTMLPARSER

QPointF QWidgetTextControl::anchorPosition(const QString &name) const
{
    Q_D(const QWidgetTextControl);
    if (name.isEmpty())
        return QPointF();

    QRectF r;
    for (BOBUIextBlock block = d->doc->begin(); block.isValid(); block = block.next()) {
        BOBUIextCharFormat format = block.charFormat();
        if (format.isAnchor() && format.anchorNames().contains(name)) {
            r = d->rectForPosition(block.position());
            break;
        }

        for (BOBUIextBlock::Iterator it = block.begin(); !it.atEnd(); ++it) {
            BOBUIextFragment fragment = it.fragment();
            format = fragment.charFormat();
            if (format.isAnchor() && format.anchorNames().contains(name)) {
                r = d->rectForPosition(fragment.position());
                block = BOBUIextBlock();
                break;
            }
        }
    }
    if (!r.isValid())
        return QPointF();
    return QPointF(0, r.top());
}

void QWidgetTextControl::adjustSize()
{
    Q_D(QWidgetTextControl);
    d->doc->adjustSize();
}

bool QWidgetTextControl::find(const QString &exp, BOBUIextDocument::FindFlags options)
{
    Q_D(QWidgetTextControl);
    BOBUIextCursor search = d->doc->find(exp, d->cursor, options);
    if (search.isNull())
        return false;

    setTextCursor(search);
    return true;
}

#if BOBUI_CONFIG(regularexpression)
bool QWidgetTextControl::find(const QRegularExpression &exp, BOBUIextDocument::FindFlags options)
{
    Q_D(QWidgetTextControl);
    BOBUIextCursor search = d->doc->find(exp, d->cursor, options);
    if (search.isNull())
        return false;

    setTextCursor(search);
    return true;
}
#endif

QString QWidgetTextControl::toPlainText() const
{
    return document()->toPlainText();
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
QString QWidgetTextControl::toHtml() const
{
    return document()->toHtml();
}
#endif

#if BOBUI_CONFIG(textmarkdownwriter)
QString QWidgetTextControl::toMarkdown(BOBUIextDocument::MarkdownFeatures features) const
{
    return document()->toMarkdown(features);
}
#endif

void QWidgetTextControlPrivate::insertParagraphSeparator()
{
    // clear blockFormat properties that the user is unlikely to want duplicated:
    // - don't insert <hr/> automatically
    // - the next paragraph after a heading should be a normal paragraph
    // - remove the bottom margin from the last list item before appending
    // - the next checklist item after a checked item should be unchecked
    auto blockFmt = cursor.blockFormat();
    auto charFmt = cursor.charFormat();
    blockFmt.clearProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth);
    if (blockFmt.hasProperty(BOBUIextFormat::HeadingLevel)) {
        blockFmt.clearProperty(BOBUIextFormat::HeadingLevel);
        charFmt = BOBUIextCharFormat();
    }
    if (cursor.currentList()) {
        auto existingFmt = cursor.blockFormat();
        existingFmt.clearProperty(BOBUIextBlockFormat::BlockBottomMargin);
        cursor.setBlockFormat(existingFmt);
        if (blockFmt.marker() == BOBUIextBlockFormat::MarkerType::Checked)
            blockFmt.setMarker(BOBUIextBlockFormat::MarkerType::Unchecked);
    }

    // After a blank line, reset block and char formats. I.e. you can end a list,
    // block quote, etc. by hitting enter twice, and get back to normal paragraph style.
    if (cursor.block().text().isEmpty() &&
            !cursor.blockFormat().hasProperty(BOBUIextFormat::BlockTrailingHorizontalRulerWidth) &&
            !cursor.blockFormat().hasProperty(BOBUIextFormat::BlockCodeLanguage)) {
        blockFmt = BOBUIextBlockFormat();
        const bool blockFmtChanged = (cursor.blockFormat() != blockFmt);
        charFmt = BOBUIextCharFormat();
        cursor.setBlockFormat(blockFmt);
        cursor.setCharFormat(charFmt);
        // If the user hit enter twice just to get back to default format,
        // don't actually insert a new block. But if the user then hits enter
        // yet again, the block format will not change, so we will insert a block.
        // This is what many word processors do.
        if (blockFmtChanged)
            return;
    }

    cursor.insertBlock(blockFmt, charFmt);
}

void QWidgetTextControlPrivate::append(const QString &text, BobUI::TextFormat format)
{
    BOBUIextCursor tmp(doc);
    tmp.beginEditBlock();
    tmp.movePosition(BOBUIextCursor::End);

    if (!doc->isEmpty())
        tmp.insertBlock(cursor.blockFormat(), cursor.charFormat());
    else
        tmp.setCharFormat(cursor.charFormat());

    // preserve the char format
    BOBUIextCharFormat oldCharFormat = cursor.charFormat();

#ifndef BOBUI_NO_TEXTHTMLPARSER
    if (format == BobUI::RichText || (format == BobUI::AutoText && BobUI::mightBeRichText(text))) {
        tmp.insertHtml(text);
    } else {
        tmp.insertText(text);
    }
#else
    Q_UNUSED(format);
    tmp.insertText(text);
#endif // BOBUI_NO_TEXTHTMLPARSER
    if (!cursor.hasSelection())
        cursor.setCharFormat(oldCharFormat);

    tmp.endEditBlock();
}

void QWidgetTextControl::append(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->append(text, BobUI::AutoText);
}

void QWidgetTextControl::appendHtml(const QString &html)
{
    Q_D(QWidgetTextControl);
    d->append(html, BobUI::RichText);
}

void QWidgetTextControl::appendPlainText(const QString &text)
{
    Q_D(QWidgetTextControl);
    d->append(text, BobUI::PlainText);
}


void QWidgetTextControl::ensureCursorVisible()
{
    Q_D(QWidgetTextControl);
    QRectF crect = d->rectForPosition(d->cursor.position()).adjusted(-5, 0, 5, 0);
    emit visibilityRequest(crect);
    emit microFocusChanged();
}

QPalette QWidgetTextControl::palette() const
{
    Q_D(const QWidgetTextControl);
    return d->palette;
}

void QWidgetTextControl::setPalette(const QPalette &pal)
{
    Q_D(QWidgetTextControl);
    d->palette = pal;
}

QAbstractTextDocumentLayout::PaintContext QWidgetTextControl::getPaintContext(QWidget *widget) const
{
    Q_D(const QWidgetTextControl);

    QAbstractTextDocumentLayout::PaintContext ctx;

    ctx.selections = d->extraSelections;
    ctx.palette = d->palette;
#if BOBUI_CONFIG(style_stylesheet)
    if (widget) {
        if (auto cssStyle = bobui_styleSheet(widget->style())) {
            QStyleOption option;
            option.initFrom(widget);
            cssStyle->styleSheetPalette(widget, &option, &ctx.palette);
        }
    }
#endif // style_stylesheet
    if (d->cursorOn && d->isEnabled) {
        if (d->hideCursor)
            ctx.cursorPosition = -1;
        else if (d->preeditCursor != 0)
            ctx.cursorPosition = - (d->preeditCursor + 2);
        else
            ctx.cursorPosition = d->cursor.position();
    }

    if (!d->dndFeedbackCursor.isNull())
        ctx.cursorPosition = d->dndFeedbackCursor.position();
#ifdef BOBUI_KEYPAD_NAVIGATION
    if (!QApplicationPrivate::keypadNavigationEnabled() || d->hasEditFocus)
#endif
    if (d->cursor.hasSelection()) {
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = d->cursor;
        if (d->cursorIsFocusIndicator) {
            QStyleOption opt;
            opt.palette = ctx.palette;
            QStyleHintReturnVariant ret;
            QStyle *style = QApplication::style();
            if (widget)
                style = widget->style();
            style->styleHint(QStyle::SH_TextControl_FocusIndicatorTextCharFormat, &opt, widget, &ret);
            selection.format = qvariant_cast<BOBUIextFormat>(ret.variant).toCharFormat();
        } else {
            QPalette::ColorGroup cg = d->hasFocus ? QPalette::Active : QPalette::Inactive;
            selection.format.setBackground(ctx.palette.brush(cg, QPalette::Highlight));
            selection.format.setForeground(ctx.palette.brush(cg, QPalette::HighlightedText));
            QStyleOption opt;
            QStyle *style = QApplication::style();
            if (widget) {
                opt.initFrom(widget);
                style = widget->style();
            }
            if (style->styleHint(QStyle::SH_RichText_FullWidthSelection, &opt, widget))
                selection.format.setProperty(BOBUIextFormat::FullWidthSelection, true);
        }
        ctx.selections.append(selection);
    }

    return ctx;
}

void QWidgetTextControl::drawContents(QPainter *p, const QRectF &rect, QWidget *widget)
{
    Q_D(QWidgetTextControl);
    p->save();
    QAbstractTextDocumentLayout::PaintContext ctx = getPaintContext(widget);
    if (rect.isValid())
        p->setClipRect(rect, BobUI::IntersectClip);
    ctx.clip = rect;

    d->doc->documentLayout()->draw(p, ctx);
    p->restore();
}

void QWidgetTextControlPrivate::_q_copyLink()
{
#ifndef BOBUI_NO_CLIPBOARD
    QMimeData *md = new QMimeData;
    md->setText(linkToCopy);
    QGuiApplication::clipboard()->setMimeData(md);
#endif
}

int QWidgetTextControl::hitTest(const QPointF &point, BobUI::HitTestAccuracy accuracy) const
{
    Q_D(const QWidgetTextControl);
    return d->doc->documentLayout()->hitTest(point, accuracy);
}

QRectF QWidgetTextControl::blockBoundingRect(const BOBUIextBlock &block) const
{
    Q_D(const QWidgetTextControl);
    return d->doc->documentLayout()->blockBoundingRect(block);
}

#ifndef BOBUI_NO_CONTEXTMENU
#define NUM_CONTROL_CHARACTERS 14
const struct QUnicodeControlCharacter {
    const char *text;
    ushort character;
} bobui_controlCharacters[NUM_CONTROL_CHARACTERS] = {
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "LRM Left-to-right mark"), 0x200e },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "RLM Right-to-left mark"), 0x200f },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "ZWJ Zero width joiner"), 0x200d },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "ZWNJ Zero width non-joiner"), 0x200c },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "ZWSP Zero width space"), 0x200b },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "LRE Start of left-to-right embedding"), 0x202a },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "RLE Start of right-to-left embedding"), 0x202b },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "LRO Start of left-to-right override"), 0x202d },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "RLO Start of right-to-left override"), 0x202e },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "PDF Pop directional formatting"), 0x202c },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "LRI Left-to-right isolate"), 0x2066 },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "RLI Right-to-left isolate"), 0x2067 },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "FSI First strong isolate"), 0x2068 },
    { BOBUI_TRANSLATE_NOOP("QUnicodeControlCharacterMenu", "PDI Pop directional isolate"), 0x2069 }
};

QUnicodeControlCharacterMenu::QUnicodeControlCharacterMenu(QObject *_editWidget, QWidget *parent)
    : QMenu(parent), editWidget(_editWidget)
{
    setTitle(tr("Insert Unicode control character"));
    for (int i = 0; i < NUM_CONTROL_CHARACTERS; ++i) {
        addAction(tr(bobui_controlCharacters[i].text), this, SLOT(menuActionTriggered()));
    }
}

void QUnicodeControlCharacterMenu::menuActionTriggered()
{
    QAction *a = qobject_cast<QAction *>(sender());
    int idx = actions().indexOf(a);
    if (idx < 0 || idx >= NUM_CONTROL_CHARACTERS)
        return;
    QChar c(bobui_controlCharacters[idx].character);
    QString str(c);

#if BOBUI_CONFIG(textedit)
    if (BOBUIextEdit *edit = qobject_cast<BOBUIextEdit *>(editWidget)) {
        edit->insertPlainText(str);
        return;
    }
#endif
    if (QWidgetTextControl *control = qobject_cast<QWidgetTextControl *>(editWidget)) {
        control->insertPlainText(str);
    }
#if BOBUI_CONFIG(lineedit)
    if (QLineEdit *edit = qobject_cast<QLineEdit *>(editWidget)) {
        edit->insert(str);
        return;
    }
#endif
}
#endif // BOBUI_NO_CONTEXTMENU

static constexpr auto supportedMimeTypes = qOffsetStringArray(
    "text/plain",
    "text/plain;charset=utf-8",
    "text/html"
#if BOBUI_CONFIG(textmarkdownwriter)
    , "text/markdown"
#endif
#if BOBUI_CONFIG(textodfwriter)
    , "application/vnd.oasis.opendocument.text"
#endif
);

/*! \internal
    \reimp
*/
QStringList BOBUIextEditMimeData::formats() const
{
    if (!fragment.isEmpty()) {
        constexpr auto size = supportedMimeTypes.count();
        QStringList ret;
        ret.reserve(size);
        for (int i = 0; i < size; ++i)
            ret.emplace_back(QLatin1StringView(supportedMimeTypes.at(i)));

        return ret;
    }

    return QMimeData::formats();
}

/*! \internal
    \reimp
*/
bool BOBUIextEditMimeData::hasFormat(const QString &format) const
{
    if (!fragment.isEmpty()) {
        constexpr auto size = supportedMimeTypes.count();
        for (int i = 0; i < size; ++i) {
            if (format == QLatin1StringView(supportedMimeTypes.at(i)))
                return true;
        }
        return false;
    }

    return QMimeData::hasFormat(format);
}

QVariant BOBUIextEditMimeData::retrieveData(const QString &mimeType, QMetaType type) const
{
    if (!fragment.isEmpty())
        setup();
    return QMimeData::retrieveData(mimeType, type);
}

void BOBUIextEditMimeData::setup() const
{
    BOBUIextEditMimeData *that = const_cast<BOBUIextEditMimeData *>(this);
#ifndef BOBUI_NO_TEXTHTMLPARSER
    that->setData("text/html"_L1, fragment.toHtml().toUtf8());
#endif
#if BOBUI_CONFIG(textmarkdownwriter)
    that->setData("text/markdown"_L1, fragment.toMarkdown().toUtf8());
#endif
#ifndef BOBUI_NO_TEXTODFWRITER
    {
        QBuffer buffer;
        BOBUIextDocumentWriter writer(&buffer, "ODF");
        writer.write(fragment);
        buffer.close();
        that->setData("application/vnd.oasis.opendocument.text"_L1, buffer.data());
    }
#endif
    that->setText(fragment.toPlainText());
    that->setData("text/plain;charset=utf-8"_L1, fragment.toRawText().toUtf8());
    fragment = BOBUIextDocumentFragment();
}

BOBUI_END_NAMESPACE

#include "moc_qwidgettextcontrol_p.cpp"

#endif // BOBUI_NO_TEXTCONTROL
