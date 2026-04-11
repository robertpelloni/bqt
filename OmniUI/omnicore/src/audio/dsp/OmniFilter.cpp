#include "OmniFilter.h"
#include <QDebug>
#include <cmath>

// JUCE Core
#include "JuceHeader.h"

OmniFilter::OmniFilter(QObject *parent)
    : QObject(parent),
      m_type(LowPass),
      m_cutoff(2000.0),
      m_resonance(0.707)
{
    setObjectName("OmniFilter");
    
    // Initialize the JUCE DSP Filter mathematically
    m_filter = std::make_unique<juce::dsp::StateVariableTPTFilter>();
    m_filter->setType(m_type);
    m_filter->setCutoffFrequency(m_cutoff);
    m_filter->setResonance(m_resonance);
}

OmniFilter::~OmniFilter() = default;

int OmniFilter::type() const { return m_type; }
void OmniFilter::setType(int t) {
    if (m_type != t) {
        m_type = t;
        if (m_filter) m_filter->setType(m_type);
        emit typeChanged();
    }
}

qreal OmniFilter::cutoff() const { return m_cutoff; }
void OmniFilter::setCutoff(qreal c) {
    if (!qFuzzyCompare(m_cutoff, c)) {
        m_cutoff = c;
        if (m_filter) m_filter->setCutoffFrequency(m_cutoff);
        emit cutoffChanged();
    }
}

qreal OmniFilter::resonance() const { return m_resonance; }
void OmniFilter::setResonance(qreal r) {
    if (!qFuzzyCompare(m_resonance, r)) {
        m_resonance = r;
        if (m_filter) m_filter->setResonance(m_resonance);
        emit resonanceChanged();
    }
}

void OmniFilter::processAudioBlock(float* leftChannel, float* rightChannel, int numSamples) {
    // True native hardware DSP processing using JUCE state variable logic
    if (m_filter) {
        m_filter->process(leftChannel, rightChannel, numSamples);
    }
}
