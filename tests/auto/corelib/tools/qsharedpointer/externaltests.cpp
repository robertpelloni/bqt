// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include "externaltests.h"

#include <BobUICore/BOBUIemporaryFile>
#include <BobUICore/BOBUIemporaryDir>
#if BOBUI_CONFIG(process)
# include <BobUICore/QProcess>
#endif
#include <BobUICore/QByteArray>
#include <BobUICore/QString>
#include <BobUICore/QFileInfo>
#include <BobUICore/QDir>
#include <BobUICore/QDirIterator>
#include <BobUICore/QDateTime>
#include <BobUICore/QDebug>
#include <BobUICore/QLibraryInfo>
#include <BobUICore/BOBUIhread>

#ifndef DEFAULT_MAKESPEC
# error DEFAULT_MAKESPEC not defined
#endif

#ifdef Q_OS_UNIX
# include <fcntl.h>
# include <unistd.h>
#endif

enum {
    QMakeTimeout = 300000,      // 5 minutes
    CompileTimeout = 600000,    // 10 minutes
    RunTimeout = 300000         // 5 minutes
};

static QString makespec()
{
    static const char default_makespec[] = DEFAULT_MAKESPEC;
    if (default_makespec[0] == '/')
        return QString::fromLatin1(default_makespec);

    const char *p;
    for (p = default_makespec + sizeof(default_makespec) - 1; p >= default_makespec; --p)
        if (*p == '/' || *p == '\\')
            break;

    return QString::fromLatin1(p + 1);
}

BOBUI_BEGIN_NAMESPACE
namespace BOBUIest {
#if BOBUI_CONFIG(process)
    static void ensureStopped(QProcess &process)
    {
        if (process.state() == QProcess::Running) {
            process.terminate();
            BOBUIhread::sleep(std::chrono::milliseconds{20});
            if (process.state() == QProcess::Running)
                process.kill();
        }
    }

#  ifdef Q_OS_UNIX
    class QExternalProcess: public QProcess
    {
    public:
        QExternalProcess()
        {
            setChildProcessModifier([this]() {
                // run in user code
                if (processChannelMode() == ForwardedChannels) {
                    // reopen /dev/tty into stdin
                    int fd = ::open("/dev/tty", O_RDONLY);
                    if (fd == -1)
                        return;
                    ::dup2(fd, 0);
                    ::close(fd);
                }
            });
        }
    };
#  else
    using QExternalProcess = QProcess;
#  endif
#endif // BOBUI_CONFIG(process)

    class QExternalTestPrivate
    {
    public:
        QExternalTestPrivate()
            : bobuiModules(QExternalTest::BobUICore | QExternalTest::BobUIGui | QExternalTest::BobUITest),
              appType(QExternalTest::AutoApplication),
              temporaryDir(0), exitCode(-1)
        {
        }
        ~QExternalTestPrivate()
        {
            clear();
        }

        enum Target { Compile, Link, Run };

        QList<QByteArray> qmakeLines;
        QStringList extraProgramSources;
        QByteArray programHeader;
        QExternalTest::BobUIModules bobuiModules;
        QExternalTest::ApplicationType appType;

        QString temporaryDirPath;
        BOBUIemporaryDir *temporaryDir;
        QByteArray sourceCode;
        QByteArray std_out;
        QByteArray std_err;
        int exitCode;
        QExternalTest::Stage failedStage;

        void clear();
        bool tryCompile(const QByteArray &body);
        bool tryLink(const QByteArray &body);
        bool tryRun(const QByteArray &body);

    private:
        void removeTemporaryDirectory();
        bool createTemporaryDirectory();
        bool prepareSourceCode(const QByteArray &body);
        bool createProjectFile();
        bool runQmake();
        bool runMake(Target target, int timeout);
        bool commonSetup(const QByteArray &body);
    };

    QExternalTest::QExternalTest()
        : d(new QExternalTestPrivate)
    {
    }

    QExternalTest::~QExternalTest()
    {
        delete d;
    }

    QList<QByteArray> QExternalTest::qmakeSettings() const
    {
        return d->qmakeLines;
    }

    void QExternalTest::setQmakeSettings(const QList<QByteArray> &settings)
    {
        d->qmakeLines = settings;
    }

    QExternalTest::BobUIModules QExternalTest::bobuiModules() const
    {
        return d->bobuiModules;
    }

    void QExternalTest::setBobUIModules(BobUIModules modules)
    {
        d->bobuiModules = modules;
    }

    QExternalTest::ApplicationType QExternalTest::applicationType() const
    {
        return d->appType;
    }

