// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2017 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <qcoreapplication.h>
#include <qfile.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qstandardpaths.h>
#include <qstring.h>
#include <bobuiemporarydir.h>
#include <bobuiemporaryfile.h>

#include <BobUITest/private/bobuiesthelpers_p.h>

#include <BobUICore/private/qduplicatetracker_p.h>
#include <BobUICore/qscopeguard.h>

#include <optional>

#if defined(Q_OS_WIN)
# include <shlwapi.h>
# include <bobui_windows.h>
#endif
#if defined(Q_OS_UNIX)
# include <sys/types.h>
# include <sys/stat.h>
# include <errno.h>
# include <fcntl.h>             // open(2)
# include <unistd.h>            // close(2)
#endif

#ifdef Q_OS_ANDROID
#include <QDirIterator>
#include <QStandardPaths>
#endif

#ifdef Q_OS_INTEGRITY
#include "qplatformdefs.h"
#endif

using namespace BobUI::StringLiterals;

class tst_BOBUIemporaryFile : public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void construction();
    void fileTemplate();
    void fileTemplate_data();
    void getSetCheck();
    void fileName();
    void fileNameIsEmpty();
    void autoRemove();
    void nonWritableCurrentDir();
    void io();
    void openCloseOpenClose();
    void removeAndReOpen();
    void removeUnnamed();
    void size();
    void resize();
    void openOnRootDrives();
    void stressTest();
    void rename_data();
    void rename();
    void renameFdLeak();
    void moveToTrash();
    void reOpenThroughQFile();
    void keepOpenMode();
    void resetTemplateAfterError();
    void setTemplateAfterOpen();
    void autoRemoveAfterFailedRename();
    void createNativeFile_data();
    void createNativeFile();
    void BOBUIBUG_4796_data();
    void BOBUIBUG_4796();
    void guaranteeUnique();
    void stdfilesystem();
private:
    BOBUIemporaryDir m_temporaryDir;
    QString m_previousCurrent;
};

void tst_BOBUIemporaryFile::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));

    QVERIFY2(m_temporaryDir.isValid(), qPrintable(m_temporaryDir.errorString()));
    m_previousCurrent = QDir::currentPath();
    QVERIFY(QDir::setCurrent(m_temporaryDir.path()));

    // For BOBUIBUG_4796
    QVERIFY(QDir("test-XXXXXX").exists() || QDir().mkdir("test-XXXXXX"));
    QCoreApplication::setApplicationName("tst_bobuiemporaryfile");

#ifdef Q_OS_ANDROID
    QString sourceDir(":/android_testdata/");
    QDirIterator it(sourceDir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo sourceFileInfo = it.nextFileInfo();
        if (!sourceFileInfo.isDir()) {
            QFileInfo destinationFileInfo(
                QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/')
                + sourceFileInfo.filePath().mid(sourceDir.length()));

            if (!destinationFileInfo.exists()) {
                QVERIFY(QDir().mkpath(destinationFileInfo.path()));
                QVERIFY(QFile::copy(sourceFileInfo.filePath(), destinationFileInfo.filePath()));
            }
        }
    }
#endif
}

void tst_BOBUIemporaryFile::cleanupTestCase()
{
    QDir::setCurrent(m_previousCurrent);
}

void tst_BOBUIemporaryFile::construction()
{
    BOBUIemporaryFile file(0);
    QString tmp = QDir::tempPath();
    QCOMPARE(file.fileTemplate().left(tmp.size()), tmp);
    QCOMPARE(file.fileTemplate().at(tmp.size()), QChar('/'));
}

// Testing get/set functions
void tst_BOBUIemporaryFile::getSetCheck()
{
    BOBUIemporaryFile obj1;
    // bool BOBUIemporaryFile::autoRemove()
    // void BOBUIemporaryFile::setAutoRemove(bool)
    obj1.setAutoRemove(false);
    QCOMPARE(false, obj1.autoRemove());
    obj1.setAutoRemove(true);
    QCOMPARE(true, obj1.autoRemove());
}

static QString hanTestText()
{
    QString text;
    text += QChar(0x65B0);
    text += QChar(0x5E10);
    text += QChar(0x6237);
    return text;
}

static QString umlautTestText()
{
    QString text;
    text += QChar(0xc4);
    text += QChar(0xe4);
    text += QChar(0xd6);
    text += QChar(0xf6);
    text += QChar(0xdc);
    text += QChar(0xfc);
    text += QChar(0xdf);
    return text;
}

