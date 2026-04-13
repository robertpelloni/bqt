// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIWidgets>
#include "window.h"

//! [Window constructor]
Window::Window(QWidget *parent)
    : QWidget(parent), editor(new BOBUIextEdit(this))
{
    QPushButton *sendButton = new QPushButton(tr("&Send message"));

    connect(sendButton, &QPushButton::clicked,
            this, &Window::sendMessage);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(sendButton);
    buttonLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(editor);
    layout->addLayout(buttonLayout);

    setWindowTitle(tr("Custom Type Sending"));
}
//! [Window constructor]

//! [sending a message]
void Window::sendMessage()
{
    thisMessage = Message(editor->toPlainText(), thisMessage.headers());
    emit messageSent(thisMessage);
}
//! [sending a message]

//! [receiving a message]
void Window::setMessage(const Message &message)
{
    thisMessage = message;
    editor->setPlainText(thisMessage.body());
}
//! [receiving a message]
