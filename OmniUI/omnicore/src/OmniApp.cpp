#include "OmniApp.h"
#include "JuceWidget.h"
#include "OmniInputManager.h"
#include "OmniDeveloperOverlay.h"
#include "OmniQmlRegistration.h"
#include "OmniNativeEventFilter.h"
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
    
    // Install the native OS hardware interception filter
    m_nativeFilter = new OmniNativeEventFilter(inputManager());
    installNativeEventFilter(m_nativeFilter);
    m_nativeFilter->registerRawInput();

    // Register OmniUI Custom QML Types before loading anything
    OmniUI::registerQmlTypes();
}

OmniApplication::~OmniApplication()
{
    if (m_juceInitialized) {
        qDebug() << "OmniUI: Shutting down JUCE integration.";
    }
    delete m_nativeFilter;
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
        
        // Mock hardware extraction for Qt High-Level Events:
        // A true implementation correlates the RAWINPUT (caught by m_nativeFilter) 
        // with the timestamp of this QMouseEvent to assign the true hardware ID.
        QString deviceId = "sys-mouse-0"; 
        
        manager->updateCursor(deviceId, mouseEvent->globalPosition());

        if (e->type() == QEvent::MouseButtonPress) {
            manager->setDeviceFocus(deviceId, receiver);
        }
    }
    else if (e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease) {
        auto *keyEvent = static_cast<QKeyEvent*>(e);
        QString deviceId = "sys-kb-0"; 
        
        if (manager->routeKeyEvent(deviceId, keyEvent)) {
            return true;
        }
    }
    else if (e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchUpdate || e->type() == QEvent::TouchEnd) {
        auto *touchEvent = static_cast<QTouchEvent*>(e);
        for (const auto& point : touchEvent->points()) {
            QString deviceId = QString("sys-touch-%1").arg(point.id());
            manager->registerDevice(deviceId, "Touch Point", "touch");
            manager->updateCursor(deviceId, point.globalPosition());
        }
    }

    return QApplication::notify(receiver, e);
}
