// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "button.h"

//! [0]
Button::Button(const QString &text, QWidget *parent)
    : BOBUIoolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setText(text);
}
//! [0]

//! [1]
QSize Button::sizeHint() const
//! [1] //! [2]
{
    QSize size = BOBUIoolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}
//! [2]
