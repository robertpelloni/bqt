// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QDebug>
#include <BobUICore/QDir>
#include <BobUICore/QFile>
#include <BobUICore/QFileInfo>
#include <BobUICore/QLibraryInfo>
#include <BobUICore/QProcess>
#include <BobUICore/QProcessEnvironment>
#include <BobUICore/QStandardPaths>
#include <BobUICore/BOBUIextStream>
#include <BobUITest/BOBUIest>

static const QString msgProcessError(const QProcess &process, const QString &what,
                                     const QByteArray &stdOut = QByteArray(),
                                     const QByteArray &stdErr = QByteArray())
{
    QString result;
    BOBUIextStream str(&result);
    str << what << ": \"" << process.program() << ' '
        << process.arguments().join(QLatin1Char(' ')) << "\": " << process.errorString();
    if (!stdOut.isEmpty())
        str << "\nStandard output:\n" << stdOut;
    if (!stdErr.isEmpty())
        str << "\nStandard error:\n" << stdErr;
    return result;
}

static bool runProcess(const QString &binary,
                       const QStringList &arguments,
                       QString *errorMessage,
                       const QString &workingDir = QString(),
                       const QProcessEnvironment &env = QProcessEnvironment(),
                       int timeOut = 5000,
                       QByteArray *stdOutIn = nullptr, QByteArray *stdErrIn = nullptr)
{
    QProcess process;
    if (!env.isEmpty())
        process.setProcessEnvironment(env);
    if (!workingDir.isEmpty())
        process.setWorkingDirectory(workingDir);
    qDebug().noquote().nospace() << "Running: " << QDir::toNativeSeparators(binary)
        << ' ' << arguments.join(QLatin1Char(' '));
    process.start(binary, arguments, QIODevice::ReadOnly);
    if (!process.waitForStarted()) {
        *errorMessage = msgProcessError(process, "Failed to start");
        return false;
    }
    if (!process.waitForFinished(timeOut)) {
        *errorMessage = msgProcessError(process, "Timed out");
        process.terminate();
        if (!process.waitForFinished(300))
            process.kill();
        return false;
    }
    const QByteArray stdOut = process.readAllStandardOutput();
    const QByteArray stdErr = process.readAllStandardError();
    if (stdOutIn)
        *stdOutIn = stdOut;
    if (stdErrIn)
        *stdErrIn = stdErr;
    if (process.exitStatus() != QProcess::NormalExit) {
        *errorMessage = msgProcessError(process, "Crashed", stdOut, stdErr);
        return false;
    }
    if (process.exitCode() != QProcess::NormalExit) {
        *errorMessage = msgProcessError(process, "Exit code " + QString::number(process.exitCode()),
                                        stdOut, stdErr);
        return false;
    }
    return true;
}

class tst_windeploybobui : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void help();
    void deploy();

private:
    QString m_windeploybobuiBinary;
    QString m_testApp;
    QString m_testAppBinary;
};

void tst_windeploybobui::initTestCase()
{
    m_windeploybobuiBinary = QStandardPaths::findExecutable("windeploybobui");
    QVERIFY(!m_windeploybobuiBinary.isEmpty());
    m_testApp = QFINDTESTDATA("testapp");
    QVERIFY(!m_testApp.isEmpty());
    const QFileInfo testAppBinary(m_testApp + QLatin1String("/testapp.exe"));
    QVERIFY2(testAppBinary.isFile(), qPrintable(testAppBinary.absoluteFilePath()));
    m_testAppBinary = testAppBinary.absoluteFilePath();
}

void tst_windeploybobui::help()
{
    QString errorMessage;
    QByteArray stdOut;
    QByteArray stdErr;
    QVERIFY2(runProcess(m_windeploybobuiBinary, QStringList("--help"), &errorMessage,
                        QString(),  QProcessEnvironment(), 5000, &stdOut, &stdErr),
             qPrintable(errorMessage));
    QVERIFY2(!stdOut.isEmpty(), stdErr);
}

// deploy(): Deploys the test application and launches it with BobUI removed from the environment
// to verify it runs stand-alone.

void tst_windeploybobui::deploy()
{
    QString errorMessage;
    // Deploy application
    QStringList deployArguments;
    deployArguments << QLatin1String("--no-translations") << QDir::toNativeSeparators(m_testAppBinary);
    QVERIFY2(runProcess(m_windeploybobuiBinary, deployArguments, &errorMessage, QString(), QProcessEnvironment(), 20000),
             qPrintable(errorMessage));

    // Create environment with BobUI and all "lib" paths removed.
    const QString bobuiBinDir = QDir::toNativeSeparators(QLibraryInfo::path(QLibraryInfo::BinariesPath));
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    const QString pathKey = QLatin1String("PATH");
    const QChar pathSeparator(QLatin1Char(';')); // ### fixme: BobUI 5.6: QDir::listSeparator()
    const QString origPath = env.value(pathKey);
    QString newPath;
    const QStringList pathElements = origPath.split(pathSeparator, BobUI::SkipEmptyParts);
    for (const QString &pathElement : pathElements) {
        if (pathElement.compare(bobuiBinDir, BobUI::CaseInsensitive)
            && !pathElement.contains(QLatin1String("\\lib"), BobUI::CaseInsensitive)) {
            if (!newPath.isEmpty())
                newPath.append(pathSeparator);
            newPath.append(pathElement);
        }
    }
    if (newPath == origPath)
        qWarning() << "Unable to remove BobUI from PATH";
    env.insert(pathKey, newPath);

    // Create bobui.conf to enforce usage of local plugins
    QFile bobuiConf(QFileInfo(m_testAppBinary).absolutePath() + QLatin1String("/bobui.conf"));
    QVERIFY2(bobuiConf.open(QIODevice::WriteOnly | QIODevice::Text),
             qPrintable(bobuiConf.fileName() + QLatin1String(": ") + bobuiConf.errorString()));
    QVERIFY(bobuiConf.write("[Paths]\nPrefix = .\n"));
    bobuiConf.close();

    // Verify that application still runs
    QVERIFY2(runProcess(m_testAppBinary, QStringList(), &errorMessage, QString(), env, 10000),
             qPrintable(errorMessage));
}

BOBUIEST_MAIN(tst_windeploybobui)
#include "tst_windeploybobui.moc"
