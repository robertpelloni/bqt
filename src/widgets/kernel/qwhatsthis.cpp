// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qwhatsthis.h"
#include "qpointer.h"
#include "qapplication.h"
#include <private/qguiapplication_p.h>
#include "qwidget.h"
#include "qevent.h"
#include "qpixmap.h"
#include "qscreen.h"
#include "qpainter.h"
#if BOBUI_CONFIG(action)
#include "qaction.h"
#endif // BOBUI_CONFIG(action)
#include "qcursor.h"
#include "qbitmap.h"
#include "bobuiextdocument.h"
#include <qpa/qplatformtheme.h>
#include "private/bobuiextdocumentlayout_p.h"
#include "qdebug.h"
#if BOBUI_CONFIG(accessibility)
#include "qaccessible.h"
#endif

BOBUI_BEGIN_NAMESPACE

/*!
    \class QWhatsThis
    \brief The QWhatsThis class provides a simple description of any
    widget, i.e. answering the question "What's This?".

    \ingroup helpsystem
    \inmodule BobUIWidgets

    "What's This?" help is part of an application's online help
    system, and provides users with information about the
    functionality and usage of a particular widget. "What's This?"
    help texts are typically longer and more detailed than
    \l{BOBUIoolTip}{tooltips}, but generally provide less information
    than that supplied by separate help windows.

    QWhatsThis provides a single window with an explanatory text that
    pops up when the user asks "What's This?". The default way for
    users to ask the question is to move the focus to the relevant
    widget and press Shift+F1. The help text appears immediately; it
    goes away as soon as the user does something else.
    (Note that if there is a shortcut for Shift+F1, this mechanism
    will not work.) Some dialogs provide a "?" button that users can
    click to enter "What's This?" mode; they then click the relevant
    widget to pop up the "What's This?" window. It is also possible to
    provide a a menu option or toolbar button to switch into "What's
    This?" mode.

    To add "What's This?" text to a widget or an action, you simply
    call QWidget::setWhatsThis() or QAction::setWhatsThis().

    The text can be either rich text or plain text. If you specify a
    rich text formatted string, it will be rendered using the default
    stylesheet, making it possible to embed images in the displayed
    text. To be as fast as possible, the default stylesheet uses a
    simple method to determine whether the text can be rendered as
    plain text. See BobUI::mightBeRichText() for details.

    \snippet whatsthis/whatsthis.cpp 0

    An alternative way to enter "What's This?" mode is to call
    createAction(), and add the returned QAction to either a menu or
    a tool bar. By invoking this context help action (in the picture
    below, the button with the arrow and question mark icon) the user
    switches into "What's This?" mode. If they now click on a widget
    the appropriate help text is shown. The mode is left when help is
    given or when the user presses Esc.

    \image whatsthis.png {"What's This?" mode for New File button}

    You can enter "What's This?" mode programmatically with
    enterWhatsThisMode(), check the mode with inWhatsThisMode(), and
    return to normal mode with leaveWhatsThisMode().

    If you want to control the "What's This?" behavior of a widget
    manually see BobUI::WA_CustomWhatsThis.

    It is also possible to show different help texts for different
    regions of a widget, by using a QHelpEvent of type
    QEvent::WhatsThis. Intercept the help event in your widget's
    QWidget::event() function and call QWhatsThis::showText() with the
    text you want to display for the position specified in
    QHelpEvent::pos(). If the text is rich text and the user clicks
    on a link, the widget also receives a QWhatsThisClickedEvent with
    the link's reference as QWhatsThisClickedEvent::href(). If a
    QWhatsThisClickedEvent is handled (i.e. QWidget::event() returns
    true), the help window remains visible. Call
    QWhatsThis::hideText() to hide it explicitly.

    \sa BOBUIoolTip
*/

class QWhatsThat : public QWidget
{
    Q_OBJECT

public:
    QWhatsThat(const QString& txt, QWidget* parent, QWidget *showTextFor);
    ~QWhatsThat() ;

    static QWhatsThat *instance;

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;

private:
    QPointer<QWidget>widget;
    bool pressed;
    QString text;
    BOBUIextDocument* doc;
    QString anchor;
};

QWhatsThat *QWhatsThat::instance = nullptr;

