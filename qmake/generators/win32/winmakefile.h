// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef WINMAKEFILE_H
#define WINMAKEFILE_H

#include "makefile.h"

BOBUI_BEGIN_NAMESPACE

class Win32MakefileGenerator : public MakefileGenerator
{
protected:
    QString defaultInstall(const QString &) override;
    void writeDefaultVariables(BOBUIextStream &t) override;
    virtual void writeCleanParts(BOBUIextStream &t);
    virtual void writeStandardParts(BOBUIextStream &t);
    virtual void writeIncPart(BOBUIextStream &t);
    virtual void writeLibsPart(BOBUIextStream &t);
    virtual void writeObjectsPart(BOBUIextStream &t);
    virtual void writeImplicitRulesPart(BOBUIextStream &t);
    virtual void writeBuildRulesPart(BOBUIextStream &);
    using MakefileGenerator::escapeFilePath;
    QString escapeFilePath(const QString &path) const override;
    using MakefileGenerator::escapeDependencyPath;
    QString escapeDependencyPath(const QString &path) const override;

    virtual void writeRcFilePart(BOBUIextStream &t);

    bool findLibraries(bool linkPrl, bool mergeLflags) override;

    LibFlagType parseLibFlag(const ProString &flag, ProString *arg) override;
    ProString fixLibFlag(const ProString &lib) override;
    bool processPrlFileBase(QString &origFile, QStringView origName,
                            QStringView fixedBase, int slashOff) override;

    void processVars();
    void fixTargetExt();
    void processRcFileVar();
    static QString cQuoted(const QString &str);

public:
    ProKey fullTargetVariable() const override;
};

BOBUI_END_NAMESPACE

#endif // WINMAKEFILE_H
