# ScrollBar Occupancy and Two-Axis Polish

## Date
2026-04-05

## Scope
This note documents the next runtime polish step for the Go `ScrollView` baseline:
- overlay vs occupy scrollbar placement
- clearer two-axis scrollbar behavior

Affected files:
- `internal/ui/widgets/scrollview.go`
- `internal/ui/widgets/demo_surface.go`

## Goal
Improve the scroll baseline beyond a single overlay-style primary bar while remaining compile-safe and honest about current scope.

## What changed
### 1. Added scrollbar placement modes
`ScrollView` now supports:
- `ScrollBarOverlay`
- `ScrollBarOccupy`

These can be configured independently for vertical and horizontal bars.

### 2. Added occupied-space relayout logic
When a scrollbar is configured to occupy space, `ScrollView` now:
- performs an initial content layout pass
- determines bar visibility
- computes occupied width/height
- re-lays out content with reserved scrollbar space

This keeps the behavior small but much closer to a real controls-style scroll container.

### 3. Added lightweight two-axis behavior
The primary axis remains list-driven, but the secondary axis now maintains its own normalized viewport state.

`ScrollView` now supports:
- rendering both bars when configured
- maintaining cross-axis position/page size
- updating cross-axis position from scrollbar drag/click deltas

This is not full 2D virtualization, but it is a truthful step beyond a purely single-axis scroll baseline.

### 4. Demo integration
`DemoSurface` now exercises:
- occupied vertical scrollbar placement
- occupied horizontal scrollbar placement
- cross-axis viewport state
- live labels for main/cross positions and deltas

## Design notes
- The primary list axis remains the only axis truly backed by Gio list scrolling.
- The secondary axis is intentionally modeled as a normalized viewport state rather than pretending the content is already fully scrollable in two dimensions.
- This keeps the API useful and honest while creating a clean path toward richer two-axis containers later.

## Why this approach
The current BobUI strategy is to prefer verified, behaviorally real increments over speculative completeness. Adding occupancy policy and explicit secondary-axis state improves realism without claiming a full 2D scroll/view architecture.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- true two-dimensional content virtualization
- richer scrollbar theming and corner handling when both bars are visible
- touch momentum / overscroll behavior
- fuller runtime demo loop integration

## Recommended next step
Wire the demo surface into a fuller Gio runtime loop or add automated validation around WebView bridge/runtime behavior and scroll-state transitions.
