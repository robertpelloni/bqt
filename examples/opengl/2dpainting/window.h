// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include "helper.h"

#include <QWidget>

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    Helper helper;
};
//! [0]

#endif
