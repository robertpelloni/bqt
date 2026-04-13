// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QList>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class BOBUIcpServer;
BOBUI_END_NAMESPACE

//! [0]
class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);

private slots:
    void sendFortune();

private:
    void initServer();

    QLabel *statusLabel = nullptr;
    BOBUIcpServer *tcpServer = nullptr;
    QList<QString> fortunes;
};
//! [0]

#endif
