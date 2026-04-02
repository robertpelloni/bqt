#include "OmniSequencer.h"
#include <QDebug>

OmniSequencer::OmniSequencer(QObject *parent)
    : QObject(parent),
      m_bpm(120),
      m_isPlaying(false),
      m_currentStep(0)
{
    setObjectName("OmniSequencer");
    connect(&m_timer, &QTimer::timeout, this, &OmniSequencer::onTimerTick);
}

OmniSequencer::~OmniSequencer() {
    stop();
}

int OmniSequencer::bpm() const { return m_bpm; }
void OmniSequencer::setBpm(int b) {
    if (m_bpm != b && b > 0) {
        m_bpm = b;
        emit bpmChanged();
        
        if (m_isPlaying) {
            // Recalculate timer interval for 16th notes
            int msPerBeat = 60000 / m_bpm;
            m_timer.setInterval(msPerBeat / 4);
        }
    }
}

bool OmniSequencer::isPlaying() const { return m_isPlaying; }

void OmniSequencer::start() {
    if (!m_isPlaying) {
        m_isPlaying = true;
        m_currentStep = 0;
        int msPerBeat = 60000 / m_bpm;
        m_timer.start(msPerBeat / 4); // 16th note steps
        emit isPlayingChanged();
        qDebug() << "OmniSequencer: Started playing at" << m_bpm << "BPM";
    }
}

void OmniSequencer::stop() {
    if (m_isPlaying) {
        m_isPlaying = false;
        m_timer.stop();
        emit isPlayingChanged();
        qDebug() << "OmniSequencer: Stopped";
    }
}

void OmniSequencer::onTimerTick() {
    emit beat(m_currentStep);
    m_currentStep = (m_currentStep + 1) % 16; // 16-step sequencer
}
