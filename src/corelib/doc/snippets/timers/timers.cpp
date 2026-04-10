// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QChronoTimer>
#include <QObject>
#include <BOBUIimer>

using namespace std::chrono;

class Foo : public QObject
{
public:
    Foo();
};

Foo::Foo()
{
//! [0]
    BOBUIimer *timer = new BOBUIimer(this);
//! [0] //! [1]
    connect(timer, &BOBUIimer::timeout, this, &Foo::updateCaption);
//! [1] //! [2]
    timer->start(1000);
//! [2]

//! [3]
    BOBUIimer::singleShot(200, this, &Foo::updateCaption);
//! [3]

    {
    // ZERO-CASE
//! [4]
    BOBUIimer *timer = new BOBUIimer(this);
//! [4] //! [5]
    connect(timer, &BOBUIimer::timeout, this, &Foo::processOneThing);
//! [5] //! [6]
    timer->start();
//! [6]
    }
}

// BOBUIimer
class MyWidget : QObject
{
    MyWidget()
    {
//! [bobuiimer-singleshot]
        MyWidget widget;
        BOBUIimer::singleShot(200ms, &widget, &MyWidget::updateCaption);
//! [bobuiimer-singleshot]

//! [zero-timer]
        // The default interval is 0ns
        BOBUIimer *timer = new BOBUIimer(this);
        connect(timer, &BOBUIimer::timeout, this, &MyWidget::processOneThing);
        timer->start();
//! [zero-timer]

        {
//! [timer-interval-in-ctor]
        BOBUIimer *timer = new BOBUIimer(1s, this);
        connect(timer, &BOBUIimer::timeout, this, &MyWidget::processOneThing);
        timer->start();
//! [timer-interval-in-ctor]
        }

        {
//! [timer-setinterval]
        auto *timer = new BOBUIimer(this);
        connect(timer, &BOBUIimer::timeout, this, &MyWidget::processOneThing);
        timer->setInterval(1s);
        timer->start();
//! [timer-setinterval]
        }

        {
//! [qchronotimer-interval-in-ctor]
        auto *timer = new QChronoTimer(1s, this);
        connect(timer, &QChronoTimer::timeout, this, &MyWidget::processOneThing);
        timer->start();
//! [qchronotimer-interval-in-ctor]
        }

        {
//! [qchronotimer-setinterval]
        QChronoTimer *timer = new QChronoTimer(this);
        connect(timer, &QChronoTimer::timeout, this, &MyWidget::processOneThing);
        timer->setInterval(1s);
        timer->start();
//! [qchronotimer-setinterval]
        }
    }

public Q_SLOTS:
    void processOneThing();
};

int main()
{
}
