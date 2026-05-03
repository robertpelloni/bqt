// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#pragma once
#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QGraphicsScene *scene = nullptr, QWidget *parent = nullptr);

    bool viewportEvent(QEvent *event) override;

private:
    qreal totalScaleFactor = 1;
};
