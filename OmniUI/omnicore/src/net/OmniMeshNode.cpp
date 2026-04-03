#include "OmniMeshNode.h"
#include "OmniInputManager.h"
#include <QDebug>

OmniMeshNode* OmniMeshNode::instance() {
    static OmniMeshNode* node = new OmniMeshNode();
    return node;
}

OmniMeshNode::OmniMeshNode(QObject *parent)
    : QObject(parent),
      m_server(nullptr),
      m_isConnected(false)
{
    setObjectName("OmniMeshNode");
    m_server = new QWebSocketServer(QStringLiteral("OmniMesh P2P Node"), QWebSocketServer::NonSecureMode, this);
}

OmniMeshNode::~OmniMeshNode() {
    stopNode();
}

bool OmniMeshNode::isConnected() const { return m_isConnected; }
int OmniMeshNode::activePeers() const { return m_peers.size(); }

bool OmniMeshNode::startNode(int port) {
    if (m_isConnected) stopNode();

    if (m_server->listen(QHostAddress::Any, port)) {
        m_isConnected = true;
        connect(m_server, &QWebSocketServer::newConnection, this, &OmniMeshNode::onNewConnection);
        qDebug() << "OmniMeshNode: P2P Server started. Listening on port" << port;
        emit connectionStatusChanged();
        return true;
    } else {
        qCritical() << "OmniMeshNode: Failed to start server on port" << port << m_server->errorString();
        return false;
    }
}

void OmniMeshNode::stopNode() {
    if (m_isConnected) {
        m_server->close();
        for (auto* peer : m_peers) {
            peer->close();
            peer->deleteLater();
        }
        m_peers.clear();
        m_isConnected = false;
        qDebug() << "OmniMeshNode: P2P Server stopped.";
        emit connectionStatusChanged();
        emit activePeersChanged();
    }
}

bool OmniMeshNode::connectToPeer(const QString& urlStr) {
    QUrl url(urlStr);
    QWebSocket* newPeer = new QWebSocket();
    
    connect(newPeer, &QWebSocket::connected, this, &OmniMeshNode::onConnectedToRemote);
    connect(newPeer, &QWebSocket::disconnected, this, &OmniMeshNode::peerSocketDisconnected);
    connect(newPeer, &QWebSocket::textMessageReceived, this, &OmniMeshNode::processPeerMessage);
    
    qDebug() << "OmniMeshNode: Attempting to connect to remote peer at" << urlStr;
    newPeer->open(url);
    return true; // Asynchronous
}

void OmniMeshNode::onConnectedToRemote() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_peers << pClient;
        QString address = pClient->peerAddress().toString();
        qDebug() << "OmniMeshNode: Successfully connected to remote peer ->" << address;
        emit peerConnected(address);
        emit activePeersChanged();
    }
}

void OmniMeshNode::onNewConnection() {
    QWebSocket *pSocket = m_server->nextPendingConnection();
    
    connect(pSocket, &QWebSocket::textMessageReceived, this, &OmniMeshNode::processPeerMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &OmniMeshNode::peerSocketDisconnected);

    m_peers << pSocket;
    QString address = pSocket->peerAddress().toString();
    qDebug() << "OmniMeshNode: Remote peer connected to us ->" << address;
    emit peerConnected(address);
    emit activePeersChanged();
}

void OmniMeshNode::processPeerMessage(const QString& message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isNull() && doc.isObject()) {
        handleRemotePayload(doc.object());
    }
}

void OmniMeshNode::handleRemotePayload(const QJsonObject& obj) {
    QString type = obj["type"].toString();
    QString deviceId = obj["deviceId"].toString();

    // Map remote devices uniquely so they don't clash with local devices
    QString netDeviceId = "net-" + deviceId;

    if (type == "cursor") {
        qreal x = obj["x"].toDouble();
        qreal y = obj["y"].toDouble();
        
        auto manager = OmniInputManager::instance();
        manager->registerDevice(netDeviceId, "Remote Peer Cursor", "mouse");
        manager->updateCursor(netDeviceId, QPointF(x, y));
    } 
    else if (type == "focus") {
        QString targetName = obj["target"].toString();
        // Since we don't have the explicit pointer over the network, 
        // a real implementation would search the QML DOM for the objectName.
        // For simulation purposes, we just log that the remote peer focused an object.
        qDebug() << "OmniMeshNode: Remote peer [" << netDeviceId << "] focused on [" << targetName << "]";
    }
}

void OmniMeshNode::peerSocketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_peers.removeAll(pClient);
        QString address = pClient->peerAddress().toString();
        qDebug() << "OmniMeshNode: Peer Disconnected ->" << address;
        pClient->deleteLater();
        emit peerDisconnected(address);
        emit activePeersChanged();
    }
}

void OmniMeshNode::broadcastJson(const QJsonObject& obj) {
    if (m_peers.isEmpty()) return;
    
    QJsonDocument doc(obj);
    QString payload = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    
    for (QWebSocket *pClient : std::as_const(m_peers)) {
        pClient->sendTextMessage(payload);
    }
}

void OmniMeshNode::broadcastCursor(const QString& deviceId, const QPointF& pos) {
    if (m_peers.isEmpty()) return;
    
    // Only broadcast local devices to the mesh
    if (deviceId.startsWith("sys-") || deviceId.startsWith("sim-")) {
        QJsonObject obj;
        obj["type"] = "cursor";
        obj["deviceId"] = deviceId;
        obj["x"] = pos.x();
        obj["y"] = pos.y();
        broadcastJson(obj);
    }
}

void OmniMeshNode::broadcastFocus(const QString& deviceId, const QString& targetObjectName) {
    if (m_peers.isEmpty() || targetObjectName.isEmpty() || targetObjectName == "unnamed") return;
    
    if (deviceId.startsWith("sys-") || deviceId.startsWith("sim-")) {
        QJsonObject obj;
        obj["type"] = "focus";
        obj["deviceId"] = deviceId;
        obj["target"] = targetObjectName;
        broadcastJson(obj);
    }
}
