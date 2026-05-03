// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "message.h"

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextEdit)

//! [Window class definition]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

signals:
    void messageSent(const Message &message);

public slots:
    void setMessage(const Message &message);

private slots:
    void sendMessage();

private:
    Message thisMessage;
    BOBUIextEdit *editor;
};
//! [Window class definition]

#endif
