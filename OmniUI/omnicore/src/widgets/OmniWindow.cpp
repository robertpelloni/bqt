#include "OmniWindow.h"
#include "OmniThemeManager.h"
#include "OmniUserManager.h"
#include "OmniInputManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QDebug>
#include <QLinearGradient>

OmniWindow::OmniWindow(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_title("OmniWindow"),
      m_accentColor(QColor("#0078D7")),
      m_isMaximized(false),
      m_collaborative(false),
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
    
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

OmniWindow::~OmniWindow() = default;

QString OmniWindow::title() const { return m_title; }
void OmniWindow::setTitle(const QString& title) {
    if (m_title != title) { m_title = title; emit titleChanged(); update(); }
}

QColor OmniWindow::accentColor() const { return m_accentColor; }
void OmniWindow::setAccentColor(const QColor& color) {
    if (m_accentColor != color) { m_accentColor = color; emit accentColorChanged(); update(); }
}

bool OmniWindow::isMaximized() const { return m_isMaximized; }
void OmniWindow::setIsMaximized(bool max) {
    if (m_isMaximized != max) {
        m_isMaximized = max;
        if (m_isMaximized) {
            m_preMaximizeGeometry = QRectF(x(), y(), width(), height());
            if (parentItem()) {
                setX(0); setY(0);
                setWidth(parentItem()->width()); setHeight(parentItem()->height());
            }
        } else {
            setX(m_preMaximizeGeometry.x()); setY(m_preMaximizeGeometry.y());
            setWidth(m_preMaximizeGeometry.width()); setHeight(m_preMaximizeGeometry.height());
        }
        emit isMaximizedChanged(); update();
    }
}

QString OmniWindow::ownerId() const { return m_ownerId; }
void OmniWindow::setOwnerId(const QString& id) {
    if (m_ownerId != id) { m_ownerId = id; emit ownerIdChanged(); }
}

bool OmniWindow::collaborative() const { return m_collaborative; }
void OmniWindow::setCollaborative(bool collabo) {
    if (m_collaborative != collabo) { m_collaborative = collabo; emit collaborativeChanged(); }
}

void OmniWindow::close() { setVisible(false); emit windowClosed(); }

void OmniWindow::bringToFront() {
    if (parentItem()) {
        qreal maxZ = 0;
        for (auto* child : parentItem()->childItems()) {
            if (child != this && child->z() > maxZ) maxZ = child->z();
        }
        setZ(maxZ + 1);
    }
}

bool OmniWindow::checkInteractionPermission(QMouseEvent* event) {
    if (m_ownerId.isEmpty() || m_collaborative) return true;

    // This logic relies on the Multi-Cursor routing we built.
    // In a real run, we'd need to know WHICH device triggered this event.
    // Since Qt events don't natively carry device IDs yet, we assume the last active device 
    // from OmniInputManager for this interaction check (Simulation).
    
    // TODO: Link raw Win32 MSG pointer to this specific QMouseEvent to get true DeviceId.
    return true; 
}

void OmniWindow::mousePressEvent(QMouseEvent *event) {
    if (!checkInteractionPermission(event)) {
        emit permissionDenied("UnknownUser");
        return;
    }

    bringToFront();
    forceActiveFocus();
    QPointF pos = event->position();
    
    if (closeButtonRect().contains(pos)) { close(); return; }

    if (!m_isMaximized) {
        if (pos.x() > width() - 15 && pos.y() > height() - 15) {
            m_isResizing = true; m_resizeEdge = 3;
            m_dragStartPos = event->globalPosition();
        } else if (titleBarRect().contains(pos)) {
            m_isDragging = true; m_dragStartPos = event->globalPosition();
        }
    }
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPointF delta = event->globalPosition() - m_dragStartPos;
        setX(x() + delta.x()); setY(y() + delta.y());
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
    m_isDragging = false; m_isResizing = false;
    QQuickPaintedItem::mouseReleaseEvent(event);
}

QRectF OmniWindow::titleBarRect() const { return QRectF(0, 0, width(), 30); }
QRectF OmniWindow::closeButtonRect() const { return QRectF(width() - 30, 0, 30, 30); }

void OmniWindow::hoverMoveEvent(QHoverEvent *event) { QQuickPaintedItem::hoverMoveEvent(event); }

void OmniWindow::paint(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF rect = boundingRect();
    QRectF tBar = titleBarRect();
    QRectF cBtn = closeButtonRect();

    auto themeMgr = OmniThemeManager::instance();
    painter->setPen(QColor(0, 0, 0, 150));
    painter->drawRect(rect);
    painter->fillRect(rect.adjusted(1, 1, -1, -1), themeMgr->surfaceColor());
    painter->fillRect(tBar, themeMgr->primaryColor());
    
    painter->setPen(Qt::white);
    QFont font = painter->font(); font.setBold(true); painter->setFont(font);
    painter->drawText(tBar.adjusted(10, 0, -30, 0), Qt::AlignLeft | Qt::AlignVCenter, m_title);

    painter->fillRect(cBtn, themeMgr->dangerColor());
    painter->drawText(cBtn, Qt::AlignCenter, "X");
}
