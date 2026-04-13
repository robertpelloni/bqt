// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUITest/private/qpropertytesthelper_p.h>
#include <QSignalSpy>
#include <BOBUIestEventLoop>
#include <QDBusConnection>
#include <QDBusServiceWatcher>

class tst_QDBusServiceWatcher: public QObject
{
    Q_OBJECT
    int testCounter;
public:
    tst_QDBusServiceWatcher();

private slots:
    void initTestCase();
    void watchForCreation_data();
    void watchForCreation();
    void watchForDisappearance_data();
    void watchForDisappearance();
    void watchForDisappearanceUniqueConnection();
    void watchForOwnerChange_data();
    void watchForOwnerChange();
    void modeChange_data();
    void modeChange();
    void disconnectedConnection();
    void setConnection_data();
    void setConnection();
    void bindings();
    void bindingsAutomated();

private:
    QString generateServiceName();
};

tst_QDBusServiceWatcher::tst_QDBusServiceWatcher()
    : testCounter(0)
{
}

void tst_QDBusServiceWatcher::initTestCase()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());
}

QString tst_QDBusServiceWatcher::generateServiceName() {
    return "com.example.TestService" + QString::number(testCounter++);
}

void tst_QDBusServiceWatcher::watchForCreation_data()
{
    BOBUIest::addColumn<QString>("watchedName");
    BOBUIest::addColumn<QString>("registeredName");

    //com.example.TestService5 matches com.example.TestService5
    QString name = generateServiceName();
    BOBUIest::newRow("normal") << name << name;

    //com.example* matches com.example.TestService5
    name = generateServiceName();
    BOBUIest::newRow("wildcard") << "com.example*" << name;

    //com.example.TestService5* matches com.example.TestService5
    name = generateServiceName();
    BOBUIest::newRow("wildcard_exact") << name+"*" << name;

    //com.example.TestService5* matches com.example.TestService5.Foo
    name = generateServiceName();
    BOBUIest::newRow("wildcard_subdomain") << name+"*" << name + ".Foo";

    //com.example.TestService5* matches com.example.TestService5.Foo.Bar
    name = generateServiceName();
    BOBUIest::newRow("wildcard_subsubdomain") << name+"*" << name + ".Foo.Bar";
}

void tst_QDBusServiceWatcher::watchForCreation()
{
    QFETCH(QString, watchedName);
    QFETCH(QString, registeredName);

    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QDBusServiceWatcher watcher(watchedName, con, QDBusServiceWatcher::WatchForRegistration);

    QSignalSpy spyR(&watcher, SIGNAL(serviceRegistered(QString)));
    QSignalSpy spyU(&watcher, SIGNAL(serviceUnregistered(QString)));
    QSignalSpy spyO(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)));
    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceRegistered(QString)), SLOT(exitLoop()));

    // register a name
    QVERIFY(con.registerService(registeredName));

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 1);
    QCOMPARE(spyR.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyU.size(), 0);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QVERIFY(spyO.at(0).at(1).toString().isEmpty());
    QCOMPARE(spyO.at(0).at(2).toString(), con.baseService());

    spyR.clear();
    spyU.clear();
    spyO.clear();

    // unregister it:
    con.unregisterService(registeredName);

    // and register again
    QVERIFY(con.registerService(registeredName));

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 1);
    QCOMPARE(spyR.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyU.size(), 0);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QVERIFY(spyO.at(0).at(1).toString().isEmpty());
    QCOMPARE(spyO.at(0).at(2).toString(), con.baseService());
}

void tst_QDBusServiceWatcher::watchForDisappearance_data()
{
    tst_QDBusServiceWatcher::watchForCreation_data();
}

void tst_QDBusServiceWatcher::watchForDisappearance()
{
    QFETCH(QString, watchedName);
    QFETCH(QString, registeredName);

    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QDBusServiceWatcher watcher(watchedName, con, QDBusServiceWatcher::WatchForUnregistration);
    watcher.setObjectName("watcher for disappearance");

    QSignalSpy spyR(&watcher, SIGNAL(serviceRegistered(QString)));
    QSignalSpy spyU(&watcher, SIGNAL(serviceUnregistered(QString)));
    QSignalSpy spyO(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)));
    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceUnregistered(QString)), SLOT(exitLoop()));

    // register a name
    QVERIFY(con.registerService(registeredName));

    // unregister it:
    con.unregisterService(registeredName);

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 0);

    QCOMPARE(spyU.size(), 1);
    QCOMPARE(spyU.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QCOMPARE(spyO.at(0).at(1).toString(), con.baseService());
    QVERIFY(spyO.at(0).at(2).toString().isEmpty());
}

