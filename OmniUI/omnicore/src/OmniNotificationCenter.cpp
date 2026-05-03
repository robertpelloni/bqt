#include "OmniNotificationCenter.h"
#include <QUuid>
#include <QDebug>

OmniNotificationCenter* OmniNotificationCenter::instance() {
    static OmniNotificationCenter* center = new OmniNotificationCenter();
    return center;
}

OmniNotificationCenter::OmniNotificationCenter(QObject *parent) : QObject(parent) {
    setObjectName("OmniNotificationCenter");
}

OmniNotificationCenter::~OmniNotificationCenter() = default;

void OmniNotificationCenter::showToast(const QString& title, const QString& message, const QString& type, int durationMs) {
    OmniToast toast;
    toast.id = QUuid::createUuid().toString();
    toast.title = title;
    toast.message = message;
    toast.type = type;
    toast.timestamp = QDateTime::currentDateTime();
    toast.durationMs = durationMs;

    m_activeToasts.append(toast);
    
    qDebug() << "OmniNotificationCenter: Spawned [" << type.toUpper() << "] Toast ->" << title << "-" << message;
    emit toastSpawned(toast);
}

QList<OmniToast> OmniNotificationCenter::activeToasts() const {
    return m_activeToasts;
}

void OmniNotificationCenter::removeToast(const QString& id) {
    for (int i = 0; i < m_activeToasts.size(); ++i) {
        if (m_activeToasts[i].id == id) {
            m_activeToasts.removeAt(i);
            emit toastRemoved(id);
            break;
        }
    }
}
