#include "OmniMidiHandler.h"
#include <QDebug>

// If juce headers are not found, the compiler will error, enforcing strict multimedia parity
#include "JuceHeader.h"

OmniMidiHandler::OmniMidiHandler(QObject *parent)
    : QObject(parent),
      m_currentDevice("None"),
      m_midiOutput(nullptr)
{
    setObjectName("OmniMidiHandler");
}

OmniMidiHandler::~OmniMidiHandler() {
    // juce::MidiOutput::openDevice() returns a unique_ptr in JUCE 6/7
    // but the mock might not. Clean up depending on implementation.
}

QString OmniMidiHandler::currentDevice() const { return m_currentDevice; }

void OmniMidiHandler::setCurrentDevice(const QString& device) {
    if (m_currentDevice != device) {
        m_currentDevice = device;
        
        // Setup actual JUCE MidiOutput
        auto midiDevices = juce::MidiOutput::getAvailableDevices();
        for (const auto& dev : midiDevices) {
            if (dev.name == device.toStdString()) {
                m_midiOutput = juce::MidiOutput::openDevice(dev.identifier);
                if (m_midiOutput) {
                    qDebug() << "OmniMidiHandler: Connected hardware MIDI device:" << device;
                } else {
                    qWarning() << "OmniMidiHandler: Failed to open MIDI device:" << device;
                }
                break;
            }
        }
        
        emit currentDeviceChanged();
    }
}

void OmniMidiHandler::sendNoteOn(int channel, int note, int velocity) {
    // Generate JUCE payload
    auto msg = juce::MidiMessage::noteOn(channel, note, static_cast<juce::uint8>(velocity));
    
    if (m_midiOutput) {
        m_midiOutput->sendMessageNow(msg);
        qDebug() << "OmniMidiHandler: JUCE NoteOn dispatched CH:" << channel << "Note:" << note << "Vel:" << velocity;
    } else {
        qWarning() << "OmniMidiHandler: Cannot send NoteOn, no valid JUCE MidiOutput connected.";
    }
}

void OmniMidiHandler::sendNoteOff(int channel, int note) {
    auto msg = juce::MidiMessage::noteOff(channel, note);
    
    if (m_midiOutput) {
        m_midiOutput->sendMessageNow(msg);
        qDebug() << "OmniMidiHandler: JUCE NoteOff dispatched CH:" << channel << "Note:" << note;
    }
}
