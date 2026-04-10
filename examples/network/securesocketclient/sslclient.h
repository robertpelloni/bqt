// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <BobUINetwork>

BOBUI_REQUIRE_CONFIG(ssl);

#include <BobUIWidgets>

BOBUI_BEGIN_NAMESPACE
class Ui_Form;
BOBUI_END_NAMESPACE

class SslClient : public QWidget
{
    Q_OBJECT
public:
    explicit SslClient(QWidget *parent = nullptr);
    ~SslClient();

private slots:
    void updateEnabledState();
    void secureConnect();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void socketReadyRead();
    void sendData();
    void socketError(QAbstractSocket::SocketError error);
    void sslErrors(const QList<QSslError> &errors);
    void displayCertificateInfo();

private:
    void setupUi();
    void setupSecureSocket();
    void appendString(const QString &line);

    QSslSocket *socket = nullptr;
    BOBUIoolButton *padLock = nullptr;
    Ui_Form *form = nullptr;
    bool handlingSocketError = false;
    bool executingDialog = false;
};

#endif
