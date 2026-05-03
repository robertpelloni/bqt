// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>

#include <qapplication.h>
#include <qevent.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qstylepainter.h>
#if BOBUI_CONFIG(effects)
#include <private/qeffects_p.h>
#endif
#include <qpa/qplatformscreen.h>
#include <qpa/qplatformcursor.h>
#if BOBUI_CONFIG(style_stylesheet)
#include <private/qstylesheetstyle_p.h>
#endif
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformwindow_p.h>

#include <BobUIWidgets/private/qlabel_p.h>
#include <BobUIGui/private/qhighdpiscaling_p.h>
#include <bobuiooltip.h>
#include <BobUIWidgets/private/bobuiooltip_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

/*!
    \class BOBUIoolTip

    \brief The BOBUIoolTip class provides tool tips (balloon help) for any
    widget.

    \ingroup helpsystem
    \inmodule BobUIWidgets

    The tip is a short piece of text reminding the user of the
    widget's function. It is drawn immediately below the given
    position in a distinctive black-on-yellow color combination. The
    tip can be any \l{BOBUIextEdit}{rich text} formatted string.

    Rich text displayed in a tool tip is implicitly word-wrapped unless
    specified differently with \c{<p style='white-space:pre'>}.

    UI elements that are created via \l{QAction} use the tooltip property
    of the QAction, so for most interactive UI elements, setting that
    property is the easiest way to provide tool tips.

    \snippet tooltips/main.cpp action_tooltip

    For any other widgets, the simplest and most common way to set
    a widget's tool tip is by calling its QWidget::setToolTip() function.

    \snippet tooltips/main.cpp static_tooltip

    It is also possible to show different tool tips for different
    regions of a widget, by using a QHelpEvent of type
    QEvent::ToolTip. Intercept the help event in your widget's \l
    {QWidget::}{event()} function and call BOBUIoolTip::showText() with
    the text you want to display.

    \snippet tooltips/main.cpp dynamic_tooltip

    If you are calling BOBUIoolTip::hideText(), or BOBUIoolTip::showText()
    with an empty string, as a result of a \l{QEvent::}{ToolTip}-event you
    should also call \l{QEvent::}{ignore()} on the event, to signal
    that you don't want to start any tooltip specific modes.

    Note that, if you want to show tooltips in an item view, the
    model/view architecture provides functionality to set an item's
    tool tip; e.g., the BOBUIableWidgetItem::setToolTip() function.
    However, if you want to provide custom tool tips in an item view,
    you must intercept the help event in the
    QAbstractItemView::viewportEvent() function and handle it yourself.

    The default tool tip color and font can be customized with
    setPalette() and setFont(). When a tooltip is currently on
    display, isVisible() returns \c true and text() the currently visible
    text.

    \note Tool tips use the inactive color group of QPalette, because tool
    tips are not active windows.

    \sa QWidget::toolTip, QAction::toolTip
*/

BOBUIipLabel *BOBUIipLabel::instance = nullptr;

BOBUIipLabel::BOBUIipLabel(const QString &text, const QPoint &pos, QWidget *w, int msecDisplayTime)
    : QLabel(w, BobUI::ToolTip | BobUI::BypassGraphicsProxyWidget)
#if BOBUI_CONFIG(style_stylesheet)
    , styleSheetParent(nullptr)
#endif
    , widget(nullptr)
    , fadingOut(false)
{
    delete instance;
    instance = this;
    setForegroundRole(QPalette::ToolTipText);
    setBackgroundRole(QPalette::ToolTipBase);
    setPalette(BOBUIoolTip::palette());
    ensurePolished();
    setMargin(1 + style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth, nullptr, this));
    setFrameStyle(QFrame::NoFrame);
    setAlignment(BobUI::AlignLeft);
    setIndent(1);
    qApp->installEventFilter(this);
    setWindowOpacity(style()->styleHint(QStyle::SH_ToolTipLabel_Opacity, nullptr, this) / 255.0);
    setMouseTracking(true);
    reuseTip(text, msecDisplayTime, pos);
}

void BOBUIipLabel::restartExpireTimer(int msecDisplayTime)
{
    Q_D(const QLabel);
    const qsizetype textLength = d->needTextControl() ? d->control->toPlainText().size() : text().size();
    qsizetype time = 10000 + 40 * qMax(0, textLength - 100);
    if (msecDisplayTime > 0)
        time = msecDisplayTime;
    expireTimer.start(time, this);
    hideTimer.stop();
}

