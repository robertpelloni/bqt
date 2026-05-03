#include "OmniDeveloperOverlay.h"
#include "OmniInputManager.h"
#include "OmniThemeManager.h"
#include "OmniNotificationCenter.h"
#include "imgui.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMetaObject>
#include <QMetaProperty>
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>
#include <BOBUIimer>

OmniDeveloperOverlay::OmniDeveloperOverlay(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(BobUI::WA_TransparentForMouseEvents, true);
    setAttribute(BobUI::WA_NoSystemBackground);
    setAttribute(BobUI::WA_TranslucentBackground);
    
    // Redraw whenever a cursor moves or a toast spawns
    connect(OmniInputManager::instance(), &OmniInputManager::cursorUpdated, this, [this]() { update(); });
    connect(OmniNotificationCenter::instance(), &OmniNotificationCenter::toastSpawned, this, [this](const OmniToast& toast) {
        update();
        BOBUIimer::singleShot(toast.durationMs, this, [this, toast]() {
            OmniNotificationCenter::instance()->removeToast(toast.id);
            update();
        });
    });
}

OmniDeveloperOverlay::~OmniDeveloperOverlay() = default;

void OmniDeveloperOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    // Process ImGui Frame
    ImGui::NewFrame();
    ImGui::Render();
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    qreal dpr = 1.0;
    if (auto* screen = QGuiApplication::primaryScreen()) {
        dpr = screen->devicePixelRatio();
    }

    // 1. Render Active Notifications (Toasts)
    auto toasts = OmniNotificationCenter::instance()->activeToasts();
    qreal toastYOffset = 20 * dpr;
    
    for (const auto& toast : toasts) {
        QRectF toastRect(width() - (320 * dpr), toastYOffset, 300 * dpr, 60 * dpr);
        
        // Dynamic coloring based on Theme Manager
        QColor bgColor = OmniThemeManager::instance()->surfaceColor();
        QColor accentColor = OmniThemeManager::instance()->primaryColor();
        
        if (toast.type == "error") accentColor = OmniThemeManager::instance()->dangerColor();
        else if (toast.type == "success") accentColor = OmniThemeManager::instance()->successColor();
        
        painter.setBrush(QColor(bgColor.red(), bgColor.green(), bgColor.blue(), 230));
        painter.setPen(accentColor);
        painter.drawRoundedRect(toastRect, 8 * dpr, 8 * dpr);
        
        painter.setPen(OmniThemeManager::instance()->textColor());
        QFont titleFont("Segoe UI", 12 * dpr, QFont::Bold);
        painter.setFont(titleFont);
        painter.drawText(toastRect.adjusted(15 * dpr, 10 * dpr, -10 * dpr, -30 * dpr), BobUI::AlignLeft | BobUI::AlignVCenter, toast.title);
        
        QFont msgFont("Segoe UI", 10 * dpr);
        painter.setFont(msgFont);
        painter.drawText(toastRect.adjusted(15 * dpr, 30 * dpr, -10 * dpr, -10 * dpr), BobUI::AlignLeft | BobUI::AlignVCenter, toast.message);
        
        toastYOffset += (70 * dpr);
    }

    // 2. Render Virtual Cursors & Introspection Data
    auto devices = OmniInputManager::instance()->devices();
    
    for (const auto& dev : devices) {
        if (dev.type == "mouse" || dev.type == "touch") {
            // A. Draw the Virtual Cursor scaled
            painter.setBrush(BobUI::red);
            painter.setPen(BobUI::white);
            
            QPolygonF cursorPoly;
            cursorPoly << QPointF(dev.cursorPosition.x(), dev.cursorPosition.y())
                       << QPointF(dev.cursorPosition.x() + (15 * dpr), dev.cursorPosition.y() + (10 * dpr))
                       << QPointF(dev.cursorPosition.x() + (10 * dpr), dev.cursorPosition.y() + (15 * dpr));
            
            painter.drawPolygon(cursorPoly);
            
            painter.setPen(QColor(255, 255, 0, 200)); 
            QFont nameFont("Consolas", 10 * dpr, QFont::Bold);
            painter.setFont(nameFont);
            painter.drawText(dev.cursorPosition + QPointF(20 * dpr, 15 * dpr), dev.name);

            // B. QMetaObject Introspection
            QObject* hoveredObj = OmniInputManager::instance()->deviceHover(dev.id);
            if (hoveredObj) {
                const QMetaObject* metaObj = hoveredObj->metaObject();
                QString className = metaObj->className();
                
                QRectF tooltipRect(dev.cursorPosition.x() + (20 * dpr), 
                                   dev.cursorPosition.y() + (25 * dpr), 
                                   250 * dpr, 20 * dpr);
                
                QStringList propertiesData;
                propertiesData << QString("Class: %1").arg(className);
                
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
                
                painter.setPen(BobUI::white);
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

    painter.setPen(QColor(0, 255, 0, 100));
    QFont diagFont("Consolas", 10 * dpr, QFont::Bold);
    painter.setFont(diagFont);
    painter.drawText(10 * dpr, 20 * dpr, "OmniUI: Developer Layer, Introspection, & Toast Center Active");
}

void OmniDeveloperOverlay::mousePressEvent(QMouseEvent *event) { QWidget::mousePressEvent(event); }
void OmniDeveloperOverlay::mouseReleaseEvent(QMouseEvent *event) { QWidget::mouseReleaseEvent(event); }
void OmniDeveloperOverlay::mouseMoveEvent(QMouseEvent *event) { QWidget::mouseMoveEvent(event); }
