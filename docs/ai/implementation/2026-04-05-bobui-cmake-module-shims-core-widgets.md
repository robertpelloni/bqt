# BobUI CMake Module Shims for Core and Widgets

## Date
2026-04-05

## Scope
This note documents the first direct BobUI module-package compatibility slice on top of the existing additive BobUI top-level package work.

Affected files:
- `cmake/BobUI6CoreConfig.cmake`
- `cmake/BobUICoreConfig.cmake`
- `cmake/BobUI6WidgetsConfig.cmake`
- `cmake/BobUIWidgetsConfig.cmake`
- `cmake/QtBobUIHelpers.cmake`
- `cmake/QtModuleHelpers.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Extend the BobUI compatibility-first migration from top-level package discovery into a small verified subset of direct module-package discovery, without broad source renaming.

## What changed

### 1. Added direct BobUI module package shims
The repository now contains additive BobUI module config shims for:
- `BobUI6Core`
- `BobUICore`
- `BobUI6Widgets`
- `BobUIWidgets`

These shims forward to the canonical Qt module packages:
- `Qt6Core`
- `Qt6Widgets`

They also create BobUI-namespaced compatibility targets such as:
- `BobUI6::Core`
- `BobUI::Core`
- `BobUI6::Widgets`
- `BobUI::Widgets`

### 2. Generalized the publication helper safely
`QtBobUIHelpers.cmake` now treats missing BobUI config sources as an intentional no-op. That makes it safe to call the helper from the module-package generation path without forcing BobUI coverage for every module immediately.

### 3. Hooked module-package publication into `QtModuleHelpers.cmake`
`qt_internal_write_basic_module_package(...)` now:
- includes the BobUI publication helper,
- tries to publish BobUI module compatibility packages for public modules,
- only tracks configure dependencies for BobUI module config files that actually exist.

This keeps the migration additive and opt-in.

### 4. Strengthened tests
The forwarding smoke test now validates direct module package lookups for Core and Widgets, not just top-level component routing.

The publication configure smoke test now validates that the publication helper can emit module-package compat files for the same slice.

## Why this matters
This is the first step where BobUI compatibility extends beyond the umbrella package into direct module-package naming.

That is useful because many native consumers use direct package lookup patterns like:
- `find_package(Qt6Core ...)`
- `find_package(Qt6Widgets ...)`

The BobUI migration path can now start to mirror that style incrementally.

## Verified
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- only a small module subset (`Core`, `Widgets`) has BobUI direct-package shims so far,
- deeper native qtbase end-to-end validation is still separate work,
- source-identifier renaming is still intentionally deferred.

## Recommended next step
Continue module-package expansion only in small verified slices, prioritizing high-value public modules and preserving the additive compatibility-first model.
