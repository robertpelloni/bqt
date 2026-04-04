# QML / Quick / QuickControls2 / WebEngineQuick Parity Audit

## Scope
This audit is about what `bobui` currently provides as a framework and what remains missing or only partially represented, especially in the Go track.

## 1. QML / Quick Strengths Already Reflected
- Immediate widget rendering concepts exist.
- Multi-window state and scene-like composition exist.
- GPU-backed render loops exist in both C++ and Go experiments.
- Basic model/view and text/document rendering primitives exist.

## 2. QuickControls2 Progress
### Present in some form
- Button / CheckBox / ProgressBar / Slider / Dial
- Popup-like surface (`internal/ui/widgets/popup.go`)
- Dialog baseline (`internal/ui/widgets/dialog.go`)
- Drawer baseline (`internal/ui/widgets/drawer.go`)
- ToolTip baseline (`internal/ui/widgets/tooltip.go`)
- Menu baseline (`internal/ui/widgets/menu.go`)
- Scroll container baseline (`internal/ui/widgets/scrollview.go`)
- ScrollBar baseline (`internal/ui/widgets/scrollbar.go`)
- Layout primitives exist (split, flow, grid, stack, tile)

### Still missing or incomplete
- ScrollBar interaction polish and policy/runtime behavior coupling
- Swipe / touch-driven control parity
- State/transition ergonomics comparable to declarative QML animations
- Rich validation / focus / accessibility semantics across all controls

## 3. WebEngineQuick Progress
### Present in some form
- Lightweight Go-native `WebView` navigation/history model exists
- Baseline event hooks exist:
  - `OnNavigate`
  - `OnLoad`
  - `OnTitleChanged`
  - `OnHistoryChanged`
  - `OnScriptMessage`
- Baseline host/page bridge methods exist:
  - `EvalJS(...)`
  - `PostMessage(...)`
  - `BridgeContractJSON()`
- Asset/VFS and markdown/document rendering exist

### Missing / incomplete
- Real embedded browser parity
- Real JavaScript execution environment
- Remote content sandboxing and permissions
- Resource interception comparable to request interceptors

## 4. Recommended Next Framework Targets
1. Add runtime-integrated `ScrollBar` behavior demo.
2. Add touch/swipe behavior primitives.
3. Upgrade the WebView bridge from callback contract to executable bridge semantics.
4. Continue runtime validation instead of file-count-based parity claims.
