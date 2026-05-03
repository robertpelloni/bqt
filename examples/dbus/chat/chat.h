// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CHAT_H
#define CHAT_H

#include <QStringList>

#include "ui_chatmainwindow.h"

class ChatMainWindow: public QMainWindow, Ui::ChatMainWindow
{
    Q_OBJECT
    QString m_nickname;
    QStringList m_messages;
public:
    ChatMainWindow();

private:
    void displayMessage(const QString &message);

signals:
    void message(const QString &nickname, const QString &text);
    void action(const QString &nickname, const QString &text);

private slots:
    bool changeNickname(bool initial = false);
};

#endif // CHAT_H
