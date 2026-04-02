# Project Roadmap

## Phase 1 - 7: Core Infrastructure, Layouts, & Multi-Cursor Logic (Complete)
- [x] Scaffold and generate native C++ backend framework (`QQuickPaintedItem` base).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware events natively (`RegisterRawInputDevices`) and route into focus trees.
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.
- [x] Zero Mock Milestone Achieved.
- [x] Implement complex advanced widgets (`OmniCodeEditor`, `OmniCalendar`).

## Phase 8: Networking, Data, & Core Parity (Complete)
- [x] **Data Persistence:** Implemented `OmniDatabase` wrapping SQLite.
- [x] **Networking:** Implemented `OmniHttpClient` and `OmniWebSocket` wrapping `QNetworkAccessManager` and `QWebSocket`.
- [x] **Build System Configuration:** Successfully linked `Qt6::Sql`, `Qt6::Network`, and `Qt6::WebSockets` within CMake.

## Phase 9: Real-World OS Stress Test & Polishing (Current)
- [ ] **WASM CI/CD:** Establish GitHub Actions to compile the entire `omnicore` codebase via Emscripten natively to a `dist/` web folder.
- [ ] Combine redundant functionality from linked submodules.
- [ ] Launch a native OmniUI Window, spawn two physical mice, and execute concurrent typing in `OmniCodeEditor` and `OmniTextField` via physical QA testing.
- [ ] 100% automated test coverage.
