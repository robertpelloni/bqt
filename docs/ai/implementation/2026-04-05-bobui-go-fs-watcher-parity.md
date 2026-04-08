# BobUI Go FileSystemWatcher Parity and C++ Concurrent Rename

## Date
2026-04-05

## Scope
This note documents the improvement of the Go `Watcher` service to match the feature-set of Qt's `QFileSystemWatcher`, and the targeted internal rename of the C++ `Concurrent` module.

Affected files:
- `internal/kernel/fs_watcher.go`
- `internal/kernel/fs_watcher_test.go`
- `src/concurrent/`
- `src/corelib/global/bobuimacros.h`

## Goal
Achieve feature parity for the filesystem monitoring service in Go, and continue the targeted C++ source renames to solidify the BobUI identity.

## What changed

### 1. Go `FileSystemWatcher` Parity
Added the `Watcher` service mirroring `QFileSystemWatcher`:
- Tracks files and directories using thread-safe maps.
- Exposes `AddPath` and `RemovePath` methods.
- Provides an asynchronous `Start` method and `OnChanged` channel for event routing.

### 2. C++ `Concurrent` Rename
Performed a bulk rename across `src/concurrent/`:
- `QtConcurrent` -> `BobUIConcurrent`
- `QT_BEGIN_NAMESPACE` -> `BOBUI_BEGIN_NAMESPACE`
- `qtconcurrent_global.h` renamed to `bobuiconcurrent_global.h`

### 3. Core Class Aliases
Added the first set of core class aliases to `bobuimacros.h`:
- `BobUI::Object` -> `::QObject`
- `BobUI::String` -> `::QString`
- `BobUI::Variant` -> `::QVariant`

## Why this matters
The `Watcher` service is a key system integration point. By providing an idiomatic Go implementation that matches the Qt capability, we make the Go port a more complete replacement for backend logic.

The `Concurrent` rename continues to prove that "Bucket D" internal renames are safe when scoped carefully.

The class aliases are a significant milestone. They allow developers to write `BobUI::Object` and `BobUI::String`, creating a genuine BobUI development experience while remaining 100% ABI compatible with the underlying Qt types.

## Verified
```bash
cmake -P cmake/tests/bobui_full_compatibility_validation.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Recommended next step
Expand the parity audit to `QSettings` in Go. In C++, consider expanding the core class aliases and exploring the `Network` or `Sql` module renames.
