// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiextobject.h"
#include "bobuiextobject_p.h"
#include "bobuiextcursor_p.h"
#include "bobuiextdocument.h"
#include "bobuiextformat_p.h"
#include "bobuiextdocument_p.h"
#include "bobuiextcursor.h"
#include "bobuiextlist.h"
#include "qabstracttextdocumentlayout.h"
#include "bobuiextengine_p.h"
#include "qdebug.h"

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

// ### DOC: We ought to explain the CONCEPT of objectIndexes if
// relevant to the public API
/*!
    \class BOBUIextObject
    \reentrant

    \brief The BOBUIextObject class is a base class for different kinds
    of objects that can group parts of a BOBUIextDocument together.
    \inmodule BobUIGui

    \ingroup richtext-processing

    The common grouping text objects are lists (BOBUIextList), frames
    (BOBUIextFrame), and tables (BOBUIextTable). A text object has an
    associated format() and document().

    There are essentially two kinds of text objects: those that are used
    with blocks (block formats), and those that are used with characters
    (character formats). The first kind are derived from BOBUIextBlockGroup,
    and the second kind from BOBUIextFrame.

    You rarely need to use this class directly. When creating custom text
    objects, you will also need to reimplement BOBUIextDocument::createObject()
    which acts as a factory method for creating text objects.

    \sa BOBUIextDocument
*/

/*!
    \fn BOBUIextObject::BOBUIextObject(BOBUIextDocument *document)

    Creates a new BOBUIextObject for the given \a document.

    \warning This function should never be called directly, but only
    from BOBUIextDocument::createObject().
*/
BOBUIextObject::BOBUIextObject(BOBUIextDocument *doc)
    : QObject(*new BOBUIextObjectPrivate(doc), doc)
{
}

/*!
  \fn BOBUIextObject::BOBUIextObject(BOBUIextObjectPrivate &p, BOBUIextDocument *document)

  \internal
*/
BOBUIextObject::BOBUIextObject(BOBUIextObjectPrivate &p, BOBUIextDocument *doc)
    : QObject(p, doc)
{
}

/*!
    Destroys the text object.

    \warning Text objects are owned by the document, so you should
    never destroy them yourself.
*/
BOBUIextObject::~BOBUIextObject()
{
}

/*!
    Returns the text object's format.

    \sa setFormat(), document()
*/
BOBUIextFormat BOBUIextObject::format() const
{
    Q_D(const BOBUIextObject);
    return d->pieceTable->formatCollection()->objectFormat(d->objectIndex);
}

/*!
    Returns the index of the object's format in the document's internal
    list of formats.

    \sa BOBUIextDocument::allFormats()
*/
int BOBUIextObject::formatIndex() const
{
    Q_D(const BOBUIextObject);
    return d->pieceTable->formatCollection()->objectFormatIndex(d->objectIndex);
}


/*!
    Sets the text object's \a format.

    \sa format()
*/
void BOBUIextObject::setFormat(const BOBUIextFormat &format)
{
    Q_D(BOBUIextObject);
    int idx = d->pieceTable->formatCollection()->indexForFormat(format);
    d->pieceTable->changeObjectFormat(this, idx);
}

/*!
    Returns the object index of this object. This can be used together with
    BOBUIextFormat::setObjectIndex().
*/
int BOBUIextObject::objectIndex() const
{
    Q_D(const BOBUIextObject);
    return d->objectIndex;
}

/*!
    Returns the document this object belongs to.

    \sa format()
*/
BOBUIextDocument *BOBUIextObject::document() const
{
    return static_cast<BOBUIextDocument *>(parent());
}

/*!
    \class BOBUIextBlockGroup
    \reentrant

    \brief The BOBUIextBlockGroup class provides a container for text blocks within
    a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing

    Block groups can be used to organize blocks of text within a document.
    They maintain an up-to-date list of the text blocks that belong to
    them, even when text blocks are being edited.

    Each group has a parent document which is specified when the group is
    constructed.

    Text blocks can be inserted into a group with blockInserted(), and removed
    with blockRemoved(). If a block's format is changed, blockFormatChanged()
    is called.

    The list of blocks in the group is returned by blockList(). Note that the
    blocks in the list are not necessarily adjacent elements in the document;
    for example, the top-level items in a multi-level list will be separated
    by the items in lower levels of the list.

    \sa BOBUIextBlock, BOBUIextDocument
*/

void BOBUIextBlockGroupPrivate::markBlocksDirty()
{
    for (int i = 0; i < blocks.size(); ++i) {
        const BOBUIextBlock &block = blocks.at(i);
        pieceTable->documentChange(block.position(), block.length());
    }
}

/*!
    \fn BOBUIextBlockGroup::BOBUIextBlockGroup(BOBUIextDocument *document)

    Creates a new block group for the given \a document.

    \warning This function should only be called from
    BOBUIextDocument::createObject().
*/
BOBUIextBlockGroup::BOBUIextBlockGroup(BOBUIextDocument *doc)
    : BOBUIextObject(*new BOBUIextBlockGroupPrivate(doc), doc)
{
}

/*!
  \internal
*/
BOBUIextBlockGroup::BOBUIextBlockGroup(BOBUIextBlockGroupPrivate &p, BOBUIextDocument *doc)
    : BOBUIextObject(p, doc)
{
}

/*!
    Destroys this block group; the blocks are not deleted, they simply
    don't belong to this block anymore.
*/
BOBUIextBlockGroup::~BOBUIextBlockGroup()
{
}

// ### DOC: Shouldn't this be insertBlock()?
/*!
    Appends the given \a block to the end of the group.

    \warning If you reimplement this function you must call the base
    class implementation.
*/
void BOBUIextBlockGroup::blockInserted(const BOBUIextBlock &block)
{
    Q_D(BOBUIextBlockGroup);
    BOBUIextBlockGroupPrivate::BlockList::Iterator it = std::lower_bound(d->blocks.begin(), d->blocks.end(), block);
    d->blocks.insert(it, block);
    d->markBlocksDirty();
}

