# Window Manager Runtime Tests

## Date
2026-04-05

## Scope
This note documents automated validation for the new Go window-manager runtime integration.

Affected files:
- `internal/ui/engine_test.go`
- `internal/ui/wm_test.go`

## Test coverage added
### 1. Engine initialization coverage
Verified that `initializeDemo()` now also:
- initializes the `WindowManager`
- seeds managed windows when empty
- preserves tab initialization

### 2. Window manager behavior
Verified that `WindowManager` now:
- resets deterministically
- spawns managed windows correctly
- returns full-frame dimensions from `Layout(...)`

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
Managed-window composition is now part of the verified Go runtime path rather than a disconnected helper structure.
