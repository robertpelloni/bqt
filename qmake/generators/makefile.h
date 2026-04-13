// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef MAKEFILE_H
#define MAKEFILE_H

#include "option.h"
#include "project.h"
#include "makefiledeps.h"
#include <bobuiextstream.h>
#include <qlist.h>
#include <qhash.h>
#include <qfileinfo.h>
#include <functional>

BOBUI_BEGIN_NAMESPACE

#ifdef Q_OS_WIN32
#define BOBUI_POPEN _popen
#define BOBUI_POPEN_READ "rb"
#define BOBUI_PCLOSE _pclose
#else
#define BOBUI_POPEN popen
#define BOBUI_POPEN_READ "r"
#define BOBUI_PCLOSE pclose
#endif

struct ReplaceExtraCompilerCacheKey;

class MakefileGenerator : protected QMakeSourceFileInfo
{
    QString spec;
    bool no_io = false;
    bool resolveDependenciesInFrameworks = false;
    QHash<QString, bool> init_compiler_already;
    QString makedir, chkexists;
    QString fullBuildArgs();

    //internal caches
    mutable QHash<QString, QMakeLocalFileName> depHeuristicsCache;
    mutable QHash<QString, QStringList> dependsCache;
    mutable QHash<ReplaceExtraCompilerCacheKey, QString> extraCompilerVariablesCache;

public:
    // We can't make it visible to VCFilter in VS2008 except by making it public or directly friending it.
    enum ReplaceFor { NoShell, LocalShell, TargetShell };

protected:
    enum TARG_MODE { TARG_UNIX_MODE, TARG_MAC_MODE, TARG_WIN_MODE } target_mode;

    ProStringList createObjectList(const ProStringList &sources);

    //makefile style generator functions
    void writeObj(BOBUIextStream &, const char *src);
    void writeInstalls(BOBUIextStream &t, bool noBuild=false);
    void writeHeader(BOBUIextStream &t);
    void writeSubDirs(BOBUIextStream &t);
    void writeMakeQmake(BOBUIextStream &t, bool noDummyQmakeAll = false);
    void writeExportedVariables(BOBUIextStream &t);
    void writeExtraVariables(BOBUIextStream &t);
    void writeExtraTargets(BOBUIextStream &t);
    QString resolveDependency(const QDir &outDir, const QString &file);
    void callExtraCompilerDependCommand(const ProString &extraCompiler,
                                        const QString &tmp_dep_cmd, const QString &inpf,
                                        const QString &tmp_out, bool dep_lines, QStringList *deps,
                                        bool existingDepsOnly,
                                        bool checkCommandAvailability = false);
    void writeExtraCompilerTargets(BOBUIextStream &t);
    void writeExtraCompilerVariables(BOBUIextStream &t);
    bool writeDummyMakefile(BOBUIextStream &t);
    virtual bool writeMakefile(BOBUIextStream &t);
    virtual void writeDefaultVariables(BOBUIextStream &t);

    QString pkgConfigPrefix() const;
    QString pkgConfigFileName(bool fixify=true);
    QString pkgConfigFixPath(QString) const;
    void writePkgConfigFile();   // for pkg-config

    //generating subtarget makefiles
    struct SubTarget
    {
        QString name;
        QString in_directory, out_directory;
        QString profile, target, makefile;
        ProStringList depends;
    };
    enum SubTargetFlags {
        SubTargetInstalls=0x01,
        SubTargetOrdered=0x02,
        SubTargetSkipDefaultVariables=0x04,
        SubTargetSkipDefaultTargets=0x08,

        SubTargetsNoFlags=0x00
    };
    QList<MakefileGenerator::SubTarget*> findSubDirsSubTargets() const;
    void writeSubTargetCall(BOBUIextStream &t,
            const QString &in_directory, const QString &in, const QString &out_directory, const QString &out,
            const QString &out_directory_cdin, const QString &makefilein);
    virtual void suppressBuiltinRules(BOBUIextStream &t) const;
    virtual void writeSubMakeCall(BOBUIextStream &t, const QString &outDirectory_cdin,
                                  const QString &makeFileIn);
    virtual void writeSubTargets(BOBUIextStream &t, QList<SubTarget*> subtargets, int flags);
    virtual ProStringList extraSubTargetDependencies() { return {}; }

