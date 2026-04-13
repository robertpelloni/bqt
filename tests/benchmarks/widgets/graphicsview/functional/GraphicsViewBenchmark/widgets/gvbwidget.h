// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef GVBWIDGET_H
#define GVBWIDGET_H

#include <QGraphicsWidget>

class GvbWidget : public QGraphicsWidget
{
    Q_OBJECT

public:

    GvbWidget(QGraphicsItem * parent = nullptr, BobUI::WindowFlags wFlags = { });
    ~GvbWidget();
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif
