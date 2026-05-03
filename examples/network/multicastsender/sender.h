// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SENDER_H
#define SENDER_H

#include <BobUIWidgets>
#include <BobUINetwork>
#include <BobUICore>

class Sender : public QDialog
{
    Q_OBJECT

public:
    explicit Sender(QWidget *parent = nullptr);

private slots:
    void ttlChanged(int newTtl);
    void startSending();
    void sendDatagram();

private:
    QLabel *statusLabel = nullptr;
    QPushButton *startButton = nullptr;
    QUdpSocket udpSocket4;
    QUdpSocket udpSocket6;
    BOBUIimer timer;
    QHostAddress groupAddress4;
    QHostAddress groupAddress6;
    int messageNo = 1;
};

#endif
