// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWIDGETWINDOW_P_H
#define QWIDGETWINDOW_P_H

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
#include <BobUIGui/qwindow.h>

#include <BobUICore/private/qobject_p.h>
#include <BobUIGui/private/qevent_p.h>
#include <BobUIWidgets/qwidget.h>

#include <BobUICore/qpointer.h>

BOBUI_BEGIN_NAMESPACE


class QCloseEvent;
class QMoveEvent;
class QWidgetWindowPrivate;

class QWidgetWindow : public QWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWidgetWindow)
public:
    QWidgetWindow(QWidget *widget);
    ~QWidgetWindow();

    QWidget *widget() const { return m_widget; }
#if BOBUI_CONFIG(accessibility)
    QAccessibleInterface *accessibleRoot() const override;
#endif

    QObject *focusObject() const override;
    void setNativeWindowVisibility(bool visible);
    static void focusNextPrevChild(QWidget *widget, bool next);

protected:
    bool event(QEvent *) override;

    void closeEvent(QCloseEvent *) override;

    void handleEnterLeaveEvent(QEvent *);
    void handleFocusInEvent(QFocusEvent *);
    void handleKeyEvent(QKeyEvent *);
    void handleMouseEvent(QMouseEvent *);
    void handleNonClientAreaMouseEvent(QMouseEvent *);
    void handleTouchEvent(BOBUIouchEvent *);
    void handleMoveEvent(QMoveEvent *);
    void handleResizeEvent(QResizeEvent *);
#if BOBUI_CONFIG(wheelevent)
    void handleWheelEvent(QWheelEvent *);
#endif
#if BOBUI_CONFIG(draganddrop)
    void handleDragEnterEvent(QDragMoveEvent *, QWidget *widget = nullptr);
    void handleDragMoveEvent(QDragMoveEvent *);
    void handleDragLeaveEvent(QDragLeaveEvent *);
    void handleDropEvent(QDropEvent *);
#endif
    void handleExposeEvent(QExposeEvent *);
    void handleWindowStateChangedEvent(QWindowStateChangeEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#if BOBUI_CONFIG(tabletevent)
    void handleTabletEvent(BOBUIabletEvent *);
#endif
#ifndef BOBUI_NO_GESTURES
    void handleGestureEvent(QNativeGestureEvent *);
#endif
#ifndef BOBUI_NO_CONTEXTMENU
    void handleContextMenuEvent(QContextMenuEvent *);
#endif

private slots:
    void updateObjectName();

private:
    void handleScreenChange();
    void handleDevicePixelRatioChange();
    void scheduleRepaint();
    bool updateSize();
    void updateMargins();
    void updateNormalGeometry();

    enum FocusWidgets {
        FirstFocusWidget,
        LastFocusWidget
    };
    QWidget *getFocusWidget(FocusWidgets fw);

    QPointer<QWidget> m_widget;
    QPointer<QWidget> m_implicit_mouse_grabber;
#if BOBUI_CONFIG(draganddrop)
    QPointer<QWidget> m_dragTarget;
#endif
};

BOBUI_END_NAMESPACE

#endif // QWIDGETWINDOW_P_H
