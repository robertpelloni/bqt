// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QLabel;
BOBUI_END_NAMESPACE
//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
};
//! [0]

#endif // WINDOW_H
