#include "OmniDockArea.h"
#include <QDebug>

OmniDockArea::OmniDockArea(QQuickItem *parent)
    : QQuickItem(parent),
      m_areaName("Dock")
{
    setObjectName("OmniDockArea");
    setFlag(ItemHasContents, false); // Just a container
}

OmniDockArea::~OmniDockArea() = default;

QString OmniDockArea::areaName() const { return m_areaName; }
void OmniDockArea::setAreaName(const QString& name) {
    if (m_areaName != name) {
        m_areaName = name;
        emit areaNameChanged();
    }
}

void OmniDockArea::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    layoutChildren();
}

void OmniDockArea::layoutChildren() {
    // A true docking system (like ImGui) requires a complex tree structure
    // tracking splits, tabs, and undocked floating windows.
    // For now, this container just forces its children to fill its rect.
    
    QRectF rect = boundingRect();
    const auto items = childItems();
    for (QQuickItem *item : items) {
        if (item->isVisible()) {
            item->setPosition(QPointF(0, 0));
            item->setSize(QSizeF(rect.width(), rect.height()));
        }
    }
}
