#include "OmniPlot.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>

OmniPlot::OmniPlot(QQuickItem *parent) : QQuickPaintedItem(parent), m_maxPoints(100) {
    setFlag(ItemHasContents, true);
}

OmniPlot::~OmniPlot() = default;

QList<qreal> OmniPlot::data() const { return m_data; }
void OmniPlot::setData(const QList<qreal>& d) { m_data = d; emit dataChanged(); update(); }

void OmniPlot::addPoint(qreal p) {
    m_data.append(p);
    if (m_data.size() > m_maxPoints) m_data.removeFirst();
    emit dataChanged();
    update();
}

void OmniPlot::paint(QPainter *painter) {
    if (m_data.isEmpty()) return;

    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();

    painter->fillRect(rect, theme->surface().darker(150));

    QPainterPath path;
    qreal step = rect.width() / (m_maxPoints - 1);
    
    for (int i = 0; i < m_data.size(); ++i) {
        qreal x = i * step;
        qreal y = rect.height() - (m_data[i] * rect.height()); // Assumes normalized 0-1
        if (i == 0) path.moveTo(x, y);
        else path.lineTo(x, y);
    }

    painter->setPen(QPen(theme->primary(), 2));
    painter->drawPath(path);
}
