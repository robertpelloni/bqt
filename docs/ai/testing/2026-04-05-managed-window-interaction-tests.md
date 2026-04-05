# Managed Window Interaction Tests

## Date
2026-04-05

## Scope
This note documents automated validation for the richer managed-window interaction/content upgrade.

Affected files:
- `internal/ui/wm_test.go`
- `internal/ui/engine_test.go`

## Test coverage added
### 1. Tab-aware visibility
Verified that:
- `WindowManager.ActiveTab()` reports the active tab title
- `VisibleWindows()` filters tab-bound windows correctly
- shared windows remain visible across tabs

### 2. Programmatic tab switching
Verified that:
- `TabBar.ClickTab(...)` changes the active tab after layout processing
- visible managed windows update accordingly

### 3. Engine initialization
Verified that:
- runtime initialization now seeds tab-aware managed windows
- the initial visible window set matches the active tab

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
Managed-window composition is now not only present in the runtime path, but meaningfully interactive and test-covered.
