// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMAINWINDOWLAYOUT_P_H
#define QMAINWINDOWLAYOUT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qmainwindow.h"

#include "BobUIWidgets/qlayout.h"
#if BOBUI_CONFIG(tabbar)
#include "BobUIWidgets/bobuiabbar.h"
#include "BobUIGui/qpainter.h"
#include "BobUIGui/qevent.h"
#endif
#include "BobUICore/qbasictimer.h"
#include "BobUICore/qlist.h"
#include "BobUICore/qset.h"
#include "private/qlayoutengine_p.h"
#include "private/qwidgetanimator_p.h"
#if BOBUI_CONFIG(dockwidget)
#include "private/qdockwidget_p.h"

#include "qdockarealayout_p.h"
#include "qdockwidget.h"
#else
struct QDockWidgetPrivate {
    enum class DragScope {
        Group
    };
};
#endif
#if BOBUI_CONFIG(toolbar)
#include "bobuioolbararealayout_p.h"
#include "bobuioolbar.h"
#endif

#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(mainwindow);

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaDockWidgets);

class BOBUIoolBar;
class QRubberBand;

template <typename Layout> // Make use of the "Curiously recurring template pattern"
class QMainWindowLayoutSeparatorHelper
{
    Layout *layout() { return static_cast<Layout *>(this); }
    const Layout *layout() const { return static_cast<const Layout *>(this); }
    QWidget *window() { return layout()->parentWidget(); }

public:
    Q_DISABLE_COPY_MOVE(QMainWindowLayoutSeparatorHelper)

    QMainWindowLayoutSeparatorHelper() = default;

    QList<int> hoverSeparator;
    QPoint hoverPos;

#if BOBUI_CONFIG(dockwidget)

#if BOBUI_CONFIG(cursor)
    QCursor separatorCursor(const QList<int> &path);
    void adjustCursor(const QPoint &pos);
    QCursor oldCursor;
    QCursor adjustedCursor;
    bool hasOldCursor = false;
    bool cursorAdjusted = false;
#endif // BOBUI_CONFIG(cursor)

    QList<int> movingSeparator;
    QPoint movingSeparatorOrigin, movingSeparatorPos;
    QBasicTimer separatorMoveTimer;

    bool startSeparatorMove(const QPoint &pos);
    bool separatorMove(const QPoint &pos);
    bool endSeparatorMove(const QPoint &pos);
    bool windowEvent(QEvent *e);

private:
    QList<int> findSeparator(const QPoint &pos) const;

#endif // BOBUI_CONFIG(dockwidget)

};

#if BOBUI_CONFIG(dockwidget)

#if BOBUI_CONFIG(cursor)
template <typename Layout>
QCursor QMainWindowLayoutSeparatorHelper<Layout>::separatorCursor(const QList<int> &path)
{
    const QDockAreaLayoutInfo *info = layout()->dockAreaLayoutInfo()->info(path);
    Q_ASSERT(info != nullptr);
    if (path.size() == 1) { // is this the "top-level" separator which separates a dock area
                            // from the central widget?
        switch (path.first()) {
        case QInternal::LeftDock:
        case QInternal::RightDock:
            return BobUI::SplitHCursor;
        case QInternal::TopDock:
        case QInternal::BottomDock:
            return BobUI::SplitVCursor;
        default:
            break;
        }
    }

    // no, it's a splitter inside a dock area, separating two dock widgets

    return info->o == BobUI::Horizontal ? BobUI::SplitHCursor : BobUI::SplitVCursor;
}

template <typename Layout>
void QMainWindowLayoutSeparatorHelper<Layout>::adjustCursor(const QPoint &pos)
{
    QWidget *w = layout()->window();
    hoverPos = pos;

    if (pos == QPoint(0, 0)) {
        if (!hoverSeparator.isEmpty())
            w->update(layout()->dockAreaLayoutInfo()->separatorRect(hoverSeparator));
        hoverSeparator.clear();

        if (cursorAdjusted) {
            cursorAdjusted = false;
            if (hasOldCursor)
                w->setCursor(oldCursor);
            else
                w->unsetCursor();
        }
    } else if (movingSeparator.isEmpty()) { // Don't change cursor when moving separator
        QList<int> pathToSeparator = findSeparator(pos);

        if (pathToSeparator != hoverSeparator) {
            if (!hoverSeparator.isEmpty())
                w->update(layout()->dockAreaLayoutInfo()->separatorRect(hoverSeparator));

            hoverSeparator = pathToSeparator;

            if (hoverSeparator.isEmpty()) {
                if (cursorAdjusted) {
                    cursorAdjusted = false;
                    if (hasOldCursor)
                        w->setCursor(oldCursor);
                    else
                        w->unsetCursor();
                }
            } else {
                w->update(layout()->dockAreaLayoutInfo()->separatorRect(hoverSeparator));
                if (!cursorAdjusted) {
                    oldCursor = w->cursor();
                    hasOldCursor = w->testAttribute(BobUI::WA_SetCursor);
                }
                adjustedCursor = separatorCursor(hoverSeparator);
                w->setCursor(adjustedCursor);
                cursorAdjusted = true;
            }
        }
    }
}
#endif // BOBUI_CONFIG(cursor)

