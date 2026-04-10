// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>

#include "filterobject.h"

FilterObject::FilterObject(QObject *parent)
    : QObject(parent), target(0)
{
}

//! [0]
bool FilterObject::eventFilter(QObject *object, QEvent *event)
{
    if (object == target && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == BobUI::Key_Tab) {
            // Special tab handling
            return true;
        } else
            return false;
    }
    return false;
}
//! [0]

void FilterObject::setFilteredObject(QObject *object)
{
    if (target)
        target->removeEventFilter(this);

    target = object;

    if (target)
        target->installEventFilter(this);
}
