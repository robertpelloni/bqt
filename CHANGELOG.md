# Changelog

All notable changes to this project will be documented in this file.

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

### Changed
- Updated `ROADMAP.md`, `TODO.md`, and `HANDOFF.md` to prioritize:
  - runtime demo integration for the new controls
  - `WebView` signals / JS bridge semantics
  - `ScrollBar` behavior polish

## [1.1.6] - 2026-04-02
### Added
- Added a Go-native QuickControls2-style `ScrollBar` baseline in `internal/ui/widgets/scrollbar.go`.
- Expanded `internal/ui/widgets/scrollview.go` to carry policy/composition concepts for richer scroll behavior.

### Fixed
- Resolved a Go compile regression caused by helper-name collision in the new scrollbar baseline.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

### Changed
- Updated parity planning/docs to treat `ScrollView` / `ScrollBar` as the next verified QuickControls2 layer.

## [1.1.5] - 2026-04-02
### Added
- Added Go-native QuickControls2-style baseline controls for `Dialog`, `Drawer`, and `ToolTip` in `internal/ui/widgets/`.
- Extended `docs/QML_QUICK_PARITY_AUDIT.md` to reflect the new verified control coverage.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

### Changed
- Updated `ROADMAP.md`, `TODO.md`, and `HANDOFF.md` to prioritize the remaining Quick / QuickControls2 / WebEngineQuick gaps: `ScrollBar`, richer `WebView`, and runtime integration.
