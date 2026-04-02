#include "OmniButton.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHoverEvent>

OmniButton::OmniButton(QQuickItem *parent) 
    : QQuickPaintedItem(parent),
      m_text("Button"),
      m_omniStyle("primary"),
      m_accentColor(QColor("#0078D7")), // Windows 11 default blue
      m_cornerRadius(6),
      m_isHovered(false),
      m_isPressed(false)
{
    setObjectName("OmniButton");
    
    // Enable mouse and hover events for the QQuickItem
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}

OmniButton::~OmniButton() = default;

QString OmniButton::text() const { return m_text; }
void OmniButton::setText(const QString& text) {
    if (m_text != text) {
        m_text = text;
        emit textChanged();
        update();
    }
}

QString OmniButton::omniStyle() const { return m_omniStyle; }
void OmniButton::setOmniStyle(const QString& styleName) {
    if (m_omniStyle != styleName) {
        m_omniStyle = styleName;
        emit omniStyleChanged();
        update();
    }
}

QColor OmniButton::accentColor() const { return m_accentColor; }
void OmniButton::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

int OmniButton::cornerRadius() const { return m_cornerRadius; }
void OmniButton::setCornerRadius(int radius) {
    if (m_cornerRadius != radius) {
        m_cornerRadius = radius;
        emit cornerRadiusChanged();
        update();
    }
}

void OmniButton::hoverEnterEvent(QHoverEvent *event) {
    m_isHovered = true;
    update();
    QQuickPaintedItem::hoverEnterEvent(event);
}

void OmniButton::hoverLeaveEvent(QHoverEvent *event) {
    m_isHovered = false;
    update();
    QQuickPaintedItem::hoverLeaveEvent(event);
}

void OmniButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        update();
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniButton::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isPressed) {
        m_isPressed = false;
        update();
        emit clicked(); // Trigger the QML onClicked signal
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniButton::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Dynamic theming based on interaction state
    QColor bgColor = m_accentColor;
    if (m_isPressed) {
        bgColor = bgColor.darker(120);
    } else if (m_isHovered) {
        bgColor = bgColor.lighter(110);
    }

    if (m_omniStyle == "secondary") {
        bgColor = QColor("#333333");
        if (m_isHovered) bgColor = QColor("#444444");
    }

    // Draw the rounded background
    QPainterPath path;
    path.addRoundedRect(boundingRect(), m_cornerRadius, m_cornerRadius);
    painter->fillPath(path, bgColor);

    // Draw the text
    painter->setPen(Qt::white);
    
    QFont f = painter->font();
    f.setPixelSize(14); // Hardcoded for demo, would be exposed property
    painter->setFont(f);
    
    painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
}
