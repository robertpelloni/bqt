#include "OmniWindow.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QDebug>
#include <QLinearGradient>
#include <QRadialGradient>

OmniWindow::OmniWindow(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_title("OmniWindow"),
      m_accentColor(QColor("#0078D7")),
      m_isMaximized(false),
      m_isDragging(false),
      m_isResizing(false),
      m_resizeEdge(0)
{
    setObjectName("OmniWindow");
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents, true);

    setWidth(400);
    setHeight(300);
    
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() {
        update(); // Force repaint when global theme changes
    });
}

OmniWindow::~OmniWindow() = default;

QString OmniWindow::title() const { return m_title; }
void OmniWindow::setTitle(const QString& title) {
    if (m_title != title) {
        m_title = title;
        emit titleChanged();
        update();
    }
}

QColor OmniWindow::accentColor() const { return m_accentColor; }
void OmniWindow::setAccentColor(const QColor& color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        emit accentColorChanged();
        update();
    }
}

bool OmniWindow::isMaximized() const { return m_isMaximized; }
void OmniWindow::setIsMaximized(bool max) {
    if (m_isMaximized != max) {
        m_isMaximized = max;
        
        if (m_isMaximized) {
            m_preMaximizeGeometry = QRectF(x(), y(), width(), height());
            if (parentItem()) {
                setX(0);
                setY(0);
                setWidth(parentItem()->width());
                setHeight(parentItem()->height());
            }
        } else {
            setX(m_preMaximizeGeometry.x());
            setY(m_preMaximizeGeometry.y());
            setWidth(m_preMaximizeGeometry.width());
            setHeight(m_preMaximizeGeometry.height());
        }
        
        emit isMaximizedChanged();
        update();
    }
}

void OmniWindow::close() {
    setVisible(false);
    emit windowClosed();
}

void OmniWindow::bringToFront() {
    if (parentItem()) {
        qreal maxZ = 0;
        for (auto* child : parentItem()->childItems()) {
            if (child != this && child->z() > maxZ) {
                maxZ = child->z();
            }
        }
        setZ(maxZ + 1);
    }
}

QRectF OmniWindow::titleBarRect() const {
    return QRectF(0, 0, width(), 30);
}

QRectF OmniWindow::closeButtonRect() const {
    return QRectF(width() - 30, 0, 30, 30);
}

void OmniWindow::hoverMoveEvent(QHoverEvent *event) {
    if (m_isMaximized) {
        QQuickPaintedItem::hoverMoveEvent(event);
        return;
    }

    QPointF pos = event->position();
    int edge = 0;
    
    if (pos.x() > width() - 10 && pos.y() > height() - 10) edge = 3;
    else if (pos.x() > width() - 5) edge = 1;
    else if (pos.y() > height() - 5) edge = 2;

    QQuickPaintedItem::hoverMoveEvent(event);
}

void OmniWindow::mousePressEvent(QMouseEvent *event) {
    bringToFront();
    forceActiveFocus();

    QPointF pos = event->position();
    
    if (closeButtonRect().contains(pos)) {
        close();
        return;
    }

    if (!m_isMaximized) {
        if (pos.x() > width() - 15 && pos.y() > height() - 15) {
            m_isResizing = true;
            m_resizeEdge = 3;
            m_dragStartPos = event->globalPosition();
        } else if (titleBarRect().contains(pos)) {
            m_isDragging = true;
            m_dragStartPos = event->globalPosition();
        }
    }

    QQuickPaintedItem::mousePressEvent(event);
}

void OmniWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPointF delta = event->globalPosition() - m_dragStartPos;
        setX(x() + delta.x());
        setY(y() + delta.y());
        m_dragStartPos = event->globalPosition();
    } else if (m_isResizing) {
        QPointF delta = event->globalPosition() - m_dragStartPos;
        setWidth(std::max(100.0, width() + delta.x()));
        setHeight(std::max(50.0, height() + delta.y()));
        m_dragStartPos = event->globalPosition();
        update();
    }

    QQuickPaintedItem::mouseMoveEvent(event);
}

