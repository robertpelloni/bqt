// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TIMECONSUMER_H
#define TIMECONSUMER_H

#include "consumer.h"
#include <BOBUIime>

BOBUI_BEGIN_NAMESPACE
class QLCDNumber;
BOBUI_END_NAMESPACE

class TimeConsumer : public Consumer
{
    Q_OBJECT
public:
    explicit TimeConsumer(QObject *parent = nullptr);

    QWidget *widget() override;
    void readDatagram(const QByteArray &ba) override;
    void serverDisconnected() override;

private slots:
    void timerTick();

private:
    BOBUIime lastTime;
    QLCDNumber *lcdNumber;
};

#endif
