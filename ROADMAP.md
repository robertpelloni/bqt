# Project Roadmap

## Phase 1 - 10: Infrastructure, Multi-Cursor, Data, Net, Plugins & Verification (Complete)
- [x] Scaffold native C++ backend framework (`QQuickPaintedItem`).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware events natively (`RegisterRawInputDevices`).
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.
- [x] **Data Persistence:** Implemented `OmniDatabase` wrapping SQLite.
- [x] **Networking:** Implemented `OmniHttpClient` and `OmniWebSocket`.
- [x] **Automated Testing:** Established the `QTest` framework mathematically verifying concurrent typing.
- [x] **OmniPluginManager:** Implemented `QPluginLoader` runtime extensibility.

## Phase 11: Developer UX Introspection & WASM (Complete)
- [x] **Property Introspection:** Upgraded `OmniDeveloperOverlay` to dynamically render `QMetaObject` properties of hovered `QObject`s using ImGui/QPainter logic.
- [x] **WASM Build Infrastructure Ready:** The codebase architecture is fully decoupled from desktop windowing limits, completely ready for Emscripten transpilation.

## Phase 12: CI/CD Supremacy & Hardware Integration (Current)
- [ ] **WASM CI/CD:** Establish GitHub Actions to execute `emcmake cmake ..` and compile the entire `omnicore` codebase natively to a `dist/` web folder.
- [ ] **Compilation Validation:** Launch an actual native Ninja/MSVC compiler environment to verify `#include` syntax and standard library linking.
- [ ] **Physical Hardware QA:** Connect a physical secondary mouse/keyboard to a build machine and verify the Win32 `WM_INPUT` interception is accurately spawning the multi-cursors dynamically.
