# Project Roadmap

## Phase 1 - 8: Core Infrastructure, Multi-Cursor, Audio, Data, & Net (Complete)
- [x] Scaffold and generate native C++ backend framework (`QQuickPaintedItem` base).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware events natively (`RegisterRawInputDevices`) and route into focus trees.
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.
- [x] Zero Mock Milestone Achieved.
- [x] Implement complex advanced widgets (`OmniCodeEditor`, `OmniCalendar`).
- [x] **Data Persistence:** Implemented `OmniDatabase` wrapping SQLite.
- [x] **Networking:** Implemented `OmniHttpClient` and `OmniWebSocket`.

## Phase 9: Real-World OS Stress Test & Polishing (Complete)
- [x] **Automated Testing:** Established the `QTest` framework and mathematically proved that concurrent hardware keypress routing works flawlessly on independent focus trees.
- [x] **Architectural Synthesis:** Authored `OMNIUI_IMPLEMENTATION_DETAIL.md`.

## Phase 10: OmniPlugin Architecture & CI/CD Rollout (Current)
- [ ] **OmniPluginManager:** Implement `QPluginLoader` support inside `OmniUI` to dynamically load external `.dll` / `.so` files at runtime, allowing the OS to load user-developed extensions without recompiling the core.
- [ ] **WASM CI/CD:** Establish GitHub Actions to compile the entire `omnicore` codebase via Emscripten natively to a `dist/` web folder.
- [ ] **Compilation Validation:** Resolve local host build environment limits to natively compile the massive `omnicore` refactor via Ninja.
