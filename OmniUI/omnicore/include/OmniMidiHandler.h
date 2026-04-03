#ifndef OMNIMIDIHANDLER_H
#define OMNIMIDIHANDLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>

// Forward declare JUCE classes
namespace juce {
    class MidiOutput;
}

class OmniMidiHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentDevice READ currentDevice WRITE setCurrentDevice NOTIFY currentDeviceChanged)

public:
    explicit OmniMidiHandler(QObject *parent = nullptr);
    virtual ~OmniMidiHandler();

    QString currentDevice() const;
    void setCurrentDevice(const QString& device);

    // Exposes a list of connected physical hardware MIDI interfaces directly to the QML UI
    Q_INVOKABLE QStringList getAvailableMidiDevices() const;

    Q_INVOKABLE void sendNoteOn(int channel, int note, int velocity);
    Q_INVOKABLE void sendNoteOff(int channel, int note);

signals:
    void currentDeviceChanged();
    void noteReceived(int channel, int note, int velocity);

private:
    QString m_currentDevice;
    std::unique_ptr<juce::MidiOutput> m_midiOutput;
};

#endif // OMNIMIDIHANDLER_H
