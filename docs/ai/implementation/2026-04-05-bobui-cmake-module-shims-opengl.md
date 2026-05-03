# BobUI CMake Module Shims for OpenGL and OpenGLWidgets

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the Sql/Xml work.

Affected files:
- `cmake/BobUI6OpenGLConfig.cmake`
- `cmake/BobUIOpenGLConfig.cmake`
- `cmake/BobUI6OpenGLWidgetsConfig.cmake`
- `cmake/BobUIOpenGLWidgetsConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Continue expanding additive BobUI package identity through high-value public module slices that are easy to validate and do not require broad source renaming.

## What changed

### 1. Added direct OpenGL module shims
The repository now includes:
- `BobUI6OpenGL`
- `BobUIOpenGL`

These forward to:
- `Qt6OpenGL`

and expose:
- `BobUI6::OpenGL`
- `BobUI::OpenGL`

### 2. Added direct OpenGLWidgets module shims
The repository now includes:
- `BobUI6OpenGLWidgets`
- `BobUIOpenGLWidgets`

These forward to:
- `Qt6OpenGLWidgets`

and expose:
- `BobUI6::OpenGLWidgets`
- `BobUI::OpenGLWidgets`

### 3. Extended smoke tests
The forwarding smoke test now validates direct package lookup for:
- `BobUI6OpenGL`
- `BobUIOpenGL`
- `BobUI6OpenGLWidgets`
- `BobUIOpenGLWidgets`

The publication configure smoke test now validates that the publication helper can emit:
- OpenGL module-package compat files
- OpenGLWidgets module-package compat files

in both build-style and install-style package directories.

## Why this matters
OpenGL and OpenGLWidgets are useful consumer-facing package surfaces in the native framework ecosystem. Extending BobUI naming here increases real downstream compatibility depth without pretending the entire native C++ tree has already been renamed.

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
Keep extending only high-value public module packages in small validated slices, or re-run the qtbase-native configure preflight in a compiler-capable environment when available.
