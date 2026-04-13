// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUICore/private/qfilesystementry_p.h>

using namespace BobUI::StringLiterals;

class tst_QFileSystemEntry : public QObject
{
    Q_OBJECT

private slots:
    void getSetCheck_data();
    void getSetCheck();
    void suffix_data();
    void suffix();
    void completeSuffix_data();
    void completeSuffix();
    void baseName_data();
    void baseName();
    void completeBaseName_data();
    void completeBaseName();
#if defined(Q_OS_WIN)
    void absoluteOrRelative_data();
    void absoluteOrRelative();
#endif
    void isClean_data();
    void isClean();
    void defaultCtor();
};

#if defined(Q_OS_WIN)
void tst_QFileSystemEntry::getSetCheck_data()
{
    BOBUIest::addColumn<QString>("nativeFilePath");
    BOBUIest::addColumn<QString>("internalnativeFilePath");
    BOBUIest::addColumn<QString>("filepath");
    BOBUIest::addColumn<QString>("filename");
    BOBUIest::addColumn<QString>("baseName");
    BOBUIest::addColumn<QString>("completeBasename");
    BOBUIest::addColumn<QString>("suffix");
    BOBUIest::addColumn<QString>("completeSuffix");
    BOBUIest::addColumn<bool>("absolute");
    BOBUIest::addColumn<bool>("relative");

    QString absPrefix = QLatin1String("\\\\?\\");
    QString relPrefix = absPrefix
                + QDir::toNativeSeparators(QDir::currentPath())
                + QLatin1String("\\");

    BOBUIest::newRow("simple")
            << QString("A:\\home\\bobui\\in\\a\\dir.tar.gz")
            << absPrefix +  QString("A:\\home\\bobui\\in\\a\\dir.tar.gz")
            << "A:/home/bobui/in/a/dir.tar.gz"
            << "dir.tar.gz" << "dir" << "dir.tar" << "gz" << "tar.gz" << true << false;

    BOBUIest::newRow("relative")
            << QString("in\\a\\dir.tar.gz")
            << relPrefix +  QString("in\\a\\dir.tar.gz")
            << "in/a/dir.tar.gz"
            << "dir.tar.gz" << "dir" << "dir.tar" << "gz" << "tar.gz" << false <<true;

    BOBUIest::newRow("noSuffix")
            << QString("myDir\\myfile")
            << relPrefix + QString("myDir\\myfile")
            << "myDir/myfile" << "myfile" << "myfile" << "myfile" << "" << "" << false <<true;

    BOBUIest::newRow("noLongSuffix")
            << QString("myDir\\myfile.txt")
            << relPrefix + QString("myDir\\myfile.txt")
            << "myDir/myfile.txt" << "myfile.txt" << "myfile" << "myfile" << "txt" << "txt" << false << true;

    BOBUIest::newRow("endingSlash")
            << QString("myDir\\myfile.bla\\")
            << relPrefix + QString("myDir\\myfile.bla\\")
            << "myDir/myfile.bla/" << "" << "" << "" << "" << "" << false << true;

    BOBUIest::newRow("absolutePath")
            << QString("A:dir\\without\\leading\\backslash.bat")
            << absPrefix + QString("A:\\dir\\without\\leading\\backslash.bat")
            << "A:dir/without/leading/backslash.bat" << "backslash.bat" << "backslash" << "backslash" << "bat" << "bat" << false << false;

    BOBUIest::newRow("longpath")
            << uR"(\\?\D:\)"_s
            << absPrefix + QLatin1String(R"(D:\)")
            << "D:/" << "" << "" << "" << "" << "" << true << false;

    BOBUIest::newRow("uncprefix")
            << uR"(\\?\UNC\localhost\C$\tmp.txt)"_s
            << absPrefix + QLatin1String(R"(UNC\localhost\C$\tmp.txt)")
            << "//localhost/C$/tmp.txt" << "tmp.txt" << "tmp" << "tmp" << "txt" << "txt" << true
            << false;
}

