#ifndef OMNIUSERMANAGER_H
#define OMNIUSERMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QColor>
#include <QPointer>

struct OmniUser {
    QString id;
    QString name;
    QColor cursorColor;
    QStringList assignedDevices;
    bool isRemote;
};

// The Identity & Permissioning Kernel
class OmniUserManager : public QObject {
    Q_OBJECT
public:
    static OmniUserManager* instance();

    Q_INVOKABLE QString createUser(const QString& name, const QColor& cursorColor, bool isRemote = false);
    Q_INVOKABLE bool assignDeviceToUser(const QString& deviceId, const QString& userId);

    // --- INTERACTION LOCKING (Single User Tool Actions) ---
    // Attempts to lock an object to a specific user/device. Returns true if successful.
    Q_INVOKABLE bool tryLockInteraction(const QString& deviceId, QObject* target);
    
    // Releases the lock on an object
    Q_INVOKABLE void unlockInteraction(const QString& deviceId, QObject* target);

    // Checks if a specific device is currently allowed to interact with an object
    bool canInteract(const QString& deviceId, QObject* target) const;

    QString getUserIdForDevice(const QString& deviceId) const;

signals:
    void usersChanged();
    void interactionLocked(const QString& deviceId, QObject* target);
    void interactionUnlocked(const QString& deviceId, QObject* target);

private:
    explicit OmniUserManager(QObject *parent = nullptr);
    ~OmniUserManager() override;

    QMap<QString, OmniUser> m_users;
    QMap<QString, QString> m_deviceToUserMap;
    
    // Object Pointer -> DeviceID that owns the current interaction
    QMap<QObject*, QString> m_locks;
};

#endif // OMNIUSERMANAGER_H