// ### DOC: Shouldn't this be removeBlock()?
/*!
    Removes the given \a block from the group; the block itself is not
    deleted, it simply isn't a member of this group anymore.
*/
void BOBUIextBlockGroup::blockRemoved(const BOBUIextBlock &block)
{
    Q_D(BOBUIextBlockGroup);
    d->blocks.removeAll(block);
    d->markBlocksDirty();
    if (d->blocks.isEmpty()) {
        BOBUIextDocumentPrivate::get(document())->deleteObject(this);
        return;
    }
}

/*!
    This function is called whenever the specified \a block of text is changed.
    The text block is a member of this group.

    The base class implementation does nothing.
*/
void BOBUIextBlockGroup::blockFormatChanged(const BOBUIextBlock &)
{
}

/*!
    Returns a (possibly empty) list of all the blocks that are part of
    the block group.
*/
QList<BOBUIextBlock> BOBUIextBlockGroup::blockList() const
{
    Q_D(const BOBUIextBlockGroup);
    return d->blocks;
}



BOBUIextFrameLayoutData::~BOBUIextFrameLayoutData()
{
}


/*!
    \class BOBUIextFrame
    \reentrant

    \brief The BOBUIextFrame class represents a frame in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing

    Text frames provide structure for the text in a document. They are used
    as generic containers for other document elements.
    Frames are usually created by using BOBUIextCursor::insertFrame().

    \omit
    Each frame in a document consists of a frame start character,
    QChar(0xFDD0), followed by the frame's contents, followed by a
    frame end character, QChar(0xFDD1). The character formats of the
    start and end character contain a reference to the frame object's
    objectIndex.
    \endomit

    Frames can be used to create hierarchical structures in rich text documents.
    Each document has a root frame (BOBUIextDocument::rootFrame()), and each frame
    beneath the root frame has a parent frame and a (possibly empty) list of
    child frames. The parent frame can be found with parentFrame(), and the
    childFrames() function provides a list of child frames.

    Each frame contains at least one text block to enable text cursors to
    insert new document elements within. As a result, the BOBUIextFrame::iterator
    class is used to traverse both the blocks and child frames within a given
    frame. The first and last child elements in the frame can be found with
    begin() and end().

    A frame also has a format (specified using BOBUIextFrameFormat) which can be set
    with setFormat() and read with format().

    Text cursors can be obtained that point to the first and last valid cursor
    positions within a frame; use the firstCursorPosition() and
    lastCursorPosition() functions for this. The frame's extent in the
    document can be found with firstPosition() and lastPosition().

    You can iterate over a frame's contents using the
    BOBUIextFrame::iterator class: this provides read-only access to its
    internal list of text blocks and child frames.

    \sa BOBUIextCursor, BOBUIextDocument
*/

/*!
    \typedef BOBUIextFrame::Iterator

    BobUI-style synonym for BOBUIextFrame::iterator.
*/

/*!
    \fn BOBUIextFrame *BOBUIextFrame::iterator::parentFrame() const

    Returns the parent frame of the current frame.

    \sa currentFrame(), BOBUIextFrame::parentFrame()
*/

/*!
    \fn bool BOBUIextFrame::iterator::operator==(const iterator &other) const

    Returns true if the iterator is the same as the \a other iterator;
    otherwise returns \c false.
*/

/*!
    \fn bool BOBUIextFrame::iterator::operator!=(const iterator &other) const

    Returns true if the iterator is different from the \a other iterator;
    otherwise returns \c false.
*/

/*!
    \fn BOBUIextFrame::iterator BOBUIextFrame::iterator::operator++(int)

    The postfix \c{++} operator (\c{i++}) advances the iterator to the
    next item in the text frame, and returns an iterator to the old item.
*/

/*!
    \fn BOBUIextFrame::iterator BOBUIextFrame::iterator::operator--(int)

    The postfix \c{--} operator (\c{i--}) makes the preceding item in the
    current frame, and returns an iterator to the old item.
*/

/*!
    \fn void BOBUIextFrame::setFrameFormat(const BOBUIextFrameFormat &format)

    Sets the frame's \a format.

    \sa frameFormat()
*/

/*!
    \fn BOBUIextFrameFormat BOBUIextFrame::frameFormat() const

    Returns the frame's format.

    \sa setFrameFormat()
*/

/*!
    \fn BOBUIextFrame::BOBUIextFrame(BOBUIextDocument *document)

    Creates a new empty frame for the text \a document.
*/
BOBUIextFrame::BOBUIextFrame(BOBUIextDocument *doc)
    : BOBUIextObject(*new BOBUIextFramePrivate(doc), doc)
{
}

/*!
    Destroys the text frame.

    \warning Text frames are owned by the document, so you should
    never destroy them yourself. In order to remove a frame from
    its document, remove its contents using a \c BOBUIextCursor.
*/
BOBUIextFrame::~BOBUIextFrame()
{
    Q_D(BOBUIextFrame);
    delete d->layoutData;
}

/*!
    \internal
*/
BOBUIextFrame::BOBUIextFrame(BOBUIextFramePrivate &p, BOBUIextDocument *doc)
    : BOBUIextObject(p, doc)
{
}

/*!
    Returns a (possibly empty) list of the frame's child frames.

    \sa parentFrame()
*/
QList<BOBUIextFrame *> BOBUIextFrame::childFrames() const
{
    Q_D(const BOBUIextFrame);
    return d->childFrames;
}

/*!
    Returns the frame's parent frame. If the frame is the root frame of a
    document, this will return 0.

    \sa childFrames(), BOBUIextDocument::rootFrame()
*/
BOBUIextFrame *BOBUIextFrame::parentFrame() const
{
    Q_D(const BOBUIextFrame);
    return d->parentFrame;
}


/*!
    Returns the first cursor position inside the frame.

    \sa lastCursorPosition(), firstPosition(), lastPosition()
*/
BOBUIextCursor BOBUIextFrame::firstCursorPosition() const
{
    Q_D(const BOBUIextFrame);
    return BOBUIextCursorPrivate::fromPosition(d->pieceTable, firstPosition());
}

