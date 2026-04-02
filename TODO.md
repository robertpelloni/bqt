# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits to natively compile the massive `omnicore` refactor. Ensure Emscripten WASM build passes.
- [ ] **Git Sync:** Intelligently merge any pending feature branches from the remote if they emerge.

## Implementation Tasks (Phase 6 & 7 Focus)

### 1. Hardware-Level Device Extraction
- [ ] Implement a Windows platform plugin hook in `OmniApp::notify` that reads `MSG*` structures and extracts `HANDLE hDevice` from `RAWINPUT`.
- [ ] Pass the true hardware handles to `OmniInputManager::registerDevice`.

### 2. Layout & Advanced Widgets
- [ ] Implement `OmniDockArea.cpp` to provide a robust window docking framework natively inside QML (similar to ImGui docking).
- [ ] Implement `OmniSplitView.cpp`.

### 3. Audio & MIDI Perfection
- [ ] Hook `OmniMidiHandler` directly into `juce::MidiInput`.
- [ ] Replace the mock DSP loops in `OmniFilter` with actual `juce::dsp::StateVariableTPTFilter` calls.
