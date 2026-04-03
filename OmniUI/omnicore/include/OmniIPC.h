#ifndef OMNIIPC_H
#define OMNIIPC_H

#include <QObject>
#include <QString>
#include <QSharedMemory>

class OmniIPC : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString lastMessage READ lastMessage NOTIFY messageReceived)

public:
    explicit OmniIPC(QObject *parent = nullptr);
    virtual ~OmniIPC();

    bool isConnected() const;
    QString lastMessage() const;

    // Creates or attaches to a named shared memory segment (e.g. "OmniNexus_Bus_01")
    Q_INVOKABLE bool connectToBus(const QString& busName);
    
    // Disconnects from the shared memory segment
    Q_INVOKABLE void disconnectFromBus();

    // Broadcasts a string payload instantly to all attached processes
    Q_INVOKABLE bool broadcast(const QString& message);
    
    // Reads the current payload from the shared memory block
    Q_INVOKABLE QString readBroadcast();

signals:
    void isConnectedChanged();
    void messageReceived(const QString& message);

private:
    QSharedMemory m_sharedMemory;
    QString m_lastMessage;
};

#endif // OMNIIPC_H