/*!
    Returns the last cursor position inside the frame.

    \sa firstCursorPosition(), firstPosition(), lastPosition()
*/
BOBUIextCursor BOBUIextFrame::lastCursorPosition() const
{
    Q_D(const BOBUIextFrame);
    return BOBUIextCursorPrivate::fromPosition(d->pieceTable, lastPosition());
}

/*!
    Returns the first document position inside the frame.

    \sa lastPosition(), firstCursorPosition(), lastCursorPosition()
*/
int BOBUIextFrame::firstPosition() const
{
    Q_D(const BOBUIextFrame);
    if (!d->fragment_start)
        return 0;
    return d->pieceTable->fragmentMap().position(d->fragment_start) + 1;
}

/*!
    Returns the last document position inside the frame.

    \sa firstPosition(), firstCursorPosition(), lastCursorPosition()
*/
int BOBUIextFrame::lastPosition() const
{
    Q_D(const BOBUIextFrame);
    if (!d->fragment_end)
        return d->pieceTable->length() - 1;
    return d->pieceTable->fragmentMap().position(d->fragment_end);
}

/*!
  \internal
*/
BOBUIextFrameLayoutData *BOBUIextFrame::layoutData() const
{
    Q_D(const BOBUIextFrame);
    return d->layoutData;
}

/*!
  \internal
*/
void BOBUIextFrame::setLayoutData(BOBUIextFrameLayoutData *data)
{
    Q_D(BOBUIextFrame);
    delete d->layoutData;
    d->layoutData = data;
}



void BOBUIextFramePrivate::fragmentAdded(QChar type, uint fragment)
{
    if (type == BOBUIextBeginningOfFrame) {
        Q_ASSERT(!fragment_start);
        fragment_start = fragment;
    } else if (type == BOBUIextEndOfFrame) {
        Q_ASSERT(!fragment_end);
        fragment_end = fragment;
    } else if (type == QChar::ObjectReplacementCharacter) {
        Q_ASSERT(!fragment_start);
        Q_ASSERT(!fragment_end);
        fragment_start = fragment;
        fragment_end = fragment;
    } else {
        Q_ASSERT(false);
    }
}

void BOBUIextFramePrivate::fragmentRemoved(QChar type, uint fragment)
{
    Q_UNUSED(fragment); // --release warning
    if (type == BOBUIextBeginningOfFrame) {
        Q_ASSERT(fragment_start == fragment);
        fragment_start = 0;
    } else if (type == BOBUIextEndOfFrame) {
        Q_ASSERT(fragment_end == fragment);
        fragment_end = 0;
    } else if (type == QChar::ObjectReplacementCharacter) {
        Q_ASSERT(fragment_start == fragment);
        Q_ASSERT(fragment_end == fragment);
        fragment_start = 0;
        fragment_end = 0;
    } else {
        Q_ASSERT(false);
    }
    remove_me();
}


void BOBUIextFramePrivate::remove_me()
{
    Q_Q(BOBUIextFrame);
    if (fragment_start == 0 && fragment_end == 0
        && !parentFrame) {
        BOBUIextDocumentPrivate::get(q->document())->deleteObject(q);
        return;
    }

    if (!parentFrame)
        return;

    int index = parentFrame->d_func()->childFrames.indexOf(q);

    // iterator over all children and move them to the parent
    for (int i = 0; i < childFrames.size(); ++i) {
        BOBUIextFrame *c = childFrames.at(i);
        parentFrame->d_func()->childFrames.insert(index, c);
        c->d_func()->parentFrame = parentFrame;
        ++index;
    }
    Q_ASSERT(parentFrame->d_func()->childFrames.at(index) == q);
    parentFrame->d_func()->childFrames.removeAt(index);

    childFrames.clear();
    parentFrame = nullptr;
}

/*!
    \class BOBUIextFrame::iterator
    \reentrant

    \brief The iterator class provides an iterator for reading
    the contents of a BOBUIextFrame.

    \inmodule BobUIGui
    \ingroup richtext-processing

    A frame consists of an arbitrary sequence of \l{BOBUIextBlock}s and
    child \l{BOBUIextFrame}s. This class provides a way to iterate over the
    child objects of a frame, and read their contents. It does not provide
    a way to modify the contents of the frame.

*/

/*!
    \fn bool BOBUIextFrame::iterator::atEnd() const

    Returns \c true if the current item is the last item in the text frame.
*/

/*!
    Returns an iterator pointing to the first document element inside the frame.
    Please see the document \l{STL-style-Iterators} for more information.

    \sa end()
*/
BOBUIextFrame::iterator BOBUIextFrame::begin() const
{
    const BOBUIextDocumentPrivate *priv = BOBUIextDocumentPrivate::get(this);
    int b = priv->blockMap().findNode(firstPosition());
    int e = priv->blockMap().findNode(lastPosition()+1);
    return iterator(const_cast<BOBUIextFrame *>(this), b, b, e);
}

/*!
    Returns an iterator pointing to the position past the last document element inside the frame.
    Please see the document \l{STL-Style Iterators} for more information.
    \sa begin()
*/
BOBUIextFrame::iterator BOBUIextFrame::end() const
{
    const BOBUIextDocumentPrivate *priv = BOBUIextDocumentPrivate::get(this);
    int b = priv->blockMap().findNode(firstPosition());
    int e = priv->blockMap().findNode(lastPosition()+1);
    return iterator(const_cast<BOBUIextFrame *>(this), e, b, e);
}

/*!
    \fn BOBUIextFrame::iterator::iterator()

    Constructs an invalid iterator.
*/

/*!
    \fn BOBUIextFrame *BOBUIextFrame::iterator::currentFrame() const
    Returns the current frame pointed to by the iterator, or \nullptr
    if the iterator currently points to a block.

    \sa currentBlock()
*/

