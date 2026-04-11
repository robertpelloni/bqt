#include "OmniButton.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

OmniButton::OmniButton(QQuickItem *parent) 
    : QQuickPaintedItem(parent), m_text("Button"), m_isHovered(false), m_isPressed(false)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

void OmniButton::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();
    auto mode = theme->currentTheme();

    QPainterPath path;
    if (mode == OmniThemeManager::Cyberpunk) {
        path.moveTo(10, 0); path.lineTo(rect.width(), 0);
        path.lineTo(rect.width() - 10, rect.height()); path.lineTo(0, rect.height()); path.closeSubpath();
        
        painter->fillPath(path, m_isPressed ? theme->accent() : theme->surface());
        painter->setPen(QPen(theme->primary(), 1.5));
        painter->drawPath(path);
        
        painter->setPen(m_isPressed ? Qt::black : theme->primary());
        QFont f = painter->font(); f.setItalic(true); f.setBold(true); painter->setFont(f);
    } else if (mode == OmniThemeManager::LiquidGlass) {
        path.addRoundedRect(rect, 4, 4);
        painter->fillPath(path, QColor(255,255,255, m_isHovered ? 60 : 30));
        painter->setPen(QPen(QColor(255,255,255, 100), 1));
        painter->drawPath(path);
        painter->setPen(theme->text());
    } else if (mode == OmniThemeManager::Aetheria) {
        path.addRoundedRect(rect, 2, 2);
        painter->fillPath(path, QColor("#1A0B2E"));
        painter->setPen(QPen(theme->primary(), 1));
        painter->drawPath(path);
        painter->setPen(theme->primary());
    } else {
        path.addRoundedRect(rect, 4, 4);
        painter->fillPath(path, m_isPressed ? theme->primary().darker() : theme->primary());
        painter->setPen(Qt::NoPen);
        painter->drawPath(path);
        painter->setPen(theme->text());
    }

    painter->drawText(rect, Qt::AlignCenter, m_text.toUpper());
}

// Boilerplate property handlers...
QString OmniButton::text() const { return m_text; }
void OmniButton::setText(const QString& t) { m_text = t; update(); }
void OmniButton::hoverEnterEvent(QHoverEvent *e) { m_isHovered = true; update(); }
void OmniButton::hoverLeaveEvent(QHoverEvent *e) { m_isHovered = false; update(); }
void OmniButton::mousePressEvent(QMouseEvent *e) { m_isPressed = true; update(); }
void OmniButton::mouseReleaseEvent(QMouseEvent *e) { m_isPressed = false; update(); if (rect().contains(e->pos())) emit clicked(); }
OmniButton::~OmniButton() = default;
