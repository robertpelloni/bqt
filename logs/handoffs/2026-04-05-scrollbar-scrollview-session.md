# Handoff Archive - 2026-04-05 - ScrollBar / ScrollView Runtime Session

## Summary
This session implemented the next recommended Go runtime milestone after the BTK comparison work: interactive `ScrollBar` behavior coupled to `ScrollView` state.

## What changed
- Reworked `internal/ui/widgets/scrollbar.go` so the BobUI `ScrollBar` now delegates interaction state to Gio's scrollbar gesture logic.
- Added runtime fields for hover, dragging, and scroll delta.
- Reworked `internal/ui/widgets/scrollview.go` to use a Gio widget list state and derive normalized viewport ranges from list position.
- Applied returned scrollbar delta back to the list via `ScrollBy(...)`.
- Updated `internal/ui/widgets/demo_surface.go` to include a fixed-height scrolling demo region with 40 sample rows and live status text.
- Added implementation/testing docs under `docs/ai/implementation/` and `docs/ai/testing/`.

## Validation
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Key finding
The best way to keep BobUI honest is to continue using compact, proven Gio primitives internally while exposing a BobUI-owned framework surface externally. This yields real behavior without pretending the runtime is more complete than it is.

## Recommended next steps
1. Add touch/swipe primitives.
2. Improve visual/runtime polish for scrollbar occupancy and two-axis behavior.
3. Upgrade the Go `WebView` bridge from contract-only behavior toward executable runtime semantics.
