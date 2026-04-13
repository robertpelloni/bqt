// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef PBUILDER_PBX_H
#define PBUILDER_PBX_H

#include "unixmake.h"

BOBUI_BEGIN_NAMESPACE

class ProjectBuilderMakefileGenerator : public UnixMakefileGenerator
{
    bool writingUnixMakefileGenerator;
    mutable QString pbx_dir;
    int pbuilderVersion() const;
    bool writeSubDirs(BOBUIextStream &);
    bool writeMakeParts(BOBUIextStream &);
    bool writeMakefile(BOBUIextStream &) override;
    bool replaceLibrarySuffix(const QString &lib_file, const ProString &opt, QString &name,
                              QString &library);

    QString pbxbuild();
    QHash<QString, QString> keys;
    QString keyFor(const QString &file);
    QString findProgram(const ProString &prog);
    QString fixForOutput(const QString &file);
    ProStringList fixListForOutput(const char *where);
    ProStringList fixListForOutput(const ProStringList &list);
    int     reftypeForFile(const QString &where);
    QString projectSuffix() const;
    enum { SettingsAsList=0x01, SettingsNoQuote=0x02 };
    inline QString writeSettings(const QString &var, const char *val, int flags=0, int indent_level=0)
        { return writeSettings(var, ProString(val), flags, indent_level); }
    inline QString writeSettings(const QString &var, const ProString &val, int flags=0, int indent_level=0)
        { return writeSettings(var, ProStringList(val), flags, indent_level); }
    QString writeSettings(const QString &var, const ProStringList &vals, int flags=0, int indent_level=0);

public:
    bool supportsMetaBuild() override { return false; }
    bool openOutput(QFile &, const QString &) const override;
protected:
    bool doPrecompiledHeaders() const override { return false; }
    bool doDepends() const override { return writingUnixMakefileGenerator && UnixMakefileGenerator::doDepends(); }
    bool inhibitMakeDirOutPath(const ProKey &path) const override;
};
BOBUI_END_NAMESPACE

#endif // PBUILDER_PBX_H
