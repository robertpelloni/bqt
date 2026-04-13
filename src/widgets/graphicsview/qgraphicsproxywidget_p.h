// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSPROXYWIDGET_P_H
#define QGRAPHICSPROXYWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qgraphicsproxywidget.h"
#include "private/qgraphicswidget_p.h"

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QGraphicsProxyWidgetPrivate : public QGraphicsWidgetPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsProxyWidget)
public:
    QGraphicsProxyWidgetPrivate();
    ~QGraphicsProxyWidgetPrivate();

    void init();
    void sendWidgetMouseEvent(QGraphicsSceneMouseEvent *event);
    void sendWidgetMouseEvent(QGraphicsSceneHoverEvent *event);
    void sendWidgetKeyEvent(QKeyEvent *event);
    void setWidget_helper(QWidget *widget, bool autoShow);

    QWidget *findFocusChild(QWidget *child, bool next) const;
    void removeSubFocusHelper(QWidget *widget, BobUI::FocusReason reason);

    void _q_removeWidgetSlot();

    void embedSubWindow(QWidget *);
    void unembedSubWindow(QWidget *);

    bool isProxyWidget() const override;

    QPointer<QWidget> widget;
    QPointer<QWidget> lastWidgetUnderMouse;
    QPointer<QWidget> embeddedMouseGrabber;
    QWidget *dragDropWidget;
    BobUI::DropAction lastDropAction;

    void updateWidgetGeometryFromProxy();
    void updateProxyGeometryFromWidget();

    void updateProxyInputMethodAcceptanceFromWidget();

    QPointF mapToReceiver(const QPointF &pos, const QWidget *receiver) const;

    enum ChangeMode {
        NoMode,
        ProxyToWidgetMode,
        WidgetToProxyMode
    };
    quint32 posChangeMode : 2;
    quint32 sizeChangeMode : 2;
    quint32 visibleChangeMode : 2;
    quint32 enabledChangeMode : 2;
    quint32 styleChangeMode : 2;
    quint32 paletteChangeMode : 2;
    quint32 tooltipChangeMode : 2;
    quint32 focusFromWidgetToProxy : 1;
    quint32 proxyIsGivingFocus : 1;
};

BOBUI_END_NAMESPACE

#endif
