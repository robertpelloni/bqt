// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui>

#ifndef _DUMMYANIMATION_H__

class DummyObject;

class DummyAnimation : public QVariantAnimation
{
public:
    DummyAnimation(DummyObject *d);

    void updateCurrentValue(const QVariant &value) override;
    void updateState(State newstate, State oldstate) override;

private:
    DummyObject *m_dummy;
};

#endif
