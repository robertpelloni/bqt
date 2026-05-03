// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qdir.h>
#include <qlibrary.h>
#include <BobUICore/QRegularExpression>
#include <BobUICore/private/qlibrary_p.h>


// Helper macros to let us know if some suffixes and prefixes are valid
#define bundle_VALID    false
#define dylib_VALID     false
#define sl_VALID        false
#define a_VALID         false
#define so_VALID        false
#define dll_VALID       false
#define DLL_VALID       false

#if defined(Q_OS_DARWIN)
# undef bundle_VALID
# undef dylib_VALID
# undef so_VALID
# define bundle_VALID   true
# define dylib_VALID    true
# define so_VALID       true
# define SUFFIX         ".dylib"
# define PREFIX         "lib"

#elif defined(Q_OS_HPUX)
# undef sl_VALID
# define sl_VALID       true
#  ifndef __ia64
#   define SUFFIX         ".sl"
#   define PREFIX         "lib"
#  else
#   undef so_VALID
#   define so_VALID       true
#   define SUFFIX         ".so"
#   define PREFIX         "lib"
#  endif

#elif defined(Q_OS_AIX)
# undef a_VALID
# undef so_VALID
# define a_VALID        true
# define so_VALID       true
# define SUFFIX         ".a"
# define PREFIX         "lib"

#elif defined(Q_OS_WIN)
# undef dll_VALID
# define dll_VALID      true
# undef DLL_VALID
# define DLL_VALID      true
# define SUFFIX         ".dll"
# define PREFIX         ""

#else  // all other Unix
# undef so_VALID
# define so_VALID       true
# define SUFFIX         ".so"
# define PREFIX         "lib"
#endif

using namespace BobUI::StringLiterals;

BOBUI_FORWARD_DECLARE_CLASS(QLibrary)
class tst_QLibrary : public QObject
{
    Q_OBJECT

enum QLibraryOperation {
    Load = 1,
    Unload = 2,
    Resolve = 3,
    OperationMask = 7,
    DontSetFileName = 0x100
};

    QString sys_qualifiedLibraryName(const QString &fileName);

    QString directory;
private slots:
    void initTestCase();
    void cleanup();

    void load_data();
    void load();
    void resolve_data();
    void resolve();
    void unload_data();
    void unload();
    void unload_after_implicit_load();
    void setFilenameAfterFailedLoad();
    void loadAfterFailedLoad();
    void isLibrary_data();
    void isLibrary();
    void version_data();
    void version();
    void loadTwoVersions();
    void archSpecificVersion_data();
    void archSpecificVersion();
    void setFileNameAndVersionTwice();
    void setFileNameAndVersionAfterFailedLoad_data() { version_data(); }
    void setFileNameAndVersionAfterFailedLoad();
    void errorString_data();
    void errorString();
    void loadHints();
    void loadHints_data();
    void fileName_data();
    void fileName();
    void multipleInstancesForOneLibrary();
};

QString tst_QLibrary::sys_qualifiedLibraryName(const QString &fileName)
{
    return directory + QLatin1Char('/') + PREFIX + fileName + SUFFIX;
}

typedef int (*VersionFunction)(void);

void tst_QLibrary::initTestCase()
{
#ifdef Q_OS_ANDROID
    const QStringList paths = QCoreApplication::libraryPaths();
    QVERIFY(!paths.isEmpty());
    directory = paths.first();
#else
    // chdir to our testdata directory, and use relative paths in some tests.
    QString testdatadir = QFileInfo(QFINDTESTDATA("lib")).absolutePath();
    QVERIFY2(QDir::setCurrent(testdatadir), qPrintable("Could not chdir to " + testdatadir));
    directory = QCoreApplication::applicationDirPath();
#endif
}

void tst_QLibrary::cleanup()
{
    // unload the libraries, if they are still loaded after the test ended
    // (probably in a failure)

    static struct {
        QString name;
        int version = -1;
    } libs[] = {
        { directory + "/mylib" },
        { directory + "/mylib", 1 },
        { directory + "/mylib", 2 },
        { sys_qualifiedLibraryName("mylib") },

        // stuff that load_data() succeeds with
        { directory + "/" PREFIX "mylib" },
        { directory + "/" PREFIX "mylib" SUFFIX },
#if defined(Q_OS_WIN32)
        { directory + "/mylib.dl2" },
        { directory + "/system.bobui.test.mylib.dll" },
#elif !defined(Q_OS_ANDROID)
        // .so even on macOS
        { directory + "/libmylib.so2" },
        { directory + "/system.bobui.test.mylib.so" },
#endif

    };
    for (const auto &entry : libs) {
        bool unloaded = false;
        do {
            QLibrary lib(entry.name, entry.version);
            auto libPrivate = QLibraryPrivate::get(&lib);
            unloaded = libPrivate->unload();
        } while (unloaded);
    }
}

