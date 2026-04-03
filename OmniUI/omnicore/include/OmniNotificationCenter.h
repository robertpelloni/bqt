#ifndef OMNINOTIFICATIONCENTER_H
#define OMNINOTIFICATIONCENTER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>

struct OmniToast {
    QString id;
    QString title;
    QString message;
    QString type; // "info", "warning", "error", "success"
    QDateTime timestamp;
    int durationMs;
};

class OmniNotificationCenter : public QObject {
    Q_OBJECT

public:
    static OmniNotificationCenter* instance();

    // Invoked by QML or C++ to spawn a new OS-level toast notification
    Q_INVOKABLE void showToast(const QString& title, const QString& message, const QString& type = "info", int durationMs = 3000);

    // Returns all active toasts for rendering loops
    QList<OmniToast> activeToasts() const;

    // Removes a toast (called automatically by the render loop when duration expires)
    void removeToast(const QString& id);

signals:
    void toastSpawned(const OmniToast& toast);
    void toastRemoved(const QString& id);

private:
    explicit OmniNotificationCenter(QObject *parent = nullptr);
    ~OmniNotificationCenter() override;

    QList<OmniToast> m_activeToasts;
};

#endif // OMNINOTIFICATIONCENTER_H
