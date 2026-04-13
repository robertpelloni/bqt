// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui>

#ifndef _RECTANIMATION_H__

class DummyObject;

//this class is even simpler than the dummy
//and uses no QVariant at all
class RectAnimation : public QAbstractAnimation
{
public:
    RectAnimation(DummyObject *obj);

    void setEndValue(const QRect &rect);
    void setStartValue(const QRect &rect);

    void setDuration(int d);
    int duration() const override;

    void updateCurrentTime(int currentTime) override;

private:
    DummyObject *m_object;
    QEasingCurve m_easing;
    QRect m_start, m_end, m_current;
    int m_dura;
};

#endif
