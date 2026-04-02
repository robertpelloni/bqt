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

class QKeyEvent;

class OmniInputManager : public QObject {
    Q_OBJECT
public:
    static OmniInputManager* instance();

    QList<OmniInputDevice> devices() const;

    // Registers a new device (simulating raw input detection)
    void registerDevice(const QString& id, const QString& name, const QString& type);
    
    // Updates the state of a specific pointing device
    void updateCursor(const QString& deviceId, const QPointF& pos);

    // --- Multi-Focus & Hover Tree Management ---
    // Sets the focused widget for a specific device
    Q_INVOKABLE void setDeviceFocus(const QString& deviceId, QObject* target);
    Q_INVOKABLE QObject* deviceFocus(const QString& deviceId) const;

    // Sets the hovered widget for a specific device (used for Developer Introspection)
    Q_INVOKABLE void setDeviceHover(const QString& deviceId, QObject* target);
    Q_INVOKABLE QObject* deviceHover(const QString& deviceId) const;

    // Processes a key event and routes it to the specific device's focused widget
    bool routeKeyEvent(const QString& deviceId, QKeyEvent* event);

signals:
    void deviceConnected(const OmniInputDevice& device);
    void deviceDisconnected(const QString& deviceId);
    void cursorUpdated(const QString& deviceId, const QPointF& pos);
    void focusChanged(const QString& deviceId, QObject* target);
    void hoverChanged(const QString& deviceId, QObject* target);

private:
    explicit OmniInputManager(QObject *parent = nullptr);
    QMap<QString, OmniInputDevice> m_devices;
    QMap<QString, QObject*> m_deviceFocusMap; // Maps DeviceId -> Focused QObject
    QMap<QString, QObject*> m_deviceHoverMap; // Maps DeviceId -> Hovered QObject
};

#endif // OMNIINPUTMANAGER_H
