// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include <qpa/qplatformcursor.h>
#include "qxcbscreen.h"
#include <xcb/xcb_cursor.h>

#include <BobUICore/QCache>

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_NO_CURSOR

struct QXcbCursorCacheKey
{
    explicit QXcbCursorCacheKey(const QCursor &c);
    explicit QXcbCursorCacheKey(BobUI::CursorShape s) : shape(s), bitmapCacheKey(0), maskCacheKey(0) {}
    QXcbCursorCacheKey() : shape(BobUI::CustomCursor), bitmapCacheKey(0), maskCacheKey(0) {}

    BobUI::CursorShape shape;
    qint64 bitmapCacheKey;
    qint64 maskCacheKey;
    union {
        qint64 hashKey;
        struct {
            qint32 x;
            qint32 y;
        };
    } hotspotCacheKey;
};

inline bool operator==(const QXcbCursorCacheKey &k1, const QXcbCursorCacheKey &k2)
{
    return k1.shape == k2.shape &&
           k1.bitmapCacheKey == k2.bitmapCacheKey &&
           k1.maskCacheKey == k2.maskCacheKey &&
           k1.hotspotCacheKey.hashKey == k2.hotspotCacheKey.hashKey;
}

inline size_t qHash(const QXcbCursorCacheKey &k, size_t seed) noexcept
{
    return (size_t(k.shape) + size_t(k.bitmapCacheKey) + size_t(k.maskCacheKey)) ^ seed;
}

#endif // !BOBUI_NO_CURSOR

class QXcbCursor : public QXcbObject, public QPlatformCursor
{
public:
    QXcbCursor(QXcbConnection *conn, QXcbScreen *screen);
    ~QXcbCursor();
#ifndef BOBUI_NO_CURSOR
    void changeCursor(QCursor *cursor, QWindow *window) override;
#endif
    QPoint pos() const override;
    void setPos(const QPoint &pos) override;

    QSize size() const override;

    void updateContext();

    static void queryPointer(QXcbConnection *c, QXcbVirtualDesktop **virtualDesktop, QPoint *pos, int *keybMask = nullptr);

#ifndef BOBUI_NO_CURSOR
    xcb_cursor_t xcbCursor(const QCursor &c) const
        { return m_cursorHash.value(QXcbCursorCacheKey(c), xcb_cursor_t(0)); }
#endif

private:

#ifndef BOBUI_NO_CURSOR
    typedef QHash<QXcbCursorCacheKey, xcb_cursor_t> CursorHash;

    struct CachedCursor
    {
        explicit CachedCursor(xcb_connection_t *conn, xcb_cursor_t c)
            : cursor(c), connection(conn) {}
        ~CachedCursor() { xcb_free_cursor(connection, cursor); }
        xcb_cursor_t cursor;
        xcb_connection_t *connection;
    };
    typedef QCache<QXcbCursorCacheKey, CachedCursor> BitmapCursorCache;

    xcb_cursor_t createFontCursor(int cshape);
    xcb_cursor_t createBitmapCursor(QCursor *cursor);
    xcb_cursor_t createNonStandardCursor(int cshape);
#endif

    QXcbScreen *m_screen;
    xcb_cursor_context_t *m_cursorContext;
#ifndef BOBUI_NO_CURSOR
    CursorHash m_cursorHash;
    BitmapCursorCache m_bitmapCache;
#endif
    static void cursorThemePropertyChanged(QXcbVirtualDesktop *screen,
                                           const QByteArray &name,
                                           const QVariant &property,
                                           void *handle);
    bool m_callbackForPropertyRegistered;
};

BOBUI_END_NAMESPACE
