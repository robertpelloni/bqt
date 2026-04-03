#include "OmniDropShadow.h"
#include <QSGSimpleRectNode>

OmniDropShadow::OmniDropShadow(QQuickItem *parent) 
    : QQuickItem(parent), m_radius(10), m_color(0,0,0,150), m_offset(2,2) {
    setFlag(ItemHasContents, true);
}

OmniDropShadow::~OmniDropShadow() = default;

QSGNode* OmniDropShadow::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    // In a production build, this would use a QSGNinePatchNode or a custom blurred texture.
    // For this milestone, we provide the structural RHI node that creates the shadow geometry.
    QSGSimpleRectNode* node = static_cast<QSGSimpleRectNode*>(oldNode);
    if (!node) {
        node = new QSGSimpleRectNode();
    }
    
    node->setRect(boundingRect().translated(m_offset));
    node->setColor(m_color);
    return node;
}
