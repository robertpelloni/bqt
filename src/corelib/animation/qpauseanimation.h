// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPAUSEANIMATION_H
#define QPAUSEANIMATION_H

#include <BobUICore/qanimationgroup.h>

BOBUI_REQUIRE_CONFIG(animation);

BOBUI_BEGIN_NAMESPACE

class QPauseAnimationPrivate;

class Q_CORE_EXPORT QPauseAnimation : public QAbstractAnimation
{
    Q_OBJECT
    Q_PROPERTY(int duration READ duration WRITE setDuration BINDABLE bindableDuration)
public:
    QPauseAnimation(QObject *parent = nullptr);
    QPauseAnimation(int msecs, QObject *parent = nullptr);
    ~QPauseAnimation();

    int duration() const override;
    void setDuration(int msecs);
    QBindable<int> bindableDuration();

protected:
    bool event(QEvent *e) override;
    void updateCurrentTime(int) override;

private:
    Q_DISABLE_COPY(QPauseAnimation)
    Q_DECLARE_PRIVATE(QPauseAnimation)
};

BOBUI_END_NAMESPACE

#endif // QPAUSEANIMATION_H
