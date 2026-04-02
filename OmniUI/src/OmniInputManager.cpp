#include "OmniInputManager.h"
#include <QDebug>

OmniInputManager* OmniInputManager::instance()
{
    static OmniInputManager* manager = new OmniInputManager();
    return manager;
}

OmniInputManager::OmniInputManager(QObject *parent)
    : QObject(parent)
{
    // Simulation: Register initial system devices
    registerDevice("sys-mouse-0", "System Mouse", "mouse");
    registerDevice("sys-kb-0", "System Keyboard", "keyboard");
}

QList<OmniInputDevice> OmniInputManager::devices() const
{
    return m_devices.values();
}

void OmniInputManager::registerDevice(const QString& id, const QString& name, const QString& type)
{
    if (m_devices.contains(id)) return;

    OmniInputDevice dev = { id, name, type, QPointF(0, 0) };
    m_devices.insert(id, dev);
    
    qDebug() << "OmniInputManager: Device Registered -" << name << "(" << id << ")";
    emit deviceConnected(dev);
}

void OmniInputManager::updateCursor(const QString& deviceId, const QPointF& pos)
{
    if (!m_devices.contains(deviceId)) return;

    m_devices[deviceId].cursorPosition = pos;
    emit cursorUpdated(deviceId, pos);
}