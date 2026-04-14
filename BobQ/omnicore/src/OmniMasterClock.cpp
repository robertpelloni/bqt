#include "OmniMasterClock.h"

// Forward/Mock declarations for external loops
namespace juce {
    class MessageManager {
    public:
        static MessageManager* getInstance() { static MessageManager m; return &m; }
        void runDispatchLoopUntil(int milliseconds) {}
    };
}

namespace Upp {
    // Usually defined in <CtrlCore/CtrlCore.h>
    void ProcessEvents(bool* quit = nullptr);
}

// Ensure mock compiles for CMake
namespace Upp {
    void ProcessEvents(bool* quit) {
        // Pump U++ queue
    }
}

OmniMasterClock* OmniMasterClock::s_instance = nullptr;

OmniMasterClock* OmniMasterClock::instance() {
    if (!s_instance) {
        s_instance = new OmniMasterClock();
    }
    return s_instance;
}

OmniMasterClock::OmniMasterClock(QObject* parent) : QObject(parent) {
    connect(&m_timer, &QTimer::timeout, this, &OmniMasterClock::onTick);
}

OmniMasterClock::~OmniMasterClock() {
    stop();
}

void OmniMasterClock::start(int refreshRateHz) {
    if (refreshRateHz <= 0) refreshRateHz = 60;

    // Calculate interval in milliseconds
    int intervalMs = 1000 / refreshRateHz;
    m_timer.start(intervalMs);
}

void OmniMasterClock::stop() {
    m_timer.stop();
}

void OmniMasterClock::onTick() {
    // 1. Pump JUCE Event Loop (give it a tiny slice of time, e.g., 2ms)
    juce::MessageManager::getInstance()->runDispatchLoopUntil(2);

    // 2. Pump Ultimate++ Event Loop
    bool quit = false;
    Upp::ProcessEvents(&quit);

    // 3. Emit tick for internal BobQ physics/particle/animation systems
    emit tick();
}
