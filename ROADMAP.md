# Project Roadmap

## Phase 1 - 11: Multi-Cursor, Data, Net, Plugins, Verification, UX (Complete)
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
- [x] **Property Introspection:** Upgraded `OmniDeveloperOverlay` to dynamically render `QMetaObject` properties of hovered `QObject`s using ImGui/QPainter logic.

## Phase 12: CI/CD Supremacy & Hardware Integration (Complete)
- [x] **WASM CI/CD:** Established GitHub Actions (`build_wasm.yml`) to execute `emcmake cmake ..` and compile the entire `omnicore` codebase natively to a `dist/` web folder.
- [x] **Compilation Validation:** Established GitHub Actions (`build_native.yml`) to automatically compile across Windows/Ubuntu runners via Ninja and run the `QTest` suite on every push.
- [x] **Submodule Tracking:** Published `SUBMODULE_DASHBOARD.md` to map the finalized OS directory layout.

## Phase 13: Live Deployment & Community Rollout (Current)
- [ ] **GitHub Releases:** Tie the `VERSION.md` dynamically into the CI/CD pipeline so that pushes to `main` auto-generate binary `.exe` and `.wasm` releases.
- [ ] **Physical Hardware QA:** Connect a physical secondary mouse/keyboard to a build machine and verify the Win32 `WM_INPUT` interception is accurately spawning the multi-cursors dynamically.
