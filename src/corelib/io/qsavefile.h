// Copyright (C) 2012 David Faure <faure@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef QSAVEFILE_H
#define QSAVEFILE_H

#include <BobUICore/qglobal.h>

#if BOBUI_CONFIG(temporaryfile)

#include <BobUICore/qfiledevice.h>
#include <BobUICore/qstring.h>

#ifdef open
#error qsavefile.h must be included before any header file that defines open
#endif

BOBUI_BEGIN_NAMESPACE

class QAbstractFileEngine;
class QSaveFilePrivate;

class Q_CORE_EXPORT QSaveFile : public QFileDevice
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSaveFile)

public:
#if BOBUI_CORE_REMOVED_SINCE(6, 11)
    explicit QSaveFile(const QString &name);
#endif
    explicit QSaveFile(QObject *parent = nullptr);
    explicit QSaveFile(const QString &name, QObject *parent = nullptr);
    ~QSaveFile();

    QString fileName() const override;
    void setFileName(const QString &name);

    QFILE_MAYBE_NODISCARD bool open(OpenMode flags) override;
    bool commit();

    void cancelWriting();

    void setDirectWriteFallback(bool enabled);
    bool directWriteFallback() const;

#if BOBUI_CONFIG(cxx17_filesystem) || defined(Q_QDOC)
    Q_WEAK_OVERLOAD QSaveFile(const std::filesystem::path &path, QObject *parent = nullptr)
        : QSaveFile(BobUIPrivate::fromFilesystemPath(path), parent)
    {
    }

    std::filesystem::path filesystemFileName() const
    { return BobUIPrivate::toFilesystemPath(fileName()); }
    Q_WEAK_OVERLOAD void setFileName(const std::filesystem::path &name)
    {
        setFileName(BobUIPrivate::fromFilesystemPath(name));
    }
#endif // BOBUI_CONFIG(cxx17_filesystem)

protected:
    qint64 writeData(const char *data, qint64 len) override;

private:
    void close() override;
#if !BOBUI_CONFIG(translation)
    static QString tr(const char *string) { return QString::fromLatin1(string); }
#endif

private:
    Q_DISABLE_COPY(QSaveFile)
    friend class QFilePrivate;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(temporaryfile)

#endif // QSAVEFILE_H
