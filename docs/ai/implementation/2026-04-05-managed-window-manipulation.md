# Managed Window Move / Resize / Close Baseline

## Date
2026-04-05

## Scope
This note documents the next managed-window runtime step: direct manipulation behavior.

Affected files:
- `internal/ui/widgets/window.go`
- `internal/ui/wm.go`
- `internal/ui/wm_test.go`
- `internal/ui/widgets/window_test.go`

## Goal
Move managed windows beyond static content + tab visibility by adding:
- close behavior
- move behavior
- resize behavior

## What changed
### 1. `Window` now supports direct manipulation state
The Go window baseline now includes:
- minimum size handling
- closed state
- move and resize helpers
- titlebar drag handling
- resize-handle drag handling
- close button support

### 2. `WindowManager` now exposes manipulation helpers
The manager now supports:
- `MoveWindow(...)`
- `ResizeWindow(...)`
- `CloseWindow(...)`

This gives runtime code a stable manipulation API even before a deeper docking/windowing architecture exists.

### 3. Visibility now respects closed state
Closed managed windows are no longer returned from `VisibleWindows()`.

## Why this approach
This is the smallest meaningful interaction upgrade after making tabs and visibility real:
- windows can now be manipulated
- the API remains deterministic and testable
- the implementation avoids claiming full workspace/docking behavior prematurely

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- z-order management
- focus/activation semantics for managed windows
- richer resize handles and edge interactions
- explicit reopen/restore semantics
- deeper docking/workspace integration
