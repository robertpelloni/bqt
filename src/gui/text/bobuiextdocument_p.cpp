// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <private/bobuiools_p.h>
#include <qdebug.h>

#include <qscopedvaluerollback.h>
#include "bobuiextdocument_p.h"
#include "bobuiextdocument.h"
#include <bobuiextformat.h>
#include "bobuiextformat_p.h"
#include "bobuiextobject_p.h"
#include "bobuiextcursor.h"
#include "bobuiextimagehandler_p.h"
#include "bobuiextcursor_p.h"
#include "bobuiextdocumentlayout_p.h"
#include "bobuiexttable.h"
#include "bobuiextengine_p.h"

#include <BobUICore/q20utility.h>

#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

#define PMDEBUG if(0) qDebug

// The VxWorks DIAB compiler crashes when initializing the anonymous union with { a7 }
#if !defined(Q_CC_DIAB)
#  define BOBUI_INIT_TEXTUNDOCOMMAND(c, a1, a2, a3, a4, a5, a6, a7, a8) \
          BOBUIextUndoCommand c = { a1, a2, 0, 0, quint8(a3), a4, quint32(a5), quint32(a6), { int(a7) }, quint32(a8) }
#else
#  define BOBUI_INIT_TEXTUNDOCOMMAND(c, a1, a2, a3, a4, a5, a6, a7, a8) \
          BOBUIextUndoCommand c = { a1, a2, 0, 0, a3, a4, a5, a6 }; c.blockFormat = a7; c.revision = a8
#endif

/*
  Structure of a document:

  DOCUMENT :== FRAME_CONTENTS
  FRAME :== START_OF_FRAME  FRAME_CONTENTS END_OF_FRAME
  FRAME_CONTENTS = LIST_OF_BLOCKS ((FRAME | TABLE) LIST_OF_BLOCKS)*
  TABLE :== (START_OF_FRAME TABLE_CELL)+ END_OF_FRAME
  TABLE_CELL = FRAME_CONTENTS
  LIST_OF_BLOCKS :== (BLOCK END_OF_PARA)* BLOCK
  BLOCK :== (FRAGMENT)*
  FRAGMENT :== String of characters

  END_OF_PARA :== 0x2029 # Paragraph separator in Unicode
  START_OF_FRAME :== 0xfdd0
  END_OF_FRAME := 0xfdd1

  Note also that LIST_OF_BLOCKS can be empty. Nevertheless, there is
  at least one valid cursor position there where you could start
  typing. The block format is in this case determined by the last
  END_OF_PARA/START_OF_FRAME/END_OF_FRAME (see below).

  Lists are not in here, as they are treated specially. A list is just
  a collection of (not necessarily connected) blocks, that share the
  same objectIndex() in the format that refers to the list format and
  object.

  The above does not clearly note where formats are. Here's
  how it looks currently:

  FRAGMENT: one charFormat associated

  END_OF_PARA: one charFormat, and a blockFormat for the _next_ block.

  START_OF_FRAME: one char format, and a blockFormat (for the next
  block). The format associated with the objectIndex() of the
  charFormat decides whether this is a frame or table and its
  properties

  END_OF_FRAME: one charFormat and a blockFormat (for the next
  block). The object() of the charFormat is the same as for the
  corresponding START_OF_BLOCK.


  The document is independent of the layout with certain restrictions:

  * Cursor movement (esp. up and down) depend on the layout.
  * You cannot have more than one layout, as the layout data of BOBUIextObjects
    is stored in the text object itself.

*/

void BOBUIextBlockData::invalidate() const
{
    if (layout)
        layout->engine()->invalidate();
}

static bool isValidBlockSeparator(QChar ch)
{
    return ch == QChar::ParagraphSeparator
        || ch == BOBUIextBeginningOfFrame
        || ch == BOBUIextEndOfFrame;
}

static bool noBlockInString(QStringView str)
{
    return !str.contains(QChar::ParagraphSeparator)
        && !str.contains(BOBUIextBeginningOfFrame)
        && !str.contains(BOBUIextEndOfFrame);
}

bool BOBUIextUndoCommand::tryMerge(const BOBUIextUndoCommand &other)
{
    if (command != other.command)
        return false;

    if (command == Inserted
        && (pos + length == other.pos)
        && (strPos + length == other.strPos)
        && format == other.format) {

        length += other.length;
        return true;
    }

    // removal to the 'right' using 'Delete' key
    if (command == Removed
        && pos == other.pos
        && (strPos + length == other.strPos)
        && format == other.format) {

        length += other.length;
        return true;
    }

    // removal to the 'left' using 'Backspace'
    if (command == Removed
        && (other.pos + other.length == pos)
        && (other.strPos + other.length == strPos)
        && (format == other.format)) {

        int l = length;
        (*this) = other;

        length += l;
        return true;
    }

    return false;
}

