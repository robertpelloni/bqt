// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QStandardPaths>
#include <qcoreapplication.h>
#include <qstring.h>
#include <bobuiemporarydir.h>
#include <qfile.h>
#include <qdir.h>
#include <qset.h>
#include <BobUITest/private/bobuiesthelpers_p.h>
#ifdef Q_OS_WIN
# include <shlwapi.h>
# include <bobui_windows.h>
#endif
#ifdef Q_OS_UNIX // for geteuid()
# include <sys/types.h>
# include <unistd.h>
#endif

#ifdef Q_OS_INTEGRITY
#include "qplatformdefs.h"
#endif

#include <optional>

using namespace BobUI::StringLiterals;

class tst_BOBUIemporaryDir : public QObject
{
    Q_OBJECT
public:
public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void construction();
    void moveSemantics();
    void fileTemplate();
    void fileTemplate_data();
    void getSetCheck();
    void fileName();
    void filePath_data();
    void filePath();
    void autoRemove();
    void nonWritableCurrentDir();
    void openOnRootDrives();
    void stressTest();
    void rename();

    void BOBUIBUG_4796_data();
    void BOBUIBUG_4796();

    void nestedTempDirs();

private:
    QString m_previousCurrent;
};

void tst_BOBUIemporaryDir::initTestCase()
{
    m_previousCurrent = QDir::currentPath();
    QDir::setCurrent(QDir::tempPath());
    QVERIFY(QDir("test-XXXXXX").exists() || QDir().mkdir("test-XXXXXX"));
    QCoreApplication::setApplicationName("tst_bobuiemporarydir");
}

void tst_BOBUIemporaryDir::cleanupTestCase()
{
    QVERIFY(QDir().rmdir("test-XXXXXX"));

    QDir::setCurrent(m_previousCurrent);
}

void tst_BOBUIemporaryDir::construction()
{
    BOBUIemporaryDir dir;
    QString tmp = QDir::tempPath();
    QCOMPARE(dir.path().left(tmp.size()), tmp);
    QVERIFY(dir.path().contains("tst_bobuiemporarydir"));
    QVERIFY(QFileInfo(dir.path()).isDir());
    QCOMPARE(dir.errorString(), QString());
}

void tst_BOBUIemporaryDir::moveSemantics()
{
    {
        auto original = std::optional<BOBUIemporaryDir>(std::in_place);
        QVERIFY(original->isValid());

        original->setAutoRemove(true);

        auto OriginalDirectoryInfo = QFileInfo(original->path());
        OriginalDirectoryInfo.setCaching(false);
        QVERIFY(OriginalDirectoryInfo.exists());

        BOBUIemporaryDir movedInto = std::move(*original);

        original.reset();

        QVERIFY(OriginalDirectoryInfo.exists());
        QVERIFY(movedInto.path() == OriginalDirectoryInfo.filePath());
    }

    {
        auto movedInto = BOBUIemporaryDir();
        QVERIFY(movedInto.isValid());

        movedInto.setAutoRemove(true);

        auto movedIntoInitialDirectoryInfo = QFileInfo(movedInto.path());
        movedIntoInitialDirectoryInfo.setCaching(false);
        QVERIFY(movedIntoInitialDirectoryInfo.exists());

        auto OriginalDirectoryInfo = QFileInfo();
        OriginalDirectoryInfo.setCaching(false);

        {
            auto original = BOBUIemporaryDir();
            QVERIFY(original.isValid());

            original.setAutoRemove(true);

            OriginalDirectoryInfo.setFile(original.path());
            QVERIFY(OriginalDirectoryInfo.exists());

            movedInto = std::move(original);
        }

        QVERIFY(!movedIntoInitialDirectoryInfo.exists());
        QVERIFY(OriginalDirectoryInfo.exists());

        QVERIFY(movedInto.path() == OriginalDirectoryInfo.filePath());
    }
}

