// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUINetwork/QNetworkAccessManager>
#include <BobUINetwork/QNetworkReply>

#include <BobUICore/QDebug>

class tst_QNetworkAccessManager : public QObject
{
    Q_OBJECT

public:
    tst_QNetworkAccessManager();

private slots:
    void alwaysCacheRequest();
};

tst_QNetworkAccessManager::tst_QNetworkAccessManager()
{
}

void tst_QNetworkAccessManager::alwaysCacheRequest()
{
    QNetworkAccessManager manager;

    QNetworkRequest req;
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysCache);
    QNetworkReply *reply = manager.get(req);
    reply->close();
    delete reply;
}

BOBUIEST_MAIN(tst_QNetworkAccessManager)
#include "tst_qnetworkaccessmanager.moc"
