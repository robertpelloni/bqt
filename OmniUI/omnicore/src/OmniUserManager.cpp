#include "OmniUserManager.h"
#include <QUuid>
#include <QDebug>

OmniUserManager* OmniUserManager::instance() {
    static OmniUserManager* manager = new OmniUserManager();
    return manager;
}

OmniUserManager::OmniUserManager(QObject *parent) : QObject(parent) {
    createUser("Administrator", QColor("#FF0000"), false);
}

OmniUserManager::~OmniUserManager() = default;

QString OmniUserManager::createUser(const QString& name, const QColor& cursorColor, bool isRemote) {
    QString id = QUuid::createUuid().toString();
    OmniUser user{id, name, cursorColor, {}, isRemote};
    m_users.insert(id, user);
    emit usersChanged();
    return id;
}

bool OmniUserManager::assignDeviceToUser(const QString& deviceId, const QString& userId) {
    if (!m_users.contains(userId)) return false;
    m_deviceToUserMap[deviceId] = userId;
    return true;
}

bool OmniUserManager::tryGrab(const QString& deviceId, QObject* target) {
    if (!target) return false;
    QString userId = m_deviceToUserMap.value(deviceId, "");

    // If the object is already locked by someone else, deny grab
    if (m_interactionLocks.contains(target) && m_interactionLocks[target] != userId) {
        return false;
    }

    m_interactionLocks[target] = userId;
    emit grabAcquired(userId, target);
    return true;
}

void OmniUserManager::releaseGrab(const QString& deviceId, QObject* target) {
    if (!target) return;
    QString userId = m_deviceToUserMap.value(deviceId, "");
    if (m_interactionLocks.value(target) == userId) {
        m_interactionLocks.remove(target);
        emit grabReleased(userId, target);
    }
}

bool OmniUserManager::canInteract(const QString& deviceId, QObject* target) const {
    if (!target) return true;
    QString userId = m_deviceToUserMap.value(deviceId, "");
    
    // If no one has a grab, anyone can interact
    if (!m_interactionLocks.contains(target)) return true;
    
    // Otherwise, only the grab owner can interact
    return m_interactionLocks[target] == userId;
}

QString OmniUserManager::getUserIdForDevice(const QString& deviceId) const {
    return m_deviceToUserMap.value(deviceId, "");
}
