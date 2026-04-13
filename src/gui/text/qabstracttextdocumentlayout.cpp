// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qabstracttextdocumentlayout.h>
#include <bobuiextformat.h>
#include "bobuiextdocument_p.h"
#include "bobuiextengine_p.h"
#include "bobuiextlist.h"

#include "qabstracttextdocumentlayout_p.h"

BOBUI_BEGIN_NAMESPACE

QAbstractTextDocumentLayoutPrivate::~QAbstractTextDocumentLayoutPrivate()
{
}

BOBUIextObjectInterface::~BOBUIextObjectInterface()
{
}

/*!
    \class QAbstractTextDocumentLayout
    \reentrant

    \brief The QAbstractTextDocumentLayout class is an abstract base
    class used to implement custom layouts for BOBUIextDocuments.
    \inmodule BobUIGui

    \ingroup richtext-processing

    The standard layout provided by BobUI can handle simple word processing
    including inline images, lists and tables.

    Some applications, e.g., a word processor or a DTP application might need
    more features than the ones provided by BobUI's layout engine, in which case
    you can subclass QAbstractTextDocumentLayout to provide custom layout
    behavior for your text documents.

    An instance of the QAbstractTextDocumentLayout subclass can be installed
    on a BOBUIextDocument object with the
    \l{BOBUIextDocument::}{setDocumentLayout()} function.

    You can insert custom objects into a BOBUIextDocument; see the
    BOBUIextObjectInterface class description for details.

    \sa BOBUIextObjectInterface
*/

/*!
    \class BOBUIextObjectInterface
    \brief The BOBUIextObjectInterface class allows drawing of
           custom text objects in \l{BOBUIextDocument}s.
    \since 4.5
    \inmodule BobUIGui

    A text object describes the structure of one or more elements in a
    text document; for instance, images imported from HTML are
    implemented using text objects. A text object knows how to lay out
    and draw its elements when a document is being rendered.

    BobUI allows custom text objects to be inserted into a document by
    registering a custom \l{BOBUIextCharFormat::objectType()}{object
    type} with BOBUIextCharFormat. A BOBUIextObjectInterface must also be
    implemented for this type and be
    \l{QAbstractTextDocumentLayout::registerHandler()}{registered}
    with the QAbstractTextDocumentLayout of the document. When the
    object type is encountered while rendering a BOBUIextDocument, the
    intrinsicSize() and drawObject() functions of the interface are
    called.

    The following list explains the required steps of inserting a
    custom text object into a document:

    \list
        \li Choose an \a objectType. The \a objectType is an integer with a
            value greater or equal to BOBUIextFormat::UserObject.
         \li Create a BOBUIextCharFormat object and set the object type to the
            chosen type using the setObjectType() function.
         \li Implement the BOBUIextObjectInterface class.
         \li Call QAbstractTextDocumentLayout::registerHandler() with an instance of your
            BOBUIextObjectInterface subclass to register your object type.
         \li Insert QChar::ObjectReplacementCharacter with the aforementioned
            BOBUIextCharFormat of the chosen object type into the document.
            As mentioned, the functions of BOBUIextObjectInterface
            \l{BOBUIextObjectInterface::}{intrinsicSize()} and
            \l{BOBUIextObjectInterface::}{drawObject()} will then be called with the
            BOBUIextFormat as parameter whenever the replacement character is
            encountered.
    \endlist

    A class implementing a text object needs to inherit both QObject
    and BOBUIextObjectInterface. QObject must be the first class
    inherited. For instance:

    \snippet bobuiextobject/textobjectinterface.h 0

    The data of a text object is usually stored in the BOBUIextCharFormat
    using BOBUIextCharFormat::setProperty(), and then retrieved with
    BOBUIextCharFormat::property().

    \warning Copy and Paste operations ignore custom text objects.

    \sa BOBUIextCharFormat, BOBUIextLayout
*/

/*!
    \fn BOBUIextObjectInterface::~BOBUIextObjectInterface()

    Destroys this BOBUIextObjectInterface.
*/

/*!
    \fn virtual QSizeF BOBUIextObjectInterface::intrinsicSize(BOBUIextDocument *doc, int posInDocument, const BOBUIextFormat &format) = 0

    The intrinsicSize() function returns the size of the text object
    represented by \a format in the given document (\a doc) at the
    given position (\a posInDocument).

    The size calculated will be used for subsequent calls to
    drawObject() for this \a format.

    \sa drawObject()
*/

