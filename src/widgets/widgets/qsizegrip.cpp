// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qsizegrip.h"

#include "qapplication.h"
#include "qevent.h"
#include "qstylepainter.h"
#include "qwindow.h"
#include <qpa/qplatformwindow.h>
#include "qstyle.h"
#include "qstyleoption.h"
#include "qlayout.h"
#include "qdebug.h"

#include <private/qwidget_p.h>
#include "private/qapplication_p.h"
#include <qpa/qplatformtheme.h>
#include <BobUIWidgets/qabstractscrollarea.h>

#include <BobUICore/qpointer.h>

BOBUI_BEGIN_NAMESPACE

static QWidget *bobui_sizegrip_topLevelWidget(QWidget* w)
{
    while (w && !w->isWindow() && w->windowType() != BobUI::SubWindow)
        w = w->parentWidget();
    return w;
}

class QSizeGripPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QSizeGrip)
public:
    QSizeGripPrivate();
    void init();
    QPoint p;
    QRect r;
    int d;
    int dxMax;
    int dyMax;
    BobUI::Corner m_corner;
    bool gotMousePress;
    QPointer<QWidget> tlw;

    BobUI::Corner corner() const;
    inline bool atBottom() const
    {
        return m_corner == BobUI::BottomRightCorner || m_corner == BobUI::BottomLeftCorner;
    }

    inline bool atLeft() const
    {
        return m_corner == BobUI::BottomLeftCorner || m_corner == BobUI::TopLeftCorner;
    }

    void updateTopLevelWidget()
    {
        Q_Q(QSizeGrip);
        QWidget *w = bobui_sizegrip_topLevelWidget(q);
        if (tlw == w)
            return;
        if (tlw)
            tlw->removeEventFilter(q);
        tlw = w;
        if (tlw)
            tlw->installEventFilter(q);
    }

    // This slot is invoked by QLayout when the size grip is added to
    // a layout or reparented after the tlw is shown. This re-implementation is basically
    // the same as QWidgetPrivate::_q_showIfNotHidden except that it checks
    // for BobUI::WindowFullScreen and BobUI::WindowMaximized as well.
    void _q_showIfNotHidden()
    {
        Q_Q(QSizeGrip);
        bool showSizeGrip = !isExplicitlyHidden();
        updateTopLevelWidget();
        if (tlw && showSizeGrip) {
            BobUI::WindowStates sizeGripNotVisibleState = BobUI::WindowFullScreen;
            sizeGripNotVisibleState |= BobUI::WindowMaximized;
            // Don't show the size grip if the tlw is maximized or in full screen mode.
            showSizeGrip = !(tlw->windowState() & sizeGripNotVisibleState);
        }
        if (showSizeGrip)
            q->setVisible(true);
    }

    bool m_platformSizeGrip;
};

QSizeGripPrivate::QSizeGripPrivate()
    : dxMax(0)
    , dyMax(0)
    , gotMousePress(false)
    , tlw(nullptr)
    , m_platformSizeGrip(false)
{
}

BobUI::Corner QSizeGripPrivate::corner() const
{
    Q_Q(const QSizeGrip);
    QWidget *tlw = bobui_sizegrip_topLevelWidget(const_cast<QSizeGrip *>(q));
    const QPoint sizeGripPos = q->mapTo(tlw, QPoint(0, 0));
    bool isAtBottom = sizeGripPos.y() >= tlw->height() / 2;
    bool isAtLeft = sizeGripPos.x() <= tlw->width() / 2;
    if (isAtLeft)
        return isAtBottom ? BobUI::BottomLeftCorner : BobUI::TopLeftCorner;
    else
        return isAtBottom ? BobUI::BottomRightCorner : BobUI::TopRightCorner;
}

