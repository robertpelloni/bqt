# Codebase Memory and Observations

## High-Confidence Findings
- `bobui` should be treated as the **framework/kernel**, not the full shell/desktop product. The shell belongs to `bobfilez`.
- The repository now contains **both** a large C++/BobUI/OmniUI codebase and a large experimental Go port.
- The Go port is **not fully build-verified**. A direct `go test ./internal/...` currently exposes package cycles and incomplete integrations.
- The C++ tree contains many exploratory components; some are stubs or partial implementations intended as architectural placeholders.
- Documentation drift is severe: several files claim total completion, while the actual code still needs stabilization and verification.

## Verified Technical Problems
- Go package cycles exist, especially around `internal/net`, `internal/kernel`, `internal/vm`, and `internal/ui`.
- Some Go files use placeholder or invalid APIs and need compile-focused cleanup.
- Some docs still describe `bobui` as a full OS shell instead of a framework consumed by `bobfilez`.
- `VERSION.md` and `CHANGELOG.md` became inconsistent due prior uncontrolled bumps.

## Design Preferences Going Forward
- Treat previous large-scale additions as **experimental until compiled**.
- Prefer fixing compile/runtime integrity over adding new speculative layers.
- Preserve interesting ideas, but move them to `IDEAS.md` unless or until implemented and verified.
- Keep `bobui` focused on framework parity and multiplayer foundations.

## Immediate Priorities
1. Stabilize Go package graph.
2. Normalize docs to actual scope.
3. Verify build/test status incrementally.
4. Implement missing framework features only after baseline integrity is restored.

## 2026-04-04 BTK Comparison Findings
- The active Go `bobui` framework track is much smaller than `BTK` and should be treated as an experimental, fast-iteration framework lane rather than a mature native framework replacement today.
- `BTK` is substantially stronger in native framework breadth, module depth, packaging, and staged compatibility/migration strategy.
- `bobui` remains stronger in explicit framework-vs-shell separation, collaboration framing, and lightweight modern bridge-contract experimentation.
- `BTK` has already started formalizing BobUI-inspired ownership semantics in stronger C++ types such as `BtkInputOwner`, `BtkFocusToken`, and `BtkInputArbitrator`.
- The right strategic relationship is complementary, not competitive: `BTK` as native framework mainline, `bobui` as idea incubator and parity/abstraction lab.
- `bobui` should keep prioritizing verified runtime semantics, especially around `ScrollBar`/`ScrollView` interaction and `WebView` bridge execution, instead of broad but shallow surface expansion.
- 2026-04-05: The Go `ScrollBar`/`ScrollView` baseline now has compile-verified interaction coupling by reusing Gio's internal scrollbar gesture machinery underneath a BobUI-owned widget surface.
- `DemoSurface` now exercises the scrolling baseline with sample content and live status text, which gives future runtime work a visible integration target.
- 2026-04-05: The Go baseline now includes a `TouchArea` primitive with directional swipe recognition and optional mouse-as-touch fallback for development on non-touch systems.
- The touch/swipe addition is intentionally small and framework-owned; it is a foundation for later multi-touch and gesture-composition work, not a claim of full gesture-stack parity.
- 2026-04-05: The Go `WebView` bridge now supports a small executable runtime model with queueing, pumping, eval handlers, correlated request/reply behavior, and explicit external message handling.
- The most useful near-term pattern remains: use Gio internals for proven interaction mechanics while preserving BobUI-owned framework abstractions and honest scope boundaries.
- 2026-04-05: `ScrollView` now supports overlay vs occupy placement and a lightweight secondary-axis viewport model, improving realism without pretending to have full 2D virtualization.
- 2026-04-05: `Engine.Run()` now uses a real Gio window/frame loop, rendering `LoginView`, `DemoSurface`, and `Shell` in a live runtime path instead of only safe initialization.
- 2026-04-05: `internal/ui/widgets/webview_test.go` now provides automated coverage for WebView bridge queueing, reply correlation, eval handling, error propagation, and external message routing.
- 2026-04-05: `internal/ui/engine_test.go` now provides automated validation for live runtime composition using synthetic Gio frame contexts, covering both login and authenticated runtime states.
- 2026-04-05: `internal/ui/widgets/webview_test.go` now also covers navigation/history semantics including callback firing, back/forward behavior, and forward-history truncation.
- 2026-04-05: `internal/ui/widgets/shell_test.go` now provides synthetic frame-context coverage for taskbar start-button toggling and start-menu visibility/layout behavior.
- 2026-04-05: `internal/kernel/managers_test.go`, `internal/data/data_test.go`, and `internal/net/mesh_node_test.go` now provide the first focused automated coverage for Go kernel/data/net support packages.
- 2026-04-05: `internal/kernel/services_test.go` now adds deterministic coverage for `UndoStack`, `Clipboard`, and `MergeKernel`, extending verification into higher-value kernel services.
- 2026-04-05: The Go `WindowManager` is now integrated into the live runtime path, with seeded managed windows and deterministic tests covering initialization, reset/spawn behavior, and layout dimensions.
- 2026-04-05: Managed windows now have clickable tabs, tab-aware visibility filtering, and per-window body content, making runtime composition more meaningful without claiming full docking complexity.
- 2026-04-05: Managed windows now also have a first direct manipulation baseline through close/move/resize helpers plus deterministic manager/window tests covering that behavior.
