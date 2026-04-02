# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits to natively compile the massive `omnicore` refactor. Run `cmake -G Ninja`.

## Implementation Tasks (Phase 7 Focus)

### 1. Complex Widgets
- [ ] Implement `OmniCodeEditor.cpp`. This requires a robust backend structure potentially wrapping `QTextDocument` but rendering via the SceneGraph.
- [ ] Implement `OmniCalendar.cpp`.

### 2. Audio & MIDI Perfection
- [ ] The `OmniMidiHandler` currently just prints `qDebug()`. It needs to be hooked structurally to a `juce::MidiOutput` pointer.
- [ ] Replace the mock DSP loops in `OmniFilter` with actual `juce::dsp::StateVariableTPTFilter` calls if the JUCE library is confirmed linked.

### 3. Polish & Documentation
- [ ] Ensure all new properties (like `accentColor`, `omniStyle`) are documented in `Manual.md`.
- [ ] Write exhaustive unit tests for `OmniInputManager`'s independent focus logic.
