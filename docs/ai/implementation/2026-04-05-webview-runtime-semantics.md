# WebView Runtime Semantics

## Date
2026-04-05

## Scope
This note documents the upgrade of the Go `WebView` bridge from a contract-only surface into a more executable runtime model.

Affected files:
- `internal/ui/widgets/webview.go`
- `internal/ui/engine.go`

## Goal
Preserve the lightweight, compile-safe BobUI `WebView` surface while making bridge operations behave more like a runtime instead of only a static API contract.

## What changed
### 1. `JSBridge` now supports queued runtime processing
The bridge now maintains:
- queued messages
- message history
- optional auto-dispatch behavior
- executable dispatch for `emit`, `eval`, `request`, `reply`, and `error`

This means host/page messages can now be:
- queued
- pumped through local handlers
- correlated by message ID
- observed as both outbound requests and generated replies/errors

### 2. Eval handling is now executable
The bridge now supports a dedicated eval handler via:
- `RegisterEvalHandler(...)`

`EvalJS(...)` no longer only records the source string. It now routes through the bridge runtime and can produce a real reply payload.

### 3. Request routing now preserves request IDs end-to-end
`Request(...)` now:
- queues a request message
- pumps the bridge runtime when auto-dispatch is enabled
- returns the correlated reply/error using the same message ID

### 4. Explicit runtime entry points were added
The WebView now exposes:
- `SetAutoDispatch(...)`
- `PumpRuntime()`
- `QueueDepth()`
- `HandleScriptMessage(...)`
- `RegisterEvalHandler(...)`

This creates a small but real execution model that can later evolve toward async/browser-backed semantics.

### 5. Runtime state is now surfaced in layout
The `WebView` layout now displays a runtime summary including:
- queue depth
- auto-dispatch state
- last reply kind/channel
- last eval result

### 6. Engine demo now exercises the runtime path
`internal/ui/engine.go` now:
- registers a normal request handler (`ping`)
- registers a title-mutating request handler (`title`)
- registers an eval handler
- executes `PostMessage(...)`
- executes `EvalJS(...)`
- executes `Request(...)`
- routes an external script message through `HandleScriptMessage(...)`

## Design notes
- This is still not a real browser runtime.
- The runtime is local and compile-safe.
- Auto-dispatch defaults to enabled so existing behavior stays straightforward.
- The queue/pump model creates a clean path toward future async semantics without forcing them prematurely.

## Why this approach
The current BobUI strategy is to keep abstractions small, verified, and behaviorally real. A queued local bridge runtime is a better foundation than either:
- a purely declarative contract with no execution
- or a speculative browser subsystem that cannot yet be verified

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- real asynchronous execution
- embedded browser backing
- permission/interception policy layers
- navigation/resource loading hooks beyond the lightweight baseline
- automated tests for reply correlation and queue pumping

## Recommended next step
Improve scrollbar occupancy/two-axis polish or wire the demo surface into a fuller interactive Gio frame loop so the verified primitives can be exercised more realistically.
