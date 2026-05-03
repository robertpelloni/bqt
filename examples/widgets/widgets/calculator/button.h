// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef BUTTON_H
#define BUTTON_H

#include <BOBUIoolButton>

//! [0]
class Button : public BOBUIoolButton
{
    Q_OBJECT

public:
    explicit Button(const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const override;
};
//! [0]

#endif
