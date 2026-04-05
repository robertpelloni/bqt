# Window Manager Runtime Integration

## Date
2026-04-05

## Scope
This note documents the first meaningful integration of the existing Go `WindowManager` into the live Gio runtime path.

Affected files:
- `internal/ui/wm.go`
- `internal/ui/engine.go`
- `internal/ui/engine_test.go`
- `internal/ui/wm_test.go`

## Goal
Move the Go `WindowManager` from an isolated utility into the actual runtime composition path so managed windows and tabs become part of the live BobUI surface.

## What changed
### 1. `Engine` now owns a `WindowManager`
`NewEngine()` now captures the shared `WindowManager` and the engine initializes it as part of runtime setup.

### 2. Runtime initialization now seeds managed windows
The engine now creates a small initial managed-window set when the manager is empty, including:
- a kernel-inspector window
- a runtime-log window

This keeps the integration visible without pretending the full docking/windowing system is complete.

### 3. `WindowManager` now supports reset and real layout return values
`WindowManager` now:
- exposes `Reset()` for deterministic test setup
- returns a real `layout.Dimensions` from `Layout(...)`
- paints tabs and managed windows as part of a full-frame composition area

### 4. Live runtime composition now overlays managed windows on the demo surface
Inside the authenticated runtime path, the engine now stacks:
- the existing `DemoSurface`
- the `WindowManager` overlay

This makes the managed windows part of the actual runtime, not just unused code.

## Why this approach
This is the smallest meaningful composition step:
- it exercises existing window-manager code
- it remains compile-safe and testable
- it avoids inventing a speculative full docking/runtime architecture too early

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- richer managed-window content
- deeper docking integration
- active tab behavior and interaction
- richer tests around managed-window placement and overlap semantics