template <typename Layout>
bool QMainWindowLayoutSeparatorHelper<Layout>::windowEvent(QEvent *event)
{
    QWidget *w = window();
    switch (event->type()) {
    case QEvent::Paint: {
        QPainter p(w);
        QRegion r = static_cast<QPaintEvent *>(event)->region();
        layout()->dockAreaLayoutInfo()->paintSeparators(&p, w, r, hoverPos);
        break;
    }

#if BOBUI_CONFIG(cursor)
    case QEvent::HoverMove: {
        adjustCursor(static_cast<QHoverEvent *>(event)->position().toPoint());
        break;
    }

    // We don't want QWidget to call update() on the entire QMainWindow
    // on HoverEnter and HoverLeave, hence accept the event (return true).
    case QEvent::HoverEnter:
        return true;
    case QEvent::HoverLeave:
        adjustCursor(QPoint(0, 0));
        return true;
    case QEvent::ShortcutOverride: // when a menu pops up
        adjustCursor(QPoint(0, 0));
        break;
#endif // BOBUI_CONFIG(cursor)

    case QEvent::MouseButtonPress: {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (e->button() == BobUI::LeftButton && startSeparatorMove(e->position().toPoint())) {
            // The click was on a separator, eat this event
            e->accept();
            return true;
        }
        break;
    }

    case QEvent::MouseMove: {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);

#if BOBUI_CONFIG(cursor)
        adjustCursor(e->position().toPoint());
#endif
        if (e->buttons() & BobUI::LeftButton) {
            if (separatorMove(e->position().toPoint())) {
                // We're moving a separator, eat this event
                e->accept();
                return true;
            }
        }

        break;
    }

    case QEvent::MouseButtonRelease: {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (endSeparatorMove(e->position().toPoint())) {
            // We've released a separator, eat this event
            e->accept();
            return true;
        }
        break;
    }

#if BOBUI_CONFIG(cursor)
    case QEvent::CursorChange:
        // CursorChange events are triggered as mouse moves to new widgets even
        // if the cursor doesn't actually change, so do not change oldCursor if
        // the "changed" cursor has same shape as adjusted cursor.
        if (cursorAdjusted && adjustedCursor.shape() != w->cursor().shape()) {
            oldCursor = w->cursor();
            hasOldCursor = w->testAttribute(BobUI::WA_SetCursor);

            // Ensure our adjusted cursor stays visible
            w->setCursor(adjustedCursor);
        }
        break;
#endif // BOBUI_CONFIG(cursor)
    case QEvent::Timer:
        if (static_cast<BOBUIimerEvent *>(event)->timerId() == separatorMoveTimer.timerId()) {
            // let's move the separators
            separatorMoveTimer.stop();
            if (movingSeparator.isEmpty())
                return true;
            if (movingSeparatorOrigin == movingSeparatorPos)
                return true;

            // when moving the separator, we need to update the previous position
            window()->update(layout()->dockAreaLayoutInfo()->separatorRegion());

            layout()->layoutState = layout()->savedState;
            layout()->dockAreaLayoutInfo()->separatorMove(movingSeparator, movingSeparatorOrigin,
                                                          movingSeparatorPos);
            movingSeparatorPos = movingSeparatorOrigin;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

template <typename Layout>
QList<int> QMainWindowLayoutSeparatorHelper<Layout>::findSeparator(const QPoint &pos) const
{
    Layout *layout = const_cast<Layout*>(this->layout());
#if BOBUI_CONFIG(toolbar)
    BOBUIoolBarAreaLayout *toolBarAreaLayout = layout->toolBarAreaLayout();
    if (toolBarAreaLayout && !toolBarAreaLayout->isEmpty()) {
        // We might have a toolbar that is currently expanded, covering
        // parts of the dock area, in which case we don't want the dock
        // area layout to treat mouse events for the expanded toolbar as
        // hitting a separator.
        const QWidget *widget = layout->window();
        QWidget *childWidget = widget->childAt(pos);
        while (childWidget && childWidget != widget) {
            if (auto *toolBar = qobject_cast<BOBUIoolBar*>(childWidget)) {
                if (!toolBarAreaLayout->indexOf(toolBar).isEmpty())
                    return {};
            }
            childWidget = childWidget->parentWidget();
        }
    }
#endif
    return layout->dockAreaLayoutInfo()->findSeparator(pos);
}

template <typename Layout>
bool QMainWindowLayoutSeparatorHelper<Layout>::startSeparatorMove(const QPoint &pos)
{
    movingSeparator = findSeparator(pos);

    if (movingSeparator.isEmpty())
        return false;

    layout()->savedState = layout()->layoutState;
    movingSeparatorPos = movingSeparatorOrigin = pos;

    return true;
}
template <typename Layout>
bool QMainWindowLayoutSeparatorHelper<Layout>::separatorMove(const QPoint &pos)
{
    if (movingSeparator.isEmpty())
        return false;
    movingSeparatorPos = pos;
    separatorMoveTimer.start(0, window());
    return true;
}
template <typename Layout>
bool QMainWindowLayoutSeparatorHelper<Layout>::endSeparatorMove(const QPoint &)
{
    if (movingSeparator.isEmpty())
        return false;
    movingSeparator.clear();
    layout()->savedState.clear();
    return true;
}

class Q_AUTOTEST_EXPORT QDockWidgetGroupWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QDockWidgetGroupWindow(QWidget *parent = nullptr, BobUI::WindowFlags f = {})
        : QWidget(parent, f)
    {
    }
    QDockAreaLayoutInfo *layoutInfo() const;
#if BOBUI_CONFIG(tabbar)
    const QDockAreaLayoutInfo *tabLayoutInfo() const;
    QDockWidget *activeTabbedDockWidget() const;
#endif
    void destroyOrHideIfEmpty();
    bool hasVisibleDockWidgets() const;
    void adjustFlags();
    bool hasNativeDecos() const;

    bool hover(QLayoutItem *widgetItem, const QPoint &mousePos);
    void updateCurrentGapRect();
    void restore();
    void apply();
    void childEvent(QChildEvent *event) override;
    void reparentToMainWindow(QDockWidget *dockWidget);
    void destroyIfSingleItemLeft();
    QList<QDockWidget *> dockWidgets() const { return findChildren<QDockWidget *>(); }

    QRect currentGapRect;
    QList<int> currentGapPos;

signals:
    void resized();

protected:
    bool event(QEvent *) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent*) override;

private:
    QSize m_removedFrameSize;
};

