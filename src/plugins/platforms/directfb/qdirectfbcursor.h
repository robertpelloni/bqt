// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDIRECTFBCURSOR_H
#define QDIRECTFBCURSOR_H

#include <qpa/qplatformcursor.h>
#include <directfb.h>

#include "qdirectfbconvenience.h"

BOBUI_BEGIN_NAMESPACE

class QDirectFbScreen;
class QDirectFbBlitter;

class QDirectFBCursor : public QPlatformCursor
{
public:
    QDirectFBCursor(QPlatformScreen *screen);
#ifndef BOBUI_NO_CURSOR
    void changeCursor(QCursor *cursor, QWindow *window) override;
#endif

private:
#ifndef BOBUI_NO_CURSOR
    QScopedPointer<QPlatformCursorImage> m_image;
#endif
    QPlatformScreen *m_screen;
};

BOBUI_END_NAMESPACE

#endif // QDIRECTFBCURSOR_H
