# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits to natively compile the massive `omnicore` refactor. Ensure Emscripten WASM build passes.

## Implementation Tasks (Phase 5 Focus)

### 1. Multimedia & JUCE Fusion
- [ ] Rewrite `OmniQmlJuceView` to be a real `QQuickItem` that mounts a `juce::Component` within the QML SceneGraph, moving away from the legacy `JuceWidget` fallback.
- [ ] Connect `OmniFilter::processAudioBlock` and `OmniGain::processAudioBlock` to the actual JUCE audio processing thread. 
- [ ] Hook `OmniMidiHandler` directly into `juce::MidiInput`.

### 2. Remaining Essential UI Components
- [ ] Implement `OmniCheckBox.cpp`.
- [ ] Implement `OmniProgressBar.cpp`.

### 3. Polish & Documentation
- [ ] Ensure all new properties (like `accentColor`, `omniStyle`) are documented in `Manual.md`.
- [ ] Re-verify the `OmniDeveloperOverlay` ImGui code cleanly accepts input without stalling the Qt event loop.