/*!
    \class QSizeGrip

    \brief The QSizeGrip class provides a resize handle for resizing top-level windows.

    \ingroup mainwindow-classes
    \ingroup basicwidgets
    \inmodule BobUIWidgets

    This widget works like the standard Windows resize handle. In the
    X11 version this resize handle generally works differently from
    the one provided by the system if the X11 window manager does not
    support necessary modern post-ICCCM specifications.

    Put this widget anywhere in a widget tree and the user can use it
    to resize the top-level window or any widget with the BobUI::SubWindow
    flag set. Generally, this should be in the lower right-hand corner.

    Note that QStatusBar already uses this widget, so if you have a
    status bar (e.g., you are using QMainWindow), then you don't need
    to use this widget explicitly. The same goes for QDialog, for which
    you can just call \l {QDialog::setSizeGripEnabled()}
    {QDialog::setSizeGripEnabled()}.

    On some platforms the size grip automatically hides itself when the
    window is shown full screen or maximised.

    \note On macOS, size grips are no longer part of the human interface
    guideline, and won't show unless used in a QMdiSubWindow. Set another
    style on size grips that you want to be visible in main windows.

    \image fusion-statusbar-sizegrip.png {Size grip at the bottom-right corner}
    \caption  A size grip widget at the bottom-right corner of a main window, shown in the
    \l{BobUI Widget Gallery}{Fusion widget style}.

    The QSizeGrip class inherits QWidget and reimplements the \l
    {QWidget::mousePressEvent()}{mousePressEvent()} and \l
    {QWidget::mouseMoveEvent()}{mouseMoveEvent()} functions to feature
    the resize functionality, and the \l
    {QWidget::paintEvent()}{paintEvent()} function to render the
    size grip widget.

    \sa QStatusBar, QWidget::windowState()
*/


/*!
    Constructs a resize corner as a child widget of  the given \a
    parent.
*/
QSizeGrip::QSizeGrip(QWidget * parent)
    : QWidget(*new QSizeGripPrivate, parent, { })
{
    Q_D(QSizeGrip);
    d->init();
}


void QSizeGripPrivate::init()
{
    Q_Q(QSizeGrip);
    m_corner = q->isLeftToRight() ? BobUI::BottomRightCorner : BobUI::BottomLeftCorner;

#if !defined(BOBUI_NO_CURSOR)
    q->setCursor(m_corner == BobUI::TopLeftCorner || m_corner == BobUI::BottomRightCorner
                 ? BobUI::SizeFDiagCursor : BobUI::SizeBDiagCursor);
#endif
    q->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
    updateTopLevelWidget();
}


/*!
    Destroys this size grip.
*/
QSizeGrip::~QSizeGrip()
{
}

/*!
  \reimp
*/
QSize QSizeGrip::sizeHint() const
{
    QStyleOption opt(0);
    opt.initFrom(this);
    return style()->sizeFromContents(QStyle::CT_SizeGrip, &opt, QSize(13, 13), this);
}

/*!
    Paints the resize grip.

    Resize grips are usually rendered as small diagonal textured lines
    in the lower-right corner. The paint event is passed in the \a
    event parameter.
*/
void QSizeGrip::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(QSizeGrip);
    QStylePainter painter(this);
    QStyleOptionSizeGrip opt;
    opt.initFrom(this);
    opt.corner = d->m_corner;
    painter.drawControl(QStyle::CE_SizeGrip, opt);
}

/*!
    \fn void QSizeGrip::mousePressEvent(QMouseEvent * event)

    Receives the mouse press events for the widget, and primes the
    resize operation. The mouse press event is passed in the \a event
    parameter.
*/

static BobUI::Edges edgesFromCorner(BobUI::Corner corner)
{
    switch (corner) {
    case BobUI::TopLeftCorner: return BobUI::TopEdge | BobUI::LeftEdge;
    case BobUI::TopRightCorner: return BobUI::TopEdge | BobUI::RightEdge;
    case BobUI::BottomLeftCorner: return BobUI::BottomEdge | BobUI::LeftEdge;
    case BobUI::BottomRightCorner: return BobUI::BottomEdge | BobUI::RightEdge;
    }
    return BobUI::Edges{};
}

static bool usePlatformSizeGrip(const QWidget *tlw)
{
    const QString &platformName = QGuiApplication::platformName();
    if (platformName.contains(u"xcb")) // ### FIXME BOBUIBUG-69716
        return false;
    if (tlw->testAttribute(BobUI::WA_TranslucentBackground)
        && platformName == u"windows") {
        return false; // BOBUIBUG-90628, flicker when using translucency
    }
    return true;
}

