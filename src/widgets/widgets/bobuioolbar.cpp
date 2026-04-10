// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuioolbar.h"

#include <qapplication.h>
#if BOBUI_CONFIG(draganddrop)
#include <qdrag.h>
#endif
#include <qevent.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qmenu.h>
#include <qmimedata.h>
#include <qstylepainter.h>
#include <qstyleoption.h>
#include <bobuioolbutton.h>
#include <qwidgetaction.h>
#include <private/qwidgetaction_p.h>
#include <private/qmainwindowlayout_p.h>
#include <private/qhighdpiscaling_p.h>

#ifdef Q_OS_MACOS
#include <qpa/qplatformnativeinterface.h>
#endif

#include "bobuioolbar_p.h"
#include "bobuioolbarseparator_p.h"
#include "bobuioolbarlayout_p.h"

#include  "qdebug.h"

#define POPUP_TIMER_INTERVAL 500

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

// qmainwindow.cpp
extern QMainWindowLayout *bobui_mainwindow_layout(const QMainWindow *window);

/******************************************************************************
** BOBUIoolBarPrivate
*/

void BOBUIoolBarPrivate::init()
{
    Q_Q(BOBUIoolBar);
    q->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    q->setBackgroundRole(QPalette::Button);
    q->setAttribute(BobUI::WA_Hover);
    q->setAttribute(BobUI::WA_X11NetWmWindowTypeToolBar);

    QStyle *style = q->style();
    int e = style->pixelMetric(QStyle::PM_ToolBarIconSize, nullptr, q);
    iconSize = QSize(e, e);

    layout = new BOBUIoolBarLayout(q);
    layout->updateMarginAndSpacing();

    toggleViewAction = new QAction(q);
    toggleViewAction->setCheckable(true);
    q->setMovable(q->style()->styleHint(QStyle::SH_ToolBar_Movable, nullptr, q ));
    QObject::connect(toggleViewAction, SIGNAL(triggered(bool)), q, SLOT(_q_toggleView(bool)));
}

void BOBUIoolBarPrivate::_q_toggleView(bool b)
{
    Q_Q(BOBUIoolBar);
    if (b == q->isHidden()) {
        if (b)
            q->show();
        else
            q->close();
    }
}

void BOBUIoolBarPrivate::_q_updateIconSize(const QSize &sz)
{
    Q_Q(BOBUIoolBar);
    if (!explicitIconSize) {
        // iconSize not explicitly set
        q->setIconSize(sz);
        explicitIconSize = false;
    }
}

void BOBUIoolBarPrivate::_q_updateToolButtonStyle(BobUI::ToolButtonStyle style)
{
    Q_Q(BOBUIoolBar);
    if (!explicitToolButtonStyle) {
        q->setToolButtonStyle(style);
        explicitToolButtonStyle = false;
    }
}

void BOBUIoolBarPrivate::updateWindowFlags(bool floating, bool unplug)
{
    Q_Q(BOBUIoolBar);
    BobUI::WindowFlags flags = floating ? BobUI::Tool : BobUI::Widget;

    flags |= BobUI::FramelessWindowHint;

#if BOBUI_CONFIG(draganddrop)
    // If we are performing a platform drag the flag is not needed and we want to avoid recreating
    // the platform window when it would be removed later
    if (unplug && !QMainWindowLayout::needsPlatformDrag())
        flags |= BobUI::X11BypassWindowManagerHint;
#else
    Q_UNUSED(unplug);
#endif

    q->setWindowFlags(flags);
}

void BOBUIoolBarPrivate::setWindowState(bool floating, bool unplug, const QRect &rect)
{
    Q_Q(BOBUIoolBar);
    bool visible = !q->isHidden();
    bool wasFloating = q->isFloating(); // ...is also currently using popup menus

    updateWindowFlags(floating, unplug);

    if (floating != wasFloating)
        layout->checkUsePopupMenu();

    if (!rect.isNull())
        q->setGeometry(rect);

    if (visible)
        q->show();

    if (floating != wasFloating)
        emit q->topLevelChanged(floating);
}