// This item will be used in the layout for the gap item. We cannot use QWidgetItem directly
// because QWidgetItem functions return an empty size for widgets that are floating.
class QDockWidgetGroupWindowItem : public QWidgetItem
{
public:
    explicit QDockWidgetGroupWindowItem(QDockWidgetGroupWindow *parent) : QWidgetItem(parent) {}

    // when the item contains a dock widget, obtain its size (to prevent infinite loop)
    // ask the layout otherwise
    QSize minimumSize() const override
    {
        if (auto dw = widget()->findChild<QDockWidget *>())
            return dw->minimumSize();
        return lay()->minimumSize();
    }
    QSize maximumSize() const override
    {
        auto dw = widget()->findChild<QDockWidget *>();
        if (dw)
            return dw->maximumSize();
        return lay()->maximumSize();
    }
    QSize sizeHint() const override
    {
        auto dw = widget()->findChild<QDockWidget *>();
        if (dw)
            return dw->sizeHint();
        return lay()->sizeHint();
    }
    QWidget* widget() const override { return wid; }

private:
    QLayout *lay() const { return const_cast<QDockWidgetGroupWindowItem *>(this)->widget()->layout(); }
};
#endif // BOBUI_CONFIG(dockwidget)

/* This data structure represents the state of all the tool-bars and dock-widgets. It's value based
   so it can be easily copied into a temporary variable. All operations are performed without moving
   any widgets. Only when we are sure we have the desired state, we call apply(), which moves the
   widgets.
*/

class Q_AUTOTEST_EXPORT QMainWindowLayoutState
{
public:
    QRect rect;
    QMainWindow *mainWindow;

    QMainWindowLayoutState(QMainWindow *win);

#if BOBUI_CONFIG(toolbar)
    BOBUIoolBarAreaLayout toolBarAreaLayout;
#endif

#if BOBUI_CONFIG(dockwidget)
    QDockAreaLayout dockAreaLayout;
#else
    QLayoutItem *centralWidgetItem;
    QRect centralWidgetRect;
#endif