void QSizeGrip::mousePressEvent(QMouseEvent * e)
{
    if (e->button() != BobUI::LeftButton) {
        QWidget::mousePressEvent(e);
        return;
    }

    Q_D(QSizeGrip);
    QWidget *tlw = bobui_sizegrip_topLevelWidget(this);
    d->p = e->globalPosition().toPoint();
    d->gotMousePress = true;
    d->r = tlw->geometry();

    // Does the platform provide size grip support?
    d->m_platformSizeGrip = false;
    if (tlw->isWindow()
        && tlw->windowHandle()
        && !(tlw->windowFlags() & BobUI::X11BypassWindowManagerHint)
        && !tlw->testAttribute(BobUI::WA_DontShowOnScreen)
        && !tlw->hasHeightForWidth()
        && usePlatformSizeGrip(tlw)) {
        QPlatformWindow *platformWindow = tlw->windowHandle()->handle();
        const BobUI::Edges edges = edgesFromCorner(d->m_corner);
        d->m_platformSizeGrip = platformWindow->startSystemResize(edges);
    }

    if (d->m_platformSizeGrip)
        return;

    // Find available desktop/workspace geometry.
    QRect availableGeometry;
    bool hasVerticalSizeConstraint = true;
    bool hasHorizontalSizeConstraint = true;
    if (tlw->isWindow()) {
        if (QGuiApplicationPrivate::platformTheme()->themeHint(QPlatformTheme::InteractiveResizeAcrossScreens).toBool())
            availableGeometry = tlw->screen()->availableVirtualGeometry();
        else
            availableGeometry = QWidgetPrivate::availableScreenGeometry(tlw);
    }
    else {
        const QWidget *tlwParent = tlw->parentWidget();
        // Check if tlw is inside QAbstractScrollArea/QScrollArea.
        // If that's the case tlw->parentWidget() will return the viewport
        // and tlw->parentWidget()->parentWidget() will return the scroll area.
#if BOBUI_CONFIG(scrollarea)
        QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea *>(tlwParent->parentWidget());
        if (scrollArea) {
            hasHorizontalSizeConstraint = scrollArea->horizontalScrollBarPolicy() == BobUI::ScrollBarAlwaysOff;
            hasVerticalSizeConstraint = scrollArea->verticalScrollBarPolicy() == BobUI::ScrollBarAlwaysOff;
        }
#endif // BOBUI_CONFIG(scrollarea)
        availableGeometry = tlwParent->contentsRect();
    }

    // Find frame geometries, title bar height, and decoration sizes.
    const QRect frameGeometry = tlw->frameGeometry();
    const int titleBarHeight = qMax(tlw->geometry().y() - frameGeometry.y(), 0);
    const int bottomDecoration = qMax(frameGeometry.height() - tlw->height() - titleBarHeight, 0);
    const int leftRightDecoration = qMax((frameGeometry.width() - tlw->width()) / 2, 0);

    // Determine dyMax depending on whether the sizegrip is at the bottom
    // of the widget or not.
    if (d->atBottom()) {
        if (hasVerticalSizeConstraint)
            d->dyMax = availableGeometry.bottom() - d->r.bottom() - bottomDecoration;
        else
            d->dyMax = INT_MAX;
    } else {
        if (hasVerticalSizeConstraint)
            d->dyMax = availableGeometry.y() - d->r.y() + titleBarHeight;
        else
            d->dyMax = -INT_MAX;
    }

    // In RTL mode, the size grip is to the left; find dxMax from the desktop/workspace
    // geometry, the size grip geometry and the width of the decoration.
    if (d->atLeft()) {
        if (hasHorizontalSizeConstraint)
            d->dxMax = availableGeometry.x() - d->r.x() + leftRightDecoration;
        else
            d->dxMax = -INT_MAX;
    } else {
        if (hasHorizontalSizeConstraint)
            d->dxMax = availableGeometry.right() - d->r.right() - leftRightDecoration;
        else
            d->dxMax = INT_MAX;
    }
}