void BOBUIoolBarPrivate::initDrag(const QPoint &pos)
{
    Q_Q(BOBUIoolBar);

    if (state != nullptr)
        return;

    QMainWindow *win = qobject_cast<QMainWindow*>(parent);
    Q_ASSERT(win != nullptr);
    QMainWindowLayout *layout = bobui_mainwindow_layout(win);
    Q_ASSERT(layout != nullptr);
    if (layout->pluggingWidget != nullptr) // the main window is animating a docking operation
        return;

    state = new DragState;
    state->pressPos = pos;
    state->dragging = false;
    state->moving = false;
    state->widgetItem = nullptr;

    if (q->isRightToLeft())
        state->pressPos = QPoint(q->width() - state->pressPos.x(), state->pressPos.y());
}

void BOBUIoolBarPrivate::startDrag(bool moving)
{
    Q_Q(BOBUIoolBar);

    Q_ASSERT(state != nullptr);

    if ((moving && state->moving) || state->dragging)
        return;

    QMainWindow *win = qobject_cast<QMainWindow*>(parent);
    Q_ASSERT(win != nullptr);
    QMainWindowLayout *layout = bobui_mainwindow_layout(win);
    Q_ASSERT(layout != nullptr);

#if BOBUI_CONFIG(draganddrop)
    const bool wasFloating = q->isFloating();
#endif

    if (!moving) {
        state->widgetItem = layout->unplug(q, QDockWidgetPrivate::DragScope::Group);
        Q_ASSERT(state->widgetItem != nullptr);
    }
    state->dragging = !moving;
    state->moving = moving;

#if BOBUI_CONFIG(draganddrop)
    if (QMainWindowLayout::needsPlatformDrag() && state->dragging) {
        auto result = layout->performPlatformWidgetDrag(state->widgetItem, state->pressPos);
        if (result == BobUI::IgnoreAction && !wasFloating) {
            layout->revert(state->widgetItem);
            delete state;
            state = nullptr;
        } else {
            endDrag();
        }
    }
#endif
}

void BOBUIoolBarPrivate::endDrag()
{
    Q_Q(BOBUIoolBar);
    Q_ASSERT(state != nullptr);

    q->releaseMouse();

    if (state->dragging) {
        QMainWindowLayout *layout = bobui_mainwindow_layout(qobject_cast<QMainWindow *>(q->parentWidget()));
        Q_ASSERT(layout != nullptr);

        if (!layout->plug(state->widgetItem)) {
            if (q->isFloatable()) {
                layout->restore();
                setWindowState(true); // gets rid of the X11BypassWindowManager window flag
                                      // and activates the resizer
                q->activateWindow();
            } else {
                layout->revert(state->widgetItem);
            }
        }
    }

    delete state;
    state = nullptr;
}

bool BOBUIoolBarPrivate::mousePressEvent(QMouseEvent *event)
{
    Q_Q(BOBUIoolBar);
    QStyleOptionToolBar opt;
    q->initStyleOption(&opt);
    if (q->style()->subElementRect(QStyle::SE_ToolBarHandle, &opt, q).contains(event->position().toPoint()) == false) {
#ifdef Q_OS_MACOS
        // When using the unified toolbar on OS X, the user can click and
        // drag between toolbar contents to move the window. Make this work by
        // implementing the standard mouse-dragging code and then call
        // window->move() in mouseMoveEvent below.
        if (QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent)) {
            if (mainWindow->toolBarArea(q) == BobUI::TopToolBarArea
                    && mainWindow->unifiedTitleAndToolBarOnMac()
                    && q->childAt(event->pos()) == 0) {
                macWindowDragging = true;
                macWindowDragPressPosition = event->pos();
                return true;
            }
        }
#endif
        return false;
    }

    if (event->button() != BobUI::LeftButton)
        return true;

    if (!layout->movable())
        return true;

    initDrag(event->position().toPoint());
    return true;
}

bool BOBUIoolBarPrivate::mouseReleaseEvent(QMouseEvent*)
{
#if BOBUI_CONFIG(draganddrop)
    // if we are peforming a platform drag ignore the release here and  end the drag when the actual
    // drag ends.
    if (QMainWindowLayout::needsPlatformDrag())
        return false;
#endif

    if (state != nullptr) {
        endDrag();
        return true;
    } else {
#ifdef Q_OS_MACOS
        if (!macWindowDragging)
            return false;
        macWindowDragging = false;
        macWindowDragPressPosition = QPoint();
        return true;
#endif
        return false;
    }
}

