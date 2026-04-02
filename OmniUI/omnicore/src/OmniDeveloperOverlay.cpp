#include "OmniDeveloperOverlay.h"
#include "OmniInputManager.h"
#include "imgui.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMetaObject>
#include <QMetaProperty>

OmniDeveloperOverlay::OmniDeveloperOverlay(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // Redraw whenever ANY cursor moves
    connect(OmniInputManager::instance(), &OmniInputManager::cursorUpdated, this, [this]() {
        update(); 
    });
}

OmniDeveloperOverlay::~OmniDeveloperOverlay() = default;

void OmniDeveloperOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    // 1. Process ImGui Frame (Placeholder until Vulkan RHI is ready)
    ImGui::NewFrame();
    ImGui::Render();
    
    // 2. Render Virtual Cursors & Introspection Data
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    auto devices = OmniInputManager::instance()->devices();
    
    for (const auto& dev : devices) {
        if (dev.type == "mouse" || dev.type == "touch") {
            // A. Draw the Virtual Cursor
            painter.setBrush(Qt::red);
            painter.setPen(Qt::white);
            
            QPolygonF cursorPoly;
            cursorPoly << QPointF(dev.cursorPosition.x(), dev.cursorPosition.y())
                       << QPointF(dev.cursorPosition.x() + 15, dev.cursorPosition.y() + 10)
                       << QPointF(dev.cursorPosition.x() + 10, dev.cursorPosition.y() + 15);
            
            painter.drawPolygon(cursorPoly);
            
            // Draw the user/device name
            painter.setPen(QColor(255, 255, 0, 200)); // Yellow, slightly transparent
            painter.drawText(dev.cursorPosition + QPointF(20, 15), dev.name);

            // B. QMetaObject Introspection (The Ultimate Developer UX Tool)
            QObject* hoveredObj = OmniInputManager::instance()->deviceHover(dev.id);
            if (hoveredObj) {
                const QMetaObject* metaObj = hoveredObj->metaObject();
                QString className = metaObj->className();
                
                // Draw Tooltip Background
                QRectF tooltipRect(dev.cursorPosition.x() + 20, dev.cursorPosition.y() + 25, 250, 20);
                
                QStringList propertiesData;
                propertiesData << QString("Class: %1").arg(className);
                
                // Extract all Q_PROPERTY values dynamically
                for (int i = 0; i < metaObj->propertyCount(); ++i) {
                    QMetaProperty prop = metaObj->property(i);
                    QString propName = prop.name();
                    QString propValue = hoveredObj->property(propName.toUtf8()).toString();
                    
                    if (propValue.isEmpty() && hoveredObj->property(propName.toUtf8()).typeId() == QMetaType::QColor) {
                        propValue = hoveredObj->property(propName.toUtf8()).value<QColor>().name();
                    }
                    
                    propertiesData << QString("- %1: %2").arg(propName, propValue.isEmpty() ? "<null/empty>" : propValue);
                }

                tooltipRect.setHeight(propertiesData.size() * 15 + 10);
                
                painter.setBrush(QColor(0, 0, 0, 200)); // Dark semi-transparent
                painter.setPen(QColor(0, 255, 0, 150)); // Green border
                painter.drawRoundedRect(tooltipRect, 4, 4);
                
                // Draw extracted data
                painter.setPen(Qt::white);
                QFont font("Consolas", 9);
                painter.setFont(font);
                
                int yOffset = tooltipRect.top() + 15;
                for (int i = 0; i < propertiesData.size(); ++i) {
                    if (i == 0) painter.setPen(QColor("#00FFFF")); // Cyan for ClassName
                    else painter.setPen(QColor("#AAAAAA")); // Gray for Properties

                    painter.drawText(tooltipRect.left() + 5, yOffset, propertiesData[i]);
                    yOffset += 15;
                }
            }
        }
    }

    // Top-level diagnostic
    painter.setPen(QColor(0, 255, 0, 100));
    painter.drawText(10, 20, "OmniUI: Developer Layer & QMetaObject Introspection Active");
}

void OmniDeveloperOverlay::mousePressEvent(QMouseEvent *event)
{
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
