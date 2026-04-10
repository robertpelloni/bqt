#include "OmniWebSocket.h"
#include <QDebug>

OmniWebSocket::OmniWebSocket(QObject *parent)
    : QObject(parent),
      m_isConnected(false)
{
    setObjectName("OmniWebSocket");
    
    connect(&m_webSocket, &QWebSocket::connected, this, &OmniWebSocket::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &OmniWebSocket::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &OmniWebSocket::onTextMessageReceived);
    // Connect error securely avoiding overloaded signal ambiguity in BobUI 5/6
    connect(&m_webSocket, &QWebSocket::errorOccurred, this, &OmniWebSocket::onError);
}

OmniWebSocket::~OmniWebSocket() {
    disconnectFromServer();
}

bool OmniWebSocket::isConnected() const {
    return m_isConnected;
}

void OmniWebSocket::connectToServer(const QString& urlStr) {
    QUrl url(urlStr);
    qDebug() << "OmniWebSocket: Connecting to" << urlStr;
    m_webSocket.open(url);
}

void OmniWebSocket::disconnectFromServer() {
    if (m_webSocket.isValid()) {
        m_webSocket.close();
    }
}

void OmniWebSocket::sendMessage(const QString& message) {
    if (m_isConnected) {
        m_webSocket.sendTextMessage(message);
    } else {
        qWarning() << "OmniWebSocket: Cannot send message, not connected.";
    }
}

void OmniWebSocket::onConnected() {
    qDebug() << "OmniWebSocket: Connected!";
    m_isConnected = true;
    emit isConnectedChanged();
}

void OmniWebSocket::onDisconnected() {
    qDebug() << "OmniWebSocket: Disconnected.";
    m_isConnected = false;
    emit isConnectedChanged();
}

void OmniWebSocket::onTextMessageReceived(const QString& message) {
    emit messageReceived(message);
}

void OmniWebSocket::onError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error);
    QString errStr = m_webSocket.errorString();
    qWarning() << "OmniWebSocket Error:" << errStr;
    emit errorOccurred(errStr);
}
