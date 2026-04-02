# Project Roadmap

## Phase 1 - 4: Core Implementation & Multi-Cursor OS Routing (Complete)
- [x] Scaffold and generate native C++ backend framework (`QQuickPaintedItem` base).
- [x] Establish `OmniInputManager` with independent focus trees (`DeviceId -> Focused Widget`).
- [x] Intercept hardware (`QMouseEvent`, `QKeyEvent`, `QTouchEvent`) and route into focus trees.
- [x] Create comprehensive Native Widgets (Button, Slider, TextField, Dial, ProgressBar, CheckBox).

## Phase 5: Hybrid Framework Transcendence (Complete)
- [x] **JUCE RHI Bridge:** Implemented `OmniJuceView` to mount `juce::Component` within the QML SceneGraph natively.
- [x] **Audio Graphing:** Implemented `OmniAudioGraph` wrapping DSP nodes (`OmniFilter`, `OmniGain`).

## Phase 6: Core Engine Compilation & Hardware Platform Drivers (Current)
- [ ] **Native Platform Hooks:** Replace "mock" string device IDs (`sys-mouse-0`) with actual OS handles via `RegisterRawInputDevices` (Windows) and `libinput` (Linux).
- [ ] **CMake Integrity:** Execute a full native build using Ninja/CMake to ensure zero compilation errors across the massive `omnicore` refactor.

## Phase 7: UI Comprehensiveness & Expansion
- [ ] Implement `OmniCodeEditor` (using a lightweight Monaco/Scintilla wrapper or native QML text documents).
- [ ] Implement `OmniCalendar` and `OmniSplitView` layout controllers.
- [ ] Combine redundant functionality from linked submodules.
- [ ] 100% test coverage.
