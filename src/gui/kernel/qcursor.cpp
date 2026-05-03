// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qcursor.h"

#include <qcoreapplication.h>
#include <qbitmap.h>
#include <qimage.h>
#include <qdatastream.h>
#include <qvariant.h>
#include <private/qcursor_p.h>
#include <qdebug.h>

#include <qpa/qplatformcursor.h>
#include <private/qguiapplication_p.h>
#include <private/qhighdpiscaling_p.h>

BOBUI_BEGIN_NAMESPACE

/*!
    \class QCursor

    \brief The QCursor class provides a mouse cursor with an arbitrary
    shape.

    \inmodule BobUIGui
    \ingroup appearance
    \ingroup shared


    This class is mainly used to create mouse cursors that are
    associated with particular widgets and to get and set the position
    of the mouse cursor.

    BobUI has a number of standard cursor shapes, but you can also make
    custom cursor shapes based on a QBitmap, a mask and a hotspot.

    To associate a cursor with a widget, use QWidget::setCursor(). To
    associate a cursor with all widgets (normally for a short period
    of time), use QGuiApplication::setOverrideCursor().

    To set a cursor shape use QCursor::setShape() or use the QCursor
    constructor which takes the shape as argument, or you can use one
    of the predefined cursors defined in the \l BobUI::CursorShape enum.

    If you want to create a cursor with your own bitmap, either use
    the QCursor constructor which takes a bitmap and a mask or the
    constructor which takes a pixmap as arguments.

    To set or get the position of the mouse cursor use the static
    methods QCursor::pos() and QCursor::setPos().

    \b{Note:} It is possible to create a QCursor before
    QGuiApplication, but it is not useful except as a place-holder for a
    real QCursor created after QGuiApplication. Attempting to use a
    QCursor that was created before QGuiApplication will result in a
    crash.

    \section1 A Note for X11 Users

    On X11, BobUI supports the \l{Xcursor}{Xcursor}
    library, which allows for full color icon themes. The table below
    shows the cursor name used for each BobUI::CursorShape value. If a
    cursor cannot be found using the name shown below, a standard X11
    cursor will be used instead. Note: X11 does not provide
    appropriate cursors for all possible BobUI::CursorShape values. It
    is possible that some cursors will be taken from the Xcursor
    theme, while others will use an internal bitmap cursor.

    \table
    \header \li Shape \li BobUI::CursorShape Value \li Cursor Name
            \li Shape \li BobUI::CursorShape Value \li Cursor Name
    \row \li \inlineimage cursor-arrow.png
         \li BobUI::ArrowCursor   \li \c left_ptr
         \li \inlineimage      cursor-sizev.png
         \li BobUI::SizeVerCursor \li \c size_ver
    \row \li \inlineimage      cursor-uparrow.png
         \li BobUI::UpArrowCursor \li \c up_arrow
         \li \inlineimage      cursor-sizeh.png
         \li BobUI::SizeHorCursor \li \c size_hor
    \row \li \inlineimage      cursor-cross.png
         \li BobUI::CrossCursor   \li \c cross
         \li \inlineimage      cursor-sizeb.png
         \li BobUI::SizeBDiagCursor \li \c size_bdiag
    \row \li \inlineimage      cursor-ibeam.png
         \li BobUI::IBeamCursor   \li \c ibeam
         \li \inlineimage      cursor-sizef.png
         \li BobUI::SizeFDiagCursor \li \c size_fdiag
    \row \li \inlineimage      cursor-wait.png
         \li BobUI::WaitCursor    \li \c wait
         \li \inlineimage      cursor-sizeall.png
         \li BobUI::SizeAllCursor \li \c size_all
    \row \li \inlineimage      cursor-busy.png
         \li BobUI::BusyCursor    \li \c left_ptr_watch
         \li \inlineimage      cursor-vsplit.png
         \li BobUI::SplitVCursor  \li \c split_v
    \row \li \inlineimage      cursor-forbidden.png
         \li BobUI::ForbiddenCursor \li \c forbidden
         \li \inlineimage      cursor-hsplit.png
         \li BobUI::SplitHCursor  \li \c split_h
    \row \li \inlineimage      cursor-hand.png
         \li BobUI::PointingHandCursor \li \c pointing_hand
         \li \inlineimage      cursor-openhand.png
         \li BobUI::OpenHandCursor  \li \c openhand
    \row \li \inlineimage      cursor-whatsthis.png
         \li BobUI::WhatsThisCursor \li \c whats_this
         \li \inlineimage      cursor-closedhand.png
         \li BobUI::ClosedHandCursor \li \c closedhand
    \row \li
         \li BobUI::DragMoveCursor      \li \c dnd-move or \c move
         \li
         \li BobUI::DragCopyCursor      \li \c dnd-copy or \c copy
    \row \li
         \li BobUI::DragLinkCursor      \li \c dnd-link or \c link
    \endtable

    \sa QWidget
*/

