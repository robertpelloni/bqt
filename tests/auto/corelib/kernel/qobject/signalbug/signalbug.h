// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef SIGNAL_BUG_H
#define SIGNAL_BUG_H


#include <QObject>


class Sender;


class Receiver : public QObject
{
Q_OBJECT

public:
    Receiver ();
    virtual ~Receiver () {}

protected slots:
    void received ();

public:
    Sender *s;
};

class Disconnector : public QObject
{
Q_OBJECT

public:
    Disconnector ();
    virtual ~Disconnector () {}

protected slots:
    void received ();

public:
    Sender *s;
};

class Sender : public QObject
{
Q_OBJECT

public:
    Sender (Receiver *r, Disconnector *d);
    virtual ~Sender () {}

    void fire ();

signals:
    void fired ();

public:
    Receiver *r;
    Disconnector *d;
};

#endif  // SIGNAL_BUG_H