BOBUIextDocumentPrivate::BOBUIextDocumentPrivate()
    : wasUndoAvailable(false),
    wasRedoAvailable(false),
    docChangeOldLength(0),
    docChangeLength(0),
    framesDirty(true),
    rtFrame(nullptr),
    initialBlockCharFormatIndex(-1), // set correctly later in init()
    resourceProvider(nullptr),
    cssMedia(QStringLiteral("screen"))
{
    editBlock = 0;
    editBlockCursorPosition = -1;
    docChangeFrom = -1;

    undoState = 0;
    revision = -1; // init() inserts a block, bringing it to 0

    lout = nullptr;

    modified = false;
    modifiedState = 0;

    undoEnabled = true;
    inContentsChange = false;
    blockCursorAdjustment = false;

    defaultTextOption.setTabStopDistance(80); // same as in bobuiextengine.cpp
    defaultTextOption.setWrapMode(BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    defaultCursorMoveStyle = BobUI::LogicalMoveStyle;

    indentWidth = 40;
    documentMargin = 4;

    maximumBlockCount = 0;
    needsEnsureMaximumBlockCount = false;
    unreachableCharacterCount = 0;
    lastBlockCount = 0;
}

void BOBUIextDocumentPrivate::init()
{
    framesDirty = false;

    bool undoState = undoEnabled;
    undoEnabled = false;
    initialBlockCharFormatIndex = formats.indexForFormat(BOBUIextCharFormat());
    insertBlock(0, formats.indexForFormat(BOBUIextBlockFormat()), formats.indexForFormat(BOBUIextCharFormat()));
    undoEnabled = undoState;
    modified = false;
    modifiedState = 0;

    qRegisterMetaType<BOBUIextDocument *>();
}

void BOBUIextDocumentPrivate::clear()
{
    Q_Q(BOBUIextDocument);

    for (BOBUIextCursorPrivate *curs : std::as_const(cursors)) {
        curs->setPosition(0);
        curs->currentCharFormat = -1;
        curs->anchor = 0;
        curs->adjusted_anchor = 0;
    }

    QSet<BOBUIextCursorPrivate *> oldCursors = cursors;
    BOBUI_TRY{
        cursors.clear();

        QMap<int, BOBUIextObject *>::Iterator objectIt = objects.begin();
        while (objectIt != objects.end()) {
            if (*objectIt != rtFrame) {
                delete *objectIt;
                objectIt = objects.erase(objectIt);
            } else {
                ++objectIt;
            }
        }
        // also clear out the remaining root frame pointer
        // (we're going to delete the object further down)
        objects.clear();

        title.clear();
        clearUndoRedoStacks(BOBUIextDocument::UndoAndRedoStacks);
        text = QString();
        unreachableCharacterCount = 0;
        modifiedState = 0;
        modified = false;
        formats.clear();
        int len = fragments.length();
        fragments.clear();
        blocks.clear();
        cachedResources.clear();
        delete rtFrame;
        rtFrame = nullptr;
        init();
        cursors = oldCursors;
        {
            QScopedValueRollback<bool> bg(inContentsChange, true);
            emit q->contentsChange(0, len, 0);
        }
        if (lout)
            lout->documentChanged(0, len, 0);
    } BOBUI_CATCH(...) {
        cursors = oldCursors; // at least recover the cursors
        BOBUI_RETHROW;
    }
}

BOBUIextDocumentPrivate::~BOBUIextDocumentPrivate()
{
    for (BOBUIextCursorPrivate *curs : std::as_const(cursors))
        curs->priv = nullptr;
    cursors.clear();
    undoState = 0;
    undoEnabled = true;
    clearUndoRedoStacks(BOBUIextDocument::RedoStack);
}

void BOBUIextDocumentPrivate::setLayout(QAbstractTextDocumentLayout *layout)
{
    Q_Q(BOBUIextDocument);
    if (lout == layout)
        return;
    const bool firstLayout = !lout;
    delete lout;
    lout = layout;

    if (!firstLayout)
        for (BlockMap::Iterator it = blocks.begin(); !it.atEnd(); ++it)
            it->free();

    emit q->documentLayoutChanged();
    {
        QScopedValueRollback<bool> bg(inContentsChange, true);
        emit q->contentsChange(0, 0, length());
    }
    if (lout)
        lout->documentChanged(0, 0, length());
}


void BOBUIextDocumentPrivate::insert_string(int pos, uint strPos, uint length, int format, BOBUIextUndoCommand::Operation op)
{
    // ##### optimize when only appending to the fragment!
    Q_ASSERT(noBlockInString(QStringView{text}.mid(strPos, length)));

    split(pos);
    uint x = fragments.insert_single(pos, length);
    BOBUIextFragmentData *X = fragments.fragment(x);
    X->format = format;
    X->stringPosition = strPos;
    uint w = fragments.previous(x);
    if (w)
        unite(w);

    int b = blocks.findNode(pos);
    blocks.setSize(b, blocks.size(b)+length);

    Q_ASSERT(blocks.length() == fragments.length());

    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(objectForFormat(format));
    if (frame) {
        frame->d_func()->fragmentAdded(text.at(strPos), x);
        framesDirty = true;
    }

    adjustDocumentChangesAndCursors(pos, length, op);
}

int BOBUIextDocumentPrivate::insert_block(int pos, uint strPos, int format, int blockFormat, BOBUIextUndoCommand::Operation op, int command)
{
    split(pos);
    uint x = fragments.insert_single(pos, 1);
    BOBUIextFragmentData *X = fragments.fragment(x);
    X->format = format;
    X->stringPosition = strPos;
    // no need trying to unite, since paragraph separators are always in a fragment of their own

    Q_ASSERT(isValidBlockSeparator(text.at(strPos)));
    Q_ASSERT(blocks.length()+1 == fragments.length());

    int block_pos = pos;
    if (blocks.length() && command == BOBUIextUndoCommand::BlockRemoved)
        ++block_pos;
    int size = 1;
    int n = blocks.findNode(block_pos);
    int key = n ? blocks.position(n) : blocks.length();

    Q_ASSERT(n || block_pos == blocks.length());
    if (key != block_pos) {
        Q_ASSERT(key < block_pos);
        int oldSize = blocks.size(n);
        blocks.setSize(n, block_pos-key);
        size += oldSize - (block_pos-key);
    }
    int b = blocks.insert_single(block_pos, size);
    BOBUIextBlockData *B = blocks.fragment(b);
    B->format = blockFormat;

    Q_ASSERT(blocks.length() == fragments.length());

    BOBUIextBlockGroup *group = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(blockFormat));
    if (group) {
        group->blockInserted(BOBUIextBlock(this, b));
        if (command != BOBUIextUndoCommand::BlockDeleted) {
            docChangeOldLength--;
            docChangeLength--;
        }
    }

    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(objectForFormat(formats.format(format)));
    if (frame) {
        frame->d_func()->fragmentAdded(text.at(strPos), x);
        framesDirty = true;
    }

    adjustDocumentChangesAndCursors(pos, 1, op);
    return x;
}

int BOBUIextDocumentPrivate::insertBlock(QChar blockSeparator,
                                  int pos, int blockFormat, int charFormat, BOBUIextUndoCommand::Operation op)
{
    Q_ASSERT(formats.format(blockFormat).isBlockFormat());
    Q_ASSERT(formats.format(charFormat).isCharFormat());
    Q_ASSERT(pos >= 0 && (pos < fragments.length() || (pos == 0 && fragments.length() == 0)));
    Q_ASSERT(isValidBlockSeparator(blockSeparator));

    beginEditBlock();

    int strPos = text.size();
    text.append(blockSeparator);

    int ob = blocks.findNode(pos);
    bool atBlockEnd = true;
    bool atBlockStart = true;
    int oldRevision = 0;
    if (ob) {
        atBlockEnd = (pos - blocks.position(ob) == blocks.size(ob)-1);
        atBlockStart = ((int)blocks.position(ob) == pos);
        oldRevision = blocks.fragment(ob)->revision;
    }

    const int fragment = insert_block(pos, strPos, charFormat, blockFormat, op, BOBUIextUndoCommand::BlockRemoved);

    Q_ASSERT(blocks.length() == fragments.length());

    int b = blocks.findNode(pos);
    BOBUIextBlockData *B = blocks.fragment(b);

    BOBUI_INIT_TEXTUNDOCOMMAND(c, BOBUIextUndoCommand::BlockInserted, (editBlock != 0),
                            op, charFormat, strPos, pos, blockFormat,
                            B->revision);

    appendUndoItem(c);
    Q_ASSERT(undoState == undoStack.size());

    // update revision numbers of the modified blocks.
    B->revision = (atBlockEnd && !atBlockStart)? oldRevision : revision;
    b = blocks.next(b);
    if (b) {
        B = blocks.fragment(b);
        B->revision = atBlockStart ? oldRevision : revision;
    }

    if (formats.charFormat(charFormat).objectIndex() == -1)
        needsEnsureMaximumBlockCount = true;

    endEditBlock();
    return fragment;
}

int BOBUIextDocumentPrivate::insertBlock(int pos, int blockFormat, int charFormat, BOBUIextUndoCommand::Operation op)
{
    return insertBlock(QChar::ParagraphSeparator, pos, blockFormat, charFormat, op);
}

void BOBUIextDocumentPrivate::insert(int pos, int strPos, int strLength, int format)
{
    if (strLength <= 0)
        return;

    Q_ASSERT(pos >= 0 && pos < fragments.length());
    Q_ASSERT(formats.format(format).isCharFormat());

    insert_string(pos, strPos, strLength, format, BOBUIextUndoCommand::MoveCursor);
    if (undoEnabled) {
        int b = blocks.findNode(pos);
        BOBUIextBlockData *B = blocks.fragment(b);

        BOBUI_INIT_TEXTUNDOCOMMAND(c, BOBUIextUndoCommand::Inserted, (editBlock != 0),
                                BOBUIextUndoCommand::MoveCursor, format, strPos, pos, strLength,
                                B->revision);
        appendUndoItem(c);
        B->revision = revision;
        Q_ASSERT(undoState == undoStack.size());
    }
    finishEdit();
}

void BOBUIextDocumentPrivate::insert(int pos, QStringView str, int format)
{
    if (str.size() == 0)
        return;

    Q_ASSERT(noBlockInString(str));

    int strPos = text.size();
    text.append(str);
    insert(pos, strPos, str.size(), format);
}

