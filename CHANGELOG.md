# Changelog

All notable changes to this project will be documented in this file.

## [1.1.9] - 2026-04-02
### Added
- Expanded the lightweight Go `WebView` baseline with a real event surface:
  - `OnNavigate`
  - `OnLoad`
  - `OnTitleChanged`
  - `OnHistoryChanged`
  - `OnScriptMessage`
- Added a compile-safe Go `DemoSurface` composition target that groups the new QuickControls2-style controls and WebView into a single runtime-oriented surface.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

### Changed
- Updated `internal/ui/engine.go` to initialize the demo surface and wire baseline WebView callbacks.
- Updated parity docs and planning to focus next on JS bridge semantics, richer runtime integration, and touch/swipe primitives.

## [1.1.8] - 2026-04-02
### Added
- Added `internal/ui/widgets/demo_surface.go`, a compile-safe Go composition surface that groups the QuickControls2-style baseline controls into a single framework demo target.
- Extended the lightweight Go `WebView` with baseline callback hooks for navigation and load events.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

### Changed
- Updated `internal/ui/engine.go` so the Go baseline initializes the demo surface and logs basic `WebView` callback activity.
- Updated planning docs to move next toward richer runtime integration and WebEngineQuick-style JS bridge semantics.

## [1.1.7] - 2026-04-02
### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds after adding baseline QuickControls2-style controls.

### Added
- Added Go-native baseline `Dialog`, `Drawer`, `ToolTip`, `Menu`, `ScrollView`, and `ScrollBar` primitives under `internal/ui/widgets/`.
- Expanded `docs/QML_QUICK_PARITY_AUDIT.md` to reflect concrete coverage and remaining Quick / QuickControls2 / WebEngineQuick gaps.
