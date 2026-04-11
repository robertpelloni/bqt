#include "OmniGridPane.h"
#include <QDebug>
#include <algorithm>

OmniGridPane::OmniGridPane(QQuickItem *parent) : QQuickItem(parent), m_hGap(5), m_vGap(5) {
    setObjectName("OmniGridPane");
}

OmniGridPane::~OmniGridPane() = default;

int OmniGridPane::horizontalGap() const { return m_hGap; }
void OmniGridPane::setHorizontalGap(int gap) { m_hGap = gap; emit horizontalGapChanged(); layoutChildren(); }

int OmniGridPane::verticalGap() const { return m_vGap; }
void OmniGridPane::setVerticalGap(int gap) { m_vGap = gap; emit verticalGapChanged(); layoutChildren(); }

void OmniGridPane::setConstraints(QQuickItem* item, int column, int row) {
    if (!item) return;
    m_constraints[item] = {column, row};
    layoutChildren();
}

void OmniGridPane::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    layoutChildren();
}

void OmniGridPane::itemChildAdded(QQuickItem*) {
    layoutChildren();
}

void OmniGridPane::layoutChildren() {
    QRectF rect = boundingRect();
    const auto children = childItems();
    if (children.isEmpty()) return;

    // Simple uniform grid calculation for this milestone
    int maxCol = 0, maxRow = 0;
    for (auto* child : children) {
        if (m_constraints.contains(child)) {
            maxCol = std::max(maxCol, m_constraints[child].col);
            maxRow = std::max(maxRow, m_constraints[child].row);
        }
    }
    maxCol++; maxRow++; // 0-based to count

    qreal cellW = (rect.width() - (m_hGap * (maxCol - 1))) / maxCol;
    qreal cellH = (rect.height() - (m_vGap * (maxRow - 1))) / maxRow;

    for (auto* child : children) {
        if (!child->isVisible()) continue;
        
        int col = 0, row = 0;
        if (m_constraints.contains(child)) {
            col = m_constraints[child].col;
            row = m_constraints[child].row;
        }

        qreal x = col * (cellW + m_hGap);
        qreal y = row * (cellH + m_vGap);

        child->setPosition(QPointF(x, y));
        child->setSize(QSizeF(cellW, cellH));
    }
}
