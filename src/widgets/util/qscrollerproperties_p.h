// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCROLLERPROPERTIES_P_H
#define QSCROLLERPROPERTIES_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <QPointF>
#include <QEasingCurve>
#include <qscrollerproperties.h>

BOBUI_BEGIN_NAMESPACE

class QScrollerPropertiesPrivate
{
public:
    static QScrollerPropertiesPrivate *defaults();

    bool operator==(const QScrollerPropertiesPrivate &) const;

    qreal mousePressEventDelay;
    qreal dragStartDistance;
    qreal dragVelocitySmoothingFactor;
    qreal axisLockThreshold;
    QEasingCurve scrollingCurve;
    qreal decelerationFactor;
    qreal minimumVelocity;
    qreal maximumVelocity;
    qreal maximumClickThroughVelocity;
    qreal acceleratingFlickMaximumTime;
    qreal acceleratingFlickSpeedupFactor;
    qreal snapPositionRatio;
    qreal snapTime;
    qreal overshootDragResistanceFactor;
    qreal overshootDragDistanceFactor;
    qreal overshootScrollDistanceFactor;
    qreal overshootScrollTime;
    QScrollerProperties::OvershootPolicy hOvershootPolicy;
    QScrollerProperties::OvershootPolicy vOvershootPolicy;
    QScrollerProperties::FrameRates frameRate;
};

BOBUI_END_NAMESPACE

#endif // QSCROLLERPROPERTIES_P_H

