# BobUI CMake Compatibility Shims

## Date
2026-04-05

## Scope
This note documents the first actual C++ compatibility-bucket implementation for BobUI naming work.

Affected files:
- `cmake/BobUICompatibilityHelpers.cmake`
- `cmake/BobUI6Config.cmake`
- `cmake/BobUIConfig.cmake`
- `cmake/tests/bobui_compatibility_helpers_mapping.cmake`

## Goal
Introduce a **small additive BobUI-facing package/export surface** in `cmake/` without rewriting the canonical Qt-derived build internals and without adopting the large set of untracked renamed CMake duplicates as source of truth.

## What changed

### 1. Added `BobUICompatibilityHelpers.cmake`
This file provides pure helper functions for:
- mapping BobUI package names to canonical Qt package names,
- forwarding `find_package(...)` arguments while preserving version/quiet/required/component semantics,
- creating BobUI-namespaced compatibility targets that point at existing Qt targets.

Key idea:
- treat BobUI naming as an additive compatibility layer,
- keep canonical Qt package/export logic authoritative underneath.

### 2. Added `BobUI6Config.cmake`
This is a small forwarding package shim.

Behavior:
- `find_package(BobUI6 ...)` forwards to `find_package(Qt6 ...)`,
- if Qt6 is found, the shim marks `BobUI6_FOUND` and mirrors version variables,
- for requested components, it creates `BobUI6::Component` compatibility targets that link to the existing Qt targets.

### 3. Added `BobUIConfig.cmake`
This is a convenience wrapper for versionless BobUI lookup.

Behavior:
- `find_package(BobUI ...)` forwards to `find_package(BobUI6 ...)`,
- if found, it mirrors version variables and creates `BobUI::Component` compatibility targets where possible.

### 4. Added a script-mode mapping test
`cmake/tests/bobui_compatibility_helpers_mapping.cmake` validates the pure helper layer without requiring a full native build.

It verifies:
- BobUI package-name mapping,
- preservation of forwarded `find_package(...)` arguments,
- that the compatibility helper logic is syntactically valid and runnable in CMake script mode.

## Why this approach is safe
This implementation deliberately avoids:
- deep runtime/source identifier rewrites,
- adopting untracked renamed `BobUI*` CMake duplicates as canonical,
- test-asset churn,
- docs/qdoc churn,
- third-party subtree changes.

Instead, it gives the C++ path a real BobUI-facing discovery layer while preserving the existing Qt-derived package surface under it.

## Current limits
This is intentionally a **first slice**, not a completed migration.

Current limits include:
- the shims are source-tree compatibility files and are not yet wired into install/export generation,
- they do not rename internal Qt helper functions,
- they do not rewrite module-generation templates,
- they do not yet provide a full installed BobUI package story.

## Why this still matters
It establishes the correct migration pattern:
- additive compatibility first,
- canonical tracked Qt files remain the authority,
- BobUI identity is layered on top in small verified pieces.

That is exactly the pattern needed to eventually support clean side-by-side C++ and Go compilation.

## Verified
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Recommended next step
Compare the top-level tracked package/export generation inputs (especially around `QtBaseGlobalTargets.cmake`, `QtConfig.cmake.in`, and related config-generation paths) and decide how to wire the new BobUI compatibility shims into install/export generation **without** replacing the canonical Qt package layout in one shot.
