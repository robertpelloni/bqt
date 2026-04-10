// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <BOBUIcpSocket>

BOBUI_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class BOBUIcpSocket;
BOBUI_END_NAMESPACE

//! [0]
class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();

private:
    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *getFortuneButton = nullptr;

    BOBUIcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentFortune;
};
//! [0]

#endif
