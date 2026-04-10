// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QDebug>
#include "hello.h"

MyPushButton::MyPushButton(const QString &text)
    : QPushButton(text)
{
    setObjectName("mypushbutton");
    qDebug() << "My PushButton has been constructed";
}
