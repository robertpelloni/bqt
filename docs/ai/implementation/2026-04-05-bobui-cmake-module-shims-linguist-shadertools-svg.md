# BobUI CMake Module Shims for Linguist, ShaderTools, and Svg

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the DBus/PrintSupport work, along with a significant refactor of the smoke test machinery to use the centralized manifest.

Affected files:
- `cmake/BobUI6LinguistConfig.cmake`
- `cmake/BobUILinguistConfig.cmake`
- `cmake/BobUI6ShaderToolsConfig.cmake`
- `cmake/BobUIShaderToolsConfig.cmake`
- `cmake/BobUI6SvgConfig.cmake`
- `cmake/BobUISvgConfig.cmake`
- `cmake/BobUISupportedComponents.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Continue expanding additive BobUI package identity into high-value public modules while improving the maintenance surface of the compatibility tests.

## What changed

### 1. Added direct Linguist module shims
The repository now includes:
- `BobUI6Linguist`
- `BobUILinguist`

These forward to:
- `Qt6Linguist`

### 2. Added direct ShaderTools module shims
The repository now includes:
- `BobUI6ShaderTools`
- `BobUIShaderTools`

These forward to:
- `Qt6ShaderTools`

### 3. Added direct Svg module shims
The repository now includes:
- `BobUI6Svg`
- `BobUISvg`

These forward to:
- `Qt6Svg`

### 4. Refactored smoke tests to use manifest loop
The `bobui_package_forwarding_smoke.cmake` and `bobui_export_publication_configure_smoke.cmake` scripts were previously using hardcoded lists of modules. They have been refactored to iterate over `BOBUI_SUPPORTED_COMPONENTS` from `cmake/BobUISupportedComponents.cmake`.

This makes adding new modules much safer and less prone to "forgot to update test" errors.

## Why this matters
Linguist, ShaderTools, and Svg are common dependencies for modern UI projects. Including them in the BobUI namespace allows developers to start adopting BobUI naming in their project build files today.

The refactor to use the manifest loop is a critical "quality of life" improvement for the migration itself, as it centralizes the inventory and ensures all tests stay in sync.

## Verified
```bash
cmake -P cmake/tests/bobui_full_compatibility_validation.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- the module-package coverage is still selective,
- stronger native qtbase configure/install validation remains environment-gated,
- source-identifier renaming is still intentionally deferred.

## Recommended next step
Continue extending module-package shims to other public modules found in the `D:/Qt` reference, or begin investigating "Bucket C" (Public Header Aliases) for the already-shimmed modules.
