#include "OmniInputManager.h"
#include <QKeyEvent>
#include <QCoreApplication>
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
    registerDevice("sys-kb-1", "Secondary User Keyboard", "keyboard");
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

void OmniInputManager::setDeviceFocus(const QString& deviceId, QObject* target)
{
    if (m_deviceFocusMap.value(deviceId) == target) return;

    m_deviceFocusMap[deviceId] = target;
    qDebug() << "OmniInputManager: Device [" << deviceId << "] focus changed to" << target;
    emit focusChanged(deviceId, target);
}

QObject* OmniInputManager::deviceFocus(const QString& deviceId) const
{
    return m_deviceFocusMap.value(deviceId, nullptr);
}

void OmniInputManager::setDeviceHover(const QString& deviceId, QObject* target)
{
    if (m_deviceHoverMap.value(deviceId) == target) return;

    m_deviceHoverMap[deviceId] = target;
    emit hoverChanged(deviceId, target);
}

QObject* OmniInputManager::deviceHover(const QString& deviceId) const
{
    return m_deviceHoverMap.value(deviceId, nullptr);
}

bool OmniInputManager::routeKeyEvent(const QString& deviceId, QKeyEvent* event)
{
    QObject* target = deviceFocus(deviceId);
    if (!target) return false;

    qDebug() << "OmniInputManager: Routing Key" << event->key() << "specifically to device [" << deviceId << "] focus target.";
    QCoreApplication::sendEvent(target, event);
    return true;
}
