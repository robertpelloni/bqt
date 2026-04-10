// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "dummyanimation.h"
#include "dummyobject.h"


DummyAnimation::DummyAnimation(DummyObject *d) : m_dummy(d)
{
}

void DummyAnimation::updateCurrentValue(const QVariant &value)
{
    if (state() == Stopped)
        return;
    if (m_dummy)
        m_dummy->setRect(value.toRect());
}

void DummyAnimation::updateState(State newstate, State oldstate)
{
    Q_UNUSED(newstate);
    Q_UNUSED(oldstate);
}
