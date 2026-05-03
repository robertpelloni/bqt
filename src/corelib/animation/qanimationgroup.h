// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QANIMATIONGROUP_H
#define QANIMATIONGROUP_H

#include <BobUICore/qabstractanimation.h>

BOBUI_REQUIRE_CONFIG(animation);

BOBUI_BEGIN_NAMESPACE

class QAnimationGroupPrivate;
class Q_CORE_EXPORT QAnimationGroup : public QAbstractAnimation
{
    Q_OBJECT

public:
    QAnimationGroup(QObject *parent = nullptr);
    ~QAnimationGroup();

    QAbstractAnimation *animationAt(int index) const;
    int animationCount() const;
    int indexOfAnimation(QAbstractAnimation *animation) const;
    void addAnimation(QAbstractAnimation *animation);
    void insertAnimation(int index, QAbstractAnimation *animation);
    void removeAnimation(QAbstractAnimation *animation);
    QAbstractAnimation *takeAnimation(int index);
    void clear();

protected:
    QAnimationGroup(QAnimationGroupPrivate &dd, QObject *parent);
    bool event(QEvent *event) override;

private:
    Q_DISABLE_COPY(QAnimationGroup)
    Q_DECLARE_PRIVATE(QAnimationGroup)
};

BOBUI_END_NAMESPACE

#endif //QANIMATIONGROUP_H