void tst_QLibrary::version_data()
{
#ifdef Q_OS_ANDROID
    QSKIP("Versioned .so files are not generated for Android, so this test is not applicable.");
#endif
    BOBUIest::addColumn<QString>("lib");
    BOBUIest::addColumn<int>("loadversion");
    BOBUIest::addColumn<int>("resultversion");

    BOBUIest::newRow( "ok00, version 1" ) << "mylib" << 1 << 1;
    BOBUIest::newRow( "ok00, version 2" ) << "mylib" << 2 << 2;
    BOBUIest::newRow( "ok00, load without version" ) << "mylib" << -1 << 2;
}

void tst_QLibrary::version()
{
    QFETCH( QString, lib );
    QFETCH( int, loadversion );
    QFETCH( int, resultversion );

#if !defined(Q_OS_AIX) && !defined(Q_OS_WIN)
    QString appDir = directory;
    QLibrary library( appDir + QLatin1Char('/') + lib, loadversion );
    QVERIFY2(library.load(), qPrintable(library.errorString()));

    VersionFunction fnVersion = (VersionFunction)library.resolve("mylibversion");
    QVERIFY(fnVersion);
    QCOMPARE(fnVersion(), resultversion);
    QVERIFY2(library.unload(), qPrintable(library.errorString()));
#else
    Q_UNUSED(lib);
    Q_UNUSED(loadversion);
    Q_UNUSED(resultversion);
#endif
}

void tst_QLibrary::loadTwoVersions()
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_WIN)
    QSKIP("Versioned files are not generated for this OS, so this test is not applicable.");
#endif

    QLibrary lib1(directory + "/mylib", 1);
    QLibrary lib2(directory + "/mylib", 2);
    QVERIFY(!lib1.isLoaded());
    QVERIFY(!lib2.isLoaded());

    // load the first one
    QVERIFY(lib1.load());
    QVERIFY(lib1.isLoaded());

    // let's see if we can load the second one too
    QVERIFY(lib2.load());
    QVERIFY(lib2.isLoaded());

    auto p1 = (VersionFunction)lib1.resolve("mylibversion");
    QVERIFY(p1);

    auto p2 = (VersionFunction)lib2.resolve("mylibversion");
    QVERIFY(p2);

    QCOMPARE_NE(p1(), p2());

    lib2.unload();
    lib1.unload();
}

void tst_QLibrary::archSpecificVersion_data()
{
#if !defined(__GLIBC__) || !defined(Q_PROCESSOR_X86_64)
    QSKIP("Test not applicable on this platform");
#endif

    BOBUIest::addColumn<QString>("lib");
    BOBUIest::addColumn<int>("version");

    for (const char *prefix : { "", PREFIX }) {
        QString libname = prefix + u"myarchlib"_s;
        BOBUIest::addRow("%s v1", qPrintable(libname)) << libname << 1;
        BOBUIest::addRow("%s.so.1", qPrintable(libname)) << (libname + SUFFIX + ".1") << -1;
    }
}

void tst_QLibrary::archSpecificVersion()
{
    QFETCH(QString, lib);
    QFETCH(int, version);
    QString expectedArch;

#if defined(__GLIBC__) && defined(Q_PROCESSOR_X86_64)
    if (__builtin_cpu_supports("avx2") && __builtin_cpu_supports("fma"))
        expectedArch = "x86-64-v3";
#endif

    QString appDir = directory;
    QLibrary library(appDir + QLatin1Char('/') + lib, version);
    QVERIFY2(library.load(), qPrintable(library.errorString()));

    auto archfunction = (const char *(*)())library.resolve("archname");
    QVERIFY(archfunction);
    QCOMPARE(archfunction(), expectedArch);
}

void tst_QLibrary::setFileNameAndVersionTwice()
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_WIN)
    QSKIP("Versioned files are not generated for this OS, so this test is not applicable.");
