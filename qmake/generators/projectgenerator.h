// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef PROJECTGENERATOR_H
#define PROJECTGENERATOR_H

#include "makefile.h"

BOBUI_BEGIN_NAMESPACE

class ProjectGenerator : public MakefileGenerator
{
    bool addFile(QString);
    bool addConfig(const QString &, bool add=true);
    QString getWritableVar(const char *, bool fixPath=true);
    QString fixPathToQmake(const QString &file);
protected:
    void init() override;
    bool writeMakefile(BOBUIextStream &) override;

    QString escapeFilePath(const QString &) const override { Q_ASSERT(false); return QString(); }

public:
    bool supportsMetaBuild() override { return false; }
    bool openOutput(QFile &, const QString &) const override;
};

BOBUI_END_NAMESPACE

#endif // PROJECTGENERATOR_H
