# WebView Runtime Automated Tests

## Date
2026-04-05

## Scope
This note documents the first automated unit tests for the Go `WebView` bridge runtime.

Affected file:
- `internal/ui/widgets/webview_test.go`

## Goal
Move WebView runtime validation beyond compile/build success by asserting real bridge behavior.

## Test coverage added
### 1. Auto-dispatch request/reply correlation
Verified that:
- `Request(...)` auto-dispatches by default
- replies preserve the original request ID
- queue depth returns to zero after processing
- emitted script messages include both request and reply phases

### 2. Manual queue pumping
Verified that:
- disabling auto-dispatch leaves requests queued
- `PumpRuntime()` processes the queued request
- the resulting reply preserves the queued request ID

### 3. Eval handler execution
Verified that:
- `EvalJS(...)` records the source string
- eval handling produces a real reply
- `LastEvalResult` and `LastReply` are updated correctly

### 4. Error propagation
Verified that:
- missing handlers produce `error`
- handler failures propagate their error message
- missing eval handlers produce `error`

### 5. External message handling
Verified that:
- `HandleScriptMessage(...)` can route external request messages through local handlers
- external reply messages without an ID are assigned one
- `LastReply` is updated from externally supplied replies

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
This test pass confirms that the Go `WebView` runtime is no longer only a compile-safe abstraction. The bridge now has verifiable behavior around correlation, queueing, eval handling, and error routing.

## Remaining gaps
- direct tests for history/navigation callbacks
- tests around `BridgeContractJSON()` structure
- automated live-loop/runtime integration tests
