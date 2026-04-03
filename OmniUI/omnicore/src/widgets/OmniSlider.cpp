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
    setAcceptedMouseButtons(Qt::LeftButton);
    setWidth(200); setHeight(20);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

void OmniSlider::mousePressEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (OmniUserManager::instance()->tryLockInteraction(deviceId, this)) {
        m_isDragging = true;
        updateValueFromMouse(event->position());
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniSlider::mouseReleaseEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (m_isDragging) {
        m_isDragging = false;
        OmniUserManager::instance()->unlockInteraction(deviceId, this);
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniSlider::updateValueFromMouse(const QPointF& pos) {
    qreal ratio = std::clamp(pos.x() / width(), 0.0, 1.0);
    qreal newVal = m_minimum + ratio * (m_maximum - m_minimum);
    if (!qFuzzyCompare(m_value, newVal)) { m_value = newVal; emit valueChanged(); update(); }
}

void OmniSlider::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();
    auto userManager = OmniUserManager::instance();
    
    // --- OWNERSHIP VISUALIZATION ---
    // If someone has grabbed this slider, draw a glowing border in their color.
    if (!userManager->canInteract("", this)) {
        QString userId = userManager->getUserIdForDevice(property("_omni_active_device").toString());
        // For simulation, we draw a subtle gold highlight if locked
        painter->setPen(QPen(theme->primary(), 2, Qt::DotLine));
        painter->drawRect(rect.adjusted(-2, -2, 2, 2));
    }

    qreal trackH = 6.0;
    QRectF trackRect(0, (rect.height() - trackH)/2.0, rect.width(), trackH);
    painter->fillPath(QPainterPath().addRoundedRect(trackRect, 3, 3), theme->surface().darker(150));

    qreal ratio = (m_value - m_minimum) / (m_maximum - m_minimum);
    QRectF activeRect = trackRect; activeRect.setWidth(trackRect.width() * ratio);
    painter->fillPath(QPainterPath().addRoundedRect(activeRect, 3, 3), m_accentColor);

    painter->setBrush(m_isDragging ? m_accentColor.lighter(120) : Qt::white);
    painter->drawEllipse(QPointF(activeRect.right(), rect.height()/2.0), 8.0, 8.0);
}

// Logic implementations omitted but preserved in kernel...
qreal OmniSlider::value() const { return m_value; }
void OmniSlider::setValue(qreal val) { m_value = val; update(); }
qreal OmniSlider::minimum() const { return m_minimum; }
void OmniSlider::setMinimum(qreal min) { m_minimum = min; update(); }
qreal OmniSlider::maximum() const { return m_maximum; }
void OmniSlider::setMaximum(qreal max) { m_maximum = max; update(); }
QColor OmniSlider::accentColor() const { return m_accentColor; }
void OmniSlider::setAccentColor(const QColor& color) { m_accentColor = color; update(); }
OmniSlider::~OmniSlider() = default;
void OmniSlider::mouseMoveEvent(QMouseEvent *e) { 
    QString deviceId = property("_omni_active_device").toString();
    if (m_isDragging && OmniUserManager::instance()->canInteract(deviceId, this)) updateValueFromMouse(e->position());
    QQuickPaintedItem::mouseMoveEvent(e); 
}
