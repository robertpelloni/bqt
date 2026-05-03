// Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Tobias Koenig <tobias.koenig@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QHAIKUCURSOR_H
#define QHAIKUCURSOR_H

#include <qpa/qplatformcursor.h>

#include <Cursor.h>

BOBUI_BEGIN_NAMESPACE

class QHaikuCursor : public QPlatformCursor
{
public:
    QHaikuCursor();

#ifndef BOBUI_NO_CURSOR
    void changeCursor(QCursor *windowCursor, QWindow *window) override;
#endif

private:
    QHash<BobUI::CursorShape, BCursorID> m_cursorIds;
    QHash<BobUI::CursorShape, BCursor*> m_cursors;
};

BOBUI_END_NAMESPACE

#endif
