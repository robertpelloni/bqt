# ScrollBar / ScrollView Runtime Wiring

## Date
2026-04-05

## Scope
This note documents the first runtime-coupled Go implementation step for the QuickControls2-style scroll baseline in `bobui`.

Affected files:
- `internal/ui/widgets/scrollbar.go`
- `internal/ui/widgets/scrollview.go`
- `internal/ui/widgets/demo_surface.go`

## Goal
Move the Go `ScrollBar` and `ScrollView` from a static compile-safe surface toward actual interactive behavior while preserving build safety.

## What changed
### 1. `ScrollBar` now has real interaction state
The BobUI `ScrollBar` surface now wraps Gio's scrollbar gesture machinery internally while preserving BobUI-owned fields such as:
- `Orientation`
- `Position`
- `PageSize`
- `Visible`

Additional runtime state now tracked:
- `Hovered`
- `TrackHovered`
- `Dragging`
- `LastDelta`

### 2. Pointer-driven scroll semantics
The scrollbar now responds to:
- track clicks
- indicator dragging
- indicator hover state

The implementation intentionally reuses Gio's proven scrollbar event handling instead of introducing custom pointer math from scratch.

### 3. `ScrollView` now couples list position to scrollbar state
`ScrollView` now uses a Gio widget list state and derives a normalized viewport range from the current list position.

That range is used to:
- update `VerticalBar` / `HorizontalBar`
- show or hide the active scrollbar according to policy
- apply returned scroll delta back into the list state via `ScrollBy(...)`

### 4. `DemoSurface` now exercises the feature
`DemoSurface` now includes:
- a fixed-height scrolling region
- 40 demo rows
- a live status label reporting normalized scrollbar position, page size, and drag state

This keeps the runtime behavior visible inside the compile-safe framework demo surface.

## Design notes
- The implementation is still intentionally lightweight.
- It targets verified interaction behavior first, not perfect visual parity.
- The current `ScrollView` focuses on the primary list axis rather than full two-axis content virtualization.
- Overlay-style scrollbar rendering was chosen to keep the integration small and compile-safe.

## Why this approach
The BTK comparison reinforced that BobUI is most valuable when it produces compact, verified abstractions rather than broad speculative claims. Reusing Gio's internal gesture handling while keeping the BobUI widget API small aligns with that philosophy.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- richer visual polish and occupancy policies
- touch/swipe-specific primitives
- stronger runtime loop integration for the full demo surface
- broader parity work around momentum, overscroll, and richer scroll policies

## Recommended next step
Upgrade the Go `WebView` bridge from compile-safe contract behavior toward executable runtime semantics, or add touch/swipe primitives if the interaction foundation should be broadened first.