/*!
    Returns the current block the iterator points to. If the iterator
    points to a child frame, the returned block is invalid.

    \sa currentFrame()
*/
BOBUIextBlock BOBUIextFrame::iterator::currentBlock() const
{
    if (!f)
        return BOBUIextBlock();
    return BOBUIextBlock(BOBUIextDocumentPrivate::get(f), cb);
}

/*!
    Moves the iterator to the next frame or block.

    \sa currentBlock(), currentFrame()
*/
BOBUIextFrame::iterator &BOBUIextFrame::iterator::operator++()
{
    const BOBUIextDocumentPrivate *priv = BOBUIextDocumentPrivate::get(f);
    const BOBUIextDocumentPrivate::BlockMap &map = priv->blockMap();
    if (cf) {
        int end = cf->lastPosition() + 1;
        cb = map.findNode(end);
        cf = nullptr;
    } else if (cb) {
        cb = map.next(cb);
        if (cb == e)
            return *this;

        if (!f->d_func()->childFrames.isEmpty()) {
            int pos = map.position(cb);
            // check if we entered a frame
            BOBUIextDocumentPrivate::FragmentIterator frag = priv->find(pos-1);
            if (priv->buffer().at(frag->stringPosition) != QChar::ParagraphSeparator) {
                BOBUIextFrame *nf = qobject_cast<BOBUIextFrame *>(priv->objectForFormat(frag->format));
                if (nf) {
                    if (priv->buffer().at(frag->stringPosition) == BOBUIextBeginningOfFrame && nf != f) {
                        cf = nf;
                        cb = 0;
                    } else {
                        Q_ASSERT(priv->buffer().at(frag->stringPosition) != BOBUIextEndOfFrame);
                    }
                }
            }
        }
    }
    return *this;
}

/*!
    Moves the iterator to the previous frame or block.

    \sa currentBlock(), currentFrame()
*/
BOBUIextFrame::iterator &BOBUIextFrame::iterator::operator--()
{
    const BOBUIextDocumentPrivate *priv = BOBUIextDocumentPrivate::get(f);
    const BOBUIextDocumentPrivate::BlockMap &map = priv->blockMap();
    if (cf) {
        int start = cf->firstPosition() - 1;
        cb = map.findNode(start);
        cf = nullptr;
    } else {
        if (cb == b)
            goto end;
        if (cb != e) {
            int pos = map.position(cb);
            // check if we have to enter a frame
            BOBUIextDocumentPrivate::FragmentIterator frag = priv->find(pos-1);
            if (priv->buffer().at(frag->stringPosition) != QChar::ParagraphSeparator) {
                BOBUIextFrame *pf = qobject_cast<BOBUIextFrame *>(priv->objectForFormat(frag->format));
                if (pf) {
                    if (priv->buffer().at(frag->stringPosition) == BOBUIextBeginningOfFrame) {
                        Q_ASSERT(pf == f);
                    } else if (priv->buffer().at(frag->stringPosition) == BOBUIextEndOfFrame) {
                        Q_ASSERT(pf != f);
                        cf = pf;
                        cb = 0;
                        goto end;
                    }
                }
            }
        }
        cb = map.previous(cb);
    }
 end:
    return *this;
}

/*!
    \class BOBUIextBlockUserData
    \reentrant

    \brief The BOBUIextBlockUserData class is used to associate custom data with blocks of text.
    \inmodule BobUIGui
    \since 4.1

    \ingroup richtext-processing

    BOBUIextBlockUserData provides an abstract interface for container classes that are used
    to associate application-specific user data with text blocks in a BOBUIextDocument.

    Generally, subclasses of this class provide functions to allow data to be stored
    and retrieved, and instances are attached to blocks of text using
    BOBUIextBlock::setUserData(). This makes it possible to store additional data per text
    block in a way that can be retrieved safely by the application.

    Each subclass should provide a reimplementation of the destructor to ensure that any
    private data is automatically cleaned up when user data objects are deleted.

    \sa BOBUIextBlock
*/

/*!
    Destroys the user data.
*/
BOBUIextBlockUserData::~BOBUIextBlockUserData()
{
}

/*!
    \class BOBUIextBlock
    \reentrant

    \brief The BOBUIextBlock class provides a container for text fragments in a
    BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing

    A text block encapsulates a block or paragraph of text in a BOBUIextDocument.
    BOBUIextBlock provides read-only access to the block/paragraph structure of
    BOBUIextDocuments. It is mainly of use if you want to implement your own
    layouts for the visual representation of a BOBUIextDocument, or if you want to
    iterate over a document and write out the contents in your own custom
    format.

    Text blocks are created by their parent documents. If you need to create
    a new text block, or modify the contents of a document while examining its
    contents, use the cursor-based interface provided by BOBUIextCursor instead.

    Each text block is located at a specific position() in a document().
    The contents of the block can be obtained by using the text() function.
    The length() function determines the block's size within the document
    (including formatting characters).
    The visual properties of the block are determined by its text layout(),
    its charFormat(), and its blockFormat().

    The next() and previous() functions enable iteration over consecutive
    valid blocks in a document under the condition that the document is not
    modified by other means during the iteration process. Note that, although
    blocks are returned in sequence, adjacent blocks may come from different
    places in the document structure. The validity of a block can be determined
    by calling isValid().

    BOBUIextBlock provides comparison operators to make it easier to work with
    blocks: \l operator==() compares two block for equality, \l operator!=()
    compares two blocks for inequality, and \l operator<() determines whether
    a block precedes another in the same document.

    \image bobuiextblock-sequence.png {Diagram showing multiple text blocks in
           a hierarchy of text frames, and the iteration order between the
           text blocks.}

    \sa BOBUIextBlockFormat, BOBUIextCharFormat, BOBUIextFragment
 */

/*!
    \fn BOBUIextBlock::BOBUIextBlock(BOBUIextDocumentPrivate *priv, int b)

    \internal
*/

/*!
    \fn BOBUIextBlock::BOBUIextBlock()

    \internal
*/

/*!
    \fn BOBUIextBlock::BOBUIextBlock(const BOBUIextBlock &other)

    Copies the \a other text block's attributes to this text block.
*/