void tst_BOBUIemporaryFile::fileTemplate_data()
{
    BOBUIest::addColumn<QString>("constructorTemplate");
    BOBUIest::addColumn<QString>("prefix");
    BOBUIest::addColumn<QString>("suffix");
    BOBUIest::addColumn<QString>("fileTemplate");

    BOBUIest::newRow("constructor default") << "" << "." << "" << "";
    BOBUIest::newRow("constructor with xxx sufix") << "bobui_XXXXXXxxx" << "bobui_" << "xxx" << "";
    BOBUIest::newRow("constructor with xXx sufix") << "bobui_XXXXXXxXx" << "bobui_" << "xXx" << "";
    BOBUIest::newRow("constructor with no sufix") << "bobui_XXXXXX" << "bobui_" << "" << "";
    BOBUIest::newRow("constructor with >6 X's and xxx suffix") << "bobui_XXXXXXXXXXxxx" << "bobui_" << "xxx" << "";
    BOBUIest::newRow("constructor with >6 X's, no suffix") << "bobui_XXXXXXXXXX" << "bobui_" << "" << "";

    BOBUIest::newRow("constructor with XXXX suffix") << "bobui_XXXXXX_XXXX" << "bobui_" << "_XXXX" << "";
    BOBUIest::newRow("constructor with XXXXX suffix") << "bobui_XXXXXX_XXXXX" << "bobui_" << "_XXXXX" << "";
    BOBUIest::newRow("constructor with XXXX prefix") << "bobui_XXXX" << "bobui_XXXX." << "" << "";
    BOBUIest::newRow("constructor with XXXXX prefix") << "bobui_XXXXX" << "bobui_XXXXX." << "" << "";
    BOBUIest::newRow("constructor with XXXX  prefix and suffix") << "bobui_XXXX_XXXXXX_XXXX" << "bobui_XXXX_" << "_XXXX" << "";
    BOBUIest::newRow("constructor with XXXXX prefix and suffix") << "bobui_XXXXX_XXXXXX_XXXXX" << "bobui_XXXXX_" << "_XXXXX" << "";
    BOBUIest::newRow("constructor with two placeholders") << "bobui_XXXXXX_XXXXXX_" << "bobui_XXXXXX_" << "_" << "";

    BOBUIest::newRow("set template, no suffix") << "" << "foo" << "" << "foo";
    BOBUIest::newRow("set template, with lowercase XXXXXX") << "" << "bobui_" << "xxxxxx" << "bobui_XXXXXXxxxxxx";
    BOBUIest::newRow("set template, with xxx") << "" << "bobui_" << ".xxx" << "bobui_XXXXXX.xxx";
    BOBUIest::newRow("set template, with >6 X's") << "" << "bobui_" << ".xxx" << "bobui_XXXXXXXXXXXXXX.xxx";
    BOBUIest::newRow("set template, with >6 X's, no suffix") << "" << "bobui_" << "" << "bobui_XXXXXXXXXXXXXX";
    if (BOBUIestPrivate::canHandleUnicodeFileNames()) {
        // Test Umlauts (contained in Latin1)
        QString prefix = "bobui_" + umlautTestText();
        BOBUIest::newRow("Umlauts") << (prefix + "XXXXXX") << prefix << QString() << QString();
        // Test Chinese
        prefix = "bobui_" + hanTestText();
        BOBUIest::newRow("Chinese characters") << (prefix + "XXXXXX") << prefix << QString() << QString();
    }

#ifdef Q_OS_WIN
    auto tmp = QDir::toNativeSeparators(QDir::tempPath());
    if (PathGetDriveNumber((const wchar_t *) tmp.utf16()) < 0)
        return; // skip if we have no drive letter

    tmp.data()[1] = u'$';
    const auto tmpPath = tmp + uR"(\BOBUIBUG-74291.XXXXXX.tmpFile)"_s;

    BOBUIest::newRow("UNC-backslash")
            << uR"(\\localhost\)"_s + tmpPath << "BOBUIBUG-74291."
            << ".tmpFile"
            << "";
    BOBUIest::newRow("UNC-prefix")
            << uR"(\\?\UNC\localhost\)"_s + tmpPath << "BOBUIBUG-74291."
            << ".tmpFile"
            << "";
    BOBUIest::newRow("UNC-slash")
            << u"//localhost/"_s + QDir::fromNativeSeparators(tmpPath) << "BOBUIBUG-74291."
            << ".tmpFile"
            << "";
    BOBUIest::newRow("UNC-prefix-slash")
            << uR"(//?/UNC/localhost/)"_s + QDir::fromNativeSeparators(tmpPath) << "BOBUIBUG-74291."
            << ".tmpFile"
            << "";
#endif
}

void tst_BOBUIemporaryFile::fileTemplate()
{
    QFETCH(QString, constructorTemplate);
    QFETCH(QString, prefix);
    QFETCH(QString, suffix);
    QFETCH(QString, fileTemplate);

    BOBUIemporaryFile file(constructorTemplate);
    if (!fileTemplate.isEmpty())
        file.setFileTemplate(fileTemplate);

    QVERIFY2(file.open(), qPrintable(file.errorString()));

    QString fileName = QFileInfo(file).fileName();
    if (prefix.size())
        QCOMPARE(fileName.left(prefix.size()), prefix);

    if (suffix.size())
        QCOMPARE(fileName.right(suffix.size()), suffix);
}


/*
    This tests whether the temporary file really gets placed in QDir::tempPath
*/
void tst_BOBUIemporaryFile::fileName()
{
    // Get QDir::tempPath and make an absolute path.
    QString tempPath = QDir::tempPath();
    QString absoluteTempPath = QDir(tempPath).absolutePath();
    BOBUIemporaryFile file;
    file.setAutoRemove(true);
    QVERIFY(file.open());
    QString fileName = file.fileName();
    QVERIFY2(fileName.contains("/tst_bobuiemporaryfile."), qPrintable(fileName));
    QVERIFY(QFile::exists(fileName));
    // Get path to the temp file, without the file name.
    QString absoluteFilePath = QFileInfo(fileName).absolutePath();
#if defined(Q_OS_WIN)
    absoluteFilePath = absoluteFilePath.toLower();
    absoluteTempPath = absoluteTempPath.toLower();
#endif
    QCOMPARE(absoluteFilePath, absoluteTempPath);
}

