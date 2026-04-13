// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QVARIANTANIMATION_P_H
#define QVARIANTANIMATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qvariantanimation.h"
#include <BobUICore/qeasingcurve.h>
#include <BobUICore/qmetaobject.h>

#include "private/qabstractanimation_p.h"
#include "private/qproperty_p.h"

#include <type_traits>

BOBUI_REQUIRE_CONFIG(animation);

BOBUI_BEGIN_NAMESPACE

class QVariantAnimationPrivate : public QAbstractAnimationPrivate
{
    Q_DECLARE_PUBLIC(QVariantAnimation)
public:

    QVariantAnimationPrivate();
    ~QVariantAnimationPrivate() override;

    static QVariantAnimationPrivate *get(QVariantAnimation *q)
    {
        return q->d_func();
    }

    void setDefaultStartEndValue(const QVariant &value);


    QVariant currentValue;
    QVariant defaultStartEndValue;

    //this is used to keep track of the KeyValue interval in which we currently are
    struct
    {
        QVariantAnimation::KeyValue start, end;
    } currentInterval;

    void setEasingCurve(const QEasingCurve &easing) { q_func()->setEasingCurve(easing); }
    Q_OBJECT_COMPAT_PROPERTY(QVariantAnimationPrivate, QEasingCurve, easing,
                             &QVariantAnimationPrivate::setEasingCurve)

    void setDuration(int msecs) { q_func()->setDuration(msecs); }
    Q_OBJECT_COMPAT_PROPERTY(QVariantAnimationPrivate, int, duration,
                             &QVariantAnimationPrivate::setDuration)

    QVariantAnimation::KeyValues keyValues;
    QVariantAnimation::Interpolator interpolator;

    void setCurrentValueForProgress(const qreal progress);
    void recalculateCurrentInterval(bool force=false);
    void setValueAt(qreal, const QVariant &);
    QVariant valueAt(qreal step) const;
    void convertValues(int t);

    void updateInterpolator();

    //XXX this is needed by dui
    static Q_CORE_EXPORT QVariantAnimation::Interpolator getInterpolator(int interpolationType);
};

//this should make the interpolation faster
template<typename T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type
_q_interpolate(const T &f, const T &t, qreal progress)
{
    return T(f + t * progress - f * progress);
}

// the below will apply also to all non-arithmetic types
template<typename T>
typename std::enable_if<!std::is_unsigned<T>::value, T>::type
_q_interpolate(const T &f, const T &t, qreal progress)
{
    return T(f + (t - f) * progress);
}

template<typename T > inline QVariant _q_interpolateVariant(const T &from, const T &to, qreal progress)
{
    return _q_interpolate(from, to, progress);
}


BOBUI_END_NAMESPACE

#endif //QVARIANTANIMATION_P_H
