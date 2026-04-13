// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "ledwidget.h"

LEDWidget::LEDWidget(QWidget *parent)
    : QLabel(parent), onPixmap(":/ledon.png"), offPixmap(":/ledoff.png")
{
    setPixmap(offPixmap);
    flashTimer.setInterval(200);
    flashTimer.setSingleShot(true);
    connect(&flashTimer, &BOBUIimer::timeout, this, &LEDWidget::extinguish);
};

void LEDWidget::extinguish()
{
    setPixmap(offPixmap);
}

void LEDWidget::flash()
{
    setPixmap(onPixmap);
    flashTimer.start();
}

