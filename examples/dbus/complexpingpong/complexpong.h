// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef COMPLEXPONG_H
#define COMPLEXPONG_H

#include <BobUICore/QObject>
#include <BobUIDBus/QDBusAbstractAdaptor>
#include <BobUIDBus/QDBusVariant>

class Pong : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.example.BobUIDBus.ComplexPong.Pong")
    Q_PROPERTY(QString value READ value WRITE setValue)
public:
    QString value() const;
    void setValue(const QString &newValue);

    Pong(QObject *obj) : QDBusAbstractAdaptor(obj) { }

signals:
    void aboutToQuit();

public slots:
    QDBusVariant query(const QString &query);
    Q_NOREPLY void quit();

private:
    QString m_value;
};

#endif
