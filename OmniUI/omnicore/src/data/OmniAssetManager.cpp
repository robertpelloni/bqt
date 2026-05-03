#include "OmniAssetManager.h"
#include <QDataStream>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>

const quint32 OMNI_MAGIC = 0x4F4D4E49; // "OMNI" in hex

OmniAssetManager* OmniAssetManager::instance() {
    static OmniAssetManager* manager = new OmniAssetManager();
    return manager;
}

OmniAssetManager::OmniAssetManager(QObject *parent) : QObject(parent) {
    setObjectName("OmniAssetManager");
}

OmniAssetManager::~OmniAssetManager() = default;

bool OmniAssetManager::mountArchive(const QString& pakFilePath) {
    QFile file(pakFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "OmniAssetManager: Failed to open archive:" << pakFilePath;
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::BobUI_6_0);

    quint32 magic;
    in >> magic;
    if (magic != OMNI_MAGIC) {
        qWarning() << "OmniAssetManager: Invalid magic number in archive:" << pakFilePath;
        return false;
    }

    int fileCount;
    in >> fileCount;

    for (int i = 0; i < fileCount; ++i) {
        QString virtualPath;
        qint64 offset;
        qint64 size;
        in >> virtualPath >> offset >> size;

        FileEntry entry;
        entry.pakPath = pakFilePath;
        entry.offset = offset;
        entry.size = size;

        m_vfs.insert(virtualPath, entry);
    }

    qDebug() << "OmniAssetManager: Successfully mounted" << fileCount << "files from" << pakFilePath;
    emit archiveMounted(pakFilePath, fileCount);
    return true;
}

bool OmniAssetManager::fileExists(const QString& virtualPath) const {
    return m_vfs.contains(virtualPath);
}

QByteArray OmniAssetManager::getFileBytes(const QString& virtualPath) const {
    if (!m_vfs.contains(virtualPath)) {
        qWarning() << "OmniAssetManager: File not found in VFS:" << virtualPath;
        return QByteArray();
    }

    const FileEntry& entry = m_vfs[virtualPath];
    QFile file(entry.pakPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }

    file.seek(entry.offset);
    return file.read(entry.size);
}

bool OmniAssetManager::createArchive(const QString& sourceDirectory, const QString& outputPakPath) {
    QDir dir(sourceDirectory);
    if (!dir.exists()) return false;

    QFile outFile(outputPakPath);
    if (!outFile.open(QIODevice::WriteOnly)) return false;

    QDataStream out(&outFile);
    out.setVersion(QDataStream::BobUI_6_0);

    // Write Magic
    out << OMNI_MAGIC;

    QDirIterator it(sourceDirectory, QDir::Files, QDirIterator::Subdirectories);
    QList<QFileInfo> files;
    while (it.hasNext()) {
        it.next();
        files.append(it.fileInfo());
    }

    // Write file count
    out << files.size();

    // Calculate metadata size to know where raw data begins
    qint64 headerStartPos = outFile.pos();
    
    // Write placeholder metadata (will be overwritten with exact offsets later)
    for (const QFileInfo& fi : std::as_const(files)) {
        QString virtualPath = dir.relativeFilePath(fi.absoluteFilePath());
        out << virtualPath << (qint64)0 << (qint64)0;
    }

    qint64 dataStartPos = outFile.pos();
    QMap<QString, QPair<qint64, qint64>> offsets; // virtualPath -> <offset, size>

    // Write Raw Data
    for (const QFileInfo& fi : std::as_const(files)) {
        QString virtualPath = dir.relativeFilePath(fi.absoluteFilePath());
        QFile inFile(fi.absoluteFilePath());
        if (inFile.open(QIODevice::ReadOnly)) {
            qint64 offset = outFile.pos();
            QByteArray data = inFile.readAll();
            outFile.write(data);
            qint64 size = data.size();
            offsets.insert(virtualPath, qMakePair(offset, size));
        }
    }

    // Go back and rewrite the actual offsets in the header
    outFile.seek(headerStartPos);
    for (const QFileInfo& fi : std::as_const(files)) {
        QString virtualPath = dir.relativeFilePath(fi.absoluteFilePath());
        out << virtualPath << offsets[virtualPath].first << offsets[virtualPath].second;
    }

    qDebug() << "OmniAssetManager: Packed" << files.size() << "files into" << outputPakPath;
    return true;
}
