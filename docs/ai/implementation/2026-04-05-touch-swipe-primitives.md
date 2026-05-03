# Touch / Swipe Primitives

## Date
2026-04-05

## Scope
This note documents the addition of a lightweight touch/swipe primitive to the verified Go `bobui` baseline.

Affected files:
- `internal/ui/widgets/touch.go`
- `internal/ui/widgets/demo_surface.go`

## Goal
Add a compact, framework-owned touch/swipe primitive that:
- recognizes directional swipe gestures
- remains compile-safe
- is visible in the demo surface
- does not over-claim a full gesture framework

## What changed
### 1. Added `TouchArea`
A new `TouchArea` widget now provides:
- touch-oriented drag tracking
- directional swipe recognition
- configurable minimum swipe distance
- optional mouse-as-touch fallback for non-touch desktop development
- simple callback hooks:
  - `OnSwipe`
  - `OnTouch`

### 2. Added `SwipeDirection`
The touch primitive exposes a small directional vocabulary:
- `SwipeNone`
- `SwipeLeft`
- `SwipeRight`
- `SwipeUp`
- `SwipeDown`

This keeps the baseline easy to reason about and immediately useful for higher-level gesture work.

### 3. Demo integration
`DemoSurface` now includes a visible swipe demo region and reports the most recent recognized swipe.

This gives the Go baseline a concrete interaction surface beyond buttons and scrollbars.

## Design notes
- The implementation intentionally uses Gio's `gesture.Drag` internally.
- Mouse fallback is enabled in the demo so the gesture can be explored on non-touch hardware without claiming that mouse drag and touch are identical.
- The primitive is intentionally small: it detects swipes and touch-like activity but does not yet attempt multi-touch, pinch, rotation, inertia, or gesture composition.

## Why this approach
The current BobUI strategy is to prefer verified, behaviorally meaningful primitives over speculative breadth. A minimal `TouchArea` keeps the framework honest while creating a clean landing zone for future touch APIs.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- true multi-touch support
- pinch/zoom and rotation primitives
- gesture arbitration with other interactive widgets
- haptic/runtime integration
- automated interaction tests beyond compile/build verification

## Recommended next step
Upgrade the Go `WebView` bridge from compile-safe contract behavior toward executable runtime semantics, or deepen touch support with multi-touch primitives if input work remains the priority.