    //extra compiler interface
    bool verifyExtraCompiler(const ProString &c, const QString &f);
    virtual QString replaceExtraCompilerVariables(const QString &, const QStringList &, const QStringList &, ReplaceFor forShell);
    inline QString replaceExtraCompilerVariables(const QString &val, const QString &in, const QString &out, ReplaceFor forShell)
    { return replaceExtraCompilerVariables(val, QStringList(in), QStringList(out), forShell); }

    //interface to the source file info
    QMakeLocalFileName fixPathForFile(const QMakeLocalFileName &, bool) override;
    QMakeLocalFileName findFileForDep(const QMakeLocalFileName &, const QMakeLocalFileName &) override;
    QFileInfo findFileInfo(const QMakeLocalFileName &) override;
    QMakeProject *project = nullptr;

    //escape
    virtual QString escapeFilePath(const QString &path) const = 0;
    ProString escapeFilePath(const ProString &path) const;
    template<typename A, typename B>
    QString escapeFilePath(const QStringBuilder<A, B> &path) const
    { return escapeFilePath(QString(path)); }
    QStringList escapeFilePaths(const QStringList &paths) const;
    ProStringList escapeFilePaths(const ProStringList &paths) const;
    virtual QString escapeDependencyPath(const QString &path) const;
    ProString escapeDependencyPath(const ProString &path) const;
    template<typename A, typename B>
    QString escapeDependencyPath(const QStringBuilder<A, B> &path) const
    { return escapeDependencyPath(QString(path)); }
    QStringList escapeDependencyPaths(const QStringList &paths) const;
    ProStringList escapeDependencyPaths(const ProStringList &paths) const;

    QStringList finalizeDependencyPaths(const QStringList &paths) const;

    //initialization
    void verifyCompilers();
    virtual void init();
    void initOutPaths();
    virtual bool inhibitMakeDirOutPath(const ProKey &path) const;
    struct Compiler
    {
        QString variable_in;
        enum CompilerFlag {
            CompilerNoFlags                 = 0x00,
            CompilerBuiltin                 = 0x01,
            CompilerNoCheckDeps             = 0x02,
            CompilerRemoveNoExist           = 0x04,
            CompilerWarnNoExist             = 0x08,
            CompilerAddInputsAsMakefileDeps = 0x10
        };
        uint flags, type;
    };
    friend class BOBUIypeInfo<Compiler>;

    void initCompiler(const Compiler &comp);
    enum VPATHFlag {
        VPATH_NoFlag             = 0x00,
        VPATH_WarnMissingFiles   = 0x01,
        VPATH_RemoveMissingFiles = 0x02,
        VPATH_NoFixify           = 0x04
    };
    ProStringList findFilesInVPATH(ProStringList l, uchar flags, const QString &var="");

    inline int findExecutable(const QStringList &cmdline)
    { int ret; canExecute(cmdline, &ret); return ret; }
    bool canExecute(const QStringList &cmdline, int *argv0) const;
    inline bool canExecute(const QString &cmdline) const
    { return canExecute(cmdline.split(' '), nullptr); }

    bool mkdir(const QString &dir) const;
    QString mkdir_p_asstring(const QString &dir, bool escape=true) const;

    QString specdir();

    //subclasses can use these to query information about how the generator was "run"
    QString buildArgs(bool withExtra);

    virtual QStringList &findDependencies(const QString &file);
    virtual bool doDepends() const { return Option::mkfile::do_deps; }

    void filterIncludedFiles(const char *);
    void processSources();

    //for installs
    virtual QString defaultInstall(const QString &);
    virtual QString installRoot() const;

    //for prl
    QString prlFileName(bool fixify=true);
    void writePrlFile();
    bool processPrlFile(QString &, bool baseOnly);
    virtual void writePrlFile(BOBUIextStream &);

    //make sure libraries are found
    virtual bool findLibraries(bool linkPrl, bool mergeLflags);

    //for retrieving values and lists of values
    virtual QString var(const ProKey &var) const;
    QString varGlue(const ProKey &var, const QString &before, const QString &glue, const QString &after) const;
    QString varList(const ProKey &var) const;
    QString fixFileVarGlue(const ProKey &var, const QString &before, const QString &glue, const QString &after) const;
    QString fileVarList(const ProKey &var) const;
    QString fileVarGlue(const ProKey &var, const QString &before, const QString &glue, const QString &after) const;
    QString fileVar(const ProKey &var) const;
    QString depVar(const ProKey &var) const;
    QString val(const ProStringList &varList) const;
    QString val(const QStringList &varList) const;
    QString valGlue(const QStringList &varList, const QString &before, const QString &glue, const QString &after) const;
    QString valGlue(const ProStringList &varList, const QString &before, const QString &glue, const QString &after) const;
    QString valList(const QStringList &varList) const;
    QString valList(const ProStringList &varList) const;

