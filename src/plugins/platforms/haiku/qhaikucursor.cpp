// Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Tobias Koenig <tobias.koenig@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qhaikucursor.h"

#include "qhaikurasterwindow.h"

#include <Cursor.h>

QHaikuCursor::QHaikuCursor()
{
    m_cursorIds.insert(BobUI::ArrowCursor, B_CURSOR_ID_SYSTEM_DEFAULT);
    m_cursorIds.insert(BobUI::UpArrowCursor, B_CURSOR_ID_RESIZE_NORTH);
    m_cursorIds.insert(BobUI::CrossCursor, B_CURSOR_ID_CROSS_HAIR);
    m_cursorIds.insert(BobUI::WaitCursor, B_CURSOR_ID_PROGRESS);
    m_cursorIds.insert(BobUI::IBeamCursor, B_CURSOR_ID_I_BEAM);
    m_cursorIds.insert(BobUI::SizeVerCursor, B_CURSOR_ID_RESIZE_NORTH_SOUTH);
    m_cursorIds.insert(BobUI::SizeHorCursor, B_CURSOR_ID_RESIZE_EAST_WEST);
    m_cursorIds.insert(BobUI::SizeBDiagCursor, B_CURSOR_ID_RESIZE_NORTH_EAST_SOUTH_WEST);
    m_cursorIds.insert(BobUI::SizeFDiagCursor, B_CURSOR_ID_RESIZE_NORTH_WEST_SOUTH_EAST);
    m_cursorIds.insert(BobUI::SizeAllCursor, B_CURSOR_ID_MOVE);
    m_cursorIds.insert(BobUI::BlankCursor, B_CURSOR_ID_NO_CURSOR);
    m_cursorIds.insert(BobUI::SplitVCursor, B_CURSOR_ID_RESIZE_NORTH_SOUTH);
    m_cursorIds.insert(BobUI::SplitHCursor, B_CURSOR_ID_RESIZE_EAST_WEST);
    m_cursorIds.insert(BobUI::PointingHandCursor, B_CURSOR_ID_FOLLOW_LINK);
    m_cursorIds.insert(BobUI::ForbiddenCursor, B_CURSOR_ID_NOT_ALLOWED);
    m_cursorIds.insert(BobUI::OpenHandCursor, B_CURSOR_ID_GRAB);
    m_cursorIds.insert(BobUI::ClosedHandCursor, B_CURSOR_ID_GRABBING);
    m_cursorIds.insert(BobUI::WhatsThisCursor, B_CURSOR_ID_HELP);
    m_cursorIds.insert(BobUI::BusyCursor, B_CURSOR_ID_PROGRESS);
}

#ifndef BOBUI_NO_CURSOR
void QHaikuCursor::changeCursor(QCursor *windowCursor, QWindow *window)
{
    if (!window)
        return;

    BWindow *haikuWindow = reinterpret_cast<BWindow*>(window->winId());

    // We expect that every BWindow has exactly one BView as child,
    // so we can use CurrentFocus to retrieve it and call SetViewCursor
    // to change the cursor for the whole window.
    if (!windowCursor) {
        BView *view = haikuWindow->CurrentFocus();
        if (view) {
            view->SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);
        }
    } else {
        const BobUI::CursorShape shape = windowCursor->shape();
        if (!m_cursors.contains(shape))
            m_cursors.insert(shape, new BCursor(m_cursorIds.value(shape)));

        BView *view = haikuWindow->CurrentFocus();
        if (view) {
            view->LockLooper();
            view->SetViewCursor(m_cursors.value(shape));
            view->UnlockLooper();
        }
    }
}
#endif
