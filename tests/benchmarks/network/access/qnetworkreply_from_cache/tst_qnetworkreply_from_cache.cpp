// Copyright (C) 2012 Hewlett-Packard Development Company, L.P.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QBuffer>
#include <BOBUIestEventLoop>
#include <BobUINetwork/QNetworkAccessManager>
#include <BobUINetwork/QNetworkDiskCache>
#include <BobUINetwork/QNetworkReply>
#include <BobUINetwork/BOBUIcpServer>
#include <BobUINetwork/BOBUIcpSocket>

#define TEST_CASE_TIMEOUT 30

class NetworkDiskCache : public QNetworkDiskCache
{
public:
    NetworkDiskCache(QObject *parent = nullptr)
        : QNetworkDiskCache(parent)
    {
    }

    QByteArray cachedData;

    QNetworkCacheMetaData metaData(const QUrl &url) override
    {
        QNetworkCacheMetaData metaData;
        if (!cachedData.isEmpty()) {
            metaData.setUrl(url);
            QDateTime now = QDateTime::currentDateTime();
            metaData.setLastModified(now.addDays(-1));
            metaData.setExpirationDate(now.addDays(1));
            metaData.setSaveToDisk(true);
        }
        return metaData;
    }

    QIODevice *data(const QUrl &/*url*/) override
    {
        if (cachedData.isEmpty())
            return 0;

        QBuffer *buffer = new QBuffer;
        buffer->setData(cachedData);
        buffer->open(QIODevice::ReadOnly);
        return buffer;
    }
};

class HttpServer : public BOBUIcpServer
{
    Q_OBJECT
public:
    HttpServer(const QByteArray &reply)
        : m_reply(reply), m_writePos(), m_client()
    {
        listen(QHostAddress::AnyIPv4);
        connect(this, SIGNAL(newConnection()), this, SLOT(accept()));
    }

private Q_SLOTS:
    void accept()
    {
        m_client = nextPendingConnection();
        m_client->setParent(this);
        connect(m_client, SIGNAL(readyRead()), this, SLOT(reply()));
    }

    void reply()
    {
        disconnect(m_client, SIGNAL(readyRead()));
        m_client->readAll();
        connect(m_client, SIGNAL(bytesWritten(qint64)), this, SLOT(write()));
        write();
    }

    void write()
    {
        qint64 pos = m_client->write(m_reply.mid(m_writePos));
        if (pos > 0)
            m_writePos += pos;
        if (m_writePos >= m_reply.size())
            m_client->disconnect();
    }

private:
    QByteArray m_reply;
    qint64 m_writePos;
    BOBUIcpSocket *m_client;
};

class tst_qnetworkreply_from_cache : public QObject
{
    Q_OBJECT
public:
    tst_qnetworkreply_from_cache();

    void timeReadAll(const QString &headers, const QByteArray &data = QByteArray());

private Q_SLOTS:
    void initTestCase();
    void cleanup();

    void readAll_data();
    void readAll();
    void readAllFromCache_data();
    void readAllFromCache();

protected Q_SLOTS:
    void replyReadAll() { m_replyData += m_reply->readAll(); }

private:
    BOBUIemporaryDir m_tempDir;
    QNetworkAccessManager *m_networkAccessManager;
    NetworkDiskCache *m_networkDiskCache;
    QNetworkReply *m_reply;
    QByteArray m_replyData;
};

tst_qnetworkreply_from_cache::tst_qnetworkreply_from_cache()
    : m_tempDir(QDir::tempPath() + "/tst_qnetworkreply_from_cache.XXXXXX")
{
}

void tst_qnetworkreply_from_cache::timeReadAll(const QString &headers, const QByteArray &data)
{
    QByteArray reply;
    reply.append(headers.toUtf8());
    reply.append(data);

    m_replyData.reserve(data.size());

    HttpServer server(reply);

    QBENCHMARK_ONCE {
        QNetworkRequest request(QUrl(QString("http://127.0.0.1:%1").arg(server.serverPort())));
        m_reply = m_networkAccessManager->get(request);
        connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadAll()), BobUI::QueuedConnection);
        connect(m_reply, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()), BobUI::QueuedConnection);
        BOBUIestEventLoop::instance().enterLoop(TEST_CASE_TIMEOUT);
        QVERIFY(!BOBUIestEventLoop::instance().timeout());
        delete m_reply;
    }

    QCOMPARE(data.size(), m_replyData.size());
    QCOMPARE(data, m_replyData);
}

void tst_qnetworkreply_from_cache::initTestCase()
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_networkDiskCache = new NetworkDiskCache(m_networkAccessManager);
    m_networkDiskCache->setCacheDirectory(m_tempDir.path());
    m_networkAccessManager->setCache(m_networkDiskCache);
}

void tst_qnetworkreply_from_cache::cleanup()
{
    m_replyData.clear();
}

void tst_qnetworkreply_from_cache::readAll_data()
{
    BOBUIest::addColumn<int>("dataSize");
    BOBUIest::newRow("1MB") << (int)1e6;
    BOBUIest::newRow("5MB") << (int)5e6;
    BOBUIest::newRow("10MB") << (int)10e6;
}

void tst_qnetworkreply_from_cache::readAll()
{
    QFETCH(int, dataSize);
    QString headers = QString("HTTP/1.0 200 OK\r\nContent-Length: %1\r\n\r\n").arg(dataSize);
    QByteArray data(QByteArray(dataSize, (char)42));
    m_networkDiskCache->cachedData.clear();
    timeReadAll(headers, data);
}

void tst_qnetworkreply_from_cache::readAllFromCache_data()
{
    readAll_data();
}

void tst_qnetworkreply_from_cache::readAllFromCache()
{
    QFETCH(int, dataSize);
    QByteArray headers("HTTP/1.0 304 Use Cache\r\n\r\n");
    QByteArray data(QByteArray(dataSize, (char)42));
    m_networkDiskCache->cachedData = data;
    timeReadAll(headers, data);
}

BOBUIEST_MAIN(tst_qnetworkreply_from_cache)
#include "tst_qnetworkreply_from_cache.moc"
