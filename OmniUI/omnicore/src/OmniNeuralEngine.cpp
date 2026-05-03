#include "OmniNeuralEngine.h"
#include <QQuickItem>
#include <QJsonDocument>
#include <QMetaObject>
#include <QMetaProperty>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

OmniNeuralEngine* OmniNeuralEngine::instance() {
    static OmniNeuralEngine* engine = new OmniNeuralEngine();
    return engine;
}

OmniNeuralEngine::OmniNeuralEngine(QObject *parent)
    : QObject(parent),
      m_rootItem(nullptr),
      m_isStreaming(false),
      m_streamFramerate(10) // 10 FPS is usually enough for an LLM to "see" a UI without overwhelming IPC
{
    setObjectName("OmniNeuralEngine");
    connect(&m_streamTimer, &BOBUIimer::timeout, this, &OmniNeuralEngine::processStreamFrame);
}

OmniNeuralEngine::~OmniNeuralEngine() {
    setIsStreaming(false);
}

bool OmniNeuralEngine::isStreaming() const { return m_isStreaming; }
void OmniNeuralEngine::setIsStreaming(bool streaming) {
    if (m_isStreaming != streaming) {
        m_isStreaming = streaming;
        if (m_isStreaming && m_rootItem) {
            int interval = 1000 / m_streamFramerate;
            m_streamTimer.start(interval);
            qDebug() << "OmniNeural: LLM DOM Streaming STARTED at" << m_streamFramerate << "FPS.";
        } else {
            m_streamTimer.stop();
            qDebug() << "OmniNeural: LLM DOM Streaming STOPPED.";
        }
        emit isStreamingChanged();
    }
}

int OmniNeuralEngine::streamFramerate() const { return m_streamFramerate; }
void OmniNeuralEngine::setStreamFramerate(int fps) {
    if (fps > 0 && m_streamFramerate != fps) {
        m_streamFramerate = fps;
        if (m_isStreaming) {
            m_streamTimer.setInterval(1000 / m_streamFramerate);
        }
        emit streamFramerateChanged();
    }
}

void OmniNeuralEngine::setRootItem(QQuickItem* root) {
    m_rootItem = root;
    qDebug() << "OmniNeural: Root QQuickItem bound. Ready for DOM extraction.";
}

QString OmniNeuralEngine::captureVisualDOM() const {
    if (!m_rootItem) return "{\"error\": \"No root item bound.\"}";

    QJsonObject rootObj = serializeNode(m_rootItem);
    QJsonDocument doc(rootObj);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

void OmniNeuralEngine::processStreamFrame() {
    if (!m_rootItem) return;
    QString payload = captureVisualDOM();
    emit domSnapshotGenerated(payload);
}

QJsonObject OmniNeuralEngine::serializeNode(QQuickItem* item) const {
    QJsonObject obj;
    if (!item) return obj;

    const QMetaObject* meta = item->metaObject();
    obj["class"] = meta->className();
    obj["name"] = item->objectName().isEmpty() ? "unnamed" : item->objectName();
    
    // Geometry
    QJsonObject rect;
    QRectF bounds = item->boundingRect();
    QPointF globalPos = item->mapToGlobal(QPointF(0,0));
    rect["x"] = globalPos.x();
    rect["y"] = globalPos.y();
    rect["w"] = bounds.width();
    rect["h"] = bounds.height();
    obj["rect"] = rect;

    // Visibility & Z-Index
    obj["visible"] = item->isVisible();
    obj["z"] = item->z();

    // Extract critical Q_PROPERTY data dynamically (Text, Checked states, Values)
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);
        QString pName = prop.name();
        if (pName == "text" || pName == "value" || pName == "checked" || pName == "title") {
            QVariant val = prop.read(item);
            if (val.typeId() == QMetaType::QString) obj[pName] = val.toString();
            else if (val.typeId() == QMetaType::Bool) obj[pName] = val.toBool();
            else if (val.typeId() == QMetaType::Double || val.typeId() == QMetaType::Int) obj[pName] = val.toDouble();
        }
    }

    // Recurse Children
    const auto childItems = item->childItems();
    if (!childItems.isEmpty()) {
        QJsonArray childrenArr;
        for (QQuickItem* child : childItems) {
            // Only serialize visible UI components to save LLM token context
            if (child->isVisible()) {
                childrenArr.append(serializeNode(child));
            }
        }
        obj["children"] = childrenArr;
    }

    return obj;
}

// AI Action Execution
bool OmniNeuralEngine::executeAction(const QString& targetObjectName, const QString& action, const QString& payload) {
    if (!m_rootItem || targetObjectName.isEmpty()) return false;

    // Search for the target item by objectName recursively
    QQuickItem* target = m_rootItem->findChild<QQuickItem*>(targetObjectName, BobUI::FindChildrenRecursively);
    if (!target) {
        qWarning() << "OmniNeural: Action failed. Could not find target:" << targetObjectName;
        return false;
    }

    qDebug() << "OmniNeural: Executing AI Action [" << action << "] on [" << targetObjectName << "] with payload [" << payload << "]";

    if (action == "click") {
        // Simulate a native left-click at the center of the widget bounds
        QPointF center = target->boundingRect().center();
        QMouseEvent pressEvent(QEvent::MouseButtonPress, center, target->mapToGlobal(center), BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier);
        QMouseEvent releaseEvent(QEvent::MouseButtonRelease, center, target->mapToGlobal(center), BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier);
        
        QCoreApplication::sendEvent(target, &pressEvent);
        QCoreApplication::sendEvent(target, &releaseEvent);
        return true;
    } 
    else if (action == "type") {
        // Send key events for the payload string natively
        for (QChar c : payload) {
            QKeyEvent keyPress(QEvent::KeyPress, 0, BobUI::NoModifier, QString(c));
            QKeyEvent keyRelease(QEvent::KeyRelease, 0, BobUI::NoModifier, QString(c));
            QCoreApplication::sendEvent(target, &keyPress);
            QCoreApplication::sendEvent(target, &keyRelease);
        }
        return true;
    }

    return false;
}
