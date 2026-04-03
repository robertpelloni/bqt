# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub CI/CD:** Verify `v10.0.0` build on Windows/Ubuntu/WASM. The new `Multimedia` and `RHI` components must link correctly.

## Implementation Tasks (Phase 45 Focus)

### 1. Multiplayer OS Hardening
- [ ] Implement `OmniUserManager::lockWindow(userId, windowId)` to physically block other users from interacting with private windows.
- [ ] Sync the `OmniTimeMachine` ledger across the `OmniMeshNode` so peers can "Rewind" each other's file states.

### 2. Graphics Perfection
- [ ] Finish the `OmniRhiNode` implementation for Metal/Vulkan backends in the Qt 7 Beta context.
