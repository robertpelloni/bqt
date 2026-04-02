#include "OmniDeveloperOverlay.h"
#include "imgui.h"
#include <QPainter>
#include <QDebug>

OmniDeveloperOverlay::OmniDeveloperOverlay(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // In a real implementation, we would bridge ImGui's draw data
    // directly into a Qt RHI / OpenGL context.
}

OmniDeveloperOverlay::~OmniDeveloperOverlay() = default;

void OmniDeveloperOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    ImGui::NewFrame();
    // Here we would define our OS developer tools/console windows
    qDebug() << "OmniUI: ImGui Developer Frame Processing...";
    ImGui::Render();
    
    QPainter painter(this);
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawRect(rect());
    painter.setPen(Qt::green);
    painter.drawText(10, 20, "OmniUI: Developer Layer Active (ImGui-Powered)");
}

void OmniDeveloperOverlay::mousePressEvent(QMouseEvent *event)
{
    // Route to ImGui IO for interaction with overlay tools
    qDebug() << "OmniUI: Developer Overlay Intercept at:" << event->pos();
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