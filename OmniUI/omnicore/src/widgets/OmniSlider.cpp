#include "OmniSlider.h"
#include "OmniThemeManager.h"
#include "OmniUserManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <algorithm>

OmniSlider::OmniSlider(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_value(50.0), m_minimum(0.0), m_maximum(100.0),
      m_accentColor(QColor("#0078D7")), m_isDragging(false)
{
    setObjectName("OmniSlider");
    setAcceptedMouseButtons(Qt::LeftButton);
    setWidth(200); setHeight(20);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

OmniSlider::~OmniSlider() = default;

void OmniSlider::mousePressEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    
    // --- SINGLE-USER ACTION LOCK ---
    // Only the first person to touch this slider owns the drag action
    if (OmniUserManager::instance()->tryGrab(deviceId, this)) {
        m_isDragging = true;
        updateValueFromMouse(event->position());
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniSlider::mouseMoveEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    
    if (m_isDragging && OmniUserManager::instance()->canInteract(deviceId, this)) {
        updateValueFromMouse(event->position());
    }
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniSlider::mouseReleaseEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (m_isDragging) {
        m_isDragging = false;
        OmniUserManager::instance()->releaseGrab(deviceId, this);
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniSlider::updateValueFromMouse(const QPointF& pos) {
    qreal ratio = pos.x() / width();
    ratio = std::clamp(ratio, 0.0, 1.0);
    qreal newVal = m_minimum + ratio * (m_maximum - m_minimum);
    if (!qFuzzyCompare(m_value, newVal)) {
        m_value = newVal;
        emit valueChanged();
        update();
    }
}

void OmniSlider::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();
    
    qreal trackHeight = 6.0;
    QRectF trackRect(0, (rect.height() - trackHeight) / 2.0, rect.width(), trackHeight);
    painter->fillPath(QPainterPath().addRoundedRect(trackRect, 3, 3), theme->surfaceColor().darker(150));

    qreal ratio = (m_value - m_minimum) / (m_maximum - m_minimum);
    QRectF activeRect = trackRect; activeRect.setWidth(trackRect.width() * ratio);
    painter->fillPath(QPainterPath().addRoundedRect(activeRect, 3, 3), m_accentColor);

    QPointF thumbCenter(activeRect.right(), rect.height() / 2.0);
    painter->setBrush(m_isDragging ? m_accentColor.lighter(120) : Qt::white);
    painter->drawEllipse(thumbCenter, 8.0, 8.0);
}

qreal OmniSlider::value() const { return m_value; }
void OmniSlider::setValue(qreal val) { m_value = val; update(); }
qreal OmniSlider::minimum() const { return m_minimum; }
void OmniSlider::setMinimum(qreal min) { m_minimum = min; update(); }
qreal OmniSlider::maximum() const { return m_maximum; }
void OmniSlider::setMaximum(qreal max) { m_maximum = max; update(); }
QColor OmniSlider::accentColor() const { return m_accentColor; }
void OmniSlider::setAccentColor(const QColor& color) { m_accentColor = color; update(); }
