# Project Roadmap

## 1. Scope & Ownership
- [x] `bobui` is the framework/kernel.
- [ ] `bobfilez` remains the shell/desktop product.

## 2. Verified Go Baseline
- [x] `go test ./internal/...` passes.
- [x] `go build -buildvcs=false .` succeeds.
- [ ] Add these exact commands to CI.

## 3. QML / Quick / QuickControls2 / WebEngineQuick Focus (Current)
- [x] Add a first Go-native QuickControls2-style `Popup` primitive.
- [x] Add baseline `Menu`, `Dialog`, `Drawer`, `ToolTip`, `ScrollView`, and `ScrollBar` primitives.
- [x] Add a lightweight `WebView` navigation/history model.
- [x] Add baseline `WebView` callbacks (`OnNavigate`, `OnLoad`, `OnTitleChanged`, `OnHistoryChanged`, `OnScriptMessage`).
- [x] Add a compile-safe demo composition surface for the new controls.
- [ ] Add runtime-integrated `ScrollBar` behavior and richer demo wiring.
- [ ] Add touch/swipe behavior primitives.
- [ ] Formalize the `WebView` JS bridge contract.

## 4. Framework Parity Work
- [ ] Continue runtime-verifying parity claims instead of expanding only by file count.
- [ ] Strengthen model/view parity (`TableView`, selection, sorting, delegates).
- [ ] Tighten JUCE-style parameter/state automation and mesh-aware DSP routing.

## 5. Stabilization
- [ ] Replace remaining placeholder Gio widgets with richer implementations.
- [ ] Continue reducing package coupling.
- [ ] Add tests for Go kernel/data/net packages.
