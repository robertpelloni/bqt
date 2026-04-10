// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLocalSocket>
#include <QPushButton>

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QLocalSocket::LocalSocketError socketError);
    void enableGetFortuneButton();

private:
    QLineEdit *hostLineEdit;
    QPushButton *getFortuneButton;
    QLabel *statusLabel;

    QLocalSocket *socket;
    QDataStream in;
    quint32 blockSize;

    QString currentFortune;
};

#endif