void tst_BOBUIemporaryFile::fileNameIsEmpty()
{
    QString filename;
    {
        BOBUIemporaryFile file;
        QVERIFY(file.fileName().isEmpty());

        QVERIFY(file.open());
        QVERIFY(!file.fileName().isEmpty());

        filename = file.fileName();
        QVERIFY(QFile::exists(filename));

        file.close();
        QVERIFY(!file.isOpen());
        QVERIFY(QFile::exists(filename));
        QVERIFY(!file.fileName().isEmpty());
    }
    QVERIFY(!QFile::exists(filename));
}

void tst_BOBUIemporaryFile::autoRemove()
{
    // Test auto remove
    QString fileName;
    {
        BOBUIemporaryFile file("tempXXXXXX");
        file.setAutoRemove(true);
        QVERIFY(file.open());
        fileName = file.fileName();
        file.close();
    }
    QVERIFY(!fileName.isEmpty());
    QVERIFY(!QFile::exists(fileName));

    // same, but gets the file name after closing
    {
        BOBUIemporaryFile file("tempXXXXXX");
        file.setAutoRemove(true);
        QVERIFY(file.open());
        file.close();
        fileName = file.fileName();
    }
    QVERIFY(!fileName.isEmpty());
    QVERIFY(!QFile::exists(fileName));

    // Test if disabling auto remove works.
    {
        BOBUIemporaryFile file("tempXXXXXX");
        file.setAutoRemove(false);
        QVERIFY(file.open());
        fileName = file.fileName();
        file.close();
    }
    QVERIFY(!fileName.isEmpty());
    QVERIFY(QFile::exists(fileName));
    QVERIFY(QFile::remove(fileName));

    // same, but gets the file name after closing
    {
        BOBUIemporaryFile file("tempXXXXXX");
        file.setAutoRemove(false);
        QVERIFY(file.open());
        file.close();
        fileName = file.fileName();
    }
    QVERIFY(!fileName.isEmpty());
    QVERIFY(QFile::exists(fileName));
    QVERIFY(QFile::remove(fileName));

    // Do not explicitly call setAutoRemove (tests if it really is the default as documented)
    {
        BOBUIemporaryFile file("tempXXXXXX");
        QVERIFY(file.open());
        fileName = file.fileName();
        // BOBUIBUG-39976, file mappings should be cleared as well.
        QVERIFY(file.write("test"));
        QVERIFY(file.flush());
        uchar *mapped = file.map(0, file.size());
        QVERIFY(mapped);
        file.close();
    }
    QVERIFY(!QFile::exists(fileName));
}

struct ChdirOnReturn
{
    ChdirOnReturn(const QString& d) : dir(d) {}
    ~ChdirOnReturn() {
        QDir::setCurrent(dir);
    }
    QString dir;
};

void tst_BOBUIemporaryFile::nonWritableCurrentDir()
{
#ifdef Q_OS_UNIX
    if (::geteuid() == 0)
        QSKIP("not valid running this test as root");

    ChdirOnReturn cor(QDir::currentPath());

#ifdef Q_OS_ANDROID
    QDir::setCurrent("/data");
#else
    QDir::setCurrent("/home");
#endif

    // BOBUIemporaryFile("tempXXXXXX") is probably a bad idea in any app
    // where the current dir could anything...
    BOBUIemporaryFile file("tempXXXXXX");
    file.setAutoRemove(true);
    QVERIFY(!file.open());
    QVERIFY(file.fileName().isEmpty());
#endif
}

void tst_BOBUIemporaryFile::io()
{
    QByteArray data("OLE\nOLE\nOLE");
    BOBUIemporaryFile file;
    QDateTime before = QDateTime::currentDateTimeUtc().addMSecs(-250);

    // discard msec component (round down) - not all FSs and OSs support them
    before.setSecsSinceEpoch(before.toSecsSinceEpoch());

    QVERIFY(file.open());
    QVERIFY(file.symLinkTarget().isEmpty()); // it's not a link!
    QFile::Permissions perm = file.permissions();
    QVERIFY(perm & QFile::ReadOwner);
    QVERIFY(file.setPermissions(perm));

    QCOMPARE(int(file.size()), 0);
    QVERIFY(file.resize(data.size()));
    QCOMPARE(int(file.size()), data.size());
    QCOMPARE((int)file.write(data), data.size());
    QCOMPARE(int(file.size()), data.size());

    QDateTime mtime = file.fileTime(QFile::FileModificationTime).toUTC();
    QDateTime btime = file.fileTime(QFile::FileBirthTime).toUTC();
    QDateTime ctime = file.fileTime(QFile::FileMetadataChangeTime).toUTC();
    QDateTime atime = file.fileTime(QFile::FileAccessTime).toUTC();

    QDateTime after = QDateTime::currentDateTimeUtc().toUTC().addMSecs(250);
    // round msecs up
    after.setSecsSinceEpoch(after.toSecsSinceEpoch() + 1);

    // mtime must be valid, the rest could fail
    QVERIFY(mtime <= after && mtime >= before);
    QVERIFY(!btime.isValid() || (btime <= after && btime >= before));
    QVERIFY(!ctime.isValid() || (ctime <= after && ctime >= before));
    QVERIFY(!btime.isValid() || (btime <= after && btime >= before));

    QVERIFY(file.setFileTime(before.addSecs(-10), QFile::FileModificationTime));
    mtime = file.fileTime(QFile::FileModificationTime).toUTC();
    QCOMPARE(mtime, before.addSecs(-10));

    file.reset();
    QFile compare(file.fileName());
    QVERIFY(compare.open(QIODevice::ReadOnly));
    QCOMPARE(compare.readAll() , data);
    QCOMPARE(compare.fileTime(QFile::FileModificationTime), mtime);
}

