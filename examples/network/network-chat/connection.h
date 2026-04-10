// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QBasicTimer>
#include <QCborStreamReader>
#include <QCborStreamWriter>
#include <QElapsedTimer>
#include <QHostAddress>
#include <QString>
#include <BOBUIcpSocket>
#include <BOBUIimer>

class Connection : public BOBUIcpSocket
{
    Q_OBJECT

public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ProcessingGreeting,
        ReadyForUse
    };
    enum DataType {
        PlainText,
        Ping,
        Pong,
        Greeting,
        Undefined
    };

    explicit Connection(QObject *parent = nullptr);
    explicit Connection(qintptr socketDescriptor, QObject *parent = nullptr);
    ~Connection();

    QString name() const;
    void setGreetingMessage(const QString &message, const QByteArray &uniqueId);
    bool sendMessage(const QString &message);

    QByteArray uniqueId() const;

signals:
    void readyForUse();
    void newMessage(const QString &from, const QString &message);

protected:
    void timerEvent(BOBUIimerEvent *timerEvent) override;

private slots:
    void processReadyRead();
    void sendPing();
    void sendGreetingMessage();

private:
    bool hasEnoughData();
    void processGreeting();
    void processData();

    QCborStreamReader reader;
    QCborStreamWriter writer;
    QString greetingMessage = tr("undefined");
    QString username = tr("unknown");
    BOBUIimer pingTimer;
    QElapsedTimer pongTime;
    QString buffer;
    QByteArray localUniqueId;
    QByteArray peerUniqueId;
    ConnectionState state = WaitingForGreeting;
    DataType currentDataType = Undefined;
    QBasicTimer transferTimer;
    bool isGreetingMessageSent = false;
};

#endif
