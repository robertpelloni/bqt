// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDCURSOR_H
#define QWAYLANDCURSOR_H

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

#include <qpa/qplatformcursor.h>
#include <BobUICore/QMap>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwayland-cursor-shape-v1.h>
#include <BobUICore/private/qglobal_p.h>

#if BOBUI_CONFIG(cursor)

#include <memory>

struct wl_cursor;
struct wl_cursor_image;
struct wl_cursor_theme;

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandBuffer;
class QWaylandDisplay;
class QWaylandScreen;
class QWaylandShm;

class Q_WAYLANDCLIENT_EXPORT QWaylandCursorTheme
{
public:
    static std::unique_ptr<QWaylandCursorTheme> create(QWaylandShm *shm, int size, const QString &themeName);
    ~QWaylandCursorTheme();
    ::wl_cursor *cursor(BobUI::CursorShape shape);

protected:
    enum WaylandCursor {
        ArrowCursor = BobUI::ArrowCursor,
        UpArrowCursor,
        CrossCursor,
        WaitCursor,
        IBeamCursor,
        SizeVerCursor,
        SizeHorCursor,
        SizeBDiagCursor,
        SizeFDiagCursor,
        SizeAllCursor,
        BlankCursor,
        SplitVCursor,
        SplitHCursor,
        PointingHandCursor,
        ForbiddenCursor,
        WhatsThisCursor,
        BusyCursor,
        OpenHandCursor,
        ClosedHandCursor,
        DragCopyCursor,
        DragMoveCursor,
        DragLinkCursor,
        // The following are used for cursors that don't have equivalents in BobUI
        ResizeNorthCursor = BobUI::CustomCursor + 1,
        ResizeSouthCursor,
        ResizeEastCursor,
        ResizeWestCursor,
        ResizeNorthWestCursor,
        ResizeSouthEastCursor,
        ResizeNorthEastCursor,
        ResizeSouthWestCursor,

        NumWaylandCursors
    };

    explicit QWaylandCursorTheme(struct ::wl_cursor_theme *theme) : m_theme(theme) {}
    struct ::wl_cursor *requestCursor(WaylandCursor shape);
    struct ::wl_cursor_theme *m_theme = nullptr;
    wl_cursor *m_cursors[NumWaylandCursors] = {};
};

class Q_WAYLANDCLIENT_EXPORT QWaylandCursorShape : public BobUIWayland::wp_cursor_shape_device_v1
{
public:
    QWaylandCursorShape(struct ::wp_cursor_shape_device_v1 *object);
    ~QWaylandCursorShape();
    void setShape(uint32_t serial, BobUI::CursorShape shape);
};

class Q_WAYLANDCLIENT_EXPORT QWaylandCursor : public QPlatformCursor
{
public:
    explicit QWaylandCursor(QWaylandDisplay *display);

    void changeCursor(QCursor *cursor, QWindow *window) override;
    void pointerEvent(const QMouseEvent &event) override;
    QPoint pos() const override;
    void setPos(const QPoint &pos) override;
    void setPosFromEnterEvent(const QPoint &pos);

    QSize size() const override;

    static QSharedPointer<QWaylandBuffer> cursorBitmapBuffer(QWaylandDisplay *display, const QCursor *cursor);

protected:
    QWaylandDisplay *mDisplay = nullptr;
    QPoint mLastPos;
};

}

BOBUI_END_NAMESPACE

#endif // cursor
#endif // QWAYLANDCURSOR_H