void tst_QFileSystemEntry::getSetCheck()
{
    QFETCH(QString, nativeFilePath);
    QFETCH(QString, internalnativeFilePath);
    QFETCH(QString, filepath);
    QFETCH(QString, filename);
    QFETCH(QString, baseName);
    QFETCH(QString, completeBasename);
    QFETCH(QString, suffix);
    QFETCH(QString, completeSuffix);
    QFETCH(bool, absolute);
    QFETCH(bool, relative);

    QFileSystemEntry entry1(filepath);
    QCOMPARE(entry1.filePath(), filepath);
    QCOMPARE(entry1.nativeFilePath().toLower(), internalnativeFilePath.toLower());
    QCOMPARE(entry1.fileName(), filename);
    QCOMPARE(entry1.suffix(), suffix);
    QCOMPARE(entry1.completeSuffix(), completeSuffix);
    QCOMPARE(entry1.isAbsolute(), absolute);
    QCOMPARE(entry1.isRelative(), relative);
    QCOMPARE(entry1.baseName(), baseName);
    QCOMPARE(entry1.completeBaseName(), completeBasename);

    QFileSystemEntry entry2(nativeFilePath, QFileSystemEntry::FromNativePath());
    QCOMPARE(entry2.suffix(), suffix);
    QCOMPARE(entry2.completeSuffix(), completeSuffix);
    QCOMPARE(entry2.isAbsolute(), absolute);
    QCOMPARE(entry2.isRelative(), relative);
    QCOMPARE(entry2.filePath(), filepath);
    // Since this entry was created using the native path,
    // the object shouldn't change nativeFilePath.
    QCOMPARE(entry2.nativeFilePath(), nativeFilePath);
    QCOMPARE(entry2.fileName(), filename);
    QCOMPARE(entry2.baseName(), baseName);
    QCOMPARE(entry2.completeBaseName(), completeBasename);
}

#else

void tst_QFileSystemEntry::getSetCheck_data()
{
    BOBUIest::addColumn<QByteArray>("nativeFilePath");
    BOBUIest::addColumn<QString>("filepath");
    BOBUIest::addColumn<QString>("filename");
    BOBUIest::addColumn<QString>("basename");
    BOBUIest::addColumn<QString>("completeBasename");
    BOBUIest::addColumn<QString>("suffix");
    BOBUIest::addColumn<QString>("completeSuffix");
    BOBUIest::addColumn<bool>("absolute");

    BOBUIest::newRow("empty")
        << QByteArray()
        << QString()
        << QString() << QString() << QString() << QString() << QString() << false;

    BOBUIest::newRow("simple")
        << QByteArray("/home/bobui/in/a/dir.tar.gz")
        << "/home/bobui/in/a/dir.tar.gz"
        << "dir.tar.gz" << "dir" << "dir.tar" << "gz" << "tar.gz" << true;

    BOBUIest::newRow("relative")
        << QByteArray("in/a/dir.tar.gz")
        << "in/a/dir.tar.gz"
        << "dir.tar.gz" << "dir" << "dir.tar" << "gz" << "tar.gz" << false;

    BOBUIest::newRow("noSuffix")
        << QByteArray("myDir/myfile")
        << "myDir/myfile" << "myfile" << "myfile" << "myfile" << "" << "" << false;

    BOBUIest::newRow("noLongSuffix")
        << QByteArray("myDir/myfile.txt")
        << "myDir/myfile.txt" << "myfile.txt" << "myfile" << "myfile" << "txt" << "txt" << false;

    BOBUIest::newRow("endingSlash")
        << QByteArray("myDir/myfile.bla/")
        << "myDir/myfile.bla/" << "" << "" << "" << "" << "" << false;

    BOBUIest::newRow("relativePath")
        << QByteArray("A:dir/without/leading/backslash.bat")
        << "A:dir/without/leading/backslash.bat" << "backslash.bat" << "backslash" << "backslash" << "bat" << "bat" << false;
}

void tst_QFileSystemEntry::getSetCheck()
{
    QFETCH(QByteArray, nativeFilePath);
    QFETCH(QString, filepath);
    QFETCH(QString, filename);
    QFETCH(QString, basename);
    QFETCH(QString, completeBasename);
    QFETCH(QString, suffix);
    QFETCH(QString, completeSuffix);
    QFETCH(bool, absolute);

    QFileSystemEntry entry1(filepath);
    QCOMPARE(entry1.filePath(), filepath);
    QCOMPARE(entry1.nativeFilePath(), nativeFilePath);
    QCOMPARE(entry1.fileName(), filename);
    QCOMPARE(entry1.suffix(), suffix);
    QCOMPARE(entry1.completeSuffix(), completeSuffix);
    QCOMPARE(entry1.isAbsolute(), absolute);
    QCOMPARE(entry1.isRelative(), !absolute);
    QCOMPARE(entry1.baseName(), basename);
    QCOMPARE(entry1.completeBaseName(), completeBasename);

    QFileSystemEntry entry2(nativeFilePath, QFileSystemEntry::FromNativePath());
    QCOMPARE(entry2.suffix(), suffix);
    QCOMPARE(entry2.completeSuffix(), completeSuffix);
    QCOMPARE(entry2.isAbsolute(), absolute);
    QCOMPARE(entry2.isRelative(), !absolute);
    QCOMPARE(entry2.filePath(), filepath);
    QCOMPARE(entry2.nativeFilePath(), nativeFilePath);
    QCOMPARE(entry2.fileName(), filename);
    QCOMPARE(entry2.baseName(), basename);
    QCOMPARE(entry2.completeBaseName(), completeBasename);
}
#endif