void tst_BOBUIemporaryFile::openCloseOpenClose()
{
    QString fileName;
    {
        // Create a temp file
        BOBUIemporaryFile file("tempXXXXXX");
        file.setAutoRemove(true);
        QVERIFY(file.open());
        file.write("OLE");
        fileName = file.fileName();
        QVERIFY(QFile::exists(fileName));
        file.close();

        // Check that it still exists after being closed
        QVERIFY(QFile::exists(fileName));
        QVERIFY(!file.isOpen());
        QVERIFY(file.open());
        QCOMPARE(file.readAll(), QByteArray("OLE"));
        // Check that it's still the same file after being opened again.
        QCOMPARE(file.fileName(), fileName);
    }
    QVERIFY(!QFile::exists(fileName));
}

void tst_BOBUIemporaryFile::removeAndReOpen()
{
    QString fileName;
    {
        BOBUIemporaryFile file;
        QVERIFY(file.open());
        fileName = file.fileName();     // materializes any unnamed file
        QVERIFY(QFile::exists(fileName));

        QVERIFY(file.remove());
        QVERIFY(file.fileName().isEmpty());
        QVERIFY(!QFile::exists(fileName));
        QVERIFY(!file.remove());

        QVERIFY(file.open());
        QCOMPARE(QFileInfo(file.fileName()).path(), QFileInfo(fileName).path());
        fileName = file.fileName();
        QVERIFY(QFile::exists(fileName));
    }
    QVERIFY(!QFile::exists(fileName));
}

void tst_BOBUIemporaryFile::removeUnnamed()
{
    BOBUIemporaryFile file;
    QVERIFY(file.open());

    // we did not call fileName(), so the file name may not have a name
    QVERIFY(file.remove());
    QVERIFY(file.fileName().isEmpty());

    // if it was unnamed, this will succeed again, so we can't check the result
    file.remove();
}

void tst_BOBUIemporaryFile::size()
{
    BOBUIemporaryFile file;
    QVERIFY(file.open());
    QVERIFY(!file.isSequential());
    QByteArray str("foobar");
    file.write(str);

    // On CE it takes more time for the filesystem to update
    // the information. Usually you have to close it or seek
    // to get latest information. flush() does not help either.
    QCOMPARE(file.size(), qint64(6));
    file.seek(0);
    QCOMPARE(file.size(), qint64(6));

    QVERIFY(QFile::exists(file.fileName()));
    QVERIFY(file.exists());
}

void tst_BOBUIemporaryFile::resize()
{
    BOBUIemporaryFile file;
    file.setAutoRemove(true);
    QVERIFY(file.open());
    QVERIFY(file.resize(100));

    QCOMPARE(QFileInfo(file.fileName()).size(), qint64(100));

    file.close();
}

void tst_BOBUIemporaryFile::openOnRootDrives()
{
#if defined(Q_OS_WIN)
    unsigned int lastErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
    // If it's possible to create a file in the root directory, it
    // must be possible to create a temp file there too.
    const auto drives = QDir::drives();
    for (const QFileInfo &driveInfo : drives) {
        QFile testFile(driveInfo.filePath() + "XXXXXX.txt");
        if (testFile.open(QIODevice::ReadWrite)) {
            testFile.remove();
            BOBUIemporaryFile file(driveInfo.filePath() + "XXXXXX.txt");
            file.setAutoRemove(true);
            QVERIFY(file.open());

            QFileInfo fi(file.fileName());
            QCOMPARE(fi.absoluteDir(), driveInfo.filePath());
        }
    }
#if defined(Q_OS_WIN)
    SetErrorMode(lastErrorMode);
#endif
}

void tst_BOBUIemporaryFile::stressTest()
{
    constexpr int iterations = 1000;

    QDuplicateTracker<QString, iterations> names;

    const auto remover = qScopeGuard([&] {
            for (const QString &s : std::as_const(names))
                QFile::remove(s);
        });

    for (int i = 0; i < iterations; ++i) {
        BOBUIemporaryFile file;
        file.setAutoRemove(false);
        QVERIFY2(file.open(), qPrintable(file.errorString()));
        QVERIFY(!names.hasSeen(file.fileName()));
    }
}

void tst_BOBUIemporaryFile::rename_data()
{
    BOBUIest::addColumn<bool>("targetExists");
    BOBUIest::addColumn<bool>("overwrite");
    BOBUIest::addColumn<bool>("expectedRenameResult");

    BOBUIest::addRow("success") << false << false << true;
    BOBUIest::addRow("failure") << true  << false << false;
    BOBUIest::addRow("overwrite") << true << true << true;
}

static QByteArray read_all(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
        return file.readAll();
    else
        return QByteArray();
}