bool BOBUIoolBarPrivate::mouseMoveEvent(QMouseEvent *event)
{
    Q_Q(BOBUIoolBar);

    if (!state) {
#ifdef Q_OS_MACOS
        if (!macWindowDragging)
            return false;
        QWidget *w = q->window();
        const QPoint delta = event->pos() - macWindowDragPressPosition;
        w->move(w->pos() + delta);
        return true;
#endif
        return false;
    }

    QMainWindow *win = qobject_cast<QMainWindow*>(parent);
    if (win == nullptr)
        return true;

    QMainWindowLayout *layout = bobui_mainwindow_layout(win);
    Q_ASSERT(layout != nullptr);

    if (layout->pluggingWidget == nullptr
        && (event->position().toPoint() - state->pressPos).manhattanLength() > QApplication::startDragDistance()) {
            const bool wasDragging = state->dragging;
            const bool moving = !q->isWindow() && (orientation == BobUI::Vertical ?
                event->position().toPoint().x() >= 0 && event->position().toPoint().x() < q->width() :
                event->position().toPoint().y() >= 0 && event->position().toPoint().y() < q->height());

            startDrag(moving);
            if (!moving && !wasDragging)
                q->grabMouse();
    }

    if (!state) {
        q->releaseMouse();
        return true;
    }

    if (state->dragging) {
        QPoint pos = event->globalPosition().toPoint();
        // if we are right-to-left, we move so as to keep the right edge the same distance
        // from the mouse
        if (q->isLeftToRight())
            pos -= state->pressPos;
        else
            pos += QPoint(state->pressPos.x() - q->width(), -state->pressPos.y());

        q->move(pos);
        layout->hover(state->widgetItem, event->globalPosition().toPoint());
    } else if (state->moving) {

        const QPoint rtl(q->width() - state->pressPos.x(), state->pressPos.y()); //for RTL
        const QPoint globalPressPos = q->mapToGlobal(q->isRightToLeft() ? rtl : state->pressPos);
        int pos = 0;

        const QWindow *handle = q->window() ? q->window()->windowHandle() : nullptr;
        const QPoint delta = handle
                ? QHighDpi::fromNativePixels(event->globalPosition(), handle).toPoint()
                  - QHighDpi::fromNativePixels(globalPressPos, handle)
                : event->globalPosition().toPoint() - globalPressPos;

        if (orientation == BobUI::Vertical) {
            pos = q->y() + delta.y();
        } else {
            if (q->isRightToLeft()) {
                pos = win->width() - q->width() - q->x()  - delta.x();
            } else {
                pos = q->x() + delta.x();
            }
        }

        layout->moveToolBar(q, pos);
    }
    return true;
}

void BOBUIoolBarPrivate::unplug(const QRect &_r)
{
    Q_Q(BOBUIoolBar);
    QRect r = _r;
    r.moveTopLeft(q->mapToGlobal(QPoint(0, 0)));
    setWindowState(true, true, r);
    layout->setExpanded(false);
}

void BOBUIoolBarPrivate::plug(const QRect &r)
{
    setWindowState(false, false, r);
}

/******************************************************************************
** BOBUIoolBar
*/

/*!
    \class BOBUIoolBar

    \brief The BOBUIoolBar class provides a movable panel that contains a
    set of controls.

    \ingroup mainwindow-classes
    \inmodule BobUIWidgets

    A toolbar is typically created by calling
    \l QMainWindow::addToolBar(const QString &title), but it can also
    be added as the first widget in a QVBoxLayout, for example.

    Toolbar buttons are added by adding \e actions, using addAction()
    or insertAction(). Groups of buttons can be separated using
    addSeparator() or insertSeparator(). If a toolbar button is not
    appropriate, a widget can be inserted instead using addWidget() or
    insertWidget(). Examples of suitable widgets are QSpinBox,
    QDoubleSpinBox, and QComboBox. When a toolbar button is pressed, it
    emits the actionTriggered() signal.

    A toolbar can be fixed in place in a particular area (e.g., at the
    top of the window), or it can be movable between toolbar areas;
    see setMovable(), isMovable(), allowedAreas() and isAreaAllowed().

    When a toolbar is resized in such a way that it is too small to
    show all the items it contains, an extension button will appear as
    the last item in the toolbar. Pressing the extension button will
    pop up a menu containing the items that do not currently fit in
    the toolbar.

    When a BOBUIoolBar is not a child of a QMainWindow, it loses the ability
    to populate the extension pop up with widgets added to the toolbar using
    addWidget(). Please use widget actions created by inheriting QWidgetAction
    and implementing QWidgetAction::createWidget() instead.

    \sa BOBUIoolButton, QMenu, QAction
*/