int BOBUIextDocumentPrivate::remove_string(int pos, uint length, BOBUIextUndoCommand::Operation op)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(blocks.length() == fragments.length());
    Q_ASSERT(q20::cmp_greater_equal(blocks.length(), pos+length));

    int b = blocks.findNode(pos);
    uint x = fragments.findNode(pos);

    Q_ASSERT(blocks.size(b) > length);
    Q_ASSERT(x && q20::cmp_equal(fragments.position(x), pos) && fragments.size(x) == length);
    Q_ASSERT(noBlockInString(QStringView{text}.mid(fragments.fragment(x)->stringPosition, length)));

    blocks.setSize(b, blocks.size(b)-length);

    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(objectForFormat(fragments.fragment(x)->format));
    if (frame) {
        frame->d_func()->fragmentRemoved(text.at(fragments.fragment(x)->stringPosition), x);
        framesDirty = true;
    }

    const int w = fragments.erase_single(x);

    if (!undoEnabled)
        unreachableCharacterCount += length;

    adjustDocumentChangesAndCursors(pos, -int(length), op);

    return w;
}

int BOBUIextDocumentPrivate::remove_block(int pos, int *blockFormat, int command, BOBUIextUndoCommand::Operation op)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(blocks.length() == fragments.length());
    Q_ASSERT(blocks.length() > pos);

    int b = blocks.findNode(pos);
    uint x = fragments.findNode(pos);

    Q_ASSERT(x && (int)fragments.position(x) == pos);
    Q_ASSERT(fragments.size(x) == 1);
    Q_ASSERT(isValidBlockSeparator(text.at(fragments.fragment(x)->stringPosition)));
    Q_ASSERT(b);

    if (blocks.size(b) == 1 && command == BOBUIextUndoCommand::BlockAdded) {
        Q_ASSERT((int)blocks.position(b) == pos);
        // qDebug("removing empty block");
        // empty block remove the block itself
    } else {
        // non empty block, merge with next one into this block
        // qDebug("merging block with next");
        int n = blocks.next(b);
        Q_ASSERT((int)blocks.position(n) == pos + 1);
        blocks.setSize(b, blocks.size(b) + blocks.size(n) - 1);
        blocks.fragment(b)->userState = blocks.fragment(n)->userState;
        b = n;
    }
    *blockFormat = blocks.fragment(b)->format;

    BOBUIextBlockGroup *group = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(blocks.fragment(b)->format));
    if (group)
        group->blockRemoved(BOBUIextBlock(this, b));

    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(objectForFormat(fragments.fragment(x)->format));
    if (frame) {
        frame->d_func()->fragmentRemoved(text.at(fragments.fragment(x)->stringPosition), x);
        framesDirty = true;
    }

    blocks.erase_single(b);
    const int w = fragments.erase_single(x);

    adjustDocumentChangesAndCursors(pos, -1, op);

    return w;
}

#if !defined(BOBUI_NO_DEBUG)
static bool isAncestorFrame(BOBUIextFrame *possibleAncestor, BOBUIextFrame *child)
{
    while (child) {
        if (child == possibleAncestor)
            return true;
        child = child->parentFrame();
    }
    return false;
}
#endif

void BOBUIextDocumentPrivate::move(int pos, int to, int length, BOBUIextUndoCommand::Operation op)
{
    Q_ASSERT(to <= fragments.length() && to <= pos);
    Q_ASSERT(pos >= 0 && pos+length <= fragments.length());
    Q_ASSERT(blocks.length() == fragments.length());

    if (pos == to)
        return;

    const bool needsInsert = to != -1;

#if !defined(BOBUI_NO_DEBUG)
    const bool startAndEndInSameFrame = (frameAt(pos) == frameAt(pos + length - 1));

    const bool endIsEndOfChildFrame = (isAncestorFrame(frameAt(pos), frameAt(pos + length - 1))
                                       && text.at(find(pos + length - 1)->stringPosition) == BOBUIextEndOfFrame);

    const bool startIsStartOfFrameAndEndIsEndOfFrameWithCommonParent
               = (text.at(find(pos)->stringPosition) == BOBUIextBeginningOfFrame
                  && text.at(find(pos + length - 1)->stringPosition) == BOBUIextEndOfFrame
                  && frameAt(pos)->parentFrame() == frameAt(pos + length - 1)->parentFrame());

    const bool isFirstTableCell = (qobject_cast<BOBUIextTable *>(frameAt(pos + length - 1))
                                  && frameAt(pos + length - 1)->parentFrame() == frameAt(pos));

    Q_ASSERT(startAndEndInSameFrame || endIsEndOfChildFrame || startIsStartOfFrameAndEndIsEndOfFrameWithCommonParent || isFirstTableCell);
#endif

    split(pos);
    split(pos+length);

    uint dst = needsInsert ? fragments.findNode(to) : 0;
    uint dstKey = needsInsert ? fragments.position(dst) : 0;

    uint x = fragments.findNode(pos);
    uint end = fragments.findNode(pos+length);

    uint w = 0;
    while (x != end) {
        uint n = fragments.next(x);

        uint key = fragments.position(x);
        uint b = blocks.findNode(key+1);
        BOBUIextBlockData *B = blocks.fragment(b);
        int blockRevision = B->revision;

        BOBUIextFragmentData *X = fragments.fragment(x);
        BOBUI_INIT_TEXTUNDOCOMMAND(c, BOBUIextUndoCommand::Removed, (editBlock != 0),
                                op, X->format, X->stringPosition, key, X->size_array[0],
                                blockRevision);
        BOBUI_INIT_TEXTUNDOCOMMAND(cInsert, BOBUIextUndoCommand::Inserted, (editBlock != 0),
                                op, X->format, X->stringPosition, dstKey, X->size_array[0],
                                blockRevision);

        if (key+1 != blocks.position(b)) {
//          qDebug("remove_string from %d length %d", key, X->size_array[0]);
            Q_ASSERT(noBlockInString(QStringView{text}.mid(X->stringPosition, X->size_array[0])));
            w = remove_string(key, X->size_array[0], op);

            if (needsInsert) {
                insert_string(dstKey, X->stringPosition, X->size_array[0], X->format, op);
                dstKey += X->size_array[0];
            }
        } else {
//          qDebug("remove_block at %d", key);
            Q_ASSERT(X->size_array[0] == 1 && isValidBlockSeparator(text.at(X->stringPosition)));
            b = blocks.previous(b);
            B = nullptr;
            c.command = blocks.size(b) == 1 ? BOBUIextUndoCommand::BlockDeleted : BOBUIextUndoCommand::BlockRemoved;
            w = remove_block(key, &c.blockFormat, BOBUIextUndoCommand::BlockAdded, op);

            if (needsInsert) {
                insert_block(dstKey++, X->stringPosition, X->format, c.blockFormat, op, BOBUIextUndoCommand::BlockRemoved);
                cInsert.command = blocks.size(b) == 1 ? BOBUIextUndoCommand::BlockAdded : BOBUIextUndoCommand::BlockInserted;
                cInsert.blockFormat = c.blockFormat;
            }
        }
        appendUndoItem(c);
        if (B)
            B->revision = revision;
        x = n;

        if (needsInsert)
            appendUndoItem(cInsert);
    }
    if (w)
        unite(w);

    Q_ASSERT(blocks.length() == fragments.length());

    if (!blockCursorAdjustment)
        finishEdit();
}

