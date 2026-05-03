#include "OmniIPC.h"
#include <QBuffer>
#include <QDataStream>
#include <QDebug>

OmniIPC::OmniIPC(QObject *parent)
    : QObject(parent),
      m_sharedMemory(this)
{
    setObjectName("OmniIPC");
}

OmniIPC::~OmniIPC() {
    disconnectFromBus();
}

bool OmniIPC::isConnected() const {
    return m_sharedMemory.isAttached();
}

QString OmniIPC::lastMessage() const {
    return m_lastMessage;
}

bool OmniIPC::connectToBus(const QString& busName) {
    if (m_sharedMemory.isAttached()) {
        disconnectFromBus();
    }

    m_sharedMemory.setKey(busName);
    
    // Try to attach if it already exists
    if (m_sharedMemory.attach()) {
        qDebug() << "OmniIPC: Attached to existing bus:" << busName;
        emit isConnectedChanged();
        return true;
    }
    
    // If not, create it (1MB buffer)
    if (m_sharedMemory.create(1024 * 1024)) {
        qDebug() << "OmniIPC: Created and attached to new bus:" << busName;
        emit isConnectedChanged();
        return true;
    }

    qWarning() << "OmniIPC: Failed to attach or create bus:" << busName << "Error:" << m_sharedMemory.errorString();
    return false;
}

void OmniIPC::disconnectFromBus() {
    if (m_sharedMemory.isAttached()) {
        m_sharedMemory.detach();
        emit isConnectedChanged();
        qDebug() << "OmniIPC: Detached from bus.";
    }
}

bool OmniIPC::broadcast(const QString& message) {
    if (!m_sharedMemory.isAttached()) return false;

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << message;
    int size = buffer.size();

    // Lock the segment, copy data, unlock
    m_sharedMemory.lock();
    char *to = (char*)m_sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(m_sharedMemory.size(), size));
    m_sharedMemory.unlock();

    qDebug() << "OmniIPC: Broadcasted payload to bus.";
    return true;
}

QString OmniIPC::readBroadcast() {
    if (!m_sharedMemory.isAttached()) return QString();

    QBuffer buffer;
    QDataStream in(&buffer);
    QString message;

    // Lock the segment, read data, unlock
    m_sharedMemory.lock();
    buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> message;
    m_sharedMemory.unlock();

    if (m_lastMessage != message) {
        m_lastMessage = message;
        emit messageReceived(m_lastMessage);
    }

    return message;
}
