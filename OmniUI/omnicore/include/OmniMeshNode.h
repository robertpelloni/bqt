#ifndef OMNIMESHNODE_H
#define OMNIMESHNODE_H

#include <QObject>
#include <QString>
#include <QPointF>
#include <QList>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

class OmniMeshNode : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)
    Q_PROPERTY(int activePeers READ activePeers NOTIFY activePeersChanged)

public:
    static OmniMeshNode* instance();

    bool isConnected() const;
    int activePeers() const;

    // Start a P2P listening node on a specific port
    Q_INVOKABLE bool startNode(int port = 8081);
    Q_INVOKABLE void stopNode();

    // Connect to a remote peer's IP/Port
    Q_INVOKABLE bool connectToPeer(const QString& url);

    // Broadcasts the local user's cursor movement to all connected peers
    void broadcastCursor(const QString& deviceId, const QPointF& pos);
    
    // Broadcasts focus changes (clicks) to peers
    void broadcastFocus(const QString& deviceId, const QString& targetObjectName);

signals:
    void connectionStatusChanged();
    void activePeersChanged();
    void peerConnected(const QString& peerAddress);
    void peerDisconnected(const QString& peerAddress);

private slots:
    void onNewConnection();
    void processPeerMessage(const QString& message);
    void peerSocketDisconnected();
    void onConnectedToRemote();

private:
    explicit OmniMeshNode(QObject *parent = nullptr);
    ~OmniMeshNode() override;

    void broadcastJson(const QJsonObject& obj);
    void handleRemotePayload(const QJsonObject& obj);

    QWebSocketServer* m_server;
    QList<QWebSocket*> m_peers;
    bool m_isConnected;
};

#endif // OMNIMESHNODE_H
