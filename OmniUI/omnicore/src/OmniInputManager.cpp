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
    : QObject(parent),
      m_devMode(false)
{
    // Simulation: Register initial system devices
    registerDevice("sys-mouse-0", "System Mouse", "mouse");
    registerDevice("sys-kb-0", "System Keyboard", "keyboard");
    registerDevice("sys-kb-1", "Secondary User Keyboard", "keyboard");
    
    // Virtual DX Device
    registerDevice("sim-mouse-1", "Virtual DX Mouse", "mouse");
    updateCursor("sim-mouse-1", QPointF(400, 300)); // Start in middle of screen
}

bool OmniInputManager::devMode() const { return m_devMode; }
void OmniInputManager::setDevMode(bool enabled) {
    if (m_devMode != enabled) {
        m_devMode = enabled;
        emit devModeChanged();
        qDebug() << "OmniInputManager: DevMode (Virtual Cursor Simulation) set to" << enabled;
    }
}

QList<OmniInputDevice> OmniInputManager::devices() const { return m_devices.values(); }

void OmniInputManager::registerDevice(const QString& id, const QString& name, const QString& type) {
    if (m_devices.contains(id)) return;
    OmniInputDevice dev = { id, name, type, QPointF(0, 0) };
    m_devices.insert(id, dev);
    emit deviceConnected(dev);
}

void OmniInputManager::updateCursor(const QString& deviceId, const QPointF& pos) {
    if (!m_devices.contains(deviceId)) return;
    m_devices[deviceId].cursorPosition = pos;
    emit cursorUpdated(deviceId, pos);
}

void OmniInputManager::setDeviceFocus(const QString& deviceId, QObject* target) {
    if (m_deviceFocusMap.value(deviceId) == target) return;
    m_deviceFocusMap[deviceId] = target;
    emit focusChanged(deviceId, target);
}

QObject* OmniInputManager::deviceFocus(const QString& deviceId) const {
    return m_deviceFocusMap.value(deviceId, nullptr);
}

void OmniInputManager::setDeviceHover(const QString& deviceId, QObject* target) {
    if (m_deviceHoverMap.value(deviceId) == target) return;
    m_deviceHoverMap[deviceId] = target;
    emit hoverChanged(deviceId, target);
}

QObject* OmniInputManager::deviceHover(const QString& deviceId) const {
    return m_deviceHoverMap.value(deviceId, nullptr);
}

bool OmniInputManager::routeKeyEvent(const QString& deviceId, QKeyEvent* event) {
    QObject* target = deviceFocus(deviceId);
    if (!target) return false;
    QCoreApplication::sendEvent(target, event);
    return true;
}

// --- Developer Experience (DX) Simulation ---
void OmniInputManager::simulateSecondaryCursorMove(qreal dx, qreal dy) {
    if (!m_devMode || !m_devices.contains("sim-mouse-1")) return;
    
    QPointF current = m_devices["sim-mouse-1"].cursorPosition;
    QPointF newPos(current.x() + dx, current.y() + dy);
    
    // Basic bounds checking (assuming 1920x1080 screen for simulation)
    newPos.setX(std::clamp(newPos.x(), 0.0, 1920.0));
    newPos.setY(std::clamp(newPos.y(), 0.0, 1080.0));
    
    updateCursor("sim-mouse-1", newPos);
}

void OmniInputManager::simulateSecondaryCursorClick(QObject* receiver) {
    if (!m_devMode) return;
    setDeviceFocus("sim-mouse-1", receiver);
    qDebug() << "OmniInputManager: Virtual DX Mouse clicked and focused on" << receiver;
}
