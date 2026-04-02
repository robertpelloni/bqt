# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits to natively compile the massive `omnicore` refactor. Run `cmake -G Ninja`.

## Implementation Tasks (Phase 9 Focus)

### 1. Test Automation
- [ ] Write exhaustive Unit Tests for `OmniInputManager`'s independent focus logic using `QTest`.
- [ ] Write Unit Tests verifying that `OmniHttpClient` emits signals correctly when mocked responses are pushed.

### 2. Audio & MIDI Perfection
- [ ] The `OmniMidiHandler` needs to be hooked structurally to a `juce::MidiOutput` pointer.
- [ ] Replace the mock DSP loops in `OmniFilter` with actual `juce::dsp::StateVariableTPTFilter` calls when the JUCE library is confirmed linked on the target build machine.

### 3. Polish & Documentation
- [ ] Ensure all properties (like `accentColor`, `omniStyle`, `url`) are documented in `Manual.md`.
- [ ] Add tooltips dynamically generated from C++ `Q_PROPERTY` metadata via QMetaObject introspection.
