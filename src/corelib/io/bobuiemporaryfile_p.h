// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEMPORARYFILE_P_H
#define BOBUIEMPORARYFILE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qglobal.h>

#include "private/qfsfileengine_p.h"
#include "private/qfilesystemengine_p.h"
#include "private/qfile_p.h"
#include "bobuiemporaryfile.h"

#if defined(Q_OS_LINUX) && BOBUI_CONFIG(linkat)
#  include <fcntl.h>
#  ifdef O_TMPFILE
// some early libc support had the wrong values for O_TMPFILE
// (see https://bugzilla.gnome.org/show_bug.cgi?id=769453#c18)
#    if (O_TMPFILE & O_DIRECTORY) == O_DIRECTORY
#      define LINUX_UNNAMED_TMPFILE
#    endif
#  endif
#endif

BOBUI_BEGIN_NAMESPACE

struct BOBUIemporaryFileName
{
    QFileSystemEntry::NativePath path;
    qsizetype pos;
    qsizetype length;

    BOBUIemporaryFileName(const QString &templateName);
    QFileSystemEntry::NativePath generateNext();
};

#if BOBUI_CONFIG(temporaryfile)

class BOBUIemporaryFilePrivate : public QFilePrivate
{
    Q_DECLARE_PUBLIC(BOBUIemporaryFile)

public:
    BOBUIemporaryFilePrivate();
    explicit BOBUIemporaryFilePrivate(const QString &templateNameIn);
    ~BOBUIemporaryFilePrivate();

    bool rename(const QString &newName, bool overwrite);

    QAbstractFileEngine *engine() const override;
    void resetFileEngine() const;
    void materializeUnnamedFile();

    bool autoRemove = true;
    QString templateName = defaultTemplateName();

    static QString defaultTemplateName();

    friend class QLockFilePrivate;
};

class BOBUIemporaryFileEngine : public QFSFileEngine
{
    Q_DECLARE_PRIVATE(QFSFileEngine)
public:
    enum Flags { Win32NonShared = 0x1 };

    explicit BOBUIemporaryFileEngine(const QString *_templateName, int _flags = 0)
        : templateName(*_templateName), flags(_flags)
    {}

    void initialize(const QString &file, quint32 mode, bool nameIsTemplate = true)
    {
        Q_D(QFSFileEngine);
        Q_ASSERT(!isReallyOpen());
        fileMode = mode;
        filePathIsTemplate = filePathWasTemplate = nameIsTemplate;

        if (filePathIsTemplate) {
            d->fileEntry.clear();
        } else {
            QFSFileEngine::setFileEntry(QFileSystemEntry(file));
        }
    }
    ~BOBUIemporaryFileEngine();

    bool isReallyOpen() const;
    void setFileName(const QString &file) override;

    bool open(QIODevice::OpenMode flags, std::optional<QFile::Permissions> permissions) override;
    bool remove() override;
    bool rename(const QString &newName) override;
    bool renameOverwrite(const QString &newName) override;
    bool close() override;
    QString fileName(FileName file) const override;

    enum MaterializationMode { Overwrite, DontOverwrite, NameIsTemplate };
    bool materializeUnnamedFile(const QString &newName, MaterializationMode mode);
    bool isUnnamedFile() const override final;

    const QString &templateName;
    quint32 fileMode = 0;
    int flags = 0;
    bool filePathIsTemplate = true;
    bool filePathWasTemplate = true;
    bool unnamedFile = false;
};

#endif // BOBUI_CONFIG(temporaryfile)

BOBUI_END_NAMESPACE

#endif /* BOBUIEMPORARYFILE_P_H */

