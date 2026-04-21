# Short-Term Execution Goals (TODO)

<<<<<<< HEAD
## Immediate
- [x] Add `go test ./internal/...` and `go build -buildvcs=false .` to Go CI.
- [x] Wire the `DemoSurface` into a real interactive Gio frame loop.
- [ ] Add richer runtime integration for `Popup`, `Dialog`, `Drawer`, `ToolTip`, `Menu`, `ScrollView`, and `ScrollBar`.
- [x] Add automated runtime validation for the live Gio frame loop.
- [x] Add richer synthetic frame-context coverage for shell/taskbar/start-menu interaction.
- [x] Implement interactive `ScrollBar` drag behavior and couple it to `ScrollView` state.
- [x] Improve scrollbar occupancy/overlay policy and two-axis behavior.
- [ ] Add richer scroll corner/theming behavior when both bars are visible.
- [x] Add richer managed-window content and interaction in the live runtime path.
- [x] Add managed-window move/resize/close interaction.
- [ ] Improve docking/tab behavior beyond active-tab filtering.
- [x] Add focus/activation semantics for managed windows.
- [x] Add pointer-driven activation/focus routing from direct window clicks.
- [ ] Add richer focus routing on top of activation state.
- [x] Classify the untracked C++ rename artifacts into canonical vs generated vs duplicate buckets before any broad migration work.
- [x] Choose one safe C++ package/export/header compatibility bucket for the first side-by-side migration implementation.
- [x] Compare tracked top-level `cmake/` files against renamed copies and mark canonical inputs.
- [x] Implement the first additive BobUI package/export compatibility slice in `cmake/` without deep source renames.
- [x] Wire the BobUI CMake compatibility shims into install/export generation without replacing the canonical Qt package layout in one shot.
- [x] Add an install-layout-style prefix-path validation path for `find_package(BobUI6 ...)` through a fake-prefix smoke test.
- [x] Add stronger configure-time validation for the modified top-level package-generation path via a publication-helper smoke test.
- [x] Add a first direct BobUI module-package slice for `Core` and `Widgets`.
- [x] Add a second BobUI module-package slice for `Gui` and `Network`.
- [x] Add a third BobUI module-package slice for `Sql` and `Xml`.
- [x] Add a fourth BobUI module-package slice for `OpenGL` and `OpenGLWidgets`.
- [x] Add a fifth BobUI module-package slice for `DBus` and `PrintSupport`.
- [x] Add a sixth BobUI module-package slice for `Concurrent` and `Test`.
- [x] Strengthen the umbrella BobUI package smoke test to cover a broader supported component matrix.
- [x] Centralize the supported BobUI component matrix in a manifest and validate it with a consistency test.
- [x] Add a checked human-readable supported component status report.
- [x] Generate the supported component status report from the manifest-backed component inventory.
- [x] Add a consolidated BobUI CMake compatibility validation command.
- [x] Wire the consolidated BobUI compatibility validation command into Go CI with CI-friendly native skip behavior.
- [x] Add a repeatable qtbase-native configure preflight script.
- [ ] Run the qtbase-native configure gate in an environment with a visible C/C++ compiler toolchain.
- [ ] Add stronger native end-to-end qtbase configure/install validation when practical.
- [ ] Extend BobUI module-package shims to other high-value public modules in small verified slices.
- [ ] Classify the untracked C++ rename artifacts into canonical vs generated vs duplicate buckets before any broad migration work.
- [ ] Choose one safe C++ package/export/header compatibility bucket for the first side-by-side migration implementation.
=======
## Phase 1 Parity Wrappers (Completed)
1. ~~**Build System Validation:**~~ Modularized `CMakeLists.txt` via `BobQSubmodules.cmake`.
2. ~~**Submodule Wrapper:**~~ Implemented `BobQJuceHost`.
3. ~~**Go Port Coverage:**~~ Generated `ANALYSIS.md`.
4. ~~**Documentation Sync:**~~ Generated `SUBMODULE_DASHBOARD.md`.
5. ~~**GTK Parity Research:**~~ Implemented `BobQBox` and `BobQGrid`.
>>>>>>> 677b0f352ad2c50efba02126daac7b26465b876d
=======
## Go Runtime Integration (Priority 1)
- [x] Add richer runtime integration for `Popup` and `Dialog`.
  - Extend the existing Gio-based widget implementations with focus semantics and overlay policies.
- [x] Add richer runtime integration for `Drawer`, `ToolTip`, and `Menu`.
  - These transient overlay widgets must compose cleanly into the `WindowManager`.
- [x] Improve `ScrollView` and `ScrollBar` behavior.
  - Complete the interactive ScrollBar drag behavior and couple it to ScrollView state.
  - Improve scrollbar overlay policy and two-axis behavior.
- [x] Write tests for all above widgets inside `internal/ui/widgets/`.
>>>>>>> origin/jules-11090863842246041945-58931a03

## C++ Core Fixes (Priority 2)
- [x] Consolidate BobUI CMake compatibility validation command and wire it into Go CI. (Added `cmake/tests/bobui_consolidated_compatibility_validation.cmake` and updated `.github/workflows/go_wasm.yml`).
- [ ] Complete the generic component registry (`BobQUppComponentRegistry`) so any QML string can summon a U++ or JUCE component.

## Refactoring & Documentation
- [x] Outline U++ and JUCE bridging details explicitly inside `Manual.md` and the universal instruction suite.
- [ ] Audit the BobQ C++ codebase to ensure that variable naming, structure, and paradigms match the cleanliness of the GTK implementation.
- [ ] Continue porting `OmniAudioGraph` to pure Go.
