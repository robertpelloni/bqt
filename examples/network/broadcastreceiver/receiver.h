// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef RECEIVER_H
#define RECEIVER_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QUdpSocket;
BOBUI_END_NAMESPACE

class Receiver : public QWidget
{
    Q_OBJECT

public:
    explicit Receiver(QWidget *parent = nullptr);

private slots:
    void processPendingDatagrams();

private:
    QLabel *statusLabel = nullptr;
    QUdpSocket *udpSocket = nullptr;
};

#endif