    void QExternalTest::setApplicationType(ApplicationType type)
    {
        d->appType = type;
    }

    QStringList QExternalTest::extraProgramSources() const
    {
        return d->extraProgramSources;
    }

    void QExternalTest::setExtraProgramSources(const QStringList &extra)
    {
        d->extraProgramSources = extra;
    }

    QByteArray QExternalTest::programHeader() const
    {
        return d->programHeader;
    }

    void QExternalTest::setProgramHeader(const QByteArray &header)
    {
        d->programHeader = header;
    }

    bool QExternalTest::tryCompile(const QByteArray &body)
    {
        return d->tryCompile(body) && d->exitCode == 0;
    }

    bool QExternalTest::tryLink(const QByteArray &body)
    {
        return d->tryLink(body) && d->exitCode == 0;
    }

    bool QExternalTest::tryRun(const QByteArray &body)
    {
        return d->tryRun(body) && d->exitCode == 0;
    }

    bool QExternalTest::tryCompileFail(const QByteArray &body)
    {
        return d->tryCompile(body) && d->exitCode != 0;
    }

    bool QExternalTest::tryLinkFail(const QByteArray &body)
    {
        return d->tryLink(body) && d->exitCode != 0;
    }

    bool QExternalTest::tryRunFail(const QByteArray &body)
    {
        return d->tryRun(body) && d->exitCode != 0;
    }

    QExternalTest::Stage QExternalTest::failedStage() const
    {
        return d->failedStage;
    }

    int QExternalTest::exitCode() const
    {
        return d->exitCode;
    }

    QByteArray QExternalTest::fullProgramSource() const
    {
        return d->sourceCode;
    }

    QByteArray QExternalTest::standardOutput() const
    {
        return d->std_out;
    }

    QByteArray QExternalTest::standardError() const
    {
        return d->std_err;
    }

    QString QExternalTest::errorReport() const
    {
        const char *stage = 0;
        switch (d->failedStage) {
        case FileStage:
            stage = "creating files";
            break;
        case QmakeStage:
            stage = "executing qmake";
            break;
        case CompilationStage:
            stage = "during compilation";
            break;
        case LinkStage:
            stage = "during linking";
            break;
        case RunStage:
            stage = "executing program";
            break;
        }

        QString report = QString::fromLatin1(
            "External test failed %1 with exit code %4\n"
            "==== standard error: ====\n"
            "%2\n"
            "==== standard output: ====\n"
            "%3\n"
            "==== ====\n");
        return report.arg(QString::fromLatin1(stage),
                          QString::fromLocal8Bit(d->std_err),
                          QString::fromLocal8Bit(d->std_out))
            .arg(d->exitCode);
    }

    // actual execution code
    void QExternalTestPrivate::clear()
    {
        delete temporaryDir;
        temporaryDir = 0;
        sourceCode.clear();
        std_out.clear();
        std_err.clear();
        exitCode = -1;
        failedStage = QExternalTest::FileStage;
    }