/*!
    \fn bool BOBUIextBlock::isValid() const

    Returns \c true if this text block is valid; otherwise returns \c false.
*/

bool BOBUIextBlock::isValid() const
{
    return p != nullptr && p->blockMap().isValid(n);
}

/*!
    \fn BOBUIextBlock &BOBUIextBlock::operator=(const BOBUIextBlock &other)

    Assigns the \a other text block to this text block.
*/

/*!
    \fn bool BOBUIextBlock::operator==(const BOBUIextBlock &other) const

    Returns \c true if this text block is the same as the \a other text
    block.
*/

/*!
    \fn bool BOBUIextBlock::operator!=(const BOBUIextBlock &other) const

    Returns \c true if this text block is different from the \a other
    text block.
*/

/*!
    \fn bool BOBUIextBlock::operator<(const BOBUIextBlock &other) const

    Returns \c true if this text block occurs before the \a other text
    block in the document.
*/

/*!
    \class BOBUIextBlock::iterator
    \reentrant

    \brief The BOBUIextBlock::iterator class provides an iterator for reading
    the contents of a BOBUIextBlock.
    \inmodule BobUIGui

    \ingroup richtext-processing

    A block consists of a sequence of text fragments. This class provides
    a way to iterate over these, and read their contents. It does not provide
    a way to modify the internal structure or contents of the block.

    An iterator can be constructed and used to access the fragments within
    a text block in the following way:

    \snippet textblock-fragments/xmlwriter.cpp 4
    \snippet textblock-fragments/xmlwriter.cpp 7

    \sa BOBUIextFragment
*/

/*!
    \typedef BOBUIextBlock::Iterator

    BobUI-style synonym for BOBUIextBlock::iterator.
*/

/*!
    \fn BOBUIextBlock::iterator::iterator()

    Constructs an iterator for this text block.
*/

/*!
    \fn bool BOBUIextBlock::iterator::atEnd() const

    Returns \c true if the current item is the last item in the text block.
*/

/*!
    \fn bool BOBUIextBlock::iterator::operator==(const iterator &other) const

    Returns true if this iterator is the same as the \a other iterator;
    otherwise returns \c false.
*/

/*!
    \fn bool BOBUIextBlock::iterator::operator!=(const iterator &other) const

    Returns true if this iterator is different from the \a other iterator;
    otherwise returns \c false.
*/

/*!
    \fn BOBUIextBlock::iterator BOBUIextBlock::iterator::operator++(int)

    The postfix ++ operator (\c{i++}) advances the iterator to the
    next item in the text block and returns an iterator to the old current
    item.
*/

/*!
    \fn BOBUIextBlock::iterator BOBUIextBlock::iterator::operator--(int)

    The postfix -- operator (\c{i--}) makes the preceding item current and
    returns an iterator to the old current item.
*/

/*!
    \fn int BOBUIextBlock::fragmentIndex() const

    \internal
*/

/*!
    Returns the index of the block's first character within the document.
 */
int BOBUIextBlock::position() const
{
    if (!p || !n)
        return 0;

    return p->blockMap().position(n);
}

/*!
    Returns the length of the block in characters.

    \note The length returned includes all formatting characters,
    for example, newline.

    \sa text(), charFormat(), blockFormat()
 */
int BOBUIextBlock::length() const
{
    if (!p || !n)
        return 0;

    return p->blockMap().size(n);
}

/*!
    Returns \c true if the given \a position is located within the text
    block; otherwise returns \c false.
 */
bool BOBUIextBlock::contains(int position) const
{
    if (!p || !n)
        return false;

    int pos = p->blockMap().position(n);
    int len = p->blockMap().size(n);
    return position >= pos && position < pos + len;
}

/*!
    Returns the BOBUIextLayout that is used to lay out and display the
    block's contents.

    Note that the returned BOBUIextLayout object can only be modified from the
    documentChanged implementation of a QAbstractTextDocumentLayout subclass.
    Any changes applied from the outside cause undefined behavior.

    \sa clearLayout()
 */
BOBUIextLayout *BOBUIextBlock::layout() const
{
    if (!p || !n)
        return nullptr;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    if (!b->layout)
        b->layout = new BOBUIextLayout(*this);
    return b->layout;
}

/*!
    \since 4.4
    Clears the BOBUIextLayout that is used to lay out and display the
    block's contents.

    \sa layout()
 */
void BOBUIextBlock::clearLayout()
{
    if (!p || !n)
        return;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    if (b->layout)
        b->layout->clearLayout();
}

/*!
    Returns the BOBUIextBlockFormat that describes block-specific properties.

    \sa charFormat()
 */
BOBUIextBlockFormat BOBUIextBlock::blockFormat() const
{
    if (!p || !n)
        return BOBUIextFormat().toBlockFormat();

    return p->formatCollection()->blockFormat(p->blockMap().fragment(n)->format);
}

/*!
    Returns an index into the document's internal list of block formats
    for the text block's format.

    \sa BOBUIextDocument::allFormats()
*/
int BOBUIextBlock::blockFormatIndex() const
{
    if (!p || !n)
        return -1;

    return p->blockMap().fragment(n)->format;
}

/*!
    Returns the BOBUIextCharFormat that describes the block's character
    format. The block's character format is used when inserting text into
    an empty block.

    \sa blockFormat()
 */
BOBUIextCharFormat BOBUIextBlock::charFormat() const
{
    if (!p || !n)
        return BOBUIextFormat().toCharFormat();

    return p->formatCollection()->charFormat(charFormatIndex());
}

/*!
    Returns an index into the document's internal list of character formats
    for the text block's character format.

    \sa BOBUIextDocument::allFormats()
*/
int BOBUIextBlock::charFormatIndex() const
{
    if (!p || !n)
        return -1;

    return p->blockCharFormatIndex(n);
}

