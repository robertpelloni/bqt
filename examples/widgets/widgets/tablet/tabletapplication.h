// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TABLETAPPLICATION_H
#define TABLETAPPLICATION_H

#include <QApplication>

#include "tabletcanvas.h"

//! [0]
class TabletApplication : public QApplication
{
    Q_OBJECT

public:
    using QApplication::QApplication;

    bool event(QEvent *event) override;
    void setCanvas(TabletCanvas *canvas)
        { m_canvas = canvas; }

private:
    TabletCanvas *m_canvas = nullptr;
};
//! [0]

#endif