void BOBUIextDocumentPrivate::remove(int pos, int length, BOBUIextUndoCommand::Operation op)
{
    if (length == 0)
        return;
    blockCursorAdjustment = true;
    move(pos, -1, length, op);
    blockCursorAdjustment = false;
    for (BOBUIextCursorPrivate *curs : std::as_const(cursors)) {
        if (curs->adjustPosition(pos, -length, op) == BOBUIextCursorPrivate::CursorMoved) {
            curs->changed = true;
        }
    }
    finishEdit();
}

void BOBUIextDocumentPrivate::setCharFormat(int pos, int length, const BOBUIextCharFormat &newFormat, FormatChangeMode mode)
{
    beginEditBlock();

    Q_ASSERT(newFormat.isValid());

    int newFormatIdx = -1;
    if (mode == SetFormatAndPreserveObjectIndices) {
        BOBUIextCharFormat cleanFormat = newFormat;
        cleanFormat.clearProperty(BOBUIextFormat::ObjectIndex);
        newFormatIdx = formats.indexForFormat(cleanFormat);
    } else if (mode == SetFormat) {
        newFormatIdx = formats.indexForFormat(newFormat);
    }

    if (pos == -1) {
        if (mode == MergeFormat) {
            BOBUIextFormat format = formats.format(initialBlockCharFormatIndex);
            format.merge(newFormat);
            initialBlockCharFormatIndex = formats.indexForFormat(format);
        } else if (mode == SetFormatAndPreserveObjectIndices
                   && formats.format(initialBlockCharFormatIndex).objectIndex() != -1) {
            BOBUIextCharFormat f = newFormat;
            f.setObjectIndex(formats.format(initialBlockCharFormatIndex).objectIndex());
            initialBlockCharFormatIndex = formats.indexForFormat(f);
        } else {
            initialBlockCharFormatIndex = newFormatIdx;
        }

        ++pos;
        --length;
    }

    const int startPos = pos;
    const int endPos = pos + length;

    split(startPos);
    split(endPos);

    while (pos < endPos) {
        FragmentMap::Iterator it = fragments.find(pos);
        Q_ASSERT(!it.atEnd());

        BOBUIextFragmentData *fragment = it.value();

        Q_ASSERT(formats.format(fragment->format).type() == BOBUIextFormat::CharFormat);

        int offset = pos - it.position();
        int length = qMin(endPos - pos, int(fragment->size_array[0] - offset));
        int oldFormat = fragment->format;

        if (mode == MergeFormat) {
            BOBUIextFormat format = formats.format(fragment->format);
            format.merge(newFormat);
            fragment->format = formats.indexForFormat(format);
        } else if (mode == SetFormatAndPreserveObjectIndices
                   && formats.format(oldFormat).objectIndex() != -1) {
            BOBUIextCharFormat f = newFormat;
            f.setObjectIndex(formats.format(oldFormat).objectIndex());
            fragment->format = formats.indexForFormat(f);
        } else {
            fragment->format = newFormatIdx;
        }

        BOBUI_INIT_TEXTUNDOCOMMAND(c, BOBUIextUndoCommand::CharFormatChanged, true, BOBUIextUndoCommand::MoveCursor, oldFormat,
                                0, pos, length, 0);
        appendUndoItem(c);

        pos += length;
        Q_ASSERT(q20::cmp_equal(pos, (it.position() + fragment->size_array[0])) || pos >= endPos);
    }

    int n = fragments.findNode(startPos - 1);
    if (n)
        unite(n);

    n = fragments.findNode(endPos);
    if (n)
        unite(n);

    BOBUIextBlock blockIt = blocksFind(startPos);
    BOBUIextBlock endIt = blocksFind(endPos);
    if (endIt.isValid())
        endIt = endIt.next();
    for (; blockIt.isValid() && blockIt != endIt; blockIt = blockIt.next())
        BOBUIextDocumentPrivate::block(blockIt)->invalidate();

    documentChange(startPos, length);

    endEditBlock();
}

void BOBUIextDocumentPrivate::setBlockFormat(const BOBUIextBlock &from, const BOBUIextBlock &to,
                                          const BOBUIextBlockFormat &newFormat, FormatChangeMode mode)
{
    beginEditBlock();

    Q_ASSERT(mode != SetFormatAndPreserveObjectIndices); // only implemented for setCharFormat

    Q_ASSERT(newFormat.isValid());

    int newFormatIdx = -1;
    if (mode == SetFormat)
        newFormatIdx = formats.indexForFormat(newFormat);
    BOBUIextBlockGroup *group = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(newFormat));

    BOBUIextBlock it = from;
    BOBUIextBlock end = to;
    if (end.isValid())
        end = end.next();

    for (; it != end; it = it.next()) {
        int oldFormat = block(it)->format;
        BOBUIextBlockFormat format = formats.blockFormat(oldFormat);
        BOBUIextBlockGroup *oldGroup = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(format));
        if (mode == MergeFormat) {
            format.merge(newFormat);
            newFormatIdx = formats.indexForFormat(format);
            group = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(format));
        }
        block(it)->format = newFormatIdx;

        block(it)->invalidate();

        BOBUI_INIT_TEXTUNDOCOMMAND(c, BOBUIextUndoCommand::BlockFormatChanged, true, BOBUIextUndoCommand::MoveCursor, oldFormat,
                                0, it.position(), 1, 0);
        appendUndoItem(c);

        if (group != oldGroup) {
            if (oldGroup)
                oldGroup->blockRemoved(it);
            if (group)
                group->blockInserted(it);
        } else if (group) {
            group->blockFormatChanged(it);
        }
    }

    documentChange(from.position(), to.position() + to.length() - from.position());

    endEditBlock();
}


bool BOBUIextDocumentPrivate::split(int pos)
{
    uint x = fragments.findNode(pos);
    if (x) {
        int k = fragments.position(x);
//          qDebug("found fragment with key %d, size_left=%d, size=%d to split at %d",
//                k, (*it)->size_left[0], (*it)->size_array[0], pos);
        if (k != pos) {
            Q_ASSERT(k <= pos);
            // need to resize the first fragment and add a new one
            BOBUIextFragmentData *X = fragments.fragment(x);
            int oldsize = X->size_array[0];
            fragments.setSize(x, pos-k);
            uint n = fragments.insert_single(pos, oldsize-(pos-k));
            X = fragments.fragment(x);
            BOBUIextFragmentData *N = fragments.fragment(n);
            N->stringPosition = X->stringPosition + pos-k;
            N->format = X->format;
            return true;
        }
    }
    return false;
}

bool BOBUIextDocumentPrivate::unite(uint f)
{
    uint n = fragments.next(f);
    if (!n)
        return false;

    BOBUIextFragmentData *ff = fragments.fragment(f);
    BOBUIextFragmentData *nf = fragments.fragment(n);

    if (nf->format == ff->format && (ff->stringPosition + (int)ff->size_array[0] == nf->stringPosition)) {
        if (isValidBlockSeparator(text.at(ff->stringPosition))
            || isValidBlockSeparator(text.at(nf->stringPosition)))
            return false;

        fragments.setSize(f, ff->size_array[0] + nf->size_array[0]);
        fragments.erase_single(n);
        return true;
    }
    return false;
}


