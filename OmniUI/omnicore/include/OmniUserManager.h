#ifndef OMNIUSERMANAGER_H
#define OMNIUSERMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QColor>
#include <QList>

struct OmniUser {
    QString id;
    QString name;
    QColor cursorColor;
    QStringList assignedDevices; // List of DeviceIds (hw-..., sys-..., net-...)
    bool isRemote;
};

// The OS-level Identity Manager. Standard OSs have 1 user. 
// BobUI supports an infinite mesh of local and remote users.
class OmniUserManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int userCount READ userCount NOTIFY usersChanged)

public:
    static OmniUserManager* instance();

    int userCount() const;

    // Registers a new logical user in the OS
    Q_INVOKABLE QString createUser(const QString& name, const QColor& cursorColor, bool isRemote = false);
    
    // Assigns a physical/virtual hardware device to a specific user
    Q_INVOKABLE bool assignDeviceToUser(const QString& deviceId, const QString& userId);

    // Checks if a specific device is allowed to interact with an object owned by another user
    Q_INVOKABLE bool checkPermission(const QString& deviceId, QObject* target);

    // Retrieves the user associated with a device
    QString getUserIdForDevice(const QString& deviceId) const;
    
    QList<OmniUser> activeUsers() const;

signals:
    void usersChanged();
    void userConnected(const QString& userId);
    void userDisconnected(const QString& userId);

private:
    explicit OmniUserManager(QObject *parent = nullptr);
    ~OmniUserManager() override;

    QMap<QString, OmniUser> m_users;
    QMap<QString, QString> m_deviceToUserMap; // DeviceId -> UserId
};

#endif // OMNIUSERMANAGER_H
