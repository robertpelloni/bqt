# Changelog

All notable changes to this project will be documented in this file.

## [1.1.0] - 2026-04-03
### Added
- **Liquid Glass Edition**: Complete Notepad++ UI overhaul with Apple-inspired liquid glass aesthetics.
- `LiquidGlass.h` — Comprehensive glass material system:
  - `LiquidGlassPalette`: Centralized color/opacity token system (17 design tokens).
  - `LiquidGlassDWM`: Windows DWM integration (Win11 Acrylic, Win10 Acrylic, Vista blur-behind).
    - `enableBlurBehind()`: Real OS-level backdrop blur via DWM.
    - `setDarkMode()`: DWM dark title bar chrome.
    - `setRoundedCorners()`: Windows 11 window corner rounding preference.
    - `setBorderColor()`: DWM border accent color tinted to glass rim.
  - `LiquidGlassPainter`: Multi-layer glass surface renderer:
    - `drawGlassBackground()`: 7-layer glass: tint→sheen→refraction→rim highlight→rim shadow→border.
    - `drawAccentGlow()`: Radial focus glow for active elements.
    - `drawLiquidDrop()`: Animated teardrop/bubble with specular catch-light.
  - `LiquidGlassEffect` (QGraphicsEffect subclass): Software multi-pass Gaussian blur fallback
    for inner panels (3× box blur approximation = O(n) per pixel).
  - `LiquidGlassStyleSheet`: Complete QSS token library for: MainWindow, MenuBar, Menu,
    TabWidget/TabBar, StatusBar, ToolBar, Editor (QPlainTextEdit + scrollbars), Dialog.
- `NppLiquidGlass_Main.cpp` — Full Liquid Glass main window:
  - `BubbleOverlay`: Animated liquid bubble decoration widget (mouse-transparent, 18 bubble cap,
    sinusoidal drift, alpha lifecycle fade-in/out, 5-color palette, ~30fps timer).
  - `GlassEditorPanel`: Glass-framed editor widget with paintEvent glass rim painting.
  - `GlassStatusWidget`: Custom glass status bar with live stats (Ln/Col/Sel/Words/Encoding/EOL/Zoom/Version).
  - `GlassFindDialog`: Frameless, draggable glass find/replace dialog (regex, case, whole word).
  - `BobNppGlassWindow`: Full-featured main window with:
    - DWM glass activated in showEvent (deferred HWND).
    - Full menu parity: File/Edit/Search/View/Encoding/Language/TextFX/Macro/Run/Window/Settings/Help.
    - All TextFX operations wired with status bar feedback.
    - Glass intensity toggle (Mica/Acrylic/MicaTabbed) in Settings menu.
    - Bubble animations toggle in View menu.
    - Find/Replace: find-next, replace, replace-all, wrap-around.
    - Unsaved changes detection on tab close.
    - Multi-file open, Save All, Reload, Clone Tab.
    - HTML entity encode/decode in TextFX HTML Tidy submenu.
    - Blank operations: Trim Trailing/Leading Space, Tab→Space.
- CMakeLists.txt: Added `npp_liquid_glass` target (WIN32 subsystem) with dwmapi linkage.
- `VERSION` file at repo root (single source of truth — read at runtime by the app).
- Version propagated to both CMake targets via `file(READ VERSION)`.
- Per-monitor DPI awareness V2 enabled at startup (Windows 10 1703+ HWND-based).

## [1.0.3] - 2026-04-02
### Added
- Added `internal/kernel/fs_watcher.go` and `internal/kernel/fs_watcher_test.go` to provide a Go-native process matching `QFileSystemWatcher` functionality.
- Added implementation documentation at `docs/ai/implementation/2026-04-05-bobui-go-fs-watcher-parity.md`.
- Added an archived session handoff at `logs/handoffs/2026-04-05-bobui-go-fs-watcher-parity-session.md`.

### Changed
- Performed a **Targeted Internal Source Rename** of the `Concurrent` module in C++, replacing `QtConcurrent` with `BobUIConcurrent` and renaming `qtconcurrent_global.h` to `bobuiconcurrent_global.h`.
- Updated `src/concurrent/CMakeLists.txt` to include the new BobUI headers.
- Added **Core Class Aliases** to `src/corelib/global/bobuimacros.h`, aliasing `QObject` to `BobUI::Object`, `QString` to `BobUI::String`, and `QVariant` to `BobUI::Variant`.

### Verified
- `cmake -P cmake/tests/bobui_full_compatibility_validation.cmake` passes.
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

## [1.1.51] - 2026-04-05
### Added
- Comprehensive documentation overhaul (VISION.md, MEMORY.md, DEPLOY.md, ROADMAP.md, TODO.md, IDEAS.md).
- Global AI instruction files (AGENTS.md, CLAUDE.md, GEMINI.md, GPT.md, copilot-instructions.md) standardizing development workflow.
- Established autonomous session loop protocols for multi-model AI handoffs.
- Initiated deep analysis of `bobui` towards 100% Qt6 parity.
### Added
- Conducted extreme deep-dive codebase analysis mapping out OS-level capabilities.
- Documented full architectural details for JUCE, JavaFX, and Dear ImGui integration in `OS_TOOLKIT_ANALYSIS.md`.
- Updated `HANDOFF.md` with targeted instructions for the next agent cycle.

## [1.0.2] - 2026-04-02
### Added
- Created `OS_TOOLKIT_ANALYSIS.md` detailing the architectural parity with Qt6/Qt7 and the path to becoming an OS-ready toolkit.

## [1.0.1] - 2026-04-01
### Added
- Comprehensive documentation overhaul (VISION.md, MEMORY.md, DEPLOY.md, ROADMAP.md, TODO.md, IDEAS.md).
- Global AI instruction files (AGENTS.md, CLAUDE.md, GEMINI.md, GPT.md, copilot-instructions.md) standardizing development workflow.
- Established autonomous session loop protocols for multi-model AI handoffs.
- Initiated deep analysis of `bobui` towards 100% Qt6 parity.
## [1.1.29] - 2026-04-05
### Added
- Added `docs/ai/design/2026-04-05-cpp-go-side-by-side-migration-strategy.md` documenting the safe staged strategy for keeping C++ and Go buildable side by side during naming migration.
- Added `docs/ai/implementation/2026-04-05-cpp-rename-safety-rules.md` documenting explicit guardrails for future C++ rename work.
- Added an archived session handoff at `logs/handoffs/2026-04-05-cpp-side-by-side-strategy-session.md`.

### Changed
- Updated `ROADMAP.md`, `TODO.md`, `MEMORY.md`, and `HANDOFF.md` to record compatibility-first bucketed migration as the safe path instead of a destructive global rename.

### Verified
- `go test ./internal/...` passes.
- `go build -buildvcs=false .` succeeds.

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
