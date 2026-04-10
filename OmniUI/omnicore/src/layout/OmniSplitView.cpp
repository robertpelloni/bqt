#include "OmniSplitView.h"
#include <QDebug>

OmniSplitView::OmniSplitView(QQuickItem *parent)
    : QQuickItem(parent),
      m_orientation(BobUI::Horizontal),
      m_handleSize(4.0)
{
    setObjectName("OmniSplitView");
}

OmniSplitView::~OmniSplitView() = default;

BobUI::Orientation OmniSplitView::orientation() const { return m_orientation; }
void OmniSplitView::setOrientation(BobUI::Orientation o) {
    if (m_orientation != o) {
        m_orientation = o;
        emit orientationChanged();
        layoutChildren();
    }
}

qreal OmniSplitView::handleSize() const { return m_handleSize; }
void OmniSplitView::setHandleSize(qreal size) {
    if (!qFuzzyCompare(m_handleSize, size)) {
        m_handleSize = size;
        emit handleSizeChanged();
        layoutChildren();
    }
}

void OmniSplitView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    layoutChildren();
}

void OmniSplitView::layoutChildren() {
    // Simple proportional splitter logic
    const auto items = childItems();
    if (items.isEmpty()) return;

    QRectF rect = boundingRect();
    int visibleCount = 0;
    for (QQuickItem *item : items) {
        if (item->isVisible()) visibleCount++;
    }

    if (visibleCount == 0) return;

    qreal totalHandleSpace = m_handleSize * (visibleCount - 1);
    
    if (m_orientation == BobUI::Horizontal) {
        qreal itemWidth = (rect.width() - totalHandleSpace) / visibleCount;
        qreal currentX = 0;
        
        for (QQuickItem *item : items) {
            if (item->isVisible()) {
                item->setPosition(QPointF(currentX, 0));
                item->setSize(QSizeF(itemWidth, rect.height()));
                currentX += itemWidth + m_handleSize;
            }
        }
    } else {
        qreal itemHeight = (rect.height() - totalHandleSpace) / visibleCount;
        qreal currentY = 0;
        
        for (QQuickItem *item : items) {
            if (item->isVisible()) {
                item->setPosition(QPointF(0, currentY));
                item->setSize(QSizeF(rect.width(), itemHeight));
                currentY += itemHeight + m_handleSize;
            }
        }
    }
}
