// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <qwidget.h>

#include "ui_untitled.h"

class SomeObject : public QWidget, public Ui_Form
{
    Q_OBJECT
public:
    SomeObject();
signals:
    void someSignal();
};