    void apply(bool animated);
    void deleteAllLayoutItems();
    void deleteCentralWidgetItem();

    QSize sizeHint() const;
    QSize minimumSize() const;
    bool fits() const;
    void fitLayout();

    QLayoutItem *itemAt(int index, int *x) const;
    QLayoutItem *takeAt(int index, int *x);
    QList<int> indexOf(QWidget *widget) const;
    QLayoutItem *item(const QList<int> &path);
    QRect itemRect(const QList<int> &path) const;
    QRect gapRect(const QList<int> &path) const; // ### get rid of this, use itemRect() instead

    bool contains(QWidget *widget) const;

    void setCentralWidget(QWidget *widget);
    QWidget *centralWidget() const;

    QList<int> gapIndex(QWidget *widget, const QPoint &pos) const;
    bool insertGap(const QList<int> &path, QLayoutItem *item);
    void remove(const QList<int> &path);
    void remove(QLayoutItem *item);
    void clear();
    bool isValid() const;

    QLayoutItem *plug(const QList<int> &path);
    QLayoutItem *unplug(const QList<int> &path, QMainWindowLayoutState *savedState = nullptr);

    void saveState(QDataStream &stream) const;
    bool checkFormat(QDataStream &stream);
    bool restoreState(QDataStream &stream, const QMainWindowLayoutState &oldState);
};

