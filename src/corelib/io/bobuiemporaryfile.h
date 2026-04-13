// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEMPORARYFILE_H
#define BOBUIEMPORARYFILE_H

#include <BobUICore/qiodevice.h>
#include <BobUICore/qfile.h>

#ifdef open
#error bobuiemporaryfile.h must be included before any header file that defines open
#endif

BOBUI_BEGIN_NAMESPACE


#if BOBUI_CONFIG(temporaryfile)

class BOBUIemporaryFilePrivate;
class QLockFilePrivate;

class Q_CORE_EXPORT BOBUIemporaryFile : public QFile
{
#ifndef BOBUI_NO_QOBJECT
    Q_OBJECT
#endif
    Q_DECLARE_PRIVATE(BOBUIemporaryFile)

public:
    BOBUIemporaryFile();
    explicit BOBUIemporaryFile(const QString &templateName);
#ifndef BOBUI_NO_QOBJECT
    explicit BOBUIemporaryFile(QObject *parent);
    BOBUIemporaryFile(const QString &templateName, QObject *parent);

#  if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD
    explicit BOBUIemporaryFile(const std::filesystem::path &templateName, QObject *parent = nullptr)
        : BOBUIemporaryFile(BobUIPrivate::fromFilesystemPath(templateName), parent)
    {
    }
#  endif // BOBUI_CONFIG(cxx17_filesystem)
#endif // !BOBUI_NO_QOBJECT

    ~BOBUIemporaryFile();

    bool autoRemove() const;
    void setAutoRemove(bool b);

    // ### Hides open(flags)
    QFILE_MAYBE_NODISCARD bool open() { return open(QIODevice::ReadWrite); }

    QString fileName() const override;
    QString fileTemplate() const;
    void setFileTemplate(const QString &name);
#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD
    void setFileTemplate(const std::filesystem::path &name)
    {
        return setFileTemplate(BobUIPrivate::fromFilesystemPath(name));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    // Hides QFile::rename
    bool rename(const QString &newName);
    bool renameOverwrite(const QString &newName);

#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD
    bool rename(const std::filesystem::path &newName)
    {
        return rename(BobUIPrivate::fromFilesystemPath(newName));
    }
    Q_WEAK_OVERLOAD
    bool renameOverwrite(const std::filesystem::path &newName)
    { return renameOverwrite(BobUIPrivate::fromFilesystemPath(newName)); }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    inline static BOBUIemporaryFile *createNativeFile(const QString &fileName)
        { QFile file(fileName); return createNativeFile(file); }
    static BOBUIemporaryFile *createNativeFile(QFile &file);

#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD
    static BOBUIemporaryFile *createNativeFile(const std::filesystem::path &fileName)
    {
        QFile file(fileName);
        return createNativeFile(file);
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

protected:
    bool open(OpenMode flags) override;

private:
    friend class QFile;
    friend class QLockFilePrivate;
    Q_DISABLE_COPY(BOBUIemporaryFile)
};

#endif // BOBUI_CONFIG(temporaryfile)

BOBUI_END_NAMESPACE

#endif // BOBUIEMPORARYFILE_H
