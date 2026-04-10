// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef VALIDATORWIDGET_H
#define VALIDATORWIDGET_H

#include <QWidget>

#include "ui_validators.h"

class ValidatorWidget : public QWidget, public Ui::ValidatorsForm
{
    Q_OBJECT
public:
    ValidatorWidget(QWidget *parent = nullptr);

private slots:
    void updateValidator();
    void updateDoubleValidator();
};

#endif // VALIDATORWIDGET_H