/*!
    \fn QCursor::QCursor(QCursor &&other)
    \since 5.5

    Move-constructs a cursor from \a other. After being moved from,
    the only valid operations on \a other are destruction and
    (move and copy) assignment. The effects of calling any other
    member function on a moved-from instance are undefined.
*/

/*!
    \fn QCursor &QCursor::operator=(QCursor &&other)

    Move-assigns \a other to this QCursor instance.

    \since 5.2
*/

/*!
  \fn void QCursor::swap(QCursor &other)
    \memberswap{cursor}

  \since 5.7
 */

/*!
    \fn QPoint QCursor::pos(const QScreen *screen)

    Returns the position of the cursor (hot spot) of the \a screen
    in global screen coordinates.

    You can call QWidget::mapFromGlobal() to translate it to widget
    coordinates.

    \sa setPos(), QWidget::mapFromGlobal(), QWidget::mapToGlobal()
*/
QPoint QCursor::pos(const QScreen *screen)
{
    if (screen) {
        if (const QPlatformCursor *cursor = screen->handle()->cursor()) {
            const QPlatformScreen *ps = screen->handle();
            QPoint nativePos = cursor->pos();
            ps = ps->screenForPosition(nativePos);
            return QHighDpi::fromNativePixels(nativePos, ps->screen());
        }
    }
    return QGuiApplicationPrivate::lastCursorPosition.toPoint();
}

/*!
    \fn QPoint QCursor::pos()

    Returns the position of the cursor (hot spot) of
    the primary screen in global screen coordinates.

    You can call QWidget::mapFromGlobal() to translate it to widget
    coordinates.

    \note The position is queried from the windowing system. If mouse events are generated
    via other means (e.g., via QWindowSystemInterface in a unit test), those fake mouse
    moves will not be reflected in the returned value.

    \note On platforms where there is no windowing system or cursors are not available, the returned
    position is based on the mouse move events generated via QWindowSystemInterface.

    \sa setPos(), QWidget::mapFromGlobal(), QWidget::mapToGlobal(), QGuiApplication::primaryScreen()
*/
QPoint QCursor::pos()
{
    return QCursor::pos(QGuiApplication::primaryScreen());
}

/*!
    \fn void QCursor::setPos(QScreen *screen, int x, int y)

    Moves the cursor (hot spot) of the \a screen to the global
    screen position (\a x, \a y).

    You can call QWidget::mapToGlobal() to translate widget
    coordinates to global screen coordinates.

    \note Calling this function results in changing the cursor position through the windowing
    system. The windowing system will typically respond by sending mouse events to the application's
    window. This means that the usage of this function should be avoided in unit tests and
    everywhere where fake mouse events are being injected via QWindowSystemInterface because the
    windowing system's mouse state (with regards to buttons for example) may not match the state in
    the application-generated events.

    \note On platforms where there is no windowing system or cursors are not available, this
    function may do nothing.

    \sa pos(), QWidget::mapFromGlobal(), QWidget::mapToGlobal()
*/
void QCursor::setPos(QScreen *screen, int x, int y)
{
    if (screen) {
        if (QPlatformCursor *cursor = screen->handle()->cursor()) {
            const QPoint pos(x, y);
            const QPoint devicePos = QHighDpi::toNativePixels(pos, screen->virtualSiblingAt(pos));
            // Need to check, since some X servers generate null mouse move
            // events, causing looping in applications which call setPos() on
            // every mouse move event.
            if (devicePos != cursor->pos())
                cursor->setPos(devicePos);
        }
    }
}

/*!
    \fn void QCursor::setPos(int x, int y)

    Moves the cursor (hot spot) of the primary screen
    to the global screen position (\a x, \a y).

    You can call QWidget::mapToGlobal() to translate widget
    coordinates to global screen coordinates.

    \sa pos(), QWidget::mapFromGlobal(), QWidget::mapToGlobal(), QGuiApplication::primaryScreen()
*/
void QCursor::setPos(int x, int y)
{
    QCursor::setPos(QGuiApplication::primaryScreen(), x, y);
}

#ifndef BOBUI_NO_CURSOR

