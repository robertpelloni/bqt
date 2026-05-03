#include "OmniValueTree.h"
#include "OmniMeshNode.h"
#include <QMutexLocker>
#include <QJsonObject>
#include <QDebug>

OmniValueTree* OmniValueTree::instance() {
    static OmniValueTree* tree = new OmniValueTree();
    return tree;
}

OmniValueTree::OmniValueTree(QObject *parent) : QObject(parent) {
    setObjectName("OmniValueTree");
}

OmniValueTree::~OmniValueTree() = default;

void OmniValueTree::setParameter(const QString& key, const QVariant& value, bool broadcast) {
    {
        QMutexLocker locker(&m_mutex);
        if (m_parameters.value(key) == value) return;
        m_parameters[key] = value;
    }

    qDebug() << "OmniValueTree: Param [" << key << "] updated to" << value;
    emit parameterChanged(key, value);

    if (broadcast) {
        // --- MULTIPLAYER SYSTEM SYNC ---
        QJsonObject obj;
        obj["type"] = "param_sync";
        obj["key"] = key;
        obj["value"] = QJsonValue::fromVariant(value);
        OmniMeshNode::instance()->broadcastPayload(obj);
    }
}

QVariant OmniValueTree::getParameter(const QString& key, const QVariant& fallback) const {
    QMutexLocker locker(&m_mutex);
    return m_parameters.value(key, fallback);
}

void OmniValueTree::bindProperty(QObject* target, const QString& propertyName, const QString& key) {
    if (!target) return;
    
    // Initial sync
    target->setProperty(propertyName.toUtf8(), getParameter(key));

    // Reactive listener
    connect(this, &OmniValueTree::parameterChanged, target, [target, propertyName, key](const QString& k, const QVariant& v) {
        if (k == key) {
            target->setProperty(propertyName.toUtf8(), v);
        }
    });
}
