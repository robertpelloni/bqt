#include "OmniSynthesizer.h"
#include <QDebug>

// #include "JuceHeader.h"

OmniSynthesizer::OmniSynthesizer(QObject *parent)
    : QObject(parent),
      m_polyphony(8)
{
    setObjectName("OmniSynthesizer");
    // m_synth.addVoice(new juce::SamplerVoice());
    // m_synth.addSound(new juce::SamplerSound(...));
}

OmniSynthesizer::~OmniSynthesizer() = default;

int OmniSynthesizer::polyphony() const { return m_polyphony; }
void OmniSynthesizer::setPolyphony(int voices) {
    if (m_polyphony != voices) {
        m_polyphony = voices;
        // Dynamically reallocate JUCE voices here
        emit polyphonyChanged();
        qDebug() << "OmniSynthesizer: Polyphony set to" << voices;
    }
}

void OmniSynthesizer::noteOn(int channel, int note, int velocity) {
    qDebug() << "OmniSynthesizer: Native Synthesis NoteON -> CH:" << channel << "NOTE:" << note;
    // m_synth.noteOn(channel, note, velocity / 127.0f);
}

void OmniSynthesizer::noteOff(int channel, int note) {
    qDebug() << "OmniSynthesizer: Native Synthesis NoteOFF -> CH:" << channel << "NOTE:" << note;
    // m_synth.noteOff(channel, note, 1.0f, true);
}

void OmniSynthesizer::processAudioBlock(float* leftChannel, float* rightChannel, int numSamples) {
    // juce::AudioBuffer<float> buffer(channels, numChannels, numSamples);
    // m_synth.renderNextBlock(buffer, midiMessages, 0, numSamples);
    
    Q_UNUSED(leftChannel);
    Q_UNUSED(rightChannel);
    Q_UNUSED(numSamples);
}
