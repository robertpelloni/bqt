#include "OmniFilter.h"
#include <QDebug>
#include <cmath>

OmniFilter::OmniFilter(QObject *parent)
    : QObject(parent),
      m_type(LowPass),
      m_cutoff(2000.0),
      m_resonance(0.707)
{
    setObjectName("OmniFilter");
}

OmniFilter::~OmniFilter() = default;

int OmniFilter::type() const { return m_type; }
void OmniFilter::setType(int t) {
    if (m_type != t) {
        m_type = t;
        emit typeChanged();
        // Trigger JUCE graph rebuild/parameter update
    }
}

qreal OmniFilter::cutoff() const { return m_cutoff; }
void OmniFilter::setCutoff(qreal c) {
    if (!qFuzzyCompare(m_cutoff, c)) {
        m_cutoff = c;
        emit cutoffChanged();
    }
}

qreal OmniFilter::resonance() const { return m_resonance; }
void OmniFilter::setResonance(qreal r) {
    if (!qFuzzyCompare(m_resonance, r)) {
        m_resonance = r;
        emit resonanceChanged();
    }
}

void OmniFilter::processAudioBlock(float* leftChannel, float* rightChannel, int numSamples) {
    // Mock DSP: just attenuates to simulate "filtering" for now
    float attenuation = (m_type == LowPass) ? 0.8f : 0.5f;
    for (int i = 0; i < numSamples; ++i) {
        leftChannel[i] *= attenuation;
        rightChannel[i] *= attenuation;
    }
}