// shadowWidth not const, for XP drop-shadow-fu turns it to 0
static int shadowWidth = 6;   // also used as '5' and '6' and even '8' below
static const int vMargin = 8;
static const int hMargin = 12;

static inline bool dropShadow()
{
    if (const QPlatformTheme *theme = QGuiApplicationPrivate::platformTheme())
        return theme->themeHint(QPlatformTheme::DropShadow).toBool();
    return false;
}

QWhatsThat::QWhatsThat(const QString& txt, QWidget* parent, QWidget *showTextFor)
    : QWidget(parent, BobUI::Popup),
      widget(showTextFor), pressed(false), text(txt)
{
    delete instance;
    instance = this;
    setAttribute(BobUI::WA_DeleteOnClose, true);
    setAttribute(BobUI::WA_NoSystemBackground, true);
    if (parent)
        setPalette(parent->palette());
    setMouseTracking(true);
    setFocusPolicy(BobUI::StrongFocus);
#ifndef BOBUI_NO_CURSOR
    setCursor(BobUI::ArrowCursor);
#endif
    QRect r;
    doc = nullptr;
    ensurePolished(); // Ensures style sheet font before size calc
    if (BobUI::mightBeRichText(text)) {
        doc = new BOBUIextDocument();
        doc->setUndoRedoEnabled(false);
        doc->setDefaultFont(QApplication::font(this));
#ifdef BOBUI_NO_TEXTHTMLPARSER
        doc->setPlainText(text);
#else
        doc->setHtml(text);
#endif
        doc->setUndoRedoEnabled(false);
        doc->adjustSize();
        r.setTop(0);
        r.setLeft(0);
        r.setSize(doc->size().toSize());
    }
    else
    {
        int sw = QGuiApplication::primaryScreen()->virtualGeometry().width() / 3;
        if (sw < 200)
            sw = 200;
        else if (sw > 300)
            sw = 300;

        r = fontMetrics().boundingRect(0, 0, sw, 1000,
                                        BobUI::AlignLeft | BobUI::AlignTop
                                        | BobUI::TextWordWrap | BobUI::TextExpandTabs,
                                        text);
    }
    shadowWidth = dropShadow() ? 0 : 6;
    resize(r.width() + 2*hMargin + shadowWidth, r.height() + 2*vMargin + shadowWidth);
}

QWhatsThat::~QWhatsThat()
{
    instance = nullptr;
    if (doc)
        delete doc;
}

void QWhatsThat::mousePressEvent(QMouseEvent* e)
{
    pressed = true;
    if (e->button() == BobUI::LeftButton && rect().contains(e->position().toPoint())) {
        if (doc)
            anchor = doc->documentLayout()->anchorAt(e->position().toPoint() -  QPoint(hMargin, vMargin));
        return;
    }
    close();
}

void QWhatsThat::mouseReleaseEvent(QMouseEvent* e)
{
    if (!pressed)
        return;
    if (widget && e->button() == BobUI::LeftButton && doc && rect().contains(e->position().toPoint())) {
        QString a = doc->documentLayout()->anchorAt(e->position().toPoint() -  QPoint(hMargin, vMargin));
        QString href;
        if (anchor == a)
            href = a;
        anchor.clear();
        if (!href.isEmpty()) {
            QWhatsThisClickedEvent e(href);
            if (QCoreApplication::sendEvent(widget, &e))
                return;
        }
    }
    close();
}

void QWhatsThat::mouseMoveEvent(QMouseEvent* e)
{
#ifdef BOBUI_NO_CURSOR
    Q_UNUSED(e);
#else
    if (!doc)
        return;
    QString a = doc->documentLayout()->anchorAt(e->position().toPoint() -  QPoint(hMargin, vMargin));
    if (!a.isEmpty())
        setCursor(BobUI::PointingHandCursor);
    else
        setCursor(BobUI::ArrowCursor);
#endif
}

void QWhatsThat::keyPressEvent(QKeyEvent*)
{
    close();
}

