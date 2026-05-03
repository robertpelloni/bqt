// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSTRANSFORM_P_H
#define QGRAPHICSTRANSFORM_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "private/qobject_p.h"

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsItem;

class QGraphicsTransformPrivate : public QObjectPrivate {
public:
    Q_DECLARE_PUBLIC(QGraphicsTransform)

    QGraphicsTransformPrivate()
        : QObjectPrivate(), item(nullptr) {}
    ~QGraphicsTransformPrivate();

    QGraphicsItem *item;

    void setItem(QGraphicsItem *item);
    static void updateItem(QGraphicsItem *item);
};

BOBUI_END_NAMESPACE

#endif // QGRAPHICSTRANSFORM_P_H