/*!
    \fn virtual void BOBUIextObjectInterface::drawObject(QPainter *painter, const QRectF &rect, BOBUIextDocument *doc, int posInDocument, const BOBUIextFormat &format) = 0

    Draws this text object using the specified \a painter.

    The size of the rectangle, \a rect, to draw in is the size
    previously calculated by intrinsicSize(). The rectangles position
    is relative to the \a painter.

    You also get the document (\a doc) and the position (\a
    posInDocument) of the \a format in that document.

    \sa intrinsicSize()
*/

/*!
    \fn void QAbstractTextDocumentLayout::update(const QRectF &rect)

    This signal is emitted when the rectangle \a rect has been updated.

    Subclasses of QAbstractTextDocumentLayout should emit this signal when
    the layout of the contents change in order to repaint.
*/

/*!
   \fn void QAbstractTextDocumentLayout::updateBlock(const BOBUIextBlock &block)
   \since 4.4

   This signal is emitted when the specified \a block has been updated.

   Subclasses of QAbstractTextDocumentLayout should emit this signal when
   the layout of \a block has changed in order to repaint.
*/

/*!
    \fn void QAbstractTextDocumentLayout::documentSizeChanged(const QSizeF &newSize)

    This signal is emitted when the size of the document layout changes to
    \a newSize.

    Subclasses of QAbstractTextDocumentLayout should emit this signal when the
    document's entire layout size changes. This signal is useful for widgets
    that display text documents since it enables them to update their scroll
    bars correctly.

    \sa documentSize()
*/

/*!
    \fn void QAbstractTextDocumentLayout::pageCountChanged(int newPages)

    This signal is emitted when the number of pages in the layout changes;
    \a newPages is the updated page count.

    Subclasses of QAbstractTextDocumentLayout should emit this signal when
    the number of pages in the layout has changed. Changes to the page count
    are caused by changes to the layout or the document content itself.

    \sa pageCount()
*/

/*!
    \fn int QAbstractTextDocumentLayout::pageCount() const

    Returns the number of pages contained in the layout.

    \sa pageCountChanged()
*/

/*!
    \fn QSizeF QAbstractTextDocumentLayout::documentSize() const

    Returns the total size of the document's layout.

    This information can be used by display widgets to update their scroll bars
    correctly.

    \sa documentSizeChanged(), BOBUIextDocument::pageSize
*/

/*!
    \fn void QAbstractTextDocumentLayout::draw(QPainter *painter, const PaintContext &context)

    Draws the layout with the given \a painter using the given \a context.
*/

/*!
    \fn int QAbstractTextDocumentLayout::hitTest(const QPointF &point, BobUI::HitTestAccuracy accuracy) const

    Returns the cursor position for the given \a point with the specified
    \a accuracy. Returns -1 if no valid cursor position was found.
*/

/*!
    \fn void QAbstractTextDocumentLayout::documentChanged(int position, int charsRemoved, int charsAdded)

    This function is called whenever the contents of the document change. A
    change occurs when text is inserted, removed, or a combination of these
    two. The change is specified by \a position, \a charsRemoved, and
    \a charsAdded corresponding to the starting character position of the
    change, the number of characters removed from the document, and the
    number of characters added.

    For example, when inserting the text "Hello" into an empty document,
    \a charsRemoved would be 0 and \a charsAdded would be 5 (the length of
    the string).

    Replacing text is a combination of removing and inserting. For example, if
    the text "Hello" gets replaced by "Hi", \a charsRemoved would be 5 and
    \a charsAdded would be 2.

    For subclasses of QAbstractTextDocumentLayout, this is the central function
    where a large portion of the work to lay out and position document contents
    is done.

    For example, in a subclass that only arranges blocks of text, an
    implementation of this function would have to do the following:

    \list
        \li Determine the list of changed \l{BOBUIextBlock}(s) using the parameters
            provided.
        \li Each BOBUIextBlock object's corresponding BOBUIextLayout object needs to
            be processed. You can access the \l{BOBUIextBlock}'s layout using the
            BOBUIextBlock::layout() function. This processing should take the
            document's page size into consideration.
        \li If the total number of pages changed, the pageCountChanged() signal
            should be emitted.
        \li If the total size changed, the documentSizeChanged() signal should
            be emitted.
        \li The update() signal should be emitted to schedule a repaint of areas
            in the layout that require repainting.
    \endlist

    \sa BOBUIextLayout
*/

