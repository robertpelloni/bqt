#include "OmniQmlRegistration.h"
#include "OmniInputManager.h"
#include <QQmlEngine>
#include <QDebug>

// Temporary dummy item to satisfy QML imports until fully implemented as QQuickItems
#include <QQuickItem>

class OmniQmlButton : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    OmniQmlButton() { qDebug() << "OmniQmlButton created"; }
    QString text() const { return m_text; }
    void setText(const QString& t) { if(t != m_text) { m_text = t; emit textChanged(); } }
signals:
    void textChanged();
    void clicked();
private:
    QString m_text;
};

// ... other dummy QQuickItems for the examples (Slider, Dial, etc.)
class OmniQmlSlider : public QQuickItem { Q_OBJECT };
class OmniQmlTextField : public QQuickItem { Q_OBJECT };
class OmniQmlDial : public QQuickItem { Q_OBJECT };
class OmniQmlCheckBox : public QQuickItem { Q_OBJECT };
class OmniQmlProgressBar : public QQuickItem { Q_OBJECT };
class OmniQmlJuceView : public QQuickItem { Q_OBJECT };

class OmniQmlMidiHandler : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void sendNoteOn(int channel, int note, int velocity) { qDebug() << "MIDI NoteOn" << channel << note << velocity; }
    Q_INVOKABLE void sendNoteOff(int channel, int note) { qDebug() << "MIDI NoteOff" << channel << note; }
};
class OmniQmlAudioGraph : public QQuickItem {
    Q_OBJECT
public:
    Q_INVOKABLE void addProcessor(QObject* p) { qDebug() << "AudioGraph: added processor"; }
};
class OmniQmlAudioProcessor : public QObject { Q_OBJECT };
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
            // QQmlEngine takes ownership, but instance() returns a static pointer.
            // In a real app we might need a wrapper object here.
            return OmniInputManager::instance();
        });

    // Register UI Controls (Mocked as QQuickItems for now to make QML run)
    qmlRegisterType<OmniQmlButton>("OmniUI", 1, 0, "Button");
    qmlRegisterType<OmniQmlSlider>("OmniUI", 1, 0, "Slider");
    qmlRegisterType<OmniQmlTextField>("OmniUI", 1, 0, "TextField");
    qmlRegisterType<OmniQmlDial>("OmniUI", 1, 0, "Dial");
    qmlRegisterType<OmniQmlCheckBox>("OmniUI", 1, 0, "CheckBox");
    qmlRegisterType<OmniQmlProgressBar>("OmniUI", 1, 0, "ProgressBar");
    qmlRegisterType<OmniQmlJuceView>("OmniUI", 1, 0, "JuceView");

    // Register Audio
    qmlRegisterType<OmniQmlMidiHandler>("OmniAudio", 1, 0, "MidiHandler");
    qmlRegisterType<OmniQmlAudioGraph>("OmniAudio", 1, 0, "AudioGraph");
    qmlRegisterType<OmniQmlAudioProcessor>("OmniAudio", 1, 0, "AudioProcessor");
    qmlRegisterType<OmniQmlSequencer>("OmniAudio", 1, 0, "Sequencer");
    
    qDebug() << "OmniUI: QML Types Registered successfully.";
}

#include "OmniQmlRegistration.moc"
