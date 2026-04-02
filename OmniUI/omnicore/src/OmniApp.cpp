#include "OmniApp.h"
#include "JuceWidget.h"
#include "OmniInputManager.h"
#include "OmniDeveloperOverlay.h"
#include "OmniQmlRegistration.h"
#include <QQmlApplicationEngine>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTouchEvent>
#include <QDebug>
#include <QUrl>

// Stub implementation for Juce initialization
namespace juce {
    void initialiseJuce_GUI();
    void shutdownJuce_GUI();
}

OmniApplication::OmniApplication(int &argc, char **argv)
    : QApplication(argc, argv), m_juceInitialized(false), m_developerOverlay(nullptr)
{
    setObjectName("OmniApplication");
    m_qmlEngine = new QQmlApplicationEngine(this);
    
    OmniUI::registerQmlTypes();
}

OmniApplication::~OmniApplication()
{
    if (m_juceInitialized) {
        qDebug() << "OmniUI: Shutting down JUCE integration.";
    }
}

void OmniApplication::initializeJuce()
{
    if (m_juceInitialized) return;
    m_juceInitialized = true;
    qDebug() << "OmniUI: JUCE integration initialized successfully.";
}

bool OmniApplication::loadMainSource(const QString& sourcePath)
{
    const QUrl url = QUrl::fromLocalFile(sourcePath);
    QObject::connect(m_qmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "OmniUI: Failed to load QML source!" << url;
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    
    m_qmlEngine->load(url);
    return true;
}

JuceWidget* OmniApplication::createJuceWidget(QWidget* parent)
{
    if (!m_juceInitialized) initializeJuce();
    auto* widget = new JuceWidget(parent);
    m_managedWidgets.append(widget);
    return widget;
}

void OmniApplication::setDeveloperOverlayEnabled(bool enabled)
{
    if (enabled) {
        if (!m_developerOverlay) m_developerOverlay = new OmniDeveloperOverlay();
        m_developerOverlay->show();
    } else if (m_developerOverlay) {
        m_developerOverlay->hide();
    }
}

OmniInputManager* OmniApplication::inputManager() const
{
    return OmniInputManager::instance();
}

bool OmniApplication::notify(QObject *receiver, QEvent *e)
{
    // MULTI-CURSOR & MULTI-FOCUS ROUTING INTERCEPTION
    auto manager = OmniInputManager::instance();

    if (e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent*>(e);
        
        // Mock hardware extraction:
        // A real implementation would parse raw input handles.
        QString deviceId = "sys-mouse-0"; 
        
        manager->updateCursor(deviceId, mouseEvent->globalPosition());

        // On Mouse Press, we automatically assign this device's focus to the widget under the cursor.
        if (e->type() == QEvent::MouseButtonPress) {
            manager->setDeviceFocus(deviceId, receiver);
        }
    }
    else if (e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease) {
        auto *keyEvent = static_cast<QKeyEvent*>(e);
        
        // Mock hardware extraction:
        // A real implementation maps raw USB endpoints to these IDs.
        QString deviceId = "sys-kb-0"; 
        
        // Try to route directly to the device's independent focus target
        if (manager->routeKeyEvent(deviceId, keyEvent)) {
            // If successfully routed, we consume the event so standard Qt global focus doesn't double-process it.
            return true;
        }
    }
    else if (e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchUpdate || e->type() == QEvent::TouchEnd) {
        // Multi-touch implicitly supports multi-cursor routing natively, 
        // we map TouchPoints to Virtual Cursors here.
        auto *touchEvent = static_cast<QTouchEvent*>(e);
        for (const auto& point : touchEvent->points()) {
            QString deviceId = QString("sys-touch-%1").arg(point.id());
            manager->registerDevice(deviceId, "Touch Point", "touch");
            manager->updateCursor(deviceId, point.globalPosition());
        }
    }

    // Pass the event down the normal Qt chain if not consumed
    return QApplication::notify(receiver, e);
}