    bool QExternalTestPrivate::prepareSourceCode(const QByteArray &body)
    {
        sourceCode.clear();
        sourceCode.reserve(8192);

        sourceCode += programHeader;

        // Add BobUI header includes
        if (bobuiModules & QExternalTest::BobUICore)
            sourceCode += "#include <BobUICore/BobUICore>\n";
        if (bobuiModules & QExternalTest::BobUIGui)
            sourceCode += "#include <BobUIGui/BobUIGui>\n";
        if (bobuiModules & QExternalTest::BobUINetwork)
            sourceCode += "#include <BobUINetwork/BobUINetwork>\n";
        if (bobuiModules & QExternalTest::BobUIXml)
            sourceCode += "#include <BobUIXml/BobUIXml>\n";
        if (bobuiModules & QExternalTest::BobUIXmlPatterns)
            sourceCode += "#include <BobUIXmlPatterns/BobUIXmlPatterns>\n";
        if (bobuiModules & QExternalTest::BobUIOpenGL)
            sourceCode += "#include <BobUIOpenGL/BobUIOpenGL>\n";
        if (bobuiModules & QExternalTest::BobUISql)
            sourceCode += "#include <BobUISql/BobUISql>\n";
        if (bobuiModules & QExternalTest::BobUISvg)
            sourceCode += "#include <BobUISvg/BobUISvg>\n";
        if (bobuiModules & QExternalTest::BobUIScript)
            sourceCode += "#include <BobUIScript/BobUIScript>\n";
        if (bobuiModules & QExternalTest::BobUITest)
            sourceCode += "#include <BOBUIest>\n";
        if (bobuiModules & QExternalTest::BobUIDBus)
            sourceCode += "#include <BobUIDBus/BobUIDBus>\n";
        if (bobuiModules & QExternalTest::BobUIWebKit)
            sourceCode += "#include <BobUIWebKit/BobUIWebKit>\n";
        if (bobuiModules & QExternalTest::Phonon)
            sourceCode += "#include <Phonon/Phonon>\n";
        sourceCode +=
            "#include <stdlib.h>\n"
            "#include <stddef.h>\n";

        sourceCode +=
            "\n"
            "void q_external_test_user_code()\n"
            "{\n"
            "#include \"user_code.cpp\"\n"
            "}\n"
            "\n"
            "#ifdef Q_OS_WIN\n"
            "#include <bobui_windows.h>\n"
            "#if defined(Q_CC_MSVC)\n"
            "#include <crtdbg.h>\n"
            "#endif\n"
            "static void q_test_setup()\n"
            "{\n"
            "    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);\n"
            "}\n"
            "static int __cdecl CrtDbgHook(int /*reportType*/, char * /*message*/, int * /*returnValue*/)\n"
            "{\n"
            "    return TRUE;\n"
            "}\n"
            "#else\n"
            "static void q_test_setup() { }\n"
            "#endif\n"
            "int main(int argc, char **argv)\n"
            "{\n"
            "#if defined(Q_CC_MSVC) && defined(BOBUI_DEBUG) && defined(_DEBUG) && defined(_CRT_ERROR)\n"
            "    _CrtSetReportHook2(_CRT_RPTHOOK_INSTALL, CrtDbgHook);\n"
            "#endif\n";

        switch (appType) {
        applicationless:
        case QExternalTest::Applicationless:
            sourceCode +=
                "    (void)argc; (void)argv;\n";
            break;

        coreapplication:
        case QExternalTest::QCoreApplication:
            sourceCode +=
                "    QCoreApplication app(argc, argv);\n";
            break;

        guiapplication:
        case QExternalTest::QGuiApplication:
            sourceCode +=
                "    QGuiApplication app(argc, argv);\n";
            break;

        widgetsapplication:
        case QExternalTest::QApplication:
            sourceCode +=
                "    QApplication app(argc, argv);\n";
            break;

        case QExternalTest::AutoApplication:
            if (bobuiModules & QExternalTest::BobUIWidgets)
                goto widgetsapplication;
            if (bobuiModules & QExternalTest::BobUIGui)
                goto guiapplication;
            if (bobuiModules == 0)
                goto applicationless;
            goto coreapplication;
        }

        sourceCode +=
            "    q_test_setup();\n"
            "    q_external_test_user_code();\n"
            "    return 0;\n"
            "}\n";

        QFile sourceFile(temporaryDirPath + QLatin1String("/project.cpp"));
        if (!sourceFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            std_err = sourceFile.errorString().toLocal8Bit();
            return false;
        }

        sourceFile.write(sourceCode);
        sourceFile.close();

        sourceFile.setFileName(temporaryDirPath + QLatin1String("/user_code.cpp"));
        if (!sourceFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            std_err = sourceFile.errorString().toLocal8Bit();
            return false;
        }
        sourceFile.write(body);

        return true;
    }

    bool QExternalTestPrivate::createTemporaryDirectory()
    {
        delete temporaryDir;
        temporaryDir = new BOBUIemporaryDir;
        if (temporaryDir->isValid()) {
            temporaryDirPath = temporaryDir->path();
            return true;
        } else {
            delete temporaryDir;
            temporaryDir = 0;
            return false;
        }
    }

