#include "BobQBox.h"

BobQBox::BobQBox(QQuickItem *parent) : QQuickItem(parent) {
}

Qt::Orientation BobQBox::orientation() const { return m_orientation; }
void BobQBox::setOrientation(Qt::Orientation orientation) {
    if (m_orientation == orientation) return;
    m_orientation = orientation;
    relayout();
    emit orientationChanged();
}

int BobQBox::spacing() const { return m_spacing; }
void BobQBox::setSpacing(int spacing) {
    if (m_spacing == spacing) return;
    m_spacing = spacing;
    relayout();
    emit spacingChanged();
}

bool BobQBox::homogeneous() const { return m_homogeneous; }
void BobQBox::setHomogeneous(bool homogeneous) {
    if (m_homogeneous == homogeneous) return;
    m_homogeneous = homogeneous;
    relayout();
    emit homogeneousChanged();
}

void BobQBox::packStart(QQuickItem* child, bool expand, bool fill, int padding) {
    if(!child) return;
    child->setParentItem(this);
    m_children.append({child, expand, fill, padding, false});
    relayout();
}

void BobQBox::packEnd(QQuickItem* child, bool expand, bool fill, int padding) {
    if(!child) return;
    child->setParentItem(this);
    m_children.append({child, expand, fill, padding, true});
    relayout();
}

void BobQBox::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    relayout();
}

void BobQBox::relayout() {
    // Basic mock layout algorithm to emulate GtkBox
    qreal currentPosStart = 0;
    qreal currentPosEnd = (m_orientation == Qt::Horizontal) ? width() : height();

    for (const auto& c : std::as_const(m_children)) {
        if (!c.item) continue;

        qreal cWidth = c.item->width();
        qreal cHeight = c.item->height();

        if (m_orientation == Qt::Horizontal) {
            if (c.packEnd) {
                currentPosEnd -= (cWidth + m_spacing + c.padding);
                c.item->setPosition(QPointF(currentPosEnd, 0));
            } else {
                currentPosStart += c.padding;
                c.item->setPosition(QPointF(currentPosStart, 0));
                currentPosStart += (cWidth + m_spacing);
            }
        } else {
            if (c.packEnd) {
                currentPosEnd -= (cHeight + m_spacing + c.padding);
                c.item->setPosition(QPointF(0, currentPosEnd));
            } else {
                currentPosStart += c.padding;
                c.item->setPosition(QPointF(0, currentPosStart));
                currentPosStart += (cHeight + m_spacing);
            }
        }
    }
}
