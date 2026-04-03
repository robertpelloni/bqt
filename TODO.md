# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub Actions:** Upon pushing this commit, verify that `.github/workflows/ci_cd.yml` completely passes the CMake build process now that the C++ linkage errors in the mock headers are fixed and the new files are added.

## Implementation Tasks (Phase 16 Focus)

### 1. Final Stability Audits
- [ ] Add exception handling wrappers to `OmniDatabase::executeQuery` so broken SQL syntax from the QML frontend does not crash the `C++` backend application natively.
- [ ] Optimize the drawing geometry caching in `OmniJuceView::paint` so high-framerate JUCE OpenGL context calls do not stall the Qt Main event loop unnecessarily.

### 2. Physical QA
- [ ] Spin up a Windows PC with two USB mice attached. Open `HelloOmni.exe`. Verify two red cursors appear and move independently based on the specific mouse being used.