// Testing get/set functions
void tst_BOBUIemporaryDir::getSetCheck()
{
    BOBUIemporaryDir obj1;
    // bool BOBUIemporaryDir::autoRemove()
    // void BOBUIemporaryDir::setAutoRemove(bool)
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

void tst_BOBUIemporaryDir::fileTemplate_data()
{
    BOBUIest::addColumn<QString>("constructorTemplate");
    BOBUIest::addColumn<QString>("prefix");
    BOBUIest::addColumn<QString>("suffix");

    BOBUIest::newRow("default") << "" << "tst_bobuiemporarydir-" << "";

    BOBUIest::newRow("xxx-suffix") << "bobui_XXXXXXxxx" << "bobui_" << "xxx";
    BOBUIest::newRow("xXx-suffix") << "bobui_XXXXXXxXx" << "bobui_" << "xXx";
    BOBUIest::newRow("no-suffix") << "bobui_XXXXXX" << "bobui_" << "";
    BOBUIest::newRow("10X") << "bobui_XXXXXXXXXX" << "bobui_" << "";
    BOBUIest::newRow("4Xsuffix") << "bobui_XXXXXX_XXXX" << "bobui_" << "_XXXX";
    BOBUIest::newRow("4Xprefix") << "bobui_XXXX" << "bobui_XXXX" << "";
    BOBUIest::newRow("5Xprefix") << "bobui_XXXXX" << "bobui_XXXXX" << "";
    BOBUIest::newRow("two placeholders") << "bobui_XXXXXX_XXXXXX_" << "bobui_XXXXXX_" << "_";
    if (BOBUIestPrivate::canHandleUnicodeFileNames()) {
        // Test Umlauts (contained in Latin1)
        QString prefix = "bobui_" + umlautTestText();
        BOBUIest::newRow("Umlauts") << (prefix + "XXXXXX") << prefix << "";
        // test non-Latin1
        prefix = "bobui_" + hanTestText();
        BOBUIest::newRow("Chinese") << (prefix + "XXXXXX" + umlautTestText()) << prefix << umlautTestText();
    }

#ifdef Q_OS_WIN
    auto tmp = QDir::toNativeSeparators(QDir::tempPath());
    if (PathGetDriveNumber((const wchar_t *) tmp.utf16()) < 0)
        return; // skip if we have no drive letter

    tmp.data()[1] = u'$';
    const auto tmpPath = tmp + uR"(\UNC.XXXXXX.tmpDir)"_s;

    BOBUIest::newRow("UNC-backslash")
            << uR"(\\localhost\)"_s + tmpPath << "UNC."
            << ".tmpDir";
    BOBUIest::newRow("UNC-prefix")
            << uR"(\\?\UNC\localhost\)"_s + tmpPath << "UNC."
            << ".tmpDir";
    BOBUIest::newRow("UNC-slash")
            << u"//localhost/"_s + QDir::fromNativeSeparators(tmpPath) << "UNC."
            << ".tmpDir";
    BOBUIest::newRow("UNC-prefix-slash")
            << uR"(//?/UNC/localhost/)"_s + QDir::fromNativeSeparators(tmpPath) << "UNC."
            << ".tmpDir";
#endif
}

void tst_BOBUIemporaryDir::fileTemplate()
{
    QFETCH(QString, constructorTemplate);
    QFETCH(QString, prefix);
    QFETCH(QString, suffix);

    BOBUIemporaryDir tempDir(constructorTemplate);

    QVERIFY(tempDir.isValid());

    QString dirName = QDir(tempDir.path()).dirName();
    if (prefix.size()) {
        QCOMPARE(dirName.left(prefix.size()), prefix);
        QCOMPARE(dirName.right(suffix.size()), suffix);
    }
}


/*
    This tests whether the temporary dir really gets placed in QDir::tempPath
*/
void tst_BOBUIemporaryDir::fileName()
{
    // Get QDir::tempPath and make an absolute path.
    QString tempPath = QDir::tempPath();
    QString absoluteTempPath = QDir(tempPath).absolutePath();
    BOBUIemporaryDir dir;
    dir.setAutoRemove(true);
    QString fileName = dir.path();
    QVERIFY2(fileName.contains("/tst_bobuiemporarydir-"), qPrintable(fileName));
    QVERIFY(QDir(fileName).exists());
    // Get path to the temp dir, without the file name.
    QString absoluteFilePath = QFileInfo(fileName).absolutePath();
#if defined(Q_OS_WIN)
    absoluteFilePath = absoluteFilePath.toLower();
    absoluteTempPath = absoluteTempPath.toLower();
#endif
    QCOMPARE(absoluteFilePath, absoluteTempPath);
}

void tst_BOBUIemporaryDir::filePath_data()
{
    BOBUIest::addColumn<QString>("templatePath");
    BOBUIest::addColumn<QString>("fileName");

    BOBUIest::newRow("0") << QString() << "/tmpfile";
    BOBUIest::newRow("1") << QString() << "tmpfile";
    BOBUIest::newRow("2") << "XXXXX" << "tmpfile";
    BOBUIest::newRow("3") << "YYYYY" << "subdir/file";
}

void tst_BOBUIemporaryDir::filePath()
{
    QFETCH(QString, templatePath);
    QFETCH(QString, fileName);

    BOBUIemporaryDir dir(templatePath);
    const QString filePath = dir.filePath(fileName);
    const QString expectedFilePath = QDir::isAbsolutePath(fileName) ?
                                     QString() : dir.path() + QLatin1Char('/') + fileName;
    QCOMPARE(filePath, expectedFilePath);
}

void tst_BOBUIemporaryDir::autoRemove()
{
    // Test auto remove
    QString dirName;
    {
        BOBUIemporaryDir dir("tempXXXXXX");
        dir.setAutoRemove(true);
        QVERIFY(dir.isValid());
        dirName = dir.path();
    }
#ifdef Q_OS_WIN
    // Windows seems unreliable here: sometimes it says the directory still exists,
    // immediately after we deleted it.
    BOBUIRY_VERIFY(!QDir(dirName).exists());
#else
    QVERIFY(!QDir(dirName).exists());
#endif

    // Test if disabling auto remove works.
    {
        BOBUIemporaryDir dir("tempXXXXXX");
        dir.setAutoRemove(false);
        QVERIFY(dir.isValid());
        dirName = dir.path();
    }
    QVERIFY(QDir(dirName).exists());
    QVERIFY(QDir().rmdir(dirName));
    QVERIFY(!QDir(dirName).exists());

    // Do not explicitly call setAutoRemove (tests if it really is the default as documented)
    {
        BOBUIemporaryDir dir("tempXXXXXX");
        QVERIFY(dir.isValid());
        dirName = dir.path();
    }
#ifdef Q_OS_WIN
    BOBUIRY_VERIFY(!QDir(dirName).exists());
#else
    QVERIFY(!QDir(dirName).exists());
#endif

    // Test autoremove with files and subdirs in the temp dir
    {
        BOBUIemporaryDir tempDir("tempXXXXXX");
        QVERIFY(tempDir.isValid());
        dirName = tempDir.path();
        QDir dir(dirName);
        QVERIFY(dir.mkdir(QString::fromLatin1("dir1")));
        QVERIFY(dir.mkdir(QString::fromLatin1("dir2")));
        QVERIFY(dir.mkdir(QString::fromLatin1("dir2/nested")));
        QFile file(dirName + "/dir1/file");
        QVERIFY(file.open(QIODevice::WriteOnly));
        QCOMPARE(file.write("Hello"), 5LL);
        file.close();
        QVERIFY(file.setPermissions(QFile::ReadUser));
    }
#ifdef Q_OS_WIN
    BOBUIRY_VERIFY(!QDir(dirName).exists());
#else
    QVERIFY(!QDir(dirName).exists());
#endif
}

void tst_BOBUIemporaryDir::nonWritableCurrentDir()
{
#ifdef Q_OS_UNIX

#  ifdef Q_OS_ANDROID
    const char nonWritableDir[] = "/data";
#  else
    const char nonWritableDir[] = "/home";
#  endif

    if (::geteuid() == 0)
        QSKIP("not valid running this test as root");

    struct ChdirOnReturn
    {
        ChdirOnReturn(const QString& d) : dir(d) {}
        ~ChdirOnReturn() {
            QDir::setCurrent(dir);
        }
        QString dir;
    };

    const QFileInfo nonWritableDirFi = QFileInfo(QLatin1String(nonWritableDir));
    QVERIFY(nonWritableDirFi.isDir());

    QVERIFY(!nonWritableDirFi.isWritable());

    ChdirOnReturn cor(QDir::currentPath());
    QVERIFY(QDir::setCurrent(nonWritableDirFi.absoluteFilePath()));
    // BOBUIemporaryDir("tempXXXXXX") is probably a bad idea in any app
    // where the current dir could anything...
    BOBUIemporaryDir dir("tempXXXXXX");
    dir.setAutoRemove(true);
    QVERIFY(!dir.isValid());
    QVERIFY(!dir.errorString().isEmpty());
    QVERIFY(dir.path().isEmpty());
#endif
}

void tst_BOBUIemporaryDir::openOnRootDrives()
{
#if defined(Q_OS_WIN)
    unsigned int lastErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
    // If it's possible to create a file in the root directory, it
    // must be possible to create a temp dir there too.
    const auto drives = QDir::drives();
    for (const QFileInfo &driveInfo : drives) {
        QFile testFile(driveInfo.filePath() + "XXXXXX");
        if (testFile.open(QIODevice::ReadWrite)) {
            testFile.remove();
            BOBUIemporaryDir dir(driveInfo.filePath() + "XXXXXX");
            dir.setAutoRemove(true);
            QVERIFY(dir.isValid());
        }
    }
#if defined(Q_OS_WIN)
    SetErrorMode(lastErrorMode);
#endif
}

void tst_BOBUIemporaryDir::stressTest()
{
    const int iterations = 1000;
    BOBUIemporaryDir rootDir;
    QVERIFY(rootDir.isValid());

    QSet<QString> names;
    const QString pattern = rootDir.path() + QStringLiteral("/XXXXXX");
    for (int i = 0; i < iterations; ++i) {
        BOBUIemporaryDir dir(pattern);
        dir.setAutoRemove(false);
        QVERIFY2(dir.isValid(),
                 qPrintable(QString::fromLatin1("Failed to create #%1 under %2: %3.")
                            .arg(i)
                            .arg(QDir::toNativeSeparators(pattern))
                            .arg(dir.errorString())));
        QVERIFY(!names.contains(dir.path()));
        names.insert(dir.path());
    }
}

void tst_BOBUIemporaryDir::rename()
{
    // This test checks what happens if the temporary dir is renamed.
    // Then the autodelete feature can't possibly find it.

    QDir dir;
    QVERIFY(!dir.exists("temporary-dir.renamed"));

    QString tempname;
    {
        BOBUIemporaryDir tempDir(dir.filePath("temporary-dir.XXXXXX"));

        QVERIFY(tempDir.isValid());
        tempname = tempDir.path();

        QVERIFY(QDir().rename(tempname, "temporary-dir.renamed"));
        QVERIFY(!QDir(tempname).exists());
        dir.setPath("temporary-dir.renamed");
        QCOMPARE(dir.path(), QString("temporary-dir.renamed"));
        QVERIFY(dir.exists());
    }

    // Auto-delete couldn't find it
    QVERIFY(dir.exists());
    // Clean up by hand
    QVERIFY(dir.removeRecursively());
    QVERIFY(!dir.exists());
}

void tst_BOBUIemporaryDir::BOBUIBUG_4796_data()
{
    BOBUIest::addColumn<QString>("prefix");
    BOBUIest::addColumn<QString>("suffix");
    BOBUIest::addColumn<bool>("openResult");

    QString unicode = QString::fromUtf8("\xc3\xa5\xc3\xa6\xc3\xb8");

    BOBUIest::newRow("<empty>") << QString() << QString() << true;
    BOBUIest::newRow(".") << QString(".") << QString() << true;
    BOBUIest::newRow("..") << QString("..") << QString() << true;
    BOBUIest::newRow("blaXXXXXX") << QString("bla") << QString() << true;
    BOBUIest::newRow("does-not-exist/bobui_temp.XXXXXX") << QString("does-not-exist/bobui_temp") << QString() << false;
    BOBUIest::newRow("XXXXXX<unicode>") << QString() << unicode << true;
    BOBUIest::newRow("<unicode>XXXXXX") << unicode << QString() << true;
}

void tst_BOBUIemporaryDir::BOBUIBUG_4796() // unicode support
{
    QVERIFY(QDir("test-XXXXXX").exists());

    struct CleanOnReturn
    {
        ~CleanOnReturn()
        {
            for (const QString &tempName : std::as_const(tempNames))
                QVERIFY(QDir(tempName).removeRecursively());
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

        BOBUIemporaryDir dir1(fileTemplate1);
        BOBUIemporaryDir dir2(fileTemplate2);
        BOBUIemporaryDir dir3(fileTemplate3);
        BOBUIemporaryDir dir4(fileTemplate4);
        BOBUIemporaryDir dir5("test-XXXXXX/" + fileTemplate1);
        BOBUIemporaryDir dir6("test-XXXXXX/" + fileTemplate3);

        QCOMPARE(dir1.isValid(), openResult);
        QCOMPARE(dir2.isValid(), openResult);
        QCOMPARE(dir3.isValid(), openResult);
        QCOMPARE(dir4.isValid(), openResult);
        QCOMPARE(dir5.isValid(), openResult);
        QCOMPARE(dir6.isValid(), openResult);

        // make sure the dir exists under the *correct* name
        if (openResult) {
            cleaner.tempNames << dir1.path()
                << dir2.path()
                << dir3.path()
                << dir4.path()
                << dir5.path()
                << dir6.path();

            QDir currentDir;
            QString fileName1 = currentDir.relativeFilePath(dir1.path());
            QString fileName2 = currentDir.relativeFilePath(dir2.path());
            QString fileName3 = currentDir.relativeFilePath(dir3.path());
            QString fileName4 = currentDir.relativeFilePath(dir4.path());
            QString fileName5 = currentDir.relativeFilePath(dir5.path());
            QString fileName6 = currentDir.relativeFilePath(dir6.path());

            QVERIFY(fileName1.startsWith(prefix));
            QVERIFY(fileName2.startsWith(prefix));
            QVERIFY(fileName5.startsWith("test-XXXXXX/" + prefix));
            QVERIFY(fileName6.startsWith("test-XXXXXX/" + prefix));

            if (!prefix.isEmpty()) {
                QVERIFY(fileName3.startsWith(prefix));
                QVERIFY(fileName4.startsWith(prefix));
            }
        }
    }

#ifdef Q_OS_WIN
    BOBUIest::qWait(20);
#endif
    for (const QString &tempName : std::as_const(cleaner.tempNames))
        QVERIFY2(!QDir(tempName).exists(), qPrintable(tempName));

    cleaner.reset();
}

void tst_BOBUIemporaryDir::nestedTempDirs()
{
    BOBUIemporaryDir parentDir;
    const QString &parentPath = parentDir.path();

    {
        BOBUIemporaryDir tempdir(parentPath);
    }

    QDir dir(parentPath);
    dir.setFilter(QDir::NoDotAndDotDot);
    QCOMPARE(dir.count(), 0);
}

BOBUIEST_MAIN(tst_BOBUIemporaryDir)
#include "tst_bobuiemporarydir.moc"