/*!
  \since 4.7

  Returns the resolved text direction.

  If the block has no explicit direction set, it will resolve the
  direction from the blocks content. Returns either BobUI::LeftToRight
  or BobUI::RightToLeft.

  \sa BOBUIextFormat::layoutDirection(), QString::isRightToLeft(), BobUI::LayoutDirection
*/
BobUI::LayoutDirection BOBUIextBlock::textDirection() const
{
    BobUI::LayoutDirection dir = blockFormat().layoutDirection();
    if (dir != BobUI::LayoutDirectionAuto)
        return dir;

    dir = p->defaultTextOption.textDirection();
    if (dir != BobUI::LayoutDirectionAuto)
        return dir;

    const QString buffer = p->buffer();

    const int pos = position();
    BOBUIextDocumentPrivate::FragmentIterator it = p->find(pos);
    BOBUIextDocumentPrivate::FragmentIterator end = p->find(pos + length() - 1); // -1 to omit the block separator char
    for (; it != end; ++it) {
        const BOBUIextFragmentData * const frag = it.value();
        const QChar *p = buffer.constData() + frag->stringPosition;
        const QChar * const end = p + frag->size_array[0];
        while (p < end) {
            uint ucs4 = p->unicode();
            if (QChar::isHighSurrogate(ucs4) && p + 1 < end) {
                ushort low = p[1].unicode();
                if (QChar::isLowSurrogate(low)) {
                    ucs4 = QChar::surrogateToUcs4(ucs4, low);
                    ++p;
                }
            }
            switch (QChar::direction(ucs4)) {
            case QChar::DirL:
                return BobUI::LeftToRight;
            case QChar::DirR:
            case QChar::DirAL:
                return BobUI::RightToLeft;
            default:
                break;
            }
            ++p;
        }
    }
    return BobUI::LeftToRight;
}

/*!
    Returns the block's contents as plain text.

    \sa length(), charFormat(), blockFormat()
 */
QString BOBUIextBlock::text() const
{
    if (!p || !n)
        return QString();

    const QString buffer = p->buffer();
    QString text;
    text.reserve(length());

    const int pos = position();
    BOBUIextDocumentPrivate::FragmentIterator it = p->find(pos);
    BOBUIextDocumentPrivate::FragmentIterator end = p->find(pos + length() - 1); // -1 to omit the block separator char
    for (; it != end; ++it) {
        const BOBUIextFragmentData * const frag = it.value();
        text += QStringView(buffer.constData() + frag->stringPosition, frag->size_array[0]);
    }

    return text;
}

/*!
    \since 5.3

    Returns the block's text format options as a list of continuous ranges
    of BOBUIextCharFormat. The range's character format is used when inserting text
    within the range boundaries.

    \sa charFormat(), blockFormat()
*/
QList<BOBUIextLayout::FormatRange> BOBUIextBlock::textFormats() const
{
    QList<BOBUIextLayout::FormatRange> formats;
    if (!p || !n)
        return formats;

    const BOBUIextFormatCollection *formatCollection = p->formatCollection();

    int start = 0;
    int cur = start;
    int format = -1;

    const int pos = position();
    BOBUIextDocumentPrivate::FragmentIterator it = p->find(pos);
    BOBUIextDocumentPrivate::FragmentIterator end = p->find(pos + length() - 1); // -1 to omit the block separator char
    for (; it != end; ++it) {
        const BOBUIextFragmentData * const frag = it.value();
        if (format != it.value()->format) {
            if (cur - start > 0) {
                BOBUIextLayout::FormatRange range;
                range.start = start;
                range.length = cur - start;
                range.format = formatCollection->charFormat(format);
                formats.append(range);
            }

            format = frag->format;
            start = cur;
        }
        cur += frag->size_array[0];
    }
    if (cur - start > 0) {
        BOBUIextLayout::FormatRange range;
        range.start = start;
        range.length = cur - start;
        range.format = formatCollection->charFormat(format);
        formats.append(range);
    }

    return formats;
}

/*!
    Returns the text document this text block belongs to, or \nullptr
    if the text block does not belong to any document.
*/
const BOBUIextDocument *BOBUIextBlock::document() const
{
    return p ? p->document() : nullptr;
}

/*!
    If the block represents a list item, returns the list that the item belongs
    to; otherwise returns \nullptr.
*/
BOBUIextList *BOBUIextBlock::textList() const
{
    if (!isValid())
        return nullptr;

    const BOBUIextBlockFormat fmt = blockFormat();
    BOBUIextObject *obj = p->document()->objectForFormat(fmt);
    return qobject_cast<BOBUIextList *>(obj);
}

/*!
    \since 4.1

    Returns a pointer to a BOBUIextBlockUserData object,
    if one has been set with setUserData(), or \nullptr.
*/
BOBUIextBlockUserData *BOBUIextBlock::userData() const
{
    if (!p || !n)
        return nullptr;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    return b->userData;
}

/*!
    \since 4.1

    Attaches the given \a data object to the text block.

    BOBUIextBlockUserData can be used to store custom settings.  The
    ownership is passed to the underlying text document, i.e. the
    provided BOBUIextBlockUserData object will be deleted if the
    corresponding text block gets deleted. The user data object is
    not stored in the undo history, so it will not be available after
    undoing the deletion of a text block.

    For example, if you write a programming editor in an IDE, you may
    want to let your user set breakpoints visually in your code for an
    integrated debugger. In a programming editor a line of text
    usually corresponds to one BOBUIextBlock. The BOBUIextBlockUserData
    interface allows the developer to store data for each BOBUIextBlock,
    like for example in which lines of the source code the user has a
    breakpoint set. Of course this could also be stored externally,
    but by storing it inside the BOBUIextDocument, it will for example be
    automatically deleted when the user deletes the associated
    line. It's really just a way to store custom information in the
    BOBUIextDocument without using custom properties in BOBUIextFormat which
    would affect the undo/redo stack.
*/
void BOBUIextBlock::setUserData(BOBUIextBlockUserData *data)
{
    if (!p || !n)
        return;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    if (data != b->userData)
        delete b->userData;
    b->userData = data;
}

