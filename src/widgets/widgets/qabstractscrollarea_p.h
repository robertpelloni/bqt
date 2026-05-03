// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTSCROLLAREA_P_H
#define QABSTRACTSCROLLAREA_P_H

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
#include "private/qframe_p.h"
#include "qabstractscrollarea.h"
#include <BobUIGui/private/qgridlayoutengine_p.h>

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(scrollarea)

class QScrollBar;
class QAbstractScrollAreaScrollBarContainer;

class Q_AUTOTEST_EXPORT QAbstractScrollAreaPrivate: public QFramePrivate
{
    Q_DECLARE_PUBLIC(QAbstractScrollArea)

public:
    QAbstractScrollAreaPrivate();
    ~QAbstractScrollAreaPrivate();

    void replaceScrollBar(QScrollBar *scrollBar, BobUI::Orientation orientation);

    QHVContainer<QAbstractScrollAreaScrollBarContainer *> scrollBarContainers;
    QScrollBar *hbar, *vbar;
    BobUI::ScrollBarPolicy vbarpolicy, hbarpolicy;

    bool shownOnce;
    bool inResize;
    mutable QSize sizeHint;
    QAbstractScrollArea::SizeAdjustPolicy sizeAdjustPolicy;

    QWidget *viewport;
    QWidget *cornerWidget;
    QRect cornerPaintingRect;

    int left, top, right, bottom; // viewport margin

    int xoffset, yoffset;
    QPoint overshoot;

    void init();
    void layoutChildren();
    void layoutChildren_helper(bool *needHorizontalScrollbar, bool *needVerticalScrollbar);
    virtual void scrollBarPolicyChanged(BobUI::Orientation, BobUI::ScrollBarPolicy) {}
    virtual bool canStartScrollingAt( const QPoint &startPos ) const;

    void flashScrollBars();
    void setScrollBarTransient(QScrollBar *scrollBar, bool transient);

    void _q_hslide(int);
    void _q_vslide(int);
    void _q_showOrHideScrollBars();

    virtual QPoint contentsOffset() const;

    inline bool viewportEvent(QEvent *event)
    { return q_func()->viewportEvent(event); }
    QScopedPointer<QObject> viewportFilter;

    int defaultSingleStep() const;
};

class QAbstractScrollAreaFilter : public QObject
{
    Q_OBJECT
public:
    QAbstractScrollAreaFilter(QAbstractScrollAreaPrivate *p) : d(p)
    { setObjectName(QLatin1StringView("bobui_abstractscrollarea_filter")); }
    bool eventFilter(QObject *o, QEvent *e) override
    { return (o == d->viewport ? d->viewportEvent(e) : false); }
private:
    QAbstractScrollAreaPrivate *d;
};

class QBoxLayout;
class QAbstractScrollAreaScrollBarContainer : public QWidget
{
public:
    enum LogicalPosition { LogicalLeft = 1, LogicalRight = 2 };

    QAbstractScrollAreaScrollBarContainer(BobUI::Orientation orientation, QWidget *parent);
    void addWidget(QWidget *widget, LogicalPosition position);
    QWidgetList widgets(LogicalPosition position);
    void removeWidget(QWidget *widget);

    QScrollBar *scrollBar;
    QBoxLayout *layout;
private:
    int scrollBarLayoutIndex() const;

    BobUI::Orientation orientation;
};

#endif // BOBUI_CONFIG(scrollarea)

BOBUI_END_NAMESPACE

#endif // QABSTRACTSCROLLAREA_P_H