/*!
    \class QAbstractTextDocumentLayout::PaintContext
    \reentrant
    \inmodule BobUIGui

    \brief The QAbstractTextDocumentLayout::PaintContext class is a convenience
    class defining the parameters used when painting a document's layout.

    A paint context is used when rendering custom layouts for BOBUIextDocuments
    with the QAbstractTextDocumentLayout::draw() function. It is specified by
    a \l {cursorPosition}{cursor position}, \l {palette}{default text color},
    \l clip rectangle and a collection of \l selections.

    \sa QAbstractTextDocumentLayout
*/

/*!
    \fn QAbstractTextDocumentLayout::PaintContext::PaintContext()
    \internal
*/

/*!
    \variable QAbstractTextDocumentLayout::PaintContext::cursorPosition

    \brief the position within the document, where the cursor line should be
    drawn.

    The default value is -1.
*/

/*!
    \variable QAbstractTextDocumentLayout::PaintContext::palette

    \brief the default color that is used for the text, when no color is
    specified.

    The default value is the application's default palette.
*/

/*!
    \variable QAbstractTextDocumentLayout::PaintContext::clip

    \brief a hint to the layout specifying the area around paragraphs, frames
    or text require painting.

    Everything outside of this rectangle does not need to be painted.

    Specifying a clip rectangle can speed up drawing of large documents
    significantly. Note that the clip rectangle is in document coordinates (not
    in viewport coordinates). It is not a substitute for a clip region set on
    the painter but merely a hint.

    The default value is a null rectangle indicating everything needs to be
    painted.
*/

/*!
    \variable QAbstractTextDocumentLayout::PaintContext::selections

    \brief the collection of selections that will be rendered when passing this
    paint context to QAbstractTextDocumentLayout's draw() function.

    The default value is an empty list indicating no selection.
*/

/*!
    \class QAbstractTextDocumentLayout::Selection
    \reentrant
    \inmodule BobUIGui

    \brief The QAbstractTextDocumentLayout::Selection class is a convenience
    class defining the parameters of a selection.

    A selection can be used to specify a part of a document that should be
    highlighted when drawing custom layouts for BOBUIextDocuments with the
    QAbstractTextDocumentLayout::draw() function. It is specified using
    \l cursor and a \l format.

    \sa QAbstractTextDocumentLayout, PaintContext
*/

/*!
    \variable QAbstractTextDocumentLayout::Selection::format

    \brief the format of the selection

    The default value is BOBUIextFormat::InvalidFormat.
*/

/*!
    \variable QAbstractTextDocumentLayout::Selection::cursor
    \brief the selection's cursor

    The default value is a null cursor.
*/

/*!
    Creates a new text document layout for the given \a document.
*/
QAbstractTextDocumentLayout::QAbstractTextDocumentLayout(BOBUIextDocument *document)
    : QObject(*new QAbstractTextDocumentLayoutPrivate, document)
{
    Q_D(QAbstractTextDocumentLayout);
    d->setDocument(document);
}

/*!
    \internal
*/
QAbstractTextDocumentLayout::QAbstractTextDocumentLayout(QAbstractTextDocumentLayoutPrivate &p, BOBUIextDocument *document)
    :QObject(p, document)
{
    Q_D(QAbstractTextDocumentLayout);
    d->setDocument(document);
}

/*!
    \internal
*/
QAbstractTextDocumentLayout::~QAbstractTextDocumentLayout()
{
}

/*!
    Registers the given \a component as a handler for items of the given \a objectType.

    \note registerHandler() has to be called once for each object type. This
    means that there is only one handler for multiple replacement characters
    of the same object type.

    The text document layout does not take ownership of \c component.
*/
void QAbstractTextDocumentLayout::registerHandler(int objectType, QObject *component)
{
    Q_D(QAbstractTextDocumentLayout);

    BOBUIextObjectInterface *iface = qobject_cast<BOBUIextObjectInterface *>(component);
    if (!iface)
        return; // ### print error message on terminal?

    QObjectPrivate::connect(component, &QObject::destroyed, d,
                            &QAbstractTextDocumentLayoutPrivate::_q_handlerDestroyed);

    BOBUIextObjectHandler h;
    h.iface = iface;
    h.component = component;
    d->handlers.insert(objectType, h);
}

/*!
    \since 5.2

    Unregisters the given \a component as a handler for items of the given \a objectType, or
    any handler if the \a component is not specified.
*/
void QAbstractTextDocumentLayout::unregisterHandler(int objectType, QObject *component)
{
    Q_D(QAbstractTextDocumentLayout);

    const auto it = d->handlers.constFind(objectType);
    if (it != d->handlers.cend() && (!component || component == it->component)) {
        if (component)
            QObjectPrivate::disconnect(component, &QObject::destroyed, d,
                                       &QAbstractTextDocumentLayoutPrivate::_q_handlerDestroyed);
        d->handlers.erase(it);
    }
}

