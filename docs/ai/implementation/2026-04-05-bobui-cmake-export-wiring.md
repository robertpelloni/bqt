# BobUI CMake Export Wiring

## Date
2026-04-05

## Scope
This note documents the next additive C++ compatibility-bucket step after the initial BobUI CMake shim files were introduced.

Affected files:
- `cmake/QtBaseGlobalTargets.cmake`
- `cmake/BobUICompatibilityHelpers.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`

## Goal
Move the BobUI CMake compatibility layer from a source-tree-only shim toward a real generated package/export surface, while preserving the canonical Qt-derived package layout.

## What changed

### 1. Hooked BobUI package publication into `QtBaseGlobalTargets.cmake`
The global package-generation path now additively publishes:
- `BobUI6/BobUI6Config.cmake`
- `BobUI6/BobUI6ConfigVersion.cmake`
- `BobUI6/BobUICompatibilityHelpers.cmake`
- `BobUI/BobUIConfig.cmake`
- `BobUI/BobUIConfigVersion.cmake`
- `BobUI/BobUICompatibilityHelpers.cmake`

These are emitted alongside the canonical `Qt6/` package directory instead of replacing it.

### 2. Kept Qt package generation authoritative
The new wiring does **not** rename or replace:
- `Qt6Config.cmake`
- `Qt6Targets.cmake`
- module/package generation templates
- internal Qt helper function names

Instead, the BobUI package directories forward to the canonical Qt package surface.

### 3. Hardened argument forwarding in `BobUICompatibilityHelpers.cmake`
A smoke test exposed that undefined `find_package(...)` boolean variables could produce invalid CMake `if(...)` expressions.

The helper now uses nested `if(DEFINED ...)` checks before evaluating boolean forwarding flags such as:
- `*_FIND_VERSION_EXACT`
- `*_FIND_QUIETLY`
- `*_FIND_REQUIRED`

### 4. Added an end-to-end forwarding smoke test
`cmake/tests/bobui_package_forwarding_smoke.cmake` now:
- creates a temporary fake prefix,
- writes a minimal fake `Qt6` / `Qt6Core` / `Qt6Widgets` package layout,
- copies the BobUI shims into `BobUI6/` and `BobUI/` package directories,
- configures a tiny CMake project using:
  - `find_package(BobUI6 REQUIRED COMPONENTS Core Widgets)`
  - `find_package(BobUI REQUIRED COMPONENTS Core Widgets)`
- asserts that:
  - `BobUI6::Core`
  - `BobUI6::Widgets`
  - `BobUI::Core`
  - `BobUI::Widgets`
  exist.

## Why this approach is safe
This remains compatibility-first migration:
- canonical Qt package/export generation stays intact,
- BobUI naming is additive,
- no deep runtime/source rename was attempted,
- no giant untracked rename tree was staged.

## Why this matters
This is the first step where the BobUI package story is no longer just conceptual or source-local. The Qt base package-generation path now has an explicit hook for publishing BobUI-facing package directories.

That is a materially stronger foundation for eventual side-by-side C++ + Go buildability than broad text replacement would be.

## Verified
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- full native Qt/C++ build verification of the modified `QtBaseGlobalTargets.cmake` path has not been completed in this session,
- module-specific BobUI package/export generation has not been added,
- deeper identifier renaming is still intentionally deferred.

## Recommended next step
Use the same additive approach for the next smallest package/export surface that benefits from BobUI identity, but prefer more end-to-end prefix/install validation before broadening the migration scope.