void tst_QFileSystemEntry::suffix_data()
{
    BOBUIest::addColumn<QString>("file");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("empty") << QString() << QString();
    BOBUIest::newRow("noextension0") << "file" << "";
    BOBUIest::newRow("noextension1") << "/path/to/file" << "";
    BOBUIest::newRow("data0") << "file.tar" << "tar";
    BOBUIest::newRow("data1") << "file.tar.gz" << "gz";
    BOBUIest::newRow("data2") << "/path/file/file.tar.gz" << "gz";
    BOBUIest::newRow("data3") << "/path/file.tar" << "tar";
    BOBUIest::newRow("hidden1-1") << ".ext1" << "ext1";
    BOBUIest::newRow("hidden1-2") << ".ext" << "ext";
    BOBUIest::newRow("hidden1-3") << ".ex" << "ex";
    BOBUIest::newRow("hidden1-4") << ".e" << "e";
    BOBUIest::newRow("hidden2-1") << ".ext1.ext2" << "ext2";
    BOBUIest::newRow("hidden2-2") << ".ext.ext2" << "ext2";
    BOBUIest::newRow("hidden2-3") << ".ex.ext2" << "ext2";
    BOBUIest::newRow("hidden2-4") << ".e.ext2" << "ext2";
    BOBUIest::newRow("hidden2-5") << "..ext2" << "ext2";
    BOBUIest::newRow("dots") << "/path/file.with.dots/file..ext2" << "ext2";
    BOBUIest::newRow("dots2") << "/path/file.with.dots/.file..ext2" << "ext2";
}

void tst_QFileSystemEntry::suffix()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileSystemEntry fe(file);
    QCOMPARE(fe.suffix(), expected);

    QFileSystemEntry fi2(file);
    // first resolve the last slash
    (void) fi2.path();
    QCOMPARE(fi2.suffix(), expected);
}

void tst_QFileSystemEntry::completeSuffix_data()
{
    BOBUIest::addColumn<QString>("file");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("empty") << QString() << QString();
    BOBUIest::newRow("noextension0") << "file" << "";
    BOBUIest::newRow("noextension1") << "/path/to/file" << "";
    BOBUIest::newRow("data0") << "file.tar" << "tar";
    BOBUIest::newRow("data1") << "file.tar.gz" << "tar.gz";
    BOBUIest::newRow("data2") << "/path/file/file.tar.gz" << "tar.gz";
    BOBUIest::newRow("data3") << "/path/file.tar" << "tar";
    BOBUIest::newRow("dots") << "/path/file.with.dots/file..ext2" << ".ext2";
    BOBUIest::newRow("dots2") << "/path/file.with.dots/.file..ext2" << "file..ext2";
}

void tst_QFileSystemEntry::completeSuffix()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileSystemEntry fi(file);
    QCOMPARE(fi.completeSuffix(), expected);

    QFileSystemEntry fi2(file);
    // first resolve the last slash
    (void) fi2.path();
    QCOMPARE(fi2.completeSuffix(), expected);
}

void tst_QFileSystemEntry::baseName_data()
{
    BOBUIest::addColumn<QString>("file");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("empty") << QString() << QString();
    BOBUIest::newRow("data0") << "file.tar" << "file";
    BOBUIest::newRow("data1") << "file.tar.gz" << "file";
    BOBUIest::newRow("data2") << "/path/file/file.tar.gz" << "file";
    BOBUIest::newRow("data3") << "/path/file.tar" << "file";
    BOBUIest::newRow("data4") << "/path/file" << "file";
    BOBUIest::newRow("dots") << "/path/file.with.dots/file..ext2" << "file";
    BOBUIest::newRow("dots2") << "/path/file.with.dots/.file..ext2" << "";
}

void tst_QFileSystemEntry::baseName()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileSystemEntry fi(file);
    QCOMPARE(fi.baseName(), expected);

    QFileSystemEntry fi2(file);
    // first resolve the last slash
    (void) fi2.path();
    QCOMPARE(fi2.baseName(), expected);
}

