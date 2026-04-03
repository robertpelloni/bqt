#include "OmniSequencer.h"
#include "OmniMasterClock.h"
#include <QDebug>

OmniSequencer::OmniSequencer(QObject *parent)
    : QObject(parent),
      m_isPlaying(false),
      m_currentStep(0)
{
    setObjectName("OmniSequencer");
    
    // Connect strictly to the Master Clock for 16th note timing
    connect(OmniMasterClock::instance(), &OmniMasterClock::sixteenthStep, this, &OmniSequencer::onTimerTick);
}

OmniSequencer::~OmniSequencer() {
    stop();
}

int OmniSequencer::bpm() const { return OmniMasterClock::instance()->bpm(); }
void OmniSequencer::setBpm(int b) {
    OmniMasterClock::instance()->setBpm(b);
    emit bpmChanged();
}

bool OmniSequencer::isPlaying() const { return m_isPlaying; }

void OmniSequencer::start() {
    if (!m_isPlaying) {
        m_isPlaying = true;
        m_currentStep = 0;
        OmniMasterClock::instance()->start();
        emit isPlayingChanged();
        qDebug() << "OmniSequencer: Bound to Master Clock and Started.";
    }
}

void OmniSequencer::stop() {
    if (m_isPlaying) {
        m_isPlaying = false;
        emit isPlayingChanged();
        qDebug() << "OmniSequencer: Stopped.";
    }
}

void OmniSequencer::onTimerTick(int stepNumber) {
    if (m_isPlaying) {
        m_currentStep = stepNumber;
        emit beat(m_currentStep);
    }
}