/*!
    \fn bool BOBUIoolBar::isAreaAllowed(BobUI::ToolBarArea area) const

    Returns \c true if this toolbar is dockable in the given \a area;
    otherwise returns \c false.
*/

/*!
    \fn void BOBUIoolBar::actionTriggered(QAction *action)

    This signal is emitted when an action in this toolbar is triggered.
    This happens when the action's tool button is pressed, or when the
    action is triggered in some other way outside the toolbar. The parameter
    holds the triggered \a action.
*/

/*!
    \fn void BOBUIoolBar::allowedAreasChanged(BobUI::ToolBarAreas allowedAreas)

    This signal is emitted when the collection of allowed areas for the
    toolbar is changed. The new areas in which the toolbar can be positioned
    are specified by \a allowedAreas.

    \sa allowedAreas
*/

/*!
    \fn void BOBUIoolBar::iconSizeChanged(const QSize &iconSize)

    This signal is emitted when the icon size is changed. The \a
    iconSize parameter holds the toolbar's new icon size.

    \sa iconSize, QMainWindow::iconSize
*/

/*!
    \fn void BOBUIoolBar::movableChanged(bool movable)

    This signal is emitted when the toolbar becomes movable or fixed.
    If the toolbar can be moved, \a movable is true; otherwise it is
    false.

    \sa movable
*/

/*!
    \fn void BOBUIoolBar::orientationChanged(BobUI::Orientation orientation)

    This signal is emitted when the orientation of the toolbar changes.
    The \a orientation parameter holds the toolbar's new orientation.

    \sa orientation
*/

/*!
    \fn void BOBUIoolBar::toolButtonStyleChanged(BobUI::ToolButtonStyle toolButtonStyle)

    This signal is emitted when the tool button style is changed. The
    \a toolButtonStyle parameter holds the toolbar's new tool button
    style.

    \sa toolButtonStyle, QMainWindow::toolButtonStyle
*/

/*!
    \since 4.6

    \fn void BOBUIoolBar::topLevelChanged(bool topLevel)

    This signal is emitted when the \l floating property changes.
    The \a topLevel parameter is true if the toolbar is now floating;
    otherwise it is false.

    \sa isWindow()
*/


/*!
    \fn void BOBUIoolBar::visibilityChanged(bool visible)
    \since 4.7

    This signal is emitted when the toolbar becomes \a visible (or
    invisible). This happens when the widget is hidden or shown.
*/

/*!
    Constructs a BOBUIoolBar with the given \a parent.
*/
BOBUIoolBar::BOBUIoolBar(QWidget *parent)
    : QWidget(*new BOBUIoolBarPrivate, parent, { })
{
    Q_D(BOBUIoolBar);
    d->init();
}

/*!
    Constructs a BOBUIoolBar with the given \a parent.

    The given window \a title identifies the toolbar and is shown in
    the context menu provided by QMainWindow.

    \sa setWindowTitle()
*/
BOBUIoolBar::BOBUIoolBar(const QString &title, QWidget *parent)
    : BOBUIoolBar(parent)
{
    setWindowTitle(title);
}


/*!
    Destroys the toolbar.
*/
BOBUIoolBar::~BOBUIoolBar()
{
}

/*! \property BOBUIoolBar::movable
    \brief whether the user can move the toolbar within the toolbar area,
    or between toolbar areas.

    By default, this property is \c true.

    This property only makes sense if the toolbar is in a
    QMainWindow.

    \sa allowedAreas
*/

void BOBUIoolBar::setMovable(bool movable)
{
    Q_D(BOBUIoolBar);
    if (!movable == !d->movable)
        return;
    d->movable = movable;
    d->layout->invalidate();
    emit movableChanged(d->movable);
}

bool BOBUIoolBar::isMovable() const
{
    Q_D(const BOBUIoolBar);
    return d->movable;
}

/*!
    \property BOBUIoolBar::floatable
    \brief whether the toolbar can be dragged and dropped as an independent window.

    The default is true.
*/
bool BOBUIoolBar::isFloatable() const
{
    Q_D(const BOBUIoolBar);
    return d->floatable;
}

void BOBUIoolBar::setFloatable(bool floatable)
{
    Q_D(BOBUIoolBar);
    d->floatable = floatable;
}

