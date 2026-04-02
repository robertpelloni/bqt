# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Compilation Test:** Resolve local host build environment limits. Run `cmake -G Ninja` successfully for the main `omnicore` library and the `tests/` executable.

## Implementation Tasks (Phase 10 Focus)

### 1. OmniPlugin Architecture
- [ ] Implement `OmniPluginManager.cpp`. Provide `Q_INVOKABLE void loadPlugin(QString path)` functionality.
- [ ] Ensure that plugins can register their own QML components into the global `QQmlApplicationEngine` dynamically.

### 2. Audio & MIDI Perfection
- [ ] Connect `OmniMidiHandler` physically to `juce::MidiOutput`.
- [ ] Connect the mock DSP loops in `OmniFilter` to actual `juce::dsp::StateVariableTPTFilter` calls when the JUCE library is verified linked.

### 3. Final OS Polishing
- [ ] Expand `OmniDeveloperOverlay` to dynamically list the currently connected plugins and active SQLite `OmniDatabase` instances.
- [ ] Document all properties (like `accentColor`, `omniStyle`, `url`) dynamically generated from C++ `Q_PROPERTY` metadata via QMetaObject introspection.
