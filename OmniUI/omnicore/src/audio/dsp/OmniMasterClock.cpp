#include "OmniMasterClock.h"
#include <QDebug>

OmniMasterClock* OmniMasterClock::instance()
{
    static OmniMasterClock* clock = new OmniMasterClock();
    return clock;
}

OmniMasterClock::OmniMasterClock(QObject *parent)
    : QObject(parent),
      m_bpm(120),
      m_isRunning(false),
      m_dspTime(0.0),
      m_lastSyncTime(0.0),
      m_currentBeat(0),
      m_currentStep(0)
{
    setObjectName("OmniMasterClock");

    // The UI polls the DSP time every ~16ms (roughly 60fps)
    connect(&m_uiSyncTimer, &QTimer::timeout, this, &OmniMasterClock::onUiSyncTick);
}

OmniMasterClock::~OmniMasterClock() {
    stop();
}

int OmniMasterClock::bpm() const { return m_bpm; }
void OmniMasterClock::setBpm(int b) {
    if (m_bpm != b && b > 0) {
        m_bpm = b;
        emit bpmChanged();
    }
}

bool OmniMasterClock::isRunning() const { return m_isRunning; }

void OmniMasterClock::start() {
    if (!m_isRunning) {
        m_isRunning = true;
        m_uiSyncTimer.start(16); // 60 FPS UI Sync
        emit isRunningChanged();
        qDebug() << "OmniMasterClock: Started at" << m_bpm << "BPM";
    }
}

void OmniMasterClock::stop() {
    if (m_isRunning) {
        m_isRunning = false;
        m_uiSyncTimer.stop();
        emit isRunningChanged();
        qDebug() << "OmniMasterClock: Stopped";
    }
}

void OmniMasterClock::reset() {
    m_dspTime = 0.0;
    m_lastSyncTime = 0.0;
    m_currentBeat = 0;
    m_currentStep = 0;
}

void OmniMasterClock::advanceDspTime(double deltaSeconds) {
    if (m_isRunning) {
        // This would be called by juce::AudioProcessor::processBlock natively
        double current = m_dspTime.load(std::memory_order_relaxed);
        m_dspTime.store(current + deltaSeconds, std::memory_order_relaxed);
    }
}

void OmniMasterClock::onUiSyncTick() {
    // If not connected to a real JUCE block, simulate DSP time passing
    // This allows the clock to work even if the audio hardware is disconnected.
    advanceDspTime(0.016); 

    double currentDspTime = m_dspTime.load(std::memory_order_acquire);
    
    // Calculate beats based on BPM
    double beatsPerSecond = m_bpm / 60.0;
    double totalBeats = currentDspTime * beatsPerSecond;
    double totalSteps = totalBeats * 4.0; // 16th notes
    
    int newBeat = static_cast<int>(totalBeats);
    int newStep = static_cast<int>(totalSteps) % 16;

    if (newBeat > m_currentBeat) {
        m_currentBeat = newBeat;
        emit beat(m_currentBeat);
    }

    if (newStep != m_currentStep) {
        m_currentStep = newStep;
        emit sixteenthStep(m_currentStep);
    }

    m_lastSyncTime = currentDspTime;
}
