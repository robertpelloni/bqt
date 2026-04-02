# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub Actions:** Upon pushing this commit, verify that `.github/workflows/ci_cd.yml` completely passes the CMake build process now that the C++ linkage errors in the mock headers are fixed.

## Implementation Tasks (Phase 15 Focus)

### 1. OmniMasterClock Integration
- [ ] Create `OmniMasterClock.cpp`. 
- [ ] Have `OmniSequencer` and all UI Animations tie their ticks to this unified clock instead of arbitrary `QTimer` instances to guarantee DSP-level precision.

### 2. Global IPC (Inter-Process Communication)
- [ ] Implement `OmniIPC.cpp` wrapping `QSharedMemory`. 
- [ ] Provide `Q_INVOKABLE` methods to allow QML to broadcast binary payloads locally between multiple running BobUI OS instances.

### 3. Physical QA
- [ ] Spin up a Windows PC with two USB mice attached. Open `HelloOmni.exe`. Verify two red cursors appear and move independently based on the specific mouse being used.
