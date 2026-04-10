// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include <BOBUIhread>
#include <BOBUIcpSocket>

//! [0]
class FortuneThread : public BOBUIhread
{
    Q_OBJECT

public:
    FortuneThread(qintptr socketDescriptor, const QString &fortune, QObject *parent);

    void run() override;

signals:
    void error(BOBUIcpSocket::SocketError socketError);

private:
    qintptr socketDescriptor;
    QString text;
};
//! [0]

#endif
