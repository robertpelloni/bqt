#include "OmniFlowPane.h"

OmniFlowPane::OmniFlowPane(QQuickItem *parent) : QQuickItem(parent), m_hgap(5), m_vgap(5) {
    setObjectName("OmniFlowPane");
}

OmniFlowPane::~OmniFlowPane() = default;

int OmniFlowPane::hgap() const { return m_hgap; }
void OmniFlowPane::setHgap(int gap) { m_hgap = gap; emit gapsChanged(); layoutChildren(); }
int OmniFlowPane::vgap() const { return m_vgap; }
void OmniFlowPane::setVgap(int gap) { m_vgap = gap; emit gapsChanged(); layoutChildren(); }

void OmniFlowPane::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    layoutChildren();
}

void OmniFlowPane::itemChildAdded(QQuickItem*) { layoutChildren(); }

void OmniFlowPane::layoutChildren() {
    qreal curX = 0;
    qreal curY = 0;
    qreal rowHeight = 0;
    qreal maxW = width();

    const auto children = childItems();
    for (QQuickItem* child : children) {
        if (!child->isVisible()) continue;

        if (curX + child->width() > maxW && curX > 0) {
            curX = 0;
            curY += rowHeight + m_vgap;
            rowHeight = 0;
        }

        child->setPosition(QPointF(curX, curY));
        curX += child->width() + m_hgap;
        rowHeight = qMax(rowHeight, child->height());
    }
}
