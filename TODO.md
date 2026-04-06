# TODO

## Immediate
- [ ] Add `go test ./internal/...` and `go build -buildvcs=false .` to Go CI.
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
- [x] Add a repeatable qtbase-native configure preflight script.
- [ ] Run the qtbase-native configure gate in an environment with a visible C/C++ compiler toolchain.
- [ ] Add stronger native end-to-end qtbase configure/install validation when practical.
- [ ] Extend BobUI module-package shims to other high-value public modules in small verified slices.

## Near-Term QML/Quick Focus
- [x] Upgrade `WebView` JS bridge from compile-safe contract to executable runtime semantics.
- [x] Add touch/swipe behavior primitives.
- [ ] Add multi-touch/pinch/rotation primitives after the single-swipe baseline is exercised further.
- [ ] Re-audit declarative state/transition parity versus QML/Quick.
- [x] Add automated validation around WebView bridge queueing, correlation, and error routing.
- [x] Add automated tests around navigation/history callback behavior.
- [ ] Use the new BTK comparison doc to keep parity language grounded in verified runtime behavior rather than repo breadth.

## Quality
- [x] Add tests for Go kernel/data/net packages.
- [x] Expand coverage into additional kernel services such as undo/clipboard/merge logic.
- [ ] Keep docs aligned with verified state.
- [ ] Revisit which abstractions should remain BobUI experiments versus long-term native framework candidates after each major parity milestone.
