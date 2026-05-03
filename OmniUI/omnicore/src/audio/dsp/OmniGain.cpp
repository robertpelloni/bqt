#include "OmniGain.h"

OmniGain::OmniGain(QObject *parent)
    : QObject(parent),
      m_gain(1.0)
{
    setObjectName("OmniGain");
}

OmniGain::~OmniGain() = default;

qreal OmniGain::gain() const { return m_gain; }
void OmniGain::setGain(qreal g) {
    if (!qFuzzyCompare(m_gain, g)) {
        m_gain = g;
        emit gainChanged();
    }
}

void OmniGain::processAudioBlock(float* leftChannel, float* rightChannel, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        leftChannel[i] *= static_cast<float>(m_gain);
        rightChannel[i] *= static_cast<float>(m_gain);
    }
}
