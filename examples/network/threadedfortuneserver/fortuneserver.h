// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QStringList>
#include <BOBUIcpServer>

//! [0]
class FortuneServer : public BOBUIcpServer
{
    Q_OBJECT

public:
    FortuneServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QStringList fortunes;
};
//! [0]

#endif
