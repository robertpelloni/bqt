# Managed Window Activation Tests

## Date
2026-04-05

## Scope
This note documents automated validation for the managed-window activation baseline.

Affected files:
- `internal/ui/wm_test.go`
- `internal/ui/engine_test.go`

## Test coverage added
### 1. Initial active window selection
Verified that the first visible managed window becomes active after runtime/layout initialization.

### 2. Explicit activation
Verified that:
- `ActivateWindow(...)` succeeds for existing visible windows
- the active window changes
- activated windows are promoted in ordering

### 3. Active-window fallback behavior
Verified that:
- tab changes preserve active shared windows when still visible
- closing the active window falls back to another visible window
- missing-window activation fails cleanly

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
The managed-window runtime path now has a real, deterministic activation baseline instead of relying only on visibility and manipulation behavior.
