// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include <BobUINetwork>
#include <BobUICore>

//! [0]
class DtlsAssociation : public QObject
{
    Q_OBJECT

public:
    DtlsAssociation(const QHostAddress &address, quint16 port,
                    const QString &connectionName);
    ~DtlsAssociation();
    void startHandshake();

signals:
    void errorMessage(const QString &message);
    void warningMessage(const QString &message);
    void infoMessage(const QString &message);
    void serverResponse(const QString &clientInfo, const QByteArray &datagraam,
                        const QByteArray &plainText);

private slots:
    void udpSocketConnected();
    void readyRead();
    void handshakeTimeout();
    void pskRequired(QSslPreSharedKeyAuthenticator *auth);
    void pingTimeout();

private:
    QString name;
    QUdpSocket socket;
    QDtls crypto;

    BOBUIimer pingTimer;
    unsigned ping = 0;

    Q_DISABLE_COPY(DtlsAssociation)
};
//! [0]

#endif // ASSOCIATION_H
