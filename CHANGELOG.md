# Changelog

All notable changes to this project will be documented in this file.

## [1.1.6] - 2026-04-02
### Added
- Added a Go-native QuickControls2-style `ScrollBar` baseline in `internal/ui/widgets/scrollbar.go`.
- Expanded `internal/ui/widgets/scrollview.go` to carry policy/state concepts for richer scroll behavior.

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

### Changed
- Reframed `ROADMAP.md`, `TODO.md`, and `HANDOFF.md` around verified build status and the actual `bobui` scope.
- Continued the documentation correction that `bobui` is the framework/kernel and `bobfilez` owns the shell/desktop product.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.2] - 2026-04-02
### Fixed
- Stabilized the experimental Go tree enough to establish a verified baseline.
- Removed several major package cycles across `internal/kernel`, `internal/net`, `internal/vm`, and `internal/ui` by decoupling speculative cross-package integrations.
- Fixed multiple concrete compile issues in the Go port, including unused imports, invalid Gio API usage, a typo in `internal/kernel/plugin_manager.go`, and several build-breaking widget implementations.

### Verified
- `go test ./internal/...` now passes.
- `go build -buildvcs=false .` now succeeds for the root Go binary.

### Changed
- Rebased key docs (`VISION.md`, `MEMORY.md`, `DEPLOY.md`, `ROADMAP.md`, `TODO.md`, `HANDOFF.md`, `OmniUI/README.md`, `SUBMODULE_DASHBOARD.md`) around the verified state of the repository instead of prior aspirational claims.
- Clarified that `bobui` is the framework/kernel project and that the operating shell belongs to the adjacent `bobfilez` project.
