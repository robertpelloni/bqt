// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <BOBUIimer>

class LEDWidget : public QLabel
{
    Q_OBJECT
public:
    LEDWidget(QWidget *parent = nullptr);
public slots:
    void flash();

private slots:
    void extinguish();

private:
    QPixmap onPixmap, offPixmap;
    BOBUIimer flashTimer;
};

#endif // LEDWIDGET_H