/*!
    \property BOBUIoolBar::floating
    \brief whether the toolbar is an independent window.

    By default, this property is \c true.

    \sa QWidget::isWindow()
*/
bool BOBUIoolBar::isFloating() const
{
    return isWindow();
}

/*!
    \property BOBUIoolBar::allowedAreas
    \brief areas where the toolbar may be placed

    The default is BobUI::AllToolBarAreas.

    This property only makes sense if the toolbar is in a
    QMainWindow.

    \sa movable
*/

void BOBUIoolBar::setAllowedAreas(BobUI::ToolBarAreas areas)
{
    Q_D(BOBUIoolBar);
    areas &= BobUI::ToolBarArea_Mask;
    if (areas == d->allowedAreas)
        return;
    d->allowedAreas = areas;
    emit allowedAreasChanged(d->allowedAreas);
}

BobUI::ToolBarAreas BOBUIoolBar::allowedAreas() const
{
    Q_D(const BOBUIoolBar);
    return d->allowedAreas;
}

/*! \property BOBUIoolBar::orientation
    \brief orientation of the toolbar

    The default is BobUI::Horizontal.

    This function should not be used when the toolbar is managed
    by QMainWindow. You can use QMainWindow::addToolBar() or
    QMainWindow::insertToolBar() if you wish to move a toolbar that
    is already added to a main window to another BobUI::ToolBarArea.
*/

void BOBUIoolBar::setOrientation(BobUI::Orientation orientation)
{
    Q_D(BOBUIoolBar);
    if (orientation == d->orientation)
        return;

    d->orientation = orientation;

    if (orientation == BobUI::Vertical)
        setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    else
        setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));

    d->layout->invalidate();
    d->layout->activate();

    emit orientationChanged(d->orientation);
}

BobUI::Orientation BOBUIoolBar::orientation() const
{ Q_D(const BOBUIoolBar); return d->orientation; }

/*!
    \property BOBUIoolBar::iconSize
    \brief size of icons in the toolbar.

    The default size is determined by the application's style and is
    derived from the QStyle::PM_ToolBarIconSize pixel metric. It is
    the maximum size an icon can have. Icons of smaller size will not
    be scaled up.
*/

QSize BOBUIoolBar::iconSize() const
{ Q_D(const BOBUIoolBar); return d->iconSize; }

void BOBUIoolBar::setIconSize(const QSize &iconSize)
{
    Q_D(BOBUIoolBar);
    QSize sz = iconSize;
    if (!sz.isValid()) {
        QMainWindow *mw = qobject_cast<QMainWindow *>(parentWidget());
        if (mw && mw->layout()) {
            QLayout *layout = mw->layout();
            int i = 0;
            QLayoutItem *item = nullptr;
            do {
                item = layout->itemAt(i++);
                if (item && (item->widget() == this))
                    sz = mw->iconSize();
            } while (!sz.isValid() && item != nullptr);
        }
    }
    if (!sz.isValid()) {
        const int metric = style()->pixelMetric(QStyle::PM_ToolBarIconSize, nullptr, this);
        sz = QSize(metric, metric);
    }
    if (d->iconSize != sz) {
        d->iconSize = sz;
        setMinimumSize(0, 0);
        emit iconSizeChanged(d->iconSize);
    }
    d->explicitIconSize = iconSize.isValid();

    d->layout->invalidate();
}

/*!
    \property BOBUIoolBar::toolButtonStyle
    \brief the style of toolbar buttons

    This property defines the style of all tool buttons that are added
    as \l{QAction}s. Note that if you add a BOBUIoolButton with the
    addWidget() method, it will not get this button style.

    To have the style of toolbuttons follow the system settings, set this property to BobUI::ToolButtonFollowStyle.
    On Unix, the user settings from the desktop environment will be used.
    On other platforms, BobUI::ToolButtonFollowStyle means icon only.

    The default is BobUI::ToolButtonIconOnly.
*/

BobUI::ToolButtonStyle BOBUIoolBar::toolButtonStyle() const
{ Q_D(const BOBUIoolBar); return d->toolButtonStyle; }

void BOBUIoolBar::setToolButtonStyle(BobUI::ToolButtonStyle toolButtonStyle)
{
    Q_D(BOBUIoolBar);
    d->explicitToolButtonStyle = true;
    if (d->toolButtonStyle == toolButtonStyle)
        return;
    d->toolButtonStyle = toolButtonStyle;
    setMinimumSize(0, 0);
    emit toolButtonStyleChanged(d->toolButtonStyle);
}

