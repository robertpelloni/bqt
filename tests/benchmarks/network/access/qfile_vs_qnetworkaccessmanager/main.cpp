// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QDebug>
#include <bobuiest.h>
#include <BOBUIest>
#include <BOBUIestEventLoop>
#include <BobUINetwork/qnetworkreply.h>
#include <BobUINetwork/qnetworkrequest.h>
#include <BobUINetwork/qnetworkaccessmanager.h>
#include <BobUICore/BOBUIemporaryFile>
#include <BobUICore/QElapsedTimer>
#include <BobUICore/QFile>

class qfile_vs_qnetworkaccessmanager : public QObject
{
    Q_OBJECT
protected:
    void qnamFileRead_iteration(QNetworkAccessManager &manager, QNetworkRequest &request);
    void qnamImmediateFileRead_iteration(QNetworkAccessManager &manager, QNetworkRequest &request);
    void qfileFileRead_iteration();
    static const int iterations = 10;

private slots:
    void qnamFileRead();
    void qnamImmediateFileRead();
    void qfileFileRead();

    void initTestCase();
    void cleanupTestCase();

public:
    qint64 size;
    BOBUIemporaryFile testFile;

    qfile_vs_qnetworkaccessmanager() : QObject(), size(0) {};
};

void qfile_vs_qnetworkaccessmanager::initTestCase()
{
    QVERIFY2(testFile.open(), "Cannot open temporary file");
    QByteArray qba(1*1024*1024, 'x'); // 1 MB
    for (int i = 0; i < 100; i++) {
        testFile.write(qba);
        testFile.flush();
        size += qba.size();
    } // 100 MB or 10 MB
    testFile.reset();
}

void qfile_vs_qnetworkaccessmanager::cleanupTestCase()
{

}

void qfile_vs_qnetworkaccessmanager::qnamFileRead_iteration(QNetworkAccessManager &manager, QNetworkRequest &request)
{
    QNetworkReply* reply = manager.get(request);
    connect(reply, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()), BobUI::QueuedConnection);
    BOBUIestEventLoop::instance().enterLoop(10);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    QByteArray qba = reply->readAll();
    delete reply;
}

void qfile_vs_qnetworkaccessmanager::qnamFileRead()
{
    QNetworkAccessManager manager;
    QElapsedTimer t;
    QNetworkRequest request(QUrl::fromLocalFile(testFile.fileName()));

    // do 3 dry runs for cache warmup
    qnamFileRead_iteration(manager, request);
    qnamFileRead_iteration(manager, request);
    qnamFileRead_iteration(manager, request);

    t.start();
    // 10 real runs
    QBENCHMARK_ONCE {
        for (int i = 0; i < iterations; i++) {
            qnamFileRead_iteration(manager, request);
        }
    }

    qint64 elapsed = t.elapsed();
    qDebug() << BobUI::endl << "Finished!";
    qDebug() << "Bytes:" << size;
    qDebug() << "Speed:" <<  (qreal(size*iterations) / 1024.0) / (qreal(elapsed) / 1000.0) << "KB/sec";
}

void qfile_vs_qnetworkaccessmanager::qnamImmediateFileRead_iteration(QNetworkAccessManager &manager, QNetworkRequest &request)
{
    QNetworkReply* reply = manager.get(request);
    QVERIFY(reply->isFinished()); // should be like that!
    QByteArray qba = reply->readAll();
    delete reply;
}

void qfile_vs_qnetworkaccessmanager::qnamImmediateFileRead()
{
    QNetworkAccessManager manager;
    QElapsedTimer t;
    QNetworkRequest request(QUrl::fromLocalFile(testFile.fileName()));

    // do 3 dry runs for cache warmup
    qnamImmediateFileRead_iteration(manager, request);
    qnamImmediateFileRead_iteration(manager, request);
    qnamImmediateFileRead_iteration(manager, request);

    t.start();
    // 10 real runs
    QBENCHMARK_ONCE {
        for (int i = 0; i < iterations; i++) {
            qnamImmediateFileRead_iteration(manager, request);
        }
    }

    qint64 elapsed = t.elapsed();
    qDebug() << BobUI::endl << "Finished!";
    qDebug() << "Bytes:" << size;
    qDebug() << "Speed:" <<  (qreal(size*iterations) / 1024.0) / (qreal(elapsed) / 1000.0) << "KB/sec";
}

void qfile_vs_qnetworkaccessmanager::qfileFileRead_iteration()
{
    testFile.reset();
    QByteArray qba = testFile.readAll();
}

void qfile_vs_qnetworkaccessmanager::qfileFileRead()
{
    QElapsedTimer t;

    // do 3 dry runs for cache warmup
    qfileFileRead_iteration();
    qfileFileRead_iteration();
    qfileFileRead_iteration();

    t.start();
    // 10 real runs
    QBENCHMARK_ONCE {
        for (int i = 0; i < iterations; i++) {
            qfileFileRead_iteration();
        }
    }

    qint64 elapsed = t.elapsed();
    qDebug() << BobUI::endl << "Finished!";
    qDebug() << "Bytes:" << size;
    qDebug() << "Speed:" <<  (qreal(size*iterations) / 1024.0) / (qreal(elapsed) / 1000.0) << "KB/sec";
}

BOBUIEST_MAIN(qfile_vs_qnetworkaccessmanager)

#include "main.moc"
