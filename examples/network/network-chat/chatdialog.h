// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "ui_chatdialog.h"
#include "client.h"

class ChatDialog : public QDialog, private Ui::ChatDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();

private:
    Client client;
    QString myNickName;
    BOBUIextTableFormat tableFormat;
};

#endif
