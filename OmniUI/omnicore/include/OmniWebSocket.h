#ifndef OMNIWEBSOCKET_H
#define OMNIWEBSOCKET_H

#include <QObject>
#include <QString>
#include <QtWebSockets/QWebSocket>

class OmniWebSocket : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

public:
    explicit OmniWebSocket(QObject *parent = nullptr);
    virtual ~OmniWebSocket();

    bool isConnected() const;

    Q_INVOKABLE void connectToServer(const QString& url);
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendMessage(const QString& message);

signals:
    void isConnectedChanged();
    void messageReceived(const QString& message);
    void errorOccurred(const QString& errorString);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString& message);
    void onError(QAbstractSocket::SocketError error);

private:
    QWebSocket m_webSocket;
    bool m_isConnected;
};

#endif // OMNIWEBSOCKET_H
