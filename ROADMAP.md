# Project Roadmap

## Phase 1 - 13: Architecture, Audio, Plugins, UX, CI/CD, Automation (Complete)
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
- [x] **Release Automation:** CI/CD pipeline reads `VERSION.md` dynamically to generate formal `.exe` and `.wasm` GitHub Releases on every push.
- [x] **Intelligent Multi-Agent Branch Merging:** Merged parallel `OmniNexus` and `Dependabot` branches autonomously.

## Phase 14: Community Polish & Physical Hardware QA (Complete)
- [x] **Compilation Validation:** Fixed C++ Linkage bugs (`juce_mock.cpp`) induced by the raw multi-agent concatenation.
- [x] **OmniNexus Consolidation:** Cleaned and combined `README.md` and `Manual.md` maintaining zero feature loss.

## Phase 15: Master Clock Unification & Global IPC (Current)
- [ ] **Master Clock Unification:** Build an `OmniMasterClock` singleton to sync the UI rendering frames perfectly with the `juce::AudioProcessorGraph` DSP frame rate across WebAssembly and Desktop natively.
- [ ] **Global IPC:** Build an `OmniIPC` module using `QSharedMemory` to communicate instantly between separate `.exe` instances locally without WebSockets.