int BOBUIextDocumentPrivate::undoRedo(bool undo)
{
    PMDEBUG("%s, undoState=%d, undoStack size=%d", undo ? "undo:" : "redo:", undoState, int(undoStack.size()));
    if (!undoEnabled || (undo && undoState == 0) || (!undo && undoState == undoStack.size()))
        return -1;

    undoEnabled = false;
    beginEditBlock();
    int editPos = -1;
    int editLength = -1;
    while (1) {
        if (undo)
            --undoState;
        BOBUIextUndoCommand &c = undoStack[undoState];
        int resetBlockRevision = c.pos;

        switch (c.command) {
        case BOBUIextUndoCommand::Inserted:
            remove(c.pos, c.length, (BOBUIextUndoCommand::Operation)c.operation);
            PMDEBUG("   erase: from %d, length %d", c.pos, c.length);
            c.command = BOBUIextUndoCommand::Removed;
            editPos = c.pos;
            editLength = 0;
            break;
        case BOBUIextUndoCommand::Removed:
            PMDEBUG("   insert: format %d (from %d, length %d, strpos=%d)", c.format, c.pos, c.length, c.strPos);
            insert_string(c.pos, c.strPos, c.length, c.format, (BOBUIextUndoCommand::Operation)c.operation);
            c.command = BOBUIextUndoCommand::Inserted;
            if (editPos != (int)c.pos)
                editLength = 0;
            editPos = c.pos;
            editLength += c.length;
            break;
        case BOBUIextUndoCommand::BlockInserted:
        case BOBUIextUndoCommand::BlockAdded:
            remove_block(c.pos, &c.blockFormat, c.command, (BOBUIextUndoCommand::Operation)c.operation);
            PMDEBUG("   blockremove: from %d", c.pos);
            if (c.command == BOBUIextUndoCommand::BlockInserted)
                c.command = BOBUIextUndoCommand::BlockRemoved;
            else
                c.command = BOBUIextUndoCommand::BlockDeleted;
            editPos = c.pos;
            editLength = 0;
            break;
        case BOBUIextUndoCommand::BlockRemoved:
        case BOBUIextUndoCommand::BlockDeleted:
            PMDEBUG("   blockinsert: charformat %d blockformat %d (pos %d, strpos=%d)", c.format, c.blockFormat, c.pos, c.strPos);
            insert_block(c.pos, c.strPos, c.format, c.blockFormat, (BOBUIextUndoCommand::Operation)c.operation, c.command);
            resetBlockRevision += 1;
            if (c.command == BOBUIextUndoCommand::BlockRemoved)
                c.command = BOBUIextUndoCommand::BlockInserted;
            else
                c.command = BOBUIextUndoCommand::BlockAdded;
            if (editPos != (int)c.pos)
                editLength = 0;
            editPos = c.pos;
            editLength += 1;
            break;
        case BOBUIextUndoCommand::CharFormatChanged: {
            resetBlockRevision = -1; // ## TODO
            PMDEBUG("   charFormat: format %d (from %d, length %d)", c.format, c.pos, c.length);
            FragmentIterator it = find(c.pos);
            Q_ASSERT(!it.atEnd());

            int oldFormat = it.value()->format;
            setCharFormat(c.pos, c.length, formats.charFormat(c.format));
            c.format = oldFormat;
            if (editPos != (int)c.pos)
                editLength = 0;
            editPos = c.pos;
            editLength += c.length;
            break;
        }
        case BOBUIextUndoCommand::BlockFormatChanged: {
            resetBlockRevision = -1; // ## TODO
            PMDEBUG("   blockformat: format %d pos %d", c.format, c.pos);
            BOBUIextBlock it = blocksFind(c.pos);
            Q_ASSERT(it.isValid());

            int oldFormat = block(it)->format;
            block(it)->format = c.format;
            BOBUIextBlockGroup *oldGroup = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(formats.blockFormat(oldFormat)));
            BOBUIextBlockGroup *group = qobject_cast<BOBUIextBlockGroup *>(objectForFormat(formats.blockFormat(c.format)));
            c.format = oldFormat;
            if (group != oldGroup) {
                if (oldGroup)
                    oldGroup->blockRemoved(it);
                if (group)
                    group->blockInserted(it);
            } else if (group) {
                group->blockFormatChanged(it);
            }
            documentChange(it.position(), it.length());
            editPos = -1;
            break;
        }
        case BOBUIextUndoCommand::GroupFormatChange: {
            resetBlockRevision = -1; // ## TODO
            PMDEBUG("   group format change");
            BOBUIextObject *object = objectForIndex(c.objectIndex);
            int oldFormat = formats.objectFormatIndex(c.objectIndex);
            changeObjectFormat(object, c.format);
            c.format = oldFormat;
            editPos = -1;
            break;
        }
        case BOBUIextUndoCommand::CursorMoved:
            editPos = c.pos;
            editLength = 0;
            break;
        case BOBUIextUndoCommand::Custom:
            resetBlockRevision = -1; // ## TODO
            if (undo)
                c.custom->undo();
            else
                c.custom->redo();
            editPos = -1;
            break;
        default:
            Q_ASSERT(false);
        }

        if (resetBlockRevision >= 0) {
            int b = blocks.findNode(resetBlockRevision);
            BOBUIextBlockData *B = blocks.fragment(b);
            B->revision = c.revision;
        }

        if (!undo)
            ++undoState;

        bool inBlock = (
                undoState > 0
                && undoState < undoStack.size()
                && undoStack.at(undoState).block_part
                && undoStack.at(undoState - 1).block_part
                && !undoStack.at(undoState - 1).block_end
                );
        if (!inBlock)
            break;
    }
    undoEnabled = true;

    int newCursorPos = -1;

    if (editPos >=0)
        newCursorPos = editPos + editLength;
    else if (docChangeFrom >= 0)
        newCursorPos= qMin(docChangeFrom + docChangeLength, length() - 1);

    endEditBlock();
    emitUndoAvailable(isUndoAvailable());
    emitRedoAvailable(isRedoAvailable());

    return newCursorPos;
}

/*!
    \internal
    Appends a custom undo \a item to the undo stack.
*/
void BOBUIextDocumentPrivate::appendUndoItem(QAbstractUndoItem *item)
{
    if (!undoEnabled) {
        delete item;
        return;
    }

    BOBUIextUndoCommand c;
    c.command = BOBUIextUndoCommand::Custom;
    c.block_part = editBlock != 0;
    c.block_end = 0;
    c.operation = BOBUIextUndoCommand::MoveCursor;
    c.format = 0;
    c.strPos = 0;
    c.pos = 0;
    c.blockFormat = 0;

    c.custom = item;
    appendUndoItem(c);
}

void BOBUIextDocumentPrivate::appendUndoItem(const BOBUIextUndoCommand &c)
{
    PMDEBUG("appendUndoItem, command=%d enabled=%d", c.command, undoEnabled);
    if (!undoEnabled)
        return;
    if (undoState < undoStack.size())
        clearUndoRedoStacks(BOBUIextDocument::RedoStack);

    if (editBlock != 0 && editBlockCursorPosition >= 0) { // we had a beginEditBlock() with a cursor position
        if (q20::cmp_not_equal(c.pos, editBlockCursorPosition)) { // and that cursor position is different from the command
            // generate a CursorMoved undo item
            BOBUI_INIT_TEXTUNDOCOMMAND(cc, BOBUIextUndoCommand::CursorMoved, true, BOBUIextUndoCommand::MoveCursor,
                                    0, 0, editBlockCursorPosition, 0, 0);
            undoStack.append(cc);
            undoState++;
            editBlockCursorPosition = -1;
        }
    }


    if (!undoStack.isEmpty() && modified) {
        const int lastIdx = undoState - 1;
        const BOBUIextUndoCommand &last = undoStack.at(lastIdx);

        if ( (last.block_part && c.block_part && !last.block_end) // part of the same block => can merge
            || (!c.block_part && !last.block_part) // two single undo items => can merge
            || (c.command == BOBUIextUndoCommand::Inserted && last.command == c.command && (last.block_part && !c.block_part))) {
            // two sequential inserts that are not part of the same block => can merge
            if (undoStack[lastIdx].tryMerge(c))
                return;
        }
    }
    if (modifiedState > undoState)
        modifiedState = -1;
    undoStack.append(c);
    undoState++;
    emitUndoAvailable(true);
    emitRedoAvailable(false);

    if (!c.block_part)
        emit document()->undoCommandAdded();
}

