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

## Phase 14: Community Polish & Physical Hardware QA (Current)
- [ ] **Compilation Validation:** If the GitHub Actions runner fails due to `#include` errors across the massive refactor or from the recent blind merge, fix the codebase instantly.
- [ ] **Physical Hardware QA:** Connect a physical secondary mouse/keyboard to a build machine and verify the Win32 `WM_INPUT` interception is accurately spawning the multi-cursors dynamically.
- [ ] **Redundancy Sweeps:** Combine redundant functionality from linked submodules if any exist outside of `bobui`.
