# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub Actions:** Upon pushing this commit, verify that `.github/workflows/ci_cd.yml` succeeds and generates the `v2.1.0` GitHub Release correctly.

## Implementation Tasks (Phase 14 Focus)

### 1. Robust API Expansions
- [ ] Implement `OmniDatabase::executeTransaction(QList<QString> queries)` to allow QML to push massive data blocks cleanly to SQLite.
- [ ] Expose `QList<QString> getAvailableMidiDevices()` inside `OmniMidiHandler` dynamically mapping `juce::MidiOutput::getAvailableDevices()`.

### 2. Physical QA
- [ ] Spin up a Windows PC with two USB mice attached. Open `HelloOmni.exe`. Verify two red cursors appear and move independently based on the specific mouse being used.
- [ ] Spin up a browser, run the WASM version. Verify that touches/mouse clicks route correctly into the WASM canvas and translate into `OmniInputManager` events.
