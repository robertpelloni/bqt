// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "ui_mydialog.h"

class MyDialog : public QDialog, public Ui::MyDialog
{
    Q_OBJECT

public:
    MyDialog(QWidget *parent = nullptr);
};

#endif
