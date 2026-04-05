# Changelog

All notable changes to this project will be documented in this file.

## [1.1.18] - 2026-04-05
### Added
- Added `internal/ui/widgets/webview_test.go` with automated unit tests for WebView bridge queueing, correlation, eval handling, error propagation, and external message routing.
- Added `docs/ai/testing/2026-04-05-webview-runtime-automated-tests.md` documenting the new WebView bridge test coverage.
- Added an archived session handoff at `logs/handoffs/2026-04-05-webview-tests-session.md`.

### Changed
- Updated `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the completion of automated WebView bridge validation.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.17] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-live-gio-frame-loop.md` documenting the live Gio runtime integration milestone.
- Added `docs/ai/testing/2026-04-05-live-gio-frame-loop-validation.md` documenting validation for the live frame loop path.
- Added an archived session handoff at `logs/handoffs/2026-04-05-live-gio-frame-loop-session.md`.

### Changed
- Reworked `internal/ui/engine.go` so `Engine.Run()` now opens a real Gio window, processes frame/destroy events, and renders the existing login/demo/shell composition in a live runtime surface.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified live Gio frame loop milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.16] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-scrollbar-occupancy-two-axis.md` documenting overlay/occupy placement and lightweight two-axis scroll behavior.
- Added `docs/ai/testing/2026-04-05-scrollbar-occupancy-validation.md` documenting validation for the scroll occupancy/two-axis milestone.
- Added an archived session handoff at `logs/handoffs/2026-04-05-scrollbar-occupancy-session.md`.

### Changed
- Reworked `internal/ui/widgets/scrollview.go` to support overlay vs occupy scrollbar placement, occupied-space relayout, and lightweight secondary-axis viewport behavior.
- Updated `internal/ui/widgets/demo_surface.go` to visibly exercise occupied vertical and horizontal scrollbar behavior and report live state.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified scroll occupancy/two-axis milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.15] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-webview-runtime-semantics.md` documenting the executable local WebView bridge runtime milestone.
- Added `docs/ai/testing/2026-04-05-webview-runtime-validation.md` documenting validation for the upgraded WebView bridge.
- Added an archived session handoff at `logs/handoffs/2026-04-05-webview-runtime-session.md`.

### Changed
- Reworked `internal/ui/widgets/webview.go` to support queue/pump runtime behavior, eval handlers, correlated request/reply routing, and explicit external message handling.
- Updated `internal/ui/engine.go` to exercise the new WebView runtime semantics through request handlers, eval handling, and externally supplied script messages.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified WebView runtime milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.14] - 2026-04-05
### Added
- Added `internal/ui/widgets/touch.go` with a lightweight `TouchArea` primitive and `SwipeDirection` model for the verified Go baseline.
- Added `docs/ai/implementation/2026-04-05-touch-swipe-primitives.md` documenting the new touch/swipe milestone.
- Added `docs/ai/testing/2026-04-05-touch-swipe-validation.md` documenting validation for the touch/swipe baseline.
- Added an archived session handoff at `logs/handoffs/2026-04-05-touch-swipe-session.md`.

### Changed
- Updated `internal/ui/widgets/demo_surface.go` to visibly exercise the new touch/swipe primitive and report the latest swipe.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified touch/swipe milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.13] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-scrollbar-scrollview-runtime.md` documenting the first runtime-coupled Go scroll implementation step.
- Added `docs/ai/testing/2026-04-05-scrollbar-scrollview-validation.md` documenting validation for the new scroll behavior baseline.
- Added an archived session handoff at `logs/handoffs/2026-04-05-scrollbar-scrollview-session.md`.

### Changed
- Reworked `internal/ui/widgets/scrollbar.go` to support interactive click/drag state using Gio's scrollbar gesture machinery underneath the BobUI widget surface.
- Reworked `internal/ui/widgets/scrollview.go` to couple list position to normalized scrollbar viewport state and apply returned delta back into the list.
- Updated `internal/ui/widgets/demo_surface.go` to visibly exercise the scroll baseline with sample items and live status text.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified runtime milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.12] - 2026-04-04
### Added
- Added `docs/ai/design/2026-04-04-bobui-vs-btk-comparison.md`, a grounded comparison between the active Go `bobui` framework track and `BTK`.
- Added an archived session handoff at `logs/handoffs/2026-04-04-btk-comparison-session.md`.

### Changed
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, `VISION.md`, `IDEAS.md`, and `HANDOFF.md` to reflect the BTK comparison findings and clarify BobUI's strategic role as an idea/incubation track for framework primitives and ownership semantics.
- Kept planning focused on verified runtime behavior, especially upcoming `ScrollBar`/`ScrollView` interaction work and future `WebView` runtime semantics.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.11] - 2026-04-02
### Added
- Extended the Go `WebView` baseline into a richer WebEngineQuick-style bridge contract:
  - handler registration via `RegisterHandler(...)`
  - request/reply semantics via `Request(...)`
  - structured message metadata with `id`, `channel`, `payload`, and `kind`
  - machine-readable bridge description via `BridgeContractJSON()`

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

### Changed
- Updated the Go engine baseline to exercise navigation, load, title, history, script-message, and request/reply bridge activity.
- Updated the Quick/QuickControls2/WebEngineQuick audit and planning docs to reflect the richer bridge contract.

## [1.1.10] - 2026-04-02
### Added
- Extended the lightweight Go `WebView` with a richer WebEngineQuick-style event and bridge surface:
  - `OnNavigate`
  - `OnLoad`
  - `OnTitleChanged`
  - `OnHistoryChanged`
  - `OnScriptMessage`
  - `EvalJS(...)`
  - `PostMessage(...)`
  - `BridgeContractJSON()`

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

### Changed
- Updated the Go engine baseline to initialize and log the extended WebView event surface.
- Updated parity docs and planning around JS bridge semantics and richer runtime integration.

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
- Updated planning docs to move next toward richer runtime integration and WebEngineQuick-style JS bridge semantics.
