// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKINTERFACEPRIVATE_H
#define QNETWORKINTERFACEPRIVATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include <BobUINetwork/qnetworkinterface.h>
#include <BobUICore/qatomic.h>
#include <BobUICore/qdeadlinetimer.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qstring.h>
#include <BobUINetwork/qhostaddress.h>
#include <BobUINetwork/qabstractsocket.h>
#include <private/qhostaddress_p.h>

#ifndef BOBUI_NO_NETWORKINTERFACE

BOBUI_BEGIN_NAMESPACE

class QNetworkAddressEntryPrivate
{
public:
    QHostAddress address;
    QHostAddress broadcast;
    QDeadlineTimer preferredLifetime = QDeadlineTimer::Forever;
    QDeadlineTimer validityLifetime = QDeadlineTimer::Forever;

    QNetmask netmask;
    bool lifetimeKnown = false;
    QNetworkAddressEntry::DnsEligibilityStatus dnsEligibility = QNetworkAddressEntry::DnsEligibilityUnknown;
};

class QNetworkInterfacePrivate: public QSharedData
{
public:
    QNetworkInterfacePrivate() : index(0)
    { }
    ~QNetworkInterfacePrivate()
    { }

    int index;                  // interface index, if know
    int mtu = 0;
    QNetworkInterface::InterfaceFlags flags;
    QNetworkInterface::InterfaceType type = QNetworkInterface::Unknown;

    QString name;
    QString friendlyName;
    QString hardwareAddress;

    QList<QNetworkAddressEntry> addressEntries;

    static QString makeHwAddress(int len, uchar *data);
    static void calculateDnsEligibility(QNetworkAddressEntry *entry, bool isTemporary,
                                        bool isDeprecated)
    {
        // this implements an algorithm that yields the same results as Windows
        // produces, for the same input (as far as I can test)
        if (isTemporary || isDeprecated) {
            entry->setDnsEligibility(QNetworkAddressEntry::DnsIneligible);
        } else {
            AddressClassification cl = QHostAddressPrivate::classify(entry->ip());
            if (cl == LoopbackAddress || cl == LinkLocalAddress)
                entry->setDnsEligibility(QNetworkAddressEntry::DnsIneligible);
            else
                entry->setDnsEligibility(QNetworkAddressEntry::DnsEligible);
        }
    }

private:
    // disallow copying -- avoid detaching
    QNetworkInterfacePrivate &operator=(const QNetworkInterfacePrivate &other);
    QNetworkInterfacePrivate(const QNetworkInterfacePrivate &other);
};

class QNetworkInterfaceManager
{
public:
    QNetworkInterfaceManager();
    ~QNetworkInterfaceManager();

    QSharedDataPointer<QNetworkInterfacePrivate> interfaceFromName(const QString &name);
    QSharedDataPointer<QNetworkInterfacePrivate> interfaceFromIndex(int index);
    QList<QSharedDataPointer<QNetworkInterfacePrivate> > allInterfaces();

    static uint interfaceIndexFromName(const QString &name);
    static QString interfaceNameFromIndex(uint index);

    // convenience:
    QSharedDataPointer<QNetworkInterfacePrivate> empty;

private:
    QList<QNetworkInterfacePrivate *> scan();
};


BOBUI_END_NAMESPACE

#endif // BOBUI_NO_NETWORKINTERFACE

#endif
