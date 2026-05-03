#ifndef OMNIWEBSOCKETSERVER_H
#define OMNIWEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QList>
#include <BobUIWebSockets/QWebSocketServer>
#include <BobUIWebSockets/QWebSocket>

class OmniWebSocketServer : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(int port READ port NOTIFY portChanged)

public:
    static OmniWebSocketServer* instance();

    bool isRunning() const;
    int port() const;

    // Starts the local OS WebSocket Server for IPC/LLM connections
    Q_INVOKABLE bool startServer(int port = 8080);
    Q_INVOKABLE void stopServer();

    // Broadcasts a string payload to all connected clients (e.g. the OmniNeural JSON DOM)
    Q_INVOKABLE void broadcastMessage(const QString& message);

signals:
    void isRunningChanged();
    void portChanged();
    void clientConnected(const QString& clientAddress);
    void clientDisconnected(const QString& clientAddress);
    
    // Fired when an external Python script/LLM sends a command into the OS
    void messageReceived(const QString& message);

private slots:
    void onNewConnection();
    void processTextMessage(const QString& message);
    void socketDisconnected();

private:
    explicit OmniWebSocketServer(QObject *parent = nullptr);
    ~OmniWebSocketServer() override;

    QWebSocketServer* m_server;
    QList<QWebSocket*> m_clients;
    bool m_isRunning;
    int m_port;
};

#endif // OMNIWEBSOCKETSERVER_H