    QString filePrefixRoot(const QString &, const QString &);

    enum LibFlagType { LibFlagLib, LibFlagPath, LibFlagFile, LibFlagOther };
    virtual LibFlagType parseLibFlag(const ProString &flag, ProString *arg);
    ProStringList fixLibFlags(const ProKey &var);
    virtual ProString fixLibFlag(const ProString &lib);

public:
    //file fixification to unify all file names into a single pattern
    enum FileFixifyType {
        FileFixifyFromIndir = 0,
        FileFixifyFromOutdir = 1,
        FileFixifyToOutDir = 0,
        FileFixifyToIndir = 2,
        FileFixifyBackwards = FileFixifyFromOutdir | FileFixifyToIndir,
        FileFixifyDefault = 0,
        FileFixifyAbsolute = 4,
        FileFixifyRelative = 8
    };
    Q_DECLARE_FLAGS(FileFixifyTypes, FileFixifyType)
protected:
    QString fileFixify(const QString &file, FileFixifyTypes fix = FileFixifyDefault, bool canon = true) const;
    QStringList fileFixify(const QStringList &files, FileFixifyTypes fix = FileFixifyDefault, bool canon = true) const;
    QString createSedArgs(const ProKey &replace_rule, const QString &file_type = QString()) const;
    QString installMetaFile(const ProKey &replace_rule, const QString &src,
                            const QString &dst) const;

    virtual bool processPrlFileBase(QString &origFile, QStringView origName,
                                    QStringView fixedBase, int slashOff);
    bool processPrlFileCore(QString &origFile, QStringView origName,
                            const QString &fixedFile);
    QString createResponseFile(const QString &baseName,
                               const ProStringList &objList,
                               const QString &prefix = QString()) const;

    struct LinkerResponseFileInfo
    {
        QString filePath;
        bool onlyObjects;

        bool isValid() const { return !filePath.isEmpty(); }
    };

    LinkerResponseFileInfo maybeCreateLinkerResponseFile() const;

public:
    QMakeProject *projectFile() const;
    void setProjectFile(QMakeProject *p);

    void setNoIO(bool o);
    bool noIO() const;

    inline bool exists(QString file) const { return fileInfo(file).exists(); }
    QFileInfo fileInfo(QString file) const;

    static MakefileGenerator *create(QMakeProject *);
    virtual bool write();
    virtual bool writeProjectMakefile();
    virtual bool supportsMetaBuild() { return true; }
    virtual bool supportsMergedBuilds() { return false; }
    virtual bool mergeBuildProject(MakefileGenerator * /*other*/) { return false; }
    virtual bool openOutput(QFile &, const QString &build) const;
    bool isWindowsShell() const { return Option::dir_sep == QLatin1String("\\"); }
    QString shellQuote(const QString &str) const;
    virtual ProKey fullTargetVariable() const;
};
Q_DECLARE_TYPEINFO(MakefileGenerator::Compiler, Q_RELOCATABLE_TYPE);
Q_DECLARE_OPERATORS_FOR_FLAGS(MakefileGenerator::FileFixifyTypes)

inline void MakefileGenerator::setNoIO(bool o)
{ no_io = o; }

inline bool MakefileGenerator::noIO() const
{ return no_io; }

inline QString MakefileGenerator::defaultInstall(const QString &)
{ return QString(""); }

inline QString MakefileGenerator::installRoot() const
{ return QStringLiteral("$(INSTALL_ROOT)"); }

inline bool MakefileGenerator::findLibraries(bool, bool)
{ return true; }

struct ReplaceExtraCompilerCacheKey
{
    mutable size_t hash;
    QString var, in, out, pwd;
    MakefileGenerator::ReplaceFor forShell;
    ReplaceExtraCompilerCacheKey(const QString &v, const QStringList &i, const QStringList &o, MakefileGenerator::ReplaceFor s);
    bool operator==(const ReplaceExtraCompilerCacheKey &f) const;
    inline size_t hashCode() const {
        if (!hash)
            hash = (size_t)forShell ^ qHash(var) ^ qHash(in) ^ qHash(out) /*^ qHash(pwd)*/;
        return hash;
    }
};
inline size_t qHash(const ReplaceExtraCompilerCacheKey &f) { return f.hashCode(); }

BOBUI_END_NAMESPACE

#endif // MAKEFILE_H
