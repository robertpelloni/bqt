# Project Roadmap

## Phase 1 - 14: Core Infrastructure, Mocks Purged, UX, & CI/CD Validation (Complete)
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
- [x] **Intelligent Multi-Agent Branch Merging:** Merged parallel `OmniNexus` and `Dependabot` branches autonomously.
- [x] **Syntax Validation:** Cleaned the raw concatenation files and guaranteed `juce_mock.cpp` C++ linkage parity.

## Phase 15: Master Clock Unification & Global IPC (Complete)
- [x] **Master Clock Unification:** Built the `OmniMasterClock` singleton to sync UI rendering frames perfectly with the DSP frame rate across WebAssembly and Desktop natively.
- [x] **Global IPC:** Built `OmniIPC` using `QSharedMemory` to communicate instantly between separate `.exe` instances locally without WebSockets.

## Phase 16: The Ultimate Polishing & Live Community Deployment (Current)
- [ ] **Hardware QA Certification:** Connect a physical secondary mouse/keyboard to a build machine and verify the Win32 `WM_INPUT` interception is accurately spawning the multi-cursors dynamically.
- [ ] **Redundancy Sweeps:** Combine redundant functionality from linked submodules if any exist outside of `bobui`.
