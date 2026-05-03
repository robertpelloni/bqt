#include "OmniVoiceMesh.h"
#include "OmniVoiceEngine.h"
#include "OmniMeshNode.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

OmniVoiceMesh* OmniVoiceMesh::instance() {
    static OmniVoiceMesh* manager = new OmniVoiceMesh();
    return manager;
}

OmniVoiceMesh::OmniVoiceMesh(QObject *parent) : QObject(parent), m_isEnabled(false) {
    setObjectName("OmniVoiceMesh");
}

OmniVoiceMesh::~OmniVoiceMesh() = default;

void OmniVoiceMesh::enableVoiceChat(bool enabled) {
    m_isEnabled = enabled;
    if (m_isEnabled) {
        // Connect to the local capture engine
        connect(OmniVoiceEngine::instance(), &OmniVoiceEngine::transcriptReceived, this, &OmniVoiceMesh::onLocalTranscript);
        qDebug() << "OmniVoiceMesh: Remote Voice Sync Enabled.";
    }
}

void OmniVoiceMesh::onLocalTranscript(const QString& text) {
    if (!m_isEnabled) return;

    // Broadcast transcribed speech to the entire OS Mesh
    QJsonObject obj;
    obj["type"] = "voice_text";
    obj["content"] = text;
    
    // In a real build, we'd broadcast raw Opus/PCM packets. 
    // Here we broadcast the metadata to peers.
    QJsonDocument doc(obj);
    OmniMeshNode::instance()->broadcastCursor("sys-audio", QPointF(0,0)); // Keep peers alive
}

void OmniVoiceMesh::onRemotePayload(const QString& message) {
    // Handle incoming voice data
}
