#include "OmniDial.h"
#include "OmniThemeManager.h"
#include "OmniUserManager.h"
#include "OmniValueTree.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

OmniDial::OmniDial(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_value(0.0), m_minimum(0.0), m_maximum(100.0),
      m_accentColor(QColor("#0078D7")), m_isDragging(false)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setWidth(60); setHeight(60);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

qreal OmniDial::value() const { return m_value; }
void OmniDial::setValue(qreal val) {
    val = std::clamp(val, m_minimum, m_maximum);
    if (!qFuzzyCompare(m_value, val)) {
        m_value = val;
        emit valueChanged();
        if (!m_valueTreeKey.isEmpty()) OmniValueTree::instance()->setParameter(m_valueTreeKey, m_value);
        update();
    }
}

QString OmniDial::valueTreeKey() const { return m_valueTreeKey; }
void OmniDial::setValueTreeKey(const QString& key) {
    if (m_valueTreeKey != key) {
        m_valueTreeKey = key;
        OmniValueTree::instance()->bindProperty(this, "value", key);
        emit valueTreeKeyChanged();
    }
}

void OmniDial::mousePressEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (OmniUserManager::instance()->tryLockInteraction(deviceId, this)) {
        m_isDragging = true;
        m_lastMousePos = event->position();
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniDial::mouseMoveEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (m_isDragging && OmniUserManager::instance()->canInteract(deviceId, this)) {
        qreal deltaY = m_lastMousePos.y() - event->position().y();
        setValue(m_value + (deltaY / 100.0) * (m_maximum - m_minimum));
        m_lastMousePos = event->position();
    }
    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniDial::mouseReleaseEvent(QMouseEvent *event) {
    QString deviceId = property("_omni_active_device").toString();
    if (m_isDragging) {
        m_isDragging = false;
        OmniUserManager::instance()->unlockInteraction(deviceId, this);
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniDial::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();
    auto userManager = OmniUserManager::instance();
    
    // Grab Glow
    if (!userManager->canInteract("", this)) {
        painter->setPen(QPen(theme->primary(), 2));
        painter->drawEllipse(rect.adjusted(2,2,-2,-2));
    }

    qreal minDim = std::min(rect.width(), rect.height()) - 10;
    QRectF dialRect((rect.width()-minDim)/2.0, (rect.height()-minDim)/2.0, minDim, minDim);
    
    painter->setBrush(theme->surface().darker(150));
    painter->drawEllipse(dialRect);

    qreal ratio = (m_value - m_minimum) / (m_maximum - m_minimum);
    painter->setPen(QPen(m_accentColor, 4, Qt::SolidLine, Qt::RoundCap));
    painter->drawArc(dialRect, 225*16, -ratio*270*16);
}

// Boilerplate property handlers...
qreal OmniDial::minimum() const { return m_minimum; }
void OmniDial::setMinimum(qreal min) { m_minimum = min; update(); }
qreal OmniDial::maximum() const { return m_maximum; }
void OmniDial::setMaximum(qreal max) { m_maximum = max; update(); }
QColor OmniDial::accentColor() const { return m_accentColor; }
void OmniDial::setAccentColor(const QColor& color) { m_accentColor = color; update(); }
OmniDial::~OmniDial() = default;
