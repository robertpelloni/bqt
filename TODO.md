# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits. Execute `cmake -G Ninja` successfully.
- [ ] **WASM Build Test:** Install Emscripten and run `emcmake cmake ..`.

## Implementation Tasks (Phase 11 Focus)

### 1. Developer Overlay Enhancements (QMetaObject)
- [ ] Upgrade `OmniDeveloperOverlay.cpp` to listen for mouse hovers.
- [ ] Extract the `QMetaObject` of the focused `QObject`.
- [ ] Dynamically render an ImGui tooltip listing all `Q_PROPERTY` items (names and values) associated with that specific widget.

### 2. Audio & MIDI Perfection
- [ ] Connect `OmniMidiHandler` physically to `juce::MidiOutput`.
- [ ] Connect the mock DSP loops in `OmniFilter` to actual `juce::dsp::StateVariableTPTFilter` calls when the JUCE library is verified linked.

### 3. Redundancy Sweeps
- [ ] Combine redundant functionality from linked submodules if any exist outside of `bobui`.