/*!
    Returns a handler for objects of the given \a objectType.
*/
BOBUIextObjectInterface *QAbstractTextDocumentLayout::handlerForObject(int objectType) const
{
    Q_D(const QAbstractTextDocumentLayout);

    BOBUIextObjectHandler handler = d->handlers.value(objectType);
    if (!handler.component)
        return nullptr;

    return handler.iface;
}

/*!
    Sets the size of the inline object \a item corresponding to the text
    \a format.

    \a posInDocument specifies the position of the object within the document.

    The default implementation resizes the \a item to the size returned by
    the object handler's intrinsicSize() function. This function is called only
    within BobUI. Subclasses can reimplement this function to customize the
    resizing of inline objects.
*/
void QAbstractTextDocumentLayout::resizeInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format)
{
    Q_D(QAbstractTextDocumentLayout);

    BOBUIextCharFormat f = format.toCharFormat();
    Q_ASSERT(f.isValid());
    BOBUIextObjectHandler handler = d->handlers.value(f.objectType());
    if (!handler.component)
        return;

    QSizeF s = handler.iface->intrinsicSize(document(), posInDocument, format);
    item.setWidth(s.width());
    item.setAscent(s.height());
    item.setDescent(0);
}

/*!
    Lays out the inline object \a item using the given text \a format.

    \a posInDocument specifies the position of the object within the document.

    The default implementation does nothing. This function is called only
    within BobUI. Subclasses can reimplement this function to customize the
    position of inline objects.

    \sa drawInlineObject()
*/
void QAbstractTextDocumentLayout::positionInlineObject(BOBUIextInlineObject item, int posInDocument, const BOBUIextFormat &format)
{
    Q_UNUSED(item);
    Q_UNUSED(posInDocument);
    Q_UNUSED(format);
}

/*!
    \fn void QAbstractTextDocumentLayout::drawInlineObject(QPainter *painter, const QRectF &rect, BOBUIextInlineObject object, int posInDocument, const BOBUIextFormat &format)

    This function is called to draw the inline object, \a object, with the
    given \a painter within the rectangle specified by \a rect using the
    specified text \a format.

    \a posInDocument specifies the position of the object within the document.

    The default implementation calls drawObject() on the object handlers. This
    function is called only within BobUI. Subclasses can reimplement this function
    to customize the drawing of inline objects.

    \sa draw()
*/
void QAbstractTextDocumentLayout::drawInlineObject(QPainter *p, const QRectF &rect, BOBUIextInlineObject item,
                                                   int posInDocument, const BOBUIextFormat &format)
{
    Q_UNUSED(item);
    Q_D(QAbstractTextDocumentLayout);

    BOBUIextCharFormat f = format.toCharFormat();
    Q_ASSERT(f.isValid());
    BOBUIextObjectHandler handler = d->handlers.value(f.objectType());
    if (!handler.component)
        return;

    handler.iface->drawObject(p, rect, document(), posInDocument, format);
}

void QAbstractTextDocumentLayoutPrivate::_q_handlerDestroyed(QObject *obj)
{
    HandlerHash::Iterator it = handlers.begin();
    while (it != handlers.end())
        if ((*it).component == obj)
            it = handlers.erase(it);
        else
            ++it;
}

/*!
    \internal

    Returns the index of the format at position \a pos.
*/
int QAbstractTextDocumentLayout::formatIndex(int pos)
{
    BOBUIextDocumentPrivate *pieceTable = BOBUIextDocumentPrivate::get(qobject_cast<BOBUIextDocument *>(parent()));
    return pieceTable->find(pos).value()->format;
}

/*!
    \fn BOBUIextCharFormat QAbstractTextDocumentLayout::format(int position)

    Returns the character format that is applicable at the given \a position.
*/
BOBUIextCharFormat QAbstractTextDocumentLayout::format(int pos)
{
    BOBUIextDocumentPrivate *pieceTable = BOBUIextDocumentPrivate::get(qobject_cast<BOBUIextDocument *>(parent()));
    int idx = pieceTable->find(pos).value()->format;
    return pieceTable->formatCollection()->charFormat(idx);
}



/*!
    Returns the text document that this layout is operating on.
*/
BOBUIextDocument *QAbstractTextDocumentLayout::document() const
{
    Q_D(const QAbstractTextDocumentLayout);
    return d->document;
}

