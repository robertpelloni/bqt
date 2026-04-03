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

class OmniUserManager : public QObject {
    Q_OBJECT
public:
    static OmniUserManager* instance();

    Q_INVOKABLE QString createUser(const QString& name, const QColor& cursorColor, bool isRemote = false);
    Q_INVOKABLE bool assignDeviceToUser(const QString& deviceId, const QString& userId);

    // --- TOOL ACTION ISOLATION ---
    // Attempts to lock an object to a specific user. Returns true if the user now owns the interaction.
    Q_INVOKABLE bool tryGrab(const QString& deviceId, QObject* target);
    
    // Releases the interaction lock
    Q_INVOKABLE void releaseGrab(const QString& deviceId, QObject* target);

    // Checks if a device is currently allowed to interact with an object
    bool canInteract(const QString& deviceId, QObject* target) const;

    QString getUserIdForDevice(const QString& deviceId) const;

signals:
    void usersChanged();
    void grabAcquired(const QString& userId, QObject* target);
    void grabReleased(const QString& userId, QObject* target);

private:
    explicit OmniUserManager(QObject *parent = nullptr);
    ~OmniUserManager() override;

    QMap<QString, OmniUser> m_users;
    QMap<QString, QString> m_deviceToUserMap;
    
    // Interaction Locks: Object Pointer -> UserID
    QMap<QObject*, QString> m_interactionLocks;
};

#endif // OMNIUSERMANAGER_H