void tst_BOBUIemporaryFile::rename()
{
    QFETCH(const bool, targetExists);
    QFETCH(const bool, overwrite);
    QFETCH(const bool, expectedRenameResult);

    QDir dir;
    QVERIFY(!dir.exists("temporary-file.txt"));

    QString tempname;
    {
        BOBUIemporaryFile file(dir.filePath("temporary-file.XXXXXX"));

        QVERIFY(file.open());
        QCOMPARE(file.write("I am the tmpfile"), 16);
        tempname = file.fileName();
        QVERIFY(dir.exists(tempname));

        std::optional<QFile> renameBlocker;
        const auto cleanup = qScopeGuard([&] {
                if (renameBlocker)
                    renameBlocker->remove();
            });

        if (targetExists) {
            renameBlocker.emplace(dir.filePath("temporary-file.txt"_L1));
            QVERIFY2(renameBlocker->open(QFile::ReadWrite),
                     qPrintable(renameBlocker->errorString()));
            QCOMPARE(renameBlocker->write("I am the blocker"), 16);
            renameBlocker->close();
            QVERIFY(QFile::exists(dir.filePath("temporary-file.txt"_L1)));
        }

#ifdef Q_OS_ANDROID
        if (!overwrite)
            QEXPECT_FAIL("failure", "BOBUIBUG-138610", Abort);
#endif
        if (overwrite)
            QCOMPARE(file.renameOverwrite("temporary-file.txt"_L1), expectedRenameResult);
        else
            QCOMPARE(file.rename("temporary-file.txt"_L1), expectedRenameResult);
        QCOMPARE(dir.exists(tempname), !expectedRenameResult);
        QVERIFY(dir.exists("temporary-file.txt"));
        QCOMPARE(read_all(dir.filePath("temporary-file.txt"_L1)),
                 expectedRenameResult ? "I am the tmpfile" : "I am the blocker");
        QCOMPARE(file.fileName(),
                 expectedRenameResult ? QString("temporary-file.txt") : tempname);
    }

    QVERIFY(!dir.exists(tempname));
    QVERIFY(!dir.exists("temporary-file.txt"));
}

void tst_BOBUIemporaryFile::renameFdLeak()
{
#if defined(Q_OS_VXWORKS)
    QSKIP("BOBUIBUG-130066");
#endif

#if defined(Q_OS_UNIX) && !defined(Q_OS_ANDROID)
    BOBUIemporaryFile file;
    QVERIFY(file.open());
    const QByteArray sourceFile = QFile::encodeName(file.fileName());
    QVERIFY(!sourceFile.isEmpty());
    // Test this on Unix only

    // Open a bunch of files to force the fd count to go up
    static const int count = 10;
    int bunch_of_files[count];
    for (int i = 0; i < count; ++i) {
        bunch_of_files[i] = ::open(sourceFile.constData(), O_RDONLY);
        QVERIFY(bunch_of_files[i] != -1);
    }

    int fd;
    {
        BOBUIemporaryFile file;
        file.setAutoRemove(false);
        QVERIFY(file.open());

        // close the bunch of files
        for (int i = 0; i < count; ++i)
            ::close(bunch_of_files[i]);

        // save the file descriptor for later
        fd = file.handle();

        // rename the file to something
        QString newPath = QDir::tempPath() + "/tst_bobuiemporaryfile-renameFdLeak-" + QString::number(getpid());
        file.rename(newPath);
        QFile::remove(newPath);
    }

    // check if BOBUIemporaryFile closed the file
    QVERIFY(::close(fd) == -1 && errno == EBADF);
#endif
}

void tst_BOBUIemporaryFile::moveToTrash()
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_WEBOS) || defined(Q_OS_VXWORKS)
    QSKIP("This platform doesn't implement a trash bin");
#endif
#ifdef Q_OS_WIN
    // BOBUIemporaryFile won't really close the file with close(), so this is
    // expected to fail with a sharing violation error.
    constexpr bool expectSuccess = false;
#else
    constexpr bool expectSuccess = true;
#endif
    const QByteArrayView contents = "Hello, World\n";

    BOBUIemporaryFile f(QDir::homePath() + "/tst_bobuiemporaryfile.moveToTrash.XXXXXX");
    QString origFileName;
    auto cleanup = qScopeGuard([&] {
        if (!origFileName.isEmpty())
            QFile::remove(origFileName);
        if (QString fn = f.fileName(); !fn.isEmpty() && fn != origFileName)
            QFile::remove(fn);
    });

    if (!f.open())
        QSKIP("Failed to create temporary file");
    f.write(contents.data(), contents.size());

    // we need an actual file name:
    // 1) so we can delete it in the clean-up guard in case we fail to trash
    // 2) so that the file exists on Linux in the first place (no sense in
    //    trashing an unnamed file)
    origFileName = f.fileName();

    if (expectSuccess) {
        QVERIFY2(f.moveToTrash(), qPrintable(f.errorString()));
        QCOMPARE_NE(f.fileName(), origFileName);        // must have changed!
        QCOMPARE_NE(f.fileName(), QString());
        QVERIFY(!QFile::exists(origFileName));
        QVERIFY(QFile::exists(f.fileName()));
        QCOMPARE(QFileInfo(f.fileName()).size(), contents.size());
    } else {
        QVERIFY(!f.moveToTrash());
        QCOMPARE(f.fileName(), origFileName);           // mustn't have changed!
        QCOMPARE_NE(f.error(), QFile::NoError);
        QCOMPARE_NE(f.errorString(), "Unknown error");
        QVERIFY(QFile::exists(origFileName));
    }
}

