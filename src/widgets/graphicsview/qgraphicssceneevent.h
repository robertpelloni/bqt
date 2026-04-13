// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSSCENEEVENT_H
#define QGRAPHICSSCENEEVENT_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qcoreevent.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qscopedpointer.h>
#include <BobUICore/qrect.h>
#include <BobUIGui/qpolygon.h>
#include <BobUICore/qset.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QMimeData;
class QPointF;
class QSizeF;
class QWidget;

class QGraphicsSceneEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneEvent : public QEvent
{
public:
    explicit QGraphicsSceneEvent(Type type);
    ~QGraphicsSceneEvent();

    QWidget *widget() const;
    void setWidget(QWidget *widget);

    quint64 timestamp() const;
    void setTimestamp(quint64 ts);

protected:
    QGraphicsSceneEvent(QGraphicsSceneEventPrivate &dd, Type type = None);
    QScopedPointer<QGraphicsSceneEventPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QGraphicsSceneEvent)
private:
    Q_DISABLE_COPY(QGraphicsSceneEvent)
};

class QGraphicsSceneMouseEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneMouseEvent : public QGraphicsSceneEvent
{
public:
    explicit QGraphicsSceneMouseEvent(Type type = None);
    ~QGraphicsSceneMouseEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    QPointF buttonDownPos(BobUI::MouseButton button) const;
    void setButtonDownPos(BobUI::MouseButton button, const QPointF &pos);

    QPointF buttonDownScenePos(BobUI::MouseButton button) const;
    void setButtonDownScenePos(BobUI::MouseButton button, const QPointF &pos);

    QPoint buttonDownScreenPos(BobUI::MouseButton button) const;
    void setButtonDownScreenPos(BobUI::MouseButton button, const QPoint &pos);

    QPointF lastPos() const;
    void setLastPos(const QPointF &pos);

    QPointF lastScenePos() const;
    void setLastScenePos(const QPointF &pos);

    QPoint lastScreenPos() const;
    void setLastScreenPos(const QPoint &pos);

    BobUI::MouseButtons buttons() const;
    void setButtons(BobUI::MouseButtons buttons);

    BobUI::MouseButton button() const;
    void setButton(BobUI::MouseButton button);

    BobUI::KeyboardModifiers modifiers() const;
    void setModifiers(BobUI::KeyboardModifiers modifiers);

    BobUI::MouseEventSource source() const;
    void setSource(BobUI::MouseEventSource source);

    BobUI::MouseEventFlags flags() const;
    void setFlags(BobUI::MouseEventFlags);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneMouseEvent)
    Q_DISABLE_COPY(QGraphicsSceneMouseEvent)
};

class QGraphicsSceneWheelEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneWheelEvent : public QGraphicsSceneEvent
{
public:
    explicit QGraphicsSceneWheelEvent(Type type = None);
    ~QGraphicsSceneWheelEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    BobUI::MouseButtons buttons() const;
    void setButtons(BobUI::MouseButtons buttons);

    BobUI::KeyboardModifiers modifiers() const;
    void setModifiers(BobUI::KeyboardModifiers modifiers);

    int delta() const;
    void setDelta(int delta);

    BobUI::Orientation orientation() const;
    void setOrientation(BobUI::Orientation orientation);

    BobUI::ScrollPhase phase() const;
    void setPhase(BobUI::ScrollPhase scrollPhase);

    QPoint pixelDelta() const;
    void setPixelDelta(QPoint delta);

    bool isInverted() const;
    void setInverted(bool inverted);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneWheelEvent)
    Q_DISABLE_COPY(QGraphicsSceneWheelEvent)
};

class QGraphicsSceneContextMenuEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneContextMenuEvent : public QGraphicsSceneEvent
{
public:
    enum Reason { Mouse, Keyboard, Other };

    explicit QGraphicsSceneContextMenuEvent(Type type = None);
    ~QGraphicsSceneContextMenuEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    BobUI::KeyboardModifiers modifiers() const;
    void setModifiers(BobUI::KeyboardModifiers modifiers);

    Reason reason() const;
    void setReason(Reason reason);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneContextMenuEvent)
    Q_DISABLE_COPY(QGraphicsSceneContextMenuEvent)
};

class QGraphicsSceneHoverEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneHoverEvent : public QGraphicsSceneEvent
{
public:
    explicit QGraphicsSceneHoverEvent(Type type = None);
    ~QGraphicsSceneHoverEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    QPointF lastPos() const;
    void setLastPos(const QPointF &pos);

    QPointF lastScenePos() const;
    void setLastScenePos(const QPointF &pos);

    QPoint lastScreenPos() const;
    void setLastScreenPos(const QPoint &pos);

    BobUI::KeyboardModifiers modifiers() const;
    void setModifiers(BobUI::KeyboardModifiers modifiers);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneHoverEvent)
    Q_DISABLE_COPY(QGraphicsSceneHoverEvent)
};

class QGraphicsSceneHelpEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneHelpEvent : public QGraphicsSceneEvent
{
public:
    explicit QGraphicsSceneHelpEvent(Type type = None);
    ~QGraphicsSceneHelpEvent();

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneHelpEvent)
    Q_DISABLE_COPY(QGraphicsSceneHelpEvent)
};

class QGraphicsSceneDragDropEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneDragDropEvent : public QGraphicsSceneEvent
{
public:
    explicit QGraphicsSceneDragDropEvent(Type type = None);
    ~QGraphicsSceneDragDropEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    BobUI::MouseButtons buttons() const;
    void setButtons(BobUI::MouseButtons buttons);

    BobUI::KeyboardModifiers modifiers() const;
    void setModifiers(BobUI::KeyboardModifiers modifiers);

    BobUI::DropActions possibleActions() const;
    void setPossibleActions(BobUI::DropActions actions);

    BobUI::DropAction proposedAction() const;
    void setProposedAction(BobUI::DropAction action);
    void acceptProposedAction();

    BobUI::DropAction dropAction() const;
    void setDropAction(BobUI::DropAction action);

    QWidget *source() const;
    void setSource(QWidget *source);

    const QMimeData *mimeData() const;
    void setMimeData(const QMimeData *data);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneDragDropEvent)
    Q_DISABLE_COPY(QGraphicsSceneDragDropEvent)
};

class QGraphicsSceneResizeEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneResizeEvent : public QGraphicsSceneEvent
{
    Q_DECLARE_PRIVATE(QGraphicsSceneResizeEvent)
    Q_DISABLE_COPY(QGraphicsSceneResizeEvent)
public:
    QGraphicsSceneResizeEvent();
    ~QGraphicsSceneResizeEvent();

    QSizeF oldSize() const;
    void setOldSize(const QSizeF &size);

    QSizeF newSize() const;
    void setNewSize(const QSizeF &size);
};

class QGraphicsSceneMoveEventPrivate;
class Q_WIDGETS_EXPORT QGraphicsSceneMoveEvent : public QGraphicsSceneEvent
{
    Q_DECLARE_PRIVATE(QGraphicsSceneMoveEvent)
    Q_DISABLE_COPY(QGraphicsSceneMoveEvent)
public:
    QGraphicsSceneMoveEvent();
    ~QGraphicsSceneMoveEvent();

    QPointF oldPos() const;
    void setOldPos(const QPointF &pos);

    QPointF newPos() const;
    void setNewPos(const QPointF &pos);
};

#ifndef BOBUI_NO_DEBUG_STREAM
Q_WIDGETS_EXPORT QDebug operator<<(QDebug, const QGraphicsSceneEvent *);
#endif

BOBUI_END_NAMESPACE

#endif