void OmniWindow::mouseReleaseEvent(QMouseEvent *event) {
    m_isDragging = false;
    m_isResizing = false;
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniWindow::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    QRectF tBar = titleBarRect();
    QRectF cBtn = closeButtonRect();

    auto themeMgr = OmniThemeManager::instance();
    auto currentTheme = themeMgr->currentTheme();
    QColor themePrimary = themeMgr->primaryColor();
    QColor themeSurface = themeMgr->surfaceColor();
    QColor themeText = themeMgr->textColor();

    if (currentTheme == OmniThemeManager::Cyberpunk) {
        // Glow effect outer border
        painter->setPen(QPen(QColor(themePrimary.red(), themePrimary.green(), themePrimary.blue(), 100), 6));
        painter->drawRect(rect);
        
        // Inner Sharp border
        painter->setPen(QPen(themePrimary, 1));
        painter->fillRect(rect.adjusted(3, 3, -3, -3), QColor(10, 10, 15, 240));
        
        // Neon Trace Circuit Lines Background
        painter->setPen(QPen(QColor(0, 240, 255, 30), 1, Qt::DashLine));
        for (int i = 30; i < rect.height(); i += 20) {
            painter->drawLine(5, i, rect.width() - 5, i);
        }
        for (int i = 20; i < rect.width(); i += 20) {
            painter->drawLine(i, 30, i, rect.height() - 5);
        }
        
        // Cyberpunk Title Bar
        QLinearGradient grad(0, 0, rect.width(), 0);
        grad.setColorAt(0, themePrimary);
        grad.setColorAt(1, QColor("#00F0FF")); // Cyan fade
        painter->fillRect(tBar.adjusted(3, 3, -3, 0), grad);

    } else if (currentTheme == OmniThemeManager::LiquidGlass) {
        // Refractive Glass Look
        painter->setPen(QPen(QColor(255, 255, 255, 100), 1));
        painter->setBrush(themeSurface); // Highly transparent base
        QPainterPath glassPath;
        glassPath.addRoundedRect(rect, 8, 8);
        painter->drawPath(glassPath);
        
        // Glare/Highlight at the top edge
        QLinearGradient glare(0, 0, 0, 40);
        glare.setColorAt(0, QColor(255, 255, 255, 150));
        glare.setColorAt(1, QColor(255, 255, 255, 0));
        painter->fillRect(QRectF(1, 1, rect.width() - 2, 40), glare);

        // Glass Title Bar (invisible, just borders)
        painter->drawLine(0, 30, rect.width(), 30);
        
    } else if (currentTheme == OmniThemeManager::ChronosShift) {
        // ChronosShift (AI Invented) - Master Clock Temporal Energy
        painter->setPen(QPen(themePrimary, 2));
        
        QRadialGradient radial(rect.center(), rect.width());
        radial.setColorAt(0, QColor("#2D004D")); // Core temporal purple
        radial.setColorAt(1, QColor("#120024")); // Edge deep space
        painter->fillRect(rect, radial);
        
        // Title Bar
        painter->fillRect(tBar, themePrimary);
        
    } else {
        // Classic Dark/Light Mode
        painter->setPen(QColor(0, 0, 0, 150));
        painter->drawRect(rect);
        painter->fillRect(rect.adjusted(1, 1, -1, -1), themeSurface);
        painter->fillRect(tBar, themePrimary);
    }
    
    // Draw Title Text
    painter->setPen(currentTheme == OmniThemeManager::ChronosShift || currentTheme == OmniThemeManager::Light ? Qt::black : Qt::white);
    QFont font = painter->font();
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(tBar.adjusted(10, 0, -30, 0), Qt::AlignLeft | Qt::AlignVCenter, m_title);

    // Close Button (Consistent across themes for usability, styled slightly)
    if (currentTheme == OmniThemeManager::LiquidGlass) {
        painter->setPen(QColor(255, 255, 255, 150));
        painter->setBrush(QColor(255, 50, 50, 100)); // Transparent red
        painter->drawRoundedRect(cBtn.adjusted(5, 5,
