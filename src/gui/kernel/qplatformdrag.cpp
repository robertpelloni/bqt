// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qplatformdrag.h"

#include <BobUIGui/private/qdnd_p.h>
#include <BobUIGui/QKeyEvent>
#include <BobUIGui/QGuiApplication>
#include <BobUICore/QEventLoop>

BOBUI_BEGIN_NAMESPACE

#ifdef QDND_DEBUG
#  include <BobUICore/QDebug>
#endif

QPlatformDropBobUIResponse::QPlatformDropBobUIResponse(bool accepted, BobUI::DropAction acceptedAction)
    : m_accepted(accepted)
    , m_accepted_action(acceptedAction)
{
}

bool QPlatformDropBobUIResponse::isAccepted() const
{
    return m_accepted;
}

BobUI::DropAction QPlatformDropBobUIResponse::acceptedAction() const
{
    return m_accepted_action;
}

QPlatformDragBobUIResponse::QPlatformDragBobUIResponse(bool accepted, BobUI::DropAction acceptedAction, QRect answerRect)
    : QPlatformDropBobUIResponse(accepted,acceptedAction)
    , m_answer_rect(answerRect)
{
}

QRect QPlatformDragBobUIResponse::answerRect() const
{
    return m_answer_rect;
}

class QPlatformDragPrivate {
public:
    QPlatformDragPrivate() : cursor_drop_action(BobUI::IgnoreAction) {}

    BobUI::DropAction cursor_drop_action;
};

/*!
    \class QPlatformDrag
    \since 5.0
    \internal
    \preliminary
    \ingroup qpa

    \brief The QPlatformDrag class provides an abstraction for drag.
 */
QPlatformDrag::QPlatformDrag() : d_ptr(new QPlatformDragPrivate)
{
}

QPlatformDrag::~QPlatformDrag()
{
    delete d_ptr;
}

QDrag *QPlatformDrag::currentDrag() const
{
    return QDragManager::self()->object();
}

BobUI::DropAction QPlatformDrag::defaultAction(BobUI::DropActions possibleActions,
                                           BobUI::KeyboardModifiers modifiers) const
{
#ifdef QDND_DEBUG
    qDebug() << "QDragManager::defaultAction(BobUI::DropActions possibleActions)\nkeyboard modifiers : " << modifiers;
#endif

    BobUI::DropAction default_action = BobUI::IgnoreAction;

    if (currentDrag()) {
        default_action = currentDrag()->defaultAction();
    }


    if (default_action == BobUI::IgnoreAction) {
        //This means that the drag was initiated by QDrag::start and we need to
        //preserve the old behavior
        default_action = BobUI::CopyAction;
    }

    if (modifiers & BobUI::ControlModifier && modifiers & BobUI::ShiftModifier)
        default_action = BobUI::LinkAction;
    else if (modifiers & BobUI::ControlModifier)
        default_action = BobUI::CopyAction;
    else if (modifiers & BobUI::ShiftModifier)
        default_action = BobUI::MoveAction;
    else if (modifiers & BobUI::AltModifier)
        default_action = BobUI::LinkAction;

#ifdef QDND_DEBUG
    qDebug() << "possible actions : " << possibleActions;
#endif

    // Check if the action determined is allowed
    if (!(possibleActions & default_action)) {
        if (possibleActions & BobUI::CopyAction)
            default_action = BobUI::CopyAction;
        else if (possibleActions & BobUI::MoveAction)
            default_action = BobUI::MoveAction;
        else if (possibleActions & BobUI::LinkAction)
            default_action = BobUI::LinkAction;
        else
            default_action = BobUI::IgnoreAction;
    }

#ifdef QDND_DEBUG
    qDebug() << "default action : " << default_action;
#endif

    return default_action;
}

/*!
    \brief Cancels the currently active drag (only for drags of
    the current application initiated by QPlatformDrag::drag()).

    The default implementation does nothing.

    \since 5.7
 */

void QPlatformDrag::cancelDrag()
{
    Q_UNIMPLEMENTED();
}

/*!
    \brief Called to notify QDrag about changes of the current action.
 */

void QPlatformDrag::updateAction(BobUI::DropAction action)
{
    Q_D(QPlatformDrag);
    if (d->cursor_drop_action != action) {
        d->cursor_drop_action = action;
        emit currentDrag()->actionChanged(action);
    }
}

static const char *const default_pm[] = {
"13 9 3 1",
".      c None",
"       c #000000",
"X      c #FFFFFF",
"X X X X X X X",
" X X X X X X ",
"X ......... X",
" X.........X ",
"X ......... X",
" X.........X ",
"X ......... X",
" X X X X X X ",
"X X X X X X X",
};

Q_GLOBAL_STATIC_WITH_ARGS(QPixmap,bobui_drag_default_pixmap,(default_pm))

QPixmap QPlatformDrag::defaultPixmap()
{
    return *bobui_drag_default_pixmap();
}

/*!
    \since 5.4
    \brief Returns bool indicating whether QPlatformDrag takes ownership
    and therefore responsibility of deleting the QDrag object passed in
    from QPlatformDrag::drag. This can be useful on platforms where QDrag
    object has to be kept around.
 */
bool QPlatformDrag::ownsDragObject() const
{
    return false;
}

BOBUI_END_NAMESPACE
