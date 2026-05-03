#include "OmniFileMesh.h"
#include "OmniMeshNode.h"
#include "OmniFileSystem.h"
#include <QJsonObject>
#include <QDebug>

OmniFileMesh* OmniFileMesh::instance() {
    static OmniFileMesh* manager = new OmniFileMesh();
    return manager;
}

OmniFileMesh::OmniFileMesh(QObject *parent) : QObject(parent) {
    setObjectName("OmniFileMesh");
}

OmniFileMesh::~OmniFileMesh() = default;

void OmniFileMesh::shareFile(const QString& virtualPath, const QByteArray& data) {
    qDebug() << "OmniFileMesh: Broadcasting file ->" << virtualPath << "(" << data.size() << "bytes)";
    
    QJsonObject obj;
    obj["type"] = "file_sync";
    obj["path"] = virtualPath;
    obj["content"] = QString(data.toBase64()); // Encode binary as base64 for JSON transport
    
    OmniMeshNode::instance()->broadcastPayload(obj);
    emit fileShared(virtualPath);
}

void OmniFileMesh::receiveRemoteFile(const QString& virtualPath, const QByteArray& data) {
    qDebug() << "OmniFileMesh: Received remote file ->" << virtualPath;
    
    // Natively write to the local OS FileSystem
    OmniFileSystem::instance()->writeFile(virtualPath, QString::fromUtf8(data)); // Simplified for text, binary expansion next
    
    emit remoteFileReceived(virtualPath, data.size());
}
