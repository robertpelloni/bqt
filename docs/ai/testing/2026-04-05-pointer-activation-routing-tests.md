# Pointer Activation Routing Tests

## Date
2026-04-05

## Scope
This note documents automated validation for pointer-driven activation routing in the managed-window path.

Affected files:
- `internal/ui/widgets/window_test.go`
- `internal/ui/wm_test.go`

## Test coverage added
### 1. Window-level activation requests
Verified that:
- programmatic activation requests survive a layout pass
- activation requests can be consumed exactly once

### 2. Manager-level activation routing
Verified that:
- a window activation request routed through `WindowManager.Layout(...)` changes the active window
- the routed activation still uses manager-owned ordering and active-state semantics
- active-window fallback remains coherent after subsequent tab changes and closes

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
The managed-window runtime path now has a real bridge between direct window interaction and manager-owned activation state. This is a stronger and more realistic baseline than activation by explicit manager API alone.
