#include "OmniStackPane.h"
#include <QDebug>

OmniStackPane::OmniStackPane(QQuickItem *parent) 
    : QQuickItem(parent), m_alignment(Qt::AlignCenter) {
    setObjectName("OmniStackPane");
}

OmniStackPane::~OmniStackPane() = default;

Qt::Alignment OmniStackPane::alignment() const { return m_alignment; }
void OmniStackPane::setAlignment(Qt::Alignment align) {
    if (m_alignment != align) {
        m_alignment = align;
        emit alignmentChanged();
        layoutChildren();
    }
}

void OmniStackPane::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    layoutChildren();
}

void OmniStackPane::itemChildAdded(QQuickItem*) {
    layoutChildren();
}

void OmniStackPane::layoutChildren() {
    QRectF rect = boundingRect();
    const auto children = childItems();
    
    for (QQuickItem* child : children) {
        if (!child->isVisible()) continue;

        qreal x = 0;
        qreal y = 0;

        // Horizontal Alignment
        if (m_alignment & Qt::AlignLeft) x = 0;
        else if (m_alignment & Qt::AlignRight) x = rect.width() - child->width();
        else if (m_alignment & Qt::AlignHCenter) x = (rect.width() - child->width()) / 2.0;

        // Vertical Alignment
        if (m_alignment & Qt::AlignTop) y = 0;
        else if (m_alignment & Qt::AlignBottom) y = rect.height() - child->height();
        else if (m_alignment & Qt::AlignVCenter) y = (rect.height() - child->height()) / 2.0;

        child->setPosition(QPointF(x, y));
    }
}