void BOBUIextDocumentPrivate::clearUndoRedoStacks(BOBUIextDocument::Stacks stacksToClear,
                                               bool emitSignals)
{
    bool undoCommandsAvailable = undoState != 0;
    bool redoCommandsAvailable = undoState != undoStack.size();
    if (stacksToClear == BOBUIextDocument::UndoStack && undoCommandsAvailable) {
        for (int i = 0; i < undoState; ++i) {
            BOBUIextUndoCommand c = undoStack.at(i);
            if (c.command & BOBUIextUndoCommand::Custom)
                delete c.custom;
        }
        undoStack.remove(0, undoState);
        undoState = 0;
        if (emitSignals)
            emitUndoAvailable(false);
    } else if (stacksToClear == BOBUIextDocument::RedoStack
               && redoCommandsAvailable) {
        for (int i = undoState; i < undoStack.size(); ++i) {
            BOBUIextUndoCommand c = undoStack.at(i);
            if (c.command & BOBUIextUndoCommand::Custom)
                delete c.custom;
        }
        undoStack.resize(undoState);
        if (emitSignals)
            emitRedoAvailable(false);
    } else if (stacksToClear == BOBUIextDocument::UndoAndRedoStacks
               && !undoStack.isEmpty()) {
        for (int i = 0; i < undoStack.size(); ++i) {
            BOBUIextUndoCommand c = undoStack.at(i);
            if (c.command & BOBUIextUndoCommand::Custom)
                delete c.custom;
        }
        undoState = 0;
        undoStack.clear();
        if (emitSignals && undoCommandsAvailable)
            emitUndoAvailable(false);
        if (emitSignals && redoCommandsAvailable)
            emitRedoAvailable(false);
    }
}

void BOBUIextDocumentPrivate::emitUndoAvailable(bool available)
{
    if (available != wasUndoAvailable) {
        Q_Q(BOBUIextDocument);
        emit q->undoAvailable(available);
        wasUndoAvailable = available;
    }
}

void BOBUIextDocumentPrivate::emitRedoAvailable(bool available)
{
    if (available != wasRedoAvailable) {
        Q_Q(BOBUIextDocument);
        emit q->redoAvailable(available);
        wasRedoAvailable = available;
    }
}

void BOBUIextDocumentPrivate::enableUndoRedo(bool enable)
{
    if (enable && maximumBlockCount > 0)
        return;

    if (!enable) {
        undoState = 0;
        clearUndoRedoStacks(BOBUIextDocument::RedoStack);
        emitUndoAvailable(false);
        emitRedoAvailable(false);
    }
    modifiedState = modified ? -1 : undoState;
    undoEnabled = enable;
    if (!undoEnabled)
        compressPieceTable();
}

void BOBUIextDocumentPrivate::joinPreviousEditBlock()
{
    beginEditBlock();

    if (undoEnabled && undoState)
        undoStack[undoState - 1].block_end = false;
}

void BOBUIextDocumentPrivate::endEditBlock()
{
    Q_ASSERT(editBlock > 0);
    if (--editBlock)
        return;

    if (undoEnabled && undoState > 0) {
        const bool wasBlocking = !undoStack.at(undoState - 1).block_end;
        if (undoStack.at(undoState - 1).block_part) {
            undoStack[undoState - 1].block_end = true;
            if (wasBlocking)
                emit document()->undoCommandAdded();
        }
    }

    editBlockCursorPosition = -1;

    finishEdit();
}

void BOBUIextDocumentPrivate::finishEdit()
{
    Q_Q(BOBUIextDocument);

    if (editBlock)
        return;

    if (framesDirty)
        scan_frames(docChangeFrom, docChangeOldLength, docChangeLength);

    if (lout && docChangeFrom >= 0) {
        if (!inContentsChange) {
            QScopedValueRollback<bool> bg(inContentsChange, true);
            emit q->contentsChange(docChangeFrom, docChangeOldLength, docChangeLength);
        }
        lout->documentChanged(docChangeFrom, docChangeOldLength, docChangeLength);
    }

    docChangeFrom = -1;

    if (needsEnsureMaximumBlockCount) {
        needsEnsureMaximumBlockCount = false;
        if (ensureMaximumBlockCount()) {
            // if ensureMaximumBlockCount() returns true
            // it will have called endEditBlock() and
            // compressPieceTable() itself, so we return here
            // to prevent getting two contentsChanged emits
            return;
        }
    }

    QList<BOBUIextCursor> changedCursors;
    for (BOBUIextCursorPrivate *curs : std::as_const(cursors)) {
        if (curs->changed) {
            curs->changed = false;
            changedCursors.append(BOBUIextCursor(curs));
        }
    }
    for (const BOBUIextCursor &cursor : std::as_const(changedCursors))
        emit q->cursorPositionChanged(cursor);

    contentsChanged();

    if (blocks.numNodes() != lastBlockCount) {
        lastBlockCount = blocks.numNodes();
        emit q->blockCountChanged(lastBlockCount);
    }

    if (!undoEnabled && unreachableCharacterCount)
        compressPieceTable();
}

void BOBUIextDocumentPrivate::documentChange(int from, int length)
{
//     qDebug("BOBUIextDocumentPrivate::documentChange: from=%d,length=%d", from, length);
    if (docChangeFrom < 0) {
        docChangeFrom = from;
        docChangeOldLength = length;
        docChangeLength = length;
        return;
    }
    int start = qMin(from, docChangeFrom);
    int end = qMax(from + length, docChangeFrom + docChangeLength);
    int diff = qMax(0, end - start - docChangeLength);
    docChangeFrom = start;
    docChangeOldLength += diff;
    docChangeLength += diff;
}

/*
    adjustDocumentChangesAndCursors is called whenever there is an insert or remove of characters.
    param from is the cursor position in the document
    param addedOrRemoved is the amount of characters added or removed.  A negative number means characters are removed.

    The function stores information to be emitted when finishEdit() is called.
*/
void BOBUIextDocumentPrivate::adjustDocumentChangesAndCursors(int from, int addedOrRemoved, BOBUIextUndoCommand::Operation op)
{
    if (!editBlock)
        ++revision;

    if (blockCursorAdjustment)  {
        ; // postpone, will be called again from BOBUIextDocumentPrivate::remove()
    } else {
        for (BOBUIextCursorPrivate *curs : std::as_const(cursors)) {
            if (curs->adjustPosition(from, addedOrRemoved, op) == BOBUIextCursorPrivate::CursorMoved) {
                curs->changed = true;
            }
        }
    }

//     qDebug("BOBUIextDocumentPrivate::adjustDocumentChanges: from=%d,addedOrRemoved=%d", from, addedOrRemoved);
    if (docChangeFrom < 0) {
        docChangeFrom = from;
        if (addedOrRemoved > 0) {
            docChangeOldLength = 0;
            docChangeLength = addedOrRemoved;
        } else {
            docChangeOldLength = -addedOrRemoved;
            docChangeLength = 0;
        }
//         qDebug("adjustDocumentChanges:");
//         qDebug("    -> %d %d %d", docChangeFrom, docChangeOldLength, docChangeLength);
        return;
    }

    // have to merge the new change with the already existing one.
    int added = qMax(0, addedOrRemoved);
    int removed = qMax(0, -addedOrRemoved);

    int diff = 0;
    if (from + removed < docChangeFrom)
        diff = docChangeFrom - from - removed;
    else if (from > docChangeFrom + docChangeLength)
        diff = from - (docChangeFrom + docChangeLength);

    int overlap_start = qMax(from, docChangeFrom);
    int overlap_end = qMin(from + removed, docChangeFrom + docChangeLength);
    int removedInside = qMax(0, overlap_end - overlap_start);
    removed -= removedInside;

//     qDebug("adjustDocumentChanges: from=%d, addedOrRemoved=%d, diff=%d, removedInside=%d", from, addedOrRemoved, diff, removedInside);
    docChangeFrom = qMin(docChangeFrom, from);
    docChangeOldLength += removed + diff;
    docChangeLength += added - removedInside + diff;
//     qDebug("    -> %d %d %d", docChangeFrom, docChangeOldLength, docChangeLength);

}


