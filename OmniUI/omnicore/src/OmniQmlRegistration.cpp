#include "OmniQmlRegistration.h"
#include "OmniInputManager.h"
#include "OmniButton.h"
#include "OmniSlider.h"
#include "OmniTextField.h"
#include "OmniDial.h"
#include "OmniProgressBar.h"
#include "OmniCheckBox.h"
#include "OmniFilter.h"
#include "OmniGain.h"
#include "OmniAudioGraph.h"
#include "OmniJuceView.h"
#include <QQmlEngine>
#include <QDebug>

#include <QQuickItem>

class OmniQmlMidiHandler : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void sendNoteOn(int channel, int note, int velocity) { qDebug() << "MIDI NoteOn" << channel << note << velocity; }
    Q_INVOKABLE void sendNoteOff(int channel, int note) { qDebug() << "MIDI NoteOff" << channel << note; }
};
class OmniQmlSequencer : public QObject {
    Q_OBJECT
    Q_PROPERTY(int bpm READ bpm WRITE setBpm NOTIFY bpmChanged)
public:
    int bpm() const { return m_bpm; }
    void setBpm(int b) { m_bpm = b; emit bpmChanged(); }
signals:
    void bpmChanged();
private:
    int m_bpm = 120;
};

void OmniUI::registerQmlTypes() {
    qDebug() << "OmniUI: Registering QML Types...";
    
    // Register Singletons
    qmlRegisterSingletonType<OmniInputManager>("OmniUI", 1, 0, "InputManager", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return OmniInputManager::instance();
        });

    // Register Actual Fully-Implemented Widgets & Views
    qmlRegisterType<OmniButton>("OmniUI", 1, 0, "Button");
    qmlRegisterType<OmniSlider>("OmniUI", 1, 0, "Slider");
    qmlRegisterType<OmniTextField>("OmniUI", 1, 0, "TextField");
    qmlRegisterType<OmniDial>("OmniUI", 1, 0, "Dial");
    qmlRegisterType<OmniProgressBar>("OmniUI", 1, 0, "ProgressBar");
    qmlRegisterType<OmniCheckBox>("OmniUI", 1, 0, "CheckBox");
    qmlRegisterType<OmniJuceView>("OmniUI", 1, 0, "JuceView");

    // Register Audio DSP
    qmlRegisterType<OmniFilter>("OmniAudio", 1, 0, "Filter");
    qmlRegisterType<OmniGain>("OmniAudio", 1, 0, "Gain");
    qmlRegisterType<OmniAudioGraph>("OmniAudio", 1, 0, "AudioGraph");

    // Register Audio (Mocked)
    qmlRegisterType<OmniQmlMidiHandler>("OmniAudio", 1, 0, "MidiHandler");
    qmlRegisterType<OmniQmlSequencer>("OmniAudio", 1, 0, "Sequencer");
    
    qDebug() << "OmniUI: QML Types Registered successfully.";
}

#include "OmniQmlRegistration.moc"