/*!
    \since 4.1

    Returns the integer value previously set with setUserState() or -1.
*/
int BOBUIextBlock::userState() const
{
    if (!p || !n)
        return -1;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    return b->userState;
}

/*!
    \since 4.1

    Stores the specified \a state integer value in the text block. This may be
    useful for example in a syntax highlighter to store a text parsing state.
*/
void BOBUIextBlock::setUserState(int state)
{
    if (!p || !n)
        return;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    b->userState = state;
}

/*!
    \since 4.4

    Returns the blocks revision.

    \sa setRevision(), BOBUIextDocument::revision()
*/
int BOBUIextBlock::revision() const
{
    if (!p || !n)
        return -1;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    return b->revision;
}

/*!
    \since 4.4

    Sets a blocks revision to \a rev.

    \sa revision(), BOBUIextDocument::revision()
*/
void BOBUIextBlock::setRevision(int rev)
{
    if (!p || !n)
        return;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    b->revision = rev;
}

/*!
    \since 4.4

    Returns \c true if the block is visible; otherwise returns \c false.

    \sa setVisible()
*/
bool BOBUIextBlock::isVisible() const
{
    if (!p || !n)
        return true;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    return !b->hidden;
}

/*!
    \since 4.4

    Sets the block's visibility to \a visible.

    \sa isVisible()
*/
void BOBUIextBlock::setVisible(bool visible)
{
    if (!p || !n)
        return;

    const BOBUIextBlockData *b = p->blockMap().fragment(n);
    b->hidden = !visible;
}


/*!
\since 4.4

    Returns the number of this block, or -1 if the block is invalid.

    \sa BOBUIextCursor::blockNumber()

*/
int BOBUIextBlock::blockNumber() const
{
    if (!p || !n)
        return -1;
    return p->blockMap().position(n, 1);
}

/*!
\since 4.5

    Returns the first line number of this block, or -1 if the block is invalid.
    Unless the layout supports it, the line number is identical to the block number.

    \sa BOBUIextBlock::blockNumber()

*/
int BOBUIextBlock::firstLineNumber() const
{
    if (!p || !n)
        return -1;
    return p->blockMap().position(n, 2);
}


/*!
\since 4.5

Sets the line count to \a count.

\sa lineCount()
*/
void BOBUIextBlock::setLineCount(int count)
{
    if (!p || !n)
        return;
    p->blockMap().setSize(n, count, 2);
}
/*!
\since 4.5

Returns the line count. Not all document layouts support this feature.

\sa setLineCount()
 */
int BOBUIextBlock::lineCount() const
{
    if (!p || !n)
        return -1;
    return p->blockMap().size(n, 2);
}


/*!
    Returns a text block iterator pointing to the beginning of the
    text block.

    \sa end()
*/
BOBUIextBlock::iterator BOBUIextBlock::begin() const
{
    if (!p || !n)
        return iterator();

    int pos = position();
    int len = length() - 1; // exclude the fragment that holds the paragraph separator
    int b = p->fragmentMap().findNode(pos);
    int e = p->fragmentMap().findNode(pos+len);
    return iterator(p, b, e, b);
}

/*!
    Returns a text block iterator pointing to the end of the text
    block.

    \sa begin(), next(), previous()
*/
BOBUIextBlock::iterator BOBUIextBlock::end() const
{
    if (!p || !n)
        return iterator();

    int pos = position();
    int len = length() - 1; // exclude the fragment that holds the paragraph separator
    int b = p->fragmentMap().findNode(pos);
    int e = p->fragmentMap().findNode(pos+len);
    return iterator(p, b, e, e);
}


/*!
    Returns the text block in the document after this block, or an empty
    text block if this is the last one.

    Note that the next block may be in a different frame or table to this block.

    \sa previous(), begin(), end()
*/
BOBUIextBlock BOBUIextBlock::next() const
{
    if (!isValid())
        return BOBUIextBlock();

    return BOBUIextBlock(p, p->blockMap().next(n));
}

/*!
    Returns the text block in the document before this block, or an empty text
    block if this is the first one.

    Note that the previous block may be in a different frame or table to this block.

    \sa next(), begin(), end()
*/
BOBUIextBlock BOBUIextBlock::previous() const
{
    if (!p)
        return BOBUIextBlock();

    return BOBUIextBlock(p, p->blockMap().previous(n));
}


/*!
    Returns the text fragment the iterator currently points to.
*/
BOBUIextFragment BOBUIextBlock::iterator::fragment() const
{
    int ne = n;
    int formatIndex = p->fragmentMap().fragment(n)->format;
    do {
        ne = p->fragmentMap().next(ne);
    } while (ne != e && p->fragmentMap().fragment(ne)->format == formatIndex);
    return BOBUIextFragment(p, n, ne);
}

/*!
    The prefix ++ operator (\c{++i}) advances the iterator to the
    next item in the hash and returns an iterator to the new current
    item.
*/

BOBUIextBlock::iterator &BOBUIextBlock::iterator::operator++()
{
    int ne = n;
    int formatIndex = p->fragmentMap().fragment(n)->format;
    do {
        ne = p->fragmentMap().next(ne);
    } while (ne != e && p->fragmentMap().fragment(ne)->format == formatIndex);
    n = ne;
    return *this;
}

/*!
    The prefix -- operator (\c{--i}) makes the preceding item
    current and returns an iterator pointing to the new current item.
*/

BOBUIextBlock::iterator &BOBUIextBlock::iterator::operator--()
{
    n = p->fragmentMap().previous(n);

    if (n == b)
        return *this;

    int formatIndex = p->fragmentMap().fragment(n)->format;
    int last = n;

    while (n != b && p->fragmentMap().fragment(n)->format != formatIndex) {
        last = n;
        n = p->fragmentMap().previous(n);
    }

    n = last;
    return *this;
}


