#ifndef OMNIINPUTMANAGER_H
#define OMNIINPUTMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPointF>
#include <QMap>

struct OmniInputDevice {
    QString id;
    QString name;
    QString type; // "mouse", "keyboard", "touch", "gamepad"
    QPointF cursorPosition;
};

class OmniInputManager : public QObject {
    Q_OBJECT
public:
    static OmniInputManager* instance();

    QList<OmniInputDevice> devices() const;

    // Registers a new device (simulating raw input detection)
    void registerDevice(const QString& id, const QString& name, const QString& type);
    
    // Updates the state of a specific device
    void updateCursor(const QString& deviceId, const QPointF& pos);

signals:
    void deviceConnected(const OmniInputDevice& device);
    void deviceDisconnected(const QString& deviceId);
    void cursorUpdated(const QString& deviceId, const QPointF& pos);

private:
    explicit OmniInputManager(QObject *parent = nullptr);
    QMap<QString, OmniInputDevice> m_devices;
};

#endif // OMNIINPUTMANAGER_H