QString BOBUIextDocumentPrivate::plainText() const
{
    QString result;
    result.resize(length());
    const QChar *text_unicode = text.unicode();
    QChar *data = result.data();
    for (BOBUIextDocumentPrivate::FragmentIterator it = begin(); it != end(); ++it) {
        const BOBUIextFragmentData *f = *it;
        ::memcpy(data, text_unicode + f->stringPosition, f->size_array[0] * sizeof(QChar));
        data += f->size_array[0];
    }
    // remove trailing block separator
    result.chop(1);
    return result;
}

int BOBUIextDocumentPrivate::blockCharFormatIndex(int node) const
{
    int pos = blocks.position(node);
    if (pos == 0)
        return initialBlockCharFormatIndex;

    return fragments.find(pos - 1)->format;
}

int BOBUIextDocumentPrivate::nextCursorPosition(int position, BOBUIextLayout::CursorMode mode) const
{
    if (position == length()-1)
        return position;

    BOBUIextBlock it = blocksFind(position);
    int start = it.position();
    int end = start + it.length() - 1;
    if (position == end)
        return end + 1;

    return it.layout()->nextCursorPosition(position-start, mode) + start;
}

int BOBUIextDocumentPrivate::previousCursorPosition(int position, BOBUIextLayout::CursorMode mode) const
{
    if (position == 0)
        return position;

    BOBUIextBlock it = blocksFind(position);
    int start = it.position();
    if (position == start)
        return start - 1;

    return it.layout()->previousCursorPosition(position-start, mode) + start;
}

int BOBUIextDocumentPrivate::leftCursorPosition(int position) const
{
    BOBUIextBlock it = blocksFind(position);
    int start = it.position();
    return it.layout()->leftCursorPosition(position-start) + start;
}

int BOBUIextDocumentPrivate::rightCursorPosition(int position) const
{
    BOBUIextBlock it = blocksFind(position);
    int start = it.position();
    return it.layout()->rightCursorPosition(position-start) + start;
}

void BOBUIextDocumentPrivate::changeObjectFormat(BOBUIextObject *obj, int format)
{
    beginEditBlock();
    int objectIndex = obj->objectIndex();
    int oldFormatIndex = formats.objectFormatIndex(objectIndex);
    formats.setObjectFormatIndex(objectIndex, format);

    BOBUIextBlockGroup *b = qobject_cast<BOBUIextBlockGroup *>(obj);
    if (b) {
        b->d_func()->markBlocksDirty();
    }
    BOBUIextFrame *f = qobject_cast<BOBUIextFrame *>(obj);
    if (f)
        documentChange(f->firstPosition(), f->lastPosition() - f->firstPosition());

    BOBUI_INIT_TEXTUNDOCOMMAND(c, BOBUIextUndoCommand::GroupFormatChange, (editBlock != 0), BOBUIextUndoCommand::MoveCursor, oldFormatIndex,
                            0, 0, obj->d_func()->objectIndex, 0);
    appendUndoItem(c);

    endEditBlock();
}

static BOBUIextFrame *findChildFrame(BOBUIextFrame *f, int pos)
{
    /* Binary search for frame at pos */
    const QList<BOBUIextFrame *> children = f->childFrames();
    int first = 0;
    int last = children.size() - 1;
    while (first <= last) {
        int mid = (first + last) / 2;
        BOBUIextFrame *c = children.at(mid);
        if (pos > c->lastPosition())
            first = mid + 1;
        else if (pos < c->firstPosition())
            last = mid - 1;
        else
            return c;
    }
    return nullptr;
}

BOBUIextFrame *BOBUIextDocumentPrivate::rootFrame() const
{
    if (!rtFrame) {
        BOBUIextFrameFormat defaultRootFrameFormat;
        defaultRootFrameFormat.setMargin(documentMargin);
        rtFrame = qobject_cast<BOBUIextFrame *>(const_cast<BOBUIextDocumentPrivate *>(this)->createObject(defaultRootFrameFormat));
    }
    return rtFrame;
}

void BOBUIextDocumentPrivate::addCursor(BOBUIextCursorPrivate *c)
{
    cursors.insert(c);
}

void BOBUIextDocumentPrivate::removeCursor(BOBUIextCursorPrivate *c)
{
    cursors.remove(c);
}

BOBUIextFrame *BOBUIextDocumentPrivate::frameAt(int pos) const
{
    BOBUIextFrame *f = rootFrame();

    while (1) {
        BOBUIextFrame *c = findChildFrame(f, pos);
        if (!c)
            return f;
        f = c;
    }
}

void BOBUIextDocumentPrivate::clearFrame(BOBUIextFrame *f)
{
    for (int i = 0; i < f->d_func()->childFrames.size(); ++i)
        clearFrame(f->d_func()->childFrames.at(i));
    f->d_func()->childFrames.clear();
    f->d_func()->parentFrame = nullptr;
}

void BOBUIextDocumentPrivate::scan_frames(int pos, int charsRemoved, int charsAdded)
{
    // ###### optimize
    Q_UNUSED(pos);
    Q_UNUSED(charsRemoved);
    Q_UNUSED(charsAdded);

    BOBUIextFrame *f = rootFrame();
    clearFrame(f);

    for (FragmentIterator it = begin(); it != end(); ++it) {
        // BOBUIextFormat fmt = formats.format(it->format);
        BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(objectForFormat(it->format));
        if (!frame)
            continue;

        Q_ASSERT(it.size() == 1);
        QChar ch = text.at(it->stringPosition);

        if (ch == BOBUIextBeginningOfFrame) {
            if (f != frame) {
                // f == frame happens for tables
                Q_ASSERT(frame->d_func()->fragment_start == it.n || frame->d_func()->fragment_start == 0);
                frame->d_func()->parentFrame = f;
                f->d_func()->childFrames.append(frame);
                f = frame;
            }
        } else if (ch == BOBUIextEndOfFrame) {
            Q_ASSERT(f == frame);
            Q_ASSERT(frame->d_func()->fragment_end == it.n || frame->d_func()->fragment_end == 0);
            f = frame->d_func()->parentFrame;
        } else if (ch == QChar::ObjectReplacementCharacter) {
            Q_ASSERT(f != frame);
            Q_ASSERT(frame->d_func()->fragment_start == it.n || frame->d_func()->fragment_start == 0);
            Q_ASSERT(frame->d_func()->fragment_end == it.n || frame->d_func()->fragment_end == 0);
            frame->d_func()->parentFrame = f;
            f->d_func()->childFrames.append(frame);
        } else {
            Q_ASSERT(false);
        }
    }
    Q_ASSERT(f == rtFrame);
    framesDirty = false;
}

