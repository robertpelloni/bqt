#include "OmniWebSocketServer.h"
#include <QDebug>

OmniWebSocketServer* OmniWebSocketServer::instance() {
    static OmniWebSocketServer* server = new OmniWebSocketServer();
    return server;
}

OmniWebSocketServer::OmniWebSocketServer(QObject *parent)
    : QObject(parent),
      m_server(nullptr),
      m_isRunning(false),
      m_port(8080)
{
    setObjectName("OmniWebSocketServer");
    m_server = new QWebSocketServer(QStringLiteral("OmniUI OS Server"), QWebSocketServer::NonSecureMode, this);
}

OmniWebSocketServer::~OmniWebSocketServer() {
    stopServer();
}

bool OmniWebSocketServer::isRunning() const { return m_isRunning; }
int OmniWebSocketServer::port() const { return m_port; }

bool OmniWebSocketServer::startServer(int port) {
    if (m_isRunning) stopServer();

    m_port = port;
    if (m_server->listen(QHostAddress::Any, m_port)) {
        m_isRunning = true;
        connect(m_server, &QWebSocketServer::newConnection, this, &OmniWebSocketServer::onNewConnection);
        qDebug() << "OmniWebSocketServer: OS Local Host started on port" << m_port;
        emit isRunningChanged();
        emit portChanged();
        return true;
    } else {
        qCritical() << "OmniWebSocketServer: Failed to start server on port" << m_port << m_server->errorString();
        return false;
    }
}

void OmniWebSocketServer::stopServer() {
    if (m_isRunning) {
        m_server->close();
        qDeleteAll(m_clients.begin(), m_clients.end());
        m_clients.clear();
        m_isRunning = false;
        qDebug() << "OmniWebSocketServer: OS Local Host stopped.";
        emit isRunningChanged();
    }
}

void OmniWebSocketServer::onNewConnection() {
    QWebSocket *pSocket = m_server->nextPendingConnection();
    
    connect(pSocket, &QWebSocket::textMessageReceived, this, &OmniWebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &OmniWebSocketServer::socketDisconnected);

    m_clients << pSocket;
    QString address = pSocket->peerAddress().toString();
    qDebug() << "OmniWebSocketServer: Client Connected ->" << address;
    emit clientConnected(address);
}

void OmniWebSocketServer::processTextMessage(const QString& message) {
    // This allows Python LLM scripts to send JSON commands (like 'click Button_1') into the OS
    emit messageReceived(message);
}

void OmniWebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        QString address = pClient->peerAddress().toString();
        qDebug() << "OmniWebSocketServer: Client Disconnected ->" << address;
        pClient->deleteLater();
        emit clientDisconnected(address);
    }
}

void OmniWebSocketServer::broadcastMessage(const QString& message) {
    for (QWebSocket *pClient : std::as_const(m_clients)) {
        pClient->sendTextMessage(message);
    }
}