/*!
    Removes all actions from the toolbar.

    \sa removeAction()
*/
void BOBUIoolBar::clear()
{
    QList<QAction *> actions = this->actions();
    for(int i = 0; i < actions.size(); i++)
        removeAction(actions.at(i));
}

/*!
     Adds a separator to the end of the toolbar.

     \sa insertSeparator()
*/
QAction *BOBUIoolBar::addSeparator()
{
    QAction *action = new QAction(this);
    action->setSeparator(true);
    addAction(action);
    return action;
}

/*!
    Inserts a separator into the toolbar in front of the toolbar
    item associated with the \a before action.

    \sa addSeparator()
*/
QAction *BOBUIoolBar::insertSeparator(QAction *before)
{
    QAction *action = new QAction(this);
    action->setSeparator(true);
    insertAction(before, action);
    return action;
}

/*!
    Adds the given \a widget to the toolbar as the toolbar's last
    item.

    The toolbar takes ownership of \a widget.

    If you add a BOBUIoolButton with this method, the toolbar's
    BobUI::ToolButtonStyle will not be respected.

    \note You should use QAction::setVisible() to change the
    visibility of the widget. Using QWidget::setVisible(),
    QWidget::show() and QWidget::hide() does not work.

    \sa insertWidget()
*/
QAction *BOBUIoolBar::addWidget(QWidget *widget)
{
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    action->d_func()->autoCreated = true;
    addAction(action);
    return action;
}

/*!
    Inserts the given \a widget in front of the toolbar item
    associated with the \a before action.

    Note: You should use QAction::setVisible() to change the
    visibility of the widget. Using QWidget::setVisible(),
    QWidget::show() and QWidget::hide() does not work.

    \sa addWidget()
*/
QAction *BOBUIoolBar::insertWidget(QAction *before, QWidget *widget)
{
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(widget);
    action->d_func()->autoCreated = true;
    insertAction(before, action);
    return action;
}

/*!
    \internal

    Returns the geometry of the toolbar item associated with the given
    \a action, or an invalid QRect if no matching item is found.
*/
QRect BOBUIoolBar::actionGeometry(QAction *action) const
{
    Q_D(const BOBUIoolBar);

    int index = d->layout->indexOf(action);
    if (index == -1)
        return QRect();
    return d->layout->itemAt(index)->widget()->geometry();
}

/*!
    Returns the action at point \a p. This function returns zero if no
    action was found.

    \sa QWidget::childAt()
*/
QAction *BOBUIoolBar::actionAt(const QPoint &p) const
{
    Q_D(const BOBUIoolBar);
    QWidget *widget = childAt(p);
    int index = d->layout->indexOf(widget);
    if (index == -1)
        return nullptr;
    QLayoutItem *item = d->layout->itemAt(index);
    return static_cast<BOBUIoolBarItem*>(item)->action;
}

/*! \fn QAction *BOBUIoolBar::actionAt(int x, int y) const
    \overload

    Returns the action at the point \a x, \a y. This function returns
    zero if no action was found.
*/

/*! \reimp */
void BOBUIoolBar::actionEvent(QActionEvent *event)
{
    Q_D(BOBUIoolBar);
    auto action = static_cast<QAction *>(event->action());
    QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(action);

    switch (event->type()) {
        case QEvent::ActionAdded: {
            Q_ASSERT_X(widgetAction == nullptr || d->layout->indexOf(widgetAction) == -1,
                        "BOBUIoolBar", "widgets cannot be inserted multiple times");

            // reparent the action to this toolbar if it has been created
            // using the addAction(text) etc. convenience functions, to
            // preserve BobUI 4.1.x behavior. The widget is already
            // reparented to us due to the createWidget call inside
            // createItem()
            if (widgetAction != nullptr && widgetAction->d_func()->autoCreated)
                widgetAction->setParent(this);

            int index = d->layout->count();
            if (event->before()) {
                index = d->layout->indexOf(event->before());
                Q_ASSERT_X(index != -1, "BOBUIoolBar::insertAction", "internal error");
            }
            d->layout->insertAction(index, action);
            break;
        }

        case QEvent::ActionChanged:
            d->layout->invalidate();
            break;

        case QEvent::ActionRemoved: {
            int index = d->layout->indexOf(action);
            if (index != -1) {
                delete d->layout->takeAt(index);
            }
            break;
        }

        default:
            Q_ASSERT_X(false, "BOBUIoolBar::actionEvent", "internal error");
    }
}