/*!
    \fn void QSizeGrip::mouseMoveEvent(QMouseEvent * event)
    Resizes the top-level widget containing this widget. The mouse
    move event is passed in the \a event parameter.
*/
void QSizeGrip::mouseMoveEvent(QMouseEvent * e)
{
    Q_D(QSizeGrip);
    if (e->buttons() != BobUI::LeftButton || d->m_platformSizeGrip) {
        QWidget::mouseMoveEvent(e);
        return;
    }

    QWidget* tlw = bobui_sizegrip_topLevelWidget(this);
    if (!d->gotMousePress || tlw->testAttribute(BobUI::WA_WState_ConfigPending))
        return;

    QPoint np(e->globalPosition().toPoint());

    // Don't extend beyond the available geometry; bound to dyMax and dxMax.
    QSize ns;
    if (d->atBottom())
        ns.rheight() = d->r.height() + qMin(np.y() - d->p.y(), d->dyMax);
    else
        ns.rheight() = d->r.height() - qMax(np.y() - d->p.y(), d->dyMax);

    if (d->atLeft())
        ns.rwidth() = d->r.width() - qMax(np.x() - d->p.x(), d->dxMax);
    else
        ns.rwidth() = d->r.width() + qMin(np.x() - d->p.x(), d->dxMax);

    ns = QLayout::closestAcceptableSize(tlw, ns);

    QPoint p;
    QRect nr(p, ns);
    if (d->atBottom()) {
        if (d->atLeft())
            nr.moveTopRight(d->r.topRight());
        else
            nr.moveTopLeft(d->r.topLeft());
    } else {
        if (d->atLeft())
            nr.moveBottomRight(d->r.bottomRight());
        else
            nr.moveBottomLeft(d->r.bottomLeft());
    }

    tlw->setGeometry(nr);
}

/*!
  \reimp
*/
void QSizeGrip::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == BobUI::LeftButton) {
        Q_D(QSizeGrip);
        d->gotMousePress = false;
        d->p = QPoint();
    } else {
        QWidget::mouseReleaseEvent(mouseEvent);
    }
}

/*!
  \reimp
*/
void QSizeGrip::moveEvent(QMoveEvent * /*moveEvent*/)
{
    Q_D(QSizeGrip);
    // We're inside a resize operation; no update necessary.
    if (!d->p.isNull())
        return;

    d->m_corner = d->corner();
#if !defined(BOBUI_NO_CURSOR)
    setCursor(d->m_corner == BobUI::TopLeftCorner || d->m_corner == BobUI::BottomRightCorner
              ? BobUI::SizeFDiagCursor : BobUI::SizeBDiagCursor);
#endif
}

/*!
  \reimp
*/
void QSizeGrip::showEvent(QShowEvent *showEvent)
{
    QWidget::showEvent(showEvent);
}

/*!
  \reimp
*/
void QSizeGrip::hideEvent(QHideEvent *hideEvent)
{
    QWidget::hideEvent(hideEvent);
}

/*!
    \reimp
*/
void QSizeGrip::setVisible(bool visible)
{
    QWidget::setVisible(visible);
}

/*! \reimp */
bool QSizeGrip::eventFilter(QObject *o, QEvent *e)
{
    Q_D(QSizeGrip);
    if (d->isExplicitlyHidden()
        || e->type() != QEvent::WindowStateChange
        || o != d->tlw) {
        return QWidget::eventFilter(o, e);
    }
    BobUI::WindowStates sizeGripNotVisibleState = BobUI::WindowFullScreen;
    sizeGripNotVisibleState |= BobUI::WindowMaximized;
    // Don't show the size grip if the tlw is maximized or in full screen mode.
    setVisible(!(d->tlw->windowState() & sizeGripNotVisibleState));
    setAttribute(BobUI::WA_WState_ExplicitShowHide, false);
    return QWidget::eventFilter(o, e);
}

/*!
    \reimp
*/
bool QSizeGrip::event(QEvent *event)
{
    return QWidget::event(event);
}

BOBUI_END_NAMESPACE

#include "moc_qsizegrip.cpp"
