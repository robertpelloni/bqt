#include "JuceHeader.h"
#include <iostream>

// Mock Implementations for JUCE
namespace juce {
    void initialiseJuce_GUI() {
        std::cout << "[JUCE MOCK] Initializing JUCE GUI subsystem..." << std::endl;
    }

    void shutdownJuce_GUI() {
        std::cout << "[JUCE MOCK] Shutting down JUCE GUI subsystem..." << std::endl;
    }

    MessageManager* MessageManager::getInstance() { return nullptr; }

    void MessageManager::runDispatchLoop() {}
    void MessageManager::stopDispatchLoop() {}
}

// Appending Midi and DSP Mocks
namespace juce {
    struct MidiDeviceInfo {
        std::string name;
        std::string identifier;
    };

    class MidiMessage {
    public:
        static MidiMessage noteOn(int channel, int noteNumber, uint8 velocity) { return MidiMessage(); }
        static MidiMessage noteOff(int channel, int noteNumber) { return MidiMessage(); }
    };

    class MidiOutput {
    public:
        static std::vector<MidiDeviceInfo> getAvailableDevices() {
            return { {"Mock Synth A", "dev_1"}, {"Mock Synth B", "dev_2"} };
        }
        static std::unique_ptr<MidiOutput> openDevice(const std::string& identifier) {
            return std::make_unique<MidiOutput>();
        }
        void sendMessageNow(const MidiMessage& message) {}
    };

    namespace dsp {
        class StateVariableTPTFilter {
        public:
            void setType(int type) {}
            void setCutoffFrequency(double freq) {}
            void setResonance(double res) {}
            void process(float* left, float* right, int numSamples) {}
        };
    }
}
