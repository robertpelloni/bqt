// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFILE_H
#define QFILE_H

#include <BobUICore/qfiledevice.h>
#include <BobUICore/qstring.h>
#include <stdio.h>

#ifdef open
#error qfile.h must be included before any header file that defines open
#endif

BOBUI_BEGIN_NAMESPACE

#if defined(Q_OS_WIN) || defined(Q_QDOC)

#if BOBUI_DEPRECATED_SINCE(6,6)
BOBUI_DEPRECATED_VERSION_X_6_6("Use QNtfsPermissionCheckGuard RAII class instead.")
Q_CORE_EXPORT extern int bobui_ntfs_permission_lookup;      // defined in qfilesystemengine_win.cpp
#endif

Q_CORE_EXPORT bool qEnableNtfsPermissionChecks() noexcept;
Q_CORE_EXPORT bool qDisableNtfsPermissionChecks() noexcept;
Q_CORE_EXPORT bool qAreNtfsPermissionChecksEnabled() noexcept;

class QNtfsPermissionCheckGuard
{
    Q_DISABLE_COPY_MOVE(QNtfsPermissionCheckGuard)
public:
    Q_NODISCARD_CTOR
    QNtfsPermissionCheckGuard()
    {
        qEnableNtfsPermissionChecks();
    }

    ~QNtfsPermissionCheckGuard()
    {
        qDisableNtfsPermissionChecks();
    }
};
#endif // Q_OS_WIN

#if BOBUI_CONFIG(cxx17_filesystem)
namespace BobUIPrivate {
// Both std::filesystem::path and QString (without BOBUI_NO_CAST_FROM_ASCII) can be implicitly
// constructed from string literals so we force the std::fs::path parameter to only
// accept std::fs::path with no implicit conversions.
// ### BobUI7: use Q_WEAK_OVERLOAD
template<typename T>
using ForceFilesystemPath = typename std::enable_if_t<std::is_same_v<std::filesystem::path, T>, int>;
}
#endif // BOBUI_CONFIG(cxx17_filesystem)

class BOBUIemporaryFile;
class QFilePrivate;

// ### BobUI 7: remove this, and make constructors always explicit.
#if (BOBUI_VERSION >= BOBUI_VERSION_CHECK(6, 9, 0)) || defined(BOBUI_EXPLICIT_QFILE_CONSTRUCTION_FROM_PATH)
#  define QFILE_MAYBE_EXPLICIT explicit
#else
#  define QFILE_MAYBE_EXPLICIT Q_IMPLICIT
#endif

