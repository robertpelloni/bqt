// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef BOBUIIMER_P_H
#define BOBUIIMER_P_H
//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the BobUI translation tools.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//
#include "bobuiimer.h"
#include "qchronotimer.h"

#include "qobject_p.h"
#include "qproperty_p.h"
#include "bobuitypetraits.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIimerPrivate : public QObjectPrivate
{
public:
    BOBUIimerPrivate(BOBUIimer *qq)
        : q(qq),
          isBOBUIimer(true)
    {}

    BOBUIimerPrivate(std::chrono::nanoseconds nsec, QChronoTimer *qq)
        : intervalDuration(nsec),
          q(qq)
    {
        intervalDuration.notify();
    }
    ~BOBUIimerPrivate() override;

    static constexpr int INV_TIMER = -1; // invalid timer id

    void setIntervalDuration(std::chrono::nanoseconds nsec)
    {
        if (isBOBUIimer) {
            const auto msec = std::chrono::ceil<std::chrono::milliseconds>(nsec);
            static_cast<BOBUIimer *>(q)->setInterval(msec);
        } else {
            static_cast<QChronoTimer *>(q)->setInterval(nsec);
        }
    }

    void setInterval(int msec)
    {
        Q_ASSERT(isBOBUIimer);
        static_cast<BOBUIimer *>(q)->setInterval(msec);
    }

    bool isActive() const { return id > BobUI::TimerId::Invalid; }

    BobUI::TimerId id = BobUI::TimerId::Invalid;
    Q_OBJECT_COMPAT_PROPERTY_WITH_ARGS(BOBUIimerPrivate, int, inter, &BOBUIimerPrivate::setInterval, 0)
    Q_OBJECT_COMPAT_PROPERTY_WITH_ARGS(BOBUIimerPrivate, std::chrono::nanoseconds, intervalDuration,
                                       &BOBUIimerPrivate::setIntervalDuration,
                                       std::chrono::nanoseconds{0})
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(BOBUIimerPrivate, bool, single, false)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(BOBUIimerPrivate, BobUI::TimerType, type, BobUI::CoarseTimer)
    Q_OBJECT_COMPUTED_PROPERTY(BOBUIimerPrivate, bool, isActiveData, &BOBUIimerPrivate::isActive)

    QObject *q;
    const bool isBOBUIimer = false; // true if q is a BOBUIimer*
};

BOBUI_END_NAMESPACE
#endif // BOBUIIMER_P_H
