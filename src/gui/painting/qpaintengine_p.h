// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPAINTENGINE_P_H
#define QPAINTENGINE_P_H

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
#include "BobUIGui/qpainter.h"
#include "BobUIGui/qpaintengine.h"
#include "BobUIGui/qregion.h"
#include "private/qobject_p.h"

BOBUI_BEGIN_NAMESPACE

class QPaintDevice;

class Q_GUI_EXPORT QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QPaintEngine)
public:
    QPaintEnginePrivate() : pdev(nullptr), q_ptr(nullptr), currentClipDevice(nullptr), hasSystemTransform(0),
                            hasSystemViewport(0) {}
    virtual ~QPaintEnginePrivate();

    QPaintDevice *pdev;
    QPaintEngine *q_ptr;
    QRegion baseSystemClip;
    QRegion systemClip;
    QRect systemRect;
    QRegion systemViewport;
    BOBUIransform systemTransform;
    QPaintDevice *currentClipDevice;
    uint hasSystemTransform : 1;
    uint hasSystemViewport : 1;

    inline void updateSystemClip()
    {
        systemClip = baseSystemClip;
        if (systemClip.isEmpty())
            return;

        if (hasSystemTransform) {
            if (systemTransform.type() <= BOBUIransform::TxTranslate)
                systemClip.translate(qRound(systemTransform.dx()), qRound(systemTransform.dy()));
            else
                systemClip = systemTransform.map(systemClip);
        }

        // Make sure we're inside the viewport.
        if (hasSystemViewport) {
            systemClip &= systemViewport;
            if (systemClip.isEmpty()) {
                // We don't want to paint without system clip, so set it to 1 pixel :)
                systemClip = QRect(systemViewport.boundingRect().topLeft(), QSize(1, 1));
            }
        }
    }

    inline void setSystemTransform(const BOBUIransform &xform)
    {
        systemTransform = xform;
        hasSystemTransform = !xform.isIdentity();
        updateSystemClip();
        if (q_ptr->state)
            systemStateChanged();
    }

    inline void setSystemViewport(const QRegion &region)
    {
        systemViewport = region;
        hasSystemViewport = !systemViewport.isEmpty();
        updateSystemClip();
        if (q_ptr->state)
            systemStateChanged();
    }

    inline void setSystemTransformAndViewport(const BOBUIransform &xform, const QRegion &region)
    {
        systemTransform = xform;
        hasSystemTransform = !xform.isIdentity();
        systemViewport = region;
        hasSystemViewport = !systemViewport.isEmpty();
        updateSystemClip();
        if (q_ptr->state)
            systemStateChanged();
    }

    virtual void systemStateChanged() { }

    void drawBoxTextItem(const QPointF &p, const BOBUIextItemInt &ti);

    static QPaintEnginePrivate *get(QPaintEngine *paintEngine) { return paintEngine->d_func(); }

    virtual QPaintEngine *aggregateEngine() { return nullptr; }
    virtual BobUI::HANDLE nativeHandle() { return nullptr; }
};

BOBUI_END_NAMESPACE

#endif // QPAINTENGINE_P_H
