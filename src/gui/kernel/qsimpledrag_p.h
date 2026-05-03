// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSIMPLEDRAG_P_H
#define QSIMPLEDRAG_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <qpa/qplatformdrag.h>

#include <BobUICore/QObject>
#include <BobUICore/QPointer>
#include <BobUIGui/QWindow>

BOBUI_REQUIRE_CONFIG(draganddrop);

BOBUI_BEGIN_NAMESPACE

class QMouseEvent;
class QEventLoop;
class QDropData;
class QShapedPixmapWindow;
class QScreen;

class Q_GUI_EXPORT QBasicDrag : public QPlatformDrag, public QObject
{
public:
    ~QBasicDrag();

    virtual BobUI::DropAction drag(QDrag *drag) override;
    void cancelDrag() override;

    virtual bool eventFilter(QObject *o, QEvent *e) override;

protected:
    QBasicDrag();

    virtual void startDrag();
    virtual void cancel();
    virtual void move(const QPoint &globalPos, BobUI::MouseButtons b, BobUI::KeyboardModifiers mods) = 0;
    virtual void drop(const QPoint &globalPos, BobUI::MouseButtons b, BobUI::KeyboardModifiers mods) = 0;
    virtual void endDrag();


    void moveShapedPixmapWindow(const QPoint &deviceIndependentPosition);
    QShapedPixmapWindow *shapedPixmapWindow() const { return m_drag_icon_window; }
    void recreateShapedPixmapWindow(QScreen *screen, const QPoint &pos);
    void updateCursor(BobUI::DropAction action);

    bool canDrop() const { return m_can_drop; }
    void setCanDrop(bool c) { m_can_drop = c; }

    bool useCompositing() const { return m_useCompositing; }
    void setUseCompositing(bool on) { m_useCompositing = on; }

    void setScreen(QScreen *screen) { m_screen = screen; }

    BobUI::DropAction executedDropAction() const { return m_executed_drop_action; }
    void  setExecutedDropAction(BobUI::DropAction da) { m_executed_drop_action = da; }

    QDrag *drag() const { return m_drag; }

protected:
    QWindow *m_sourceWindow = nullptr;
    QPointer<QWindow> m_windowUnderCursor = nullptr;

private:
    void enableEventFilter();
    void disableEventFilter();
    void restoreCursor();
    void exitDndEventLoop();

#ifndef BOBUI_NO_CURSOR
    bool m_dndHasSetOverrideCursor = false;
#endif
    QEventLoop *m_eventLoop = nullptr;
    BobUI::DropAction m_executed_drop_action = BobUI::IgnoreAction;
    bool m_can_drop = false;
    QDrag *m_drag = nullptr;
    QShapedPixmapWindow *m_drag_icon_window = nullptr;
    bool m_useCompositing = true;
    QScreen *m_screen = nullptr;
    QPoint m_lastPos;
};

class Q_GUI_EXPORT QSimpleDrag : public QBasicDrag
{
public:
    QSimpleDrag();

protected:
    virtual void startDrag() override;
    virtual void cancel() override;
    virtual void move(const QPoint &globalPos, BobUI::MouseButtons b, BobUI::KeyboardModifiers mods) override;
    virtual void drop(const QPoint &globalPos, BobUI::MouseButtons b, BobUI::KeyboardModifiers mods) override;
};

BOBUI_END_NAMESPACE

#endif