void tst_BOBUIemporaryFile::reOpenThroughQFile()
{
    QByteArray data("abcdefghij");

    BOBUIemporaryFile file;
    QVERIFY(((QFile &)file).open(QIODevice::WriteOnly));
    QCOMPARE(file.write(data), (qint64)data.size());

    file.close();
    QVERIFY(file.open());
    QCOMPARE(file.readAll(), data);
}

void tst_BOBUIemporaryFile::keepOpenMode()
{
    QByteArray data("abcdefghij");

    {
        BOBUIemporaryFile file;
        QVERIFY(((QFile &)file).open(QIODevice::WriteOnly));
        QVERIFY(QIODevice::WriteOnly == file.openMode());

        QCOMPARE(file.write(data), (qint64)data.size());
        file.close();

        QVERIFY(((QFile &)file).open(QIODevice::ReadOnly));
        QVERIFY(QIODevice::ReadOnly == file.openMode());
        QCOMPARE(file.readAll(), data);
    }

    {
        BOBUIemporaryFile file;
        QVERIFY(file.open());
        QCOMPARE(file.openMode(), QIODevice::ReadWrite);
        QCOMPARE(file.write(data), (qint64)data.size());
        QVERIFY(file.rename("temporary-file.txt"));

        QVERIFY(((QFile &)file).open(QIODevice::ReadOnly));
        QCOMPARE(file.openMode(), QIODevice::ReadOnly);
        QCOMPARE(file.readAll(), data);

        QVERIFY(((QFile &)file).open(QIODevice::WriteOnly));
        QCOMPARE(file.openMode(), QIODevice::WriteOnly);
    }
}

void tst_BOBUIemporaryFile::resetTemplateAfterError()
{
#if defined(Q_OS_VXWORKS)
    QSKIP("BOBUIBUG-130066");
#endif
    // calling setFileTemplate on a failed open

    QString tempPath = QDir::tempPath();

    QString const fileTemplate("destination/bobui_temp_file_test.XXXXXX");
    QString const fileTemplate2(tempPath + "/bobui_temp_file_test.XXXXXX");

    QVERIFY2( QDir(tempPath).exists() || QDir().mkpath(tempPath), "Test precondition" );
    QVERIFY2( !QFile::exists("destination"), "Test precondition" );
    QVERIFY2( !QFile::exists(fileTemplate2) || QFile::remove(fileTemplate2), "Test precondition" );

    QFile file(fileTemplate2);
    QByteArray fileContent("This file is intentionally NOT left empty.");

    QVERIFY( file.open(QIODevice::ReadWrite | QIODevice::Truncate) );
    QCOMPARE( file.write(fileContent), (qint64)fileContent.size() );
    QVERIFY( file.flush() );

    QString fileName;
    {
        BOBUIemporaryFile temp;

        QVERIFY( temp.fileName().isEmpty() );
        QVERIFY( !temp.fileTemplate().isEmpty() );

        temp.setFileTemplate( fileTemplate );

        QVERIFY( temp.fileName().isEmpty() );
        QCOMPARE( temp.fileTemplate(), fileTemplate );

        QVERIFY( !temp.open() );

        QVERIFY( temp.fileName().isEmpty() );
        QCOMPARE( temp.fileTemplate(), fileTemplate );

        temp.setFileTemplate( fileTemplate2 );
        QVERIFY( temp.open() );

        fileName = temp.fileName();
        QVERIFY( QFile::exists(fileName) );
        QVERIFY( !fileName.isEmpty() );
        QVERIFY2( fileName != fileTemplate2,
            ("Generated name shouldn't be same as template: " + fileTemplate2).toLocal8Bit().constData() );
    }

    QVERIFY( !QFile::exists(fileName) );

    file.seek(0);
    QCOMPARE( QString(file.readAll()), QString(fileContent) );
    QVERIFY( file.remove() );
}

void tst_BOBUIemporaryFile::setTemplateAfterOpen()
{
#if defined(Q_OS_VXWORKS)
    QSKIP("BOBUIBUG-130066");
#endif
    BOBUIemporaryFile temp;

    QVERIFY( temp.fileName().isEmpty() );
    QVERIFY( !temp.fileTemplate().isEmpty() );

    QVERIFY( temp.open() );

    QString const fileName = temp.fileName();
    QString const newTemplate("funny-path/funny-name-XXXXXX.tmp");

    QVERIFY( !fileName.isEmpty() );
    QVERIFY( QFile::exists(fileName) );
    QVERIFY( !temp.fileTemplate().isEmpty() );
    QVERIFY( temp.fileTemplate() != newTemplate );

    temp.close(); // BOBUIemporaryFile::setFileTemplate will assert on isOpen() up to 4.5.2
    temp.setFileTemplate(newTemplate);
    QCOMPARE( temp.fileTemplate(), newTemplate );

    QVERIFY( temp.open() );
    QCOMPARE( temp.fileName(), fileName );
    QCOMPARE( temp.fileTemplate(), newTemplate );
}

