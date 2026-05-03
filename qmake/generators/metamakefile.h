// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef METAMAKEFILE_H
#define METAMAKEFILE_H

#include <qlist.h>
#include <qstring.h>

BOBUI_BEGIN_NAMESPACE

class QMakeProject;
class MakefileGenerator;

class MetaMakefileGenerator
{
protected:
    MetaMakefileGenerator(QMakeProject *p, const QString &n, bool op=true) : project(p), own_project(op), name(n) { }
    QMakeProject *project;
    bool own_project;
    QString name;

public:

    virtual ~MetaMakefileGenerator();

    static MetaMakefileGenerator *createMetaGenerator(QMakeProject *proj, const QString &name, bool op=true, bool *success = nullptr);
    static MakefileGenerator *createMakefileGenerator(QMakeProject *proj, bool noIO = false);

    inline QMakeProject *projectFile() const { return project; }

    virtual bool init() = 0;
    virtual int type() const { return -1; }
    virtual bool write() = 0;
};

BOBUI_END_NAMESPACE

#endif // METAMAKEFILE_H
