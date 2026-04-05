# Changelog

All notable changes to this project will be documented in this file.

## [1.1.28] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-pointer-activation-routing.md` documenting direct window activation routing into manager-owned activation state.
- Added `docs/ai/testing/2026-04-05-pointer-activation-routing-tests.md` documenting validation for pointer-style activation routing.
- Added an archived session handoff at `logs/handoffs/2026-04-05-pointer-activation-routing-session.md`.

### Changed
- Reworked `internal/ui/widgets/window.go` to expose direct activation requests through a click surface.
- Reworked `internal/ui/wm.go` so `WindowManager.Layout(...)` routes window activation requests through manager activation logic.
- Extended `internal/ui/widgets/window_test.go` and `internal/ui/wm_test.go` to validate one-shot activation requests and click-routed active-window transitions.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified pointer activation routing milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.27] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-managed-window-activation.md` documenting the managed-window focus/activation baseline.
- Added `docs/ai/testing/2026-04-05-managed-window-activation-tests.md` documenting validation for managed-window activation behavior.
- Added an archived session handoff at `logs/handoffs/2026-04-05-managed-window-activation-session.md`.

### Changed
- Reworked `internal/ui/wm.go` to track an active window, promote activated windows, and choose fallback active windows when visibility changes.
- Reworked `internal/ui/widgets/window.go` to reflect active/inactive visual state.
- Extended `internal/ui/wm_test.go` to validate activation, ordering promotion, and fallback behavior.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified managed-window activation milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.26] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-managed-window-manipulation.md` documenting the managed-window move/resize/close baseline.
- Added `docs/ai/testing/2026-04-05-managed-window-manipulation-tests.md` documenting validation for managed-window manipulation behavior.
- Added `internal/ui/widgets/window_test.go` with deterministic tests for window-level move/resize/close behavior.
- Added an archived session handoff at `logs/handoffs/2026-04-05-managed-window-manipulation-session.md`.

### Changed
- Reworked `internal/ui/widgets/window.go` to support close state, move/resize helpers, drag handlers, and a close button.
- Reworked `internal/ui/wm.go` to expose `MoveWindow(...)`, `ResizeWindow(...)`, and `CloseWindow(...)` and to hide closed windows.
- Extended `internal/ui/wm_test.go` to validate manager-level manipulation behavior.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified managed-window manipulation milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.25] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-managed-window-interaction.md` documenting clickable tabs and tab-aware managed-window content/visibility.
- Added `docs/ai/testing/2026-04-05-managed-window-interaction-tests.md` documenting validation for managed-window interaction behavior.
- Added an archived session handoff at `logs/handoffs/2026-04-05-managed-window-interaction-session.md`.

### Changed
- Reworked `internal/ui/widgets/tabbar.go` to support real clickable tab selection.
- Reworked `internal/ui/widgets/window.go` to render richer title/body metadata and carry tab/content semantics.
- Reworked `internal/ui/wm.go` to expose active-tab and visible-window behavior.
- Reworked `internal/ui/engine.go` to seed tab-aware managed windows with meaningful content.
- Extended `internal/ui/wm_test.go` and `internal/ui/engine_test.go` to validate tab-aware runtime composition behavior.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified managed-window interaction milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.24] - 2026-04-05
### Added
- Added `docs/ai/implementation/2026-04-05-window-manager-runtime-integration.md` documenting the first meaningful Go window-manager runtime integration.
- Added `docs/ai/testing/2026-04-05-window-manager-runtime-tests.md` documenting validation for the new managed-window composition path.
- Added `internal/ui/wm_test.go` with deterministic tests for `WindowManager` reset/spawn/layout behavior.
- Added an archived session handoff at `logs/handoffs/2026-04-05-window-manager-runtime-session.md`.

### Changed
- Reworked `internal/ui/engine.go` so the authenticated runtime now includes managed windows/tabs through the existing `WindowManager` path.
- Reworked `internal/ui/wm.go` to support reset and real layout return values.
- Extended `internal/ui/engine_test.go` to validate managed-window initialization in the live runtime path.
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect the verified window-manager runtime milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.23] - 2026-04-05
### Added
- Added `internal/kernel/services_test.go` covering `UndoStack`, `Clipboard`, and `MergeKernel` deterministic behavior.
- Added `docs/ai/testing/2026-04-05-kernel-services-tests.md` documenting the new kernel-services test coverage.
- Added an archived session handoff at `logs/handoffs/2026-04-05-kernel-services-tests-session.md`.

### Changed
- Updated `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect completion of deterministic coverage for undo/clipboard/merge kernel services.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.22] - 2026-04-05
### Added
- Added `internal/kernel/managers_test.go` covering `InputManager`, `PermissionManager`, and `UserManager` behavior.
- Added `internal/data/data_test.go` covering `VFS`, in-memory SQLite query execution, and safe mesh-linked data-layer calls.
- Added `internal/net/mesh_node_test.go` covering `GetMeshNode()` singleton behavior and empty-peer broadcast safety.
- Added `docs/ai/testing/2026-04-05-kernel-data-net-tests.md` documenting the new kernel/data/net test coverage.
- Added an archived session handoff at `logs/handoffs/2026-04-05-kernel-data-net-tests-session.md`.

### Changed
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect completion of the first focused Go kernel/data/net test milestone.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.21] - 2026-04-05
### Added
- Added `internal/ui/widgets/shell_test.go` with synthetic runtime tests for taskbar start-button toggling and start-menu visibility/layout behavior.
- Added `docs/ai/testing/2026-04-05-shell-runtime-tests.md` documenting the new shell runtime test coverage.
- Added an archived session handoff at `logs/handoffs/2026-04-05-shell-tests-session.md`.

### Changed
- Updated `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect completion of synthetic shell/taskbar/start-menu test coverage.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.20] - 2026-04-05
### Added
- Added `docs/ai/testing/2026-04-05-webview-navigation-history-tests.md` documenting automated WebView navigation/history coverage.
- Added an archived session handoff at `logs/handoffs/2026-04-05-webview-navigation-tests-session.md`.

### Changed
- Extended `internal/ui/widgets/webview_test.go` with automated tests for navigation/history callback behavior, back/forward state changes, and forward-history truncation.
- Updated `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect completion of automated WebView navigation/history validation.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.19] - 2026-04-05
### Added
- Added `internal/ui/engine_test.go` with automated validation for the live Gio runtime composition path.
- Added `docs/ai/testing/2026-04-05-live-gio-frame-loop-automated-tests.md` documenting the new live runtime test coverage.
- Added an archived session handoff at `logs/handoffs/2026-04-05-live-gio-tests-session.md`.

### Changed
- Updated `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to reflect completion of automated live Gio runtime validation using synthetic frame contexts.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

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
