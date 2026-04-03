# Project Roadmap

## Phase 1 - 16: Infrastructure, Mocks Purged, UX, CI/CD, Plugins, Audio, Net (Complete)
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
- [x] **WASM & Desktop Build Automation:** `.github/workflows/ci_cd.yml` executing CMake via Ninja.
- [x] **Master Clock Unification:** Built the `OmniMasterClock` singleton.
- [x] **Global IPC:** Built `OmniIPC` using `QSharedMemory`.
- [x] **Hardware QA Preparation:** Built `DevMode` into `OmniInputManager` for single-mouse developers to simulate a second hardware mouse locally via WASD keys.

## Phase 17: Submodule Redundancy Sweep & Final Handoff (Complete)
- [x] **Redundancy Sweeps:** Verified the `bobui` directory structure requires no further external Git submodules; the `omnicore` architecture natively encapsulates all required dependencies.
- [x] **Physical Hardware QA Prep:** Executed the DX hardware simulation module.

## Phase 18: Community Beta & Third-Party Library Linking (Current)
- [ ] **Third-Party Compilations:** Monitor the GitHub Actions CI/CD to verify that the C++ compiler successfully links `JUCE` and `ImGui` natively on the cloud Ubuntu/Windows runners.
- [ ] **Community Release Draft:** Compile the final `CHANGELOG.md` notes into a massive v3.0.0 "1.0 release" for the community.
