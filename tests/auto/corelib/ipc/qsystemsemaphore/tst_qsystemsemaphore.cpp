// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2022 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#if BOBUI_CONFIG(process)
#include <QProcess>
#endif

#include <BobUICore/QList>
#include <BobUICore/QSystemSemaphore>
#include <BobUICore/BOBUIemporaryDir>

#include "../ipctestcommon.h"

#define HELPERWAITTIME 10000

using namespace BobUI::StringLiterals;

class tst_QSystemSemaphore : public QObject
{
    Q_OBJECT

public:
    tst_QSystemSemaphore();

    QString mangleKey(QStringView key)
    {
        if (key.isEmpty())
            return key.toString();
        return u"tstsyssem_%1-%2_%3"_s.arg(QCoreApplication::applicationPid())
                .arg(seq).arg(key);
    }

    QNativeIpcKey platformSafeKey(const QString &key)
    {
        QFETCH_GLOBAL(QNativeIpcKey::Type, keyType);
        return QSystemSemaphore::platformSafeKey(mangleKey(key), keyType);
    }

public Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

private slots:
    void nativeKey_data();
    void nativeKey();
    void legacyKey_data() { nativeKey_data(); }
    void legacyKey();

    void changeKeyType();
    void basicacquire();
    void complexacquire();
    void release();
    void twoSemaphores();

    void basicProcesses();

    void processes_data();
    void processes();

    void undo();
    void initialValue();

private:
    int seq = 0;
    QSystemSemaphore *existingLock;

    const QString m_helperBinary;
};

tst_QSystemSemaphore::tst_QSystemSemaphore()
    : m_helperBinary("./acquirerelease_helper")
{
}

void tst_QSystemSemaphore::initTestCase()
{
    IpcTestCommon::addGlobalTestRows<QSystemSemaphore>();
}

void tst_QSystemSemaphore::init()
{
    QNativeIpcKey key = platformSafeKey("existing");
    existingLock = new QSystemSemaphore(key, 1, QSystemSemaphore::Create);
}

void tst_QSystemSemaphore::cleanup()
{
    delete existingLock;
    ++seq;
}

void tst_QSystemSemaphore::nativeKey_data()
{
    BOBUIest::addColumn<QString>("constructorKey");
    BOBUIest::addColumn<QString>("setKey");

    BOBUIest::newRow("null, null") << QString() << QString();
    BOBUIest::newRow("null, one") << QString() << QString("one");
    BOBUIest::newRow("one, two") << QString("one") << QString("two");
}

/*!
    Basic key testing
 */
void tst_QSystemSemaphore::nativeKey()
{
    QFETCH(QString, constructorKey);
    QFETCH(QString, setKey);
    QNativeIpcKey constructorIpcKey = platformSafeKey(constructorKey);
    QNativeIpcKey setIpcKey = platformSafeKey(setKey);

    QSystemSemaphore sem(constructorIpcKey);
    QCOMPARE(sem.nativeIpcKey(), constructorIpcKey);
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());

    sem.setNativeKey(setIpcKey);
    QCOMPARE(sem.nativeIpcKey(), setIpcKey);
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());

    // change the key type
    QNativeIpcKey::Type nextKeyType = IpcTestCommon::nextKeyType(setIpcKey.type());
    if (nextKeyType != setIpcKey.type()) {
        QNativeIpcKey setIpcKey2 = QSystemSemaphore::platformSafeKey(setKey, nextKeyType);
        sem.setNativeKey(setIpcKey2);
        QCOMPARE(sem.nativeIpcKey(), setIpcKey2);
    }
}

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
void tst_QSystemSemaphore::legacyKey()
{
    QFETCH(QString, constructorKey);
    QFETCH(QString, setKey);

    QSystemSemaphore sem(constructorKey);
    QCOMPARE(sem.key(), constructorKey);
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());

    sem.setKey(setKey);
    QCOMPARE(sem.key(), setKey);
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());
}
BOBUI_WARNING_POP

void tst_QSystemSemaphore::changeKeyType()
{
    QString keyName = "changeKeyType";
    QNativeIpcKey key = platformSafeKey(keyName);
    QNativeIpcKey otherKey =
            QSystemSemaphore::platformSafeKey(mangleKey(keyName), IpcTestCommon::nextKeyType(key.type()));
    if (key == otherKey)
        QSKIP("System only supports one key type");

    QSystemSemaphore sem1(key, 1, QSystemSemaphore::Create);
    QSystemSemaphore sem2(otherKey);
    sem1.setNativeKey(otherKey);
    sem2.setNativeKey(key);
}

void tst_QSystemSemaphore::basicacquire()
{
    QNativeIpcKey key = platformSafeKey("basicacquire");
    QSystemSemaphore sem(key, 1, QSystemSemaphore::Create);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());
}

void tst_QSystemSemaphore::complexacquire()
{
    QNativeIpcKey key = platformSafeKey("complexacquire");
    QSystemSemaphore sem(key, 2, QSystemSemaphore::Create);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());
}

