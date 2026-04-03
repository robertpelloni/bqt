#include "OmniButton.h"
#include "OmniThemeManager.h"
#include "OmniStyleSheet.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHoverEvent>

OmniButton::OmniButton(QQuickItem *parent) 
    : QQuickPaintedItem(parent),
      m_text("Button"),
      m_omniStyle("primary"),
      m_accentColor(QColor("#0078D7")), 
      m_cornerRadius(6),
      m_parsedStyle(new OmniStyleSheet()),
      m_isHovered(false),
      m_isPressed(false)
{
    setObjectName("OmniButton");
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() {
        update(); 
    });
}

OmniButton::~OmniButton() {
    delete m_parsedStyle;
}

QString OmniButton::text() const { return m_text; }
void OmniButton::setText(const QString& text) {
    if (m_text != text) { m_text = text; emit textChanged(); update(); }
}

QString OmniButton::omniStyle() const { return m_omniStyle; }
void OmniButton::setOmniStyle(const QString& styleName) {
    if (m_omniStyle != styleName) { m_omniStyle = styleName; emit omniStyleChanged(); update(); }
}

QColor OmniButton::accentColor() const { return m_accentColor; }
void OmniButton::setAccentColor(const QColor& color) {
    if (m_accentColor != color) { m_accentColor = color; emit accentColorChanged(); update(); }
}

int OmniButton::cornerRadius() const { return m_cornerRadius; }
void OmniButton::setCornerRadius(int radius) {
    if (m_cornerRadius != radius) { m_cornerRadius = radius; emit cornerRadiusChanged(); update(); }
}

QString OmniButton::styleSheet() const { return m_styleSheetString; }
void OmniButton::setStyleSheet(const QString& style) {
    if (m_styleSheetString != style) {
        m_styleSheetString = style;
        m_parsedStyle->parse(style);
        emit styleSheetChanged();
        update();
    }
}

void OmniButton::hoverEnterEvent(QHoverEvent *event) {
    m_isHovered = true; update();
    QQuickPaintedItem::hoverEnterEvent(event);
}

void OmniButton::hoverLeaveEvent(QHoverEvent *event) {
    m_isHovered = false; update();
    QQuickPaintedItem::hoverLeaveEvent(event);
}

void OmniButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) { m_isPressed = true; update(); }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniButton::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isPressed) {
        m_isPressed = false;
        update();
        emit clicked();
    }
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniButton::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();

    auto themeMgr = OmniThemeManager::instance();
    auto currentTheme = themeMgr->currentTheme();
    
    // --- JAVAFX CSS PARITY ENGINE OVERRIDES ---
    // If the developer passed inline styles, they override the global ThemeManager.
    QColor baseColor = m_omniStyle == "primary" ? themeMgr->primaryColor() : themeMgr->secondaryColor();
    baseColor = m_parsedStyle->getColor("-omni-background-color", baseColor);
    
    int radius = m_parsedStyle->getInt("-omni-border-radius", m_cornerRadius);

    if (m_isPressed) baseColor = baseColor.darker(120);
    else if (m_isHovered) baseColor = baseColor.lighter(110);

    QPainterPath path;
    
    if (currentTheme == OmniThemeManager::Cyberpunk) {
        path.moveTo(10, 0); path.lineTo(rect.width(), 0);
        path.lineTo(rect.width() - 10, rect.height()); path.lineTo(0, rect.height()); path.closeSubpath();
        
        painter->setPen(QPen(baseColor.darker(200), 2));
        painter->fillPath(path, QColor(0, 0, 0, 200)); 
        
        if (m_isHovered) {
            painter->setPen(QPen(baseColor, 2));
            painter->drawPath(path);
        }
        
    } else if (currentTheme == OmniThemeManager::LiquidGlass) {
        path.addRoundedRect(rect, radius, radius);
        painter->setPen(QPen(QColor(255,255,255, 100), 1));
        painter->fillPath(path, QColor(255, 255, 255, m_isHovered ? 40 : 20));
        
    } else if (currentTheme == OmniThemeManager::ChronosShift) {
        path.addRoundedRect(rect, 4, 4);
        painter->setPen(Qt::NoPen);
        QLinearGradient grad(0, 0, rect.width(), rect.height());
        grad.setColorAt(0, baseColor.lighter(110));
        grad.setColorAt(1, baseColor.darker(150));
        painter->fillPath(path, grad);
        
    } else {
        path.addRoundedRect(rect, radius, radius);
        painter->setPen(Qt::NoPen);
        painter->fillPath(path, baseColor);
    }

    QColor textColor = m_parsedStyle->getColor("-omni-text-color", themeMgr->textColor());
    if (currentTheme == OmniThemeManager::ChronosShift) textColor = QColor("#120024");
    
    painter->setPen(textColor);
    
    QFont f = painter->font();
    f.setPixelSize(14); 
    if (currentTheme == OmniThemeManager::Cyberpunk) f.setItalic(true); 
    
    // Apply inline font styles from CSS parity engine
    f = m_parsedStyle->getFont(f);
    painter->setFont(f);
    
    painter->drawText(rect, Qt::AlignCenter, m_text);
}
