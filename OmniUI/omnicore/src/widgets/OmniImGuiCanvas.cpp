#include "OmniImGuiCanvas.h"
#include "OmniThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>

OmniImGuiCanvas::OmniImGuiCanvas(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_mouseDown(false),
      m_mouseClicked(false),
      m_currentY(0),
      m_currentPainter(nullptr)
{
    setObjectName("OmniImGuiCanvas");
    setAcceptedMouseButtons(BobUI::LeftButton);
    setFlag(ItemHasContents, true);
    setWidth(400);
    setHeight(400);
}

OmniImGuiCanvas::~OmniImGuiCanvas() = default;

QJSValue OmniImGuiCanvas::onPaint() const { return m_onPaintCallback; }
void OmniImGuiCanvas::setOnPaint(const QJSValue& callback) {
    if (!callback.equals(m_onPaintCallback)) {
        m_onPaintCallback = callback;
        emit onPaintChanged();
        update(); // Trigger the first Immediate Mode frame
    }
}

void OmniImGuiCanvas::mousePressEvent(QMouseEvent *event) {
    m_mouseDown = true;
    m_mouseClicked = true;
    m_mousePos = event->position();
    update();
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniImGuiCanvas::mouseReleaseEvent(QMouseEvent *event) {
    m_mouseDown = false;
    update();
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void OmniImGuiCanvas::mouseMoveEvent(QMouseEvent *event) {
    m_mousePos = event->position();
    update();
    QQuickPaintedItem::mouseMoveEvent(event);
}

// Immediate Mode API Implementation
bool OmniImGuiCanvas::button(const QString& label) {
    if (!m_currentPainter) return false;

    auto theme = OmniThemeManager::instance();
    QRectF btnRect(10, m_currentY, 150, 30);
    
    bool isHovered = btnRect.contains(m_mousePos);
    bool isPressed = isHovered && m_mouseDown;
    bool isClicked = isHovered && m_mouseClicked; // True only on the frame the click started

    // Render Immediate Mode Button
    QColor color = isPressed ? theme->primaryColor().darker() : 
                   (isHovered ? theme->primaryColor().lighter() : theme->primaryColor());

    QPainterPath path;
    path.addRoundedRect(btnRect, 4, 4);
    m_currentPainter->fillPath(path, color);

    m_currentPainter->setPen(theme->textColor());
    m_currentPainter->drawText(btnRect, BobUI::AlignCenter, label);

    m_currentY += 40; // Advance layout cursor
    return isClicked;
}

void OmniImGuiCanvas::text(const QString& label) {
    if (!m_currentPainter) return;
    
    auto theme = OmniThemeManager::instance();
    m_currentPainter->setPen(theme->textColor());
    m_currentPainter->drawText(QRectF(10, m_currentY, width(), 20), BobUI::AlignLeft | BobUI::AlignVCenter, label);
    
    m_currentY += 30; // Advance layout cursor
}

void OmniImGuiCanvas::paint(QPainter *painter) {
    if (!m_onPaintCallback.isCallable()) return;

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillRect(boundingRect(), OmniThemeManager::instance()->surfaceColor());

    // Setup Immediate Mode state
    m_currentPainter = painter;
    m_currentY = 10;

    // Execute the Javascript callback (which natively invokes the IMGUI functions on this object)
    QJSValueList args;
    m_onPaintCallback.call(args);

    // Reset IMGUI State for next frame
    m_currentPainter = nullptr;
    m_mouseClicked = false; // Only true for one single frame
}
