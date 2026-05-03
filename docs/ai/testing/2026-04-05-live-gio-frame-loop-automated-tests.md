# Live Gio Frame Loop Automated Tests

## Date
2026-04-05

## Scope
This note documents the first automated tests covering the live Gio runtime path in `internal/ui/engine.go`.

Affected file:
- `internal/ui/engine_test.go`

## Goal
Add automated validation for the live runtime composition without pretending unit tests should open real OS windows in CI.

## Test coverage added
### 1. Demo/runtime initialization
Verified that `initializeDemo()`:
- initializes the WebView bridge
- registers runtime handlers
- executes initial eval behavior
- leaves the queue drained after startup work
- supports follow-up requests through the initialized runtime path

### 2. Layout path coverage
Verified that `layout(...)` can render both major engine states using synthetic frame contexts:
- login state
- authenticated runtime state

The tests assert that both states produce non-zero dimensions and that the authenticated runtime surface fits within the provided frame size.

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Important limitation
These tests validate the live runtime composition logic using synthetic frame contexts. They do **not** open a real OS window. That limitation is intentional and keeps the tests deterministic and CI-safe.

## Findings
This adds meaningful automated coverage to the live runtime path without over-claiming full end-to-end UI automation. It is an honest and useful middle ground between pure compile checks and fragile windowed integration tests.
