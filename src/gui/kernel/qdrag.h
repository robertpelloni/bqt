// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDRAG_H
#define QDRAG_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>

BOBUI_REQUIRE_CONFIG(draganddrop);

BOBUI_BEGIN_NAMESPACE

class QMimeData;
class QDragPrivate;
class QPixmap;
class QPoint;
class QDragManager;


class Q_GUI_EXPORT QDrag : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDrag)
public:
    explicit QDrag(QObject *dragSource);
    ~QDrag();

    void setMimeData(QMimeData *data);
    QMimeData *mimeData() const;

    void setPixmap(const QPixmap &);
    QPixmap pixmap() const;

    void setHotSpot(const QPoint &hotspot);
    QPoint hotSpot() const;

    QObject *source() const;
    QObject *target() const;

    BobUI::DropAction exec(BobUI::DropActions supportedActions = BobUI::MoveAction);
    BobUI::DropAction exec(BobUI::DropActions supportedActions, BobUI::DropAction defaultAction);

    void setDragCursor(const QPixmap &cursor, BobUI::DropAction action);
    QPixmap dragCursor(BobUI::DropAction action) const;

    BobUI::DropActions supportedActions() const;
    BobUI::DropAction defaultAction() const;

    static void cancel();

Q_SIGNALS:
    void actionChanged(BobUI::DropAction action);
    void targetChanged(QObject *newTarget);

private:
    friend class QDragManager;
    Q_DISABLE_COPY(QDrag)
};

BOBUI_END_NAMESPACE

#endif // QDRAG_H
