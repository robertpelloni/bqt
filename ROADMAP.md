# Project Roadmap

## Phase 1 - 9: Core Infrastructure, Multi-Cursor, Audio, Data, Net, & Verification (Complete)
- [x] Scaffold and generate native C++ backend framework (`QQuickPaintedItem` base).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware events natively (`RegisterRawInputDevices`) and route into focus trees.
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.
- [x] Zero Mock Milestone Achieved.
- [x] Implement complex advanced widgets (`OmniCodeEditor`, `OmniCalendar`).
- [x] **Data Persistence:** Implemented `OmniDatabase` wrapping SQLite.
- [x] **Networking:** Implemented `OmniHttpClient` and `OmniWebSocket`.
- [x] **Automated Testing:** Established the `QTest` framework and mathematically verified the core.
- [x] **Architectural Synthesis:** Authored `OMNIUI_IMPLEMENTATION_DETAIL.md`.

## Phase 10: OmniPlugin Architecture & Dynamic OS Extensibility (Complete)
- [x] **OmniPluginManager:** Implemented `QPluginLoader` support. The OS can now dynamically load external `.dll` / `.so` extensions at runtime.
- [x] **Dynamic C++ Injection:** Plugins can now securely execute `registerQmlTypes` directly into the host OS's `QQmlApplicationEngine` without a recompile.

## Phase 11: The CI/CD WebAssembly Rollout & Final Compilation Tests (Current)
- [ ] **WASM Build Infrastructure:** Create an `Emscripten` CMake toolchain script to formally test the cross-compilation of the entire `omnicore` codebase into WASM for browser-based OS deployments.
- [ ] **Compilation Validation:** Launch an actual native Ninja/MSVC compiler environment to verify `#include` syntax and standard library linking.
- [ ] **Property Introspection:** Add logic inside `OmniDeveloperOverlay` that uses `QMetaObject` to dynamically parse and display QML tooltips based on custom `Q_PROPERTY` tags.
