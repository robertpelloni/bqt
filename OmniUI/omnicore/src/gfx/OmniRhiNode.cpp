#include "OmniRhiNode.h"
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

OmniRhiNode::OmniRhiNode(QQuickItem *parent) : QQuickItem(parent), m_handle(0) {
    setFlag(ItemHasContents, true);
}

OmniRhiNode::~OmniRhiNode() = default;

void OmniRhiNode::setNativeTextureHandle(quintptr h) {
    if (m_handle != h) {
        m_handle = h;
        emit textureChanged();
        update();
    }
}

QSGNode* OmniRhiNode::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    if (m_handle == 0) return oldNode;

    QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>(oldNode);
    if (!node) {
        node = new QSGSimpleTextureNode();
    }

    // In a real Qt 7 implementation, we'd use QQuickWindow::createTextureFromNative()
    // node->setTexture(window()->createTextureFromNative(m_handle, ...));
    
    node->setRect(boundingRect());
    return node;
}
