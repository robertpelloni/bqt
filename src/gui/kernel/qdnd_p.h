// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDND_P_H
#define QDND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUICore/qobject.h"
#include "BobUICore/qmap.h"
#include "BobUICore/qmimedata.h"
#include "BobUIGui/qdrag.h"
#include "BobUIGui/qpixmap.h"
#include "BobUIGui/qcursor.h"
#include "BobUIGui/qwindow.h"
#include "BobUICore/qpoint.h"
#include "private/qobject_p.h"
#include "BobUIGui/qbackingstore.h"

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(draganddrop);

BOBUI_BEGIN_NAMESPACE

class QPlatformDrag;

class QDragPrivate : public QObjectPrivate
{
public:
    QDragPrivate()
        : source(nullptr)
        , target(nullptr)
        , data(nullptr)
    { }
    QObject *source;
    QObject *target;
    QMimeData *data;
    QPixmap pixmap;
    QPoint hotspot;
    BobUI::DropAction executed_action;
    BobUI::DropActions supported_actions;
    BobUI::DropAction default_action;
    QMap<BobUI::DropAction, QPixmap> customCursors;
};

class Q_GUI_EXPORT QDragManager : public QObject {
    Q_OBJECT

public:
    QDragManager();
    ~QDragManager();
    static QDragManager *self();

    BobUI::DropAction drag(QDrag *);

    void setCurrentTarget(QObject *target, bool dropped = false);
    QObject *currentTarget() const;

    QPointer<QDrag> object() const { return m_object; }
    QObject *source() const;

private:
    QObject *m_currentDropTarget;
    QPlatformDrag *m_platformDrag;
    QPointer<QDrag> m_object;

    static QDragManager *m_instance;
    Q_DISABLE_COPY_MOVE(QDragManager)
};

BOBUI_END_NAMESPACE

#endif // QDND_P_H
