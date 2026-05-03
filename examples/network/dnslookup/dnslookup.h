// Copyright (C) 2016 Jeremy Lainé <jeremy.laine@m4x.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QDnsLookup>
#include <QHostAddress>

//! [0]

struct DnsQuery
{
    DnsQuery() : type(QDnsLookup::A) {}

    QDnsLookup::Type type;
    QHostAddress nameServer;
    QString name;
};

//! [0]

class DnsManager : public QObject
{
    Q_OBJECT

public:
    DnsManager();
    void setQuery(const DnsQuery &q) { query = q; }

public slots:
    void execute();
    void showResults();

private:
    QDnsLookup *dns;
    DnsQuery query;
};

