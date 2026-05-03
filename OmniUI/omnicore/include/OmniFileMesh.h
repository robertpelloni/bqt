#ifndef OMNIFILEMESH_H
#define OMNIFILEMESH_H

#include <QObject>
#include <QString>
#include <QByteArray>

// Beams physical file data across the global P2P OS Mesh.
class OmniFileMesh : public QObject {
    Q_OBJECT
public:
    static OmniFileMesh* instance();

    // Broadcasts a local file's bytes to the mesh
    Q_INVOKABLE void shareFile(const QString& virtualPath, const QByteArray& data);

    // Internal hook for MeshNode to inject remote files
    void receiveRemoteFile(const QString& virtualPath, const QByteArray& data);

signals:
    void fileShared(const QString& virtualPath);
    void remoteFileReceived(const QString& virtualPath, int byteSize);

private:
    explicit OmniFileMesh(QObject *parent = nullptr);
    ~OmniFileMesh() override;
};

#endif // OMNIFILEMESH_H
