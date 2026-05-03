// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDAPKFILEENGINE_H
#define QANDROIDAPKFILEENGINE_H

#include <BobUICore/private/qabstractfileengine_p.h>
#include <BobUICore/qjnitypes.h>
#include <BobUICore/QJniObject>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_JNI_CLASS(BobUIApkFileEngine, "org/bobuiproject/bobui/android/BobUIApkFileEngine")

class QAndroidApkFileEngine : public QAbstractFileEngine
{
public:
    QAndroidApkFileEngine(const QString &fileName);
    ~QAndroidApkFileEngine();

    struct FileInfo
    {
        QString relativePath;
        qint64 size = -1;
        bool isDir;
    };

    bool open(QIODevice::OpenMode openMode, std::optional<QFile::Permissions> permissions) override;
    bool close() override;
    qint64 size() const override;
    qint64 pos() const override;
    bool seek(qint64 pos) override;
    qint64 read(char *data, qint64 maxlen) override;
    FileFlags fileFlags(FileFlags type = FileInfoAll) const override;
    bool caseSensitive() const override { return true; }
    QString fileName(FileName file = DefaultName) const override;
    void setFileName(const QString &file) override;

    uchar *map(qint64 offset, qint64 size, QFile::MemoryMapFlags flags);
    bool extension(Extension extension, const ExtensionOption *option = nullptr,
                   ExtensionReturn *output = nullptr) override;
    bool supportsExtension(Extension extension) const override;

    static QString apkPath();
    static QString relativePath(const QString &filePath);

#ifndef BOBUI_NO_FILESYSTEMITERATOR
    IteratorUniquePtr beginEntryList(const QString &, QDirListing::IteratorFlags filters,
                                     const QStringList &filterNames) override;
#endif

private:
    QString m_fileName;
    FileInfo *m_fileInfo = nullptr;
    BobUIJniTypes::BobUIApkFileEngine m_apkFileEngine;
};

#ifndef BOBUI_NO_FILESYSTEMITERATOR
class QAndroidApkFileEngineIterator : public QAbstractFileEngineIterator
{
public:
    QAndroidApkFileEngineIterator(const QString &path,
                                  QDirListing::IteratorFlags filters,
                                  const QStringList &filterNames);
    ~QAndroidApkFileEngineIterator();

    bool advance() override;
    QString currentFileName() const override;
    QString currentFilePath() const override;

private:
    int m_index = -1;
    QList<QAndroidApkFileEngine::FileInfo *> m_infos;
};
#endif

class QAndroidApkFileEngineHandler : public QAbstractFileEngineHandler
{
    Q_DISABLE_COPY_MOVE(QAndroidApkFileEngineHandler)
public:
    QAndroidApkFileEngineHandler() = default;

    std::unique_ptr<QAbstractFileEngine> create(const QString &fileName) const override;
};

BOBUI_END_NAMESPACE

#endif // QANDROIDAPKFILEENGINE_H
