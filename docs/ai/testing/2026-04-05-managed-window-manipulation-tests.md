# Managed Window Manipulation Tests

## Date
2026-04-05

## Scope
This note documents automated validation for the managed-window move/resize/close baseline.

Affected files:
- `internal/ui/widgets/window_test.go`
- `internal/ui/wm_test.go`

## Test coverage added
### 1. Window-level behavior
Verified that:
- `MoveBy(...)` updates position
- `ResizeBy(...)` clamps to minimum size
- visible windows produce non-zero layout dimensions
- close-button clicks close the window
- closed windows layout to zero dimensions

### 2. Window-manager behavior
Verified that:
- `MoveWindow(...)` updates managed-window position
- `ResizeWindow(...)` updates managed-window size
- `CloseWindow(...)` hides the managed window from visible-window results
- missing-window manipulation calls fail cleanly

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
Managed-window interaction is now deeper than tab selection/content visibility alone. The Go runtime path now has a small but real manipulation baseline with deterministic coverage.