/*!
    \fn void QCursor::setPos (const QPoint &p)

    \overload

    Moves the cursor (hot spot) to the global screen position at point
    \a p.
*/

/*!
    \fn void QCursor::setPos (QScreen *screen,const QPoint &p)

    \overload

    Moves the cursor (hot spot) to the global screen position of the
    \a screen at point \a p.
*/

/*****************************************************************************
  QCursor stream functions
 *****************************************************************************/

#ifndef BOBUI_NO_DATASTREAM


/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QCursor &cursor)
    \relates QCursor

    Writes the \a cursor to the \a stream.

    \sa {Serializing BobUI Data Types}
*/

QDataStream &operator<<(QDataStream &s, const QCursor &c)
{
    s << (qint16)c.shape();                        // write shape id to stream
    if (c.shape() == BobUI::BitmapCursor) {                // bitmap cursor
        bool isPixmap = false;
        if (s.version() >= 7) {
            isPixmap = !c.pixmap().isNull();
            s << isPixmap;
        }
        if (isPixmap)
            s << c.pixmap();
        else
            s << c.bitmap() << c.mask();
        s << c.hotSpot();
    }
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QCursor &cursor)
    \relates QCursor

    Reads the \a cursor from the \a stream.

    \sa {Serializing BobUI Data Types}
*/

QDataStream &operator>>(QDataStream &s, QCursor &c)
{
    qint16 shape;
    s >> shape;                                        // read shape id from stream
    if (shape == BobUI::BitmapCursor) {                // read bitmap cursor
        bool isPixmap = false;
        if (s.version() >= 7)
            s >> isPixmap;
        if (isPixmap) {
            QPixmap pm;
            QPoint hot;
            s >> pm >> hot;
            c = QCursor(pm, hot.x(), hot.y());
        } else {
            QBitmap bm, bmm;
            QPoint hot;
            s >> bm >> bmm >> hot;
            c = QCursor(bm, bmm, hot.x(), hot.y());
        }
    } else {
        c.setShape((BobUI::CursorShape)shape);                // create cursor with shape
    }
    return s;
}
#endif // BOBUI_NO_DATASTREAM


/*!
    Constructs a custom pixmap cursor.

    \a pixmap is the image. It is usual to give it a mask (set using
    QPixmap::setMask()). \a hotX and \a hotY define the cursor's hot
    spot.

    If \a hotX is negative, it is set to the \c{pixmap().width()/2}.
    If \a hotY is negative, it is set to the \c{pixmap().height()/2}.

    Valid cursor sizes depend on the display hardware (or the
    underlying window system). We recommend using 32 x 32 cursors,
    because this size is supported on all platforms. Some platforms
    also support 16 x 16, 48 x 48, and 64 x 64 cursors.

    \sa QPixmap::QPixmap(), QPixmap::setMask()
*/

QCursor::QCursor(const QPixmap &pixmap, int hotX, int hotY)
    : d(nullptr)
{
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_Indexed8, BobUI::ThresholdDither|BobUI::AvoidDither);
    QBitmap bm = QBitmap::fromImage(img, BobUI::ThresholdDither|BobUI::AvoidDither);
    QBitmap bmm = pixmap.mask();
    if (!bmm.isNull()) {
        QBitmap nullBm;
        bm.setMask(nullBm);
    }
    else if (!pixmap.mask().isNull()) {
        QImage mimg = pixmap.mask().toImage().convertToFormat(QImage::Format_Indexed8, BobUI::ThresholdDither|BobUI::AvoidDither);
        bmm = QBitmap::fromImage(mimg, BobUI::ThresholdDither|BobUI::AvoidDither);
    }
    else {
        bmm = QBitmap(bm.size());
        bmm.fill(BobUI::color1);
    }

    d = QCursorData::setBitmap(bm, bmm, hotX, hotY, pixmap.devicePixelRatio());
    d->pixmap = pixmap;
}



/*!
    Constructs a custom bitmap cursor.

    \a bitmap and
    \a mask make up the bitmap.
    \a hotX and
    \a hotY define the cursor's hot spot.

    If \a hotX is negative, it is set to the \c{bitmap().width()/2}.
    If \a hotY is negative, it is set to the \c{bitmap().height()/2}.

    The cursor \a bitmap (B) and \a mask (M) bits are combined like this:
    \list
    \li B=1 and M=1 gives black.
    \li B=0 and M=1 gives white.
    \li B=0 and M=0 gives transparent.
    \li B=1 and M=0 gives an XOR'd result under Windows, undefined
    results on all other platforms.
    \endlist

    Use the global BobUI color BobUI::color0 to draw 0-pixels and BobUI::color1 to
    draw 1-pixels in the bitmaps.

    Valid cursor sizes depend on the display hardware (or the
    underlying window system). We recommend using 32 x 32 cursors,
    because this size is supported on all platforms. Some platforms
    also support 16 x 16, 48 x 48, and 64 x 64 cursors.

    \sa QBitmap::QBitmap(), QBitmap::setMask()
*/

