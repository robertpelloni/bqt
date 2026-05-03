#include "OmniProgressBar.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>

OmniProgressBar::OmniProgressBar(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_value(0.0),
      m_minimum(0.0),
      m_maximum(100.0),
      m_accentColor(QColor("#0078D7")) // Windows 11 default blue
{
    setObjectName("OmniProgressBar");
    setWidth(200);
    setHeight(10);
}

OmniProgressBar::~OmniProgressBar() = default;

qreal OmniProgressBar::value() const { return m_value; }
void OmniProgressBar::setValue(qreal val) {
    val = std::clamp(val, m_minimum, m_maximum);
    if (!qFuzzyCompare(m_value, val)) {
        m_value = val;
        emit valueChanged();
        update(); // Trigger repaint
    }
}

qreal OmniProgressBar::minimum() const { return m_minimum; }
void OmniProgressBar::setMinimum(qreal min) {
    if (!qFuzzyCompare(m_minimum, min)) {
        m_minimum = min;
        emit minimumChanged();
        if (m_value < m_minimum) setValue(m_minimum);
    }
}

qreal OmniProgressBar::maximum() const { return m_maximum; }
void OmniProgressBar::setMaximum(qreal max) {
    if (!qFuzzyCompare(m_maximum, max)) {
        m_maximum = max;
        emit maximumChanged();
        if (m_value > m_maximum) setValue(m_maximum);
    }
}

QColor OmniProgressBar::accentColor() const { return m_accentColor; }
void OmniProgressBar::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniProgressBar::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    
    // Background track
    QPainterPath bgPath;
    bgPath.addRoundedRect(rect, rect.height() / 2.0, rect.height() / 2.0);
    painter->fillPath(bgPath, QColor("#333333"));

    // Active progress
    qreal ratio = (m_maximum > m_minimum) ? (m_value - m_minimum) / (m_maximum - m_minimum) : 0.0;
    if (ratio > 0.0) {
        QRectF activeRect = rect;
        activeRect.setWidth(rect.width() * ratio);
        
        QPainterPath activePath;
        activePath.addRoundedRect(activeRect, activeRect.height() / 2.0, activeRect.height() / 2.0);
        painter->fillPath(activePath, m_accentColor);
    }
}
