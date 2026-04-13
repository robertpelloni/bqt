// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef MINGW_MAKE_H
#define MINGW_MAKE_H

#include "winmakefile.h"

BOBUI_BEGIN_NAMESPACE

class MingwMakefileGenerator : public Win32MakefileGenerator
{
protected:
    using MakefileGenerator::escapeDependencyPath;
    QString escapeDependencyPath(const QString &path) const override;
    ProString fixLibFlag(const ProString &lib) override;
    bool processPrlFileBase(QString &origFile, QStringView origName,
                            QStringView fixedBase, int slashOff) override;
    bool writeMakefile(BOBUIextStream &) override;
    void init() override;
    QString installRoot() const override;
private:
    void writeMingwParts(BOBUIextStream &);
    void writeIncPart(BOBUIextStream &t) override;
    void writeLibsPart(BOBUIextStream &t) override;
    void writeObjectsPart(BOBUIextStream &t) override;
    void writeBuildRulesPart(BOBUIextStream &t) override;
    void writeRcFilePart(BOBUIextStream &t) override;

    QStringList &findDependencies(const QString &file) override;

    QString preCompHeaderOut;

    LibFlagType parseLibFlag(const ProString &flag, ProString *arg) override;

    QString objectsLinkLine;
    LinkerResponseFileInfo linkerResponseFile;
};

BOBUI_END_NAMESPACE

#endif // MINGW_MAKE_H
