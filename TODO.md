# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Monitor GitHub Actions:** Upon pushing this commit, verify that `.github/workflows/ci_cd.yml` completely passes the CMake build process and automatically tags and releases the `v3.0.0` zip files.

## Implementation Tasks (Phase 19 Focus)

### 1. Physical QA
- [ ] Download `HelloOmni-Windows.zip` from the GitHub Actions artifact tab.
- [ ] Run `HelloOmni.exe`. Press the Spacebar and use the `WASD` keys to verify that `OmniInputManager::simulateSecondaryCursorMove` accurately renders the Virtual DX Cursor on your high-DPI monitor via `OmniDeveloperOverlay`.
- [ ] Hover over the `OmniCodeEditor` or `OmniCalendar` and witness the dynamic `QMetaObject` ImGui tooltips tracking their `Q_PROPERTY` strings in real-time.
