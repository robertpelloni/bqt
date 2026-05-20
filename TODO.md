# Short-Term Execution Goals (TODO)

## Phase 1 Parity Wrappers (Completed)
1. ~~**Build System Validation:**~~ Modularized `CMakeLists.txt` via `BobQSubmodules.cmake`.
2. ~~**Submodule Wrapper:**~~ Implemented `BobQJuceHost`.
3. ~~**Go Port Coverage:**~~ Generated `ANALYSIS.md`.
4. ~~**Documentation Sync:**~~ Generated `SUBMODULE_DASHBOARD.md`.
5. ~~**GTK Parity Research:**~~ Implemented `BobQBox` and `BobQGrid`.
## Go Runtime Integration (Priority 1)
- [x] Add richer runtime integration for `Popup` and `Dialog`.
  - Extend the existing Gio-based widget implementations with focus semantics and overlay policies.
- [x] Add richer runtime integration for `Drawer`, `ToolTip`, and `Menu`.
  - These transient overlay widgets must compose cleanly into the `WindowManager`.
- [x] Improve `ScrollView` and `ScrollBar` behavior.
  - Complete the interactive ScrollBar drag behavior and couple it to ScrollView state.
  - Improve scrollbar overlay policy and two-axis behavior.
- [x] Write tests for all above widgets inside `internal/ui/widgets/`.

## C++ Core Fixes (Priority 2)
- [x] Consolidate BobUI CMake compatibility validation command and wire it into Go CI. (Added `cmake/tests/bobui_consolidated_compatibility_validation.cmake` and updated `.github/workflows/go_wasm.yml`).
- [ ] Complete the generic component registry (`BobQUppComponentRegistry`) so any QML string can summon a U++ or JUCE component.

## Refactoring & Documentation
- [x] Outline U++ and JUCE bridging details explicitly inside `Manual.md` and the universal instruction suite.
- [ ] Audit the BobQ C++ codebase to ensure that variable naming, structure, and paradigms match the cleanliness of the GTK implementation.
- [x] Continue porting `OmniAudioGraph` to pure Go.
