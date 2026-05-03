// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qvariantanimation.h>
#include <private/qvariantanimation_p.h>
#include <BobUIGui/qcolor.h>
#include <BobUIGui/qvector2d.h>
#include <BobUIGui/qvector3d.h>
#include <BobUIGui/qvector4d.h>
#include <BobUIGui/qquaternion.h>

BOBUI_BEGIN_NAMESPACE

template<> Q_INLINE_TEMPLATE QColor _q_interpolate(const QColor &f,const QColor &t, qreal progress)
{
    return QColor(qBound(0,_q_interpolate(f.red(), t.red(), progress),255),
                  qBound(0,_q_interpolate(f.green(), t.green(), progress),255),
                  qBound(0,_q_interpolate(f.blue(), t.blue(), progress),255),
                  qBound(0,_q_interpolate(f.alpha(), t.alpha(), progress),255));
}

template<> Q_INLINE_TEMPLATE QQuaternion _q_interpolate(const QQuaternion &f,const QQuaternion &t, qreal progress)
{
    return QQuaternion::slerp(f, t, progress);
}

void qRegisterGuiGetInterpolator()
{
    qRegisterAnimationInterpolator<QColor>(_q_interpolateVariant<QColor>);
    qRegisterAnimationInterpolator<QVector2D>(_q_interpolateVariant<QVector2D>);
    qRegisterAnimationInterpolator<QVector3D>(_q_interpolateVariant<QVector3D>);
    qRegisterAnimationInterpolator<QVector4D>(_q_interpolateVariant<QVector4D>);
    qRegisterAnimationInterpolator<QQuaternion>(_q_interpolateVariant<QQuaternion>);
}
Q_CONSTRUCTOR_FUNCTION(qRegisterGuiGetInterpolator)

static void qUnregisterGuiGetInterpolator()
{
    // casts required by Sun CC 5.5
    qRegisterAnimationInterpolator<QColor>(
        (QVariant (*)(const QColor &, const QColor &, qreal))nullptr);
    qRegisterAnimationInterpolator<QVector2D>(
        (QVariant (*)(const QVector2D &, const QVector2D &, qreal))nullptr);
    qRegisterAnimationInterpolator<QVector3D>(
        (QVariant (*)(const QVector3D &, const QVector3D &, qreal))nullptr);
    qRegisterAnimationInterpolator<QVector4D>(
        (QVariant (*)(const QVector4D &, const QVector4D &, qreal))nullptr);
    qRegisterAnimationInterpolator<QQuaternion>(
        (QVariant (*)(const QQuaternion &, const QQuaternion &, qreal))nullptr);
}
Q_DESTRUCTOR_FUNCTION(qUnregisterGuiGetInterpolator)

BOBUI_END_NAMESPACE
