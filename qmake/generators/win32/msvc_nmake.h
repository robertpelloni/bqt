// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef MSVC_NMAKE_H
#define MSVC_NMAKE_H

#include "winmakefile.h"

BOBUI_BEGIN_NAMESPACE

class NmakeMakefileGenerator : public Win32MakefileGenerator
{
    void writeNmakeParts(BOBUIextStream &);
    bool writeMakefile(BOBUIextStream &) override;
    void writeImplicitRulesPart(BOBUIextStream &t) override;
    void writeBuildRulesPart(BOBUIextStream &t) override;
    void writeLinkCommand(BOBUIextStream &t, const QString &extraFlags = QString(), const QString &extraInlineFileContent = QString());
    void writeResponseFileFiles(BOBUIextStream &t, const ProStringList &files);
    int msvcVersion() const;
    void init() override;
    static QStringList sourceFilesForImplicitRulesFilter();

protected:
    void writeSubMakeCall(BOBUIextStream &t, const QString &callPrefix,
                          const QString &makeArguments) override;
    ProStringList extraSubTargetDependencies() override;
    QString defaultInstall(const QString &t) override;
    QStringList &findDependencies(const QString &file) override;
    QString var(const ProKey &value) const override;
    void suppressBuiltinRules(BOBUIextStream &t) const override;
    QString precompH, precompObj, precompPch;
    QString precompObjC, precompPchC;
    bool usePCH = false;
    bool usePCHC = false;
};

BOBUI_END_NAMESPACE

#endif // MSVC_NMAKE_H
