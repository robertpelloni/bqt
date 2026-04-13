// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef KNOB_H
#define KNOB_H

#include <QGraphicsItem>

class Knob : public QGraphicsEllipseItem
{
public:
    Knob();

    bool sceneEvent(QEvent *event) override;
};

#endif // KNOB_H
