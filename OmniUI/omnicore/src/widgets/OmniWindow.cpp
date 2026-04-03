#include "OmniWindow.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QRadialGradient>

OmniWindow::OmniWindow(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_title("OmniWindow"), m_accentColor(QColor("#0078D7")),
      m_isMaximized(false), m_collaborative(false),
      m_isDragging(false), m_isResizing(false), m_resizeEdge(0)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemHasContents, true);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

OmniWindow::~OmniWindow() = default;

void OmniWindow::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    auto theme = OmniThemeManager::instance();
    auto mode = theme->currentTheme();

    if (mode == OmniThemeManager::Cyberpunk) {
        // --- CYBERPUNK RENDER PASS ---
        // 1. Draw Background with Angled Cutoff
        QPainterPath bodyPath;
        bodyPath.moveTo(20, 0); bodyPath.lineTo(rect.width(), 0);
        bodyPath.lineTo(rect.width(), rect.height() - 20);
        bodyPath.lineTo(rect.width() - 20, rect.height());
        bodyPath.lineTo(0, rect.height()); bodyPath.lineTo(0, 20); bodyPath.closeSubpath();
        
        painter->fillPath(bodyPath, theme->surface());

        // 2. Draw Neon Glow Border (Double Stroke)
        painter->setPen(QPen(QColor(theme->primary().red(), theme->primary().green(), theme->primary().blue(), 60), 4));
        painter->drawPath(bodyPath);
        painter->setPen(QPen(theme->primary(), 1.5));
        painter->drawPath(bodyPath);

        // 3. Draw Procedural Circuit Traces
        painter->setPen(QPen(QColor(0, 240, 255, 30), 1));
        painter->drawLine(10, 50, 100, 50); painter->drawLine(100, 50, 100, 150);
        painter->drawLine(rect.width() - 50, 20, rect.width() - 50, 100);

        // 4. Cyberpunk Title Bar
        painter->fillRect(QRectF(0, 0, rect.width(), 30), theme->primary());
        painter->setPen(Qt::black);
        painter->drawText(QRectF(10, 0, rect.width() - 40, 30), Qt::AlignVCenter, m_title.toUpper());

    } else if (mode == OmniThemeManager::LiquidGlass) {
        // --- LIQUID GLASS RENDER PASS ---
        QPainterPath glassPath; glassPath.addRoundedRect(rect, 12, 12);
        
        // Background with refractive tint
        QLinearGradient grad(0,0,0,rect.height());
        grad.setColorAt(0, QColor(255,255,255, 40));
        grad.setColorAt(1, QColor(200,200,255, 20));
        painter->fillPath(glassPath, grad);

        // Fresnel Border
        painter->setPen(QPen(QColor(255,255,255, 120), 1));
        painter->drawPath(glassPath);

        painter->setPen(theme->text());
        painter->drawText(QRectF(15, 0, rect.width(), 30), Qt::AlignVCenter, m_title);

    } else if (mode == OmniThemeManager::Aetheria) {
        // --- AETHERIA CELESTIAL RENDER PASS ---
        QRadialGradient cosmic(rect.center(), rect.width());
        cosmic.setColorAt(0, QColor("#2D0B5E"));
        cosmic.setColorAt(1, QColor("#0A0214"));
        painter->fillRect(rect, cosmic);

        // Golden Filigree Border
        painter->setPen(QPen(theme->primary(), 2));
        painter->drawRoundedRect(rect, 4, 4);

        painter->setPen(theme->primary());
        painter->drawText(QRectF(10, 0, rect.width(), 30), Qt::AlignVCenter, "◈ " + m_title + " ◈");
    } else {
        // Standard Dark/Light
        painter->fillRect(rect, theme->surface());
        painter->fillRect(QRectF(0,0,width(),30), theme->primary());
        painter->setPen(theme->text());
        painter->drawText(QRectF(10,0,width(),30), Qt::AlignVCenter, m_title);
    }
}

// Logic implementations omitted for brevity but preserved in kernel...
QString OmniWindow::title() const { return m_title; }
void OmniWindow::setTitle(const QString& title) { m_title = title; update(); }
QColor OmniWindow::accentColor() const { return m_accentColor; }
void OmniWindow::setAccentColor(const QColor& color) { m_accentColor = color; update(); }
bool OmniWindow::isMaximized() const { return m_isMaximized; }
void OmniWindow::setIsMaximized(bool max) { m_isMaximized = max; update(); }
QString OmniWindow::ownerId() const { return m_ownerId; }
void OmniWindow::setOwnerId(const QString& id) { m_ownerId = id; }
bool OmniWindow::collaborative() const { return m_collaborative; }
void OmniWindow::setCollaborative(bool collabo) { m_collaborative = collabo; }
QString OmniWindow::exclusiveDeviceId() const { return m_exclusive_id; }
void OmniWindow::setExclusiveDeviceId(const QString& id) { m_exclusive_id = id; }
void OmniWindow::close() { setVisible(false); }
void OmniWindow::bringToFront() { setZ(z() + 1); }
void OmniWindow::mousePressEvent(QMouseEvent *e) { QQuickPaintedItem::mousePressEvent(e); }
void OmniWindow::mouseMoveEvent(QMouseEvent *e) { QQuickPaintedItem::mouseMoveEvent(e); }
void OmniWindow::mouseReleaseEvent(QMouseEvent *e) { QQuickPaintedItem::mouseReleaseEvent(e); }
void OmniWindow::hoverMoveEvent(QHoverEvent *e) { QQuickPaintedItem::hoverMoveEvent(e); }
