# BobUI CMake Module Shims Tests for Core and Widgets

## Date
2026-04-05

## Scope
This note documents validation for the first direct BobUI module-package shims.

Affected files:
- `cmake/BobUI6CoreConfig.cmake`
- `cmake/BobUICoreConfig.cmake`
- `cmake/BobUI6WidgetsConfig.cmake`
- `cmake/BobUIWidgetsConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- top-level `BobUI6` / `BobUI` package forwarding still works,
- direct `BobUI6Core` / `BobUICore` package lookup works,
- direct `BobUI6Widgets` / `BobUIWidgets` package lookup works,
- expected BobUI compatibility targets exist for Core and Widgets.

### 2. Publication configure smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the publication helper can emit BobUI top-level package files,
- the publication helper can also emit Core/Widgets module-package compat files,
- expected module compat config/helper/version files exist in build-style and install-style directories.

### 3. Regression checks
Commands:
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- helper mapping behavior remains stable,
- the verified Go baseline remains green.

## Result
All validation passed.

## Key finding
The BobUI CMake compatibility path now covers:
- top-level package forwarding,
- top-level package publication,
- a first direct module-package slice for Core and Widgets.

That is a meaningful step toward side-by-side C++ and Go buildability without reckless broad renaming.