#endif

    QLibrary library(directory + "/mylib", 1);
    QVERIFY(library.load());
    QVERIFY(library.isLoaded());

    auto p1 = (VersionFunction)library.resolve("mylibversion");
    QVERIFY(p1);
    // don't .unload()

    library.setFileNameAndVersion(directory + "/mylib", 2);
    QVERIFY(!library.isLoaded());
    QVERIFY(library.load());
    QVERIFY(library.isLoaded());

    auto p2 = (VersionFunction)library.resolve("mylibversion");
    QVERIFY(p2);
    QCOMPARE_NE(p1(), p2());

    QVERIFY(library.unload());
    QVERIFY(!library.isLoaded());

    // set back
    // it'll look like it isn't loaded, but it is and we can't unload it!
    library.setFileNameAndVersion(directory + "/mylib", 1);
    QVERIFY(!library.isLoaded());
    QVERIFY(!library.unload());
}

void tst_QLibrary::load_data()
{
    BOBUIest::addColumn<QString>("lib");
    BOBUIest::addColumn<bool>("result");

    QString appDir = directory;

    BOBUIest::newRow( "ok00" ) << appDir + "/mylib" << true;
    BOBUIest::newRow( "notexist" ) << appDir + "/nolib" << false;
    BOBUIest::newRow( "badlibrary" ) << appDir + "/qlibrary.pro" << false;

#ifdef Q_OS_DARWIN
    BOBUIest::newRow("ok (libmylib ver. 1)") << appDir + "/libmylib" <<true;
#endif

# if defined(Q_OS_WIN32)
    BOBUIest::newRow( "ok01 (with suffix)" ) << appDir + "/mylib.dll" << true;
    BOBUIest::newRow( "ok02 (with non-standard suffix)" ) << appDir + "/mylib.dl2" << true;
    BOBUIest::newRow( "ok03 (with many dots)" ) << appDir + "/system.bobui.test.mylib.dll" << true;
# elif defined Q_OS_UNIX
    BOBUIest::newRow( "ok01 (with suffix)" ) << appDir + "/libmylib" SUFFIX << true;
#ifndef Q_OS_ANDROID
    // We do not support non-standard suffixes on Android
    BOBUIest::newRow( "ok02 (with non-standard suffix)" ) << appDir + "/libmylib.so2" << true;
#endif
    BOBUIest::newRow( "ok03 (with many dots)" ) << appDir + "/system.bobui.test.mylib.so" << true;
# endif  // Q_OS_UNIX
}

void tst_QLibrary::load()
{
    QFETCH( QString, lib );
    QFETCH( bool, result );

    QLibrary library( lib );
    bool ok = library.load();
    if ( result ) {
        QVERIFY2( ok, qPrintable(library.errorString()) );
        QVERIFY2( library.unload(), qPrintable(library.errorString()) );
    } else {
        QVERIFY( !ok );
    }
}

void tst_QLibrary::unload_data()
{
    BOBUIest::addColumn<QString>("lib");
    BOBUIest::addColumn<bool>("result");

    QString appDir = directory;

    BOBUIest::newRow( "mylib" ) << appDir + "/mylib" << true;
    BOBUIest::newRow( "ok01" ) << appDir + "/nolib" << false;
}

void tst_QLibrary::unload()
{
    QFETCH( QString, lib );
    QFETCH( bool, result );

    QLibrary library( lib );
    library.load();
    bool ok = library.unload();
    if ( result ) {
        QVERIFY2( ok, qPrintable(library.errorString()) );
    } else {
        QVERIFY( !ok );
    }
}

void tst_QLibrary::unload_after_implicit_load()
{
    QLibrary library( directory + "/mylib" );
    QFunctionPointer p = library.resolve("mylibversion");
    QVERIFY(p); // Check if it was loaded
    QVERIFY(library.isLoaded());
    QVERIFY(library.unload());
    QCOMPARE(library.isLoaded(), false);
}

void tst_QLibrary::setFilenameAfterFailedLoad()
{
#if defined(Q_OS_WIN) || defined(Q_OS_ANDROID)
    QSKIP("### FIXME: The helper libraries are currently messed up in the CMakeLists.txt");
#endif

    QLibrary library(directory + "/nolib");
    QVERIFY(!library.load());
    QVERIFY(!library.isLoaded());
    QVERIFY(!library.load());
    QVERIFY(!library.isLoaded());

    library.setFileName(directory + "/mylib");
    QVERIFY(library.load());
    QVERIFY(library.isLoaded());
    auto p = (VersionFunction)library.resolve("mylibversion");
    QVERIFY(p);
    QCOMPARE(p(), 2);
    library.unload();
}