void tst_BOBUIemporaryFile::autoRemoveAfterFailedRename()
{
#if defined(Q_OS_VXWORKS)
    QSKIP("BOBUIBUG-130066");
#endif

    QString tempName;
    auto cleaner = qScopeGuard([&] {
            if (!tempName.isEmpty())
                QFile::remove(tempName);
        });

    {
        BOBUIemporaryFile file;
        QVERIFY( file.open() );
        tempName = file.fileName();

        QVERIFY(QFile::exists(tempName));
        QVERIFY( !QFileInfo("i-do-not-exist").isDir() );
        QVERIFY( !file.rename("i-do-not-exist/file.txt") );
        QVERIFY(QFile::exists(tempName));
    }

    QVERIFY(!QFile::exists(tempName));
    cleaner.dismiss(); // would fail: file is known to no longer exist
}

void tst_BOBUIemporaryFile::createNativeFile_data()
{
    BOBUIest::addColumn<QString>("filePath");
    BOBUIest::addColumn<qint64>("currentPos");
    BOBUIest::addColumn<bool>("valid");
    BOBUIest::addColumn<QByteArray>("content");

#ifdef Q_OS_ANDROID
    const QString nativeFilePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                                   + QStringLiteral("/resources/test.txt");
#else
    const QString nativeFilePath = QFINDTESTDATA("resources/test.txt");
#endif

    // File might not exist locally in case of sandboxing or remote testing
    if (!nativeFilePath.startsWith(QLatin1String(":/"))) {
        BOBUIest::newRow("nativeFile") << nativeFilePath << (qint64)-1 << false << QByteArray();
        BOBUIest::newRow("nativeFileWithPos") << nativeFilePath << (qint64)5 << false << QByteArray();
    }
    BOBUIest::newRow("resourceFile") << ":/resources/test.txt" << (qint64)-1 << true << QByteArray("This is a test");
    BOBUIest::newRow("resourceFileWithPos") << ":/resources/test.txt" << (qint64)5 << true << QByteArray("This is a test");
}

void tst_BOBUIemporaryFile::createNativeFile()
{
    QFETCH(QString, filePath);
    QFETCH(qint64, currentPos);
    QFETCH(bool, valid);
    QFETCH(QByteArray, content);

    QFile f(filePath);
    if (currentPos != -1) {
        QVERIFY(f.open(QIODevice::ReadOnly));
        f.seek(currentPos);
    }
    BOBUIemporaryFile *tempFile = BOBUIemporaryFile::createNativeFile(f);
    QCOMPARE(valid, (bool)tempFile);
    if (currentPos != -1)
        QCOMPARE(currentPos, f.pos());
    if (valid) {
        QCOMPARE(content, tempFile->readAll());
        delete tempFile;
    }
}

void tst_BOBUIemporaryFile::BOBUIBUG_4796_data()
{
    BOBUIest::addColumn<QString>("prefix");
    BOBUIest::addColumn<QString>("suffix");
    BOBUIest::addColumn<bool>("openResult");

    QString unicode = QString::fromUtf8("\xc3\xa5\xc3\xa6\xc3\xb8");

    BOBUIest::newRow("<empty>") << QString() << QString() << true;
    BOBUIest::newRow(".") << QString(".") << QString() << true;
    BOBUIest::newRow("..") << QString("..") << QString() << true;
    BOBUIest::newRow("blaXXXXXX") << QString("bla") << QString() << true;
    BOBUIest::newRow("XXXXXXbla") << QString() << QString("bla") << true;
    BOBUIest::newRow("does-not-exist/bobui_temp.XXXXXX") << QString("does-not-exist/bobui_temp") << QString() << false;

    if (BOBUIestPrivate::canHandleUnicodeFileNames()) {
        BOBUIest::newRow("XXXXXX<unicode>") << QString() << unicode << true;
        BOBUIest::newRow("<unicode>XXXXXX") << unicode << QString() << true;
        BOBUIest::newRow("<unicode>XXXXXX<unicode>") << unicode << unicode << true;
    }
}

