// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMDRAG_H
#define QPLATFORMDRAG_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/QPixmap>

BOBUI_REQUIRE_CONFIG(draganddrop);

BOBUI_BEGIN_NAMESPACE

class QMimeData;
class QMouseEvent;
class QDrag;
class QObject;
class QEvent;
class QPlatformDragPrivate;

class Q_GUI_EXPORT QPlatformDropBobUIResponse
{
public:
    QPlatformDropBobUIResponse(bool accepted, BobUI::DropAction acceptedAction);
    bool isAccepted() const;
    BobUI::DropAction acceptedAction() const;

private:
    bool m_accepted;
    BobUI::DropAction m_accepted_action;

};

class Q_GUI_EXPORT QPlatformDragBobUIResponse : public QPlatformDropBobUIResponse
{
public:
    QPlatformDragBobUIResponse(bool accepted, BobUI::DropAction acceptedAction, QRect answerRect);

    QRect answerRect() const;

private:
    QRect m_answer_rect;
};

class Q_GUI_EXPORT QPlatformDrag
{
    Q_DECLARE_PRIVATE(QPlatformDrag)
public:
    Q_DISABLE_COPY_MOVE(QPlatformDrag)

    QPlatformDrag();
    virtual ~QPlatformDrag();

    QDrag *currentDrag() const;

    virtual BobUI::DropAction drag(QDrag *m_drag) = 0;
    virtual void cancelDrag();
    void updateAction(BobUI::DropAction action);

    virtual BobUI::DropAction defaultAction(BobUI::DropActions possibleActions, BobUI::KeyboardModifiers modifiers) const;

    static QPixmap defaultPixmap();

    virtual bool ownsDragObject() const;

private:
    QPlatformDragPrivate *d_ptr;
};

BOBUI_END_NAMESPACE

#endif