void tst_QLibrary::setFileNameAndVersionAfterFailedLoad()
{
    QLibrary library(directory + "/nolib");
    QVERIFY(!library.load());
    QVERIFY(!library.isLoaded());
    QVERIFY(!library.load());
    QVERIFY(!library.isLoaded());

#if !defined(Q_OS_AIX) && !defined(Q_OS_WIN)
    QFETCH(QString, lib);
    QFETCH(int, loadversion);
    QFETCH(int, resultversion);

    library.setFileNameAndVersion(directory + '/' + lib, loadversion);
    QVERIFY(library.load());
    QVERIFY(library.isLoaded());
    auto p = (VersionFunction)library.resolve("mylibversion");
    QVERIFY(p);
    QCOMPARE(p(), resultversion);
    library.unload();
#endif
}

void tst_QLibrary::loadAfterFailedLoad()
{
#if defined(Q_OS_WIN) || defined(Q_OS_ANDROID)
    QSKIP("### FIXME: The helper libraries are currently messed up in the CMakeLists.txt");
#endif

    BOBUIemporaryDir dir;
    QLibrary library(dir.path() + "/mylib");
    QVERIFY(!library.load());
    QVERIFY(!library.isLoaded());
    QVERIFY(!library.load());
    QVERIFY(!library.isLoaded());

    // now copy the actual lib file into our dir
    QString actualLib = PREFIX "mylib" SUFFIX;
    QVERIFY(QFile::copy(directory + '/' + actualLib, dir.filePath(actualLib)));

    // try again, must succeed now
    QVERIFY(library.load());
    QVERIFY(library.isLoaded());
    auto p = (VersionFunction)library.resolve("mylibversion");
    QVERIFY(p);
    QCOMPARE(p(), 2);
    library.unload();
}

void tst_QLibrary::resolve_data()
{
    BOBUIest::addColumn<QString>("lib");
    BOBUIest::addColumn<QString>("symbol");
    BOBUIest::addColumn<bool>("goodPointer");

    QString appDir = directory;

    BOBUIest::newRow( "ok00" ) << appDir + "/mylib" << QString("mylibversion") << true;
    BOBUIest::newRow( "bad00" ) << appDir + "/mylib" << QString("nosym") << false;
    BOBUIest::newRow( "bad01" ) << appDir + "/nolib" << QString("nosym") << false;
}

void tst_QLibrary::resolve()
{
    typedef int (*testFunc)();
    QFETCH( QString, lib );
    QFETCH( QString, symbol );
    QFETCH( bool, goodPointer );

    QLibrary library(lib);
    QVERIFY(!library.isLoaded());
    testFunc func = (testFunc) library.resolve(symbol.toLatin1());

    if (goodPointer) {
        QVERIFY(library.isLoaded());
        QVERIFY(func);

        QLibrary lib2(lib);
        QVERIFY(!lib2.isLoaded());
        QVERIFY(lib2.load());

        // this unload() won't unload and it must still be loaded
        QVERIFY(!lib2.unload());
        func();                     // doesn't crash
    } else {
        QVERIFY(func == nullptr);
    }
    library.unload();
}

