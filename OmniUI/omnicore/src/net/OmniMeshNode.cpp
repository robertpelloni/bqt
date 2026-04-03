#include "OmniMeshNode.h"
#include "OmniInputManager.h"
#include "OmniClipboard.h"
#include "OmniFileMesh.h"
#include "OmniValueTree.h"
#include <QDebug>

OmniMeshNode* OmniMeshNode::instance() {
    static OmniMeshNode* node = new OmniMeshNode();
    return node;
}

OmniMeshNode::OmniMeshNode(QObject *parent)
    : QObject(parent), m_server(nullptr), m_isConnected(false)
{
    setObjectName("OmniMeshNode");
    m_server = new QWebSocketServer(QStringLiteral("OmniMesh P2P Node"), QWebSocketServer::NonSecureMode, this);
}

OmniMeshNode::~OmniMeshNode() { stopNode(); }

bool OmniMeshNode::isConnected() const { return m_isConnected; }
int OmniMeshNode::activePeers() const { return m_peers.size(); }

bool OmniMeshNode::startNode(int port) {
    if (m_isConnected) stopNode();
    if (m_server->listen(QHostAddress::Any, port)) {
        m_isConnected = true;
        connect(m_server, &QWebSocketServer::newConnection, this, &OmniMeshNode::onNewConnection);
        return true;
    }
    return false;
}

void OmniMeshNode::stopNode() {
    m_server->close();
    for (auto* peer : m_peers) { peer->close(); peer->deleteLater(); }
    m_peers.clear(); m_isConnected = false;
}

bool OmniMeshNode::connectToPeer(const QString& urlStr) {
    QWebSocket* newPeer = new QWebSocket();
    connect(newPeer, &QWebSocket::connected, this, &OmniMeshNode::onConnectedToRemote);
    connect(newPeer, &QWebSocket::disconnected, this, &OmniMeshNode::peerSocketDisconnected);
    connect(newPeer, &QWebSocket::textMessageReceived, this, &OmniMeshNode::processPeerMessage);
    newPeer->open(QUrl(urlStr));
    return true;
}

void OmniMeshNode::onConnectedToRemote() { QWebSocket *pClient = qobject_cast<QWebSocket *>(sender()); if (pClient) { m_peers << pClient; emit activePeersChanged(); } }
void OmniMeshNode::onNewConnection() { QWebSocket *pSocket = m_server->nextPendingConnection(); connect(pSocket, &QWebSocket::textMessageReceived, this, &OmniMeshNode::processPeerMessage); connect(pSocket, &QWebSocket::disconnected, this, &OmniMeshNode::peerSocketDisconnected); m_peers << pSocket; emit activePeersChanged(); }

void OmniMeshNode::processPeerMessage(const QString& message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        QString type = obj["type"].toString();

        if (type == "cursor") {
            OmniInputManager::instance()->registerDevice("net-"+obj["deviceId"].toString(), "Remote Cursor", "mouse");
            OmniInputManager::instance()->updateCursor("net-"+obj["deviceId"].toString(), QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
        } 
        else if (type == "clipboard_sync") {
            OmniClipboard::instance()->receiveRemoteText(obj["content"].toString());
        }
        else if (type == "file_sync") {
            QByteArray data = QByteArray::fromBase64(obj["content"].toString().toUtf8());
            OmniFileMesh::instance()->receiveRemoteFile(obj["path"].toString(), data);
        }
        else if (type == "param_sync") {
            // --- GLOBAL SYSTEM STATE SYNC ---
            OmniValueTree::instance()->setParameter(obj["key"].toString(), obj["value"].toVariant(), false);
        }
    }
}

void OmniMeshNode::peerSocketDisconnected() { QWebSocket *pClient = qobject_cast<QWebSocket *>(sender()); if (pClient) { m_peers.removeAll(pClient); pClient->deleteLater(); emit activePeersChanged(); } }
void OmniMeshNode::broadcastPayload(const QJsonObject& obj) { if (m_peers.isEmpty()) return; QString payload = QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Compact)); for (QWebSocket *pClient : std::as_const(m_peers)) { pClient->sendTextMessage(payload); } }
void OmniMeshNode::broadcastCursor(const QString& deviceId, const QPointF& pos) { if (m_peers.isEmpty() || deviceId.startsWith("net-")) return; QJsonObject obj; obj["type"] = "cursor"; obj["deviceId"] = deviceId; obj["x"] = pos.x(); obj["y"] = pos.y(); broadcastPayload(obj); }
void OmniMeshNode::broadcastFocus(const QString& deviceId, const QString& target) { if (m_peers.isEmpty() || deviceId.startsWith("net-")) return; QJsonObject obj; obj["type"] = "focus"; obj["deviceId"] = deviceId; obj["target"] = target; broadcastPayload(obj); }