void tst_QDBusServiceWatcher::watchForDisappearanceUniqueConnection()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    // second connection
    QString watchedName = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "session2").baseService();
    QVERIFY(!watchedName.isEmpty());

    QDBusServiceWatcher watcher(watchedName, con, QDBusServiceWatcher::WatchForUnregistration);
    watcher.setObjectName("watcher for disappearance");

    QSignalSpy spyR(&watcher, SIGNAL(serviceRegistered(QString)));
    QSignalSpy spyU(&watcher, SIGNAL(serviceUnregistered(QString)));
    QSignalSpy spyO(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)));
    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceUnregistered(QString)), SLOT(exitLoop()));

    // unregister it:
    QDBusConnection::disconnectFromBus("session2");

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 0);

    QCOMPARE(spyU.size(), 1);
    QCOMPARE(spyU.at(0).at(0).toString(), watchedName);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), watchedName);
    QCOMPARE(spyO.at(0).at(1).toString(), watchedName);
    QVERIFY(spyO.at(0).at(2).toString().isEmpty());
}

void tst_QDBusServiceWatcher::watchForOwnerChange_data()
{
    watchForCreation_data();
}

void tst_QDBusServiceWatcher::watchForOwnerChange()
{
    QFETCH(QString, watchedName);
    QFETCH(QString, registeredName);

    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QDBusServiceWatcher watcher(watchedName, con, QDBusServiceWatcher::WatchForOwnerChange);

    QSignalSpy spyR(&watcher, SIGNAL(serviceRegistered(QString)));
    QSignalSpy spyU(&watcher, SIGNAL(serviceUnregistered(QString)));
    QSignalSpy spyO(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)));
    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceRegistered(QString)), SLOT(exitLoop()));

    // register a name
    QVERIFY(con.registerService(registeredName));

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 1);
    QCOMPARE(spyR.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyU.size(), 0);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QVERIFY(spyO.at(0).at(1).toString().isEmpty());
    QCOMPARE(spyO.at(0).at(2).toString(), con.baseService());

    spyR.clear();
    spyU.clear();
    spyO.clear();

    // unregister it:
    con.unregisterService(registeredName);

    // and register again
    QVERIFY(con.registerService(registeredName));

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 1);
    QCOMPARE(spyR.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyU.size(), 1);
    QCOMPARE(spyU.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyO.size(), 2);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QCOMPARE(spyO.at(0).at(1).toString(), con.baseService());
    QVERIFY(spyO.at(0).at(2).toString().isEmpty());
    QCOMPARE(spyO.at(1).at(0).toString(), registeredName);
    QVERIFY(spyO.at(1).at(1).toString().isEmpty());
    QCOMPARE(spyO.at(1).at(2).toString(), con.baseService());
}

void tst_QDBusServiceWatcher::modeChange_data()
{
    watchForCreation_data();
}

void tst_QDBusServiceWatcher::modeChange()
{
    QFETCH(QString, watchedName);
    QFETCH(QString, registeredName);

    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QDBusServiceWatcher watcher(watchedName, con, QDBusServiceWatcher::WatchForRegistration);

    QSignalSpy spyR(&watcher, SIGNAL(serviceRegistered(QString)));
    QSignalSpy spyU(&watcher, SIGNAL(serviceUnregistered(QString)));
    QSignalSpy spyO(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)));
    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceRegistered(QString)), SLOT(exitLoop()));

    // register a name
    QVERIFY(con.registerService(registeredName));

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 1);
    QCOMPARE(spyR.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyU.size(), 0);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QVERIFY(spyO.at(0).at(1).toString().isEmpty());
    QCOMPARE(spyO.at(0).at(2).toString(), con.baseService());

    spyR.clear();
    spyU.clear();
    spyO.clear();

    watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);

    // unregister it:
    con.unregisterService(registeredName);

    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceUnregistered(QString)), SLOT(exitLoop()));
    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 0);

    QCOMPARE(spyU.size(), 1);
    QCOMPARE(spyU.at(0).at(0).toString(), registeredName);

    QCOMPARE(spyO.size(), 1);
    QCOMPARE(spyO.at(0).at(0).toString(), registeredName);
    QCOMPARE(spyO.at(0).at(1).toString(), con.baseService());
    QVERIFY(spyO.at(0).at(2).toString().isEmpty());
}

void tst_QDBusServiceWatcher::disconnectedConnection()
{
    QDBusConnection con("");
    QVERIFY(!con.isConnected());

    QDBusServiceWatcher watcher(generateServiceName(), con, QDBusServiceWatcher::WatchForRegistration);
    watcher.addWatchedService("com.example.somethingelse");
    watcher.addWatchedService("org.freedesktop.DBus");

    watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    watcher.setWatchMode(QDBusServiceWatcher::WatchForOwnerChange);

    watcher.setWatchedServices(QStringList());
}

