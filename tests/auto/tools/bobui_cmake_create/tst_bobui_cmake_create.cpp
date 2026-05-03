// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>

#include <BobUICore/qcryptographichash.h>
#include <BobUICore/qdir.h>
#include <BobUICore/qfileinfo.h>
#include <BobUICore/qlatin1stringview.h>
#include <BobUICore/qlibraryinfo.h>
#include <BobUICore/qprocess.h>
#include <BobUICore/qstandardpaths.h>

#include <array>

using namespace BobUI::StringLiterals;

class tst_bobui_cmake_create : public QObject
{
    Q_OBJECT

public:
    tst_bobui_cmake_create();

private slots:
    void init();
    void initTestCase();
    void generatingCMakeLists_data();
    void generatingCMakeLists();

private:
    QString m_testWorkDir;
    QString m_shell;
    QString m_cmd;
};

tst_bobui_cmake_create::tst_bobui_cmake_create() : m_testWorkDir(qApp->applicationDirPath()) { }

void tst_bobui_cmake_create::initTestCase()
{
    QString binpath = QLibraryInfo::path(QLibraryInfo::BinariesPath);
#ifdef Q_OS_WINDOWS
    m_shell = QString("cmd.exe");
    m_cmd = QString("%1/bobui-cmake-create.bat").arg(binpath);
    const QString cmake = QStandardPaths::findExecutable("cmake.exe");
#else
    m_shell = QString("/bin/sh");
    m_cmd = QString("%1/bobui-cmake-create").arg(binpath);
    QVERIFY(QFile::exists(m_shell));
    const QString cmake = QStandardPaths::findExecutable("cmake");
#endif

    if (cmake.isEmpty())
        QSKIP("CMake not found");

    QVERIFY(QFile::exists(m_cmd));
}

void tst_bobui_cmake_create::init()
{
    QFETCH(QString, projectDirPath);
    QDir workDir(m_testWorkDir);
    QString fullProjectPath = m_testWorkDir + '/' + projectDirPath;
    if (workDir.exists(fullProjectPath)) {
        QDir projectDir(projectDirPath);
        projectDir.removeRecursively();
    }
    workDir.mkdir(projectDirPath);
    auto testDataPath = QFINDTESTDATA("testdata"_L1 + '/' + projectDirPath);
    QVERIFY(QFile::exists(testDataPath));

    for (const auto &fileInfo : QDir(testDataPath).entryInfoList(QDir::Files)) {
        QVERIFY(QFile::copy(fileInfo.absoluteFilePath(),
                            fullProjectPath + '/' + fileInfo.fileName()));
    }
}

void tst_bobui_cmake_create::generatingCMakeLists_data()
{
    BOBUIest::addColumn<QString>("projectDirPath");
    BOBUIest::addColumn<bool>("expectPass");
    BOBUIest::addColumn<QString>("workDir");

    const std::array<QLatin1StringView, 5> expectPass = {
        "cpp"_L1, "proto"_L1, "qml"_L1, "qrc"_L1, "ui"_L1,
    };

    const std::array<QString, 5> workDirs = {
        m_testWorkDir, ""_L1, m_testWorkDir, ""_L1, m_testWorkDir,
    };

    static_assert(expectPass.size() == workDirs.size());

    const QLatin1StringView expectFail[] = {
        "ui_only"_L1,
    };

    for (size_t i = 0; i < expectPass.size(); ++i) {
        const auto type = expectPass.at(i);
        BOBUIest::addRow("tst_bobui_cmake_create_%s", type.data())
                << QString("%1_project").arg(type) << true << workDirs.at(i);
    }

    for (const auto type : expectFail) {
        BOBUIest::addRow("tst_bobui_cmake_create_%s", type.data())
                << QString("%1_project").arg(type) << false << QString();
    }
}

void tst_bobui_cmake_create::generatingCMakeLists()
{
    QFETCH(QString, projectDirPath);
    QFETCH(bool, expectPass);
    QFETCH(QString, workDir);

    QString fullProjectPath = m_testWorkDir + '/' + projectDirPath;
    QProcess command;
    QStringList arguments = {
#ifdef Q_OS_WINDOWS
        "/C"_L1,
#endif
        m_cmd
    };

    QString workingDirectory = fullProjectPath;
    if (!workDir.isEmpty()) {
        workingDirectory = workDir;
        arguments.append(fullProjectPath);
    }
    command.setProgram(m_shell);
    command.setArguments(arguments);
    command.setWorkingDirectory(workingDirectory);

    command.start();
    QVERIFY(command.waitForFinished());
    QCOMPARE(command.exitCode() == 0, expectPass);

    QFile actualFile = QFile(fullProjectPath + '/' + "CMakeLists.txt"_L1);

    // Skip the rest if we expect that bobui-cmake-create should exit with error
    if (!expectPass) {
        QVERIFY(!actualFile.exists());
        return;
    }

    QFile expectedFile = QFile(fullProjectPath + '/' + "CMakeLists.txt.expected"_L1);
    QVERIFY(actualFile.open(QFile::ReadOnly));
    QVERIFY(expectedFile.open(QFile::ReadOnly));

    auto actualData = actualFile.readAll();
    actualData.replace(QByteArrayView("\r\n"), QByteArrayView("\n"));
    auto expectedData = expectedFile.readAll();
    expectedData.replace(QByteArrayView("\r\n"), QByteArrayView("\n"));

    static auto hash = [](const QByteArray &data) {
        return QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
    };
    QCOMPARE_EQ(hash(actualData), hash(expectedData));
}

BOBUIEST_MAIN(tst_bobui_cmake_create)
#include "tst_bobui_cmake_create.moc"