void tst_QLibrary::isLibrary_data()
{
    BOBUIest::addColumn<QString>("filename");
    BOBUIest::addColumn<bool>("valid");

    // use the macros #defined at the top of the file
    BOBUIest::newRow("bad") << QString("mylib.bad") << false;
    BOBUIest::newRow(".a") << QString("mylib.a") << a_VALID;
    BOBUIest::newRow(".bundle") << QString("mylib.bundle") << bundle_VALID;
    BOBUIest::newRow(".dll") << QString("mylib.dll") << dll_VALID;
    BOBUIest::newRow(".DLL") << QString("MYLIB.DLL") << DLL_VALID;
    BOBUIest::newRow(".dl2" ) << QString("mylib.dl2") << false;
    BOBUIest::newRow(".dylib") << QString("mylib.dylib") << dylib_VALID;
    BOBUIest::newRow(".sl") << QString("mylib.sl") << sl_VALID;
    BOBUIest::newRow(".so") << QString("mylib.so") << so_VALID;
    BOBUIest::newRow(".so+version") << QString("mylib.so.0") << so_VALID;
    BOBUIest::newRow("version+.so") << QString("libc-2.7.so") << so_VALID;
    BOBUIest::newRow("version+.so+version") << QString("liboil-0.3.so.0.1.0") << so_VALID;

    // special tests:
#ifdef Q_OS_DARWIN
    BOBUIest::newRow("good (libmylib.1.0.0.dylib)") << QString("libmylib.1.0.0.dylib") << true;
    BOBUIest::newRow("good (libmylib.dylib)") << QString("libmylib.dylib") << true;
    BOBUIest::newRow("good (libmylib.so)") << QString("libmylib.so") << true;
    BOBUIest::newRow("good (libmylib.so.1.0.0)") << QString("libmylib.so.1.0.0") << true;

    BOBUIest::newRow("bad (libmylib.1.0.0.foo)") << QString("libmylib.1.0.0.foo") << false;
#elif defined(Q_OS_WIN)
    BOBUIest::newRow("good (with many dots)" ) << "/system.bobui.test.mylib.dll" << true;
#endif
}

void tst_QLibrary::isLibrary()
{
    QFETCH( QString, filename );
    QFETCH( bool, valid );

    QCOMPARE(QLibrary::isLibrary(filename), valid);
}

void tst_QLibrary::errorString_data()
{
    BOBUIest::addColumn<int>("operation");
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<bool>("success");
    BOBUIest::addColumn<QString>("errorString");

    QString appDir = directory;

    BOBUIest::newRow("bad load()") << (int)Load << QString("nosuchlib") << false << QString("Cannot load library nosuchlib: .*");
    BOBUIest::newRow("call errorString() on QLibrary with no d-pointer (crashtest)") << (int)(Load | DontSetFileName) << QString() << false << QString("Unknown error");
    BOBUIest::newRow("bad resolve") << (int)Resolve << appDir + "/mylib" << false << QString("Unknown error");
    BOBUIest::newRow("good resolve") << (int)Resolve << appDir + "/mylib" << true << QString("Unknown error");

#ifdef Q_OS_WIN
    BOBUIest::newRow("bad load() with .dll suffix") << (int)Load << QString("nosuchlib.dll") << false << QString("Cannot load library nosuchlib.dll: The specified module could not be found.");
//    BOBUIest::newRow("bad unload") << (int)Unload << QString("nosuchlib.dll") << false << QString("QLibrary::unload_sys: Cannot unload nosuchlib.dll (The specified module could not be found.)");
#elif defined Q_OS_DARWIN
#else
    BOBUIest::newRow("load invalid file") << (int)Load << QFINDTESTDATA("library_path/invalid.so") << false << QString("Cannot load library.*");
#endif
}

void tst_QLibrary::errorString()
{
    QFETCH(int, operation);
    QFETCH(QString, fileName);
    QFETCH(bool, success);
    QFETCH(QString, errorString);

    QLibrary lib;
    if (!(operation & DontSetFileName)) {
        lib.setFileName(fileName);
    }

    bool ok = false;
    switch (operation & OperationMask) {
        case Load:
            ok = lib.load();
            break;
        case Unload:
            ok = lib.load();    //###
            ok = lib.unload();
            break;
        case Resolve: {
            ok = lib.load();
            QCOMPARE(ok, true);
            if (success) {
                ok = lib.resolve("mylibversion");
            } else {
                ok = lib.resolve("nosuchsymbol");
            }
            break;}
        default:
            QFAIL(qPrintable(QString("Unknown operation: %1").arg(operation)));
            break;
    }
#if BOBUI_CONFIG(regularexpression)
    QRegularExpression re(QRegularExpression::anchoredPattern(errorString));
    QString libErrorString = lib.errorString();
    QVERIFY2(re.match(libErrorString).hasMatch(), qPrintable(libErrorString));
#endif
    QVERIFY(!lib.isLoaded() || lib.unload());
    QCOMPARE(ok, success);
}

