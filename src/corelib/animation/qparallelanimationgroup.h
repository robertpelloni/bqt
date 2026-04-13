// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPARALLELANIMATIONGROUP_H
#define QPARALLELANIMATIONGROUP_H

#include <BobUICore/qanimationgroup.h>

BOBUI_REQUIRE_CONFIG(animation);

BOBUI_BEGIN_NAMESPACE

class QParallelAnimationGroupPrivate;
class Q_CORE_EXPORT QParallelAnimationGroup : public QAnimationGroup
{
    Q_OBJECT

public:
    QParallelAnimationGroup(QObject *parent = nullptr);
    ~QParallelAnimationGroup();

    int duration() const override;

protected:
    QParallelAnimationGroup(QParallelAnimationGroupPrivate &dd, QObject *parent);
    bool event(QEvent *event) override;

    void updateCurrentTime(int currentTime) override;
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) override;
    void updateDirection(QAbstractAnimation::Direction direction) override;

private:
    Q_DISABLE_COPY(QParallelAnimationGroup)
    Q_DECLARE_PRIVATE(QParallelAnimationGroup)
    Q_PRIVATE_SLOT(d_func(), void _q_uncontrolledAnimationFinished())
};

BOBUI_END_NAMESPACE

#endif // QPARALLELANIMATIONGROUP