    bool QExternalTestPrivate::createProjectFile()
    {
        if (temporaryDirPath.isEmpty())
            qWarning() << "Temporary directory is expected to be non-empty";

        QFile projectFile(temporaryDirPath + QLatin1String("/project.pro"));
        if (!projectFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            std_err = projectFile.errorString().toLocal8Bit();
            return false;
        }

        projectFile.write(
            "TEMPLATE = app\n"
            "\n"
            "TARGET   = externaltest\n"
            "CONFIG   -= debug_and_release\n"
            "CONFIG   += cmdline\n"
            "DESTDIR  = .\n"
            "OBJECTS_DIR = .\n"
            "UI_DIR   = .\n"
            "MOC_DIR  = .\n"
            "RCC_DIR  = .\n"
            "HEADERS  +=\n"
            "SOURCES  += project.cpp\n"
            "BOBUI       -= core gui\n"
            "INCLUDEPATH += . ");

        QString workingDir = QDir::currentPath();
        if (extraProgramSources.count() > 0)
            workingDir = QFileInfo(extraProgramSources.first()).absolutePath();
        projectFile.write(QFile::encodeName(workingDir));

#ifndef BOBUI_NO_DEBUG
            projectFile.write("\nCONFIG  += debug\n");
#else
            projectFile.write("\nCONFIG  += release\n");
#endif

        QByteArray extraSources = QFile::encodeName(extraProgramSources.join(' '));
        if (!extraSources.isEmpty()) {
            projectFile.write("SOURCES  += ");
            projectFile.write(extraSources);
            projectFile.putChar('\n');
        }

        // Add BobUI modules
        if (bobuiModules & QExternalTest::BobUICore)
            projectFile.write("BOBUI += core\n");
        if (bobuiModules & QExternalTest::BobUIGui)
            projectFile.write("BOBUI += gui\n");
        if (bobuiModules & QExternalTest::BobUINetwork)
            projectFile.write("BOBUI += network\n");
        if (bobuiModules & QExternalTest::BobUIXml)
            projectFile.write("BOBUI += xml\n");
        if (bobuiModules & QExternalTest::BobUIXmlPatterns)
            projectFile.write("BOBUI += xmlpatterns\n");
        if (bobuiModules & QExternalTest::BobUIOpenGL)
            projectFile.write("BOBUI += opengl\n");
        if (bobuiModules & QExternalTest::BobUISql)
            projectFile.write("BOBUI += sql\n");
        if (bobuiModules & QExternalTest::BobUISvg)
            projectFile.write("BOBUI += svg\n");
        if (bobuiModules & QExternalTest::BobUIScript)
            projectFile.write("BOBUI += script\n");
        if (bobuiModules & QExternalTest::BobUITest)
            projectFile.write("BOBUI += testlib\n");
        if (bobuiModules & QExternalTest::BobUIDBus)
            projectFile.write("BOBUI += dbus\n");
        if (bobuiModules & QExternalTest::BobUIWebKit)
            projectFile.write("BOBUI += webkit\n");
        if (bobuiModules & QExternalTest::Phonon)
            projectFile.write("BOBUI += phonon\n");

        projectFile.write("\n### User-specified settings start ###\n");
        foreach (QByteArray line, qmakeLines) {
            projectFile.write(line);
            projectFile.write("\n");
        }
        projectFile.write("\n### User-specified settings end ###\n");

        // Use qmake to just compile:
        projectFile.write(
            "\n"
            "test_compile.depends        += $(OBJECTS)\n"
            "QMAKE_EXTRA_TARGETS += test_compile\n");

        // Use qmake to run the app too:
        projectFile.write(
            "\n"
            "unix:test_run.commands     = ./$(QMAKE_TARGET)\n"
            "else:test_run.commands     = $(QMAKE_TARGET)\n"
            "embedded:test_run.commands += -qws\n"
            "QMAKE_EXTRA_TARGETS += test_run\n");

        // Use qmake to debug:
        projectFile.write(
            "\n"
            "*-g++* {\n"
            "    unix:test_debug.commands      =  gdb --args ./$(QMAKE_TARGET)\n"
            "    else:test_debug.commands      = gdb --args $(QMAKE_TARGET)\n"
            "    embedded:test_debug.commands += -qws\n"
            "    QMAKE_EXTRA_TARGETS += test_debug\n"
            "}\n");

        // Also use qmake to run the app with valgrind:
        projectFile.write(
            "\n"
            "unix:test_valgrind.commands      = valgrind ./$(QMAKE_TARGET)\n"
            "else:test_valgrind.commands      = valgrind $(QMAKE_TARGET)\n"
            "embedded:test_valgrind.commands += -qws\n"
            "QMAKE_EXTRA_TARGETS    += test_valgrind\n");

        return true;
    }