/*!
    \class BOBUIextFragment
    \reentrant

    \brief The BOBUIextFragment class holds a piece of text in a
    BOBUIextDocument with a single BOBUIextCharFormat.
    \inmodule BobUIGui

    \ingroup richtext-processing

    A text fragment describes a piece of text that is stored with a single
    character format. Text in which the character format changes can be
    represented by sequences of text fragments with different formats.

    If the user edits the text in a fragment and introduces a different
    character format, the fragment's text will be split at each point where
    the format changes, and new fragments will be created.
    For example, changing the style of some text in the middle of a
    sentence will cause the fragment to be broken into three separate fragments:
    the first and third with the same format as before, and the second with
    the new style. The first fragment will contain the text from the beginning
    of the sentence, the second will contain the text from the middle, and the
    third takes the text from the end of the sentence.

    \image bobuiextfragment-split.png {Screenshot showing how a text is separated
           in three fragments if the middle text is made bold.}

    A fragment's text and character format can be obtained with the text()
    and charFormat() functions. The length() function gives the length of
    the text in the fragment. position() gives the position in the document
    of the start of the fragment. To determine whether the fragment contains
    a particular position within the document, use the contains() function.

    \sa BOBUIextDocument, {Rich Text Document Structure}
*/

/*!
    \fn BOBUIextFragment::BOBUIextFragment(const BOBUIextDocumentPrivate *priv, int f, int fe)
    \internal
*/

/*!
    \fn BOBUIextFragment::BOBUIextFragment()

    Creates a new empty text fragment.
*/

/*!
    \fn BOBUIextFragment::BOBUIextFragment(const BOBUIextFragment &other)

    Copies the content (text and format) of the \a other text fragment
    to this text fragment.
*/

/*!
    \fn BOBUIextFragment &BOBUIextFragment::operator=(const BOBUIextFragment
    &other)

    Assigns the content (text and format) of the \a other text fragment
    to this text fragment.
*/

/*!
    \fn bool BOBUIextFragment::isValid() const

    Returns \c true if this is a valid text fragment (i.e. has a valid
    position in a document); otherwise returns \c false.
*/

/*!
    \fn bool BOBUIextFragment::operator==(const BOBUIextFragment &other) const

    Returns \c true if this text fragment is the same (at the same
    position) as the \a other text fragment; otherwise returns \c false.
*/

/*!
    \fn bool BOBUIextFragment::operator!=(const BOBUIextFragment &other) const

    Returns \c true if this text fragment is different (at a different
    position) from the \a other text fragment; otherwise returns
    false.
*/

/*!
    \fn bool BOBUIextFragment::operator<(const BOBUIextFragment &other) const

    Returns \c true if this text fragment appears earlier in the document
    than the \a other text fragment; otherwise returns \c false.
*/

/*!
    Returns the glyphs corresponding to \a len characters of this text fragment starting at
    position \a pos. The positions of the glyphs are relative to the position of the BOBUIextBlock's
    layout.

    If \a pos is less than zero, it will default to the start of the BOBUIextFragment. If \a len
    is less than zero, it will default to the length of the fragment.

    \sa QGlyphRun, BOBUIextBlock::layout(), BOBUIextLayout::position(), QPainter::drawGlyphRun()
*/
#if !defined(BOBUI_NO_RAWFONT)
QList<QGlyphRun> BOBUIextFragment::glyphRuns(int pos, int len) const
{
    if (!p || !n)
        return QList<QGlyphRun>();

    int blockNode = p->blockMap().findNode(position());

    const BOBUIextBlockData *blockData = p->blockMap().fragment(blockNode);
    BOBUIextLayout *layout = blockData->layout;

    int blockPosition = p->blockMap().position(blockNode);
    if (pos < 0)
        pos = position() - blockPosition;
    if (len < 0)
        len = length();
    if (len == 0)
        return QList<QGlyphRun>();

    QList<QGlyphRun> ret;
    for (int i=0; i<layout->lineCount(); ++i) {
        BOBUIextLine textLine = layout->lineAt(i);
        ret += textLine.glyphRuns(pos, len);
    }

    return ret;
}
#endif // BOBUI_NO_RAWFONT

/*!
    Returns the position of this text fragment in the document.
*/
int BOBUIextFragment::position() const
{
    if (!p || !n)
        return 0; // ### -1 instead?

    return p->fragmentMap().position(n);
}

/*!
    Returns the number of characters in the text fragment.

    \sa text()
*/
int BOBUIextFragment::length() const
{
    if (!p || !n)
        return 0;

    int len = 0;
    int f = n;
    while (f != ne) {
        len += p->fragmentMap().size(f);
        f = p->fragmentMap().next(f);
    }
    return len;
}

/*!
    Returns \c true if the text fragment contains the text at the given
    \a position in the document; otherwise returns \c false.
*/
bool BOBUIextFragment::contains(int position) const
{
    if (!p || !n)
        return false;
    int pos = this->position();
    return position >= pos && position < pos + length();
}

/*!
    Returns the text fragment's character format.

    \sa text()
*/
BOBUIextCharFormat BOBUIextFragment::charFormat() const
{
    if (!p || !n)
        return BOBUIextCharFormat();
    const BOBUIextFragmentData *data = p->fragmentMap().fragment(n);
    return p->formatCollection()->charFormat(data->format);
}

/*!
    Returns an index into the document's internal list of character formats
    for the text fragment's character format.

    \sa BOBUIextDocument::allFormats()
*/
int BOBUIextFragment::charFormatIndex() const
{
    if (!p || !n)
        return -1;
    const BOBUIextFragmentData *data = p->fragmentMap().fragment(n);
    return data->format;
}

/*!
    Returns the text fragment's as plain text.

    \sa length(), charFormat()
*/
QString BOBUIextFragment::text() const
{
    if (!p || !n)
        return QString();

    QString result;
    QString buffer = p->buffer();
    int f = n;
    while (f != ne) {
        const BOBUIextFragmentData * const frag = p->fragmentMap().fragment(f);
        result += QString(buffer.constData() + frag->stringPosition, frag->size_array[0]);
        f = p->fragmentMap().next(f);
    }
    return result;
}

BOBUI_END_NAMESPACE

#include "moc_bobuiextobject.cpp"
