# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits to natively compile the massive `omnicore` refactor. Ensure Emscripten WASM build passes.
- [ ] **Git Sync:** Intelligently merge any pending feature branches from the remote if they emerge.

## Implementation Tasks (Phase 4 & 5 Focus)

### 1. Multimedia & JUCE Fusion
- [ ] Rewrite `OmniQmlJuceView` to be a real `QQuickItem` that mounts a `juce::Component` within the QML SceneGraph, moving away from the legacy `JuceWidget` fallback.
- [ ] Implement `OmniFilter` and `OmniGain` as true nodes linking to the Audio DSP thread.
- [ ] Hook `OmniMidiHandler` directly into `juce::MidiInput`.

### 2. Remaining Essential UI Components
- [ ] Fully implement `OmniTextField.cpp`. This is critical for testing the multi-keyboard independent focus routing.
- [ ] Implement `OmniDial.cpp` with logarithmic value scaling and polar coordinate tracking for the mouse logic.

### 3. Polish & Documentation
- [ ] Ensure all new properties (like `accentColor`, `omniStyle`) are documented in `Manual.md`.
- [ ] Re-verify the `OmniDeveloperOverlay` ImGui code cleanly accepts input without stalling the Qt event loop.
