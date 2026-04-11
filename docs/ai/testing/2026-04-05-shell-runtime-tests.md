# Shell Runtime Synthetic Tests

## Date
2026-04-05

## Scope
This note documents automated synthetic-frame tests for the Go shell/taskbar/start-menu baseline.

Affected file:
- `internal/ui/widgets/shell_test.go`

## Goal
Add honest runtime validation for shell interaction behavior without attempting fragile real-window automation.

## Test coverage added
### 1. Taskbar start-button interaction
Verified that:
- `LayoutTaskbar(...)` produces non-zero dimensions
- the start menu begins hidden
- a programmatic `StartBtn.Click()` toggles the start menu visible
- a second programmatic click toggles it hidden again

### 2. Start-menu visibility/layout behavior
Verified that:
- `LayoutStartMenu(...)` returns zero dimensions when hidden
- `LayoutStartMenu(...)` returns non-zero dimensions when visible
- the visible menu remains within the provided synthetic frame size

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
The live runtime path now has additional synthetic coverage around shell composition behavior, complementing the earlier engine and WebView tests. This increases confidence in the Go runtime path without over-claiming end-to-end UI automation.
