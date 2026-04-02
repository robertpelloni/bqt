# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits. Execute `cmake -G Ninja` successfully.
- [ ] **WASM Build Test:** Install Emscripten and run `emcmake cmake ..`.

## Implementation Tasks (Phase 12 Focus)

### 1. Build Verification
- [ ] Set up the GitHub Actions `.yml` file for the `robertpelloni/bobui` repository to automatically execute the CMake build upon commit.

### 2. Audio & MIDI Perfection
- [ ] Connect `OmniMidiHandler` physically to `juce::MidiOutput`.
- [ ] Connect the mock DSP loops in `OmniFilter` to actual `juce::dsp::StateVariableTPTFilter` calls when the JUCE library is verified linked.

### 3. Redundancy Sweeps
- [ ] Combine redundant functionality from linked submodules if any exist outside of `bobui`.