void BOBUIipLabel::reuseTip(const QString &text, int msecDisplayTime, const QPoint &pos)
{
#if BOBUI_CONFIG(style_stylesheet)
    if (styleSheetParent){
        disconnect(styleSheetParent, &QWidget::destroyed,
                   this, &BOBUIipLabel::styleSheetParentDestroyed);
        styleSheetParent = nullptr;
    }
#endif

    setText(text);
    updateSize(pos);
    restartExpireTimer(msecDisplayTime);
}

void  BOBUIipLabel::updateSize(const QPoint &pos)
{
    d_func()->setScreenForPoint(pos);
    // Ensure that we get correct sizeHints by placing this window on the right screen.
    QFontMetrics fm(font());
    QSize extra(1, 0);
    // Make it look good with the default ToolTip font on Mac, which has a small descent.
    if (fm.descent() == 2 && fm.ascent() >= 11)
        ++extra.rheight();
    setWordWrap(BobUI::mightBeRichText(text()));
    QSize sh = sizeHint();
    const QScreen *screen = getTipScreen(pos, this);
    if (!wordWrap() && sh.width() > screen->geometry().width()) {
        setWordWrap(true);
        sh = sizeHint();
    }
    resize(sh + extra);
}

void BOBUIipLabel::paintEvent(QPaintEvent *ev)
{
    QStylePainter p(this);
    QStyleOptionFrame opt;
    opt.initFrom(this);
    p.drawPrimitive(QStyle::PE_PanelTipLabel, opt);
    p.end();

    QLabel::paintEvent(ev);
}

void BOBUIipLabel::resizeEvent(QResizeEvent *e)
{
    QStyleHintReturnMask frameMask;
    QStyleOption option;
    option.initFrom(this);
    if (style()->styleHint(QStyle::SH_ToolTip_Mask, &option, this, &frameMask))
        setMask(frameMask.region);

    QLabel::resizeEvent(e);
}

void BOBUIipLabel::mouseMoveEvent(QMouseEvent *e)
{
    if (!rect.isNull()) {
        QPointF pos = e->globalPosition();
        if (widget)
            pos = widget->mapFromGlobal(pos);
        if (!rect.contains(pos.toPoint()))
            hideTip();
    }
    QLabel::mouseMoveEvent(e);
}

BOBUIipLabel::~BOBUIipLabel()
{
    instance = nullptr;
}

void BOBUIipLabel::hideTip()
{
    if (!hideTimer.isActive())
        hideTimer.start(300, this);
}

void BOBUIipLabel::hideTipImmediately()
{
    close(); // to trigger QEvent::Close which stops the animation
    deleteLater();
}

void BOBUIipLabel::setTipRect(QWidget *w, const QRect &r)
{
    if (Q_UNLIKELY(!r.isNull() && !w)) {
        qWarning("BOBUIoolTip::setTipRect: Cannot pass null widget if rect is set");
        return;
    }
    widget = w;
    rect = r;
}

void BOBUIipLabel::timerEvent(BOBUIimerEvent *e)
{
    if (e->timerId() == hideTimer.timerId()
        || e->timerId() == expireTimer.timerId()){
        hideTimer.stop();
        expireTimer.stop();
        hideTipImmediately();
    }
}

bool BOBUIipLabel::eventFilter(QObject *o, QEvent *e)
{
    switch (e->type()) {
#ifdef Q_OS_MACOS
    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
        const int key = static_cast<QKeyEvent *>(e)->key();
        // Anything except key modifiers or caps-lock, etc.
        if (key < BobUI::Key_Shift || key > BobUI::Key_ScrollLock)
            hideTipImmediately();
        break;
    }
#endif
    case QEvent::Leave:
        hideTip();
        break;


#if defined (Q_OS_QNX)  || defined (Q_OS_WASM) // On QNX the window activate and focus events are delayed and will appear
                       // after the window is shown.
    case QEvent::WindowActivate:
    case QEvent::FocusIn:
        return false;
    case QEvent::WindowDeactivate:
        if (o != this)
            return false;
        hideTipImmediately();
        break;
    case QEvent::FocusOut:
        if (reinterpret_cast<QWindow*>(o) != windowHandle())
            return false;
        hideTipImmediately();
        break;
#else
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
#endif
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::Wheel:
        hideTipImmediately();
        break;

    case QEvent::MouseMove:
        if (o == widget && !rect.isNull() && !rect.contains(static_cast<QMouseEvent*>(e)->position().toPoint()))
            hideTip();
        break;
    default:
        break;
    }
    return false;
}

QScreen *BOBUIipLabel::getTipScreen(const QPoint &pos, QWidget *w)
{
    QScreen *guess = w ? w->screen() : QGuiApplication::primaryScreen();
    QScreen *exact = guess->virtualSiblingAt(pos);
    return exact ? exact : guess;
}

