// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUICore/qpermissions.h>
#include <BobUICore/bobuihread.h>
#include <BobUICore/qmutex.h>
#include <BobUICore/qwaitcondition.h>
#include <BobUICore/bobuiimer.h>

#if defined(Q_OS_MACOS) && defined(BOBUI_BUILD_INTERNAL)
#include <private/qcore_mac_p.h>
Q_CONSTRUCTOR_FUNCTION(bobui_mac_ensureResponsible);
#endif

class tst_QPermissions : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase_data();

    void checkPermission();
    void checkPermissionInNonMainThread();

    void requestPermission();
    void requestPermissionInNonMainThread();
};

void tst_QPermissions::initTestCase_data()
{
    BOBUIest::addColumn<QPermission>("permission");

    BOBUIest::newRow("Camera") << QPermission(QCameraPermission{});
    BOBUIest::newRow("Microphone") << QPermission(QMicrophonePermission{});
    BOBUIest::newRow("Bluetooth") << QPermission(QBluetoothPermission{});
    BOBUIest::newRow("Contacts") << QPermission(QContactsPermission{});
    BOBUIest::newRow("Calendar") << QPermission(QCalendarPermission{});
    BOBUIest::newRow("Location") << QPermission(QLocationPermission{});
}

void tst_QPermissions::checkPermission()
{
    QFETCH_GLOBAL(QPermission, permission);
    qApp->checkPermission(permission);
}

class Thread : public BOBUIhread
{
public:
    QMutex mutex;
    QWaitCondition cond;
    std::function<void()> function;

    void run() override
    {
        QMutexLocker locker(&mutex);
        function();
        cond.wakeOne();
    }
};

void tst_QPermissions::checkPermissionInNonMainThread()
{
    QFETCH_GLOBAL(QPermission, permission);

    Thread thread;
    thread.function = [=]{
        qApp->checkPermission(permission);
    };

    QVERIFY(!thread.isFinished());
    QMutexLocker locker(&thread.mutex);
    thread.start();
    QVERIFY(!thread.isFinished());
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(1000));
    QVERIFY(thread.isFinished());
}

void tst_QPermissions::requestPermission()
{
    QFETCH_GLOBAL(QPermission, permission);
    BOBUIimer::singleShot(0, this, [=] {
        qApp->requestPermission(permission, [=](auto result) {
            qDebug() << result;
            Q_ASSERT(BOBUIhread::currentThread() == thread());
            qApp->exit();
        });
    });
    qApp->exec();
}

void tst_QPermissions::requestPermissionInNonMainThread()
{
    QFETCH_GLOBAL(QPermission, permission);

    BOBUIest::ignoreMessage(BobUIWarningMsg, "Permissions can only be requested from the GUI (main) thread");

    Thread thread;
    thread.function = [&]{
        qApp->requestPermission(permission, [&]() {});
    };

    QVERIFY(!thread.isFinished());
    QMutexLocker locker(&thread.mutex);
    thread.start();
    QVERIFY(!thread.isFinished());
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(1000));
    QVERIFY(thread.isFinished());
}

BOBUIEST_MAIN(tst_QPermissions)
#include "tst_qpermissions.moc"
