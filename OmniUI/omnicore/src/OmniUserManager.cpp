#include "OmniUserManager.h"
#include <QUuid>
#include <QDebug>

OmniUserManager* OmniUserManager::instance() {
    static OmniUserManager* manager = new OmniUserManager();
    return manager;
}

OmniUserManager::OmniUserManager(QObject *parent) : QObject(parent) {
    setObjectName("OmniUserManager");
    
    // Create the default Admin User
    createUser("Administrator", QColor("#FF0000"), false);
}

OmniUserManager::~OmniUserManager() = default;

int OmniUserManager::userCount() const { return m_users.size(); }

QString OmniUserManager::createUser(const QString& name, const QColor& cursorColor, bool isRemote) {
    QString id = QUuid::createUuid().toString();
    OmniUser user;
    user.id = id;
    user.name = name;
    user.cursorColor = cursorColor;
    user.isRemote = isRemote;
    
    m_users.insert(id, user);
    qDebug() << "OmniUserManager: Created User [" << name << "] with ID:" << id;
    
    emit usersChanged();
    emit userConnected(id);
    return id;
}

bool OmniUserManager::assignDeviceToUser(const QString& deviceId, const QString& userId) {
    if (!m_users.contains(userId)) return false;
    
    m_deviceToUserMap[deviceId] = userId;
    m_users[userId].assignedDevices.append(deviceId);
    
    qDebug() << "OmniUserManager: Hardware Device [" << deviceId << "] assigned to User [" << m_users[userId].name << "]";
    return true;
}

QString OmniUserManager::getUserIdForDevice(const QString& deviceId) const {
    return m_deviceToUserMap.value(deviceId, "");
}

bool OmniUserManager::checkPermission(const QString& deviceId, QObject* target) {
    // In a future phase, widgets will have an 'ownerId' property.
    // For now, everyone has access to everything (God Mode).
    Q_UNUSED(deviceId);
    Q_UNUSED(target);
    return true;
}

QList<OmniUser> OmniUserManager::activeUsers() const {
    return m_users.values();
}
