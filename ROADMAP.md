# Project Roadmap

## 1. Scope & Ownership
- [x] `bobui` is the framework/kernel.
- [ ] `bobfilez` remains the shell/desktop product.

## 2. Verified Go Baseline
- [x] `go test ./internal/...` passes.
- [x] `go build -buildvcs=false .` succeeds.
- [ ] Add these exact commands to CI.
- [x] Wire the current Go demo primitives into a live Gio frame loop.

## 3. QML / Quick / QuickControls2 / WebEngineQuick Focus (Current)
- [x] Add a first Go-native QuickControls2-style `Popup` primitive.
- [x] Add baseline `Menu`, `Dialog`, `Drawer`, `ToolTip`, `ScrollView`, and `ScrollBar` primitives.
- [x] Add a lightweight `WebView` navigation/history model.
- [x] Add a richer event surface and bridge contract (`EvalJS`, `PostMessage`, `Request`, handler registration).
- [x] Add a compile-safe demo composition surface for the new controls.
- [x] Add runtime-integrated `ScrollBar` behavior and richer demo wiring.
- [x] Add touch/swipe behavior primitives.
- [x] Upgrade the `WebView` bridge from compile-safe contract to executable runtime semantics.

## 4. Framework Parity Work
- [ ] Continue runtime-verifying parity claims instead of expanding only by file count.
- [ ] Strengthen model/view parity (`TableView`, selection, sorting, delegates).
- [ ] Tighten JUCE-style parameter/state automation and mesh-aware DSP routing.
- [x] Document a grounded comparison between the active Go `bobui` track and `BTK` to clarify strategic boundaries.
- [ ] Use the BTK comparison to keep `bobui` focused on experimental framework primitives, collaboration semantics, and bridge/runtime ideas rather than inflated breadth claims.

## 5. Stabilization
- [ ] Replace remaining placeholder Gio widgets with richer implementations.
- [ ] Continue reducing package coupling.
- [x] Add tests for Go kernel/data/net packages.
- [x] Improve `ScrollView` scrollbar occupancy/overlay policy and lightweight two-axis behavior.
- [x] Integrate the Go `WindowManager` into the live runtime composition path.
- [x] Add clickable tab selection and tab-aware managed-window visibility/content.
- [x] Add managed-window move/resize/close baseline behavior.
- [x] Add managed-window focus/activation baseline behavior.
- [x] Add pointer-driven activation routing from direct window interaction.

## 6. Strategic Alignment
- [x] Record the current `BTK` relationship in `docs/ai/design/2026-04-04-bobui-vs-btk-comparison.md`.
- [x] Document a safe staged strategy for C++/Go side-by-side migration instead of a destructive global `qt` -> `bobui` rename.
- [ ] Keep `bobui` positioned as a fast idea/incubation track for ownership semantics, bridge contracts, and compile-safe parity exploration.
- [ ] Promote ideas from `bobui` into larger native framework tracks only after they are scoped, validated, and clearly separated from shell/product concerns.
