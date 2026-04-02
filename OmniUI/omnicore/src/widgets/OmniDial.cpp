#include "OmniDial.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <cmath>
#include <algorithm>

OmniDial::OmniDial(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_value(0.0),
      m_minimum(0.0),
      m_maximum(100.0),
      m_accentColor(QColor("#0078D7")),
      m_isDragging(false)
{
    setObjectName("OmniDial");
    setAcceptedMouseButtons(Qt::LeftButton);
    setWidth(60);
    setHeight(60);
}

OmniDial::~OmniDial() = default;

qreal OmniDial::value() const { return m_value; }
void OmniDial::setValue(qreal val) {
    val = std::clamp(val, m_minimum, m_maximum);
    if (!qFuzzyCompare(m_value, val)) {
        m_value = val;
        emit valueChanged();
        update();
    }
}

qreal OmniDial::minimum() const { return m_minimum; }
void OmniDial::setMinimum(qreal min) {
    if (!qFuzzyCompare(m_minimum, min)) {
        m_minimum = min;
        emit minimumChanged();
        if (m_value < m_minimum) setValue(m_minimum);
    }
}

qreal OmniDial::maximum() const { return m_maximum; }
void OmniDial::setMaximum(qreal max) {
    if (!qFuzzyCompare(m_maximum, max)) {
        m_maximum = max;
        emit maximumChanged();
        if (m_value > m_maximum) setValue(m_maximum);
    }
}

QColor OmniDial::accentColor() const { return m_accentColor; }
void OmniDial::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

void OmniDial::updateValueFromMouse(const QPointF& pos) {
    // Relative linear dragging (like Ableton/JUCE dials) instead of strict polar tracking
    // is usually much better for UX.
    qreal deltaY = m_lastMousePos.y() - pos.y(); // Up increases value
    
    // Sensitivity
    qreal range = m_maximum - m_minimum;
    qreal change = (deltaY / 100.0) * range; 
    
    setValue(m_value + change);
    m_lastMousePos = pos;
}

void OmniDial::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->position();
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniDial::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        updateValueFromMouse(event->position());
    }
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniDial::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniDial::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    qreal minDim = std::min(rect.width(), rect.height());
    QRectF dialRect((rect.width() - minDim) / 2.0, (rect.height() - minDim) / 2.0, minDim, minDim);
    
    // Padding
    dialRect.adjust(4, 4, -4, -4);

    // Background circle
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#222222"));
    painter->drawEllipse(dialRect);

    // Active Arc
    qreal ratio = (m_value - m_minimum) / (m_maximum - m_minimum);
    
    // Qt angles are in 1/16th of a degree. 0 is 3 o'clock, positive is counter-clockwise.
    // We want the dial to go from 7 o'clock to 5 o'clock (approx -135 to +135 from bottom center)
    int startAngle = 225 * 16; 
    int spanAngle = -ratio * 270 * 16; // 270 degree total travel
    
    painter->setPen(QPen(m_accentColor, 4, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(Qt::NoBrush);
    painter->drawArc(dialRect, startAngle, spanAngle);

    // Indicator Line (Thumb)
    painter->translate(rect.center());
    painter->rotate(-135 + ratio * 270);
    painter->setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(0, -minDim/2.0 + 8, 0, -minDim/2.0 + 16);
    painter->resetTransform();
}
