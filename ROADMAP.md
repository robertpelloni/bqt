# Project Roadmap

## Phase 1 - 5: Foundation, QML, Multi-Cursor & Audio (Complete)
- [x] Establish Omni-Workspace guidelines and C++ backend boilerplate.
- [x] Wire QML Engine (`QQmlApplicationEngine`), implement `qmlRegisterType` pipeline.
- [x] Resolve Dual-Hierarchy crisis (refactor to `QQuickPaintedItem`).
- [x] Map `DeviceId -> Focused Widget` and route hardware independently.
- [x] Create comprehensive Native Widgets (Button, Slider, TextField, Dial, ProgressBar, CheckBox, Layouts).
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView`.
- [x] **Audio Graphing & MIDI:** Implemented DSP nodes, Graph, and Sequencer.

## Phase 6: Core Engine Compilation & Hardware Platform Drivers (Complete)
- [x] **Native Platform Hooks:** Implemented `OmniNativeEventFilter` wrapping Win32 `RegisterRawInputDevices` to extract true `hDevice` IDs from `WM_INPUT`.
- [x] **Zero Mocks:** Removed all dummy facades from the entire codebase.

## Phase 7: UI Comprehensiveness, WASM, & The Final Polish (Current)
- [ ] **CMake Integrity:** Execute a full native build using Ninja/CMake to ensure zero compilation errors across the massive `omnicore` refactor. Ensure WASM builds pass.
- [ ] Implement `OmniCodeEditor` (using a lightweight Monaco/Scintilla wrapper or native QML text documents).
- [ ] Implement `OmniCalendar`.
- [ ] Combine redundant functionality from linked submodules.
- [ ] 100% test coverage.
