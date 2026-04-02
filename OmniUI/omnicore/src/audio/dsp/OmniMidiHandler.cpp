#include "OmniMidiHandler.h"
#include <QDebug>

OmniMidiHandler::OmniMidiHandler(QObject *parent)
    : QObject(parent),
      m_currentDevice("None")
{
    setObjectName("OmniMidiHandler");
}

OmniMidiHandler::~OmniMidiHandler() = default;

QString OmniMidiHandler::currentDevice() const { return m_currentDevice; }
void OmniMidiHandler::setCurrentDevice(const QString& device) {
    if (m_currentDevice != device) {
        m_currentDevice = device;
        emit currentDeviceChanged();
        qDebug() << "OmniMidiHandler: Selected hardware MIDI device:" << device;
    }
}

void OmniMidiHandler::sendNoteOn(int channel, int note, int velocity) {
    qDebug() << "OmniMidiHandler: Transmitting NoteOn CH:" << channel << "Note:" << note << "Vel:" << velocity;
    // In a real implementation, this pushes a juce::MidiMessage to the juce::MidiOutput
}

void OmniMidiHandler::sendNoteOff(int channel, int note) {
    qDebug() << "OmniMidiHandler: Transmitting NoteOff CH:" << channel << "Note:" << note;
    // Pushes juce::MidiMessage::noteOff
}