void tst_QSystemSemaphore::release()
{
    QNativeIpcKey key = platformSafeKey("release");
    QSystemSemaphore sem(key, 0, QSystemSemaphore::Create);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.acquire());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QVERIFY(sem.release());
    QCOMPARE(sem.error(), QSystemSemaphore::NoError);
    QCOMPARE(sem.errorString(), QString());
}

void tst_QSystemSemaphore::twoSemaphores()
{
    QNativeIpcKey key = platformSafeKey("twoSemaphores");
    QSystemSemaphore sem1(key, 1, QSystemSemaphore::Create);
    QSystemSemaphore sem2(key);
    QVERIFY(sem1.acquire());
    QVERIFY(sem2.release());
    QVERIFY(sem1.acquire());
    QVERIFY(sem2.release());
}

void tst_QSystemSemaphore::basicProcesses()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QNativeIpcKey key = platformSafeKey("store");
    QSystemSemaphore sem(key, 0, QSystemSemaphore::Create);

    QProcess acquire;
    acquire.setProcessChannelMode(QProcess::ForwardedChannels);

    QProcess release;
    release.setProcessChannelMode(QProcess::ForwardedChannels);

    acquire.start(m_helperBinary, { "acquire", key.toString() });
    QVERIFY2(acquire.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    QCOMPARE(acquire.state(), QProcess::Running);
    acquire.kill();
    release.start(m_helperBinary, { "release", key.toString() });
    QVERIFY2(release.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    release.waitForFinished(HELPERWAITTIME);
    QCOMPARE(acquire.state(), QProcess::NotRunning);
#endif
}

void tst_QSystemSemaphore::processes_data()
{
    BOBUIest::addColumn<int>("processes");
    for (int i = 0; i < 5; ++i) {
        BOBUIest::addRow("1 process (%d)", i) << 1;
        BOBUIest::addRow("3 process (%d)", i) << 3;
        BOBUIest::addRow("10 process (%d)", i) << 10;
    }
}

void tst_QSystemSemaphore::processes()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QNativeIpcKey key = platformSafeKey("store");
    QSystemSemaphore sem(key, 1, QSystemSemaphore::Create);

    QFETCH(int, processes);
    QList<QString> scripts(processes, "acquirerelease");

    QList<QProcess*> consumers;
    for (int i = 0; i < scripts.size(); ++i) {
        QProcess *p = new QProcess;
        p->setProcessChannelMode(QProcess::ForwardedChannels);
        consumers.append(p);
        p->start(m_helperBinary, { scripts.at(i), key.toString() });
    }

    while (!consumers.isEmpty()) {
        consumers.first()->waitForFinished();
        QCOMPARE(consumers.first()->exitStatus(), QProcess::NormalExit);
        QCOMPARE(consumers.first()->exitCode(), 0);
        delete consumers.takeFirst();
    }
#endif
}

void tst_QSystemSemaphore::undo()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QNativeIpcKey key = platformSafeKey("store");
    switch (key.type()) {
    case QNativeIpcKey::Type::PosixRealtime:
    case QNativeIpcKey::Type::Windows:
        QSKIP("This test only checks a System V behavior.");

    case QNativeIpcKey::Type::SystemV:
        break;
    }

    QSystemSemaphore sem(key, 1, QSystemSemaphore::Create);

    QStringList acquireArguments = { "acquire", key.toString() };
    QProcess acquire;
    acquire.setProcessChannelMode(QProcess::ForwardedChannels);
    acquire.start(m_helperBinary, acquireArguments);
    QVERIFY2(acquire.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    QVERIFY(acquire.state()== QProcess::NotRunning);

    // At process exit the kernel should auto undo

    acquire.start(m_helperBinary, acquireArguments);
    QVERIFY2(acquire.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    QVERIFY(acquire.state()== QProcess::NotRunning);
#endif
}

void tst_QSystemSemaphore::initialValue()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QNativeIpcKey key = platformSafeKey("store");
    QSystemSemaphore sem(key, 1, QSystemSemaphore::Create);

    QStringList acquireArguments = { "acquire", key.toString() };
    QStringList releaseArguments = { "release", key.toString() };
    QProcess acquire;
    acquire.setProcessChannelMode(QProcess::ForwardedChannels);

    QProcess release;
    release.setProcessChannelMode(QProcess::ForwardedChannels);

    acquire.start(m_helperBinary, acquireArguments);
    QVERIFY2(acquire.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    QVERIFY(acquire.state()== QProcess::NotRunning);

    acquire.start(m_helperBinary, acquireArguments << QLatin1String("2"));
    QVERIFY2(acquire.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    QVERIFY(acquire.state()== QProcess::Running);
    acquire.kill();

    release.start(m_helperBinary, releaseArguments);
    QVERIFY2(release.waitForStarted(), "Could not start helper binary");
    acquire.waitForFinished(HELPERWAITTIME);
    release.waitForFinished(HELPERWAITTIME);
    QVERIFY(acquire.state()== QProcess::NotRunning);
#endif
}

BOBUIEST_MAIN(tst_QSystemSemaphore)
#include "tst_qsystemsemaphore.moc"

