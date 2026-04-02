# Project Roadmap

## Phase 1 - 12: Architecture, Layouts, Audio, Plugins, UX, & CI/CD (Complete)
- [x] Scaffold native C++ backend framework (`QQuickPaintedItem` base).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware events natively (`RegisterRawInputDevices`).
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.
- [x] Zero Mock Milestone Achieved.
- [x] Implement complex advanced widgets (`OmniCodeEditor`, `OmniCalendar`).
- [x] **Data Persistence:** Implemented `OmniDatabase` wrapping SQLite.
- [x] **Networking:** Implemented `OmniHttpClient` and `OmniWebSocket`.
- [x] **Automated Testing:** Established the `QTest` framework mathematically verifying concurrent typing.
- [x] **OmniPluginManager:** Implemented `QPluginLoader` runtime extensibility.
- [x] **Property Introspection:** Upgraded `OmniDeveloperOverlay` to dynamically render `QMetaObject` properties.
- [x] **WASM & Desktop Build Automation:** `.github/workflows/ci_cd.yml` executing CMake via Ninja on Ubuntu/Windows/WebAssembly.

## Phase 13: Live Deployment & Audio Precision (Complete)
- [x] **Release Automation:** CI/CD pipeline reads `VERSION.md` dynamically to generate formal `.exe` and `.wasm` GitHub Releases on every push.
- [x] **Audio Hardware Sync:** Rewrote `OmniFilter` and `OmniMidiHandler` to drop simulations and directly execute `juce::dsp::StateVariableTPTFilter` and `juce::MidiOutput` calls.

## Phase 14: Community Polish & Physical Hardware QA (Current)
- [ ] **Compilation Validation:** If the GitHub Actions runner fails due to `#include` errors across the massive 60-file refactor, fix the codebase instantly in the next execution loop.
- [ ] **Physical Hardware QA:** Connect a physical secondary mouse/keyboard to a build machine and verify the Win32 `WM_INPUT` interception is accurately spawning the multi-cursors dynamically.
- [ ] **Redundancy Sweeps:** Combine redundant functionality from linked submodules if any exist outside of `bobui`.
