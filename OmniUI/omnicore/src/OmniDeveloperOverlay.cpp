#include "OmniDeveloperOverlay.h"
#include "OmniInputManager.h"
#include "imgui.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

OmniDeveloperOverlay::OmniDeveloperOverlay(QWidget *parent)
    : QWidget(parent)
{
    // We want the overlay to cover the whole screen/window but let events pass through
    // to the Qt widgets beneath unless we explicitly intercept them for developer tools.
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // Connect to the input manager so we can redraw when virtual cursors move
    connect(OmniInputManager::instance(), &OmniInputManager::cursorUpdated, this, [this](const QString& id, const QPointF& pos) {
        Q_UNUSED(id);
        Q_UNUSED(pos);
        update(); // Trigger a paint event to redraw cursors
    });
}

OmniDeveloperOverlay::~OmniDeveloperOverlay() = default;

void OmniDeveloperOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    // 1. Process ImGui Frame
    ImGui::NewFrame();
    // Here we would define our OS developer tools/console windows
    ImGui::Render();
    
    // 2. Render Virtual Cursors (Multi-user visual representation)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    auto devices = OmniInputManager::instance()->devices();
    
    for (const auto& dev : devices) {
        if (dev.type == "mouse" || dev.type == "touch") {
            // Draw a distinct cursor for each device
            painter.setBrush(Qt::red); // Color could be hashed from deviceId
            painter.setPen(Qt::white);
            
            // Draw a simple triangle for the cursor
            QPolygonF cursorPoly;
            cursorPoly << QPointF(dev.cursorPosition.x(), dev.cursorPosition.y())
                       << QPointF(dev.cursorPosition.x() + 15, dev.cursorPosition.y() + 10)
                       << QPointF(dev.cursorPosition.x() + 10, dev.cursorPosition.y() + 15);
            
            painter.drawPolygon(cursorPoly);
            
            // Draw the user/device name
            painter.setPen(Qt::yellow);
            painter.drawText(dev.cursorPosition + QPointF(20, 20), dev.name);
        }
    }

    // Optional: Draw developer diagnostic overlay text
    painter.setPen(Qt::green);
    painter.drawText(10, 20, "OmniUI: Developer Layer & Multi-Cursor System Active");
}

void OmniDeveloperOverlay::mousePressEvent(QMouseEvent *event)
{
    // Route to ImGui IO for interaction with overlay tools
    // qDebug() << "OmniUI: Developer Overlay Intercept at:" << event->pos();
    QWidget::mousePressEvent(event);
}

void OmniDeveloperOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

void OmniDeveloperOverlay::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}