void tst_QDBusServiceWatcher::setConnection_data()
{
    BOBUIest::addColumn<QString>("serviceName");
    BOBUIest::newRow("normal") << generateServiceName();
}

void tst_QDBusServiceWatcher::setConnection()
{
    QFETCH(QString, serviceName);
    // begin with a disconnected connection
    QDBusConnection con("");
    QVERIFY(!con.isConnected());

    QDBusServiceWatcher watcher(serviceName, con, QDBusServiceWatcher::WatchForRegistration);

    QSignalSpy spyR(&watcher, SIGNAL(serviceRegistered(QString)));
    QSignalSpy spyU(&watcher, SIGNAL(serviceUnregistered(QString)));
    BOBUIestEventLoop::instance().connect(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)), SLOT(exitLoop()));

    // move to the session bus
    con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());
    watcher.setConnection(con);

    // register a name
    QVERIFY(con.registerService(serviceName));

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 1);
    QCOMPARE(spyR.at(0).at(0).toString(), serviceName);
    QCOMPARE(spyU.size(), 0);

    // is the system bus available?
    if (!QDBusConnection::systemBus().isConnected())
        return;

    // connect to the system bus and ask to watch that base service
    QString watchedName = QDBusConnection::connectToBus(QDBusConnection::SystemBus, "system2").baseService();
    watcher.setWatchedServices(QStringList() << watchedName);
    watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);

    // move to the system bus
    watcher.setConnection(QDBusConnection::systemBus());
    spyR.clear();
    spyU.clear();

    QDBusConnection::disconnectFromBus("system2");

    BOBUIestEventLoop::instance().enterLoop(1);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(spyR.size(), 0);

    QCOMPARE(spyU.size(), 1);
    QCOMPARE(spyU.at(0).at(0).toString(), watchedName);
}

void tst_QDBusServiceWatcher::bindings()
{
    QString serviceName("normal");

    QDBusConnection con("");
    QVERIFY(!con.isConnected());

    QDBusServiceWatcher watcher(serviceName, con, QDBusServiceWatcher::WatchForRegistration);
    QProperty<QDBusServiceWatcher::WatchMode> follower;
    int notificationCounter = 0;
    auto connection = follower.subscribe([&]() { notificationCounter++; });
    QCOMPARE(notificationCounter, 1);
    follower.setBinding([&]() { return watcher.watchMode(); });
    QCOMPARE(follower.value(), QDBusServiceWatcher::WatchForRegistration);
    QCOMPARE(notificationCounter, 2);

    watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    QCOMPARE(follower.value(), QDBusServiceWatcher::WatchForUnregistration);
    QCOMPARE(notificationCounter, 3);

    QProperty<QDBusServiceWatcher::WatchMode> leader(QDBusServiceWatcher::WatchForRegistration);
    watcher.bindableWatchMode().setBinding([&]() { return leader.value(); });
    QCOMPARE(follower.value(), QDBusServiceWatcher::WatchForRegistration);
    QCOMPARE(notificationCounter, 4);

    leader = QDBusServiceWatcher::WatchForUnregistration;
    QCOMPARE(follower.value(), QDBusServiceWatcher::WatchForUnregistration);
    QCOMPARE(notificationCounter, 5);

    // check that setting a value breaks the binding
    watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    QCOMPARE(notificationCounter, 5);
    leader = QDBusServiceWatcher::WatchForRegistration;
    QCOMPARE(follower.value(), QDBusServiceWatcher::WatchForUnregistration);

    // check that setting the same value again does not trigger notification
    watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    QCOMPARE(notificationCounter, 5);
}

void tst_QDBusServiceWatcher::bindingsAutomated()
{
    QString serviceName("normal");

    QDBusConnection con("");
    QVERIFY(!con.isConnected());

    QDBusServiceWatcher watcher(serviceName, con, QDBusServiceWatcher::WatchForRegistration);

    BOBUIestPrivate::testReadWritePropertyBasics<QDBusServiceWatcher, QStringList>(
            watcher,
            QStringList() << "foo" << "bar",
            QStringList() << "bar" << "foo",
            "watchedServices");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QDBusServiceWatcher::watchedServices");
        return;
    }

    BOBUIestPrivate::testReadWritePropertyBasics<QDBusServiceWatcher,
                                              QFlags<QDBusServiceWatcher::WatchModeFlag>>(
            watcher, QDBusServiceWatcher::WatchForUnregistration,
            QDBusServiceWatcher::WatchForRegistration, "watchMode");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QDBusServiceWatcher::watchMode");
        return;
    }
}
BOBUIEST_MAIN(tst_QDBusServiceWatcher)
#include "tst_qdbusservicewatcher.moc"
