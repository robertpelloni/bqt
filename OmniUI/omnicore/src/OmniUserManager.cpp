#include "OmniUserManager.h"
#include <QUuid>
#include <QDebug>

OmniUserManager* OmniUserManager::instance() {
    static OmniUserManager* manager = new OmniUserManager();
    return manager;
}

OmniUserManager::OmniUserManager(QObject *parent) : QObject(parent) {
    createUser("Admin", QColor("#FF0000"), false);
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

bool OmniUserManager::tryLockInteraction(const QString& deviceId, QObject* target) {
    if (!target) return false;

    // If already locked by someone else, fail
    if (m_locks.contains(target) && m_locks[target] != deviceId) {
        return false;
    }

    m_locks[target] = deviceId;
    emit interactionLocked(deviceId, target);
    return true;
}

void OmniUserManager::unlockInteraction(const QString& deviceId, QObject* target) {
    if (m_locks.value(target) == deviceId) {
        m_locks.remove(target);
        emit interactionUnlocked(deviceId, target);
    }
}

bool OmniUserManager::canInteract(const QString& deviceId, QObject* target) const {
    if (!target) return true;
    
    // If no one owns the lock, anyone can touch
    if (!m_locks.contains(target)) return true;
    
    // Otherwise, only the owner can touch
    return m_locks[target] == deviceId;
}

QString OmniUserManager::getUserIdForDevice(const QString& deviceId) const {
    return m_deviceToUserMap.value(deviceId, "");
}
