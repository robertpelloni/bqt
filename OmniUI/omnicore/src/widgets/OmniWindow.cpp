#include "OmniWindow.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QDebug>

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
        // Find highest Z among siblings
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
    
    if (pos.x() > width() - 10 && pos.y() > height() - 10) edge = 3; // BR
    else if (pos.x() > width() - 5) edge = 1; // R
    else if (pos.y() > height() - 5) edge = 2; // B

    // In a full implementation, we would set the cursor shape here via QCursor
    
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

    // Drop Shadow / Border Simulation
    painter->setPen(QColor(0, 0, 0, 150));
    painter->drawRect(rect);

    // Background
    painter->fillRect(rect.adjusted(1, 1, -1, -1), QColor("#1E1E1E"));

    // Title Bar
    QRectF tBar = titleBarRect();
    painter->fillRect(tBar, m_accentColor);
    
    // Title Text
    painter->setPen(Qt::white);
    QFont font = painter->font();
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(tBar.adjusted(10, 0, -30, 0), Qt::AlignLeft | Qt::AlignVCenter, m_title);

    // Close Button
    QRectF cBtn = closeButtonRect();
    painter->fillRect(cBtn, QColor("#D32F2F")); // Red
    painter->drawText(cBtn, Qt::AlignCenter, "X");

    // Resize Grip (if not maximized)
    if (!m_isMaximized) {
        painter->setPen(QColor("#555555"));
        painter->drawLine(rect.right() - 10, rect.bottom() - 2, rect.right() - 2, rect.bottom() - 10);
        painter->drawLine(rect.right() - 6, rect.bottom() - 2, rect.right() - 2, rect.bottom() - 6);
    }
}
