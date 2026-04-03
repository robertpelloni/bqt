#include "OmniQmlRegistration.h"
#include "OmniInputManager.h"
#include "OmniButton.h"
#include "OmniSlider.h"
#include "OmniTextField.h"
#include "OmniDial.h"
#include "OmniProgressBar.h"
#include "OmniCheckBox.h"
#include "OmniCodeEditor.h"
#include "OmniCalendar.h"
#include "OmniTerminal.h"
#include "OmniWindow.h"
#include "OmniFilter.h"
#include "OmniGain.h"
#include "OmniAudioGraph.h"
#include "OmniJuceView.h"
#include "OmniDockArea.h"
#include "OmniSplitView.h"
#include "OmniMidiHandler.h"
#include "OmniSequencer.h"
#include "OmniDatabase.h"
#include "OmniHttpClient.h"
#include "OmniWebSocket.h"
#include "OmniPluginManager.h"
#include "OmniMasterClock.h"
#include "OmniIPC.h"
#include "OmniFileSystem.h"
#include "OmniThemeManager.h"
#include "OmniNotificationCenter.h"
#include "OmniNeuralEngine.h"
#include "OmniTimeMachine.h"
#include <QQmlEngine>
#include <QDebug>

void OmniUI::registerQmlTypes() {
    qDebug() << "OmniUI: Registering QML Types...";
    
    // Register Singletons
    qmlRegisterSingletonType<OmniInputManager>("OmniUI", 1, 0, "InputManager", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniInputManager::instance(); });

    qmlRegisterSingletonType<OmniPluginManager>("OmniUI", 1, 0, "PluginManager", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniPluginManager::instance(); });
        
    qmlRegisterSingletonType<OmniMasterClock>("OmniAudio", 1, 0, "MasterClock", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniMasterClock::instance(); });

    qmlRegisterSingletonType<OmniFileSystem>("OmniData", 1, 0, "FileSystem", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniFileSystem::instance(); });

    qmlRegisterSingletonType<OmniTimeMachine>("OmniData", 1, 0, "TimeMachine", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniTimeMachine::instance(); });

    qmlRegisterSingletonType<OmniThemeManager>("OmniUI", 1, 0, "ThemeManager", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniThemeManager::instance(); });

    qmlRegisterSingletonType<OmniNotificationCenter>("OmniUI", 1, 0, "NotificationCenter", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniNotificationCenter::instance(); });

    qmlRegisterSingletonType<OmniNeuralEngine>("OmniUI", 1, 0, "NeuralEngine", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* { return OmniNeuralEngine::instance(); });

    // Register Actual Fully-Implemented Widgets & Views
    qmlRegisterType<OmniButton>("OmniUI", 1, 0, "Button");
    qmlRegisterType<OmniSlider>("OmniUI", 1, 0, "Slider");
    qmlRegisterType<OmniTextField>("OmniUI", 1, 0, "TextField");
    qmlRegisterType<OmniDial>("OmniUI", 1, 0, "Dial");
    qmlRegisterType<OmniProgressBar>("OmniUI", 1, 0, "ProgressBar");
    qmlRegisterType<OmniCheckBox>("OmniUI", 1, 0, "CheckBox");
    qmlRegisterType<OmniCodeEditor>("OmniUI", 1, 0, "CodeEditor");
    qmlRegisterType<OmniCalendar>("OmniUI", 1, 0, "Calendar");
    qmlRegisterType<OmniTerminal>("OmniUI", 1, 0, "Terminal");
    qmlRegisterType<OmniJuceView>("OmniUI", 1, 0, "JuceView");
    
    // Register Layouts
    qmlRegisterType<OmniWindow>("OmniLayout", 1, 0, "Window");
    qmlRegisterType<OmniDockArea>("OmniLayout", 1, 0, "DockArea");
    qmlRegisterType<OmniSplitView>("OmniLayout", 1, 0, "SplitView");

    // Register Audio DSP & MIDI
    qmlRegisterType<OmniFilter>("OmniAudio", 1, 0, "Filter");
    qmlRegisterType<OmniGain>("OmniAudio", 1, 0, "Gain");
    qmlRegisterType<OmniAudioGraph>("OmniAudio", 1, 0, "AudioGraph");
    qmlRegisterType<OmniMidiHandler>("OmniAudio", 1, 0, "MidiHandler");
    qmlRegisterType<OmniSequencer>("OmniAudio", 1, 0, "Sequencer");

    // Register Data & Networking API
    qmlRegisterType<OmniDatabase>("OmniData", 1, 0, "Database");
    qmlRegisterType<OmniHttpClient>("OmniNet", 1, 0, "HttpClient");
    qmlRegisterType<OmniWebSocket>("OmniNet", 1, 0, "WebSocket");
    qmlRegisterType<OmniIPC>("OmniNet", 1, 0, "IPC");
    
    qDebug() << "OmniUI: QML Types Registered successfully. Full Parity Reached.";
}

#include "OmniQmlRegistration.moc"
