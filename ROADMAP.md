# Project Roadmap

## Phase 1 - 6: Foundation, QML, Multi-Cursor, Audio, OS Hooks (Complete)
- [x] Scaffold and generate native C++ backend framework (`QQuickPaintedItem` base).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware events natively (`RegisterRawInputDevices`) and route into focus trees.
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.
- [x] Zero Mock Milestone Achieved.

## Phase 7: UI Comprehensiveness, WASM, & The Final Polish (Complete)
- [x] Implement complex advanced widgets (`OmniCodeEditor`, `OmniCalendar`).
- [x] Fully wire the complete suite of `OmniUI 1.0` intended widgets into QML.

## Phase 8: Networking, Data, & WebAssembly Supremacy (Current)
- [ ] **Data Persistence:** Implement `OmniDatabase` wrapping SQLite natively for QML consumption.
- [ ] **Networking:** Implement `OmniHttpClient` and `OmniWebSocket` wrapping `QNetworkAccessManager` to bypass CORS issues in native desktop mode while transparently mapping to `fetch` in WASM.
- [ ] **WASM CI/CD:** Establish GitHub Actions to compile the entire `omnicore` codebase via Emscripten natively to a `dist/` web folder.

## Phase 9: Real-World OS Stress Test
- [ ] Combine redundant functionality from linked submodules.
- [ ] Launch a native OmniUI Window, spawn two physical mice, and execute concurrent typing in `OmniCodeEditor` and `OmniTextField`.