void tst_QFileSystemEntry::completeBaseName_data()
{
    BOBUIest::addColumn<QString>("file");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("empty") << QString() << QString();
    BOBUIest::newRow("data0") << "file.tar" << "file";
    BOBUIest::newRow("data1") << "file.tar.gz" << "file.tar";
    BOBUIest::newRow("data2") << "/path/file/file.tar.gz" << "file.tar";
    BOBUIest::newRow("data3") << "/path/file.tar" << "file";
    BOBUIest::newRow("data4") << "/path/file" << "file";
    BOBUIest::newRow("dots") << "/path/file.with.dots/file..ext2" << "file.";
    BOBUIest::newRow("dots2") << "/path/file.with.dots/.file..ext2" << ".file.";
}

void tst_QFileSystemEntry::completeBaseName()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileSystemEntry fi(file);
    QCOMPARE(fi.completeBaseName(), expected);

    QFileSystemEntry fi2(file);
    // first resolve the last slash
    (void) fi2.path();
    QCOMPARE(fi2.completeBaseName(), expected);
}

#if defined(Q_OS_WIN)
void tst_QFileSystemEntry::absoluteOrRelative_data()
{
    BOBUIest::addColumn<QString>("path");
    BOBUIest::addColumn<bool>("isAbsolute");
    BOBUIest::addColumn<bool>("isRelative");

    BOBUIest::newRow("empty") << QString() << false << true;
    BOBUIest::newRow("data0") << "file.tar" << false << true;
    BOBUIest::newRow("data1") << "/path/file/file.tar.gz" << false << false;
    BOBUIest::newRow("data1") << "C:path/file/file.tar.gz" << false << false;
    BOBUIest::newRow("data3") << "C:/path/file" << true << false;
    BOBUIest::newRow("data3") << "//machine/share" << true << false;
}

void tst_QFileSystemEntry::absoluteOrRelative()
{
    QFETCH(QString, path);
    QFETCH(bool, isAbsolute);
    QFETCH(bool, isRelative);

    QFileSystemEntry fi(path);
    QCOMPARE(fi.isAbsolute(), isAbsolute);
    QCOMPARE(fi.isRelative(), isRelative);
}
#endif

void tst_QFileSystemEntry::isClean_data()
{
    BOBUIest::addColumn<QString>("path");
    BOBUIest::addColumn<bool>("isClean");

    BOBUIest::newRow("empty") << QString() << true;
    BOBUIest::newRow("simple") << "foo" << true;
    BOBUIest::newRow("complex") << "/foo/bar/bz" << true;
    BOBUIest::newRow(".file") << "/foo/.file" << true;
    BOBUIest::newRow("..file") << "/foo/..file" << true;
    BOBUIest::newRow("...") << "/foo/.../bar" << true;
    BOBUIest::newRow("./") << "./" << false;
    BOBUIest::newRow("../") << "../" << false;
    BOBUIest::newRow(".") << "." << false;
    BOBUIest::newRow("..") << ".." << false;
    BOBUIest::newRow("/.") << "/." << false;
    BOBUIest::newRow("/..") << "/.." << false;
    BOBUIest::newRow("/../") << "foo/../bar" << false;
    BOBUIest::newRow("/./") << "foo/./bar" << false;
    BOBUIest::newRow("//") << "foo//bar" << false;
}

void tst_QFileSystemEntry::isClean()
{
    QFETCH(QString, path);
    QFETCH(bool, isClean);

    QFileSystemEntry fi(path);
    QCOMPARE(fi.isClean(), isClean);
}

void tst_QFileSystemEntry::defaultCtor()
{
    QFileSystemEntry entry;

    QVERIFY(entry.filePath().isNull());
    QVERIFY(entry.nativeFilePath().isNull());

    QVERIFY(entry.fileName().isNull());
    QCOMPARE(entry.path(), QString("."));

    QVERIFY(entry.baseName().isNull());
    QVERIFY(entry.completeBaseName().isNull());
    QVERIFY(entry.suffix().isNull());
    QVERIFY(entry.completeSuffix().isNull());

    QVERIFY(!entry.isAbsolute());
    QVERIFY(entry.isRelative());

    QVERIFY(entry.isClean());

#if defined(Q_OS_WIN)
    QVERIFY(!entry.isDriveRoot());
#endif
    QVERIFY(!entry.isRoot());

    QVERIFY(entry.isEmpty());
}

BOBUIEST_MAIN(tst_QFileSystemEntry)
#include <tst_qfilesystementry.moc>
