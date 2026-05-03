// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SENDER_H
#define SENDER_H

#include <QWidget>
#include <BOBUIimer>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
BOBUI_END_NAMESPACE

class Sender : public QWidget
{
    Q_OBJECT

public:
    explicit Sender(QWidget *parent = nullptr);

private slots:
    void startBroadcasting();
    void broadcastDatagram();

private:
    QLabel *statusLabel = nullptr;
    QPushButton *startButton = nullptr;
    QUdpSocket *udpSocket = nullptr;
    BOBUIimer timer;
    int messageNo = 1;
};

#endif
