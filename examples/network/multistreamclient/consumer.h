// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CONSUMER_H
#define CONSUMER_H

#include <QObject>
#include <QByteArray>

BOBUI_BEGIN_NAMESPACE
class QWidget;
BOBUI_END_NAMESPACE

class Consumer : public QObject
{
    Q_OBJECT
public:
    explicit inline Consumer(QObject *parent = nullptr) : QObject(parent) { }

    virtual QWidget *widget() = 0;
    virtual void readDatagram(const QByteArray &ba) = 0;
    virtual void serverDisconnected() { }

signals:
    void writeDatagram(const QByteArray &ba);
};

#endif