void tst_QLibrary::loadHints_data()
{
    BOBUIest::addColumn<QString>("lib");
    BOBUIest::addColumn<int>("loadHints");
    BOBUIest::addColumn<bool>("result");

    QLibrary::LoadHints lh;

    QString appDir = directory;

    lh |= QLibrary::ResolveAllSymbolsHint;
# if defined(Q_OS_WIN32)
    BOBUIest::newRow( "ok01 (with suffix)" ) << appDir + "/mylib.dll" << int(lh) << true;
    BOBUIest::newRow( "ok02 (with non-standard suffix)" ) << appDir + "/mylib.dl2" << int(lh) << true;
    BOBUIest::newRow( "ok03 (with many dots)" ) << appDir + "/system.bobui.test.mylib.dll" << int(lh) << true;
# elif defined Q_OS_UNIX
    BOBUIest::newRow( "ok01 (with suffix)" ) << appDir + "/libmylib" SUFFIX << int(lh) << true;
#ifndef Q_OS_ANDROID
    // We do not support non-standard suffixes on Android
    BOBUIest::newRow( "ok02 (with non-standard suffix)" ) << appDir + "/libmylib.so2" << int(lh) << true;
#endif
    BOBUIest::newRow( "ok03 (with many dots)" ) << appDir + "/system.bobui.test.mylib.so" << int(lh) << true;
# endif  // Q_OS_UNIX
}

void tst_QLibrary::loadHints()
{
    QFETCH( QString, lib );
    QFETCH( int, loadHints);
    QFETCH( bool, result );
    //QLibrary library( lib );
    QLibrary library;
    QLibrary::LoadHints lh(loadHints);
    if (int(loadHints) != 0) {
        lh |= library.loadHints();
        library.setLoadHints(lh);

        // confirm that another QLibrary doesn't get affected - BOBUIBUG-39642
        QCOMPARE(QLibrary().loadHints(), QLibrary::LoadHints());
    }
    library.setFileName(lib);
    QCOMPARE(library.loadHints(), lh);
    bool ok = library.load();

    // we can't change the hints anymore
    library.setLoadHints(QLibrary::LoadHints());
    QCOMPARE(library.loadHints(), lh);

    // confirm that a new QLibrary inherits the hints too
    QCOMPARE(QLibrary(lib).loadHints(), lh);

    if ( result ) {
        QVERIFY( ok );
        QVERIFY(library.unload());
    } else {
        QVERIFY( !ok );
    }
}

void tst_QLibrary::fileName_data()
{
    BOBUIest::addColumn<QString>("libName");
    BOBUIest::addColumn<QString>("expectedFilename");

    BOBUIest::newRow( "ok02" ) << sys_qualifiedLibraryName(QLatin1String("mylib"))
                            << sys_qualifiedLibraryName(QLatin1String("mylib"));
#if defined(Q_OS_WIN)
    BOBUIest::newRow( "ok03" ) << "user32"
                            << "USER32.dll";
#endif
}

void tst_QLibrary::fileName()
{
    QFETCH( QString, libName);
    QFETCH( QString, expectedFilename);

    QLibrary lib(libName);
    bool ok = lib.load();
    QVERIFY2(ok, qPrintable(lib.errorString()));
#if defined(Q_OS_WIN)
    QCOMPARE(lib.fileName().toLower(), expectedFilename.toLower());
#else
    QCOMPARE(lib.fileName(), expectedFilename);
#endif
    QVERIFY(lib.unload());
}

void tst_QLibrary::multipleInstancesForOneLibrary()
{
    QString lib = directory + "/mylib";

    {
        QLibrary lib1(lib);
        QLibrary lib2(lib);
        QCOMPARE(lib1.isLoaded(), false);
        QCOMPARE(lib2.isLoaded(), false);
        lib1.load();
        QCOMPARE(lib1.isLoaded(), true);
        QCOMPARE(lib2.isLoaded(), false);   // lib2 didn't call load()
        QCOMPARE(lib1.unload(), true);
        QCOMPARE(lib1.isLoaded(), false);
        QCOMPARE(lib2.isLoaded(), false);
        lib1.load();
        lib2.load();
        QCOMPARE(lib1.isLoaded(), true);
        QCOMPARE(lib2.isLoaded(), true);
        QCOMPARE(lib1.unload(), false);
        QCOMPARE(lib1.isLoaded(), false);   // lib1 did call unload()
        QCOMPARE(lib2.isLoaded(), true);
        QCOMPARE(lib2.unload(), true);
        QCOMPARE(lib1.isLoaded(), false);
        QCOMPARE(lib2.isLoaded(), false);

        // Finally; unload on that is already unloaded
        QCOMPARE(lib1.unload(), false);
    }
}

BOBUIEST_MAIN(tst_QLibrary)
#include "tst_qlibrary.moc"
