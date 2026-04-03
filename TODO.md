# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Run `python scripts/check_release.py`:** Upon pushing this commit, wait exactly 5 minutes for GitHub Actions to compile the massive new `OmniMeshNode` networking architecture, then use the script to verify the `v7.0.0` binaries are publicly available.

## Implementation Tasks (Phase 38 Focus)

### 1. OmniAnimator
- [ ] Implement `OmniAnimator.cpp`. Build a static manager that receives a target `QObject*`, a `propertyName`, a target value, and a duration, using native C++ interpolation curves to slide properties smoothly.

### 2. OmniTabBar (Docking)
- [ ] Build `OmniTabBar.cpp`. Allow `OmniWindow` instances to snap into the bar.

### 3. Multiplayer Verification
- [ ] Deploy two instances of `HelloOmni.exe` on your local machine. Trigger `OmniNet.MeshNode.startNode()` on one and `connectToPeer("ws://localhost:8081")` on the other. Physically witness the red cursor rendering smoothly on the server node while you drag the client mouse.
