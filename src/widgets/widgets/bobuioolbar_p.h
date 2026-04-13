// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDYNAMICTOOLBAR_P_H
#define QDYNAMICTOOLBAR_P_H

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
#include "bobuioolbar.h"
#include "BobUIGui/qaction.h"
#include "private/qwidget_p.h"
#include <BobUICore/qbasictimer.h>

BOBUI_REQUIRE_CONFIG(toolbar);

BOBUI_BEGIN_NAMESPACE

class BOBUIoolBarLayout;

class BOBUIoolBarPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(BOBUIoolBar)

public:
    inline BOBUIoolBarPrivate()
        : explicitIconSize(false), explicitToolButtonStyle(false), movable(true), floatable(true),
          allowedAreas(BobUI::AllToolBarAreas), orientation(BobUI::Horizontal),
          toolButtonStyle(BobUI::ToolButtonIconOnly),
          layout(nullptr), state(nullptr)
#ifdef Q_OS_MACOS
        , macWindowDragging(false)
#endif
    { }

    void init();
    void actionTriggered();
    void _q_toggleView(bool b);
    void _q_updateIconSize(const QSize &sz);
    void _q_updateToolButtonStyle(BobUI::ToolButtonStyle style);

    bool explicitIconSize;
    bool explicitToolButtonStyle;
    bool movable;
    bool floatable;
    BobUI::ToolBarAreas allowedAreas;
    BobUI::Orientation orientation;
    BobUI::ToolButtonStyle toolButtonStyle;
    QSize iconSize;

    QAction *toggleViewAction;

    BOBUIoolBarLayout *layout;

    struct DragState {
        QPoint pressPos;
        bool dragging;
        bool moving;
        QLayoutItem *widgetItem;
    };
    DragState *state;

#ifdef Q_OS_MACOS
    bool macWindowDragging;
    QPoint macWindowDragPressPosition;
#endif

    bool mousePressEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);

    void updateWindowFlags(bool floating, bool unplug = false);
    void setWindowState(bool floating, bool unplug = false, const QRect &rect = QRect());
    void initDrag(const QPoint &pos);
    void startDrag(bool moving = false);
    void endDrag();

    void unplug(const QRect &r);
    void plug(const QRect &r);

    QBasicTimer waitForPopupTimer;
};

BOBUI_END_NAMESPACE

#endif // QDYNAMICTOOLBAR_P_H
