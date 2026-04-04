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
- Layout primitives exist (split, flow, grid, stack, tile)
- Scroll container baseline (`internal/ui/widgets/scrollview.go`)

### Still missing or incomplete
- ScrollBar visual parity and policy control
- Swipe / touch-driven control parity
- State/transition ergonomics comparable to declarative QML animations
- Rich validation / focus / accessibility semantics across all controls

## 3. WebEngineQuick Progress
### Present in some form
- Lightweight Go-native `WebView` navigation/history model exists
- Asset/VFS and markdown/document rendering exist

### Missing / incomplete
- Real embedded browser parity
- JS bridge / page lifecycle / history / navigation signals beyond minimal baseline
- Remote content sandboxing and permissions
- Resource interception comparable to request interceptors

## 4. Recommended Next Framework Targets
1. Add `ScrollBar` visuals and policies.
2. Add touch/swipe behavior primitives.
3. Add `WebView` navigation signals and JS bridge semantics.
4. Continue runtime validation instead of file-count-based parity claims.
