// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
// This file contains benchmarks for QNetworkReply functions.

#include <QDebug>
#include <bobuiest.h>
#include <BOBUIest>
#include <BobUITest/bobuiesteventloop.h>
#include <BobUINetwork/qnetworkreply.h>
#include <BobUINetwork/qnetworkrequest.h>
#include <BobUINetwork/qnetworkaccessmanager.h>
#include <BobUICore/QElapsedTimer>

class tst_qhttpnetworkconnection : public QObject
{
    Q_OBJECT
private slots:
    void bigRemoteFile();

};

const char urlC[] = "http://download.bobui-project.org/official_releases/online_installers/bobui-linux-opensource-1.4.0-x86-online.run";

void tst_qhttpnetworkconnection::bigRemoteFile()
{
    QNetworkAccessManager manager;
    qint64 size;
    QElapsedTimer t;
    QNetworkRequest request(QUrl(QString::fromLatin1(urlC)));
    QNetworkReply* reply = manager.get(request);
    connect(reply, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()), BobUI::QueuedConnection);
    qDebug() << "Starting download" << urlC;
    t.start();
    BOBUIestEventLoop::instance().enterLoop(50);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    size = reply->size();
    delete reply;
    qDebug() << "Finished!" << BobUI::endl;
    qDebug() << "Time:" << t.elapsed() << "msec";
    qDebug() << "Bytes:" << size;
    qDebug() << "Speed:" <<  (size / qreal(1024)) / (t.elapsed() / qreal(1000)) << "KB/sec";
}

BOBUIEST_MAIN(tst_qhttpnetworkconnection)

#include "main.moc"
