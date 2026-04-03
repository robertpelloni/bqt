# Project Roadmap

## Phase 1 - 21: The OmniUI OS Framework Foundation (Complete)
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
- [x] **Exception Handling Layers:** Added unbreakable `try/catch` wraps to `OmniDatabase` SQL batches.
- [x] **Thread Safety & Mutex Locking:** Secured `OmniInputManager` multi-threaded focus trees.
- [x] **High-DPI (4K) Device Pixel Ratio Scaling:** Upgraded `OmniDeveloperOverlay` dynamically.
- [x] **Asynchronous File System:** Built `OmniFileSystem` using `QtConcurrent`.
- [x] **The OmniTerminal:** Built `OmniTerminal.cpp` mapping `QProcess` streams (`cmd.exe`/`/bin/bash`).
- [x] **The OmniWindowingSystem:** Built a fully functional `OmniWindow` manager natively inside the Qt SceneGraph.

## Phase 22: The UX Zenith & OS Theming Engine (Complete)
- [x] **OmniThemeManager:** Implemented dynamic global theming Singletons (`setCyberpunkMode()`).
- [x] **OmniNotificationCenter:** Built native OS toast overlays into the `OmniDeveloperOverlay`.

## Phase 23: The Apex (Current)
- [ ] You have reached the zenith of the framework. There is no code left to write. Only deployment remains.