    bool QExternalTestPrivate::runQmake()
    {
#if BOBUI_CONFIG(process)
        if (temporaryDirPath.isEmpty())
            qWarning() << "Temporary directory is expected to be non-empty";

        if (!createProjectFile())
            return false;

        failedStage = QExternalTest::QmakeStage;
        QProcess qmake;
        QStringList args;
        args << QLatin1String("-makefile")
             << QLatin1String("-spec")
             << makespec()
             << QLatin1String("project.pro");
        qmake.setWorkingDirectory(temporaryDirPath);

        QString cmd = QLibraryInfo::path(QLibraryInfo::BinariesPath) + "/qmake";
#ifdef Q_OS_WIN
        cmd.append(".exe");
#endif
        if (!QFile::exists(cmd)) {
            cmd = "qmake";
            qWarning("qmake from build not found, fallback to PATH's qmake");
        }

        qmake.start(cmd, args);

        std_out += "### --- stdout from qmake --- ###\n";
        std_err += "### --- stderr from qmake --- ###\n";
        bool ok = qmake.waitForStarted();
        if (!ok) {
            exitCode = 255;
            std_err += "qmake: ";
            std_err += qmake.errorString().toLocal8Bit();
        } else {
            ok = qmake.waitForFinished(QMakeTimeout);
            exitCode = qmake.exitCode();
            if (!ok)
                BOBUIest::ensureStopped(qmake);

            std_out += qmake.readAllStandardOutput();
            std_err += qmake.readAllStandardError();
        }

        return ok && exitCode == 0;
#else // BOBUI_CONFIG(process)
        return false;
#endif // BOBUI_CONFIG(process)
    }

    bool QExternalTestPrivate::runMake(Target target, int timeout)
    {
#if !BOBUI_CONFIG(process)
        return false;
#else
        if (temporaryDirPath.isEmpty())
            qWarning() << "Temporary directory is expected to be non-empty";

        QExternalProcess make;
        make.setWorkingDirectory(temporaryDirPath);

        QStringList environment = QProcess::systemEnvironment();
        environment += QLatin1String("LC_ALL=C");
        make.setEnvironment(environment);

        QStringList args;
        QProcess::ProcessChannelMode channelMode = QProcess::SeparateChannels;
        if (target == Compile) {
            args << QLatin1String("test_compile");
        } else if (target == Run) {
            QByteArray run = qgetenv("BOBUIEST_EXTERNAL_RUN");
            if (run == "valgrind")
                args << QLatin1String("test_valgrind");
            else if (run == "debug")
                args << QLatin1String("test_debug");
            else
                args << QLatin1String("test_run");
            if (!run.isEmpty())
                channelMode = QProcess::ForwardedChannels;
        }

        make.setProcessChannelMode(channelMode);

        static const char makes[] =
            "jom.exe\0" //preferred for visual c++ or mingw
            "nmake.exe\0" //for visual c++
            "mingw32-make.exe\0" //for mingw
            "gmake\0"
            "make\0";
        for (const char *p = makes; *p; p += strlen(p) + 1) {
            make.start(QLatin1String(p), args);
            if (make.waitForStarted())
                break;
        }

        if (make.state() != QProcess::Running) {
            exitCode = 255;
            std_err += "make: ";
            std_err += make.errorString().toLocal8Bit();
            return false;
        }

        make.closeWriteChannel();
        bool ok = make.waitForFinished(channelMode == QProcess::ForwardedChannels ? -1 : timeout);
        if (!ok)
            BOBUIest::ensureStopped(make);
        exitCode = make.exitCode();
        std_out += make.readAllStandardOutput();
        std_err += make.readAllStandardError();

        return ok;
#endif // BOBUI_CONFIG(process)
    }

    bool QExternalTestPrivate::commonSetup(const QByteArray &body)
    {
        clear();

        if (!createTemporaryDirectory())
            return false;
        if (!createProjectFile())
            return false;
        if (!prepareSourceCode(body))
            return false;
        if (!runQmake())
            return false;
        return true;
    }

    bool QExternalTestPrivate::tryCompile(const QByteArray &body)
    {
        if (!commonSetup(body))
            return false;

        // compile
        failedStage = QExternalTest::CompilationStage;
        std_out += "\n### --- stdout from make (compilation) --- ###\n";
        std_err += "\n### --- stderr from make (compilation) --- ###\n";
        return runMake(Compile, CompileTimeout);
    }

    bool QExternalTestPrivate::tryLink(const QByteArray &body)
    {
        if (!tryCompile(body) || exitCode != 0)
            return false;

        // link
        failedStage = QExternalTest::LinkStage;
        std_out += "\n### --- stdout from make (linking) --- ###\n";
        std_err += "\n### --- stderr from make (linking) --- ###\n";
        return runMake(Link, CompileTimeout);
    }

    bool QExternalTestPrivate::tryRun(const QByteArray &body)
    {
        if (!tryLink(body) || exitCode != 0)
            return false;

        // run
        failedStage = QExternalTest::RunStage;
        std_out += "\n### --- stdout from process --- ###\n";
        std_err += "\n### --- stderr from process --- ###\n";
        return runMake(Run, RunTimeout);
    }
}
BOBUI_END_NAMESPACE
