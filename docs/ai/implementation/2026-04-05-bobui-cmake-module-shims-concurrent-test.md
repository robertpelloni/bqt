# BobUI CMake Module Shims for Concurrent and Test

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the DBus/PrintSupport work.

Affected files:
- `cmake/BobUI6ConcurrentConfig.cmake`
- `cmake/BobUIConcurrentConfig.cmake`
- `cmake/BobUI6TestConfig.cmake`
- `cmake/BobUITestConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Continue expanding additive BobUI package identity into high-value public modules while preserving the same compatibility-first, test-backed migration pattern.

## What changed

### 1. Added direct Concurrent module shims
The repository now includes:
- `BobUI6Concurrent`
- `BobUIConcurrent`

These forward to:
- `Qt6Concurrent`

and expose:
- `BobUI6::Concurrent`
- `BobUI::Concurrent`

### 2. Added direct Test module shims
The repository now includes:
- `BobUI6Test`
- `BobUITest`

These forward to:
- `Qt6Test`

and expose:
- `BobUI6::Test`
- `BobUI::Test`

### 3. Extended smoke tests
The forwarding smoke test now validates direct package lookup for:
- `BobUI6Concurrent`
- `BobUIConcurrent`
- `BobUI6Test`
- `BobUITest`

The publication configure smoke test now validates that the publication helper can emit:
- Concurrent module-package compat files
- Test module-package compat files

in both build-style and install-style package directories.

## Why this matters
Concurrent and Test are widely used public framework packages. Extending BobUI naming here improves downstream package compatibility in workflow and testing-oriented areas without requiring unsafe broad rename work.

## Verified
```bash
cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- direct BobUI module-package coverage remains selective,
- native qtbase configure is still environment-gated by compiler visibility,
- source-identifier renaming is still intentionally deferred.

## Recommended next step
Continue extending only high-value public module packages in small validated slices, or re-run the qtbase-native configure preflight in a compiler-capable environment when available.
