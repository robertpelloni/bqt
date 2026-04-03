#include "OmniDeveloperOverlay.h"
#include "OmniInputManager.h"
#include "imgui.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMetaObject>
#include <QMetaProperty>
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>

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
    
    // 1. Process ImGui Frame
    ImGui::NewFrame();
    ImGui::Render();
    
    // 2. Render Virtual Cursors & Introspection Data
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fetch Device Pixel Ratio for High-DPI (4K) screen scaling
    qreal dpr = 1.0;
    if (auto* screen = QGuiApplication::primaryScreen()) {
        dpr = screen->devicePixelRatio();
    }

    auto devices = OmniInputManager::instance()->devices();
    
    for (const auto& dev : devices) {
        if (dev.type == "mouse" || dev.type == "touch") {
            // A. Draw the Virtual Cursor scaled
            painter.setBrush(Qt::red);
            painter.setPen(Qt::white);
            
            QPolygonF cursorPoly;
            cursorPoly << QPointF(dev.cursorPosition.x(), dev.cursorPosition.y())
                       << QPointF(dev.cursorPosition.x() + (15 * dpr), dev.cursorPosition.y() + (10 * dpr))
                       << QPointF(dev.cursorPosition.x() + (10 * dpr), dev.cursorPosition.y() + (15 * dpr));
            
            painter.drawPolygon(cursorPoly);
            
            // Draw the user/device name
            painter.setPen(QColor(255, 255, 0, 200)); 
            
            QFont nameFont("Consolas", 10 * dpr, QFont::Bold);
            painter.setFont(nameFont);
            painter.drawText(dev.cursorPosition + QPointF(20 * dpr, 15 * dpr), dev.name);

            // B. QMetaObject Introspection
            QObject* hoveredObj = OmniInputManager::instance()->deviceHover(dev.id);
            if (hoveredObj) {
                const QMetaObject* metaObj = hoveredObj->metaObject();
                QString className = metaObj->className();
                
                // Draw Tooltip Background
                QRectF tooltipRect(dev.cursorPosition.x() + (20 * dpr), 
                                   dev.cursorPosition.y() + (25 * dpr), 
                                   250 * dpr, 20 * dpr);
                
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

                tooltipRect.setHeight((propertiesData.size() * 15 * dpr) + (10 * dpr));
                
                painter.setBrush(QColor(0, 0, 0, 200)); 
                painter.setPen(QColor(0, 255, 0, 150)); 
                painter.drawRoundedRect(tooltipRect, 4 * dpr, 4 * dpr);
                
                // Draw extracted data
                painter.setPen(Qt::white);
                QFont toolFont("Consolas", 9 * dpr);
                painter.setFont(toolFont);
                
                qreal yOffset = tooltipRect.top() + (15 * dpr);
                for (int i = 0; i < propertiesData.size(); ++i) {
                    if (i == 0) painter.setPen(QColor("#00FFFF")); // Cyan for ClassName
                    else painter.setPen(QColor("#AAAAAA")); // Gray for Properties

                    painter.drawText(tooltipRect.left() + (5 * dpr), yOffset, propertiesData[i]);
                    yOffset += (15 * dpr);
                }
            }
        }
    }

    // Top-level diagnostic
    painter.setPen(QColor(0, 255, 0, 100));
    QFont diagFont("Consolas", 10 * dpr, QFont::Bold);
    painter.setFont(diagFont);
    painter.drawText(10 * dpr, 20 * dpr, "OmniUI: Developer Layer & Introspection Active [Thread Safe]");
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
