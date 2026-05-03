# Managed Window Focus and Activation Baseline

## Date
2026-04-05

## Scope
This note documents the next managed-window runtime step after move/resize/close behavior: explicit active-window semantics.

Affected files:
- `internal/ui/wm.go`
- `internal/ui/widgets/window.go`
- `internal/ui/wm_test.go`
- `internal/ui/engine.go`

## Goal
Add a small but real activation model so managed windows can:
- track which window is active
- expose active-window queries
- update active state when tabs change or windows close
- promote activated windows in z-order

## What changed
### 1. `WindowManager` now tracks an active window
The manager now includes:
- `ActiveWindowID`
- `ActiveWindow()`
- `ActivateWindow(...)`

### 2. Active state now synchronizes into windows
Managed windows now expose `Active` state and the manager synchronizes that state onto window instances.

### 3. Active-window fallback is handled automatically
When the active window is hidden by a tab change or closed, the manager now picks the first visible window as the new active window.

### 4. Activation promotes windows in ordering
`ActivateWindow(...)` moves the activated window to the end of the manager slice, giving the runtime a simple front-order rule.

### 5. Window rendering now reflects active state
Managed windows now render their title bar differently when active, making activation visible in the runtime path.

## Why this approach
This is the smallest meaningful activation baseline:
- deterministic
- testable
- visible
- useful for future composition work

It avoids pretending to have a full focus manager or workspace stack while still making window activation real.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- pointer-driven activation from clicking windows directly into manager state
- richer z-order rules
- keyboard focus routing
- activation interactions with docking/workspaces