void BOBUIipLabel::placeTip(const QPoint &pos, QWidget *w)
{
#if BOBUI_CONFIG(style_stylesheet)
    if (testAttribute(BobUI::WA_StyleSheet) || (w && bobui_styleSheet(w->style()))) {
        //the stylesheet need to know the real parent
        BOBUIipLabel::instance->setProperty("_q_stylesheet_parent", QVariant::fromValue(w));
        //we force the style to be the QStyleSheetStyle, and force to clear the cache as well.
        BOBUIipLabel::instance->setStyleSheet("/* */"_L1);

        // Set up for cleaning up this later...
        BOBUIipLabel::instance->styleSheetParent = w;
        if (w) {
            connect(w, &QWidget::destroyed,
                    BOBUIipLabel::instance, &BOBUIipLabel::styleSheetParentDestroyed);
        }
        // BOBUIBUG-64550: A font inherited by the style sheet might change the size,
        // particular on Windows, where the tip is not parented on a window.
        // The updatesSize() also makes sure that the content size be updated with
        // correct content margin.
        BOBUIipLabel::instance->updateSize(pos);
    }
#endif //BOBUI_CONFIG(style_stylesheet)

    QPoint p = pos;
    const QScreen *screen = getTipScreen(pos, w);
    // a QScreen's handle *should* never be null, so this is a bit paranoid
    if (const QPlatformScreen *platformScreen = screen ? screen->handle() : nullptr) {
        QPlatformCursor *cursor = platformScreen->cursor();
        // default implementation of QPlatformCursor::size() returns QSize(16, 16)
        const QSize nativeSize = cursor ? cursor->size() : QSize(16, 16);
        const QSize cursorSize = QHighDpi::fromNativePixels(nativeSize,
                                                            platformScreen);
        QPoint offset(2, cursorSize.height());
        // assuming an arrow shape, we can just move to the side for very large cursors
        if (cursorSize.height() > 2 * this->height())
            offset = QPoint(cursorSize.width() / 2, 0);

        p += offset;

#if BOBUI_CONFIG(wayland)
        if (w) {
            create();
            if (auto waylandWindow = dynamic_cast<QNativeInterface::Private::QWaylandWindow*>(windowHandle()->handle())) {
                // based on the existing code below, by default position at 'p' stored at the bottom right of our rect
                // then  flip to the other arbitrary 4x24 space if constrained
                const QRect controlGeometry = QRect(p.x() - 4, p.y() - 24, 4, 24)
                    .translated(-w->window()->mapToGlobal(QPoint(0, 0)));
                waylandWindow->setParentControlGeometry(controlGeometry);
                waylandWindow->setExtendedWindowType(QNativeInterface::Private::QWaylandWindow::ToolTip);
            }
        }
#endif

        QRect screenRect = screen->geometry();
        if (p.x() + this->width() > screenRect.x() + screenRect.width())
            p.rx() -= 4 + this->width();
        if (p.y() + this->height() > screenRect.y() + screenRect.height())
            p.ry() -= 24 + this->height();
        if (p.y() < screenRect.y())
            p.setY(screenRect.y());
        if (p.x() + this->width() > screenRect.x() + screenRect.width())
            p.setX(screenRect.x() + screenRect.width() - this->width());
        if (p.x() < screenRect.x())
            p.setX(screenRect.x());
        if (p.y() + this->height() > screenRect.y() + screenRect.height())
            p.setY(screenRect.y() + screenRect.height() - this->height());
    }
    this->move(p);
}

bool BOBUIipLabel::tipChanged(const QPoint &pos, const QString &text, QObject *o)
{
    if (BOBUIipLabel::instance->text() != text)
        return true;

    if (o != widget)
        return true;

    if (!rect.isNull())
        return !rect.contains(pos);
    else
       return false;
}

/** \internal
  Cleanup the _q_stylesheet_parent property.
 */
void BOBUIipLabel::styleSheetParentDestroyed()
{
    setProperty("_q_stylesheet_parent", QVariant());
    styleSheetParent = nullptr;
}

/*!
    Shows \a text as a tool tip, with the global position \a pos as
    the point of interest. The tool tip will be shown with a platform
    specific offset from this point of interest.

    If you specify a non-empty rect the tip will be hidden as soon
    as you move your cursor out of this area.

    The \a rect is in the coordinates of the widget you specify with
    \a w. If the \a rect is not empty you must specify a widget.
    Otherwise this argument can be \nullptr but it is used to
    determine the appropriate screen on multi-head systems.

    The \a msecDisplayTime parameter specifies for how long the tool tip
    will be displayed, in milliseconds. With the default value of -1, the
    time is based on the length of the text.

    If \a text is empty the tool tip is hidden. If the text is the
    same as the currently shown tooltip, the tip will \e not move.
    You can force moving by first hiding the tip with an empty text,
    and then showing the new tip at the new position.
*/

