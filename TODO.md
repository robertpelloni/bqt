# Task List / TODO

## Immediate System Integrity Tasks
- [ ] **Run `python scripts/check_release.py`:** Verify the `v3.4.0` binary is publicly available from the GitHub Actions cloud compilation.

## Implementation Tasks (Phase 23 Focus)

### 1. Final Handover
- [ ] Launch `HelloOmni.exe`. 
- [ ] Call `OmniUI.ThemeManager.setCyberpunkMode()` in the QML frontend.
- [ ] Trigger a toast notification `OmniUI.NotificationCenter.showToast("System Online", "Welcome to OmniUI.", "success")`.
- [ ] Watch the brilliant native C++ `QPainter` overlay render the toast seamlessly in the top right corner.
