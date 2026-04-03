#include "OmniWindow.h"
#include "OmniThemeManager.h"
#include "OmniUserManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

OmniWindow::OmniWindow(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_title("OmniWindow"), m_accentColor(QColor("#0078D7")),
      m_isMaximized(false), m_collaborative(false), m_isModal(false),
      m_isDragging(false), m_isResizing(false), m_resizeEdge(0)
{
    setObjectName("OmniWindow");
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemHasContents, true);
    connect(OmniThemeManager::instance(), &OmniThemeManager::themeChanged, this, [this]() { update(); });
}

OmniWindow::~OmniWindow() = default;

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
bool OmniWindow::isModal() const { return m_isModal; }
void OmniWindow::setIsModal(bool modal) { m_isModal = modal; emit isModalChanged(); }

void OmniWindow::close() { setVisible(false); emit windowClosed(); }
void OmniWindow::bringToFront() { setZ(z() + 1); }

bool OmniWindow::checkInteractionPermission(QMouseEvent* event) {
    QString deviceId = property("_omni_active_device").toString();
    QString userId = OmniUserManager::instance()->getUserIdForDevice(deviceId);

    // --- MODAL PINNING LOGIC ---
    // If the window is a Modal, ONLY the owner user can interact.
    if (m_isModal && !m_ownerId.isEmpty() && m_ownerId != userId) {
        return false;
    }

    // Collaborative logic for normal windows
    if (m_collaborative) return true;
    if (m_ownerId.isEmpty() || m_ownerId == userId) return true;

    return false;
}

void OmniWindow::mousePressEvent(QMouseEvent *event) {
    if (!checkInteractionPermission(event)) return;

    bringToFront();
    forceActiveFocus();
    QPointF pos = event->position();
    
    if (pos.x() > width() - 30 && pos.y() < 30) { close(); return; }

    if (!m_isMaximized) {
        if (pos.x() > width() - 15 && pos.y() > height() - 15) {
            m_isResizing = true; m_dragStartPos = event->globalPosition();
        } else if (pos.y() < 30) {
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

void OmniWindow::paint(QPainter *painter) {
    painter->fillRect(boundingRect(), OmniThemeManager::instance()->surfaceColor());
    painter->fillRect(QRectF(0,0,width(),30), m_accentColor);
    painter->setPen(Qt::white);
    painter->drawText(QRectF(10,0,width()-40,30), Qt::AlignVCenter, m_title);
}
