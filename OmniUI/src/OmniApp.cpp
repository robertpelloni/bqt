#include "OmniApp.h"
#include "JuceWidget.h"
#include "OmniInputManager.h"
#include "OmniDeveloperOverlay.h"
#include <QDebug>

// Stub implementation for Juce initialization
namespace juce {
    void initialiseJuce_GUI();
    void shutdownJuce_GUI();
}

OmniApplication::OmniApplication(int &argc, char **argv)
    : QApplication(argc, argv), m_juceInitialized(false), m_developerOverlay(nullptr)
{
    setObjectName("OmniApplication");
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
    // For now, we simulate success.
    m_juceInitialized = true;
    qDebug() << "OmniUI: JUCE integration initialized successfully.";
}

bool OmniApplication::loadMainSource(const QString& sourcePath)
{
    qDebug() << "OmniUI: Loading main source from" << sourcePath;
    
    // In a full implementation, we would instantiate a QQmlApplicationEngine
    // and load the JS/QML payload here.
    
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