void QWhatsThat::paintEvent(QPaintEvent*)
{
    const bool drawShadow = dropShadow();

    QRect r = rect();
    r.adjust(0, 0, -1, -1);
    if (drawShadow)
        r.adjust(0, 0, -shadowWidth, -shadowWidth);
    QPainter p(this);
    p.setPen(QPen(palette().toolTipText(), 0));
    p.setBrush(palette().toolTipBase());
    p.drawRect(r);
    int w = r.width();
    int h = r.height();
    p.setPen(palette().brush(QPalette::Dark).color());
    p.drawRect(1, 1, w-2, h-2);
    if (drawShadow) {
        p.setPen(palette().shadow().color());
        p.drawPoint(w + 5, 6);
        p.drawLine(w + 3, 6, w + 5, 8);
        p.drawLine(w + 1, 6, w + 5, 10);
        int i;
        for(i=7; i < h; i += 2)
            p.drawLine(w, i, w + 5, i + 5);
        for(i = w - i + h; i > 6; i -= 2)
            p.drawLine(i, h, i + 5, h + 5);
        for(; i > 0 ; i -= 2)
            p.drawLine(6, h + 6 - i, i + 5, h + 5);
    }
    r.adjust(0, 0, 1, 1);
    p.setPen(palette().toolTipText().color());
    r.adjust(hMargin, vMargin, -hMargin, -vMargin);

    if (doc) {
        p.translate(r.x(), r.y());
        QRect rect = r;
        rect.translate(-r.x(), -r.y());
        p.setClipRect(rect);
        QAbstractTextDocumentLayout::PaintContext context;
        context.palette.setBrush(QPalette::Text, context.palette.toolTipText());
        doc->documentLayout()->draw(&p, context);
    }
    else
    {
        p.drawText(r, BobUI::AlignLeft | BobUI::AlignTop | BobUI::TextWordWrap | BobUI::TextExpandTabs, text);
    }
}

static const char * const button_image[] = {
"16 16 3 1",
"         c None",
"o        c #000000",
"a        c #000080",
"o        aaaaa  ",
"oo      aaa aaa ",
"ooo    aaa   aaa",
"oooo   aa     aa",
"ooooo  aa     aa",
"oooooo  a    aaa",
"ooooooo     aaa ",
"oooooooo   aaa  ",
"ooooooooo aaa   ",
"ooooo     aaa   ",
"oo ooo          ",
"o  ooo    aaa   ",
"    ooo   aaa   ",
"    ooo         ",
"     ooo        ",
"     ooo        "};

class QWhatsThisPrivate : public QObject
{
 public:
    QWhatsThisPrivate();
    ~QWhatsThisPrivate();
    static QWhatsThisPrivate *instance;
    bool eventFilter(QObject *, QEvent *) override;
#if BOBUI_CONFIG(action)
    QPointer<QAction> action;
#endif // BOBUI_CONFIG(action)
    static void say(QWidget *, const QString &, int x = 0, int y = 0);
    static void notifyToplevels(QEvent *e);
    bool leaveOnMouseRelease;
};

void QWhatsThisPrivate::notifyToplevels(QEvent *e)
{
    const QWidgetList toplevels = QApplication::topLevelWidgets();
    for (auto *w : toplevels)
        QCoreApplication::sendEvent(w, e);
}

QWhatsThisPrivate *QWhatsThisPrivate::instance = nullptr;

QWhatsThisPrivate::QWhatsThisPrivate()
    : leaveOnMouseRelease(false)
{
    instance = this;
    qApp->installEventFilter(this);

    QPoint pos = QCursor::pos();
    if (QWidget *w = QApplication::widgetAt(pos)) {
        QHelpEvent e(QEvent::QueryWhatsThis, w->mapFromGlobal(pos), pos);
        const bool sentEvent = QCoreApplication::sendEvent(w, &e);
#ifdef BOBUI_NO_CURSOR
        Q_UNUSED(sentEvent);
#else
        QGuiApplication::setOverrideCursor((!sentEvent || !e.isAccepted())?
                                        BobUI::ForbiddenCursor:BobUI::WhatsThisCursor);
    } else {
        QGuiApplication::setOverrideCursor(BobUI::WhatsThisCursor);
#endif
    }
#if BOBUI_CONFIG(accessibility)
    QAccessibleEvent event(this, QAccessible::ContextHelpStart);
    QAccessible::updateAccessibility(&event);
#endif
}

