# Kernel Services Tests

## Date
2026-04-05

## Scope
This note documents additional focused automated tests for Go kernel services.

Affected file:
- `internal/kernel/services_test.go`

## Goal
Extend kernel verification beyond state-manager basics into additional deterministic services with meaningful framework value.

## Test coverage added
### 1. Undo stack behavior
Covered:
- push behavior
- user-specific undo behavior
- redo-pool population after undo
- no-op undo for unknown users
- history cap behavior at 500 actions

### 2. Clipboard behavior
Covered:
- local text update
- binary payload update/retrieval
- remote overwrite behavior for text and binary payloads
- preservation of existing clipboard state when remote inputs are empty/nil

### 3. Merge kernel behavior
Covered:
- concurrent edit append behavior
- preservation of edit order in merge history

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
The Go kernel layer now has broader deterministic coverage across:
- input/user/permission managers
- undo service
- clipboard service
- merge history service

This strengthens the non-UI foundation of the verified Go baseline.

## Remaining gaps
- undo redo execution APIs beyond pool population
- richer clipboard semantics and multi-format behavior
- deeper merge/conflict-resolution logic
