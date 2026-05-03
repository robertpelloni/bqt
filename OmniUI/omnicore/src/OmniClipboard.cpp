#include "OmniClipboard.h"
#include "OmniMeshNode.h"
#include <QJsonObject>
#include <QDebug>

OmniClipboard* OmniClipboard::instance() {
    static OmniClipboard* manager = new OmniClipboard();
    return manager;
}

OmniClipboard::OmniClipboard(QObject *parent) : QObject(parent) {
    setObjectName("OmniClipboard");
    
    // Optional: Sync with native system clipboard
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, [this]() {
        QString nativeText = QGuiApplication::clipboard()->text();
        if (nativeText != m_text) {
            setText(nativeText);
        }
    });
}

OmniClipboard::~OmniClipboard() = default;

QString OmniClipboard::text() const { return m_text; }

void OmniClipboard::setText(const QString& text) {
    if (m_text != text) {
        m_text = text;
        emit textChanged(m_text);
        
        qDebug() << "OmniClipboard: Local Update -" << text.left(20) << "...";

        // BROADCAST to OS Mesh
        QJsonObject obj;
        obj["type"] = "clipboard_sync";
        obj["content"] = m_text;
        OmniMeshNode::instance()->broadcastPayload(obj);
    }
}

void OmniClipboard::receiveRemoteText(const QString& text) {
    if (m_text != text) {
        m_text = text;
        qDebug() << "OmniClipboard: Remote Peer Update -" << text.left(20) << "...";
        emit textChanged(m_text);
        emit remoteUpdateReceived(m_text);
        
        // Update local native clipboard
        QGuiApplication::clipboard()->setText(m_text);
    }
}