QWhatsThisPrivate::~QWhatsThisPrivate()
{
#if BOBUI_CONFIG(action)
    if (action)
        action->setChecked(false);
#endif // BOBUI_CONFIG(action)
#ifndef BOBUI_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
#if BOBUI_CONFIG(accessibility)
    QAccessibleEvent event(this, QAccessible::ContextHelpEnd);
    QAccessible::updateAccessibility(&event);
#endif
    instance = nullptr;
}

bool QWhatsThisPrivate::eventFilter(QObject *o, QEvent *e)
{
    if (!o->isWidgetType())
        return false;
    QWidget * w = static_cast<QWidget *>(o);
    bool customWhatsThis = w->testAttribute(BobUI::WA_CustomWhatsThis);
    switch (e->type()) {
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        if (me->button() == BobUI::RightButton || customWhatsThis)
            return false;
        QHelpEvent e(QEvent::WhatsThis, me->position().toPoint(), me->globalPosition().toPoint());
        if (!QCoreApplication::sendEvent(w, &e) || !e.isAccepted())
            leaveOnMouseRelease = true;

    } break;

    case QEvent::MouseMove:
    {
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        QHelpEvent e(QEvent::QueryWhatsThis, me->position().toPoint(), me->globalPosition().toPoint());
        const bool sentEvent = QCoreApplication::sendEvent(w, &e);
#ifdef BOBUI_NO_CURSOR
        Q_UNUSED(sentEvent);
#else
        QGuiApplication::changeOverrideCursor((!sentEvent || !e.isAccepted())?
                                              BobUI::ForbiddenCursor:BobUI::WhatsThisCursor);
#endif
        Q_FALLTHROUGH();
    }
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
        if (leaveOnMouseRelease && e->type() == QEvent::MouseButtonRelease)
            QWhatsThis::leaveWhatsThisMode();
        if (static_cast<QMouseEvent*>(e)->button() == BobUI::RightButton || customWhatsThis)
            return false; // ignore RMB release
        break;
    case QEvent::KeyPress:
    {
        QKeyEvent *kev = static_cast<QKeyEvent *>(e);
#if BOBUI_CONFIG(shortcut)
        if (kev->matches(QKeySequence::Cancel)) {
            QWhatsThis::leaveWhatsThisMode();
            return true;
        } else
#endif
          if (customWhatsThis) {
            return false;
        } else if (kev->key() == BobUI::Key_Menu ||
                    (kev->key() == BobUI::Key_F10 &&
                      kev->modifiers() == BobUI::ShiftModifier)) {
            // we don't react to these keys, they are used for context menus
            return false;
        } else if (kev->key() != BobUI::Key_Shift && kev->key() != BobUI::Key_Alt // not a modifier key
                   && kev->key() != BobUI::Key_Control && kev->key() != BobUI::Key_Meta) {
            QWhatsThis::leaveWhatsThisMode();
        }
    } break;
    default:
        return false;
    }
    return true;
}

#if BOBUI_CONFIG(action)
class QWhatsThisAction: public QAction
{
    Q_OBJECT

public:
    explicit QWhatsThisAction(QObject* parent = nullptr);

private slots:
    void actionTriggered();
};

QWhatsThisAction::QWhatsThisAction(QObject *parent) : QAction(tr("What's This?"), parent)
{
#ifndef BOBUI_NO_IMAGEFORMAT_XPM
    QPixmap p(button_image);
    setIcon(p);
#endif
    setCheckable(true);
    connect(this, &QWhatsThisAction::triggered, this, &QWhatsThisAction::actionTriggered);
#ifndef BOBUI_NO_SHORTCUT
    setShortcut(BobUI::ShiftModifier | BobUI::Key_F1);
#endif
}

void QWhatsThisAction::actionTriggered()
{
    if (isChecked()) {
        QWhatsThis::enterWhatsThisMode();
        QWhatsThisPrivate::instance->action = this;
    }
}
#endif // BOBUI_CONFIG(action)

