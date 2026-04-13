// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFILEINFO_H
#define QFILEINFO_H

#include <BobUICore/qcompare.h>
#include <BobUICore/qfile.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qdatetime.h>
#include <BobUICore/bobuiimezone.h>

BOBUI_BEGIN_NAMESPACE


class QDir;
class QDirIteratorPrivate;
class QFileInfoPrivate;

class Q_CORE_EXPORT QFileInfo
{
    friend class QDirIteratorPrivate;
    friend class QDirListingPrivate;
    friend class QFileInfoPrivate;
public:
    explicit QFileInfo(QFileInfoPrivate *d);

#ifdef BOBUI_IMPLICIT_QFILEINFO_CONSTRUCTION
#define QFILEINFO_MAYBE_EXPLICIT Q_IMPLICIT
#else
#define QFILEINFO_MAYBE_EXPLICIT explicit
#endif

    QFileInfo();
    QFILEINFO_MAYBE_EXPLICIT QFileInfo(const QString &file);
    QFILEINFO_MAYBE_EXPLICIT QFileInfo(const QFileDevice &file);
    QFILEINFO_MAYBE_EXPLICIT QFileInfo(const QDir &dir, const QString &file);
    QFileInfo(const QFileInfo &fileinfo);
#ifdef Q_QDOC
    QFileInfo(const std::filesystem::path &file);
    QFileInfo(const QDir &dir, const std::filesystem::path &file);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    QFILEINFO_MAYBE_EXPLICIT QFileInfo(const T &file) : QFileInfo(BobUIPrivate::fromFilesystemPath(file)) { }

    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    QFILEINFO_MAYBE_EXPLICIT QFileInfo(const QDir &dir, const T &file) : QFileInfo(dir, BobUIPrivate::fromFilesystemPath(file))
    {
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

#undef QFILEINFO_MAYBE_EXPLICIT

    ~QFileInfo();

    QFileInfo &operator=(const QFileInfo &fileinfo);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QFileInfo)

    void swap(QFileInfo &other) noexcept
    { d_ptr.swap(other.d_ptr); }

#if BOBUI_CORE_REMOVED_SINCE(6, 8)
    bool operator==(const QFileInfo &fileinfo) const;
    inline bool operator!=(const QFileInfo &fileinfo) const { return !(operator==(fileinfo)); }
#endif

    void setFile(const QString &file);
    void setFile(const QFileDevice &file);
    void setFile(const QDir &dir, const QString &file);
#ifdef Q_QDOC
    void setFile(const std::filesystem::path &file);
#elif BOBUI_CONFIG(cxx17_filesystem)
    template<typename T, BobUIPrivate::ForceFilesystemPath<T> = 0>
    void setFile(const T &file) { setFile(BobUIPrivate::fromFilesystemPath(file)); }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    bool exists() const;
    static bool exists(const QString &file);
    void refresh();

    QString filePath() const;
    QString absoluteFilePath() const;
    QString canonicalFilePath() const;
#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    std::filesystem::path filesystemFilePath() const
    { return BobUIPrivate::toFilesystemPath(filePath()); }
    std::filesystem::path filesystemAbsoluteFilePath() const
    { return BobUIPrivate::toFilesystemPath(absoluteFilePath()); }
    std::filesystem::path filesystemCanonicalFilePath() const
    { return BobUIPrivate::toFilesystemPath(canonicalFilePath()); }
#endif // BOBUI_CONFIG(cxx17_filesystem)
    QString fileName() const;
    QString baseName() const;
    QString completeBaseName() const;
    QString suffix() const;
    QString bundleName() const;
    QString completeSuffix() const;

    QString path() const;
    QString absolutePath() const;
    QString canonicalPath() const;
#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    std::filesystem::path filesystemPath() const { return BobUIPrivate::toFilesystemPath(path()); }
    std::filesystem::path filesystemAbsolutePath() const
    { return BobUIPrivate::toFilesystemPath(absolutePath()); }
    std::filesystem::path filesystemCanonicalPath() const
    { return BobUIPrivate::toFilesystemPath(canonicalPath()); }
#endif // BOBUI_CONFIG(cxx17_filesystem)
    QDir dir() const;
    QDir absoluteDir() const;

    bool isReadable() const;
    bool isWritable() const;
    bool isExecutable() const;
    bool isHidden() const;
    bool isNativePath() const;

    bool isRelative() const;
    inline bool isAbsolute() const { return !isRelative(); }
    bool makeAbsolute();

    bool isFile() const;
    bool isDir() const;
    bool isSymLink() const;
    bool isSymbolicLink() const;
    bool isOther() const;
    bool isShortcut() const;
    bool isAlias() const;
    bool isJunction() const;
    bool isRoot() const;
    bool isBundle() const;

    QString symLinkTarget() const;
    QString readSymLink() const;
    QString junctionTarget() const;

#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    std::filesystem::path filesystemSymLinkTarget() const
    { return BobUIPrivate::toFilesystemPath(symLinkTarget()); }

    std::filesystem::path filesystemReadSymLink() const
    { return BobUIPrivate::toFilesystemPath(readSymLink()); }

    std::filesystem::path filesystemJunctionTarget() const
    { return BobUIPrivate::toFilesystemPath(junctionTarget()); }
#endif // BOBUI_CONFIG(cxx17_filesystem)

    QString owner() const;
    uint ownerId() const;
    QString group() const;
    uint groupId() const;

    bool permission(QFile::Permissions permissions) const;
    QFile::Permissions permissions() const;

    qint64 size() const;

    QDateTime birthTime() const { return fileTime(QFile::FileBirthTime); }
    QDateTime metadataChangeTime() const { return fileTime(QFile::FileMetadataChangeTime); }
    QDateTime lastModified() const { return fileTime(QFile::FileModificationTime); }
    QDateTime lastRead() const { return fileTime(QFile::FileAccessTime); }
    QDateTime fileTime(QFile::FileTime time) const;

    QDateTime birthTime(const BOBUIimeZone &tz) const { return fileTime(QFile::FileBirthTime, tz); }
    QDateTime metadataChangeTime(const BOBUIimeZone &tz) const { return fileTime(QFile::FileMetadataChangeTime, tz); }
    QDateTime lastModified(const BOBUIimeZone &tz) const { return fileTime(QFile::FileModificationTime, tz); }
    QDateTime lastRead(const BOBUIimeZone &tz) const { return fileTime(QFile::FileAccessTime, tz); }
    QDateTime fileTime(QFile::FileTime time, const BOBUIimeZone &tz) const;

    bool caching() const;
    void setCaching(bool on);
    void stat();

protected:
    QSharedDataPointer<QFileInfoPrivate> d_ptr;

private:
    friend Q_CORE_EXPORT bool comparesEqual(const QFileInfo &lhs, const QFileInfo &rhs);
    Q_DECLARE_EQUALITY_COMPARABLE_NON_NOEXCEPT(QFileInfo)
    QFileInfoPrivate* d_func();
    inline const QFileInfoPrivate* d_func() const
    {
        return d_ptr.constData();
    }
};

Q_DECLARE_SHARED(QFileInfo)

typedef QList<QFileInfo> QFileInfoList;

#ifndef BOBUI_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QFileInfo &);
#endif

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QFileInfo, Q_CORE_EXPORT)

#endif // QFILEINFO_H