void BOBUIextDocumentPrivate::insert_frame(BOBUIextFrame *f)
{
    int start = f->firstPosition();
    int end = f->lastPosition();
    BOBUIextFrame *parent = frameAt(start-1);
    Q_ASSERT(parent == frameAt(end+1));

    if (start != end) {
        // iterator over the parent and move all children contained in my frame to myself
        for (int i = 0; i < parent->d_func()->childFrames.size(); ++i) {
            BOBUIextFrame *c = parent->d_func()->childFrames.at(i);
            if (start < c->firstPosition() && end > c->lastPosition()) {
                parent->d_func()->childFrames.removeAt(i);
                f->d_func()->childFrames.append(c);
                c->d_func()->parentFrame = f;
            }
        }
    }
    // insert at the correct position
    int i = 0;
    for (; i < parent->d_func()->childFrames.size(); ++i) {
        BOBUIextFrame *c = parent->d_func()->childFrames.at(i);
        if (c->firstPosition() > end)
            break;
    }
    parent->d_func()->childFrames.insert(i, f);
    f->d_func()->parentFrame = parent;
}

BOBUIextFrame *BOBUIextDocumentPrivate::insertFrame(int start, int end, const BOBUIextFrameFormat &format)
{
    Q_ASSERT(start >= 0 && start < length());
    Q_ASSERT(end >= 0 && end < length());
    Q_ASSERT(start <= end || end == -1);

    if (start != end && frameAt(start) != frameAt(end))
        return nullptr;

    beginEditBlock();

    BOBUIextFrame *frame = qobject_cast<BOBUIextFrame *>(createObject(format));
    Q_ASSERT(frame);

    // #### using the default block and char format below might be wrong
    int idx = formats.indexForFormat(BOBUIextBlockFormat());
    BOBUIextCharFormat cfmt;
    cfmt.setObjectIndex(frame->objectIndex());
    int charIdx = formats.indexForFormat(cfmt);

    insertBlock(BOBUIextBeginningOfFrame, start, idx, charIdx, BOBUIextUndoCommand::MoveCursor);
    insertBlock(BOBUIextEndOfFrame, ++end, idx, charIdx, BOBUIextUndoCommand::KeepCursor);

    frame->d_func()->fragment_start = find(start).n;
    frame->d_func()->fragment_end = find(end).n;

    insert_frame(frame);

    endEditBlock();

    return frame;
}

void BOBUIextDocumentPrivate::removeFrame(BOBUIextFrame *frame)
{
    BOBUIextFrame *parent = frame->d_func()->parentFrame;
    if (!parent)
        return;

    int start = frame->firstPosition();
    int end = frame->lastPosition();
    Q_ASSERT(end >= start);

    beginEditBlock();

    // remove already removes the frames from the tree
    remove(end, 1);
    remove(start-1, 1);

    endEditBlock();
}

BOBUIextObject *BOBUIextDocumentPrivate::objectForIndex(int objectIndex) const
{
    if (objectIndex < 0)
        return nullptr;

    BOBUIextObject *object = objects.value(objectIndex, nullptr);
    if (!object) {
        BOBUIextDocumentPrivate *that = const_cast<BOBUIextDocumentPrivate *>(this);
        BOBUIextFormat fmt = formats.objectFormat(objectIndex);
        object = that->createObject(fmt, objectIndex);
    }
    return object;
}

BOBUIextObject *BOBUIextDocumentPrivate::objectForFormat(int formatIndex) const
{
    int objectIndex = formats.format(formatIndex).objectIndex();
    return objectForIndex(objectIndex);
}

BOBUIextObject *BOBUIextDocumentPrivate::objectForFormat(const BOBUIextFormat &f) const
{
    return objectForIndex(f.objectIndex());
}

BOBUIextObject *BOBUIextDocumentPrivate::createObject(const BOBUIextFormat &f, int objectIndex)
{
    BOBUIextObject *obj = document()->createObject(f);

    if (obj) {
        obj->d_func()->objectIndex = objectIndex == -1 ? formats.createObjectIndex(f) : objectIndex;
        objects[obj->d_func()->objectIndex] = obj;
    }

    return obj;
}

void BOBUIextDocumentPrivate::deleteObject(BOBUIextObject *object)
{
    const int objIdx = object->d_func()->objectIndex;
    objects.remove(objIdx);
    delete object;
}

void BOBUIextDocumentPrivate::contentsChanged()
{
    Q_Q(BOBUIextDocument);
    if (editBlock)
        return;

    bool m = undoEnabled ? (modifiedState != undoState) : true;
    if (modified != m) {
        modified = m;
        emit q->modificationChanged(modified);
    }

    emit q->contentsChanged();
}

void BOBUIextDocumentPrivate::compressPieceTable()
{
    if (undoEnabled)
        return;

    const uint garbageCollectionThreshold = 96 * 1024; // bytes

    //qDebug() << "unreachable bytes:" << unreachableCharacterCount * sizeof(QChar) << " -- limit" << garbageCollectionThreshold << "text size =" << text.size() << "capacity:" << text.capacity();

    bool compressTable = unreachableCharacterCount * sizeof(QChar) > garbageCollectionThreshold
                         && text.size() >= text.capacity() * 0.9;
    if (!compressTable)
        return;

    QString newText;
    newText.resize(text.size());
    QChar *newTextPtr = newText.data();
    int newLen = 0;

    for (FragmentMap::Iterator it = fragments.begin(); !it.atEnd(); ++it) {
        memcpy(newTextPtr, text.constData() + it->stringPosition, it->size_array[0] * sizeof(QChar));
        it->stringPosition = newLen;
        newTextPtr += it->size_array[0];
        newLen += it->size_array[0];
    }

    newText.resize(newLen);
    newText.squeeze();
    //qDebug() << "removed" << text.size() - newText.size() << "characters";
    text = newText;
    unreachableCharacterCount = 0;
}

void BOBUIextDocumentPrivate::setModified(bool m)
{
    Q_Q(BOBUIextDocument);
    if (m == modified)
        return;

    modified = m;
    if (!modified)
        modifiedState = undoState;
    else
        modifiedState = -1;

    emit q->modificationChanged(modified);
}

bool BOBUIextDocumentPrivate::ensureMaximumBlockCount()
{
    if (maximumBlockCount <= 0)
        return false;
    if (blocks.numNodes() <= maximumBlockCount)
        return false;

    beginEditBlock();

    const int blocksToRemove = blocks.numNodes() - maximumBlockCount;
    BOBUIextCursor cursor(this, 0);
    cursor.movePosition(BOBUIextCursor::NextBlock, BOBUIextCursor::KeepAnchor, blocksToRemove);

    unreachableCharacterCount += cursor.selectionEnd() - cursor.selectionStart();

    // preserve the char format of the paragraph that is to become the new first one
    BOBUIextCharFormat charFmt = cursor.blockCharFormat();
    cursor.removeSelectedText();
    cursor.setBlockCharFormat(charFmt);

    endEditBlock();

    compressPieceTable();

    return true;
}

/// This method is called from BOBUIextTable when it is about to remove a table-cell to allow cursors to update their selection.
void BOBUIextDocumentPrivate::aboutToRemoveCell(int from, int to)
{
    Q_ASSERT(from <= to);
    for (BOBUIextCursorPrivate *curs : std::as_const(cursors))
        curs->aboutToRemoveCell(from, to);
}

BOBUI_END_NAMESPACE
