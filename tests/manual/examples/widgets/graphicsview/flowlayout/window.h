// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QGraphicsWidget>

class Window : public QGraphicsWidget
{
    Q_OBJECT
public:
    Window(QGraphicsItem *parent = nullptr);
};

#endif // WINDOW_H
