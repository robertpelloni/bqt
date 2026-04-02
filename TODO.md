# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **CMake Compilation Fix:** Fix the host build environment so `cmake -G Ninja` successfully compiles the massive generated `omnicore` library. Resolve any `#include` or `MOC` generation issues that arise.
- [ ] **Git Synchronization:** Simulate a robust git pull/merge/push cycle to ensure the agent workflow respects history and intelligent conflict resolution.

## Implementation Tasks (Phase 3 & 4 Focus)

### 1. Robust Widget Logic (No Half-Measures)
- [ ] Flesh out `OmniQmlButton::paint` using `QSGNode` for hardware accelerated rendering, or map it to Qt Quick Controls 2 styling logic.
- [ ] Implement `OmniDial` property bindings and map it to logarithmic scaling for Audio DSP usage.
- [ ] Ensure all widgets have rigorous inline comments explaining the logic, side-effects, and optimization strategies.

### 2. Multi-Input OS Layer Deepening
- [ ] `OmniApp::notify` currently intercepts `QMouseEvent`s. It must be extended to intercept `QKeyEvent`, `QTouchEvent`, and `QTabletEvent`.
- [ ] Implement `OmniInputManager::getFocusedWidget(deviceId)` and `OmniInputManager::setFocus(deviceId, widget)`.

### 3. Audio & DSP
- [ ] Implement `OmniAudioGraph::addProcessor` and wire it to an actual JUCE `AudioProcessorGraph`.
- [ ] Expose MIDI mapping slots to QML.

## Continuous Maintenance
- [ ] Track all AI session notes and codebase quirks in `MEMORY.md`.
- [ ] Ensure `VERSION.md` is updated and committed before the end of this session.
