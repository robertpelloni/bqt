// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "digitalclock.h"

#include <BOBUIime>
#include <BOBUIimer>

//! [0]
DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(parent)
{
    setSegmentStyle(Filled);

    BOBUIimer *timer = new BOBUIimer(this);
    connect(timer, &BOBUIimer::timeout, this, &DigitalClock::showTime);
    timer->start(1000);

    showTime();

    setWindowTitle(tr("Digital Clock"));
    resize(150, 60);
}
//! [0]

//! [1]
void DigitalClock::showTime()
//! [1] //! [2]
{
    BOBUIime time = BOBUIime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    display(text);
}
//! [2]
