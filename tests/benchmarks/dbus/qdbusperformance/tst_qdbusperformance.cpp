// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BOBUIestEventLoop>
#include <QProcess>
#include <QDBusServiceWatcher>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QElapsedTimer>

#include "./serverobject.h"

static const char serviceName[] = "org.bobuiproject.autotests.performance";
static const int runTime = 500;

class tst_QDBusPerformance: public QObject
{
    Q_OBJECT
    QProcess proc;
    QDBusInterface *target;

    QDBusInterface *remote;
    QDBusInterface *local;

    bool executeTest(const char *funcname, int size, const QVariant &data);

public slots:
    void initTestCase_data();
    void initTestCase();
    void init();

private slots:
    void callSpeed();

    void oneWay_data();
    void oneWay();
    void oneWayVariant_data();
    void oneWayVariant();

    void roundTrip_data();
    void roundTrip();
    void roundTripVariant_data();
    void roundTripVariant();
};

void tst_QDBusPerformance::initTestCase()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QDBusServiceWatcher watcher(serviceName, con,
                                QDBusServiceWatcher::WatchForRegistration);
    connect(&watcher, SIGNAL(serviceRegistered(QString)),
            &BOBUIestEventLoop::instance(), SLOT(exitLoop()));

#ifdef Q_OS_WIN
#  define EXE ".exe"
#else
#  define EXE ""
#endif
    proc.start(QFINDTESTDATA("../server/server" EXE));
    QVERIFY2(proc.waitForStarted(), qPrintable(proc.errorString()));
    QVERIFY(proc.waitForReadyRead());

    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(con.interface()->isServiceRegistered(serviceName));

    remote = new QDBusInterface(serviceName, "/", "org.bobuiproject.autotests.Performance", con, this);
    QVERIFY(remote->isValid());

    new ServerObject("/", con, this);
    local = new QDBusInterface(con.baseService(), "/", "org.bobuiproject.autotests.Performance", con, this);
    QVERIFY(local->isValid());
}

void tst_QDBusPerformance::initTestCase_data()
{
    BOBUIest::addColumn<bool>("loopback");

    BOBUIest::newRow("normal") << false;
    BOBUIest::newRow("loopback") << true;
}

void tst_QDBusPerformance::init()
{
    QFETCH_GLOBAL(bool, loopback);
    if (loopback)
        target = local;
    else
        target = remote;
}

void tst_QDBusPerformance::callSpeed()
{
    QElapsedTimer timer;

    int callCount = 0;
    timer.start();
    while (timer.elapsed() < runTime) {
        QDBusReply<void> reply = target->call("nothing");
        QVERIFY(reply.isValid());

        ++callCount;
    }
    qDebug() << callCount << "calls in" << timer.elapsed() << "ms:"
             << (callCount * 1000.0 / timer.elapsed()) << "calls/sec";
}

bool tst_QDBusPerformance::executeTest(const char *funcname, int size, const QVariant &data)
{
    QElapsedTimer timer;

    int callCount = 0;
    qint64 transferred = 0;
    timer.start();
    while (timer.elapsed() < runTime) {
        QDBusMessage reply = target->call(funcname, data);
        if (reply.type() != QDBusMessage::ReplyMessage)
            return false;

        transferred += size;
        ++callCount;
    }
    qDebug() << transferred << "bytes in" << timer.elapsed() << "ms"
             << "(in" << callCount << "calls):"
             << (transferred * 1000.0 / timer.elapsed() / 1024 / 1024) << "MB/s";

    return true;
}

void tst_QDBusPerformance::oneWay_data()
{
    BOBUIest::addColumn<QVariant>("data");
    BOBUIest::addColumn<int>("size");

    QByteArray ba(256, 'a');
    while (ba.size() < 8193) {
        BOBUIest::newRow(QString("%1-byteArray").arg(ba.size()).toLatin1()) << QVariant::fromValue(ba) << ba.size();
        ba += ba;
    }

    QString s(256, QLatin1Char('a'));
    while (s.size() < 8193) {
        BOBUIest::newRow(QString("%1-string").arg(s.size()).toLatin1()) << QVariant::fromValue(s) << s.size();
        s += s;
    }
}

void tst_QDBusPerformance::oneWay()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("size", size, data));
}

void tst_QDBusPerformance::oneWayVariant_data()
{
    oneWay_data();
}

void tst_QDBusPerformance::oneWayVariant()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("size", size, QVariant::fromValue(QDBusVariant(data))));
}

void tst_QDBusPerformance::roundTrip_data()
{
    oneWay_data();
}

void tst_QDBusPerformance::roundTrip()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("echo", size, data));
}

void tst_QDBusPerformance::roundTripVariant_data()
{
    oneWay_data();
}

void tst_QDBusPerformance::roundTripVariant()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("echo", size, QVariant::fromValue(QDBusVariant(data))));
}

BOBUIEST_MAIN(tst_QDBusPerformance)
#include "tst_qdbusperformance.moc"