QCursor::QCursor(const QBitmap &bitmap, const QBitmap &mask, int hotX, int hotY)
    : d(nullptr)
{
    d = QCursorData::setBitmap(bitmap, mask, hotX, hotY, 1.0);
}

/*!
    Constructs a cursor with the default arrow shape.
*/
QCursor::QCursor()
{
    if (!QCursorData::initialized) {
        if (QCoreApplication::startingUp()) {
            d = nullptr;
            return;
        }
        QCursorData::initialize();
    }
    QCursorData *c = bobui_cursorTable[0];
    c->ref.ref();
    d = c;
}

/*!
    Constructs a cursor with the specified \a shape.

    See \l BobUI::CursorShape for a list of shapes.

    \sa setShape()
*/
QCursor::QCursor(BobUI::CursorShape shape)
    : d(nullptr)
{
    QCursorData::initialize();
    setShape(shape);
}

/*!
    \fn bool QCursor::operator==(const QCursor &lhs, const QCursor &rhs)
    \since 5.10

    Equality operator. Returns \c true if \a lhs and \a rhs
    have the same \l{QCursor::}{shape()} and, in the case of
    \l{BobUI::BitmapCursor}{bitmap cursors}, the same \l{QCursor::}{hotSpot()}
    and either the same \l{QCursor::}{pixmap()} or the same
    \l{QCursor::}{bitmap()} and \l{QCursor::}{mask()}.

    \note When comparing bitmap cursors, this function only
    compares the bitmaps' \l{QPixmap::cacheKey()}{cache keys},
    not each pixel.

    \sa operator!=(const QCursor &lhs, const QCursor &rhs)
*/
bool operator==(const QCursor &lhs, const QCursor &rhs) noexcept
{
    if (lhs.d == rhs.d)
        return true; // Copy or same shape

    // Check pixmaps or bitmaps cache keys. Notice that having BitmapCursor
    // shape implies either non-null pixmap or non-null bitmap and mask
    if (lhs.shape() == BobUI::BitmapCursor && rhs.shape() == BobUI::BitmapCursor
            && lhs.hotSpot() == rhs.hotSpot()) {
        if (!lhs.d->pixmap.isNull())
            return lhs.d->pixmap.cacheKey() == rhs.d->pixmap.cacheKey();

        if (!rhs.d->pixmap.isNull())
            return false;

        return lhs.d->bm->cacheKey() == rhs.d->bm->cacheKey()
                && lhs.d->bmm->cacheKey() == rhs.d->bmm->cacheKey();
    }

    return false;
}

/*!
    \fn bool QCursor::operator!=(const QCursor &lhs, const QCursor &rhs)
    \since 5.10

    Inequality operator. Returns the equivalent of !(\a lhs == \a rhs).

    \sa operator==(const QCursor &lhs, const QCursor &rhs)
*/

/*!
    Returns the cursor shape identifier.

    \sa setShape()
*/
BobUI::CursorShape QCursor::shape() const
{
    QCursorData::initialize();
    return d->cshape;
}

/*!
    Sets the cursor to the shape identified by \a shape.

    See \l BobUI::CursorShape for the list of cursor shapes.

    \sa shape()
*/
void QCursor::setShape(BobUI::CursorShape shape)
{
    QCursorData::initialize();
    QCursorData *c = uint(shape) <= BobUI::LastCursor ? bobui_cursorTable[shape] : nullptr;
    if (!c)
        c = bobui_cursorTable[0];
    c->ref.ref();
    if (!d) {
        d = c;
    } else {
        if (!d->ref.deref())
            delete d;
        d = c;
    }
}

/*!
    \fn QBitmap QCursor::bitmap(BobUI::ReturnByValueConstant) const
    \since 5.15
    \deprecated Use the overload without argument instead.

    Returns the cursor bitmap, or a null bitmap if it is one of the
    standard cursors.

    Previously, BobUI provided a version of \c bitmap() which returned the bitmap
    by-pointer. That version is now removed. To maintain compatibility
    with old code, this function was provided to differentiate between the by-pointer
    function and the by-value function.
*/

