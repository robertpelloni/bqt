# BobUI Go Process Parity with QProcess

## Date
2026-04-05

## Scope
This note documents the improvement of the Go `Process` service to match the feature-set of Qt's `QProcess`.

Affected files:
- `internal/kernel/process.go`
- `internal/kernel/process_test.go`
- `internal/ui/widgets/terminal.go`

## Goal
Achieve high feature parity for the process management service, supporting working directories, environment variables, and explicit state management.

## What changed

### 1. Generic `Process` Struct
Refactored `ShellProcess` into a generic `Process` struct:
```go
type Process struct {
    Program          string
    Arguments        []string
    WorkingDirectory string
    Environment      []string
    // ... internals
}
```

### 2. State Management
Added explicit process states matching the `QProcess::ProcessState` model:
- `NotRunning`
- `Starting`
- `Running`

### 3. Channels for I/O and Lifecycle
Replaced simple output pipes with structured channels:
- `Output chan string` (Stdout)
- `Error chan string` (Stderr)
- `Done chan error` (Exit signal)

### 4. Control Methods
Added methods mirroring `QProcess`:
- `Start()`
- `Kill()`
- `Terminate()`
- `State()`

## Why this matters
The `Process` service is now a capable framework primitive that can be used for building terminals, build tools, and other system-integrating features in Go with the same level of control provided by Qt.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- standard input handling (currently only `Write` method),
- more granular error reporting (matching `QProcess::ProcessError`),
- system-specific process attributes.

## Recommended next step
Expand the parity audit to `QFileSystemWatcher` or `QSettings`.
