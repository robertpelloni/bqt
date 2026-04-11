# BobUI CMake Module Shims for Gui and Network

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the initial Core/Widgets module work.

Affected files:
- `cmake/BobUI6GuiConfig.cmake`
- `cmake/BobUIGuiConfig.cmake`
- `cmake/BobUI6NetworkConfig.cmake`
- `cmake/BobUINetworkConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Extend the additive BobUI module-package surface into more high-value public modules while preserving the compatibility-first migration model.

## What changed

### 1. Added direct Gui module shims
The repository now includes:
- `BobUI6Gui`
- `BobUIGui`

These forward to:
- `Qt6Gui`

and expose BobUI-compatible targets:
- `BobUI6::Gui`
- `BobUI::Gui`

### 2. Added direct Network module shims
The repository now includes:
- `BobUI6Network`
- `BobUINetwork`

These forward to:
- `Qt6Network`

and expose BobUI-compatible targets:
- `BobUI6::Network`
- `BobUI::Network`

### 3. Extended smoke tests
The forwarding smoke test now validates direct package lookup for:
- `BobUI6Gui`
- `BobUIGui`
- `BobUI6Network`
- `BobUINetwork`

The publication configure smoke test now validates that the BobUI publication helper can emit:
- Gui module-package compat files
- Network module-package compat files

in both build-style and install-style package directories.

## Why this matters
This continues the right migration pattern:
- start with the umbrella package,
- add a few high-value public modules,
- validate each slice,
- avoid broad identifier churn.

Gui and Network are useful next steps because they are common public package surfaces and extend the BobUI naming story beyond the first Core/Widgets slice.

## Verified
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- the module-package coverage is still selective, not comprehensive,
- stronger qtbase-native configure/install validation remains a separate milestone,
- source-identifier renaming is still intentionally deferred.

## Recommended next step
Continue module-package expansion only for high-value public modules that can be verified cheaply, or pivot to stronger native qtbase validation if that becomes practical.