/*!
    This function switches the user interface into "What's This?"
    mode. The user interface can be switched back into normal mode by
    the user (e.g. by them clicking or pressing Esc), or
    programmatically by calling leaveWhatsThisMode().

    When entering "What's This?" mode, a QEvent of type
    BobUI::EnterWhatsThisMode is sent to all toplevel widgets.

    \sa inWhatsThisMode(), leaveWhatsThisMode()
*/
void QWhatsThis::enterWhatsThisMode()
{
    if (QWhatsThisPrivate::instance)
        return;
    (void) new QWhatsThisPrivate;
    QEvent e(QEvent::EnterWhatsThisMode);
    QWhatsThisPrivate::notifyToplevels(&e);
 }

/*!
    Returns \c true if the user interface is in "What's This?" mode;
    otherwise returns \c false.

    \sa enterWhatsThisMode()
*/
bool QWhatsThis::inWhatsThisMode()
{
    return (QWhatsThisPrivate::instance != nullptr);
}

/*!
    If the user interface is in "What's This?" mode, this function
    switches back to normal mode; otherwise it does nothing.

    When leaving "What's This?" mode, a QEvent of type
    BobUI::LeaveWhatsThisMode is sent to all toplevel widgets.

    \sa enterWhatsThisMode(), inWhatsThisMode()
*/
void QWhatsThis::leaveWhatsThisMode()
{
    delete QWhatsThisPrivate::instance;
    QEvent e(QEvent::LeaveWhatsThisMode);
    QWhatsThisPrivate::notifyToplevels(&e);
}

void QWhatsThisPrivate::say(QWidget * widget, const QString &text, int x, int y)
{
    if (text.size() == 0)
        return;
    // make a fresh widget, and set it up
    QWhatsThat *whatsThat = new QWhatsThat(text, nullptr, widget);

    // okay, now to find a suitable location
    QScreen *screen = widget ? widget->screen()
                             : QGuiApplication::screenAt(QPoint(x, y));
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->geometry();

    int w = whatsThat->width();
    int h = whatsThat->height();
    int sx = screenRect.x();
    int sy = screenRect.y();

    // first try locating the widget immediately above/below,
    // with nice alignment if possible.
    QPoint pos;
    if (widget)
        pos = widget->mapToGlobal(QPoint(0,0));

    if (widget && w > widget->width() + 16)
        x = pos.x() + widget->width()/2 - w/2;
    else
        x = x - w/2;

    // squeeze it in if that would result in part of what's this
    // being only partially visible
    if (x + w  + shadowWidth > sx+screenRect.width()) {
        x = (widget ? qMin(screenRect.width(), pos.x() + widget->width())
                    : screenRect.width())
            - w;
    }

    if (x < sx)
        x = sx;

    if (widget && h > widget->height() + 16) {
        y = pos.y() + widget->height() + 2; // below, two pixels spacing
        // what's this is above or below, wherever there's most space
        if (y + h + 10 > sy + screenRect.height())
            y = pos.y() + 2 - shadowWidth - h; // above, overlap
    }
    y = y + 2;

    // squeeze it in if that would result in part of what's this
    // being only partially visible
    if (y + h + shadowWidth > sy + screenRect.height()) {
        y = (widget ? qMin(screenRect.height(), pos.y() + widget->height())
                    : screenRect.height())
            - h;
    }
    if (y < sy)
        y = sy;

    whatsThat->move(x, y);
    whatsThat->show();
    whatsThat->grabKeyboard();
}

/*!
    Shows \a text as a "What's This?" window, at global position \a
    pos. The optional widget argument, \a w, is used to determine the
    appropriate screen on multi-head systems.

    \sa hideText()
*/
void QWhatsThis::showText(const QPoint &pos, const QString &text, QWidget *w)
{
    leaveWhatsThisMode();
    QWhatsThisPrivate::say(w, text, pos.x(), pos.y());
}

/*!
    If a "What's This?" window is showing, this destroys it.

    \sa showText()
*/
void QWhatsThis::hideText()
{
    delete QWhatsThat::instance;
}

/*!
    Returns a ready-made QAction, used to invoke "What's This?" context
    help, with the given \a parent.

    The returned QAction provides a convenient way to let users enter
    "What's This?" mode.
*/
#if BOBUI_CONFIG(action)
QAction *QWhatsThis::createAction(QObject *parent)
{
    return new QWhatsThisAction(parent);
}
#endif // BOBUI_CONFIG(action)

BOBUI_END_NAMESPACE

#include "qwhatsthis.moc"