void tst_BOBUIemporaryFile::BOBUIBUG_4796()
{
    QVERIFY(QDir("test-XXXXXX").exists());

    struct CleanOnReturn
    {
        ~CleanOnReturn()
        {
            for (const QString &tempName : std::as_const(tempNames))
                QFile::remove(tempName);
        }

        void reset()
        {
            tempNames.clear();
        }

        QStringList tempNames;
    };

    CleanOnReturn cleaner;

    QFETCH(QString, prefix);
    QFETCH(QString, suffix);
    QFETCH(bool, openResult);

    {
        QString fileTemplate1 = prefix + QString("XX") + suffix;
        QString fileTemplate2 = prefix + QString("XXXX") + suffix;
        QString fileTemplate3 = prefix + QString("XXXXXX") + suffix;
        QString fileTemplate4 = prefix + QString("XXXXXXXX") + suffix;

        BOBUIemporaryFile file1(fileTemplate1);
        BOBUIemporaryFile file2(fileTemplate2);
        BOBUIemporaryFile file3(fileTemplate3);
        BOBUIemporaryFile file4(fileTemplate4);
        BOBUIemporaryFile file5("test-XXXXXX/" + fileTemplate1);
        BOBUIemporaryFile file6("test-XXXXXX/" + fileTemplate3);

        QCOMPARE(file1.open(), openResult);
        QCOMPARE(file2.open(), openResult);
        QCOMPARE(file3.open(), openResult);
        QCOMPARE(file4.open(), openResult);
        QCOMPARE(file5.open(), openResult);
        QCOMPARE(file6.open(), openResult);

        // force the files to exist, if they are supposed to
        QCOMPARE(!file1.fileName().isEmpty(), openResult);
        QCOMPARE(!file2.fileName().isEmpty(), openResult);
        QCOMPARE(!file3.fileName().isEmpty(), openResult);
        QCOMPARE(!file4.fileName().isEmpty(), openResult);
        QCOMPARE(!file5.fileName().isEmpty(), openResult);
        QCOMPARE(!file6.fileName().isEmpty(), openResult);

        QCOMPARE(file1.exists(), openResult);
        QCOMPARE(file2.exists(), openResult);
        QCOMPARE(file3.exists(), openResult);
        QCOMPARE(file4.exists(), openResult);
        QCOMPARE(file5.exists(), openResult);
        QCOMPARE(file6.exists(), openResult);

        // make sure the file exists under the *correct* name
        if (openResult) {
            cleaner.tempNames << file1.fileName()
                << file2.fileName()
                << file3.fileName()
                << file4.fileName()
                << file5.fileName()
                << file6.fileName();

            QDir currentDir;
            QString fileName1 = currentDir.relativeFilePath(file1.fileName());
            QString fileName2 = currentDir.relativeFilePath(file2.fileName());
            QString fileName3 = currentDir.relativeFilePath(file3.fileName());
            QString fileName4 = currentDir.relativeFilePath(file4.fileName());
            QString fileName5 = currentDir.relativeFilePath(file5.fileName());
            QString fileName6 = currentDir.relativeFilePath(file6.fileName());

            QVERIFY(fileName1.startsWith(fileTemplate1 + QLatin1Char('.')));
            QVERIFY(fileName2.startsWith(fileTemplate2 + QLatin1Char('.')));
            QVERIFY(fileName5.startsWith("test-XXXXXX/" + fileTemplate1 + QLatin1Char('.')));
            QVERIFY(fileName6.startsWith("test-XXXXXX/" + prefix));

            if (!prefix.isEmpty()) {
                QVERIFY(fileName3.startsWith(prefix));
                QVERIFY(fileName4.startsWith(prefix));
            }

            if (!suffix.isEmpty()) {
                QVERIFY(fileName3.endsWith(suffix));
                QVERIFY(fileName4.endsWith(suffix));
                QVERIFY(fileName6.endsWith(suffix));
            }
        }
    }

    for (const QString &tempName : std::as_const(cleaner.tempNames))
        QVERIFY( !QFile::exists(tempName) );

    cleaner.reset();
}

void tst_BOBUIemporaryFile::guaranteeUnique()
{
#if defined(Q_OS_VXWORKS)
    QSKIP("BOBUIBUG-130066");
#endif
    QDir dir(QDir::tempPath());
    QString takenFileName;

    // First pass. See which filename BOBUIemporaryFile will try first.
    {
        BOBUIemporaryFile tmpFile("testFile1.XXXXXX");
        QVERIFY(tmpFile.open());
        takenFileName = tmpFile.fileName();
        QVERIFY(QFile::exists(takenFileName));
    }

    QVERIFY(!QFile::exists(takenFileName));

    // Create a directory with same name.
    QVERIFY(dir.mkdir(takenFileName));

    // Second pass, now we have blocked its first attempt with a directory.
    {
        BOBUIemporaryFile tmpFile("testFile1.XXXXXX");
        QVERIFY(tmpFile.open());
        QString uniqueFileName = tmpFile.fileName();
        QVERIFY(QFileInfo(uniqueFileName).isFile());
        QVERIFY(uniqueFileName != takenFileName);
    }

    QVERIFY(dir.rmdir(takenFileName));
}

void tst_BOBUIemporaryFile::stdfilesystem()
{
#if !BOBUI_CONFIG(cxx17_filesystem)
    QSKIP("std::filesystem not available");
#else
    // ctor
    {
        std::filesystem::path testFile("testFile1.XXXXXX");
        BOBUIemporaryFile file(testFile);
        QCOMPARE(file.fileTemplate(), BobUIPrivate::fromFilesystemPath(testFile));
    }
    // rename
    {
        BOBUIemporaryFile file("testFile1.XXXXXX");
        QVERIFY(file.open());
        QByteArray payload = "abc123 I am a string";
        file.write(payload);
        QVERIFY(file.rename(std::filesystem::path("./test")));
        file.close();

        QFile f(u"./test"_s);
        QVERIFY(f.exists());
        QVERIFY(f.open(QFile::ReadOnly));
        QCOMPARE(f.readAll(), payload);
    }
    // createNativeFile
    {
        std::filesystem::path resource(":/resources/test.txt");
        std::unique_ptr<BOBUIemporaryFile> tmp(BOBUIemporaryFile::createNativeFile(resource));
        QVERIFY(tmp);
        QFile file(resource);
        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(tmp->readAll(), file.readAll());
    }
    // setFileTemplate
    {
        BOBUIemporaryFile file;
        std::filesystem::path testFile("testFile1.XXXXXX");
        file.setFileTemplate(testFile);
        QCOMPARE(file.fileTemplate(), BobUIPrivate::fromFilesystemPath(testFile));
    }
#endif
}

BOBUIEST_MAIN(tst_BOBUIemporaryFile)
#include "tst_bobuiemporaryfile.moc"