/*!
    \fn QString QAbstractTextDocumentLayout::anchorAt(const QPointF &position) const

    Returns the reference of the anchor the given \a position, or an empty
    string if no anchor exists at that point.
*/
QString QAbstractTextDocumentLayout::anchorAt(const QPointF& pos) const
{
    BOBUIextCharFormat fmt = formatAt(pos).toCharFormat();
    return fmt.anchorHref();
}

/*!
    \since 5.8

    Returns the source of the image at the given position \a pos, or an empty
    string if no image exists at that point.
*/
QString QAbstractTextDocumentLayout::imageAt(const QPointF &pos) const
{
    BOBUIextImageFormat fmt = formatAt(pos).toImageFormat();
    return fmt.name();
}

/*!
    \since 5.8

    Returns the text format at the given position \a pos.
*/
BOBUIextFormat QAbstractTextDocumentLayout::formatAt(const QPointF &pos) const
{
    int cursorPos = hitTest(pos, BobUI::ExactHit);
    if (cursorPos == -1)
        return BOBUIextFormat();

    // compensate for preedit in the hit text block
    BOBUIextBlock block = document()->firstBlock();
    while (block.isValid()) {
        QRectF blockBr = blockBoundingRect(block);
        if (blockBr.contains(pos)) {
            BOBUIextLayout *layout = block.layout();
            int relativeCursorPos = cursorPos - block.position();
            const int preeditLength = layout ? layout->preeditAreaText().size() : 0;
            if (preeditLength > 0 && relativeCursorPos > layout->preeditAreaPosition())
                cursorPos -= qMin(cursorPos - layout->preeditAreaPosition(), preeditLength);
            break;
        }
        block = block.next();
    }

    const BOBUIextDocumentPrivate *pieceTable = BOBUIextDocumentPrivate::get(qobject_cast<const BOBUIextDocument *>(parent()));
    BOBUIextDocumentPrivate::FragmentIterator it = pieceTable->find(cursorPos);
    return pieceTable->formatCollection()->format(it->format);
}

/*!
    \since 5.14

    Returns the block (probably a list item) whose \l{BOBUIextBlockFormat::marker()}{marker}
    is found at the given position \a pos.
*/
BOBUIextBlock QAbstractTextDocumentLayout::blockWithMarkerAt(const QPointF &pos) const
{
    BOBUIextBlock block = document()->firstBlock();
    while (block.isValid()) {
        if (block.blockFormat().marker() != BOBUIextBlockFormat::MarkerType::NoMarker) {
            QRectF blockBr = blockBoundingRect(block);
            BOBUIextBlockFormat blockFmt = block.blockFormat();
            QFontMetrics fm(block.charFormat().font());
            qreal totalIndent = blockFmt.indent() + blockFmt.leftMargin() + blockFmt.textIndent();
            if (block.textList())
                totalIndent += block.textList()->format().indent() * 40;
            QRectF adjustedBr = blockBr.adjusted(totalIndent - fm.height(), 0, totalIndent - blockBr.width(), fm.height() - blockBr.height());
            if (adjustedBr.contains(pos)) {
                //qDebug() << "hit block" << block.text() << blockBr << adjustedBr << "marker" << block.blockFormat().marker()
                //         << "font" << block.charFormat().font() << "adj" << lineHeight << totalIndent;
                if (block.blockFormat().hasProperty(BOBUIextFormat::BlockMarker))
                    return block;
            }
        }
        block = block.next();
    }
    return BOBUIextBlock();
}

/*!
    \fn QRectF QAbstractTextDocumentLayout::frameBoundingRect(BOBUIextFrame *frame) const

    Returns the bounding rectangle of \a frame.
*/

/*!
    \fn QRectF QAbstractTextDocumentLayout::blockBoundingRect(const BOBUIextBlock &block) const

    Returns the bounding rectangle of \a block.
*/

/*!
    Sets the paint device used for rendering the document's layout to the given
    \a device.

    \sa paintDevice()
*/
void QAbstractTextDocumentLayout::setPaintDevice(QPaintDevice *device)
{
    Q_D(QAbstractTextDocumentLayout);
    d->paintDevice = device;
}

/*!
    Returns the paint device used to render the document's layout.

    \sa setPaintDevice()
*/
QPaintDevice *QAbstractTextDocumentLayout::paintDevice() const
{
    Q_D(const QAbstractTextDocumentLayout);
    return d->paintDevice;
}

BOBUI_END_NAMESPACE

#include "moc_qabstracttextdocumentlayout.cpp"