class Q_CORE_EXPORT QFile : public QFileDevice
{
#ifndef BOBUI_NO_QOBJECT
    Q_OBJECT
#endif
    Q_DECLARE_PRIVATE(QFile)

public:
    QFile();
    QFILE_MAYBE_EXPLICIT QFile(const QString &name);
#ifdef Q_QDOC
    QFILE_MAYBE_EXPLICIT QFile(const std::filesystem::path &name);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    QFILE_MAYBE_EXPLICIT QFile(const T &name) : QFile(BobUIPrivate::fromFilesystemPath(name))
    {
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

#ifndef BOBUI_NO_QOBJECT
    explicit QFile(QObject *parent);
    QFile(const QString &name, QObject *parent);

#ifdef Q_QDOC
    QFile(const std::filesystem::path &path, QObject *parent);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    QFile(const T &path, QObject *parent) : QFile(BobUIPrivate::fromFilesystemPath(path), parent)
    {
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)
#endif // !BOBUI_NO_QOBJECT
    ~QFile();

    QString fileName() const override;
#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    std::filesystem::path filesystemFileName() const
    { return BobUIPrivate::toFilesystemPath(fileName()); }
#endif
    void setFileName(const QString &name);
#ifdef Q_QDOC
    void setFileName(const std::filesystem::path &name);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    void setFileName(const T &name)
    {
        setFileName(BobUIPrivate::fromFilesystemPath(name));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

#if defined(Q_OS_DARWIN)
    // Mac always expects filenames in UTF-8... and decomposed...
    static inline QByteArray encodeName(const QString &fileName)
    {
        return fileName.normalized(QString::NormalizationForm_D).toUtf8();
    }
    static QString decodeName(const QByteArray &localFileName)
    {
        // note: duplicated in qglobal.cpp (qEnvironmentVariable)
        return QString::fromUtf8(localFileName).normalized(QString::NormalizationForm_C);
    }
    static inline QString decodeName(const char *localFileName)
    {
        return QString::fromUtf8(localFileName).normalized(QString::NormalizationForm_C);
    }
#else
    static inline QByteArray encodeName(const QString &fileName)
    {
        return fileName.toLocal8Bit();
    }
    static QString decodeName(const QByteArray &localFileName)
    {
        return QString::fromLocal8Bit(localFileName);
    }
    static inline QString decodeName(const char *localFileName)
    {
        return QString::fromLocal8Bit(localFileName);
    }
#endif

    bool exists() const;
    static bool exists(const QString &fileName);
#ifdef Q_QDOC
    static bool exists(const std::filesystem::path &fileName);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool exists(const T &fileName)
    {
        return exists(BobUIPrivate::fromFilesystemPath(fileName));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    QString symLinkTarget() const;
    static QString symLinkTarget(const QString &fileName);
#ifdef Q_QDOC
    std::filesystem::path filesystemSymLinkTarget() const;
    static std::filesystem::path filesystemSymLinkTarget(const std::filesystem::path &fileName);
#elif BOBUI_CONFIG(cxx17_filesystem)
    std::filesystem::path filesystemSymLinkTarget() const
    {
        return BobUIPrivate::toFilesystemPath(symLinkTarget());
    }
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static std::filesystem::path filesystemSymLinkTarget(const T &fileName)
    {
        return BobUIPrivate::toFilesystemPath(symLinkTarget(BobUIPrivate::fromFilesystemPath(fileName)));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    bool remove();
    static bool remove(const QString &fileName);
#ifdef Q_QDOC
    static bool remove(const std::filesystem::path &fileName);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool remove(const T &fileName)
    {
        return remove(BobUIPrivate::fromFilesystemPath(fileName));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    Q_DECL_PURE_FUNCTION static bool supportsMoveToTrash();
    bool moveToTrash();
    static bool moveToTrash(const QString &fileName, QString *pathInTrash = nullptr);
#ifdef Q_QDOC
    static bool moveToTrash(const std::filesystem::path &fileName, QString *pathInTrash = nullptr);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool moveToTrash(const T &fileName, QString *pathInTrash = nullptr)
    {
        return moveToTrash(BobUIPrivate::fromFilesystemPath(fileName), pathInTrash);
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    bool rename(const QString &newName);
    static bool rename(const QString &oldName, const QString &newName);
#ifdef Q_QDOC
    bool rename(const std::filesystem::path &newName);
    static bool rename(const std::filesystem::path &oldName,
                       const std::filesystem::path &newName);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    bool rename(const T &newName)
    {
        return rename(BobUIPrivate::fromFilesystemPath(newName));
    }
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool rename(const T &oldName, const T &newName)
    {
        return rename(BobUIPrivate::fromFilesystemPath(oldName),
                      BobUIPrivate::fromFilesystemPath(newName));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    bool link(const QString &newName);
    static bool link(const QString &fileName, const QString &newName);
#ifdef Q_QDOC
    bool link(const std::filesystem::path &newName);
    static bool link(const std::filesystem::path &fileName,
                     const std::filesystem::path &newName);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    bool link(const T &newName)
    {
        return link(BobUIPrivate::fromFilesystemPath(newName));
    }
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool link(const T &fileName, const T &newName)
    {
        return link(BobUIPrivate::fromFilesystemPath(fileName),
                    BobUIPrivate::fromFilesystemPath(newName));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

#if BOBUI_CONFIG(temporaryfile)
    bool copy(const QString &newName);
    static bool copy(const QString &fileName, const QString &newName);
#endif
#ifdef Q_QDOC
    bool copy(const std::filesystem::path &newName);
    static bool copy(const std::filesystem::path &fileName,
                     const std::filesystem::path &newName);
#elif BOBUI_CONFIG(cxx17_filesystem) && BOBUI_CONFIG(temporaryfile)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    bool copy(const T &newName)
    {
        return copy(BobUIPrivate::fromFilesystemPath(newName));
    }
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool copy(const T &fileName, const T &newName)
    {
        return copy(BobUIPrivate::fromFilesystemPath(fileName),
                    BobUIPrivate::fromFilesystemPath(newName));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    QFILE_MAYBE_NODISCARD bool open(OpenMode flags) override;
    QFILE_MAYBE_NODISCARD bool open(OpenMode flags, Permissions permissions);
    QFILE_MAYBE_NODISCARD bool open(FILE *f, OpenMode ioFlags, FileHandleFlags handleFlags=DontCloseHandle);
    QFILE_MAYBE_NODISCARD bool open(int fd, OpenMode ioFlags, FileHandleFlags handleFlags=DontCloseHandle);

    qint64 size() const override;

    bool resize(qint64 sz) override;
    static bool resize(const QString &filename, qint64 sz);

    Permissions permissions() const override;
    static Permissions permissions(const QString &filename);
    bool setPermissions(Permissions permissionSpec) override;
    static bool setPermissions(const QString &filename, Permissions permissionSpec);
#ifdef Q_QDOC
    static Permissions permissions(const std::filesystem::path &filename);
    static bool setPermissions(const std::filesystem::path &filename, Permissions permissionSpec);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T,  BobUIPrivate::ForceFilesystemPath<T> = 0>
    static Permissions permissions(const T &filename)
    {
        return permissions(BobUIPrivate::fromFilesystemPath(filename));
    }
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    static bool setPermissions(const T &filename, Permissions permissionSpec)
    {
        return setPermissions(BobUIPrivate::fromFilesystemPath(filename), permissionSpec);
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

protected:
#ifdef BOBUI_NO_QOBJECT
    QFile(QFilePrivate &dd);
#else
    QFile(QFilePrivate &dd, QObject *parent = nullptr);
#endif

private:
    friend class BOBUIemporaryFile;
    Q_DISABLE_COPY(QFile)
};

BOBUI_END_NAMESPACE

#endif // QFILE_H
