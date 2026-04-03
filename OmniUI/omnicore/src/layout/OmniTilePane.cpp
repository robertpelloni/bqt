#include "OmniTilePane.h"
#include <algorithm>

OmniTilePane::OmniTilePane(QQuickItem *parent) : QQuickItem(parent), m_tileW(100), m_tileH(100) {
    setObjectName("OmniTilePane");
}

OmniTilePane::~OmniTilePane() = default;

int OmniTilePane::prefTileWidth() const { return m_tileW; }
void OmniTilePane::setPrefTileWidth(int w) { m_tileW = w; emit prefTileWidthChanged(); layoutChildren(); }
int OmniTilePane::prefTileHeight() const { return m_tileH; }
void OmniTilePane::setPrefTileHeight(int h) { m_tileH = h; emit prefTileHeightChanged(); layoutChildren(); }

void OmniTilePane::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    layoutChildren();
}

void OmniTilePane::itemChildAdded(QQuickItem*) { layoutChildren(); }

void OmniTilePane::layoutChildren() {
    qreal maxW = width();
    if (maxW <= 0) return;

    int cols = std::max(1, static_cast<int>(maxW / m_tileW));
    const auto children = childItems();

    int count = 0;
    for (QQuickItem* child : children) {
        if (!child->isVisible()) continue;

        int row = count / cols;
        int col = count % cols;

        child->setPosition(QPointF(col * m_tileW, row * m_tileH));
        child->setSize(QSizeF(m_tileW, m_tileH));
        count++;
    }
}
