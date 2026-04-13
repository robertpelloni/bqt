// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ICONSIZESPINBOX_H
#define ICONSIZESPINBOX_H

#include <QSpinBox>

//! [0]
class IconSizeSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit IconSizeSpinBox(QWidget *parent = nullptr);

    int valueFromText(const QString &text) const override;
    QString textFromValue(int value) const override;
};
//! [0]

#endif
