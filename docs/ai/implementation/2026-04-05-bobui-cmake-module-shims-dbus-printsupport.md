# BobUI CMake Module Shims for DBus and PrintSupport

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the OpenGL/OpenGLWidgets work.

Affected files:
- `cmake/BobUI6DBusConfig.cmake`
- `cmake/BobUIDBusConfig.cmake`
- `cmake/BobUI6PrintSupportConfig.cmake`
- `cmake/BobUIPrintSupportConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Continue expanding additive BobUI package identity into high-value public modules while keeping the migration compatibility-first and verifiable.

## What changed

### 1. Added direct DBus module shims
The repository now includes:
- `BobUI6DBus`
- `BobUIDBus`

These forward to:
- `Qt6DBus`

and expose:
- `BobUI6::DBus`
- `BobUI::DBus`

### 2. Added direct PrintSupport module shims
The repository now includes:
- `BobUI6PrintSupport`
- `BobUIPrintSupport`

These forward to:
- `Qt6PrintSupport`

and expose:
- `BobUI6::PrintSupport`
- `BobUI::PrintSupport`

### 3. Extended smoke tests
The forwarding smoke test now validates direct package lookup for:
- `BobUI6DBus`
- `BobUIDBus`
- `BobUI6PrintSupport`
- `BobUIPrintSupport`

The publication configure smoke test now validates that the publication helper can emit:
- DBus module-package compat files
- PrintSupport module-package compat files

in both build-style and install-style package directories.

## Why this matters
DBus and PrintSupport are meaningful public module surfaces in the native framework. Extending BobUI naming here improves downstream package compatibility in additional real-world areas without forcing unsafe tree-wide rename work.

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
