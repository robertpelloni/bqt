# Changelog

All notable changes to this project will be documented in this file.

## [1.1.7] - 2026-04-02
### Verified
- `go test ./internal/...` passes after adding baseline QuickControls2-style controls.
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

## [1.1.4] - 2026-04-02
### Added
- Added `docs/REPO_HYGIENE_AUDIT.md` documenting the large untracked artifact set created by the earlier blanket `qt` → `bobui` rename.

### Changed
- Updated `TODO.md` and `HANDOFF.md` to prioritize repository hygiene and verified Go work over blindly staging renamed legacy artifacts.

### Verified
- `go test ./internal/...` still passes.
- `go build -buildvcs=false .` still succeeds.

## [1.1.3] - 2026-04-02
### Added
- Added `docs/FRAMEWORK_PARITY_AUDIT.md`, a grounded comparison of current `bobui` capabilities against Qt 6/7, JUCE, JavaFX, and Dear ImGui.
- Added a first Go-native `TableView` baseline in `internal/ui/widgets/table.go` to begin closing a real model/view parity gap.
