#include "OmniSlider.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <algorithm>

OmniSlider::OmniSlider(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_value(50.0),
      m_minimum(0.0),
      m_maximum(100.0),
      m_accentColor(QColor("#0078D7")),
      m_isDragging(false)
{
    setObjectName("OmniSlider");
    setAcceptedMouseButtons(Qt::LeftButton);
    
    // Set a sensible default size if not driven by layouts
    setWidth(200);
    setHeight(20);
}

OmniSlider::~OmniSlider() = default;

qreal OmniSlider::value() const { return m_value; }
void OmniSlider::setValue(qreal val) {
    val = std::clamp(val, m_minimum, m_maximum);
    if (!qFuzzyCompare(m_value, val)) {
        m_value = val;
        emit valueChanged();
        update();
    }
}

qreal OmniSlider::minimum() const { return m_minimum; }
void OmniSlider::setMinimum(qreal min) {
    if (!qFuzzyCompare(m_minimum, min)) {
        m_minimum = min;
        emit minimumChanged();
        if (m_value < m_minimum) setValue(m_minimum);
    }
}

qreal OmniSlider::maximum() const { return m_maximum; }
void OmniSlider::setMaximum(qreal max) {
    if (!qFuzzyCompare(m_maximum, max)) {
        m_maximum = max;
        emit maximumChanged();
        if (m_value > m_maximum) setValue(m_maximum);
    }
}

QColor OmniSlider::accentColor() const { return m_accentColor; }
void OmniSlider::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniSlider::updateValueFromMouse(const QPointF& pos) {
    // Assuming horizontal slider
    qreal ratio = pos.x() / width();
    ratio = std::clamp(ratio, 0.0, 1.0);
    qreal newVal = m_minimum + ratio * (m_maximum - m_minimum);
    setValue(newVal);
}

void OmniSlider::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        updateValueFromMouse(event->position());
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniSlider::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        updateValueFromMouse(event->position());
    }
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniSlider::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniSlider::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    qreal trackHeight = 6.0;
    QRectF trackRect(0, (rect.height() - trackHeight) / 2.0, rect.width(), trackHeight);

    // Draw background track
    QPainterPath bgPath;
    bgPath.addRoundedRect(trackRect, 3, 3);
    painter->fillPath(bgPath, QColor("#333333"));

    // Draw active track
    qreal ratio = (m_value - m_minimum) / (m_maximum - m_minimum);
    QRectF activeRect = trackRect;
    activeRect.setWidth(trackRect.width() * ratio);
    
    QPainterPath activePath;
    activePath.addRoundedRect(activeRect, 3, 3);
    painter->fillPath(activePath, m_accentColor);

    // Draw thumb handle
    qreal thumbRadius = 8.0;
    QPointF thumbCenter(activeRect.right(), rect.height() / 2.0);
    painter->setBrush(m_isDragging ? m_accentColor.lighter(120) : Qt::white);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(thumbCenter, thumbRadius, thumbRadius);
}