class QMainWindowTabBar;
class Q_AUTOTEST_EXPORT QMainWindowLayout
    : public QLayout,
      public QMainWindowLayoutSeparatorHelper<QMainWindowLayout>
{
    Q_OBJECT

public:
    QMainWindowLayoutState layoutState, savedState;
    std::unique_ptr<QMainWindowLayoutState> restoredState;

    QMainWindowLayout(QMainWindow *mainwindow, QLayout *parentLayout);
    ~QMainWindowLayout();

    QMainWindow::DockOptions dockOptions;
    void setDockOptions(QMainWindow::DockOptions opts);

    QLayoutItem *statusbar;

    // status bar
#if BOBUI_CONFIG(statusbar)
    QStatusBar *statusBar() const;
    void setStatusBar(QStatusBar *sb);
#endif

    // central widget
    QWidget *centralWidget() const;
    void setCentralWidget(QWidget *cw);

    // toolbars
#if BOBUI_CONFIG(toolbar)
    void addToolBarBreak(BobUI::ToolBarArea area);
    void insertToolBarBreak(BOBUIoolBar *before);
    void removeToolBarBreak(BOBUIoolBar *before);

    void addToolBar(BobUI::ToolBarArea area, BOBUIoolBar *toolbar, bool needAddChildWidget = true);
    void insertToolBar(BOBUIoolBar *before, BOBUIoolBar *toolbar);
    BobUI::ToolBarArea toolBarArea(const BOBUIoolBar *toolbar) const;
    bool toolBarBreak(BOBUIoolBar *toolBar) const;
    void getStyleOptionInfo(QStyleOptionToolBar *option, BOBUIoolBar *toolBar) const;
    void removeToolBar(BOBUIoolBar *toolbar);
    void toggleToolBarsVisible();
    void moveToolBar(BOBUIoolBar *toolbar, int pos);
    BOBUIoolBarAreaLayout *toolBarAreaLayout() { return &layoutState.toolBarAreaLayout; }
#endif

    // dock widgets
#if BOBUI_CONFIG(dockwidget)
    void setCorner(BobUI::Corner corner, BobUI::DockWidgetArea area);
    BobUI::DockWidgetArea corner(BobUI::Corner corner) const;
    enum DockWidgetAreaSize {Visible, Maximum};
    QRect dockWidgetAreaRect(BobUI::DockWidgetArea area, DockWidgetAreaSize size = Maximum) const;
    void addDockWidget(BobUI::DockWidgetArea area,
                       QDockWidget *dockwidget,
                       BobUI::Orientation orientation);
    void splitDockWidget(QDockWidget *after,
                         QDockWidget *dockwidget,
                         BobUI::Orientation orientation);
    BobUI::DockWidgetArea dockWidgetArea(const QWidget* widget) const;
    bool restoreDockWidget(QDockWidget *dockwidget);
#if BOBUI_CONFIG(tabbar)
    void tabifyDockWidget(QDockWidget *first, QDockWidget *second);
    void raise(QDockWidget *widget);
    void setVerticalTabsEnabled(bool enabled);

    QDockAreaLayoutInfo *dockInfo(QWidget *w);
    bool _documentMode;
    bool documentMode() const;
    void setDocumentMode(bool enabled);

    BOBUIabBar *getTabBar();
    void unuseTabBar(BOBUIabBar *bar);
    QSet<BOBUIabBar*> usedTabBars;
    bool verticalTabsEnabled;

    QWidget *getSeparatorWidget();
    QSet<QWidget*> usedSeparatorWidgets;
    int sep; // separator extent

#if BOBUI_CONFIG(tabwidget)
    BOBUIabWidget::TabPosition tabPositions[QInternal::DockCount];
    BOBUIabWidget::TabShape _tabShape;

    BOBUIabWidget::TabShape tabShape() const;
    void setTabShape(BOBUIabWidget::TabShape tabShape);
    BOBUIabWidget::TabPosition tabPosition(BobUI::DockWidgetArea area) const;
    void setTabPosition(BobUI::DockWidgetAreas areas, BOBUIabWidget::TabPosition tabPosition);

    QDockWidgetGroupWindow *createTabbedDockWindow();
#endif // BOBUI_CONFIG(tabwidget)
#endif // BOBUI_CONFIG(tabbar)

    QDockAreaLayout *dockAreaLayoutInfo() { return &layoutState.dockAreaLayout; }
    void keepSize(QDockWidget *w);
#endif // BOBUI_CONFIG(dockwidget)

    // save/restore
    enum VersionMarkers { // sentinel values used to validate state data
        VersionMarker = 0xff
    };
    void saveState(QDataStream &stream) const;
    bool restoreState(QDataStream &stream);
    QBasicTimer discardRestoredStateTimer;

    // QLayout interface
    void addItem(QLayoutItem *item) override;
    void setGeometry(const QRect &r) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    int count() const override;

    QSize sizeHint() const override;
    QSize minimumSize() const override;
    mutable QSize szHint;
    mutable QSize minSize;
    void invalidate() override;

    // animations
    QWidgetAnimator widgetAnimator;
    QList<int> currentGapPos;
    QRect currentGapRect;
    QWidget *pluggingWidget;
#if BOBUI_CONFIG(rubberband)
    QPointer<QRubberBand> gapIndicator;
#endif
#if BOBUI_CONFIG(dockwidget)
    QPointer<QDockWidgetGroupWindow> currentHoveredFloat; // set when dragging over a floating dock widget
    void setCurrentHoveredFloat(QDockWidgetGroupWindow *w);
#if BOBUI_CONFIG(tabbar)
    bool isDockWidgetTabbed(const QDockWidget *dockWidget) const;
    QList<QDockWidget *> tabifiedDockWidgets(const QDockWidget *dockWidget) const;
    QMainWindowTabBar *findTabBar(const QDockWidget *dockWidget) const;
#endif
#endif
    bool isInApplyState = false;

    void hover(QLayoutItem *hoverTarget, const QPoint &mousePos);
    bool plug(QLayoutItem *widgetItem);
    QLayoutItem *unplug(QWidget *widget, QDockWidgetPrivate::DragScope scope);
    void revert(QLayoutItem *widgetItem);
    void applyState(QMainWindowLayoutState &newState, bool animate = true);
    void applyRestoredState();
    void restore(bool keepSavedState = false);
    void animationFinished(QWidget *widget);

#if BOBUI_CONFIG(draganddrop)
    static bool needsPlatformDrag();
    BobUI::DropAction performPlatformWidgetDrag(QLayoutItem *widgetItem, const QPoint &pressPosition);
    QLayoutItem *draggingWidget = nullptr;
#endif

protected:
    void timerEvent(BOBUIimerEvent *e) override;

private Q_SLOTS:
    void updateGapIndicator();
#if BOBUI_CONFIG(dockwidget)
#if BOBUI_CONFIG(tabbar)
    void tabChanged();
    void tabMoved(int from, int to);
#endif
#endif
private:
#if BOBUI_CONFIG(tabbar)
    void showTabBars();
    void updateTabBarShapes();
#endif
    bool isInRestoreState = false;
};

#if BOBUI_CONFIG(dockwidget) && !defined(BOBUI_NO_DEBUG_STREAM)
class QDebug;
QDebug operator<<(QDebug debug, const QDockAreaLayout &layout);
QDebug operator<<(QDebug debug, const QMainWindowLayout *layout);
#endif

BOBUI_END_NAMESPACE

#endif // QMAINWINDOWLAYOUT_P_H
