#include "OmniApp.h"
#include "JuceWidget.h"
#include "OmniInputManager.h"
#include "OmniDeveloperOverlay.h"
#include "OmniQmlRegistration.h"
#include <QQmlApplicationEngine>
#include <QMouseEvent>
#include <QKeyEvent>
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
    
    // Register OmniUI Custom QML Types before loading anything
    OmniUI::registerQmlTypes();
}

OmniApplication::~OmniApplication()
{
    if (m_juceInitialized) {
        // juce::shutdownJuce_GUI();
        qDebug() << "OmniUI: Shutting down JUCE integration.";
    }
}

void OmniApplication::initializeJuce()
{
    if (m_juceInitialized) return;
    
    // Call to juce::initialiseJuce_GUI() would go here.
    m_juceInitialized = true;
    qDebug() << "OmniUI: JUCE integration initialized successfully.";
}

bool OmniApplication::loadMainSource(const QString& sourcePath)
{
    qDebug() << "OmniUI: Loading main source from" << sourcePath;
    
    const QUrl url = QUrl::fromLocalFile(sourcePath);
    QObject::connect(m_qmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "OmniUI: Failed to load QML source!" << url;
            QCoreApplication::exit(-1);
        } else {
            qDebug() << "OmniUI: QML source loaded successfully.";
        }
    }, Qt::QueuedConnection);
    
    m_qmlEngine->load(url);
    return true;
}

JuceWidget* OmniApplication::createJuceWidget(QWidget* parent)
{
    if (!m_juceInitialized) {
        initializeJuce();
    }
    
    auto* widget = new JuceWidget(parent);
    m_managedWidgets.append(widget);
    
    qDebug() << "OmniUI: Created new JuceWidget instance.";
    return widget;
}

void OmniApplication::setDeveloperOverlayEnabled(bool enabled)
{
    if (enabled) {
        if (!m_developerOverlay) {
            m_developerOverlay = new OmniDeveloperOverlay();
        }
        m_developerOverlay->show();
    } else if (m_developerOverlay) {
        m_developerOverlay->hide();
    }
    
    qDebug() << "OmniUI: Developer Overlay set to" << enabled;
}

OmniInputManager* OmniApplication::inputManager() const
{
    return OmniInputManager::instance();
}

bool OmniApplication::notify(QObject *receiver, QEvent *e)
{
    // MULTI-CURSOR ROUTING INTERCEPTION
    // Here we intercept events at the absolute lowest level before they reach widgets.
    if (e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent*>(e);
        
        // In a true raw-input OS implementation, we would extract the hardware device ID 
        // from the native event payload attached to QMouseEvent.
        // For now, we simulate tagging it to the "sys-mouse-0" default device.
        QString deviceId = "sys-mouse-0";
        
        // Feed the position back into the input manager so the overlay can draw the virtual cursor
        OmniInputManager::instance()->updateCursor(deviceId, mouseEvent->globalPosition());
        
        // TODO: Implement focus-tree routing based on deviceId instead of global focus.
    }

    // Pass the event down the normal Qt chain
    return QApplication::notify(receiver, e);
}
