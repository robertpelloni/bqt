#ifndef OMNIINPUTMANAGER_H
#define OMNIINPUTMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPointF>
#include <QMap>
#include <QMutex>

struct OmniInputDevice {
    QString id;
    QString name;
    QString type; // "mouse", "keyboard", "touch", "gamepad"
    QPointF cursorPosition;
};

class QKeyEvent;

class OmniInputManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool devMode READ devMode WRITE setDevMode NOTIFY devModeChanged)

public:
    static OmniInputManager* instance();

    bool devMode() const;
    void setDevMode(bool enabled);

    QList<OmniInputDevice> devices() const;

    // Registers a new device (simulating raw input detection)
    void registerDevice(const QString& id, const QString& name, const QString& type);
    
    // Updates the state of a specific pointing device
    void updateCursor(const QString& deviceId, const QPointF& pos);

    // --- Multi-Focus & Hover Tree Management ---
    Q_INVOKABLE void setDeviceFocus(const QString& deviceId, QObject* target);
    Q_INVOKABLE QObject* deviceFocus(const QString& deviceId) const;

    Q_INVOKABLE void setDeviceHover(const QString& deviceId, QObject* target);
    Q_INVOKABLE QObject* deviceHover(const QString& deviceId) const;

    // Processes a key event and routes it to the specific device's focused widget
    bool routeKeyEvent(const QString& deviceId, QKeyEvent* event);

    // --- Developer Experience (DX) Simulation ---
    Q_INVOKABLE void simulateSecondaryCursorMove(qreal dx, qreal dy);
    Q_INVOKABLE void simulateSecondaryCursorClick(QObject* receiver);

signals:
    void devModeChanged();
    void deviceConnected(const OmniInputDevice& device);
    void deviceDisconnected(const QString& deviceId);
    void cursorUpdated(const QString& deviceId, const QPointF& pos);
    void focusChanged(const QString& deviceId, QObject* target);
    void hoverChanged(const QString& deviceId, QObject* target);

private:
    explicit OmniInputManager(QObject *parent = nullptr);
    QMap<QString, OmniInputDevice> m_devices;
    QMap<QString, QObject*> m_deviceFocusMap; 
    QMap<QString, QObject*> m_deviceHoverMap; 
    bool m_devMode;
    
    // Mutex to guarantee thread-safety when raw OS hooks and UI threads
    // concurrently update independent focus trees.
    mutable QMutex m_mutex;
};

#endif // OMNIINPUTMANAGER_H