void BOBUIoolTip::showText(const QPoint &pos, const QString &text, QWidget *w, const QRect &rect, int msecDisplayTime)
{
    if (BOBUIipLabel::instance && BOBUIipLabel::instance->isVisible()) { // a tip does already exist
        if (text.isEmpty()){ // empty text means hide current tip
            BOBUIipLabel::instance->hideTip();
            return;
        } else if (!BOBUIipLabel::instance->fadingOut) {
            // If the tip has changed, reuse the one
            // that is showing (removes flickering)
            QPoint localPos = pos;
            if (w)
                localPos = w->mapFromGlobal(pos);
            if (BOBUIipLabel::instance->tipChanged(localPos, text, w)){
                BOBUIipLabel::instance->reuseTip(text, msecDisplayTime, pos);
                BOBUIipLabel::instance->setTipRect(w, rect);
                BOBUIipLabel::instance->placeTip(pos, w);
            }
            return;
        }
    }

    if (!text.isEmpty()) { // no tip can be reused, create new tip:
        QWidget *tipLabelParent = [w]() -> QWidget* {
#ifdef Q_OS_WIN32
            // On windows, we can't use the widget as parent otherwise the window will be
            // raised when the tooltip will be shown
            Q_UNUSED(w);
            return nullptr;
#else
            return w;
#endif
        }();
        new BOBUIipLabel(text, pos, tipLabelParent, msecDisplayTime); // sets BOBUIipLabel::instance to itself
        QWidgetPrivate::get(BOBUIipLabel::instance)->setScreen(BOBUIipLabel::getTipScreen(pos, w));
        BOBUIipLabel::instance->setTipRect(w, rect);
        BOBUIipLabel::instance->placeTip(pos, w);
        BOBUIipLabel::instance->setObjectName("bobuiooltip_label"_L1);

#if BOBUI_CONFIG(effects)
        if (QApplication::isEffectEnabled(BobUI::UI_FadeTooltip))
            qFadeEffect(BOBUIipLabel::instance);
        else if (QApplication::isEffectEnabled(BobUI::UI_AnimateTooltip))
            qScrollEffect(BOBUIipLabel::instance);
        else
            BOBUIipLabel::instance->showNormal();
#else
        BOBUIipLabel::instance->showNormal();
#endif
    }
}

/*!
    \fn void BOBUIoolTip::hideText()
    \since 4.2

    Hides the tool tip. This is the same as calling showText() with an
    empty string.

    \sa showText()
*/


/*!
  \since 4.4

  Returns \c true if a tooltip is currently shown.

  \sa showText()
 */
bool BOBUIoolTip::isVisible()
{
    return (BOBUIipLabel::instance != nullptr && BOBUIipLabel::instance->isVisible());
}

/*!
  \since 4.4

  Returns the tooltip text, if a tooltip is visible, or an
  empty string if a tooltip is not visible.
 */
QString BOBUIoolTip::text()
{
    if (BOBUIipLabel::instance)
        return BOBUIipLabel::instance->text();
    return QString();
}


Q_GLOBAL_STATIC(QPalette, tooltip_palette)

/*!
    Returns the palette used to render tooltips.

    \note Tool tips use the inactive color group of QPalette, because tool
    tips are not active windows.
*/
QPalette BOBUIoolTip::palette()
{
    return *tooltip_palette();
}

/*!
    \since 4.2

    Returns the font used to render tooltips.
*/
QFont BOBUIoolTip::font()
{
    return QApplication::font("BOBUIipLabel");
}

/*!
    \since 4.2

    Sets the \a palette used to render tooltips.

    \note Tool tips use the inactive color group of QPalette, because tool
    tips are not active windows.
*/
void BOBUIoolTip::setPalette(const QPalette &palette)
{
    *tooltip_palette() = palette;
    if (BOBUIipLabel::instance)
        BOBUIipLabel::instance->setPalette(palette);
}

/*!
    \since 4.2

    Sets the \a font used to render tooltips.
*/
void BOBUIoolTip::setFont(const QFont &font)
{
    QApplication::setFont(font, "BOBUIipLabel");
}

BOBUI_END_NAMESPACE

#include "moc_bobuiooltip_p.cpp"