/*! \reimp */
void BOBUIoolBar::changeEvent(QEvent *event)
{
    Q_D(BOBUIoolBar);
    switch (event->type()) {
    case QEvent::WindowTitleChange:
        d->toggleViewAction->setText(windowTitle());
        break;
    case QEvent::StyleChange:
        d->layout->invalidate();
        if (!d->explicitIconSize) {
            QStyleOptionToolBar opt;
            initStyleOption(&opt);
            const int metric = style()->pixelMetric(QStyle::PM_ToolBarIconSize, &opt, this);
            setIconSize({metric, metric});
            d->explicitIconSize = false;
        }
        d->layout->updateMarginAndSpacing();
        break;
    case QEvent::LayoutDirectionChange:
        d->layout->invalidate();
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

/*! \reimp */
void BOBUIoolBar::paintEvent(QPaintEvent *)
{
    Q_D(BOBUIoolBar);

    QPainter p(this);
    QStyle *style = this->style();
    QStyleOptionToolBar opt;
    initStyleOption(&opt);

    if (d->layout->expanded || d->layout->animating || isWindow()) {
        //if the toolbar is expended, we need to fill the background with the window color
        //because some styles may expects that.
        p.fillRect(opt.rect, palette().window());
        style->drawControl(QStyle::CE_ToolBar, &opt, &p, this);
        style->drawPrimitive(QStyle::PE_FrameMenu, &opt, &p, this);
    } else {
        style->drawControl(QStyle::CE_ToolBar, &opt, &p, this);
    }

    opt.rect = style->subElementRect(QStyle::SE_ToolBarHandle, &opt, this);
    if (opt.rect.isValid())
        style->drawPrimitive(QStyle::PE_IndicatorToolBarHandle, &opt, &p, this);
}

/*
    Checks if an expanded toolbar has to wait for this popup to close before
    the toolbar collapses. This is true if
    1) the popup has the toolbar in its parent chain,
    2) the popup is a menu whose menuAction is somewhere in the toolbar.
*/
static bool waitForPopup(BOBUIoolBar *tb, QWidget *popup)
{
    if (popup == nullptr || popup->isHidden())
        return false;

    QWidget *w = popup;
    while (w != nullptr) {
        if (w == tb)
            return true;
        w = w->parentWidget();
    }

    QMenu *menu = qobject_cast<QMenu*>(popup);
    if (menu == nullptr)
        return false;

    const QAction *action = menu->menuAction();
    for (auto object : action->associatedObjects()) {
        if (QWidget *widget = qobject_cast<QWidget*>(object)) {
            if (waitForPopup(tb, widget))
                return true;
        }
    }

    return false;
}

#ifdef Q_OS_MACOS
static void enableMacToolBar(BOBUIoolBar *toolbar, bool enable)
{
    QPlatformNativeInterface *nativeInterface = QApplication::platformNativeInterface();
    if (!nativeInterface)
        return;
    QPlatformNativeInterface::NativeResourceForIntegrationFunction function =
        nativeInterface->nativeResourceFunctionForIntegration("setContentBorderAreaEnabled");
    if (!function)
        return; // Not Cocoa platform plugin.

    typedef void (*SetContentBorderAreaEnabledFunction)(QWindow *window, void *identifier, bool enabled);
    (reinterpret_cast<SetContentBorderAreaEnabledFunction>(QFunctionPointer(function)))(
        toolbar->window()->windowHandle(), toolbar, enable);
}
#endif


/*! \reimp */
bool BOBUIoolBar::event(QEvent *event)
{
    Q_D(BOBUIoolBar);

    switch (event->type()) {
    case QEvent::Timer:
        if (d->waitForPopupTimer.timerId() == static_cast<BOBUIimerEvent*>(event)->timerId()) {
            QWidget *w = QApplication::activePopupWidget();
            if (!waitForPopup(this, w)) {
                d->waitForPopupTimer.stop();
                if (!this->underMouse())
                    d->layout->setExpanded(false);
            }
        }
        break;
    case QEvent::Hide:
        if (!isHidden())
            break;
        Q_FALLTHROUGH();
    case QEvent::Show:
        d->toggleViewAction->setChecked(event->type() == QEvent::Show);
#ifdef Q_OS_MACOS
        enableMacToolBar(this, event->type() == QEvent::Show);
#endif
        emit visibilityChanged(event->type() == QEvent::Show);
        break;
    case QEvent::ParentChange:
        d->layout->checkUsePopupMenu();
        break;

    case QEvent::MouseButtonPress: {
        if (d->mousePressEvent(static_cast<QMouseEvent*>(event)))
            return true;
        break;
    }
    case QEvent::MouseButtonRelease:
        if (d->mouseReleaseEvent(static_cast<QMouseEvent*>(event)))
            return true;
        break;
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
        // there's nothing special to do here and we don't want to update the whole widget
        return true;
    case QEvent::HoverMove: {
#ifndef BOBUI_NO_CURSOR
        QHoverEvent *e = static_cast<QHoverEvent*>(event);
        QStyleOptionToolBar opt;
        initStyleOption(&opt);
        if (style()->subElementRect(QStyle::SE_ToolBarHandle, &opt, this).contains(e->position().toPoint()))
            setCursor(BobUI::SizeAllCursor);
        else
            unsetCursor();
#endif
        break;
    }
    case QEvent::MouseMove:
        if (d->mouseMoveEvent(static_cast<QMouseEvent*>(event)))
            return true;
        break;
    case QEvent::Leave:
        if (d->state != nullptr && d->state->dragging) {
#ifdef Q_OS_WIN
            // This is a workaround for losing the mouse on Vista.
            QPoint pos = QCursor::pos();
            QMouseEvent fake(QEvent::MouseMove, mapFromGlobal(pos), pos, BobUI::NoButton,
                             QGuiApplication::mouseButtons(), QGuiApplication::keyboardModifiers());
            d->mouseMoveEvent(&fake);
#endif
        } else {
            if (!d->layout->expanded)
                break;

            QWidget *w = QApplication::activePopupWidget();
            if (waitForPopup(this, w)) {
                d->waitForPopupTimer.start(POPUP_TIMER_INTERVAL, this);
                break;
            }

            d->waitForPopupTimer.stop();
            d->layout->setExpanded(false);
            break;
        }
        break;
    default:
        break;
    }
    return QWidget::event(event);
}

/*!
    Returns a checkable action that can be used to show or hide this
    toolbar.

    The action's text is set to the toolbar's window title.

    \sa QAction::text, QWidget::windowTitle
*/
QAction *BOBUIoolBar::toggleViewAction() const
{ Q_D(const BOBUIoolBar); return d->toggleViewAction; }

/*!
    \since 4.2

    Returns the widget associated with the specified \a action.

    \sa addWidget()
*/
QWidget *BOBUIoolBar::widgetForAction(QAction *action) const
{
    Q_D(const BOBUIoolBar);

    int index = d->layout->indexOf(action);
    if (index == -1)
        return nullptr;

    return d->layout->itemAt(index)->widget();
}

extern QMainWindowLayout *bobui_mainwindow_layout(const QMainWindow *window);

/*!
    \internal
*/
void BOBUIoolBar::initStyleOption(QStyleOptionToolBar *option) const
{
    Q_D(const BOBUIoolBar);

    if (!option)
        return;

    option->initFrom(this);
    if (orientation() == BobUI::Horizontal)
        option->state |= QStyle::State_Horizontal;
    option->lineWidth = style()->pixelMetric(QStyle::PM_ToolBarFrameWidth, nullptr, this);
    option->features = d->layout->movable()
                        ? QStyleOptionToolBar::Movable
                        : QStyleOptionToolBar::None;
    // if the tool bar is not in a QMainWindow, this will make the painting right
    option->toolBarArea = BobUI::NoToolBarArea;

    // Add more styleoptions if the toolbar has been added to a mainwindow.
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());

    if (!mainWindow)
        return;

    QMainWindowLayout *layout = bobui_mainwindow_layout(mainWindow);
    Q_ASSERT_X(layout != nullptr, "BOBUIoolBar::initStyleOption()",
               "QMainWindow->layout() != QMainWindowLayout");

    layout->getStyleOptionInfo(option, const_cast<BOBUIoolBar *>(this));
}

BOBUI_END_NAMESPACE

#include "moc_bobuioolbar.cpp"
