# Kernel / Data / Net Tests

## Date
2026-04-05

## Scope
This note documents the first focused automated tests for the Go kernel/data/net support layers.

Affected files:
- `internal/kernel/managers_test.go`
- `internal/data/data_test.go`
- `internal/net/mesh_node_test.go`

## Goal
Extend verification beyond UI/runtime surfaces into foundational support packages without introducing fragile external side effects.

## Test coverage added
### 1. Kernel manager behavior
Covered:
- `InputManager`
  - device registration
  - duplicate registration stability
  - cursor updates
  - snapshot retrieval
- `PermissionManager`
  - lock acquisition
  - lock ownership enforcement
  - unlock behavior
  - interaction checks
- `UserManager`
  - user creation
  - device assignment
  - device-to-user lookup

### 2. Data-layer behavior
Covered:
- `VFS`
  - mount
  - asset retrieval
  - missing asset lookup
- `Database`
  - in-memory SQLite connection
  - table creation
  - inserts
  - query execution through `ExecuteQuery(...)`
- lightweight mesh-linked calls in:
  - `HistoryReconciler`
  - `PackageRegistry`
  using empty-peer conditions to confirm safe execution

### 3. Net-layer behavior
Covered:
- `GetMeshNode()` singleton behavior
- `MeshNode.Broadcast(...)` safety with an empty peer set

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
This is the first time the Go kernel/data/net support layer has explicit automated coverage in the current verified baseline. The tests intentionally favor deterministic state/manager behavior and in-memory execution over fragile network integration.

## Remaining gaps
- deeper network protocol validation
- richer data-layer tests around package/pak/database workflows
- broader kernel coverage for undo, clipboard, merge logic, and related services
