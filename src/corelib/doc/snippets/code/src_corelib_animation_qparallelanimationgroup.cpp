// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QParallelAnimationGroup>

void example(QAbstractAnimation *anim1, QAbstractAnimation *anim2)
{
    //! [0]
        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        group->addAnimation(anim1);
        group->addAnimation(anim2);

        group->start();
    //! [0]
}
