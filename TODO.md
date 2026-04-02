# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub Actions:** Upon pushing this commit, verify that `.github/workflows/build_native.yml` succeeds. If CMake `#include` errors arise from the massive 60-file generation, they must be fixed in the next session.

## Implementation Tasks (Phase 13 Focus)

### 1. Release Automation
- [ ] Extract the version string from `VERSION.md` inside a bash step in the `.yml` workflow, and use it to tag the GitHub Release.

### 2. Audio & MIDI Perfection
- [ ] Connect `OmniMidiHandler` physically to `juce::MidiOutput`.
- [ ] Connect the mock DSP loops in `OmniFilter` to actual `juce::dsp::StateVariableTPTFilter` calls when the JUCE library is verified linked on the target build machine.

### 3. Redundancy Sweeps
- [ ] Combine redundant functionality from linked submodules if any exist outside of `bobui`.
