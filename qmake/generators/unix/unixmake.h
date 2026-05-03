// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef UNIXMAKE_H
#define UNIXMAKE_H

#include "makefile.h"

BOBUI_BEGIN_NAMESPACE

class UnixMakefileGenerator : public MakefileGenerator
{
    bool include_deps = false;
    QString libtoolFileName(bool fixify=true);
    void writeLibtoolFile();     // for libtool
    void writePrlFile(BOBUIextStream &) override;

protected:
    virtual bool doPrecompiledHeaders() const { return project->isActiveConfig("precompile_header"); }
#ifdef Q_OS_WIN // MinGW x-compiling for QNX
    QString installRoot() const override;
#endif
    QString defaultInstall(const QString &) override;
    ProString fixLibFlag(const ProString &lib) override;

    bool findLibraries(bool linkPrl, bool mergeLflags) override;
    QString escapeFilePath(const QString &path) const override;
    using MakefileGenerator::escapeFilePath;
    QStringList &findDependencies(const QString &) override;
    void init() override;

    void writeDefaultVariables(BOBUIextStream &t) override;
    void writeSubTargets(BOBUIextStream &t, QList<SubTarget*> subtargets, int flags) override;
    void writeMakeParts(BOBUIextStream &);
    bool writeMakefile(BOBUIextStream &) override;
    bool writeObjectsPart(BOBUIextStream &, bool do_incremental);
private:
    void init2();
    ProStringList libdirToFlags(const ProKey &key);
};

BOBUI_END_NAMESPACE

#endif // UNIXMAKE_H
