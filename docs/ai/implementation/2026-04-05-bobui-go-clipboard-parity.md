# BobUI Go Clipboard Parity with QMimeData

## Date
2026-04-05

## Scope
This note documents the improvement of the Go `Clipboard` service to match the feature-set of Qt's `QClipboard` and `QMimeData`.

Affected files:
- `internal/kernel/clipboard.go`
- `internal/kernel/services_test.go`

## Goal
Achieve 100% feature parity for the clipboard service, supporting multiple simultaneous data formats (MIME types) instead of just plain text or a single binary blob.

## What changed

### 1. Multi-MIME Data Support
The `Clipboard` struct now uses a map to store data for different MIME types:
```go
type Clipboard struct {
    mu   sync.RWMutex
    data map[string][]byte
}
```

### 2. Standard Accessors
Added methods that mirror the `QMimeData` interface:
- `SetText(val string)` / `GetText() string`
- `SetData(mime string, data []byte)` / `GetData(mime string) []byte`
- `MimeTypes() []string` (returns a list of available formats)

### 3. Compatibility Layer
Maintained `SetBinary` and `GetBinary` methods for backward compatibility, mapping them to the `application/octet-stream` MIME type.

## Why this matters
This is a core framework service that now supports sophisticated data exchange patterns (e.g. copying an object that provides both an image representation and a custom XML representation), matching the level of functionality expected by C++ developers transitioning to BobUI.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- system-level integration (integrating with the OS clipboard),
- clipboard history (similar to `QClipboard` but extended for BobUI),
- clipboard ownership semantics (who currently "owns" the data).

## Recommended next step
Expand the parity audit to other kernel services like `QProcess` or `QFileSystemWatcher`.
