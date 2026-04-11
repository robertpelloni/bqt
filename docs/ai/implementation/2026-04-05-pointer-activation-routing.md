# Pointer-Driven Managed Window Activation Routing

## Date
2026-04-05

## Scope
This note documents the next step in the managed-window activation model: routing direct window click intent into the `WindowManager`.

Affected files:
- `internal/ui/widgets/window.go`
- `internal/ui/wm.go`
- `internal/ui/widgets/window_test.go`
- `internal/ui/wm_test.go`

## Goal
Allow direct pointer-style interaction with managed windows to request activation so the manager, not just tests or explicit APIs, can drive active-window transitions.

## What changed
### 1. `Window` now exposes an activation request surface
The managed-window baseline now has:
- a click gesture covering the window area
- an internal activation-request flag
- `RequestActivate()` for deterministic test support
- `ConsumeActivationRequest()` for manager-side routing

### 2. `WindowManager.Layout(...)` now routes activation requests
During layout, the manager now:
- renders visible windows
- checks whether any window requested activation
- promotes/activates the requested window through manager-owned activation logic

This keeps activation state authoritative in the manager instead of letting individual windows mutate global runtime state directly.

## Why this approach
This is the smallest meaningful pointer-driven activation step:
- real direct window interaction surface
- deterministic tests remain possible
- manager stays responsible for active-window state and ordering

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- true OS-pointer end-to-end interaction tests
- richer hit testing between activation, move, resize, and close regions
- keyboard focus routing on top of activation state
