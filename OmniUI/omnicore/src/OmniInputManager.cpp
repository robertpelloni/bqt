#include "OmniInputManager.h"
#include "OmniRustBridge.h"
#include "OmniMacroRecorder.h"
#include "OmniMeshNode.h"
#include <QKeyEvent>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QDebug>
#include <algorithm>

OmniInputManager* OmniInputManager::instance()
{
    static OmniInputManager* manager = new OmniInputManager();
    return manager;
}

OmniInputManager::OmniInputManager(QObject *parent)
    : QObject(parent),
      m_devMode(false)
{
    registerDevice("sys-mouse-0", "System Mouse", "mouse");
    registerDevice("sys-kb-0", "System Keyboard", "keyboard");
    registerDevice("sys-kb-1", "Secondary User Keyboard", "keyboard");
    
    registerDevice("sim-mouse-1", "Virtual DX Mouse", "mouse");
    updateCursor("sim-mouse-1", QPointF(400, 300));
}

bool OmniInputManager::devMode() const { 
    QMutexLocker locker(&m_mutex);
    return m_devMode; 
}

void OmniInputManager::setDevMode(bool enabled) {
    QMutexLocker locker(&m_mutex);
    if (m_devMode != enabled) {
        m_devMode = enabled;
        emit devModeChanged();
        qDebug() << "OmniInputManager: DevMode (Virtual Cursor Simulation) set to" << enabled;
    }
}

QList<OmniInputDevice> OmniInputManager::devices() const { 
    QMutexLocker locker(&m_mutex);
    return m_devices.values(); 
}

void OmniInputManager::registerDevice(const QString& id, const QString& name, const QString& type) {
    QMutexLocker locker(&m_mutex);
    if (m_devices.contains(id)) return;
    OmniInputDevice dev = { id, name, type, QPointF(0, 0) };
    m_devices.insert(id, dev);
    
    OmniRustBridge::registerDevice(id, name, type);

    emit deviceConnected(dev);
}

void OmniInputManager::updateCursor(const QString& deviceId, const QPointF& pos) {
    QMutexLocker locker(&m_mutex);
    if (!m_devices.contains(deviceId)) return;
    
    OmniRustBridge::updateCursor(deviceId, pos.x(), pos.y());

    m_devices[deviceId].cursorPosition = pos;
    emit cursorUpdated(deviceId, pos);

    // --- P2P MULTIPLAYER OS MESH BROADCAST ---
    OmniMeshNode::instance()->broadcastCursor(deviceId, pos);
}

void OmniInputManager::setDeviceFocus(const QString& deviceId, QObject* target) {
    QMutexLocker locker(&m_mutex);
    if (m_deviceFocusMap.value(deviceId) == target) return;
    m_deviceFocusMap[deviceId] = target;
    
    if (target && !target->objectName().isEmpty()) {
        OmniMacroRecorder::instance()->recordClick(target->objectName());
        
        // --- P2P MULTIPLAYER OS MESH BROADCAST ---
        OmniMeshNode::instance()->broadcastFocus(deviceId, target->objectName());
    }

    emit focusChanged(deviceId, target);
}

QObject* OmniInputManager::deviceFocus(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_deviceFocusMap.value(deviceId, nullptr);
}

void OmniInputManager::setDeviceHover(const QString& deviceId, QObject* target) {
    QMutexLocker locker(&m_mutex);
    if (m_deviceHoverMap.value(deviceId) == target) return;
    m_deviceHoverMap[deviceId] = target;
    emit hoverChanged(deviceId, target);
}

QObject* OmniInputManager::deviceHover(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_deviceHoverMap.value(deviceId, nullptr);
}

bool OmniInputManager::routeKeyEvent(const QString& deviceId, QKeyEvent* event) {
    QObject* target = nullptr;
    {
        QMutexLocker locker(&m_mutex);
        target = m_deviceFocusMap.value(deviceId, nullptr);
    }
    
    if (!target) return false;
    
    if (event->type() == QEvent::KeyPress && !event->text().isEmpty() && target && !target->objectName().isEmpty()) {
        OmniMacroRecorder::instance()->recordKey(target->objectName(), event->text());
    }

    QCoreApplication::sendEvent(target, event);
    return true;
}

void OmniInputManager::simulateSecondaryCursorMove(qreal dx, qreal dy) {
    QPointF current;
    bool enabled = false;
    {
        QMutexLocker locker(&m_mutex);
        enabled = m_devMode && m_devices.contains("sim-mouse-1");
        if (enabled) current = m_devices["sim-mouse-1"].cursorPosition;
    }
    
    if (!enabled) return;
    
    QPointF newPos(current.x() + dx, current.y() + dy);
    newPos.setX(std::clamp(newPos.x(), 0.0, 3840.0)); 
    newPos.setY(std::clamp(newPos.y(), 0.0, 2160.0));
    
    updateCursor("sim-mouse-1", newPos);
}

void OmniInputManager::simulateSecondaryCursorClick(QObject* receiver) {
    bool enabled = false;
    {
        QMutexLocker locker(&m_mutex);
        enabled = m_devMode;
    }
    
    if (!enabled) return;
    setDeviceFocus("sim-mouse-1", receiver);
    qDebug() << "OmniInputManager: Virtual DX Mouse clicked and focused on" << receiver;
}
