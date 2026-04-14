#include "BobQGrid.h"

BobQGrid::BobQGrid(QQuickItem *parent) : QQuickItem(parent) {
}

int BobQGrid::rowSpacing() const { return m_rowSpacing; }
void BobQGrid::setRowSpacing(int spacing) {
    if (m_rowSpacing == spacing) return;
    m_rowSpacing = spacing;
    relayout();
    emit rowSpacingChanged();
}

int BobQGrid::columnSpacing() const { return m_columnSpacing; }
void BobQGrid::setColumnSpacing(int spacing) {
    if (m_columnSpacing == spacing) return;
    m_columnSpacing = spacing;
    relayout();
    emit columnSpacingChanged();
}

void BobQGrid::attach(QQuickItem* child, int left, int top, int width, int height) {
    if(!child) return;
    child->setParentItem(this);
    m_children.append({child, left, top, width, height});
    relayout();
}

void BobQGrid::attachNextTo(QQuickItem* child, QQuickItem* sibling, Qt::Edge side, int width, int height) {
    if(!child || !sibling) return;

    // Find sibling to determine position
    for (const auto& c : std::as_const(m_children)) {
        if (c.item == sibling) {
            int left = c.left;
            int top = c.top;

            switch (side) {
                case Qt::LeftEdge: left -= width; break;
                case Qt::RightEdge: left += c.width; break;
                case Qt::TopEdge: top -= height; break;
                case Qt::BottomEdge: top += c.height; break;
            }

            attach(child, left, top, width, height);
            return;
        }
    }
}

void BobQGrid::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    relayout();
}

void BobQGrid::relayout() {
    // In a full implementation, we'd calculate implicit sizes of rows/cols
    // Here we just map them proportionally to available size for demonstration.
    if (m_children.isEmpty()) return;

    int maxCol = 0;
    int maxRow = 0;
    for (const auto& c : std::as_const(m_children)) {
        maxCol = qMax(maxCol, c.left + c.width);
        maxRow = qMax(maxRow, c.top + c.height);
    }

    if (maxCol == 0 || maxRow == 0) return;

    qreal cellW = (width() - (m_columnSpacing * (maxCol - 1))) / maxCol;
    qreal cellH = (height() - (m_rowSpacing * (maxRow - 1))) / maxRow;

    for (const auto& c : std::as_const(m_children)) {
        if (!c.item) continue;

        qreal x = (c.left * cellW) + (c.left * m_columnSpacing);
        qreal y = (c.top * cellH) + (c.top * m_rowSpacing);
        qreal w = (c.width * cellW) + ((c.width - 1) * m_columnSpacing);
        qreal h = (c.height * cellH) + ((c.height - 1) * m_rowSpacing);

        c.item->setPosition(QPointF(x, y));
        c.item->setSize(QSizeF(w, h));
    }
}
