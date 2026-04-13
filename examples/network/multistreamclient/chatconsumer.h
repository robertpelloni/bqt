// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CHATCONSUMER_H
#define CHATCONSUMER_H

#include "consumer.h"

BOBUI_BEGIN_NAMESPACE
class BOBUIextEdit;
class QLineEdit;
BOBUI_END_NAMESPACE

class ChatConsumer : public Consumer
{
    Q_OBJECT
public:
    explicit ChatConsumer(QObject *parent = nullptr);

    QWidget *widget() override;
    void readDatagram(const QByteArray &ba) override;

private slots:
    void returnPressed();

private:
    QWidget *frameWidget;
    BOBUIextEdit *textEdit;
    QLineEdit *lineEdit;
};

#endif
