# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub Actions:** Upon pushing this commit, verify that `.github/workflows/ci_cd.yml` passes perfectly on the `v2.6.0` release.

## Implementation Tasks (Phase 18 Focus)

### 1. Final Polishing Tasks
- [ ] Verify `OmniDeveloperOverlay` ImGui text rendering scaling across high-DPI (4k) screens.
- [ ] Add `std::mutex` locks inside `OmniInputManager`'s device mapping trees to ensure that when `devMode` and actual physical mice send simultaneous `updateCursor()` commands from multiple OS threads, a race condition does not occur.

### 2. Prepare for Version 3.0.0
- [ ] Collate the entire architectural design implemented by the AI multi-agents over the last 20 version bumps into a cohesive "BobUI OS 1.0 Release Manifesto" for the end-users.
