#ifndef OMNIASSETMANAGER_H
#define OMNIASSETMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>
#include <QFile>

// A custom Virtual File System (VFS) to dynamically mount .pak/.wad style asset bundles
// at runtime, bypassing the need to recompile Qt .qrc resource files.
class OmniAssetManager : public QObject {
    Q_OBJECT

public:
    static OmniAssetManager* instance();

    // Mounts an OmniPak binary file into the VFS
    Q_INVOKABLE bool mountArchive(const QString& pakFilePath);

    // Checks if a file exists in the mounted VFS
    Q_INVOKABLE bool fileExists(const QString& virtualPath) const;

    // Retrieves the raw binary data of an asset
    QByteArray getFileBytes(const QString& virtualPath) const;

    // A helper to pack a directory into an OmniPak file (Developer Tooling)
    Q_INVOKABLE bool createArchive(const QString& sourceDirectory, const QString& outputPakPath);

signals:
    void archiveMounted(const QString& pakFilePath, int fileCount);

private:
    explicit OmniAssetManager(QObject *parent = nullptr);
    ~OmniAssetManager() override;

    struct FileEntry {
        QString pakPath;
        qint64 offset;
        qint64 size;
    };

    // Virtual Path -> Entry
    QMap<QString, FileEntry> m_vfs;
};

#endif // OMNIASSETMANAGER_H
