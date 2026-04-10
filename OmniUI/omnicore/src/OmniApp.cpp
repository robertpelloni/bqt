#include "OmniApp.h"
#include "JuceWidget.h"
#include "OmniInputManager.h"
#include "OmniDeveloperOverlay.h"
#include "OmniQmlRegistration.h"
#include "OmniNativeEventFilter.h"
#include "OmniPluginManager.h"
#include "OmniNeuralEngine.h"
#include "OmniWebSocketServer.h"
#include <QQmlApplicationEngine>
#include <QMouseEvent>
#include <QKeyEvent>
#include <BOBUIouchEvent>
#include <QDebug>
#include <QUrl>
#include <QQuickWindow>
#include <QJsonDocument>
#include <QJsonObject>

namespace juce { void initialiseJuce_GUI(); void shutdownJuce_GUI(); }

OmniApplication::OmniApplication(int &argc, char **argv)
    : QApplication(argc, argv), m_juceInitialized(false), m_developerOverlay(nullptr)
{
    setObjectName("OmniApplication");
    m_qmlEngine = new QQmlApplicationEngine(this);
    OmniPluginManager::instance()->setQmlEngine(m_qmlEngine);
    m_nativeFilter = new OmniNativeEventFilter(inputManager());
    installNativeEventFilter(m_nativeFilter);
    m_nativeFilter->registerRawInput();
    OmniWebSocketServer::instance()->startServer(8080);

    OmniUI::registerQmlTypes();
}

OmniApplication::~OmniApplication() {
    delete m_nativeFilter;
    OmniWebSocketServer::instance()->stopServer();
}

void OmniApplication::initializeJuce() {
    if (m_juceInitialized) return;
    m_juceInitialized = true;
}

bool OmniApplication::loadMainSource(const QString& sourcePath) {
    const QUrl url = QUrl::fromLocalFile(sourcePath);
    QObject::connect(m_qmlEngine, &QQmlApplicationEngine::objectCreated, this, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
        auto* window = qobject_cast<QQuickWindow*>(obj);
        if (window) OmniNeuralEngine::instance()->setRootItem(window->contentItem());
    }, BobUI::QueuedConnection);
    m_qmlEngine->load(url);
    return true;
}

bool OmniApplication::notify(QObject *receiver, QEvent *e) {
    auto manager = OmniInputManager::instance();
    QString currentDeviceId = "sys-mouse-0"; // Default fallback

    if (e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent*>(e);
        // TODO: Map raw hardware handle to deviceId via Side-Channel
        manager->updateCursor(currentDeviceId, mouseEvent->globalPosition());
        if (e->type() == QEvent::MouseMove) manager->setDeviceHover(currentDeviceId, receiver);
        if (e->type() == QEvent::MouseButtonPress) manager->setDeviceFocus(currentDeviceId, receiver);
    }

    // --- CONTEXTUAL TAGGING ---
    // We attach the current DeviceId to the receiver object dynamically
    // so that the widget logic (Button/Slider) knows WHO is interacting with it.
    if (receiver) {
        receiver->setProperty("_omni_active_device", currentDeviceId);
    }

    return QApplication::notify(receiver, e);
}

OmniInputManager* OmniApplication::inputManager() const { return OmniInputManager::instance(); }