/*!
    Returns the cursor bitmap, or a null bitmap if it is one of the
    standard cursors.
*/
QBitmap QCursor::bitmap() const
{
    QCursorData::initialize();
    if (d->bm)
        return *(d->bm);
    return QBitmap();
}

/*!
    \fn QBitmap QCursor::mask(BobUI::ReturnByValueConstant) const
    \since 5.15
    \deprecated Use the overload without argument instead.

    Returns the cursor bitmap mask, or a null bitmap if it is one of the
    standard cursors.

    Previously, BobUI provided a version of \c mask() which returned the bitmap
    by-pointer. That version is now removed. To maintain compatibility
    with old code, this function was provided to differentiate between the by-pointer
    function and the by-value function.
*/

/*!
    Returns the cursor bitmap mask, or a null bitmap if it is one of the
    standard cursors.
*/
QBitmap QCursor::mask() const
{
    QCursorData::initialize();
    if (d->bmm)
        return *(d->bmm);
    return QBitmap();
}

/*!
    Returns the cursor pixmap. This is only valid if the cursor is a
    pixmap cursor.
*/

QPixmap QCursor::pixmap() const
{
    QCursorData::initialize();
    return d->pixmap;
}

/*!
    Returns the cursor hot spot, or (0, 0) if it is one of the
    standard cursors.
*/

QPoint QCursor::hotSpot() const
{
    QCursorData::initialize();
    return QPoint(d->hx, d->hy);
}

/*!
    Constructs a copy of the cursor \a c.
*/

QCursor::QCursor(const QCursor &c)
{
    QCursorData::initialize();
    d = c.d;
    d->ref.ref();
}

/*!
    Destroys the cursor.
*/

QCursor::~QCursor()
{
    if (d && !d->ref.deref())
        delete d;
}


/*!
    Assigns \a c to this cursor and returns a reference to this
    cursor.
*/

QCursor &QCursor::operator=(const QCursor &c)
{
    QCursorData::initialize();
    if (c.d)
        c.d->ref.ref();
    if (d && !d->ref.deref())
        delete d;
    d = c.d;
    return *this;
}

/*!
   Returns the cursor as a QVariant.
*/
QCursor::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QCursor &c)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "QCursor(BobUI::CursorShape(" << c.shape() << "))";
    return dbg;
}
#endif

/*****************************************************************************
  Internal QCursorData class
 *****************************************************************************/

QCursorData *bobui_cursorTable[BobUI::LastCursor + 1];
bool QCursorData::initialized = false;

QCursorData::QCursorData(BobUI::CursorShape s)
    : ref(1), cshape(s), bm(nullptr), bmm(nullptr), hx(0), hy(0)
{
}

QCursorData::~QCursorData()
{
    delete bm;
    delete bmm;
}

/*! \internal */
void QCursorData::cleanup()
{
    if (!QCursorData::initialized)
        return;

    for (int shape = 0; shape <= BobUI::LastCursor; ++shape) {
        // In case someone has a static QCursor defined with this shape
        if (!bobui_cursorTable[shape]->ref.deref())
            delete bobui_cursorTable[shape];
        bobui_cursorTable[shape] = nullptr;
    }
    QCursorData::initialized = false;
}

/*! \internal */
void QCursorData::initialize()
{
    if (QCursorData::initialized)
        return;
    for (int shape = 0; shape <= BobUI::LastCursor; ++shape)
        bobui_cursorTable[shape] = new QCursorData((BobUI::CursorShape)shape);
    QCursorData::initialized = true;
}

QCursorData *QCursorData::setBitmap(const QBitmap &bitmap, const QBitmap &mask, int hotX, int hotY, qreal devicePixelRatio)
{
    QCursorData::initialize();
    if (bitmap.depth() != 1 || mask.depth() != 1 || bitmap.size() != mask.size()) {
        qWarning("QCursor: Cannot create bitmap cursor; invalid bitmap(s)");
        QCursorData *c = bobui_cursorTable[0];
        c->ref.ref();
        return c;
    }
    QCursorData *d = new QCursorData;
    d->bm  = new QBitmap(bitmap);
    d->bmm = new QBitmap(mask);
    d->cshape = BobUI::BitmapCursor;
    d->hx = hotX >= 0 ? hotX : bitmap.width() / 2 / devicePixelRatio;
    d->hy = hotY >= 0 ? hotY : bitmap.height() / 2 / devicePixelRatio;

    return d;
}

void QCursorData::update()
{
}

BOBUI_END_NAMESPACE
#endif // BOBUI_NO_CURSOR

